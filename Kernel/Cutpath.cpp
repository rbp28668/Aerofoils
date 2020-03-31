/* Aerofoil
Aerofoil plotting and CNC cutter driver
Kernel / core algorithms
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
*//*
File:             CUTPATH.CPP

Program/rev.:     Aerofoil 0.0

Function:         Cutter path plotting routines

Author:           Bruce

Date:             circa Sept 91

Routines:         cut_core(CAerofoil *root,CAerofoil *tip)

Input Parameters: root and tip sections

Output Parameters:

Error conditions:

Description:        cut_core sets up spline curves for both
the root and the tip and then plots the basic
outline. Depending on settings of plot_flags
bits of structure and markers may then be drawn.

Revsision History:
					
*/

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include "cutpath.h"
#include "OutputDevice.h"
#include "PointT.h"
#include "PlotFlags.h"
#include "Aerofoil.h"
#include "Wing.h"
#include "ObjectSerializer.h"

using namespace std;

const std::string CPathCutter::TYPE("pathCutter");
static CObjectFactory<CPathCutter> factory(CPathCutter::TYPE.c_str());


/************************************************************/
/** CPathCutter::CPathCutter                               **/
/************************************************************/
CPathCutter::CPathCutter(const CWing* pw)
	: pWing(pw)
	, tool_offset(0.0f)
	, blToolOffsetSet(false)
{
	assert(pWing);
}

CPathCutter::CPathCutter()
	: pWing(0)
	, tool_offset(0.0f)
	, blToolOffsetSet(false)
{
}

/************************************************************/
/** SET_TOOL_OFFSET                                        **/
/************************************************************/
float CPathCutter::set_tool_offset(float fNewOffset)
{
	float fOldOffset = tool_offset;
	tool_offset = fNewOffset;
	blToolOffsetSet = 1;
	return fOldOffset;
}

/************************************************************/
/** GET_TOOL_OFFSET                                        **/
/************************************************************/
float CPathCutter::get_tool_offset(void)
{
	return tool_offset;
}


/**********************************************************/
/** FIND_FORWARD_PointT                                   **/
/** This find the value of u which gives the least value **/
/** of x. This should be right on the leading edge.      **/
/**********************************************************/
void CPathCutter::find_forward_PointT(const CAerofoil& foil,float *u)
{
	float u0,u1;
	float um;
	PointT middle_pt;
	PointT middle_tangent;
	int i;
	
	u0=(float)WEENY;        /* initial search posn */
	u1=(float)(1.0f-WEENY);
	
	for(i=0;i<FORWARD_ITER;++i)
    {
		um = (u0 + u1) /2;
		
		middle_pt = foil.Point(um, middle_tangent);		//get_spline_point(spline_handle,um,&middle_pt);
		//middle_tangent = spline.Tangent();	//get_spline_tangent(spline_handle,&middle_tangent);
		
		if (middle_tangent.fx >= 0)  /* then middle_pt on bottom surface */
			u1 = um;
		else                        /* middle_pt on top surface */
			u0 = um;
    }
	
	*u = (u0 + u1)/2;
	return;
}

