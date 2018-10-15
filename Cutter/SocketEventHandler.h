// SocketEventHandler.h: interface for the CSocketEventHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETEVENTHANDLER_H__D76CBE17_CA15_11D6_955D_000000000000__INCLUDED_)
#define AFX_SOCKETEVENTHANDLER_H__D76CBE17_CA15_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocketEventHandler  
{
public:
	CSocketEventHandler();
	virtual ~CSocketEventHandler();
	
	virtual void connected() {}
	virtual void error(int nErrorCode) {}
	virtual void received(const char* data, int nBytes) {}
	virtual void closed() {}



};

#endif // !defined(AFX_SOCKETEVENTHANDLER_H__D76CBE17_CA15_11D6_955D_000000000000__INCLUDED_)
