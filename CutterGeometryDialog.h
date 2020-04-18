#pragma once

class CutterGeometry;

// CutterGeometryDialog dialog


class CutterGeometryDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CutterGeometryDialog)

	CutterGeometry* pGeometry;

public:
	CutterGeometryDialog(CutterGeometry* geometry, CWnd* pParent = nullptr);  
	virtual ~CutterGeometryDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUTTER_GEOMETRY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double x_travel;
	double y_travel;
	double cutter_width;
	double block_lhs;
	double block_width;
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
