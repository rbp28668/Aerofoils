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

#include <limits>
#include <assert.h>
#include "Bounds.h"


Bounds::Bounds()
{
	reset();
}

Bounds::~Bounds()
{
}

void Bounds::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	lastMove[iStream] = pt;
	lastOpIsMove[iStream] = true;
}
void Bounds::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	if (lastOpIsMove[iStream])
	{
		lastOpIsMove[iStream] = false;
		LineTo(iStream, lastMove[iStream]);
	}

	assert(pt.fx != std::numeric_limits<float>::infinity());
	assert(pt.fy != std::numeric_limits<float>::infinity());
	assert(pt.fz != std::numeric_limits<float>::infinity());
	assert(pt.fx != -std::numeric_limits<float>::infinity());
	assert(pt.fy != -std::numeric_limits<float>::infinity());
	assert(pt.fz != -std::numeric_limits<float>::infinity());

	overall.add(pt.fx, pt.fy, pt.fz);
	if (iStream == 0) {
		root.add(pt.fx, pt.fy, pt.fz);
	}
	else {
		tip.add(pt.fx, pt.fy, pt.fz);
	}

	// Capture the first and last lines.
	if (isFirstLine[iStream]) {
		first[iStream] = pt;
		isFirstLine[iStream] = false;
	}
	last[iStream] = pt;
}

RectT Bounds::getBounds() const
{
	RectT bounds(overall.minx, overall.maxy, overall.maxx, overall.miny);
	return bounds;
}

RectT Bounds::getPlanBounds() const
{
	RectT bounds(overall.minz, overall.maxx, overall.maxz, overall.minx);
	return bounds;
}

void Bounds::reset()
{
	overall.reset();
	root.reset();
	tip.reset();
	lastOpIsMove[0] = lastOpIsMove[1] = false;
	isFirstLine[0] = isFirstLine[1] = true;

	lastMove[0] = lastMove[1] = PointT();
	first[0] = first[1] = PointT();
	last[0] = last[1] = PointT();

}

Bounds::BoundsT::BoundsT()
{
}

void Bounds::BoundsT::add(NumericT x, NumericT y, NumericT z)
{
	if (x < minx) minx = x;
	if (x > maxx) maxx = x;
	if (y < miny) miny = y;
	if (y > maxy) maxy = y;
	if (z < minz) minz = z;
	if (z > maxz) maxz = z;

}

void Bounds::BoundsT::reset()
{
	minx = miny = minz = std::numeric_limits<NumericT>::max();
	maxx = maxy = maxz = -std::numeric_limits<NumericT>::max();
}
