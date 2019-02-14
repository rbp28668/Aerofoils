/* Aerofoil
Aerofoil plotting and CNC cutter driver
Kernel / core algorithms
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
*/// PointPlotter.cpp: implementation of the CPointPlotter class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <sstream>
#include "PointPlotter.h"
#include "PointStructure.h"
#include "ObjectSerializer.h"

using namespace std;

static CObjectFactory<CPointPlotter> factory("pointPlotter");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointPlotter::CPointPlotter(CPointStructure* pps)
:structure(pps)
{

}

CPointPlotter::CPointPlotter()
:structure(0)
{

}

CPointPlotter::~CPointPlotter()
{

}

void CPointPlotter::plot(COutputDevice *pdev)
{
	assert(this);
	assert(pdev);
	
	setDevice(pdev);

	if(pdev->isCNC())
		interp_move_to(structure->getRoot(), structure->getTip());
	else
	{
		const float size = 1.0f;

		PointT r = structure->getRoot();
		PointT t = structure->getTip();

		r.fx -= size;
		t.fx -= size;
		interp_move_to(r,t); // left

		r.fx += size;
		r.fy -= size;
		t.fx += size;
		t.fy -= size;
		interp_line_to(r,t); // bottom

		r.fy += 2*size;
		t.fy += 2*size;
		interp_line_to(r,t); // top

		r.fx += size;
		r.fy -= size;
		t.fx += size;
		t.fy -= size;
		interp_line_to(r,t); // right

		r.fx -= 2*size;
		t.fx -= 2*size;
		interp_line_to(r,t); // left

	}

}

string CPointPlotter::getDescriptiveText() const
{
	stringstream ss;

	ss << "Point at " << getSectionPos() << ends;
	return ss.str();
}

CStructure* CPointPlotter::getStructure()
{
	assert(this);
	return structure;
}

const CStructure* CPointPlotter::getStructure() const
{
	assert(this);
	return structure;
}

void CPointPlotter::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("pointPlotter",this);
	CPlotStructure::serializeTo(os);
	os.writeReference("structure",structure);
	os.endSection();
}

void CPointPlotter::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("pointPlotter",this);
	CPlotStructure::serializeFrom(os);
	structure = static_cast<CPointStructure*>(os.readReference("structure"));
	os.endReadSection();
}

