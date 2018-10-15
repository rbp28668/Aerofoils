// RectT.h: interface for the RectT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTT_H__06CE6422_BB77_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_RECTT_H__06CE6422_BB77_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointT.h"

class RectT  
{
public:
	RectT(float l = 0.0f, float t = 0.0f, float r = 0.0f, float b = 0.0f);
	virtual ~RectT();

	PointT topLeft;
	PointT bottomRight;

};

#endif // !defined(AFX_RECTT_H__06CE6422_BB77_11D6_AF75_002018BA082E__INCLUDED_)
