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
// CutterSimulationOutputDevice.h: interface for the CCutterSimulationOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUTTERSIMULATIONOUTPUTDEVICE_H__E1094EC3_C71D_11D6_9558_00047660C89A__INCLUDED_)
#define AFX_CUTTERSIMULATIONOUTPUTDEVICE_H__E1094EC3_C71D_11D6_9558_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CoordMap.h"
#include "kernel\OutputDevice.h"
#include "kernel\PointT.h"

class CCutterSimulationOutputDevice : public COutputDevice
{
public:
	CCutterSimulationOutputDevice(float sizex, float sizey, CDC* pdc, float zoom);
	virtual ~CCutterSimulationOutputDevice();

	// COutputDevice
	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();

	virtual void startObject(CPlotStructure*);
	virtual void endObject(CPlotStructure*);

private:

	enum {AXIS_COUNT=4};
	CDC* plotDC;
	CCoordMap map;

	long last[AXIS_COUNT];  // last position 0 = root.x, 1 = root.y, 2 = tip.x, 3 = tip.y
	long axis[AXIS_COUNT];	// next position
	bool hasRoot;
	bool hasTip;
};


#endif // !defined(AFX_CUTTERSIMULATIONOUTPUTDEVICE_H__E1094EC3_C71D_11D6_9558_00047660C89A__INCLUDED_)
