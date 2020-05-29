
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
#include <sstream>
#include <ctime>

#include "PointT.h"
#include "PlotStructure.h"
#include "GCodeOutputDevice.h"
#include "ObjectSerializer.h"
#include "CutterGeometry.h"

using namespace std;


GCodeOutputDevice::GCodeOutputDevice()
	: GCodeOutputDevice(&defaultConfig,0)
{
}

GCodeOutputDevice::GCodeOutputDevice(GCodeOutputDevice::GCodeConfig* config, CutterGeometry* geometry)
	: x(0), y(0), u(0), v(0)
	, hasLeft(false), hasRight(false)
	, fastFeedSet(false)
	, currentFeedRate(0)
	, config(config)
	, geometry(geometry)
	, previousSet(false)
{
	assert(config);
	_isCNC = true;
	currentFeedRate = config->cutFeedRate;
}

GCodeOutputDevice::~GCodeOutputDevice()
{
}
void GCodeOutputDevice::sendFeedRate(double mmPerSec) {
	std::ostringstream os;
	if (config->useUnitsPerMin) {
		mmPerSec *= 60; // change to distance per min.
	}
	os << "F" << mmPerSec << std::endl;  // removed initial G01
	send(os.str());

}

void GCodeOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if (iStream == 0) {
		x = pt.fx;
		y = pt.fy;
		hasLeft = true;
	}
	if (iStream == 1) {
		u = pt.fx;
		v = pt.fy;
		hasRight = true;
	}
	if (hasLeft && hasRight) {
		if (config->useG1forMoves) {
			if (!fastFeedSet) {
				sendFeedRate(config->fastFeedRate);
				fastFeedSet = true;
			}
			sendMove("G01");
		}
		else {
			sendMove("G00");
		}
	}
}

void GCodeOutputDevice::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if (iStream == 0) {
		x = pt.fx;
		y = pt.fy;
		hasLeft = true;
	}
	if (iStream == 1) {
		u = pt.fx;
		v = pt.fy;
		hasRight = true;
	}
	if (hasLeft && hasRight) {
		// May have been using G01 with a fast feed rate for moves.
		// Reset to normal if the fast feed rate was set.
		if (fastFeedSet) {
			sendFeedRate(currentFeedRate);
			fastFeedSet = false;
		}
		sendMove("G01");
	}
}

void GCodeOutputDevice::sendMove(const char* pszCommand) {
	assert(this);
	assert(pszCommand);

	// Don't want to send zero length redundant moves
	// Note doesn't matter whether we lose a G0 or G1 as we resend with
	// each move.
	if (previousSet) {
		if (x == xprevious
			&& y == yprevious
			&& u == uprevious
			&& v == vprevious) {
			hasLeft = hasRight = false; // treat as sent
			return; // but abort sending
		}
	}

	xprevious = x;
	yprevious = y;
	uprevious = u;
	vprevious = v;
	previousSet = true;


	if (config->preCorrectGeometry && geometry) {
		Position<double> axes;
		axes.u = u;
		axes.v = v;
		axes.x = x;
		axes.y = y;
		geometry->blockToAxes(axes);
		x = axes.x;
		y = axes.y;
		u = axes.u;
		v = axes.v;
	}


	ostringstream oss;
	oss.precision(2);
	oss << std::fixed;
	oss << pszCommand << " "
		<< config->axesNames[0] << x << " "
		<< config->axesNames[1] << y << " "
		<< config->axesNames[2] << u << " "
		<< config->axesNames[3] << v << " "
		<< endl;
	send(oss.str());
	hasLeft = hasRight = false; // as now sent
}




void GCodeOutputDevice::Label(int iStream, const char* psz)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	assert(psz);

	ostringstream oss;
	oss << "(" << psz << ")" << endl; //  treat as comment
	send(oss.str());
}

void GCodeOutputDevice::Home()
{
	assert(this);
	ostringstream oss;
	oss << "G28" << endl; // home command
	send(oss.str());
	hasLeft = hasRight = false;
	x = y = u = v = 0;
}

