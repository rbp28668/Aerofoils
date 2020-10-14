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

#pragma once
#include "Kernel.h"
#include "OutputDevice.h"
#include "RectT.h"

// Bounds class - plot/cut to this device and it will record the bounding box
// of the plot.
class Bounds :
	public COutputDevice
{
public:
	Bounds();
	~Bounds();

public:

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz) {};
	virtual void Home() {};
	virtual void Flush() {};
	virtual PointT position(int iStream) { return PointT(); }

	RectT getBounds() const;
	RectT getPlanBounds() const;
	NumericT width() const { return maxx - minx; }
	NumericT height() const { return maxy - miny; }
	NumericT depth() const { return maxz - minz; }
	NumericT getMinx() const { return minx; }
	NumericT getMaxx() const { return maxx; }
	NumericT getMiny() const { return miny; }
	NumericT getMaxy() const { return maxy; }
	NumericT getMinz() const { return minz; }
	NumericT getMaxz() const { return maxz; }

	void reset();

private:
	NumericT minx, maxx, miny, maxy, minz, maxz;
	PointT lastMove[2];
	bool lastOpIsMove[2];

};

