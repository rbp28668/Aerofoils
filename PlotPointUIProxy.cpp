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


