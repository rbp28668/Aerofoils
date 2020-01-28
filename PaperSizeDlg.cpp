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
// PaperSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "aerofoil.h"
#include "PaperSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaperSizeDlg dialog


CPaperSizeDlg::CPaperSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaperSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaperSizeDlg)
	m_height = 0.0f;
	m_width = 0.0f;
	m_sizeSelect = -1;
	//}}AFX_DATA_INIT
}


void CPaperSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaperSizeDlg)
	DDX_Control(pDX, IDC_EDT_WIDTH, m_editWidth);
	DDX_Control(pDX, IDC_EDT_HEIGHT, m_editHeight);
	DDX_Text(pDX, IDC_EDT_HEIGHT, m_height);
	DDV_MinMaxFloat(pDX, m_height, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDT_WIDTH, m_width);
	DDV_MinMaxFloat(pDX, m_width, 0.f, 1000.f);
	DDX_Radio(pDX, IDC_RAD_A4, m_sizeSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaperSizeDlg, CDialog)
	//{{AFX_MSG_MAP(CPaperSizeDlg)
	ON_BN_CLICKED(IDC_RAD_CUSTOM, OnRadCustom)
	ON_BN_CLICKED(IDC_RAD_A3, OnRadA3)
	ON_BN_CLICKED(IDC_RAD_A3L, OnRadA3l)
	ON_BN_CLICKED(IDC_RAD_A4, OnRadA4)
	ON_BN_CLICKED(IDC_RAD_A4L, OnRadA4l)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaperSizeDlg message handlers

void CPaperSizeDlg::OnRadCustom() 
{
	m_editWidth.EnableWindow();
	m_editHeight.EnableWindow();
}

void CPaperSizeDlg::OnRadA3() 
{
	m_editWidth.EnableWindow(FALSE);
	m_editHeight.EnableWindow(FALSE);
}

void CPaperSizeDlg::OnRadA3l() 
{
	m_editWidth.EnableWindow(FALSE);
	m_editHeight.EnableWindow(FALSE);
}

void CPaperSizeDlg::OnRadA4() 
{
	m_editWidth.EnableWindow(FALSE);
	m_editHeight.EnableWindow(FALSE);
}

void CPaperSizeDlg::OnRadA4l() 
{
	m_editWidth.EnableWindow(FALSE);
	m_editHeight.EnableWindow(FALSE);
}
