#pragma once
#include "IPlatformSingleSvr.h"
#include "platformID.h"
#include "easymutex.h"
#include <map>
#include <vector>
#include <string>
#include <set>
using std::map;
using std::string;
using std::set;
using std::vector;

class PlatformSvrMgr
{
public:
	static void CreateInstance();
	static void DestroyInstance();

    void CreateAccount(ePLATFORMTYPE PlatformType=PTYPE_NULL,const string& strAccount = "");
	void ReleaseAccount( const string strAccount);
	void ReleaseAccount( IPlatformSingleSvr* pSvr);
    void ReleaseAllAccount();
	IPlatformSingleSvr* GetPlatformSvr( const string strAccount = "");
	IPlatformSingleSvr* GetPlatformSvr( int PlatformID );
    string GetAccount(int PlatformID);
	IPlatformSingleSvr* GetDefaultPlatformSvr();
	IPlatformSingleSvr* GetCurrentPlatformSvr();
	IPlatformSingleSvr* GetFirstPlatformSvr();
	void SetCurrentPlatformSvr(IPlatformSingleSvr* pSvr);
	void GetAllPlatformSvr(set<IPlatformSingleSvr*>& vec);
	vector<IPlatformSingleSvr*> GetLogonPlatformSvr();
    vector<string> GetLogonAccounts();
    //取PlatformID，失败返回-1
    int GetPlatformID(IPlatformSingleSvr* pSvr);
    int GetPlatformID(const string& strAccount);

    void SetAccount(IPlatformSingleSvr* pSvr,const string& strAccount);
    //通知某个Svr初始化查询完成
    void NotifyInitQrySucc(IPlatformSingleSvr* pSvr);

private:
	PlatformSvrMgr();
	~PlatformSvrMgr();
    
    //设置缺省Svr。首先找已登录的PlatformID最小的，否则找PlatformID最小的
    void PlatformSvrMgr::ResetDefSvr_Internal(void);

private:
	map<string, IPlatformSingleSvr*> m_allPlatform;
    IPlatformSingleSvr* m_pDefSvr;
    IPlatformSingleSvr* m_pCurSvr;
    IPlatformSingleSvr* m_pFirstSvr;        //在多账号中，这个对应登录多账号的Svr，里面有多账号登录用户名和密码信息

    map<IPlatformSingleSvr*, int> m_mapPlatformID;
    map<int, IPlatformSingleSvr*> m_mapPlatformID2;
    //int m_platformID;

    Ceasymutex m_mutex;

};