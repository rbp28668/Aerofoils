// SelectWingDlg.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include "aerofoil.h"
#include "SelectWingDlg.h"

#include "kernel\plot.h"
#include "kernel\wing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CSelectWingDlg dialog


CSelectWingDlg::CSelectWingDlg(CPlot* pp, CWnd* pParent /*=NULL*/)
	: CDialog(CSelectWingDlg::IDD, pParent)
	, plot(pp)
	, selected(0)
{
	//{{AFX_DATA_INIT(CSelectWingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectWingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectWingDlg)
	DDX_Control(pDX, IDOK, m_cmdOK);
	DDX_Control(pDX, IDC_LST_WINGS, m_lstWings);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectWingDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectWingDlg)
	ON_LBN_SELCHANGE(IDC_LST_WINGS, OnSelchangeLstWings)
	ON_LBN_SELCANCEL(IDC_LST_WINGS, OnSelcancelLstWings)
	ON_LBN_DBLCLK(IDC_LST_WINGS, OnDblclkLstWings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectWingDlg message handlers

void CSelectWingDlg::OnSelchangeLstWings() 
{
	m_cmdOK.EnableWindow();	
}

void CSelectWingDlg::OnSelcancelLstWings() 
{
	m_cmdOK.EnableWindow(FALSE);	
}

BOOL CSelectWingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for( CPlot::WingIterator iter = plot->getWings();
	iter != plot->endWings();
	++iter)
	{
		CWing* pw = *iter;
		string text = pw->getDescriptiveText();
		int idx = m_lstWings.AddString(text.c_str());
		if(idx != LB_ERR)
			m_lstWings.SetItemDataPtr(idx,pw);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectWingDlg::OnOK() 
{
	int idx = m_lstWings.GetCurSel();
	if(idx != LB_ERR)
		selected = static_cast<CWing*>(m_lstWings.GetItemDataPtr(idx));
	CDialog::OnOK();
}

void CSelectWingDlg::OnDblclkLstWings() 
{
	OnOK();	
}
