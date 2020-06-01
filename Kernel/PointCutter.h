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
#include "Kernel.h"
#include "CutStructure.h"

class CPointStructure;

// PointCutter cuts to a given point (CPointStructure) with usual
// translation provided.  Hence points can be cut to anywhere.
class PointCutter :
	public CutStructure
{
	CPointStructure* pStructure;
	bool fast;

public:
	static const std::string TYPE;

	explicit PointCutter();
	PointCutter(CPointStructure* pps);
	~PointCutter();

	bool isFast() const;
	void setFast(bool fast);

	virtual void cut(COutputDevice *pdev, const CutStructure::Context& context);
	virtual std::string getDescriptiveText() const;
	virtual std::string getType() const;
	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

	virtual	void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

};