/************************************************************/
/** PLOT_SEGMENT                                           **/
/************************************************************/
void CPathCutter::plot_segment(COutputDevice* pdev, const CWing& wing, Intercept* start, Intercept* finish, float delta)
{
	PointT tangent;
	float r_skin,t_skin;
	float ru0,ru1;
	float tu0,tu1;
	PointT r_here,t_here;
	float ru,tu;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	
	/* Get, & Scale the skin thickness to aerofoil space */
	r_skin=wing.getSkinThickness() - tool_offset;
	r_skin /= wing.getRootTransform()->getChord(); 
	t_skin=wing.getSkinThickness() - tool_offset;
	t_skin /= wing.getTipTransform()->getChord();
	
	
	/* set positions of the start an finish of this segment */
	ru0 = start->rootPosition();
	ru1 = finish->rootPosition();
	tu0 = start->tipPosition();
	tu1 = finish->tipPosition();
	
	/* Get starting PointT for root */
	r_here = root->Point(ru0,tangent);
	r_here.fx -= r_skin*tangent.fy;
	r_here.fy += r_skin*tangent.fx;
	
	/* and starting PointT for tip */
	t_here = tip->Point(tu0,tangent);
	t_here.fx -= t_skin*tangent.fy;
	t_here.fy += t_skin*tangent.fx;
	
	r_here = wing.getRootTransform()->transform(r_here);
	t_here = wing.getTipTransform()->transform(t_here);
	
	line(pdev, r_here,t_here);
	
	for(ru=ru0+delta;ru<=ru1;ru+=delta)
    {
		/* Get root coordinate */
		r_here = root->Point(ru, tangent);
		r_here.fx -= r_skin*tangent.fy;
		r_here.fy += r_skin*tangent.fx;
		
		/* and tip coordinate */
		tu=tu0+(ru-ru0)*(tu1-tu0)/(ru1-ru0);
		
		t_here = tip->Point(tu,tangent);
		t_here.fx -= t_skin*tangent.fy;
		t_here.fy += t_skin*tangent.fx;
		
		r_here = wing.getRootTransform()->transform(r_here);
		t_here = wing.getTipTransform()->transform(t_here);
		
		line(pdev, r_here,t_here);
    }
	
	r_here = root->Point(ru1,tangent);
	r_here.fx -= r_skin*tangent.fy;
	r_here.fy += r_skin*tangent.fx;
	
	t_here = tip->Point(tu1,tangent);
	t_here.fx -= t_skin*tangent.fy;
	t_here.fy += t_skin*tangent.fx;
	
	r_here = wing.getRootTransform()->transform(r_here);
	t_here = wing.getTipTransform()->transform(t_here);
	
	line(pdev, r_here,t_here);
	
	return;
}





/************************************************************/
/** SET_LE_Intercept sets up the intercept PointT for       **/
/** the leading edge.                                      **/
/************************************************************/
void CPathCutter::set_le_intercept(Intercepts& intercepts, Frame* frame)
{
	assert(this);
	assert(frame);
	
	const CAerofoil *root = pWing->getRoot();
	
	/* Do LE first: LE intercept is on the lower surface and entirely
	// determined by the root.
	*/
	if((pWing->getLE() - tool_offset) > 0.0f)
    {
		float rx=(pWing->getLE() - tool_offset) / pWing->getRootTransform()->getChord();
		float ru1 = root->FirstX(rx,0.75f,-1);  /* lower surface */
		
		/* Get tu in segment ruf..ru1. */
		float tu1 = frame->tipForward() + (ru1-frame->rootForward()) * (frame->tipFinish() - frame->tipForward())/
			(frame->rootFinish() - frame->rootForward());
		
		Intercept* intercept = new LeadingEdgeIntercept(ru1, tu1);
		intercepts.add(intercept);
    }
	return;
}

/************************************************************/
/** SET_TE_Intercept sets up the intercept PointTs for      **/
/** the trailing edge.                                     **/
/************************************************************/
void CPathCutter::set_te_intercept(Intercepts& intercepts, Frame* frame)
{
	assert(this);
	assert(frame);
	
	float rx;
	float tu0,tu1;
	float ru0,ru1;
	
	const CAerofoil *root = pWing->getRoot();
	
	/* TE has an upper and a lower intercept */
	if((pWing->getTE() - tool_offset) > 0.0f)
    {
		float root_chord = pWing->getRootTransform()->getChord();
		rx=(root_chord - pWing->getTE() + tool_offset) / root_chord;
		
		ru0 = root->FirstX(rx,frame->rootForward(),-1); /* upper surface */
		ru1 = root->FirstX(rx,frame->rootForward(),1);  /* lower surface */
		
		
		/* Get tu in segment ru0..ruf. */
		tu0 = frame->tipStart() + (ru0-frame->rootStart()) * (frame->tipForward() - frame->tipStart())/
			(frame->rootForward() - frame->rootStart());
		
		/* Get tu in segment ruf..ru1. */
		tu1 = frame->tipForward() + (ru1-frame->rootForward()) * (frame->tipFinish() - frame->tipForward())/
			(frame->rootFinish() - frame->rootForward());

		intercepts.add(new TrailingEdgeIntercept(ru0, tu0, true));
		intercepts.add(new TrailingEdgeIntercept(ru1, tu1, false));
    }
	
	return;
}


