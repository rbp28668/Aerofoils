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
#if !defined(AFX_WINGFLAGSDLG_H__44A146B3_BBF2_11D6_9543_000000000000__INCLUDED_)
#define AFX_WINGFLAGSDLG_H__44A146B3_BBF2_11D6_9543_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WingFlagsDlg.h : header file
//

class CPlotFlags;

/////////////////////////////////////////////////////////////////////////////
// CWingFlagsDlg dialog

class CWingFlagsDlg : public CDialog
{
// Construction
public:
	CWingFlagsDlg(CPlotFlags* pf, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWingFlagsDlg)
	enum { IDD = IDD_WING_FLAGS };
	BOOL	m_blPlotChord;
	BOOL	m_blPlotLabels;
	BOOL	m_blPlotMarkers;
	BOOL	m_blPlotSection;
	BOOL	m_blPlotSkin;
	BOOL	m_blPlotSpars;
	BOOL	m_blPlotTE;
	BOOL	m_blPlotLE;
	BOOL	m_blPlotCutouts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWingFlagsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWingFlagsDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CPlotFlags* flags;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINGFLAGSDLG_H__44A146B3_BBF2_11D6_9543_000000000000__INCLUDED_)
