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
// AerofoilDoc.cpp : implementation of the CAerofoilDoc class
//

#include "stdafx.h"
#pragma warning(disable: 4786)

#include <assert.h>
#include <fstream>

#include "Aerofoil.h"

#include "AerofoilDoc.h"
#include "AerofoilView.h"
#include "SectionPositionDlg.h"
#include "WingDlg.h"
#include "EllipsePairDlg.h"
#include "PlotPointDlg.h"
#include "WingFlagsDlg.h"
#include "PaperSizeDlg.h"
#include "SelectStructureDlg.h"
#include "EllipseFlagsDlg.h"
#include "WingUIProxy.h"
#include "EllipseUIProxy.h"
#include "PlotPointUIProxy.h"
#include "PlotOrderDlg.h"

#include "BackgroundGridDlg.hpp"

#include "kernel\drivers\DXFOutputDevice.h"
#include "kernel\drivers\LaserJet.h"
#include "kernel\drivers\Postscript.h"
#include "Kernel\GCodeOutputFile.h"

#include "kernel\plotfoil.h" // CPathPlotter
#include "kernel\wing.h"
#include "kernel\ObjectSerializer.h"
#include "kernel\EllipsePair.h"
#include "kernel\EllipsePlotter.h"
#include "kernel\PointStructure.h"
#include "kernel\PointPlotter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CAerofoilDoc

IMPLEMENT_DYNCREATE(CAerofoilDoc, CDocument)

