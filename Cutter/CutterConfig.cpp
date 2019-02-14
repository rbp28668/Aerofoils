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
	os.endReadSection();
}
