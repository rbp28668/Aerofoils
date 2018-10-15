// PointStructure.cpp: implementation of the CPointStructure class.
//
//////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <sstream>

#include "PointStructure.h"
#include "ObjectSerializer.h"

using namespace std;

static CObjectFactory<CPointStructure> factory("pointStructure");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointStructure::CPointStructure()
{
}

CPointStructure::CPointStructure(const CPointStructure& point)
: root(point.root)
, tip(point.tip)
{
}

CPointStructure::~CPointStructure()
{
}

string CPointStructure::getDescriptiveText() const
{
	assert(this);
	ostringstream os;
	os << "Point (" << root.fx << "," << root.fy << "),(" 
		<< tip.fx << "," << tip.fy << ")" << ends;
	return os.str();
}

void CPointStructure::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("pointStructure",this);
	CStructure::serializeTo(os);
	os.write("rootX",root.fx);
	os.write("rootY",root.fy);
	os.write("tipX", tip.fx);
	os.write("tipY", tip.fy);
	os.endSection();
}

void CPointStructure::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("pointStructure",this);
	CStructure::serializeFrom(os);
	os.read("rootX",root.fx);
	os.read("rootY",root.fy);
	os.read("tipX", tip.fx);
	os.read("tipY", tip.fy);
	os.endReadSection();
}

