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

// CutterView.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "CutterView.h"
#include "CutterDoc.h"
#include "CutterFrame.h"
#include "CoordMap.h"
#include "ZoomDlg.h"
#include "WindowsOutputDevice.h"
#include "CutterPreviewWindow.h"
#include "Kernel/PointT.h"
#include "Kernel/RectT.h"


// CutterView

IMPLEMENT_DYNCREATE(CutterView, CScrollView)

CutterView::CutterView()
	: zoom(1.0)
	, drawMoves(true)
{

}

CutterView::~CutterView()
{
}


BEGIN_MESSAGE_MAP(CutterView, CScrollView)
	ON_COMMAND(ID_VIEW_DRAWMOVES, &CutterView::OnViewDrawmoves)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAWMOVES, &CutterView::OnUpdateViewDrawmoves)
	ON_COMMAND(ID_VIEW_ZOOM, &CutterView::OnViewZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, &CutterView::OnUpdateViewZoom)
	ON_COMMAND(ID_VIEW_PREVIEW_CUT_PATH, &CutterView::OnViewPreviewCutPath)
END_MESSAGE_MAP()


// CutterView drawing

void CutterView::OnInitialUpdate()
{
	DocSizeChanged();
}

void CutterView::DocSizeChanged()
{
	CutterDoc* doc = GetDocument();

	PointT docSize(doc->sizeX(), doc->sizeY());
	CCoordMap map(docSize.fx, docSize.fy, GetDC(), zoom);
	docSize.fy = 0;	// due to inversion of coords in MM_TEXT
	POINT size = map.toDevice(docSize);

	SetScrollSizes(MM_TEXT, CSize(size.x, size.y));
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

CutterDoc * CutterView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CutterDoc)));
	return static_cast<CutterDoc*>(m_pDocument);
}


void CutterView::OnDraw(CDC* pDC)
{
	CutterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPen pen(PS_DOT, 1, RGB(192, 192, 192));
	CPen* penOld = pDC->SelectObject(&pen);
	PointT size(pDoc->sizeX(), pDoc->sizeY());
	CCoordMap map(size.fx, size.fy, pDC, zoom);

	// Draw print boundary.
	RectT rLogical(0, size.fy, size.fx, 0);
	CRect rDoc = map.toDevice(rLogical);
	pDC->Rectangle(&rDoc);

	// Draw Grid
	CBackgroundGrid& grid = pDoc->getGrid();
	if (grid.isHorizontalEnabled())
	{
		for (NumericT fy = size.fy; fy >= 0.0f; fy -= grid.getHorizontalSize())
		{
			PointT start(0, fy);
			PointT finish(size.fx, fy);

			POINT st = map.toDevice(start);
			POINT fn = map.toDevice(finish);

			pDC->MoveTo(st);
			pDC->LineTo(fn);
		}
	}


	if (grid.isVerticalEnabled())
	{
		for (NumericT fx = size.fx; fx >= 0.0f; fx -= grid.getVerticalSize())
		{
			PointT start(fx, 0);
			PointT finish(fx, size.fy);

			POINT st = map.toDevice(start);
			POINT fn = map.toDevice(finish);

			pDC->MoveTo(st);
			pDC->LineTo(fn);
		}
	}

	pDC->SelectObject(penOld);

	// Draw main plot.
	CWindowsOutputDevice output(pDoc->sizeX(), pDoc->sizeY(), pDC, zoom);
	output.setDrawMoves(drawMoves);
	pDoc->runCut(output);
}


// CutterView diagnostics

#ifdef _DEBUG
void CutterView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CutterView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CutterView message handlers


void CutterView::OnViewDrawmoves()
{
	drawMoves = !drawMoves;
	RedrawWindow();
}


void CutterView::OnUpdateViewDrawmoves(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(drawMoves ? 1 : 0);
}


void CutterView::OnViewZoom()
{
	CZoomDlg dlg;
	dlg.m_zoomIndex = 1; // 100%
	if (dlg.DoModal())
	{
		switch (dlg.m_zoomIndex)
		{
		case 0:	zoom = 2;		break;
		case 1: zoom = 1;		break;
		case 2: zoom = 0.5f;	break;
		case 3: zoom = 0.25f;	break;
		case 4:
		{
			// Get current size of window
			CRect rClient;
			GetClientRect(&rClient);

			// Get size of window needed for client at 1.0 zoom
			CutterDoc* doc = GetDocument();

			PointT docSize(doc->sizeX(), doc->sizeY());
			CCoordMap map(docSize.fx, docSize.fy, GetDC(), 1.0f);
			docSize.fy = 0;	// due to inversion of coords in MM_TEXT
			POINT size = map.toDevice(docSize);

			float zx = float(rClient.Width() / float(size.x));
			float zy = float(rClient.Height() / float(size.y));

			zoom = min(zx, zy);
			break;
		}
		}

		OnInitialUpdate(); // recalc sizes.
	}

}


void CutterView::OnUpdateViewZoom(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(1);
}

void CutterView::OnViewPreviewCutPath()
{
	
	CutterPreviewWindow* preview = new CutterPreviewWindow(this, &(GetDocument()->cut), &(GetDocument()->getGeometry()));
	preview->ShowWindow(TRUE);

}

