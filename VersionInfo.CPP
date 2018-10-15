/************************************************************************
// File:        VersionInfo.CPP
// Class:       CVersionInfo
// Description: 
//              
// Date:        12 February 98
// Author:      Bruce Porteous
************************************************************************/

#include "stdafx.h"
#include "VersionInfo.HPP"
#include <assert.h>

#pragma comment( lib, "version" ) // need to link with version.lib

/************************************************************************
* Function:   CVersionInfo::CVersionInfo
* Constructor for CVersionInfo
* Parameters: NONE
* Returns:    NOTHING
************************************************************************/
CVersionInfo::CVersionInfo()
{
  m_blValid = FALSE;
  m_pvVerInfo = 0;
  m_wLang = 0x0409;   // U.S. English
  m_wCharset = 1252;  // Windows, Multilingual

  m_pszFileVersion = 0;
  m_pszDescription = 0;
  m_pszProductName = 0;
  m_pszProductVersion = 0;
  m_pszCopyright = 0;
  m_pvsffi = 0;

  m_uiSizeFileVersion = 0;
  m_uiSizeDescription = 0;
  m_uiSizeProductName = 0;
  m_uiSizeProductVersion = 0;
  m_uiSizeCopyright = 0;
  m_uiSizeFixedFileInfo = 0;  

  char szFullPath[256];
  if(::GetModuleFileName(AfxGetInstanceHandle( ),szFullPath,sizeof(szFullPath)) != 0)
    {
    DWORD dwHandle = 0;
    DWORD dwVerInfoSize = ::GetFileVersionInfoSize(szFullPath,&dwHandle);

    if(dwVerInfoSize > 0)
      {
      m_pvVerInfo = LPVOID(new BYTE[dwVerInfoSize]);
      m_blValid = ::GetFileVersionInfo(szFullPath, dwHandle, dwVerInfoSize, m_pvVerInfo);
      }
    }
          
  // Set up a default translation int uiLang & uiCharset
  if(m_blValid)
    {
    WORD* pwTranslation = 0;
    UINT uiSize = 0;
    if(::VerQueryValue(m_pvVerInfo, _T("\\VarFileInfo\\Translation"), (void**)&pwTranslation, &uiSize) && uiSize)
      {
      // use the first language in the block
      m_wLang = pwTranslation[0];
      m_wCharset = pwTranslation[1];                                       
      }
    }
  
  // Now look for various resources & cache pointers to them
  if(m_blValid)
    {
    m_strKeyBase.Format("\\StringFileInfo\\%4.4X%4.4X\\",m_wLang,m_wCharset);
    
    CString strKey;
    BOOL blExists = FALSE;

    strKey = m_strKeyBase + _T("FileVersion");
    ::VerQueryValue(m_pvVerInfo, const_cast<LPTSTR>(LPCTSTR(strKey)), (void**)&m_pszFileVersion, &m_uiSizeFileVersion);

    strKey = m_strKeyBase + _T("FileDescription");
    ::VerQueryValue(m_pvVerInfo, const_cast<LPTSTR>(LPCTSTR(strKey)), (void**)&m_pszDescription, &m_uiSizeDescription);

    strKey = m_strKeyBase + _T("ProductName");
    ::VerQueryValue(m_pvVerInfo, const_cast<LPTSTR>(LPCTSTR(strKey)), (void**)&m_pszProductName, &m_uiSizeProductName);

    strKey = m_strKeyBase + _T("ProductVersion");
    ::VerQueryValue(m_pvVerInfo, const_cast<LPTSTR>(LPCTSTR(strKey)), (void**)&m_pszProductVersion, &m_uiSizeProductVersion);

    strKey = m_strKeyBase + _T("LegalCopyright");
    ::VerQueryValue(m_pvVerInfo, const_cast<LPTSTR>(LPCTSTR(strKey)), (void**)&m_pszCopyright, &m_uiSizeCopyright);
    
    // VS_FIXEDFILEINFO structure
    ::VerQueryValue(m_pvVerInfo, _T("\\"), (void**)&m_pvsffi, &m_uiSizeFixedFileInfo);
    }

}

