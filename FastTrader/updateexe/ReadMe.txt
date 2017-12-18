此工程生成updatefasttrader2.exe。
作用：
1. 将update目录下的新文件复制到当前目录下，然后删除update目录下文件
2. 启动FastTrader.exe

逻辑：
1. 看update下是否有FastTrader_Update.xml，没有则退出
2. 每隔200ms判断FastTrader.exe进程是否存在，连续20次(4s)
3. 如果FastTrader.exe一直在，提示用户关闭FastTrader.exe
4. 将update目录下的新文件复制到当前目录下
5. 删除update目录下文件
6. 启动FastTrader.exe


========================================================================
    WIN32 APPLICATION : updatefasttrader2 Project Overview
========================================================================

AppWizard has created this updatefasttrader2 application for you.

This file contains a summary of what you will find in each of the files that
make up your updatefasttrader2 application.


updatefasttrader2.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

updatefasttrader2.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
AppWizard has created the following resources:

updatefasttrader2.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

updatefasttrader2.ico
    This is an icon file, which is used as the application's icon (32x32).
    This icon is included by the main resource file updatefasttrader2.rc.

small.ico
    This is an icon file, which contains a smaller version (16x16)
    of the application's icon. This icon is included by the main resource
    file updatefasttrader2.rc.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named updatefasttrader2.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
