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
/************************************************************************
// File:        VersionInfo.HPP
// Class:       CVersionInfo
// Description: 
//              
// Date:        12 February 98
// Author:      Bruce Porteous
************************************************************************/
#ifndef _VersionInfo_HPP
#define _VersionInfo_HPP

class CVersionInfo
{
  BOOL m_blValid;           // true if we have valid version info.
  LPVOID m_pvVerInfo;       // pointer to version info.
  WORD m_wLang;             // language to get resources
  WORD m_wCharset;          // character set to display it

  CString m_strKeyBase;     // string for querying resource

  LPCTSTR m_pszFileVersion;
  LPCTSTR m_pszDescription;
  LPCTSTR m_pszProductName;
  LPCTSTR m_pszProductVersion;
  LPCTSTR m_pszCopyright;
  VS_FIXEDFILEINFO* m_pvsffi;

  UINT m_uiSizeFileVersion;
  UINT m_uiSizeDescription;
  UINT m_uiSizeProductName;
  UINT m_uiSizeProductVersion;
  UINT m_uiSizeCopyright;
  UINT m_uiSizeFixedFileInfo;

  CString ToString(LPCTSTR psz, UINT uiLen);

  public:
  CVersionInfo();
  ~CVersionInfo();

  CString FileVersion();
  CString Description();
  CString ProductName();
  CString ProductVersion();
  CString Copyright();
  CString Resource(LPCTSTR pszResource);

};

#endif

