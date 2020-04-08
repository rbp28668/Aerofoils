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
*/// PlotFlags.cpp: implementation of the CPlotFlags class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "PlotFlags.h"
#include "ObjectSerializer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlotFlags::CPlotFlags() :
	plot_section(true),  /* True if intermediate sect wanted */
	plot_skin(false),     /* True if skin thickness plot */
	plot_chord(false),    /* True if chord plot wanted */
	plot_markers(false),  /* True if 10% markers wanted */
	plot_spars(false),    /* True if plot of spars wanted */
	plot_cutouts(false),  /* True if plot of cutouts wanted*/
	plot_le(false),       /* True if LE wanted */
	plot_te(false),       /* True if TE wanted */
	plot_labels(false),   /* True if annotated drawing wanted */
	
//	bool plot_ellipse_markers; /* True if we want ellipse markers */
	plot_inverted(false) /* True if should plot inverted section */

{

}

CPlotFlags::~CPlotFlags()
{

}

void CPlotFlags::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection("plotFlags",this);
	os.write("plotSection",plot_section);  /* True if intermediate sect wanted */
	os.write("plotSkin",plot_skin);     /* True if skin thickness plot */
	os.write("plotChord",plot_chord);    /* True if chord plot wanted */
	os.write("plotMarkers",plot_markers);  /* True if 10% markers wanted */
	os.write("plotSpars",plot_spars);    /* True if plot of spars wanted */
	os.write("plotCutouts", plot_cutouts); /* True if plot of cutouts wanted*/
	os.write("plotLe",plot_le);       /* True if LE wanted */
	os.write("plotTe",plot_te);       /* True if TE wanted */
	os.write("plotLabels",plot_labels);   /* True if annotated drawing wanted */
	os.write("plotInverted",plot_inverted); /* True if should plot inverted section */
	os.endSection();

}

void CPlotFlags::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection("plotFlags",this);
	os.read("plotSection",plot_section);  /* True if intermediate sect wanted */
	
	// Conditional for backwards compatibility with buggy XML.
	if(os.ifExists("plotSkin)"))
		os.read("plotSkin)",plot_skin);     /* True if skin thickness plot */
	else
		os.read("plotSkin",plot_skin);

	if(os.ifExists("plotChord)"))
		os.read("plotChord)",plot_chord);    /* True if chord plot wanted */
	else
		os.read("plotChord",plot_chord);

	os.read("plotMarkers",plot_markers);  /* True if 10% markers wanted */
	os.read("plotSpars",plot_spars);    /* True if plot of spars wanted */

	if (os.ifExists("plotCutouts"))
		os.read("plotCutouts", plot_cutouts);

	os.read("plotLe",plot_le);       /* True if LE wanted */
	os.read("plotTe",plot_te);       /* True if TE wanted */
	os.read("plotLabels",plot_labels);   /* True if annotated drawing wanted */
	os.read("plotInverted",plot_inverted); /* True if should plot inverted section */
	os.endReadSection();
}

