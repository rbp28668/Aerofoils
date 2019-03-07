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

#ifndef _PLOTFOIL_H
#define _PLOTFOIL_H

#include "PlotFlags.h"
#include "PlotStructure.h"
#include "PlotCommonImpl.h"

class PointT;
class COutputDevice;
class CWing;
class CSpar;

class CPathPlotter : public CPlotStructure, private CPlotCommonImpl
{
	CWing* wing;	// the wing this is plotting.

public:
	CPathPlotter(CWing* pWing);
	explicit CPathPlotter(); // for serialization

	float getSpan() const;
	CWing* getWing() { return wing; }
	virtual CStructure* getStructure();
	virtual const CStructure* getStructure() const;

	virtual void plot(COutputDevice* pdev);
	virtual std::string getDescriptiveText() const;

	virtual	void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

private:
	void plot_skin(COutputDevice* pdev,const CWing& wing) const;
	void plot_chord(COutputDevice* pdev, const CWing& wing) const;
	void plot_markers(COutputDevice* pdev, const CWing& wing) const;
	void plot_le(COutputDevice* pdev, const CWing& wing) const;
	void plot_te(COutputDevice* pdev, const CWing& wing) const;
	void plot_labels(COutputDevice* pdev, const CWing& wing) const;
	void plot_spar(COutputDevice* pdev, const CWing& wing, const CSpar& spar) const;
	void plot_full_depth_spar_side(COutputDevice* pdev, const CWing& wing, float rx,float tx, bool submerged) const;
	void plot_full_depth_spar(COutputDevice* pdev, const CWing& wing, const CSpar& spar) const;
	void plot_all_spars(COutputDevice* pdev, const CWing& wing) const;

	CPlotFlags plot_flags;

};

#endif