BEGIN_MESSAGE_MAP(CAerofoilDoc, CDocument)
	ON_COMMAND(ID_WING_FLAGS, OnWingFlags)
	ON_UPDATE_COMMAND_UI(ID_WING_FLAGS, OnUpdateWingFlags)
	ON_COMMAND(ID_WING_NEW, OnWingNew)
	ON_COMMAND(ID_WING_NEW_CORE, OnWingNewCore)
	ON_UPDATE_COMMAND_UI(ID_WING_NEW_CORE, OnUpdateWingNewCore)
	ON_COMMAND(ID_WING_NEW_SECTION, OnWingNewSection)
	ON_UPDATE_COMMAND_UI(ID_WING_NEW_SECTION, OnUpdateWingNewSection)
	ON_COMMAND(ID_WING_EDIT, OnWingEdit)
	ON_UPDATE_COMMAND_UI(ID_WING_EDIT, OnUpdateCurrentWingSet)
	ON_COMMAND(ID_FILE_PLOT_SIZE, OnFilePlotSize)
	ON_COMMAND(ID_ELLIPSE_EDIT, OnEllipseEdit)
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE_EDIT, OnUpdateEllipseSelected)
	ON_COMMAND(ID_ELLIPSE_FLAGS, OnEllipseFlags)
	ON_COMMAND(ID_ELLIPSE_NEW, OnEllipseNew)
	ON_COMMAND(ID_ELLIPSE_NEW_SECTION, OnEllipseNewSection)
	ON_COMMAND(ID_FILE_DXF, OnFileDxf)
	ON_COMMAND(ID_FILE_LASERJET, OnFileLaserjet)
	ON_COMMAND(ID_FILE_POSTSCRIPT, OnFilePostscript)
	ON_COMMAND(ID_EDIT_POSITION, OnEditPosition)
	ON_UPDATE_COMMAND_UI(ID_EDIT_POSITION, OnUpdateItemIsSelected)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_STRUCTURE, OnEditStructure)
	ON_COMMAND(ID_EDIT_PLOTFLAGS, OnEditPlotflags)
	ON_COMMAND(ID_POINT_NEW, OnPointNew)
	ON_COMMAND(ID_EDIT_PLOTORDER, OnEditPlotorder)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PLOTORDER, OnUpdateEditPlotorder)
	ON_COMMAND(ID_EDIT_DELETESTRUCTURE, OnEditDeletestructure)
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE_FLAGS, OnUpdateEllipseSelected)
	ON_UPDATE_COMMAND_UI(ID_ELLIPSE_NEW_SECTION, OnUpdateEllipseSelected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateItemIsSelected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STRUCTURE, OnUpdateItemIsSelected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PLOTFLAGS, OnUpdateItemIsSelected)
	ON_COMMAND(ID_FILE_SETGRID, OnFileSetgrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAerofoilDoc construction/destruction

CAerofoilDoc::CAerofoilDoc()
: currentWing(0)
, currentEllipse(0)
, selected(0)
, plotOrderDialog(0)
{
	setDocSize(210,297);	// A4
}

CAerofoilDoc::~CAerofoilDoc()
{
	if(plotOrderDialog)
		closePlotOrderDialog();
}

BOOL CAerofoilDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CAerofoilDoc::setDocSize(float width, float height)
{
	assert(this);
	size_x = width;
	size_y = height;

	place_x = 10;
	place_y = height - 30;

	POSITION pos = GetFirstViewPosition();
	while (pos)
	{
		CAerofoilView* pView = (CAerofoilView*)GetNextView(pos);
		ASSERT_VALID(pView);
		pView->DocSizeChanged();
	}

	RedrawNow();
}

void CAerofoilDoc::updatePlacePosition()
{
	assert(this);
	place_y -= 15;
	if (place_y < 30)
	{
		place_y = size_y - 30;
		place_x += 20;
		if(place_x > size_x - 100) // large as foils go l->r
			place_x = 10;
	}
}

void CAerofoilDoc::UpdateNow()
{
  assert(this);
  POSITION pos = GetFirstViewPosition();
  while (pos)
    {
    CView* pView = GetNextView(pos);
    ASSERT_VALID(pView);
    pView->UpdateWindow();
    }
  return;
}

void CAerofoilDoc::RedrawNow()
{
  assert(this);
  POSITION pos = GetFirstViewPosition();
  while (pos)
    {
    CView* pView = GetNextView(pos);
    ASSERT_VALID(pView);
    pView->RedrawWindow();
    }
  return;
}

void CAerofoilDoc::setSelection(CPlotStructure* ps)
{
	assert(this);
	assert(ps);

	CPlotStructure* old = selected;
	selected = ps;
	if(ps != old)
	{
		CWindowsUIProxy* proxy = static_cast<CWindowsUIProxy*>(ps->getUIProxy());
		proxy->SetSelectedStructure(this,ps);

		CPlotStructure* old = selected;
		RedrawNow();

		// And finally, if there is an active plot order dialog,
		// then tell it to update it's selection to match:
		if(plotOrderDialog)
		{
			plotOrderDialog->setSelection(ps);
		}
	}
}

void CAerofoilDoc::clearSelection()
{
	assert(this);

	CPlotStructure* old = selected;
	selected = 0;
	currentWing = 0;
	currentEllipse = 0;
	currentPoint = 0;

	if(old)
		RedrawNow();
}


void CAerofoilDoc::selectWing(CWing* pw)
{
	assert(this);
	assert(pw);
	currentWing = pw;
	currentEllipse = 0;
	currentPoint = 0;
}

void CAerofoilDoc::selectEllipses(CEllipsePair* pep)
{
	assert(this);
	assert(pep);
	currentWing = 0;
	currentEllipse = pep;
	currentPoint = 0;
}

void CAerofoilDoc::selectPointStructure(CPointStructure* pps)
{
	assert(this);
	assert(pps);
	currentWing = 0;
	currentEllipse = 0;
	currentPoint = pps;
}

void CAerofoilDoc::closePlotOrderDialog()
{
	assert(this);
	assert(plotOrderDialog);
	plotOrderDialog->DestroyWindow();
	plotOrderDialog = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CAerofoilDoc serialization

void CAerofoilDoc::Serialize(CArchive& ar)
{
	assert(false);	// serialization handled in OnOpenDocument, OnSaveDocument
}

/////////////////////////////////////////////////////////////////////////////
// CAerofoilDoc diagnostics

#ifdef _DEBUG
void CAerofoilDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAerofoilDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAerofoilDoc commands

BOOL CAerofoilDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	assert(this);
	assert(lpszPathName);

	BOOL success = TRUE;
	try
	{
		fstream ifs(lpszPathName, ios_base::in );
		CObjectSerializer serializer(&ifs);
		getPlot().serializeFrom(serializer);
	}
	catch(CSerializeException& se)
	{
		::MessageBox(0,se.what(),"Unable to read plot file",MB_OK | MB_ICONERROR);
		success = FALSE;
	}
	catch(...)
	{
		::MessageBox(0,"Error opening or parsing input","Unable to read plot file",MB_OK | MB_ICONERROR);
		success = FALSE;
	}
	return success;
}

BOOL CAerofoilDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	assert(this);
	assert(lpszPathName);

	BOOL success = TRUE;
	try
	{
		fstream ofs(lpszPathName, ios_base::out);
		CObjectSerializer serializer(&ofs);
		getPlot().serializeTo(serializer);
	}
	catch(...)
	{
		success = FALSE;
	}
	return success;
}

