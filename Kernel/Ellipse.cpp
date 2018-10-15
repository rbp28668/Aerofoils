/*
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
	os.endReadSection();
}
