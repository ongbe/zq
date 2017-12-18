此工程生成updatelib.lib，在FastTrader中使用。
作用：
1. 连接升级服务器
2. 判断是否需要升级
3. 下载升级文件
4. 启动updateexe.exe，关闭FastTrader.exe，等待updateexe.exe替换升级文件，并重启FastTrader.exe

逻辑：
1. 从AutoUpdate.xml中取出升级服务器的IP/Port
2. 建立到升级服务器的TcpClient连接
3. 连接成功，查询update.xml
4. 断开连接
5. 比较本地版本号和服务器版本号，检查否需要更新
6. 如果需要更新，检查AutoUpdate.xml中是否忽略此版本号
7. 问用户是否需要更新。如果不更新，问是否忽略此版本的更新
8. 再次建立TcpClient连接
9. 下载全部更新文件
10.断开连接
11.启动updateexe.exe
12.关闭交易终端
13.在updateexe.exe中，替换文件，完成升级，再次启动FastTrader

注意：
1. 如果用户已经开始登陆，则升级程序关闭交易终端前需进行关闭确认。否则不用
2. 交易终端启动后，用单独线程下载FastTrader_xxx_Update.xml进行版本判断。在连不上、不需要升级等情况下，不影响交易终端的正常使用

服务器上的更新配置内容如下：

<update>
   <version>1.5.0.7</version>
   <version_min>1.3.2.84</version_min>
   <version_svrpath>update_std_01050007</version_svrpath>
   <ReplaceFiles>
		<FileName>AutoUpdate.xml</FileName>
		<FileName>FastTrader.exe</FileName>
		<FileName>help.chm</FileName>
		<FileName>HypertextNotice.exe</FileName>
		<FileName>InstrumentGroup.xml</FileName>
		<FileName>language.csv</FileName>
		<FileName>layout.txt</FileName>
		<FileName>platform.xml</FileName>
		<FileName>ServerParam.xml</FileName>
		<FileName>syscfg.xml</FileName>
		<FileName>thostmduserapi.dll</FileName>
		<FileName>thosttraderapi.dll</FileName>
		<FileName>updatefasttrader.exe</FileName>
   		<FileName>layout\1.layout</FileName>
   		<FileName>layout\2.layout</FileName>
   		<FileName>layout\3.layout</FileName>
   		<FileName>layout\4.layout</FileName>
   </ReplaceFiles >  
   <note>更新说明</note>
</update>  

========================================================================
    WIN32 APPLICATION : updatefasttrader1 Project Overview
========================================================================

AppWizard has created this updatefasttrader1 application for you.

This file contains a summary of what you will find in each of the files that
make up your updatefasttrader1 application.


updatefasttrader1.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

updatefasttrader1.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
AppWizard has created the following resources:

updatefasttrader1.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

updatefasttrader1.ico
    This is an icon file, which is used as the application's icon (32x32).
    This icon is included by the main resource file updatefasttrader1.rc.

small.ico
    This is an icon file, which contains a smaller version (16x16)
    of the application's icon. This icon is included by the main resource
    file updatefasttrader1.rc.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named updatefasttrader1.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
