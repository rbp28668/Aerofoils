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

class CutterHardware;
class CCutterDlg;
struct CutterConfig;

class CHardwareDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHardwareDialog)

	CutterHardware* pCutter;
	CCutterDlg* pMainDialog;

	long lxSteps;
	long lySteps;
	long rxSteps;
	long rySteps;
	long totalSteps;

	CButton lxStepDirection;
	CButton lyStepDirection;
	CButton rxStepDirection;
	CButton ryStepDirection;
	CButton lxStepPulse;
	CButton lyStepPulse;
	CButton rxStepPulse;
	CButton ryStepPulse;
	CStatic statusText;
	
	void showStatus(int stat);

public:
	CHardwareDialog(CutterHardware* pCutter, CWnd* pParent = NULL);   // standard constructor
	virtual ~CHardwareDialog();

	void setMainDialog(CCutterDlg* pDlg);
	void configUpdated(CutterConfig* pConfig);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HARDWARE };
#endif

	afx_msg void OnBnClickedBtnHome();
	afx_msg void OnBnClickedBtnAbort();
	afx_msg void OnBnClickedBtnStatus();
	afx_msg void OnBnClickedBtnLine();
	afx_msg void OnBnClickedBtnStep();
	afx_msg void OnBnClickedBtnWireon();
	afx_msg void OnBnClickedBtnWireoff();
	afx_msg void OnBnClickedBtnEnable();
	afx_msg void OnBnClickedBtnDisable();
	afx_msg void OnBnClickedBtnAbortNow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
};
