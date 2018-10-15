
#ifndef _CUTPATH_H
#define _CUTPATH_H

#include "PlotCommonImpl.h"
#include "PlotFlags.h"

class CAerofoil;
class PointT;
class COutputDevice;
class CWing;
class CObjectSerializer;

class CPathCutter  : public CPlotCommonImpl
{

public:

	CPathCutter(CWing* pWing);
	explicit CPathCutter(); // for serialization


	float set_tool_offset(float fNewOffset);
	float get_tool_offset(void);
	void set_root_side(int left);

	virtual void plot(COutputDevice* pdev);
	virtual std::string getDescriptiveText() const;

	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

private:

	struct INTERCEPT;
	struct FRAME;

	struct INTERCEPT
	{
	  int   idx;            /* index (mainly for spars) */
	  float ru;             /* root U for this intercept */
	  float tu;             /* tip .... */
	  void (CPathCutter::*fn)(INTERCEPT*, FRAME*);   /* run this fn at the intercept */
	  void* data;           /* and pass it this ptr. */
	};

	/* FRAME structure: used to pass the cutting parameters to the
	// cut_ functions
	*/
	struct FRAME
	{
	  float ru0,tu0;  /* start values for u */
	  float ruf,tuf;  /* forward (at le) values for u */
	  float ru1,tu1;  /* end values for u */
	  const CWing* wing;
	};


	void find_forward_PointT(const CAerofoil& foil, float *u);
	void plot_segment(const CWing& wing,INTERCEPT* start, INTERCEPT* finish, float delta);
	void sort_intercepts(void);

	void cut_le(INTERCEPT* /*icept*/,FRAME* frame);
	void cut_te(INTERCEPT* icept,FRAME* frame);
	void cut_spar(INTERCEPT* icept,FRAME* frame);

	void set_le_intercept(FRAME* frame);
	void set_te_intercept(FRAME* frame);
	void set_spars_intercept(FRAME* frame);

	CPlotFlags plot_flags;

	float tool_offset;
	bool blToolOffsetSet;
	INTERCEPT ilist[32];
	int icount;
};

#endif

