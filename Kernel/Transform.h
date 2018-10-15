// Transform.h: interface for the CTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFORM_H__EF095242_B4C9_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_TRANSFORM_H__EF095242_B4C9_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointT.h"

class CObjectSerializer;

class CTransform  
{
public:
	CTransform();
	virtual ~CTransform();

	PointT transform(PointT pos) const;

	void setChord(float _chord);
	void setSweep(float _sweep);
	void setWashout(float _washout);
	void setHeight(float _height);
	void setInvert(bool _invert);


	float getChord() const {return chord;}
	float getSweep() const {return sweep;}
	float getWashout() const {return washout;}
	float getHeight() const {return height;}

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:

	void set_rotate(float degrees);

	float chord;          /* chord in mm */
	float sweep;          /* offset of x=0 from datum */
	float washout;        /* in degrees, +ve is washout -ve washin */
	float height;         /* height of l/e above base (in mm) */
	bool invert;

	struct ROTATE
	{
		float cos_theta;
		float sin_theta;
	};

	ROTATE rotate;
};

#endif // !defined(AFX_TRANSFORM_H__EF095242_B4C9_11D6_AF75_002018BA082E__INCLUDED_)
