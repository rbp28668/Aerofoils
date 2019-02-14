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
#if !defined(AFX_SECTIONDLG_H__1103E542_BB64_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_SECTIONDLG_H__1103E542_BB64_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SectionDlg.h : header file
//

class CTransform;

#include "kernel/Aerofoil.h"

/////////////////////////////////////////////////////////////////////////////
// CSectionDlg dialog

class CSectionDlg : public CDialog
{
// Construction
public:
	CSectionDlg(CAerofoil* pa, CTransform* pt, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSectionDlg)
	enum { IDD = IDD_EDIT_SECTION };
	CStatic	m_lblSectionName;
	float	m_chord;
	float	m_height;
	float	m_sweep;
	float	m_thicknessMod;
	float	m_washout;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSectionName();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CAerofoil* aerofoil;
	CAerofoil copyAerofoil;
	CTransform* transform;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECTIONDLG_H__1103E542_BB64_11D6_AF75_002018BA082E__INCLUDED_)
