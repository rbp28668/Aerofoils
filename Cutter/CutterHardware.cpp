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

#include "stdafx.h"
#include <assert.h>
#include "CutterHardware.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int CutterHardware::processResponse()
{
	assert(this);

#ifdef OUTPUT_TO_FILE
	return (int)StatusT::VALID;
#endif


	serialLink.read(6);   // 4 status characters + CRLF
	while (!serialLink.readComplete());
	std::string response;
	serialLink.getReadData(response);
	
	// This is the code on the device that sends the response
	//outputBuffer[0] = ok ? 'Y' : 'N';
	//outputBuffer[1] = commandQueue.isFull() ? 'F' : '-';
	//outputBuffer[2] = commandQueue.isEmpty() ? 'E' : '-';
	//outputBuffer[3] = hexDigits[hardware.limitSwitches() & 0x0F];
	int combinedResults = 0;
	if (response.size() >= 4) {
		std::ostringstream os;
		os << "Response: " << response[0] << response[1] << response[2] << response[3] << std::endl;
		::OutputDebugString(os.str().c_str());
	
		if (response[0] == 'Y') {
			combinedResults |= (int) StatusT::VALID;
		}
		if (response[1] == 'F') {
			combinedResults |= (int) StatusT::FULL;
			queueFull = true;
			std::cout << "Queue full" << std::endl;
		}
		else {
			queueFull = false;
		}
		if (response[2] == 'E') {
			combinedResults |= (int) StatusT::EMPTY;
			queueEmpty = true;
		}
		else {
			queueEmpty = false;
		}
		int switches = 0;
		char ch = response[3];
		if (ch >= '0' && ch <= '9') {
			switches = (ch - '0');
		}
		else if (ch >= 'A' && ch <= 'F') {
			switches = 10 + (ch - 'A');
		}
		combinedResults |= switches;
	}
	else {
		combinedResults |= (int)StatusT::COMMS;
	}
	return combinedResults;
}


int CutterHardware::send(const std::string& msg)
{
	assert(this);

#ifdef OUTPUT_TO_FILE
	ofs << msg << std::endl;
#else
	if (!isConnected()) {
		return 0; // without valid bit set
	}

	serialLink.send(msg);
	::Sleep(10); // not needed;
#endif


	/*std::ostringstream os;
	os << "Command: " << msg << std::endl;
	::OutputDebugString(os.str().c_str());*/

	return processResponse();
}

int CutterHardware::queueCommand(const std::string& msg)
{
#ifndef OUTPUT_TO_FILE
	while(queueFull) {
	  ::Sleep(10); // just back off a bit
	  ping(); // will reset queueEmpty/queueFull if no longer full
	}
#endif
	return send(msg);
}

CutterHardware::CutterHardware()
	: queueFull(false)
	, queueEmpty(true)
{
}

CutterHardware::CutterHardware(const char * pszPort) 
	: serialLink(pszPort)
	, queueFull(false)
	, queueEmpty(true)

{
}

int CutterHardware::connect(const char * pszPort)
{
	assert(this);
	assert(pszPort);

#ifdef OUTPUT_TO_FILE
	ofs.open("cutter.out");
#endif

	return (int)serialLink.connect(pszPort);
}

void CutterHardware::disconnect()
{
	assert(this);

#ifdef OUTPUT_TO_FILE
	ofs.close();
#endif

	if (serialLink.isConnected()) {
		serialLink.disconnect();
	}
}

boolean CutterHardware::isConnected()
{
	assert(this);
	return serialLink.isConnected();
}

int CutterHardware::ping()
{
	assert(this);
	if (!serialLink.isConnected()) throw NotConnectedException();
	std::string msg("P");
	return send(msg); // valid at any time
}

int CutterHardware::disable()
{
	assert(this);
	if (!serialLink.isConnected()) throw NotConnectedException();
	std::string msg("D");
	return queueCommand(msg);
}

int CutterHardware::enable()
{
	assert(this);
	if (!serialLink.isConnected()) throw NotConnectedException();
	std::string msg("E");
	return queueCommand(msg);
}

int CutterHardware::abort()
{
	assert(this);
	if (!serialLink.isConnected()) throw NotConnectedException();
	std::string msg("Z");
	return queueCommand(msg);
}

int CutterHardware::abortNow()
{
	assert(this);
	if (!serialLink.isConnected()) throw NotConnectedException();
	std::string msg("A");
	return send(msg); // valid at any time
}

int CutterHardware::home()
{
	assert(this);
	if (!serialLink.isConnected()) throw NotConnectedException();
	std::string msg("H");
	return queueCommand(msg);
}

int CutterHardware::line(AxisT steps, AxisT lx, AxisT ly, AxisT rx, AxisT ry)
{
	assert(this);

	assert(abs(lx) <= steps);
	assert(abs(ly) <= steps);
	assert(abs(rx) <= steps);
	assert(abs(ry) <= steps);

	if (!serialLink.isConnected()) throw NotConnectedException();

	std::stringstream out;
	out << "L";
	out <<std::hex << std::setfill('0') << std::right 
		<< std::setw(8) << steps 
		<< std::setw(8) << lx 
		<< std::setw(8) << ly 
		<< std::setw(8) << rx 
		<< std::setw(8) << ry;
	return queueCommand(out.str());
}

int CutterHardware::step(int direction, int pulse)
{
	assert(this);
	assert(0 <= direction && direction <= 0x0F);
	assert(0 <= pulse && pulse <= 0x0F);

	if (!serialLink.isConnected()) throw NotConnectedException();

	int data = ((direction & 0x0F) << 4) | (pulse & 0x0F);

	std::stringstream out;
	out << "S"
		<< std::hex << std::setfill('0') << std::right << std::setw(2)
		<< data;
	return queueCommand(out.str());
}

int CutterHardware::wireOn()
{
	assert(this);
	if (!serialLink.isConnected()) throw NotConnectedException();
	std::string msg("W");
	return queueCommand(msg);
}

int CutterHardware::wireOff()
{
	assert(this);
	if (!serialLink.isConnected()) throw NotConnectedException();
	std::string msg("X");
	return queueCommand(msg);
}
