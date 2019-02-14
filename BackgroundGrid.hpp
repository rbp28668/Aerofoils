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
// BackgroundGrid.hpp: interface for the CBackgroundGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDGRID_HPP__4F2DDB9F_1647_44A7_AB0A_827A7BC35962__INCLUDED_)
#define AFX_BACKGROUNDGRID_HPP__4F2DDB9F_1647_44A7_AB0A_827A7BC35962__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Class to describe a background grid for printing.  Note that this (currently) is not saved
// as part of the plot - if it is it will need to be moved to kernel.

class CBackgroundGrid  
{
public:
	CBackgroundGrid();
	virtual ~CBackgroundGrid();

	inline float getHorizontalSize() { return m_horizontalSpacing; }
	inline float getVerticalSize() { return m_verticalSpacing; }
	inline bool isHorizontalEnabled() { return m_plotHorizontal;}
	inline bool isVerticalEnabled() { return m_plotVertical;}

	inline void setHorizontalSize(float size) { m_horizontalSpacing = size; }
	inline void setVerticalSize(float size) { m_verticalSpacing = size; }
	inline void enableHorizontal(bool on) { m_plotHorizontal = on;}
	inline void enableVertical(bool on) { m_plotVertical = on;}


private:
	bool m_plotHorizontal;
	bool m_plotVertical;
	float m_horizontalSpacing;
	float m_verticalSpacing;
};

#endif // !defined(AFX_BACKGROUNDGRID_HPP__4F2DDB9F_1647_44A7_AB0A_827A7BC35962__INCLUDED_)
