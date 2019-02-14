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
// EllipsePairDlg.cpp : implementation file
//

#include "stdafx.h"
#include "aerofoil.h"
#include "EllipsePairDlg.h"

#include "kernel/EllipsePair.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEllipsePairDlg dialog


CEllipsePairDlg::CEllipsePairDlg(CEllipsePair* pep, CWnd* pParent /*=NULL*/)
	: CDialog(CEllipsePairDlg::IDD, pParent)
	, ellipses(pep)
{
	//{{AFX_DATA_INIT(CEllipsePairDlg)
	m_firstABottom = 0.0f;
	m_firstATop = 0.0f;
	m_firstB = 0.0f;
	m_firstX = 0.0f;
	m_firstY = 0.0f;
	m_secondX = 0.0f;
	m_secondY = 0.0f;
	m_secondB = 0.0f;
	m_secondABottom = 0.0f;
	m_secondATop = 0.0f;
	//}}AFX_DATA_INIT
}


void CEllipsePairDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEllipsePairDlg)
	DDX_Text(pDX, IDC_EDT_FIRST_A_BOTTOM, m_firstABottom);
	DDX_Text(pDX, IDC_EDT_FIRST_A_TOP, m_firstATop);
	DDX_Text(pDX, IDC_EDT_FIRST_B, m_firstB);
	DDX_Text(pDX, IDC_EDT_FIRST_X, m_firstX);
	DDX_Text(pDX, IDC_EDT_FIRST_Y, m_firstY);
	DDX_Text(pDX, IDC_EDT_SECOND_X, m_secondX);
	DDX_Text(pDX, IDC_EDT_SECOND_Y, m_secondY);
	DDX_Text(pDX, IDC_EDT_SECOND_B, m_secondB);
	DDX_Text(pDX, IDC_EDT_SECOND_A_BOTTOM, m_secondABottom);
	DDX_Text(pDX, IDC_EDT_SECOND_A_TOP, m_secondATop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEllipsePairDlg, CDialog)
	//{{AFX_MSG_MAP(CEllipsePairDlg)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEllipsePairDlg message handlers

BOOL CEllipsePairDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEllipse* first = ellipses->getFirst();
	m_firstABottom= first->getABottom();
	m_firstATop= first->getATop();
	m_firstB= first->getB();
	m_firstX= first->getX();
	m_firstY= first->getY();

	CEllipse* second = ellipses->getSecond();
	m_secondATop = second->getATop();
	m_secondABottom = second->getABottom();
	m_secondX = second->getX();
	m_secondY = second->getY();
	m_secondB = second->getB();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEllipsePairDlg::OnOK() 
{
	if(UpdateData())
	{
		CEllipse* first = ellipses->getFirst();
		first->setABottom(m_firstABottom);
		first->setATop(m_firstATop);
		first->setB(m_firstB);
		first->setX(m_firstX);
		first->setY(m_firstY);

		CEllipse* second = ellipses->getSecond();
		second->setATop(m_secondATop);
		second->setABottom(m_secondABottom);
		second->setX(m_secondX);
		second->setY(m_secondY);
		second->setB(m_secondB);

		CDialog::OnOK();
	}
}

void CEllipsePairDlg::OnCopy() 
{
	if(UpdateData())
	{
		m_secondABottom = m_firstABottom;
		m_secondATop = m_firstATop;
		m_secondB = m_firstB;
		m_secondX = m_firstX;
		m_secondY = m_firstY;
		UpdateData(FALSE);
	}
}
