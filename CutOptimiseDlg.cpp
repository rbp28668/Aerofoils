// CutOptimiseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "CutOptimiseDlg.h"
#include "afxdialogex.h"


// CutOptimiseDlg dialog

IMPLEMENT_DYNAMIC(CutOptimiseDlg, CDialogEx)

CutOptimiseDlg::CutOptimiseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CUT_OPTIMISE, pParent)
	, optimiseOutput(FALSE)
	, resolution(0)
{

}

CutOptimiseDlg::~CutOptimiseDlg()
{
}

void CutOptimiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_OPTIMISE_CUT, optimiseOutput);
	DDX_Text(pDX, IDC_EDIT_RESOLUTION, resolution);
}


BEGIN_MESSAGE_MAP(CutOptimiseDlg, CDialogEx)
END_MESSAGE_MAP()


// CutOptimiseDlg message handlers
