/* Aerofoil
Aerofoil plotting and CNC cutter driver
Copyright(C) 1995-2019 R Bruce Porteous

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

	void sendMove(const char* pszCommand);

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

	double x;
	double y;
	double u;
	double v;

	bool hasLeft;
	bool hasRight;


};

#endif // !defined(AFX_CNCCONNECTIONOUTPUTDEVICE_H__B5325B57_C8EA_11D6_955D_000000000000__INCLUDED_)
