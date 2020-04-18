
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

#pragma once
#include "Kernel.h"
#include "Position.h"
#include "Steps.h"

class CutterGeometry;

class PointPlotter {
public:
	virtual bool plot(Position<long long> axes) = 0; // return true to terminate.
	virtual void setResolution(double xStepsPerMM, double yStepsPerMM) = 0;
	virtual void setCutterBounds(double xTravel, double yTravel) = 0;
};

class CutterSimulation
{
public:
	CutterSimulation(PointPlotter* plotter);
	~CutterSimulation();

	void blockToAxes(Position<double>& pos);
	void setGeometry(const CutterGeometry& geometry);

	void stepTo(double x, double y, double u, double v);
	Position<double> getPosition();
	void reset();

private:
	
	double xStepsPerMM;
	double yStepsPerMM;

	// Current position of axes
	Position<long long> axesPosition;
	
	// Derived values that describe block/cutter geometry
	double wd, // width of block (wr - wl)
		wl,  // left side of block measured from LHS of cutter
		wr,  // right side of block measrured from LHS of cutter
		wl1,  // left side of block measured from RHS of cutter
		wr1;  // right side of block measured from RHS of cutter

	Steps xSteps;
	Steps ySteps;
	Steps uSteps;
	Steps vSteps;

	PointPlotter* pPlotter;

	void moveSteppers(long long steps, Position<long long>& stepDeltas);

};

