#pragma once
#include "stdafx.h"

// CutOptimiseDlg dialog

class CutOptimiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CutOptimiseDlg)

public:
	CutOptimiseDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CutOptimiseDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUT_OPTIMISE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL optimiseOutput;
	double resolution;
};
