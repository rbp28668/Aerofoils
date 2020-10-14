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
*/// Transform.h: interface for the CTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFORM_H__EF095242_B4C9_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_TRANSFORM_H__EF095242_B4C9_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointT.h"

class CObjectSerializer;

// CTransform is used to scale a raw aerofoil section with a 
// chord 0..1 to the actual size, thickness and washout that's 
// on the wing.
class CTransform  
{
public:
	CTransform();
	virtual ~CTransform();

	PointT transform(PointT pos) const;

	void setChord(NumericT _chord);
	void setSweep(NumericT _sweep);
	void setWashout(NumericT _washout);
	void setHeight(NumericT _height);
	void setInvert(bool _invert);


	NumericT getChord() const {return chord;}
	NumericT getSweep() const {return sweep;}
	NumericT getWashout() const {return washout;}
	NumericT getHeight() const {return height;}

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:

	void set_rotate(NumericT degrees);

	NumericT chord;          /* chord in mm */
	NumericT sweep;          /* offset of x=0 from datum */
	NumericT washout;        /* in degrees, +ve is washout -ve washin */
	NumericT height;         /* height of l/e above base (in mm) */
	bool invert;

	struct ROTATE
	{
		NumericT cos_theta;
		NumericT sin_theta;
	};

	ROTATE rotate;
};

#endif // !defined(AFX_TRANSFORM_H__EF095242_B4C9_11D6_AF75_002018BA082E__INCLUDED_)
