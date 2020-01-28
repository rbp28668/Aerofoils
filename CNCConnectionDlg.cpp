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
// CNCConnectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "aerofoil.h"
#include "CNCConnectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCNCConnectionDlg dialog


CCNCConnectionDlg::CCNCConnectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCNCConnectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCNCConnectionDlg)
	m_port = 0;
	m_host = _T("");
	//}}AFX_DATA_INIT
}


void CCNCConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCNCConnectionDlg)
	DDX_Text(pDX, IDC_EDT_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 1024, 65535);
	DDX_Text(pDX, IDC_EDT_HOST, m_host);
	DDV_MaxChars(pDX, m_host, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCNCConnectionDlg, CDialog)
	//{{AFX_MSG_MAP(CCNCConnectionDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCNCConnectionDlg message handlers
