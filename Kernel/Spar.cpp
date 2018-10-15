// Spar.cpp: implementation of the CSpar class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <sstream>
#include <string>

#include "Spar.h"
#include "ObjectSerializer.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpar::CSpar()
: spar_type(top)
, root_x(0.0)			/* distance of spar from le at root */
, tip_x(0.0)			/* ditto at tip */
, submerged(false)      /* true if under skin */
, root_width(6)			/* size of spar at root*/
, root_height(3.5)		/* size of spar at root*/
, tip_width(6)			/* size at tip */
, tip_height(3.5)		/* ditto */
{

}

CSpar::~CSpar()
{

}

const char* CSpar::getText()
{
	assert(this);

	ostringstream ost;
	switch(spar_type)
	{
	case top: ost << "top"; break;
	case bottom: ost << "bottom"; break;
	case full_depth: ost << "full depth"; break;
	default:
		assert(false);
		break;
	}
	ost << " at " << root_x << "/" << tip_x;

	text = ost.str();

	return text.c_str();
}

void CSpar::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("spar",this);

	char* psType = "???";
	switch(spar_type)
	{
	case top: psType = "top"; break;
	case bottom:psType =  "bottom"; break;
	case full_depth: psType =  "full depth"; break;
	default:
		assert(false);
		break;
	}
	os.write("type",psType);
	os.write("submerged",submerged);
	os.write("rootHeight",root_height);
	os.write("rootWidth", root_width);
	os.write("rootX", root_x);
	os.write("tipHeight",tip_height);
	os.write("tipWidth", tip_width);
	os.write("tipX", tip_x);
	os.endSection();
}

void CSpar::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("spar",this);

	string typeName;
	os.read("type",typeName);

	if(typeName.compare("top") == 0)
		spar_type = top;
	else if(typeName.compare("bottom") == 0)
		spar_type = bottom;
	else if(typeName.compare("full depth") == 0)
		spar_type = full_depth;
	else
	{
		// something bad happened
		assert(false);
		spar_type = none;
	}

	os.read("submerged",submerged);
	os.read("rootHeight",root_height);
	os.read("rootWidth", root_width);
	os.read("rootX", root_x);
	os.read("tipHeight",tip_height);
	os.read("tipWidth", tip_width);
	os.read("tipX", tip_x);
	os.endReadSection();

}

