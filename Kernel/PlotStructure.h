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
*/// PlotStructure.h: interface for the CPlotStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOTSTRUCTURE_H__D83773A2_BA49_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_PLOTSTRUCTURE_H__D83773A2_BA49_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "PointT.h"
#include "RectT.h"
#include "OutputDevice.h"
#include "Structure.h"

class CObjectSerializer;
class CUIProxy;

class CPlotStructure  
{
public:
	CPlotStructure();
	virtual ~CPlotStructure();

	virtual void plot(COutputDevice *pdev) = 0;
	virtual std::string getDescriptiveText() const = 0;
	virtual CStructure* getStructure() = 0;
	virtual const CStructure* getStructure() const = 0;

	float getSpan() const {return getStructure()->getSpan();}

	COutputDevice* setDevice(COutputDevice *pdev);
	void setUIProxy(CUIProxy* pp);
	CUIProxy* getUIProxy();

	float getX() const {return xpos;}
	float getY() const {return ypos;}
	void setPosition(float x, float y) {xpos = x; ypos = y;}

	RectT getBounds();

	// Interpolation between ends
	float getSectionPos() const;
	void setSectionPos(float pos);

	bool getInterpolate() const {return shouldInterpolate;}
	void setInterpolate(bool bl) { shouldInterpolate = bl;}

	void setRootSide(bool isLeft);
	bool isRootLeft() const {return rootIsOnLeft;}

	void setInvert(bool inv);
	bool isInverted() const {return invert;}

	void setReflect(bool ref);
	bool isReflected() const {return reflect;}

	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

protected:
	PointT interpolate(const PointT& root, const PointT& tip) const;
	void interp_move_to(const PointT& root, const PointT& tip) const;
	void interp_line_to(const PointT& root,const PointT& tip) const;
	void LabelAt(const PointT& where, const std::string& text, bool atRoot) const;

private:

	PointT offset(PointT pos) const;

	class CBounds : public COutputDevice
	{
	public:

		CBounds();
		virtual void MoveTo(int iStream, const PointT& pt);
		virtual void LineTo(int iStream, const PointT& pt);
		virtual void Label(int iStream, const char* psz) {};
		virtual void Home() {};
		virtual void Flush() {};

		RectT getBounds() const;
	private:
		float minx,maxx,miny,maxy;
		PointT lastMove;
		bool lastOpIsMove;
	};

	float xpos,ypos;	// position of section on plot
	COutputDevice* plotDevice;

	float section_pos;		// position for cross section.
	bool shouldInterpolate;

	bool rootIsOnLeft;
	int root_stream;
	int tip_stream;

	bool invert;	// top to bottom
	bool reflect;	// left to right

	CUIProxy* proxy;		// UI implementation

};

#endif // !defined(AFX_PLOTSTRUCTURE_H__D83773A2_BA49_11D6_AF75_002018BA082E__INCLUDED_)
