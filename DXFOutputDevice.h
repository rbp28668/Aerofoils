// DXFOutputDevice.h: interface for the CDXFOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DXFOUTPUTDEVICE_H__B1A20901_C3FE_11D6_9551_00047660C89A__INCLUDED_)
#define AFX_DXFOUTPUTDEVICE_H__B1A20901_C3FE_11D6_9551_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include "kernel/OutputDevice.h"
#include "kernel/PointT.h"

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

private:

	void writeHeader();
	void writeFooter();

	std::ofstream os;
	PointT last[2];	// last position for each stream
	int lastStream;	// last stream that altered its position
};

#endif // !defined(AFX_DXFOUTPUTDEVICE_H__B1A20901_C3FE_11D6_9551_00047660C89A__INCLUDED_)
