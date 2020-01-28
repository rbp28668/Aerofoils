/* Aerofoil
Aerofoil plotting and CNC cutter driver
Copyright(C) 1995-2019 R Bruce Porteous

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

#include <assert.h>
#include <exception>
#include <sstream>
#include <map>
#include "CNCSocket.h"


class CSocketException : public std::exception
{
public:
	CSocketException(const char* psz)
		:info(psz)
	{
	}

	virtual const char *what() const throw() { return info; }

private:
	const char* info;
};

CNCSocket::ERRORS CNCSocket::errors;
bool CNCSocket::socketsInitialised = false;

CNCSocket::CNCSocket()
	:_canSend(false)
	, connected(false)
{

	if (!socketsInitialised)
	{
		AfxSocketInit();
		socketsInitialised = true;
		setupErrorMap();
	}
}

CNCSocket::CNCSocket(const char* address, unsigned int port) : CNCSocket() {
	connect(address, port);
}

CNCSocket::~CNCSocket()
{
	if (isConnected()) {
		Close();
	}
}

void CNCSocket::connect(const char* address, unsigned int port) {
	DWORD noblock = 0;

	int iErr = 0;
	if (Create() == 0
		|| AsyncSelect() == 0
		//||  link.IOCtl(FIONBIO, &noblock)==0
		|| Connect(address, port) == 0
		)
	{
		int iErr = GetLastError();
		if (iErr == WSAEWOULDBLOCK)
		{
			waitForConnection();
		}
		else
		{
			const char* err = translate(iErr);
			throw CSocketException(err);
		}
	}
}


void CNCSocket::OnAccept(int nErrorCode) { assert(this); }

void CNCSocket::OnClose(int nErrorCode)
{
	assert(this);
	connected = false;
}

void CNCSocket::OnConnect(int nErrorCode)
{
	assert(this);
	connected = true;
}

void CNCSocket::OnOutOfBandData(int nErrorCode) { assert(this); }
void CNCSocket::OnReceive(int nErrorCode) { assert(this); }

void CNCSocket::OnSend(int nErrorCode)
{
	assert(this);
	_canSend = true;
}

int CNCSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	assert(this);
	assert(lpBuf);
	assert(_canSend);
	_canSend = false;
	int iRet = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
	if (iRet == SOCKET_ERROR || iRet == nBufLen)
		_canSend = true;
	return iRet;
}

void CNCSocket::waitToSend()
{
	waitUntil(&_canSend);
}

void CNCSocket::waitForConnection()
{
	waitUntil(&connected);
}

void CNCSocket::waitUntil(volatile bool* flag)
{
	assert(this);
	assert(flag);
	CWinApp* app = AfxGetApp();
	long count = 0;
	while (!*flag)
	{
		MSG msg;
		if (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			::OutputDebugString("pump\n");
			app->PumpMessage();
		}
		std::ostringstream os;
		os << "wait " << count++ << std::endl;
		::OutputDebugString(os.str().c_str());
		Sleep(0);
	}
}

void CNCSocket::send(const std::string& msg)
{
	assert(this);
	size_t toSend = msg.length();
	const char* bytes = msg.c_str();
	size_t sent = 0;

	while (toSend > 0)
	{

		waitToSend();

		int nBytes = Send(bytes + (int)sent, (int)toSend);
		::Sleep(0);

		if (nBytes == SOCKET_ERROR)
		{
			int iErr = GetLastError();
			if (iErr == WSAEWOULDBLOCK)
				waitToSend();
			else
			{
				const char* err = translate(iErr);
				throw CSocketException(err);
			}
		}
		else
		{
			toSend -= nBytes;
			sent += nBytes;
		}
	}
}

void CNCSocket::setupErrorMap()
{
	errors.insert(ERRORS::value_type(WSANOTINITIALISED, "A successful AfxSocketInit must occur before using this API."));
	errors.insert(ERRORS::value_type(WSAENETDOWN, "The Windows Sockets implementation detected that the network subsystem failed."));
	errors.insert(ERRORS::value_type(WSAEACCES, "The requested address is a broadcast address, but the appropriate flag was not set."));
	errors.insert(ERRORS::value_type(WSAEINPROGRESS, "A blocking Windows Sockets operation is in progress."));
	errors.insert(ERRORS::value_type(WSAEFAULT, "The lpBuf argument is not in a valid part of the user address space."));
	errors.insert(ERRORS::value_type(WSAENETRESET, "The connection must be reset because the Windows Sockets implementation dropped it."));
	errors.insert(ERRORS::value_type(WSAENOBUFS, "The Windows Sockets implementation reports a buffer deadlock."));
	errors.insert(ERRORS::value_type(WSAENOTCONN, "The socket is not connected."));
	errors.insert(ERRORS::value_type(WSAENOTSOCK, "The descriptor is not a socket."));
	errors.insert(ERRORS::value_type(WSAEOPNOTSUPP, "MSG_OOB was specified, but the socket is not of type SOCK_STREAM."));
	errors.insert(ERRORS::value_type(WSAESHUTDOWN, "The socket has been shut down; it is not possible to call Send on a socket after ShutDown has been invoked with nHow set to 1 or 2."));
	errors.insert(ERRORS::value_type(WSAEWOULDBLOCK, "The socket is marked as nonblocking and the requested operation would block."));
	errors.insert(ERRORS::value_type(WSAEMSGSIZE, "The socket is of type SOCK_DGRAM, and the datagram is larger than the maximum supported by the Windows Sockets implementation."));
	errors.insert(ERRORS::value_type(WSAEINVAL, "The socket has not been bound with Bind."));
	errors.insert(ERRORS::value_type(WSAECONNABORTED, "The virtual circuit was aborted due to timeout or other failure."));
	errors.insert(ERRORS::value_type(WSAECONNRESET, "The virtual circuit was reset by the remote side. "));
	errors.insert(ERRORS::value_type(WSAEADDRINUSE, "The specified address is already in use."));
	errors.insert(ERRORS::value_type(WSAEADDRNOTAVAIL, "The specified address is not available from the local machine."));
	errors.insert(ERRORS::value_type(WSAEAFNOSUPPORT, "Addresses in the specified family cannot be used with this socket."));
	errors.insert(ERRORS::value_type(WSAECONNREFUSED, "The attempt to connect was rejected."));
	errors.insert(ERRORS::value_type(WSAEDESTADDRREQ, "A destination address is required."));
	errors.insert(ERRORS::value_type(WSAEFAULT, "The nSockAddrLen argument is incorrect."));
	errors.insert(ERRORS::value_type(WSAEINVAL, "Invalid host address."));
	errors.insert(ERRORS::value_type(WSAEISCONN, "The socket is already connected."));
	errors.insert(ERRORS::value_type(WSAEMFILE, "No more file descriptors are available."));
	errors.insert(ERRORS::value_type(WSAENETUNREACH, "The network cannot be reached from this host at this time."));
	errors.insert(ERRORS::value_type(WSAETIMEDOUT, "Attempt to connect timed out without establishing a connection."));
}

const char* CNCSocket::translate(int nErr)
{
	assert(this);
	ERRORS::iterator iter = errors.find(nErr);
	if (iter != errors.end())
		return iter->second;

	return "UNKNOWN";
}


