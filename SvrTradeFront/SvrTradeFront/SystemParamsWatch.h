#pragma once

#include <windows.h>

class CSystemParamsWatch
{
public:
	#define SystemBasicInformation			0 
	#define SystemPerformanceInformation	2 
	#define SystemTimeInformation			3

	#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))
 
	typedef struct { 
		DWORD dwUnknown1; 
		ULONG uKeMaximumIncrement; 
		ULONG uPageSize; 
		ULONG uMmNumberOfPhysicalPages; 
		ULONG uMmLowestPhysicalPage; 
		ULONG uMmHighestPhysicalPage; 
		ULONG uAllocationGranularity; 
		PVOID pLowestUserAddress; 
		PVOID pMmHighestUserAddress; 
		ULONG uKeActiveProcessors; 
		BYTE bKeNumberProcessors; 
		BYTE bUnknown2; 
		WORD wUnknown3; 
	} SYSTEM_BASIC_INFORMATION;
 
	typedef struct { 
		LARGE_INTEGER liIdleTime; 
		DWORD dwSpare[76]; 
	} SYSTEM_PERFORMANCE_INFORMATION;
 
	typedef struct { 
		LARGE_INTEGER liKeBootTime; 
		LARGE_INTEGER liKeSystemTime; 
		LARGE_INTEGER liExpTimeZoneBias; 
		ULONG uCurrentTimeZoneId; 
		DWORD dwReserved; 
	} SYSTEM_TIME_INFORMATION;
 
	typedef LONG (WINAPI *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);

public:
	static int GetCpuUsage();
	static INT64 GetMemory();
	static INT64 GetDiskSpace();

public:
	CSystemParamsWatch(void);
	virtual ~CSystemParamsWatch(void);

};
