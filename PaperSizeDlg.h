#if !defined(AFX_PAPERSIZEDLG_H__1500ED76_BE66_11D6_9546_00047660C89A__INCLUDED_)
#define AFX_PAPERSIZEDLG_H__1500ED76_BE66_11D6_9546_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaperSizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaperSizeDlg dialog

class CPaperSizeDlg : public CDialog
{
// Construction
public:
	CPaperSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPaperSizeDlg)
	enum { IDD = IDD_PAGE_SIZE };
	CEdit	m_editWidth;
	CEdit	m_editHeight;
	float	m_height;
	float	m_width;
	int		m_sizeSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaperSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPaperSizeDlg)
	afx_msg void OnRadCustom();
	afx_msg void OnRadA3();
	afx_msg void OnRadA3l();
	afx_msg void OnRadA4();
	afx_msg void OnRadA4l();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAPERSIZEDLG_H__1500ED76_BE66_11D6_9546_00047660C89A__INCLUDED_)
