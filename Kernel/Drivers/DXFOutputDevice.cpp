// DXFOutputDevice.cpp: implementation of the CDXFOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "DXFOutputDevice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDXFOutputDevice::CDXFOutputDevice(const char* path)
{
	_isCNC = true;
	os.open(path);
	writeHeader();
}

CDXFOutputDevice::~CDXFOutputDevice()
{
	writeFooter();
	os.close();
}

void CDXFOutputDevice::MoveTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	lastStream = iStream;
	last[iStream] = pt;

}

void CDXFOutputDevice::LineTo(int iStream, const PointT& pt)
{
	assert(this);
	assert(iStream == 0 || iStream == 1);

	os << 0 << endl;
	os << "LINE" << endl;
	os << 8 << endl;  // layer
	os << ((iStream == 0) ? "root" : "tip") << endl;
	os << 10 << endl; // start X
	os << last[iStream].fx << endl;
	os << 20 << endl; // start Y
	os << last[iStream].fy << endl;
	os << 11 << endl; // end X
	os << pt.fx << endl;
	os << 21 << endl; // end Y
	os << pt.fy << endl;

	lastStream = iStream;
	last[iStream] = pt;

}


void CDXFOutputDevice::Label(int iStream, const char* psz)
{
	assert(this);
}

void CDXFOutputDevice::Home()
{
	assert(this);
	last[0] = last[1] = PointT(0,0);
	lastStream = 0;

}

void CDXFOutputDevice::Flush()
{
	assert(this);
}

///////////////////////////////////////////////////////////////////////////////

void CDXFOutputDevice::writeHeader()
{
	assert(this);
	os << 999 << endl;
	os << "DXF File created by Aerofoil" << endl;

	os << 0 << endl;
	os << "SECTION" << endl;
	os << 2 << endl;
	os << "HEADER" << endl;
	os << 0 << endl;
	os << "ENDSEC" << endl;


	os << 0 << endl;
	os << "SECTION" << endl;
	os << 2 << endl;
	os << "BLOCKS" << endl;
	os << 0 << endl;
	os << "ENDSEC" << endl;

	os << 0 << endl;
	os << "SECTION" << endl;
	os << 2 << endl;
	os << "ENTITIES" << endl;

	// Need to write out (geometric) entities ....
}

void CDXFOutputDevice::writeFooter()
{
	assert(this);

	// terminate entities section
	os << 0 << endl;
	os << "ENDSEC" << endl;

	os << 0 << endl;
	os << "EOF" << endl;

}

