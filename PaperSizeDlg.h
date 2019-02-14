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
#if !defined(AFX_PAPERSIZEDLG_H__1500ED76_BE66_11D6_9546_00047660C89A__INCLUDED_)
#define AFX_PAPERSIZEDLG_H__1500ED76_BE66_11D6_9546_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaperSizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaperSizeDlg dialog

class CPaperSizeDlg : public CDialog
{
// Construction
public:
	CPaperSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPaperSizeDlg)
	enum { IDD = IDD_PAGE_SIZE };
	CEdit	m_editWidth;
	CEdit	m_editHeight;
	float	m_height;
	float	m_width;
	int		m_sizeSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaperSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPaperSizeDlg)
	afx_msg void OnRadCustom();
	afx_msg void OnRadA3();
	afx_msg void OnRadA3l();
	afx_msg void OnRadA4();
	afx_msg void OnRadA4l();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAPERSIZEDLG_H__1500ED76_BE66_11D6_9546_00047660C89A__INCLUDED_)
