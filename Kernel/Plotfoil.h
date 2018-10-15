#ifndef _PLOTFOIL_H
#define _PLOTFOIL_H

#include "PlotFlags.h"
#include "PlotCommonImpl.h"

class PointT;
class COutputDevice;
class CWing;
class CSpar;

class CPathPlotter : public CPlotCommonImpl
{
public:
	CPathPlotter(CWing* pWing);
	explicit CPathPlotter(); // for serialization

	
	virtual void plot(COutputDevice* pdev);
	virtual std::string getDescriptiveText() const;

	virtual	void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

private:
	void plot_skin(const CWing& wing) const;
	void plot_chord(const CWing& wing) const;
	void plot_markers(const CWing& wing) const;
	void plot_le(const CWing& wing) const;
	void plot_te(const CWing& wing) const;
	void plot_labels(const CWing& wing) const;
	void plot_spar(const CWing& wing, const CSpar& spar) const;
	void plot_full_depth_spar_side(const CWing& wing, float rx,float tx, bool submerged) const;
	void plot_full_depth_spar(const CWing& wing, const CSpar& spar) const;
	void plot_all_spars(const CWing& wing) const;

	CPlotFlags plot_flags;

};

#endif

