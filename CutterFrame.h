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

#ifdef _WIN32_WCE
#error "CMDIChildWnd is not supported for Windows CE."
#endif 

// CutterFrame frame with splitter
// Overall frame window for a Cutter Doc which manages a splitter window
// for the cutter to provide a tree view of the structure/cuts and 
// a rendering of the current cut.

class CutterFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CutterFrame)
protected:
	CutterFrame();           // protected constructor used by dynamic creation
	virtual ~CutterFrame();

	CSplitterWnd m_wndSplitter;

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewDrawmoves();
	afx_msg void OnUpdateViewDrawmoves(CCmdUI *pCmdUI);
	afx_msg void OnViewZoom();
	afx_msg void OnUpdateViewZoom(CCmdUI *pCmdUI);
};