void GCodeOutputDevice::Flush()
{
	assert(this);
}

PointT GCodeOutputDevice::position(int iStream)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	if (iStream == 0) {
		return PointT(x, y);
	} else {
		return PointT(u, v);
	}
}


void GCodeOutputDevice::startObject(const char* description, bool selected)
{
	assert(this);
	assert(description);
	ostringstream oss;
	oss << "( START " << description << ")" << endl; //  treat as comment
	send(oss.str());
}

void GCodeOutputDevice::endObject(const char* description, bool selected)
{
	assert(this);
	assert(description);
	ostringstream oss;
	oss << "( END " << description << ")" << endl; //  treat as comment
	send(oss.str());
}

void GCodeOutputDevice::startPlot()
{
	assert(this);
	ostringstream oss;

	// Format current date and time.
	char mbstr[100];
	time_t now = std::time(0);
	string t("");
	if (std::strftime(mbstr, sizeof(mbstr), " at %T on %e %B %Y", std::localtime(&now))) {
		t = mbstr;
	}

	oss << "(Generated by Aerofoil" << t << ")" << endl;
	oss << "(See https://github.com/rbp28668/Aerofoils )" << endl;
	oss << "G21 (mm)" << endl; // mm
	
	if (config->sendMirror) {
		oss << "G39 (mirror off)" << endl; // mirror off
	}
	
	if (config->sendWorkshifts) {
		oss << "G53 (cancel local workshifts)" << endl; // cancel local workshift
	}

	oss << "G90 (absolute coordinates)" << endl; // absolute

	if (config->sendEnableControls) {
		oss << config->enableMotors << " (enable motors)" << endl; // enable steppers
	}

	if (config->sendWireControls) {
		oss << config->wireOn << " (wire on)" << endl; // wire on
	}

	// Make sure Mach3 is set up properly, cutter generally ignores these.
	oss << "G40 (set cutter compensation off)" << endl; // G40 SET CUTTER COMPENSATION - OFF
	oss << "G49 (clear existing tool length offset)" << endl; // G49 clear existing tool length offset.
	oss << "G64 (set path control mode to constant velocity)" << endl; // G64 SET PATH CONTROL MODE - CONSTANT VELOCITY

	if (config->useUnitsPerMin) {
		oss << "G94 (set feed rate mode to units per minute)" << endl; // G94 - SET FEED RATE MODE - UNITS PER MINUTE
	}


	send(oss.str());

	hasLeft = hasRight = false;
	x = y = u = v = 0;
}

void GCodeOutputDevice::endPlot()
{
	assert(this);
	ostringstream oss;
	
	if (config->sendWireControls) {
		oss << config->wireOff << " (wire off)" << endl; // wire on
	}

	oss << "M02 (end of program)" << endl; 

	send(oss.str()); // wire off, end of program
}

void GCodeOutputDevice::passthrough(const char * data)
{
	assert(this);
	assert(data);
	ostringstream oss;
	oss << data << endl; //  treat as comment
	send(oss.str());
}

void GCodeOutputDevice::feedRate(double mmPerSec)
{
	currentFeedRate = mmPerSec;
	sendFeedRate(currentFeedRate);
}

GCodeOutputDevice::GCodeConfig::GCodeConfig()
	: sendFeedRate(false)
	, cutFeedRate(1.5f)
	, useG1forMoves(false)
	, fastFeedRate(100)
	, sendWireControls(true)
	, sendEnableControls(true)
	, sendMirror(true)
	, sendWorkshifts(true)
	, useUnitsPerMin(false)
	, preCorrectGeometry(false)
	, wireOn("M03")
	, wireOff("M05")
	, enableMotors("M17")
	, disableMotors("M18")
{
	setAxesNames("XYUV");
}

void GCodeOutputDevice::GCodeConfig::setAxesNames(const char* names)
{
	for (int i = 0; i < 4; ++i) {
		axesNames[i] = *names++;
	}
}

