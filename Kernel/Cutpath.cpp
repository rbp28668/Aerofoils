/*
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

static CObjectFactory<CPathCutter> factory("pathCutter");

/************************************************************/
/** CPathCutter::CPathCutter                               **/
/************************************************************/
CPathCutter::CPathCutter(CWing* pWing)
: CPlotCommonImpl(pWing)
{
	assert(pWing);
	
	tool_offset = 0.0f;
	blToolOffsetSet = false;
	icount = 0;
}

CPathCutter::CPathCutter()
: CPlotCommonImpl()
{
	tool_offset = 0.0f;
	blToolOffsetSet = false;
	icount = 0;
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
void CPathCutter::plot_segment(const CWing& wing, INTERCEPT* start, INTERCEPT* finish, float delta)
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
	ru0 = start->ru;
	ru1 = finish->ru;
	tu0 = start->tu;
	tu1 = finish->tu;
	
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
	
	interp_move_to(r_here,t_here);
	
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
		
		interp_line_to(r_here,t_here);
    }
	
	r_here = root->Point(ru1,tangent);
	r_here.fx -= r_skin*tangent.fy;
	r_here.fy += r_skin*tangent.fx;
	
	t_here = tip->Point(tu1,tangent);
	t_here.fx -= t_skin*tangent.fy;
	t_here.fy += t_skin*tangent.fx;
	
	r_here = wing.getRootTransform()->transform(r_here);
	t_here = wing.getTipTransform()->transform(t_here);
	
	interp_line_to(r_here,t_here);
	
	return;
}


/************************************************************/
/** SORT_INTERCEPTS sorts the intercept list by root u.    **/
/** The ilist array has icount entries in it.              **/
/************************************************************/
void CPathCutter::sort_intercepts(void)
{
	int i,j;
	float umax;
	int nMaxPos;
	INTERCEPT temp;
	
	for(i=icount;i>0;--i)
    {
		
		/* Array has i unsorted entries in it. */
		
		/* Find the biggest */
		umax = ilist[0].ru;
		nMaxPos = 0;
		for(j=1;j<i;++j)
		{
			if(ilist[j].ru > umax)
			{
				umax= ilist[j].ru;
				nMaxPos = j;
			}
		}
		/* now swap the biggest with the top of the unsorted array */
		temp = ilist[i-1];
		ilist[i-1] = ilist[nMaxPos];
		ilist[nMaxPos] = temp;
    }
	
	return;
}



/************************************************************/
/** CUT_LE cuts the leading edge at the LE intercept       **/
/************************************************************/

void CPathCutter::cut_le(INTERCEPT* /*icept*/,FRAME* frame)
{
	const CWing* wing = frame->wing;
	
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();
	
	float rx;
	float ru0,ru1;
	float tu0,tu1;
	PointT rt,rb,tt,tb;      /* root top, root bottom, tip top, tip bottom */
	PointT rtt,rbt,ttt,tbt;  /* root top tangent ... etc */
	//ROTATE r_washout,t_washout;
	float r_skin,t_skin;
	float ru,tu;
	PointT ri,ti;
	
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
	rx= (wing->getLE() - tool_offset) / wing->getRootTransform()->getChord();
	ru0 = root->FirstX(rx,0.25f,1); /* upper surface */
	ru1 = root->FirstX(rx,0.75f,-1);  /* lower surface */
	
	
	tu0 = frame->tu0 + (ru0-frame->ru0) * (frame->tuf - frame->tu0)/
		(frame->ruf - frame->ru0);
	
	
	tu1 = frame->tuf + (ru1-frame->ruf) * (frame->tu1 - frame->tuf)/
		(frame->ru1 - frame->ruf);
	
	/* Get the 4 PointTs and their tangents */
	rt = root->Point(ru0,rtt);
	rb = root->Point(ru1,rbt);
	
	tt = tip->Point(tu0, ttt);
	tb = tip->Point(tu1, tbt);
	
	/* Move in (or out) to the skin */
	rt.fx -= r_skin*rtt.fy;
	rt.fy += r_skin*rtt.fx;
	
	rb.fx -= r_skin*rbt.fy;
	rb.fy += r_skin*rbt.fx;
	
	tt.fx -= t_skin*ttt.fy;
	tt.fy += t_skin*ttt.fx;
	
	tb.fx -= t_skin*tbt.fy;
	tb.fy += t_skin*tbt.fx;
	
	/* Now work out where the tangents meet: calculate ru and tu
	// at intersection PointT such that these values of u, placed
	// in the parametric line equations of the lower surface
	// tangents, will give the intersection. Substitute back to
	// into the lower line to give the intersection PointT.
	*/
	ru = (rtt.fx * (rt.fy - rb.fy) + rtt.fy * (rt.fx - rb.fx))/
		(rtt.fx * rbt.fy - rtt.fy * rbt.fx);
	ri.fx = rb.fx + ru * rbt.fx;
	ri.fy = rb.fy + ru * rbt.fy;
	
	tu = (ttt.fx * (tt.fy - tb.fy) + ttt.fy * (tt.fx - tb.fx))/
		(ttt.fx * tbt.fy - ttt.fy * tbt.fx );
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
	interp_line_to(rt,tt);
	interp_line_to(ri,ti);
	interp_line_to(rb,tb);
	
	return;
}


