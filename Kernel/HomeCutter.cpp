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

#include <assert.h>
#include "HomeCutter.h"
#include "OutputDevice.h"
#include "ObjectSerializer.h"

const std::string HomeCutter::TYPE("homeCutter");
static CObjectFactory<HomeCutter> factory(HomeCutter::TYPE.c_str());

HomeCutter::HomeCutter()
{
}

HomeCutter::HomeCutter(const HomeCutter& source)
	: CutStructure(source)
{
}


HomeCutter::~HomeCutter()
{
}

void HomeCutter::cut(COutputDevice * pdev, const CutStructure::Context& context)
{
	assert(this);
	assert(pdev);
	pdev->Home();
}

std::string HomeCutter::getDescriptiveText() const
{
	return std::string("Home");
}

std::string HomeCutter::getType() const
{
	return TYPE;
}

CutStructure* HomeCutter::clone() const
{
	return new HomeCutter(*this);
}

CStructure * HomeCutter::getStructure()
{
	return nullptr;
}

const CStructure * HomeCutter::getStructure() const
{
	return nullptr;
}

void HomeCutter::serializeTo(CObjectSerializer & os)
{
	os.startSection(TYPE.c_str(), this);
	os.endSection();
}

void HomeCutter::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection(TYPE.c_str(), this);
	os.endReadSection();
}
