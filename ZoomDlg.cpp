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
// ZoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "aerofoil.h"
#include "ZoomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog


CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
	, zoom(1)
	, fit(false)
{
	//{{AFX_DATA_INIT(CZoomDlg)
	m_zoomIndex = 2;
	//}}AFX_DATA_INIT
}


void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomDlg)
	DDX_Radio(pDX, IDC_RADZOOM1000, m_zoomIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
	//{{AFX_MSG_MAP(CZoomDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg message handlers


void CZoomDlg::OnOK()
{
	if (UpdateData()) {
		fit = false;
		switch (m_zoomIndex)
		{
		case 0:	zoom = 10;		break;
		case 1: zoom = 2;		break;
		case 2: zoom = 1;	break;
		case 3: zoom = 0.5;	break;
		case 4: zoom = 0.25; break;
		default:
			zoom = -1;
			fit = true;
			break;
		}
	}

	CDialog::OnOK();
}
