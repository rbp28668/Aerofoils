// CutterDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include <sstream>
#include <fstream>
#include "Cutter.h"
#include "CutterDlg.h"

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
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCutterDlg dialog

CCutterDlg::CCutterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCutterDlg::IDD, pParent)
	, listener(this)
	, link(this)
{
	//{{AFX_DATA_INIT(CCutterDlg)
	m_port = 8193;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCutterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCutterDlg)
	DDX_Control(pDX, IDC_LST_MESSAGES, m_lstMessages);
	DDX_Control(pDX, IDC_LBL_STATUS, m_lblStatus);
	DDX_Text(pDX, IDC_EDT_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 1024, 65535);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCutterDlg, CDialog)
	//{{AFX_MSG_MAP(CCutterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CMD_LISTEN, OnCmdListen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketEventHandler over-rides

void CCutterDlg::connected()
{
	assert(this);
	m_lblStatus.SetWindowText("Connected");
}

void CCutterDlg::error(int nErrorCode)
{
	assert(this);
	ostringstream os;
	os << "Error code " << nErrorCode << ends;
	m_lblStatus.SetWindowText(os.str().c_str());
}

void CCutterDlg::received(const char* bytes, int nBytes)
{
	assert(this);

	string msg(bytes,nBytes);
	m_lstMessages.AddString(msg.c_str());
	if(m_lstMessages.GetCount() > 64)
		m_lstMessages.DeleteString(0);

	data.append(msg);
}

void CCutterDlg::closed()
{
	assert(this);
	m_lblStatus.SetWindowText("Closed");
	listener.setLink(&link);

	ofstream ofs("c:/temp/dump.tmp");
	ofs << data;
	ofs << endl;

	//int pos = 0;
	//data.find_first_of("</l>",pos);
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
	
	// TODO: Add extra initialization here
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
	listener.Create(m_port);
	listener.setLink(&link);
	listener.Listen();
	
}

void CCutterDlg::OnCmdAccept() 
{
	listener.Accept(link);	
}
