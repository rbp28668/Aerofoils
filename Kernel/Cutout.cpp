
#include <assert.h>
#include <sstream>
#include "Cutout.h"
#include "ObjectSerializer.h"


Cutout::Cutout()
	: topSurface(false)
	, centre(true)
	, root_x(0)
	, tip_x(0)
	, root_y(0)
	, tip_y(0)
	, root_width(0)
	, tip_width(0)
	, root_height(0)
	, tip_height(0)
{
}

const char* Cutout::getText()
{
	assert(this);

	std::ostringstream ost;
	ost << "Cutout at " << root_x << "/" << tip_x;

	text = ost.str();
	return text.c_str();
}

void Cutout::serializeTo(CObjectSerializer& os)
{
	os.startSection("cutout", this);
	os.write("topSurface", topSurface);      // true if going in from the top service.  
	os.write("centre", centre);          // true if ignore y_position and centre in wing
	os.write("rootx", root_x);         /* distance of cutout from le at root */
	os.write("tipx", tip_x);          /* ditto at tip */
	os.write("rooty", root_y);
	os.write("tipy", tip_y);
	os.write("rootWidth", root_width);     // in mm of cutout
	os.write("tipWidth", tip_width);
	os.write("rootHeight", root_height);
	os.write("tipHeight", tip_height);
	os.endSection();
}

void Cutout::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection("cutout", this);
	os.read("topSurface", topSurface);      // true if going in from the top service.  
	os.read("centre", centre);          // true if ignore y_position and centre in wing
	os.read("rootx", root_x);         /* distance of cutout from le at root */
	os.read("tipx", tip_x);          /* ditto at tip */
	os.read("rooty", root_y);
	os.read("tipy", tip_y);
	os.read("rootWidth", root_width);     // in mm of cutout
	os.read("tipWidth", tip_width);
	os.read("rootHeight", root_height);
	os.read("tipHeight", tip_height);
	os.endReadSection();
}
