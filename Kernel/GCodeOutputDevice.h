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
#include "Position.h"

class CObjectSerializer;
class CutterGeometry;

// GCodeOutputDevice sends GCode to the output mechanism implemented
// by send in a derived class.
// 
class GCodeOutputDevice :
	public COutputDevice
{
public:
	struct GCodeConfig {
		char axesNames[4];		// Default XYUV but may by XYAZ
		bool useG1forMoves;		// If not linear G0 use G1 with fast feed rate.
		double fastFeedRate;	// fast feed rate to use if useG1forMoves set
		bool sendFeedRate;		// true if we want to
		double cutFeedRate;		// feed rate to use for normal cutting
		bool preCorrectGeometry; // use cutter geometry to pre-calculate actual paths of steppers

		bool sendWireControls;	// M03, M05
		bool sendEnableControls; // M17, M18
		bool sendMirror;		// Mach3 doesn't know about G38/G39
		bool sendWorkshifts;	// Mach3 treats G53/G54 differently.
		bool useUnitsPerMin;	// rather than per sec.
		std::string wireOn;
		std::string wireOff;
		std::string enableMotors;
		std::string disableMotors;

		GCodeConfig();
		void setAxesNames(const char* names);
		void setDefaults();  // set defaults for use with own cutter
		void setMach3();     // set values to enable Mach3 compatible output.
		void serializeTo(CObjectSerializer& os) const;
		void serializeFrom(CObjectSerializer& os);
	};

	GCodeOutputDevice();
	GCodeOutputDevice(GCodeOutputDevice::GCodeConfig* config, CutterGeometry* geometry);
	virtual ~GCodeOutputDevice();

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();
	virtual PointT position(int iStream);

	// These are optional but can provide hints to the output device.
	virtual void startObject(const char* description, bool selected = false);
	virtual void endObject(const char* description, bool selected = false);
	virtual void startPlot();
	virtual void endPlot();
	virtual void passthrough(const char* data);
	virtual void feedRate(double mmPerSec);

protected:
	void sendFeedRate(double mmPerSec);
	void sendMove(const char* pszCommand);
	virtual void send(const std::string& str) = 0;

private:

	double x;
	double y;
	double u;
	double v;

	bool hasLeft;
	bool hasRight;

	bool fastFeedSet;
	double currentFeedRate;

	double xprevious;
	double yprevious;
	double uprevious;
	double vprevious;
	bool previousSet;

	GCodeConfig defaultConfig;
	GCodeConfig* config;
	CutterGeometry* geometry;
	
};

