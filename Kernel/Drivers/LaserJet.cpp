/*
File:              LaserJet.cpp

Program/rev.:      Aerofoil 3.0

Function:          To plot on a HP Laserjet 

Author:            Bruce

Date:              Nov 91

Routines:

Input Parameters:

Output Parameters:

Error conditions:

Description:

Revision History:	Modified for Windows version 2002.

*/

#include <assert.h>
#include "laserjet.h"

CLaserJetOutputDevice::CLaserJetOutputDevice(const char* path)
: lineOpen(false)
, firstSegment(true)
, lastStream(0)
{
	assert(this);
	assert(path);

	os.open(path);
	StartDocument();
	StartPage();
}

CLaserJetOutputDevice::~CLaserJetOutputDevice()
{
	if(lineOpen)
		EndLine();

	EndPage();
	EndDocument();
}

void CLaserJetOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if(lineOpen)
		EndLine();

	lastStream = iStream;
	last[iStream] = pt;
}

void CLaserJetOutputDevice::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if( (iStream != lastStream) && lineOpen)
			EndLine();

	if(!lineOpen)
		StartLine(last[iStream]);

	LineSegment(pt);

	lastStream = iStream;
	last[iStream] = pt;
}

void CLaserJetOutputDevice::Label(int iStream, const char* psz)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	assert(psz);

	if(lineOpen)
		EndLine();

	Label(last[iStream],psz);
}

void CLaserJetOutputDevice::Home()
{
	assert(this);

	if(lineOpen)
		EndLine();

	last[0] = last[1] = PointT(0,0);
	lastStream = 0;


}

void CLaserJetOutputDevice::Flush()
{
	assert(this);
}


///////////////////////////////////////////////////////////////////////////////

/************************************************************
* CLaserJetOutputDevice::StartDocument
* 
************************************************************/
void CLaserJetOutputDevice::StartDocument()
{
	assert(this);
	reset();
}

/************************************************************
* CLaserJetOutputDevice::StartPage
* 
************************************************************/
void CLaserJetOutputDevice::StartPage()
{
    hpgl2();
    land();
    mm_scale();
    os << "SP1";          /* Select Pen 1*/
    os << "PW0.1";        /* Pen width 0.1mm */
}

/************************************************************
* CLaserJetOutputDevice::StartLine
* 
************************************************************/
void CLaserJetOutputDevice::StartLine(const PointT& ptInitial)
{ 
    os << "PU" << ptInitial.fx << "," << ptInitial.fy;
    os << "PD";
	firstSegment = true;
	lineOpen = true;
}

/************************************************************
* CLaserJetOutputDevice::LineSegment
* 
************************************************************/
void CLaserJetOutputDevice::LineSegment(const PointT& pt)
{
	if(!firstSegment)
		os << ",";
    os << pt.fx << "," << pt.fy;
	firstSegment = false;
}

/************************************************************
* CLaserJetOutputDevice::EndLine
* 
************************************************************/
void CLaserJetOutputDevice::EndLine()
{ 
    os << ";";
	lineOpen = false;
}

/************************************************************
* CLaserJetOutputDevice::Label
* 
************************************************************/
void CLaserJetOutputDevice::Label(const PointT& pt, const char* psz)
{   
	assert(this);
	assert(psz);
    os << "PU" << pt.fx << "," << pt.fy;
    os << "LB";
	os << psz << "\003";
}

/************************************************************
* CLaserJetOutputDevice::EndPage
* 
************************************************************/
void CLaserJetOutputDevice::EndPage()
{
    os << ";";
    reset();
}

/************************************************************
* CLaserJetOutputDevice::EndDocument
* 
************************************************************/
void CLaserJetOutputDevice::EndDocument()
{
    pcl();
}


