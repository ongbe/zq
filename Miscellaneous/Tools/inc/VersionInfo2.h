/*
Module : VersionInfo.H
Purpose: Interface for an MFC class encapsulation of "Version Infos"
Created: PJN / 10-04-2000

Copyright (c) 2000 - 2008 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////// Macros / Defines //////////////////////////////

#pragma once

#ifndef __VERSIONINFO_H__
#define __VERSIONINFO_H__
 
#ifndef CVERSIONINFO_EXT_CLASS
#define CVERSIONINFO_EXT_CLASS
#endif
#ifndef CVERSIONINFO_EXT_API
#define CVERSIONINFO_EXT_API
#endif
 
#include <windows.h>
#include <string>
using std::string;

/////////////////////////////// Classes ///////////////////////////////////////

class CVERSIONINFO_EXT_CLASS CVersionInfo2
{
public:
    struct TRANSLATION
    {
        unsigned short m_wLangID;   //e.g. 0x0409 LANG_ENGLISH, SUBLANG_ENGLISH_USA
        unsigned short m_wCodePage; //e.g. 1252 Codepage for Windows:Multilingual
    };

    //Constructors / Destructors
    CVersionInfo2();
    CVersionInfo2(const char* szFileName);
    ~CVersionInfo2();

    //methods:
    bool                            Load(const char* szFileName);
    VS_FIXEDFILEINFO*               GetFixedFileInfo();
    DWORD                           GetFileFlagsMask();
    DWORD                           GetFileFlags();
    DWORD                           GetOS();
    DWORD                           GetFileType();
    DWORD                           GetFileSubType();
    FILETIME                        GetCreationTime();
    unsigned __int64                GetFileVersion();
    unsigned __int64                GetProductVersion();
    string                          GetValue(const string& sKeyName);
    string                          GetComments();
    string                          GetCompanyName();
    string                          GetFileDescription();
    string                          GetFileVersionAsString();
    string                          GetInternalName();
    string                          GetLegalCopyright();
    string                          GetLegalTrademarks();
    string                          GetOriginalFilename();
    string                          GetPrivateBuild();
    string                          GetProductName();
    string                          GetProductVersionAsString();
    string                          GetSpecialBuild();
    int                             GetNumberOfTranslations();
    TRANSLATION*                    GetTranslation(int nIndex);
    void                            SetTranslation(int nIndex);
 

protected:
    //Methods
    void Unload();

    //Data
    unsigned short                  m_wLangID;       //The current language ID of the resource
    unsigned short                  m_wCharset;      //The current Character set ID of the resource
    char*                           m_VerData;       //Pointer to the version info blob
    TRANSLATION*                    m_pTranslations; //Pointer to the "\\VarFileInfo\\Translation" version info
    int                             m_nTranslations; //The number of translated version infos in the resource
    VS_FIXEDFILEINFO*               m_pffi;          //Pointer to the fixed size version info data
};

#endif //__VERSIONINFO_H__
