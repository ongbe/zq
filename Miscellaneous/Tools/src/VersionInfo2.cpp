/*
Module : VersionInfo.CPP
Purpose: Implementation for a MFC class encapsulation of Version Infos
Created: PJN / 10-04-2000
History: PJN / 07-07-2006 1. Updated copyright details
                          2. Updated the code to clean compile on VC 2005
                          3. Addition of CVERSIONINFO_EXT_CLASS and CVERSIONINFO_EXT_API macros to allow
                          the class to be easily added to an extension DLL.
                          4. Optimized CVersionInfo2 constructor code
                          5. Reviewed all TRACE statements for correctness
                          6. Updated the documentation to use the same style as the web site.
         PJN / 14-09-2008 1. Updated copyright details.
                          2. Code now compiles cleanly using Code Analysis (/analyze)
                          3. Updated code to compile correctly using _ATL_wxString_EXPLICIT_CONSTRUCTORS define
                          4. Updated sample app to clean compile on VC 2008
                          5. The code has now been updated to support VC 2005 or later only. 
                          6. Removed VC 6 style AppWizard comments from the code.
                          7. Reworked code to use ATL::CHeapPtr for required memory allocations

Copyright (c) 2000 - 2008 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

//////////////// Includes /////////////////////////////////////////////////////

#include <TCHAR.h>
#include "stdafx.h"
#include "VersionInfo2.h"


//////////////// Macros / Locals //////////////////////////////////////////////

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

//Automatically pull in the win32 version Library
#pragma comment(lib, "version.lib")


//////////////// Implementation ///////////////////////////////////////////////

CVersionInfo2::CVersionInfo2() 
:   m_pffi(NULL),
    m_wLangID(0),
    m_wCharset(1252), //Use the ANSI code page as a default
    m_pTranslations(NULL),
    m_nTranslations(0)
{
}
CVersionInfo2::CVersionInfo2(const char* szFileName) 
:   m_pffi(NULL),
    m_wLangID(0),
    m_wCharset(1252), //Use the ANSI code page as a default
    m_pTranslations(NULL),
    m_nTranslations(0)
{
    Load(szFileName);
}

CVersionInfo2::~CVersionInfo2()
{
    Unload();
}

void CVersionInfo2::Unload()
{
    m_pffi = NULL;
    if (m_VerData != NULL)
    {
        m_VerData= NULL;
        /*delete m_VerData;*/
        free(m_VerData);
    }
    m_wLangID = 0;
    m_wCharset = 1252; //Use the ANSI code page as a default
    m_pTranslations = NULL;
    m_nTranslations = 0;
}

bool CVersionInfo2::Load(const char* szFileName)
{
    //Free up any previous memory lying around
    Unload();

    bool bSuccess = false;
    DWORD dwHandle = 0;
    DWORD dwSize = GetFileVersionInfoSize(szFileName, &dwHandle);
    if (dwSize)
    {
        //Allocate some memory to hold the version info data
        //m_VerData=new char[dwSize+1];
        m_VerData=(char*)malloc(dwSize+1);
        if (m_VerData==NULL)
        {
            SetLastError(ERROR_OUTOFMEMORY);
            return false;
        }
        if (GetFileVersionInfo(szFileName, dwHandle, dwSize, m_VerData))
        {
            //Get the fixed size version info data
            UINT nLen = 0;
            if (VerQueryValue(m_VerData, _T("\\"), reinterpret_cast<LPVOID*>(&m_pffi), &nLen))
            {
                //Retrieve the Lang ID and Character set ID
                if (VerQueryValue(m_VerData, _T("\\VarFileInfo\\Translation"), reinterpret_cast<LPVOID*>(&m_pTranslations), &nLen) && (nLen >= sizeof(TRANSLATION))) 
                {
                    m_nTranslations = nLen / sizeof(TRANSLATION);
                    m_wLangID = m_pTranslations[0].m_wLangID;
                    m_wCharset = m_pTranslations[0].m_wCodePage;
                }
                bSuccess = true;
            }
            else
            {
                //TRACE(_T("CVersionInfo2::Load, Failed to query file size version info for file %s, LastError:%d\n"), szFileName, ::GetLastError());
                ;
            }
        }
        else
        {
          //TRACE(_T("CVersionInfo2::Load, Failed to read in version info for file %s, LastError:%d\n"), szFileName, ::GetLastError());
            ;
        }
    }
    else
    {
        //TRACE(_T("CVersionInfo2::Load, Failed to get version info for file %s, LastError:%d\n"), szFileName, ::GetLastError());
        ;
    }

    return bSuccess;
}

