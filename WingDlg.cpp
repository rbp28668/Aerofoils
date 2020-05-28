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
// WingDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Aerofoil.h"
#include "WingDlg.h"
#include "SectionDlg.h"
#include "SparDlg.h"
#include "CutoutDialog.h"

#include "kernel/Wing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWingDlg dialog


CWingDlg::CWingDlg(CWing* w, CWnd* pParent /*=NULL*/)
	: CDialog(CWingDlg::IDD, pParent)
	, wing(w)
	, wingCopy(*w)
	, rootSection(&wingCopy, true)
	, tipSection(&wingCopy, false)
	, rootChord(0)
	, rootSweep(0)
	, rootWashout(0)
	, rootThickness(0)
	, tipChord(0)
	, tipSweep(0)
	, tipHeight(0)
	, tipWashout(0)
	, tipThickness(0)
	, rootHeight(0)
{
	//{{AFX_DATA_INIT(CWingDlg)
	m_LE = 0.0f;
	m_span = 0.0f;
	m_TE = 0.0f;
	m_fSkin = 0.0f;
	//}}AFX_DATA_INIT
}


void CWingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWingDlg)
	DDX_Control(pDX, IDC_LST_SPARS, m_lstSpars);
	DDX_Text(pDX, IDC_EDT_LE, m_LE);
	DDV_MinMaxFloat(pDX, m_LE, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDT_SPAN, m_span);
	DDV_MinMaxFloat(pDX, m_span, 0.f, 10000.f);
	DDX_Text(pDX, IDC_EDT_TE, m_TE);
	DDV_MinMaxFloat(pDX, m_TE, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDT_SKIN, m_fSkin);
	DDV_MinMaxFloat(pDX, m_fSkin, 0.f, 10.f);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LST_CUTOUTS, m_lstCutouts);
	DDX_Control(pDX, IDC_ROOT_DISPLAY, rootSection);
	DDX_Control(pDX, IDC_TIP_DISPLAY, tipSection);
	DDX_Text(pDX, IDC_EDT_ROOT_CHORD, rootChord);
	DDV_MinMaxDouble(pDX, rootChord, 0.0, 1000.0);
	DDX_Text(pDX, IDC_EDT_ROOT_SWEEP, rootSweep);
	DDV_MinMaxDouble(pDX, rootSweep, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDT_ROOT_HEIGHT, rootHeight);
	DDV_MinMaxDouble(pDX, rootHeight, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDT_ROOT_WASHOUT, rootWashout);
	DDV_MinMaxDouble(pDX, rootWashout, -10.f, 10.f);
	DDX_Text(pDX, IDC_EDT_ROOT_THICKNESS_MOD, rootThickness);
	DDV_MinMaxDouble(pDX, rootThickness, 0.1f, 10.f);
	DDX_Text(pDX, IDC_EDT_TIP_CHORD, tipChord);
	DDV_MinMaxDouble(pDX, tipChord, 0.0, 1000.0);
	DDX_Text(pDX, IDC_EDT_TIP_SWEEP, tipSweep);
	DDV_MinMaxDouble(pDX, tipSweep, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDT_TIP_HEIGHT, tipHeight);
	DDV_MinMaxDouble(pDX, tipHeight, 0.0, 1000.0);
	DDX_Text(pDX, IDC_EDT_TIP_WASHOUT, tipWashout);
	DDV_MinMaxDouble(pDX, tipWashout, -10.f, 10.f);
	DDX_Text(pDX, IDC_EDT_TIP_THICKNESS_MOD, tipThickness);
	DDV_MinMaxDouble(pDX, tipThickness, 0.1f, 10.f);
	DDX_Control(pDX, IDC_LBL_ROOT_SECTION_NAME, lblRootInfo);
	DDX_Control(pDX, IDC_LBL_TIP_SECTION_NAME, lblTipInfo);
}


