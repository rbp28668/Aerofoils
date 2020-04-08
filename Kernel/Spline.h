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
*//*
File:         SPLINE.H

Program/rev.: foil 0.0

Function:     Header for parametric spline curves

Author:       Bruce (spline & splint from NRIC)

Date:         1991 

Routines:

Input Parameters:

Output Parameters:

Error conditions:

Description:

Revision History:

*/

#ifndef _SPLINE_H
#define _SPLINE_H

#include "Kernel.h"

class PointT;

class CSpline
{
public:

	CSpline(PointT* curve, int n);
	~CSpline();
	bool Bad() const {return m_blBad;}
	PointT Point(NumericT u) const;
	PointT Point(NumericT u, PointT& tangent) const;
	PointT Tangent(NumericT u) const;
	NumericT FirstX(NumericT req_x, NumericT start, int dirn) const;

private:

	// Forbid copying
	CSpline(const CSpline& rhs) {}
	CSpline& operator= (const CSpline& rhs) {return *this;}
	
	void spline(NumericT *x,NumericT *y,int n, NumericT *y2);
	void splint(NumericT *xa, NumericT *ya, NumericT *y2a, int n, NumericT x, NumericT *y,NumericT *dydx) const;

	bool m_blBad;
	NumericT *xx;        /* array of x coordinates */
	NumericT *yy;        /* array of y coordinates */
	NumericT *uu;        /* array of parametric coordinates */
	NumericT *xx2;       /* 2nd deriv of x wrt u */
	NumericT *yy2;       /* 2nd deriv of y wrt u */
	int npts;       /* number of points on curve */
};

#endif
