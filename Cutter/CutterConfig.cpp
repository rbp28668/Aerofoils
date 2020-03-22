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
#include "CutterConfig.h"
#include "fstream"
#include "../Kernel/ObjectSerializer.h"

using namespace std;


CutterConfig::CutterConfig() :
	  cutterWidth(1000)
	, blockLeft(100)
	, blockRight(900)
	, defaultFeedRate(5)
	, defaultComPort("COM1")
	, connectAutomatically(false)
	, defaultListenPort(8193)
	, listenAutomatically(false)
	, xScrewLead(2.0)
	, yScrewLead(2.0)
	, xStepsPerRev(200)
	, yStepsPerRev(200)
	, xMicroStepping(4)
	, yMicroStepping(4)
	, stepFrequency(2000)
{
}


CutterConfig::~CutterConfig()
{
}

bool CutterConfig::save(std::string& path)
{
	bool success = true;
	try	{
		fstream ofs(path, ios_base::out);
		CObjectSerializer serializer(&ofs);
		serializeTo(serializer);
	} catch (...) {
		success = false;
	}
	return success;
}

bool CutterConfig::load(std::string& path)
{
	bool success = true;
	try	{
		fstream ifs(path, ios_base::in);
		CObjectSerializer serializer(&ifs);
		if (ifs.good()) {
			serializeFrom(serializer);
		}
	} catch (...) {
		success = false;
	}
	return success;

}

void CutterConfig::serializeTo(CObjectSerializer & os)
{
	os.startSection("config", this);
	os.write("cutterWidth", cutterWidth);
	os.write("blockLeft", blockLeft);
	os.write("blockRight", blockRight);
	os.write("defaultFeedRate", defaultFeedRate);
	os.write("xLead", xScrewLead);
	os.write("xStepsPerRev", xStepsPerRev);
	os.write("xMicroStepping", xMicroStepping);
	os.write("yLead", yScrewLead);
	os.write("yStepsPerRev", yStepsPerRev);
	os.write("yMicroStepping", yMicroStepping);
	os.write("stepFrequency", stepFrequency);
	os.write("defaultComPort", defaultComPort.c_str());
	os.write("connectAutomatically", connectAutomatically);
	os.write("defaultListenPort", defaultListenPort);
	os.write("listenAutomatically", listenAutomatically);
	os.startCollection("buttons", BUTTON_COUNT);
	for (int i = 0; i < BUTTON_COUNT; ++i) {
		os.startSection("button", buttons + i);
		os.write("label", buttons[i].label.c_str());
		os.write("code", buttons[i].code.c_str());
		os.endSection();
	}
	os.endCollection();
	os.endSection();
}

void CutterConfig::serializeFrom(CObjectSerializer & os)
{
	os.startReadSection("config", this);
	os.read("cutterWidth", cutterWidth);
	os.read("blockLeft", blockLeft);
	os.read("blockRight", blockRight);
	os.read("defaultFeedRate", defaultFeedRate);
	os.read("xLead", xScrewLead);
	os.read("xStepsPerRev", xStepsPerRev);
	os.read("xMicroStepping", xMicroStepping);
	os.read("yLead", yScrewLead);
	os.read("yStepsPerRev", yStepsPerRev);
	os.read("yMicroStepping", yMicroStepping);
	os.read("stepFrequency", stepFrequency);
	os.read("defaultComPort", defaultComPort);
	os.read("connectAutomatically", connectAutomatically);
	os.read("defaultListenPort", defaultListenPort);
	os.read("listenAutomatically", listenAutomatically);

	if (os.ifExists("buttons")) {
		int values = os.startReadCollection("buttons");
		for (int i = 0; i < values; ++i) {
			std::string label;
			std::string code;
			os.startReadSection("button", buttons + i);
			os.read("label", label);
			os.read("code", code);
			os.endReadSection();
			if (i < BUTTON_COUNT) {
				buttons[i].label = label;
				buttons[i].code = code;
			}

		}
		os.endReadCollection();
	}
	os.endReadSection();
}
