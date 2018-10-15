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