/************************************************************************
* Function:   CVersionInfo::~CVersionInfo
* Destructor for CVersionInfo
* Parameters: NONE
* Returns:    NOTHING
************************************************************************/
CVersionInfo::~CVersionInfo()
{
  delete [] m_pvVerInfo;
}


/************************************************************************
* Function:   CVersionInfo::FileVersion
* This gets a file version string for the module
* Parameters: NONE
* Returns:    A file version string, empty if the infomation is not available
************************************************************************/
CString CVersionInfo::FileVersion()
{
  assert(this);
  CString strVersion = _T("");
  if(m_uiSizeFileVersion > 0)
    strVersion = ToString(m_pszFileVersion,m_uiSizeFileVersion);

  if(m_pvsffi && m_uiSizeFixedFileInfo > 0)
    {
    if(m_pvsffi->dwFileFlags & VS_FF_DEBUG)
      strVersion += _T(" DEBUG");

    if(m_pvsffi->dwFileFlags & VS_FF_PRERELEASE)
      strVersion += _T(" Pre-release");
    }
  return strVersion;
}

/************************************************************************
* Function:   CVersionInfo::Description
* This gets a file description string for the module
* Parameters: NONE
* Returns:    A file description string, empty if the infomation is not available
************************************************************************/
CString CVersionInfo::Description()
{
  assert(this);
  if(m_uiSizeDescription > 0)
    return ToString(m_pszDescription,m_uiSizeDescription);
  else
    return _T("");
}

/************************************************************************
* Function:   CVersionInfo::ProductName
* This gets a product name string for the module
* Parameters: NONE
* Returns:    A product name string, empty if the infomation is not available
************************************************************************/
CString CVersionInfo::ProductName()
{
  assert(this);
  if(m_uiSizeProductName > 0)
    return ToString(m_pszProductName,m_uiSizeProductName);
  else
    return _T("");
}

/************************************************************************
* Function:   CVersionInfo::ProductVersion
* This gets a product version string for the module
* Parameters: NONE
* Returns:    A product version string, empty if the infomation is not available
************************************************************************/
CString CVersionInfo::ProductVersion()
{
  assert(this);
  if(m_uiSizeProductVersion > 0)
    return ToString(m_pszProductVersion,m_uiSizeProductVersion);
  else
    return _T("");
}

/************************************************************************
* Function:   CVersionInfo::Copyright
* This gets a copyright string for the module
* Parameters: NONE
* Returns:    A copyright string, empty if the infomation is not available
************************************************************************/
CString CVersionInfo::Copyright()
{
  assert(this);
  if(m_uiSizeCopyright > 0)
    return ToString(m_pszCopyright,m_uiSizeCopyright);
  else
    return _T("");
}

/************************************************************************
* Function:   CVersionInfo::Resource
* This allows the system to query any of the resources.   Look at the
* help for String in "Win32 Structures" to get a list of likely resources.
* Parameters: pszResource is the resource string to look for
* Returns:    A string with the resource or empty if not found
************************************************************************/
CString CVersionInfo::Resource(LPCTSTR pszResource)
{
  assert(this);
  
  CString str(_T(""));

  if(pszResource)
    {
    LPCTSTR psz = 0;
    UINT uiLen = 0;
    CString strKey = m_strKeyBase + pszResource;
    ::VerQueryValue(m_pvVerInfo, const_cast<LPTSTR>(LPCTSTR(strKey)), (void**)&psz, &uiLen);
    if(uiLen > 0)
      str = psz;
    }

  return str;
}

/************************************************************************
* Function:   CVersionInfo::ToString
* Private function. Converts to a unicode or ansi string.  Which of these
* depends on the presence of a _UNICODE symbol
* Parameters: pwsz is the wide character string to convert
* Returns:    
************************************************************************/
CString CVersionInfo::ToString(LPCTSTR psz, UINT uiLen)
{
  assert(this);
  assert(psz);
  return CString(psz,uiLen-1); // -1 to allow for trailing NULL 
}


