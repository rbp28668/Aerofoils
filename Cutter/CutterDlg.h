// CutterDlg.h : header file
//

#if !defined(AFX_CUTTERDLG_H__D76CBE0C_CA15_11D6_955D_000000000000__INCLUDED_)
#define AFX_CUTTERDLG_H__D76CBE0C_CA15_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCutterDlg dialog
#include <afxsock.h>
#include <string>

#include "SocketEventHandler.h"
#include "LinkSocket.h"
#include "ListenerSocket.h"

class CCutterDlg : public CDialog, CSocketEventHandler
{
// Construction
public:
	CCutterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCutterDlg)
	enum { IDD = IDD_CUTTER_DIALOG };
	CListBox	m_lstMessages;
	CStatic	m_lblStatus;
	UINT	m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCutterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCutterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCmdListen();
	afx_msg void OnCmdAccept();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// CSocketEventHandler overrides
	virtual void connected();
	virtual void error(int nErrorCode);
	virtual void received(const char* data, int nBytes);
	virtual void closed();


	CListenerSocket listener;
	CLinkSocket link;

	std::string data;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUTTERDLG_H__D76CBE0C_CA15_11D6_955D_000000000000__INCLUDED_)
