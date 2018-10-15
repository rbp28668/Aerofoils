// EllipsePair.cpp: implementation of the CEllipsePair class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <sstream>
#include "EllipsePair.h"
#include "ObjectSerializer.h"

using namespace std;

static CObjectFactory<CEllipsePair> factory("ellipsePair");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipsePair::CEllipsePair()
{

}

CEllipsePair::CEllipsePair(const CEllipsePair& ep)
: first(ep.first)
, second(ep.second)
{
}

CEllipsePair::~CEllipsePair()
{

}

string CEllipsePair::getDescriptiveText() const
{
	assert(this);
	ostringstream os;
	os << "Ellipses " 
		<< getFirst()->getB() << ","
		<< getFirst()->getATop() << "/"
		<< getFirst()->getABottom() << ":"
		<< getSecond()->getB() << ","
		<< getSecond()->getATop() << "/"
		<< getSecond()->getABottom() << ends;

	return os.str();
}


void CEllipsePair::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("ellipsePair",this);
	CStructure::serializeTo(os);
	first.serializeTo(os);
	second.serializeTo(os);
	os.endSection();
}

void CEllipsePair::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("ellipsePair",this);
	CStructure::serializeFrom(os);
	first.serializeFrom(os);
	second.serializeFrom(os);
	os.endReadSection();
}
