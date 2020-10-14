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

	if (pt.fx < minx) minx = pt.fx;
	if (pt.fx > maxx) maxx = pt.fx;
	if (pt.fy < miny) miny = pt.fy;
	if (pt.fy > maxy) maxy = pt.fy;
	if (pt.fz < minz) minz = pt.fz;
	if (pt.fz > maxz) maxz = pt.fz;
}

RectT Bounds::getBounds() const
{
	RectT bounds(minx, maxy, maxx, miny);
	return bounds;
}

RectT Bounds::getPlanBounds() const
{
	RectT bounds(minz, maxx, maxz, minx);
	return bounds;
}

void Bounds::reset()
{
	minx = miny = minz = std::numeric_limits<NumericT>::max();
	maxx = maxy = maxz = -std::numeric_limits<NumericT>::max();
	lastOpIsMove[0] = lastOpIsMove[1] = false;
	lastMove[0] = lastMove[1] = PointT();
}

