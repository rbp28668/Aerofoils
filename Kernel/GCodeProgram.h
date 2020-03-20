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

#pragma once
#include <vector>
#include <list>
#include <iostream>
#include "ParserContext.h"

class GCodeInterpreter;
class CObjectSerializer;

class GCodeProgram :
	public ParserContext
{
private:
	GCodeInterpreter* pInterpreter;
	std::vector<std::string>  lines;
	std::vector<std::string>::iterator currentLine;
	std::list<std::string>  errors;
	bool bRunning; // set by start
	bool bPaused;
	bool bComplete;
	bool bPausable;
	ParserContext* upstreamContext;

public:
	GCodeProgram(GCodeInterpreter* pInterpreter = 0);
	virtual ~GCodeProgram();

	void setInterpreter(GCodeInterpreter* pInterpreter);
	void unsetInterpreter();
	void setUpstreamContext(ParserContext* upstreamContext);
	void unsetUpstreamContext();

	// Parser Context
	virtual void showError(const std::string& line, size_t where, const std::string& msg);
	virtual void showLine(const std::string& line);
	virtual bool canPause();
	virtual void pause();
	virtual void complete();
	virtual void restart();

	// Managing program
	void clear(); 
	void add(const std::string& line);
	void load(std::istream& is);
	void save(std::ostream& os);
	void asString(std::string& str);

	// Running program
	inline bool isPaused() { return bPaused; }
	inline bool isComplete() { return bComplete; }
	inline void setPausable(bool pausable) { bPausable = pausable; }
	inline bool isRunning() { return bRunning; }
	inline void unpause() { bPaused = false; }
	std::string nextLine();

	void start();	 // before run or step.
	bool step();  // one program line, true if there's still stuff to do 
	void reset();    // ready for start or run

	// Error reporting
	void clearErrors();
	bool hasError();
	std::string popError();
	
	// Serializing
	void serializeTo(CObjectSerializer & os);
	void serializeFrom(CObjectSerializer & os);
};

