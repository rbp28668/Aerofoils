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
*/// Structure.h: interface for the CStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRUCTURE_H__B71FC393_CC68_11D6_9566_00047660C89A__INCLUDED_)
#define AFX_STRUCTURE_H__B71FC393_CC68_11D6_9566_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "Kernel.h"

class CObjectSerializer;

class CStructure  
{
public:
	CStructure();
	virtual ~CStructure();
	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	virtual std::string getDescriptiveText() const = 0;
	virtual std::string getType() const = 0;

	NumericT getSpan() const {return span;}
	void setSpan(NumericT s) {span = s;}

	NumericT span;     /* span of this wing (root to tip) or depth of object across the cutter */

};

#endif // !defined(AFX_STRUCTURE_H__B71FC393_CC68_11D6_9566_00047660C89A__INCLUDED_)
