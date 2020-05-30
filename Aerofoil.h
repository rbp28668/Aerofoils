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
// Aerofoil.h : main header file for the AEROFOIL application
//

#if !defined(AFX_AEROFOIL_H__BBA1B6A5_B27A_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_AEROFOIL_H__BBA1B6A5_B27A_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CAerofoilDoc;
class CutterDoc;

/////////////////////////////////////////////////////////////////////////////
// CAerofoilApp:
// See Aerofoil.cpp for the implementation of this class
//

class CAerofoilApp : public CWinApp
{
	CMultiDocTemplate* pAerofoilTemplate;
	CMultiDocTemplate* pCutterTemplate;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CAerofoilApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAerofoilApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	CAerofoilDoc* createAerofoilDocument();
	CutterDoc* createCutterDocument();

// Implementation
	//{{AFX_MSG(CAerofoilApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AEROFOIL_H__BBA1B6A5_B27A_11D6_AF75_002018BA082E__INCLUDED_)
