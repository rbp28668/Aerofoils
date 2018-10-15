// WingUIProxy.cpp: implementation of the CWingUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "aerofoil.h"
#include "WingUIProxy.h"
#include "WingDlg.h"
#include "AerofoilDoc.h"
#include "kernel/ObjectSerializer.h"
#include "kernel/PlotCommonImpl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CObjectFactory<CWingUIProxy> factory("wingUIProxy");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWingUIProxy::CWingUIProxy()
{

}

CWingUIProxy::~CWingUIProxy()
{

}

const char* CWingUIProxy::className() const
{
	return "wingUIProxy";
}


UINT CWingUIProxy::getPopupID()
{
	assert(this);
	return IDR_WING_FLOATING;
}

bool CWingUIProxy::Edit(CPlotStructure* toEdit)
{
	assert(this);
	assert(toEdit);

	CPlotCommonImpl* ps = static_cast<CPlotCommonImpl*>(toEdit);
	CWingDlg dlg(ps->getWing());
	return dlg.DoModal() == IDOK;
}

void CWingUIProxy::SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel)
{
	assert(this);
	assert(pDoc);
	assert(sel);

	CPlotCommonImpl* ps = static_cast<CPlotCommonImpl*>(sel);
	pDoc->selectWing(ps->getWing());

}