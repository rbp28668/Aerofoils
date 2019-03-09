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
#include "sstream"
#include "GCodeSnippet.h"
#include "GCodeProgram.h"
#include "GCodeInterpreter.h"
#include "DummyCutter.h"
#include "ObjectSerializer.h"

const std::string GCodeSnippet::TYPE("gcodeSnippet");
static CObjectFactory<GCodeSnippet> factory(GCodeSnippet::TYPE.c_str());

GCodeSnippet::GCodeSnippet()
{
	assert(this);

	pProgram = new GCodeProgram();
}

GCodeSnippet::GCodeSnippet(const char* content)
{
	assert(this);
	assert(content);

	pProgram = new GCodeProgram();
	updateProgram(content);
}


GCodeSnippet::~GCodeSnippet()
{
	delete pProgram;
}

void GCodeSnippet::serializeTo(CObjectSerializer & os)
{
	os.startSection(TYPE.c_str(), this);
	CStructure::serializeTo(os);
	pProgram->serializeTo(os);
	os.endSection();
}

void GCodeSnippet::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection(TYPE.c_str(), this);
	CStructure::serializeFrom(os);
	pProgram->serializeFrom(os);
	os.endReadSection();

}

std::string GCodeSnippet::getDescriptiveText() const
{
	return std::string("GCode Snippet");
}

std::string GCodeSnippet::getType() const
{
	return TYPE;
}

void GCodeSnippet::programText(std::string& str) const
{
	pProgram->asString(str);
}

void GCodeSnippet::updateProgram(const char * content)
{
	std::string textStr(content);
	std::istringstream is(textStr);
	pProgram->clear();
	pProgram->load(is);
}

 bool GCodeSnippet::validateProgram()
{
	GCodeInterpreter interpreter;
	interpreter.setContext(pProgram);
	pProgram->setInterpreter(&interpreter);

	DummyCutter dummy;
	interpreter.setCutter(&dummy);

	pProgram->clearErrors();
	pProgram->start();
	while (!pProgram->isComplete()) {
		pProgram->step();
		if (pProgram->isPaused()) {
			pProgram->unpause();
		}
	}

	interpreter.unsetCutter();
	pProgram->unsetInterpreter();
	
	return pProgram->hasError();
}

 bool GCodeSnippet::hasErrors() const
 {
	 assert(this);
	 return pProgram->hasError();
 }

 std::string GCodeSnippet::getFirstError()
 {
	 assert(this);
	 return pProgram->popError();
 }

 void GCodeSnippet::runWith(Cutter * pCutter)
 {
	 assert(this);
	 assert(pCutter);
	 
	 GCodeInterpreter interpreter;
	 interpreter.setContext(pProgram);
	 pProgram->setInterpreter(&interpreter);

	 interpreter.setCutter(pCutter);

	 pProgram->clearErrors();
	 pProgram->start();
	 while (!pProgram->isComplete()) {
		 pProgram->step();
		 if (pProgram->isPaused()) {
			 pProgram->unpause();
		 }
	 }

	 interpreter.unsetCutter();
	 pProgram->unsetInterpreter();

 }
