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
// PlotProxy.h: interface for the CWindowsOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSOUTPUTDEVICE_H__D83773A4_BA49_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_WINDOWSOUTPUTDEVICE_H__D83773A4_BA49_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CoordMap.h"
#include "Kernel/Kernel.h"
#include "kernel\OutputDevice.h"
#include "kernel\PointT.h"

class CWindowsOutputDevice : public COutputDevice
{
public:
	CWindowsOutputDevice(NumericT sizex, NumericT sizey, CDC* pdc, float zoom);
	virtual ~CWindowsOutputDevice();

	void setSelection(CPlotStructure* sel);
	void setDrawMoves(bool drawMoves) { this->drawMoves = drawMoves; }

	// COutputDevice
	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();
	virtual PointT position(int iStream);


	virtual void startObject(CPlotStructure*);
	virtual void endObject(CPlotStructure*);

private:
	CDC* plotDC;
	CCoordMap map;

	PointT last[2];	// last position for each stream
	int lastStream;	// last stream that altered its position

	CPlotStructure* selected;
	CPen* penGrey;
	CPen* penBlack;
	bool drawMoves;

};

#endif // !defined(AFX_WINDOWSOUTPUTDEVICE_H__D83773A4_BA49_11D6_AF75_002018BA082E__INCLUDED_)
