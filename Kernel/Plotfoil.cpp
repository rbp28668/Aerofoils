/*
File:              PLOTFOIL.C

Program/rev.:      Aerofoil 0.0

Function:          Main Aerofoil Plotting Routines

Author:            Bruce

Date:              circa Sept 91

Routines:          draw_sections(AEROFOIL *root,AEROFOIL *tip)

Input Parameters:  root and tip sections

Output Parameters:

Error conditions:

Description:        draw_sections sets up spline curves for both
the root and the tip and then plots the basic
outline. Depending on settings of plot_flags
bits of structure and markers may then be drawn.

Revsision History:

*/



#include <assert.h>
#include <sstream>
#include <limits>

#include "OutputDevice.h"
#include "KernelError.h"
#include "plotfoil.h"
#include "PointT.h"
#include "Wing.h"
#include "Spar.h"
#include "ObjectSerializer.h"

using namespace std;

#define LABEL_HEIGHT 7.0f  /* Y space for a label under an aerofoil */

#ifdef max 
#undef max
#endif

static CObjectFactory<CPathPlotter> factory("pathPlotter");

/************************************************************/
/************************************************************/
CPathPlotter::CPathPlotter(CWing* pw)
: CPlotCommonImpl(pw)
{
	assert(this);
	assert(pw);
}

CPathPlotter::CPathPlotter()
: CPlotCommonImpl()
{
	assert(this);
}


/************************************************************/
/** PLOT_SKIN plots an aerofoil allowing for the current  **/
/** skin thickness. Note that this routine has to work    **/
/** slightly differently if plot_flags.plot_section is set.**/
/** If plot_section is set then we want an intermediate   **/
/** section at some partial span. The skin then needs to  **/
/** be plotted using the same value of u for root and tip   **/
/** and the start and finish values of u need to be inter-  **/
/** polated from the core start and finish values for the  **/
/** root and the tip. If plot_section is not set then we  **/
/** just want to plot the root and the tip sections     **/
/** seperately. In this case root and tip u are different  **/
/** as the plot moves round the aerofoil and the tip value  **/
/** of u is calculated from the root value of u.      **/
/************************************************************/
void CPathPlotter::plot_skin(const CWing& wing) const
{
	PointT tangent;
	float r_skin,t_skin;
	float ru0,ru1;
	float tu0,tu1;
	PointT r_here,t_here;
	float delta;
	float ru,tu;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	
	/* Get, & Scale the skin thickness to aerofoil space */
	r_skin = wing.getSkinThickness();
	r_skin /= rootTransform->getChord();
	t_skin = wing.getSkinThickness();
	t_skin /= tipTransform->getChord();
	
	
	/* And position of trailing edges. */
	find_core_te(*root,r_skin,&ru0,&ru1);
	find_core_te(*tip,t_skin,&tu0,&tu1);
	
	/* If we are to interpolate wing then estimate interpolated */
	/* section u0 and u1 (start & finish of skin) by linear    */
	/* interpolation.                      */
	
	float span = wing.getSpan();
	if(plot_flags.plot_section)
    {
		ru0 = tu0= (ru0 * (span - getSectionPos()) + tu0 * getSectionPos()) / span;
		ru1 = tu1= (ru1 * (span - getSectionPos()) + tu1 * getSectionPos()) / span;
    }
	
	/* Get starting point for root */
	r_here = root->Point(ru0,tangent);
	r_here.fx -= r_skin*tangent.fy;
	r_here.fy += r_skin*tangent.fx;
	
	/* and starting point for tip */
	t_here = tip->Point(tu0,tangent);
	t_here.fx -= t_skin*tangent.fy;
	t_here.fy += t_skin*tangent.fx;
	
	r_here = rootTransform->transform(r_here);
	t_here = tipTransform->transform(t_here);
	
	interp_move_to(r_here,t_here);
	
	delta=0.001f;
	
	for(ru=ru0+delta;ru<=ru1;ru+=delta)
    {
		/* Get root coordinate */
		r_here = root->Point(ru,tangent);
		r_here.fx -= r_skin*tangent.fy;
		r_here.fy += r_skin*tangent.fx;
		
		/* and tip coordinate */
		if(plot_flags.plot_section)
			tu=ru;
		else
			tu=tu0+(ru-ru0)*(tu1-tu0)/(ru1-ru0);
		
		t_here = tip->Point(tu,tangent);
		t_here.fx -= t_skin*tangent.fy;
		t_here.fy += t_skin*tangent.fx;
		
		r_here = rootTransform->transform(r_here);
		t_here = tipTransform->transform(t_here);
		
		interp_line_to(r_here,t_here);
    }
	
	r_here = root->Point(ru1,tangent);
	r_here.fx -= r_skin*tangent.fy;
	r_here.fy += r_skin*tangent.fx;
	
	t_here = tip->Point(tu1,tangent);
	t_here.fx -= t_skin*tangent.fy;
	t_here.fy += t_skin*tangent.fx;
	
	r_here = rootTransform->transform(r_here);
	t_here = tipTransform->transform(t_here);
	
	interp_line_to(r_here,t_here);
	return;
}

