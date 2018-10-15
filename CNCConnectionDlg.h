#if !defined(AFX_CNCCONNECTIONDLG_H__D76CBE05_CA15_11D6_955D_000000000000__INCLUDED_)
#define AFX_CNCCONNECTIONDLG_H__D76CBE05_CA15_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CNCConnectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCNCConnectionDlg dialog

class CCNCConnectionDlg : public CDialog
{
// Construction
public:
	CCNCConnectionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCNCConnectionDlg)
	enum { IDD = IDD_CNC_CONNECTION };
	UINT	m_port;
	CString	m_host;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCNCConnectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCNCConnectionDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CNCCONNECTIONDLG_H__D76CBE05_CA15_11D6_955D_000000000000__INCLUDED_)
