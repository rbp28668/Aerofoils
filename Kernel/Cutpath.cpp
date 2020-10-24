
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

#define DELTA 0.1f   /* angle increment for plotting */
#define PI 3.14159265358979323846264338

/************************************************************/
/** CPathCutter::CPathCutter                               **/
/************************************************************/
CPathCutter::CPathCutter(const CWing* pw)
	: pWing(pw)
	, mode(Mode::NORMAL)

{
	assert(pWing);
}

CPathCutter::CPathCutter()
	: pWing(0)
	, mode(Mode::NORMAL)
{
}

CPathCutter::CPathCutter(const CPathCutter& source)
	: CutStructure(source)
	, plot_flags(source.plot_flags)
	, pWing(source.pWing)
	, mode(source.mode)
{
}


/**********************************************************/
/** FIND_FORWARD_PointT                                   **/
/** This find the value of u which gives the least value **/
/** of x. This should be right on the leading edge.      **/
/**********************************************************/
void CPathCutter::find_forward_PointT(const CAerofoil& foil,NumericT *u)
{
	NumericT u0,u1;
	NumericT um;
	PointT middle_pt;
	PointT middle_tangent;
	int i;
	
	u0=(NumericT)WEENY;        /* initial search posn */
	u1=(NumericT)(1.0f-WEENY);
	
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
void CPathCutter::plot_segment(COutputDevice* pdev, Intercept* start, Intercept* finish, NumericT delta, const CutStructure::Context& context)
{
	assert(this);
	assert(pdev);
	assert(start);
	assert(finish);
	assert(start->rootPosition() < finish->rootPosition());
	assert(start->tipPosition() < finish->tipPosition());

	/* Get, & Scale the skin thickness to aerofoil space */
	NumericT r_skin, t_skin;
	calc_skin(r_skin, t_skin, context);

	NumericT ru0 = start->rootPosition();
	NumericT ru1 = finish->rootPosition();
	NumericT tu0 = start->tipPosition();
	NumericT tu1 = finish->tipPosition();

	if (context.optimiseOutput) {
		// scale tolerance to aerofoil space
		NumericT t = context.tolerance / max(pWing->getRootTransform()->getChord(), pWing->getTipTransform()->getChord());
		toPoint(ru0, pdev, start, finish, r_skin, t_skin);
		plot_segment_opt(pdev, ru0, ru1, tu0, tu1, t, context);
	}
	else {
		for (NumericT ru = ru0; ru <= ru1 - delta; ru += delta) {
			toPoint(ru, pdev, start, finish, r_skin, t_skin);
		}
		toPoint(ru1, pdev, start, finish, r_skin, t_skin);
	}
	return;
}

/************************************************************/
/** PLOT_SEGMENT_REVERSE                                   **/
/************************************************************/
void CPathCutter::plot_segment_reverse(COutputDevice* pdev, Intercept* start, Intercept* finish, NumericT delta, const CutStructure::Context& context)
{
	assert(pdev);
	assert(start);
	assert(finish);
	assert(delta > 0);
	assert(start->rootPosition() > finish->rootPosition());  // U should be decreasing as we cut round the wing
	assert(start->tipPosition() > finish->tipPosition());

	/* Get, & Scale the skin thickness to aerofoil space */
	NumericT r_skin, t_skin;
	calc_skin(r_skin, t_skin, context);

	NumericT ru0 = start->rootPosition();
	NumericT ru1 = finish->rootPosition();
	NumericT tu0 = start->tipPosition();
	NumericT tu1 = finish->tipPosition();

	if (context.optimiseOutput) {
		// scale tolerance to aerofoil space
		NumericT t = context.tolerance / max(pWing->getRootTransform()->getChord(), pWing->getTipTransform()->getChord());
		toPoint(ru0, pdev, start, finish, r_skin, t_skin);
		plot_segment_opt(pdev, ru0, ru1, tu0, tu1, t, context);
	}
	else {
		for (NumericT ru = ru0; ru >= ru1 + delta; ru -= delta) {
			toPoint(ru, pdev, start, finish, r_skin, t_skin);
		}
		toPoint(ru1, pdev, start, finish, r_skin, t_skin);
	}
	return;
}

/************************************************************/
// PLOT_SEGMENT_OPT
// Recursive plot optimised segment.  Note that this works in
// both directions providing that ru0, tu0 are always the 
// start and ru1,tu1 are always the finish. 
/************************************************************/
void CPathCutter::plot_segment_opt(COutputDevice* pdev, NumericT ru0, NumericT ru1,  NumericT tu0, NumericT tu1, NumericT scaledTolerance, const CutStructure::Context& context)
{
	assert(this);
	assert(pdev);

	/* Get, & Scale the skin thickness to aerofoil space */
	NumericT r_skin, t_skin;
	calc_skin(r_skin, t_skin, context);

	const CAerofoil* root = pWing->getRoot();
	const CAerofoil* tip = pWing->getTip();
	
	// Ends of line segment
	PointT rp0 = root->Point(ru0);
	PointT rp1 = root->Point(ru1);
	PointT tp0 = tip->Point(tu0);
	PointT tp1 = tip->Point(tu1);

	// Midpoint of line segment
	NumericT rmidu = (ru0 + ru1) / 2;
	NumericT tmidu = (tu0 + tu1) / 2;

	PointT rmid = root->Point(rmidu);
	PointT tmid = tip->Point(tmidu);

	NumericT dr = error_distance(rp0, rp1, rmid);
	NumericT dt = error_distance(tp0, tp1, tmid);

	if (dr < scaledTolerance && dt < scaledTolerance) {
		// Line from rp0 to rp1 and tp0 to tp1
		/* Get, & Scale the skin thickness to aerofoil space */
		NumericT r_skin, t_skin;
		calc_skin(r_skin, t_skin, context);

		// Just calculate the finish end of the line segment
		// Start end should already have been emitted.
		PointT r_here = root->PointAtSkin(ru1, r_skin);
		PointT t_here = tip->PointAtSkin(tu1, t_skin);

		r_here = pWing->getRootTransform()->transform(r_here);
		t_here = pWing->getTipTransform()->transform(t_here);
		r_here.fz = 0;
		t_here.fz = pWing->getSpan();
		line(pdev, r_here, t_here);
	}
	else {  // Else out of tolerance so need to split
		// Note - important we do them in order low half, high half as that
		// ensures output is in the correct order.
		plot_segment_opt(pdev, ru0, rmidu, tu0,tmidu, scaledTolerance, context);
		plot_segment_opt(pdev, rmidu, ru1, tmidu, tu1, scaledTolerance, context);
	}
		return;
}



NumericT CPathCutter::error_distance(PointT p0, PointT p1, PointT midpoint)
{
	// See https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
	NumericT dx = p1.fx - p0.fx;
	NumericT dy = p1.fy - p0.fy;

	NumericT dist = abs(dy * midpoint.fx - dx * midpoint.fy + p1.fx * p0.fy - p1.fy * p0.fx) /
		sqrt( dx * dx + dy * dy);

	return dist;
}

/************************************************************/
/** TO_POINT                                               **/
/************************************************************/
void CPathCutter::toPoint(NumericT ru, COutputDevice* pdev, Intercept* start, Intercept* finish, NumericT r_skin, NumericT t_skin) {
	const CAerofoil* root = pWing->getRoot();
	const CAerofoil* tip = pWing->getTip();

	// Calculate the tip u such that as root u moves between the start 
	// and finish intercepts, the tip u moves between them at the rate
	// to ensure they arrive at the finish at the same time i.e.
	// tu = finish->tipPosition() when ru = finish->rootPosition().
	NumericT ru0 = start->rootPosition();
	NumericT ru1 = finish->rootPosition();
	NumericT tu0 = start->tipPosition();
	NumericT tu1 = finish->tipPosition();
	NumericT tu = tu0 + (ru - ru0) * (tu1 - tu0) / (ru1 - ru0);

	PointT r_here = root->PointAtSkin(ru, r_skin);
	PointT t_here = tip->PointAtSkin(tu, t_skin);

	r_here = pWing->getRootTransform()->transform(r_here);
	t_here = pWing->getTipTransform()->transform(t_here);
	r_here.fz = 0;
	t_here.fz = pWing->getSpan();
	line(pdev, r_here, t_here);
}



/************************************************************/
/** CALC_SKIN calculates the skin thickness in aerofoil    **/
/** space for both root and tip.                           **/
/************************************************************/
void CPathCutter::calc_skin(NumericT& r_skin, NumericT& t_skin, const CutStructure::Context& context) const
{
	r_skin = pWing->getSkinThickness() - context.toolOffset;
	r_skin /= pWing->getRootTransform()->getChord();
	t_skin = pWing->getSkinThickness() - context.toolOffset;
	t_skin /= pWing->getTipTransform()->getChord();
}

/************************************************************/
/** SET_LE_Intercept sets up the intercept PointT for       **/
/** the leading edge.                                      **/
/************************************************************/
void CPathCutter::set_le_intercept(Intercepts& intercepts, const Frame* frame, const CutStructure::Context& context)
{
	assert(this);
	assert(frame);
	
	const CAerofoil *root = pWing->getRoot();
	
	/* Do LE first: LE intercept is on the lower surface and entirely
	// determined by the root.
	*/
	NumericT position = pWing->getLE() - context.toolOffset;
	if(position > 0.0f)
    {
		NumericT rx = position / pWing->getRootTransform()->getChord();
		NumericT ru0 = root->FirstX(rx, 0.25f, 1);  /* upper surface */
		NumericT ru1 = root->FirstX(rx,0.75f,-1);  /* lower surface */
		
		/* Get tu in segment ruf..ru1. */
		NumericT tu0 = frame->toTip(ru0);
		NumericT tu1 = frame->toTip(ru1);

		intercepts.add(new LeadingEdgeIntercept(ru0, tu0, true));
		intercepts.add(new LeadingEdgeIntercept(ru1, tu1, false));
    }
	return;
}

/************************************************************/
/** SET_TE_Intercept sets up the intercept PointTs for      **/
/** the trailing edge.                                     **/
/************************************************************/
void CPathCutter::set_te_intercept(Intercepts& intercepts, const Frame* frame, const CutStructure::Context& context)
{
	assert(this);
	assert(frame);
	
	NumericT rx;
	NumericT tu0,tu1;
	NumericT ru0,ru1;
	
	const CAerofoil *root = pWing->getRoot();
	
	/* TE has an upper and a lower intercept */
	if((pWing->getTE() - context.toolOffset) > 0.0f)
    {
		NumericT root_chord = pWing->getRootTransform()->getChord();
		rx=(root_chord - pWing->getTE() + context.toolOffset) / root_chord;
		
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
void CPathCutter::set_spars_intercept(Intercepts& intercepts, const Frame* frame, const CutStructure::Context& context)
{
	assert(this);
	assert(frame);
	
	NumericT rx;
	NumericT tu0,tu1;
	NumericT ru0,ru1;
	
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

void CPathCutter::set_cutouts_intercept(Intercepts& intercepts, const Frame* frame, const CutStructure::Context& context)
{
	const CAerofoil* root = pWing->getRoot();
	const CAerofoil* tip = pWing->getTip();
	const CTransform* rootTransform = pWing->getRootTransform();
	const CTransform* tipTransform = pWing->getTipTransform();

	NumericT root_chord = rootTransform->getChord();
	NumericT tip_chord = tipTransform->getChord();

	for (int i = 0; i < pWing->getCutoutCount(); ++i)
	{
		NumericT ru, tu;
		const Cutout* cutout = pWing->getCutout(i);

		NumericT rx = cutout->getRootX() / root_chord;
		//NumericT tx = cutout->getTipX() / tip_chord;

		// Doesn't mater whether centred vertically.  Need to pick upper or lower intercept
		if (cutout->getTopSurface()) {
			ru = root->FirstX(rx, frame->rootForward(), -1); /* upper surface */
			tu = frame->tipStart() + (ru - frame->rootStart()) * (frame->tipForward() - frame->tipStart()) /
				(frame->rootForward() - frame->rootStart());
		} else { // bottom surface
			ru = root->FirstX(rx, frame->rootForward(), 1);
			tu = frame->tipForward() + (ru - frame->rootForward()) * (frame->tipFinish() - frame->tipForward()) /
				(frame->rootFinish() - frame->rootForward());
		}

		intercepts.add(new CutoutIntercept(ru, tu, cutout));
	}

}

/************************************************************/
/** CREATE_FRAME                                           **/
/** initialises the cutting frame of reference which has   **/
/** the start forward and finish points for both root and  **/
/** tip                                                    **/
/************************************************************/
void CPathCutter::createFrame(Frame& frame, const CutStructure::Context& context)
{
	assert(this);

	NumericT ru0, ru1; // root start and finish point
	NumericT tu0, tu1; // tip start and finish point
	NumericT ruf, tuf; // root and tip forward point

	NumericT root_chord = pWing->getRootTransform()->getChord();
	NumericT tip_chord = pWing->getTipTransform()->getChord();

	/* Get, & Scale the skin thickness to aerofoil space */
	NumericT r_skin, t_skin;
	calc_skin(r_skin, t_skin, context);

	/* Find position of the trailing edge */
	const CAerofoil* root = pWing->getRoot();
	const CAerofoil* tip = pWing->getTip();
	find_core_te(*root, r_skin, &ru0, &ru1);
	find_core_te(*tip, t_skin, &tu0, &tu1);

	/* Find the most forward PointTs of each section: add this as
	// an Intercept
	*/
	find_forward_PointT(*root, &ruf);
	find_forward_PointT(*tip, &tuf);

	frame = Frame(ru0, tu0, ruf, tuf, ru1, tu1);
}

/************************************************************/
/** CREATE_INTERCEPTS creates all the intercept points for **/
/** this cut.                                              **/
/************************************************************/
void CPathCutter::createIntercepts(Intercepts& intercepts, const Frame& frame, const CutStructure::Context& context)
{
	assert(this);

	plot_flags = *(pWing->getPlotFlags());

	/* store these values in Intercept list */
	intercepts.add(new NoOpIntercept(frame.rootStart(), frame.tipStart()));
	intercepts.add(new NoOpIntercept(frame.rootForward(), frame.tipForward()));
	intercepts.add(new NoOpIntercept(frame.rootFinish(), frame.tipFinish()));

	if (plot_flags.plot_le) {
		set_le_intercept(intercepts, &frame, context);
	}

	if (plot_flags.plot_te) {
		set_te_intercept(intercepts, &frame, context);
	}

	if (plot_flags.plot_spars) {
		set_spars_intercept(intercepts, &frame, context);
	}

	if (plot_flags.plot_cutouts) {
		set_cutouts_intercept(intercepts, &frame, context);
	}

}
/************************************************************/
/** CUT_CORE  cuts the core in a single pass of the cutter.**/
/** it sets up the start, finish and le intercept PointTs,  **/
/** any intercepts for spars, LE and TE and then cuts      **/
/** segments between the intercepts.                       **/
/************************************************************/
void CPathCutter::cut(COutputDevice* pdev, const CutStructure::Context& context)
{
	assert(this);
	assert(pdev);
	
	plot_flags = *(pWing->getPlotFlags());
	
	/* Get, & Scale the skin thickness to aerofoil space */
	NumericT root_chord = pWing->getRootTransform()->getChord();
	NumericT tip_chord = pWing->getTipTransform()->getChord();
	
	NumericT r_skin, t_skin;
	calc_skin(r_skin, t_skin, context);
	
	Frame frame(0, 0, 0, 0, 0, 0);
	Intercepts intercepts; // collect up intercepts as we go. Will auto delete contents.
	createFrame(frame, context);
	createIntercepts(intercepts, frame, context);

	// Get initial plot points - use to position cutter
	PointT rte = pWing->getRoot()->PointAtSkin(frame.rootStart(), r_skin);
	rte = pWing->getRootTransform()->transform(rte);

	PointT tte = pWing->getTip()->PointAtSkin(frame.tipStart(), t_skin);
	tte = pWing->getTipTransform()->transform(tte);

	PointT rle = pWing->getRoot()->PointAtSkin(frame.rootForward(), r_skin);
	rle = pWing->getRootTransform()->transform(rle);

	PointT tle = pWing->getTip()->PointAtSkin(frame.tipForward(), t_skin);
	tle = pWing->getTipTransform()->transform(tle);

	rte.fz = rle.fz = 0;
	tte.fz = tle.fz = pWing->getSpan();

	/* Work out delta (in U) to give a line segment length of about 0.2 mm */
	NumericT delta = max(root_chord, tip_chord);
	delta = 1.0f / (5 * delta);

	// Run the plot....

	Intercept* prev = 0;
	bool skipSegments = false;

	switch (mode) {
	case NORMAL:
		line(pdev, rte, tte);

		for (auto it = intercepts.begin(); it != intercepts.end(); ++it) {
			Intercept* intercept = *it;
			if (prev  && !skipSegments) {
				plot_segment(pdev, prev, intercept, delta, context); // from previous to current
			}
			intercept->process(this, pdev, &frame, context); // now run the intercept after the segment.
			if (intercept->skipSegment()) skipSegments = !skipSegments;
			prev = intercept;
		}

		line(pdev, rte, tte);
		break;

	case REVERSE:
		line(pdev, rte, tte);

		for (auto it = intercepts.rbegin(); it != intercepts.rend(); ++it) {
			Intercept* intercept = *it;
			if (prev && !skipSegments) {
				plot_segment_reverse(pdev, prev, intercept, delta, context); // from previous to current
			}

			intercept->process(this, pdev, &frame, context); // now run the intercept after the segment.
			if (intercept->skipSegment()) skipSegments = !skipSegments;
			prev = intercept;
		}

		line(pdev, rte, tte);
		break;

	case TOP_FROM_LE:
		line(pdev, rle, tle);

		for (auto it = intercepts.rbegin(); it != intercepts.rend(); ++it) {
			Intercept* intercept = *it;

			// Skip over bottom surface
			if (intercept->rootPosition() > frame.rootForward()) {
				if (intercept->skipSegment()) skipSegments = !skipSegments; // track even if we're not cutting
				continue;
			}

			if (prev && !skipSegments) {
				plot_segment_reverse(pdev, prev, intercept, delta, context); // from previous to current
			}

			intercept->process(this, pdev, &frame, context); // now run the intercept after the segment.
			if (intercept->skipSegment()) skipSegments = !skipSegments;
			prev = intercept;
		}

		line(pdev, rte, tte);
		break;

	case TOP_FROM_TE:
		line(pdev, rte, tte);

		for (auto it = intercepts.begin(); it != intercepts.end(); ++it) {
			Intercept* intercept = *it;

			// Skip over bottom surface
			if (intercept->rootPosition() > frame.rootForward()) {
				break;
			}

			if (prev && !skipSegments) {
				plot_segment(pdev, prev, intercept, delta, context); // from previous to current
			}

			intercept->process(this, pdev, &frame, context); // now run the intercept after the segment.
			if (intercept->skipSegment()) skipSegments = !skipSegments;
			prev = intercept;
		}

		line(pdev, rle, tle);
		break;

	case BOTTOM_FROM_LE:
		line(pdev, rle, tle);

		for (auto it = intercepts.begin(); it != intercepts.end(); ++it) {
			Intercept* intercept = *it;

			// Skip over bottom surface
			if (intercept->rootPosition() < frame.rootForward()) {
				if (intercept->skipSegment()) skipSegments = !skipSegments; // track even if we're not cutting
				continue;
			}

			if (prev && !skipSegments) {
				plot_segment(pdev, prev, intercept, delta, context); // from previous to current
			}

			intercept->process(this, pdev, &frame, context); // now run the intercept after the segment.
			if (intercept->skipSegment()) skipSegments = !skipSegments;
			prev = intercept;
		}

		line(pdev, rte, tte);

		break;

	case BOTTOM_FROM_TE:
		line(pdev, rte, tte);

		for (auto it = intercepts.rbegin(); it != intercepts.rend(); ++it) {
			Intercept* intercept = *it;

			// Skip over top surface
			if (intercept->rootPosition() < frame.rootForward()) {
				break;
			}

			if (prev && !skipSegments) {
				plot_segment_reverse(pdev, prev, intercept, delta, context); // from previous to current
			}

			intercept->process(this, pdev, &frame, context); // now run the intercept after the segment.
			if (intercept->skipSegment()) skipSegments = !skipSegments;
			prev = intercept;
		}

		line(pdev, rle, tle);
		break;
	}

	return;
}

string CPathCutter::getDescriptiveText() const
{
	stringstream ss;
	char* modeText = "";

	switch (mode) {
	case NORMAL:
		modeText = "";
		break;

	case REVERSE:
		modeText = "REVERSE ";
		break;

	case TOP_FROM_LE:
		modeText = "TOP LE->TE ";
		break;

	case TOP_FROM_TE:
		modeText = "TOP TE->LE ";
		break;

	case BOTTOM_FROM_LE:
		modeText = "BOTTOM LE->TE ";
		break;

	case BOTTOM_FROM_TE:
		modeText = "BOTTOM TE->LE ";
		break;
	}

	ss << "Wing Core " << modeText << "for " << pWing->getDescriptiveText() << ends;
	return ss.str();
}

std::string CPathCutter::getType() const
{
	return TYPE;
}

CutStructure* CPathCutter::clone() const
{
	return new CPathCutter(*this);
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
	os.write("mode", mode);
	os.endSection();
}

void CPathCutter::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection(TYPE.c_str(),this);
	CutStructure::serializeFrom(os);
	pWing = static_cast<CWing*>(os.readReference("wing"));
	
	// Now passed in, read this to allow old files to work.
	if (os.ifExists("toolOffset")) {
		double tool_offset;
		os.read("toolOffset", tool_offset);
	}

	if(os.ifExists("mode")) {
		int imode;
		os.read("mode", imode); 
		mode = static_cast<Mode>(imode);
	}
	else {
		mode = Mode::NORMAL;
	}
	os.endReadSection();
	updateBounds();
}

/****************************************************************************************************/
CPathCutter::Intercept::Intercept(NumericT root_u, NumericT tip_u)
	: ru(root_u)
	, tu(tip_u)
	, skip(false)
{
}

/****************************************************************************************************/

CPathCutter::NoOpIntercept::NoOpIntercept(NumericT ru, NumericT tu)
	: Intercept(ru, tu)
{
}

void CPathCutter::NoOpIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context)
{
	// No operation here!
}


/****************************************************************************************************/

CPathCutter::LeadingEdgeIntercept::LeadingEdgeIntercept(NumericT ru, NumericT tu, bool topSurface)
	: Intercept(ru, tu)
	, topSurface(topSurface)
{
	setSkip(true);
}

void CPathCutter::LeadingEdgeIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context)
{
	const CWing* wing = cutter->wing();

	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();

	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();

	NumericT tool_offset = context.toolOffset;

	PointT rtt, rbt, ttt, tbt;  /* root top tangent ... etc */
	//ROTATE r_washout,t_washout;
	NumericT ru, tu;

	/* Get, & Scale the skin thickness to aerofoil space */
	NumericT r_skin, t_skin;
	cutter->calc_skin(r_skin, t_skin, context);

	/* ru1 and tu1 should be given by the intercept, calculate them
	anyway.*/
	/* Root u is determined by the thickness of the LE, Tip u
	// from the root U and the frame.
	*/
	NumericT rx = (wing->getLE() - tool_offset) / wing->getRootTransform()->getChord();
	NumericT ru0 = root->FirstX(rx, 0.25f, 1); /* upper surface */
	NumericT ru1 = root->FirstX(rx, 0.75f, -1);  /* lower surface */
	NumericT tu0 = frame->toTip(ru0);
	NumericT tu1 = frame->toTip(ru1);

	/* Get the 4 PointTs, root top, root bottom, tip top, tip bottom, and their tangents */
	PointT rt = root->Point(ru0, rtt);
	PointT rb = root->Point(ru1, rbt);
	PointT tt = tip->Point(tu0, ttt);
	PointT tb = tip->Point(tu1, tbt);

	/* Move up or down to the skin, assuming right angle triangle close enough approx of how far it needs to go down*/
	/* Note we just want to travel vertically - LE should be a straight cut*/

	// Root first, top then bottom.
	NumericT dx = r_skin;
	NumericT sign = (r_skin > 0) ? 1 : -1;

	NumericT dy = r_skin * rtt.fy / rtt.fx;
	rt.fy -= sign * sqrt(dx * dx + dy * dy); // down

	dy = r_skin * rbt.fy / rbt.fx;
	rb.fy += sign * sqrt(dx * dx + dy * dy);  // up

	// Now tip,  top then bottom
	dx = t_skin;
	sign = (t_skin > 0) ? 1 : -1;
	dy = t_skin * ttt.fy / ttt.fx;
	tt.fy -= sign * sqrt(dx * dx + dy * dy); // down

	dy = t_skin * tbt.fy / tbt.fx;
	tb.fy += sign * sqrt(dx * dx + dy * dy); // up



	/* Now work out where the tangents meet: calculate ru and tu
	// at intersection PointT such that these values of u, placed
	// in the parametric line equations of the lower surface
	// tangents, will give the intersection. Substitute back to
	// into the lower line to give the intersection PointT.
	*/
	PointT ri, ti; // root and tip intersections.
	ru = (rtt.fx * (rt.fy - rb.fy) + rtt.fy * (rt.fx - rb.fx)) /
		(rtt.fx * rbt.fy - rtt.fy * rbt.fx);
	ri.fx = rb.fx + ru * rbt.fx;
	ri.fy = rb.fy + ru * rbt.fy;

	tu = (ttt.fx * (tt.fy - tb.fy) + ttt.fy * (tt.fx - tb.fx)) /
		(ttt.fx * tbt.fy - ttt.fy * tbt.fx);
	ti.fx = tb.fx + tu * tbt.fx;
	ti.fy = tb.fy + tu * tbt.fy;
	
	// Half way points
	PointT rh = PointT((rt.fx + rb.fx) / 2, (rt.fy + rb.fy) / 2);
	PointT th = PointT((tt.fx + tb.fx) / 2, (tt.fy + tb.fy) / 2);

	// Convert from aerofoil coordinate space to proper size, thickness, sweep etc.
	rh = rootTransform->transform(rh);
	th = tipTransform->transform(th);

	rt = rootTransform->transform(rt);
	rb = rootTransform->transform(rb);

	ri = rootTransform->transform(ri);

	tt = tipTransform->transform(tt);
	tb = tipTransform->transform(tb);
	ti = tipTransform->transform(ti);

	// Set the z coordinates appropriately.
	rt.fz = rb.fz = ri.fz = rh.fz = 0;
	tt.fz = tb.fz = ti.fz = th.fz = wing->getSpan();


	// This is one of the few (only?) intercept that cares which direction
	// the cutter is going. Most are symmetric and don't care so...
	CPathCutter::Mode mode = cutter->get_mode();
	bool forward = mode == CPathCutter::NORMAL 
		|| mode == CPathCutter::TOP_FROM_TE 
		|| mode == CPathCutter::BOTTOM_FROM_LE;

	/* OK, so we now want to cut up the front of the wing from
	// bottom to top. forward to the intersection and back to
	// the bottom. Cutter should already be at the bottom.
	*/
	if (topSurface) {
		if (forward) { //top surface, forward direction.
			cutter->line(output, rt, tt); // start
			cutter->line(output, rh, th); // down to half way point
			cutter->line(output, rt, tt); // back up to start
			cutter->line(output, ri, ti); // and out to intersection point
		}
		else { // top surface reverse direction.
			cutter->line(output, ri, ti); // start at intersection point
			cutter->line(output, rt, tt); // up to top
			cutter->line(output, rh, th); // down to half way point
			cutter->line(output, rt, tt); // back up to top
		}
	}
	else {
		if (forward) {  // bottom surface, forward direction
			cutter->line(output, ri, ti); // start at intersection point
			cutter->line(output, rb, tb); // down to bottom
			cutter->line(output, rh, th); // up to half way point
			cutter->line(output, rb, tb); // back down to bottom
		}
		else { // bottom surface, reverse direction
			cutter->line(output, rb, tb); // start on bottom
			cutter->line(output, rh, th); // up to half way point
			cutter->line(output, rb, tb); // back down to bottom
			cutter->line(output, ri, ti); // and out to intersection point
		}
	}
		

	return;
}

/****************************************************************************************************/

CPathCutter::TrailingEdgeIntercept::TrailingEdgeIntercept(NumericT ru, NumericT tu, bool topSurface)
	: Intercept(ru, tu)
	, topSurface(topSurface)
{
}

void CPathCutter::TrailingEdgeIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context)
{
	const CWing* wing = cutter->wing();
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();
	const NumericT tool_offset = context.toolOffset;

	NumericT ru0, ru1;
	NumericT tu0, tu1;
	PointT rt, rb, tt, tb;      /* root top, root bottom, tip top, tip bottom */
	PointT rtt, rbt, ttt, tbt;  /* root top tangent ... etc */
	PointT rm, tm;            /* middle PointTs */
	//  ROTATE r_washout,t_washout;
	NumericT rx;

	/* Get, & Scale the skin thickness to aerofoil space */
	NumericT r_skin, t_skin;
	cutter->calc_skin(r_skin, t_skin, context);

	/* Calculate top and bottom PointTs */
	NumericT root_chord = rootTransform->getChord();
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

	rt.fz = rb.fz = rm.fz = 0;
	tt.fz = tb.fz = tm.fz = wing->getSpan();
	

	/* Now cut... */
	cutter->line(output, rm, tm);    /* cut to midPointT */

	if (topSurface)         /* then top surface */
		cutter->line(output, rt, tt);    /* so move back up */
	else                        /* bottom surface */
		cutter->line(output, rb, tb);    /* so move back down */
	return;
}

