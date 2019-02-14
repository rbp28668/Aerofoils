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

#pragma once
#include <string>

class SerialLink 
{
	boolean connected;   // true if connected
	HANDLE hComm;		// handle to port
	HANDLE hEventRead;		// handle to event to be signalled on completion
	HANDLE hEventWrite;		// handle to event to be signalled on completion
	DWORD dwToWrite;	// bytes to write
	DWORD bytesRead;
	OVERLAPPED osRead; // track the read
	OVERLAPPED osWrite; // track the write
	char readBuffer[256];

public:
	SerialLink();
	SerialLink(const char* pszPort);
	virtual ~SerialLink(void);

	void connect(const char* pszPort);
	void disconnect();
	boolean isConnected();

	virtual bool canSend();
	virtual void send(std::string& str);
	void read(size_t bytesToRead);
	bool readComplete();
	void getReadData(std::string& str);
};

