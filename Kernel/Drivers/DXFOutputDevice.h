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
*/// DXFOutputDevice.h: interface for the CDXFOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXFOUTPUTDEVICE_H__B1A20901_C3FE_11D6_9551_00047660C89A__INCLUDED_)
#define AFX_DXFOUTPUTDEVICE_H__B1A20901_C3FE_11D6_9551_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include "..\OutputDevice.h"
#include "..\PointT.h"

class CDXFOutputDevice : public COutputDevice  
{
public:
	CDXFOutputDevice(const char* path);
	virtual ~CDXFOutputDevice();

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();
	virtual PointT position(int iStream);

private:

	void writeHeader();
	void writeFooter();

	std::ofstream os;
	PointT last[2];	// last position for each stream
	int lastStream;	// last stream that altered its position
};

#endif // !defined(AFX_DXFOUTPUTDEVICE_H__B1A20901_C3FE_11D6_9551_00047660C89A__INCLUDED_)
