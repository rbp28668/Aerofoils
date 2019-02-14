/* Cutter
Copyright(C) 2019 R Bruce Porteous

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "StdAfx.h"
#include <iostream>
#include "SerialLink.h"


SerialLink::SerialLink()
	: dwToWrite(0),
	hComm(INVALID_HANDLE_VALUE),
	hEventRead(INVALID_HANDLE_VALUE),
	hEventWrite(INVALID_HANDLE_VALUE),
	connected(false)
{
}

SerialLink::SerialLink(const char* pszPort)
	: dwToWrite(0),
	hComm(INVALID_HANDLE_VALUE),
	hEventRead(INVALID_HANDLE_VALUE),
	hEventWrite(INVALID_HANDLE_VALUE),
	connected(false)
{
	assert(pszPort);
	connect(pszPort);
}


SerialLink::~SerialLink(void)
{
	disconnect();
}

void SerialLink::connect(const char * pszPort)
{
	assert(this);
	assert(pszPort);

	hComm = ::CreateFileA(pszPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);
	if (hComm == INVALID_HANDLE_VALUE) {
		std::cerr << "Unable to open serial port " << pszPort << std::endl;
		exit(3);
	}


	DCB dcb;
	::memset(&dcb, 0, sizeof(dcb));

	dcb.DCBlength = sizeof(dcb);
	if (!BuildCommDCBA("4800,n,8,1", &dcb)) {
		std::cerr << "Unable to build device control block " << std::endl;
		exit(3);
	}
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_ENABLE; // just enable it
	dcb.fDsrSensitivity = 0;
	dcb.fOutX = 0;
	dcb.fInX = 0;
	dcb.fRtsControl = RTS_CONTROL_ENABLE; // just enable it.
	dcb.fAbortOnError = 0;


	if (!SetCommState(hComm, &dcb)) {
		std::cerr << "Unable to set comms state " << ::GetLastError() << std::endl;
		exit(4);
	}

	// Set timeouts - see https://docs.microsoft.com/en-gb/windows/desktop/api/winbase/nf-winbase-getcommtimeouts
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 100;       // 100mS break terminates message
	timeouts.ReadTotalTimeoutConstant = 100;   // allow total of 100mS
	timeouts.ReadTotalTimeoutMultiplier = 5;  // + 5mS per character
	timeouts.WriteTotalTimeoutMultiplier = 10;  // 10mS per char
	timeouts.WriteTotalTimeoutConstant = 100;   // + 100mS per message

	if (!::SetCommTimeouts(hComm, &timeouts)) {
		std::cerr << "Unable to set comms timeouts " << ::GetLastError() << std::endl;
		exit(5);

	}
	// Create event for use by overlapped IO
	hEventRead = ::CreateEvent(NULL, TRUE, TRUE, NULL); // Manual reset, initially signalled.
	hEventWrite = ::CreateEvent(NULL, TRUE, TRUE, NULL); // Manual reset, initially signalled.

	connected = true;
}

void SerialLink::disconnect()
{
	assert(this);
	if (hEventRead != INVALID_HANDLE_VALUE) {
		::CloseHandle(hEventRead);
	}
	if (hEventWrite != INVALID_HANDLE_VALUE) {
		::CloseHandle(hEventWrite);
	}
	if (hComm != INVALID_HANDLE_VALUE) {
		::CloseHandle(hComm);
	}
	connected = false;
}

boolean SerialLink::isConnected()
{
	assert(this);
	return connected;
}

bool SerialLink::canSend() {
	assert(this);
	assert(connected);

	DWORD dwWritten;
	DWORD dw = ::WaitForSingleObject(hEventWrite,0); // sample but don't wait.
	std::cout << "WFSO returned " << dw << std::endl;
	switch(dw) {
	case WAIT_OBJECT_0:	// event is signalled so write should have completed.

		// If bytes have been written then check completed.
		if(dwToWrite > 0) {
			if (!::GetOverlappedResult(hComm, &osWrite, &dwWritten, FALSE)) {
				std::cerr << "GetOverlappedResult failed" << std::endl;
			} else { // success
			   if (dwWritten != dwToWrite) {
				   std::cerr << "Only wrote " << dwWritten << " of " << dwToWrite << " bytes" << std::endl;
			   }
			}
		}

		::ResetEvent(osWrite.hEvent);
		return true;

	case WAIT_TIMEOUT:  // not yet signalled, still pending.
		std::cout << "Waiting..." << std::endl;
		return false;

	default:
		std::cerr << "Unknown status " << dw << " from WaitForSingleObject" << std::endl;
		break;
	}
	return false;
}


void SerialLink::send(std::string& str) {
	assert(this);
	assert(connected);
   
   DWORD dwWritten;

    // reset OVERLAPPED structure for this write
   	::memset(&osWrite, 0, sizeof(osWrite));
    osWrite.hEvent = hEventWrite;

   const char * lpBuf = str.c_str();
   dwToWrite = (DWORD)str.length();

    // Issue write
   if (!WriteFile(hComm, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
      if (GetLastError() != ERROR_IO_PENDING) { 
         std::cerr << "Write file failed" << std::endl;
      }
   }
}

void SerialLink::read(size_t bytesToRead) {
	assert(this);
	assert(connected);

	if (bytesToRead > sizeof(readBuffer)) {
		bytesToRead = sizeof(readBuffer);
	}
	// reset OVERLAPPED structure for this read
	::memset(&osRead, 0, sizeof(osRead));
	osRead.hEvent = hEventRead;

	if (!ReadFile(hComm, &readBuffer, (DWORD)bytesToRead, NULL, &osRead)) {
		if (GetLastError() != ERROR_IO_PENDING) {
			std::cerr << "Read file failed" << std::endl;
		}
	}
}

bool SerialLink::readComplete() {
	assert(this);
	assert(connected);

	DWORD dwRead = 0;
	DWORD dw = ::WaitForSingleObject(hEventRead, 0); // sample but don't wait.
	std::cout << "WFSO returned " << dw << std::endl;
	switch (dw) {
	case WAIT_OBJECT_0:	// event is signalled so read should have completed.
		if (!::GetOverlappedResult(hComm, &osRead, &dwRead, FALSE)) {
			std::cerr << "GetOverlappedResult failed" << std::endl;
		} else {
			bytesRead = dwRead;
		}

		::ResetEvent(osRead.hEvent);
		return true;

	case WAIT_TIMEOUT:  // not yet signalled, still pending.
		std::cout << "Waiting..." << std::endl;
		return false;

	default:
		std::cerr << "Unknown status " << dw << " from WaitForSingleObject" << std::endl;
		break;
	}
	return false;

}

void SerialLink::getReadData(std::string & str)
{
	assert(this);
	assert(connected);

	str.erase();
	str.append(readBuffer, bytesRead);
}



