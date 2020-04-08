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

#define DELTA 0.1f   /* angle increment for plotting */
#define PI 3.14159265358979323846264338

static CObjectFactory<CPathPlotter> factory("pathPlotter");

/************************************************************/
/************************************************************/
CPathPlotter::CPathPlotter(CWing* pw)
: wing(pw)
{
	assert(this);
	assert(pw);
}

CPathPlotter::CPathPlotter()
: wing(0)
{
	assert(this);
}

NumericT CPathPlotter::getSpan() const
{
	assert(this);
	assert(wing);
	return wing->getSpan();
}

CStructure* CPathPlotter::getStructure()
{
	assert(this);
	return wing;
}

const CStructure* CPathPlotter::getStructure() const
{
	assert(this);
	return wing;
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
void CPathPlotter::plot_skin(COutputDevice* pdev, const CWing& wing) const
{
	PointT tangent;
	NumericT r_skin,t_skin;
	NumericT ru0,ru1;
	NumericT tu0,tu1;
	PointT r_here,t_here;
	NumericT delta;
	NumericT ru,tu;
	
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
	
	NumericT span = wing.getSpan();
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
	
	interp_move_to(pdev, r_here,t_here);
	
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
		
		interp_line_to(pdev,r_here,t_here);
    }
	
	r_here = root->Point(ru1,tangent);
	r_here.fx -= r_skin*tangent.fy;
	r_here.fy += r_skin*tangent.fx;
	
	t_here = tip->Point(tu1,tangent);
	t_here.fx -= t_skin*tangent.fy;
	t_here.fy += t_skin*tangent.fx;
	
	r_here = rootTransform->transform(r_here);
	t_here = tipTransform->transform(t_here);
	
	interp_line_to(pdev,r_here,t_here);
	return;
}

/************************************************************/
/** PLOT_CHORD plots a chord line for each section.    **/
/************************************************************/
void CPathPlotter::plot_chord(COutputDevice* pdev, const CWing& wing) const
{
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	
	PointT r_here = rootTransform->transform( PointT(0.0f, 0.0f));
	PointT t_here = tipTransform->transform( PointT(0.0f, 0.0f));
	
	interp_move_to(pdev, r_here,t_here);
	
	r_here = rootTransform->transform( PointT(1.0f, 0.0f));
	t_here = tipTransform->transform( PointT(1.0f, 0.0f));
	
	interp_line_to(pdev, r_here,t_here);
	
	return;
}

/************************************************************/
/** PLOT_MARKERS plots vertical markers every 10 %      **/
/************************************************************/
void CPathPlotter::plot_markers(COutputDevice* pdev, const CWing& wing) const
{
	int i;
	NumericT x;
	NumericT urt,urb;    /* u for root top and bottom */
	NumericT utt,utb;    /* u for tip top and bottom */
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	for(i=1;i<10;++i)        /* do markers 10%  to 90% inc */
    {
		x=i/10.0f;          /* convert to fraction */
		
		/* find parametric positions for the given x, top & bottom, root & tip */
		urt = root->FirstX(x,WEENY,1); 
		urb = root->FirstX(x,(NumericT)(1.0f-WEENY),-1);  /* lower surface u */
		utt = tip->FirstX(x,WEENY,1); 
		utb = tip->FirstX(x,(NumericT)(1.0f-WEENY),-1);  /* lower surface u */
		
		PointT pt_rt = root->Point(urt);
		PointT pt_rb = root->Point(urb);
		PointT pt_tt = tip->Point(utt);
		PointT pt_tb = tip->Point(utb);
		
		pt_rt = rootTransform->transform(pt_rt);
		pt_rb = rootTransform->transform(pt_rb);
		pt_tt = tipTransform->transform(pt_tt);
		pt_tb = tipTransform->transform(pt_tb);
		
		interp_move_to(pdev, pt_rb,pt_tb);
		interp_line_to(pdev, pt_rt,pt_tt);
    }
	return;
}


/************************************************************/
/** PLOT_LE plots the leading edge.                        **/
/************************************************************/
void CPathPlotter::plot_le(COutputDevice* pdev, const CWing& wing) const
{
	NumericT u0,u1;
	PointT rt,rb,tt,tb;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	
	/* Do LE first */
	NumericT rx = wing.getLE() / rootTransform->getChord();
	NumericT tx = wing.getLE() / tipTransform->getChord();
	
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
		
		interp_move_to(pdev, rt,tt);
		interp_line_to(pdev, rb,tb);
	}
	
	return;
}

