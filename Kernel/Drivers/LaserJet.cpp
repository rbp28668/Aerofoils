/* Aerofoil
Aerofoil plotting and CNC cutter driver
Kernel / core algorithms
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
*//*
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

PointT CLaserJetOutputDevice::position(int iStream)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	return last[iStream];
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


