// PlotPoint.h: interface for the CPlotPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOTPOINT_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_)
#define AFX_PLOTPOINT_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlotStructure.h"

class CPlotPoint : public CPlotStructure  
{
public:
	CPlotPoint();
	virtual ~CPlotPoint();
	virtual void plot(COutputDevice *pdev);
	virtual float getSpan() const;
	virtual std::string getDescriptiveText() const;

	virtual	void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);


	PointT getRoot() const {return root;}
	PointT getTip() const {return tip;}

	void setRoot(PointT pt) {root = pt;}
	void setTip(PointT pt) {tip = pt;}

	void setSpan(float f) {span = f;}

	void setSilent(bool s) {silent = s;}
	bool isSilent() const {return silent;}

private:
	bool silent; // if true, only does a move (for a cutter)
	PointT root;	// offsets;
	PointT tip;		// offsets;
	float span;
};

#endif // !defined(AFX_PLOTPOINT_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_)