/************************************************************/
/** PLOT_CHORD plots a chord line for each section.    **/
/************************************************************/
void CPathPlotter::plot_chord(const CWing& wing) const
{
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	
	PointT r_here = rootTransform->transform( PointT(0.0f, 0.0f));
	PointT t_here = tipTransform->transform( PointT(0.0f, 0.0f));
	
	interp_move_to(r_here,t_here);
	
	r_here = rootTransform->transform( PointT(1.0f, 0.0f));
	t_here = tipTransform->transform( PointT(1.0f, 0.0f));
	
	interp_line_to(r_here,t_here);
	
	return;
}

/************************************************************/
/** PLOT_MARKERS plots vertical markers every 10 %      **/
/************************************************************/
void CPathPlotter::plot_markers(const CWing& wing) const
{
	int i;
	float x;
	float urt,urb;    /* u for root top and bottom */
	float utt,utb;    /* u for tip top and bottom */
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	for(i=1;i<10;++i)        /* do markers 10%  to 90% inc */
    {
		x=i/10.0f;          /* convert to fraction */
		
		/* find parametric positions for the given x, top & bottom, root & tip */
		urt = root->FirstX(x,WEENY,1); 
		urb = root->FirstX(x,(float)(1.0f-WEENY),-1);  /* lower surface u */
		utt = tip->FirstX(x,WEENY,1); 
		utb = tip->FirstX(x,(float)(1.0f-WEENY),-1);  /* lower surface u */
		
		PointT pt_rt = root->Point(urt);
		PointT pt_rb = root->Point(urb);
		PointT pt_tt = tip->Point(utt);
		PointT pt_tb = tip->Point(utb);
		
		pt_rt = rootTransform->transform(pt_rt);
		pt_rb = rootTransform->transform(pt_rb);
		pt_tt = tipTransform->transform(pt_tt);
		pt_tb = tipTransform->transform(pt_tb);
		
		interp_move_to(pt_rb,pt_tb);
		interp_line_to(pt_rt,pt_tt);
    }
	return;
}


/************************************************************/
/** PLOT_LE plots the leading edge.                        **/
/************************************************************/
void CPathPlotter::plot_le(const CWing& wing) const
{
	float u0,u1;
	PointT rt,rb,tt,tb;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	
	/* Do LE first */
	float rx = wing.getLE() / rootTransform->getChord();
	float tx = wing.getLE() / tipTransform->getChord();
	
	if( rx > 0.0f && tx > 0.0f)
	{
		
		u0 = root->FirstX(rx,0.25f,1);  /* upper surface */
		u1 = root->FirstX(rx,0.75f,-1);  /* lower surface */
		rt = root->Point(u0);
		rb = root->Point(u1);
		
		u0 = tip->FirstX(tx,0.25f,1);  /* upper surface */
		u1 = tip->FirstX(tx,0.75f,-1);  /* lower surface */
		tt = tip->Point(u0);
		tb = tip->Point(u1);
		
		rt = rootTransform->transform(rt);
		rb = rootTransform->transform(rb);
		tt = tipTransform->transform(tt);
		tb = tipTransform->transform(tb);
		
		interp_move_to(rt,tt);
		interp_line_to(rb,tb);
	}
	
	return;
}