/************************************************************/
/** SET_SPARS_Intercept sets up any intercept PointTs for   **/
/** the spars.                                             **/
/************************************************************/
void CPathCutter::set_spars_intercept(Intercepts& intercepts, Frame* frame)
{
	assert(this);
	assert(frame);
	
	float rx;
	float tu0,tu1;
	float ru0,ru1;
	
	const CAerofoil *root = pWing->getRoot();
	const CTransform* rootTransform = pWing->getRootTransform();
	
	for(int i=0; i< pWing->getSparCount(); ++i)
	{
		const CSpar* spar = pWing->getSpar(i);
		
		/* intercept at midPointT of spar */
		rx = (spar->getRootX() + spar->getRootWidth()/2) / rootTransform->getChord();
		
		switch (spar->getType())
		{
		case CSpar::top:
			ru0 = root->FirstX(rx,frame->rootForward(),-1); 
			tu0 = frame->tipStart() + (ru0-frame->rootStart()) * (frame->tipForward() - frame->tipStart())/
				(frame->rootForward() - frame->rootStart());
			intercepts.add(new SparIntercept(ru0, tu0, spar));
			break;
			
		case CSpar::bottom:
			ru1 = root->FirstX(rx,frame->rootForward(),1); 
			tu1 = frame->tipForward() + (ru1-frame->rootForward()) * (frame->tipFinish() - frame->tipForward())/
				(frame->rootFinish() - frame->rootForward());
			intercepts.add(new SparIntercept(ru1, tu1, spar));
			break;
			
		case CSpar::full_depth:
			ru0 = root->FirstX(rx,frame->rootForward(),-1); 
			tu0 = frame->tipStart() + (ru0-frame->rootStart()) * (frame->tipForward() - frame->tipStart())/
				(frame->rootForward() - frame->rootStart());
			ru1 = root->FirstX(rx,frame->rootForward(),1); 
			tu1 = frame->tipForward() + (ru1-frame->rootForward()) * (frame->tipFinish() - frame->tipForward())/
				(frame->rootFinish() - frame->rootForward());
			intercepts.add(new SparIntercept(ru0, tu0, spar));
			intercepts.add(new SparIntercept(ru1, tu1, spar));
			break;
		}
	}
	
	return;
}


/************************************************************/
/** CUT_CORE  cuts the core in a single pass of the cutter.**/
/** it sets up the start, finish and le intercept PointTs,  **/
/** any intercepts for spars, LE and TE and then cuts      **/
/** segments between the intercepts.                       **/
/************************************************************/
void CPathCutter::cut(COutputDevice* pdev, double toolOffset)
{
	assert(this);
	assert(pdev);
	
	set_tool_offset((float)toolOffset);

	plot_flags = *(pWing->getPlotFlags());

	const CAerofoil *root = pWing->getRoot();
	const CAerofoil *tip = pWing->getTip();
	
	float ru0,ru1;
	float tu0,tu1;
	float delta;
	float r_skin,t_skin;
	float ruf,tuf;

	
	float root_chord = pWing->getRootTransform()->getChord();
	float tip_chord = pWing->getTipTransform()->getChord();
	
	/* Work out delta (in U) to give a line segment length of about 0.2 mm */
	delta = max(root_chord, tip_chord);
	delta = 1.0f / (5 * delta);
	
	
	/* Get, & Scale the skin thickness to aerofoil space */
	r_skin = pWing->getSkinThickness() - tool_offset;
	r_skin /= root_chord;
	t_skin = pWing->getSkinThickness() - tool_offset;
	t_skin /= tip_chord;

	
	/* Find position of the trailing edge */
	find_core_te(*root,r_skin,&ru0,&ru1);
	find_core_te(*tip,t_skin,&tu0,&tu1);
	
	// Get initial plot points - use to position cutter
	PointT tangent;
	PointT rte = root->Point(ru0,tangent);
	rte.fx -= r_skin*tangent.fy;
	rte.fy += r_skin*tangent.fx;
	rte = pWing->getRootTransform()->transform(rte);

	PointT tte = tip->Point(tu0,tangent);
	tte.fx -= t_skin*tangent.fy;
	tte.fy += t_skin*tangent.fx;
	tte = pWing->getTipTransform()->transform(tte);
	
	Intercepts intercepts; // collect up intercepts as we go. Will auto delete contents.

	/* store these values in Intercept list */
	intercepts.add(new NoOpIntercept(ru0, tu0));
	intercepts.add(new NoOpIntercept(ru1, tu1));
	
	/* Find the most forward PointTs of each section: add this as
	// an Intercept
	*/
	find_forward_PointT(*root,&ruf);
	find_forward_PointT(*tip,&tuf);
	intercepts.add(new NoOpIntercept(ruf, tuf));
	
	/* Set up coordinate frame for le/te & spars */
	Frame frame(ru0, tu0, ruf, tuf, ru1, tu1);
	
	if(plot_flags.plot_le)
    {
		set_le_intercept(intercepts, &frame);
    }
	
	if(plot_flags.plot_te)
    {
		set_te_intercept(intercepts, &frame);
    }
	
	if(plot_flags.plot_spars)
    {
		set_spars_intercept(intercepts, &frame);
    }
			
	// Run the plot....
	line(pdev, rte,tte);

	Intercept* prev = 0;
	for (auto it = intercepts.begin(); it != intercepts.end(); ++it) {
		Intercept* intercept = *it;
		if (prev) {
			plot_segment(pdev, *pWing, prev, intercept, delta); // from previous to current
		}
		
		intercept->process(this, pdev, &frame); // now run the intercept after the segment.
		prev = intercept;
	}

	line(pdev, rte,tte);
	
	return;
}

