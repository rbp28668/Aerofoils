#ifndef ELLIPSE_H_INCLUDED
#define ELLIPSE_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjectSerializer;

class CEllipse
{
public:
	CEllipse();
	virtual ~CEllipse();

	float getATop() const {return a_top;}
	float getABottom() const {return a_bottom;}
	float getB() const {return b;}
	float getX() const {return centre_x;}
	float getY() const {return centre_y;}

	void setATop(float rhs)  { a_top = rhs;}
	void setABottom(float rhs)  { a_bottom = rhs;}
	void setB(float rhs)  { b = rhs;}
	void setX(float rhs)  { centre_x = rhs;}
	void setY(float rhs)  { centre_y = rhs;}

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:
	float a_top;
	float a_bottom;
	float b;
	float centre_x;
	float centre_y;
//	float start_theta;
//	float end_theta;
};

#endif