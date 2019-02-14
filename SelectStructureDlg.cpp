/* Aerofoil
Aerofoil plotting and CNC cutter driver
Copyright(C) 1995-2019 R Bruce Porteous

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
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
