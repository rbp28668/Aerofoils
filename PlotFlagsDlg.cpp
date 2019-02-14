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
// PlotFlagsDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "aerofoil.h"
#include "PlotFlagsDlg.h"
#include "kernel/PlotStructure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotFlagsDlg dialog


CPlotFlagsDlg::CPlotFlagsDlg(CPlotStructure* pps, CWnd* pParent /*=NULL*/)
	: CDialog(CPlotFlagsDlg::IDD, pParent)
	, toEdit(pps)
{
	assert(pps);
	//{{AFX_DATA_INIT(CPlotFlagsDlg)
	m_blInvert = FALSE;
	m_blReflect = FALSE;
	m_rootSide = -1;
	//}}AFX_DATA_INIT
}


void CPlotFlagsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotFlagsDlg)
	DDX_Check(pDX, IDC_CHK_INVERT, m_blInvert);
	DDX_Check(pDX, IDC_CHK_REFLECT, m_blReflect);
	DDX_Radio(pDX, IDC_RAD_LEFT, m_rootSide);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotFlagsDlg, CDialog)
	//{{AFX_MSG_MAP(CPlotFlagsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotFlagsDlg message handlers

BOOL CPlotFlagsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_blInvert = toEdit->isInverted() ? TRUE : FALSE;
	m_blReflect = toEdit->isReflected() ? TRUE : FALSE;
	m_rootSide = toEdit->isRootLeft() ? 0 : 1;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlotFlagsDlg::OnOK() 
{
	if(UpdateData())
	{
		toEdit->setInvert(m_blInvert == TRUE);
		toEdit->setReflect(m_blReflect == TRUE);
		toEdit->setRootSide(m_rootSide == 0);
	}
	CDialog::OnOK();
}
