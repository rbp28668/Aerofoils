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
#include "Position.h"

class Cutter
{
public:
	virtual ~Cutter() = 0 {}

	virtual void fastMove(const Position<double>& deltas) = 0;
	virtual void cutMove(const Position<double>& deltas) = 0;
	virtual void dwell(int mS) = 0;
	virtual void home() = 0;
	virtual void wireOn() = 0;
	virtual void wireOff() = 0;
	virtual void enableMotors() = 0;
	virtual void disableMotors() = 0;
	virtual void setFeedRate(double rate) = 0;
	virtual double getFeedRate() = 0;
	virtual bool hasFeedRateError() = 0;
	virtual void resetFeedRateError() = 0;

	virtual Position<double> getCurrentPosition() = 0;


};

