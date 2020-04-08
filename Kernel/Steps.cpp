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

#include <assert.h>
#include "Steps.h"


// Class to encapsulate the core of Brenenham's line drawing algorithm.  
// Once started, call step repeatedly, moving that axis if step returns true.
Steps::Steps() :
	dist(0),
	steps(0),
	count(0),
	err(0)
{
}


Steps::~Steps()
{
}

void Steps::start(long long dist, long long steps) {
	assert(dist >= 0);
	assert(steps >= 0);
	assert(dist <= steps);

	this->dist = dist;
	this->steps = steps;
	count = steps;
	err = 2 * dist - steps;
}

bool Steps::isRunning() {
	return count > 0;
}

bool Steps::step() {
	bool doStep = false;
	--count;
	if (err > 0) {
		doStep = true;
		err = err - 2 * steps;
	}
	err = err + 2 * dist;

	return doStep;
}

void Steps::stop() {
	dist = 0;
	steps = 0;
	count = 0;
	err = 0;
}


