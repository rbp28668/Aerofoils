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
#include "cutter.h"

// A dummy CNC cutter used to validate G-Code.  Interpreter/program 
// can then send cutting commands to this rather than the real cuter.
class DummyCutter :
	public Cutter
{
public:
	DummyCutter();
	virtual ~DummyCutter();

	virtual void fastMove(const Position<double>& deltas)  {}
	virtual void cutMove(const Position<double>& deltas)  {}
	virtual void dwell(int mS)  {}
	virtual void home()  {}
	virtual void wireOn()  {}
	virtual void wireOff()  {}
	virtual void enableMotors()  {}
	virtual void disableMotors()  {}
	virtual void setFeedRate(double rate)  {}
	virtual double getFeedRate() { return 0.0; }
	virtual bool hasFeedRateError() { return false; }
	virtual void resetFeedRateError()  {}

	virtual Position<double> getCurrentPosition() { return Position<double>(); }

};

