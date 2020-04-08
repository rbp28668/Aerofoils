/* Aerofoil
Aerofoil plotting and CNC cutter driver
Kernel / core algorithms
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
*///
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTDEVICE_H__A4E22B63_B474_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_OUTPUTDEVICE_H__A4E22B63_B474_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <exception>
#include "Kernel.h"

class PointT;
class CPlotStructure;

class COutputDevice  
{
public:

	// Some generic exceptions for an output device.  Subclasses should throw
	// some variant of OutputException in the event of the operation failing.
	struct OutputException : public std::exception {};

	struct LimitsException : public OutputException {
		const char * what() const throw () {
			return "Reached limits of device";
		}
	};

	struct StoppedException : public OutputException {
		const char * what() const throw () {
			return "Device has been stopped";
		}
	};

	COutputDevice();
	virtual ~COutputDevice();

	virtual void MoveTo(int iStream, const PointT& pt) = 0;
	virtual void LineTo(int iStream, const PointT& pt) = 0;
	virtual void Label(int iStream, const char* psz) = 0;
	virtual void Home() = 0;
	virtual void Flush() = 0;

	// These are optional but can provide hints to the output device.
	virtual void startObject(const char* description) {}
	virtual void endObject(const char* description) {}
	virtual void startPlot() {}
	virtual void endPlot() {}
	virtual void passthrough(const char* data) {}
	virtual void feedRate(float mmPerSec) {}

	// Information about the device;
	bool isCNC() const {return _isCNC;}
	//virtual PointT getResolutionPixPerMM() const;

	// Get current position;
	virtual PointT position(int iStream) = 0;

protected:
	enum {MoveToCode, LineToCode, LabelCode, HomeCode, FlushCode};
	bool _isCNC;

};

#endif // !defined(AFX_OUTPUTDEVICE_H__A4E22B63_B474_11D6_AF75_002018BA082E__INCLUDED_)
