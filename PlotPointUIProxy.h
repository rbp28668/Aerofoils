// PlotPointUIProxy.h: interface for the CPlotPointUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOTPOINTUIPROXY_H__462C60A7_C8A0_11D6_955C_000000000000__INCLUDED_)
#define AFX_PLOTPOINTUIPROXY_H__462C60A7_C8A0_11D6_955C_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WindowsUIProxy.h"

class CPlotPointUIProxy : public CWindowsUIProxy  
{
public:
	CPlotPointUIProxy();
	virtual ~CPlotPointUIProxy();

	virtual UINT getPopupID();
	virtual bool Edit(CPlotStructure* toEdit);
	virtual void SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel);

	virtual const char* className() const;

};

#endif // !defined(AFX_PLOTPOINTUIPROXY_H__462C60A7_C8A0_11D6_955C_000000000000__INCLUDED_)
