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
// ConfigDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigDialog.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CutterConfig.h"
#include "ComPortEnumerator.h"
#include "CutterDlg.h"
#include "../Kernel/Cutter.h"
#include "CNCFoamCutter.h"

// CConfigDialog dialog

IMPLEMENT_DYNAMIC(CConfigDialog, CDialogEx)

CConfigDialog::CConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONFIG, pParent)
	, config(0)
{
	config = new CutterConfig();

}

CConfigDialog::~CConfigDialog()
{
	delete config;

}

void CConfigDialog::initialize(CCutterDlg * pApp )
{
	this->pCutter = pApp->getCutter();

	ComPortEnumerator cpe;
	std::vector< tstring > ports;
	cpe.detect(ports, 16);

	for (std::vector< tstring >::const_iterator it = ports.begin(); it != ports.end(); ++it) {
		defaultComPort.AddString(it->c_str());
	}

	if (config->load(std::string("cutter_config.xml"))) {

		pApp->configLoaded(config);

		// Make UI controls match settings
		defaultComPort.SelectString(-1, config->defaultComPort.c_str());
		connectAutomatically.SetCheck(config->connectAutomatically ? BST_CHECKED : BST_UNCHECKED);
		listenAutomatically.SetCheck(config->listenAutomatically ? BST_CHECKED : BST_UNCHECKED);
	}

}

void CConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_CUTTER_WIDTH, config->cutterWidth);
	DDV_MinMaxDouble(pDX, config->cutterWidth, 0, 10000);
	DDX_Text(pDX, IDC_EDT_BLOCK_LEFT, config->blockLeft);
	DDV_MinMaxDouble(pDX, config->blockLeft, 0, config->cutterWidth);
	DDX_Text(pDX, IDC_EDT_BLOCK_RIGHT, config->blockRight);
	DDV_MinMaxDouble(pDX, config->blockRight, config->blockLeft, config->cutterWidth);
	DDX_Control(pDX, IDC_CMB_DEFAULT_COM, defaultComPort);
	DDX_Control(pDX, IDC_CHK_CONNECT_AUTO, connectAutomatically);
	DDX_Text(pDX, IDC_EDT_DEFAULT_LISTEN, config->defaultListenPort);
	DDV_MinMaxInt(pDX, config->defaultListenPort, 1, 65535);
	DDX_Control(pDX, IDC_CHK_LISTEN_AUTO, listenAutomatically);
	DDX_Text(pDX, IDC_EDT_DEFAULT_FEED, config->defaultFeedRate);
	DDV_MinMaxDouble(pDX, config->defaultFeedRate, 0, 100);
	DDX_Text(pDX, IDC_EDT_X_LEAD, config->xScrewLead);
	DDV_MinMaxDouble(pDX, config->xScrewLead, 0, 10);
	DDX_Text(pDX, IDC_EDT_Y_LEAD, config->yScrewLead);
	DDV_MinMaxDouble(pDX, config->yScrewLead, 0, 10);
	DDX_Text(pDX, IDC_EDT_X_STEPS_PER_REV, config->xStepsPerRev);
	DDV_MinMaxInt(pDX, config->xStepsPerRev, 0, 1000);
	DDX_Text(pDX, IDC_EDT_Y_STEPS_PER_REV, config->yStepsPerRev);
	DDV_MinMaxInt(pDX, config->yStepsPerRev, 0, 1000);
	DDX_Text(pDX, IDC_EDT_X_MICRO_STEP, config->xMicroStepping);
	DDV_MinMaxInt(pDX, config->xMicroStepping, 0, 64);
	DDX_Text(pDX, IDC_EDT_Y_MICRO_STEP, config->yMicroStepping);
	DDV_MinMaxInt(pDX, config->yMicroStepping, 0, 64);
	DDX_Text(pDX, IDC_EDT_STEP_FREQUENCY, config->stepFrequency);
	DDV_MinMaxDouble(pDX, config->stepFrequency, 0, 10000);
}


BEGIN_MESSAGE_MAP(CConfigDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_UPDATE_CUTTER, &CConfigDialog::OnBnClickedBtnUpdateCutter)
	ON_BN_CLICKED(IDC_BTN_SAVE_CONFIG, &CConfigDialog::OnBnClickedBtnSaveConfig)
	ON_BN_CLICKED(IDC_CHK_CONNECT_AUTO, &CConfigDialog::OnBnClickedChkConnectAuto)
	ON_BN_CLICKED(IDC_CHK_LISTEN_AUTO, &CConfigDialog::OnBnClickedChkListenAuto)
	ON_CBN_SELCHANGE(IDC_CMB_DEFAULT_COM, &CConfigDialog::OnCbnSelchangeCmbDefaultCom)
END_MESSAGE_MAP()


// CConfigDialog message handlers


void CConfigDialog::OnBnClickedBtnUpdateCutter()
{
	if (UpdateData(TRUE)) {
		pCutter->setBlockLeft(config->blockLeft);
		pCutter->setBlockRight(config->blockRight);
		pCutter->setWidth(config->cutterWidth);
		pCutter->setFeedRate(config->defaultFeedRate);
	}
}


void CConfigDialog::OnBnClickedBtnSaveConfig()
{
	if (UpdateData(TRUE)) {
		if (!config->save(std::string("cutter_config.xml"))) {
			::MessageBox(0, "Problem", "Unable to save configuration", MB_OK | MB_ICONERROR);
		}
	}
}


void CConfigDialog::OnBnClickedChkConnectAuto()
{
	config->connectAutomatically = (connectAutomatically.GetCheck() == BST_CHECKED);
}


void CConfigDialog::OnBnClickedChkListenAuto()
{
	config->listenAutomatically = (listenAutomatically.GetCheck() == BST_CHECKED);
}


void CConfigDialog::OnCbnSelchangeCmbDefaultCom()
{
	int idx = defaultComPort.GetCurSel();
	if (idx != CB_ERR) {
		CString value;
		defaultComPort.GetLBText(idx, value);
		config->defaultComPort = value;
	}
}
