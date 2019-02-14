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
// WingUIProxy.h: interface for the CWingUIProxy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINGUIPROXY_H__7C73CD17_C566_11D6_9554_00047660C89A__INCLUDED_)
#define AFX_WINGUIPROXY_H__7C73CD17_C566_11D6_9554_00047660C89A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WindowsUIProxy.h"

class CWingUIProxy : public CWindowsUIProxy  
{
public:
	CWingUIProxy();
	virtual ~CWingUIProxy();

	const char* className() const; // use default serialization

	virtual UINT getPopupID();
	virtual bool Edit(CPlotStructure* toPlot);
	virtual void SetSelectedStructure(CAerofoilDoc* pDoc, CPlotStructure* sel);

};

#endif // !defined(AFX_WINGUIPROXY_H__7C73CD17_C566_11D6_9554_00047660C89A__INCLUDED_)
