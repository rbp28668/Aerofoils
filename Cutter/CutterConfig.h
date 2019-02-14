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

	
public:
	CutterConfig();
	~CutterConfig();

	bool save(std::string& path);
	bool load(std::string& path);

private:
	void serializeTo(CObjectSerializer& os);
	void serializeFrom(CObjectSerializer& os);

};

