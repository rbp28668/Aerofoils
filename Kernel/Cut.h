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

#pragma once
#include <list>

class COutputDevice;
class CWing;
class CEllipsePair;
class CPointStructure;
class GCodeSnippet;
class DXFObject;
class CutStructure;
class CStructure;
class CPathCutter;
class EllipseCutter;
class PointCutter;
class GCodeSnippetCutter;
class DXFObjectCutter;
class HomeCutter;
class CObjectSerializer;

// Manages structures and cuts using those structures.  Cut is for CNC
// as CPlot is for printing on paper.
class Cut
{
	typedef std::list<CStructure*> StructureList;
	typedef std::list<CutStructure*> CutList;

	StructureList structures;
	CutList cut_structures;

	// Cutter parameters
	double toolOffset;
	double feedRate;

public:

	typedef StructureList::iterator StructureIterator;
	typedef CutList::iterator CutIterator;

	Cut();
	~Cut();

	inline double getToolOffset() const { return toolOffset; }
	inline void setToolOffset(double offset) { toolOffset = offset; }
	inline double getFeedRate() const { return feedRate; }
	inline void setFeedRate(double rate) { feedRate = rate; }

	void cut(COutputDevice& pdev);

	// Add structure
	void addStructure(CStructure* pStructure);

	// Add cutters
	CPathCutter* addPathCutter(CWing* pWing);
	EllipseCutter* addEllipseCutter(CEllipsePair* pep);
	PointCutter* addPointCutter(CPointStructure* pps);
	DXFObjectCutter* addDXFObjectCutter(DXFObject* pdxf);
	GCodeSnippetCutter* addGCodeSnippetCutter(GCodeSnippet* pgcode);
	HomeCutter* addHomePosition();

	// plotter manipulation
	void moveUp(CutStructure* cs);
	void moveDown(CutStructure* cs);
	int indexOf(CutStructure* cs);
	void moveToEnd(CutStructure* cs);
	void deleteCut(CutStructure* toDelete);
	void deleteStructure(CStructure* toDelete);

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

	int getStructureCount() const { return (int)structures.size(); }

	StructureIterator beginStructures() { return structures.begin(); }
	StructureIterator endStructures() { return structures.end(); }
	CutIterator beginCutStructures() { return cut_structures.begin(); }
	CutIterator endCutStructures() { return cut_structures.end(); }
};
