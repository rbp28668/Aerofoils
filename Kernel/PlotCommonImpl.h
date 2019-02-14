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
*/// PlotCommonImpl.h: interface for the CPlotCommonImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOTCOMMONIMPL_H__EF095243_B4C9_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_PLOTCOMMONIMPL_H__EF095243_B4C9_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef min
#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a>b)?a:b)
#endif

#include "PlotStructure.h"

class CAerofoil;
class COutputDevice;
class CWing;

class CPlotCommonImpl: public CPlotStructure
{

public:
	float getSpan() const;
	CWing* getWing() {return wing;}
	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

protected:

	CPlotCommonImpl(CWing* pWing);
	explicit CPlotCommonImpl();
	
	virtual ~CPlotCommonImpl();
	void find_core_te(const CAerofoil& foil,float skin,float *nu0,float *nu1) const;

	const static float CLOSE;  /* limit of resolution in mm per step */
	const static int MAX_ITER;  /* max number of runs thro' the iteration */
	const static int FORWARD_ITER;  /* 1 part in 16000 */
	const static float WEENY; /* very small...*/

	CWing* wing;	// the wing this is plotting.
};

#endif // !defined(AFX_PLOTCOMMONIMPL_H__EF095243_B4C9_11D6_AF75_002018BA082E__INCLUDED_)
