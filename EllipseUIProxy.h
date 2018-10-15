// EllipseUIProxy.h: interface for the CEllipseUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIPSEUIPROXY_H__7C73CD18_C566_11D6_9554_00047660C89A__INCLUDED_)
#define AFX_ELLIPSEUIPROXY_H__7C73CD18_C566_11D6_9554_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WindowsUIProxy.h"

class CEllipseUIProxy : public CWindowsUIProxy  
{
public:
	CEllipseUIProxy();
	virtual ~CEllipseUIProxy();

	const char* className() const; // use default serialization

	virtual UINT getPopupID();
	virtual bool Edit(CPlotStructure* toEdit);
	virtual void SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel);


};

#endif // !defined(AFX_ELLIPSEUIPROXY_H__7C73CD18_C566_11D6_9554_00047660C89A__INCLUDED_)
