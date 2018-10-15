// BackgroundGridDlg.cpp : implementation file
//

#include "stdafx.h"
#include "aerofoil.h"
#include "BackgroundGridDlg.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackgroundGridDlg dialog


CBackgroundGridDlg::CBackgroundGridDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackgroundGridDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBackgroundGridDlg)
	m_blPlotHoriz = FALSE;
	m_blPlotVert = FALSE;
	m_fHorizDistance = 0.0f;
	m_fVertDistance = 0.0f;
	//}}AFX_DATA_INIT
}


void CBackgroundGridDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackgroundGridDlg)
	DDX_Control(pDX, IDC_EDT_VERT_MM, m_edtVert);
	DDX_Control(pDX, IDC_EDT_HORIZ_MM, m_edtHoriz);
	DDX_Control(pDX, IDC_RAD_VERT_3, m_radVert);
	DDX_Control(pDX, IDC_RAD_HORIZ_3, m_radHoriz);
	DDX_Check(pDX, IDC_CHK_HORIZ, m_blPlotHoriz);
	DDX_Check(pDX, IDC_CHK_VERT, m_blPlotVert);
	DDX_Text(pDX, IDC_EDT_HORIZ_MM, m_fHorizDistance);
	DDV_MinMaxFloat(pDX, m_fHorizDistance, 1.f, 200.f);
	DDX_Text(pDX, IDC_EDT_VERT_MM, m_fVertDistance);
	DDV_MinMaxFloat(pDX, m_fVertDistance, 1.f, 200.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBackgroundGridDlg, CDialog)
	//{{AFX_MSG_MAP(CBackgroundGridDlg)
	ON_BN_CLICKED(IDC_CHK_HORIZ, OnChkHoriz)
	ON_BN_CLICKED(IDC_CHK_VERT, OnChkVert)
	ON_BN_CLICKED(IDC_RAD_HORIZ_3, OnRadHoriz3)
	ON_BN_CLICKED(IDC_RAD_HORIZ_4, OnRadHoriz4)
	ON_BN_CLICKED(IDC_RAD_HORIZ_ANY, OnRadHorizAny)
	ON_BN_CLICKED(IDC_RAD_VERT_3, OnRadVert3)
	ON_BN_CLICKED(IDC_RAD_VERT_4, OnRadVert4)
	ON_BN_CLICKED(IDC_RAD_VERT_ANY, OnRadVertAny)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackgroundGridDlg message handlers

void CBackgroundGridDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CBackgroundGridDlg::OnChkHoriz() 
{
	bool checked = IsDlgButtonChecked(IDC_CHK_HORIZ) > 0;
	GetDlgItem(IDC_RAD_HORIZ_3)->EnableWindow(checked);
	GetDlgItem(IDC_RAD_HORIZ_4)->EnableWindow(checked);
	GetDlgItem(IDC_RAD_HORIZ_ANY)->EnableWindow(checked);

	checked &= (IsDlgButtonChecked(IDC_RAD_HORIZ_ANY) > 0);
	GetDlgItem(IDC_EDT_HORIZ_MM)->EnableWindow(checked);
	UpdateData();

}

void CBackgroundGridDlg::OnChkVert() 
{
	bool checked = IsDlgButtonChecked(IDC_CHK_VERT) > 0;
	GetDlgItem(IDC_RAD_VERT_3)->EnableWindow(checked);
	GetDlgItem(IDC_RAD_VERT_4)->EnableWindow(checked);
	GetDlgItem(IDC_RAD_VERT_ANY)->EnableWindow(checked);
	
	checked &= (IsDlgButtonChecked(IDC_RAD_VERT_ANY) > 0);
	GetDlgItem(IDC_EDT_VERT_MM)->EnableWindow(checked);
	UpdateData();
}


void CBackgroundGridDlg::OnRadHoriz3() 
{
	m_fHorizDistance = 3 * 25.4f;
	m_edtHoriz.EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CBackgroundGridDlg::OnRadHoriz4() 
{
	m_fHorizDistance = 4 * 25.4f;
	m_edtHoriz.EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CBackgroundGridDlg::OnRadHorizAny() 
{
	m_edtHoriz.EnableWindow(TRUE);
}

void CBackgroundGridDlg::OnRadVert3() 
{
	m_fVertDistance = 3 * 25.4f;
	m_edtVert.EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CBackgroundGridDlg::OnRadVert4() 
{
	m_fVertDistance = 4 * 25.4f;
	m_edtVert.EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CBackgroundGridDlg::OnRadVertAny() 
{
	m_edtVert.EnableWindow(TRUE);
}

BOOL CBackgroundGridDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_fHorizDistance == 3 * 25.4f)
	{
		CheckRadioButton(IDC_RAD_HORIZ_3, IDC_RAD_HORIZ_ANY, IDC_RAD_HORIZ_3);
		GetDlgItem(IDC_EDT_HORIZ_MM)->EnableWindow(false);
	} 
	else if (m_fHorizDistance == 4 * 25.4f)
	{
		CheckRadioButton(IDC_RAD_HORIZ_3, IDC_RAD_HORIZ_ANY, IDC_RAD_HORIZ_4);
		GetDlgItem(IDC_EDT_HORIZ_MM)->EnableWindow(false);
	}
	else
	{
		CheckRadioButton(IDC_RAD_HORIZ_3, IDC_RAD_HORIZ_ANY, IDC_RAD_HORIZ_ANY);
		GetDlgItem(IDC_EDT_HORIZ_MM)->EnableWindow(true);
	}
	OnChkHoriz();


	if(m_fVertDistance == 3 * 25.4f)
	{
		CheckRadioButton(IDC_RAD_VERT_3, IDC_RAD_VERT_ANY, IDC_RAD_VERT_3);
		GetDlgItem(IDC_EDT_VERT_MM)->EnableWindow(false);
	} 
	else if (m_fVertDistance == 4 * 25.4f)
	{
		CheckRadioButton(IDC_RAD_VERT_3, IDC_RAD_VERT_ANY, IDC_RAD_VERT_4);
		GetDlgItem(IDC_EDT_VERT_MM)->EnableWindow(false);
	}
	else
	{
		CheckRadioButton(IDC_RAD_VERT_3, IDC_RAD_VERT_ANY, IDC_RAD_VERT_ANY);
		GetDlgItem(IDC_EDT_VERT_MM)->EnableWindow(true);
	}
	OnChkVert();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
