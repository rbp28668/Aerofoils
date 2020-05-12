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

#include <assert.h>
#include <cmath>
#include <algorithm>
#include "CutterSimulation.h"
#include "CutterGeometry.h"



// Class to simulate the cutter.  Manages virtual steppers, one per axis and does appropriate
// coordinate translations to drive the steppers.  By tracking virtual steppers it can then feed the 
// output into the point plotter after every step.
CutterSimulation::CutterSimulation(PointPlotter* plotter)
	:pPlotter(plotter),
	xStepsPerMM(4),  // assume roughly 100 DPI screen
	yStepsPerMM(4)
{
	assert(this);
	assert(plotter);
	plotter->setResolution(xStepsPerMM, yStepsPerMM);
}


CutterSimulation::~CutterSimulation()
{
}


void CutterSimulation::setGeometry(const CutterGeometry & geometry)
{
	pPlotter->setCutterBounds(geometry.getXTravel(), geometry.getYTravel());
	this->geometry = geometry;
}

void CutterSimulation::stepTo(double x, double y, double u, double v)
{
	// Work out the position that the actual axes have to move to for a given position on the block.
	Position<double> axes(x,y,u,v);
	geometry.blockToAxes(axes);

	// axes now contains the coordinates of where we want the motors to actually end up.
	// in practice, they'll get close as there's not infinite resolution.
	// So now work out the equivalent in steps
	Position<long long> target;
	target.x = llround(axes.x * xStepsPerMM);
	target.y = llround(axes.y * yStepsPerMM);
	target.u = llround(axes.u * xStepsPerMM);
	target.v = llround(axes.v * yStepsPerMM);

	Position<long long> stepDeltas(target);
	stepDeltas.sub(axesPosition);

	// step deltas now contains the steps we want to take to move the current
	// axesPosition to the new target position.

	// cut as fast as possible - no oversampling to control speed.
	long long longest = std::max(
		std::max(abs(stepDeltas.x), abs(stepDeltas.y)),
		std::max(abs(stepDeltas.u), abs(stepDeltas.v))
	);

	moveSteppers(longest, stepDeltas);

	axesPosition.set(target); // update current axes position to reflect this move.

}

Position<double> CutterSimulation::getPosition()
{
	Position<double> axes(
		axesPosition.x / xStepsPerMM,
		axesPosition.y / yStepsPerMM,
		axesPosition.u / xStepsPerMM,
		axesPosition.v / yStepsPerMM
		);
	return axes;
}

void CutterSimulation::reset()
{
	axesPosition.set(0, 0, 0, 0);
	xSteps.stop();
	ySteps.stop();
	uSteps.stop();
	vSteps.stop();
}

void CutterSimulation::moveSteppers(long long steps, Position<long long>& stepDeltas)
{
	int dirX = (stepDeltas.x >= 0) ? 1 : -1;
	int dirY = (stepDeltas.y >= 0) ? 1 : -1;
	int dirU = (stepDeltas.u >= 0) ? 1 : -1;
	int dirV = (stepDeltas.v >= 0) ? 1 : -1;

	stepDeltas.x = abs(stepDeltas.x);
	stepDeltas.y = abs(stepDeltas.y);
	stepDeltas.u = abs(stepDeltas.u);
	stepDeltas.v = abs(stepDeltas.v);
	
	xSteps.start(stepDeltas.x, steps);
	ySteps.start(stepDeltas.y, steps);
	uSteps.start(stepDeltas.u, steps);
	vSteps.start(stepDeltas.v, steps);

	while (steps-- > 0) {
		axesPosition.x += xSteps.step() ? dirX : 0;
		axesPosition.y += ySteps.step() ? dirY : 0;
		axesPosition.u += uSteps.step() ? dirU : 0;
		axesPosition.v += vSteps.step() ? dirV : 0;

		if (pPlotter->plot(axesPosition)) {
			break;
		}
	}
}

