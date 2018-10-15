// PointPlotter.h: interface for the CPointPlotter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PointPlotter_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_)
#define AFX_PointPlotter_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlotStructure.h"

class CPointStructure;

class CPointPlotter : public CPlotStructure
{
public:
	explicit CPointPlotter();
	CPointPlotter(CPointStructure* _struct);
	virtual ~CPointPlotter();

	virtual void plot(COutputDevice *pdev);
	virtual std::string getDescriptiveText() const;

	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

	virtual	void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	CPointStructure* getPointStructure() {return structure;}

private:
	CPointStructure* structure;
};

#endif // !defined(AFX_PointPlotter_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_)
