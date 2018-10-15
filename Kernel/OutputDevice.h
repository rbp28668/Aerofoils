// OutputDevice.h: interface for the COutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTDEVICE_H__A4E22B63_B474_11D6_AF75_002018BA082E__INCLUDED_)
#define AFX_OUTPUTDEVICE_H__A4E22B63_B474_11D6_AF75_002018BA082E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PointT;
class CPlotStructure;

class COutputDevice  
{
public:
	COutputDevice();
	virtual ~COutputDevice();

	virtual void MoveTo(int iStream, const PointT& pt) = 0;
	virtual void LineTo(int iStream, const PointT& pt) = 0;
	virtual void Label(int iStream, const char* psz) = 0;
	virtual void Home() = 0;
	virtual void Flush() = 0;

	// These are optional but can provide hints to the output device.
	virtual void startObject(CPlotStructure*) {}
	virtual void endObject(CPlotStructure*) {}
	virtual void startPlot() {}
	virtual void endPlot() {}

	// Information about the device;
	bool isCNC() const {return _isCNC;}
	//virtual PointT getResolutionPixPerMM() const;


protected:
	enum {MoveToCode, LineToCode, LabelCode, HomeCode, FlushCode};
	bool _isCNC;

};

#endif // !defined(AFX_OUTPUTDEVICE_H__A4E22B63_B474_11D6_AF75_002018BA082E__INCLUDED_)
