// PlotStructure.cpp: implementation of the CPlotStructure class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <limits>
#include "PlotStructure.h"
#include "ObjectSerializer.h"
#include "UIProxy.h"

CPlotStructure::CBounds::CBounds()
: lastOpIsMove(false)
{
	minx = miny = std::numeric_limits<float>::max();
	maxx = maxy = -std::numeric_limits<float>::max();
}

void CPlotStructure::CBounds::MoveTo(int iStream, const PointT& pt)
{
	lastMove = pt;
	lastOpIsMove = true;
}
void CPlotStructure::CBounds::LineTo(int iStream, const PointT& pt)
{
	if(lastOpIsMove)
	{
		lastOpIsMove = false;
		LineTo(iStream,lastMove);
	}

	if(pt.fx < minx) minx = pt.fx;
	if(pt.fx > maxx) maxx = pt.fx;
	if(pt.fy < miny) miny = pt.fy;
	if(pt.fy > maxy) maxy = pt.fy;
}

RectT CPlotStructure::CBounds::getBounds() const
{
	RectT bounds(minx,maxy,maxx,miny);
	return bounds;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/************************************************************/
/************************************************************/
CPlotStructure::CPlotStructure()
: plotDevice(0)
, xpos(0), ypos(0)
, section_pos(0)
, shouldInterpolate(true)
, root_stream(0)
, tip_stream(1)
, rootIsOnLeft(true)
, proxy(0)
, invert(false)
, reflect(false)
{
}

/************************************************************/
/************************************************************/
CPlotStructure::~CPlotStructure()
{
	delete proxy;
}

/************************************************************/
/************************************************************/
COutputDevice* CPlotStructure::setDevice(COutputDevice *pdev)
{
	assert(this);
	assert(pdev);

	COutputDevice* old = plotDevice;
	plotDevice = pdev;
	return old;
}

/************************************************************/
// setUIProxy
// this sets up a basic UI proxy that will manage UI tasks 
// on behalf of this plot structure.  The plot structure
// owns the proxy and will delete the proxy when it the 
// structure itself is deleted.
// As this should be a 1:1 relationship, it's an error to
// set the proxy more than once (think ownership).
/************************************************************/
void CPlotStructure::setUIProxy(CUIProxy* pp)
{
	assert(this);
	assert(pp);
	assert(proxy == 0);
	proxy = pp;
}

/************************************************************/
/************************************************************/
CUIProxy* CPlotStructure::getUIProxy()
{
	assert(this);
	assert(proxy); // if you haven't set it then why are you asking?
	return proxy;
}

/************************************************************/
/************************************************************/
RectT CPlotStructure::getBounds()
{
	CBounds bounds;
	plot(&bounds);
	return bounds.getBounds();
}

/************************************************************/
/************************************************************/
void CPlotStructure::setSectionPos(float pos)
{
	assert(this);
	section_pos = pos;
}

/************************************************************/
/************************************************************/
float CPlotStructure::getSectionPos() const
{
	assert(this);
	return section_pos;
}

/************************************************************/
/** INTERPOLATE takes a root & a tip coordinate and using  **/
/** the wing span and section variables does a linear      **/
/** interpolation between them.                            **/
/************************************************************/
PointT CPlotStructure::interpolate(const PointT& root, const PointT& tip) const
{
	PointT here;
	
	here.fx = (root.fx * (getSpan() - section_pos)  + tip.fx * section_pos) / getSpan();
	here.fy = (root.fy * (getSpan() - section_pos)  + tip.fy * section_pos) / getSpan();
	return here;
}

/************************************************************/
/** INTERP_MOVE_TO either moves to 2 pts or  to an     **/
/** interpolated end point. Which is done depends on the  **/
/** status of plot_flags.plot_section.            **/
/************************************************************/
void CPlotStructure::interp_move_to(const PointT& root, const PointT& tip) const
{
	assert(this);
	
	if(shouldInterpolate)
	{
		PointT here = interpolate(root,tip);
		plotDevice->MoveTo(root_stream,offset(here));
	}
	else
	{
		plotDevice->MoveTo(root_stream,offset(root));
		plotDevice->MoveTo(tip_stream,offset(tip));
	}
	return;
}

/************************************************************/
/** INTERP_LINE_TO either plots 2 lines or one line to an   **/
/** interpolated end point. Which is done depends on the  **/
/** status of plot_flags.plot_section.            **/
/************************************************************/
void CPlotStructure::interp_line_to(const PointT& root,const PointT& tip) const
{
	assert(this);
	
	if(shouldInterpolate)
	{
		PointT here = interpolate(root,tip);
		plotDevice->LineTo(root_stream,offset(here));
	}
	else
	{
		plotDevice->LineTo(root_stream,offset(root));
		plotDevice->LineTo(tip_stream,offset(tip));
	}
	return;
}

void CPlotStructure::LabelAt(const PointT& where, const std::string& text, bool atRoot) const
{
	int stream = (atRoot) ? root_stream : tip_stream;
	plotDevice->MoveTo( stream, offset(where));
	plotDevice->Label( stream, text.c_str());
}

PointT CPlotStructure::offset(PointT pos) const 
{
	if(reflect)
		pos.fx = xpos - pos.fx;
	else
		pos.fx += xpos; 

	if(invert)
		pos.fy = ypos - pos.fy;
	else
		pos.fy += ypos; 
	
	return pos;
}

void CPlotStructure::setInvert(bool inv) 
{
	invert = inv;
	if(invert != inv)
	{
		RectT r = getBounds();
		float height = r.topLeft.fy - r.bottomRight.fy;
		if(invert)
			ypos -= height;
		else
			ypos += height;

	}
	invert = inv;
}

void CPlotStructure::setReflect(bool ref)
{
	if(reflect != ref) // we're changing!
	{
		RectT r = getBounds();
		float width = r.bottomRight.fx - r.topLeft.fx;

		if(reflect)
			xpos -= width;
		else
			xpos += width;
	}
	reflect = ref;
}

void CPlotStructure::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("plotStructure",this);
	os.write("xpos",xpos);
	os.write("ypos",ypos);
	os.write("position", section_pos);
	os.write("rootLeft", rootIsOnLeft);
	if(proxy)
	{
		os.startOptional("proxy");
		proxy->serializeTo(os);
		os.endOptional();
	}

	os.write("invert",invert);
	os.write("reflect",reflect);
	os.endSection();
}

void CPlotStructure::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("plotStructure",this);
	os.read("xpos",xpos);
	os.read("ypos",ypos);
	os.read("position", section_pos);
	os.read("rootLeft", rootIsOnLeft);
	setRootSide(rootIsOnLeft);
	
	if(os.ifOptional("proxy"))
	{
		CUIProxy* pp = static_cast<CUIProxy*>(os.createSubtype());
		pp->serializeFrom(os);
		setUIProxy(pp);
		os.endIfOptional();
	}
	
	if(os.ifExists("invert")) os.read("invert",invert);
	if(os.ifExists("reflect")) os.read("reflect",reflect);

	os.endReadSection();
}

/************************************************************/
/** setRootSize sets up the side of the wing root.       **/
/************************************************************/
void CPlotStructure::setRootSide(bool isLeft)
{
	rootIsOnLeft = isLeft;
	if(rootIsOnLeft)
    {
		root_stream = 0;
		tip_stream = 1;
    }
	else
    {
		root_stream = 1;
		tip_stream = 0;
    }
}

