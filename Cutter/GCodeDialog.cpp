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
#include <assert.h>
#include "sstream"
#include "fstream"
#include "GCodeDialog.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CutterConfig.h"
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

	// Show feed rate and any feed rate error
	ss.str("");
	ss << pCutter->getFeedRate();
	feedRateDisplay.SetWindowTextA(ss.str().c_str());
	feedRateError.SetCheck(pCutter->hasFeedRateError() ? BST_CHECKED : BST_UNCHECKED);

	// General status
	inches.SetCheck(pInterpreter->isInches() ? BST_CHECKED : BST_UNCHECKED);
	relative.SetCheck(pInterpreter->isRelative() ? BST_CHECKED : BST_UNCHECKED);
	fastMove.SetCheck(pInterpreter->isFast() ? BST_CHECKED : BST_UNCHECKED);
	mirrored.SetCheck(pInterpreter->isMirrored() ? BST_CHECKED : BST_UNCHECKED);

	// show next line
	if (pProgram->isRunning() && !pProgram->isComplete()) {
		programEditor.EnableWindow(false);
	} else {
		programEditor.EnableWindow(true);
	}

	// TODO update status buttons on main dialog. (see hardware dialog)
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

void CGCodeDialog::sendAxisCommand(const char* cmd)
{
	std::ostringstream os;
	CString value;

	os << cmd << " ";

	xValue.GetWindowTextA(value);
	value.Trim();
	if (value.GetLength() > 0) {
		os << "X" << value.GetBuffer() << " ";
	}

	yValue.GetWindowTextA(value);
	value.Trim();
	if (value.GetLength() > 0) {
		os << "Y" << value.GetBuffer() << " ";
	}

	uValue.GetWindowTextA(value);
	value.Trim();
	if (value.GetLength() > 0) {
		os << "U" << value.GetBuffer() << " ";
	}

	vValue.GetWindowTextA(value);
	value.Trim();
	if (value.GetLength() > 0) {
		os << "V" << value.GetBuffer() << " ";
	}

	pInterpreter->process(os.str());
	showData();
}

CGCodeDialog::CGCodeDialog(GCodeInterpreter* pInterpreter, GCodeProgram* pProgram, Cutter* pCutter, CutterConfig* pConfig, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GCODE, pParent)
	, isStepping(false)
	, pCutter(pCutter)
	, pInterpreter(pInterpreter)
	, pMainDialog(0)
	, pProgram(pProgram)
	, pConfig(pConfig)
{
	// want program to forward interpreter events to this dialog
	pProgram->setUpstreamContext(this);

}

CGCodeDialog::~CGCodeDialog()
{
}


void CGCodeDialog::setMainDialog(CCutterDlg * pDlg)
{
	assert(this);
	assert(pDlg);
	pMainDialog = pDlg;
}

