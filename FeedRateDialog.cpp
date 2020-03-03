// FeedRateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "FeedRateDialog.h"
#include "afxdialogex.h"


// FeedRateDialog dialog

IMPLEMENT_DYNAMIC(FeedRateDialog, CDialogEx)

FeedRateDialog::FeedRateDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FEED_RATE, pParent)
	, feedRate(0)
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
}


BEGIN_MESSAGE_MAP(FeedRateDialog, CDialogEx)
END_MESSAGE_MAP()


// FeedRateDialog message handlers
