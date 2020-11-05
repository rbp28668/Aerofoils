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
*/// EllipseFlags.cpp: implementation of the CEllipseFlags class.
//
//////////////////////////////////////////////////////////////////////

#include <string>
#include "EllipseFlags.h"
#include "ObjectSerializer.h"

const std::string CEllipseFlags::TYPE("ellipseFlags");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipseFlags::CEllipseFlags()
: plot_upper(true)
, plot_lower(true)
, plot_markers(true)
, is_external(true)
{

}

CEllipseFlags::~CEllipseFlags()
{

}

void CEllipseFlags::serializeTo(CObjectSerializer& os)
{
	os.startSection(TYPE.c_str(), this);
	os.write("upper", plot_upper);
	os.write("lower", plot_lower);
	os.write("markers", plot_markers);
	os.write("external", is_external);
	os.endSection();
}

void CEllipseFlags::serializeFrom(CObjectSerializer& os)
{
	os.startReadSection(TYPE.c_str(), this);
	os.read("upper", plot_upper);
	os.read("lower", plot_lower);
	os.read("markers", plot_markers);
	os.read("external", is_external);
	os.endReadSection();
}
