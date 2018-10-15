// Structure.h: interface for the CStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRUCTURE_H__B71FC393_CC68_11D6_9566_00047660C89A__INCLUDED_)
#define AFX_STRUCTURE_H__B71FC393_CC68_11D6_9566_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CObjectSerializer;

class CStructure  
{
public:
	CStructure();
	virtual ~CStructure();
	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	virtual std::string getDescriptiveText() const = 0;


	float getSpan() const {return span;}
	void setSpan(float s) {span = s;}

	float span;           /* span of this wing (root to tip) */

};

#endif // !defined(AFX_STRUCTURE_H__B71FC393_CC68_11D6_9566_00047660C89A__INCLUDED_)
