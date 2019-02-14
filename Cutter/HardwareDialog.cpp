// HardwareDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Cutter.h"
#include "HardwareDialog.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CutterHardware.h"


// CHardwareDialog dialog

IMPLEMENT_DYNAMIC(CHardwareDialog, CDialogEx)

void CHardwareDialog::showStatus(int status)
{
	std::string msg;

	if (status & CutterHardware::VALID) {
		msg = "OK";
	}
	else {
		msg = "ERR";
	}


	if (status & CutterHardware::EMPTY) {
		msg += " Empty";
	}

	if (status & CutterHardware::FULL) {
		msg += " Full";
	}

	statusText.SetWindowTextA(msg.c_str());


}

CHardwareDialog::CHardwareDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HARDWARE, pParent)
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

void CHardwareDialog::setHardware(CutterHardware * pHardware)
{
	assert(this);
	assert(pHardware);
	pCutter = pHardware;
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

END_MESSAGE_MAP()


// CHardwareDialog message handlers
void CHardwareDialog::OnBnClickedBtnHome()
{
	if (pCutter != 0) {
		int stat = pCutter->home();
		showStatus(stat);
	}
}


void CHardwareDialog::OnBnClickedBtnAbort()
{
	if (pCutter != 0) {
		int stat = pCutter->abort();
		showStatus(stat);
	}
}


void CHardwareDialog::OnBnClickedBtnStatus()
{
	if (pCutter != 0) {
		int stat = pCutter->ping();
		showStatus(stat);
	}
}


void CHardwareDialog::OnBnClickedBtnLine()
{
	if (pCutter != 0) {

		int stat = pCutter->line(lxSteps, lySteps, rxSteps, rySteps, totalSteps);
		showStatus(stat);
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


		int stat = pCutter->step(direction, pulse);
		showStatus(stat);

	}
}


void CHardwareDialog::OnBnClickedBtnWireon()
{
	if (pCutter != 0) {
		int stat = pCutter->wireOn();
		showStatus(stat);
	}
}


void CHardwareDialog::OnBnClickedBtnWireoff()
{
	if (pCutter != 0) {
		int stat = pCutter->wireOff();
		showStatus(stat);
	}
}


void CHardwareDialog::OnBnClickedBtnEnable()
{
	if (pCutter != 0) {
		int stat = pCutter->enable();
		showStatus(stat);
	}
}


void CHardwareDialog::OnBnClickedBtnDisable()
{
	if (pCutter != 0) {
		int stat = pCutter->disable();
		showStatus(stat);
	}
}

