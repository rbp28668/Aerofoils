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
#include "ComPortEnumerator.h"

#include <vector>
#include <string> 
#include <iostream>

// this can be defined in a separate file
#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif   


ComPortEnumerator::ComPortEnumerator()
{
}


ComPortEnumerator::~ComPortEnumerator()
{
}

void ComPortEnumerator::test() {
	std::vector< tstring > ports;
	detect(ports, 32);

	for (std::vector< tstring >::const_iterator it = ports.begin(); it != ports.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
}

void ComPortEnumerator::detect(std::vector< tstring >& ports, size_t upperLimit = 128)
{
	for (size_t i = 1; i <= upperLimit; i++)
	{
		TCHAR strPort[32] = { 0 };
//		_stprintf(strPort, _T("\\\\.\\COM%d"), i);
		_stprintf(strPort, _T("COM%d"), i);

		COMMCONFIG cfg;
		DWORD dwSize = cfg.dwSize = sizeof(cfg);
		BOOL ret = GetDefaultCommConfig(strPort, &cfg, &dwSize);
		if (ret) ports.push_back(strPort);
	}
}