/************************************************************/
/** PLOT_TE plots the trailing edge.                       **/
/************************************************************/
void CPathPlotter::plot_te(const CWing& wing) const
{
	float u0,u1;
	PointT rt,rb,tt,tb;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	if(wing.getTE() > 0.0f)
	{
		/* Now do the TE */
		float rx = (rootTransform->getChord() - wing.getTE()) / rootTransform->getChord();
		u0 = root->FirstX(rx,0.25f,-1);  /* upper surface */
		u1 = root->FirstX(rx,0.75f,1);  /* lower surface */
		rt = root->Point(u0);
		rb = root->Point(u1);
		
		float tx = (tipTransform->getChord() - wing.getTE()) / tipTransform->getChord();
		u0 = tip->FirstX(tx,0.25f,-1);  /* upper surface */
		u1 = tip->FirstX(tx,0.75f,1);    /* lower surface */
		tt = tip->Point(u0);
		tb = tip->Point(u1);
		
		rt = rootTransform->transform(rt);
		rb = rootTransform->transform(rb);
		tt = tipTransform->transform(tt);
		tb = tipTransform->transform(tb);
		
		interp_move_to(rt,tt);
		interp_line_to(rb,tb);
	}
	return;
}

/************************************************************/
/** PLOT_LABELS labels the aerofoils that are drawn.    **/
/************************************************************/
void CPathPlotter::plot_labels(const CWing& wing) const
{
	float u,delta;
	float root_lowest,tip_lowest;
	PointT r_here,t_here;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	/* Look for the lowest point on each aerofoil or on the section */
	root_lowest = tip_lowest = numeric_limits<float>::max();
	delta=0.05f;          /* Coarse search */
	for(u=0.0f;u<=1.0f;u+=delta)
    {
		r_here = root->Point(u);
		r_here = rootTransform->transform(r_here);
		
		t_here = tip->Point(u);
		t_here = tipTransform->transform(t_here);
		
		if(plot_flags.plot_section)
		{
			r_here = interpolate(r_here, t_here);
		}
		if(r_here.fy < root_lowest)
			root_lowest=r_here.fy;
		if(t_here.fy < tip_lowest)
			tip_lowest=t_here.fy;
    }
	
	
	/** OK, so now we have the lowest point on the root section in   **/
	/** root_lowest and the lowest point on the tip section in     **/
	/** tip_lowest. (If interpolating then only use root_lowest as  **/
	/** it stores the interpolated value.) Note also that       **/
	/** root_lowest and tip_lowest are in output (mm) space.    **/
	
	/** and convert to leftmost position of the sections **/
	r_here = rootTransform->transform( PointT(0.0f, 0.0f));
	t_here = tipTransform->transform( PointT(0.0f, 0.0f));
	
	r_here.fy = root_lowest-LABEL_HEIGHT;
	t_here.fy = tip_lowest-LABEL_HEIGHT;
	
	string root_name = root->getName();
	string tip_name = tip->getName();
	
	ostringstream text;
	
	if(plot_flags.plot_section)
    {
		r_here = interpolate(r_here,t_here);
		
		text << root_name 
			<< " to " << tip_name 
			<< " at " << getSectionPos()/wing.getSpan() * 100.0 << "%" << ends;
		
		LabelAt(r_here, text.str(), true);
    }
	else
    {
		text << root_name << " Chord: " << rootTransform->getChord() << ends; 
		//  sprintf(text,"%s Chord: %2.1f%s",root_name,
		//      root->chord/get_unit_scale(),get_units_text());
		LabelAt(r_here, text.str(), true);
		
		text.seekp(0);
		text << tip_name << " Chord: " << tipTransform->getChord() << ends; 
		//    sprintf(text,"%s Chord: %2.1f%s",tip_name,
		//      tip->chord/get_unit_scale(),get_units_text());
		
		LabelAt(t_here, text.str(), false);
		
    }
	return;
}


