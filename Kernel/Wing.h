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
*/// Wing.h: interface for the CWing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WING_H__BBA1B6BB_B27A_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_WING_H__BBA1B6BB_B27A_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include "aerofoil.h"
#include "transform.h"
#include "spar.h"
#include "PlotFlags.h"
#include "Structure.h"

class CWing : public CStructure
{
public:
	CWing(const char* rootSection, float rootThickness, const char* tipSection, float tipThickness);
	explicit CWing(const CWing& rhs);
	explicit CWing();

	virtual ~CWing();

	CWing& operator = (const CWing& rhs);

	virtual std::string getDescriptiveText() const;

	const CAerofoil* getRoot() const {return &root;}
	CAerofoil* getRoot() {return &root;}
	const CAerofoil* getTip() const {return &tip;}
	CAerofoil* getTip()  {return &tip;}

	const CTransform* getRootTransform() const {return &rootTransform;}
	CTransform* getRootTransform() {return &rootTransform;}
	const CTransform* getTipTransform() const {return &tipTransform;}
	CTransform* getTipTransform() {return &tipTransform;}

	float getSkinThickness() const {return skin_thickness;}
	float getLE() const {return le;}
	float getTE() const {return te;}

	void setSkinThickness(float st) {skin_thickness = st;}
	void setLE( float width) {le = width;}
	void setTE( float width) {te = width;}

	const CSpar* getSpar(int idx) const;
	CSpar* getSpar(int idx);
	int getSparCount() const {return (int)spars.size();}

	CSpar* addSpar(const CSpar& spar);
	void deleteSpar(const CSpar* spar);

	CPlotFlags* getPlotFlags() {return &flags;}
	const CPlotFlags* getPlotFlags() const {return &flags;}

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:

	void copy(const CWing& rhs);

	float skin_thickness; /* */
	float le;             /* Leading edge width */
	float te;             /* trailing edge width */

	CAerofoil root;			/* root section */
	CAerofoil tip;			// tip section
	CTransform rootTransform;
	CTransform tipTransform;

	typedef std::vector<CSpar> SPARS;
	SPARS spars;		   /* list of spars in this wing */
	CPlotFlags flags;		// how this wing should be plotted.

};

#endif // !defined(AFX_WING_H__BBA1B6BB_B27A_11D6_AF75_002018BA082E__INCLUDED_)
