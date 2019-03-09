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
// BackgroundGrid.cpp: implementation of the CBackgroundGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aerofoil.h"
#include "BackgroundGrid.hpp"
#include "Kernel/ObjectSerializer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackgroundGrid::CBackgroundGrid()
: m_plotHorizontal(false)
, m_plotVertical(false)
, m_horizontalSpacing(3 * 25.4f)
, m_verticalSpacing(3 * 25.4f)

{

}

CBackgroundGrid::~CBackgroundGrid()
{

}

void CBackgroundGrid::serializeTo(CObjectSerializer & os)
{
	os.startSection("grid", this);
	os.write("plotHorizontal",m_plotHorizontal);
	os.write("plotVertical", m_plotVertical);
	os.write("horizontalSpacing",m_horizontalSpacing);
	os.write("verticalSpacing", m_verticalSpacing);
	os.endSection();
}

void CBackgroundGrid::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection("grid", this);
	os.read("plotHorizontal", m_plotHorizontal);
	os.read("plotVertical", m_plotVertical);
	os.read("horizontalSpacing", m_horizontalSpacing);
	os.read("verticalSpacing", m_verticalSpacing);
	os.endReadSection();
}
