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
*/// PointT.h: interface for the PointT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTT_H__A4E22B61_B474_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_POINTT_H__A4E22B61_B474_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PointT  
{
public:
	PointT(float x = 0.0f, float y = 0.0f);
	~PointT();

	float fx;
	float fy;

};

#endif // !defined(AFX_POINTT_H__A4E22B61_B474_11D6_AF75_002018BA082E__INCLUDED_)