/************************************************************/
/** PLOT_SPAR draws a spar on the aerofoil. Note that this  **/
/** routine does not deal with full-depth spars ! This   **/
/** routine has a first order correction to allow for the  **/
/** local gradient of the surface/skin thickness at the   **/
/** spar but ignores second order corrections.        **/
/************************************************************/
void CPathPlotter::plot_spar(const CWing& wing, const CSpar& spar) const
{
	float ru,tu;
	PointT here;
	PointT r_tangent,t_tangent;
	PointT r_here,t_here;
	PointT rt,tt;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	float root_chord = rootTransform->getChord();
	float tip_chord = tipTransform->getChord();
	
	/* Convert spar sizes & position to aerofoil coords. */
	float r_xpos = spar.getRootX() / root_chord;
	float r_width = spar.getRootWidth() / root_chord;
	float r_height = spar.getRootHeight() / root_chord;
	float t_xpos = spar.getTipX() / tip_chord;
	float t_width = spar.getTipWidth() / tip_chord;
	float t_height = spar.getTipHeight() / tip_chord;
	
	/* and convert skin thickness to aerofoil space */
	float r_skin = wing.getSkinThickness() / root_chord;
	float t_skin = wing.getSkinThickness() / tip_chord;
	
	if(!spar.isSubmerged())     /* ignore skin if spar not under it */
		r_skin = t_skin=0.0f;
	
	// centre x coordinates
	float crx=r_xpos+r_width/2.0f;
	float ctx=t_xpos+t_width/2.0f;
	
	if(wing.getSkinThickness() != 0.0f)
    {
		if (spar.getType() == CSpar::top)
		{
			ru = root->FirstX(crx,0.0f,1);
			tu = tip->FirstX(ctx,0.0f,1);
		}
		else
		{
			ru = root->FirstX(crx,1.0f,-1);
			tu = tip->FirstX(ctx,1.0f,-1);
		}
		
		
		/* go out by the skin thickness to give an updated x */
		here = root->Point(ru,r_tangent);
		here.fx += r_skin*r_tangent.fy;
		crx=here.fx;   /* this is the new x to allow for skin thickess */
		
		
		/* get updated x for centre of the spar */
		here = tip->Point(tu, t_tangent);
		here.fx += t_skin*t_tangent.fy;
		ctx=here.fx;
    }
	
	/* Now go through most of that again to get the top central point */
	/* and the tangent for the spar */
	if (spar.getType() == CSpar::top)
    {
		ru = root->FirstX(crx,0.0f,1);
		tu = tip->FirstX(ctx,0.0f,1);
    }
	else
    {
		ru = root->FirstX(crx,1.0f,-1);
		tu = tip->FirstX(ctx,1.0f,-1);
    }
	
	r_here = root->Point(ru,r_tangent);
	t_here = tip->Point(tu,t_tangent);
	
	r_here.fx -= r_skin*r_tangent.fy;
	r_here.fy += r_skin*r_tangent.fx;
	
	t_here.fx -= t_skin*t_tangent.fy;
	t_here.fy += t_skin*t_tangent.fx;
	
	/* The spar is now completely defined by (for the root) crx */
	/* which is the centre of the top side, r_tangent which   */
	/* determines the angle of the spar and r_width & r_height */
	/* which determine its size */
	/* Note that the tangent is in the direction of increasing u */
	/* So now draw round 3 sides: */
	
	/* go to top-right corner: */
	r_here.fx -= r_width*r_tangent.fx/2.0f;
	r_here.fy -= r_width*r_tangent.fy/2.0f;
	t_here.fx -= t_width*t_tangent.fx/2.0f;
	t_here.fy -= t_width*t_tangent.fy/2.0f;
	rt=r_here;
	tt=t_here;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_move_to(rt,tt);
	
	
	/* down to bottom right corner: */
	r_here.fx -= r_height*r_tangent.fy;
	r_here.fy += r_height*r_tangent.fx;
	t_here.fx -= t_height*t_tangent.fy;
	t_here.fy += t_height*t_tangent.fx;
	rt=r_here;
	tt=t_here;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_line_to(rt,tt);
	
	/* along to bottom left corner: */
	r_here.fx += r_width*r_tangent.fx;
	r_here.fy += r_width*r_tangent.fy;
	t_here.fx += t_width*t_tangent.fx;
	t_here.fy += t_width*t_tangent.fy;
	rt=r_here;
	tt=t_here;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_line_to(rt,tt);
	
	/* and finally up to top left corner: */
	r_here.fx += r_height*r_tangent.fy;
	r_here.fy -= r_height*r_tangent.fx;
	t_here.fx += t_height*t_tangent.fy;
	t_here.fy -= t_height*t_tangent.fx;
	rt=r_here;
	tt=t_here;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_line_to(rt,tt);
	
	return;
}
/************************************************************/
/** PLOT_FULL_DEPTH SPAR_SIDE draws the side of a spar on  **/
/** the aerofoil. This                                      **/
/** routine has a first order correction to allow for the  **/
/** local gradient of the surface/skin thickness at the   **/
/** spar but ignores second order corrections.        **/
/************************************************************/
void CPathPlotter::plot_full_depth_spar_side(const CWing& wing, float rx,float tx, bool submerged) const
{
	float rut,tut;          /* root & tip u (top) */
	float rub,tub;          /* ditto (bottom) */
	float r_skin,t_skin;      /* skin thickness in aerofoil space*/
	PointT here;
	PointT tangent;
	PointT rt,tt;
	float txt,rxt,txb,rxb;
	float tyt,ryt,tyb,ryb;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	
	/* Convert edge position to aerofoil coords. */
	rx /= rootTransform->getChord();
	tx /= tipTransform->getChord();
	
	/* and convert skin thickness to aerofoil space */
	r_skin=wing.getSkinThickness() / rootTransform->getChord();
	t_skin=wing.getSkinThickness() / tipTransform->getChord();
	
	if(!submerged)         /* ignore skin if spar not under it */
		r_skin=t_skin=0.0f;
	
	rxt=rxb=rx;
	txt=txb=tx;
	
	if(wing.getSkinThickness() != 0.0f)
    {
		rut = root->FirstX(rx,0.0f,1);
		tut = tip->FirstX(tx,0.0f,1);
		rub = root->FirstX(rx,1.0f,-1);
		tub = tip->FirstX(tx,1.0f,-1);
		
		/* Get approx x values to allow for skin thickness */
		/* root top */
		here = root->Point(rut,tangent);
		here.fx += r_skin*tangent.fy;
		rxt=here.fx;   /* this is the new x to allow for skin thickess */
		
		/* tip top*/
		here = tip->Point(tut,tangent);
		here.fx += t_skin*tangent.fy;
		txt=here.fx;
		
		/* root bottom */
		here = root->Point(rub,tangent);
		here.fx += r_skin*tangent.fy;
		rxb=here.fx;   /* this is the new x to allow for skin thickess */
		
		/* tip bottom*/
		here = tip->Point(tub,tangent);
		here.fx += t_skin*tangent.fy;
		txb=here.fx;
    }
	
	/* Get u for all 4 corners */
	rut = root->FirstX(rxt,0.0f,1);
	tut = tip->FirstX(txt,0.0f,1);
	rub = root->FirstX(rxb,1.0f,-1);
	tub = tip->FirstX(txb,1.0f,-1);
	
	/* and get y for all 4 corners */
	here = root->Point(rut,tangent);
	here.fy += r_skin*tangent.fx;
	ryt=here.fy;
	
	here = tip->Point(tut,tangent);
	here.fy += t_skin*tangent.fx;
	tyt=here.fy;
	
	here = root->Point(rub,tangent);
	here.fy += r_skin*tangent.fx;
	ryb=here.fy;
	
	here = tip->Point(tub,tangent);
	here.fy += t_skin*tangent.fx;
	tyb=here.fy;
	
	/* Now move to the top of the spar side */
	rt.fx=rx;
	rt.fy=ryt;
	tt.fx=tx;
	tt.fy=tyt;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_move_to(rt,tt);
	
	/* and draw down to the bottom of the spar side */
	rt.fx=rx;
	rt.fy=ryb;
	tt.fx=tx;
	tt.fy=tyb;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_line_to(rt,tt);
	return;
}

