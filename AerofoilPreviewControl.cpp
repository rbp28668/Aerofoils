#include "stdafx.h"
#include "AerofoilPreviewControl.h"
#include "WindowsOutputDevice.h"
#include "Kernel/Plotfoil.h"
#include "Kernel/Wing.h"
#include "Kernel/Bounds.h"


BEGIN_MESSAGE_MAP(AerofoilPreviewControl, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

AerofoilPreviewControl::AerofoilPreviewControl(CWing* pWing, bool isRoot)
    : pWing(pWing)
    , isRoot(isRoot)
{
}

void AerofoilPreviewControl::OnPaint()
{

    // device context for painting
    CPaintDC dc(this);

    // save current brush
    CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(BLACK_BRUSH);

    CBrush background;
    // background.CreateSolidBrush(RGB(255, 200, 200));
    background.CreateSysColorBrush(COLOR_3DFACE); // Face color for three-dimensional display elements and for dialog box backgrounds.
    RECT r;
    GetWindowRect(&r);
    ScreenToClient(&r);

    dc.FillRect(&r, &background);

    CPathPlotter plotter(pWing);
    plotter.setInterpolate(true); 
    plotter.setInvert(false);
    plotter.setReflect(false);
    plotter.setPosition(0, 0);
    plotter.setSectionPos(isRoot ? 0 : plotter.getSpan());

    pWing->getPlotFlags()->plot_le = true;
    pWing->getPlotFlags()->plot_te = true;
    pWing->getPlotFlags()->plot_skin = true;
    pWing->getPlotFlags()->plot_spars = true;
    pWing->getPlotFlags()->plot_cutouts = true;
    pWing->getPlotFlags()->plot_chord = false;
    pWing->getPlotFlags()->plot_labels = false;
    pWing->getPlotFlags()->plot_inverted = false;

    Bounds bounds;
    plotter.plot(&bounds);
    RectT logical = bounds.getBounds();
    
    CWindowsOutputDevice output(&dc, logical, r);
    
    plotter.plot(&output);

    // select original brush into device contect
    dc.SelectObject(pOldBrush);
}
