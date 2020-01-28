/* Aerofoil
Aerofoil plotting and CNC cutter driver
Kernel / core algorithms
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
*/// RectT.h: interface for the RectT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTT_H__06CE6422_BB77_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_RECTT_H__06CE6422_BB77_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointT.h"

class RectT  
{
public:
	RectT(float l = 0.0f, float t = 0.0f, float r = 0.0f, float b = 0.0f);
	virtual ~RectT();

	PointT topLeft;
	PointT bottomRight;

};

#endif // !defined(AFX_RECTT_H__06CE6422_BB77_11D6_AF75_002018BA082E__INCLUDED_)
