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

// TransformDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "TransformDialog.h"
#include "afxdialogex.h"


// TransformDialog dialog

IMPLEMENT_DYNAMIC(TransformDialog, CDialogEx)

TransformDialog::TransformDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUT_TRANSFORM, pParent)
	, rootXOffset(0)
	, rootYOffset(0)
	, tipXOffset(0)
	, tipYOffset(0)
	, invert(false)
	, reflect(false)
	, rootSide(0)
{

}

TransformDialog::~TransformDialog()
{
}

void TransformDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ROOT_XOFFSET, rootXOffset);
	DDX_Text(pDX, IDC_EDT_ROOT_YOFFSET, rootYOffset);
	DDX_Text(pDX, IDC_EDT_TIP_XOFFSET, tipXOffset);
	DDX_Text(pDX, IDC_EDT_TIP_YOFFSET, tipYOffset);
	DDX_Check(pDX, IDC_CHK_INVERT, invert);
	DDX_Check(pDX, IDC_CHK_REFLECT, reflect);
	DDX_Radio(pDX, IDC_RAD_LEFT, rootSide);
}


BEGIN_MESSAGE_MAP(TransformDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_COPY, &TransformDialog::OnBnClickedBtnCopy)
END_MESSAGE_MAP()


// TransformDialog message handlers

void TransformDialog::OnBnClickedBtnCopy()
{
	if(UpdateData()){
		tipXOffset = rootXOffset;
		tipYOffset = rootYOffset;
		UpdateData(FALSE);
	}
}
