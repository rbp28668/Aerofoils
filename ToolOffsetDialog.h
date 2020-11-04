#pragma once


// ToolOffsetDialog dialog

class ToolOffsetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ToolOffsetDialog)

public:
	ToolOffsetDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ToolOffsetDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOL_OFFSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double toolOffset;
	double tipToolOffset;
};
