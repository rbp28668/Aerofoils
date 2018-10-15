// BackgroundGrid.cpp: implementation of the CBackgroundGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "aerofoil.h"
#include "BackgroundGrid.hpp"

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
