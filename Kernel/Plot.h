
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
*/// Plot.h: interface for the CPlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOT_H__D83773A1_BA49_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_PLOT_H__D83773A1_BA49_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "Kernel.h"
#include "OutputDevice.h"
#include "PointT.h"

class CStructure;
class CWing;
class CEllipsePair;
class CPointStructure;

class CPlotStructure;
class CPathPlotter;
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

	void plot(COutputDevice& pdev, CPlotStructure* selected=0);

	// Add structure
	CWing* addWing(const char* rootSection, NumericT rootThickness, const char* tipSection, NumericT tipThickness);
	CEllipsePair* addEllipsePair(const CEllipsePair& pair);
	CPointStructure* addPointStructure(const CPointStructure& point);

	// Add plotters
	CPathPlotter* addPathPlotter(CWing* pWing);
	CEllipsePlotter* addEllipsePlotter(CEllipsePair* pep);
	CPointPlotter* addPointPlotter(CPointStructure* pps);

	// plotter manipulation
	void deletePlotStructure(CPlotStructure* toDelete);
	CPlotStructure* closestTo(NumericT x, NumericT y, NumericT& minDist) const;

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
		CClosest(NumericT x, NumericT y);

		virtual void MoveTo(int iStream, const PointT& pt) {};
		virtual void LineTo(int iStream, const PointT& pt);
		virtual void Label(int iStream, const char* psz) {};
		virtual void Home() {};
		virtual void Flush() {};
		virtual PointT position(int iStream) { return PointT(); }

		NumericT getMinDist() const {return d2min;}
	private:
		NumericT d2min;		// square of min distance from plot to test point
		NumericT xpos, ypos;	// position of test point

	};


//	WINGS wings;
//	ELLIPSES ellipses;
//	PLOT_POINTS points;
	STRUCTURES structures;
	PLOT_STRUCTURES plot_structures;
};

#endif // !defined(AFX_PLOT_H__D83773A1_BA49_11D6_AF75_002018BA082E__INCLUDED_)
