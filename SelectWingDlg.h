#if !defined(AFX_SELECTWINGDLG_H__1500ED77_BE66_11D6_9546_00047660C89A__INCLUDED_)
#define AFX_SELECTWINGDLG_H__1500ED77_BE66_11D6_9546_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectWingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectWingDlg dialog
class CPlot;
class CWing;

class CSelectWingDlg : public CDialog
{
// Construction
public:
	CSelectWingDlg(CPlot* pp, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectWingDlg)
	enum { IDD = IDD_SELECT_WING };
	CButton	m_cmdOK;
	CListBox	m_lstWings;
	//}}AFX_DATA

	CWing* selected;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectWingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectWingDlg)
	afx_msg void OnSelchangeLstWings();
	afx_msg void OnSelcancelLstWings();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkLstWings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CPlot* plot;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTWINGDLG_H__1500ED77_BE66_11D6_9546_00047660C89A__INCLUDED_)