void CAerofoilDoc::OnFilePlotSize() 
{
	CPaperSizeDlg dlg;
	dlg.m_width = size_x;
	dlg.m_height = size_y;
	dlg.m_sizeSelect = 1;	// default to A4
	if(dlg.DoModal())
	{
		switch(dlg.m_sizeSelect)
		{
		case 0: // A4
			size_x = 210;	// A4
			size_y = 297;
			break;
		case 1: // A4 landscape
			size_x = 297;	// A4
			size_y = 210;
			break;
		case 2: // A3
			size_x = 297;	// A3
			size_y = 210 * 2;
			break;
		case 3: // A3 landscape
			size_x = 210 * 2;
			size_y = 297;	// A3
			break;
		case 4: // custom
			size_x = dlg.m_width;
			size_y = dlg.m_height;
			break;

		}
		setDocSize(size_x,size_y);
	}
}

///////////////////////////////////////////////////////////////////////
// New structure
void CAerofoilDoc::OnWingNew() 
{
	CPlot& plot = getPlot();

	CFileDialog dlg(TRUE, 
		".DAT",
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		"Aerofoil Files (.DAT)|*.DAT||"
		);

	if(dlg.DoModal() == IDOK)
	{
		CString rootName = dlg.GetPathName();
		if(dlg.DoModal() == IDOK)
		{
			CString tipName = dlg.GetPathName();
			currentWing = plot.addWing(rootName, 1.0f, tipName, 1.0f );
			CPathPlotter* ppp = plot.addPathPlotter(currentWing);
			ppp->setUIProxy(new CWingUIProxy());
			ppp->setPosition(place_x,place_y);
			updatePlacePosition();
			RedrawNow();
		}
	}
}

void CAerofoilDoc::OnEllipseNew() 
{
	assert(this);
	
	CEllipsePair pair;
	CEllipsePairDlg dlg(&pair);
	if(dlg.DoModal() == IDOK)
	{
		CPlot& plot = getPlot();
		currentEllipse = plot.addEllipsePair(pair);
		CPlotStructure* pps = plot.addEllipsePlotter(currentEllipse);
		pps->setUIProxy(new CEllipseUIProxy());
		pps->setPosition(place_x,place_y);
		updatePlacePosition();
		RedrawNow();
	}
}

void CAerofoilDoc::OnPointNew() 
{
	CPointStructure point;
	CPlotPointDlg dlg(&point);
	if(dlg.DoModal() == IDOK)
	{
		CPlot& plot = getPlot();
		currentPoint = plot.addPointStructure(point);
		CPointPlotter* ppp = plot.addPointPlotter(currentPoint);
		ppp->setUIProxy(new CPlotPointUIProxy());
		ppp->setPosition(place_x,place_y);
		updatePlacePosition();
		RedrawNow();
	}
}

///////////////////////////////////////////////////////////////////////
// Sections

void CAerofoilDoc::OnWingNewSection() 
{
	assert(this);
	assert(currentWing != 0);	

	CSectionPositionDlg dlg;
	dlg.m_maxPosition = currentWing->getSpan();

	if(dlg.DoModal() == IDOK)
	{
		CPathPlotter* ppp = getPlot().addPathPlotter(currentWing);
		ppp->setSectionPos(dlg.m_position);
		ppp->setUIProxy(new CWingUIProxy());
		ppp->setPosition(place_x,place_y);
		updatePlacePosition();
		RedrawNow();
	}
}

void CAerofoilDoc::OnWingNewCore() 
{
	assert(this);
	assert(currentWing != 0);	
    // Now a NOP - TODO delete in due course.
}

///////////////////////////////////////////////////////////////////////

void CAerofoilDoc::OnWingFlags() 
{
	assert(this);
	assert(currentWing);

	CWingFlagsDlg dlg(currentWing->getPlotFlags());
	if(dlg.DoModal())
		RedrawNow();
}

void CAerofoilDoc::OnUpdateWingFlags(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(currentWing != 0);	
}



void CAerofoilDoc::OnUpdateWingNewCore(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(currentWing != 0);	
}


void CAerofoilDoc::OnUpdateWingNewSection(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(currentWing != 0);	
}

void CAerofoilDoc::OnWingEdit() 
{
	CWingDlg dlg(currentWing);
	if(dlg.DoModal())
		RedrawNow();
}

void CAerofoilDoc::OnUpdateCurrentWingSet(CCmdUI* pCmdUI) 
{
	assert(this);
	assert(pCmdUI);

	pCmdUI->Enable(currentWing != 0);	
}



void CAerofoilDoc::OnEllipseEdit() 
{
	assert(this);
	assert(currentEllipse);

	CEllipsePairDlg dlg(currentEllipse);
	if(dlg.DoModal() == IDOK)
		RedrawNow();
}

void CAerofoilDoc::OnUpdateEllipseSelected(CCmdUI* pCmdUI) 
{
	assert(this);
	assert(pCmdUI);

	pCmdUI->Enable( currentEllipse != 0);
}

