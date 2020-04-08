#pragma once

class Cutout;

// CutoutDialog dialog

class CutoutDialog : public CDialogEx
{
	Cutout* pCutout;
	DECLARE_DYNAMIC(CutoutDialog)

public:
	CutoutDialog(Cutout* pCutout, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CutoutDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_CUTOUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL top_surface;
	BOOL centre;
	float root_x;
	float root_y;
	float root_width;
	float root_height;
	float tip_x;
	float tip_y;
	float tip_width;
	float tip_height;
	afx_msg void OnBnClickedCopy();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
