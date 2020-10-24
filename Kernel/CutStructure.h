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
#include "Kernel.h"
#include "StructureOutput.h"
#include "CutStructure.h"
#include "PointT.h"


class CObjectSerializer;
class COutputDevice;
class CStructure;
class Bounds;


// CutStructure is the analogue for PlotStructure except that it's designed to provide a common interface to items
// that will drive a CNC machine.


class CutStructure : public StructureOutput
{
	PointT rootOffsets;  // Offsets in XYZ of where we put the root
	PointT tipOffsets;   // offsets in XYZ of where we put the tip
	bool invert;         // invert this plot if true
	bool reflect;        // reflect front to back if true
	bool rootIsOnLeft;   // swap root and tip if true
	int root_stream;     // stream to use for root
	int tip_stream;      // stream to use for tip

	PointT rotateOrigin;
	double rotateRadians;
	double rotateSin;
	double rotateCos;

	Bounds* pBounds;
	
public:
	virtual void move(COutputDevice *pdev, const PointT& root, const PointT& tip);
	virtual void line(COutputDevice *pdev, const PointT& root, const PointT& tip);
protected:
	void transform(PointT& r, PointT& t) const;
	void updateBounds();

public:
	class Context {
	public:
		double toolOffset;
		bool optimiseOutput;
		double tolerance;

		Context() : toolOffset(0.5), optimiseOutput(true), tolerance(0.005) {}
		void serializeTo(CObjectSerializer& os) const;
		void serializeFrom(CObjectSerializer& os);

	};
	
	CutStructure();
	explicit CutStructure(const CutStructure& source);

	~CutStructure();

	virtual void cut(COutputDevice *pdev, const CutStructure::Context& context) = 0;

	virtual std::string getDescriptiveText() const = 0; // For UI
	virtual std::string getType() const = 0; // for serialization, basic type identification etc.
	virtual CStructure* getStructure() = 0;
	virtual const CStructure* getStructure() const = 0;
	virtual CutStructure* clone() const = 0;

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

	void setRotation(PointT origin, double angleRadians);

	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

};

