#ifndef _LASERJET_H
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