string CPathCutter::getDescriptiveText() const
{
	stringstream ss;

	ss << "Wing Core for " << pWing->getDescriptiveText() << ends;
	return ss.str();
}

std::string CPathCutter::getType() const
{
	return TYPE;
}

CStructure * CPathCutter::getStructure()
{
	assert(this);
	assert(pWing);
	return const_cast<CWing*>(pWing);  // YUK
}

const CStructure * CPathCutter::getStructure() const
{
	assert(this);
	assert(pWing);
	return pWing;
}

void CPathCutter::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection(TYPE.c_str(),this);
	CutStructure::serializeTo(os);
	os.writeReference("wing", pWing);
	os.write("toolOffset",tool_offset);
	os.endSection();
}

void CPathCutter::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection(TYPE.c_str(),this);
	CutStructure::serializeFrom(os);
	pWing = static_cast<CWing*>(os.readReference("wing"));
	os.read("toolOffset",tool_offset);
	os.endReadSection();
	updateBounds();
}

/****************************************************************************************************/
CPathCutter::Intercept::Intercept(float root_u, float tip_u)
	: ru(root_u)
	, tu(tip_u)
{
}

/****************************************************************************************************/

CPathCutter::NoOpIntercept::NoOpIntercept(float ru, float tu)
	: Intercept(ru, tu)
{
}

void CPathCutter::NoOpIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame)
{
	// No operation here!
}


/****************************************************************************************************/

CPathCutter::LeadingEdgeIntercept::LeadingEdgeIntercept(float ru, float tu)
	: Intercept(ru, tu)
{
}

