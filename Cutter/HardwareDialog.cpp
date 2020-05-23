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
// HardwareDialog.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "HardwareDialog.h"
#include "CutterDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CutterHardware.h"
#include "../Kernel/Cutter.h"

// CHardwareDialog dialog

IMPLEMENT_DYNAMIC(CHardwareDialog, CDialogEx)

void CHardwareDialog::showStatus(int status)
{
	std::string msg;

	if (status & (int)CutterHardware::StatusT::VALID) {
		msg = "OK";
	}
	else {
		msg = "ERR";
	}


	if (status & (int)CutterHardware::StatusT::EMPTY) {
		msg += " Empty";
	}

	if (status & (int)CutterHardware::StatusT::FULL) {
		msg += " Full";
	}

	statusText.SetWindowTextA(msg.c_str());

	if (pMainDialog) {
		pMainDialog->showLimitSwitches(status);
	}
}

CHardwareDialog::CHardwareDialog(CutterHardware* pCutter, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HARDWARE, pParent)
	, pCutter(pCutter)
	, pMainDialog(0)
	, lxSteps(0)
	, lySteps(0)
	, rxSteps(0)
	, rySteps(0)
	, totalSteps(0)
{

}

CHardwareDialog::~CHardwareDialog()
{
}


void CHardwareDialog::setMainDialog(CCutterDlg * pDlg)
{
	assert(this);
	assert(pDlg);
	pMainDialog = pDlg;
}

void CHardwareDialog::configUpdated(CutterConfig* pConfig)
{
}

void CHardwareDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_LX, lxSteps);
	DDX_Text(pDX, IDC_EDT_LY, lySteps);
	DDX_Text(pDX, IDC_EDT_RX, rxSteps);
	DDX_Text(pDX, IDC_EDT_RY, rySteps);
	DDX_Text(pDX, IDC_EDT_STEPS, totalSteps);
	DDX_Control(pDX, IDC_LBL_STATUS, statusText);
	DDX_Control(pDX, IDC_CHECK_LXDIR, lxStepDirection);
	DDX_Control(pDX, IDC_CHECK_LYDIR, lyStepDirection);
	DDX_Control(pDX, IDC_CHECK_RXDIR, rxStepDirection);
	DDX_Control(pDX, IDC_CHECK_RYDIR, ryStepDirection);
	DDX_Control(pDX, IDC_CHECK_LXPULSE, lxStepPulse);
	DDX_Control(pDX, IDC_CHECK_LYPULSE, lyStepPulse);
	DDX_Control(pDX, IDC_CHECK_RXPULSE, rxStepPulse);
	DDX_Control(pDX, IDC_CHECK_RYPULSE, ryStepPulse);
}


BEGIN_MESSAGE_MAP(CHardwareDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_HOME, &CHardwareDialog::OnBnClickedBtnHome)
	ON_BN_CLICKED(IDC_BTN_ABORT, &CHardwareDialog::OnBnClickedBtnAbort)
	ON_BN_CLICKED(IDC_BTN_STATUS, &CHardwareDialog::OnBnClickedBtnStatus)
	ON_BN_CLICKED(IDC_BTN_LINE, &CHardwareDialog::OnBnClickedBtnLine)
	ON_BN_CLICKED(IDC_BTN_STEP, &CHardwareDialog::OnBnClickedBtnStep)
	ON_BN_CLICKED(IDC_BTN_WIREON, &CHardwareDialog::OnBnClickedBtnWireon)
	ON_BN_CLICKED(IDC_BTN_WIREOFF, &CHardwareDialog::OnBnClickedBtnWireoff)
	ON_BN_CLICKED(IDC_BTN_ENABLE, &CHardwareDialog::OnBnClickedBtnEnable)
	ON_BN_CLICKED(IDC_BTN_DISABLE, &CHardwareDialog::OnBnClickedBtnDisable)
	ON_BN_CLICKED(IDC_BTN_ABORT_NOW, &CHardwareDialog::OnBnClickedBtnAbortNow)
END_MESSAGE_MAP()


// CHardwareDialog message handlers
void CHardwareDialog::OnBnClickedBtnHome()
{
	if (pCutter != 0) {
		try{
			int stat = pCutter->home();
			showStatus(stat);
		}
		catch (HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}


void CHardwareDialog::OnBnClickedBtnAbort()
{
	if (pCutter != 0) {
		try {
			int stat = pCutter->abort();
			showStatus(stat);
		}
		catch (HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}


void CHardwareDialog::OnBnClickedBtnStatus()
{
	if (pCutter != 0) {
		try {
			int stat = pCutter->ping();
			showStatus(stat);
		}
		catch (HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}


void CHardwareDialog::OnBnClickedBtnLine()
{
	if (pCutter != 0) {
		if (UpdateData() == TRUE) {
			try {
				int stat = pCutter->line(totalSteps, lxSteps, lySteps, rxSteps, rySteps);
				showStatus(stat);
			}
			catch (HardwareException& e) {
				AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
			}
		}
	}
}


void CHardwareDialog::OnBnClickedBtnStep()
{
	if (pCutter != 0) {
		int direction = 0;
		if (lxStepDirection.GetCheck() == BST_CHECKED) direction |= 1;
		direction <<= 1;
		if (lyStepDirection.GetCheck() == BST_CHECKED) direction |= 1;
		direction <<= 1;
		if (rxStepDirection.GetCheck() == BST_CHECKED) direction |= 1;
		direction <<= 1;
		if (ryStepDirection.GetCheck() == BST_CHECKED) direction |= 1;

		int pulse = 0;
		if (lxStepPulse.GetCheck() == BST_CHECKED) pulse |= 1;
		pulse <<= 1;
		if (lyStepPulse.GetCheck() == BST_CHECKED) pulse |= 1;
		pulse <<= 1;
		if (rxStepPulse.GetCheck() == BST_CHECKED) pulse |= 1;
		pulse <<= 1;
		if (ryStepPulse.GetCheck() == BST_CHECKED) pulse |= 1;

		try {
			int stat = pCutter->step(direction, pulse);
			showStatus(stat);
		}
		catch (HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}


void CHardwareDialog::OnBnClickedBtnWireon()
{
	if (pCutter != 0) {
		try {
			int stat = pCutter->wireOn();
			showStatus(stat);
		}
		catch (const HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}


void CHardwareDialog::OnBnClickedBtnWireoff()
{
	if (pCutter != 0) {
		try {
			int stat = pCutter->wireOff();
			showStatus(stat);
		}
		catch (const HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}


void CHardwareDialog::OnBnClickedBtnEnable()
{
	if (pCutter != 0) {
		try {
			int stat = pCutter->enable();
			showStatus(stat);
		}
		catch (HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}


void CHardwareDialog::OnBnClickedBtnDisable()
{
	if (pCutter != 0) {
		try {
			int stat = pCutter->disable();
			showStatus(stat);
		}
		catch (HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}

void CHardwareDialog::OnBnClickedBtnAbortNow()
{
	if (pCutter != 0) {
		try {
			int stat = pCutter->abortNow();
			showStatus(stat);
		}
		catch (HardwareException& e) {
			AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
		}
	}
}

