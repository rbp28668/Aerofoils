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
*/
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

