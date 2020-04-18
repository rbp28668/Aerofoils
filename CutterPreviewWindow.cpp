// CutterPreviewWindow.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include "Aerofoil.h"
#include "CutterPreviewWindow.h"
#include "Kernel/CutterGeometry.h"
#include "Kernel/OutputDevice.h"
#include "Kernel/CutterSimulation.h"
#include "Kernel/CutterSimulationOutputDevice.h"
#include "Kernel/ObjectSerializer.h"


// CutterPreviewWindow
// Provides a window of the display of a cut.  Knows how to plot a single point on all 4 axes (via inheriting PointPlotter).   
// To allow immediate update and still redraw properly the window draws directly to its DC but also maintains
// a bitmap.  Note also that as a side-effect plot processes windows message so that the display doesn't freeze.
// Note for scrolling see: https://docs.microsoft.com/en-us/windows/desktop/controls/scroll-a-bitmap-in-scroll-bars

IMPLEMENT_DYNAMIC(CutterPreviewWindow, CFrameWnd)

CutterPreviewWindow::CutterPreviewWindow(CWnd* parentWindow, const Cut* cut, const CutterGeometry* geometry)
	: bitmap(0)
	, nWidth(0)
	, nHeight(0)
	, xMinScroll(0)
	, xCurrentScroll(0)
	, xMaxScroll(0)
	, yMinScroll(0)
	, yCurrentScroll(0)
	, yMaxScroll(0)
	, terminate(false)
	, simulation(0)
	, device(0)
	, isCutting(false)
	, isPaused(false)
	, hasFlash(false)

{

	// Set up timing info for cut to control speed.
	plotTime.QuadPart = 0;
	QueryPerformanceFrequency(&counterFrequency); 
	stepTime.QuadPart = counterFrequency.QuadPart / DEFAULT_STEPS_PER_SEC;

	windowClass = AfxRegisterWndClass( CS_VREDRAW | CS_HREDRAW ,
		::LoadCursor(NULL, IDC_ARROW),
		0, // always draw bitmap so no need for (HBRUSH) ::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION)
	);
	
	// Choose size from parent Window.
	RECT r;
	parentWindow->GetClientRect(&r);

	Create(windowClass, "Cutter Preview Window",
		WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_THICKFRAME | WS_SYSMENU | WS_HSCROLL | WS_VSCROLL,
		CRect(r.left, r.top, r.right, r.bottom),
		parentWindow,
		MAKEINTRESOURCEA(IDR_CONTROLCUTTER)
	);
	


	// Set up simulation and geometry
	simulation = new CutterSimulation(this);
	simulation->setGeometry(*geometry);
	device = new CutterSimulationOutputDevice(simulation);

	// Now geometry is set up can decide the size of the bitmap / drawing area.
	nWidth = (int)round((xTravel + 2 * border) * xStepsPerMM) + 2; // +2 to keep a 1 pixel border
	nHeight = (int)round((yTravel + 2 * border) * yStepsPerMM) + 2;

	CDC* pdc = GetDC();
	createBitmap(pdc);
	ReleaseDC(pdc);

	// Wake up the scroll bars
	yCurrentScroll = nHeight; // scroll to bottom as we do.
	OnSize(SIZE_RESTORED, r.right, r.bottom);

	// Initialise the local cut object by searializing/deserializing. Detaches this from 
	// any sneaky edits that might happen in the background.
	std::stringstream stream;
	CObjectSerializer serializer(&stream);
	cut->serializeTo(serializer);
	this->cut.serializeFrom(serializer);

	// Simulate start command to fire off preview process.
	PostMessage(WM_COMMAND, ID_CONTROLCUTTER_START, 0);
}

CutterPreviewWindow::~CutterPreviewWindow()
{
	delete bitmap;
	delete device;
	delete simulation;

}

void CutterPreviewWindow::runCut() {
	
	if (bitmapDirty) {
		delete bitmap;
		CDC* pdc = GetDC();
		createBitmap(pdc);
		ReleaseDC(pdc);
		// Force repaint of window now bitmap reset
		Invalidate();
		toolbar.Invalidate();
		UpdateWindow();
	}

	terminate = false;
	device->reset();
	simulation->reset();
	
	try {
		cut.cut(*device);
	}
	catch (COutputDevice::StoppedException & ) {
		// Nop
	}
	catch (COutputDevice::LimitsException & limits) {
		std::string msg("Output device out of limits: ");
		msg.append(limits.what());
		AfxMessageBox(msg.c_str(), MB_OK | MB_ICONWARNING);
	}

}

