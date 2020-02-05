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
#include "SerialLink.h"
#include <string>
#include <fstream>

// #define OUTPUT_TO_FILE 1
class CutterHardware {

private:
#ifdef OUTPUT_TO_FILE
	std::ofstream ofs;
#endif
	SerialLink serialLink;
	bool queueFull;
	bool queueEmpty;

	int processResponse();
	int send(const std::string& msg);
	int queueCommand(const std::string& msg);
public:
	// Bitmask for return values.  Note LS 4 bits has the 
	// status of the limit switches for the 4 axes.
	enum class StatusT {
		VALID = 0x10,
	    FULL = 0x20,
		EMPTY = 0x40,
		COMMS = 0x80
	};

	// Inline convenience functions for pulling apart the return codes.
	static inline bool isValid(int status) { return (status & (int)StatusT::VALID) != 0; }
	static inline bool isQueueFull(int status) { return (status & (int)StatusT::FULL) != 0; }
	static inline bool isQueueEmpty(int status) { return (status & (int)StatusT::EMPTY) != 0; }
	static inline bool isCommsError(int status) { return (status & (int)StatusT::COMMS) != 0; }
	static inline int limitSwitches(int status) { return (status & 0x0F); }


	typedef __int32 AxisT;

	CutterHardware();
	CutterHardware(const char* pszPort);

	int connect(const char* pszPort);
	void disconnect();
	boolean isConnected();

	// Commands
	// P - PING to see state of queue
	int ping();

	// ? - Help
	// Not implemented

	// D - Disable motors
	int disable();

	// E - Enable motors
	int enable();

	// Z - Abort
	int abort();

	// A - Abort NOW
	int abortNow();

	// H - Home
	int home();

	// L - Line in 4 dimensions SSSSSSSSAAAAAAAABBBBBBBBCCCCCCCCDDDDDDDD where S gives total number of steps and A-D are axes. All in hex, 2s complement
	int line(AxisT steps, AxisT lx, AxisT ly, AxisT rx, AxisT ry);

	// S - Single step - DB where D is 4 bits of direction, P are pulse bits (1 to pulse), both hex digits
	int step(int direction, int pulse);

	// W - Turn on hot wire
	int wireOn();

	// X - Turn off hot wire
	int wireOff();

	// Response
	// outputBuffer[0] = ok ? 'Y' : 'N';
	// outputBuffer[1] = commandQueue.isFull() ? 'F' : '-';
	// outputBuffer[2] = commandQueue.isEmpty() ? 'E' : '-';
	// outputBuffer[3] = 0;

};