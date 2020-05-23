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

CWindowsOutputDevice::CWindowsOutputDevice(NumericT sx, NumericT sy, CDC* pdc, float zoom)
: map(sx, sy, pdc, zoom)
, plotDC(pdc)
, penBlack(0)
, penGrey(0)
{
	penGrey = new CPen(PS_SOLID, 0, RGB(128, 128, 128));
	penBlack = new CPen(PS_SOLID, 0, RGB(0,0,0));
	penOriginal = pdc->SelectObject(penBlack);
}

CWindowsOutputDevice::~CWindowsOutputDevice()
{
	plotDC->SelectObject(penOriginal);
	delete penBlack;
	delete penGrey;
}

void CWindowsOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if (drawMoves) {
		plotDC->SelectObject(penGrey);
		if (iStream != lastStream)	// then device pos incorrect
		{
			plotDC->MoveTo(map.toDevice(last[iStream]));
		}
		plotDC->LineTo(map.toDevice(pt));
	}
	else {
		plotDC->MoveTo(map.toDevice(pt));
	}
	lastStream = iStream;
	last[iStream] = pt;

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

	if (drawMoves) {
		plotDC->SelectObject(penBlack);
	}

	plotDC->LineTo(map.toDevice(pt));
}

void CWindowsOutputDevice::Label(int iStream, const char* psz)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	POINT pt = map.toDevice(last[iStream]);
	if (drawMoves) {
		plotDC->SelectObject(penBlack);
	}
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

PointT CWindowsOutputDevice::position(int iStream)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	return last[iStream];
}


void CWindowsOutputDevice::startObject(const char* description, bool selected)
{
	if(selected)
	{
		plotDC->SelectObject(penGrey);
	}
}

void CWindowsOutputDevice::endObject(const char* description, bool selected)
{
	if(selected)
	{
		plotDC->SelectObject(penBlack);
	}
}
