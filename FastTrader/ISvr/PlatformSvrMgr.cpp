#include "StdAfx.h"
#include "PlatformSvrMgr.h"
#include "Tools_Ansi.h"
#include "Module-Misc2/GlobalConfigMgr.h"

PlatformSvrMgr* g_pPlatformMgr = NULL;

void PlatformSvrMgr::CreateInstance()
{
	if ( NULL == g_pPlatformMgr )
	{
		g_pPlatformMgr = new PlatformSvrMgr();
	}
}

void PlatformSvrMgr::DestroyInstance()
{
	if ( NULL != g_pPlatformMgr )
	{
		delete g_pPlatformMgr;
		g_pPlatformMgr = NULL;
	}
}

void PlatformSvrMgr::CreateAccount(ePLATFORMTYPE PlatformType,const string& strAccount)
{
    CeasymutexGuard guard(m_mutex);

	map<string, IPlatformSingleSvr*>::iterator it = m_allPlatform.find(strAccount);
	if ( it != m_allPlatform.end() )
		return;

    if(PlatformType==PTYPE_NULL) PlatformType=PTYPE_CTP;

    IPlatformSingleSvr* pSvr =NULL;

    //平台最大值。多账号版为100，其它为1。
    int MaxPlatformID = IS_MULTIACCOUNT_VERSION ? 100 : 1;

    int PlatformID;
    for(PlatformID=0;PlatformID<MaxPlatformID;PlatformID++)
        if(m_mapPlatformID2.find(PlatformID)==m_mapPlatformID2.end()) break;
    if(PlatformID>=MaxPlatformID) return;

	pSvr=IPlatformSingleSvr::Create(strAccount,PlatformType,PlatformID,MaxPlatformID);
    //pSvr->SetPlatformID(m_platformID);

    if(m_allPlatform.size()==0)
        m_pFirstSvr=pSvr;

    m_allPlatform[strAccount] = pSvr;
	m_mapPlatformID[pSvr] = PlatformID;
	m_mapPlatformID2[PlatformID] = pSvr;
	//m_platformID++;

    ResetDefSvr_Internal();
}

void PlatformSvrMgr::ReleaseAccount(const string strAccount )
{
    CeasymutexGuard guard(m_mutex);

	map<string, IPlatformSingleSvr*>::iterator it = m_allPlatform.find(strAccount);
	if ( it != m_allPlatform.end() )
	{
        IPlatformSingleSvr* pSvr=it->second;
		if ( pSvr != NULL )
		{
			IPlatformSingleSvr::Release(pSvr);
            if(m_pDefSvr==pSvr) m_pDefSvr=NULL;
            if(m_pCurSvr==pSvr) m_pCurSvr=NULL;
            if(m_pFirstSvr==pSvr) m_pFirstSvr=NULL;

            map<IPlatformSingleSvr*, int>::iterator it2=m_mapPlatformID.find(pSvr);
            if(it2!=m_mapPlatformID.end())
            {
                CTools_Ansi::EraseInMap<int, IPlatformSingleSvr*>(m_mapPlatformID2,it2->second);
                m_mapPlatformID.erase(it2);
            }
		}
        m_allPlatform.erase(it);
        ResetDefSvr_Internal();
	}
}

void PlatformSvrMgr::ReleaseAccount(IPlatformSingleSvr* pSvr )
{
    CeasymutexGuard guard(m_mutex);

	IPlatformSingleSvr::Release(pSvr);
    if(m_pDefSvr==pSvr) m_pDefSvr=NULL;
    if(m_pCurSvr==pSvr) m_pCurSvr=NULL;
    if(m_pFirstSvr==pSvr) m_pFirstSvr=NULL;

    map<IPlatformSingleSvr*, int>::iterator it2=m_mapPlatformID.find(pSvr);
    if(it2!=m_mapPlatformID.end())
    {
        CTools_Ansi::EraseInMap<int, IPlatformSingleSvr*>(m_mapPlatformID2,it2->second);
        m_mapPlatformID.erase(it2);
    }

    map<string, IPlatformSingleSvr*>::iterator it = m_allPlatform.begin();
	while(it!=m_allPlatform.end())
	{
		if(it->second==pSvr) 
            it=m_allPlatform.erase(it);
        else it++;
    }

    ResetDefSvr_Internal();
}
void PlatformSvrMgr::ReleaseAllAccount()
{
    m_mutex.lock();
    vector<IPlatformSingleSvr*> pSvrs;
    for(map<string, IPlatformSingleSvr*>::const_iterator it=m_allPlatform.begin();it!=m_allPlatform.end();it++)
        pSvrs.push_back(it->second);
    m_mutex.unlock();

    for(int i=0;i<(int)pSvrs.size();i++)
        ReleaseAccount(pSvrs[i]);
}

void PlatformSvrMgr::SetAccount(IPlatformSingleSvr* pSvr,const string& strAccount)
{
    CeasymutexGuard guard(m_mutex);

	for(map<string, IPlatformSingleSvr*>::iterator it=m_allPlatform.begin();it!=m_allPlatform.end();it++)
	{
		if(it->second==pSvr) 
        {
            m_allPlatform.erase(it);
            break;
        }
	}
    m_allPlatform[strAccount]=pSvr;
    pSvr->m_Account=strAccount;
}

