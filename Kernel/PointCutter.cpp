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

const std::string PointCutter::TYPE("pointCutter");
static CObjectFactory<PointCutter> factory(PointCutter::TYPE.c_str());

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

void PointCutter::cut(COutputDevice * pdev, const CutStructure::Context& context)
{
	assert(this);
	assert(pdev);

	PointT r = pStructure->getRoot();
	PointT t = pStructure->getTip();
	r.fz = 0;
	t.fz = pStructure->getSpan();

	if (fast) {
		move(pdev, r, t);
	}
	else {
		line(pdev, r, t);
	}
}

std::string PointCutter::getDescriptiveText() const
{
	std::stringstream ss;
	ss.precision(1);

	PointT root = pStructure->getRoot();
	PointT tip = pStructure->getTip();
	transform(root, tip);

	ss << ((fast) ? "Move to point" : "Cut to point") 
		<<  std::fixed 
		<< " [" <<  root.fx << "," << root.fy << "],["
		<< tip.fx << "," << tip.fy << "]"
		<< std::ends;
	return ss.str();
}

std::string PointCutter::getType() const
{
	return TYPE;
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
	os.startSection(TYPE.c_str(), this);
	CutStructure::serializeTo(os);
	os.write("fast", fast);
	os.writeReference("structure", pStructure);
	os.endSection();
}

void PointCutter::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection(TYPE.c_str(), this);
	CutStructure::serializeFrom(os);
	if (os.ifExists("fast")) {
		os.read("fast", fast);
	}
	pStructure = static_cast<CPointStructure*>(os.readReference("structure"));
	os.endReadSection();
	updateBounds();
}
