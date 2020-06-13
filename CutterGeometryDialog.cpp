
/* Aerofoil
Aerofoil plotting and CNC cutter driver
Kernel / core algorithms
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

// CutterGeometryDialog.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Aerofoil.h"
#include "CutterGeometryDialog.h"
#include "afxdialogex.h"
#include "Kernel/CutterGeometry.h"

// CutterGeometryDialog dialog

IMPLEMENT_DYNAMIC(CutterGeometryDialog, CDialogEx)

CutterGeometryDialog::CutterGeometryDialog(CutterGeometry* geometry, Cut* cut, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CUTTER_GEOMETRY, pParent)
	, copy(*geometry)
	, cutPlanform(cut, &copy)
	, pGeometry(geometry)
	, pCut(cut)
	, x_travel(0)
	, y_travel(0)
	, cutter_width(0)
	, block_lhs(0)
	, block_width(0)
{
	assert(pGeometry);
}


CutterGeometryDialog::~CutterGeometryDialog()
{
}

void CutterGeometryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_XTRAVEL, x_travel);
	DDV_MinMaxDouble(pDX, x_travel, 0, 1000);
	DDX_Text(pDX, IDC_EDT_YTRAVEL, y_travel);
	DDV_MinMaxDouble(pDX, y_travel, 0, 500);
	DDX_Text(pDX, IDC_EDT_WIDTH, cutter_width);
	DDV_MinMaxDouble(pDX, cutter_width, 0, 2000);
	DDX_Text(pDX, IDC_EDT_BLOCKLHS, block_lhs);
	DDV_MinMaxDouble(pDX, block_lhs, 0, cutter_width);
	DDX_Text(pDX, IDC_EDT_BLOCKWIDTH, block_width);
	DDV_MinMaxDouble(pDX, block_width, 0, cutter_width - block_lhs);


#ifndef NDEBUG
	if (pDX->m_bSaveAndValidate) {
		assert(block_lhs >= 0);
		assert(block_lhs <= cutter_width);
		assert(block_lhs + block_width <= cutter_width);
	}
#endif

	DDX_Control(pDX, IDC_PLANFORM_CONTROL, cutPlanform);
}


BEGIN_MESSAGE_MAP(CutterGeometryDialog, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDT_XTRAVEL, &CutterGeometryDialog::OnKillfocusEdtXtravel)
	ON_EN_KILLFOCUS(IDC_EDT_YTRAVEL, &CutterGeometryDialog::OnKillfocusEdtYtravel)
	ON_EN_KILLFOCUS(IDC_EDT_WIDTH, &CutterGeometryDialog::OnKillfocusEdtWidth)
	ON_EN_KILLFOCUS(IDC_EDT_BLOCKLHS, &CutterGeometryDialog::OnKillfocusEdtBlocklhs)
//	ON_EN_CHANGE(IDC_EDT_BLOCKWIDTH, &CutterGeometryDialog::OnChangeEdtBlockwidth)
ON_EN_KILLFOCUS(IDC_EDT_BLOCKWIDTH, &CutterGeometryDialog::OnKillfocusEdtBlockwidth)
END_MESSAGE_MAP()


// CutterGeometryDialog message handlers


void CutterGeometryDialog::OnOK()
{
	if (UpdateData(TRUE)) {
		pGeometry->setWidth(cutter_width);
		pGeometry->setXTravel(x_travel);
		pGeometry->setYTravel(y_travel);
		pGeometry->setBlockLeft(block_lhs);
		pGeometry->setBlockRight(block_lhs + block_width);
	}

	CDialogEx::OnOK();
}


BOOL CutterGeometryDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	cutter_width = pGeometry->getWidth();
	x_travel = pGeometry->getXTravel();
	y_travel = pGeometry->getYTravel();
	block_lhs = pGeometry->getBlockLeft();
	block_width = pGeometry->getBlockRight() - pGeometry->getBlockLeft();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CutterGeometryDialog::OnKillfocusEdtXtravel()
{
	if (UpdateData()) {
		copy.setXTravel(x_travel);
		cutPlanform.RedrawWindow();
	}
}


void CutterGeometryDialog::OnKillfocusEdtYtravel()
{
	if (UpdateData()) {
		copy.setYTravel(y_travel);
		cutPlanform.RedrawWindow();
	}
}


void CutterGeometryDialog::OnKillfocusEdtWidth()
{
	if (UpdateData()) {
		copy.setWidth(cutter_width);
		cutPlanform.RedrawWindow();
	}

}


void CutterGeometryDialog::OnKillfocusEdtBlocklhs()
{
	if (UpdateData()) {
		copy.setBlockLeft(block_lhs);
		copy.setBlockRight(block_lhs + block_width);
		cutPlanform.RedrawWindow();
	}
}


void CutterGeometryDialog::OnKillfocusEdtBlockwidth()
{
	if (UpdateData()) {
		copy.setBlockRight(block_lhs + block_width);
		cutPlanform.RedrawWindow();
	}
}
