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
*/// PointStructure.h: interface for the CPointStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PointStructure_H__163A2F03_CDF5_11D6_9568_000000000000__INCLUDED_)
#define AFX_PointStructure_H__163A2F03_CDF5_11D6_9568_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Kernel.h"
#include "Structure.h"
#include "PointT.h"

class CPointStructure : public CStructure  
{
public:
	static const std::string TYPE;

	CPointStructure();
	explicit CPointStructure(const CPointStructure& point);
	virtual ~CPointStructure();

	virtual	void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	virtual std::string getDescriptiveText() const;
	virtual std::string getType() const;

	PointT getRoot() const {return root;}
	PointT getTip() const {return tip;}

	void setRoot(PointT pt) {root = pt;}
	void setTip(PointT pt) {tip = pt;}

	void setLabel(const std::string& label) { this->label = label; }
	void setLabel(const char* label) { this->label = label; }

private:
	PointT root;	// offsets;
	PointT tip;		// offsets;
	std::string label;

};

#endif // !defined(AFX_PointStructure_H__163A2F03_CDF5_11D6_9568_000000000000__INCLUDED_)
