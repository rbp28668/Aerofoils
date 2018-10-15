/*
File:               Postscript.h

Program/rev.:       Aerofoil 0.0

Function:           Postscript printer driver

Author:             Bruce

Date:               25 Oct 91

Routines:

Input Parameters:

Output Parameters:

Error conditions:

Description:

Revsision History:

*/




#include <assert.h>
#include "Postscript.h"

CPostscriptOutputDevice::CPostscriptOutputDevice(const char* path)
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

CPostscriptOutputDevice::~CPostscriptOutputDevice()
{
	if(lineOpen)
		EndLine();

	EndPage();
	EndDocument();
}

void CPostscriptOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	if(lineOpen)
		EndLine();

	lastStream = iStream;
	last[iStream] = pt;
}

void CPostscriptOutputDevice::LineTo(int iStream, const PointT& pt)
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

void CPostscriptOutputDevice::Label(int iStream, const char* psz)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);
	assert(psz);

	if(lineOpen)
		EndLine();

	Label(last[iStream],psz);
}

void CPostscriptOutputDevice::Home()
{
	assert(this);

	if(lineOpen)
		EndLine();

	last[0] = last[1] = PointT(0,0);
	lastStream = 0;


}

void CPostscriptOutputDevice::Flush()
{
	assert(this);
}


/************************************************************
* CPostscriptOutputDevice::StartDocument
* 
************************************************************/
void CPostscriptOutputDevice::StartDocument()
{
  
    os << "%%!PS-Adobe-2.0\n";
    os << "%%%%Creator: LaserPlot 0.0\n";
    os << "%%%%EndComments\n";
  
    os << "/Times-Roman findfont 10 scalefont setfont\n";
    os << "0 setlinewidth\n";  /* thin as possible */
    os << "/L {lineto} def\n"; /* shorthand for lineto */
    os << "/M {moveto} def\n"; /* ditto for moveto */
    os << "/in {72 mul} def\n";  /* inch scale */
    os << "90 rotate ";      /* set up landscape */
    os << "0.25 in -7 in translate\n"; /* and drop 0,0 to bl corner */
}

/************************************************************
* CPostscriptOutputDevice::StartPage
* 
************************************************************/
void CPostscriptOutputDevice::StartPage()
{
  // NOP
}

/************************************************************
* CPostscriptOutputDevice::StartLine
* 
************************************************************/
void CPostscriptOutputDevice::StartLine(const PointT& ptInitial)
{ 
    os << "newpath " << ptInitial.fx << " " << ptInitial.fy << " M\n";
}

/************************************************************
* CPostscriptOutputDevice::LineSegment
* 
************************************************************/
void CPostscriptOutputDevice::LineSegment(const PointT& pt)
{
  
    os << pt.fx << " " << pt.fy << " L\n";
}

/************************************************************
* CPostscriptOutputDevice::EndLine
* 
************************************************************/
void CPostscriptOutputDevice::EndLine()
{ 
  
    os << "stroke\n";
}

/************************************************************
* CPostscriptOutputDevice::Label
* 
************************************************************/
void CPostscriptOutputDevice::Label(const PointT& pt, const char* psz)
{
    os << pt.fx << " " << pt.fy << " M\n";
    os << "(" << psz << ") show\n";
}

/************************************************************
* CPostscriptOutputDevice::EndPage
* 
************************************************************/
void CPostscriptOutputDevice::EndPage()
{
    os << "showpage\n";
	os.flush();
}

/************************************************************
* CPostscriptOutputDevice::EndDocument
* 
************************************************************/
void CPostscriptOutputDevice::EndDocument()
{
  
    os << "%%End of output\n";
}

