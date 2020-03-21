/* Cutter
Copyright(C) 2019 R Bruce Porteous

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

#include "StdAfx.h"
#include <assert.h>
#include "CutterHardware.h"
#include "CNCFoamCutter.h"

void CNCFoamCutter::blockToAxes(Position<double>& pos)
{
	double x = pos.x;
	double y = pos.y;
	double u = pos.u;
	double v = pos.v;

	pos.x = (x*wr - u*wl) / wd;
	pos.u = (u*wl1 - x*wr1) / wd;
	pos.y = (y*wr - v*wl) / wd;
	pos.v = (v*wl1 - y*wr1) / wd;

}

void CNCFoamCutter::updateBlockInfo()
{
	wl = blockLeft;
	wr = blockRight;

	wl1 = width - wl;
	wr1 = width - wr;
	wd = wr - wl;
}

void CNCFoamCutter::validate(int status)
{
	if (!CutterHardware::isValid(status)) {
		if (CutterHardware::isCommsError(status)) {
			throw CutterException("Communications failure to hardware");
		}
		else {
			throw CutterException("Invalid response from hardware");
		}
	}
}

CNCFoamCutter::CNCFoamCutter(CutterHardware* ph)
	: pHardware(ph),
	width(1000.0),
	xLead(2.0),
	yLead(2.0),
	xStepsPerRev(200),
	yStepsPerRev(200),
	xMicroStep(4),
	yMicroStep(4),
	blockLeft(0),
	blockRight(1000.0),
	stepFrequency(2500),
	feedRate(2.0), // arbitrary 2mm per sec
	feedRateError(false)
{
	assert(this);
	assert(ph);

	updateBlockInfo();
}

CNCFoamCutter::~CNCFoamCutter()
{
	pHardware = 0;
}


void CNCFoamCutter::setWidth(double width)
{
	assert(this);
	assert(!isnan(width));
	assert(width > 0.0);
	this->width = width;
	updateBlockInfo();
}

void CNCFoamCutter::setXLead(double lead)
{
	assert(this);
	assert(!isnan(lead));
	assert(lead > 0.0);
	this->xLead = lead;
}

void CNCFoamCutter::setYLead(double lead)
{
	assert(this);
	assert(!isnan(lead));
	assert(lead > 0.0);
	this->yLead = lead;
}

void CNCFoamCutter::setXStepsPerRev(int steps)
{
	assert(this);
	assert(steps > 0);
	this->xStepsPerRev = steps;
}

void CNCFoamCutter::setYStepsPerRev(int steps)
{
	assert(this);
	assert(steps > 0);
	this->yStepsPerRev = steps;
}

void CNCFoamCutter::setXMicroStepping(int steps)
{
	assert(this);
	assert(steps > 0);
	this->xMicroStep = steps;
}

void CNCFoamCutter::setYMicroStepping(int steps)
{
	assert(this);
	assert(steps > 0);
	this->yMicroStep = steps;
}

void CNCFoamCutter::setBlockLeft(double side)
{
	assert(this);
	assert(!isnan(side));
	assert(side >= 0.0 && side <= width);
	assert(side < blockRight);
	this->blockLeft = side;
}

void CNCFoamCutter::setBlockRight(double side)
{
	assert(this);
	assert(!isnan(side));
	assert(side >= 0.0 && side <= width);
	assert(side > blockLeft);
	this->blockRight = side;
}

void CNCFoamCutter::setStepFrequency(double hz)
{
	assert(this);
	assert(!isnan(hz));
	assert(hz > 0.0);
	this->stepFrequency = hz;
}

bool CNCFoamCutter::hasFeedRateError()
{
	return feedRateError;
}

void CNCFoamCutter::resetFeedRateError()
{
	feedRateError = false;
}

Position<double> CNCFoamCutter::getCurrentPosition()
{
	return currentPosition;
}

Position<long long> CNCFoamCutter::getAxesPosition()
{
	return axesPosition;
}

void CNCFoamCutter::fastMove(const Position<double> & deltas)
{
	currentPosition.add(deltas);  // new current position;
	Position<double> axes(currentPosition);
	blockToAxes(axes);

	// axes now contains the coordinates of where we want the motors to actually end up.
	// in practice, they'll get close as there's not infinite resolution.
	// So now work out the equivalent in steps
	Position<long long> target;
	target.x = llround( (axes.x / xLead) * xStepsPerRev * xMicroStep);
	target.y = llround( (axes.y / yLead) * yStepsPerRev * yMicroStep);
	target.u = llround( (axes.u / xLead) * xStepsPerRev * xMicroStep);
	target.v = llround( (axes.v / yLead) * yStepsPerRev * yMicroStep);

	Position<long long> stepDeltas(target);
	stepDeltas.sub(axesPosition);

	// step deltas now contains the steps we want to take to move the current
	// axesPosition to the new target position.

	// cut as fast as possible - no oversampling to control speed.
	long long longest = max(
		max(abs(stepDeltas.x), abs(stepDeltas.y)), 
		max(abs(stepDeltas.u), abs(stepDeltas.v))
	);
	int status = pHardware->line((CutterHardware::AxisT)longest, 
		(CutterHardware::AxisT)stepDeltas.x, (CutterHardware::AxisT)stepDeltas.y, 
		(CutterHardware::AxisT)stepDeltas.u, (CutterHardware::AxisT)stepDeltas.v );
	validate(status);
	axesPosition.set(target); // update current axes position to reflect this move.
	
}

void CNCFoamCutter::cutMove(const Position<double> & deltas)
{
	currentPosition.add(deltas);  // new current position;
	Position<double> axes(currentPosition);
	blockToAxes(axes);

	// axes now contains the coordinates of where we want the motors to actually end up.
	// in practice, they'll get close as there's not infinite resolution.
	// So now work out the equivalent in steps
	Position<long long> target;
	target.x = llround((axes.x / xLead) * xStepsPerRev * xMicroStep);
	target.y = llround((axes.y / yLead) * yStepsPerRev * yMicroStep);
	target.u = llround((axes.u / xLead) * xStepsPerRev * xMicroStep);
	target.v = llround((axes.v / yLead) * yStepsPerRev * yMicroStep);

	Position<long long> stepDeltas(target);
	stepDeltas.sub(axesPosition);

	// Figure out speeds. The longest move is the important one.  The distance we care
	// about is one the edge of the block (not the axes themselves).
	double distLeft = sqrt(deltas.x * deltas.x + deltas.y * deltas.y);
	double distRight = sqrt(deltas.u * deltas.u + deltas.v * deltas.v);
    double dist = max(distLeft, distRight);

	// step deltas now contains the steps we want to take to move the current
	// axesPosition to the new target position.

	// Work out number of steps.
	long long longest = max(
		max(abs(stepDeltas.x), abs(stepDeltas.y)), 
		max(abs(stepDeltas.u), abs(stepDeltas.v))
	);
	double t = longest / stepFrequency;

	// now we've got distance, and the minimal time needed to drive the motors hence can work out speed.
	// Note that we can increase the time (slow down the cut) by increasing the number of steps but 
	// we can't decrease them by going below "longest".
	double speed = dist / t;

	long long steps;
	if (speed > feedRate) {
		steps = llround((longest * speed) / feedRate);
		if (steps < longest) { // make absolutely sure!
			steps = longest;
		}
	}
	else { // can't meet desired feedrate so go as fast as possible.
		steps = longest;
		feedRateError = true;
	}

	int status = pHardware->line((CutterHardware::AxisT)steps, 
		(CutterHardware::AxisT)stepDeltas.x, (CutterHardware::AxisT)stepDeltas.y, 
		(CutterHardware::AxisT)stepDeltas.u, (CutterHardware::AxisT)stepDeltas.v);
	validate(status);

	axesPosition.set(target); // update current axes position to reflect this move.

}

void CNCFoamCutter::dwell(int mS)
{
	// use line with 0 x,y,u,v and pick steps to give delay
	double t = (double)mS / 1000.0; // in seconds.
	double pulses = stepFrequency * t;
	int status = pHardware->line(lround(pulses),0,0,0,0);
	validate(status);
}

void CNCFoamCutter::home()
{
	currentPosition.zero();
	axesPosition.zero();
	int status = pHardware->home();
	validate(status);
}

void CNCFoamCutter::wireOn()
{
	int status = pHardware->wireOn();
	validate(status);
}

void CNCFoamCutter::wireOff()
{
	int status = pHardware->wireOff();
	validate(status);
}

void CNCFoamCutter::enableMotors()
{
	int status = pHardware->enable();
	validate(status);
}

void CNCFoamCutter::disableMotors()
{
	int status = pHardware->disable();
	validate(status);
}

void CNCFoamCutter::stop()
{
	int status = pHardware->abortNow();
	validate(status);
}

double CNCFoamCutter::getFeedRate()
{
	return feedRate;
}

void CNCFoamCutter::setFeedRate(double rate)
{
	feedRate = rate;
}
