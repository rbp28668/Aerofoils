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

// CutoutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "CutoutDialog.h"
#include "afxdialogex.h"
#include "Kernel/Cutout.h"

// CutoutDialog dialog

IMPLEMENT_DYNAMIC(CutoutDialog, CDialogEx)

CutoutDialog::CutoutDialog(Cutout* pCutout, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_CUTOUT, pParent)
	, pCutout(pCutout)
	, top_surface(FALSE)
	, centre(FALSE)
	, root_x(0)
	, root_y(0)
	, root_width(0)
	, root_height(0)
	, tip_x(0)
	, tip_y(0)
	, tip_width(0)
	, tip_height(0)
{

}

CutoutDialog::~CutoutDialog()
{
}

void CutoutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_TOP_SURFACE, top_surface);
	DDX_Check(pDX, IDC_CHK_CENTRE, centre);
	DDX_Text(pDX, IDC_EDT_ROOT_X, root_x);
	DDX_Text(pDX, IDC_EDT_ROOT_Y, root_y);
	DDX_Text(pDX, IDC_EDT_ROOT_WIDTH, root_width);
	DDX_Text(pDX, IDC_EDT_ROOT_HEIGHT, root_height);
	DDX_Text(pDX, IDC_EDT_TIP_X, tip_x);
	DDX_Text(pDX, IDC_EDT_TIP_Y, tip_y);
	DDX_Text(pDX, IDC_EDT_TIP_WIDTH, tip_width);
	DDX_Text(pDX, IDC_EDT_TIP_HEIGHT, tip_height);
}


BEGIN_MESSAGE_MAP(CutoutDialog, CDialogEx)
	ON_BN_CLICKED(ID_COPY, &CutoutDialog::OnBnClickedCopy)
END_MESSAGE_MAP()


// CutoutDialog message handlers




void CutoutDialog::OnBnClickedCopy()
{
	if (UpdateData())
	{
		tip_x = root_x;
		tip_y = root_y;
		tip_height = root_height;
		tip_width = root_width;
		UpdateData(FALSE);
	}
}


BOOL CutoutDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	top_surface = pCutout->getTopSurface() ? TRUE : FALSE;
	centre = pCutout->getIsCentred() ? TRUE : FALSE;
	root_x = (float)pCutout->getRootX();
	tip_x = (float)pCutout->getTipX();
	root_y = (float)pCutout->getRootY();
	tip_y = (float)pCutout->getTipY();
	root_width = (float)pCutout->getRootWidth();
	tip_width = (float)pCutout->getTipWidth();
	root_height = (float)pCutout->getRootHeight();
	tip_height = (float)pCutout->getTipHeight();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CutoutDialog::OnOK()
{
	if (UpdateData())
	{
		pCutout->setTopSurface(top_surface == TRUE);
		pCutout->setCentred(centre == TRUE);
		pCutout->setRootX(root_x);
		pCutout->setTipX(tip_x);
		pCutout->setRootY(root_y);
		pCutout->setTipY(tip_y);
		pCutout->setRootWidth(root_width);
		pCutout->setTipWidth(tip_width);
		pCutout->setRootHeight(root_height);
		pCutout->setTipHeight(tip_height);

	}

	CDialogEx::OnOK();
}