void CPathCutter::LeadingEdgeIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame)
{
	const CWing* wing = cutter->wing();

	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();

	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();

	float tool_offset = cutter->tool_offset;

	float rx;
	float ru0, ru1;
	float tu0, tu1;
	PointT rt, rb, tt, tb;      /* root top, root bottom, tip top, tip bottom */
	PointT rtt, rbt, ttt, tbt;  /* root top tangent ... etc */
	//ROTATE r_washout,t_washout;
	float r_skin, t_skin;
	float ru, tu;
	PointT ri, ti;

	/* Get, & Scale the skin thickness to aerofoil space */
	r_skin = wing->getSkinThickness() - tool_offset;
	r_skin /= rootTransform->getChord();
	t_skin = wing->getSkinThickness() - tool_offset;
	t_skin /= tipTransform->getChord();


	/* ru1 and tu1 should be given by the intercept, calculate them
	anyway.*/


	/* Root u is determined by the thickness of the LE, Tip u
	// from the root U and the frame.
	*/
	rx = (wing->getLE() - tool_offset) / wing->getRootTransform()->getChord();
	ru0 = root->FirstX(rx, 0.25f, 1); /* upper surface */
	ru1 = root->FirstX(rx, 0.75f, -1);  /* lower surface */


	tu0 = frame->tipStart() + (ru0 - frame->rootStart()) * (frame->tipForward() - frame->tipStart()) /
		(frame->rootForward() - frame->rootStart());


	tu1 = frame->tipForward() + (ru1 - frame->rootForward()) * (frame->tipFinish() - frame->tipForward()) /
		(frame->rootFinish() - frame->rootForward());

	/* Get the 4 PointTs and their tangents */
	rt = root->Point(ru0, rtt);
	rb = root->Point(ru1, rbt);

	tt = tip->Point(tu0, ttt);
	tb = tip->Point(tu1, tbt);

	/* Move in (or out) to the skin */
	rt.fx -= r_skin * rtt.fy;
	rt.fy += r_skin * rtt.fx;

	rb.fx -= r_skin * rbt.fy;
	rb.fy += r_skin * rbt.fx;

	tt.fx -= t_skin * ttt.fy;
	tt.fy += t_skin * ttt.fx;

	tb.fx -= t_skin * tbt.fy;
	tb.fy += t_skin * tbt.fx;

	/* Now work out where the tangents meet: calculate ru and tu
	// at intersection PointT such that these values of u, placed
	// in the parametric line equations of the lower surface
	// tangents, will give the intersection. Substitute back to
	// into the lower line to give the intersection PointT.
	*/
	ru = (rtt.fx * (rt.fy - rb.fy) + rtt.fy * (rt.fx - rb.fx)) /
		(rtt.fx * rbt.fy - rtt.fy * rbt.fx);
	ri.fx = rb.fx + ru * rbt.fx;
	ri.fy = rb.fy + ru * rbt.fy;

	tu = (ttt.fx * (tt.fy - tb.fy) + ttt.fy * (tt.fx - tb.fx)) /
		(ttt.fx * tbt.fy - ttt.fy * tbt.fx);
	ti.fx = tb.fx + tu * tbt.fx;
	ti.fy = tb.fy + tu * tbt.fy;

	rt = rootTransform->transform(rt);
	rb = rootTransform->transform(rb);
	ri = rootTransform->transform(ri);

	tt = tipTransform->transform(tt);
	tb = tipTransform->transform(tb);
	ti = tipTransform->transform(ti);


	/* OK, so we now want to cut up the front of the wing from
	// bottom to top. forward to the intersection and back to
	// the bottom. Cutter should already be at the bottom.
	*/
	cutter->line(output, rt, tt);
	cutter->line(output, ri, ti);
	cutter->line(output, rb, tb);

	return;
}

/****************************************************************************************************/

CPathCutter::TrailingEdgeIntercept::TrailingEdgeIntercept(float ru, float tu, bool topSurface)
	: Intercept(ru, tu)
	, topSurface(topSurface)
{
}

