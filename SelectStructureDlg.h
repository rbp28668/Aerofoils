#if !defined(AFX_SELECTStructureDLG_H__1500ED77_BE66_11D6_9546_00047660C89A__INCLUDED_)
#define AFX_SELECTStructureDLG_H__1500ED77_BE66_11D6_9546_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectStructureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectStructureDlg dialog
class CPlot;
class CStructure;

class CSelectStructureDlg : public CDialog
{
// Construction
public:
	CSelectStructureDlg(CPlot* pp, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectStructureDlg)
	enum { IDD = IDD_SELECT_STRUCTURE };
	CButton	m_cmdOK;
	CListBox	m_lstStructures;
	//}}AFX_DATA

	CStructure* selected;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectStructureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectStructureDlg)
	afx_msg void OnSelchangeLstStructures();
	afx_msg void OnSelcancelLstStructures();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkLstStructures();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CPlot* plot;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTStructureDLG_H__1500ED77_BE66_11D6_9546_00047660C89A__INCLUDED_)
