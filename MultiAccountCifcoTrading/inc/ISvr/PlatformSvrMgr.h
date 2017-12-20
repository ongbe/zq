#pragma once
#include "IPlatformSingleSvr.h"
#include "platformID.h"
class PlatformSvrMgr
{
public:
	static void CreateInstance();
	static void DestroyInstance();

    void CreateAccount(const std::string strAccount = "");
	void ReleaseAccount( const std::string strAccount = "");
	void ReleaseAccount( IPlatformSingleSvr* pSvr);
	IPlatformSingleSvr* GetPlatformSvr( const std::string strAccount = "");
	IPlatformSingleSvr* GetDefaultPlatformSvr();
	IPlatformSingleSvr* GetCurrentPlatformSvr();
	void GetAllPlatformSvr(set<IPlatformSingleSvr*>& vec);
private:
	PlatformSvrMgr();
	~PlatformSvrMgr();
private:
	std::map<std::string,IPlatformSingleSvr*> m_allPlatform;

};