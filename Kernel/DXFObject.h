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
#include <memory>
#include "Structure.h"
#include "DXFParser.h"

class CutStructure;
class StructureOutput;

// A structure object that holds the contents of a DXF file as a collection
// of DXFItem.  Slightly unusual in that the corresponding DXFObjectCutter
// delegates the actual cutting to this class.  This is so each individual
// DXFItem can know how to cut and not need a large set of corresponding
// cutter objects.
class DXFObject :
	public CStructure, DXFItemReceiver
{
	std::list<std::unique_ptr<DXFItem>> items;
	typedef std::shared_ptr<DXFItem> SharedItemT;
	std::list<SharedItemT> sharedItems;

public:
	static const std::string TYPE;

	DXFObject();
	DXFObject(const char* pszPath);
	~DXFObject();

	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	virtual std::string getDescriptiveText() const;
	virtual std::string getType() const;

	// DXFItemReceiver 
	virtual void add(DXFItem* item);
	virtual void addSharedItem(DXFItem* item);

	void cutAll(StructureOutput* pOutput, COutputDevice* pdev);

	size_t itemCount() const { return items.size(); }
	DXFObject* extractFirstItem();
};

