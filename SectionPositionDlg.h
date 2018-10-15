#if !defined(AFX_SECTIONPOSITIONDLG_H__A85DB443_BB1C_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_SECTIONPOSITIONDLG_H__A85DB443_BB1C_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SectionPositionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSectionPositionDlg dialog

class CSectionPositionDlg : public CDialog
{
// Construction
public:
	CSectionPositionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSectionPositionDlg)
	enum { IDD = IDD_SECTION_POS };
	float	m_position;
	//}}AFX_DATA

	float m_maxPosition; // set to wing span

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionPositionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSectionPositionDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTIONPOSITIONDLG_H__A85DB443_BB1C_11D6_AF75_002018BA082E__INCLUDED_)
