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


EllipseCutter::~EllipseCutter()
{
}

void EllipseCutter::cut(COutputDevice * pdev, double toolOffset)
{
	assert(this);
	assert(pdev);

	float theta;
	float x0, y0;
	float x1, y1;
	PointT h0, h1;
	double rads;


	const CEllipse *first = pEllipses->getFirst();
	const CEllipse *second = pEllipses->getSecond();

	/** Set intial point on RHS of ellipse **/

	/** Plot top half */
	if (pEllipses->getPlotFlags()->plot_upper)
	{
		x0 = first->getB() / 2.0f;
		x1 = second->getB() / 2.0f;
		y0 = y1 = 0.0f;

		h0.fx = x0 + first->getX();
		h0.fy = y0 + first->getY();

		h1.fx = x1 + second->getX();
		h1.fy = y1 + second->getY();

		move(pdev, h0, h1);

		for (theta = DELTA; theta <= 180.0f; theta += DELTA)
		{
			rads = theta*PI / 180.0;
			x0 = (float)(first->getB() / 2.0 * cos(rads));
			y0 = (float)(first->getATop() * sin(rads));
			x1 = (float)(second->getB() / 2.0 * cos(rads));
			y1 = (float)(second->getATop() * sin(rads));
			h0.fx = x0 + first->getX();
			h0.fy = y0 + first->getY();
			h1.fx = x1 + second->getX();
			h1.fy = y1 + second->getY();
			line(pdev, h0, h1);
		}
	}

	/** Continue with bottom half **/
	if (pEllipses->getPlotFlags()->plot_lower)
	{
		x0 = -first->getB() / 2.0f;
		x1 = -second->getB() / 2.0f;
		y0 = y1 = 0.0f;

		h0.fx = x0 + first->getX();
		h0.fy = y0 + first->getY();

		h1.fx = x1 + second->getX();
		h1.fy = y1 + second->getY();

		line(pdev, h0, h1);

		for (theta = 180.0f + DELTA; theta <= 360.0f; theta += DELTA)
		{
			rads = theta*PI / 180.0;
			x0 = (float)(first->getB() / 2.0 * cos(rads));
			y0 = (float)(first->getABottom() * sin(rads));
			x1 = (float)(second->getB() / 2.0 * cos(rads));
			y1 = (float)(second->getABottom() * sin(rads));
			h0.fx = x0 + first->getX();
			h0.fy = y0 + first->getY();
			h1.fx = x1 + second->getX();
			h1.fy = y1 + second->getY();
			line(pdev, h0, h1);
		}
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
