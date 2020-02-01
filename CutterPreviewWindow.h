#pragma once
#include "Kernel\Position.h"
#include "Kernel\CutterSimulation.h"

// CutterPreviewWindow

class CutterPreviewWindow : public CWnd, public PointPlotter
{
	DECLARE_DYNAMIC(CutterPreviewWindow)

public:
	CutterPreviewWindow(CWnd* parentWindow);
	virtual ~CutterPreviewWindow();

protected:

	CString windowClass;
	DWORD plotTime;
	CBitmap* bitmap;
	int nWidth; // of bitmap
	int nHeight; 

	boolean terminate;

	double xStepsPerMM;
	double yStepsPerMM;
	double xTravel;
	double yTravel;

	// These variables are required for horizontal scrolling. 
	int xMinScroll;       // minimum horizontal scroll value 
	int xCurrentScroll;   // current horizontal scroll value 
	int xMaxScroll;       // maximum horizontal scroll value 

								 // These variables are required for vertical scrolling. 
	int yMinScroll;       // minimum vertical scroll value 
	int yCurrentScroll;   // current vertical scroll value 
	int yMaxScroll;       // maximum vertical scroll value 

	DECLARE_MESSAGE_MAP()
	// override PointPlotter::plot
	virtual bool plot(Position<long long> axes);
	virtual void setResolution(double xStepsPerMM, double yStepsPerMM);
	virtual void setCutterBounds(double xTravel, double yTravel);

	void processMessages();
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
};


