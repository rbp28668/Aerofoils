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

void CutStructure::transform(PointT& r, PointT& t)
{
	if (invert && pBounds) {
		r.fy = pBounds->height() - r.fy;
		t.fy = pBounds->height() - t.fy;
	}
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
	cut(pb, 0.0);
	pBounds = pb;
}


CutStructure::CutStructure()
	: invert(false)
	, reflect(false)
	, rootIsOnLeft(true)
	, root_stream(0)
	, tip_stream(1)
	, pBounds(0)
{
}


CutStructure::~CutStructure()
{
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

void CutStructure::serializeTo(CObjectSerializer & os)
{
	assert(this);
	os.startSection("cutStructure", this);
	os.write("rootX", rootOffsets.fx);
	os.write("rootY", rootOffsets.fy);
	os.write("tipX", tipOffsets.fx);
	os.write("tipY", tipOffsets.fy);
	os.write("rootLeft", rootIsOnLeft);
	os.write("invert", invert);
	os.write("reflect", reflect);
	os.endSection();

}

void CutStructure::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection("cutStructure", this);
	os.read("rootX", rootOffsets.fx);
	os.read("rootY", rootOffsets.fy);
	os.read("tipX", tipOffsets.fx);
	os.read("tipY", tipOffsets.fy);
	os.read("rootLeft", rootIsOnLeft);
	setRootSide(rootIsOnLeft);
	os.read("invert", invert);   // FIXME - derived state won't be set
	os.read("reflect", reflect); // FIXME - derived state won't be set
	os.endReadSection();

}
