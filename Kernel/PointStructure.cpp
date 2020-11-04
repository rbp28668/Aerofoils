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
*/// PointStructure.cpp: implementation of the CPointStructure class.
//
//////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <sstream>

#include "PointStructure.h"
#include "ObjectSerializer.h"

using namespace std;

const std::string CPointStructure::TYPE("pointStructure");
static CObjectFactory<CPointStructure> factory(CPointStructure::TYPE.c_str());

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointStructure::CPointStructure()
{
}

CPointStructure::CPointStructure(const CPointStructure& point)
: CStructure(point)
, root(point.root)
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
	os << "Point " << label << " (" << root.fx << "," << root.fy << "),(" 
		<< tip.fx << "," << tip.fy << ")" << ends;
	return os.str();
}

std::string CPointStructure::getType() const
{
	return TYPE;
}

void CPointStructure::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection(TYPE.c_str(),this);
	CStructure::serializeTo(os);
	os.write("rootX",root.fx);
	os.write("rootY",root.fy);
	os.write("tipX", tip.fx);
	os.write("tipY", tip.fy);
	os.write("label", label.c_str());
	os.endSection();
}

void CPointStructure::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection(TYPE.c_str(),this);
	CStructure::serializeFrom(os);
	os.read("rootX",root.fx);
	os.read("rootY",root.fy);
	os.read("tipX", tip.fx);
	os.read("tipY", tip.fy);
	if (os.ifExists("label")) {
		os.read("label", label);
	}
	else {
		label = "";
	}
	os.endReadSection();
}

