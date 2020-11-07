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
	, m_lower(FALSE)
	, m_markers(FALSE)
	, m_upper(FALSE)
	,m_isExternal(FALSE)
{
	assert(pef);

}


void CEllipseFlagsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_PLOT_LOWER, m_lower);
	DDX_Check(pDX, IDC_CHK_PLOT_MARKERS, m_markers);
	DDX_Check(pDX, IDC_CHK_PLOT_UPPER, m_upper);
	DDX_Check(pDX, IDC_CHK_IS_EXTERNAL, m_isExternal);
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
		flags->is_external = (m_isExternal == TRUE);
		CDialog::OnOK();
	}
}

BOOL CEllipseFlagsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_lower = flags->plot_lower ? TRUE : FALSE;
	m_upper = flags->plot_upper ? TRUE : FALSE;
	m_markers = flags->plot_markers ? TRUE : FALSE;
	m_isExternal = flags->plot_markers ? TRUE : FALSE;
	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
