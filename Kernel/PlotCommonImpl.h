// PlotCommonImpl.h: interface for the CPlotCommonImpl class.
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