void CutterPreviewWindow::createBitmap(CDC* pdc)
{
	bitmap = new CBitmap();
	bitmap->CreateCompatibleBitmap(
		pdc, nWidth, nHeight);

	// Colours / shades of grey to use
	const DWORD background = RGB(64, 64, 64);
	const DWORD grid = RGB(80, 80, 80);  // arbitrary but slightly brighter than background

	// Fill the bitmap with grey background
	int nPixels = nWidth * nHeight;
	DWORD* bits = new DWORD[nPixels];
	DWORD* here = bits;
	for (int i = 0; i < nPixels; ++i) {
		*here++ = background;
	}

	// Draw a grid on the background.
	const double xGridSpacing = 10.0;
	const double yGridSpacing = 10.0;

	// Vertical lines
	int yStart = (int)round(yStepsPerMM * border);
	int yStop = (int)round(yStepsPerMM * (border + yTravel));
	assert(yStart < nHeight);
	assert(yStop < nHeight);

	for (double x = border; x <= xTravel + border; x += xGridSpacing) {
		int ix = 1 + (int)round(x * xStepsPerMM);
		assert(ix < nWidth);
		for (int iy = yStart; iy <= yStop; ++iy) {
			here = bits + ix + (intptr_t)iy * (intptr_t)nWidth;
			*here = grid;
		}
	}
	// Horizontal lines
	int xStart = (int)round(xStepsPerMM * border);
	int xStop = (int)round(xStepsPerMM * (border + xTravel));
	assert(xStart < nWidth);
	assert(xStop < nWidth);

	for (double y = border; y <= yTravel + border; y += yGridSpacing) {
		int iy = 1 + (int)round(y * yStepsPerMM);
		assert(iy < nHeight);
		here = bits + (intptr_t)nWidth * (intptr_t)iy;
		for (int ix = xStart; ix < xStop; ++ix) {
			here = bits + ix + (intptr_t)iy * (intptr_t)nWidth;
			*here = grid;
		}
	}
	bitmap->SetBitmapBits(sizeof(DWORD) * nPixels, bits);
	delete[] bits;

	bitmapDirty = false;
}

void CutterPreviewWindow::drawFlash(CDC* pdc)
{
	int oldMode = pdc->GetROP2();
	pdc->SetROP2(R2_XORPEN);

	CPen penl(PS_DOT, 1, leftFlashColour);
	CPen penr(PS_DOT, 1, rightFlashColour);

	CPen* penOld = pdc->SelectObject(&penl);
	pdc->MoveTo(previousFlash.x - flashSize, previousFlash.y - flashSize);
	pdc->LineTo(previousFlash.x + flashSize, previousFlash.y + flashSize);
	pdc->MoveTo(previousFlash.x - flashSize, previousFlash.y + flashSize);
	pdc->LineTo(previousFlash.x + flashSize, previousFlash.y - flashSize);
	pdc->LineTo(previousFlash.x + flashSize, previousFlash.y + flashSize);
	pdc->LineTo(previousFlash.x - flashSize, previousFlash.y + flashSize);
	pdc->LineTo(previousFlash.x - flashSize, previousFlash.y - flashSize);
	pdc->LineTo(previousFlash.x + flashSize, previousFlash.y - flashSize);

	pdc->SelectObject(&penr);
	pdc->MoveTo(previousFlash.u - flashSize, previousFlash.v - flashSize);
	pdc->LineTo(previousFlash.u + flashSize, previousFlash.v + flashSize);
	pdc->MoveTo(previousFlash.u - flashSize, previousFlash.v + flashSize);
	pdc->LineTo(previousFlash.u + flashSize, previousFlash.v - flashSize);
	pdc->LineTo(previousFlash.u + flashSize, previousFlash.v + flashSize);
	pdc->LineTo(previousFlash.u - flashSize, previousFlash.v + flashSize);
	pdc->LineTo(previousFlash.u - flashSize, previousFlash.v - flashSize);
	pdc->LineTo(previousFlash.u + flashSize, previousFlash.v - flashSize);
	pdc->SelectObject(penOld);
	pdc->SetROP2(oldMode);
}

void CutterPreviewWindow::unflash(CDC* pdc) {
	if (hasFlash) {
		drawFlash(pdc);
		hasFlash = false;
	}
}

