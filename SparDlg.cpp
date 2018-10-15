// SparDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>

#include "Aerofoil.h"
#include "SparDlg.h"

#include "kernel/Spar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSparDlg dialog


CSparDlg::CSparDlg(CSpar* ps, CWnd* pParent /*=NULL*/)
	: CDialog(CSparDlg::IDD, pParent)
	, spar(ps)
{
	assert(ps);

	//{{AFX_DATA_INIT(CSparDlg)
	m_blSubmerged = FALSE;
	m_rootHeight = 0.0f;
	m_rootWidth = 0.0f;
	m_rootX = 0.0f;
	m_tipHeight = 0.0f;
	m_tipWidth = 0.0f;
	m_tipX = 0.0f;
	m_typeSelect = -1;
	//}}AFX_DATA_INIT
}


void CSparDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSparDlg)
	DDX_Control(pDX, IDC_EDT_TIP_HEIGHT, m_edtTipHeight);
	DDX_Control(pDX, IDC_EDT_ROOT_HEIGHT, m_edtRootHeight);
	DDX_Check(pDX, IDC_CHK_SUBMERGED, m_blSubmerged);
	DDX_Text(pDX, IDC_EDT_ROOT_HEIGHT, m_rootHeight);
	DDX_Text(pDX, IDC_EDT_ROOT_WIDTH, m_rootWidth);
	DDX_Text(pDX, IDC_EDT_ROOT_X, m_rootX);
	DDX_Text(pDX, IDC_EDT_TIP_HEIGHT, m_tipHeight);
	DDX_Text(pDX, IDC_EDT_TIP_WIDTH, m_tipWidth);
	DDX_Text(pDX, IDC_EDT_TIP_X, m_tipX);
	DDX_Radio(pDX, IDC_RAD_TOP, m_typeSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSparDlg, CDialog)
	//{{AFX_MSG_MAP(CSparDlg)
	ON_BN_CLICKED(ID_COPY, OnCopy)
	ON_BN_CLICKED(IDC_RAD_BOTTOM, OnRadBottom)
	ON_BN_CLICKED(IDC_RAD_FULL_DEPTH, OnRadFullDepth)
	ON_BN_CLICKED(IDC_RAD_TOP, OnRadTop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSparDlg message handlers

void CSparDlg::OnOK() 
{

	if(UpdateData())
	{
		spar->setSubmerged(m_blSubmerged != FALSE);
		spar->setRootHeight(m_rootHeight);
		spar->setRootWidth(m_rootWidth);
		spar->setRootX(m_rootX);
		spar->setTipHeight(m_tipHeight);
		spar->setTipWidth(m_tipWidth);
		spar->setTipX(m_tipX);
		
		switch(m_typeSelect)
		{
		case 0: spar->setType(CSpar::top); break;
		case 1: spar->setType(CSpar::bottom); break;
		case 2: spar->setType(CSpar::full_depth); break;
		default:
			assert(false);
			break;
		}

	}
	CDialog::OnOK();
}

BOOL CSparDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_blSubmerged = (spar->isSubmerged()) ? TRUE : FALSE;
	m_rootHeight = spar->getRootHeight();
	m_rootWidth = spar->getRootWidth();
	m_rootX = spar->getRootX();
	m_tipHeight = spar->getTipHeight();
	m_tipWidth = spar->getTipWidth();
	m_tipX = spar->getTipX();
	
	switch(spar->getType())
	{
	case CSpar::top:
		m_typeSelect = 0;
		break;
	case CSpar::bottom:
		m_typeSelect = 1;
		break;
	case CSpar::full_depth:
		m_typeSelect = 2;
		break;
	default:
		assert(false);
		break;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSparDlg::OnCopy() 
{
	if(UpdateData())
	{
		m_tipX = m_rootX;
		m_tipHeight = m_rootHeight;
		m_tipWidth = m_rootWidth;
		UpdateData(FALSE);
	}
}

void CSparDlg::OnRadBottom() 
{
	m_edtRootHeight.EnableWindow();
	m_edtTipHeight.EnableWindow();
}

void CSparDlg::OnRadFullDepth() 
{
	m_edtRootHeight.EnableWindow(FALSE);
	m_edtTipHeight.EnableWindow(FALSE);
}

void CSparDlg::OnRadTop() 
{
	m_edtRootHeight.EnableWindow();
	m_edtTipHeight.EnableWindow();
}
