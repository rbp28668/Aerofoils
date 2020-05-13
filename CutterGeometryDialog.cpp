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

CutterGeometryDialog::CutterGeometryDialog(CutterGeometry* geometry, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CUTTER_GEOMETRY, pParent)
	, pGeometry(geometry)
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
	DDV_MinMaxDouble(pDX, block_width, 0, cutter_width-block_lhs);
	

#ifndef NDEBUG
	if (pDX->m_bSaveAndValidate) {
		assert(block_lhs >= 0);
		assert(block_lhs <= cutter_width);
		assert(block_lhs + block_width <= cutter_width);
	}
#endif

}


BEGIN_MESSAGE_MAP(CutterGeometryDialog, CDialogEx)
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
