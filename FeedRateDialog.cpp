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


// FeedRateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "FeedRateDialog.h"
#include "Kernel/GCodeOutputDevice.h"
#include "afxdialogex.h"


// FeedRateDialog dialog

IMPLEMENT_DYNAMIC(FeedRateDialog, CDialogEx)

FeedRateDialog::FeedRateDialog(GCodeOutputDevice::GCodeConfig*pConfig, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FEED_RATE, pParent)
	, pConfig(pConfig)
	, feedRate(pConfig->cutFeedRate)
	, useFeedRate((pConfig->sendFeedRate) ? TRUE :FALSE)
	, moveFeedRate(pConfig->fastFeedRate)
	, useG1forMoves((pConfig->useG1forMoves) ? TRUE : FALSE)
	, useUnitsPerMin((pConfig->useUnitsPerMin) ? TRUE : FALSE)
	, leftHorizontalAxisCode(pConfig->axesNames[0])
	, leftVerticalAxisCode(pConfig->axesNames[1])
	, rightHorizontalAxisCode(pConfig->axesNames[2])
	, rightVerticalAxisCode(pConfig->axesNames[3])
	, wireOnCode(pConfig->wireOn.c_str())
	, wireOffCode(pConfig->wireOff.c_str())
	, motorsEnableCode(pConfig->enableMotors.c_str())
	, motorsDisableCode(pConfig->disableMotors.c_str())
	, sendWireControls((pConfig->sendWireControls) ? TRUE : FALSE)
	, sendEnableControls((pConfig->sendEnableControls) ? TRUE : FALSE)
	, sendMirror((pConfig->sendMirror) ? TRUE : FALSE)
	, sendWorkshifts((pConfig->sendWorkshifts) ? TRUE : FALSE)
	, useCutterGeometry((pConfig->preCorrectGeometry) ? TRUE : FALSE)
{

}

FeedRateDialog::~FeedRateDialog()
{
}

void FeedRateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FEED_RATE, feedRate);
	DDV_MinMaxDouble(pDX, feedRate, 0.1, 10);
	DDX_Check(pDX, IDC_CHK_INCLUDE_FEED, useFeedRate);
	DDX_Text(pDX, IDC_EDT_MOVE_FEED_RATE, moveFeedRate);
	DDX_Check(pDX, IDC_CHK_USE_G1_FOR_MOVES, useG1forMoves);
	DDX_Check(pDX, IDC_CHK_UNITS_PER_MIN, useUnitsPerMin);
	DDX_Text(pDX, IDC_EDT_LEFT_HORIZ, leftHorizontalAxisCode);
	DDX_Text(pDX, IDC_EDT_LEFT_VERT, leftVerticalAxisCode);
	DDX_Text(pDX, IDC_EDT_RIGHT_HORIZ, rightHorizontalAxisCode);
	DDX_Text(pDX, IDC_EDT_RIGHT_VERT, rightVerticalAxisCode);
	DDX_Text(pDX, IDC_EDT_WIRE_ON, wireOnCode);
	DDX_Text(pDX, IDC_EDT_WIRE_OFF, wireOffCode);
	DDX_Text(pDX, IDC_EDT_ENABLE, motorsEnableCode);
	DDX_Text(pDX, IDC_EDT_DISABLE, motorsDisableCode);
	DDX_Check(pDX, IDC_CHK_SEND_WIRE_CODES, sendWireControls);
	DDX_Check(pDX, IDC_CHK_SEND_ENABLE_DISABLE, sendEnableControls);
	DDX_Check(pDX, IDC_CHK_SEND_MIRROR, sendMirror);
	DDX_Check(pDX, IDC_CHK_SEND_WORKSHIFTS, sendWorkshifts);
	DDX_Check(pDX, IDC_CHK_USE_GEOMETRY, useCutterGeometry);
}

void FeedRateDialog::updateFrom(GCodeOutputDevice::GCodeConfig* pConfig)
{
	feedRate = pConfig->cutFeedRate;
	useFeedRate = (pConfig->sendFeedRate) ? TRUE : FALSE;
	moveFeedRate = pConfig->fastFeedRate;
	useG1forMoves = (pConfig->useG1forMoves) ? TRUE : FALSE;
	useUnitsPerMin = (pConfig->useUnitsPerMin) ? TRUE : FALSE;
	leftHorizontalAxisCode = pConfig->axesNames[0];
	leftVerticalAxisCode = pConfig->axesNames[1];
	rightHorizontalAxisCode = pConfig->axesNames[2];
	rightVerticalAxisCode = pConfig->axesNames[3];
	wireOnCode = pConfig->wireOn.c_str();
	wireOffCode = pConfig->wireOff.c_str();
	motorsEnableCode = pConfig->enableMotors.c_str();
	motorsDisableCode = pConfig->disableMotors.c_str();
	sendWireControls = (pConfig->sendWireControls) ? TRUE : FALSE;
	sendEnableControls = (pConfig->sendEnableControls) ? TRUE : FALSE;
	sendMirror = (pConfig->sendMirror) ? TRUE : FALSE;
	sendWorkshifts = (pConfig->sendWorkshifts) ? TRUE : FALSE;
	useCutterGeometry = (pConfig->preCorrectGeometry) ? TRUE : FALSE;

}


BEGIN_MESSAGE_MAP(FeedRateDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DEFAULTS, &FeedRateDialog::OnBnClickedBtnDefaults)
	ON_BN_CLICKED(IDC_BTN_MACH3, &FeedRateDialog::OnBnClickedBtnMach3)
END_MESSAGE_MAP()


// FeedRateDialog message handlers


void FeedRateDialog::OnBnClickedBtnDefaults()
{
	GCodeOutputDevice::GCodeConfig defaultConfig;
	updateFrom(&defaultConfig);
	UpdateData(FALSE);
}


void FeedRateDialog::OnBnClickedBtnMach3()
{
	GCodeOutputDevice::GCodeConfig config;
	config.setMach3();
	updateFrom(&config);
	UpdateData(FALSE);
}


void FeedRateDialog::OnOK()
{
	if (UpdateData()) {
		pConfig->cutFeedRate = feedRate;
		pConfig->sendFeedRate = useFeedRate == TRUE;
		pConfig->fastFeedRate = moveFeedRate;
		pConfig->useG1forMoves = useG1forMoves == TRUE;
		pConfig->useUnitsPerMin = useUnitsPerMin == TRUE;
		pConfig->axesNames[0] = leftHorizontalAxisCode[0];
		pConfig->axesNames[1] = leftVerticalAxisCode[0];
		pConfig->axesNames[2] = rightHorizontalAxisCode[0];
		pConfig->axesNames[3] = rightVerticalAxisCode[0];
		pConfig->wireOn = wireOnCode.GetBuffer();
		pConfig->wireOff = wireOffCode.GetBuffer();
		pConfig->enableMotors = motorsEnableCode.GetBuffer();
		pConfig->disableMotors = motorsDisableCode.GetBuffer();
		pConfig->sendWireControls = sendWireControls == TRUE;
		pConfig->sendEnableControls = sendEnableControls == TRUE;
		pConfig->sendMirror = sendMirror == TRUE;
		pConfig->sendWorkshifts = sendWorkshifts == TRUE;
		pConfig->preCorrectGeometry = useCutterGeometry = TRUE;

	}

	CDialogEx::OnOK();
}
