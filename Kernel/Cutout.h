#pragma once
#include <string>
#include "Kernel.h"

class CObjectSerializer;

class Cutout
{
	bool topSurface;      // true if going in from the top service.  
	bool centre;          // true if ignore y_position and centre in wing
	NumericT root_x;         /* distance of cutout from le at root */
	NumericT tip_x;          /* ditto at tip */
	NumericT root_y;
	NumericT tip_y;
	NumericT root_width;     // in mm of cutout
	NumericT tip_width;
	NumericT root_height;
	NumericT tip_height;

	std::string text; 
public:
	Cutout();

	bool getTopSurface() const { return topSurface; }
	bool getIsCentred() const { return centre; }
	NumericT getRootX() const { return root_x; }
	NumericT getTipX() const { return  tip_x; }
	NumericT getRootY() const { return  root_y; }
	NumericT getTipY() const { return tip_y; }
	NumericT getRootWidth() const { return root_width; }
	NumericT getTipWidth() const { return tip_width; }
	NumericT getRootHeight() const { return root_height; }
	NumericT getTipHeight() const { return tip_height; }

	void setTopSurface(bool isTop) { topSurface = isTop; }
	void setCentred(bool isCentred) { centre = isCentred; }
	void setRootX(NumericT rx) { root_x = rx; }
	void setTipX(NumericT tx) { tip_x = tx; }
	void setRootY(NumericT ry) { root_y = ry; }
	void setTipY(NumericT ty) { tip_y = ty; }
	void setRootWidth(NumericT rw) { root_width = rw; }
	void setTipWidth(NumericT tw) { tip_width = tw; }
	void setRootHeight(NumericT rh) { root_height = rh; }
	void setTipHeight(NumericT th) { tip_height = th; }

	const char* getText();

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

};

