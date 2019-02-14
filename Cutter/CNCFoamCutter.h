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

#include "Cutter.h"
#include "Position.h"

class CutterHardware;

class CNCFoamCutter : public Cutter {

	// Description of the hardware, motors etc
	double width; // of cutter
	double xLead; // lead of x leadscrews in mm per rev
	double yLead; // lead of y leadscrews in mm per rev
	int xStepsPerRev; // basic steps per rev of x steppers
	int yStepsPerRev; // basic steps per rev of y steppers
	int xMicroStep;  // micro-stepping factor on x steppers (1,2,4,8 etc...)
	int yMicroStep; // micro-stepping factor on y steppers
	double blockLeft;  // distance from LHS of cutter to LHS of block
	double blockRight;  // distance from LHS of cutter to RHS of block
	double stepFrequency; // of steppers in Hz

	Position<double> currentPosition;  // on side of block
	Position<long long> axesPosition;     // of hardware axes in microsteps

	CutterHardware* pHardware;
	double feedRate;		// desired feedrate in mm per sec.
	boolean feedRateError;  // set true if not able to meet set feedrate.

	// Parameters for calculating motor position from a given position on the edges]
	// of the foam block.  set up by calling updateBlockInfo()
	double wl;   // left of block measured from LHS of cutter
	double wr;	 // right of block measured from LHS of cutter
	double wl1;  // left of block measured from RHS of cutter
	double wr1;  // right of block measured from RHS of cutter
	double wd;   // width of block (wr - l)

	void blockToAxes(Position<double>& pos);
	void updateBlockInfo(); // call when blockLeft, blockRight or width are changed;

public:
	CNCFoamCutter(CutterHardware* pHardware);
	virtual ~CNCFoamCutter();

	// Config
	inline double getWidth() { return width; }
	void setWidth(double width);
	inline double getXLead() { return xLead; }
	void setXLead(double lead);
	inline double getYLead() { return yLead; }
	void setYLead(double lead);
	inline int getXStepsPerRev(){ return xStepsPerRev; }
	void setXStepsPerRev(int steps);
	inline int getYStepsPerRev() { return yStepsPerRev; }
	void setYStepsPerRev(int steps);
	inline int getXMicroStepping() { return xMicroStep; }
	void setXMicroStepping(int steps);
	inline int getYMicroStepping() { return yMicroStep; }
	void setYMicroStepping(int steps);
	inline double getBlockLeft() { return blockLeft; }
	void setBlockLeft(double side);
	inline double getBlockRight() { return blockRight; }
	void setBlockRight(double side);
	inline double getStepFrequency() { return stepFrequency; }
	void setStepFrequency(double hz);

	virtual double getFeedRate();
	virtual void setFeedRate(double feedRate);
	// Set if we can't achieve desired feed rate.
	virtual boolean hasFeedRateError();
	virtual void resetFeedRateError();

	virtual Position<double> getCurrentPosition();// { return currentPosition; }
	virtual Position<long long> getAxesPosition();// { return axesPosition; }


	// Operations
	virtual void fastMove(const Position<double>& deltas);
	virtual void cutMove(const Position<double>& deltas);
	virtual void dwell(int mS);
	virtual void home();
	virtual void wireOn();
	virtual void wireOff();
	virtual void enableMotors();
	virtual void disableMotors();

};