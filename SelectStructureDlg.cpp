// SelectStructureDlg.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include "aerofoil.h"
#include "SelectStructureDlg.h"

#include "kernel\plot.h"
#include "kernel\Structure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CSelectStructureDlg dialog


CSelectStructureDlg::CSelectStructureDlg(CPlot* pp, CWnd* pParent /*=NULL*/)
	: CDialog(CSelectStructureDlg::IDD, pParent)
	, plot(pp)
	, selected(0)
{
	//{{AFX_DATA_INIT(CSelectStructureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectStructureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectStructureDlg)
	DDX_Control(pDX, IDOK, m_cmdOK);
	DDX_Control(pDX, IDC_LST_STRUCTURES, m_lstStructures);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectStructureDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectStructureDlg)
	ON_LBN_SELCHANGE(IDC_LST_STRUCTURES, OnSelchangeLstStructures)
	ON_LBN_SELCANCEL(IDC_LST_STRUCTURES, OnSelcancelLstStructures)
	ON_LBN_DBLCLK(IDC_LST_STRUCTURES, OnDblclkLstStructures)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectStructureDlg message handlers

void CSelectStructureDlg::OnSelchangeLstStructures() 
{
	m_cmdOK.EnableWindow();	
}

void CSelectStructureDlg::OnSelcancelLstStructures() 
{
	m_cmdOK.EnableWindow(FALSE);	
}

BOOL CSelectStructureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for( CPlot::StructureIterator iter = plot->getStructures();
	iter != plot->endStructures();
	++iter)
	{
		CStructure* pw = *iter;
		string text = pw->getDescriptiveText();
		int idx = m_lstStructures.AddString(text.c_str());
		if(idx != LB_ERR)
			m_lstStructures.SetItemDataPtr(idx,pw);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectStructureDlg::OnOK() 
{
	int idx = m_lstStructures.GetCurSel();
	if(idx != LB_ERR)
		selected = static_cast<CStructure*>(m_lstStructures.GetItemDataPtr(idx));
	CDialog::OnOK();
}

void CSelectStructureDlg::OnDblclkLstStructures() 
{
	OnOK();	
}
