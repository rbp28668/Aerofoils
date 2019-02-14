/* Cutter
Copyright(C) 2019 R Bruce Porteous

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

#include "stdafx.h"
#include<assert.h>
#include "GCodeInterpreter.h"
#include "GCodeProgram.h"


GCodeProgram::GCodeProgram(GCodeInterpreter* pi) : pInterpreter(pi)
{
}


GCodeProgram::~GCodeProgram()
{
}

void GCodeProgram::setInterpreter(GCodeInterpreter* pInterpreter)
{
	assert(this);
	assert(pInterpreter);
	this->pInterpreter = pInterpreter;
}

void GCodeProgram::showError(const std::string & line, size_t where, const std::string & msg)
{
	assert(this);
	std::string error(msg);
	error.append(" : ");
	error.append(line.substr(0, where));
	error.append(">>>");
	error.append(line.substr(where));
	errors.push_back(error);
}

boolean GCodeProgram::canPause() {
	return bPausable;
}

void GCodeProgram::pause()
{
	bPaused = true;
}

void GCodeProgram::complete()
{
	assert(this);
	bComplete = true;
}

void GCodeProgram::restart()
{
	assert(this);
	currentLine = lines.begin();
}

void GCodeProgram::clear()
{
	assert(this);
	lines.clear();
	errors.clear();
}

void GCodeProgram::add(const std::string & line)
{
	lines.push_back(line);
}

void GCodeProgram::start()
{
	assert(this);
	assert(pInterpreter);
	
	pInterpreter->setContext(this);
	errors.clear();
	currentLine = lines.begin();
	bComplete = false;
	bPaused = false;
}

boolean GCodeProgram::run()
{
	assert(this);
	assert(pInterpreter);

	while (step()) {
		if (isPaused()) {
			break;
		}
	}
	return !bComplete;
}

boolean GCodeProgram::step()
{
	if (currentLine == lines.end()) {
		bComplete = true;
	} else {
		pInterpreter->process(*currentLine);
		++currentLine;
	}
	return !bComplete;
}
