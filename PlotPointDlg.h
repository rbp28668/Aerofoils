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
#if !defined(AFX_PLOTPOINTDLG_H__B5325B53_C8EA_11D6_955D_000000000000__INCLUDED_)
#define AFX_PLOTPOINTDLG_H__B5325B53_C8EA_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlotPointDlg.h : header file
//

class CPointStructure;

/////////////////////////////////////////////////////////////////////////////
// CPlotPointDlg dialog

class CPlotPointDlg : public CDialog
{
// Construction
public:
	CPlotPointDlg(CPointStructure* ppp, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlotPointDlg)
	enum { IDD = IDD_PLOT_POINT };
	float	m_rootX;
	float	m_rootY;
	float	m_span;
	float	m_tipX;
	float	m_tipY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotPointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlotPointDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CPointStructure* point;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTPOINTDLG_H__B5325B53_C8EA_11D6_955D_000000000000__INCLUDED_)
