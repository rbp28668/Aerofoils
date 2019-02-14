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
#include "Cutter.h"
#include "ListenerSocket.h"
#include "SocketEventHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenerSocket

CListenerSocket::CListenerSocket(CSocketEventHandler* ph)
: handler(ph)
{
	assert(ph);
}

CListenerSocket::~CListenerSocket()
{
}

void CListenerSocket::setLink(CAsyncSocket* pl) 
{
	link = pl;
	AsyncSelect(FD_ACCEPT);
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenerSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenerSocket member functions

void CListenerSocket::OnAccept(int nErrorCode) 
{
	assert(this);
	assert(link);

	if(nErrorCode == 0)
	{
		if(link)
		{
			Accept(*link);
			link->	AsyncSelect(FD_READ|FD_CONNECT|FD_CLOSE);
			handler->connected();
			link = 0;
		}
	}
	else
	{
		handler->error(nErrorCode);
	}

	CAsyncSocket::OnAccept(nErrorCode);
}