/****************************************************************************************************/
CPathCutter::SparIntercept::SparIntercept(NumericT ru, NumericT tu, const CSpar* spar)
	: Intercept(ru, tu)
	, pSpar(spar)
{
}

void CPathCutter::SparIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context)
{
	const CWing* wing = cutter->wing();
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();

	

	int i;
	PointT rpos[5];
	PointT tpos[5];
	PointT rp, tp;
	PointT ros, tos;
	NumericT ru, tu;

	/* Get, & Scale the skin thickness to aerofoil space */
	NumericT r_skin, t_skin;
	cutter->calc_skin(r_skin, t_skin, context);

	/* Work out the spar dimensions */
	NumericT root_height = pSpar->getRootHeight();
	NumericT root_width = pSpar->getRootWidth() - 2 * context.toolOffset;
	NumericT tip_height = pSpar->getTipHeight();
	NumericT tip_width = pSpar->getTipWidth() - 2 * context.toolOffset;
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
		root_height -= (wing->getSkinThickness() - context.toolOffset) / rootTransform->getChord();
		tip_height -= (wing->getSkinThickness() - context.toolOffset) / tipTransform->getChord();
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
		rpos[i].fz = 0;
		tpos[i].fz = wing->getSpan();

		/* Now cut... */
		cutter->line(output, rpos[i], tpos[i]);    /* cut to midPointT */
	}

	return;
}

