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

#pragma once


class CutterDoc;


// CutterView view provides the graphical view of the cutter path (not animated).
// It's a dumb view with pretty much all the commands being handled by CutterTreeView.
class CutterView : public CScrollView
{
	DECLARE_DYNCREATE(CutterView)
	float zoom;		// zoom factor
	bool drawMoves; // true to draw moves between objects.

protected:
	CutterView();           // protected constructor used by dynamic creation
	virtual ~CutterView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	CutterDoc* GetDocument();
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()

public:
	void DocSizeChanged();
	afx_msg void OnViewDrawmoves();
	afx_msg void OnUpdateViewDrawmoves(CCmdUI *pCmdUI);
	afx_msg void OnViewZoom();
	afx_msg void OnUpdateViewZoom(CCmdUI *pCmdUI);
	afx_msg void OnViewPreviewCutPath();
};


