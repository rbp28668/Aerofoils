#ifndef _POSTSCRI_H
#define _POSTSCRI_H

#include <fstream>

#include "..\OutputDevice.h"
#include "..\PointT.h"

class CPostscriptOutputDevice: public COutputDevice
{ 
public:
	
	CPostscriptOutputDevice(const char* path);
	virtual ~CPostscriptOutputDevice();
	
	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();
	
private:

	static float pt(float x) {return x*72.0f/25.4f;}   /* convert mm -> point size */

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