/************************************************************/
/** CUT_TE cuts trailing edges at the TE intercepts        **/
/************************************************************/
void CPathCutter::cut_te(INTERCEPT* icept,FRAME* frame)
{
	const CWing* wing = frame->wing;
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();
	
	float ru0,ru1;
	float tu0,tu1;
	PointT rt,rb,tt,tb;      /* root top, root bottom, tip top, tip bottom */
	PointT rtt,rbt,ttt,tbt;  /* root top tangent ... etc */
	PointT rm,tm;            /* middle PointTs */
	//  ROTATE r_washout,t_washout;
	float r_skin,t_skin;
	float rx;
	
	/* Get, & Scale the skin thickness to aerofoil space */
	r_skin = wing->getSkinThickness() - tool_offset;
	r_skin /= rootTransform->getChord();
	t_skin = wing->getSkinThickness() - tool_offset;
	t_skin /= tipTransform->getChord();
	
	/* Calculate top and bottom PointTs */  
	float root_chord = rootTransform->getChord();
	rx=(root_chord - wing->getTE() + tool_offset) / root_chord;
	
	ru0 = root->FirstX(rx,frame->ruf,-1); /* upper surface */
	ru1 = root->FirstX(rx,frame->ruf,1);  /* lower surface */
	
	
	/* Get tu in segment ru0..ruf. */
	tu0 = frame->tu0 + (ru0 - frame->ru0) * (frame->tuf - frame->tu0)/
		(frame->ruf - frame->ru0);
	
	
	/* Get tu in segment ruf..ru1. */
	tu1 = frame->tuf + (ru1-frame->ruf) * (frame->tu1 - frame->tuf)/
		(frame->ru1 - frame->ruf);
	
	/* Get the 4 PointTs and their tangents */
	rt = root->Point(ru0,rtt);
	rb = root->Point(ru1,rbt);
	
	tt = tip->Point(tu0,ttt);
	tb = tip->Point(tu1,tbt);
	
	/* Move in (or out) to the skin */
	rt.fx -= r_skin*rtt.fy;
	rt.fy += r_skin*rtt.fx;
	
	rb.fx -= r_skin*rbt.fy;
	rb.fy += r_skin*rbt.fx;
	
	tt.fx -= t_skin*ttt.fy;
	tt.fy += t_skin*ttt.fx;
	
	tb.fx -= t_skin*tbt.fy;
	tb.fy += t_skin*tbt.fx;
	
	/* Find the midPointT */
	rm.fx = (rt.fx + rb.fx)/2;
	rm.fy = (rt.fy + rb.fy)/2;
	
	tm.fx = (tt.fx + tb.fx)/2;
	tm.fy = (tt.fy + tb.fy)/2;
	
	rt = rootTransform->transform(rt);
	rb = rootTransform->transform(rb);
	rm = rootTransform->transform(rm);
	
	tt = tipTransform->transform(tt);
	tb = tipTransform->transform(tb);
	tm = tipTransform->transform(tm);
	
	/* Now cut... */
	interp_line_to(rm,tm);    /* cut to midPointT */
	
	if(icept->idx == 0)         /* then top surface */
		interp_line_to(rt,tt);    /* so move back up */  
	else                        /* bottom surface */
		interp_line_to(rb,tb);    /* so move back down */
	return;
}

