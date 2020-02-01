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


#include "stdafx.h"
#include <assert.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include "CutterDlg.h"
#include "ComPortEnumerator.h"
#include "CutterConfig.h"
#include "MainTabCtrl.h"
#include "GCodeDialog.h"
#include "afxwin.h"
#include "../Kernel/Cutter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
private:
	char* pszText = "This program is free software; you can redistribute it and/or modify \r\n"
		"it under the terms of the GNU General Public License as published by\r\n"
		"the Free Software Foundation; either version 3 of the License, or\r\n"
		"(at your option) any later version.\r\n"
		"\r\n"
		"This program is distributed in the hope that it will be useful,\r\n"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\r\n"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\r\n"
		"GNU General Public License for more details.\r\n"
		"\r\n"
		"You should have received a copy of the GNU General Public License\r\n"
		"along with this program; if not, write to the Free Software Foundation,\r\n"
		"Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301  USA\r\n";
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CEdit license;
	CString licenseText;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, licenseText(_T(pszText))
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDT_LICENSE, license);
	DDX_Text(pDX, IDC_EDT_LICENSE, licenseText);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCutterDlg dialog

void CCutterDlg::connectHardware(const char* port)
{
	assert(this);
	assert(port);

	if (pCutterHardware->isConnected()) {
		pCutterHardware->disconnect();
	}

	int err = pCutterHardware->connect(port);
	if (err) {
		AfxMessageBox("Unable to connect", MB_OK | MB_ICONEXCLAMATION);
	} else {
		btnConnect.SetWindowTextA("Disconnect");
		cmbSerialPort.SelectString(-1,port);
		cmbSerialPort.EnableWindow(FALSE);
	}
}

void CCutterDlg::disconnectHardware()
{
	pCutterHardware->disconnect();
	btnConnect.SetWindowTextA("Connect");
	cmbSerialPort.EnableWindow(TRUE);
}

CCutterDlg::CCutterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCutterDlg::IDD, pParent)
	, pCutterHardware(0)
	, pCNCCutter(0)
	, pGCodeInterpreter(0)
	, pProgram(0)
	, pListener(0)
	, pLink(0)
	
{
	//{{AFX_DATA_INIT(CCutterDlg)
	m_port = 8193;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pCutterHardware = new CutterHardware();
	pCNCCutter = new CNCFoamCutter(pCutterHardware);
	pGCodeInterpreter = new GCodeInterpreter();
	pProgram = new GCodeProgram();

	pGCodeInterpreter->setCutter(pCNCCutter);
	pGCodeInterpreter->setContext(pProgram);
	pProgram->setInterpreter(pGCodeInterpreter);
	
}

CCutterDlg::~CCutterDlg()
{
	delete pProgram;
	delete pGCodeInterpreter;
	delete pCNCCutter;
	delete pCutterHardware;

	pProgram = 0;
	pGCodeInterpreter = 0;
	pCNCCutter = 0;
	pCutterHardware = 0;
}

void CCutterDlg::configLoaded(CutterConfig * pConfig)
{
	pCNCCutter->setWidth(pConfig->cutterWidth);
	pCNCCutter->setBlockLeft(pConfig->blockLeft);
	pCNCCutter->setBlockRight(pConfig->blockRight);
	pCNCCutter->setFeedRate(pConfig->defaultFeedRate);

	pCNCCutter->setXLead(pConfig->xScrewLead);
	pCNCCutter->setXStepsPerRev(pConfig->xStepsPerRev);
	pCNCCutter->setXMicroStepping(pConfig->xMicroStepping);
	pCNCCutter->setYLead(pConfig->yScrewLead);
	pCNCCutter->setYStepsPerRev(pConfig->yStepsPerRev);
	pCNCCutter->setYMicroStepping(pConfig->yMicroStepping);
	pCNCCutter->setStepFrequency(pConfig->stepFrequency);

	if (pConfig->connectAutomatically && pConfig->defaultComPort.length() > 0) {
		connectHardware(pConfig->defaultComPort.c_str());
	}

	if (pConfig->listenAutomatically) {
		m_port = pConfig->defaultListenPort;
		listen();
	}
}

void CCutterDlg::showLimitSwitches(int status) {
	chkLXLimit.SetCheck((status & 0x08) != 0 ? BST_CHECKED : BST_UNCHECKED);
	chkLYLimit.SetCheck((status & 0x04) != 0 ? BST_CHECKED : BST_UNCHECKED);
	chkRXLimit.SetCheck((status & 0x02) != 0 ? BST_CHECKED : BST_UNCHECKED);
	chkRYLimit.SetCheck((status & 0x01) != 0 ? BST_CHECKED : BST_UNCHECKED);
}

void CCutterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 1024, 65535);
	DDX_Control(pDX, IDC_BTN_CONNECT, btnConnect);
	DDX_Control(pDX, IDC_COMBO_SERIAL, cmbSerialPort);
	DDX_Control(pDX, IDC_TAB_MAIN, mainTabs);
	DDX_Control(pDX, IDC_TXT_STATUS, lblStatus);
	DDX_Control(pDX, IDC_CMD_LISTEN, btnListen);
	DDX_Control(pDX, IDC_EDT_PORT, edtPort);
	DDX_Control(pDX, IDC_CHK_LX_LIMIT, chkLXLimit);
	DDX_Control(pDX, IDC_CHK_LY_LIMIT, chkLYLimit);
	DDX_Control(pDX, IDC_CHK_RX_LIMIT, chkRXLimit);
	DDX_Control(pDX, IDC_CHK_RY_LIMIT, chkRYLimit);
}

BEGIN_MESSAGE_MAP(CCutterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CMD_LISTEN, OnCmdListen)
	ON_EN_CHANGE(IDC_EDT_PORT, &CCutterDlg::OnEnChangeEdtPort)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CCutterDlg::OnBnClickedBtnConnect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketEventHandler over-rides

void CCutterDlg::connected()
{
	assert(this);
	lblStatus.SetWindowText("Connected");
	progress = 0;
	lineBuffer.erase();
	pProgram->clear();
}

void CCutterDlg::error(int nErrorCode)
{
	assert(this);
	ostringstream os;
	os << "Error code " << nErrorCode << ends;
	lblStatus.SetWindowText(os.str().c_str());
}

void CCutterDlg::received(const char* bytes, int nBytes)
{
	assert(this);

	lineBuffer.append(bytes, nBytes);
	size_t index = lineBuffer.find("\n");
	while (index != string::npos) {
		string part = lineBuffer.substr(0, index);
		lineBuffer.erase(0, index + 1); // message + trailing newline
		pProgram->add(part);
		index = lineBuffer.find("\n");
	}

	char* spinner = "|/-\\";
	char str[2];
	progress = (progress + 1) & 0x03;
	str[0] = spinner[progress];
	str[1] = 0;
	lblStatus.SetWindowText(str);
}

void CCutterDlg::closed()
{
	assert(this);
	lblStatus.SetWindowText("Closed");

	// Set up new link socket for any subsequent connection
	assert(pLink);
	delete pLink;
	if(pListener){
		pLink = new CLinkSocket(this);
		pListener->setLink(pLink);
	}

	mainTabs.getGCodeDialog()->programUpdated();

}

void CCutterDlg::listen()
{
	pListener = new CListenerSocket(this);
	pListener->Create(m_port);
	pLink = new CLinkSocket(this);
	pListener->setLink(pLink);
	pListener->Listen();

	btnListen.SetWindowTextA("Stop Listening");
	edtPort.EnableWindow(FALSE);
}

void CCutterDlg::stopListening()
{
	pListener->Close();
	delete pLink;
	delete pListener;
	pListener = 0;
	pLink = 0;

	btnListen.SetWindowTextA("Listen");
	edtPort.EnableWindow(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// CCutterDlg message handlers

BOOL CCutterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

#ifdef DEBUG
	// Hack to push console IO into launching window
	if (AttachConsole(ATTACH_PARENT_PROCESS))
	{
		#pragma warning( disable : 4996)
		freopen("CONIN$", "rb", stdin);   // reopen stdin handle as console window input
		freopen("CONOUT$", "wb", stdout);  // reopen stout handle as console window output
		freopen("CONOUT$", "wb", stderr);  // reopen stderr handle as console window output
	}
#endif

	ComPortEnumerator cpe;
	std::vector< tstring > ports;
	cpe.detect(ports, 16);

	for (std::vector< tstring >::const_iterator it = ports.begin(); it != ports.end(); ++it)	{
		cmbSerialPort.AddString(it->c_str());
	}

	mainTabs.init(this);  // call this after com port combo is set up as may want to set it.

	AfxSocketInit();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCutterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCutterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCutterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCutterDlg::OnCmdListen() 
{
	if (pListener) {
		stopListening();
	} else {
		listen();
	}
}


void CCutterDlg::OnEnChangeEdtPort()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CCutterDlg::OnBnClickedBtnConnect()
{
	if (pCutterHardware->isConnected()) {
		disconnectHardware();
	} else {
		int idx = cmbSerialPort.GetCurSel();
		CString value;
		cmbSerialPort.GetLBText(idx, value);
		connectHardware(value);
	}
}




