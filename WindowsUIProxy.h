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
// WindowsUIProxy.h: interface for the CWindowsUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOWSUIPROXY_H__7C73CD16_C566_11D6_9554_00047660C89A__INCLUDED_)
#define AFX_WINDOWSUIPROXY_H__7C73CD16_C566_11D6_9554_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "kernel/UIProxy.h"

class CPlotStructure;
class CPlot;
class CAerofoilDoc;

class CWindowsUIProxy : public CUIProxy  
{
public:
	CWindowsUIProxy();
	virtual ~CWindowsUIProxy();

	virtual UINT getPopupID() = 0;
	virtual bool Edit(CPlotStructure* toEdit) = 0;
	virtual void SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel) = 0;
	virtual bool EditPlotFlags(CPlotStructure* toEdit);

};

#endif // !defined(AFX_WINDOWSUIPROXY_H__7C73CD16_C566_11D6_9554_00047660C89A__INCLUDED_)
