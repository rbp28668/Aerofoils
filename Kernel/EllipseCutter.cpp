/* Aerofoil
Aerofoil plotting and CNC cutter driver
Copyright(C) 1995-2019 R Bruce Porteous

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/


#include <assert.h>
#include "OutputDevice.h"
#include "EllipsePair.h"
#include "EllipseCutter.h"
#include "ObjectSerializer.h"

#define DELTA 0.1f   /* angle increment for plotting */
#define PI 3.14159265358979323846264338

const std::string EllipseCutter::TYPE("ellipseCutter");
static CObjectFactory<EllipseCutter> factory(EllipseCutter::TYPE.c_str());


EllipseCutter::EllipseCutter()
	: pEllipses(0)
{
	assert(this);
}

EllipseCutter::EllipseCutter(CEllipsePair * pep)
	: pEllipses(pep)
{
	assert(this);
	assert(pep);
}

EllipseCutter::EllipseCutter(const EllipseCutter& source)
	: CutStructure(source)
	, pEllipses(source.pEllipses)
{
}


EllipseCutter::~EllipseCutter()
{
}


// Note a and b aligned as per normal convention-  a along x axis.
// offset is cut offset, +ve for external cut, -ve for internal.
// z is position along "span"
static PointT pointOnEllipse(double a, double b, double thetaRadians, double offset, double z) {

	// basic coords on ellipse
	double x = a * cos(thetaRadians);
	double y = b * sin(thetaRadians);

	// params for unit length normal vector
	double dx = b * cos(thetaRadians);
	double dy = a * sin(thetaRadians);
	double unit = sqrt(dx * dx + dy * dy); 

	x += dx * offset / unit;
	y += dy * offset / unit;

	return PointT(x, y, z);
}

static NumericT error_distance(PointT p0, PointT p1, PointT midpoint)
{
	// See https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
	NumericT dx = p1.fx - p0.fx;
	NumericT dy = p1.fy - p0.fy;

	NumericT dist = abs(dy * midpoint.fx - dx * midpoint.fy + p1.fx * p0.fy - p1.fy * p0.fx) /
		sqrt(dx * dx + dy * dy);

	return dist;
}

// Cut a segment of ellipse between a given start and finish angle.
// a0, b0, a1, b1 are the x and y half sizes for the 2 ellipses (a is x, b is y axis)
// span0 and span1 are where the ellipses lie on the z axis
// offset0 and offset1 are the tool offsets to use for each
// resolution is the value the error should be pushed below.
void EllipseCutter::cut_segment(COutputDevice * pdev, double startRadians, double finishRadians, double a0, double b0, double a1, double b1, double span0, double span1, double offset0, double offset1, double resolution) {
	assert(pdev);
	double midPointRadians = (startRadians + finishRadians) / 2;

	// Find positions for start, midpoint and finish for ellipse 0 and 1
	PointT h0s = pointOnEllipse(a0, b0, startRadians, offset0, span0);
	PointT h1s = pointOnEllipse(a1, b1, startRadians, offset1, span1);
	PointT h0m = pointOnEllipse(a0, b0, midPointRadians, offset0, span0);
	PointT h1m = pointOnEllipse(a1, b1, midPointRadians, offset1, span1);
	PointT h0f = pointOnEllipse(a0, b0, finishRadians, offset0, span0);
	PointT h1f = pointOnEllipse(a1, b1, finishRadians, offset1, span1);

	// Calculate error distance on half-way point
	NumericT error0 = error_distance(h0s, h0f, h0m);
	NumericT error1 = error_distance(h1s, h1f, h1m);

	if(error0 < resolution && error1 < resolution) {
		line(pdev, h0f, h1f);
	}
	else {
		cut_segment(pdev, startRadians, midPointRadians, a0, b0, a1, b1, span0, span1, offset0, offset1, resolution);
		cut_segment(pdev, midPointRadians, finishRadians, a0, b0, a1, b1, span0, span1, offset0, offset1, resolution);
	}
}


// Cutting routine that aims to minimise the number of line segments by
// splitting each segment until the error is less than the cutter resolution.
void EllipseCutter::cut_optimised(COutputDevice* pdev, const CutStructure::Context& context)
{
	assert(this);
	assert(pdev);
	
	const CEllipse* first = pEllipses->getFirst();
	const CEllipse* second = pEllipses->getSecond();

	bool isExternal = pEllipses->getPlotFlags()->is_external;
	double rootToolOffset = (isExternal) ? context.rootToolOffset : (-context.rootToolOffset);
	double tipToolOffset = (isExternal) ? context.tipToolOffset : (-context.tipToolOffset);

	double resolution = context.tolerance / 2;

	double a0 = first->getB() / 2;
	double a1 = second->getB() / 2;

	/** Plot top half */
	if (pEllipses->getPlotFlags()->plot_upper)
	{
		double b0 = first->getATop();
		double b1 = second->getATop();

		PointT h0 = pointOnEllipse(a0, b0, 0, rootToolOffset, 0);
		PointT h1 = pointOnEllipse(a1, b1, 0, tipToolOffset, pEllipses->getSpan());
		move(pdev, h0, h1);

		cut_segment(pdev, 0, PI / 2, a0, b0 ,a1, b1, 0, pEllipses->getSpan(), rootToolOffset, tipToolOffset, resolution);
		cut_segment(pdev, PI/2, PI,  a0, b0, a1, b1, 0, pEllipses->getSpan(), rootToolOffset, tipToolOffset, resolution);
	}

	/** Continue with bottom half **/
	if (pEllipses->getPlotFlags()->plot_lower)
	{
		double b0 = first->getABottom();
		double b1 = second->getABottom();

		PointT h0 = pointOnEllipse(a0, b0, PI, rootToolOffset, 0);
		PointT h1 = pointOnEllipse(a1, b1, PI, tipToolOffset, pEllipses->getSpan());
		move(pdev, h0, h1);

		cut_segment(pdev, PI, PI*3/2, a0, b0, a1, b1, 0, pEllipses->getSpan(), rootToolOffset, tipToolOffset, resolution);
		cut_segment(pdev, PI*3/2, 2*PI, a0, b0, a1, b1, 0, pEllipses->getSpan(), rootToolOffset, tipToolOffset, resolution);
	}
}