/************************************************************/
/** PLOT_FULL_DEPTH SPAR draws a spar on the aerofoil.   **/
/** This routine deals only with full-depth spars! This   **/
/** routine has a first order correction to allow for the  **/
/** local gradient of the surface/skin thickness at the   **/
/** spar but ignores second order corrections.        **/
/************************************************************/
void CPathPlotter::plot_full_depth_spar(const CWing& wing, const CSpar& spar) const
{
	plot_full_depth_spar_side(wing,
		spar.getRootX(), 
		spar.getTipX(),
		spar.isSubmerged() );
	plot_full_depth_spar_side(wing,
		spar.getRootX() + spar.getRootWidth(),
		spar.getTipX() + spar.getTipWidth(),
		spar.isSubmerged() );
	return;
}


/************************************************************/
/** PLOT_ALL_SPARS plots all defined spars.        **/
/************************************************************/
void CPathPlotter::plot_all_spars(const CWing& wing) const
{
	
	for(int i=0; i<wing.getSparCount(); ++i)
    {
		const CSpar* spar = wing.getSpar(i);
		switch(spar->getType())
		{
		case CSpar::top:
		case CSpar::bottom:
			plot_spar(wing,*spar);
			break;
		case CSpar::full_depth:
			plot_full_depth_spar(wing, *spar);
			break;
		default:
			throw new KernelError(KernelError::IDS_ERR_INVALID_SPAR_TYPE);
			break;
		}
    }
	return;
}

