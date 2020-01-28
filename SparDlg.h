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
#if !defined(AFX_SPARDLG_H__02ECB585_BC0D_11D6_9544_00047660C89A__INCLUDED_)
#define AFX_SPARDLG_H__02ECB585_BC0D_11D6_9544_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SparDlg.h : header file
//

class CSpar;

/////////////////////////////////////////////////////////////////////////////
// CSparDlg dialog

class CSparDlg : public CDialog
{
// Construction
public:
	CSparDlg(CSpar* ps, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSparDlg)
	enum { IDD = IDD_EDIT_SPAR };
	CEdit	m_edtTipHeight;
	CEdit	m_edtRootHeight;
	BOOL	m_blSubmerged;
	float	m_rootHeight;
	float	m_rootWidth;
	float	m_rootX;
	float	m_tipHeight;
	float	m_tipWidth;
	float	m_tipX;
	int		m_typeSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSparDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSparDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCopy();
	afx_msg void OnRadBottom();
	afx_msg void OnRadFullDepth();
	afx_msg void OnRadTop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CSpar* spar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPARDLG_H__02ECB585_BC0D_11D6_9544_00047660C89A__INCLUDED_)