IPlatformSingleSvr* PlatformSvrMgr::GetPlatformSvr( const string strAccount )
{
    CeasymutexGuard guard(m_mutex);
	map<string, IPlatformSingleSvr*>::iterator it = m_allPlatform.find(strAccount);
	if ( it != m_allPlatform.end() )
		return it->second;
	else return NULL;
}

IPlatformSingleSvr* PlatformSvrMgr::GetPlatformSvr( int PlatformID )
{
    CeasymutexGuard guard(m_mutex);
    map<int, IPlatformSingleSvr*>::iterator it=m_mapPlatformID2.find(PlatformID);
    if(it!=m_mapPlatformID2.end())
        return it->second;
    else return NULL;
}
string PlatformSvrMgr::GetAccount(int PlatformID)
{
    CeasymutexGuard guard(m_mutex);
    map<int, IPlatformSingleSvr*>::iterator it=m_mapPlatformID2.find(PlatformID);
    if(it!=m_mapPlatformID2.end()&&it->second!=NULL)
        return it->second->m_Account;
    else return string("");
}


IPlatformSingleSvr* PlatformSvrMgr::GetDefaultPlatformSvr()
{
    return m_pDefSvr;
}
IPlatformSingleSvr* PlatformSvrMgr::GetCurrentPlatformSvr()
{
    return m_pCurSvr;
}
IPlatformSingleSvr* PlatformSvrMgr::GetFirstPlatformSvr()
{
    return m_pFirstSvr;
}
void PlatformSvrMgr::SetCurrentPlatformSvr(IPlatformSingleSvr* pSvr)
{
    m_pCurSvr=pSvr;
}

void PlatformSvrMgr::GetAllPlatformSvr(set<IPlatformSingleSvr*>& vec)
{
    CeasymutexGuard guard(m_mutex);
	vec.clear();
	map<string, IPlatformSingleSvr*>::iterator it;
	for (it = m_allPlatform.begin(); it != m_allPlatform.end(); ++it)	
	{
		vec.insert(it->second);
	}
}
vector<IPlatformSingleSvr*> PlatformSvrMgr::GetLogonPlatformSvr()
{
    vector<IPlatformSingleSvr*> Svrs;
    CeasymutexGuard guard(m_mutex);
	map<string, IPlatformSingleSvr*>::iterator it;
	for (it = m_allPlatform.begin(); it != m_allPlatform.end(); ++it)	
	{
        if(it->second!=NULL&&it->second->IsInitQrySucc()&&!(it->second->m_bWillStop))
            Svrs.push_back(it->second);
	}
    return Svrs;
}

vector<string> PlatformSvrMgr::GetLogonAccounts()
{
    vector<string> Accounts;
    CeasymutexGuard guard(m_mutex);
	map<string, IPlatformSingleSvr*>::iterator it;
	for (it = m_allPlatform.begin(); it != m_allPlatform.end(); ++it)	
	{
        if(it->second!=NULL&&it->second->IsInitQrySucc()&&!(it->second->m_bWillStop))
            Accounts.push_back(it->second->m_Account);
	}
    return Accounts;
}

int PlatformSvrMgr::GetPlatformID(IPlatformSingleSvr* pSvr)
{
    CeasymutexGuard guard(m_mutex);
    map<IPlatformSingleSvr*,int>::iterator it=m_mapPlatformID.find(pSvr);
    if(it!=m_mapPlatformID.end()) return it->second;
    else return -1;
}
int PlatformSvrMgr::GetPlatformID(const string& strAccount)
{
    IPlatformSingleSvr* pSvr=GetPlatformSvr(strAccount);
    if(!pSvr) return -1;
    return GetPlatformID(pSvr);
}

//设置缺省Svr。首先找已登录的PlatformID最小的，否则找PlatformID最小的
void PlatformSvrMgr::ResetDefSvr_Internal(void)
{
    for(map<int,IPlatformSingleSvr*>::const_iterator it=m_mapPlatformID2.begin();it!=m_mapPlatformID2.end();it++)
    {
        if(it->second!=NULL&&it->second->IsInitQrySucc())
        {
            m_pDefSvr=it->second;
            return;
        }
    }
    for(map<int,IPlatformSingleSvr*>::const_iterator it=m_mapPlatformID2.begin();it!=m_mapPlatformID2.end();it++)
    {
        if(it->second!=NULL)
        {
            m_pDefSvr=it->second;
            return;
        }
    }
    m_pDefSvr=NULL;
}

PlatformSvrMgr::PlatformSvrMgr()
: m_pDefSvr(NULL),
  m_pCurSvr(NULL),
  m_pFirstSvr(NULL)
  //,m_platformID(0)
{
	m_allPlatform.clear();
    m_mapPlatformID.clear();
    m_mapPlatformID2.clear();
}

PlatformSvrMgr::~PlatformSvrMgr()
{
	map<string, IPlatformSingleSvr*>::iterator it = m_allPlatform.begin();
	for ( ; it != m_allPlatform.end(); )
	{
		if ( it->second != NULL )
		{
			IPlatformSingleSvr::Release(it->second);
		}
	}
}

//通知某个Svr初始化查询完成
void PlatformSvrMgr::NotifyInitQrySucc(IPlatformSingleSvr* pSvr)
{
    ResetDefSvr_Internal();
}
