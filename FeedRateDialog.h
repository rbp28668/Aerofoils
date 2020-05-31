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

#include "Kernel/GCodeOutputDevice.h"

// FeedRateDialog dialog

class FeedRateDialog : public CDialogEx
{
	DECLARE_DYNAMIC(FeedRateDialog)

	GCodeOutputDevice::GCodeConfig* pConfig;

public:
	FeedRateDialog(GCodeOutputDevice::GCodeConfig* pConfig, CWnd* pParent = nullptr);   
	virtual ~FeedRateDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FEED_RATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void updateFrom(GCodeOutputDevice::GCodeConfig* pConfig);
	DECLARE_MESSAGE_MAP()
public:
	// feed rate in mm per second
	double feedRate;
	BOOL useFeedRate;
	double moveFeedRate;
	BOOL useG1forMoves;
	BOOL useUnitsPerMin;
	CString leftHorizontalAxisCode;
	CString leftVerticalAxisCode;
	CString rightHorizontalAxisCode;
	CString rightVerticalAxisCode;
	CString wireOnCode;
	CString wireOffCode;
	CString motorsEnableCode;
	CString motorsDisableCode;
	BOOL sendWireControls;
	BOOL sendEnableControls;
	BOOL sendMirror;
	BOOL sendWorkshifts;
	afx_msg void OnBnClickedBtnDefaults();
	afx_msg void OnBnClickedBtnMach3();
	virtual void OnOK();
	BOOL useCutterGeometry;
};
