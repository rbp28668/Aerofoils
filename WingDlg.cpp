// WingDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Aerofoil.h"
#include "WingDlg.h"
#include "SectionDlg.h"
#include "SparDlg.h"

#include "kernel/Wing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWingDlg dialog


CWingDlg::CWingDlg(CWing* w,CWnd* pParent /*=NULL*/)
	: CDialog(CWingDlg::IDD, pParent)
	, wing(w)
	, wingCopy(*w)
{
	//{{AFX_DATA_INIT(CWingDlg)
	m_LE = 0.0f;
	m_span = 0.0f;
	m_TE = 0.0f;
	m_fSkin = 0.0f;
	//}}AFX_DATA_INIT
}


void CWingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWingDlg)
	DDX_Control(pDX, IDC_LST_SPARS, m_lstSpars);
	DDX_Control(pDX, IDC_LBL_TIP_INFO, m_lblTipInfo);
	DDX_Control(pDX, IDC_LBL_ROOT_INFO, m_lblRootInfo);
	DDX_Text(pDX, IDC_EDT_LE, m_LE);
	DDV_MinMaxFloat(pDX, m_LE, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDT_SPAN, m_span);
	DDV_MinMaxFloat(pDX, m_span, 0.f, 10000.f);
	DDX_Text(pDX, IDC_EDT_TE, m_TE);
	DDV_MinMaxFloat(pDX, m_TE, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDT_SKIN, m_fSkin);
	DDV_MinMaxFloat(pDX, m_fSkin, 0.f, 10.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWingDlg, CDialog)
	//{{AFX_MSG_MAP(CWingDlg)
	ON_BN_CLICKED(IDC_CMD_ADD_SPAR, OnCmdAddSpar)
	ON_BN_CLICKED(IDC_CMD_ROOT_EDIT, OnCmdRootEdit)
	ON_BN_CLICKED(IDC_CMD_SPAR_DELETE, OnCmdSparDelete)
	ON_BN_CLICKED(IDC_CMD_SPAR_EDIT, OnCmdSparEdit)
	ON_BN_CLICKED(IDC_CMD_TIP_EDIT, OnCmdTipEdit)
	ON_LBN_SELCHANGE(IDC_LST_SPARS, OnSelchangeLstSpars)
	ON_LBN_DBLCLK(IDC_LST_SPARS, OnDblclkLstSpars)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWingDlg::addSparToList(CSpar* ps)
{
	assert(this);
	assert(ps);

	int idx = m_lstSpars.AddString(ps->getText());
	if(idx != LB_ERR)
		m_lstSpars.SetItemDataPtr(idx, ps);

}

/////////////////////////////////////////////////////////////////////////////
// CWingDlg message handlers

void CWingDlg::OnCmdRootEdit() 
{
	CSectionDlg dlg(wingCopy.getRoot(), wingCopy.getRootTransform());
	if(dlg.DoModal())
		m_lblRootInfo.SetWindowText(wingCopy.getRoot()->getName().c_str());
}

void CWingDlg::OnCmdTipEdit() 
{
	CSectionDlg dlg(wingCopy.getTip(), wingCopy.getTipTransform());
	if(dlg.DoModal())
		m_lblTipInfo.SetWindowText(wingCopy.getTip()->getName().c_str());
}


void CWingDlg::OnCmdAddSpar() 
{
	CSpar spar;
	CSparDlg dlg(&spar);
	if(dlg.DoModal())
	{
		CSpar* ps = wingCopy.addSpar(spar);	
		addSparToList(ps);
	}
}

void CWingDlg::OnCmdSparDelete() 
{
	int idx = m_lstSpars.GetCurSel();
	if(idx != LB_ERR)
	{
		// MessageBox()
		CSpar* ps = (CSpar*)m_lstSpars.GetItemDataPtr(idx);
		m_lstSpars.DeleteString(idx);

		wingCopy.deleteSpar(ps);
	}

}

void CWingDlg::OnCmdSparEdit() 
{
	int idx = m_lstSpars.GetCurSel();
	if(idx != LB_ERR)
	{
		CSpar* ps = (CSpar*)m_lstSpars.GetItemDataPtr(idx);
		CSparDlg dlg(ps);
		if(dlg.DoModal())
		{
			m_lstSpars.DeleteString(idx);
			m_lstSpars.InsertString(idx, ps->getText());
			m_lstSpars.SetItemDataPtr(idx, ps);
		}
	}
}


void CWingDlg::OnSelchangeLstSpars() 
{
	// TODO: Add your control notification handler code here
	
}

void CWingDlg::OnDblclkLstSpars() 
{
	OnCmdSparEdit();
	
}

void CWingDlg::OnOK() 
{

	if(UpdateData())
	{
		wingCopy.setSpan(m_span);
		wingCopy.setLE(m_LE);
		wingCopy.setTE(m_TE);
		wingCopy.setSkinThickness(m_fSkin);
		*wing = wingCopy;
		CDialog::OnOK();
	}
}

BOOL CWingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_span = wingCopy.getSpan();
	m_LE = wingCopy.getLE();
	m_TE = wingCopy.getTE();
	m_fSkin = wingCopy.getSkinThickness();

	m_lblRootInfo.SetWindowText(wingCopy.getRoot()->getName().c_str());
	m_lblTipInfo.SetWindowText(wingCopy.getTip()->getName().c_str());
	
	for(int i=0; i<wingCopy.getSparCount(); ++i)
	{
		CSpar* spar = wingCopy.getSpar(i);
		addSparToList(spar);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
