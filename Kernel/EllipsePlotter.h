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
*/// EllipsePlotter.h: interface for the CEllipsePlotter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELLIPSEPLOTTER_H__D83773A3_BA49_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_ELLIPSEPLOTTER_H__D83773A3_BA49_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Kernel.h"
#include "PlotStructure.h"

class CEllipse;
class CEllipsePair;
class COutputDevice;

class CEllipsePlotter : public CPlotStructure
{
public:
	CEllipsePlotter(CEllipsePair* pep);
	explicit CEllipsePlotter();		// for serialization
	virtual ~CEllipsePlotter();

	CEllipsePair* getEllipses() {return ellipses;}
	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

	virtual void plot(COutputDevice* pdev);
	virtual std::string getDescriptiveText() const;

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:
	CEllipsePair* ellipses;

};

#endif // !defined(AFX_ELLIPSEPLOTTER_H__D83773A3_BA49_11D6_AF75_002018BA082E__INCLUDED_)
