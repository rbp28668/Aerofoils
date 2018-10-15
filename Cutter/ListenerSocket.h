#if !defined(AFX_LISTENERSOCKET_H__D76CBE14_CA15_11D6_955D_000000000000__INCLUDED_)
#define AFX_LISTENERSOCKET_H__D76CBE14_CA15_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenerSocket.h : header file
//

class CSocketEventHandler;

/////////////////////////////////////////////////////////////////////////////
// CListenerSocket command target

class CListenerSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CListenerSocket(CSocketEventHandler* ph);
	virtual ~CListenerSocket();

	void setLink(CAsyncSocket* pl);
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CAsyncSocket* link;	// socket to use for Accept.
	CSocketEventHandler* handler;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENERSOCKET_H__D76CBE14_CA15_11D6_955D_000000000000__INCLUDED_)
