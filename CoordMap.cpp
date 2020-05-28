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
// CoordMap.cpp: implementation of the CCoordMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>

#include "Aerofoil.h"
#include "CoordMap.h"

#include "kernel\PointT.h"
#include "kernel\RectT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoordMap::CCoordMap(NumericT sizex, NumericT sizey, CDC* pdc, NumericT zoom)
{
	assert(this);
	assert(pdc);

	scalex = NumericT(pdc->GetDeviceCaps(LOGPIXELSX)); // pixels per inch
	scaley = NumericT(pdc->GetDeviceCaps(LOGPIXELSY));

	scalex /= 25.4f;	// to pixels per mm.
	scaley /= 25.4f;

	scalex *= zoom;
	scaley *= zoom;

	xoffset = 0.0f;
	yoffset = sizey;

}

CCoordMap::CCoordMap(const RectT& logical, const RECT& physical)
{
	scalex = (physical.right - physical.left) / (logical.bottomRight.fx - logical.topLeft.fx);
	scaley = (physical.bottom - physical.top) / (logical.topLeft.fy - logical.bottomRight.fy);
	NumericT scale = min(scalex, scaley);

	// If section doesn't fill the box in vertical direction want to centre it in y (don't care about x)
	yoffset = 0;
	if (scale < scaley) { // then doesn't fit all physical box in y direction
		// So initialise yoffset with half the difference between what you're allowed (converted to logical coordinates)
		// and what you've actually got.
		yoffset = ((physical.bottom - physical.top) / scale - (logical.topLeft.fy - logical.bottomRight.fy) ) / 2;
	}

	scalex = scaley = scale;

	xoffset = logical.topLeft.fx;
	yoffset += logical.topLeft.fy;
}

CCoordMap::~CCoordMap()
{

}

POINT CCoordMap::toDevice(const PointT& pt)
{
	POINT np;
	np.x = int((pt.fx - xoffset) * scalex + 0.5f);
	np.y = int((yoffset - pt.fy) * scaley + 0.5f);
	return np;
}

RECT CCoordMap::toDevice(const RectT& r)
{
	POINT tl = toDevice(r.topLeft);
	POINT br = toDevice(r.bottomRight);
	CRect nr = CRect(tl,br);
	nr.NormalizeRect();
	return nr;
}

PointT CCoordMap::toLogical(POINT pt)
{
	PointT np;
	np.fx = pt.x / scalex;
	np.fx += xoffset;
	np.fy = pt.y / scaley;
	np.fy = yoffset - np.fy;
	return np;
}
