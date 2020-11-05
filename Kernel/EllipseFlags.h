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
*/// EllipseFlags.h: interface for the CEllipseFlags class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIPSEFLAGS_H__3CB6E433_CC06_11D6_9565_000000000000__INCLUDED_)
#define AFX_ELLIPSEFLAGS_H__3CB6E433_CC06_11D6_9565_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include "Kernel.h"

class CObjectSerializer;

class CEllipseFlags  
{
public:
	static const std::string TYPE;

	bool plot_upper;    // true if should plot/cut upper section
	bool plot_lower;    // true if should plot/cut lower section
	bool plot_markers;  // true if should plot axis markers (plot only)
	bool is_external;   // true if this is an external surface (so that cut can allow for meltback).

	CEllipseFlags();
	~CEllipseFlags();

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

};

#endif // !defined(AFX_ELLIPSEFLAGS_H__3CB6E433_CC06_11D6_9565_000000000000__INCLUDED_)
