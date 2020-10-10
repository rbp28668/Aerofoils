#pragma once
#include "OutputDevice.h"
#include "PointT.h"
#include "CutterGeometry.h"

class Bounds;

class BlockCorrectionOutputDevice :
	public COutputDevice
{
private:
	COutputDevice* delegate;
	const Bounds& bounds;

	PointT left;
	PointT right;

	bool hasLeft;
	bool hasRight;

	CutterGeometry geometry;

	void capture(int iStream, const PointT& pt);
	void transformCaptured();

public:
	BlockCorrectionOutputDevice(COutputDevice* delegate, const Bounds& bounds);
	virtual ~BlockCorrectionOutputDevice();
	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();

	// These are optional but can provide hints to the output device.
	virtual void startObject(const char* description, bool selected = false);
	virtual void endObject(const char* description, bool selected = false);
	virtual void startPlot();
	virtual void endPlot();
	virtual void passthrough(const char* data);
	virtual void feedRate(float mmPerSec);

	// Information about the device;
	bool isCNC() const;

	// Get current position;
	virtual PointT position(int iStream);

};

