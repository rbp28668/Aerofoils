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
#pragma once

#include "CutPlanformControl.h"
#include "Kernel/CutterGeometry.h"
class Cut;

// CutterGeometryDialog dialog


class CutterGeometryDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CutterGeometryDialog)

	CutterGeometry* pGeometry;
	Cut* pCut;
	CutterGeometry copy;

public:
	CutterGeometryDialog(CutterGeometry* geometry, Cut* cut, CWnd* pParent = nullptr);  
	virtual ~CutterGeometryDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUTTER_GEOMETRY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double x_travel;
	double y_travel;
	double cutter_width;
	double block_lhs;
	double block_width;
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	CutPlanformControl cutPlanform;
		
	afx_msg void OnKillfocusEdtXtravel();
	afx_msg void OnKillfocusEdtYtravel();
	afx_msg void OnKillfocusEdtWidth();
	afx_msg void OnKillfocusEdtBlocklhs();
//	afx_msg void OnChangeEdtBlockwidth();
	afx_msg void OnKillfocusEdtBlockwidth();
};
