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

CCoordMap::CCoordMap(float sizex, float sizey, CDC* pdc, float zoom)
{
	assert(this);
	assert(pdc);

	scalex = float(pdc->GetDeviceCaps(LOGPIXELSX)); // pixels per inch
	scaley = float(pdc->GetDeviceCaps(LOGPIXELSY));

	scalex /= 25.4f;	// to pixels per mm.
	scaley /= 25.4f;

	scalex *= zoom;
	scaley *= zoom;

	xoffset = 0.0f;
	yoffset = sizey;

}

CCoordMap::~CCoordMap()
{

}

POINT CCoordMap::toDevice(const PointT& pt)
{
	POINT np;
	np.x = int(pt.fx * scalex + 0.5f);
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
	np.fy = pt.y / scaley;
	np.fy = yoffset - np.fy;
	return np;
}