CPathCutter::CutoutIntercept::CutoutIntercept(NumericT ru, NumericT tu, const Cutout* cutout)
	: Intercept(ru, tu)
	, pCutout(cutout)
{
	assert(this);
	assert(cutout);
}

void CPathCutter::CutoutIntercept::process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context)
{
	assert(this);
	assert(cutter);
	assert(output);
	assert(frame);

	const CWing* wing = cutter->wing();
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();

	NumericT root_chord = rootTransform->getChord();
	NumericT tip_chord = tipTransform->getChord();

	/* Convert cutout sizes & position to aerofoil coords. */
	NumericT r_xpos = pCutout->getRootX() / root_chord;
	NumericT r_ypos = pCutout->getRootY() / root_chord;
	NumericT r_width = pCutout->getRootWidth() / root_chord;
	NumericT r_height = pCutout->getRootHeight() / root_chord;
	NumericT t_xpos = pCutout->getTipX() / tip_chord;
	NumericT t_ypos = pCutout->getTipY() / tip_chord;
	NumericT t_width = pCutout->getTipWidth() / tip_chord;
	NumericT t_height = pCutout->getTipHeight() / tip_chord;

	/* and convert skin thickness and tool offset to aerofoil space */
	NumericT r_skin, t_skin;
	cutter->calc_skin(r_skin, t_skin, context);

	/* Figure out where on the wing (in u) the cutout sits*/
	NumericT rut, rub, tut, tub;
	rut = root->FirstX(r_xpos, 0.0f, 1);
	tut = tip->FirstX(t_xpos, 0.0f, 1);
	rub = root->FirstX(r_xpos, 1.0f, -1);
	tub = tip->FirstX(t_xpos, 1.0f, -1);

	/* Centre points of the cutout in aerofoil space*/
	NumericT rx, ry, tx, ty;

	/* Figure out where the centre points are */
	/* Note this is common to plotting as centre will be the same in both cases. */
	if (pCutout->getIsCentred()) {
		PointT top = root->Point(rut);
		PointT bottom = root->Point(rub);
		rx = (top.fx + bottom.fx) / 2;
		ry = (top.fy + bottom.fy) / 2;

		top = root->Point(tut);
		bottom = root->Point(tub);
		tx = (top.fx + bottom.fx) / 2;
		ty = (top.fy + bottom.fy) / 2;
	}
	else {  // not centred
		NumericT rdist = r_skin + r_ypos + r_height / 2;
		NumericT tdist = t_skin + t_ypos + t_height / 2;

		if (pCutout->getTopSurface()) {
			PointT rtan, ttan;
			PointT rpos = root->Point(rut, rtan);
			PointT tpos = tip->Point(tut, ttan);

			rx = rpos.fx - rdist * rtan.fy;
			ry = rpos.fy + rdist * rtan.fx;
			tx = tpos.fx - tdist * ttan.fy;
			ty = tpos.fy + tdist * ttan.fx;
		}
		else { // calculate from bottom surface
			PointT rtan, ttan;
			PointT rpos = root->Point(rub, rtan);
			PointT tpos = tip->Point(tub, ttan);

			rx = rpos.fx - rdist * rtan.fy;
			ry = rpos.fy + rdist * rtan.fx;
			tx = tpos.fx - tdist * ttan.fy;
			ty = tpos.fy + tdist * ttan.fx;
		}
	}


	/* Ok, so one way or another we've got the centre points of the cutout in rx,ry,tx,ty*/
	/* Plot it.... */

	/* So initially find the start point with the given ru & tu.  This is the same formula irrespective of whether
	top or buottom surface.  Note r_skin & t_skin already have tool offset included*/
	PointT rtan, ttan;
	PointT rpos = root->Point(rootPosition(), rtan);
	PointT tpos = tip->Point(tipPosition(), ttan);
	PointT root_start(rpos.fx - r_skin * rtan.fy, rpos.fy + r_skin * rtan.fx);
	PointT tip_start(tpos.fx - t_skin * ttan.fy, tpos.fy + t_skin * ttan.fx);

	/* So transform back into real space and cut*/
	PointT sr = rootTransform->transform(root_start);
	PointT st = tipTransform->transform(tip_start);
	sr.fz = 0;
	st.fz = wing->getSpan();
	cutter->line(output, sr, st);
	
	/* Find where the line from the start points heading inwards hits the cutout.  Need to go there!*/
	/* Note we've got the centre of the ellipse in aerofoil space and the root and tip start points*/
	/* Find angle going from centre to start point.  Cutting should start at that angle*/
	/* Note also that as it may be an ellipse we need to scale dy & dx by height and width to compensate*/
	NumericT dx, dy;
	dx = (root_start.fx - rx) * pCutout->getRootHeight(); // height is deliberate as swap to scale for ellipse using *
	dy = (root_start.fy - ry) * pCutout->getRootWidth();
	double root_start_angle = atan2(dy, dx);
	dx = (tip_start.fx - tx) * pCutout->getTipHeight();
	dy = (tip_start.fy - ty) * pCutout->getTipWidth();
	double tip_start_angle = atan2(dy, dx);


	// Need to use radius rather than diameter also allowing for tool offset.  
	double rw2 = (pCutout->getRootWidth() / 2 - context.toolOffset) / root_chord;
	double rh2 = (pCutout->getRootHeight() / 2 - context.toolOffset) / root_chord;
	double tw2 = (pCutout->getTipWidth() / 2 - context.toolOffset) / tip_chord;
	double th2 = (pCutout->getTipHeight() / 2 - context.toolOffset) / tip_chord;

	for (double theta = 0; theta <= 2 * PI + DELTA; theta += DELTA) {

		double angle = (theta > 2 * PI) ? 2 * PI : theta;
		PointT r(rx + cos(angle + root_start_angle) * rw2, ry + sin(angle + root_start_angle) * rh2);
		PointT t(tx + cos(angle + tip_start_angle) * tw2, ty + sin(angle + tip_start_angle) * th2);
		r = rootTransform->transform(r);
		t = tipTransform->transform(t);
		r.fz = 0;
		t.fz = wing->getSpan();
		cutter->line(output, r, t);
	}

	// Need to move back to where we started.
	cutter->line(output, sr, st);

}


/****************************************************************************************************/

CPathCutter::Frame::Frame(NumericT ru0, NumericT tu0, NumericT ruf, NumericT tuf, NumericT ru1, NumericT tu1)
	: ru0(ru0)
	, tu0(tu0)
	, ruf(ruf)
	, tuf(tuf)
	, ru1(ru1)
	, tu1(tu1)
{
}

// For a given root U gets the corresponding tip U
NumericT CPathCutter::Frame::toTip(NumericT ru) const
{
	NumericT tu = tipForward() + (ru - rootForward()) * (tipFinish() - tipForward()) /
		(rootFinish() - rootForward());
	return tu;
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

std::list<CPathCutter::Intercept*>::const_reverse_iterator CPathCutter::Intercepts::rbegin()
{
	if (!isSorted) {
		intercepts.sort(compare_intercepts);
		isSorted = true;
	}
	return intercepts.rbegin();

}

std::list<CPathCutter::Intercept*>::const_reverse_iterator CPathCutter::Intercepts::rend()
{
	return intercepts.rend();
}

