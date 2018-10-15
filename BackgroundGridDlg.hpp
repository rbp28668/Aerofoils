#if !defined(AFX_BACKGROUNDGRIDDLG_HPP__8CC103EF_C156_4C61_BCDA_715D42756528__INCLUDED_)
#define AFX_BACKGROUNDGRIDDLG_HPP__8CC103EF_C156_4C61_BCDA_715D42756528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackgroundGridDlg.hpp : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackgroundGridDlg dialog

class CBackgroundGridDlg : public CDialog
{
// Construction
public:
	CBackgroundGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBackgroundGridDlg)
	enum { IDD = IDD_BACKGROUND_GRID };
	CEdit	m_edtVert;
	CEdit	m_edtHoriz;
	CButton	m_radVert;
	CButton	m_radHoriz;
	BOOL	m_blPlotHoriz;
	BOOL	m_blPlotVert;
	float	m_fHorizDistance;
	float	m_fVertDistance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackgroundGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBackgroundGridDlg)
	virtual void OnOK();
	afx_msg void OnChkHoriz();
	afx_msg void OnChkVert();
	afx_msg void OnRadHoriz3();
	afx_msg void OnRadHoriz4();
	afx_msg void OnRadHorizAny();
	afx_msg void OnRadVert3();
	afx_msg void OnRadVert4();
	afx_msg void OnRadVertAny();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKGROUNDGRIDDLG_HPP__8CC103EF_C156_4C61_BCDA_715D42756528__INCLUDED_)
