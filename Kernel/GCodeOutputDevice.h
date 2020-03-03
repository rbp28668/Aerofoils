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
#include <string>
#include "OutputDevice.h"

// GCodeOutputDevice sends GCode to the output mechanism implemented
// by send in a derived class.
// 
class GCodeOutputDevice :
	public COutputDevice
{
public:
	GCodeOutputDevice();
	virtual ~GCodeOutputDevice();

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();
	virtual PointT position(int iStream);

	// These are optional but can provide hints to the output device.
	virtual void startObject(const char* description);
	virtual void endObject(const char* description);
	virtual void startPlot();
	virtual void endPlot();
	virtual void passthrough(const char* data);
	virtual void feedRate(float mmPerSec);

protected:
	void sendMove(const char* pszCommand);
	virtual void send(const std::string& str) = 0;

private:

	double x;
	double y;
	double u;
	double v;

	bool hasLeft;
	bool hasRight;

};