// Simple cutting routine that just sweeps the angle by a fixed amount.
void EllipseCutter::cut_simple(COutputDevice * pdev, const CutStructure::Context& context)
{
	assert(this);
	assert(pdev);

	double theta;
	PointT h0, h1;
	double rads;


	const CEllipse *first = pEllipses->getFirst();
	const CEllipse *second = pEllipses->getSecond();

	bool isExternal = pEllipses->getPlotFlags()->is_external;
	double rootToolOffset = (isExternal) ? context.rootToolOffset : (-context.rootToolOffset);
	double tipToolOffset = (isExternal) ? context.tipToolOffset : (-context.tipToolOffset);

	/** Plot top half */
	if (pEllipses->getPlotFlags()->plot_upper)
	{
		h0 = pointOnEllipse(first->getB() / 2, first->getATop(), 0, rootToolOffset, 0);
		h1 = pointOnEllipse(second->getB() / 2, second->getATop(), 0, tipToolOffset, pEllipses->getSpan());
		move(pdev, h0, h1);

		for (theta = DELTA; theta < 180.0f; theta += DELTA) {
			rads = theta*PI / 180.0;
			h0 = pointOnEllipse(first->getB() / 2, first->getATop(), rads, rootToolOffset, 0);
			h1 = pointOnEllipse(second->getB() / 2, second->getATop(), rads, tipToolOffset, pEllipses->getSpan());
			line(pdev, h0, h1);
		}
		h0 = pointOnEllipse(first->getB() / 2, first->getATop(), PI, rootToolOffset, 0);
		h1 = pointOnEllipse(second->getB() / 2, second->getATop(), PI, tipToolOffset, pEllipses->getSpan());
		line(pdev, h0, h1);
	}

	/** Continue with bottom half **/
	if (pEllipses->getPlotFlags()->plot_lower)
	{
		h0 = pointOnEllipse(first->getB() / 2, first->getABottom(), PI, rootToolOffset, 0);
		h1 = pointOnEllipse(second->getB() / 2, second->getABottom(), PI, tipToolOffset, pEllipses->getSpan());
		move(pdev, h0, h1);

		for (theta = 180.0 + DELTA; theta < 360.0; theta += DELTA) {
			rads = theta*PI / 180.0;
			h0 = pointOnEllipse(first->getB() / 2, first->getABottom(), rads, rootToolOffset, 0);
			h1 = pointOnEllipse(second->getB() / 2, second->getABottom(), rads, tipToolOffset, pEllipses->getSpan());
			line(pdev, h0, h1);
		}

		h0 = pointOnEllipse(first->getB() / 2, first->getABottom(), 2*PI, rootToolOffset, 0);
		h1 = pointOnEllipse(second->getB() / 2, second->getABottom(), 2*PI, tipToolOffset, pEllipses->getSpan());
		line(pdev, h0, h1);
	}
}

void EllipseCutter::cut(COutputDevice* pdev, const CutStructure::Context& context)
{
	assert(this);
	assert(pdev);

	if (context.optimiseOutput) {
		cut_optimised(pdev, context);
	}
	else {
		cut_simple(pdev, context);
	}
}


std::string EllipseCutter::getDescriptiveText() const
{
	return pEllipses->getDescriptiveText();
}

std::string EllipseCutter::getType() const
{
	return TYPE;
}

CutStructure* EllipseCutter::clone() const
{
	return new EllipseCutter(*this);
}

CStructure * EllipseCutter::getStructure()
{
	return pEllipses;
}

const CStructure * EllipseCutter::getStructure() const
{
	return pEllipses;
}

void EllipseCutter::serializeTo(CObjectSerializer & os)
{
	os.startSection(TYPE.c_str(), this);
	CutStructure::serializeTo(os);
	os.writeReference("ellipseRef", pEllipses);
	os.endSection();
}

void EllipseCutter::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection(TYPE.c_str(), this);
	CutStructure::serializeFrom(os);
	pEllipses = static_cast<CEllipsePair*>(os.readReference("ellipseRef"));
	os.endReadSection();
	updateBounds();
}
