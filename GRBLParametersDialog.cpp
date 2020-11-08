// GRBLParametersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "GRBLParametersDialog.h"
#include "afxdialogex.h"


// GRBLParametersDialog dialog

IMPLEMENT_DYNAMIC(GRBLParametersDialog, CDialogEx)

GRBLParametersDialog::GRBLParametersDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRBL_PARAMETERS, pParent)
	, feedRate(0)
	, laserPower(0)
	, dynamicLaserPower(FALSE)
{

}

GRBLParametersDialog::~GRBLParametersDialog()
{
}

void GRBLParametersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_FEED_RATE, feedRate);
	DDV_MinMaxDouble(pDX, feedRate, 100, 5000);
	DDX_Text(pDX, IDC_EDT_LASER_POWER, laserPower);
	DDV_MinMaxDouble(pDX, laserPower, 0, 1000);
	DDX_Check(pDX, IDC_CHK_DYNAMIC_LASER, dynamicLaserPower);
}


BEGIN_MESSAGE_MAP(GRBLParametersDialog, CDialogEx)
END_MESSAGE_MAP()


// GRBLParametersDialog message handlers
