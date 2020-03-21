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

#include "../Kernel/ParserContext.h"
#include "afxwin.h"

// CGCodeDialog dialog
class GCodeInterpreter;
class GCodeProgram;
class Cutter;
class CCutterDlg;

class CGCodeDialog : public CDialogEx, ParserContext
{
	DECLARE_DYNAMIC(CGCodeDialog)
	GCodeInterpreter* pInterpreter;
	GCodeProgram* pProgram;
	Cutter* pCutter;
	CCutterDlg* pMainDialog;

	bool isStepping;

	void getProgramFromEdit();
	void showData();
	void updateButtons();

public:
	CGCodeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGCodeDialog();

	void setModelObjects(GCodeInterpreter* pInterpreter, GCodeProgram* pProgram,  Cutter* pCutter);
	void setMainDialog(CCutterDlg* pDlg);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GCODE };
#endif

	virtual void showError(const std::string& line, size_t where, const std::string& msg);
	virtual void showLine(const std::string& line);
	virtual bool canPause();
	virtual void pause();
	virtual void complete();
	virtual void restart();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton canPauseCheckbox;
	CButton isPausedCheckbox;
	CButton isCompleteCheckbox;
	CStatic errorText;
	CEdit commandLine;
	CStatic lblX;
	CStatic lblY;
	CStatic lblU;
	CStatic lblV;
	CButton feedRateError;
	CButton relative;
	CButton inches;
	CButton mirrored;
	CButton fastMove;
	CStatic xOffset;
	CStatic yOffset;
	CStatic uOffset;
	CStatic vOffset;
	CEdit programEditor;

	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedBtnClearFr();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStep();
	afx_msg void OnBnClickedBtnRestart();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg	void OnBnClickedBtnSave();
	
	void programUpdated();

	CBitmap stopBitmap;
	CButton runButton;
	CButton startButton;
	CButton stepButton;
	CButton restartButton;
	CEdit currentLine;
	CButton clearButton;
	CButton stopButton;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnWireOn();
	afx_msg void OnBnClickedBtnHome();
	afx_msg void OnBnClickedBtnWireOff();
	afx_msg void OnBnClickedBtnMotorsOn();
	afx_msg void OnBnClickedBtnMotorsOff();
	afx_msg void OnBnClickedBtnAbsolute();
	afx_msg void OnBnClickedBtnRelative();
	afx_msg void OnBnClickedBtnMirror();
	afx_msg void OnBnClickedBtnNormal();
	afx_msg void OnBnClickedBtnStop();
};
