// LinkSocket.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Cutter.h"
#include "LinkSocket.h"
#include "SocketEventHandler.h"

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
	// TODO: Add your specialized code here and/or call the base class
	
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
		case 0:
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
	
	CAsyncSocket::OnReceive(nErrorCode);
}

void CLinkSocket::OnClose(int nErrorCode) 
{
	handler->closed();
	CAsyncSocket::OnClose(nErrorCode);
}
