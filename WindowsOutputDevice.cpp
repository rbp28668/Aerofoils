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
// PlotProxy.cpp: implementation of the CWindowsOutputDevice class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <assert.h>
#include "Aerofoil.h"
#include "WindowsOutputDevice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWindowsOutputDevice::CWindowsOutputDevice(float sx, float sy, CDC* pdc, float zoom)
: map(sx, sy, pdc, zoom)
, plotDC(pdc)
, selected(0)
, penBlack(0)
, penGrey(0)
{
	
}

CWindowsOutputDevice::~CWindowsOutputDevice()
{
	delete penGrey;
}

void CWindowsOutputDevice::setSelection(CPlotStructure* sel)
{
	selected = sel;
	if(sel)
	{
		penGrey = new CPen(PS_SOLID,0,RGB(128,128,128));
		//penBlack = plotDC->SelectObject(penGrey);
	}
}


void CWindowsOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	lastStream = iStream;
	last[iStream] = pt;

	plotDC->MoveTo(map.toDevice(pt));
}

void CWindowsOutputDevice::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if(iStream != lastStream)	// then device pos incorrect
	{
		plotDC->MoveTo(map.toDevice(last[iStream]));
	}

	lastStream = iStream;
	last[iStream] = pt;

	plotDC->LineTo(map.toDevice(pt));
}

void CWindowsOutputDevice::Label(int iStream, const char* psz)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	POINT pt = map.toDevice(last[iStream]);
	plotDC->TextOut(pt.x, pt.y ,CString(psz));
}

void CWindowsOutputDevice::Home()
{
	assert(this);
	last[0] = last[1] = PointT(0,0);
	lastStream = 0;
	plotDC->MoveTo(map.toDevice(last[0]));
}

void CWindowsOutputDevice::Flush()
{
	assert(this);
}


void CWindowsOutputDevice::startObject(CPlotStructure* ps)
{
	if(ps == selected)
	{
		penBlack = plotDC->SelectObject(penGrey);
	}
}

void CWindowsOutputDevice::endObject(CPlotStructure* ps)
{
	if(ps == selected)
	{
		plotDC->SelectObject(penBlack);
	}
}