BEGIN_MESSAGE_MAP(CWingDlg, CDialog)
	//{{AFX_MSG_MAP(CWingDlg)
	ON_BN_CLICKED(IDC_CMD_ADD_SPAR, OnCmdAddSpar)
	ON_BN_CLICKED(IDC_CMD_SPAR_DELETE, OnCmdSparDelete)
	ON_BN_CLICKED(IDC_CMD_SPAR_EDIT, OnCmdSparEdit)
	ON_LBN_SELCHANGE(IDC_LST_SPARS, OnSelchangeLstSpars)
	ON_LBN_DBLCLK(IDC_LST_SPARS, OnDblclkLstSpars)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_ADD_CUTOUT, &CWingDlg::OnBnClickedBtnAddCutout)
	ON_BN_CLICKED(IDC_BTN_EDIT_CUTOUT, &CWingDlg::OnBnClickedBtnEditCutout)
	ON_BN_CLICKED(IDC_BTN_DELETE_CUTOUT, &CWingDlg::OnBnClickedBtnDeleteCutout)
	ON_LBN_DBLCLK(IDC_LST_CUTOUTS, &CWingDlg::OnLbnDblclkLstCutouts)
	ON_EN_CHANGE(IDC_EDT_ROOT_CHORD, &CWingDlg::OnChangeEdtRootChord)
	ON_EN_CHANGE(IDC_EDT_ROOT_THICKNESS_MOD, &CWingDlg::OnChangeEdtRootThicknessMod)
	ON_EN_CHANGE(IDC_EDT_ROOT_WASHOUT, &CWingDlg::OnChangeEdtRootWashout)
	ON_BN_CLICKED(IDC_CMD_ROOT_SECTION_NAME, &CWingDlg::OnBnClickedCmdRootSectionName)
	ON_BN_CLICKED(IDC_CMD_TIP_SECTION_NAME, &CWingDlg::OnBnClickedCmdTipSectionName)
END_MESSAGE_MAP()

void CWingDlg::addSparToList(CSpar* ps)
{
	assert(this);
	assert(ps);

	int idx = m_lstSpars.AddString(ps->getText());
	if(idx != LB_ERR)
		m_lstSpars.SetItemDataPtr(idx, ps);

}

void CWingDlg::addCutoutToList(Cutout* pc)
{
	assert(this);
	assert(pc);
	int idx = m_lstCutouts.AddString(pc->getText());
	if (idx != LB_ERR)
		m_lstCutouts.SetItemDataPtr(idx, pc);
}

/////////////////////////////////////////////////////////////////////////////
// CWingDlg message handlers


void CWingDlg::OnCmdAddSpar() 
{
	CSpar spar;
	CSparDlg dlg(&spar);
	if(dlg.DoModal())
	{
		CSpar* ps = wingCopy.addSpar(spar);	
		addSparToList(ps);
		rootSection.RedrawWindow();
		tipSection.RedrawWindow();
	}
}

void CWingDlg::OnCmdSparDelete() 
{
	int idx = m_lstSpars.GetCurSel();
	if(idx != LB_ERR)
	{
		// MessageBox()
		CSpar* ps = (CSpar*)m_lstSpars.GetItemDataPtr(idx);
		m_lstSpars.DeleteString(idx);

		wingCopy.deleteSpar(ps);
		rootSection.RedrawWindow();
		tipSection.RedrawWindow();
	}

}

void CWingDlg::OnCmdSparEdit() 
{
	int idx = m_lstSpars.GetCurSel();
	if(idx != LB_ERR)
	{
		CSpar* ps = (CSpar*)m_lstSpars.GetItemDataPtr(idx);
		CSparDlg dlg(ps);
		if(dlg.DoModal())
		{
			m_lstSpars.DeleteString(idx);
			m_lstSpars.InsertString(idx, ps->getText());
			m_lstSpars.SetItemDataPtr(idx, ps);
			rootSection.RedrawWindow();
			tipSection.RedrawWindow();
		}
	}
}


void CWingDlg::OnSelchangeLstSpars() 
{
	// TODO: Add your control notification handler code here
	
}

void CWingDlg::OnDblclkLstSpars() 
{
	OnCmdSparEdit();
	
}

void CWingDlg::OnBnClickedBtnAddCutout()
{
	Cutout cutout;
	CutoutDialog dlg(&cutout);
	if (dlg.DoModal())
	{
		Cutout* pc = wingCopy.addCutout(cutout);
		addCutoutToList(pc);
		rootSection.RedrawWindow();
		tipSection.RedrawWindow();
	}
}


void CWingDlg::OnBnClickedBtnEditCutout()
{
	int idx = m_lstCutouts.GetCurSel();
	if (idx != LB_ERR)
	{
		Cutout* pc = (Cutout*)m_lstCutouts.GetItemDataPtr(idx);
		CutoutDialog dlg(pc);
		if (dlg.DoModal())
		{
			m_lstCutouts.DeleteString(idx);
			m_lstCutouts.InsertString(idx, pc->getText());
			m_lstCutouts.SetItemDataPtr(idx, pc);
			rootSection.RedrawWindow();
			tipSection.RedrawWindow();
		}
	}

}


