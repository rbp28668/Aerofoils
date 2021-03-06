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
#include <string>

class CObjectSerializer;
struct CutterConfig
{
	double cutterWidth;
	double blockLeft;
	double blockRight;
	double defaultFeedRate;
	std::string defaultComPort;
	bool connectAutomatically;
	int defaultListenPort;
	bool listenAutomatically;
	double xScrewLead;
	double yScrewLead;
	int xStepsPerRev;
	int yStepsPerRev;
	int xMicroStepping;
	int yMicroStepping;
	double stepFrequency;

	struct Button {
		std::string label;
		std::string code;
	};
	
	const static int BUTTON_COUNT = 16;
	Button buttons[BUTTON_COUNT];

public:
	CutterConfig();
	~CutterConfig();

	bool save(std::string& path);
	bool load(std::string& path);

private:
	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

};

