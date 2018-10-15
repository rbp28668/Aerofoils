// PlotFlags.h: interface for the CPlotFlags class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOTFLAGS_H__BBA1B6BC_B27A_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_PLOTFLAGS_H__BBA1B6BC_B27A_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjectSerializer;

class CPlotFlags  
{
public:
	CPlotFlags();
	virtual ~CPlotFlags();

//	int plot_type;
	bool plot_section;  /* True if intermediate sect wanted */
	bool plot_skin;     /* True if skin thickness plot */
	bool plot_chord;    /* True if chord plot wanted */
	bool plot_markers;  /* True if 10% markers wanted */
	bool plot_spars;    /* True if plot of spars wanted */
	bool plot_le;       /* True if LE wanted */
	bool plot_te;       /* True if TE wanted */
	bool plot_labels;   /* True if annotated drawing wanted */
//	bool plot_ellipse_markers; /* True if we want ellipse markers */
	bool plot_inverted; /* True if should plot inverted section */

	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

};

#endif // !defined(AFX_PLOTFLAGS_H__BBA1B6BC_B27A_11D6_AF75_002018BA082E__INCLUDED_)
