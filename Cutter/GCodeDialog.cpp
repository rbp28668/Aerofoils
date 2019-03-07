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
// GCodeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GCodeDialog.h"
#include "afxdialogex.h"
#include "resource.h"
#include "sstream"
#include "fstream"
#include "../Kernel/Cutter.h"
#include "../Kernel/GCodeInterpreter.h"
#include "../Kernel/GCodeProgram.h"
#include "../Kernel/Cutter.h"

// CGCodeDialog dialog

IMPLEMENT_DYNAMIC(CGCodeDialog, CDialogEx)

void CGCodeDialog::getProgramFromEdit()
{
	pProgram->clear();
	CString text;
	programEditor.GetWindowTextA(text);
	std::string textStr(text);
	std::istringstream is(textStr);
	pProgram->load(is);
}

void CGCodeDialog::showData() {

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

	// show next line
	if (pProgram->isRunning() && !pProgram->isComplete()) {
		currentLine.SetWindowTextA(pProgram->nextLine().c_str());
		programEditor.EnableWindow(false);
	} else {
		currentLine.SetWindowTextA("");
		programEditor.EnableWindow(true);
	}

	
}


void CGCodeDialog::updateButtons()
{

	isPausedCheckbox.SetCheck(pProgram->isPaused() ? BST_CHECKED : BST_UNCHECKED);
	isCompleteCheckbox.SetCheck(pProgram->isComplete() ? BST_CHECKED : BST_UNCHECKED);

	startButton.EnableWindow(!pProgram->isRunning() && !pProgram->isComplete());
	
	runButton.EnableWindow(!pProgram->isComplete());

	if (pProgram->isPaused() && !pProgram->isComplete()) {
		runButton.SetWindowTextA("Continue");
	}
	if (pProgram->isComplete()) {
		runButton.SetWindowTextA("Run");
	}

	stepButton.EnableWindow(pProgram->isPaused() && !pProgram->isComplete());
	restartButton.EnableWindow(pProgram->isComplete());

	showData();
}

CGCodeDialog::CGCodeDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GCODE, pParent)
{

}

CGCodeDialog::~CGCodeDialog()
{
}

void CGCodeDialog::setModelObjects(GCodeInterpreter* pInterpreter, GCodeProgram* pProgram, Cutter* pCutter)
{
	assert(this);
	assert(pInterpreter);
	assert(pCutter);
	assert(pProgram);
	this->pProgram = pProgram;
	this->pInterpreter = pInterpreter;
	this->pCutter = pCutter;

	// want program to forward interpreter events to this dialog
	pProgram->setUpstreamContext(this); 
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

bool CGCodeDialog::canPause()
{
	return canPauseCheckbox.GetCheck() == BST_CHECKED;
}

void CGCodeDialog::pause()
{
	isPausedCheckbox.SetCheck(BST_CHECKED);
	updateButtons();
}

void CGCodeDialog::complete()
{
	isPausedCheckbox.SetCheck(BST_UNCHECKED);
	isCompleteCheckbox.SetCheck(BST_CHECKED);
	updateButtons();
}

void CGCodeDialog::restart()
{
	isPausedCheckbox.SetCheck(BST_UNCHECKED);
	isCompleteCheckbox.SetCheck(BST_UNCHECKED);
	updateButtons();
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
	DDX_Control(pDX, IDC_EDT_PROGRAM, programEditor);
	DDX_Control(pDX, IDC_BTN_RUN, runButton);
	DDX_Control(pDX, IDC_BTN_START, startButton);
	DDX_Control(pDX, IDC_BTN_STEP, stepButton);
	DDX_Control(pDX, IDC_BTN_RESTART, restartButton);
	DDX_Control(pDX, IDC_EDT_CURRENT_LINE, currentLine);
}


BEGIN_MESSAGE_MAP(CGCodeDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CGCodeDialog::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BTN_CLEAR_FR, &CGCodeDialog::OnBnClickedBtnClearFr)
	ON_BN_CLICKED(IDC_BTN_RUN, &CGCodeDialog::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_BTN_START, &CGCodeDialog::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STEP, &CGCodeDialog::OnBnClickedBtnStep)
	ON_BN_CLICKED(IDC_BTN_RESTART, &CGCodeDialog::OnBnClickedBtnRestart)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CGCodeDialog::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CGCodeDialog::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CGCodeDialog::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CGCodeDialog message handlers



void CGCodeDialog::OnBnClickedButtonSend()
{
	CString text;
	commandLine.GetWindowTextA(text);
	std::string line(text.GetString());
	pInterpreter->process(line);
	showData();
}


void CGCodeDialog::OnBnClickedBtnClearFr()
{
	pCutter->resetFeedRateError();
	feedRateError.SetCheck((pCutter->hasFeedRateError() ? BST_CHECKED : BST_UNCHECKED));
}

void CGCodeDialog::OnBnClickedBtnRun()
{
	if (!pProgram->isRunning()) {  
		getProgramFromEdit();
		pProgram->start();
		updateButtons();
	}
	pProgram->unpause();

	BOOL bDoingBackgroundProcessing = TRUE;
	while (bDoingBackgroundProcessing)	{
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage()){
				bDoingBackgroundProcessing = FALSE;
				::PostQuitMessage(0);
				break;
			}
		}
		// let MFC do its idle processing
		LONG lIdle = 0;
		while (AfxGetApp()->OnIdle(lIdle++))
			;
		
		// Step program - stop if paused or complete
		pProgram->step();
		bDoingBackgroundProcessing = !(pProgram->isComplete() || pProgram->isPaused());
	}
	updateButtons();
}


void CGCodeDialog::OnBnClickedBtnStart()
{
	isStepping = true;
	getProgramFromEdit();
	pProgram->start();
	updateButtons();
}


void CGCodeDialog::OnBnClickedBtnStep()
{
	if (!pProgram->isComplete()) {
		pProgram->step();
	}
	updateButtons();
}


void CGCodeDialog::OnBnClickedBtnRestart()
{
	pProgram->reset();
	updateButtons();
}


void CGCodeDialog::OnBnClickedBtnClear()
{
	programEditor.Clear();
	pProgram->clear();
	updateButtons();
	programEditor.SetWindowTextA("");
}


void CGCodeDialog::OnBnClickedBtnLoad()
{
	CFileDialog dlg(true
		, ".gcode"
		, 0
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, "G-Code Files(*.gcode) |*.gcode;*.g|All Files(*.*) |*.*||"
	); 
	if (dlg.DoModal() == IDOK)	{
		CString pathName = dlg.GetPathName();
		std::ifstream ifs(pathName);
		pProgram->clear();
		pProgram->load(ifs);
		ifs.close();

		std::string text;
		pProgram->asString(text);
		programEditor.SetWindowTextA(text.c_str());
	}


}


void CGCodeDialog::OnBnClickedBtnSave()
{
	CFileDialog dlg(false
	    , ".gcode"
		, 0
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, "G-Code Files(*.gcode) |*.gcode;*.g| All Files(*.*) |*.*||"
	);
	if (dlg.DoModal() == IDOK) {
		getProgramFromEdit();
		CString pathName = dlg.GetPathName();
		std::ofstream ofs(pathName);
		pProgram->save(ofs);
		ofs.close();
	}
}


BOOL CGCodeDialog::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
