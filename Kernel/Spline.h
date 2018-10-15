/*
File:         SPLINE.H

Program/rev.: foil 0.0

Function:     Header for parametric spline curves

Author:       Bruce (spline & splint from NRIC)

Date:         1991 

Routines:

Input Parameters:

Output Parameters:

Error conditions:

Description:

Revision History:

*/

#ifndef _SPLINE_H
#define _SPLINE_H

class PointT;

class CSpline
{
public:

	CSpline(PointT* curve, int n);
	~CSpline();
	bool Bad() const {return m_blBad;}
	PointT Point(float u) const;
	PointT Point(float u, PointT& tangent) const;
	PointT Tangent(float u) const;
	float FirstX(float req_x, float start, int dirn) const;

private:

	// Forbid copying
	CSpline(const CSpline& rhs) {}
	CSpline& operator= (const CSpline& rhs) {return *this;}
	
	void spline(float *x,float *y,int n, float *y2);
	void splint(float *xa, float *ya, float *y2a, int n, float x, float *y,float *dydx) const;

	bool m_blBad;
	float *xx;        /* array of x coordinates */
	float *yy;        /* array of y coordinates */
	float *uu;        /* array of parametric coordinates */
	float *xx2;       /* 2nd deriv of x wrt u */
	float *yy2;       /* 2nd deriv of y wrt u */
	int npts;       /* number of points on curve */
};

#endif
