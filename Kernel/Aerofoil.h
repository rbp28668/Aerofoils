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
*/
// Aerofoil.h: interface for the CAerofoil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEROFOIL_H__BBA1B6B9_B27A_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_AEROFOIL_H__BBA1B6B9_B27A_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "Kernel.h"
#include "ParametricCurve2D.h"

class CSpline;
class CObjectSerializer;

class CAerofoil : public ParametricCurve2D
{
public:
	CAerofoil(const char* path, NumericT tmod);
	explicit CAerofoil(const CAerofoil& rhs);
	explicit CAerofoil(); // for serialization
	virtual ~CAerofoil();

	// raw coordinate
	PointT operator[](int idx) const; // gets point on aerofoil.

	// parametric curve
	PointT Point(NumericT u) const;
	PointT Point(NumericT u, PointT& tangent) const;
	NumericT FirstX(NumericT req_x, NumericT start, int dirn) const;

	void modifyThickness(NumericT tm);
	NumericT getThicknessModifier() const {return thick_mod;}

	void read(const char* path);

	std::string getName() const;
	std::string getSection() const {return section;}

	bool operator == (const CAerofoil& rhs) const;
	bool operator != (const CAerofoil& rhs) const {return !(*this == rhs);}
	CAerofoil& operator = (const CAerofoil& rhs);

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);



private:

	void copy(const CAerofoil& rhs);

	void createSpline();

	NumericT thick_mod;      /* thickness modifier, 1.0 = no mod */

	CSpline* spline;      /* spline for smooth interpolation - allows parametric curve */
	
	std::string section;  /* file name of section */
	std::string name;	  /* section name */
	PointT *list;         /* coordinate array */
	int npts;             /* number of points in above */

};

#endif // !defined(AFX_AEROFOIL_H__BBA1B6B9_B27A_11D6_AF75_002018BA082E__INCLUDED_)
