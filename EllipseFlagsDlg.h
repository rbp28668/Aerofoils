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
#if !defined(AFX_ELLIPSEFLAGSDLG_H__3CB6E434_CC06_11D6_9565_000000000000__INCLUDED_)
#define AFX_ELLIPSEFLAGSDLG_H__3CB6E434_CC06_11D6_9565_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EllipseFlagsDlg.h : header file
//

class CEllipseFlags;

/////////////////////////////////////////////////////////////////////////////
// CEllipseFlagsDlg dialog

class CEllipseFlagsDlg : public CDialog
{
// Construction
public:
	CEllipseFlagsDlg(CEllipseFlags* pef, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEllipseFlagsDlg)
	enum { IDD = IDD_ELLIPSE_FLAGS };
	BOOL	m_lower;
	BOOL	m_markers;
	BOOL	m_upper;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEllipseFlagsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEllipseFlagsDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CEllipseFlags* flags;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELLIPSEFLAGSDLG_H__3CB6E434_CC06_11D6_9565_000000000000__INCLUDED_)
