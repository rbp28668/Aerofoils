#pragma once
#include "OutputDevice.h"

class CutterSimulation;

class CutterSimulationOutputDevice :
	public COutputDevice
{
public:
	CutterSimulationOutputDevice(CutterSimulation* cutter);
	virtual ~CutterSimulationOutputDevice();

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();

	// These are optional but can provide hints to the output device.
	//virtual void startObject(const char* description) {}
	//virtual void endObject(const char* description) {}
	//virtual void startPlot() {}
	//virtual void endPlot() {}
	//virtual void passthrough(const char* data) {}

	// Get current position;
	virtual PointT position(int iStream);

private:

	CutterSimulation* pCutter;

	double x;
	double y;
	double u;
	double v;

	bool hasLeft;
	bool hasRight;

};
