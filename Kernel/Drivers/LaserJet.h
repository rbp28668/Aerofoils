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
*/#ifndef _LASERJET_H
#define _LASERJET_H

#include <fstream>

#include "..\OutputDevice.h"
#include "..\PointT.h"

class CLaserJetOutputDevice: public COutputDevice
{ 
public:
	
	CLaserJetOutputDevice(const char* path);
	virtual ~CLaserJetOutputDevice();
	
	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();
	virtual PointT position(int iStream);

	
private:
	void reset()  { os <<"\033E";}          // Reset printer
	void hpgl2()  { os << "\033%%0BIN";}     // Enter HP-GL/2 mode
	void pcl()    { os << "\033%%0A";}       // enter PCL at previous CAP
	void land()   { os << "RO270IP;";}       // hpgl2 landscape 
	void mm_scale() { os << "SC0.0,246.38,0.0,197.104,1;";} // isotropic mm scale 

	void StartDocument();
	void StartPage();
	void StartLine(const PointT& ptIntial);
	void LineSegment(const PointT& pt);
	void EndLine();
	void Label(const PointT& pt, const char* psz);
	void EndPage();
	void EndDocument();


	std::ofstream os;
	PointT last[2];	// last position for each stream
	bool firstSegment; // true if drawing first line segment
	bool lineOpen; // true if need to close a line 
	int lastStream;	// last stream that altered its position
};

#endif

