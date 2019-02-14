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
// SectionPositionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "SectionPositionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionPositionDlg dialog


CSectionPositionDlg::CSectionPositionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSectionPositionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSectionPositionDlg)
	m_position = 0.0f;
	//}}AFX_DATA_INIT
	m_maxPosition = 1000.0f;
}


void CSectionPositionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionPositionDlg)
	DDX_Text(pDX, IDC_EDT_POSITION, m_position);
	//}}AFX_DATA_MAP
	DDV_MinMaxFloat(pDX, m_position, 0.f, m_maxPosition);
}


BEGIN_MESSAGE_MAP(CSectionPositionDlg, CDialog)
	//{{AFX_MSG_MAP(CSectionPositionDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionPositionDlg message handlers
