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

#include <string>
#include "Position.h"

class Cutter;
class ParserContext;

class GCodeInterpreter
{
private:
	enum Units {
		MM,
		INCH
	};

	enum MoveSpeed {
		FASTEST,
		FEED_RATE
	};

	enum FeedRateType {
		UNITS_PER_SEC,   // default inches per sec or mm per sec
		UNITS_PER_MIN,   // values are inches per minute or mm per minute
		TARGET_TIME		 // value is desired time to complete the move (not supported).
	};

	enum MoveType {
		ABSOLUTE_MOVE,
		RELATIVE_MOVE
	};


	struct State {
		Units units;
		MoveSpeed moveSpeed;
		MoveType moveType;
		FeedRateType feedRateType;
		bool mirrored;
		long dwellTimeMS;
		Position<double> offsets;

		State();
		inline void zeroShift() { offsets.zero(); }
	};

	// The cutter we're controlling
	Cutter* pCutter;

	// Context for controlling program, reporting errors etc.
	ParserContext* pContext;

	// Machine state
	State* pCurrentState;

	// Parse outputs/bookeeping
	double x, y, u, v; // parse outputs from X,Y,U,V 
	double f;          // parse output from F
	int mS;         // parse output from P and S converted to mS

	typedef  int (GCodeInterpreter::*CommandFunction)();
	CommandFunction command;       // any command to be run once line is parsed
	CommandFunction motionCommand; // result of last G00 or G01.  Allows axis values to act as commands

	enum GeneralCommand {
		FAST_LINE = 0,			//G00 - straight line as fast as possible(interpolated)
		STRAIGHT_LINE = 1,		//G01 - straight line at given or active feed rate.
		DWELL = 4,				//G04 - dwell / pause for given time(P mS or S seconds)
		PLANE_XY = 17,			//G17 - use XY plane
		USE_INCHES = 20,		//G20 - program coordinates are in inches
		USE_MM = 21,			//G21 - program coordinates are in mm
		HOME = 28,				//G28 - return to home position
		MIRROR_ON = 38,			//G38 - mirror imaging “on”
		MIRROR_OFF = 39,		//G39 - mirror imaging “off”
		CUTTER_COMPENSATION_OFF = 40, //G40 SET CUTTER COMPENSATION - OFF
		CLEAR_TOOL_LENGTH_OFFSET = 49, //G49 clear existing tool length offset.
		WORKSHIFT = 52,			//G52 - set local workshift(offsets)
		CANCEL_WORKSHIFT = 53,	//G53 - cancel local workshift
		SET_PATH_CONTROL_CONSTANT_VELOCITY = 64, // G64 SET PATH CONTROL MODE - CONSTANT VELOCITY
		USE_ABSOLUTE = 90,		//G90 - Absolute programming
		USE_INCREMENTAL = 91,	//G91 - Incremental programming
		SET_FEED_RATE_UNITS_PER_MINUTE = 94, //G94 - SET FEED RATE MODE - UNITS PER MINUTE
	};

	enum MiscCommand {
		PROGRAM_STOP = 0,	//M00 - program stop
		OPTIONAL_STOP = 1,	//M01 - optional program stop
		END_PROGRAM = 2,	//M02 - end of program
		WIRE_ON = 3,		//M03 - wire on
		WIRE_OFF = 5,		// M05 - wire off
		ENABLE_MOTORS = 17, //M17 - Enable / Power all stepper motors
		DISABLE_MOTORS = 18, //M18 - Disable all stepper motors
		END_AND_RESTART = 30 // M30 - End program and reset to beginning
	};


public:
	GCodeInterpreter();
	~GCodeInterpreter();

	void setCutter(Cutter* pCutter);
	void unsetCutter();
	void setContext(ParserContext* pContext);
	void unsetContext();
	ParserContext* getContext() const;

	int process(const std::string& line);

	inline bool isInches() { return pCurrentState->units == INCH; }
	inline bool isFast() { return pCurrentState->moveSpeed == FASTEST; }
	inline bool isRelative() { return pCurrentState->moveType == RELATIVE_MOVE; }
	inline bool isMirrored() { return pCurrentState->mirrored; }
	inline Position<double> offsets() { return pCurrentState->offsets; }

private:
	size_t skipSpaces(const std::string& line, size_t idx, size_t length);
	size_t skipComment(const std::string& line, size_t idx, size_t length);
	size_t processCommand(const std::string& line, size_t idx, size_t length);
	size_t processMiscCommand(const std::string& line, size_t idx, size_t length);
	size_t processXAxis(const std::string& line, size_t idx, size_t length);
	size_t processYAxis(const std::string& line, size_t idx, size_t length);
	size_t processUAxis(const std::string& line, size_t idx, size_t length);
	size_t processVAxis(const std::string& line, size_t idx, size_t length);
	size_t processFeedRate(const std::string& line, size_t idx, size_t length);
	size_t processDwellTimeMilliSeconds(const std::string& line, size_t idx, size_t length);
	size_t processDwellTimeSeconds(const std::string& line, size_t idx, size_t length);

	size_t getIntValue(int& value, const std::string& line, size_t idx, size_t length);
	size_t getValue(double& value, const std::string& line, size_t idx, size_t length);

	int moveFast();
	int moveStraight();
	void calculateMove(Position<double> & target);
	int dwell();
	int home();
	int workshift();


};

