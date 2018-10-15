// WindowsUIProxy.h: interface for the CWindowsUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSUIPROXY_H__7C73CD16_C566_11D6_9554_00047660C89A__INCLUDED_)
#define AFX_WINDOWSUIPROXY_H__7C73CD16_C566_11D6_9554_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "kernel/UIProxy.h"

class CPlotStructure;
class CPlot;
class CAerofoilDoc;

class CWindowsUIProxy : public CUIProxy  
{
public:
	CWindowsUIProxy();
	virtual ~CWindowsUIProxy();

	virtual UINT getPopupID() = 0;
	virtual bool Edit(CPlotStructure* toEdit) = 0;
	virtual void SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel) = 0;
	virtual bool EditPlotFlags(CPlotStructure* toEdit);

};

#endif // !defined(AFX_WINDOWSUIPROXY_H__7C73CD16_C566_11D6_9554_00047660C89A__INCLUDED_)