void CWingDlg::OnBnClickedBtnDeleteCutout()
{
	int idx = m_lstCutouts.GetCurSel();
	if (idx != LB_ERR)
	{
		// MessageBox()
		Cutout* pc = (Cutout*)m_lstCutouts.GetItemDataPtr(idx);
		m_lstCutouts.DeleteString(idx);

		wingCopy.deleteCutout(pc);
		rootSection.RedrawWindow();
		tipSection.RedrawWindow();
	}
}

void CWingDlg::OnLbnDblclkLstCutouts()
{
	OnBnClickedBtnEditCutout();
}


void CWingDlg::OnOK() 
{

	if(UpdateData())
	{
		wingCopy.setSpan(m_span);
		wingCopy.setLE(m_LE);
		wingCopy.setTE(m_TE);
		wingCopy.setSkinThickness(m_fSkin);

		wingCopy.getRoot()->modifyThickness(rootThickness);
		wingCopy.getRootTransform()->setChord(rootChord);
		wingCopy.getRootTransform()->setHeight(rootHeight);
		wingCopy.getRootTransform()->setSweep(rootSweep);
		wingCopy.getRootTransform()->setWashout(rootWashout);

		wingCopy.getTip()->modifyThickness(tipThickness);
		wingCopy.getTipTransform()->setChord(tipChord);
		wingCopy.getTipTransform()->setHeight(tipHeight);
		wingCopy.getTipTransform()->setSweep(tipSweep);
		wingCopy.getTipTransform()->setWashout(tipWashout);

		*wing = wingCopy;
		CDialog::OnOK();
	}
}

BOOL CWingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_span = wingCopy.getSpan();
	m_LE = (float)wingCopy.getLE();
	m_TE = (float)wingCopy.getTE();
	m_fSkin = (float)wingCopy.getSkinThickness();

	rootThickness = wingCopy.getRoot()->getThicknessModifier();
	rootChord = wingCopy.getRootTransform()->getChord();
	rootHeight = wingCopy.getRootTransform()->getHeight();
	rootSweep = wingCopy.getRootTransform()->getSweep();
	rootWashout = wingCopy.getRootTransform()->getWashout();

	tipThickness = wingCopy.getTip()->getThicknessModifier();
	tipChord = wingCopy.getTipTransform()->getChord();
	tipHeight = wingCopy.getTipTransform()->getHeight();
	tipSweep = wingCopy.getTipTransform()->getSweep();
	tipWashout = wingCopy.getTipTransform()->getWashout();

	lblRootInfo.SetWindowText(wingCopy.getRoot()->getName().c_str());
	lblTipInfo.SetWindowText(wingCopy.getTip()->getName().c_str());


	for(int i=0; i<wingCopy.getSparCount(); ++i)
	{
		CSpar* spar = wingCopy.getSpar(i);
		addSparToList(spar);
	}

	for (int i = 0; i < wingCopy.getCutoutCount(); ++i)
	{
		Cutout* cutout = wingCopy.getCutout(i);
		addCutoutToList(cutout);
	}

	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}






void CWingDlg::OnChangeEdtRootChord()
{
	//if (UpdateData()) {
	//	wingCopy.getRootTransform()->setChord(rootChord);
	//	rootSection.RedrawWindow();
	//}
}


void CWingDlg::OnChangeEdtRootThicknessMod()
{
	//if (UpdateData()) {
	//	wingCopy.getRoot()->modifyThickness(rootThickness);
	//	rootSection.RedrawWindow();
	//}
}


void CWingDlg::OnChangeEdtRootWashout()
{
	//if (UpdateData()) {
	//	wingCopy.getRootTransform()->setWashout(rootWashout);
	//	rootSection.RedrawWindow();
	//}
}


void CWingDlg::OnBnClickedCmdRootSectionName()
{
	CFileDialog dlg(TRUE,
		".DAT",
		NULL,
		OFN_FILEMUSTEXIST,
		"Aerofoil Files (.DAT)|*.DAT||"
	);
	
	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.lpstrTitle = "Select Root Section";

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		wingCopy.getRoot()->read(path);
		lblRootInfo.SetWindowText(wingCopy.getRoot()->getName().c_str());
		rootSection.RedrawWindow();
	}
}


void CWingDlg::OnBnClickedCmdTipSectionName()
{
	CFileDialog dlg(TRUE,
		".DAT",
		NULL,
		OFN_FILEMUSTEXIST,
		"Aerofoil Files (.DAT)|*.DAT||"
	);

	OPENFILENAME& ofn = dlg.GetOFN();
	ofn.lpstrTitle = "Select Tip Section";

	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		wingCopy.getTip()->read(path);
		lblTipInfo.SetWindowText(wingCopy.getTip()->getName().c_str());
		tipSection.RedrawWindow();
	}
}
