// EllipsePlotter.cpp: implementation of the CEllipsePlotter class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <math.h>
#include <sstream>
#include "EllipsePlotter.h"
#include "Ellipse.h"
#include "EllipsePair.h"
#include "PointT.h"
#include "OutputDevice.h"
#include "ObjectSerializer.h"

using namespace std;

#define DELTA 0.1f   /* angle increment for plotting */
#define PI 3.14159265358979323846264338

static CObjectFactory<CEllipsePlotter> factory("ellipsePlotter");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipsePlotter::CEllipsePlotter(CEllipsePair* ep)
: ellipses(ep)
{

}

CEllipsePlotter::CEllipsePlotter()
: ellipses(0)
{

}

CEllipsePlotter::~CEllipsePlotter()
{

}

CStructure* CEllipsePlotter::getStructure()
{
	assert(this);
	return ellipses;
}

const CStructure* CEllipsePlotter::getStructure() const
{
	assert(this);
	return ellipses;
}

void CEllipsePlotter::plot(COutputDevice* pdev) 
{
	assert(this);
	assert(pdev);

	float theta;
	float x0,y0;
	float x1,y1;
	PointT h0,h1;
	double rads;
	
	setDevice(pdev);
	
	const CEllipse *first = ellipses->getFirst();
	const CEllipse *second = ellipses->getSecond();
	
	/** Set intial point on RHS of ellipse **/
	
	/** Plot top half */
	if(ellipses->getPlotFlags()->plot_upper)
	{
		x0 = first->getB()/2.0f;
		x1 = second->getB()/2.0f;
		y0 = y1 = 0.0f;
		
		h0.fx = x0+first->getX();
		h0.fy = y0+first->getY();
		
		h1.fx = x1+second->getX();
		h1.fy = y1+second->getY();
		
		interp_move_to(h0,h1);

		for(theta = DELTA;theta <= 180.0f; theta+=DELTA)
		{
			rads = theta*PI/180.0;
			x0 = (float)(first->getB()/2.0 * cos(rads));
			y0 = (float)(first->getATop() * sin(rads));
			x1 = (float)(second->getB()/2.0 * cos(rads));
			y1 = (float)(second->getATop() * sin(rads));
			h0.fx = x0+first->getX();
			h0.fy = y0+first->getY();
			h1.fx = x1+second->getX();
			h1.fy = y1+second->getY();
			interp_line_to(h0,h1);
		}
	}
	
	/** Continue with bottom half **/
	if(ellipses->getPlotFlags()->plot_lower)
	{
		x0 = -first->getB()/2.0f;
		x1 = -second->getB()/2.0f;
		y0 = y1 = 0.0f;
		
		h0.fx = x0+first->getX();
		h0.fy = y0+first->getY();
		
		h1.fx = x1+second->getX();
		h1.fy = y1+second->getY();
		
		interp_move_to(h0,h1);

		for(theta = 180.0f + DELTA;theta <= 360.0f; theta+=DELTA)
		{
			rads = theta*PI/180.0;
			x0=(float)(first->getB()/2.0 * cos(rads));
			y0=(float)(first->getABottom() * sin(rads));
			x1=(float)(second->getB()/2.0 * cos(rads));
			y1=(float)(second->getABottom() * sin(rads));
			h0.fx = x0+first->getX();
			h0.fy = y0+first->getY();
			h1.fx = x1+second->getX();
			h1.fy = y1+second->getY();
			interp_line_to(h0,h1);
		}
	}
	
	/** Now draw cross thro' centre of ellipse **/
	if(ellipses->getPlotFlags()->plot_markers)
    {
		x0 = first->getB()/2.0f;
		x1 = second->getB()/2.0f;
		y0 = y1 = 0.0f;
		h0.fx = x0+first->getX();
		h0.fy = y0+first->getY();
		h1.fx = x1+second->getX();
		h1.fy = y1+second->getY();
		interp_move_to(h0,h1);
		
		x0 = -first->getB()/2.0f;
		x1 = -second->getB()/2.0f;
		y0 = y1 = 0.0f;
		h0.fx = x0+first->getX();
		h0.fy = y0+first->getY();
		h1.fx = x1+second->getX();
		h1.fy = y1+second->getY();
		interp_line_to(h0,h1);
		
		x0 = x1 = 0.0f;
		if(ellipses->getPlotFlags()->plot_upper)
		{
			y0 = first->getATop();
			y1 = second->getATop();
		}
		else 
		{
			y0 = y1 = 0.0f;
		}
		h0.fx = x0+first->getX();
		h0.fy = y0+first->getY();
		h1.fx = x1+second->getX();
		h1.fy = y1+second->getY();
		interp_move_to(h0,h1);
		
		x0 = x1 = 0.0f;
		if(ellipses->getPlotFlags()->plot_lower)
		{
			y0 = -first->getABottom();
			y1 = -second->getABottom();
		}
		else
		{
			y0 = y1 = 0.0f;
		}
		h0.fx = x0+first->getX();
		h0.fy = y0+first->getY();
		h1.fx = x1+second->getX();
		h1.fy = y1+second->getY();
		interp_line_to(h0,h1);
    }
	
	return;
}

string CEllipsePlotter::getDescriptiveText() const
{
	stringstream ss;

	ss << "Ellipse at " << getSectionPos() << ends;
	return ss.str();
}

void CEllipsePlotter::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("ellipsePlotter",this);
	CPlotStructure::serializeTo(os);
	os.writeReference("ellipses",ellipses);
	os.endSection();
}

void CEllipsePlotter::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("ellipsePlotter",this);
	CPlotStructure::serializeFrom(os);
	ellipses = static_cast<CEllipsePair*>(os.readReference("ellipses"));
	os.endReadSection();
	
}
