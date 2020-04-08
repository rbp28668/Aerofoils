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

#include <list>
#include "Kernel.h"
#include "CutStructure.h"
#include "PlotCommonImpl.h"
#include "PlotFlags.h"
#include "Cutout.h"

class CAerofoil;
class PointT;
class COutputDevice;
class CWing;
class CObjectSerializer;
class CSpar;

class CPathCutter  : public CutStructure, private CPlotCommonImpl
{
	// Frame sets the cutting parameters during a cut.
	class Frame
	{
		NumericT ru0, tu0;  /* start values for u */
		NumericT ruf, tuf;  /* forward (at le) values for u */
		NumericT ru1, tu1;  /* end values for u */

	public:
		Frame(NumericT ru0, NumericT tu0, NumericT ruf, NumericT tuf, NumericT ru1, NumericT tu1);
		NumericT rootStart() const { return ru0; }
		NumericT tipStart() const { return tu0; }
		NumericT rootForward() const { return ruf; }
		NumericT tipForward() const { return tuf; }
		NumericT rootFinish() const { return ru1; }
		NumericT tipFinish() const { return tu1; }
	};

	// A position during the cut where the cutter should stop cutting the profile and do something different.
	class Intercept
	{
		NumericT ru;             /* root U for this intercept */
		NumericT tu;             /* tip .... */
	public:
		Intercept(NumericT root_u, NumericT tip_u);
		NumericT rootPosition() const { return ru; }
		NumericT tipPosition() const { return tu; }

		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame) = 0;
	};

	// Ordered list of intercepts.
	class Intercepts {
		std::list<Intercept*> intercepts;
		bool isSorted;

		static bool compare_intercepts(const CPathCutter::Intercept* first, const CPathCutter::Intercept* second);
	public:
		Intercepts();
		~Intercepts();
		void add(Intercept* intercept);
		std::list<Intercept*>::const_iterator begin();
		std::list<Intercept*>::const_iterator end();
	};


	NumericT tool_offset;
	bool blToolOffsetSet;
	CPlotFlags plot_flags;
	const CWing* pWing;

	void find_forward_PointT(const CAerofoil& foil, NumericT* u);
	void plot_segment(COutputDevice* pdev, const CWing& wing, Intercept* start, Intercept* finish, NumericT delta);
	void set_le_intercept(Intercepts& intercepts, Frame* frame);
	void set_te_intercept(Intercepts& intercepts, Frame* frame);
	void set_spars_intercept(Intercepts& intercepts, Frame* frame);
	void set_cutouts_intercept(Intercepts& intercepts, Frame* frame);

public:

	static const std::string TYPE;

	CPathCutter(const CWing* pWing);
	explicit CPathCutter(); // for serialization

	NumericT set_tool_offset(NumericT fNewOffset);
	NumericT get_tool_offset(void);

	const CWing* wing() const { return pWing; }

	virtual void cut(COutputDevice* pdev, double toolOffset);
	virtual std::string getDescriptiveText() const;
	virtual std::string getType() const;
	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;
	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

private:


	// Mark a position during the cut (start, finish, forward point) but where no operation is done.
	class NoOpIntercept : public Intercept {
	public:
		NoOpIntercept(NumericT ru, NumericT tu);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame);
	};

	// Cut the leading edge at this position
	class LeadingEdgeIntercept : public Intercept {
	public:
		LeadingEdgeIntercept(NumericT ru, NumericT tu);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame);
	};

	// Cut the trailing edge at this position
	class TrailingEdgeIntercept : public Intercept {
		bool topSurface;
	public:
		TrailingEdgeIntercept(NumericT ru, NumericT tu, bool topSurface);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame);
	};

	// Cut a spar at this position
	class SparIntercept : public Intercept {
		const CSpar* pSpar;
	public:
		SparIntercept(NumericT ru, NumericT tu, const CSpar* spar);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame);
	};

	// Cut a Cutout at this position
	class CutoutIntercept : public Intercept {
		const Cutout* pCutout;
	public:
		CutoutIntercept(NumericT ru, NumericT tu, const Cutout* cutout);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame);
	};


};

#endif

