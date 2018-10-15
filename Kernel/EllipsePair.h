// EllipsePair.h: interface for the CEllipsePair class.
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
	CEllipsePair();
	explicit CEllipsePair(const CEllipsePair& pair);
	virtual ~CEllipsePair();

	virtual std::string getDescriptiveText() const;

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
