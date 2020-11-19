#include "stdafx.h"
#include "DXFUIProxy.h"
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

#include "DXFUIProxy.h"
#include "AerofoilDoc.h"
#include "Kernel/DXFPlotter.h"
#include "kernel/ObjectSerializer.h"
#include "resource.h"
//#include "kernel/DXFPlotter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

static CObjectFactory<DXFUIProxy> factory("dxfUIProxy");

DXFUIProxy::DXFUIProxy()
{
}

DXFUIProxy::~DXFUIProxy()
{
}

const char* DXFUIProxy::className() const
{
	return "dxfUIProxy";
}

UINT DXFUIProxy::getPopupID()
{
	return IDR_DXF_FLOATING;
}

bool DXFUIProxy::Edit(CPlotStructure* toEdit)
{
	return false;
}

void DXFUIProxy::SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel)
{
	DXFPlotter* ps = static_cast<DXFPlotter*>(sel);
	pDoc->selectDxfObject(ps->getDxfObject());

}
