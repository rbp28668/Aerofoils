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
