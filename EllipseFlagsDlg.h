#if !defined(AFX_ELLIPSEFLAGSDLG_H__3CB6E434_CC06_11D6_9565_000000000000__INCLUDED_)
#define AFX_ELLIPSEFLAGSDLG_H__3CB6E434_CC06_11D6_9565_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EllipseFlagsDlg.h : header file
//

class CEllipseFlags;

/////////////////////////////////////////////////////////////////////////////
// CEllipseFlagsDlg dialog

class CEllipseFlagsDlg : public CDialog
{
// Construction
public:
	CEllipseFlagsDlg(CEllipseFlags* pef, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEllipseFlagsDlg)
	enum { IDD = IDD_ELLIPSE_FLAGS };
	BOOL	m_lower;
	BOOL	m_markers;
	BOOL	m_upper;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEllipseFlagsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEllipseFlagsDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CEllipseFlags* flags;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELLIPSEFLAGSDLG_H__3CB6E434_CC06_11D6_9565_000000000000__INCLUDED_)
