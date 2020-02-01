// CutterPreviewWindow.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "CutterPreviewWindow.h"
#include "Kernel/CutterGeometry.h"
#include "Kernel/OutputDevice.h"


// CutterPreviewWindow
// Note for scrolling see: https://docs.microsoft.com/en-us/windows/desktop/controls/scroll-a-bitmap-in-scroll-bars

IMPLEMENT_DYNAMIC(CutterPreviewWindow, CWnd)

CutterPreviewWindow::CutterPreviewWindow(CWnd* parentWindow)
	:plotTime(0)
	, bitmap(0)
	, nWidth(0)
	, nHeight(0)
	, xMinScroll(0)
	, xCurrentScroll(0)
	, xMaxScroll(0)
	, yMinScroll(0)
	, yCurrentScroll(0)
	, yMaxScroll(0)
	, terminate(false)

{
	windowClass = AfxRegisterWndClass( CS_VREDRAW | CS_HREDRAW ,
		::LoadCursor(NULL, IDC_ARROW),
		0, // always draw bitmap so no need for (HBRUSH) ::GetStockObject(WHITE_BRUSH),
		::LoadIcon(NULL, IDI_APPLICATION)
	);
	
	Create(windowClass, "Preview Window",
		WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_THICKFRAME | WS_HSCROLL | WS_VSCROLL,
		CRect(10, 10, 500, 300),
		parentWindow,
		100);
}

CutterPreviewWindow::~CutterPreviewWindow()
{
	delete bitmap;
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
		nWidth = round(xTravel * xStepsPerMM) + 2; // +2 to keep a 1 pixel border
		nHeight = round(yTravel * yStepsPerMM) + 2;

		bitmap = new CBitmap();
		bitmap->CreateCompatibleBitmap(
			pdc, nWidth, nHeight);

		// Fill the bitmap with grey background
		int nPixels = nWidth * nHeight;
		DWORD* bits = new DWORD[nPixels];
		DWORD* here = bits;
		for (int i = 0; i < nPixels; ++i) {
			*here++ = RGB(128,128,128);
		}
		bitmap->SetBitmapBits(sizeof(DWORD)*nPixels, bits);
		delete[] bits;
	}

	bool axesSuperimposed = axes.x == axes.u && axes.y == axes.v;
	
	// Write to the screen
	// Note +1 to allow 1 pixel border to mark attempts to draw/move out of area.
	if (axesSuperimposed) {
		pdc->SetPixel(axes.x - xCurrentScroll + 1, axes.y - yCurrentScroll + 1, RGB(255, 0, 255));
	}
	else {
		pdc->SetPixel(axes.x - xCurrentScroll + 1, axes.y - yCurrentScroll + 1, RGB(255, 0, 0));
		pdc->SetPixel(axes.u - xCurrentScroll + 1, axes.v -yCurrentScroll + 1, RGB(0, 0, 255));
	}
	// Write into the bitmap
	CDC memDC;
	if (memDC.CreateCompatibleDC(pdc) != 0) { //create a memory DC compatible with pDC
		CBitmap* pOldBm = memDC.SelectObject(bitmap); //Select the bitmap into the DC
		if (axesSuperimposed) {
			memDC.SetPixel(axes.x + 1, axes.y + 1, RGB(255, 0, 255));
		} else {
			memDC.SetPixel(axes.x + 1, axes.y + 1, RGB(255, 0, 0));
			memDC.SetPixel(axes.u + 1, axes.v + 1, RGB(0, 0, 255));
		}
		memDC.SelectObject(pOldBm);
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

	if (plotTime == 0) {
		plotTime = ::GetTickCount();
	}

	DWORD now;
	do {
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage()) {
				::PostQuitMessage(0);
				terminate = true;
				break;
			}
		}
		now = ::GetTickCount();
	} while (now == plotTime );
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



BEGIN_MESSAGE_MAP(CutterPreviewWindow, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CutterPreviewWindow message handlers




void CutterPreviewWindow::OnPaint()
{
	if (bitmap) {

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

		// Set the scroll flag to TRUE. 
		//fScroll = TRUE;

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

		// Set the scroll flag to TRUE. 
		//fScroll = TRUE;

		// Determine the amount scrolled (in pixels). 
		yDelta = yNewPos - yCurrentScroll;

		// Reset the current scroll position. 
		yCurrentScroll = yNewPos;

		// Scroll the window. (The system repaints most of the 
		// client area when ScrollWindowEx is called; however, it is 
		// necessary to call UpdateWindow in order to repaint the 
		// rectangle of pixels that were invalidated.) 
		ScrollWindowEx( -xDelta, -yDelta, 0, 0, 0, 0, SW_INVALIDATE);
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
