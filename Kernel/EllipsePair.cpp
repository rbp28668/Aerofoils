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
*/// EllipsePair.cpp: implementation of the CEllipsePair class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <sstream>
#include "EllipsePair.h"
#include "ObjectSerializer.h"

using namespace std;

const std::string CEllipsePair::TYPE("ellipsePair");
static CObjectFactory<CEllipsePair> factory(CEllipsePair::TYPE.c_str());

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipsePair::CEllipsePair()
{

}

CEllipsePair::CEllipsePair(const CEllipsePair& ep)
: CStructure(ep)
, first(ep.first)
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

std::string CEllipsePair::getType() const
{
	return TYPE;
}


void CEllipsePair::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection(TYPE.c_str(),this);
	CStructure::serializeTo(os);
	first.serializeTo(os);
	second.serializeTo(os);
	os.endSection();
}

void CEllipsePair::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection(TYPE.c_str(),this);
	CStructure::serializeFrom(os);
	first.serializeFrom(os);
	second.serializeFrom(os);
	os.endReadSection();
}
