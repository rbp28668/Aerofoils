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
#pragma once

class CGCodeDialog;
class CHardwareDialog;
class CConfigDialog;
class CCutterDlg;

// CMainTabCtrl

class CMainTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMainTabCtrl)


	CGCodeDialog* pGCodeDialog;
	CHardwareDialog* pHardwareDialog;
	CConfigDialog* pConfigDialog;

	CDialog *tabs[4];
	int currentTabIndex;
	int tabCount;

public:
	CMainTabCtrl();
	virtual ~CMainTabCtrl();

	void init(CCutterDlg* pApp);
	void sizeChildren();

	inline CGCodeDialog* getGCodeDialog() {	return pGCodeDialog; }
	inline CHardwareDialog* getHardwareDialog() { return pHardwareDialog; }
	inline CConfigDialog* getConfigDialog() {return pConfigDialog; }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


