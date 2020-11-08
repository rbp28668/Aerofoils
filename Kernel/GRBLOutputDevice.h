#pragma once
#include <fstream>
#include <queue>
#include "OutputDevice.h"
#include "PointT.h"

class GRBLOutputDevice :
	public COutputDevice
{
	struct SavedPoint {
		double x;
		double y;
		MoveT type;
		SavedPoint(const SavedPoint& other) : x(other.x), y(other.y), type(other.type) {}
		SavedPoint() : x(0), y(0), type(MoveT::MoveToCode) {}
		SavedPoint(double x, double y, MoveT type) : x(x), y(y), type(type) {}
	};

	std::ofstream os;
	PointT last[2];	// last position for each stream
	
	// variables to split out root and tip when not interpolating.
	std::queue<SavedPoint> tipQueue;
	SavedPoint lastRootPoint;
	SavedPoint lastTipPoint;
	bool hasLastRootPoint;
	bool hasLastTipPoint;
	const int MAX_QUEUE = 1000;

	// Parameterise output
	bool useDynamicLaser; // i.e. M4 else M3
	double feedRate;      // mm per minute
	double power;         // 0..1000
	bool headerWritten;

	void header();
	void footer();
	void emitMove(const PointT& pt);
	void emitLine(const PointT& pt);

public:
	GRBLOutputDevice(const char* filename);
	virtual ~GRBLOutputDevice();

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();
	virtual PointT position(int iStream);

	bool isDynamicLaserPower() const { return useDynamicLaser;  }
	double getFeedRate() const { return feedRate; }
	double getLaserPower() const { return power; }
	void setDynamicLaserPower(bool isDynamic) { useDynamicLaser = isDynamic; }
	void setFeedRate(double feedRate) { this->feedRate = feedRate; }
	void setLaserPower(double power) { this->power = power; }


};

