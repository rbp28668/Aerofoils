// Plot.h: interface for the CPlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOT_H__D83773A1_BA49_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_PLOT_H__D83773A1_BA49_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "OutputDevice.h"

class CStructure;
class CWing;
class CEllipsePair;
class CPointStructure;

class CPlotStructure;
class CPathPlotter;
class CPathCutter;
class CEllipsePlotter;
class CPointPlotter;

class CObjectSerializer;

class CPlot  
{
	typedef std::list<CPlotStructure*> PLOT_STRUCTURES;
	typedef std::list<CStructure*> STRUCTURES;

public:
	CPlot();
	virtual ~CPlot();

	void plot(COutputDevice& pdev);

	// Add structure
	CWing* addWing(const char* rootSection, float rootThickness, const char* tipSection, float tipThickness);
	CEllipsePair* addEllipsePair(const CEllipsePair& pair);
	CPointStructure* addPointStructure(const CPointStructure& point);

	// Add plotters
	CPathPlotter* addPathPlotter(CWing* pWing);
	CPathCutter* addPathCutter(CWing* pWing);
	CEllipsePlotter* addEllipsePlotter(CEllipsePair* pep);
	CPointPlotter* addPointPlotter(CPointStructure* pps);

	// plotter manipulation
	void deletePlotStructure(CPlotStructure* toDelete);
	CPlotStructure* closestTo(float x, float y, float& minDist) const;

	void deleteStructure(CStructure* toDelete);

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

	typedef STRUCTURES::iterator StructureIterator;
	typedef PLOT_STRUCTURES::iterator PlotStructureIterator;

	StructureIterator getStructures() {return structures.begin();}
	PlotStructureIterator getPlotStructures() {return plot_structures.begin();}

	StructureIterator endStructures() {return structures.end();}
	PlotStructureIterator endPlotStructures() {return plot_structures.end();}
	
	int getStructureCount() const {return (int)structures.size();}

	void moveToEnd(CPlotStructure* ps);

private:

	class CClosest : public COutputDevice
	{
	public:
		CClosest(float x, float y);

		virtual void MoveTo(int iStream, const PointT& pt) {};
		virtual void LineTo(int iStream, const PointT& pt);
		virtual void Label(int iStream, const char* psz) {};
		virtual void Home() {};
		virtual void Flush() {};

		float getMinDist() const {return d2min;}
	private:
		float d2min;		// square of min distance from plot to test point
		float xpos, ypos;	// position of test point

	};


//	WINGS wings;
//	ELLIPSES ellipses;
//	PLOT_POINTS points;
	STRUCTURES structures;
	PLOT_STRUCTURES plot_structures;
};

#endif // !defined(AFX_PLOT_H__D83773A1_BA49_11D6_AF75_002018BA082E__INCLUDED_)
