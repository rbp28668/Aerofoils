
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

#include<assert.h>
#include "GCodeInterpreter.h"
#include "GCodeProgram.h"
#include "ObjectSerializer.h"
#include "trim.h"


GCodeProgram::GCodeProgram(GCodeInterpreter* pi) :
	pInterpreter(pi),
	bPaused(false),
	bComplete(false),
	bRunning(false),
	bPausable(false),
	upstreamContext(0)
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

void GCodeProgram::unsetInterpreter()
{
	assert(this);
	this->pInterpreter = 0;
}

void GCodeProgram::setUpstreamContext(ParserContext * upstreamContext)
{
	this->upstreamContext = upstreamContext;
}

void GCodeProgram::unsetUpstreamContext()
{
	this->upstreamContext = 0;
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
	if (upstreamContext) {
		upstreamContext->showError(line, where, msg);
	}
}

void GCodeProgram::showLine(const std::string& line)
{
	if (upstreamContext) {
		upstreamContext->showLine(line);
	}
}

bool GCodeProgram::canPause() {
	if (upstreamContext) {
		upstreamContext->canPause();
	}
	return bPausable;
}

void GCodeProgram::pause()
{
	bPaused = true;
	if (upstreamContext) {
		upstreamContext->pause();
	}

}

void GCodeProgram::complete()
{
	assert(this);
	bComplete = true;
	if (upstreamContext) {
		upstreamContext->complete();
	}

}

void GCodeProgram::restart()
{
	assert(this);
	bRunning = true;
	bComplete = false;
	bPaused = true;
	errors.clear();
	currentLine = lines.begin();
	if (upstreamContext) {
		upstreamContext->restart();
	}

}

void GCodeProgram::clear()
{
	assert(this);
	lines.clear();
	errors.clear();
	bRunning = false;
	bComplete = false;
	bPaused = false;
	currentLine = lines.end(); // will abort any runnign program.
}

void GCodeProgram::add(const std::string & line)
{
	lines.push_back(line);
}

std::string GCodeProgram::nextLine()
{
	if (currentLine == lines.end()) {
		return std::string("");
	}
	return *currentLine;
}

void GCodeProgram::start()
{
	assert(this);
	assert(pInterpreter);
	
	pInterpreter->setContext(this);
	errors.clear();
	currentLine = lines.begin();
	bRunning = true;
	bComplete = false;
	bPaused = true;
}

bool GCodeProgram::step()
{
	assert(this);
	assert(pInterpreter);
	if (currentLine != lines.end()) {
		try {
			pInterpreter->process(*currentLine);
			++currentLine;
			if (currentLine == lines.end()) {
				bComplete = true;
				bPaused = false;
				bRunning = false;
			}
		}
		catch (std::exception & e) {
			errors.push_back(e.what());
			if (upstreamContext) {
				upstreamContext->showError(*currentLine,0, e.what());
			}

			bComplete = true;
			bPaused = false;
			bRunning = false;
		}
	}
	return !bComplete;
}

void GCodeProgram::reset()
{
	assert(this);
	bComplete = false;
	bPaused = false;
	bRunning = false;
	currentLine = lines.end();
}

void GCodeProgram::clearErrors()
{
	errors.clear();
}

bool GCodeProgram::hasError()
{
	return !errors.empty();
}

std::string GCodeProgram::popError()
{
	std::string err = std::string();
	if (!errors.empty()) {
		err = errors.front();
		errors.pop_front();
	}
	return err;
}

void GCodeProgram::serializeTo(CObjectSerializer & os)
{
	assert(this);
	os.startSection("gcodeProgram", this);
	os.startCollection("lines", (int)lines.size());
	for (std::vector<std::string>::iterator iter = lines.begin();
		iter != lines.end();
		++iter) {
		os.write("line", iter->c_str());
	}
	os.endCollection();
	os.endSection();
}

void GCodeProgram::serializeFrom(CObjectSerializer & os)
{
	assert(this);
	os.startReadSection("gcodeProgram", this);
	int nLines = os.startReadCollection("lines");
	for(int i=0; i<nLines; ++i){
		std::string line;
		os.read("line", line);
		lines.push_back(line);
	}
	os.endReadCollection();
	os.endReadSection();

}

void GCodeProgram::load(std::istream & is)
{
	assert(this);
	assert(is.good());
	std::string line;
	while (std::getline(is, line)) {
		trim(line);
		lines.push_back(line);
	}
	bRunning = false;
	bComplete = false;
	bPaused = false;
}

void GCodeProgram::save(std::ostream & os)
{
	assert(this);
	assert(os.good());
	for (auto const& line : lines) {
		os << line << std::endl;
	}
}

void GCodeProgram::asString(std::string & str)
{
	assert(this);
	str.erase();
	for (auto const&l : lines) {
		str.append(l);
		str.append("\r\n");
	}
}
