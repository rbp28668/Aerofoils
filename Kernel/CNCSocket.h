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

#pragma once
#define NO_WARN_MBCS_MFC_DEPRECATION
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#include <afxsock.h>
#include <map>

// Class to provide a socket connection to a TCP device.

class CNCSocket : public CAsyncSocket
{
public:
	CNCSocket();
	CNCSocket(const char* address, unsigned int port);
	~CNCSocket();

	void connect(const char* address, unsigned int port);

	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	bool canSend() const { return _canSend; }
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

	bool isConnected() const { return connected; }

	void waitToSend();
	void waitForConnection();
	void send(const std::string& msg);

private:
	bool _canSend;
	bool connected;
	void waitUntil(volatile bool* flag);
	static void setupErrorMap();
	const char* translate(int nErr);

	static bool socketsInitialised;

	typedef std::map<int, char*> ERRORS;
	static ERRORS errors;

};


