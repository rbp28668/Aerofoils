// CNCConnectionOutputDevice.h: interface for the CCNCConnectionOutputDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CNCCONNECTIONOUTPUTDEVICE_H__B5325B57_C8EA_11D6_955D_000000000000__INCLUDED_)
#define AFX_CNCCONNECTIONOUTPUTDEVICE_H__B5325B57_C8EA_11D6_955D_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786) // > 255 chars in debug

#include <string>
#include <map>
#include <afxsock.h>
#include "kernel/OutputDevice.h"

class CPlotStructure;

class CCNCConnectionOutputDevice : public COutputDevice
{
public:
	CCNCConnectionOutputDevice(const char* address, unsigned int port);
	virtual ~CCNCConnectionOutputDevice();

	virtual void MoveTo(int iStream, const PointT& pt);
	virtual void LineTo(int iStream, const PointT& pt);
	virtual void Label(int iStream, const char* psz);
	virtual void Home();
	virtual void Flush();

	// These are optional but can provide hints to the output device.
	virtual void startObject(CPlotStructure*);
	virtual void endObject(CPlotStructure*);
	virtual void startPlot();
	virtual void endPlot();

private:

	class CCNCSocket : public CAsyncSocket
	{
	public:
		CCNCSocket()
			:_canSend(false)
			,connected(false)
		{}

		virtual void OnAccept(int nErrorCode);
		virtual void OnClose(int nErrorCode);
		virtual void OnConnect(int nErrorCode);
		virtual void OnOutOfBandData(int nErrorCode);
		virtual void OnReceive(int nErrorCode);
		virtual void OnSend(int nErrorCode);

		bool canSend() const {return _canSend;}
		virtual int Send( const void* lpBuf, int nBufLen, int nFlags = 0 );

		bool isConnected() const {return connected;}

		void waitToSend();
		void waitForConnection();

	private:
		bool _canSend;
		bool connected;
		void waitUntil(volatile bool* flag);
		
	};
	void send(const std::string& msg);
	void setupErrorMap();
	const char* translate(int nErr);

	CCNCSocket link; 

	static bool socketsInitialised;

	typedef std::map<int,char*> ERRORS;
	ERRORS errors;

};

#endif // !defined(AFX_CNCCONNECTIONOUTPUTDEVICE_H__B5325B57_C8EA_11D6_955D_000000000000__INCLUDED_)
