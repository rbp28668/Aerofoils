
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
*/// Wing.cpp: implementation of the CWing class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <sstream>
#include "Wing.h"
#include "ObjectSerializer.h"

using namespace std;

const std::string CWing::TYPE("wing");
static CObjectFactory<CWing> factory(CWing::TYPE.c_str());

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWing::CWing(const char* rootSection, NumericT rootThickness, const char* tipSection, NumericT tipThickness)
: root(rootSection, rootThickness)
, tip(tipSection, tipThickness)
, le(0.0f)
, te(0.0f)
, skin_thickness(1.0f)
{

}

CWing::CWing(const CWing& rhs)
: root(rhs.root)
, tip(rhs.tip)
{
	copy(rhs);
}

CWing::CWing()
: root()
, tip()
, le(0.0f)
, te(0.0f)
, skin_thickness(1.5875f) // default to 1/16 in.
{
}

CWing::~CWing()
{

}

CWing& CWing::operator = (const CWing& rhs)
{
	assert(this);
	if(this == &rhs)
		return *this;

	root = rhs.root;	
	tip = rhs.tip;		
	copy(rhs);

	return *this;
}

void CWing::copy(const CWing& rhs)
{
	span = rhs.span;
	skin_thickness = rhs.skin_thickness; 
	le = rhs.le; 
	te = rhs.te;

	rootTransform = rhs.rootTransform;
	tipTransform = rhs.tipTransform;

	spars.assign(rhs.spars.begin(), rhs.spars.end());
	cutouts.assign(rhs.cutouts.begin(), rhs.cutouts.end());

	flags = rhs.flags;	
}

string CWing::getDescriptiveText() const
{
	ostringstream ost;
	ost << root.getName() << " " << rootTransform.getChord() << "mm, "
		<< tip.getName() << " " << tipTransform.getChord() << "mm" << ends;

	return ost.str();
}

std::string CWing::getType() const
{
	return TYPE;
}

const CSpar* CWing::getSpar(int idx) const
{
	assert(this);
	assert(idx >= 0);
	assert(idx < spars.size());
	return &spars[idx];
}

CSpar* CWing::getSpar(int idx)
{
	assert(this);
	assert(idx >= 0);
	assert(idx < spars.size());
	return &spars[idx];
}

CSpar* CWing::addSpar(const CSpar& spar)
{
	assert(this);
	spars.push_back(spar);
	return &spars.back();
}

void CWing::deleteSpar(const CSpar* spar)
{
	assert(this);
	assert(spar);

	for(SPARS::iterator iter = spars.begin();
	iter != spars.end();
	++iter)
	{
		if( &(*iter) == spar)	// compare pointers
		{
			spars.erase(iter);
			break;
		}

	}
}

const Cutout* CWing::getCutout(int idx) const
{
	assert(this);
	assert(idx >= 0);
	assert(idx < cutouts.size());
	return &cutouts[idx];
}

Cutout* CWing::getCutout(int idx)
{
	assert(this);
	assert(idx >= 0);
	assert(idx < cutouts.size());
	return &cutouts[idx];
}

Cutout* CWing::addCutout(const Cutout& cutout)
{
	assert(this);
	cutouts.push_back(cutout);
	return &cutouts.back();
}

// Remove the referenced cutout from the collection.
void CWing::deleteCutout(const Cutout* cutout)
{
	assert(this);
	assert(cutout);

	for (CUTOUTS::iterator iter = cutouts.begin();
		iter != cutouts.end();
		++iter)
	{
		if (&(*iter) == cutout)	// compare pointers
		{
			cutouts.erase(iter);
			break;
		}
	}
}

void CWing::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection(TYPE.c_str(),this);
	os.write("le",le);
	os.write("te",te);
	os.write("span",span);
	os.write("skin",skin_thickness);

	root.serializeTo(os);
	rootTransform.serializeTo(os);
	
	tip.serializeTo(os);
	tipTransform.serializeTo(os);
	
	flags.serializeTo(os);

	os.startCollection("spars",(int)spars.size());
	for(SPARS::iterator iter = spars.begin();
	iter != spars.end();
	++iter)
	{
		iter->serializeTo(os);
	}
	os.endCollection();

	os.startCollection("cutouts", (int)cutouts.size());
	for (CUTOUTS::iterator iter = cutouts.begin();
		iter != cutouts.end();
		++iter)
	{
		iter->serializeTo(os);
	}
	os.endCollection();

	os.endSection();
}

void CWing::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection(TYPE.c_str(),this);
	os.read("le",le);
	os.read("te",te);
	os.read("span",span);
	os.read("skin",skin_thickness);

	root.serializeFrom(os);
	rootTransform.serializeFrom(os);
	
	tip.serializeFrom(os);
	tipTransform.serializeFrom(os);
	
	flags.serializeFrom(os);

	
	int cSpars = os.startReadCollection("spars");
	for(int i=0; i<cSpars; ++i)
	{
		CSpar spar;
		spar.serializeFrom(os);
		spars.push_back(spar);

	}
	os.endReadCollection();

	if (os.ifExists("cutouts")) {
		int cCutouts = os.startReadCollection("cutouts");
		for (int i = 0; i < cCutouts; ++i)
		{
			Cutout cutout;
			cutout.serializeFrom(os);
			cutouts.push_back(cutout);

		}
		os.endReadCollection();
	}

	os.endReadSection();
}

