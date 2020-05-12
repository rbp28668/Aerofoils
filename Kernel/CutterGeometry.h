#pragma once

#include "Kernel.h"
#include "Position.h"

class CObjectSerializer;

class CutterGeometry
{

	double width;		// of cutter
	double blockLeft;	// distance from LHS of cutter to LHS of block
	double blockRight;  // distance from LHS of cutter to RHS of block
	double xTravel;		// range of X movement each side
	double yTravel;		// range of Y movement each side

		// Derived values that describe block/cutter geometry
	double wd, // width of block (wr - wl)
		wl,  // left side of block measured from LHS of cutter
		wr,  // right side of block measrured from LHS of cutter
		wl1,  // left side of block measured from RHS of cutter
		wr1;  // right side of block measured from RHS of cutter

public:
	CutterGeometry();
	~CutterGeometry();

	double getWidth() const { return width; }
	void setWidth(double width);

	double getBlockLeft() const { return blockLeft; }
	void setBlockLeft(double blockLeft);

	double getBlockRight() const { return blockRight; }
	void setBlockRight(double blockRight);

	double getXTravel() const { return xTravel; }
	void setXTravel(double xTravel);

	double getYTravel() const { return yTravel; }
	void setYTravel(double yTravel);

	void serializeTo(CObjectSerializer & os);
	void serializeFrom(CObjectSerializer & os);

	void blockToAxes(Position<double>& pos);
	void setGeometry();

};

