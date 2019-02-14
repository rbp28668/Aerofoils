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
#if !defined(AFX_BACKGROUNDGRIDDLG_HPP__8CC103EF_C156_4C61_BCDA_715D42756528__INCLUDED_)
#define AFX_BACKGROUNDGRIDDLG_HPP__8CC103EF_C156_4C61_BCDA_715D42756528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackgroundGridDlg.hpp : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackgroundGridDlg dialog

class CBackgroundGridDlg : public CDialog
{
// Construction
public:
	CBackgroundGridDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBackgroundGridDlg)
	enum { IDD = IDD_BACKGROUND_GRID };
	CEdit	m_edtVert;
	CEdit	m_edtHoriz;
	CButton	m_radVert;
	CButton	m_radHoriz;
	BOOL	m_blPlotHoriz;
	BOOL	m_blPlotVert;
	float	m_fHorizDistance;
	float	m_fVertDistance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackgroundGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBackgroundGridDlg)
	virtual void OnOK();
	afx_msg void OnChkHoriz();
	afx_msg void OnChkVert();
	afx_msg void OnRadHoriz3();
	afx_msg void OnRadHoriz4();
	afx_msg void OnRadHorizAny();
	afx_msg void OnRadVert3();
	afx_msg void OnRadVert4();
	afx_msg void OnRadVertAny();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKGROUNDGRIDDLG_HPP__8CC103EF_C156_4C61_BCDA_715D42756528__INCLUDED_)