void CAerofoilDoc::OnEllipseFlags() 
{
	assert(this);
	CEllipseFlagsDlg dlg(currentEllipse->getPlotFlags());	
	if(dlg.DoModal() == IDOK)
		RedrawNow();
}


void CAerofoilDoc::OnEllipseNewSection() 
{
	CPlotStructure* pps = plot.addEllipsePlotter(currentEllipse);
	pps->setUIProxy(new CEllipseUIProxy());
	pps->setPosition(place_x,place_y);
	updatePlacePosition();
	RedrawNow();
}

void CAerofoilDoc::OnFileDxf() 
{
	assert(this);

	CFileDialog dlg(FALSE, 
		".DXF",
		NULL,
		OFN_HIDEREADONLY,
		"DXF Files (.DXF)|*.DXF||"
		);

	if(dlg.DoModal() == IDOK)
	{
		//const char* psz = LPCTSTR(dlg.GetPathName());
		CDXFOutputDevice dev(LPCTSTR(dlg.GetPathName()));
		getPlot().plot(dev);
	}
}

void CAerofoilDoc::OnFileLaserjet() 
{
	assert(this);

	CFileDialog dlg(FALSE, 
		".HPG",
		NULL,
		OFN_HIDEREADONLY,
		"HPGL Files (.HPG)|*.HPG||"
		);

	if(dlg.DoModal() == IDOK)
	{
		CLaserJetOutputDevice dev(LPCTSTR(dlg.GetPathName()));
		getPlot().plot(dev);
	}
}

void CAerofoilDoc::OnFilePostscript() 
{
	assert(this);

	CFileDialog dlg(FALSE, 
		".PS",
		NULL,
		OFN_HIDEREADONLY,
		"Postscript Files (.PS)|*.PS||"
		);

	if(dlg.DoModal() == IDOK)
	{
		CPostscriptOutputDevice dev(LPCTSTR(dlg.GetPathName()));
		getPlot().plot(dev);
	}
}


void CAerofoilDoc::OnEditPosition() 
{
	assert(this);
	if(selected)
	{
		CSectionPositionDlg dlg;
		dlg.m_maxPosition = (float)selected->getSpan();
		dlg.m_position = (float)selected->getSectionPos();

		if(dlg.DoModal() == IDOK)
		{
			selected->setSectionPos(dlg.m_position);
			RedrawNow();
		}
	}
}

void CAerofoilDoc::OnUpdateItemIsSelected(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(selected != 0);	
}

void CAerofoilDoc::OnEditDelete() 
{
	if(selected)
	{
		getPlot().deletePlotStructure(selected);
		RedrawNow();
	}

}

void CAerofoilDoc::OnEditStructure() 
{
	if(selected)
	{
		CWindowsUIProxy* proxy = static_cast<CWindowsUIProxy*>(selected->getUIProxy());
		if(proxy->Edit(selected))
			RedrawNow();
	}
}


void CAerofoilDoc::OnEditPlotflags() 
{
	if(selected)
	{
		CWindowsUIProxy* proxy = static_cast<CWindowsUIProxy*>(selected->getUIProxy());
		if(proxy->EditPlotFlags(selected))
			RedrawNow();
	}
}



void CAerofoilDoc::OnEditPlotorder() 
{
	assert(this);
	CPlotOrderDlg* pd = new CPlotOrderDlg(this);

	plotOrderDialog = pd;
}

void CAerofoilDoc::OnUpdateEditPlotorder(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((plotOrderDialog) ?	FALSE : TRUE);
}


void CAerofoilDoc::OnEditDeletestructure() 
{
	CSelectStructureDlg dlg(&plot);
	if(dlg.DoModal() == IDOK)
	{
		clearSelection();
		plot.deleteStructure(dlg.selected);
		RedrawNow();
	}
}

void CAerofoilDoc::OnFileSetgrid() 
{
	CBackgroundGridDlg dlg;
	dlg.m_blPlotHoriz = grid.isHorizontalEnabled();
	dlg.m_blPlotVert = grid.isVerticalEnabled();
	dlg.m_fHorizDistance = grid.getHorizontalSize();
	dlg.m_fVertDistance = grid.getVerticalSize();

	if(dlg.DoModal() == IDOK)
	{
		grid.enableHorizontal(dlg.m_blPlotHoriz != 0);
		grid.enableVertical(dlg.m_blPlotVert != 0);
		grid.setHorizontalSize(dlg.m_fHorizDistance);
		grid.setVerticalSize(dlg.m_fVertDistance);
		RedrawNow();
	}

	
}


