#pragma once

class Bounds;

// CutBoundsDisplay dialog

class CutBoundsDisplay : public CDialogEx
{
	DECLARE_DYNAMIC(CutBoundsDisplay)

public:
	CutBoundsDisplay(const Bounds* bounds, CWnd* pParent = nullptr);   // standard constructor

	virtual ~CutBoundsDisplay();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOUNDS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double rootMinX;
	double rootMinY;
	double rootMinZ;
	double rootMaxX;
	double rootMaxY;
	double rootMaxZ;
	double tipMinX;
	double tipMinY;
	double tipMinZ;
	double tipMaxX;
	double tipMaxY;
	double tipMaxZ;
	double firstRootX;
	double firstRootY;
	double firstTipX;
	double firstTipY;
	double firstSpan;
	double lastRootX;
	double lastRootY;
	double lastTipX;
	double lastTipY;
	double lastSpan;
};
