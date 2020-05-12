#include "CNCConnectionOutputDevice.h"
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
// CNCConnectionOutputDevice.cpp: implementation of the CCNCConnectionOutputDevice class.
//
//////////////////////////////////////////////////////////////////////


#include <assert.h>
#include <sstream>
#include "aerofoil.h"
#include "CNCConnectionOutputDevice.h"
#include "PointT.h"
#include "PlotStructure.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCNCConnectionOutputDevice::CCNCConnectionOutputDevice(const char* address, unsigned int port)
{
	_isCNC = true;
	link = new CNCSocket(address, port);
}

CCNCConnectionOutputDevice::CCNCConnectionOutputDevice(GCodeOutputDevice::GCodeConfig* pConfig, CutterGeometry* pGeometry, const char* address, unsigned int port)
	: GCodeOutputDevice(pConfig, pGeometry)
{
	_isCNC = true;
	link = new CNCSocket(address, port);
}

CCNCConnectionOutputDevice::~CCNCConnectionOutputDevice()
{
	delete link;
}

void CCNCConnectionOutputDevice::send(const std::string & str)
{
	link->send(str);
}


