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
#include "ParserContext.h"

class GCodeInterpreter;

class GCodeProgram :
	public ParserContext
{
private:
	GCodeInterpreter* pInterpreter;
	std::vector<std::string>  lines;
	std::vector<std::string>  errors;
	std::vector<std::string>::iterator currentLine;
	boolean bPaused;
	boolean bComplete;
	boolean bPausable;

public:
	GCodeProgram(GCodeInterpreter* pInterpreter = 0);
	virtual ~GCodeProgram();

	void setInterpreter(GCodeInterpreter* pInterpreter);

	// Parser Context
	virtual void showError(const std::string& line, size_t where, const std::string& msg);
	virtual boolean canPause();
	virtual void pause();
	virtual void complete();
	virtual void restart();

	// Managing program
	void clear(); 
	void add(const std::string& line);

	// Running program
	inline boolean isPaused() { return bPaused; }
	inline boolean isComplete() { return bComplete; }
	inline void setPausable(boolean pausable) { bPausable = pausable; }

	void start();	// before run or step.
	boolean run();  // until paused or complete, true if there's still stuff to do
	boolean step();  // one program line, true if there's still stuff to do 

};

