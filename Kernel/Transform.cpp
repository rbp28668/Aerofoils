// Transform.cpp: implementation of the CTransform class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <math.h>
#include "Transform.h"
#include "ObjectSerializer.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransform::CTransform()
: chord(100)
, sweep(0)
, washout(0)
, height(0)
, invert(false)
{
	set_rotate(washout);
}

CTransform::~CTransform()
{

}

/************************************************************/
/** TRANSFORM takes a raw PointT on an aerofoil and         **/
/** transforms it to a given chord, sweep position and     **/
/** washout.                                               **/ 
/************************************************************/
PointT CTransform::transform(PointT here) const
{
	assert(this);

  /* Convert coords to give required chord */
  float x = here.fx * chord;
  float y = here.fy * chord;

  /* rotate coords to allow for washout */
  here.fx= x * rotate.cos_theta + y * rotate.sin_theta;
  here.fy= y * rotate.cos_theta - x * rotate.sin_theta;

  /* allow for sweepback & height*/
  here.fx += sweep;
  if(invert)
    here.fy = height - here.fy;
  else
    here.fy += height;

  return here;
}

void CTransform::setChord(float _chord)
{
	assert(this);
	chord = _chord;
}

void CTransform::setSweep(float _sweep)
{
	assert(this);
	sweep = _sweep;
}

void CTransform::setWashout(float _washout)
{
	assert(this);
	washout = _washout;
	set_rotate(washout);
}

void CTransform::setHeight(float _height)
{
	assert(this);
	height = _height;
}

void CTransform::setInvert(bool _invert)
{
	assert(this);
	invert = _invert;
}


/************************************************************/
/** SET_ROTATE sets up a ROTATE structure to a given angle **/
/** in degrees.                                            **/
/************************************************************/
void CTransform::set_rotate(float degrees)
{
  double rads;
  rads= degrees * -3.14159265358979323846264338 / 180.0;
  rotate.cos_theta=(float) cos(rads);
  rotate.sin_theta=(float) sin(rads);
  return;
}

void CTransform::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("transform",this);
	os.write("chord",chord);
	os.write("height",height);
	os.write("invert",invert);
	os.write("sweep", sweep);
	os.write("washout",washout);
	os.endSection();
}

void CTransform::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("transform",this);
	os.read("chord",chord);
	os.read("height",height);
	os.read("invert",invert);
	os.read("sweep", sweep);
	os.read("washout",washout);
	os.endReadSection();

	set_rotate(washout);
}

