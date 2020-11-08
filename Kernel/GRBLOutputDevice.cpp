#include <assert.h>
#include "GRBLOutputDevice.h"

void GRBLOutputDevice::header()
{
	os << "G21" << std::endl;  // metric
	os << ( useDynamicLaser ? "M04" : "M03") << std::endl;  // laser on 
	os << "S" << power << std::endl;   // but power = 0
	os << "F" << feedRate << std::endl; // and probably too fast (mm per min)
	headerWritten = true;
}

void GRBLOutputDevice::footer()
{
	os << "M05" << std::endl;  // laser off
}

void GRBLOutputDevice::emitMove(const PointT& pt)
{
	os << "G0 X" << pt.fx << " Y" << pt.fy << std::endl;
}

void GRBLOutputDevice::emitLine(const PointT& pt)
{
	os << "G1 X" << pt.fx << " Y" << pt.fy << std::endl;
}

GRBLOutputDevice::GRBLOutputDevice(const char* filename)
	: hasLastRootPoint(false)
	, hasLastTipPoint(false)
	, power(0)
	, feedRate(1000)
	, useDynamicLaser(true)
	, headerWritten(false)
{
	os.open(filename);
}

GRBLOutputDevice::~GRBLOutputDevice()
{
	Flush();
	footer();
	os.close();
}

void GRBLOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if(!headerWritten) header();

	if (iStream == 0) {
		emitMove(pt);
		lastRootPoint = SavedPoint(pt.fx, pt.fy, MoveT::MoveToCode);
		hasLastRootPoint = true;
	}
	else {
		tipQueue.push(SavedPoint(pt.fx, pt.fy, MoveT::MoveToCode));
	}
	if (tipQueue.size() > MAX_QUEUE) {
		Flush();
	}
	last[iStream] = pt;
}

void GRBLOutputDevice::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if (!headerWritten) header();

	if (iStream == 0) {
		emitLine(pt);
		lastRootPoint = SavedPoint(pt.fx, pt.fy, MoveT::MoveToCode);
		hasLastRootPoint = true;
	}
	else {
		tipQueue.push(SavedPoint(pt.fx, pt.fy, MoveT::LineToCode));
	}
	if (tipQueue.size() > MAX_QUEUE) {
		Flush();
	}

	last[iStream] = pt;

}

void GRBLOutputDevice::Label(int iStream, const char* psz)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

}

void GRBLOutputDevice::Home()
{
}

void GRBLOutputDevice::Flush()
{
	if (!headerWritten) header();

	// Go back to where we left off?
	if (hasLastTipPoint) {
		emitMove(PointT(lastTipPoint.x, lastTipPoint.y, 0));
	}

	// Write all the tip moves
	while (!tipQueue.empty()) {
		lastTipPoint = tipQueue.front();
		if (lastTipPoint.type == MoveT::MoveToCode) {
			emitMove(PointT(lastTipPoint.x, lastTipPoint.y, 0));
		}
		else if (lastTipPoint.type == MoveT::LineToCode) {
			emitLine(PointT(lastTipPoint.x, lastTipPoint.y, 0));
		}
		tipQueue.pop();
		hasLastTipPoint = true;
	}

	// Pick up again at the root
	if (hasLastRootPoint) {
		emitMove(PointT(lastRootPoint.x, lastRootPoint.y, 0));
		hasLastRootPoint = false;
	}
}

PointT GRBLOutputDevice::position(int iStream)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	return last[iStream];
}
