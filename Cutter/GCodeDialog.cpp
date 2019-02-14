// GCodeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Cutter.h"
#include "GCodeDialog.h"
#include "afxdialogex.h"
#include "resource.h"
#include "GCodeInterpreter.h"
#include "Cutter.h"
#include "sstream"

// CGCodeDialog dialog

IMPLEMENT_DYNAMIC(CGCodeDialog, CDialogEx)

CGCodeDialog::CGCodeDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GCODE, pParent)
{

}

CGCodeDialog::~CGCodeDialog()
{
}

void CGCodeDialog::setInterpreter(GCodeInterpreter * pInterpreter,  Cutter* pCutter)
{
	assert(this);
	assert(pInterpreter);
	assert(pCutter);
	this->pInterpreter = pInterpreter;
	pInterpreter->setContext(this);
	this->pCutter = pCutter;
}

void CGCodeDialog::showError(const std::string & line, size_t where, const std::string & msg)
{
	std::string error(msg);
	error.append(" : ");
	error.append(line.substr(0, where));
	error.append(">>>");
	error.append(line.substr(where));
	errorText.SetWindowTextA(error.c_str());
}

boolean CGCodeDialog::canPause()
{
	return canPauseCheckbox.GetCheck() == BST_CHECKED;
}

void CGCodeDialog::pause()
{
	isPausedCheckbox.SetCheck(BST_CHECKED);
}

void CGCodeDialog::complete()
{
	isPausedCheckbox.SetCheck(BST_UNCHECKED);
	isCompleteCheckbox.SetCheck(BST_CHECKED);
}

void CGCodeDialog::restart()
{
	isPausedCheckbox.SetCheck(BST_UNCHECKED);
	isCompleteCheckbox.SetCheck(BST_UNCHECKED);
}

void CGCodeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAN_PAUSE, canPauseCheckbox);
	DDX_Control(pDX, IDC_PAUSED, isPausedCheckbox);
	DDX_Control(pDX, IDC_CHK_COMPLETE, isCompleteCheckbox);
	DDX_Control(pDX, IDC_LBL_GCODE_ERR, errorText);
	DDX_Control(pDX, IDC_EDT_COMMAND, commandLine);
	DDX_Control(pDX, IDC_LBL_X, lblX);
	DDX_Control(pDX, IDC_LBL_Y, lblY);
	DDX_Control(pDX, IDC_LBL_U, lblU);
	DDX_Control(pDX, IDC_LBL_V, lblV);
	DDX_Control(pDX, IDC_CHK_FEED_ERROR, feedRateError);
	DDX_Control(pDX, IDC_CHK_RELATIVE, relative);
	DDX_Control(pDX, IDC_CHK_INCHES, inches);
	DDX_Control(pDX, IDC_CHK_MIRRORED, mirrored);
	DDX_Control(pDX, IDC_CHK_FAST, fastMove);
	DDX_Control(pDX, IDC_LBL_XOFFSET, xOffset);
	DDX_Control(pDX, IDC_LBL_YOFFSET, yOffset);
	DDX_Control(pDX, IDC_LBL_UOFFSET, uOffset);
	DDX_Control(pDX, IDC_LBL_VOFFSET, vOffset);
}


BEGIN_MESSAGE_MAP(CGCodeDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CGCodeDialog::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BTN_CLEAR_FR, &CGCodeDialog::OnBnClickedBtnClearFr)
END_MESSAGE_MAP()


// CGCodeDialog message handlers


void CGCodeDialog::OnBnClickedButtonSend()
{
	CString text;
	commandLine.GetWindowTextA(text);
	std::string line(text.GetString());
	pInterpreter->process(line);

	// Show current position
	Position<double> pos = pCutter->getCurrentPosition();
	std::stringstream ss;
	ss.precision(2);
	ss << std::fixed;
	ss << pos.x;
	lblX.SetWindowTextA(ss.str().c_str());
	ss.str("");
	ss << pos.y;
	lblY.SetWindowTextA(ss.str().c_str());
	ss.str("");
	ss << pos.u;
	lblU.SetWindowTextA(ss.str().c_str());
	ss.str("");
	ss << pos.v;
	lblV.SetWindowTextA(ss.str().c_str());

	// Show current offsets
	Position<double> offsets = pInterpreter->offsets();
	ss.str("");
	ss << offsets.x;
	xOffset.SetWindowTextA(ss.str().c_str());
	ss.str("");
	ss << offsets.y;
	yOffset.SetWindowTextA(ss.str().c_str());
	ss.str("");
	ss << offsets.u;
	uOffset.SetWindowTextA(ss.str().c_str());
	ss.str("");
	ss << offsets.v;
	vOffset.SetWindowTextA(ss.str().c_str());

	// Show any feed rate error
	feedRateError.SetCheck(pCutter->hasFeedRateError() ? BST_CHECKED : BST_UNCHECKED);

	// General status
	inches.SetCheck(pInterpreter->isInches() ? BST_CHECKED : BST_UNCHECKED);
	relative.SetCheck(pInterpreter->isRelative() ? BST_CHECKED : BST_UNCHECKED);
	fastMove.SetCheck(pInterpreter->isFast() ? BST_CHECKED : BST_UNCHECKED);
	mirrored.SetCheck(pInterpreter->isMirrored() ? BST_CHECKED : BST_UNCHECKED);
}






void CGCodeDialog::OnBnClickedBtnClearFr()
{
	pCutter->resetFeedRateError();
	feedRateError.SetCheck((pCutter->hasFeedRateError() ? BST_CHECKED : BST_UNCHECKED));
}