/************************************************************/
/** PLOT_TE plots the trailing edge.                       **/
/************************************************************/
void CPathPlotter::plot_te(COutputDevice* pdev, const CWing& wing) const
{
	NumericT u0,u1;
	PointT rt,rb,tt,tb;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	if(wing.getTE() > 0.0f)
	{
		/* Now do the TE */
		NumericT rx = (rootTransform->getChord() - wing.getTE()) / rootTransform->getChord();
		u0 = root->FirstX(rx,0.25f,-1);  /* upper surface */
		u1 = root->FirstX(rx,0.75f,1);  /* lower surface */
		rt = root->Point(u0);
		rb = root->Point(u1);
		
		NumericT tx = (tipTransform->getChord() - wing.getTE()) / tipTransform->getChord();
		u0 = tip->FirstX(tx,0.25f,-1);  /* upper surface */
		u1 = tip->FirstX(tx,0.75f,1);    /* lower surface */
		tt = tip->Point(u0);
		tb = tip->Point(u1);
		
		rt = rootTransform->transform(rt);
		rb = rootTransform->transform(rb);
		tt = tipTransform->transform(tt);
		tb = tipTransform->transform(tb);
		
		interp_move_to(pdev, rt,tt);
		interp_line_to(pdev, rb,tb);
	}
	return;
}

