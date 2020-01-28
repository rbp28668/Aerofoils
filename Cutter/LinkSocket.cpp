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
#include "LinkSocket.h"
#include "SocketEventHandler.h"
#include "../Kernel/Cutter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkSocket

CLinkSocket::CLinkSocket(CSocketEventHandler* ph)
: handler(ph)
{
	assert(ph);
}

CLinkSocket::~CLinkSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CLinkSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CLinkSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CLinkSocket member functions

void CLinkSocket::OnConnect(int nErrorCode) 
{
	if (nErrorCode != 0) {
		handler->error(nErrorCode);
	} else {
		handler->connected();
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

void CLinkSocket::OnReceive(int nErrorCode) 
{
	char bytes[4096];
	int bytesFree = 4096;
	int totalBytes = 0;
	bool closed = false;

	while(nErrorCode == 0 && bytesFree > 256 && !closed)
	{
		int bytesRead = Receive(bytes + totalBytes, bytesFree);
		switch(bytesRead)
		{
		case 0: // remote side has shut down gracefully.
			Close();
			closed = true;
			break;

		case SOCKET_ERROR:
			nErrorCode = GetLastError();
			break;

		default:
			totalBytes += bytesRead;
			bytesFree -= bytesRead;
		}
	}

	if(totalBytes > 0)
		handler->received(bytes,totalBytes);

	if(nErrorCode != WSAEWOULDBLOCK && nErrorCode != 0)
	{
		handler->error(nErrorCode);
	}

	if (closed) {
		handler->closed();
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CLinkSocket::OnClose(int nErrorCode) 
{
	handler->closed();
	CAsyncSocket::OnClose(nErrorCode);
}
