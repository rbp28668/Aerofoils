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