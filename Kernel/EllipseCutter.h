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
#pragma once
#include "CutStructure.h"

class CEllipsePair;

// EllipseCutter is the cutter for a CEllipsePair
class EllipseCutter :
	public CutStructure
{
	CEllipsePair* pEllipses;

public:
	static const std::string TYPE;

	explicit EllipseCutter();
	EllipseCutter(CEllipsePair * pep);
	~EllipseCutter();

	virtual void cut(COutputDevice *pdev, double toolOffset);

	virtual std::string getDescriptiveText() const;
	virtual std::string getType() const;
	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

	virtual void serializeTo(CObjectSerializer & os);
	virtual void serializeFrom(CObjectSerializer & os);
};

