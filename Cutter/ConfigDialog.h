#pragma once
#include "afxwin.h"

struct CutterConfig;
class CCutterDlg;

// CConfigDialog dialog

class CConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDialog)
	CutterConfig* config;
	Cutter* pCutter;

public:
	CConfigDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigDialog();

	void initialize(CCutterDlg* pCutter);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox defaultComPort;
	CButton connectAutomatically;
	CButton listenAutomatically;
	afx_msg void OnBnClickedBtnUpdateCutter();
	afx_msg void OnBnClickedBtnSaveConfig();
	afx_msg void OnBnClickedChkConnectAuto();
	afx_msg void OnBnClickedChkListenAuto();
	afx_msg void OnCbnSelchangeCmbDefaultCom();
};
