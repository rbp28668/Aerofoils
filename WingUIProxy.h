// WingUIProxy.h: interface for the CWingUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINGUIPROXY_H__7C73CD17_C566_11D6_9554_00047660C89A__INCLUDED_)
#define AFX_WINGUIPROXY_H__7C73CD17_C566_11D6_9554_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WindowsUIProxy.h"

class CWingUIProxy : public CWindowsUIProxy  
{
public:
	CWingUIProxy();
	virtual ~CWingUIProxy();

	const char* className() const; // use default serialization

	virtual UINT getPopupID();
	virtual bool Edit(CPlotStructure* toPlot);
	virtual void SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel);

};

#endif // !defined(AFX_WINGUIPROXY_H__7C73CD17_C566_11D6_9554_00047660C89A__INCLUDED_)
