// CutBoundsDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "CutBoundsDisplay.h"
#include "afxdialogex.h"
#include "Kernel/Bounds.h"
#include "Kernel/CutterGeometry.h"

// CutBoundsDisplay dialog

IMPLEMENT_DYNAMIC(CutBoundsDisplay, CDialogEx)

CutBoundsDisplay::CutBoundsDisplay(const Bounds* bounds, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BOUNDS, pParent)
	, rootMinX(bounds->rootBounds().minx)
	, rootMinY(bounds->rootBounds().miny)
	, rootMinZ(bounds->rootBounds().minz)
	, rootMaxX(bounds->rootBounds().maxx)
	, rootMaxY(bounds->rootBounds().maxy)
	, rootMaxZ(bounds->rootBounds().maxz)
	, tipMinX(bounds->tipBounds().minx)
	, tipMinY(bounds->tipBounds().miny)
	, tipMinZ(bounds->tipBounds().minz)
	, tipMaxX(bounds->tipBounds().maxx)
	, tipMaxY(bounds->tipBounds().maxy)
	, tipMaxZ(bounds->tipBounds().maxz)
	, firstRootX(0)
{

	// Set geometry so that a virtual block with the same width as the effective span of the cut
	// defines the nominal edges of the block.
	CutterGeometry geometry;
	geometry.setBlockLeft(0);
	geometry.setBlockRight(bounds->depth());
	geometry.setWidth(bounds->depth());

	// Calculate line corresponding to the start of the cut.
	Position<double> first;
	first.x = bounds->firstRoot().fx;
	first.y = bounds->firstRoot().fy;
	first.u = bounds->firstTip().fx;
	first.v = bounds->firstTip().fy;
	if (bounds->depth() > 0) {
		geometry.blockToAxes(first, bounds->firstRoot().fz, bounds->firstTip().fz);
	}

	firstRootX = first.x;
	firstRootY = first.y;
	firstTipX = first.u;
	firstTipY = first.v;
	firstSpan = bounds->depth();

	// Calculate line corresponding to the end of the cut.
	Position<double> last;
	last.x = bounds->lastRoot().fx;
	last.y = bounds->lastRoot().fy;
	last.u = bounds->lastTip().fx;
	last.v = bounds->lastTip().fy;
	if (bounds->depth() > 0) {
		geometry.blockToAxes(last, bounds->lastRoot().fz, bounds->lastTip().fz);
	}

	lastRootX = last.x;
	lastRootY = last.y;
	lastTipX = last.u;
	lastTipY = last.v;
	lastSpan = bounds->depth();

}

CutBoundsDisplay::~CutBoundsDisplay()
{
}

void CutBoundsDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ROOT_MIN_X, rootMinX);
	DDX_Text(pDX, IDC_EDT_ROOT_MIN_Y, rootMinY);
	DDX_Text(pDX, IDC_EDT_ROOT_MIN_Z, rootMinZ);
	DDX_Text(pDX, IDC_EDT_ROOT_MAX_X, rootMaxX);
	DDX_Text(pDX, IDC_EDT_ROOT_MAX_Y, rootMaxY);
	DDX_Text(pDX, IDC_EDT_ROOT_MAX_Z, rootMaxZ);
	DDX_Text(pDX, IDC_EDT_TIP_MIN_X, tipMinX);
	DDX_Text(pDX, IDC_EDT_TIP_MIN_Y, tipMinY);
	DDX_Text(pDX, IDC_EDT_TIP_MIN_Z, tipMinZ);
	DDX_Text(pDX, IDC_EDT_TIP_MAX_X, tipMaxX);
	DDX_Text(pDX, IDC_EDT_TIP_MAX_Y, tipMaxY);
	DDX_Text(pDX, IDC_EDT_TIP_MAX_Z, tipMaxZ);
	DDX_Text(pDX, IDC_EDT_FIRST_ROOT_X, firstRootX);
	DDX_Text(pDX, IDC_EDT_FIRST_ROOT_Y, firstRootY);
	DDX_Text(pDX, IDC_EDT_FIRST_TIP_X, firstTipX);
	DDX_Text(pDX, IDC_EDT_FIRST_TIP_Y, firstTipY);
	DDX_Text(pDX, IDC_EDT_FIRST_SPAN, firstSpan);
	DDX_Text(pDX, IDC_EDT_LAST_ROOT_X, lastRootX);
	DDX_Text(pDX, IDC_EDT_LAST_ROOT_Y, lastRootY);
	DDX_Text(pDX, IDC_EDT_LAST_TIP_X, lastTipX);
	DDX_Text(pDX, IDC_EDT_LAST_TIP_Y, lastTipY);
	DDX_Text(pDX, IDC_EDT_LAST_SPAN, lastSpan);
}


BEGIN_MESSAGE_MAP(CutBoundsDisplay, CDialogEx)
END_MESSAGE_MAP()


// CutBoundsDisplay message handlers
