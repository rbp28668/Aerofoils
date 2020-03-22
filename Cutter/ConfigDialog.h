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
#include "afxwin.h"

struct CutterConfig;
class CCutterDlg;
class CNCFoamCutter;

// CConfigDialog dialog

class CConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDialog)
	CCutterDlg* pMainDialog;
	CutterConfig* config;
	CNCFoamCutter* pCutter;

public:
	CConfigDialog(CutterConfig* pConfig, CNCFoamCutter* pCutter, CWnd* pParent = NULL);   
	virtual ~CConfigDialog();

	void setMainDialog(CCutterDlg* pCutter);
	void configUpdated(CutterConfig* pConfig);


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

	CComboBox defaultComPort;
	CButton connectAutomatically;
	CButton listenAutomatically;
	CListBox buttonList;
	CEdit editCode;
	CEdit editLabel;
	CButton buttonUpdate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnUpdateCutter();
	afx_msg void OnBnClickedBtnSaveConfig();
	afx_msg void OnBnClickedChkConnectAuto();
	afx_msg void OnBnClickedChkListenAuto();
	afx_msg void OnCbnSelchangeCmbDefaultCom();
	afx_msg void OnBnClickedBtnUpdate();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLstButtons();
};
