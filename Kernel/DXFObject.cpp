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

#include <fstream>
#include <assert.h>
#include "DXFObject.h"
#include "DXFParser.h"
#include "ObjectSerializer.h"

const std::string DXFObject::TYPE("dxfObject");
static CObjectFactory<DXFObject> factory(DXFObject::TYPE.c_str());


DXFObject::DXFObject()
{
}

DXFObject::DXFObject(const char * pszPath)
{
	DXFParser parser;
	std::ifstream ifs(pszPath);
	parser.readDxf(ifs,this);
	ifs.close();
}

DXFObject::~DXFObject()
{
	for (auto iter = items.begin(); iter != items.end(); ++iter) {
		delete (*iter);
		*iter = 0;
	}
	items.clear();

	//for (auto iter = sharedItems.begin(); iter != sharedItems.end(); ++iter) {
	//	iter->reset();
	//}
	sharedItems.clear();  // Should destroy the shared ptrs.

}

void DXFObject::serializeTo(CObjectSerializer & os)
{
	os.startSection(TYPE.c_str(), this);

	os.startCollection("dxfShared", (int)sharedItems.size());
	for (auto iter = sharedItems.begin(); iter != sharedItems.end(); ++iter) {
		(*iter)->serializeTo(os);
	}
	os.endCollection();

	os.startCollection("dxfItems", (int)items.size());
	for (auto iter = items.begin(); iter != items.end(); ++iter) {
		(*iter)->serializeTo(os);
	}
	os.endCollection();

	os.endSection();

}

void DXFObject::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection(TYPE.c_str(), this);

	int count = os.startReadCollection("dxfShared");
	for (int i = 0; i < count; ++i)
	{
		DXFItem* item = static_cast<DXFItem*>(os.createSubtype());
		item->serializeFrom(os);
		addSharedItem(item);
	}
	os.endReadCollection();

	count = os.startReadCollection("dxfItems");
	for (int i = 0; i<count; ++i)
	{
		DXFItem* item = static_cast<DXFItem*>(os.createSubtype());
		item->serializeFrom(os);
		add(item);
	}
	os.endReadCollection();

	os.endReadSection();

}

std::string DXFObject::getDescriptiveText() const
{
	return std::string("DXF Object");
}

std::string DXFObject::getType() const
{
	return TYPE;
}

void DXFObject::add(DXFItem * item)
{
	assert(this);
	assert(item);
	items.push_back(item);
}

void DXFObject::addSharedItem(DXFItem* item)
{
	sharedItems.push_back(std::shared_ptr<DXFItem>(item));

}

void DXFObject::cutAll(StructureOutput * pOutput, COutputDevice * pdev)
{
	for (auto iter = items.begin(); iter != items.end(); ++iter) {
		(*iter)->cut(pOutput, pdev, DXFParser::noOpTransform());
	}
}

DXFObject* DXFObject::extractFirstItem()
{
	assert(this);
	assert(items.size() > 1);

	DXFItem* first = items.front();
	items.pop_front();

	DXFObject* copy = new DXFObject();
	copy->span = span;
	copy->add(first);
	copy->sharedItems = sharedItems;

	return copy;
}
