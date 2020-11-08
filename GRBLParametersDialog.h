#pragma once


// GRBLParametersDialog dialog

class GRBLParametersDialog : public CDialogEx
{
	DECLARE_DYNAMIC(GRBLParametersDialog)

public:
	GRBLParametersDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~GRBLParametersDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRBL_PARAMETERS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double feedRate;
	double laserPower;
	BOOL dynamicLaserPower;
};