// Provides a bit of a flicker at the drawing point so that it's possible to see where the wire is even if
// going over old cuts.
void CutterPreviewWindow::flash(CDC* pdc, const Position<long long>& axes, DWORD leftColour, DWORD rightColour) {
	assert(!hasFlash);
	
	// Calculate new point
	previousFlash.x = (int)(axes.x - xCurrentScroll + 1);
	previousFlash.y = (int)(axes.y - yCurrentScroll + 1);
	previousFlash.u = (int)(axes.u - xCurrentScroll + 1);
	previousFlash.v = (int)(axes.v - yCurrentScroll + 1);

	// Set new colours
	leftFlashColour = leftColour;
	rightFlashColour = rightColour;

	drawFlash(pdc);

	hasFlash = true;
}

bool CutterPreviewWindow::plot(Position<long long> axes)
{
	// If being asked to plot when the window is being destroyed then treat the same
	// as any hardware being stopped:  all bets are off so abort the cut.
	if (terminate) {
		throw COutputDevice::StoppedException();
	}

	CDC* pdc = GetDC();

	// Create a bitmap if not already done.
	if (bitmap == 0) {
		createBitmap(pdc);
	}

	bool axesSuperimposed = axes.x == axes.u && axes.y == axes.v;


	// Set up colours and clip axes.  Show error colour if cutter might
	// go out of bounds (e.g. to cut a taper where LE might be at x=0 but
	// to get there the actual hardware has to go negative.
	DWORD leftColour = RGB(0, 255, 0);  
	DWORD rightColour = RGB(80, 80, 255); // slight lighter blue much easier to see
	DWORD combinedColour = RGB(0, 255, 255);
	DWORD errorLeftColour = RGB(255, 128, 0);
	DWORD errorRightColour = RGB(255, 0, 128);
	DWORD errorCombinedColour = RGB(255, 128, 128);


	// Set colour if we're out of limits. Note that due to border we can draw out
	// of limits a bit. 
	int maxx = (int)round(xTravel * xStepsPerMM);
	int maxy = (int)round(yTravel * yStepsPerMM);
	if (axes.x < 0) {
		leftColour = errorLeftColour; combinedColour = errorCombinedColour;
	}
	if (axes.y < 0) {
		leftColour = errorLeftColour; combinedColour = errorCombinedColour;
	}
	if (axes.u < 0) {
		rightColour = errorRightColour; combinedColour = errorCombinedColour;
	}
	if (axes.v < 0) {
		rightColour = errorRightColour; combinedColour = errorCombinedColour;
	}
	if (axes.x > maxx) {
		leftColour = errorLeftColour; combinedColour = errorCombinedColour;
	}
	if (axes.y > maxy) {
		leftColour = errorLeftColour; combinedColour = errorCombinedColour;
	}
	if (axes.u > maxx) {
		rightColour = errorRightColour; combinedColour = errorCombinedColour;
	}
	if (axes.v > maxy){
		rightColour = errorRightColour; combinedColour = errorCombinedColour;
	}

	// Shift drawing to allow for border
	int xOffset = (int)(border * xStepsPerMM);
	int yOffset = (int)(border * yStepsPerMM);
	axes.x += xOffset;
	axes.y += yOffset;
	axes.u += xOffset;
	axes.v += yOffset;

	// Make sure zero at the bottom:
	axes.y = nHeight - axes.y;
	axes.v = nHeight - axes.v;

	// Now constrain to drawing area
	if (axes.x < 0) axes.x = 0; 
	if (axes.y < 0) axes.y = 0;
	if (axes.u < 0) axes.u = 0;
	if (axes.v < 0) axes.v = 0;
	if (axes.x > nWidth) axes.x = nWidth; 
	if (axes.y > nHeight) axes.y = nHeight;
	if (axes.u > nWidth) axes.u = nWidth;
	if (axes.v > nHeight) axes.v = nHeight;

	unflash(pdc);

	// Write to the screen
	// Note +1 to allow 1 pixel border to mark attempts to draw/move out of area.
	if (axesSuperimposed) {
		pdc->SetPixel((int)(axes.x - xCurrentScroll + 1), (int)(axes.y - yCurrentScroll + 1), combinedColour);
	}
	else {
		pdc->SetPixel((int)(axes.x - xCurrentScroll + 1), (int)(axes.y - yCurrentScroll + 1), leftColour);
		pdc->SetPixel((int)(axes.u - xCurrentScroll + 1), (int)(axes.v -yCurrentScroll + 1), rightColour);
	}
	
	flash(pdc, axes, leftColour, rightColour);

	// Write into the bitmap
	CDC memDC;
	if (memDC.CreateCompatibleDC(pdc) != 0) { //create a memory DC compatible with pDC
		CBitmap* pOldBm = memDC.SelectObject(bitmap); //Select the bitmap into the DC
		if (axesSuperimposed) {
			memDC.SetPixel((int)(axes.x + 1), (int)(axes.y + 1), combinedColour);
		} else {
			memDC.SetPixel((int)(axes.x + 1), (int)(axes.y + 1), leftColour);
			memDC.SetPixel((int)(axes.u + 1), (int)(axes.v + 1), rightColour);
		}
		memDC.SelectObject(pOldBm);
		bitmapDirty = true;
	}
	ReleaseDC(pdc);

	processMessages();

	return terminate;
}

