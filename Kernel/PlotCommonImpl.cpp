// PlotCommonImpl.cpp: implementation of the CPlotCommonImpl class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <assert.h>
#include "PlotCommonImpl.h"
#include "Aerofoil.h"
#include "Wing.h"

const float CPlotCommonImpl::CLOSE = 1.0e-3f;     /* limit of resolution in mm per step */
const int CPlotCommonImpl::MAX_ITER = 10;			/* max number of runs thro' the iteration */
const int CPlotCommonImpl::FORWARD_ITER = 14;		/* 1 part in 16000 */
const float CPlotCommonImpl::WEENY = 1.0e-6f;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlotCommonImpl::CPlotCommonImpl(CWing* pw)
: wing(pw)
{
	assert(pw);
}

CPlotCommonImpl::CPlotCommonImpl() 
:wing(0)
{}

CPlotCommonImpl::~CPlotCommonImpl()
{

}

float CPlotCommonImpl::getSpan() const
{
	assert(this);
	assert(wing);
	return wing->getSpan();
}

CStructure* CPlotCommonImpl::getStructure()
{
	assert(this);
	return wing;
}

const CStructure* CPlotCommonImpl::getStructure() const
{
	assert(this);
	return wing;
}

/***********************************************************/
/** FIND_CORE_TE  searches along the spline curve from    **/
/** top and bottom of the TE to find where the core       **/
/** starts when a finite skin thickness is used. nu0 and  **/
/** nu1 are set to the parametric values of the core TE   **/
/** Note that skin must be in raw aerofoil units i.e.     **/
/** made independent of the chord.                        **/
/***********************************************************/
void CPlotCommonImpl::find_core_te(const CAerofoil& foil,float skin, float *nu0,float *nu1) const
{
  float x0,y0;      /* Starting coord on upper surface */
  float x1,y1;      /* Starting coord on lower surface */
  float u0,u1;      /* parametric var at current search posn */
  PointT t0,t1;      /* tangents */
  PointT s0,s1;      /* wing surface PointTs */
  float x /* ,y */ ;/* PointT of intersection */
   float dist;
  float m0,m1;      /* gradients  of top & bottom surfaces */
  int i;

  u0=(float)WEENY;         /* initial search posn */
  u1=(float)(1.0f-WEENY);

  if(skin < 0) /* then tool offset > skin thickness */
    {
    *nu0 = u0; /* so just start at trailing edge */
    *nu1 = u1;
    }
  else
    {
    for(i=0;i<MAX_ITER;++i)      /* limit no of iterations */
      {
      s0 = foil.Point(u0,t0);	//get_spline_point(spline_handle,u0,&s0);
      s1 = foil.Point(u1,t1);	//get_spline_point(spline_handle,u1,&s1);

      /* Get PointTs on core surface */
      x0 = s0.fx -= skin*t0.fy;
      y0 = s0.fy += skin*t0.fx;
      x1 = s1.fx -= skin*t1.fy;
      y1 = s1.fy += skin*t1.fx;


      /* Find distance between them */
      dist=(float)sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));

      if(dist<(float)CLOSE)
        {
        *nu0=u0;
        *nu1=u1;
        return;
        }

      /* and local gradients */
      m0 = t0.fy/t0.fx;
      m1 = t1.fy/t1.fx;

      /* we now have 2 lines defined that go thro (x0,y0) and (x1,y1) */
      /* They have slopes of m0 and m1*/
      /* using the y-b=m(x-a) for each line we want to find the x */
      /* so that the y's are equal i.e. the PointT of intersection */

      x = (x0*m0-x1*m1+y1-y0)/(m0-m1);
      /* y = m0*(x-x0)+y0; */

      /* Note that if the trailing edge spline is in fact a straight */
      /* line over the region of interest then the above PointT is the */
      /* exact position of the core trailing edge. */

      x0 = x + skin*t0.fy;    /* go back up to skin surface */
      x1 = x - skin*t1.fy;    /* and find x coordinate */

      /* now use x coordinates to find new (improved) u0 and u1 */

      u0=foil.FirstX(x0,WEENY,1); /* get upper surface u for */
                                  /* the given x, starting at */
                                  /* u=WEENY in +ve direction */
      u1=foil.FirstX(x1,(float)(1.0f-WEENY),-1);  /* lower surface u */
      }
    /* Note that if the code gets to here the convergence will be poor */
    /* but probably close enough for practical purposes */
    *nu0=u0;
    *nu1=u1;
    return;
    }
}
