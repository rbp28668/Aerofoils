// CNCConnectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "aerofoil.h"
#include "CNCConnectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCNCConnectionDlg dialog


CCNCConnectionDlg::CCNCConnectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCNCConnectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCNCConnectionDlg)
	m_port = 0;
	m_host = _T("");
	//}}AFX_DATA_INIT
}


void CCNCConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCNCConnectionDlg)
	DDX_Text(pDX, IDC_EDT_PORT, m_port);
	DDV_MinMaxUInt(pDX, m_port, 1024, 65535);
	DDX_Text(pDX, IDC_EDT_HOST, m_host);
	DDV_MaxChars(pDX, m_host, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCNCConnectionDlg, CDialog)
	//{{AFX_MSG_MAP(CCNCConnectionDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCNCConnectionDlg message handlers
