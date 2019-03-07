/* Aerofoil
Aerofoil plotting and CNC cutter driver
Copyright(C) 1995-2019 R Bruce Porteous

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

// GCodeEditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "GCodeEditDialog.h"
#include "afxdialogex.h"
#include "Kernel/GCodeSnippet.h"


// GCodeEditDialog dialog

IMPLEMENT_DYNAMIC(GCodeEditDialog, CDialogEx)

GCodeEditDialog::GCodeEditDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GCODE_EDIT, pParent)
	, programText(_T(""))
	, editError(_T(""))
{

}

GCodeEditDialog::~GCodeEditDialog()
{
}

void GCodeEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_GCODE, editGCode);
	DDX_Text(pDX, IDC_EDT_GCODE, programText);
	DDX_Text(pDX, IDC_EDT_ERROR, editError);
}


BEGIN_MESSAGE_MAP(GCodeEditDialog, CDialogEx)
END_MESSAGE_MAP()


// GCodeEditDialog message handlers


void GCodeEditDialog::OnOK()
{
	if (UpdateData())
	{
		editError = "";
		GCodeSnippet snippet(programText);
		snippet.validateProgram();
		if (snippet.hasErrors()) {
			editError = snippet.getFirstError().c_str();
			UpdateData(FALSE);
		} else {
			CDialogEx::OnOK();
		}

	}

}
