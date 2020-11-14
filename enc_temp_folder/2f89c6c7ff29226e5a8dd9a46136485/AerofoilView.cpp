/* Aerofoil
Aerofoil plotting and CNC cutter driver
Copyright(C) 1995-2019 R Bruce Porteous

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
// AerofoilView.cpp : implementation of the CAerofoilView class
//

#include "stdafx.h"
#include <assert.h>

#include "Aerofoil.h"

#include "AerofoilDoc.h"
#include "AerofoilView.h"
#include "WindowsOutputDevice.h"
#include "ZoomDlg.h"
#include "WindowsUIProxy.h"

#include "Kernel/Kernel.h"
#include "kernel/PlotStructure.h"

#define CLICK_DISTANCE 5

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAerofoilView

IMPLEMENT_DYNCREATE(CAerofoilView, CScrollView)

BEGIN_MESSAGE_MAP(CAerofoilView, CScrollView)
	//{{AFX_MSG_MAP(CAerofoilView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAerofoilView construction/destruction

CAerofoilView::CAerofoilView()
: zoom(1.0f)
, captured(0)
, dragRectDrawn(false)
, isPrinting(false)
{
}

CAerofoilView::~CAerofoilView()
{
}

BOOL CAerofoilView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAerofoilView drawing

void CAerofoilView::OnDraw(CDC* pDC)
{
	CAerofoilDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPen pen(PS_DOT,1,RGB(192,192,192));
	CPen* penOld = pDC->SelectObject(&pen);
	PointT size(pDoc->sizeX(), pDoc->sizeY());
	CCoordMap map(size.fx, size.fy, pDC, zoom);

	// Draw print boundary.
	RectT rLogical(0,size.fy,size.fx,0);
	CRect rDoc = map.toDevice(rLogical);
	pDC->Rectangle(&rDoc);

	// Draw Grid
	CBackgroundGrid& grid = pDoc->getGrid();
	if(grid.isHorizontalEnabled())
	{
		for(NumericT fy = size.fy; fy >= 0.0f; fy -= grid.getHorizontalSize())
		{
			PointT start(0,fy);
			PointT finish(size.fx,fy);

			POINT st = map.toDevice(start);
			POINT fn = map.toDevice(finish);

			pDC->MoveTo(st);
			pDC->LineTo(fn);
		}
	}


	if(grid.isVerticalEnabled())
	{
		for(NumericT fx = size.fx; fx >= 0.0f; fx -= grid.getVerticalSize())
		{
			PointT start(fx,0);
			PointT finish(fx,size.fy);

			POINT st = map.toDevice(start);
			POINT fn = map.toDevice(finish);

			pDC->MoveTo(st);
			pDC->LineTo(fn);
		}
	}

	pDC->SelectObject(penOld);

	// Draw main plot.
	CWindowsOutputDevice output(pDoc->sizeX(), pDoc->sizeY(), pDC, zoom);
	output.setDrawMoves(false);
	CPlotStructure* selected = ( isPrinting) ? 0 : pDoc->getSelection();
	pDoc->getPlot().plot(output, selected);

}

/////////////////////////////////////////////////////////////////////////////
// CAerofoilView printing

BOOL CAerofoilView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAerofoilView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	isPrinting = true;
}

void CAerofoilView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	isPrinting = false;
}

/////////////////////////////////////////////////////////////////////////////
// CAerofoilView diagnostics

#ifdef _DEBUG
void CAerofoilView::AssertValid() const
{
	CView::AssertValid();
}

void CAerofoilView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAerofoilDoc* CAerofoilView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAerofoilDoc)));
	return (CAerofoilDoc*)m_pDocument;
}
#endif //_DEBUG

void CAerofoilView::removeDragRect()
{
	assert(this);

	// remove old drag rect.
	if(dragRectDrawn)
	{
		CDC* pdc = GetDC();
		OnPrepareDC(pdc);

		CRect r = boundsRect;
		r.OffsetRect(dragOffset);
		pdc->DrawFocusRect(&r);
		dragRectDrawn = false;
	}

}

void CAerofoilView::drawDragRect()
{
	assert(this);

	if(!dragRectDrawn)
	{
		CDC* pdc = GetDC();
		OnPrepareDC(pdc);

		CRect r = boundsRect;
		r.OffsetRect(dragOffset);

		pdc->DrawFocusRect(&r);
		dragRectDrawn = true;
	}

}


/////////////////////////////////////////////////////////////////////////////
// CAerofoilView message handlers

void CAerofoilView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CAerofoilDoc* doc = GetDocument();

	mouse = point; // store raw coords for scroll mgt

	CDC* pdc = GetDC();
	OnPrepareDC(pdc);
	pdc->DPtoLP(&point);

	CCoordMap map(doc->sizeX(), doc->sizeY(), pdc, zoom);
	PointT ptmm = map.toLogical(point);

	CPlot& plot = doc->getPlot();

	NumericT minDist;

	CPlotStructure* nearest = plot.closestTo(ptmm.fx, ptmm.fy, minDist);
	if(nearest && (minDist < CLICK_DISTANCE))	// arbitrary neeed to click within this no of mm of point to select
	{
		doc->setSelection(nearest);

		captured = nearest;
		capturedPosition = point;
		dragOffset.x = dragOffset.y = 0;	// not moved yet...

		RectT bounds = captured->getBounds();
		boundsRect = map.toDevice(bounds);
		
		drawDragRect();

		SetCapture();

		SetTimer(1,50,0); // 50mS timer for scolling help
	}
	else
	{
		doc->clearSelection();
	}
}

void CAerofoilView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(captured)
	{
		KillTimer(1);

		CDC* pdc = GetDC();
		OnPrepareDC(pdc);
		pdc->DPtoLP(&point);

		removeDragRect();

		dragOffset = point - capturedPosition; // new offset to current position

		CAerofoilDoc* doc = GetDocument();
		CCoordMap map(doc->sizeX(), doc->sizeY(), pdc, zoom);

		CPoint original = map.toDevice(PointT(captured->getX(), captured->getY()));
		CPoint final = original + dragOffset; // offset original pixel coord of structure
		PointT ptmm = map.toLogical(final);
		captured->setPosition(ptmm.fx, ptmm.fy);
		
		ReleaseCapture();
		captured = 0;

		GetDocument()->RedrawNow();
	}
}

void CAerofoilView::OnMouseMove(UINT nFlags, CPoint point) 
{
	mouse = point; // store raw coords for scroll mgt

	if(dragRectDrawn)
	{
		CDC* pdc = GetDC();
		OnPrepareDC(pdc);
		pdc->DPtoLP(&point);

		removeDragRect();
		dragOffset = point - capturedPosition; // new offset to current position
		drawDragRect();
	}
}

void CAerofoilView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDC* pdc = GetDC();
	OnPrepareDC(pdc);
	pdc->DPtoLP(&point);

	pdc->MoveTo(point.x - 10, point.y);
	pdc->LineTo(point.x + 10, point.y);
	pdc->MoveTo(point.x, point.y - 10);
	pdc->LineTo(point.x, point.y + 10);
}

void CAerofoilView::OnInitialUpdate() 
{
	CAerofoilDoc* doc = GetDocument();

	PointT docSize(doc->sizeX(), doc->sizeY());
	CCoordMap map(docSize.fx, docSize.fy, GetDC(), zoom);
	docSize.fy = 0;	// due to inversion of coords in MM_TEXT
	POINT size = map.toDevice(docSize);

	SetScrollSizes(MM_TEXT,CSize(size.x, size.y));
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
}

void CAerofoilView::OnViewZoom() 
{
	CZoomDlg dlg;
	if(dlg.DoModal() == IDOK) {
		if(dlg.fit) {
			// Get current size of window
			CRect rClient;
			GetClientRect(&rClient);

			// Get size of window needed for client at 1.0 zoom
			CAerofoilDoc* doc = GetDocument();

			PointT docSize(doc->sizeX(), doc->sizeY());
			CCoordMap map(docSize.fx, docSize.fy, GetDC(), 1.0f);
			docSize.fy = 0;	// due to inversion of coords in MM_TEXT
			POINT size = map.toDevice(docSize);
				
			float zx =  float(rClient.Width() / float(size.x));
			float zy = float(rClient.Height() / float(size.y));

			zoom  = min(zx,zy);
		} else {
			zoom = dlg.zoom;
		}

		OnInitialUpdate(); // recalc sizes.
	}
}

void CAerofoilView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	assert(this);
	CAerofoilDoc* doc = GetDocument();

	CPoint logical = point;

	CDC* pdc = GetDC();
	OnPrepareDC(pdc);
	pdc->DPtoLP(&logical);

	CCoordMap map(doc->sizeX(), doc->sizeY(), pdc, zoom);
	PointT ptmm = map.toLogical(logical);

	CPlot& plot = doc->getPlot();

	NumericT minDist;

	CPlotStructure* nearest = plot.closestTo(ptmm.fx, ptmm.fy, minDist);
	if(nearest && (minDist < CLICK_DISTANCE))	// arbitrary neeed to click within this no of mm of point to select
	{
		doc->setSelection(nearest);
		ClientToScreen(&point);
		CMenu menuPopup;
		CWindowsUIProxy* proxy = static_cast<CWindowsUIProxy*>(nearest->getUIProxy());
		if(menuPopup.LoadMenu(proxy->getPopupID()))
			menuPopup.GetSubMenu(0)->TrackPopupMenu(0,point.x,point.y,this,0);

	}
	else
	{
		doc->clearSelection();
	}
	
}

void CAerofoilView::OnTimer(UINT_PTR  nIDEvent)
{
	CRect client;
	GetClientRect(&client);

	UINT msg = 0;
	WPARAM wparam = 0;

	if(mouse.x < client.left)
	{
		msg = WM_HSCROLL;
		wparam = SB_LINELEFT;
	}
	else if(mouse.x > client.right)
	{
		msg = WM_HSCROLL;
		wparam = SB_LINERIGHT;
	}

	if(mouse.y < client.top)
	{
		msg = WM_VSCROLL;
		wparam = SB_LINEUP;
	}
	else if(mouse.y > client.bottom)
	{
		msg = WM_VSCROLL;
		wparam = SB_LINEDOWN;
	}


	if(msg != 0) // we need to scroll....
	{
		removeDragRect();
		SendMessage(msg,wparam,0);
		drawDragRect();
	}
	
	CScrollView::OnTimer(nIDEvent);
}
