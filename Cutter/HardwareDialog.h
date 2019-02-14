#pragma once

class CutterHardware;

// CHardwareDialog dialog

class CHardwareDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHardwareDialog)

	CutterHardware* pCutter;

	long lxSteps;
	long lySteps;
	long rxSteps;
	long rySteps;
	long totalSteps;

	CButton lxStepDirection;
	CButton lyStepDirection;
	CButton rxStepDirection;
	CButton ryStepDirection;
	CButton lxStepPulse;
	CButton lyStepPulse;
	CButton rxStepPulse;
	CButton ryStepPulse;
	CStatic	statusText;

	void showStatus(int stat);

public:
	CHardwareDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHardwareDialog();

	void setHardware(CutterHardware* pHardware);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HARDWARE };
#endif

	afx_msg void OnBnClickedBtnHome();
	afx_msg void OnBnClickedBtnAbort();
	afx_msg void OnBnClickedBtnStatus();
	afx_msg void OnBnClickedBtnLine();
	afx_msg void OnBnClickedBtnStep();
	afx_msg void OnBnClickedBtnWireon();
	afx_msg void OnBnClickedBtnWireoff();
	afx_msg void OnBnClickedBtnEnable();
	afx_msg void OnBnClickedBtnDisable();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