VS_FIXEDFILEINFO* CVersionInfo2::GetFixedFileInfo()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    return m_pffi;
}

DWORD CVersionInfo2::GetFileFlagsMask()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    return m_pffi->dwFileFlagsMask;
}

DWORD CVersionInfo2::GetFileFlags()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    return m_pffi->dwFileFlags;
}

DWORD CVersionInfo2::GetOS()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    return m_pffi->dwFileOS;
}   

DWORD CVersionInfo2::GetFileType()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    return m_pffi->dwFileType;
}

DWORD CVersionInfo2::GetFileSubType()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    return m_pffi->dwFileSubtype;
}

FILETIME CVersionInfo2::GetCreationTime()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    FILETIME CreationTime;
    CreationTime.dwHighDateTime = m_pffi->dwFileDateMS; 
    CreationTime.dwLowDateTime = m_pffi->dwFileDateLS; 
    return CreationTime;
}

unsigned __int64 CVersionInfo2::GetFileVersion()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    unsigned __int64 nFileVersion = 0;
    nFileVersion = m_pffi->dwFileVersionLS;
    nFileVersion += ((static_cast<unsigned __int64>(m_pffi->dwFileVersionMS)) << 32);
    return nFileVersion;
}

unsigned __int64 CVersionInfo2::GetProductVersion()
{
    //ASSERT(m_VerData != NULL); //Must have been loaded successfully
    unsigned __int64 nProductVersion = 0;
    nProductVersion = m_pffi->dwProductVersionLS;
    nProductVersion += ((static_cast<unsigned __int64>(m_pffi->dwProductVersionMS)) << 32);
    return nProductVersion;
}

string CVersionInfo2::GetValue(const string& sKey)
{
	string sVal = "";
    //Validate our parameters
    if (m_VerData != NULL)
    {
        //What will be the return value from this function
        //Form the string to query with
        char buf[256];
        sprintf_s(buf,sizeof(buf),_T("\\StringFileInfo\\%04x%04x\\%s"), m_wLangID, m_wCharset, sKey.c_str());
        buf[sizeof(buf)-1]=0;

        //Note that the definition for VerQueryValue in the VC 2005 Winver.h header file is incorrectly
        //defined as taking a non-const 2nd parameter, so to avoid this issue, lets get a non const pointer
        //to the "sQueryValue" buffer
        /*LPTSTR pszQueryValue = sQueryValue.GetBuffer(sQueryValue.GetLength());*/
        LPTSTR pszQueryValue =(LPTSTR) buf;

        //Do the query
        LPTSTR pVal = NULL;
        UINT nLen = 0;
        if (VerQueryValue(m_VerData, pszQueryValue, reinterpret_cast<LPVOID*>(&pVal), &nLen)) 
	        sVal = pVal;
    }
    return sVal;
}

string CVersionInfo2::GetCompanyName()
{
    return GetValue(_T("CompanyName"));
}

string CVersionInfo2::GetFileDescription()
{
    return GetValue(_T("FileDescription"));
}

string CVersionInfo2::GetFileVersionAsString()
{
    return GetValue(_T("FileVersion"));
}

string CVersionInfo2::GetInternalName()
{
    return GetValue(_T("InternalName"));
}

string CVersionInfo2::GetLegalCopyright()
{
    return GetValue(_T("LegalCopyright"));
}

string CVersionInfo2::GetOriginalFilename()
{
    return GetValue(_T("OriginalFilename"));
}

string CVersionInfo2::GetProductName()
{
    return GetValue(_T("Productname"));
}

string CVersionInfo2::GetProductVersionAsString()
{
    return GetValue(_T("ProductVersion"));
}
  
int CVersionInfo2::GetNumberOfTranslations()
{
    return m_nTranslations;
}

string CVersionInfo2::GetComments()
{
    return GetValue(_T("Comments"));
}

string CVersionInfo2::GetLegalTrademarks()
{
    return GetValue(_T("LegalTrademarks"));
}

string CVersionInfo2::GetPrivateBuild()
{
    return GetValue(_T("PrivateBuild"));
}

string CVersionInfo2::GetSpecialBuild()
{
    return GetValue(_T("SpecialBuild"));
}

CVersionInfo2::TRANSLATION* CVersionInfo2::GetTranslation(int nIndex)
{
    //ASSERT(nIndex >= 0 && nIndex < m_nTranslations);
    if(m_pTranslations) return &m_pTranslations[nIndex];
    else return NULL;
}

void CVersionInfo2::SetTranslation(int nIndex)
{
    TRANSLATION* pTranslation = GetTranslation(nIndex);
    m_wLangID = pTranslation->m_wLangID;
    m_wCharset = pTranslation->m_wCodePage;
}