/************************************************************/
/** DRAW_SECTIONS draws a root and a tip aerofoil          **/
/** simultaneously.                       **/
/************************************************************/
void CPathPlotter::plot(COutputDevice* pdev)
{
	assert(this);
	assert(pdev);
	
	setDevice(pdev);
	plot_flags = *(wing->getPlotFlags());
	setInterpolate(plot_flags.plot_section);

	PointT r_here,t_here;
	int root_stream,tip_stream;    /* output streams for root & tip */
	float u,delta;
	
	const CAerofoil* root = wing->getRoot();
	const CAerofoil* tip = wing->getTip();
	const CTransform* rootTransform = wing->getRootTransform();
	const CTransform* tipTransform = wing->getTipTransform();
	
	
	/* set up stream numbers for output */
	root_stream=0;     
	tip_stream=1;
	
	
	/* Set initial plot points */
	r_here = root->Point(0.0f);
	r_here = rootTransform->transform(r_here);
	
	t_here = tip->Point(0.0f);
	t_here = tipTransform->transform(t_here);
	
	interp_move_to(r_here,t_here);
	
	/* Now plot the aerofoils */
	delta=0.001f;
	for(u=delta;u<=1.0f;u+=delta)
    {
		r_here = root->Point(u);
		r_here = rootTransform->transform(r_here);
		
		t_here = tip->Point(u);
		t_here = tipTransform->transform(t_here);
		
		interp_line_to(r_here,t_here);
    }
	
	if(plot_flags.plot_skin && (wing->getSkinThickness() >0.0f))
    {
		plot_skin(*wing);
    }
	
	if(plot_flags.plot_chord)
    {
		plot_chord(*wing);
    }
	
	if(plot_flags.plot_markers)
    {
		plot_markers(*wing);
    }
	
	if(plot_flags.plot_le)
    {
		plot_le(*wing);
    }
	
	if(plot_flags.plot_te)
    {
		plot_te(*wing);
    }
	
	if(plot_flags.plot_labels)
    {
		plot_labels(*wing);
    }
	
	if(plot_flags.plot_spars)
    {
		plot_all_spars(*wing);
    }
	
	return;
}

string CPathPlotter::getDescriptiveText() const
{
	ostringstream ss;

	ss << "Wing Section at " << getSectionPos() << ends;
	return ss.str();
}

void CPathPlotter::serializeTo(CObjectSerializer& os)
{
	os.startSection("pathPlotter",this);
	CPlotStructure::serializeTo(os);
	os.writeReference("wing",wing);
	os.endSection();
}

void CPathPlotter::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection("pathPlotter",this);
	CPlotStructure::serializeFrom(os);
	wing = static_cast<CWing*>(os.readReference("wing"));
	os.endReadSection();
}