void CutterPreviewWindow::setResolution(double xStepsPerMM, double yStepsPerMM)
{
	this->xStepsPerMM = xStepsPerMM;
	this->yStepsPerMM = yStepsPerMM;
}

void CutterPreviewWindow::setCutterBounds(double xTravel, double yTravel)
{
	this->xTravel = xTravel;
	this->yTravel = yTravel;
}

void CutterPreviewWindow::processMessages() {

	if (plotTime.QuadPart == 0) {
		QueryPerformanceCounter(&plotTime);
	}

	LARGE_INTEGER now;
	do {
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage()) {
				::PostQuitMessage(0);
				terminate = true;
				break;
			}
		}
		QueryPerformanceCounter(&now);
		
		if (terminate) {
			break;
		}

		// If we're paused then include idle time processing to allow any MFC operations to complete.
		// also update timer
		if (isPaused) {
			LONG lIdle = 0;
			while (AfxGetApp()->OnIdle(lIdle++))
				;
			QueryPerformanceCounter(&now);
		}
	} while (isPaused || (now.QuadPart - plotTime.QuadPart) < stepTime.QuadPart);
	plotTime = now;

	// Possible that the message pump has allowed the user to close the window. 
	// If so, terminate will be true so signal that processing is stopped.
	if (terminate) {
		throw COutputDevice::StoppedException(); // unravel back up through message loop and terminate plot
	}

	// let MFC do its idle processing
	LONG lIdle = 0;
	while (AfxGetApp()->OnIdle(lIdle++))
		;
}

void CutterPreviewWindow::PostNcDestroy()
{
	delete this; // goodbye cruel world...
}



BEGIN_MESSAGE_MAP(CutterPreviewWindow, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_CONTROLCUTTER_START, &CutterPreviewWindow::OnControlcutterStart)
	ON_COMMAND(ID_CONTROLCUTTER_PAUSE, &CutterPreviewWindow::OnControlcutterPause)
	ON_COMMAND(ID_CONTROLCUTTER_STOP, &CutterPreviewWindow::OnControlcutterStop)
	ON_UPDATE_COMMAND_UI(ID_CONTROLCUTTER_PAUSE, &CutterPreviewWindow::OnUpdateControlcutterPause)
	ON_UPDATE_COMMAND_UI(ID_CONTROLCUTTER_START, &CutterPreviewWindow::OnUpdateControlcutterStart)
	ON_UPDATE_COMMAND_UI(ID_CONTROLCUTTER_STOP, &CutterPreviewWindow::OnUpdateControlcutterStop)
	ON_COMMAND(ID_SPEED_X1, &CutterPreviewWindow::OnSpeedX1)
	ON_COMMAND(ID_SPEED_X10, &CutterPreviewWindow::OnSpeedX10)
	ON_COMMAND(ID_SPEED_X2, &CutterPreviewWindow::OnSpeedX2)
	ON_COMMAND(ID_SPEED_X5, &CutterPreviewWindow::OnSpeedX5)
	ON_COMMAND(ID_SPEED_FULL, &CutterPreviewWindow::OnSpeedFull)
END_MESSAGE_MAP()

// CutterPreviewWindow message handlers


