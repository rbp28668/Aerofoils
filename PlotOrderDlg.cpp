// PlotOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>

#include "aerofoil.h"
#include "PlotOrderDlg.h"
#include "AerofoilDoc.h"

#include "kernel/Plot.h"
#include "kernel/PlotStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CPlotOrderDlg dialog


CPlotOrderDlg::CPlotOrderDlg(CAerofoilDoc* pd, CWnd* pParent /*=NULL*/)
	: CDialog(CPlotOrderDlg::IDD, pParent)
	, pDoc(pd)
{
	Create(CPlotOrderDlg::IDD, pParent);
	
	//{{AFX_DATA_INIT(CPlotOrderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPlotOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotOrderDlg)
	DDX_Control(pDX, IDC_LST_STRUCTURES, m_lstPlots);
	DDX_Control(pDX, IDC_CMD_UP, m_cmdUp);
	DDX_Control(pDX, IDC_CMD_TOP, m_cmdTop);
	DDX_Control(pDX, IDC_CMD_DOWN, m_cmdDown);
	DDX_Control(pDX, IDC_CMD_BOTTOM, m_cmdBottom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotOrderDlg, CDialog)
	//{{AFX_MSG_MAP(CPlotOrderDlg)
	ON_LBN_SELCHANGE(IDC_LST_STRUCTURES, OnSelchangeLstStructures)
	ON_BN_CLICKED(IDC_CMD_BOTTOM, OnCmdBottom)
	ON_BN_CLICKED(IDC_CMD_DOWN, OnCmdDown)
	ON_BN_CLICKED(IDC_CMD_TOP, OnCmdTop)
	ON_BN_CLICKED(IDC_CMD_UP, OnCmdUp)
	ON_LBN_SELCANCEL(IDC_LST_STRUCTURES, OnSelcancelLstStructures)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPlotOrderDlg::setSelection(CPlotStructure* ps)
{
	int nItems = m_lstPlots.GetCount();
	for(int i=0; i<nItems; ++i)
	{
		if(m_lstPlots.GetItemDataPtr(i) == ps)
		{
			m_lstPlots.SetCurSel(i);
			OnSelchangeLstStructures();
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPlotOrderDlg message handlers

BOOL CPlotOrderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CPlot& plot = pDoc->getPlot();

	for( CPlot::PlotStructureIterator iter = plot.getPlotStructures();
	iter != plot.endPlotStructures();
	++iter)
	{
		CPlotStructure* pps = *iter;
		string text = pps->getDescriptiveText();
		int idx = m_lstPlots.AddString(text.c_str());
		if(idx != LB_ERR)
			m_lstPlots.SetItemDataPtr(idx,pps);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlotOrderDlg::PostNcDestroy() 
{
	delete this;
}

void CPlotOrderDlg::OnCancel() 
{
	pDoc->closePlotOrderDialog();
}

void CPlotOrderDlg::OnOK() 
{
	CPlot& plot = pDoc->getPlot();
	int nItems = m_lstPlots.GetCount();
	for(int i=0; i<nItems; ++i)
	{
		CPlotStructure* ps = static_cast<CPlotStructure*>(m_lstPlots.GetItemDataPtr(i));
		plot.moveToEnd(ps);

	}


	pDoc->closePlotOrderDialog();
}

void CPlotOrderDlg::OnSelchangeLstStructures() 
{
	int idx = m_lstPlots.GetCurSel();
	if(idx != LB_ERR)
	{
		CPlotStructure* selected = static_cast<CPlotStructure*>(m_lstPlots.GetItemDataPtr(idx));

		int maxIdx = m_lstPlots.GetCount()-1;

		m_cmdTop.EnableWindow( idx != 0);
		m_cmdUp.EnableWindow( idx != 0);
		m_cmdDown.EnableWindow( idx != maxIdx);
		m_cmdBottom.EnableWindow(idx != maxIdx);

		pDoc->setSelection(selected);
	}
	
}

void CPlotOrderDlg::OnCmdBottom() 
{
	int idx = m_lstPlots.GetCurSel();
	if(idx != LB_ERR)
	{
		CPlotStructure* selected = static_cast<CPlotStructure*>(m_lstPlots.GetItemDataPtr(idx));
		m_lstPlots.DeleteString(idx);
		string text = selected->getDescriptiveText();
		int idx = m_lstPlots.AddString(text.c_str());
		if(idx != LB_ERR)
		{
			m_lstPlots.SetItemDataPtr(idx,selected);
			m_lstPlots.SetCurSel(idx);
		}
	}
}

void CPlotOrderDlg::OnCmdDown() 
{
	int idx = m_lstPlots.GetCurSel();
	assert(idx != m_lstPlots.GetCount()-1);

	if((idx != LB_ERR) && (idx != m_lstPlots.GetCount()-1))
	{
		CPlotStructure* selected = static_cast<CPlotStructure*>(m_lstPlots.GetItemDataPtr(idx));
		m_lstPlots.DeleteString(idx);
		string text = selected->getDescriptiveText();
		int ip = m_lstPlots.InsertString(idx+1,text.c_str());
		if(ip != LB_ERR)
		{
			m_lstPlots.SetItemDataPtr(idx+1,selected);
			m_lstPlots.SetCurSel(idx+1);
		}
	
	}
}

void CPlotOrderDlg::OnCmdTop() 
{
	int idx = m_lstPlots.GetCurSel();
	if(idx != LB_ERR)
	{
		CPlotStructure* selected = static_cast<CPlotStructure*>(m_lstPlots.GetItemDataPtr(idx));
		m_lstPlots.DeleteString(idx);
		string text = selected->getDescriptiveText();
		int idx = m_lstPlots.InsertString(0,text.c_str());
		if(idx != LB_ERR)
		{
			m_lstPlots.SetItemDataPtr(0,selected);
			m_lstPlots.SetCurSel(0);
		}
	}
}

void CPlotOrderDlg::OnCmdUp() 
{
	int idx = m_lstPlots.GetCurSel();
	assert(idx != 0);
	if((idx != LB_ERR) && (idx != 0))
	{
		CPlotStructure* selected = static_cast<CPlotStructure*>(m_lstPlots.GetItemDataPtr(idx));
		m_lstPlots.DeleteString(idx);
		string text = selected->getDescriptiveText();
		int ip = m_lstPlots.InsertString(idx-1,text.c_str());
		if(ip != LB_ERR)
		{
			m_lstPlots.SetItemDataPtr(idx-1,selected);
			m_lstPlots.SetCurSel(idx-1);
		}
	}
}

void CPlotOrderDlg::OnSelcancelLstStructures() 
{
		m_cmdTop.EnableWindow(FALSE);
		m_cmdUp.EnableWindow(FALSE);
		m_cmdDown.EnableWindow(FALSE);
		m_cmdBottom.EnableWindow(FALSE);
}
