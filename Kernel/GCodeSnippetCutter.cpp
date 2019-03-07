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
#include "GCodeSnippetCutter.h"
#include "GCodeSnippet.h"
#include "OutputDevice.h"
#include "PlottingCutter.h"


GCodeSnippetCutter::GCodeSnippetCutter(GCodeSnippet* pgcode)
	: pSnippet(pgcode)
{
	assert(this);
	assert(pgcode);
}


GCodeSnippetCutter::~GCodeSnippetCutter()
{
}

void GCodeSnippetCutter::cut(COutputDevice * pdev, double toolOffset)
{
	assert(this);
	assert(pdev);

	if (pdev->isCNC()) {
		std::string text;
		pSnippet->programText(text);
		pdev->passthrough(text.c_str());
	}	else {
		PlottingCutter cutter(pdev);
		PointT left = pdev->position(0);
		PointT right = pdev->position(1);
		cutter.setInitialPosition(left.fx, left.fy, right.fx, right.fy);
		pSnippet->runWith(&cutter);
	}
}

std::string GCodeSnippetCutter::getDescriptiveText() const
{
	assert(this);
	return std::string("G-Code snippet");
}

CStructure * GCodeSnippetCutter::getStructure()
{
	assert(this);
	return pSnippet;
}

const CStructure * GCodeSnippetCutter::getStructure() const
{
	assert(this);
	return pSnippet;
}
