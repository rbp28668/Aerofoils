// EllipseUIProxy.cpp: implementation of the CEllipseUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "aerofoil.h"
#include "EllipseUIProxy.h"
#include "EllipsePairDlg.h"
#include "AerofoilDoc.h"

#include "kernel/ObjectSerializer.h"
#include "kernel/EllipsePlotter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CObjectFactory<CEllipseUIProxy> factory("ellipseUIProxy");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipseUIProxy::CEllipseUIProxy()
{

}

CEllipseUIProxy::~CEllipseUIProxy()
{

}

const char* CEllipseUIProxy::className() const
{
	return "ellipseUIProxy";
}

UINT CEllipseUIProxy::getPopupID()
{
	assert(this);
	return IDR_ELLIPSE_FLOATING;
}

bool CEllipseUIProxy::Edit(CPlotStructure* toEdit)
{
	assert(this);
	CEllipsePlotter* pep = static_cast<CEllipsePlotter*>(toEdit);
	CEllipsePairDlg dlg(pep->getEllipses());
	return dlg.DoModal() == IDOK;
}

void CEllipseUIProxy::SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel)
{
	assert(this);
	assert(pDoc);
	assert(sel);
	CEllipsePlotter* pep = static_cast<CEllipsePlotter*>(sel);
	pDoc->selectEllipses(pep->getEllipses());
}
