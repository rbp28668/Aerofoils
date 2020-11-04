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

	struct BoundsT {
		NumericT minx, maxx, miny, maxy, minz, maxz;
		BoundsT();
		void add(NumericT x, NumericT y, NumericT z);
		void reset();
	};

public:

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz) {};
	virtual void Home() {};
	virtual void Flush() {};
	virtual PointT position(int iStream) { return PointT(); }

	RectT getBounds() const;
	RectT getPlanBounds() const;
	NumericT width() const { return overall.maxx - overall.minx; }
	NumericT height() const { return overall.maxy - overall.miny; }
	NumericT depth() const { return overall.maxz - overall.minz; }
	NumericT getMinx() const { return overall.minx; }
	NumericT getMaxx() const { return overall.maxx; }
	NumericT getMiny() const { return overall.miny; }
	NumericT getMaxy() const { return overall.maxy; }
	NumericT getMinz() const { return overall.minz; }
	NumericT getMaxz() const { return overall.maxz; }

	const BoundsT& rootBounds() const { return root; }
	const BoundsT& tipBounds() const { return tip; }

	const PointT& firstRoot() const { return first[0]; }
	const PointT& firstTip() const { return first[1]; }
	const PointT& lastRoot() const { return last[0]; }
	const PointT& lastTip() const { return last[1]; }

	void reset();

private:
	BoundsT overall;
	BoundsT root;
	BoundsT tip;

	PointT lastMove[2];
	PointT first[2];
	PointT last[2];
	bool lastOpIsMove[2];
	bool isFirstLine[2];

};

