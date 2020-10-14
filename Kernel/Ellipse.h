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
*/#ifndef ELLIPSE_H_INCLUDED
#define ELLIPSE_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Kernel.h"

class CObjectSerializer;

class CEllipse
{
public:
	CEllipse();
	virtual ~CEllipse();

	float getATop() const {return a_top;}
	float getABottom() const {return a_bottom;}
	float getB() const {return b;}
	float getX() const {return centre_x;}
	float getY() const {return centre_y;}

	void setATop(float rhs)  { a_top = rhs;}
	void setABottom(float rhs)  { a_bottom = rhs;}
	void setB(float rhs)  { b = rhs;}
	void setX(float rhs)  { centre_x = rhs;}
	void setY(float rhs)  { centre_y = rhs;}
	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:
	float a_top;
	float a_bottom;
	float b;
	float centre_x;
	float centre_y;
//	float start_theta;
//	float end_theta;
};

#endif