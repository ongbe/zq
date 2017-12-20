
#include "stdafx.h"
#include "TestManager.h"

CTestEntity* CTestManager::sm_pTestEntity = NULL;

CTestManager::CTestManager()
{
}

CTestManager::~CTestManager()
{
}

CTestEntity* CTestManager::CreateTestEntity(const std::string& strSavePath, 
											const std::string& strRunPath)
{
	CDataEntity* pDataEntity = NULL;
	CTestEntity* pTestEntity = NULL;
	std::string strSavePath2 = strSavePath;
	SYSTEMTIME sysTime;
	char strDateTime[32];

	memset(strDateTime, 0, sizeof(strDateTime));
	::GetLocalTime(&sysTime);
	sprintf_s(strDateTime, sizeof(strDateTime)-1, "%04u%02u%02u_%02u%02u%02u\\", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strSavePath2 += strDateTime;

	pDataEntity = new CDataEntity();

	pTestEntity = new CTestEntity();
	pTestEntity->SetRunPath(strRunPath);
	pTestEntity->SetDataEntity(pDataEntity);

	return pTestEntity;
}

