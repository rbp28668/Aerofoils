#include <assert.h>
#include "DXFPlotter.h"
#include "DXFObject.h"
#include "ObjectSerializer.h"

static CObjectFactory<DXFPlotter> factory("dxfObjectPlotter");

CStructure* DXFPlotter::getStructure()
{
	return dxfObject;
}

const CStructure* DXFPlotter::getStructure() const
{
	return dxfObject;
}

void DXFPlotter::plot(COutputDevice* pdev)
{
	dxfObject->cutAll(this, pdev);
}

std::string DXFPlotter::getDescriptiveText() const
{
	return dxfObject->getDescriptiveText();
}

void DXFPlotter::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("dxfObjectPlotter", this);
	CPlotStructure::serializeTo(os);
	os.writeReference("dxfObject", dxfObject);
	os.endSection();
}

void DXFPlotter::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("dxfObjectPlotter", this);
	CPlotStructure::serializeFrom(os);
	dxfObject = static_cast<DXFObject*>(os.readReference("dxfObject"));
	os.endReadSection();
}

DXFPlotter::DXFPlotter(DXFObject* pDxf)
	: dxfObject(pDxf)
{
}

DXFPlotter::DXFPlotter()
	: dxfObject(0)
{
}

DXFPlotter::~DXFPlotter()
{
}
