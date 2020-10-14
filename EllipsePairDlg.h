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
#if !defined(AFX_ELLIPSEPAIRDLG_H__737BD963_BF45_11D6_954B_00047660C89A__INCLUDED_)
#define AFX_ELLIPSEPAIRDLG_H__737BD963_BF45_11D6_954B_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EllipsePairDlg.h : header file
//

class CEllipsePair;

/////////////////////////////////////////////////////////////////////////////
// CEllipsePairDlg dialog

class CEllipsePairDlg : public CDialog
{
// Construction
public:
	CEllipsePairDlg(CEllipsePair* pep, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEllipsePairDlg)
	enum { IDD = IDD_ELLIPSE_PAIR };
	float	m_firstABottom;
	float	m_firstATop;
	float	m_firstB;
	float	m_firstX;
	float	m_firstY;
	float	m_secondX;
	float	m_secondY;
	float	m_secondB;
	float	m_secondABottom;
	float	m_secondATop;
	double spacing;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEllipsePairDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEllipsePairDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CEllipsePair* ellipses;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELLIPSEPAIRDLG_H__737BD963_BF45_11D6_954B_00047660C89A__INCLUDED_)
