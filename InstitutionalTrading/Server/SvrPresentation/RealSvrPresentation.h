#pragma once

#include "Interface_SvrPresentation.h"
#include "UserStatic.h"
#include "commonstruct.h"
#include <vector>
#include <map>
#include <set>
#include "SocketBase.h"
#include "KernelStruct.h"
#include "CommonStructEx.h"

class CRealSvrPresentation : public CInterface_SvrPresentation
{
public:
	static CRealSvrPresentation& getObj(void);


	virtual bool clear();

	virtual bool OnTimerStatisc();

	virtual bool GenerateEmptyEnrtyList(const AssetOrgProRelation& org, std::vector<StatisticsEntry>& vec);

	virtual bool GetStatisticsEntryList(const AssetOrgProRelation& org, std::vector<StatisticsEntry>& vec);

	void SubscribeStatistics(const AssetOrgProRelation& org,SOCKET nSocket)
	{
		if(m_SubscribeStatistics.find(org) == m_SubscribeStatistics.end())
		{
			std::set<SOCKET> lSetSocket;
			lSetSocket.insert(nSocket);
			m_SubscribeStatistics[org] = lSetSocket;
		}
		else
		{
			m_SubscribeStatistics[org].insert(nSocket);
		}	
	}
	void UnSubscribeStatistics(const AssetOrgProRelation& org,SOCKET nSocket)
	{
		if(m_SubscribeStatistics.find(org) != m_SubscribeStatistics.end())
		{			
			m_SubscribeStatistics[org].erase(nSocket);
			if(m_SubscribeStatistics[org].size() == 0)
				m_SubscribeStatistics.erase(org);
		}	
	}

	void SubscribeFund(const int org,SOCKET nSocket)
	{
		if(m_SubscribeFund.find(org) == m_SubscribeFund.end())
		{
			std::set<SOCKET> lSetSocket;
			lSetSocket.insert(nSocket);
			m_SubscribeFund[org] = lSetSocket;
		}
		else
		{
			m_SubscribeFund[org].insert(nSocket);
		}	
	}
	void UnSubscribeFund(const int org,SOCKET nSocket)
	{
		if(m_SubscribeFund.find(org) != m_SubscribeFund.end())
		{			
			m_SubscribeFund[org].erase(nSocket);
			if(m_SubscribeFund[org].size() == 0)
				m_SubscribeFund.erase(org);
		}	
	}

	void GetFundList(int nUserID,std::vector<sFundInfoEx>& vec);
private:


	//订阅
	std::map<AssetOrgProRelation,std::set<SOCKET>> m_SubscribeStatistics;
	std::map<int,std::set<SOCKET>> m_SubscribeFund;  //订阅资金变化

	//统计
	std::map<int,sFundInfoEx>	mmapFundItemUser;   //单个用户的资金结果
	std::map<int,CUserStatic>			mmapStatisticsItemUser;   //单个用户的统计结果
	std::map<int,CUserStatic>			mmapStatisticsItemOrg;   //单个组织机构的统计结果
	std::map<int,CUserStatic>			mmapStatisticsItemFinicial;   //单个理财产品的统计结果

	
	CRealSvrPresentation();
	virtual ~ CRealSvrPresentation();
	static CRealSvrPresentation* m_pObj;
};