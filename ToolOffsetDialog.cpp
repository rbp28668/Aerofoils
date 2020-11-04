// ToolOffsetDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Aerofoil.h"
#include "ToolOffsetDialog.h"
#include "afxdialogex.h"


// ToolOffsetDialog dialog

IMPLEMENT_DYNAMIC(ToolOffsetDialog, CDialogEx)

ToolOffsetDialog::ToolOffsetDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TOOL_OFFSET, pParent)
	, toolOffset(0)
	, tipToolOffset(0)
{

}

ToolOffsetDialog::~ToolOffsetDialog()
{
}

void ToolOffsetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOOL_OFFSET, toolOffset);
	DDV_MinMaxDouble(pDX, toolOffset, 0, 10);
	DDX_Text(pDX, IDC_EDIT_TIP_TOOL_OFFSET, tipToolOffset);
	DDV_MinMaxDouble(pDX, tipToolOffset, 0, 10);
}


BEGIN_MESSAGE_MAP(ToolOffsetDialog, CDialogEx)
END_MESSAGE_MAP()


// ToolOffsetDialog message handlers


