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
Title:          ELLIPSE.C

Function:       plotting ellipses

Author:         Bruce

Date:           28 March 92

Directory:

Calling sequence:

Input parameters:

Output parameters:

Description:

Non-library routines used:

Possible error codes returned:

Revision history:

@*/

#include <assert.h>
#include "Ellipse.h"
#include "ObjectSerializer.h"


CEllipse::CEllipse()
: a_top(20)
, a_bottom(20)
, b(10)
, centre_x(0)
, centre_y(0)
{
}

CEllipse::~CEllipse()
{
}

void CEllipse::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("ellipse",this);
	os.write("aBottom", a_bottom);
	os.write("aTop", a_top);
	os.write("b", b);
	os.write("centreX", centre_x);
	os.write("centreY", centre_y);
	os.endSection();
}

void CEllipse::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("ellipse",this);
	os.read("aBottom", a_bottom);
	os.read("aTop", a_top);
	os.read("b", b);
	os.read("centreX", centre_x);
	os.read("centreY", centre_y);
}
