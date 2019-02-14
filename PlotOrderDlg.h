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
#if !defined(AFX_PLOTORDERDLG_H__B5325B56_C8EA_11D6_955D_000000000000__INCLUDED_)
#define AFX_PLOTORDERDLG_H__B5325B56_C8EA_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlotOrderDlg.h : header file
//

class CAerofoilDoc;
class CPlotStructure;

/////////////////////////////////////////////////////////////////////////////
// CPlotOrderDlg dialog

class CPlotOrderDlg : public CDialog
{
// Construction
public:
	CPlotOrderDlg(CAerofoilDoc* pd, CWnd* pParent = NULL);   // standard constructor

// Operations
	void setSelection(CPlotStructure* ps);

// Dialog Data
	//{{AFX_DATA(CPlotOrderDlg)
	enum { IDD = IDD_PLOT_ORDER };
	CListBox	m_lstPlots;
	CButton	m_cmdUp;
	CButton	m_cmdTop;
	CButton	m_cmdDown;
	CButton	m_cmdBottom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotOrderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlotOrderDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelchangeLstStructures();
	afx_msg void OnCmdBottom();
	afx_msg void OnCmdDown();
	afx_msg void OnCmdTop();
	afx_msg void OnCmdUp();
	afx_msg void OnSelcancelLstStructures();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CAerofoilDoc* pDoc; 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTORDERDLG_H__B5325B56_C8EA_11D6_955D_000000000000__INCLUDED_)
