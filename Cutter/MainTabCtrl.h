#pragma once

class CGCodeDialog;
class CHardwareDialog;
class CConfigDialog;
class CCutterDlg;

// CMainTabCtrl

class CMainTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMainTabCtrl)


	CGCodeDialog* pGCodeDialog;
	CHardwareDialog* pHardwareDialog;
	CConfigDialog* pConfigDialog;

	CDialog *tabs[4];
	int currentTabIndex;
	int tabCount;

public:
	CMainTabCtrl();
	virtual ~CMainTabCtrl();

	void init(CCutterDlg* pApp);
	void sizeChildren();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


