#if !defined(AFX_LINKSOCKET_H__D76CBE15_CA15_11D6_955D_000000000000__INCLUDED_)
#define AFX_LINKSOCKET_H__D76CBE15_CA15_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkSocket.h : header file
//


class CSocketEventHandler;

/////////////////////////////////////////////////////////////////////////////
// CLinkSocket command target

class CLinkSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CLinkSocket(CSocketEventHandler* ph);
	virtual ~CLinkSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinkSocket)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CLinkSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CSocketEventHandler* handler;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINKSOCKET_H__D76CBE15_CA15_11D6_955D_000000000000__INCLUDED_)
