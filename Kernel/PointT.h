// PointT.h: interface for the PointT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTT_H__A4E22B61_B474_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_POINTT_H__A4E22B61_B474_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PointT  
{
public:
	PointT(float x = 0.0f, float y = 0.0f);
	~PointT();

	float fx;
	float fy;

};

#endif // !defined(AFX_POINTT_H__A4E22B61_B474_11D6_AF75_002018BA082E__INCLUDED_)
