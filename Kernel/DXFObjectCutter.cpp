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
#include "DXFObjectCutter.h"
#include "DXFObject.h"
#include "ObjectSerializer.h"

const std::string DXFObjectCutter::TYPE("dxfObjectCutter");
static CObjectFactory<DXFObjectCutter> factory(DXFObjectCutter::TYPE.c_str());

DXFObjectCutter::DXFObjectCutter()
	: pDxfObject(0)
{
	assert(this);
}


DXFObjectCutter::DXFObjectCutter(DXFObject* pdxf)
	: pDxfObject(pdxf)
{
	assert(this);
	assert(pdxf);
}


DXFObjectCutter::~DXFObjectCutter()
{
}

void DXFObjectCutter::cut(COutputDevice * pdev, const CutStructure::Context& context)
{
	pDxfObject->cutAll(this, pdev);
}

std::string DXFObjectCutter::getDescriptiveText() const
{
	return std::string("DXF Object");
}

std::string DXFObjectCutter::getType() const
{
	return TYPE;
}

CStructure * DXFObjectCutter::getStructure()
{
	return pDxfObject;
}

const CStructure * DXFObjectCutter::getStructure() const
{
	return pDxfObject;
}

void DXFObjectCutter::serializeTo(CObjectSerializer & os)
{
	os.startSection(TYPE.c_str(), this);
	CutStructure::serializeTo(os);
	os.writeReference("dxfObjectRef", pDxfObject);
	os.endSection();
}

void DXFObjectCutter::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection(TYPE.c_str(), this);
	CutStructure::serializeFrom(os);
	pDxfObject = static_cast<DXFObject*>(os.readReference("dxfObjectRef"));
	os.endReadSection();

}
