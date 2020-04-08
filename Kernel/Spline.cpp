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
File:           SPLINE.CPP

Program/rev.:     foil 0.0

Function:       To implement parametric spline curves

Author:       Bruce (spline & splint from NRIC)

Date:         1991 

Routines:

Input Parameters:

Output Parameters:

Error conditions:

Description:

Revision History:

*/



#include <assert.h>
#include <float.h>
#include <math.h>

#include "KernelError.h"
#include "spline.h"
#include "PointT.h"


#define CLOSE 1.0e-5    /* satisfactory closeness for u search */



/************************************************************/
/** SPLINE calculates a second derivative vector for    **/
/** calculating natural splines.              **/
/************************************************************/
void CSpline::spline(NumericT *x,NumericT *y,int n, NumericT *y2)
{
  int i,k;
  NumericT p,qn,sig,un,*u;
  
  u = new NumericT[n-1];
  if(u)
    {
  
    y2[0]=u[0]=0.0f;        /* Set lower boundary for natural splines */
  
    /* Tridiagonal decomposition loop */
    for (i=1;i<=n-2;i++)
      {
      sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
      p=sig*y2[i-1]+2.0f;
      y2[i]=(sig-1.0f)/p;
      u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
      u[i]=(6.0f*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
      }
  
    qn=un=0.0f;         /* Set upper boundary for natural splines */
  
    y2[n-1]=(un-qn*u[n-2])/(qn*y2[n-2]+1.0f);
    /* Back substitution for tridiagonal algorithm */
    for (k=n-2;k>=0;k--)
      y2[k]=y2[k]*y2[k+1]+u[k];
    }
  else
    {
	delete [] u;
    m_blBad = true;
	throw new KernelError(KernelError::IDS_ERR_SPLINE_SPLINE_MEM);
    }
    
  delete [] u;
  
  return;
}

/************************************************************/
/** SPLINT is the spline interpolation routine that     **/
/** calculates y and dy/dx at a given x.          **/
/************************************************************/
void CSpline::splint(NumericT *xa, NumericT *ya, NumericT *y2a, int n, NumericT x,
  NumericT *y,NumericT *dydx) const
{
  bool blOK = true;
  int klo,khi,k;
  NumericT h,b,a;

  klo=0;
  khi=n-1;
  while (khi-klo > 1)
    {
    k=(khi+klo) >> 1;
    if (xa[k] > x)
      khi=k;
    else
      klo=k;
    }
  h=xa[khi]-xa[klo];
  if (h == 0.0f)
    {
    throw new KernelError(KernelError::IDS_ERR_SPLINE_BAD_XA);
    }
  else
    {
    a=(xa[khi]-x)/h;
    b=(x-xa[klo])/h;
    *y=a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0f;
    *dydx=(ya[khi]-ya[klo])/h -
      ((3.0f*a*a-1.0f)*y2a[klo]-(3.0f*b*b-1.0f)*y2a[khi])*h/6.0f;
    }
  return;
}

CSpline::CSpline(PointT* curve, int n)
{
  m_blBad = false;
  npts = 0;
  
  xx = new NumericT[n];
  yy = new NumericT[n];
  uu = new NumericT[n];
  xx2 = new NumericT[n];
  yy2 =  new NumericT[n];

  if( (xx == 0)
    ||(yy == 0)
    ||(xx2 == 0)
    ||(yy2 == 0)
    ||(uu == 0))
    {
		m_blBad = true;
		throw new KernelError(KernelError::IDS_ERR_SPLINE_MEM);
     }
  else
    {
    int i=0;
    
    /* Want to calculate total distance round the curve and also set **/
    /* uu[] up so that uu[n] contains the current distance from the **/
    /** first point to point x[n],y[n]. This will be used to approx **/
    /* a constant velocity of drawing (or cutting tool !) as u runs **/
    /* in equal steps from 0 to 1                   **/
    NumericT totdist = 0;
    for(i=0;i<n-1;++i)
      {
      NumericT dx = curve[i+1].fx - curve[i].fx;
      NumericT dy = curve[i+1].fy - curve[i].fy;
      NumericT dist = (NumericT)sqrt(dx*dx + dy*dy);
      uu[i] = totdist;
      totdist += dist;
      }
    uu[i] = totdist;
    assert(i == (n-1));
  
  
    /* Now set up the xx and yy arrays to store the actual coordinates. */
    /* Also scale uu array to lie in the range 0..1 */
    for(i=0;i<n;++i)
      {
      xx[i] = curve[i].fx;
      yy[i] = curve[i].fy;
      uu[i] /= totdist;
      }
  
  
    /* Now set up the 2pd arrays for x and y wrt u */
    spline(uu,xx,n,xx2);  /* calculate 2nd deriv array for x wrt u */
    spline(uu,yy,n,yy2);  /* calculate 2nd deriv array for y wrt u */
    
    npts=n;
    }
}

CSpline::~CSpline()
{
  delete [] yy2;
  delete [] xx2;
  delete [] uu;
  delete [] yy;
  delete [] xx;
}

PointT CSpline::Point(NumericT u) const
{
  NumericT x = 0;
  NumericT y = 0;
  NumericT dxdu = 0;
  NumericT dydu = 0;
  splint(uu,xx,xx2,npts,u,&x,&dxdu);    /* get x coord at posn u */
  splint(uu,yy,yy2,npts,u,&y,&dydu);    /* and y coord at posn u */
  return PointT(x,y);
}

PointT CSpline::Point(NumericT u, PointT& tangent) const
{
  NumericT x = 0;
  NumericT y = 0;
  NumericT dxdu = 0;
  NumericT dydu = 0;

  splint(uu,xx,xx2,npts,u,&x,&dxdu);    /* get x coord at posn u */
  splint(uu,yy,yy2,npts,u,&y,&dydu);    /* and y coord at posn u */

  NumericT dist;
  dist=(NumericT)sqrt(dxdu*dxdu + dydu*dydu);
  tangent = PointT(dxdu/dist, dydu/dist);

  return PointT(x,y);
}

PointT CSpline::Tangent(NumericT u) const
{
  NumericT x = 0;
  NumericT y = 0;
  NumericT dxdu = 0;
  NumericT dydu = 0;

  splint(uu,xx,xx2,npts,u,&x,&dxdu);    /* get x coord at posn u */
  splint(uu,yy,yy2,npts,u,&y,&dydu);    /* and y coord at posn u */

  NumericT dist;
  dist=(NumericT)sqrt(dxdu*dxdu + dydu*dydu);
  return PointT(dxdu/dist, dydu/dist);
}

NumericT CSpline::FirstX(NumericT req_x, NumericT start, int dirn) const
{
  NumericT step;
  NumericT u;
  NumericT prev_x;
  NumericT x0;
  NumericT low_u,high_u; /* bracketing for final seach */
  NumericT low_x,high_x; /* ditto */

  if(dirn <0)
    step=-0.01f;
  else
    step=0.01f;

  PointT here = Point(start);
  x0=here.fx;

  prev_x=0.0f;
  for(u=start+step;(u>=0.0f) && (u<=1.0f) ;u+=step)
    {
    prev_x=here.fx;
    here = Point(u);
    if( (x0-here.fx)*(here.fx-req_x) <0.0f) /* if have just passed req_x*/
      {
      break;
      }
    }

  /* req_x is now bracketed by prev_x and here.x (at u-step and u) */
  /* We now want to use successive linear interpolation to nail */
  /* the exact (ish) point.                       */
  low_u=u-step;
  high_u=u;
  low_x=prev_x;
  high_x=here.fx;

  do
    {
    /* now find an estimate new u by linear interpolation of the x values */
    u= low_u + (req_x-low_x)/(high_x-low_x)*(high_u-low_u);
    here = Point(u);

    if(here.fx < req_x)
      {
      low_u=u;
      low_x=here.fx;
      }
    else
      {
      high_u=u;
      high_x=here.fx;
      }
    }while(fabs(here.fx-req_x)>CLOSE);
  
  return u;
}




/**===============================================================**/
/**===============================================================**/
/**----------------- END OF SPLINE PACKAGE -----------------------**/
/**===============================================================**/
/**===============================================================**/





