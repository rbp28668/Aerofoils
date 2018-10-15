#if !defined(AFX_WINGDLG_H__1103E541_BB64_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_WINGDLG_H__1103E541_BB64_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WingDlg.h : header file
//

#include "kernel/Wing.h"

/////////////////////////////////////////////////////////////////////////////
// CWingDlg dialog

class CWingDlg : public CDialog
{
// Construction
public:
	CWingDlg(CWing* w, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWingDlg)
	enum { IDD = IDD_EDIT_WING };
	CListBox	m_lstSpars;
	CStatic	m_lblTipInfo;
	CStatic	m_lblRootInfo;
	float	m_LE;
	float	m_span;
	float	m_TE;
	float	m_fSkin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void addSparToList(CSpar* ps);

	// Generated message map functions
	//{{AFX_MSG(CWingDlg)
	afx_msg void OnCmdAddSpar();
	afx_msg void OnCmdRootEdit();
	afx_msg void OnCmdSparDelete();
	afx_msg void OnCmdSparEdit();
	afx_msg void OnCmdTipEdit();
	afx_msg void OnSelchangeLstSpars();
	afx_msg void OnDblclkLstSpars();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CWing* wing;
	CWing wingCopy;	// local copy to edit, update wing if ok.
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINGDLG_H__1103E541_BB64_11D6_AF75_002018BA082E__INCLUDED_)
