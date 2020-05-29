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
// SectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Aerofoil.h"
#include "SectionDlg.h"

#include "kernel/Aerofoil.h"
#include "kernel/Transform.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionDlg dialog


CSectionDlg::CSectionDlg(CAerofoil* pa, CTransform* pt, CWnd* pParent /*=NULL*/)
	: CDialog(CSectionDlg::IDD, pParent)
	, aerofoil(pa)
	, copyAerofoil(pa->getSection().c_str(), pa->getThicknessModifier())
	, transform(pt)
{
	assert(this);
	assert(pa);
	assert(pt);

	//{{AFX_DATA_INIT(CSectionDlg)
	m_chord = 0.0f;
	m_height = 0.0f;
	m_sweep = 0.0f;
	m_thicknessMod = 0.0f;
	m_washout = 0.0f;
	//}}AFX_DATA_INIT
}


void CSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionDlg)
	DDX_Control(pDX, IDC_LBL_SECTION_NAME, m_lblSectionName);
	DDX_Text(pDX, IDC_EDT_CHORD, m_chord);
	DDV_MinMaxFloat(pDX, m_chord, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDT_HEIGHT, m_height);
	DDV_MinMaxFloat(pDX, m_height, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDT_SWEEP, m_sweep);
	DDV_MinMaxFloat(pDX, m_sweep, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDT_THICKNESS_MOD, m_thicknessMod);
	DDV_MinMaxFloat(pDX, m_thicknessMod, 0.1f, 10.f);
	DDX_Text(pDX, IDC_EDT_WASHOUT, m_washout);
	DDV_MinMaxFloat(pDX, m_washout, -10.f, 10.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSectionDlg, CDialog)
	//{{AFX_MSG_MAP(CSectionDlg)
	ON_BN_CLICKED(IDC_CMD_SECTION_NAME, OnCmdSectionName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionDlg message handlers

BOOL CSectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_chord = (float)transform->getChord();
	m_height = (float)transform->getHeight();
	m_sweep = (float)transform->getSweep();
	m_washout = (float)transform->getWashout();

	m_thicknessMod = (float)aerofoil->getThicknessModifier();
	m_lblSectionName.SetWindowText(aerofoil->getName().c_str());

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSectionDlg::OnCmdSectionName() 
{
	CFileDialog dlg(TRUE, 
		".DAT",
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		"Aerofoil Files (.DAT)|*.DAT||"
		);

	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.lpstrTitle = "Select Section";

	if(dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		copyAerofoil.read(path);
		m_lblSectionName.SetWindowText(aerofoil->getName().c_str());
	}
	
}

void CSectionDlg::OnOK() 
{
	if(UpdateData())
	{
		transform->setChord(m_chord);
		transform->setHeight(m_height);
		transform->setSweep(m_sweep);
		transform->setWashout(m_washout);

		copyAerofoil.modifyThickness(m_thicknessMod);
		if(*aerofoil != copyAerofoil)
			*aerofoil = copyAerofoil;
	}

	CDialog::OnOK();
}
