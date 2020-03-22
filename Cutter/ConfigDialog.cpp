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
#include <sstream>
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

CConfigDialog::CConfigDialog(CutterConfig* pConfig, CNCFoamCutter* pCutter, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONFIG, pParent)
	, config(pConfig)
	, pCutter(pCutter)
	, pMainDialog(0)
{
	assert(pConfig);
	assert(pCutter);
}

CConfigDialog::~CConfigDialog()
{
}

void CConfigDialog::setMainDialog(CCutterDlg* pApp)
{
	assert(this);
	assert(pApp);
	pMainDialog = pApp;
}

void CConfigDialog::configUpdated(CutterConfig* pConfig)
{
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

	DDX_Control(pDX, IDC_LST_BUTTONS, buttonList);
	DDX_Control(pDX, IDC_EDT_CODE, editCode);
	DDX_Control(pDX, IDC_EDT_LABEL, editLabel);
	DDX_Control(pDX, IDC_BTN_UPDATE, buttonUpdate);
}


BEGIN_MESSAGE_MAP(CConfigDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_UPDATE_CUTTER, &CConfigDialog::OnBnClickedBtnUpdateCutter)
	ON_BN_CLICKED(IDC_BTN_SAVE_CONFIG, &CConfigDialog::OnBnClickedBtnSaveConfig)
	ON_BN_CLICKED(IDC_CHK_CONNECT_AUTO, &CConfigDialog::OnBnClickedChkConnectAuto)
	ON_BN_CLICKED(IDC_CHK_LISTEN_AUTO, &CConfigDialog::OnBnClickedChkListenAuto)
	ON_CBN_SELCHANGE(IDC_CMB_DEFAULT_COM, &CConfigDialog::OnCbnSelchangeCmbDefaultCom)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &CConfigDialog::OnBnClickedBtnUpdate)
	ON_LBN_SELCHANGE(IDC_LST_BUTTONS, &CConfigDialog::OnSelchangeLstButtons)
END_MESSAGE_MAP()


// CConfigDialog message handlers


void CConfigDialog::OnBnClickedBtnUpdateCutter()
{
	assert(this);
	assert(pMainDialog);

	if (UpdateData(TRUE)) {
		pCutter->setBlockLeft(config->blockLeft);
		pCutter->setBlockRight(config->blockRight);
		pCutter->setWidth(config->cutterWidth);
		pCutter->setFeedRate(config->defaultFeedRate);

		pMainDialog->configUpdated(config);
	}
}


void CConfigDialog::OnBnClickedBtnSaveConfig()
{
	if (UpdateData(TRUE)) {
		if (!config->save(std::string("cutter_config.xml"))) {
			::MessageBox(0, "Problem", "Unable to save configuration", MB_OK | MB_ICONERROR);
		}
		pMainDialog->configUpdated(config);
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


void CConfigDialog::OnBnClickedBtnUpdate()
{
	assert(this);
	assert(config);

	int sel = buttonList.GetCurSel();
	if (sel != LB_ERR) {
		CString label;
		CString code;
		editLabel.GetWindowTextA(label);
		editCode.GetWindowTextA(code);
		config->buttons[sel].label = label.GetBuffer();
		config->buttons[sel].code = code.GetBuffer();

		// Faff about to set the sel-th entry to the updated label.
		if (label.IsEmpty()) {
			label = "------";
		}
		buttonList.SetRedraw(FALSE);
		buttonList.DeleteString(sel);
		buttonList.InsertString(sel, label);
		buttonList.SetRedraw(TRUE);
		buttonList.UpdateWindow();

		// Will have deselected so disable edit etc.
		editLabel.EnableWindow(FALSE);
		editCode.EnableWindow(FALSE);
		buttonUpdate.EnableWindow(FALSE);

		pMainDialog->configUpdated(config);
	}
}


BOOL CConfigDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ComPortEnumerator cpe;
	std::vector< tstring > ports;
	cpe.detect(ports, 16);

	for (std::vector< tstring >::const_iterator it = ports.begin(); it != ports.end(); ++it) {
		defaultComPort.AddString(it->c_str());
	}

	if (config->load(std::string("cutter_config.xml"))) {


		pMainDialog->configLoaded(config);

		// Make UI controls match settings
		defaultComPort.SelectString(-1, config->defaultComPort.c_str());
		connectAutomatically.SetCheck(config->connectAutomatically ? BST_CHECKED : BST_UNCHECKED);
		listenAutomatically.SetCheck(config->listenAutomatically ? BST_CHECKED : BST_UNCHECKED);
	}

	for (int i = 0; i < config->BUTTON_COUNT; ++i) {
		std::string label = config->buttons[i].label;
		if (label.empty()) {
			label = "------";
		}
		buttonList.AddString(label.c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CConfigDialog::OnSelchangeLstButtons()
{
	int sel = buttonList.GetCurSel();
	if (sel != LB_ERR) {
		editLabel.SetWindowTextA(config->buttons[sel].label.c_str());
		editCode.SetWindowTextA(config->buttons[sel].code.c_str());
		editLabel.EnableWindow(TRUE);
		editCode.EnableWindow(TRUE);
		buttonUpdate.EnableWindow(TRUE);
	}
	else {
		editLabel.EnableWindow(FALSE);
		editCode.EnableWindow(FALSE);
		buttonUpdate.EnableWindow(FALSE);
	}
}
