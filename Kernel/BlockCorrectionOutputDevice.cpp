#include "BlockCorrectionOutputDevice.h"
#include "Bounds.h"


BlockCorrectionOutputDevice::BlockCorrectionOutputDevice(COutputDevice* delegate, const Bounds& bounds)
	: delegate(delegate),
	bounds(bounds)
{
	// Set geometry so that a virtual block with the same width as the effective span of the cut
	// defines the nominal edges of the block.
	geometry.setBlockLeft(0);
	geometry.setBlockRight(bounds.depth());
	geometry.setWidth(bounds.depth());
}

BlockCorrectionOutputDevice::~BlockCorrectionOutputDevice()
{
}

void BlockCorrectionOutputDevice::capture(int iStream, const PointT& pt) {
	if (iStream == 0) {
		left = pt;
		hasLeft = true;
	}
	else {
		right = pt;
		hasRight = true;
	}
}

void BlockCorrectionOutputDevice::transformCaptured() {
	Position<double> axes;
	axes.x = left.fx;
	axes.y = left.fy;
	axes.u = right.fx;
	axes.v = right.fy;
	geometry.blockToAxes(axes, left.fz - bounds.getMinz(), right.fz - bounds.getMinz());
	left.fx = axes.x;
	left.fy = axes.y;
	right.fx = axes.u;
	right.fy = axes.v;
}

void BlockCorrectionOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	capture(iStream, pt);

	if (hasLeft && hasRight) {
		transformCaptured();
		delegate->MoveTo(0, left);
		delegate->MoveTo(1, right);
		hasLeft = false;
		hasRight = false;
	}
}

void BlockCorrectionOutputDevice::LineTo(int iStream, const PointT& pt)
{
	capture(iStream, pt);


	if (hasLeft && hasRight) {
		transformCaptured();
		delegate->LineTo(0, left);
		delegate->LineTo(1, right);
		hasLeft = false;
		hasRight = false;
	}
}

void BlockCorrectionOutputDevice::Label(int iStream, const char* psz)
{
	delegate->Label(iStream, psz);
}

void BlockCorrectionOutputDevice::Home()
{
	delegate->Home();
}

void BlockCorrectionOutputDevice::Flush()
{
	delegate->Flush();
}

void BlockCorrectionOutputDevice::startObject(const char* description, bool selected)
{
	delegate->startObject(description, selected);
}

void BlockCorrectionOutputDevice::endObject(const char* description, bool selected)
{
	delegate->endObject(description, selected);
}

void BlockCorrectionOutputDevice::startPlot()
{
	delegate->startPlot();
}

void BlockCorrectionOutputDevice::endPlot()
{
	delegate->endPlot();
}

void BlockCorrectionOutputDevice::passthrough(const char* data)
{
	delegate->passthrough(data);
}

void BlockCorrectionOutputDevice::feedRate(float mmPerSec)
{
	delegate->feedRate(mmPerSec);
}

bool BlockCorrectionOutputDevice::isCNC() const
{
	return delegate->isCNC();
}

PointT BlockCorrectionOutputDevice::position(int iStream)
{
	return delegate->position(iStream);
}