void CutterPreviewWindow::OnPaint()
{
	if (bitmap) {

		if (hasFlash) {
			CDC* pdc = GetDC();
			unflash(pdc);
			ReleaseDC(pdc);
		}

		// Paint DC to copy to
		CPaintDC dc(this); // device context for painting
		BITMAP bmpStruct; //Bitmap information structure
		bitmap->GetObject(sizeof(bmpStruct), &bmpStruct); //fill the structure with useful data

		// Memory DC to copy from
		CDC memDC;
		if (memDC.CreateCompatibleDC(&dc) != 0) { //create a memory DC compatible with pDC
			CBitmap* pOldBm = memDC.SelectObject(bitmap); //Select the bitmap into the DC

			// And BitBlit pixels fom bitmap via memory DC to paint DC
			int status = dc.BitBlt(0, 0, bmpStruct.bmWidth, bmpStruct.bmHeight, &memDC, xCurrentScroll, yCurrentScroll,  SRCCOPY); // This is the heart of bitmap drawing
			printf("status %d", status);
			memDC.SelectObject(pOldBm);
		}
	}
}


void CutterPreviewWindow::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	int xNewSize = cx;
	int yNewSize = cy;

//	if (fBlt)
//		fSize = TRUE;

	SCROLLINFO si;

	// The horizontal scrolling range is defined by 
	// (bitmap_width) - (client_width). The current horizontal 
	// scroll value remains within the horizontal scrolling range. 
	xMaxScroll = max(nWidth - xNewSize, 0);
	xCurrentScroll = min(xCurrentScroll, xMaxScroll);
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = xMinScroll;
	si.nMax = nWidth;
	si.nPage = xNewSize;
	si.nPos = xCurrentScroll;
	SetScrollInfo( SB_HORZ, &si, TRUE);

	// The vertical scrolling range is defined by 
	// (bitmap_height) - (client_height). The current vertical 
	// scroll value remains within the vertical scrolling range. 
	yMaxScroll = max(nHeight - yNewSize, 0);
	yCurrentScroll = min(yCurrentScroll, yMaxScroll);
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = yMinScroll;
	si.nMax = nHeight;
	si.nPage = yNewSize;
	si.nPos = yCurrentScroll;
	SetScrollInfo( SB_VERT, &si, TRUE);

}


void CutterPreviewWindow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int xDelta;     // xDelta = new_pos - current_pos  
	int xNewPos;    // new position 
	int yDelta = 0;

	switch (nSBCode)
	{
		// User clicked the scroll bar shaft left of the scroll box. 
	case SB_PAGEUP:
		xNewPos = xCurrentScroll - 50;
		break;

		// User clicked the scroll bar shaft right of the scroll box. 
	case SB_PAGEDOWN:
		xNewPos = xCurrentScroll + 50;
		break;

		// User clicked the left arrow. 
	case SB_LINEUP:
		xNewPos = xCurrentScroll - 5;
		break;

		// User clicked the right arrow. 
	case SB_LINEDOWN:
		xNewPos = xCurrentScroll + 5;
		break;

		// User dragged the scroll box. 
	case SB_THUMBPOSITION:
		xNewPos = nPos;
		break;

	default:
		xNewPos = xCurrentScroll;
	}

	// New position must be between 0 and the screen width. 
	xNewPos = max(0, xNewPos);
	xNewPos = min(xMaxScroll, xNewPos);

	// If the current position does not change, do not scroll.
	if (xNewPos != xCurrentScroll) {

		if (hasFlash) {
			CDC* pdc = GetDC();
			unflash(pdc);
			ReleaseDC(pdc);
		}
		// Determine the amount scrolled (in pixels). 
		xDelta = xNewPos - xCurrentScroll;

		// Reset the current scroll position. 
		xCurrentScroll = xNewPos;

		// Scroll the window. (The system repaints most of the 
		// client area when ScrollWindowEx is called; however, it is 
		// necessary to call UpdateWindow in order to repaint the 
		// rectangle of pixels that were invalidated.) 
		ScrollWindowEx(-xDelta, -yDelta, 0, 0, 0, 0, SW_INVALIDATE);
		UpdateWindow();

		// Reset the scroll bar. 
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		si.nPos = xCurrentScroll;
		SetScrollInfo(SB_HORZ, &si, TRUE);
	}
}


