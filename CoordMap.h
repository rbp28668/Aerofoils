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
// CoordMap.h: interface for the CCoordMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDMAP_H__1103E544_BB64_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_COORDMAP_H__1103E544_BB64_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Kernel/Kernel.h"

class PointT;
class RectT;

class CCoordMap  
{
public:
	// Use this one when you want an accurately scaled output.
	CCoordMap(NumericT sizex, NumericT sizey, CDC* pdc, NumericT zoom);

	// Use this one when you want to map from a given set of logical bounds to a given physical windows display rect
	CCoordMap(const RectT& logical, const RECT& physical);
	virtual ~CCoordMap();

	POINT toDevice(const PointT& pt) const;
	RECT  toDevice(const RectT& r) const;

	PointT toLogical(POINT pt);
private:
	NumericT scalex;   // factor for mm to pixels
	NumericT scaley;
	NumericT xoffset;
	NumericT yoffset;

};

#endif // !defined(AFX_COORDMAP_H__1103E544_BB64_11D6_AF75_002018BA082E__INCLUDED_)
