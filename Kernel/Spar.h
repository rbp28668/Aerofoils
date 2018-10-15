// Spar.h: interface for the CSpar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPAR_H__BBA1B6BA_B27A_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_SPAR_H__BBA1B6BA_B27A_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CObjectSerializer;

class CSpar  
{
public:
	CSpar();
	virtual ~CSpar();

	enum e_spar_type {none,top,bottom,full_depth};

	float getRootX() const {return root_x;}
	float getRootWidth() const {return root_width;}
	float getRootHeight() const {return root_height;}

	float getTipX() const {return tip_x;}
	float getTipWidth() const {return tip_width;}
	float getTipHeight() const {return tip_height;}

	bool isSubmerged() const {return submerged;}
	bool isFullDepth() const {return spar_type == full_depth;}

	e_spar_type getType() const {return spar_type;}

	void setRootX(float rhs)  { root_x = rhs;}
	void setRootWidth(float rhs)  { root_width = rhs;}
	void setRootHeight(float rhs)  { root_height = rhs;}

	void setTipX(float rhs)  { tip_x = rhs;}
	void setTipWidth(float rhs)  { tip_width = rhs;}
	void setTipHeight(float rhs)  { tip_height = rhs;}

	void setSubmerged(bool bs = true)  {submerged = bs;}

	void setType(e_spar_type tp) {spar_type = tp;}

	const char* getText();

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

private:
	enum e_spar_type spar_type;
	float root_x;         /* distance of spar from le at root */
	float tip_x;          /* ditto at tip */
	bool submerged;        /* true if under skin */
	float root_width;     /* size of spar at root*/
	float root_height;    /* size of spar at root*/
	float tip_width;      /* size at tip */
	float tip_height;     /* ditto */

	std::string text;		// for display

};

#endif // !defined(AFX_SPAR_H__BBA1B6BA_B27A_11D6_AF75_002018BA082E__INCLUDED_)