void CutterPreviewWindow::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int xDelta = 0;
	int yDelta;     // yDelta = new_pos - current_pos 
	int yNewPos;    // new position 

	switch (nSBCode)
	{
		// User clicked the scroll bar shaft above the scroll box. 
	case SB_PAGEUP:
		yNewPos = yCurrentScroll - 50;
		break;

		// User clicked the scroll bar shaft below the scroll box. 
	case SB_PAGEDOWN:
		yNewPos = yCurrentScroll + 50;
		break;

		// User clicked the top arrow. 
	case SB_LINEUP:
		yNewPos = yCurrentScroll - 5;
		break;

		// User clicked the bottom arrow. 
	case SB_LINEDOWN:
		yNewPos = yCurrentScroll + 5;
		break;

		// User dragged the scroll box. 
	case SB_THUMBPOSITION:
		yNewPos = nPos;
		break;

	default:
		yNewPos = yCurrentScroll;
	}

	// New position must be between 0 and the screen height. 
	yNewPos = max(0, yNewPos);
	yNewPos = min(yMaxScroll, yNewPos);

	// If the current position does not change, do not scroll.
	if (yNewPos != yCurrentScroll) {

		if (hasFlash) {
			CDC* pdc = GetDC();
			unflash(pdc);
			ReleaseDC(pdc);
		}

		// Determine the amount scrolled (in pixels). 
		yDelta = yNewPos - yCurrentScroll;

		// Reset the current scroll position. 
		yCurrentScroll = yNewPos;

		// Scroll the window. (The system repaints most of the 
		// client area when ScrollWindowEx is called; however, it is 
		// necessary to call UpdateWindow in order to repaint the 
		// rectangle of pixels that were invalidated.) 
		ScrollWindowEx( -xDelta, -yDelta, 0, 0, 0, 0, SW_INVALIDATE);
		Invalidate(); // get whole window to redraw as otherwise it scrolls toolbar image down.
		UpdateWindow();

		// Reset the scroll bar. 
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		si.nPos = yCurrentScroll;
		SetScrollInfo( SB_VERT, &si, TRUE);
	}
}

// Need to pick up the window being destroyed so that the message pump
// can terminate any plot.
void CutterPreviewWindow::OnDestroy()
{
	__super::OnDestroy();
	terminate = true;
}


int CutterPreviewWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!toolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!toolbar.LoadToolBar(IDR_CONTROLCUTTER))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Command IDs for the toolbar:  ORDERING MUST MATCH BITMAP - see IDR_CONTROLCUTTER toolbar
	UINT commands[] = {
		ID_CONTROLCUTTER_START,
		ID_CONTROLCUTTER_PAUSE,
		ID_CONTROLCUTTER_STOP,
		ID_SPEED_X1,
		ID_SPEED_X2,
		ID_SPEED_X5,
		ID_SPEED_X10,
		ID_SPEED_FULL,
	};
	toolbar.SetButtons(commands, sizeof(commands)/sizeof(commands[0]));

	// Make toolbar dockable
	/*toolbar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&toolbar);*/

	return 0;
}


void CutterPreviewWindow::OnControlcutterStart()
{
	if (isCutting) {
		isPaused = false;
	}
	else {
		isCutting = true;
		runCut();
		isCutting = false;
	}
}

void CutterPreviewWindow::OnControlcutterPause()
{
	isPaused = true;
}


void CutterPreviewWindow::OnControlcutterStop()
{
	terminate = true;
	isPaused = false;
	isCutting = false;
}


void CutterPreviewWindow::OnUpdateControlcutterPause(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((isCutting & !isPaused) ? TRUE : FALSE);
}


void CutterPreviewWindow::OnUpdateControlcutterStart(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((!isCutting || isPaused) ? TRUE : FALSE);
}


void CutterPreviewWindow::OnUpdateControlcutterStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(isCutting ? TRUE : FALSE);
}


void CutterPreviewWindow::OnSpeedFull()
{
	stepTime.QuadPart = 0;
}


void CutterPreviewWindow::OnSpeedX1()
{
	stepTime.QuadPart = counterFrequency.QuadPart / DEFAULT_STEPS_PER_SEC;

}


void CutterPreviewWindow::OnSpeedX10()
{
	stepTime.QuadPart = counterFrequency.QuadPart / (DEFAULT_STEPS_PER_SEC * 10);
}


void CutterPreviewWindow::OnSpeedX2()
{
	stepTime.QuadPart = counterFrequency.QuadPart / (DEFAULT_STEPS_PER_SEC * 2);
}


void CutterPreviewWindow::OnSpeedX5()
{
	stepTime.QuadPart = counterFrequency.QuadPart / (DEFAULT_STEPS_PER_SEC * 5);
}
