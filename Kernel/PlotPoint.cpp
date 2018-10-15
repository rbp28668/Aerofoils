// PlotPoint.cpp: implementation of the CPlotPoint class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <sstream>
#include "PlotPoint.h"
#include "ObjectSerializer.h"

using namespace std;

static CObjectFactory<CPlotPoint> factory("plotPoint");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlotPoint::CPlotPoint()
: silent(false)
, span(1000)
{

}

CPlotPoint::~CPlotPoint()
{

}

void CPlotPoint::plot(COutputDevice *pdev)
{
	assert(this);
	assert(pdev);
	
	setDevice(pdev);

	if(silent)
		interp_move_to(root,tip);
	else
	{
		const float size = 1.0f;

		PointT r = root;
		PointT t = tip;

		r.fx -= size;
		t.fx -= size;
		interp_move_to(r,t); // left

		r.fx += size;
		r.fy -= size;
		t.fx += size;
		t.fy -= size;
		interp_line_to(r,t); // bottom

		r.fy += 2*size;
		t.fy += 2*size;
		interp_line_to(r,t); // top

		r.fx += size;
		r.fy -= size;
		t.fx += size;
		t.fy -= size;
		interp_line_to(r,t); // right

		r.fx -= 2*size;
		t.fx -= 2*size;
		interp_line_to(r,t); // left

	}

}

string CPlotPoint::getDescriptiveText() const
{
	stringstream ss;

	ss << "Point at " << getSectionPos() << ends;
	return ss.str();
}

float CPlotPoint::getSpan() const
{
	assert(this);
	return span;
}

void CPlotPoint::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("plotPoint",this);
	CPlotStructure::serializeTo(os);
	os.write("rootX",root.fx);
	os.write("rootY",root.fy);
	os.write("tipX", tip.fx);
	os.write("tipY", tip.fy);
	os.write("span", span);
	os.write("silent",silent);
	os.endSection();
}

void CPlotPoint::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("plotPoint",this);
	CPlotStructure::serializeFrom(os);
	os.read("rootX",root.fx);
	os.read("rootY",root.fy);
	os.read("tipX", tip.fx);
	os.read("tipY", tip.fy);
	os.read("span", span);
	os.read("silent",silent);
	os.endReadSection();
}

