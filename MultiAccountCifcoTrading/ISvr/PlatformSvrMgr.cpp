#include "StdAfx.h"
#include "PlatformSvrMgr.h"
PlatformStru_OrderInfo PlatformStru_OrderInfo::InvalidOrderInfo(true);
PlatformStru_PositionDetail PlatformStru_PositionDetail::InvalidPositionDetail(true);
PlatformStru_Position PlatformStru_Position::InvalidPosition(true);
PlatformStru_PositionComb PlatformStru_PositionComb::InvalidPositionDetailComb(true);
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

void PlatformSvrMgr::CreateAccount(const std::string strAccount)
{
	std::map<std::string, IPlatformSingleSvr*>::iterator it = m_allPlatform.find(strAccount);
	if ( it != m_allPlatform.end() )
	{
		return;
	}
	else
	{
		IPlatformSingleSvr* pSvr =NULL;
		if(strAccount.find("APPID")==strAccount.npos)
		{
			pSvr=IPlatformSingleSvr::Create(PTYPE_CTP);
		}
		else
		{
			pSvr=IPlatformSingleSvr::Create(PTYPE_PATSAPI);
		}
		m_allPlatform[strAccount] = pSvr;
	}
}

void PlatformSvrMgr::ReleaseAccount(const std::string strAccount )
{
	std::map<std::string, IPlatformSingleSvr*>::iterator it = m_allPlatform.find(strAccount);
	if ( it != m_allPlatform.end() )
	{
		IPlatformSingleSvr* pSvr = it->second;
		if ( NULL != pSvr )
		{
			//pSvr->StopThread();
			//pSvr->Stop();
			IPlatformSingleSvr::Release(pSvr);
		}
		m_allPlatform.erase(it);
	}
}
void PlatformSvrMgr::ReleaseAccount( IPlatformSingleSvr* pSvr)
{
	std::map<std::string, IPlatformSingleSvr*>::iterator it;
	for (it=m_allPlatform.begin();it!=m_allPlatform.end();)
	{
		if(it->second==pSvr)
			it=m_allPlatform.erase(it);
		else
			++it;
	}
}
IPlatformSingleSvr* PlatformSvrMgr::GetPlatformSvr( const std::string strAccount )
{
	std::map<std::string, IPlatformSingleSvr*>::iterator it = m_allPlatform.find(strAccount);
	if ( it != m_allPlatform.end() )
	{
		return it->second;
	}

	return NULL;
}

IPlatformSingleSvr* PlatformSvrMgr::GetDefaultPlatformSvr()
{
	IPlatformSingleSvr* pSvr = NULL;
	if( m_allPlatform.size() > 0)
		pSvr = m_allPlatform.begin()->second;
	return pSvr;
}
IPlatformSingleSvr* PlatformSvrMgr::GetCurrentPlatformSvr()
{
	std::map<std::string, IPlatformSingleSvr*>::iterator it;
	for (it = m_allPlatform.begin(); it != m_allPlatform.end(); ++it)	
	{
		if(it->second->GetMACurrentAccount())
		{
			return it->second;
		}
		
	}
	return NULL;
}
void PlatformSvrMgr::GetAllPlatformSvr(set<IPlatformSingleSvr*>& vec)
{
	vec.clear();
	std::map<std::string, IPlatformSingleSvr*>::iterator it;
	for (it = m_allPlatform.begin(); it != m_allPlatform.end(); ++it)	
	{
		vec.insert(it->second);
	}
}
PlatformSvrMgr::PlatformSvrMgr()
{
	m_allPlatform.clear();
}

PlatformSvrMgr::~PlatformSvrMgr()
{
	std::map<std::string, IPlatformSingleSvr*>::iterator it = m_allPlatform.begin();
	for ( ; it != m_allPlatform.end(); )
	{
		if ( it->second != NULL )
		{
			//it->second->StopThread();
			//it->second->Stop();
			IPlatformSingleSvr::Release(it->second);
		}

		it = m_allPlatform.erase(it);
	}
}