#include <assert.h>
#include "CutterSimulationOutputDevice.h"
#include "CutterSimulation.h"
#include "PointT.h"


CutterSimulationOutputDevice::CutterSimulationOutputDevice(CutterSimulation* cutter)
	: x(0), y(0), u(0), v(0)
	, hasLeft(false), hasRight(false)
	, pCutter(cutter)
{
	assert(cutter);

}


CutterSimulationOutputDevice::~CutterSimulationOutputDevice()
{
	pCutter = 0;
}

void CutterSimulationOutputDevice::MoveTo(int iStream, const PointT & pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if (iStream == 0) {
		x = pt.fx;
		y = pt.fy;
		hasLeft = true;
	}
	if (iStream == 1) {
		u = pt.fx;
		v = pt.fy;
		hasRight = true;
	}
	if (hasLeft && hasRight) {
		pCutter->stepTo(x, y, u, v);
		hasLeft = hasRight = false;
	}

}

void CutterSimulationOutputDevice::LineTo(int iStream, const PointT & pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if (iStream == 0) {
		x = pt.fx;
		y = pt.fy;
		hasLeft = true;
	}
	if (iStream == 1) {
		u = pt.fx;
		v = pt.fy;
		hasRight = true;
	}
	if (hasLeft && hasRight) {
		pCutter->stepTo(x, y, u, v);
		hasLeft = hasRight = false;
	}

}

void CutterSimulationOutputDevice::Label(int iStream, const char * psz)
{
	// NOP
}

void CutterSimulationOutputDevice::Home()
{
	x = y = u = v = 0;
	pCutter->stepTo(0, 0, 0, 0);
}

void CutterSimulationOutputDevice::Flush()
{
	// NOP
}

PointT CutterSimulationOutputDevice::position(int iStream)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	Position<double> position = pCutter->getPosition();

	if (iStream == 0) {
		return PointT(position.x, position.y);
	}
	else {
		return PointT(position.u, position.v);
	}
}

void CutterSimulationOutputDevice::reset()
{
	x = y = u = v = 0;
	hasLeft = hasRight = false;
}
