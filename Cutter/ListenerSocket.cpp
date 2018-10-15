// ListenerSocket.cpp : implementation file
//

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
