/* Aerofoil
Aerofoil plotting and CNC cutter driver
Copyright(C) 1995-2019 R Bruce Porteous

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
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
