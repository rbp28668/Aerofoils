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
// AerofoilView.h : interface of the CAerofoilView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEROFOILVIEW_H__BBA1B6AF_B27A_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_AEROFOILVIEW_H__BBA1B6AF_B27A_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlotStructure;

class CAerofoilView : public CScrollView
{
protected: // create from serialization only
	CAerofoilView();
	DECLARE_DYNCREATE(CAerofoilView)

// Attributes
public:
	CAerofoilDoc* GetDocument();

// Operations
public:
	void DocSizeChanged() {OnInitialUpdate();}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAerofoilView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAerofoilView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// for dragging plot structures.
	void removeDragRect();
	void drawDragRect();

private:
	float zoom;		// zoom factor
	CPlotStructure* captured;	// if dragging then this is the captured structure.
	CPoint capturedPosition;	// and where the thing was captured.
	CRect boundsRect;			// bounds of structure when mouse down
	CPoint dragOffset;			// add this to boundsRect to get last draw position
	bool dragRectDrawn;			// and flag to check it's drawn (as XOR).
	CPoint mouse;				// store raw mouse coords for auto-scroll
	bool isPrinting;			// true if currently printing.

// Generated message map functions
protected:
	//{{AFX_MSG(CAerofoilView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnViewZoom();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewPreviewCutPath();
	afx_msg void OnTimer(UINT_PTR  nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AerofoilView.cpp
inline CAerofoilDoc* CAerofoilView::GetDocument()
   { return (CAerofoilDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AEROFOILVIEW_H__BBA1B6AF_B27A_11D6_AF75_002018BA082E__INCLUDED_)
