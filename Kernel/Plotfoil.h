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
*/#ifndef _PLOTFOIL_H
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

