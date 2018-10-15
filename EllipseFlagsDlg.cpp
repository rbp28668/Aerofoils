// EllipseFlagsDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "aerofoil.h"
#include "EllipseFlagsDlg.h"

#include "kernel/EllipseFlags.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEllipseFlagsDlg dialog


CEllipseFlagsDlg::CEllipseFlagsDlg(CEllipseFlags* pef, CWnd* pParent /*=NULL*/)
	: CDialog(CEllipseFlagsDlg::IDD, pParent)
	, flags(pef)
{
	assert(pef);

	//{{AFX_DATA_INIT(CEllipseFlagsDlg)
	m_lower = FALSE;
	m_markers = FALSE;
	m_upper = FALSE;
	//}}AFX_DATA_INIT
}


void CEllipseFlagsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEllipseFlagsDlg)
	DDX_Check(pDX, IDC_CHK_PLOT_LOWER, m_lower);
	DDX_Check(pDX, IDC_CHK_PLOT_MARKERS, m_markers);
	DDX_Check(pDX, IDC_CHK_PLOT_UPPER, m_upper);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEllipseFlagsDlg, CDialog)
	//{{AFX_MSG_MAP(CEllipseFlagsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEllipseFlagsDlg message handlers

void CEllipseFlagsDlg::OnOK() 
{
	if(UpdateData())
	{
		flags->plot_lower = (m_lower == TRUE);
		flags->plot_upper = (m_upper == TRUE);   
		flags->plot_markers = (m_markers == TRUE);
		CDialog::OnOK();
	}
}

BOOL CEllipseFlagsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_lower = flags->plot_lower ? TRUE : FALSE;
	m_upper = flags->plot_upper ? TRUE : FALSE;
	m_markers = flags->plot_markers ? TRUE : FALSE;

	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
