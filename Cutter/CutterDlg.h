/* Cutter
Copyright(C) 2019 R Bruce Porteous

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


#if !defined(AFX_CUTTERDLG_H__D76CBE0C_CA15_11D6_955D_000000000000__INCLUDED_)
#define AFX_CUTTERDLG_H__D76CBE0C_CA15_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCutterDlg dialog
#include <afxsock.h>
#include <string>

#include "SocketEventHandler.h"
#include "LinkSocket.h"
#include "ListenerSocket.h"
#include "Resource.h"
#include "afxwin.h"
#include "MainTabCtrl.h"

#include "CutterHardware.h"
#include "CNCFoamCutter.h"
#include "GCodeInterpreter.h"

struct CutterConfig;

class CCutterDlg : public CDialog, CSocketEventHandler
{

	CutterHardware* pCutterHardware;
	CNCFoamCutter * pCNCCutter;
	GCodeInterpreter* pGCodeInterpreter;

	CListenerSocket listener;
	CLinkSocket link;
	std::string data;

	void connectHardware(const char* port);
	void disconnectHardware();

// Construction
public:
	CCutterDlg(CWnd* pParent = NULL);	// standard constructor
	~CCutterDlg();

	inline CutterHardware* getHardware() { return pCutterHardware; }
	inline CNCFoamCutter * getCutter() { return pCNCCutter; }
	inline GCodeInterpreter* getInterpreter() { return pGCodeInterpreter; }

	void configLoaded(CutterConfig* pConfig);

// Dialog Data
	//{{AFX_DATA(CCutterDlg)
	enum { IDD = IDD_CUTTER_DIALOG };
	CListBox	m_lstMessages;
	UINT	m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCutterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCutterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCmdListen();
	afx_msg void OnCmdAccept();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// CSocketEventHandler overrides
	virtual void connected();
	virtual void error(int nErrorCode);
	virtual void received(const char* data, int nBytes);
	virtual void closed();

	afx_msg void OnEnChangeEdtPort();

private:


	CButton btnConnect;
	CComboBox cmbSerialPort;
	afx_msg void OnBnClickedBtnConnect();
	CMainTabCtrl mainTabs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUTTERDLG_H__D76CBE0C_CA15_11D6_955D_000000000000__INCLUDED_)
