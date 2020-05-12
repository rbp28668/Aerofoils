#pragma once

#include "Kernel/GCodeOutputDevice.h"

// FeedRateDialog dialog

class FeedRateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FeedRateDialog)

	GCodeOutputDevice::GCodeConfig* pConfig;

public:
	FeedRateDialog(GCodeOutputDevice::GCodeConfig* pConfig, CWnd* pParent = nullptr);   
	virtual ~FeedRateDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FEED_RATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void updateFrom(GCodeOutputDevice::GCodeConfig* pConfig);
	DECLARE_MESSAGE_MAP()
public:
	// feed rate in mm per second
	double feedRate;
	BOOL useFeedRate;
	double moveFeedRate;
	BOOL useG1forMoves;
	BOOL useUnitsPerMin;
	CString leftHorizontalAxisCode;
	CString leftVerticalAxisCode;
	CString rightHorizontalAxisCode;
	CString rightVerticalAxisCode;
	CString wireOnCode;
	CString wireOffCode;
	CString motorsEnableCode;
	CString motorsDisableCode;
	BOOL sendWireControls;
	BOOL sendEnableControls;
	BOOL sendMirror;
	BOOL sendWorkshifts;
	afx_msg void OnBnClickedBtnDefaults();
	afx_msg void OnBnClickedBtnMach3();
	virtual void OnOK();
	BOOL useCutterGeometry;
};
