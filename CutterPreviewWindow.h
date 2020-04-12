#pragma once

#include "Kernel/Cut.h"
#include "Kernel/Position.h"
#include "Kernel/CutterSimulation.h"

// CutterPreviewWindow

class CutterSimulationOutputDevice;

// Note, needs to be a CFrameWnd to support toolbars etc.
class CutterPreviewWindow : public CFrameWnd, public PointPlotter
{
	DECLARE_DYNAMIC(CutterPreviewWindow)

public:
	CutterPreviewWindow(CWnd* parentWindow,const Cut* cut, const CutterGeometry* geometry);
	virtual ~CutterPreviewWindow();
	void runCut();

protected:

	Cut cut;
	CutterSimulation* simulation;
	CutterSimulationOutputDevice* device;

	const int DEFAULT_STEPS_PER_SEC = 50;

	CString windowClass;
	LARGE_INTEGER plotTime; // Track speed of plot
	LARGE_INTEGER stepTime; // allow this time between steps
	CBitmap* bitmap;
	int nWidth; // of bitmap and window client size
	int nHeight; // ditto

	boolean isCutting;  // true if a cut is in progress (may be paused)
	boolean isPaused;   // true if a cut is paused
	boolean terminate;  // set true to terminate a cut
	boolean bitmapDirty; // is set true if the bitmap has been written into by a plot.
	LARGE_INTEGER counterFrequency;  // use for timing to control speed of cut.

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

	const double border = 10; //mm
	CToolBar toolbar;

	void createBitmap(CDC* pdc);

	DECLARE_MESSAGE_MAP()
	// override PointPlotter::plot
	virtual bool plot(Position<long long> axes);
	virtual void setResolution(double xStepsPerMM, double yStepsPerMM);
	virtual void setCutterBounds(double xTravel, double yTravel);

	void processMessages();
public:
	virtual void PostNcDestroy();

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnControlcutterStart();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnControlcutterPause();
	afx_msg void OnControlcutterStop();
	afx_msg void OnUpdateControlcutterPause(CCmdUI* pCmdUI);
	afx_msg void OnUpdateControlcutterStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateControlcutterStop(CCmdUI* pCmdUI);
	afx_msg void OnSpeedFull();
	afx_msg void OnSpeedX1();
	afx_msg void OnSpeedX10();
	afx_msg void OnSpeedX2();
	afx_msg void OnSpeedX5();
};


