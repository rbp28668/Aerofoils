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
*/// Plot.cpp: implementation of the CPlot class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <limits>

#include "Plot.h"
#include "PlotStructure.h"
#include "plotfoil.h" // "PathPlotter.h"
#include "cutpath.h"  // "PathCutter.h"
#include "EllipsePlotter.h"
#include "PointPlotter.h"
#include "DXFPlotter.h"
#include "Wing.h"
#include "EllipsePair.h"
#include "PointStructure.h"
#include "DXFObject.h"
#include "ObjectSerializer.h"

#ifdef max
#undef max
#endif

CPlot::CClosest::CClosest(NumericT x, NumericT y)
: xpos(x)
, ypos(y)
, minDist(std::numeric_limits<NumericT>::max())
{
}

void CPlot::CClosest::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	lastPoint[iStream] = pt;
}

void CPlot::CClosest::LineTo(int iStream, const PointT& pt)
{

	// See https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
	PointT p0 = lastPoint[iStream];
	PointT p1 = pt;

	NumericT dx = p1.fx - p0.fx;
	NumericT dy = p1.fy - p0.fy;

	NumericT dist = abs(dy * xpos - dx * ypos + p1.fx * p0.fy - p1.fy * p0.fx) /
		sqrt(dx * dx + dy * dy);

	if(dist < minDist)
		minDist = dist;

	lastPoint[iStream] = pt;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlot::CPlot()
{

}

CPlot::~CPlot()
{


	for(STRUCTURES::iterator si = structures.begin();
	si != structures.end();
	++si)
	{
		delete (*si);
	}

	for(PLOT_STRUCTURES::iterator psi = plot_structures.begin();
	psi != plot_structures.end();
	++psi)
	{
		delete (*psi);
	}

}

void CPlot::plot(COutputDevice& pdev, CPlotStructure* selected)
{
	pdev.startPlot();
	for(PLOT_STRUCTURES::iterator iter = plot_structures.begin();
	iter != plot_structures.end();
	++iter)
	{
		CPlotStructure* pps = *iter;
		pdev.startObject(pps->getDescriptiveText().c_str(), pps == selected);
		pps->plot(&pdev);
		pdev.endObject(pps->getDescriptiveText().c_str(), pps == selected);
	}
	pdev.Flush();
	pdev.endPlot();
}

void CPlot::addStructure(CStructure* pStructure)
{
	assert(this);
	assert(pStructure);
	structures.push_back(pStructure);
}

CWing* CPlot::addWing(const char* rootSection, NumericT rootThickness, const char* tipSection, NumericT tipThickness)
{
	assert(this);
	CWing* pWing = new CWing(rootSection, rootThickness, tipSection, tipThickness);
	structures.push_back(pWing);
	return pWing;
}

CEllipsePair* CPlot::addEllipsePair(const CEllipsePair& pair)
{
	assert(this);
	CEllipsePair* pEllipses = new CEllipsePair(pair);
	structures.push_back(pEllipses);
	return pEllipses;
}

CPointStructure* CPlot::addPointStructure(const CPointStructure& point)
{
	assert(this);
	CPointStructure* pp = new CPointStructure(point);
	structures.push_back(pp);
	return pp;
}

DXFObject* CPlot::addDxfStructure(DXFObject* pdxf)
{
	structures.push_back(pdxf);
	return pdxf;

}


CPathPlotter* CPlot::addPathPlotter(CWing* pWing)
{
	assert(this);
	CPathPlotter* pp = new CPathPlotter(pWing);
	plot_structures.push_back(pp);
	return pp;
}


CEllipsePlotter* CPlot::addEllipsePlotter(CEllipsePair* pep)
{
	assert(this);
	CEllipsePlotter* plot = new CEllipsePlotter(pep);
	plot_structures.push_back(plot);
	return plot;
}

CPointPlotter* CPlot::addPointPlotter(CPointStructure* pps)
{
	assert(this);
	CPointPlotter* plot = new CPointPlotter(pps);
	plot_structures.push_back(plot);
//	points.push_back(plot);
	return plot;
}

DXFPlotter* CPlot::addDxfPlotter(DXFObject* pdxf)
{
	assert(this);
	DXFPlotter* plot = new DXFPlotter(pdxf);
	plot_structures.push_back(plot);
	return plot;
}

DXFPlotter* CPlot::findDxfPlotterFor(DXFObject* pdxf)
{
	for (auto iter = plot_structures.begin(); iter != plot_structures.end(); ++iter) {
		if ((*iter)->getStructure() == pdxf) {
			return static_cast<DXFPlotter*>(*iter);
		}
	}
	return nullptr;
}

void CPlot::deletePlotStructure(CPlotStructure* toDelete)
{
	assert(this);
	assert(toDelete);
	
	plot_structures.remove(toDelete);
	delete toDelete;
}

void CPlot::deleteStructure(CStructure* toDelete)
{
	assert(this);
	assert(toDelete);

	PLOT_STRUCTURES::iterator psi = plot_structures.begin();
	while(psi != plot_structures.end())
	{
		CPlotStructure* pps = *psi;
		if( pps->getStructure() == toDelete)
		{
			PLOT_STRUCTURES::iterator here = psi;
			++psi;
			plot_structures.erase(here);
			delete pps;
		}
		else
			++psi;
	}

	structures.remove(toDelete);
}

CPlotStructure* CPlot::closestTo(NumericT x, NumericT y, NumericT& minDist) const
{
	assert(this);

	CPlotStructure* closest = 0;
	NumericT d2min = std::numeric_limits<NumericT>::max();

	for(PLOT_STRUCTURES::const_iterator psi = plot_structures.begin();
	psi != plot_structures.end();
	++psi)
	{
		CClosest measure(x,y);
		(*psi)->plot(&measure);

		NumericT d = measure.getMinDist();
		if(d < d2min)
		{
			d2min = d;
			closest = *psi;
		}
	}

	if(closest)
		minDist = NumericT(sqrt(d2min));
	else
		minDist = 0.0f;

	return closest;
}



void CPlot::serializeTo(CObjectSerializer& os)
{
	os.startSection("plot",this);

	os.startCollection("structures", (int)structures.size());
	for(STRUCTURES::iterator si = structures.begin();
	si != structures.end();
	++si)
	{
		(*si)->serializeTo(os);
	}
	os.endCollection();

	os.startCollection("plotStructures", (int) plot_structures.size());
	for(PLOT_STRUCTURES::iterator psi = plot_structures.begin();
	psi != plot_structures.end();
	++psi)
	{
		(*psi)->serializeTo(os);
	}
	os.endCollection();

	os.endSection();
}

void CPlot::serializeFrom(CObjectSerializer& os)
{
	assert(this);

	os.startReadSection("plot",this);


	int count = os.startReadCollection("structures");
	for(int i=0; i<count; ++i)
	{
		CStructure* pps = static_cast<CStructure*>(os.createSubtype());
		pps->serializeFrom(os);
		structures.push_back(pps);
	}
	os.endReadCollection();

	count = os.startReadCollection("plotStructures");
	for(int i=0; i<count; ++i)
	{
		CPlotStructure* pps = static_cast<CPlotStructure*>(os.createSubtype());
		pps->serializeFrom(os);
		plot_structures.push_back(pps);
	}
	os.endReadCollection();


	os.endReadSection();

}

// moveToEnd
// Allows the order of plot structures to be modified (important
// if they're being output to a CNC system) by moving the given
// plot structure to the end of the list.  Do this, in order,
// for all the plot structures & it is then sorted into the
// required order
void CPlot::moveToEnd(CPlotStructure* ps)
{
	assert(this);
	assert(ps);
	plot_structures.remove(ps);
	plot_structures.push_back(ps);
}
