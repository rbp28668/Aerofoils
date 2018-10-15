// CoordMap.h: interface for the CCoordMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDMAP_H__1103E544_BB64_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_COORDMAP_H__1103E544_BB64_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PointT;
class RectT;

class CCoordMap  
{
public:
	CCoordMap(float sizex, float sizey, CDC* pdc, float zoom);
	virtual ~CCoordMap();

	POINT toDevice(const PointT& pt);
	RECT  toDevice(const RectT& r);

	PointT toLogical(POINT pt);
private:
	float scalex;   // factor for mm to pixels
	float scaley;
	float xoffset;
	float yoffset;

};

#endif // !defined(AFX_COORDMAP_H__1103E544_BB64_11D6_AF75_002018BA082E__INCLUDED_)
