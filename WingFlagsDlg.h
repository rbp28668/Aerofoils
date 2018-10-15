#if !defined(AFX_WINGFLAGSDLG_H__44A146B3_BBF2_11D6_9543_000000000000__INCLUDED_)
#define AFX_WINGFLAGSDLG_H__44A146B3_BBF2_11D6_9543_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WingFlagsDlg.h : header file
//

class CPlotFlags;

/////////////////////////////////////////////////////////////////////////////
// CWingFlagsDlg dialog

class CWingFlagsDlg : public CDialog
{
// Construction
public:
	CWingFlagsDlg(CPlotFlags* pf, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWingFlagsDlg)
	enum { IDD = IDD_WING_FLAGS };
	BOOL	m_blPlotChord;
	BOOL	m_blPlotLabels;
	BOOL	m_blPlotMarkers;
	BOOL	m_blPlotSection;
	BOOL	m_blPlotSkin;
	BOOL	m_blPlotSpars;
	BOOL	m_blPlotTE;
	BOOL	m_blPlotLE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWingFlagsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWingFlagsDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CPlotFlags* flags;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINGFLAGSDLG_H__44A146B3_BBF2_11D6_9543_000000000000__INCLUDED_)