void CGCodeDialog::configUpdated(CutterConfig* pConfig)
{
	assert(this);
	assert(pConfig);

	buttonProg1.SetWindowTextA(pConfig->buttons[0].label.c_str());
	buttonProg2.SetWindowTextA(pConfig->buttons[1].label.c_str());
	buttonProg3.SetWindowTextA(pConfig->buttons[2].label.c_str());
	buttonProg4.SetWindowTextA(pConfig->buttons[3].label.c_str());
	buttonProg5.SetWindowTextA(pConfig->buttons[4].label.c_str());
	buttonProg6.SetWindowTextA(pConfig->buttons[5].label.c_str());
	buttonProg7.SetWindowTextA(pConfig->buttons[6].label.c_str());
	buttonProg8.SetWindowTextA(pConfig->buttons[7].label.c_str());
	buttonProg9.SetWindowTextA(pConfig->buttons[8].label.c_str());
	buttonProg10.SetWindowTextA(pConfig->buttons[9].label.c_str());
	buttonProg11.SetWindowTextA(pConfig->buttons[10].label.c_str());
	buttonProg12.SetWindowTextA(pConfig->buttons[11].label.c_str());
	buttonProg13.SetWindowTextA(pConfig->buttons[12].label.c_str());
	buttonProg14.SetWindowTextA(pConfig->buttons[13].label.c_str());
	buttonProg15.SetWindowTextA(pConfig->buttons[14].label.c_str());
	buttonProg16.SetWindowTextA(pConfig->buttons[15].label.c_str());
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

void CGCodeDialog::showLine(const std::string& line)
{
	currentLine.SetWindowTextA(line.c_str());
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
	DDX_Control(pDX, IDC_BTN_STOP, stopButton);
	DDX_Control(pDX, IDC_EDT_X, xValue);
	DDX_Control(pDX, IDC_EDT_Y, yValue);
	DDX_Control(pDX, IDC_EDT_U, uValue);
	DDX_Control(pDX, IDC_EDT_V, vValue);
	DDX_Control(pDX, IDC_LBL_FEED_RATE, feedRateDisplay);
	DDX_Control(pDX, IDC_BTN_PROG_1, buttonProg1);
	DDX_Control(pDX, IDC_BTN_PROG_2, buttonProg2);
	DDX_Control(pDX, IDC_BTN_PROG_3, buttonProg3);
	DDX_Control(pDX, IDC_BTN_PROG_4, buttonProg4);
	DDX_Control(pDX, IDC_BTN_PROG_5, buttonProg5);
	DDX_Control(pDX, IDC_BTN_PROG_6, buttonProg6);
	DDX_Control(pDX, IDC_BTN_PROG_7, buttonProg7);
	DDX_Control(pDX, IDC_BTN_PROG_8, buttonProg8);
	DDX_Control(pDX, IDC_BTN_PROG_9, buttonProg9);
	DDX_Control(pDX, IDC_BTN_PROG_10, buttonProg10);
	DDX_Control(pDX, IDC_BTN_PROG_11, buttonProg11);
	DDX_Control(pDX, IDC_BTN_PROG_12, buttonProg12);
	DDX_Control(pDX, IDC_BTN_PROG_13, buttonProg13);
	DDX_Control(pDX, IDC_BTN_PROG_14, buttonProg14);
	DDX_Control(pDX, IDC_BTN_PROG_15, buttonProg15);
	DDX_Control(pDX, IDC_BTN_PROG_16, buttonProg16);
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
	ON_BN_CLICKED(IDC_BTN_HOME, &CGCodeDialog::OnBnClickedBtnHome)
	ON_BN_CLICKED(IDC_BTN_WIRE_ON, &CGCodeDialog::OnBnClickedBtnWireOn)
	ON_BN_CLICKED(IDC_BTN_WIRE_OFF, &CGCodeDialog::OnBnClickedBtnWireOff)
	ON_BN_CLICKED(IDC_BTN_MOTORS_ON, &CGCodeDialog::OnBnClickedBtnMotorsOn)
	ON_BN_CLICKED(IDC_BTN_MOTORS_OFF, &CGCodeDialog::OnBnClickedBtnMotorsOff)
	ON_BN_CLICKED(IDC_BTN_ABSOLUTE, &CGCodeDialog::OnBnClickedBtnAbsolute)
	ON_BN_CLICKED(IDC_BTN_RELATIVE, &CGCodeDialog::OnBnClickedBtnRelative)
	ON_BN_CLICKED(IDC_BTN_MIRROR, &CGCodeDialog::OnBnClickedBtnMirror)
	ON_BN_CLICKED(IDC_BTN_NORMAL, &CGCodeDialog::OnBnClickedBtnNormal)
	ON_BN_CLICKED(IDC_BTN_STOP, &CGCodeDialog::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_MOVE, &CGCodeDialog::OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_CUT, &CGCodeDialog::OnBnClickedBtnCut)
	ON_BN_CLICKED(IDC_BTN_OFFSET, &CGCodeDialog::OnBnClickedBtnOffset)
	ON_BN_CLICKED(IDC_BTN_CLEAR_OFFSET, &CGCodeDialog::OnBnClickedBtnClearOffset)
	ON_BN_CLICKED(IDC_BTN_PROG_1, &CGCodeDialog::OnBnClickedBtnProg1)
	ON_BN_CLICKED(IDC_BTN_PROG_2, &CGCodeDialog::OnBnClickedBtnProg2)
	ON_BN_CLICKED(IDC_BTN_PROG_3, &CGCodeDialog::OnBnClickedBtnProg3)
	ON_BN_CLICKED(IDC_BTN_PROG_4, &CGCodeDialog::OnBnClickedBtnProg4)
	ON_BN_CLICKED(IDC_BTN_PROG_5, &CGCodeDialog::OnBnClickedBtnProg5)
	ON_BN_CLICKED(IDC_BTN_PROG_6, &CGCodeDialog::OnBnClickedBtnProg6)
	ON_BN_CLICKED(IDC_BTN_PROG_7, &CGCodeDialog::OnBnClickedBtnProg7)
	ON_BN_CLICKED(IDC_BTN_PROG_8, &CGCodeDialog::OnBnClickedBtnProg8)
	ON_BN_CLICKED(IDC_BTN_PROG_9, &CGCodeDialog::OnBnClickedBtnProg9)
	ON_BN_CLICKED(IDC_BTN_PROG_10, &CGCodeDialog::OnBnClickedBtnProg10)
	ON_BN_CLICKED(IDC_BTN_PROG_11, &CGCodeDialog::OnBnClickedBtnProg11)
	ON_BN_CLICKED(IDC_BTN_PROG_12, &CGCodeDialog::OnBnClickedBtnProg12)
	ON_BN_CLICKED(IDC_BTN_PROG_13, &CGCodeDialog::OnBnClickedBtnProg13)
	ON_BN_CLICKED(IDC_BTN_PROG_14, &CGCodeDialog::OnBnClickedBtnProg14)
	ON_BN_CLICKED(IDC_BTN_PROG_15, &CGCodeDialog::OnBnClickedBtnProg15)
	ON_BN_CLICKED(IDC_BTN_PROG_16, &CGCodeDialog::OnBnClickedBtnProg16)
END_MESSAGE_MAP()


// CGCodeDialog message handlers



void CGCodeDialog::OnBnClickedButtonSend()
{
	try {
		CString text;
		commandLine.GetWindowTextA(text);
		std::string line(text.GetString());
		pInterpreter->process(line);
		showData();
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
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

	try {
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

			if (pProgram->isRunning()) {
				pProgram->step();
			}

			bDoingBackgroundProcessing = !(pProgram->isComplete() || pProgram->isPaused());

		}
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
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
	try{
		if (!pProgram->isComplete()) {
			pProgram->step();
		}
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
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
	if (pProgram->isRunning()) {
		AfxMessageBox("Existing program is running", MB_OK | MB_ICONHAND);
		return;
	}

	CFileDialog dlg(true
		, ".gcode"
		, 0
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, "G-Code Files(*.gcode) |*.gcode;*.g|All Files(*.*) |*.*||"
	); 

	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.lpstrTitle = "Select G-Code File";

	if (dlg.DoModal() == IDOK)	{
		CString pathName = dlg.GetPathName();
		std::ifstream ifs(pathName);
		pProgram->clear();
		pProgram->load(ifs);
		ifs.close();

		programUpdated();
	}


}

void CGCodeDialog::programUpdated() {
	std::string text;
	pProgram->asString(text);
	programEditor.SetWindowTextA(text.c_str());

}
void CGCodeDialog::OnBnClickedBtnSave()
{
	CFileDialog dlg(false
	    , ".gcode"
		, 0
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, "G-Code Files(*.gcode) |*.gcode;*.g| All Files(*.*) |*.*||"
	);

	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.lpstrTitle = "Select G-Code Output File";

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

	stopBitmap.LoadBitmap(IDB_STOP);
	HBITMAP hBitmap = (HBITMAP)stopBitmap.GetSafeHandle();
	stopButton.SetBitmap(hBitmap);
	configUpdated(pConfig);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CGCodeDialog::OnBnClickedBtnHome()
{
	try {
		pInterpreter->process("G28");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnWireOn()
{
	try {
		pInterpreter->process("M03");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnWireOff()
{
	try{
		pInterpreter->process("M05");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnMotorsOn()
{
	try {
		pInterpreter->process("M17");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnMotorsOff()
{
	try {
		pInterpreter->process("M18");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnAbsolute()
{
	try {
		pInterpreter->process("G90");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnRelative()
{
	try {
		pInterpreter->process("G91");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnMirror()
{
	try {
		pInterpreter->process("G38");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnNormal()
{
	try { 
		pInterpreter->process("G39");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnStop()
{
	try {
		pProgram->reset(); // stop execution of program so don't send more commands
		pCutter->stop();
		pCutter->wireOff();
		pCutter->disableMotors();
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}


void CGCodeDialog::OnBnClickedBtnMove()
{
	try {
		sendAxisCommand("G00");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
}


void CGCodeDialog::OnBnClickedBtnCut()
{
	try { 
		sendAxisCommand("G01");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
}


void CGCodeDialog::OnBnClickedBtnOffset()
{
	try {
		sendAxisCommand("G52");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
}


void CGCodeDialog::OnBnClickedBtnClearOffset()
{
	try { 
		pInterpreter->process("G53");
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}
	showData();
}

void CGCodeDialog::runProgramButton(int buttonIndex)
{
	assert(this);
	assert(pConfig);
	assert(buttonIndex >= 0 && buttonIndex <= pConfig->BUTTON_COUNT);

	std::string code = pConfig->buttons[buttonIndex].code;

	// Rather than using a proper GCodeProgram we just push the lines to the interpreter
	// one at a time.
	try {
		std::istringstream is(code);
		std::string line;
		while (std::getline(is, line)) {
			pInterpreter->process(line);
		}
	}
	catch (std::exception& e) {
		AfxMessageBox(e.what(), MB_OK | MB_ICONERROR);
	}

}

void CGCodeDialog::OnBnClickedBtnProg1()
{
	runProgramButton(0);
}

void CGCodeDialog::OnBnClickedBtnProg2()
{
	runProgramButton(1);
}

void CGCodeDialog::OnBnClickedBtnProg3()
{
	runProgramButton(2);
}

void CGCodeDialog::OnBnClickedBtnProg4()
{
	runProgramButton(3);
}

void CGCodeDialog::OnBnClickedBtnProg5()
{
	runProgramButton(4);
}

void CGCodeDialog::OnBnClickedBtnProg6()
{
	runProgramButton(5);
}

void CGCodeDialog::OnBnClickedBtnProg7()
{
	runProgramButton(6);
}

void CGCodeDialog::OnBnClickedBtnProg8()
{
	runProgramButton(7);
}

void CGCodeDialog::OnBnClickedBtnProg9()
{
	runProgramButton(8);
}

void CGCodeDialog::OnBnClickedBtnProg10()
{
	runProgramButton(9);
}

void CGCodeDialog::OnBnClickedBtnProg11()
{
	runProgramButton(10);
}

void CGCodeDialog::OnBnClickedBtnProg12()
{
	runProgramButton(11);
}

void CGCodeDialog::OnBnClickedBtnProg13()
{
	runProgramButton(12);
}

void CGCodeDialog::OnBnClickedBtnProg14()
{
	runProgramButton(13);
}

void CGCodeDialog::OnBnClickedBtnProg15()
{
	runProgramButton(14);
}

void CGCodeDialog::OnBnClickedBtnProg16()
{
	runProgramButton(15);
}