void CPathCutter::TrailingEdgeIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame)
{
	const CWing* wing = cutter->wing();
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();
	const float tool_offset = cutter->tool_offset;

	float ru0, ru1;
	float tu0, tu1;
	PointT rt, rb, tt, tb;      /* root top, root bottom, tip top, tip bottom */
	PointT rtt, rbt, ttt, tbt;  /* root top tangent ... etc */
	PointT rm, tm;            /* middle PointTs */
	//  ROTATE r_washout,t_washout;
	float r_skin, t_skin;
	float rx;

	/* Get, & Scale the skin thickness to aerofoil space */
	r_skin = wing->getSkinThickness() - tool_offset;
	r_skin /= rootTransform->getChord();
	t_skin = wing->getSkinThickness() - tool_offset;
	t_skin /= tipTransform->getChord();

	/* Calculate top and bottom PointTs */
	float root_chord = rootTransform->getChord();
	rx = (root_chord - wing->getTE() + tool_offset) / root_chord;

	ru0 = root->FirstX(rx, frame->rootForward(), -1); /* upper surface */
	ru1 = root->FirstX(rx, frame->rootForward(), 1);  /* lower surface */


	/* Get tu in segment ru0..ruf. */
	tu0 = frame->tipStart() + (ru0 - frame->rootStart()) * (frame->tipForward() - frame->tipStart()) /
		(frame->rootForward() - frame->rootStart());


	/* Get tu in segment ruf..ru1. */
	tu1 = frame->tipForward() + (ru1 - frame->rootForward()) * (frame->tipFinish() - frame->tipForward()) /
		(frame->rootFinish() - frame->rootForward());

	/* Get the 4 PointTs and their tangents */
	rt = root->Point(ru0, rtt);
	rb = root->Point(ru1, rbt);

	tt = tip->Point(tu0, ttt);
	tb = tip->Point(tu1, tbt);

	/* Move in (or out) to the skin */
	rt.fx -= r_skin * rtt.fy;
	rt.fy += r_skin * rtt.fx;

	rb.fx -= r_skin * rbt.fy;
	rb.fy += r_skin * rbt.fx;

	tt.fx -= t_skin * ttt.fy;
	tt.fy += t_skin * ttt.fx;

	tb.fx -= t_skin * tbt.fy;
	tb.fy += t_skin * tbt.fx;

	/* Find the midPointT */
	rm.fx = (rt.fx + rb.fx) / 2;
	rm.fy = (rt.fy + rb.fy) / 2;

	tm.fx = (tt.fx + tb.fx) / 2;
	tm.fy = (tt.fy + tb.fy) / 2;

	rt = rootTransform->transform(rt);
	rb = rootTransform->transform(rb);
	rm = rootTransform->transform(rm);

	tt = tipTransform->transform(tt);
	tb = tipTransform->transform(tb);
	tm = tipTransform->transform(tm);

	/* Now cut... */
	cutter->line(output, rm, tm);    /* cut to midPointT */

	if (topSurface)         /* then top surface */
		cutter->line(output, rt, tt);    /* so move back up */
	else                        /* bottom surface */
		cutter->line(output, rb, tb);    /* so move back down */
	return;
}

/****************************************************************************************************/
CPathCutter::SparIntercept::SparIntercept(float ru, float tu, const CSpar* spar)
	: Intercept(ru, tu)
	, pSpar(spar)
{
}

