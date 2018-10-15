// PlotPointUIProxy.cpp: implementation of the CPlotPointUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "aerofoil.h"
#include "PlotPointUIProxy.h"
#include "PlotPointDlg.h"
#include "AerofoilDoc.h"

#include "kernel/PointPlotter.h"
#include "kernel/ObjectSerializer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CObjectFactory<CPlotPointUIProxy> factory("plotPointUIProxy");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlotPointUIProxy::CPlotPointUIProxy()
{

}

CPlotPointUIProxy::~CPlotPointUIProxy()
{

}

const char* CPlotPointUIProxy::className() const
{
	return "plotPointUIProxy";
}

UINT CPlotPointUIProxy::getPopupID()
{
	assert(this);
	return IDR_PLOTPOINT_FLOATING;
}

bool CPlotPointUIProxy::Edit(CPlotStructure* toEdit)
{
	assert(this);
	assert(toEdit);
	CPointPlotter* ppp = static_cast<CPointPlotter*>(toEdit);
	CPlotPointDlg dlg(ppp->getPointStructure());
	return dlg.DoModal() == IDOK;
}

void CPlotPointUIProxy::SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel)
{
	assert(this);
	assert(pDoc);
	assert(sel);

	CPointPlotter* ppp = static_cast<CPointPlotter*>(sel);
	pDoc->selectPointStructure(ppp->getPointStructure());
}


