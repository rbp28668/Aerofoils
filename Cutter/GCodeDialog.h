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

#include "ParserContext.h"
#include "afxwin.h"

// CGCodeDialog dialog
class GCodeInterpreter;
class GCodeProgram;
class Cutter;

class CGCodeDialog : public CDialogEx, ParserContext
{
	DECLARE_DYNAMIC(CGCodeDialog)
	GCodeInterpreter* pInterpreter;
	GCodeProgram* pProgram;
	Cutter* pCutter;
	bool isStepping;

	void getProgramFromEdit();
	void showData();
	void updateButtons();

public:
	CGCodeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGCodeDialog();

	void setModelObjects(GCodeInterpreter* pInterpreter, GCodeProgram* pProgram,  Cutter* pCutter);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GCODE };
#endif

	virtual void showError(const std::string& line, size_t where, const std::string& msg);
	virtual boolean canPause();
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
	afx_msg void OnBnClickedBtnSave();
	
	CButton runButton;
	CButton startButton;
	CButton stepButton;
	CButton restartButton;
	virtual BOOL OnInitDialog();
	CEdit currentLine;
	CButton clearButton;
};