/************************************************************/
/** CUT_SPAR cuts a spar (or half a spar for full-depth)   **/
/** The intercept index has the index of the spar.         **/
/************************************************************/
void CPathCutter::cut_spar(INTERCEPT* icept,FRAME* frame)
{
	const CWing* wing = frame->wing;
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();
	
	const CSpar* spar = wing->getSpar(icept->idx);
	
	PointT rs,ts;                  /* start positions: at intercept */
	PointT rst,tst;                /* and their tangents */
	PointT tangent;                /* determines orientation of spar */
	int i;
	PointT rpos[5];
	PointT tpos[5];
	PointT rp,tp;
	PointT ros,tos;
	float ru,tu;
	
	/* Get, & Scale the skin thickness to aerofoil space */
	float r_skin=wing->getSkinThickness() - tool_offset;
	r_skin /= rootTransform->getChord();
	float t_skin=wing->getSkinThickness() - tool_offset;
	t_skin /= tipTransform->getChord();
	
	
	/* Work out the spar dimensions */
	float root_height = spar->getRootHeight();
	float root_width = spar->getRootWidth() - 2*tool_offset;
	float tip_height = spar->getTipHeight();
	float tip_width = spar->getTipWidth() - 2*tool_offset;
	if(!spar->isSubmerged())
    {
		root_height -= wing->getSkinThickness();
		tip_height -= wing->getSkinThickness();
    }
	root_height /= rootTransform->getChord();
	root_width /= rootTransform->getChord();
	tip_height /= tipTransform->getChord();
	tip_width /= tipTransform->getChord();
	
	
	/* Get the start PointTs on the root and tip */
	rs = root->Point(icept->ru,rst);
	ts = tip->Point(icept->tu,tst);	
	
	tangent = rst;  /* root determines spar orientation */
	
	
	/* if spar is full depth then the tangent and the depth */
	/* need to be re-calculated. Tangent is horizontal (as the */
	/* spar should run up and down */
	if(spar->isFullDepth())
    {
		if(icept->ru < frame->ruf)  /* then top intercept */
		{
			/* So set up tangent and other PointT on lower surface */
			tangent.fx = -1;
			tangent.fy = 0;
			ru = root->FirstX(rs.fx,frame->ruf,1);
			tu = tip->FirstX(ts.fx,frame->tuf,1);
		}
		else                        /* bottom intercept */
		{
			tangent.fx = 1;
			tangent.fy = 0;
			ru = root->FirstX(rs.fx,frame->ruf,-1);
			tu = tip->FirstX(ts.fx,frame->tuf,-1);
		}
		
		/* Get PointT on the other side from the start PointT.*/
		ros = root->Point(ru);
		tos = tip->Point(tu);
		
		/* Work out 1/2 the wing thickness */
		if(rs.fy > ros.fy)
			root_height = rs.fy - ros.fy;
		else
			root_height = ros.fy - rs.fy;
		
		if(ts.fy > tos.fy)
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
	rs.fx -= r_skin*rst.fy;
	rs.fy += r_skin*rst.fx;
	
	ts.fx -= t_skin*tst.fy;
	ts.fy += t_skin*tst.fx;
	
	/* Now run round setting up the PointT arrays. */
	
	rp = rs;
	tp = ts;
	
	rp.fx -= tangent.fx * root_width/2;
	rp.fy -= tangent.fy * root_width/2;
	tp.fx -= tangent.fx * tip_width/2;
	tp.fy -= tangent.fy * tip_width/2;
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
	
	for(i=0; i<5; ++i)
    {
		rpos[i] = rootTransform->transform(rpos[i]);
		tpos[i] = tipTransform->transform(tpos[i]);
		
		/* Now cut... */
		interp_line_to(rpos[i],tpos[i]);    /* cut to midPointT */
    }
	
	return;
}

/************************************************************/
/** SET_LE_INTERCEPT sets up the intercept PointT for       **/
/** the leading edge.                                      **/
/************************************************************/
void CPathCutter::set_le_intercept(FRAME* frame)
{
	assert(this);
	assert(frame);
	
	const CWing* wing = frame->wing;
	const CAerofoil *root = wing->getRoot();
	
	/* Do LE first: LE intercept is on the lower surface and entirely
	// determined by the root.
	*/
	if((wing->getLE() - tool_offset) > 0.0f)
    {
		float rx=(wing->getLE() - tool_offset) / wing->getRootTransform()->getChord();
		float ru1 = root->FirstX(rx,0.75f,-1);  /* lower surface */
		
		/* Get tu in segment ruf..ru1. */
		float tu1 = frame->tuf + (ru1-frame->ruf) * (frame->tu1 - frame->tuf)/
			(frame->ru1 - frame->ruf);
		
		ilist[icount].idx = 0;
		ilist[icount].ru = ru1;
		ilist[icount].tu = tu1;
		ilist[icount].fn = &CPathCutter::cut_le;
		ilist[icount].data = frame;
		++icount;
    }
	return;
}

/************************************************************/
/** SET_TE_INTERCEPT sets up the intercept PointTs for      **/
/** the trailing edge.                                     **/
/************************************************************/
void CPathCutter::set_te_intercept(FRAME* frame)
{
	assert(this);
	assert(frame);
	
	float rx;
	float tu0,tu1;
	float ru0,ru1;
	
	const CWing* wing = frame->wing;
	const CAerofoil *root = wing->getRoot();
	
	/* TE has an upper and a lower intercept */
	if((wing->getTE() - tool_offset) > 0.0f)
    {
		float root_chord = wing->getRootTransform()->getChord();
		rx=(root_chord - wing->getTE() + tool_offset) / root_chord;
		
		ru0 = root->FirstX(rx,frame->ruf,-1); /* upper surface */
		ru1 = root->FirstX(rx,frame->ruf,1);  /* lower surface */
		
		
		/* Get tu in segment ru0..ruf. */
		tu0 = frame->tu0 + (ru0-frame->ru0) * (frame->tuf - frame->tu0)/
			(frame->ruf - frame->ru0);
		
		/* Get tu in segment ruf..ru1. */
		tu1 = frame->tuf + (ru1-frame->ruf) * (frame->tu1 - frame->tuf)/
			(frame->ru1 - frame->ruf);
		
		ilist[icount].idx = 0;  /* mark as top surface*/
		ilist[icount].ru = ru0;
		ilist[icount].tu = tu0;
		ilist[icount].fn = &CPathCutter::cut_te;
		ilist[icount].data = frame;
		++icount;
		
		
		ilist[icount].idx = 1;  /* and bottom surface */
		ilist[icount].ru = ru1;
		ilist[icount].tu = tu1;
		ilist[icount].fn = &CPathCutter::cut_te;
		ilist[icount].data = frame;
		++icount;
		
    }
	
	return;
}


/************************************************************/
/** SET_SPARS_INTERCEPT sets up any intercept PointTs for   **/
/** the spars.                                             **/
/************************************************************/
void CPathCutter::set_spars_intercept(FRAME* frame)
{
	assert(this);
	assert(frame);
	
	float rx;
	float tu0,tu1;
	float ru0,ru1;
	
	const CWing* wing = frame->wing;
	const CAerofoil *root = wing->getRoot();
	const CTransform* rootTransform = wing->getRootTransform();
	
	for(int i=0; i<wing->getSparCount(); ++i)
	{
		const CSpar* spar = wing->getSpar(i);
		
		/* intercept at midPointT of spar */
		rx = (spar->getRootX() + spar->getRootWidth()/2) / rootTransform->getChord();
		
		switch (spar->getType())
		{
		case CSpar::top:
			ru0 = root->FirstX(rx,frame->ruf,-1); 
			tu0 = frame->tu0 + (ru0-frame->ru0) * (frame->tuf - frame->tu0)/
				(frame->ruf - frame->ru0);
			ilist[icount].idx = i;
			ilist[icount].ru = ru0;
			ilist[icount].tu = tu0;
			ilist[icount].fn = &CPathCutter::cut_spar;
			ilist[icount].data = frame;
			++icount;
			break;
			
		case CSpar::bottom:
			ru1 = root->FirstX(rx,frame->ruf,1); 
			tu1 = frame->tuf + (ru1-frame->ruf) * (frame->tu1 - frame->tuf)/
				(frame->ru1 - frame->ruf);
			ilist[icount].idx = i;
			ilist[icount].ru = ru1;
			ilist[icount].tu = tu1;
			ilist[icount].fn = &CPathCutter::cut_spar;
			ilist[icount].data = frame;
			++icount;
			break;
			
		case CSpar::full_depth:
			ru0 = root->FirstX(rx,frame->ruf,-1); 
			tu0 = frame->tu0 + (ru0-frame->ru0) * (frame->tuf - frame->tu0)/
				(frame->ruf - frame->ru0);
			ru1 = root->FirstX(rx,frame->ruf,1); 
			tu1 = frame->tuf + (ru1-frame->ruf) * (frame->tu1 - frame->tuf)/
				(frame->ru1 - frame->ruf);
			ilist[icount].idx = i;
			ilist[icount].ru = ru0;
			ilist[icount].tu = tu0;
			ilist[icount].fn = &CPathCutter::cut_spar;
			ilist[icount].data = frame;
			++icount;
			
			ilist[icount].idx = i;
			ilist[icount].ru = ru1;
			ilist[icount].tu = tu1;
			ilist[icount].fn = &CPathCutter::cut_spar;
			ilist[icount].data = frame;
			++icount;
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
void CPathCutter::plot(COutputDevice* pdev)
{
	assert(this);
	assert(pdev);
	
	plot_flags = *(wing->getPlotFlags());
	setDevice(pdev);
	setInterpolate(plot_flags.plot_section);

	const CAerofoil *root = wing->getRoot();
	const CAerofoil *tip = wing->getTip();
	
	float ru0,ru1;
	float tu0,tu1;
	float delta;
	float r_skin,t_skin;
	float ruf,tuf;
	FRAME frame;
	int i;
	
	float root_chord = wing->getRootTransform()->getChord();
	float tip_chord = wing->getTipTransform()->getChord();
	
	/* Work out delta (in U) to give a line segment length of about 0.2 mm */
	delta = max(root_chord, tip_chord);
	delta = 1.0f / (5 * delta);
	
	
	/* Get, & Scale the skin thickness to aerofoil space */
	r_skin=wing->getSkinThickness() - tool_offset;
	r_skin /= root_chord;
	t_skin=wing->getSkinThickness() - tool_offset;
	t_skin /= tip_chord;
	
	icount = 0;
	
	/* Find position of the trailing edge */
	find_core_te(*root,r_skin,&ru0,&ru1);
	find_core_te(*tip,t_skin,&tu0,&tu1);
	
	// Get initial plot points - use to position cutter
	PointT tangent;
	PointT rte = root->Point(ru0,tangent);
	rte.fx -= r_skin*tangent.fy;
	rte.fy += r_skin*tangent.fx;
	rte = wing->getRootTransform()->transform(rte);

	PointT tte = tip->Point(tu0,tangent);
	tte.fx -= t_skin*tangent.fy;
	tte.fy += t_skin*tangent.fx;
	tte = wing->getTipTransform()->transform(tte);
	
	/* store these values in INTERCEPT list */
	ilist[icount].idx = 0;
	ilist[icount].ru = ru0;
	ilist[icount].tu = tu0;
	ilist[icount].fn = NULL;
	ilist[icount].data = NULL;
	++icount;
	
	ilist[icount].idx = 0;
	ilist[icount].ru = ru1;
	ilist[icount].tu = tu1;
	ilist[icount].fn = NULL;
	ilist[icount].data = NULL;
	++icount;
	
	/* Find the most forward PointTs of each section: add this as
	// an INTERCEPT
	*/
	find_forward_PointT(*root,&ruf);
	find_forward_PointT(*tip,&tuf);
	ilist[icount].idx = 0;
	ilist[icount].ru = ruf;
	ilist[icount].tu = tuf;
	ilist[icount].fn = NULL;
	ilist[icount].data = NULL;
	++icount;
	
	
	/* Set up coordinate frame for le/te & spars */
	frame.ru0 = ru0;
	frame.tu0 = tu0;
	frame.ruf = ruf;
	frame.tuf = tuf;
	frame.ru1 = ru1;
	frame.tu1 = tu1;
	frame.wing = wing;
	
	if(plot_flags.plot_le)
    {
		set_le_intercept(&frame);
    }
	
	if(plot_flags.plot_te)
    {
		set_te_intercept(&frame);
    }
	
	if(plot_flags.plot_spars)
    {
		set_spars_intercept(&frame);
    }
	
	sort_intercepts();
	
	// Run the plot....
	interp_move_to(rte,tte);

	for(i=0; i<icount-1; ++i)
    {
		if(ilist[i].fn != NULL)
			(this->*(ilist[i].fn))(ilist+i,&frame);
		
		plot_segment(*wing,ilist+i,ilist+i+1,delta);
    }
	
	if(ilist[i].fn != NULL)
		(this->*(ilist[i].fn))(ilist+i,&frame);

	interp_move_to(rte,tte);
	
	return;
}

string CPathCutter::getDescriptiveText() const
{
	stringstream ss;

	ss << "Wing Core at " << getSectionPos() << ends;
	return ss.str();
}

void CPathCutter::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("pathCutter",this);
	CPlotStructure::serializeTo(os);
	os.writeReference("wing",wing);
	os.write("toolOffset",tool_offset);
	os.endSection();
}

void CPathCutter::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection("pathCutter",this);
	CPlotStructure::serializeFrom(os);
	wing = static_cast<CWing*>(os.readReference("wing"));
	os.read("toolOffset",tool_offset);
	os.endReadSection();
}
