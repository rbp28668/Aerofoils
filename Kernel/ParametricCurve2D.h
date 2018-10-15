#ifndef _ParametricCurve2D
#define _ParametricCurve2D

#include "PointT.h"

struct ParametricCurve2D
{
	PointT Point(float u);
	PointT Point(float u, PointT& tangent);
	float FirstX(float req_x, float start, int dirn);
};


#endif