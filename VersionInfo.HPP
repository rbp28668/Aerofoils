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

