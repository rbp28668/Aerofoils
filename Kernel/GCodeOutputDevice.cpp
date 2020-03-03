
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
#include "PointT.h"
#include "PlotStructure.h"
#include "GCodeOutputDevice.h"

using namespace std;


GCodeOutputDevice::GCodeOutputDevice()
	: x(0), y(0), u(0), v(0)
	, hasLeft(false), hasRight(false)
{
}


GCodeOutputDevice::~GCodeOutputDevice()
{
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
		sendMove("G00");
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
		sendMove("G01");
	}
}

void GCodeOutputDevice::sendMove(const char* pszCommand) {
	assert(this);
	assert(pszCommand);

	ostringstream oss;
	oss.precision(2);
	oss << std::fixed;
	oss << pszCommand << " "
		<< "X" << x << " "
		<< "Y" << y << " "
		<< "U" << u << " "
		<< "V" << v << " "
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


void GCodeOutputDevice::startObject(const char* description)
{
	assert(this);
	assert(description);
	ostringstream oss;
	oss << "( START " << description << ")" << endl; //  treat as comment
	send(oss.str());
}

void GCodeOutputDevice::endObject(const char* description)
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
	oss << "G21" << endl; // mm
	oss << "G39" << endl; // mirror off
	oss << "G53" << endl; // cancel local workshift
	oss << "G90" << endl; // absolute
	oss << "M17" << endl; // enable steppers
	oss << "M03" << endl; // wire on
	send(oss.str());

	hasLeft = hasRight = false;
	x = y = u = v = 0;
}

void GCodeOutputDevice::endPlot()
{
	assert(this);
	ostringstream oss;
	oss << "M05 M02" << endl; 
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

void GCodeOutputDevice::feedRate(float mmPerSec)
{
	std::ostringstream os;
	os << "G01 F" << mmPerSec << std::endl;
	send(os.str());

}


