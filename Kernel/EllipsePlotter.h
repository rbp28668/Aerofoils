// EllipsePlotter.h: interface for the CEllipsePlotter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIPSEPLOTTER_H__D83773A3_BA49_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_ELLIPSEPLOTTER_H__D83773A3_BA49_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlotStructure.h"

class CEllipse;
class CEllipsePair;
class COutputDevice;

class CEllipsePlotter : public CPlotStructure
{
public:
	CEllipsePlotter(CEllipsePair* pep);
	explicit CEllipsePlotter();		// for serialization
	virtual ~CEllipsePlotter();

	CEllipsePair* getEllipses() {return ellipses;}
	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

	virtual void plot(COutputDevice* pdev);
	virtual std::string getDescriptiveText() const;

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:
	CEllipsePair* ellipses;

};

#endif // !defined(AFX_ELLIPSEPLOTTER_H__D83773A3_BA49_11D6_AF75_002018BA082E__INCLUDED_)
