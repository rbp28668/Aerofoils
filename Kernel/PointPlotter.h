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
*/// PointPlotter.h: interface for the CPointPlotter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PointPlotter_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_)
#define AFX_PointPlotter_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlotStructure.h"

class CPointStructure;

class CPointPlotter : public CPlotStructure
{
public:
	explicit CPointPlotter();
	CPointPlotter(CPointStructure* _struct);
	virtual ~CPointPlotter();

	virtual void plot(COutputDevice *pdev);
	virtual std::string getDescriptiveText() const;

	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

	virtual	void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	CPointStructure* getPointStructure() {return structure;}

private:
	CPointStructure* structure;
};

#endif // !defined(AFX_PointPlotter_H__462C60A6_C8A0_11D6_955C_000000000000__INCLUDED_)
