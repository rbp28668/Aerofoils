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

// CutterFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "CutterFrame.h"
#include "CutterView.h"
#include "CutterTreeView.h"


// CutterFrame

IMPLEMENT_DYNCREATE(CutterFrame, CMDIChildWnd)

CutterFrame::CutterFrame()
{

}

CutterFrame::~CutterFrame()
{
}

BOOL CutterFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	BOOL isOk = m_wndSplitter.CreateStatic(this, 1, 2);
	if (isOk) {
		m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CutterTreeView), CSize(100, 150),
			pContext);
		m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CutterView), CSize(200, 150),
			pContext);
	}
	return isOk;
}

BEGIN_MESSAGE_MAP(CutterFrame, CMDIChildWnd)
	ON_COMMAND(ID_VIEW_DRAWMOVES, &CutterFrame::OnViewDrawmoves)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DRAWMOVES, &CutterFrame::OnUpdateViewDrawmoves)
	ON_COMMAND(ID_VIEW_ZOOM, &CutterFrame::OnViewZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, &CutterFrame::OnUpdateViewZoom)
END_MESSAGE_MAP()


// CutterFrame message handlers


void CutterFrame::OnViewDrawmoves()
{
	static_cast<CutterView*>(m_wndSplitter.GetPane(0, 1))->OnViewDrawmoves();
}


void CutterFrame::OnUpdateViewDrawmoves(CCmdUI *pCmdUI)
{
	static_cast<CutterView*>(m_wndSplitter.GetPane(0, 1))->OnUpdateViewDrawmoves(pCmdUI);
}


void CutterFrame::OnViewZoom()
{
	static_cast<CutterView*>(m_wndSplitter.GetPane(0, 1))->OnViewZoom();
}


void CutterFrame::OnUpdateViewZoom(CCmdUI *pCmdUI)
{
	static_cast<CutterView*>(m_wndSplitter.GetPane(0, 1))->OnUpdateViewZoom(pCmdUI);
}
