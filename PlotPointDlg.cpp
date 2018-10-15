// PlotPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "aerofoil.h"
#include "PlotPointDlg.h"

#include "kernel/PointStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CPlotPointDlg dialog


CPlotPointDlg::CPlotPointDlg(CPointStructure* ppp, CWnd* pParent /*=NULL*/)
	: CDialog(CPlotPointDlg::IDD, pParent)
	, point(ppp)
{
	assert(this);
	assert(ppp);

	//{{AFX_DATA_INIT(CPlotPointDlg)
	m_rootX = 0.0f;
	m_rootY = 0.0f;
	m_span = 0.0f;
	m_tipX = 0.0f;
	m_tipY = 0.0f;
	//}}AFX_DATA_INIT
}


void CPlotPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotPointDlg)
	DDX_Text(pDX, IDC_EDT_ROOT_X, m_rootX);
	DDX_Text(pDX, IDC_EDT_ROOT_Y, m_rootY);
	DDX_Text(pDX, IDC_EDT_SPAN, m_span);
	DDX_Text(pDX, IDC_EDT_TIP_X, m_tipX);
	DDX_Text(pDX, IDC_EDT_TIP_Y, m_tipY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotPointDlg, CDialog)
	//{{AFX_MSG_MAP(CPlotPointDlg)
	ON_BN_CLICKED(IDC_CMD_COPY, OnCmdCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotPointDlg message handlers

void CPlotPointDlg::OnOK() 
{
	if(UpdateData())
	{
		point->setRoot(PointT(m_rootX, m_rootY));
		point->setTip(PointT(m_tipX, m_tipY));
		point->setSpan(m_span);
		CDialog::OnOK();
	}
}

BOOL CPlotPointDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_rootX = point->getRoot().fx;
	m_rootY  = point->getRoot().fy;
	m_tipX = point->getTip().fx;
	m_tipY = point->getTip().fy;

	m_span = point->getSpan();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlotPointDlg::OnCmdCopy() 
{
	if(UpdateData())
	{
		m_tipX = m_rootX;
		m_tipY = m_rootY;
		UpdateData(FALSE);
	}
}
