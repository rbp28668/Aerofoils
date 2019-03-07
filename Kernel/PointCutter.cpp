/* Aerofoil
Aerofoil plotting and CNC cutter driver
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
*/

#include "PointCutter.h"
#include <assert.h>
#include <sstream>
#include "PointStructure.h"
#include "ObjectSerializer.h"
#include "OutputDevice.h"

static CObjectFactory<PointCutter> factory("pointCutter");

PointCutter::PointCutter()
	: pStructure(0)
	, fast(false)
{
}

PointCutter::PointCutter(CPointStructure* pps)
	: pStructure(pps)
	, fast(false)
{
	assert(pps);
}


PointCutter::~PointCutter()
{
}

bool PointCutter::isFast() const
{
	return fast;
}

void PointCutter::setFast(bool fast)
{
	assert(this);
	this->fast = fast;
}

void PointCutter::cut(COutputDevice * pdev, double toolOffset)
{
	assert(this);
	assert(pdev);

	if (fast) {
		move(pdev, pStructure->getRoot(), pStructure->getTip());
	}
	else {
		line(pdev, pStructure->getRoot(), pStructure->getTip());
	}
}

std::string PointCutter::getDescriptiveText() const
{
	std::stringstream ss;

	ss << ((fast) ? "Move to point" : "Cut to point") << std::ends;
	return ss.str();
}

CStructure* PointCutter::getStructure()
{
	assert(this);
	assert(pStructure);
	return pStructure;
}

const CStructure* PointCutter::getStructure() const
{
	assert(this);
	assert(pStructure);
	return pStructure;
}

void PointCutter::serializeTo(CObjectSerializer& os)
{
	assert(this);
	assert(pStructure);
	os.startSection("pointCutter", this);
	CutStructure::serializeTo(os);
	os.writeReference("structure", pStructure);
	os.endSection();
}

void PointCutter::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("pointCutter", this);
	CutStructure::serializeFrom(os);
	pStructure = static_cast<CPointStructure*>(os.readReference("structure"));
	os.endReadSection();
}
