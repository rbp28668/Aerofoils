// CutterSimulationOutputDevice.cpp: implementation of the CCutterSimulationOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "aerofoil.h"
#include "CutterSimulationOutputDevice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CCutterSimulationOutputDevice::CCutterSimulationOutputDevice(float sx, float sy, CDC* pdc, float zoom)
: map(sx, sy, pdc, zoom)
, plotDC(pdc)
, hasRoot(false)
, hasTip(false)
{
	_isCNC = true;

	for(int i=0;i<AXIS_COUNT;++i)
		axis[i] = last[i] = 0;
}

CCutterSimulationOutputDevice::~CCutterSimulationOutputDevice()
{
}

void CCutterSimulationOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	LineTo(iStream,pt);
}

void CCutterSimulationOutputDevice::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	CPoint pos = map.toDevice(pt);
	if(iStream == 0)
	{
		hasRoot = true;
		axis[0] = pos.x;
		axis[1] = pos.y;
	}
	else
	{
		hasTip = true;
		axis[2] = pos.x;
		axis[3] = pos.y;
	}


	if(hasRoot && hasTip) // then a full set of points
	{

		long i;
		int j;
		long dx;
		long error[AXIS_COUNT];
		long dy[AXIS_COUNT];
		int dirn[AXIS_COUNT];
		long pos[AXIS_COUNT];

		BOOL blAbort = FALSE;
		
		long dist = 0;

		for (i=0;i<AXIS_COUNT;++i)
		{
			pos[i] = last[i];			// where we're coming from
			dy[i] = axis[i] - last[i];	// convert to relative move
			last[i] = axis[i];			// and save new current position.
			
			// Make the dy values all +ve and set direction flags
			if(dy[i] < 0)
			{
				dy[i] = -dy[i];
				dirn[i] = -1;
			}
			else
				dirn[i] = 1;
		
			if(dy[i] > dist)
				dist = dy[i];

			// Double the dy values for bresenham                   
			dy[i] *= 2l;
		}

		
		// And initialise the error terms
		for (i=0;i<AXIS_COUNT;++i)
		{
			error[i] = -dist;
		}

		
		dx = 2 * dist;
		
		// Go ahead and cut
		for (i=0;i<dist;++i)
		{
			//plotDC->SetPixel(pos[0],pos[1],RGB(0,0,0));
			//plotDC->SetPixel(pos[2],pos[3],RGB(0,0,0));
			for (j=0;j<AXIS_COUNT;++j)
			{
				error[j] += dy[j];
				if(error[j] > 0)
				{
					pos[j] += dirn[j];
					error[j] -= dx;
				}
			}
			if(pos[0] == pos[2] && pos[1] == pos[3])
				plotDC->SetPixel(pos[0],pos[1],RGB(255,0,255));
			else
			{
				plotDC->SetPixel(pos[0],pos[1],RGB(255,0,0));
				plotDC->SetPixel(pos[2],pos[3],RGB(0,0,255));
			}

			DWORD now = ::GetTickCount();
			while(::GetTickCount() == now)
			{
				// NOP;
			}
				
		}

		hasRoot = false; // as have now plotted
		hasTip = false;
	}

}

void CCutterSimulationOutputDevice::Label(int iStream, const char* psz)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	// NOP cutters don't do labels.
}

void CCutterSimulationOutputDevice::Home()
{
	assert(this);
	for(int i=0;i<AXIS_COUNT;++i)
		axis[i] = last[i] = 0;

	hasRoot = false;
	hasTip = false;
}

void CCutterSimulationOutputDevice::Flush()
{
	assert(this);
}


void CCutterSimulationOutputDevice::startObject(CPlotStructure* ps)
{
}

void CCutterSimulationOutputDevice::endObject(CPlotStructure* ps)
{
}
