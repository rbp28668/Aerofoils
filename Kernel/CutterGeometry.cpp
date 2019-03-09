#include "CutterGeometry.h"
#include "ObjectSerializer.h"


CutterGeometry::CutterGeometry()
	: width(1000)
	, blockLeft(100)
	, blockRight(900)
	, xTravel(500)
	, yTravel(200)
{
}


CutterGeometry::~CutterGeometry()
{
}

void CutterGeometry::serializeTo(CObjectSerializer & os)
{
	os.startSection("cutterGeometry", this);
	os.write("width", width);
	os.write("blockLeft", blockLeft);
	os.write("blockRight", blockRight);
	os.write("xTravel", xTravel);
	os.write("yTravel", yTravel);
	os.endSection();
}

void CutterGeometry::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection("cutterGeometry", this);
	os.read("width", width);
	os.read("blockLeft", blockLeft);
	os.read("blockRight", blockRight);
	os.read("xTravel", xTravel);
	os.read("yTravel", yTravel);
	os.endReadSection();

}
