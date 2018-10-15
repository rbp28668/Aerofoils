// PointStructure.h: interface for the CPointStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PointStructure_H__163A2F03_CDF5_11D6_9568_000000000000__INCLUDED_)
#define AFX_PointStructure_H__163A2F03_CDF5_11D6_9568_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Structure.h"
#include "PointT.h"

class CPointStructure : public CStructure  
{
public:
	CPointStructure();
	explicit CPointStructure(const CPointStructure& point);
	virtual ~CPointStructure();

	virtual	void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	virtual std::string getDescriptiveText() const;

	PointT getRoot() const {return root;}
	PointT getTip() const {return tip;}

	void setRoot(PointT pt) {root = pt;}
	void setTip(PointT pt) {tip = pt;}

	PointT root;	// offsets;
	PointT tip;		// offsets;

};

#endif // !defined(AFX_PointStructure_H__163A2F03_CDF5_11D6_9568_000000000000__INCLUDED_)
