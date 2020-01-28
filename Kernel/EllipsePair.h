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
*/// EllipsePair.h: interface for the CEllipsePair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIPSEPAIR_H__A14EEE01_BAC0_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_ELLIPSEPAIR_H__A14EEE01_BAC0_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ellipse.h"
#include "EllipseFlags.h"
#include "Structure.h"

class CObjectSerializer;

class CEllipsePair : public CStructure
{
public:
	static const std::string TYPE;

	CEllipsePair();
	explicit CEllipsePair(const CEllipsePair& pair);
	virtual ~CEllipsePair();

	virtual std::string getDescriptiveText() const;
	virtual std::string getType() const;

	const CEllipse* getFirst() const {return &first;}
	CEllipse* getFirst() {return &first;}
	const CEllipse* getSecond() const {return &second;}
	CEllipse* getSecond() {return &second;}

	CEllipseFlags* getPlotFlags() {return &flags;}
	const CEllipseFlags* getPlotFlags() const {return &flags;}

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:
	CEllipse first;
	CEllipse second;

	CEllipseFlags flags;
};

#endif // !defined(AFX_ELLIPSEPAIR_H__A14EEE01_BAC0_11D6_AF75_002018BA082E__INCLUDED_)
