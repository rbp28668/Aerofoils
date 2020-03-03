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
#include <ctype.h>
#include "Cutter.h"
#include "ParserContext.h"
#include "GCodeInterpreter.h"


GCodeInterpreter::State::State() :
	units(MM),
	moveSpeed(FASTEST),
	moveType(ABSOLUTE_MOVE),
	mirrored(false),
	dwellTimeMS(0)
{
	assert(this);
}

GCodeInterpreter::GCodeInterpreter() :
	pCutter(0),
	pContext(0),
	x(0),y(0),u(0),v(0),
	f(0),
	mS(0)
{
	assert(this);
	pCurrentState = new State();
}


GCodeInterpreter::~GCodeInterpreter()
{
	assert(this);
	assert(pCurrentState);
	delete pCurrentState;
}

void GCodeInterpreter::setCutter(Cutter * pCutter)
{
	assert(this);
	assert(pCutter);
	this->pCutter = pCutter;
}

void GCodeInterpreter::unsetCutter()
{
}

void GCodeInterpreter::setContext(ParserContext* pContext) {
	assert(this);
	assert(pContext);
	this->pContext = pContext;
}

void GCodeInterpreter::unsetContext()
{
	assert(this);
	assert(pContext); // assume error if not already set.
	pContext = 0;
}

ParserContext * GCodeInterpreter::getContext() const
{
	assert(this);
	assert(pContext);
	return pContext;
}


int GCodeInterpreter::process(const std::string & line)
{
	assert(this);
	size_t length = line.length();
	size_t idx = 0;
	int status = 0;
	command = 0;

	try {
		while (idx < length) {
			char ch = line[idx++];

			if (isspace(ch)) {
				idx = skipSpaces(line, idx, length);
				continue;
			}

			switch (toupper(ch)) {
			case '(':
				idx = skipComment(line, idx, length);
				break;
			case 'G':
				idx = processCommand(line, idx, length);
				break;
			case 'M':
				idx = processMiscCommand(line, idx, length);
				break;
			case 'X':
				idx = processXAxis(line, idx, length);
				break;
			case 'Y':
				idx = processYAxis(line, idx, length);
				break;
			case 'U':
				idx = processUAxis(line, idx, length);
				break;
			case 'V':
				idx = processVAxis(line, idx, length);
				break;
			case 'F':
				idx = processFeedRate(line, idx, length);
				break;
			case 'P':
				idx = processDwellTimeMilliSeconds(line, idx, length);
				break;
			case 'S':
				idx = processDwellTimeSeconds(line, idx, length);
				break;

			default:
				if (pContext) {
					std::string err = "Unknown command: <";
					err.append(1, ch).append(">");
					pContext->showError(line, idx, err);
					idx = length; // abort processing.
				}
				status = 1;
			}
		}

		// now line is parsed have we a command to run? If so, do it...
		if (command) {
			status = ((*this).*(command))();
			command = 0; // as now run.
		}

	}
	catch (std::exception & e) {
		if (pContext) {
			std::string err(e.what());
			pContext->showError(line, idx, err);
			idx = length; // abort processing.
		}
		status = 1;
		throw; 
	}

	return status;
}

size_t GCodeInterpreter::skipSpaces(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	while (idx < length && isspace(line[idx])) {
		++idx;
	}
	return idx;
}

size_t GCodeInterpreter::skipComment(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	while (idx < length && (line[idx] != ')') ) {
		++idx;
	}
	if (idx < length && line[idx] == ')') {
		++idx;
	}
	return idx;
}

size_t GCodeInterpreter::processCommand(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	int commandIndex = -1;
	idx = getIntValue(commandIndex, line, idx, length);
	switch (commandIndex) {

	case FAST_LINE:			//G00 - straight line as fast as possible(interpolated)
		pCurrentState->moveSpeed = FASTEST;
		command = motionCommand = &GCodeInterpreter::moveFast;
		break;

	case STRAIGHT_LINE:		//G01 - straight line at given or active feed rate.
		pCurrentState->moveSpeed = FEED_RATE;
		command = motionCommand = &GCodeInterpreter::moveStraight;
		break;

	case DWELL:				//G04 - dwell / pause for given time(P mS or S seconds)
		command = &GCodeInterpreter::dwell;
		break;

	case USE_INCHES:		//G20 - program coordinates are in inches
		if (pCurrentState->units == MM) {
			x /= 25.4;
			y /= 25.4;
			u /= 25.4;
			v /= 25.4;
			f /= 25.4;
		}
		pCurrentState->units = INCH;
		break;

	case USE_MM:			//G21 - program coordinates are in mm
		if (pCurrentState->units == INCH) {
			x *= 25.4;
			y *= 25.4;
			u *= 25.4;
			v *= 25.4;
			f *= 25.4;
		}
		pCurrentState->units = MM;
		break;

	case HOME:				//G28 - return to home position
		command = &GCodeInterpreter::home;
		break;

	case MIRROR_ON:			//G38 - mirror imaging “on”
		pCurrentState->mirrored = true;
		break;

	case MIRROR_OFF:		//G39 - mirror imaging “off”
		pCurrentState->mirrored = false;
		break;

	case WORKSHIFT:			//G52 - set local workshift(offsets)
		command = &GCodeInterpreter::workshift;
		break;

	case CANCEL_WORKSHIFT:	//G53 - cancel local workshift
		pCurrentState->zeroShift();
		break;

	case USE_ABSOLUTE:		//G90 - Absolute programming
		pCurrentState->moveType = ABSOLUTE_MOVE;
		break;

	case USE_INCREMENTAL:	//G91 - Incremental programming
		pCurrentState->moveType = RELATIVE_MOVE;
		break;
		
	default:
		if (pContext) {
			pContext->showError(line, idx, "Unknown G command");
		}
		break;
	}

	return idx;
}

