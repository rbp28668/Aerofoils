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
#include "CutStructure.h"
#include "ObjectSerializer.h"
#include "OutputDevice.h"
#include "Bounds.h"


void CutStructure::move(COutputDevice * pdev, const PointT & root, const PointT & tip)
{
	PointT r = root;
	PointT t = tip;
	transform(r, t);
	pdev->MoveTo(root_stream, r);
	pdev->MoveTo(tip_stream, t);
}

void CutStructure::line(COutputDevice * pdev, const PointT & root, const PointT & tip)
{
	PointT r = root;
	PointT t = tip;
	transform(r, t);
	pdev->LineTo(root_stream, r);
	pdev->LineTo(tip_stream, t);
}

void CutStructure::transform(PointT& r, PointT& t) const
{
	if (rotateRadians != 0 && (r.fz != 0 || t.fz != 0) ) {
		double rx = r.fx - rotateOrigin.fx;
		double rz = r.fz - rotateOrigin.fz;
		double tx = t.fx -rotateOrigin.fx;
		double tz = t.fz - rotateOrigin.fz;

		// Rotate on Z,X around Y.
		r.fz = rotateOrigin.fz + rz* rotateCos + rx * rotateSin;
		r.fx = rotateOrigin.fx - rz * rotateSin + rx * rotateCos;
		t.fz = rotateOrigin.fz + tz * rotateCos + tx * rotateSin;
		t.fx = rotateOrigin.fx - tz * rotateSin + tx * rotateCos;
	}

	// For cutting cores use of bounds has become problematic for inverting as now we can split cores
	// In these circumstances bounds makes alignment hard and worse - bounds may include cutouts etc.
	// So revert to a simple inversion of y before offsets are added.
	//if (invert && pBounds) {
	//	r.fy = pBounds->height() - r.fy;
	//	t.fy = pBounds->height() - t.fy;
	//}
	if (invert) {
		r.fy = -r.fy;
		t.fy = -t.fy;
	}

	// It's fine for X though.
	if (reflect && pBounds) {
		r.fx = pBounds->width() - r.fx;
		t.fx = pBounds->width() - t.fx;
	}

	r += rootOffsets;
	t += tipOffsets;

	
}

void CutStructure::updateBounds()
{
	Bounds* pb;
	if (pBounds) {
		pb = pBounds;
		pb->reset();
		pBounds = 0;
	} else {
		pb = new Bounds(); 
	}
	CutStructure::Context context;
	context.toolOffset = 0;
	cut(pb, context);
	pBounds = pb;
}


CutStructure::CutStructure()
	: invert(false)
	, reflect(false)
	, rootIsOnLeft(true)
	, root_stream(0)
	, tip_stream(1)
	, pBounds(0)
	, rotateRadians(0)
	, rotateSin(0)
	, rotateCos(1)
{
}

CutStructure::CutStructure(const CutStructure& source)
	: rootOffsets(source.rootOffsets)
	, tipOffsets(source.tipOffsets)
	, invert(source.invert)
	, reflect(source.reflect)
	, rootIsOnLeft(source.rootIsOnLeft)
	, root_stream(source.root_stream)
	, tip_stream(source.tip_stream)
	, pBounds(0) //  recalculate bounds for copy.
	, rotateOrigin(source.rotateOrigin)
	, rotateRadians(source.rotateRadians)
	, rotateSin(source.rotateSin)
	, rotateCos(source.rotateCos)

{
}


CutStructure::~CutStructure()
{
	if (pBounds) {
		delete pBounds;
	}
}


void CutStructure::setOffsets(const PointT & root, const PointT & tip)
{
	assert(this);
	rootOffsets = root;
	tipOffsets = tip;
}

const PointT CutStructure::getRootOffsets()
{
	return rootOffsets;
}

const PointT CutStructure::getTipOffsets()
{
	return tipOffsets;
}

void CutStructure::setRootSide(bool isLeft)
{
	rootIsOnLeft = isLeft;
	if (rootIsOnLeft)
	{
		root_stream = 0;
		tip_stream = 1;
	}
	else
	{
		root_stream = 1;
		tip_stream = 0;
	}

}

void CutStructure::setInvert(bool inv)
{
	invert = inv;
	if (invert && pBounds == 0) {
		updateBounds();
	}
}

void CutStructure::setReflect(bool ref)
{
	reflect = ref;
	if (reflect && pBounds == 0) {
		updateBounds();
	}
}

void CutStructure::setRotation(PointT origin, double angleRadians)
{
	rotateOrigin = origin;
	rotateRadians = angleRadians;
	rotateSin = sin(angleRadians);
	rotateCos = cos(angleRadians);
}

void CutStructure::serializeTo(CObjectSerializer & os)
{
	assert(this);
	os.startSection("cutStructure", this);
	os.write("rootX", rootOffsets.fx);
	os.write("rootY", rootOffsets.fy);
	os.write("rootZ", rootOffsets.fz);
	os.write("tipX", tipOffsets.fx);
	os.write("tipY", tipOffsets.fy);
	os.write("tipZ", tipOffsets.fz);
	os.write("rootLeft", rootIsOnLeft);
	os.write("invert", invert);
	os.write("reflect", reflect);
	os.write("rotateRadians", rotateRadians);
	os.write("rotateOriginX", rotateOrigin.fx);
	os.write("rotateOriginZ", rotateOrigin.fz);
	os.endSection();

}

void CutStructure::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection("cutStructure", this);
	os.read("rootX", rootOffsets.fx);
	os.read("rootY", rootOffsets.fy);
	if (os.ifExists("rootZ")) {
		os.read("rootZ", rootOffsets.fz);
	}
	else {
		rootOffsets.fz = 0;
	}
	os.read("tipX", tipOffsets.fx);
	os.read("tipY", tipOffsets.fy);
	if (os.ifExists("tipZ")) {
		os.read("tipZ", tipOffsets.fz);
	}
	else {
		tipOffsets.fz = 0;
	}
	os.read("rootLeft", rootIsOnLeft);
	setRootSide(rootIsOnLeft);
	os.read("invert", invert);   // FIXME - derived state won't be set
	os.read("reflect", reflect); // FIXME - derived state won't be set

	PointT rotateOrigin(0,0,0);
	double rotateRadians(0);
	if (os.ifExists("rotateRadians")) {
		os.read("rotateRadians", rotateRadians);
		os.read("rotateOriginX", rotateOrigin.fx);
		os.read("rotateOriginZ", rotateOrigin.fz);
	}
	setRotation(rotateOrigin, rotateRadians); // ensure derived values set.
	os.endReadSection();

}

void CutStructure::Context::serializeTo(CObjectSerializer& os) const
{
	assert(this);
	os.startSection("CutContext", this);
	os.write("toolOffset", toolOffset);
	os.write("optimiseOutput", optimiseOutput);
	os.write("tolerance", tolerance);
	os.endSection();
}

void CutStructure::Context::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection("CutContext", this);
	os.write("toolOffset", toolOffset);
	os.write("optimiseOutput", optimiseOutput);
	os.write("tolerance", tolerance);
	os.endReadSection();
}
