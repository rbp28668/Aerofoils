/* Aerofoil
Aerofoil plotting and CNC cutter driver
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
*/


#include <assert.h>
#include "Cut.h"
#include "Structure.h"
#include "CutStructure.h"
#include "OutputDevice.h"
#include "ObjectSerializer.h"
#include "Cutpath.h"
#include "EllipseCutter.h"
#include "PointCutter.h"
#include "DXFObjectCutter.h"
#include "GCodeSnippetCutter.h"
#include "HomeCutter.h"


Cut::Cut()
	: toolOffset(0.5)
	, feedRate(1.0)
{
}


Cut::~Cut()
{
	for (CutIterator ci = cut_structures.begin();
		ci != cut_structures.end();
		++ci)
	{
		delete (*ci);
	}
	cut_structures.clear();


	for (StructureIterator si = structures.begin();
		si != structures.end();
		++si)
	{
		delete (*si);
	}
	structures.clear();
}


void Cut::cut(COutputDevice & pdev)
{
	try {
		pdev.startPlot();
		for (CutIterator iter = cut_structures.begin();
			iter != cut_structures.end();
			++iter)
		{
			CutStructure* pcs = *iter;
			pdev.startObject(pcs->getDescriptiveText().c_str());
			pcs->cut(&pdev, toolOffset);
			pdev.endObject(pcs->getDescriptiveText().c_str());
		}
		pdev.Flush();
		pdev.endPlot();
	}
	catch (COutputDevice::OutputException& ex) {
		// NOP for the time being.
		throw; // Just rethrow
	}

}

void Cut::addStructure(CStructure * pStructure)
{
	assert(this);
	assert(pStructure);
	structures.push_back(pStructure);
}

CPathCutter * Cut::addPathCutter(CWing * pWing)
{
	assert(this);
	assert(pWing);
	CPathCutter* ppc = new CPathCutter(pWing);
	cut_structures.push_back(ppc);
	return ppc;
}

EllipseCutter * Cut::addEllipseCutter(CEllipsePair * pep)
{
	assert(this);
	assert(pep);
	EllipseCutter* pec = new EllipseCutter(pep);
	cut_structures.push_back(pec);
	return pec;
}

PointCutter * Cut::addPointCutter(CPointStructure * pps)
{
	assert(this);
	assert(pps);
	PointCutter* ppc = new PointCutter(pps);
	cut_structures.push_back(ppc);
	return ppc;
}

DXFObjectCutter * Cut::addDXFObjectCutter(DXFObject* pdxf)
{
	assert(this);
	assert(pdxf);
	DXFObjectCutter* pcut = new DXFObjectCutter(pdxf);
	cut_structures.push_back(pcut);
	return pcut;
}

GCodeSnippetCutter * Cut::addGCodeSnippetCutter(GCodeSnippet* pgcode)
{
	assert(this);
	assert(pgcode);
	GCodeSnippetCutter* pcut = new GCodeSnippetCutter(pgcode);
	cut_structures.push_back(pcut);
	return pcut;
}

HomeCutter * Cut::addHomePosition()
{
	assert(this);
	HomeCutter* phc = new HomeCutter();
	cut_structures.push_back(phc);
	return phc;
}

void Cut::moveUp(CutStructure * cs)
{
	CutIterator it = cut_structures.begin(); 
	if (*it == cs) {
		return; // as already at the front.
	}
	++it; // point at second (or possibly off the end)
	while(it != cut_structures.end()) {
		if (*it == cs) {
			// We've found it now....
			CutIterator previous = it;
			--previous;
			cut_structures.erase(it);
			cut_structures.insert(previous, cs);
			break;
		}
		++it;
	}
}

void Cut::moveDown(CutStructure * cs)
{
	for (CutIterator it = cut_structures.begin(); it != cut_structures.end(); ++it) {
		if (*it == cs) {
			CutIterator next = it;
			++next;

			if(next == cut_structures.end()){
				break; // as element already at the end.
			}

			cut_structures.erase(it);
			++next;  // as insert before this
			if (next == cut_structures.end()) {
				cut_structures.push_back(cs);
			} else {
				cut_structures.insert(next, cs); // before this one.
			}
			break;
		}
	}
}

int Cut::indexOf(CutStructure * cs)
{
	// simple linear search.
	int index = -1;
	int idx = 0;
	for (CutIterator it = cut_structures.begin(); it != cut_structures.end(); ++it) {
		if (*it == cs) {
			index = idx;
			break;
		}
		++idx;
	}
	return index;
}

void Cut::moveToEnd(CutStructure * cs)
{
	assert(this);
	assert(cs);
	cut_structures.remove(cs);
	cut_structures.push_back(cs);

}

void Cut::deleteCut(CutStructure * toDelete)
{
	assert(this);
	assert(toDelete);

	cut_structures.remove(toDelete);
	delete toDelete;
}

void Cut::deleteStructure(CStructure * toDelete)
{
	assert(this);
	assert(toDelete);

	CutIterator ci = cut_structures.begin();
	while (ci != cut_structures.end())
	{
		CutStructure* pcs = *ci;
		if (pcs->getStructure() == toDelete)
		{
			CutIterator here = ci;
			++ci;
			cut_structures.erase(here);
			delete pcs;
		}
		else
			++ci;
	}

	structures.remove(toDelete);
	delete toDelete;
}

void Cut::serializeTo(CObjectSerializer & os)
{
	os.startSection("cut", this);
	os.write("toolOffset", toolOffset);
	os.write("feedRate", feedRate);
	os.startCollection("structures", (int)structures.size());
	for (StructureIterator si = structures.begin();
		si != structures.end();
		++si)
	{
		(*si)->serializeTo(os);
	}
	os.endCollection();

	os.startCollection("cutStructures", (int)cut_structures.size());
	for (CutIterator ci = cut_structures.begin();
		ci != cut_structures.end();
		++ci)
	{
		(*ci)->serializeTo(os);
	}
	os.endCollection();

	os.endSection();

}

void Cut::serializeFrom(CObjectSerializer & os)
{
	assert(this);

	os.startReadSection("cut", this);
	os.read("toolOffset", toolOffset);
	if (os.ifExists("feedRate")) {
		os.read("feedRate", feedRate);
	}
	int count = os.startReadCollection("structures");
	for (int i = 0; i<count; ++i)
	{
		CStructure* pps = static_cast<CStructure*>(os.createSubtype());
		pps->serializeFrom(os);
		structures.push_back(pps);
	}
	os.endReadCollection();

	count = os.startReadCollection("cutStructures");
	for (int i = 0; i<count; ++i)
	{
		CutStructure* pps = static_cast<CutStructure*>(os.createSubtype());
		pps->serializeFrom(os);
		cut_structures.push_back(pps);
	}
	os.endReadCollection();

	os.endReadSection();
}
