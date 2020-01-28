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
*/// UIProxy.cpp: implementation of the CUIProxy class.
//
//////////////////////////////////////////////////////////////////////
#include <assert.h>
#include "UIProxy.h"
#include "ObjectSerializer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIProxy::CUIProxy()
{

}

CUIProxy::~CUIProxy()
{

}

// gets the name of the class - override this if you want to use
// the default serialization & there is no extra data to serialize,
// otherwise, override serializeTo and serializeFrom.
const char* CUIProxy::className() const
{
	assert(this);
	assert(false);  // This should never be called - only derived classes
	return "UNKNOWN";
}

void CUIProxy::serializeTo(CObjectSerializer& os)
{
	assert(this);
	os.startSection(className(), this);
	os.endSection();
}

void CUIProxy::serializeFrom(CObjectSerializer& os)
{
	assert(this);
	os.startReadSection(className(), this);
	os.endReadSection();
}
