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
#include <string>
#include "CutStructure.h"
#include "PointT.h"


class CObjectSerializer;
class COutputDevice;
class CStructure;
class Bounds;


// CutStructure is the analogue for PlotStructure except that it's designed to provide a common interface to items
// that will drive a CNC machine.


class CutStructure 
{
	PointT rootOffsets;
	PointT tipOffsets;
	bool invert;
	bool reflect;
	bool rootIsOnLeft;
	int root_stream;
	int tip_stream;

	Bounds* pBounds;
	
public:
	void move(COutputDevice *pdev, const PointT& root, const PointT& tip);
	void line(COutputDevice *pdev, const PointT& root, const PointT& tip);
protected:
	void transform(PointT& r, PointT& t);
	void updateBounds();

public:
	CutStructure();
	~CutStructure();

	virtual void cut(COutputDevice *pdev, double toolOffset) = 0;

	virtual std::string getDescriptiveText() const = 0;
	virtual CStructure* getStructure() = 0;
	virtual const CStructure* getStructure() const = 0;

	void setOffsets(const PointT& root, const PointT& tip);
	const PointT getRootOffsets();
	const PointT getTipOffsets();

	void setRootSide(bool isLeft);
	bool isRootLeft() const { return rootIsOnLeft; }
	int getRootStream() const { return root_stream; }
	int getTipStream() const { return tip_stream; }

	void setInvert(bool inv);
	bool isInverted() const { return invert; }

	void setReflect(bool ref);
	bool isReflected() const { return reflect; }

	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

};