void CPathCutter::SparIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame)
{
	const CWing* wing = cutter->wing();
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();

	const float tool_offset = cutter->tool_offset;

	int i;
	PointT rpos[5];
	PointT tpos[5];
	PointT rp, tp;
	PointT ros, tos;
	float ru, tu;

	/* Get, & Scale the skin thickness to aerofoil space */
	float r_skin = wing->getSkinThickness() - tool_offset;
	r_skin /= rootTransform->getChord();
	float t_skin = wing->getSkinThickness() - tool_offset;
	t_skin /= tipTransform->getChord();


	/* Work out the spar dimensions */
	float root_height = pSpar->getRootHeight();
	float root_width = pSpar->getRootWidth() - 2 * tool_offset;
	float tip_height = pSpar->getTipHeight();
	float tip_width = pSpar->getTipWidth() - 2 * tool_offset;
	if (!pSpar->isSubmerged())
	{
		root_height -= wing->getSkinThickness();
		tip_height -= wing->getSkinThickness();
	}
	root_height /= rootTransform->getChord();
	root_width /= rootTransform->getChord();
	tip_height /= tipTransform->getChord();
	tip_width /= tipTransform->getChord();


	/* Get the start PointTs on the root and tip */
	PointT rst, tst;                /* and their tangents */
	PointT rs = root->Point(rootPosition(), rst);
	PointT ts = tip->Point(tipPosition(), tst);

	PointT tangent = rst;  /* root determines spar orientation */


	/* if spar is full depth then the tangent and the depth */
	/* need to be re-calculated. Tangent is horizontal (as the */
	/* spar should run up and down */
	if (pSpar->isFullDepth())
	{
		if (rootPosition() < frame->rootForward())  /* then top intercept */
		{
			/* So set up tangent and other PointT on lower surface */
			tangent.fx = -1;
			tangent.fy = 0;
			ru = root->FirstX(rs.fx, frame->rootForward(), 1);
			tu = tip->FirstX(ts.fx, frame->tipForward(), 1);
		}
		else                        /* bottom intercept */
		{
			tangent.fx = 1;
			tangent.fy = 0;
			ru = root->FirstX(rs.fx, frame->rootForward(), -1);
			tu = tip->FirstX(ts.fx, frame->tipForward(), -1);
		}

		/* Get PointT on the other side from the start PointT.*/
		ros = root->Point(ru);
		tos = tip->Point(tu);

		/* Work out 1/2 the wing thickness */
		if (rs.fy > ros.fy)
			root_height = rs.fy - ros.fy;
		else
			root_height = ros.fy - rs.fy;

		if (ts.fy > tos.fy)
			tip_height = ts.fy - tos.fy;
		else
			tip_height = tos.fy - ts.fy;

		root_height /= 2;
		tip_height /= 2;

		/* and allow for skin & tool clearance */
		root_height -= (wing->getSkinThickness() - tool_offset) / rootTransform->getChord();
		tip_height -= (wing->getSkinThickness() - tool_offset) / tipTransform->getChord();
	}


	/* Move the start PointT in (or out) to the skin */
	rs.fx -= r_skin * rst.fy;
	rs.fy += r_skin * rst.fx;

	ts.fx -= t_skin * tst.fy;
	ts.fy += t_skin * tst.fx;

	/* Now run round setting up the PointT arrays. */

	rp = rs;
	tp = ts;

	rp.fx -= tangent.fx * root_width / 2;
	rp.fy -= tangent.fy * root_width / 2;
	tp.fx -= tangent.fx * tip_width / 2;
	tp.fy -= tangent.fy * tip_width / 2;
	rpos[0] = rp;
	tpos[0] = tp;

	rp.fx -= tangent.fy * root_height;
	rp.fy += tangent.fx * root_height;
	tp.fx -= tangent.fy * tip_height;
	tp.fy += tangent.fx * tip_height;
	rpos[1] = rp;
	tpos[1] = tp;

	rp.fx += tangent.fx * root_width;
	rp.fy += tangent.fy * root_width;
	tp.fx += tangent.fx * tip_width;
	tp.fy += tangent.fy * tip_width;
	rpos[2] = rp;
	tpos[2] = tp;

	rp.fx += tangent.fy * root_height;
	rp.fy -= tangent.fx * root_height;
	tp.fx += tangent.fy * tip_height;
	tp.fy -= tangent.fx * tip_height;
	rpos[3] = rp;
	tpos[3] = tp;


	rpos[4] = rs; /* back to start. */
	tpos[4] = ts;

	for (i = 0; i < 5; ++i)
	{
		rpos[i] = rootTransform->transform(rpos[i]);
		tpos[i] = tipTransform->transform(tpos[i]);

		/* Now cut... */
		cutter->line(output, rpos[i], tpos[i]);    /* cut to midPointT */
	}

	return;
}


/****************************************************************************************************/

CPathCutter::Frame::Frame(float ru0, float tu0, float ruf, float tuf, float ru1, float tu1)
	: ru0(ru0)
	, tu0(tu0)
	, ruf(ruf)
	, tuf(tuf)
	, ru1(ru1)
	, tu1(tu1)
{
}

/****************************************************************************************************/

// Compare 2 intercepts by root position.  
bool CPathCutter::Intercepts::compare_intercepts(const CPathCutter::Intercept* first, const CPathCutter::Intercept* second)
{
	return first->rootPosition() < second->rootPosition();
}


CPathCutter::Intercepts::Intercepts()
	: isSorted(true) // as list empty
{
}

CPathCutter::Intercepts::~Intercepts()
{
	for (auto it = intercepts.begin(); it != intercepts.end(); ++it) {
		delete (*it);
	}
}

void CPathCutter::Intercepts::add(Intercept* intercept)
{
	assert(this);
	assert(intercept);
	intercepts.push_back(intercept);
	isSorted = false;
}

std::list<CPathCutter::Intercept*>::const_iterator CPathCutter::Intercepts::begin()
{
	if (!isSorted) {
		intercepts.sort(compare_intercepts);
		isSorted = true;
	}
	return intercepts.begin();
}

std::list<CPathCutter::Intercept*>::const_iterator CPathCutter::Intercepts::end()
{
	return intercepts.end();
}
