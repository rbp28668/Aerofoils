#if !defined(AFX_SECTIONDLG_H__1103E542_BB64_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_SECTIONDLG_H__1103E542_BB64_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SectionDlg.h : header file
//

class CTransform;

#include "kernel/Aerofoil.h"

/////////////////////////////////////////////////////////////////////////////
// CSectionDlg dialog

class CSectionDlg : public CDialog
{
// Construction
public:
	CSectionDlg(CAerofoil* pa, CTransform* pt, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSectionDlg)
	enum { IDD = IDD_EDIT_SECTION };
	CStatic	m_lblSectionName;
	float	m_chord;
	float	m_height;
	float	m_sweep;
	float	m_thicknessMod;
	float	m_washout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSectionName();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CAerofoil* aerofoil;
	CAerofoil copyAerofoil;
	CTransform* transform;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTIONDLG_H__1103E542_BB64_11D6_AF75_002018BA082E__INCLUDED_)
