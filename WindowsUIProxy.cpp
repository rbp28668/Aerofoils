// WindowsUIProxy.cpp: implementation of the CWindowsUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aerofoil.h"
#include "WindowsUIProxy.h"
#include "PlotFlagsDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWindowsUIProxy::CWindowsUIProxy()
{

}

CWindowsUIProxy::~CWindowsUIProxy()
{

}

bool CWindowsUIProxy::EditPlotFlags(CPlotStructure* toEdit)
{
	CPlotFlagsDlg dlg(toEdit);
	return dlg.DoModal() == IDOK;
}