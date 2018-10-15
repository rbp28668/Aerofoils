// PlotProxy.h: interface for the CWindowsOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSOUTPUTDEVICE_H__D83773A4_BA49_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_WINDOWSOUTPUTDEVICE_H__D83773A4_BA49_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CoordMap.h"
#include "kernel\OutputDevice.h"
#include "kernel\PointT.h"

class CWindowsOutputDevice : public COutputDevice
{
public:
	CWindowsOutputDevice(float sizex, float sizey, CDC* pdc, float zoom);
	virtual ~CWindowsOutputDevice();

	void setSelection(CPlotStructure* sel);

	// COutputDevice
	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();

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
};

#endif // !defined(AFX_WINDOWSOUTPUTDEVICE_H__D83773A4_BA49_11D6_AF75_002018BA082E__INCLUDED_)
