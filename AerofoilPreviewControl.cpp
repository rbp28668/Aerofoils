/* Aerofoil
Aerofoil plotting and CNC cutter driver
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