void GCodeOutputDevice::GCodeConfig::setDefaults()
{
	sendFeedRate = false;
	cutFeedRate = 1.5f;
	useG1forMoves = false;		// If not linear G0 use G1 with fast feed rate.
	fastFeedRate = 100;			// fast feed rate to use if useG1forMoves set
	sendWireControls = true;	// M03, M05
	sendEnableControls = true;	// M17, M18
	sendMirror = true;			// Mach3 doesn't know about G38/G39
	sendWorkshifts = true;		// Mach3 treats G53/G54 differently.
	useUnitsPerMin = false;		// We use mm per sec, 
	preCorrectGeometry = false;  // as cutter program compensates for cutter geometry
	setAxesNames("XYUV");
	wireOn = "M03";
	wireOff = "M05";
	enableMotors = "M17";
	disableMotors = "M18";

}

void GCodeOutputDevice::GCodeConfig::setMach3()
{
	sendFeedRate = true;
	cutFeedRate = 1.5f;
	useG1forMoves = true;		// If not linear G0 use G1 with fast feed rate.
	fastFeedRate = 100;			// fast feed rate to use if useG1forMoves set
	sendWireControls = false;	// M03, M05
	sendEnableControls = false; // M17, M18
	sendMirror = false;			// Mach3 doesn't know about G38/G39
	sendWorkshifts = false;		// Mach3 treats G53/G54 differently.
	useUnitsPerMin = true;		// Select units per min.
	preCorrectGeometry = true;  // as Mach3 doesn't understand cutter geometry.
	setAxesNames("XYAZ");
	wireOn = "M200";
	wireOff = "M201";
	enableMotors = "M202";
	disableMotors = "M203";
}

void GCodeOutputDevice::GCodeConfig::serializeTo(CObjectSerializer& os) const
{
	assert(this);

	// Array of 4 chars, not a null terminated string so:
	std::string axes;
	axes.push_back(axesNames[0]);
	axes.push_back(axesNames[1]);
	axes.push_back(axesNames[2]);
	axes.push_back(axesNames[3]);

	os.startSection("GCodeConfig", this);
	os.write("sendFeedRate",sendFeedRate);
	os.write("cutFeedRate", cutFeedRate);
	os.write("useG1forMoves", useG1forMoves);
	os.write("fastFeedRate", fastFeedRate);
	os.write("sendWireControls", sendWireControls);
	os.write("sendEnableControls", sendEnableControls);
	os.write("sendMirror", sendMirror);
	os.write("sendWorkshifts", sendWorkshifts);
	os.write("useUnitsPerMin", useUnitsPerMin);
	os.write("preCorrectGeometry", preCorrectGeometry);
	os.write("axesNames", axes.c_str());
	os.write("wireOn", wireOn.c_str());
	os.write("wireOff", wireOff.c_str());
	os.write("enableMotors", enableMotors.c_str());
	os.write("disableMotors", disableMotors.c_str());

	os.endSection();
}

void GCodeOutputDevice::GCodeConfig::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	std::string axes;

	os.startReadSection("GCodeConfig", this);
	os.read("sendFeedRate", sendFeedRate);
	os.read("cutFeedRate", cutFeedRate);
	os.read("useG1forMoves", useG1forMoves);
	os.read("fastFeedRate", fastFeedRate);
	os.read("sendWireControls", sendWireControls);
	os.read("sendEnableControls", sendEnableControls);
	os.read("sendMirror", sendMirror);
	os.read("sendWorkshifts", sendWorkshifts);
	os.read("useUnitsPerMin", useUnitsPerMin);
	os.read("preCorrectGeometry", preCorrectGeometry);
	os.read("axesNames", axes);
	if (axes.length() != 4) {
		throw CSerializeException("Axes names must have exactly 4 characters");
	}
	axesNames[0] = axes.at(0);
	axesNames[1] = axes.at(1);
	axesNames[2] = axes.at(2);
	axesNames[3] = axes.at(3);
	os.read("wireOn", wireOn);
	os.read("wireOff", wireOff);
	os.read("enableMotors", enableMotors);
	os.read("disableMotors", disableMotors);
	os.endReadSection();
}
