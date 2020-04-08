#pragma once

#include "Kernel.h"

class CObjectSerializer;

class CutterGeometry
{

	double width;		// of cutter
	double blockLeft;	// distance from LHS of cutter to LHS of block
	double blockRight;  // distance from LHS of cutter to RHS of block
	double xTravel;		// range of X movement each side
	double yTravel;		// range of Y movement each side

public:
	CutterGeometry();
	~CutterGeometry();

	double getWidth() const { return width; }
	void setWidth(double width) { this->width = width; }

	double getBlockLeft() const { return blockLeft; }
	void setBlockLeft(double blockLeft) { this->blockLeft = blockLeft; }

	double getBlockRight() const { return blockRight; }
	void setBlockRight(double blockRight) { this->blockRight = blockRight; }

	double getXTravel() const { return xTravel; }
	void setXTravel(double xTravel) { this->xTravel = xTravel; }

	double getYTravel() const { return yTravel; }
	void setYTravel(double yTravel) { this->yTravel = yTravel; }

	void serializeTo(CObjectSerializer & os);
	void serializeFrom(CObjectSerializer & os);
};

