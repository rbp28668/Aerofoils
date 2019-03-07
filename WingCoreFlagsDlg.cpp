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

// WingCoreFlagsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "WingCoreFlagsDlg.h"
#include "afxdialogex.h"


// WingCoreFlagsDlg dialog

IMPLEMENT_DYNAMIC(WingCoreFlagsDlg, CDialogEx)

WingCoreFlagsDlg::WingCoreFlagsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WING_CUT_FLAGS, pParent)
	, cutLeadingEdge(FALSE)
	, cutTrailingEdge(FALSE)
	, cutSpars(FALSE)
{

}

WingCoreFlagsDlg::~WingCoreFlagsDlg()
{
}

void WingCoreFlagsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_CUT_LE, cutLeadingEdge);
	DDX_Check(pDX, IDC_CHK_CUT_TE, cutTrailingEdge);
	DDX_Check(pDX, IDC_CHK_CUT_SPARS, cutSpars);
}


BEGIN_MESSAGE_MAP(WingCoreFlagsDlg, CDialogEx)
END_MESSAGE_MAP()


// WingCoreFlagsDlg message handlers
