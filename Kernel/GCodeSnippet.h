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

#pragma once
#include "Structure.h"

class GCodeProgram;
class Cutter;

// Structure object that just contains a snippet of GCode which
// is then passed unmolested to the output.
class GCodeSnippet :
	public CStructure
{
	GCodeProgram* pProgram;

public:
	static const std::string TYPE;

	GCodeSnippet();
	GCodeSnippet(const char* content);
	~GCodeSnippet();

	virtual void serializeTo(CObjectSerializer& os);
	virtual void serializeFrom(CObjectSerializer& os);

	virtual std::string getDescriptiveText() const;
	virtual std::string getType() const;

	void programText(std::string& str) const;
	void updateProgram(const char* content);
	bool validateProgram();
	bool hasErrors() const;
	std::string getFirstError();
	void runWith(Cutter* pCutter);
};

