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
public:
	enum Mode {
		NORMAL,
		REVERSE,
		TOP_FROM_TE,
		TOP_FROM_LE,
		BOTTOM_FROM_TE,
		BOTTOM_FROM_LE
	};

private:
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
		NumericT toTip(NumericT ru) const;
	};

	// A position during the cut where the cutter should stop cutting the profile and do something different.
	class Intercept
	{
		bool skip;		// if true skip segments between this intercept and the next one with this flag set
						// Works irrespective of cutting direction or partial cuts.

		NumericT ru;             /* root U for this intercept */
		NumericT tu;             /* tip .... */
	public:
		Intercept(NumericT root_u, NumericT tip_u);
		NumericT rootPosition() const { return ru; }
		NumericT tipPosition() const { return tu; }

		bool skipSegment() const { return skip; }		// Go straight on to next intercept with this flag set
		void setSkip(bool skip) { this->skip = skip; }

		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context) = 0;
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
		std::list<Intercept*>::const_reverse_iterator rbegin();
		std::list<Intercept*>::const_reverse_iterator rend();
	};


	CPlotFlags plot_flags;
	const CWing* pWing;
	CPathCutter::Mode mode;

	void find_forward_PointT(const CAerofoil& foil, NumericT* u);
	void plot_segment(COutputDevice* pdev, Intercept* start, Intercept* finish, NumericT delta, const CutStructure::Context& context);
	void plot_segment_reverse(COutputDevice* pdev, Intercept* start, Intercept* finish, NumericT delta, const CutStructure::Context& context);
	void plot_segment_opt(COutputDevice* pdev, NumericT ru0, NumericT ru1, NumericT tu0, NumericT tu1, NumericT scaledTolerance, const CutStructure::Context& context);
	NumericT error_distance(PointT p0, PointT p1, PointT midpoint);
	void toPoint(NumericT ru, COutputDevice* pdev,  Intercept* start, Intercept* finish, NumericT r_skin, NumericT t_skin);
	void calc_skin(NumericT& r_skin, NumericT& t_skin, const CutStructure::Context& context) const;
	void set_le_intercept(Intercepts& intercepts, const Frame* frame, const CutStructure::Context& context);
	void set_te_intercept(Intercepts& intercepts, const Frame* frame, const CutStructure::Context& context);
	void set_spars_intercept(Intercepts& intercepts, const Frame* frame, const CutStructure::Context& context);
	void set_cutouts_intercept(Intercepts& intercepts, const Frame* frame, const CutStructure::Context& context);

	void createFrame(Frame& frame, const CutStructure::Context& context);
	void createIntercepts(Intercepts& intercepts, const Frame& frame, const CutStructure::Context& context);

public:

	static const std::string TYPE;

	CPathCutter(const CWing* pWing);
	explicit CPathCutter(); // for serialization

	CPathCutter::Mode get_mode() const { return mode; }
	void set_mode(CPathCutter::Mode mode) { this->mode = mode; }
	
	const CWing* wing() const { return pWing; }

	virtual void cut(COutputDevice* pdev, const CutStructure::Context& context);
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
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context);
	};

	// Cut the leading edge at this position
	class LeadingEdgeIntercept : public Intercept {
		bool topSurface;
	public:
		LeadingEdgeIntercept(NumericT ru, NumericT tu, bool topSurface);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context);
	};

	// Cut the trailing edge at this position
	class TrailingEdgeIntercept : public Intercept {
		bool topSurface;
	public:
		TrailingEdgeIntercept(NumericT ru, NumericT tu, bool topSurface);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context);
	};

	// Cut a spar at this position
	class SparIntercept : public Intercept {
		const CSpar* pSpar;
	public:
		SparIntercept(NumericT ru, NumericT tu, const CSpar* spar);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context);
	};

	// Cut a Cutout at this position
	class CutoutIntercept : public Intercept {
		const Cutout* pCutout;
	public:
		CutoutIntercept(NumericT ru, NumericT tu, const Cutout* cutout);
		virtual void process(CPathCutter* cutter, COutputDevice* output, Frame* frame, const CutStructure::Context& context);
	};


};

#endif

