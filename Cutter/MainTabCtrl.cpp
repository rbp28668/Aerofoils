/* Cutter
Copyright(C) 2019 R Bruce Porteous

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
// MainTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Cutter.h"
#include "MainTabCtrl.h"
#include "resource.h"

#include "GCodeDialog.h"
#include "HardwareDialog.h"
#include "ConfigDialog.h"
#include "CNCFoamCutter.h"
#include "CutterDlg.h"

// CMainTabCtrl

IMPLEMENT_DYNAMIC(CMainTabCtrl, CTabCtrl)

CMainTabCtrl::CMainTabCtrl()
{
	pGCodeDialog = new CGCodeDialog();
	pHardwareDialog = new CHardwareDialog();
	pConfigDialog = new CConfigDialog();

	tabs[0] = pGCodeDialog;
	tabs[1] = pHardwareDialog;
	tabs[2] = pConfigDialog;
	tabCount = 3;
}

CMainTabCtrl::~CMainTabCtrl()
{
	for (int i = 0; i < tabCount; ++i) {
		delete tabs[i];
		tabs[i] = 0;
	}
}

void CMainTabCtrl::init(CCutterDlg* pApp)
{
	assert(this);
	assert(pApp);

	currentTabIndex = 0;

	tabs[0]->Create(IDD_GCODE, GetParent());
	tabs[1]->Create(IDD_HARDWARE, GetParent());
	tabs[2]->Create(IDD_CONFIG, GetParent());

	pGCodeDialog->setInterpreter(pApp->getInterpreter(), pApp->getCutter());
	pHardwareDialog->setHardware(pApp->getHardware());
	pConfigDialog->initialize(pApp);

	tabs[0]->ShowWindow(SW_SHOW);
	tabs[1]->ShowWindow(SW_HIDE);
	tabs[2]->ShowWindow(SW_HIDE);

	InsertItem(0, "G-Code");
	InsertItem(1, "Hardware");
	InsertItem(2, "Config");

	SetCurSel(0);
	sizeChildren();
}

void CMainTabCtrl::sizeChildren()
{

	int nSel = GetCurSel();
	if(nSel >= 0) {
		if (tabs[nSel]->m_hWnd)
			tabs[nSel]->ShowWindow(SW_HIDE);
	}

	CRect l_rectClient;
	CRect l_rectWnd;

	GetClientRect(l_rectClient);
	AdjustRect(FALSE, l_rectClient);
	GetWindowRect(l_rectWnd);
	GetParent()->ScreenToClient(l_rectWnd);
	l_rectClient.OffsetRect(l_rectWnd.left, l_rectWnd.top);
	for (int nCount = 0; nCount < tabCount; nCount++) {
		tabs[nCount]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_HIDEWINDOW);
	}
	if (nSel >= 0) {
		tabs[nSel]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
		tabs[nSel]->ShowWindow(SW_SHOW);
	}

}


BEGIN_MESSAGE_MAP(CMainTabCtrl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMainTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// CMainTabCtrl message handlers




void CMainTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if (currentTabIndex != GetCurFocus()) {
		tabs[currentTabIndex]->ShowWindow(SW_HIDE);
		currentTabIndex = GetCurFocus();
		tabs[currentTabIndex]->ShowWindow(SW_SHOW);
		tabs[currentTabIndex]->SetFocus();
	}

}


void CMainTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	sizeChildren();
	*pResult = 0;
}
