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
#include "Cutter.h"

class COutputDevice;

/*
PlottingCutter - allows output from the GCode interpreter to be plotted directly to an output device.
*/
class PlottingCutter :
	public Cutter
{
	Position<double> currentPosition;
	COutputDevice* pOutput;

public:
	PlottingCutter(COutputDevice* pdev);
	virtual ~PlottingCutter();

	virtual void fastMove(const Position<double>& deltas);
	virtual void cutMove(const Position<double>& deltas);
	virtual void dwell(int mS) {}
	virtual void home();
	virtual void wireOn() {}
	virtual void wireOff() {}
	virtual void enableMotors() {}
	virtual void disableMotors() {}
	virtual void setFeedRate(double rate) {}
	virtual double getFeedRate() { return 0; }
	virtual bool hasFeedRateError() { return false; }
	virtual void resetFeedRateError() {}
	virtual void stop() {}

	virtual Position<double> getCurrentPosition();

	void setInitialPosition(double x, double y, double u, double v);


};

