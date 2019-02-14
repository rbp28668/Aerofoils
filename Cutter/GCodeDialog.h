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
class Cutter;

class CGCodeDialog : public CDialogEx, ParserContext
{
	DECLARE_DYNAMIC(CGCodeDialog)
	GCodeInterpreter* pInterpreter;
	Cutter* pCutter;

public:
	CGCodeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGCodeDialog();

	void setInterpreter(GCodeInterpreter* pInterpreter, Cutter* pCutter);
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
	afx_msg void OnBnClickedButtonSend();
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
	afx_msg void OnBnClickedBtnClearFr();
	CButton relative;
	CButton inches;
	CButton mirrored;
	CButton fastMove;
	CStatic xOffset;
	CStatic yOffset;
	CStatic uOffset;
	CStatic vOffset;
};