/************************************************************/
/** PLOT_LABELS labels the aerofoils that are drawn.    **/
/************************************************************/
void CPathPlotter::plot_labels(COutputDevice* pdev, const CWing& wing) const
{
	NumericT u,delta;
	NumericT root_lowest,tip_lowest;
	PointT r_here,t_here;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	/* Look for the lowest point on each aerofoil or on the section */
	root_lowest = tip_lowest = numeric_limits<NumericT>::max();
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
		
		LabelAt(pdev, r_here, text.str(), true);
    }
	else
    {
		text << root_name << " Chord: " << rootTransform->getChord() << ends; 
		//  sprintf(text,"%s Chord: %2.1f%s",root_name,
		//      root->chord/get_unit_scale(),get_units_text());
		LabelAt(pdev, r_here, text.str(), true);
		
		text.seekp(0);
		text << tip_name << " Chord: " << tipTransform->getChord() << ends; 
		//    sprintf(text,"%s Chord: %2.1f%s",tip_name,
		//      tip->chord/get_unit_scale(),get_units_text());
		
		LabelAt(pdev, t_here, text.str(), false);
		
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
void CPathPlotter::plot_spar(COutputDevice* pdev, const CWing& wing, const CSpar& spar) const
{
	NumericT ru,tu;
	PointT here;
	PointT r_tangent,t_tangent;
	PointT r_here,t_here;
	PointT rt,tt;
	
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();
	
	NumericT root_chord = rootTransform->getChord();
	NumericT tip_chord = tipTransform->getChord();
	
	/* Convert spar sizes & position to aerofoil coords. */
	NumericT r_xpos = spar.getRootX() / root_chord;
	NumericT r_width = spar.getRootWidth() / root_chord;
	NumericT r_height = spar.getRootHeight() / root_chord;
	NumericT t_xpos = spar.getTipX() / tip_chord;
	NumericT t_width = spar.getTipWidth() / tip_chord;
	NumericT t_height = spar.getTipHeight() / tip_chord;
	
	/* and convert skin thickness to aerofoil space */
	NumericT r_skin = wing.getSkinThickness() / root_chord;
	NumericT t_skin = wing.getSkinThickness() / tip_chord;
	
	if(!spar.isSubmerged())     /* ignore skin if spar not under it */
		r_skin = t_skin=0.0f;
	
	// centre x coordinates
	NumericT crx=r_xpos+r_width/2.0f;
	NumericT ctx=t_xpos+t_width/2.0f;
	
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
	interp_move_to(pdev, rt,tt);
	
	
	/* down to bottom right corner: */
	r_here.fx -= r_height*r_tangent.fy;
	r_here.fy += r_height*r_tangent.fx;
	t_here.fx -= t_height*t_tangent.fy;
	t_here.fy += t_height*t_tangent.fx;
	rt=r_here;
	tt=t_here;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_line_to(pdev, rt,tt);
	
	/* along to bottom left corner: */
	r_here.fx += r_width*r_tangent.fx;
	r_here.fy += r_width*r_tangent.fy;
	t_here.fx += t_width*t_tangent.fx;
	t_here.fy += t_width*t_tangent.fy;
	rt=r_here;
	tt=t_here;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_line_to(pdev, rt,tt);
	
	/* and finally up to top left corner: */
	r_here.fx += r_height*r_tangent.fy;
	r_here.fy -= r_height*r_tangent.fx;
	t_here.fx += t_height*t_tangent.fy;
	t_here.fy -= t_height*t_tangent.fx;
	rt=r_here;
	tt=t_here;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_line_to(pdev, rt,tt);
	
	return;
}
/************************************************************/
/** PLOT_FULL_DEPTH SPAR_SIDE draws the side of a spar on  **/
/** the aerofoil. This                                      **/
/** routine has a first order correction to allow for the  **/
/** local gradient of the surface/skin thickness at the   **/
/** spar but ignores second order corrections.        **/
/************************************************************/
void CPathPlotter::plot_full_depth_spar_side(COutputDevice* pdev, const CWing& wing, NumericT rx,NumericT tx, bool submerged) const
{
	NumericT rut,tut;          /* root & tip u (top) */
	NumericT rub,tub;          /* ditto (bottom) */
	NumericT r_skin,t_skin;      /* skin thickness in aerofoil space*/
	PointT here;
	PointT tangent;
	PointT rt,tt;
	NumericT txt,rxt,txb,rxb;
	NumericT tyt,ryt,tyb,ryb;
	
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
	interp_move_to(pdev, rt,tt);
	
	/* and draw down to the bottom of the spar side */
	rt.fx=rx;
	rt.fy=ryb;
	tt.fx=tx;
	tt.fy=tyb;
	rt = rootTransform->transform(rt);
	tt = tipTransform->transform(tt);
	interp_line_to(pdev, rt,tt);
	return;
}

/************************************************************/
/** PLOT_FULL_DEPTH SPAR draws a spar on the aerofoil.   **/
/** This routine deals only with full-depth spars! This   **/
/** routine has a first order correction to allow for the  **/
/** local gradient of the surface/skin thickness at the   **/
/** spar but ignores second order corrections.        **/
/************************************************************/
void CPathPlotter::plot_full_depth_spar(COutputDevice* pdev, const CWing& wing, const CSpar& spar) const
{
	plot_full_depth_spar_side(pdev, wing,
		spar.getRootX(), 
		spar.getTipX(),
		spar.isSubmerged() );
	plot_full_depth_spar_side(pdev, wing,
		spar.getRootX() + spar.getRootWidth(),
		spar.getTipX() + spar.getTipWidth(),
		spar.isSubmerged() );
	return;
}


/************************************************************/
/** PLOT_ALL_SPARS plots all defined spars.        **/
/************************************************************/
void CPathPlotter::plot_all_spars(COutputDevice* pdev, const CWing& wing) const
{
	
	for(int i=0; i<wing.getSparCount(); ++i)
    {
		const CSpar* spar = wing.getSpar(i);
		switch(spar->getType())
		{
		case CSpar::top:
		case CSpar::bottom:
			plot_spar(pdev, wing,*spar);
			break;
		case CSpar::full_depth:
			plot_full_depth_spar(pdev, wing, *spar);
			break;
		default:
			throw new KernelError(KernelError::IDS_ERR_INVALID_SPAR_TYPE);
			break;
		}
    }
	return;
}

void CPathPlotter::plot_cutouts(COutputDevice* pdev, const CWing& wing) const {
	for (int i = 0; i < wing.getCutoutCount(); ++i) {
		const Cutout* cutout = wing.getCutout(i);
		plot_cutout(pdev, wing, *cutout);
	}
}


void CPathPlotter::plot_cutout(COutputDevice* pdev, const CWing& wing, const Cutout& cutout) const
{
	const CAerofoil* root = wing.getRoot();
	const CAerofoil* tip = wing.getTip();
	const CTransform* rootTransform = wing.getRootTransform();
	const CTransform* tipTransform = wing.getTipTransform();

	NumericT root_chord = rootTransform->getChord();
	NumericT tip_chord = tipTransform->getChord();

	/* Convert cutout sizes & position to aerofoil coords. */
	NumericT r_xpos = cutout.getRootX() / root_chord;
	NumericT r_ypos = cutout.getRootY() / root_chord;
	NumericT r_width = cutout.getRootWidth() / root_chord;
	NumericT r_height = cutout.getRootHeight() / root_chord;
	NumericT t_xpos = cutout.getTipX() / tip_chord;
	NumericT t_ypos = cutout.getTipY() / tip_chord;
	NumericT t_width = cutout.getTipWidth() / tip_chord;
	NumericT t_height = cutout.getTipHeight() / tip_chord;

	/* and convert skin thickness to aerofoil space */
	NumericT r_skin = wing.getSkinThickness() / root_chord;
	NumericT t_skin = wing.getSkinThickness() / tip_chord;

	/* Figure out where on the wing (in u) the cutout sits*/
	NumericT rut, rub, tut, tub;
	rut = root->FirstX(r_xpos, 0.0f, 1);
	tut = tip->FirstX(t_xpos, 0.0f, 1);
	rub = root->FirstX(r_xpos, 1.0f, -1);
	tub = tip->FirstX(t_xpos, 1.0f, -1);

	/* Centre points of the cutout*/
	NumericT rx, ry, tx, ty;

	/* Figure out where the centre points are */
	if (cutout.getIsCentred()) {
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

		if (cutout.getTopSurface()) {
			PointT rtan, ttan;
			PointT rpos = root->Point(rut,rtan);
			PointT tpos = tip->Point(tut,ttan);

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

	// Need to use radius rather than diameter
	double rw2 = r_width / 2;
	double rh2 = r_height / 2;
	double tw2 = t_width / 2;
	double th2 = t_height / 2;

	double theta = 0.0;
	PointT r(rx + cos(theta) * rw2, ry + sin(theta) * rh2);
	PointT t(tx + cos(theta) * tw2, ty + sin(theta) * th2);
	r = rootTransform->transform(r);
	t = tipTransform->transform(t);
	interp_move_to(pdev, r, t);


	for (theta = DELTA; theta <= 2 * PI + DELTA; theta += DELTA) {
		
		double angle = (theta > 2 * PI) ? 2 * PI : theta;
		double ct = cos(angle);
		double st = sin(angle);
		r.fx = rx + ct * rw2;
		r.fy = ry + st * rh2;
		t.fx = tx + ct * tw2;
		t.fy = ty + st * th2;
		r = rootTransform->transform(r);
		t = tipTransform->transform(t);
		interp_line_to(pdev, r, t);
	}

}

/************************************************************/
/** DRAW_SECTIONS draws a root and a tip aerofoil          **/
/** simultaneously.                       **/
/************************************************************/
void CPathPlotter::plot(COutputDevice* pdev)
{
	assert(this);
	assert(pdev);
	
	plot_flags = *(wing->getPlotFlags());
	setInterpolate(plot_flags.plot_section);

	PointT r_here,t_here;
	int root_stream,tip_stream;    /* output streams for root & tip */
	NumericT u,delta;
	
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
	
	interp_move_to(pdev, r_here,t_here);
	
	/* Now plot the aerofoils */
	delta=0.001f;
	for(u=delta;u<=1.0f;u+=delta)
    {
		r_here = root->Point(u);
		r_here = rootTransform->transform(r_here);
		
		t_here = tip->Point(u);
		t_here = tipTransform->transform(t_here);
		
		interp_line_to(pdev, r_here,t_here);
    }
	
	if(plot_flags.plot_skin && (wing->getSkinThickness() >0.0f))
    {
		plot_skin(pdev, *wing);
    }
	
	if(plot_flags.plot_chord)
    {
		plot_chord(pdev, *wing);
    }
	
	if(plot_flags.plot_markers)
    {
		plot_markers(pdev, *wing);
    }
	
	if(plot_flags.plot_le)
    {
		plot_le(pdev, *wing);
    }
	
	if(plot_flags.plot_te)
    {
		plot_te(pdev, *wing);
    }
	
	if(plot_flags.plot_labels)
    {
		plot_labels(pdev, *wing);
    }
	
	if(plot_flags.plot_spars)
    {
		plot_all_spars(pdev, *wing);
    }
	
	if (plot_flags.plot_cutouts)
	{
		plot_cutouts(pdev, *wing);
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
