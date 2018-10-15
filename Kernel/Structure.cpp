// Structure.cpp: implementation of the CStructure class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "Structure.h"
#include "ObjectSerializer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStructure::CStructure()
: span(1000.0f)
{

}

CStructure::~CStructure()
{

}

void CStructure::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("structure",this);
	os.write("span",span);
	os.endSection();
}

void CStructure::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("structure",this);
	os.read("span",span);
	os.endReadSection();
}

