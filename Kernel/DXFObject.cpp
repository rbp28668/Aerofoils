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


DXFObject::DXFObject(const char * pszPath)
{
	DXFParser parser;
	std::ifstream ifs(pszPath);
	parser.readDxf(ifs,this);
	ifs.close();
}

DXFObject::~DXFObject()
{
	for (std::vector<DXFItem*>::iterator iter = items.begin();
		iter != items.end();
		++iter) {
		delete (*iter);
		*iter = 0;
	}
	items.clear();

}

void DXFObject::serializeTo(CObjectSerializer & os)
{
}

void DXFObject::serializeFrom(CObjectSerializer & os)
{
}

std::string DXFObject::getDescriptiveText() const
{
	return std::string("DXF Object");
}

void DXFObject::add(DXFItem * item)
{
	assert(this);
	assert(item);
	items.push_back(item);
}

void DXFObject::cutAll(CutStructure * pCut, COutputDevice * pdev)
{
	for (std::vector<DXFItem*>::iterator iter = items.begin();
		iter != items.end();
		++iter) {
		(*iter)->cut(pCut, pdev);
	}
}
