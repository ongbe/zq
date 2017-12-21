#include "SystemParamsWatch.h"
#include <Windows.h>
#include <stdio.h>

CSystemParamsWatch::CSystemParamsWatch(void)
{
}

CSystemParamsWatch::~CSystemParamsWatch(void)
{
}

//获得CPU使用率
INT CSystemParamsWatch::GetCpuUsage()
{
	SYSTEM_INFO SysInfo = {0}; 
	FILETIME sysTimeIdle = {0}, sysTimeKernel = {0}, sysTimeUser = {0};
	INT64 n64SysTimeIdle = 0, n64SysTimeKernel = 0, n64SysTimeUser = 0;
 
    // get number of processors in the system 
	GetSystemInfo(&SysInfo);
	GetSystemTimes(&sysTimeIdle, &sysTimeKernel, &sysTimeUser);

	n64SysTimeIdle = sysTimeIdle.dwHighDateTime;
	n64SysTimeIdle = n64SysTimeIdle << 32;
	n64SysTimeIdle = n64SysTimeIdle | sysTimeIdle.dwLowDateTime;

	n64SysTimeKernel = sysTimeKernel.dwHighDateTime;
	n64SysTimeKernel = n64SysTimeKernel << 32;
	n64SysTimeKernel = n64SysTimeKernel | sysTimeKernel.dwLowDateTime;

	n64SysTimeUser = sysTimeUser.dwHighDateTime;
	n64SysTimeUser = n64SysTimeUser << 32;
	n64SysTimeUser = n64SysTimeUser | sysTimeUser.dwLowDateTime;

	static bool bFirst = true;
	static DWORD oldTick = 0;
	static INT64 n64OldSysTimeIdle = 0, n64OldSysTimeKernel = 0, n64OldSysTimeUser = 0;
	int nCpuUsage = 0;

	if(!bFirst) {
		double f1 = 0.0, f2 = 0.0, f3 = 0.0;
		DWORD dwTick = GetTickCount();
		f1 = n64SysTimeIdle - n64OldSysTimeIdle;
		f2 = n64SysTimeKernel - n64OldSysTimeKernel;
		f3 = n64SysTimeUser - n64OldSysTimeUser;
		nCpuUsage = (f2+f3-f1)/(f3+f2)*1000.0;
		////printf("CpuUsage := %I64d%\n", liOldIdleTime.QuadPart/1000/1000/10);
		//printf("CpuUsage := %d, \t%0.0f, \t%0.1f%%\n", dwTick-oldTick, f3+f2, nCpuUsage/10.0);
		////printf("CpuUsage := %d, \t%0.0f, \t%0.0f, \t%0.0f, \t%0.0f\n", dwTick-oldTick, f1, f2, f3, 100.0*(f2-f1)/f2);
	}

	bFirst = false;
	n64OldSysTimeIdle = n64SysTimeIdle;
	n64OldSysTimeKernel = n64SysTimeKernel;
	n64OldSysTimeUser = n64SysTimeUser;
	oldTick = GetTickCount();

    return nCpuUsage;
}


INT64 CSystemParamsWatch::GetMemory()
{
	// Used to convert MB to KB
	#define DIV 1024

	MEMORYSTATUSEX memStatus = {0};
	memStatus.dwLength = sizeof(memStatus);
	GlobalMemoryStatusEx(&memStatus);
	//printf("%I64d, %I64d, %I64d\n", memStatus.ullAvailPhys/DIV/DIV, memStatus.ullTotalPhys/DIV/DIV, 
	//		(memStatus.ullTotalPhys - memStatus.ullAvailPhys)/DIV/DIV);
	return memStatus.ullTotalPhys - memStatus.ullAvailPhys;
}

INT64 CSystemParamsWatch::GetDiskSpace()
{
	ULARGE_INTEGER FreeAvailable={0}, TotalNum={0}, TotalFreeNum={0};
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);

	char csDriver[3] = {0};

	csDriver[0] = lsPath[0];
	csDriver[1] = ':';
	csDriver[2] = '\0';
	GetDiskFreeSpaceEx(csDriver, &FreeAvailable, &TotalNum, &TotalFreeNum);
	//printf("%I64d, %I64d, %I64d\n", FreeAvailable.QuadPart/DIV/DIV, TotalNum.QuadPart/DIV/DIV, 
	//		TotalFreeNum.QuadPart/DIV/DIV);
	return FreeAvailable.QuadPart;
}


 	//LARGE_INTEGER liOldIdleTime = {0,0}; 
	//LARGE_INTEGER liOldSystemTime = {0,0};
	// 
	//PROCNTQSI NtQuerySystemInformation;

 //   SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo; 
 //   SYSTEM_TIME_INFORMATION SysTimeInfo; 
 //   SYSTEM_BASIC_INFORMATION SysBaseInfo; 
 //   double dbIdleTime = 0; 
 //   double dbSystemTime = 0; 
 //   LONG status; 
 //
 //   NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"),"NtQuerySystemInformation");
 //
 //   if (!NtQuerySystemInformation) 
 //       return -1;
 //
 //   // get number of processors in the system 
 //   status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL); 
 //   if (status != NO_ERROR) 
 //       return -1;
 //
 //   // get new system time 
 //   status = NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),0); 
 //   if (status!=NO_ERROR) 
 //       return -1;
 //
 //   // get new CPU's idle time 
 //   status =NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
 //   if (status != NO_ERROR) 
 //       return -1;
 //
 //   // if it's a first call - skip it 
 //   if (liOldIdleTime.QuadPart != 0) 
 //   { 
 //       // CurrentValue = NewValue - OldValue 
 //       dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime); 
 //       dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime);
 //
 //       // CurrentCpuIdle = IdleTime / SystemTime 
 //       dbIdleTime = dbIdleTime / dbSystemTime;
 //
 //       // CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors 
 //       dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;
 //
 //   }
 //
 //   // store new CPU's idle and system time 
 //   liOldIdleTime = SysPerfInfo.liIdleTime; 
 //   liOldSystemTime = SysTimeInfo.liKeSystemTime;


	//static bool bFirst = true;
	//static DWORD oldTick = 0;
	//static LARGE_INTEGER oldIdleTime = {0};
	//static LARGE_INTEGER oldSystemTime = {0};


	//if(!bFirst) {
	//	double f1 = 0.0, f2 = 0.0;
	//	DWORD dwTick = GetTickCount();
	//	f1 = liOldIdleTime.QuadPart - oldIdleTime.QuadPart;
	//	f2 = liOldSystemTime.QuadPart - oldSystemTime.QuadPart;
	//	//printf("CpuUsage := %I64d%\n", liOldIdleTime.QuadPart/1000/1000/10);
	//	printf("CpuUsage := %d, %f, %f, %f\n", dwTick-oldTick, f1, f2, 100.0*f1/(f2+f1)/2.0);
	//}

	//bFirst = false;
	//oldIdleTime = liOldIdleTime;
	//oldSystemTime = liOldSystemTime;
	//oldTick = GetTickCount();

 //
 //   return (int)dbIdleTime;

