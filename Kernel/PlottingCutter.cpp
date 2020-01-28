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

#include <assert.h>
#include "PlottingCutter.h"
#include "OutputDevice.h"
#include "PointT.h"

PlottingCutter::PlottingCutter(COutputDevice* pdev)
	: pOutput(pdev)
{
	assert(this);
	assert(pdev);
}


PlottingCutter::~PlottingCutter()
{
}

void PlottingCutter::fastMove(const Position<double>& deltas)
{
	assert(this);
	currentPosition.add(deltas);
	pOutput->LineTo(0, PointT(currentPosition.x, currentPosition.y));
	pOutput->LineTo(1, PointT(currentPosition.u, currentPosition.v));
}

void PlottingCutter::cutMove(const Position<double>& deltas)
{
	assert(this);
	currentPosition.add(deltas);
	pOutput->LineTo(0, PointT(currentPosition.x, currentPosition.y));
	pOutput->LineTo(1, PointT(currentPosition.u, currentPosition.v));
}

void PlottingCutter::home()
{
	assert(this);
	currentPosition.set(0, 0, 0, 0);
}

Position<double> PlottingCutter::getCurrentPosition()
{
	assert(this);
	return currentPosition;
}

void PlottingCutter::setInitialPosition(double x, double y, double u, double v)
{
	currentPosition.set(x, y, u, v);
}
