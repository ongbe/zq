@echo off

cd %~dp0
set config=Release
set target=reBuild
set noprojgen=
set nobuild=
set run=
set target_arch=ia32
set vs_toolset=x86
set platform=WIN32
set library=static_library




:vc-set-2008
@rem Look for Visual Studio 2008
if not defined VS90COMNTOOLS goto vc-set-notfound
if not exist "%VS90COMNTOOLS%\..\..\vc\vcvarsall.bat" goto vc-set-notfound
call "%VS90COMNTOOLS%\..\..\vc\vcvarsall.bat" %vs_toolset%
set GYP_MSVS_VERSION=2008
goto :msbuild

:vc-set-notfound
echo Warning: Visual Studio not found

:msbuild
@rem Check if VS build env is available
if not defined VCINSTALLDIR goto msbuild-not-found
goto msbuild-found

:msbuild-not-found
echo Build skipped. To build, this file needs to run from VS cmd prompt.
goto run

@rem Build the sln with msbuild.
:msbuild-found
del release.log
msbuild .\TradeServerBuild.sln /t:%target% /p:Configuration=%config% /p:Platform="%platform%"  /fileLogger /fileLoggerParameters:LogFile=release.log;Append;Verbosity=d  /nologo
if errorlevel 1 
echo Build error,not copy file.
exit /b 1
goto copy

:copy
addversion.bat

:exit
