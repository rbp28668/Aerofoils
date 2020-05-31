#pragma once

#include "Kernel.h"
#include "PlotStructure.h"

class DXFObject;

class DXFPlotter : public CPlotStructure
{
public:
	DXFPlotter(DXFObject* pDxf);
	explicit DXFPlotter();		// for serialization
	virtual ~DXFPlotter();

	DXFObject* getDxfObject() { return dxfObject; }
	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

	virtual void plot(COutputDevice* pdev);
	virtual std::string getDescriptiveText() const;

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:
	DXFObject* dxfObject;

};

