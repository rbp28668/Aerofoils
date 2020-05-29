#pragma once
#include <afxwin.h>

class CWing;
class AerofoilPreviewControl :
	public CStatic
{
	DECLARE_MESSAGE_MAP()
	bool isRoot;
	CWing* pWing;

public:
	AerofoilPreviewControl(CWing* pWing, bool isRoot);
	afx_msg void OnPaint();
};

