# Microsoft Developer Studio Project File - Name="alxBASE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=alxBASE - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "alxBASE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "alxBASE.mak" CFG="alxBASE - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "alxBASE - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "alxBASE - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "alxBASE - Win32 Release static" (based on "Win32 (x86) Static Library")
!MESSAGE "alxBASE - Win32 Debug static" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "alxBASE - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\alxBASEr.lib"

!ELSEIF  "$(CFG)" == "alxBASE - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\alxBASEd.lib"

!ELSEIF  "$(CFG)" == "alxBASE - Win32 Release static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "alxBASE___Win32_Release_static"
# PROP BASE Intermediate_Dir "alxBASE___Win32_Release_static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_static"
# PROP Intermediate_Dir "Release_static"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\alxBASEr.lib"
# ADD LIB32 /nologo /out:"Lib\alxBASErs.lib"

!ELSEIF  "$(CFG)" == "alxBASE - Win32 Debug static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "alxBASE___Win32_Debug_static"
# PROP BASE Intermediate_Dir "alxBASE___Win32_Debug_static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_static"
# PROP Intermediate_Dir "Debug_static"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\alxBASEd.lib"
# ADD LIB32 /nologo /out:"Lib\alxBASEds.lib"

!ENDIF 

# Begin Target

# Name "alxBASE - Win32 Release"
# Name "alxBASE - Win32 Debug"
# Name "alxBASE - Win32 Release static"
# Name "alxBASE - Win32 Debug static"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "ALXParser Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SRC\ALXParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\ALXParserException.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\ALXParserFuncDef.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\ALXParserOperDef.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\ALXSyntaxEditCtrl.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Include\alxBASE.rc
# End Source File
# Begin Source File

SOURCE=.\SRC\DBFCursorSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DBFException.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DBFFields.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBFMemCursorSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DBFRecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\DBFTableDef.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\DBTMemoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\FPTMemoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MemoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\SMTMemoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "ALXParser Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\INCLUDE\ALXParser.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\ALXParserDef.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\ALXParserException.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\ALXParserExceptionRes.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\ALXParserFuncDef.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\ALXParserOperDef.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\ALXSyntaxEditCtrl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\INCLUDE\DBFCursorSet.h
# End Source File
# Begin Source File

SOURCE=.\Include\DBFDef.h
# End Source File
# Begin Source File

SOURCE=.\Include\DBFException.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\DBFExceptionRes.h
# End Source File
# Begin Source File

SOURCE=.\Include\DBFFields.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\DBFMemCursorSet.h
# End Source File
# Begin Source File

SOURCE=.\Include\DBFRecordset.h
# End Source File
# Begin Source File

SOURCE=.\Include\DBFTableDef.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\DBTMemoFile.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\FPTMemoFile.h
# End Source File
# Begin Source File

SOURCE=.\Include\MemoFile.h
# End Source File
# Begin Source File

SOURCE=.\Include\resource.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\SMTMemoFile.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