size_t GCodeInterpreter::processMiscCommand(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	assert(pCurrentState);

	int commandIndex = -1;
	idx = getIntValue(commandIndex, line, idx, length);
	switch (commandIndex) {

	case PROGRAM_STOP:	//M00 - program stop
		if (pContext) {
			pContext->pause();
		}
		break;

	case OPTIONAL_STOP:	//M01 - optional program stop
		if (pContext && pContext->canPause()) {
			pContext->pause();
		}
		break;

	case END_PROGRAM:	//M02 - end of program
		if (pContext) {
			pContext->complete();
		}
		break;

	case WIRE_ON:		//M03 - wire on
		pCutter->wireOn();
		break;

	case WIRE_OFF:		// M05 - wire off
		pCutter->wireOff();
		break;

	case ENABLE_MOTORS: //M17 - Enable / Power all stepper motors
		pCutter->enableMotors();
		break;

	case DISABLE_MOTORS: //M18 - Disable all stepper motors
		pCutter->disableMotors();
		break;

	case END_AND_RESTART: //M30 - End program and reset to beginning
		if (pContext) {
			pContext->complete();
			pContext->restart();
		}

		break;

	default:
		if (pContext) {
			pContext->showError(line, idx, "Unknown M command");
		}
		break;
	}
	return idx;
}

size_t GCodeInterpreter::processXAxis(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	if (command == 0) {
		command = motionCommand;
	}
	return getValue(x, line, idx, length);
}

size_t GCodeInterpreter::processYAxis(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	if (command == 0) {
		command = motionCommand;
	}
	return getValue(y, line, idx, length);
}

size_t GCodeInterpreter::processUAxis(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	if (command == 0) {
		command = motionCommand;
	}
	return getValue(u, line, idx, length);
}

size_t GCodeInterpreter::processVAxis(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	if (command == 0) {
		command = motionCommand;
	}
	return getValue(v, line, idx, length);
}

size_t GCodeInterpreter::processFeedRate(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	idx = getValue(f, line, idx, length);
	double rate = f;
	if (pCurrentState->units == INCH) {
		rate *= 25.4;
	}

	pCutter->setFeedRate(rate);
	return idx;
}

size_t GCodeInterpreter::processDwellTimeMilliSeconds(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	return getIntValue(mS, line, idx, length);
}

size_t GCodeInterpreter::processDwellTimeSeconds(const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	int secs;
	idx = getIntValue(secs, line, idx, length);
	mS = secs * 1000;
	return idx;
}

size_t GCodeInterpreter::getIntValue(int & value, const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());
	
	idx = skipSpaces(line, idx, length);
	if (idx < length && (line[idx] == '-' || line[idx] == '+' || isdigit(line[idx]))) {
		size_t charsConverted;
		value = std::stoi(line.substr(idx), &charsConverted);
		idx += charsConverted;
	}
	else {
		value = 0;
	}

	return idx;
}

size_t GCodeInterpreter::getValue(double & value, const std::string & line, size_t idx, size_t length)
{
	assert(this);
	assert(idx <= length);
	assert(length == line.length());

	idx = skipSpaces(line, idx, length);
	if (idx < length && (line[idx] == '-' || line[idx] == '+' || isdigit(line[idx]))) {
		size_t charsConverted;
		value = std::stod(line.substr(idx), &charsConverted);
		idx += charsConverted;
	}
	else {
		value = 0;
	}

	return idx;
}

int GCodeInterpreter::moveFast()
{
	assert(this);
	assert(pCutter);
	Position<double> deltas;
	calculateMove(deltas);
	pCutter->fastMove(deltas);
	return 0;
}

int GCodeInterpreter::moveStraight()
{
	assert(this);
	assert(pCutter);
	Position<double> deltas;
	calculateMove(deltas);
	pCutter->cutMove(deltas);
	return 0;
}

void GCodeInterpreter::calculateMove(Position<double>& deltas) {
	assert(this);
	assert(pCurrentState);

	Position<double> values(x, y, u, v);

	if (pCurrentState->mirrored) {
		values.set(u, v, x, y);  // <<--- note sides swapped
	}

	if (pCurrentState->units == INCH) {
		values.scale(25.4);
	}

	if (pCurrentState->moveType == ABSOLUTE_MOVE) {
		values.add(pCurrentState->offsets);
		values.sub(pCutter->getCurrentPosition());
	}

	deltas.set(values);
}

int GCodeInterpreter::dwell()
{
	assert(this);
	assert(pCutter);
	pCutter->dwell(mS);
	return 0;
}

int GCodeInterpreter::home()

{
	assert(this);
	assert(pCutter);
	pCutter->home();
	x = y = u = v = 0; // home equivalent to move to 0,0,0,0
	return 0;
}

int GCodeInterpreter::workshift()
{
	assert(this);
	double xmm = x;
	double ymm = y;
	double umm = u;
	double vmm = v;

	if (pCurrentState->units == INCH) {
		xmm *= 25.4;
		ymm *= 25.4;
		umm *= 25.4;
		vmm *= 25.4;
	}
	pCurrentState->offsets.add(xmm, ymm, umm, vmm);
	return 0;
}




