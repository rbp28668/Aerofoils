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

#include "stdafx.h"
#include <assert.h>
#include "CutterHardware.h"
#include <iostream>
#include <sstream>
#include <iomanip>

int CutterHardware::processResponse()
{
	assert(this);
	serialLink.read(6);   // 4 status characters + CRLF
	while (!serialLink.readComplete());
	std::string response;
	serialLink.getReadData(response);
	std::cout << response << std::endl;

	// This is the code on the device that sends the response
	//outputBuffer[0] = ok ? 'Y' : 'N';
	//outputBuffer[1] = commandQueue.isFull() ? 'F' : '-';
	//outputBuffer[2] = commandQueue.isEmpty() ? 'E' : '-';
	//outputBuffer[3] = hexDigits[hardware.limitSwitches() & 0x0F];
	int combinedResults = 0;
	if (response[0] == 'Y') {
		combinedResults |= VALID;
	}
	if (response[1] == 'F') {
		combinedResults |= FULL;
	}
	if (response[2] == 'E') {
		combinedResults |= EMPTY;
	}
	int switches = 0;
	char ch = response[3];
	if (ch >= '0' && ch <= '9') {
		switches = (ch - '0');
	}
	else if (ch >= 'A' && ch <= 'F') {
		switches = 10 + (ch - 'A');
	}
	combinedResults |= switches;
	return combinedResults;
}


int CutterHardware::send(std::string& msg)
{
	assert(this);
	serialLink.send(msg);
	return processResponse();
}

CutterHardware::CutterHardware()
{
}

CutterHardware::CutterHardware(const char * pszPort) :
	serialLink(pszPort)
{
}

int CutterHardware::connect(const char * pszPort)
{
	assert(this);
	return serialLink.connect(pszPort);
}

void CutterHardware::disconnect()
{
	assert(this);
	serialLink.disconnect();
}

boolean CutterHardware::isConnected()
{
	assert(this);
	return serialLink.isConnected();
}

int CutterHardware::ping()
{
	assert(this);
	assert(serialLink.isConnected());

	std::string msg("P");
	return send(msg);
}

int CutterHardware::disable()
{
	assert(this);
	assert(serialLink.isConnected());

	std::string msg("D");
	return send(msg);
}

int CutterHardware::enable()
{
	assert(this);
	assert(serialLink.isConnected());

	std::string msg("E");
	return send(msg);
}

int CutterHardware::abort()
{
	assert(this);
	assert(serialLink.isConnected());

	std::string msg("Z");
	return send(msg);
}

int CutterHardware::home()
{
	assert(this);
	assert(serialLink.isConnected());

	std::string msg("H");
	return send(msg);
}

int CutterHardware::line(AxisT steps, AxisT lx, AxisT ly, AxisT rx, AxisT ry)
{
	assert(this);
	assert(serialLink.isConnected());

	assert(abs(lx) <= steps);
	assert(abs(ly) <= steps);
	assert(abs(rx) <= steps);
	assert(abs(ry) <= steps);

	std::stringstream out;
	out << "L";
	out <<std::hex << std::setfill('0') << std::right 
		<< std::setw(8) << steps 
		<< std::setw(8) << lx 
		<< std::setw(8) << ly 
		<< std::setw(8) << rx 
		<< std::setw(8) << ry;
	return send(out.str());
}

int CutterHardware::step(int direction, int pulse)
{
	assert(this);
	assert(serialLink.isConnected());
	assert(0 <= direction && direction <= 0x0F);
	assert(0 <= pulse && pulse <= 0x0F);

	int data = ((direction & 0x0F) << 4) | (pulse & 0x0F);

	std::stringstream out;
	out << "S"
		<< std::hex << std::setfill('0') << std::right << std::setw(2)
		<< data;
	return send(out.str());
}

int CutterHardware::wireOn()
{
	assert(this);
	assert(serialLink.isConnected());

	std::string msg("W");
	return send(msg);
}

int CutterHardware::wireOff()
{
	assert(this);
	assert(serialLink.isConnected());

	std::string msg("X");
	return send(msg);
}
