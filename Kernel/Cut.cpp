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
#include <sstream>
#include "Cut.h"
#include "Structure.h"
#include "CutStructure.h"
#include "OutputDevice.h"
#include "ObjectSerializer.h"
#include "Bounds.h"
#include "BlockCorrectionOutputDevice.h"


Cut::Cut()
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


void Cut::cut(COutputDevice & pdev, const CutStructure::Context& context)
{
	// Get bounds - Z axis we're interested in.
	Bounds bounds;
	bounds.startPlot();
	for (auto iter = cut_structures.begin(); iter != cut_structures.end(); ++iter)	{
		CutStructure* pcs = *iter;
		pcs->cut(&bounds, context);
	}
	bounds.endPlot();

	// Create proxy output device to perform correction to edges of virtual block
	BlockCorrectionOutputDevice proxy(&pdev, bounds);

	try {
		proxy.startPlot();
		
		std::ostringstream os;
		os << "(--EFFECTIVE_SPAN=" << bounds.depth() << ")";
		proxy.passthrough(os.str().c_str());

		for (CutIterator iter = cut_structures.begin();
			iter != cut_structures.end();
			++iter)
		{
			CutStructure* pcs = *iter;
			proxy.startObject(pcs->getDescriptiveText().c_str());
			pcs->cut(&proxy, context);
			proxy.endObject(pcs->getDescriptiveText().c_str());
		}
		proxy.Flush();
		proxy.endPlot();
	}
	catch (COutputDevice::OutputException& /*ex*/) {
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

CutStructure * Cut::addCutter(CutStructure* pCutStructure)
{
	assert(this);
	assert(pCutStructure);
	cut_structures.push_back(pCutStructure);
	return pCutStructure;
}

// Inserts a new Cutter after an existing element.
// existing must exist in the list of cutters - if it doesn't newCut will still be
// inserted but at the end of the list.
CutStructure* Cut::insertAfter(const CutStructure* existing, CutStructure* newCut) {
	assert(this);
	assert(existing);
	assert(newCut);
	CutIterator next = cut_structures.end();
	for (CutIterator it = cut_structures.begin(); it != cut_structures.end(); ++it) {
		if (*it == existing) {
			next = it;
			++next;  // as insert will insert before this.
			break;
		}
	}
	if (next == cut_structures.end()) {
		cut_structures.push_back(newCut);
	}
	else {
		cut_structures.insert(next, newCut); // before this one.
	}

	return newCut;
}


// Provides the zero based index of the given CutStructure in the list of structures.
//
int Cut::indexOf(CutStructure* cs)
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

void Cut::moveToStart(CutStructure* cs)
{
	assert(this);
	assert(cs);
	cut_structures.remove(cs);
	cut_structures.push_front(cs);
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

void Cut::serializeTo(CObjectSerializer & os) const
{
	os.startSection("cut", this);
	// Removed feed rate as now stored in GCodeConfig
	os.startCollection("structures", (int)structures.size());
	for (StructureConstIterator si = structures.begin();
		si != structures.end();
		++si)
	{
		(*si)->serializeTo(os);
	}
	os.endCollection();

	os.startCollection("cutStructures", (int)cut_structures.size());
	for (CutConstIterator ci = cut_structures.begin();
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

	// These are no longer stored in the cut
	float feedRate;
	bool useFeedRate;

	os.startReadSection("cut", this);
	if (os.ifExists("toolOffset")) {
		double toolOffset;
		os.read("toolOffset", toolOffset);
		// and ignore!
	}


	if (os.ifExists("feedRate")) {
		os.read("feedRate", feedRate);
	}
	if (os.ifExists("useFeedRate")) {
		os.read("useFeedRate", useFeedRate);
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
