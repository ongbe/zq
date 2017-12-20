#include "stdafx.h"
#include "RealSvrPresentation.h"
#include "..\SvrUserOrg\Interface_SvrUserOrg.h"
#include "..\SvrFinanceProduct\Interface_SvrFinanceProduct.h"
#include <vector>
#include "CommonStruct.h"
#include "..\SvrTradeData\Interface_SvrTradeData.h"
#include "..\SvrTcp\Interface_SvrTcp.h"
#include "CommonMacro.h"

CRealSvrPresentation* CRealSvrPresentation::m_pObj=NULL;


CRealSvrPresentation::CRealSvrPresentation()
{
}

CRealSvrPresentation::~CRealSvrPresentation()
{
}
CRealSvrPresentation& CRealSvrPresentation::getObj(void)
{
	// g_mutex.lock();
	if(!m_pObj)
		m_pObj=new CRealSvrPresentation();
	// g_mutex.unlock();
	return *m_pObj;
}

bool CRealSvrPresentation::clear()
{
	mmapStatisticsItemUser.clear();
	mmapStatisticsItemOrg.clear();
	mmapStatisticsItemFinicial.clear();
	mmapFundItemUser.clear();
	return true;
}


bool CRealSvrPresentation::GenerateEmptyEnrtyList( const AssetOrgProRelation& org,std::vector<StatisticsEntry>& vec)
{
	//加入initfund
	StatisticsEntry entry;
	memset(&entry,0,sizeof(StatisticsEntry));
	entry.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
	entry.mnFinancialProductID = org.mnFinancialProductID;
	entry.mnStatisticsType = ST_InitFund;
	entry.mdblValue = 0.0f;
	vec.push_back(entry);

	//加入posprofit
	StatisticsEntry entryProfit;
	memset(&entryProfit,0,sizeof(StatisticsEntry));
	entryProfit.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
	entryProfit.mnFinancialProductID = org.mnFinancialProductID;
	entryProfit.mnStatisticsType = ST_PosProfit;
	entryProfit.mdblValue = 0.0f;
	vec.push_back(entryProfit);

	//加入Balance
	StatisticsEntry entryBalance;
	memset(&entryBalance,0,sizeof(StatisticsEntry));
	entryBalance.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
	entryBalance.mnFinancialProductID = org.mnFinancialProductID;
	entryBalance.mnStatisticsType = ST_Balance;
	entryBalance.mdblValue = 0.0f;
	vec.push_back(entryBalance);

	//加入Volume
	StatisticsEntry entryPosition;
	memset(&entryPosition,0,sizeof(StatisticsEntry));
	entryPosition.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
	entryPosition.mnFinancialProductID = org.mnFinancialProductID;
	entryPosition.mnStatisticsType = ST_Position;
	entryPosition.mdblValue = 0.0f;
	vec.push_back(entryPosition);

	//加入Margin
	StatisticsEntry entryMargin;
	memset(&entryMargin,0,sizeof(StatisticsEntry));
	entryMargin.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
	entryMargin.mnFinancialProductID = org.mnFinancialProductID;
	entryMargin.mnStatisticsType = ST_UseMargin;
	entryMargin.mdblValue = 0.0f;
	vec.push_back(entryMargin);

	return true;
}
bool CRealSvrPresentation::GetStatisticsEntryList(const AssetOrgProRelation& org, std::vector<StatisticsEntry>& vec)
{
	
	//订阅的是组织机构的
	if(org.mnAssetMgmtOrgID != -1)
	{
		std::map<int,CUserStatic>::iterator lIter = mmapStatisticsItemOrg.find(org.mnAssetMgmtOrgID);
		if(mmapStatisticsItemOrg.end() != lIter)
		{
			lIter->second.GetStatisticsEntryList(org,vec);
		}	
		else
		{
			//push empty
			GenerateEmptyEnrtyList(org,vec);

		}

	}
	else if(org.mnFinancialProductID != -1)
	{
		//订阅的是理财产品的
		std::map<int,CUserStatic>::iterator lIter = mmapStatisticsItemFinicial.find(org.mnFinancialProductID);
		if(mmapStatisticsItemFinicial.end() != lIter)
		{
			lIter->second.GetStatisticsEntryList(org,vec);
		}	
		else
		{
			//push empty
			GenerateEmptyEnrtyList(org,vec);
		}

	}
	return true;
}
void CRealSvrPresentation::GetFundList(int nUserID,std::vector<sFundInfoEx>& vec)
{
	std::map<int,sFundInfoEx>::iterator lIter = mmapFundItemUser.find(nUserID);
	if(mmapFundItemUser.end() != lIter)
	{
		vec.push_back(lIter->second);
	}	
	else
	{
		sFundInfoEx lTemp;
		memset(&lTemp,0,sizeof(lTemp));
		//push empty
		vec.push_back(lTemp);

	}
}
bool CRealSvrPresentation::OnTimerStatisc()
{
	if(CInterface_SvrTradeData::getObj().GetTraderDataStatus() != TradeDataStatusTrading)
	{
		OUTPUT_LOG("Not Trading Status ,we do not need to presentation");
		return false;
	}
	clear();
    //统计每个用户的
	std::vector<UserInfo> lVecUserInfo;
	CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUserInfo);
	if(lVecUserInfo.size() == 0)
		return false;

	for(unsigned int i = 0 ; i < lVecUserInfo.size() ;i++)
	{
		if(lVecUserInfo[i].nUserType != USER_TYPE_TRADER)
			continue;
		CUserStatic  lStatic;	

		//获取初始权益
		double ldbInitfund = 0.0f;
		CInterface_SvrTradeData::getObj().GetUserInitFund(lVecUserInfo[i].szAccount,ldbInitfund);
		lStatic.SetInitFund(ldbInitfund);

		//获取当前总权益
		PlatformStru_TradingAccountInfo lAccountInfo;
		memset(&lAccountInfo,0,sizeof(lAccountInfo));
		CInterface_SvrTradeData::getObj().GetUserFundInfo(lVecUserInfo[i].szAccount,lAccountInfo);

		sFundInfoEx lFundEx;
		lFundEx.mdbInitFund  = ldbInitfund;
		memcpy(&lFundEx.mTradingAccountInfo,&lAccountInfo,sizeof(PlatformStru_TradingAccountInfo));
		mmapFundItemUser[lVecUserInfo[i].nUserID] = lFundEx;

		lStatic.SetPositionProfit(lAccountInfo.PositionProfit);
		lStatic.SetStaticBalance(lAccountInfo.DynamicProfit);

		std::vector<PlatformStru_Position> lVecPosition;
		CInterface_SvrTradeData::getObj().GetUserPositions(lVecUserInfo[i].szAccount,"",lVecPosition);
		std::vector<PlatformStru_Position>::iterator lIterPos = lVecPosition.begin();
		for(; lIterPos != lVecPosition.end();lIterPos++)
		{
			lStatic.PushInstrumentUserMargin(lIterPos->InstrumentID,lIterPos->UseMargin);
			lStatic.PushInstrumentVolumn(lIterPos->InstrumentID,lIterPos->Position);
		}
		mmapStatisticsItemUser[lVecUserInfo[i].nUserID] = lStatic;

	}

	//根据用户统计每个组织结构的
	//1.	持仓盈亏汇总
	//2.	账户初始权益汇总
	//3.	账户当前总权益汇总
	//4.	合约持仓保证金汇总
	//5.	合约的持仓量汇总
	std::vector<Organization> lVecOrgs;
	CInterface_SvrUserOrg::getObj().GetOrgs(lVecOrgs);
	if(lVecOrgs.size() != 0)
	{
		for(unsigned int i = 0; i < lVecOrgs.size();i++)
		{
			CUserStatic lStatic;
			std::vector<int> lVecOrgUserId;	
			double ldbInitFund,ldbPosiProfit,ldbStaticBalance;
			ldbStaticBalance = ldbPosiProfit = ldbInitFund = 0.0f;
			CInterface_SvrUserOrg::getObj().GetUsersIDByOrgID(lVecOrgs[i].nAssetMgmtOrgID,lVecOrgUserId);
			for(unsigned int j = 0; j < lVecOrgUserId.size(); j++)
			{			
				std::map<int,CUserStatic>::iterator lIterOrgUser = mmapStatisticsItemUser.find(lVecOrgUserId[j]);
				if(lIterOrgUser != mmapStatisticsItemUser.end())
				{
					ldbInitFund += lIterOrgUser->second.GetInitFund();
					ldbPosiProfit += lIterOrgUser->second.GetPositionProfit();
					ldbStaticBalance += lIterOrgUser->second.GetStaticBalance();

					std::set<string> lSetInstruemnt;
					lIterOrgUser->second.GetInsturments(lSetInstruemnt);
					std::set<string>::iterator lsetIter = lSetInstruemnt.begin();
					for(;lsetIter != lSetInstruemnt.end();lsetIter++)
					{
						lStatic.PushInstrumentUserMargin(*lsetIter,lIterOrgUser->second.GetInstruemntUserMargin(*lsetIter));
						lStatic.PushInstrumentVolumn(*lsetIter,lIterOrgUser->second.GetInstruemntUserVolumn(*lsetIter));
					}				

				}				

			}
			lStatic.SetInitFund(ldbInitFund);
			lStatic.SetPositionProfit(ldbPosiProfit);
			lStatic.SetStaticBalance(ldbStaticBalance);

			mmapStatisticsItemOrg[lVecOrgs[i].nAssetMgmtOrgID] = lStatic;
		}
	
	}



	//根据用户统计每个理财产品的
	//1.	持仓盈亏汇总
	//2.	账户初始权益汇总
	//3.	账户当前总权益汇总
	//4.	合约持仓保证金汇总
	//5.	合约的持仓量汇总
	std::vector<FinancialProduct> lVecFP;
	CInterface_SvrFinanceProduct::getObj().GetFinancialProduct(lVecFP);
	if(lVecFP.size() != 0)
	{
		for(unsigned int i = 0; i < lVecFP.size();i++)
		{
			CUserStatic lStatic;
			std::vector<int> lVecOrgUserInfo;	
			double ldbInitFund,ldbPosiProfit,ldbStaticBalance;
			ldbStaticBalance = ldbPosiProfit = ldbInitFund = 0.0f;
			CInterface_SvrFinanceProduct::getObj().GetTraderByFinancialProductID(lVecFP[i].nID,lVecOrgUserInfo);
			for(unsigned int j = 0; j < lVecOrgUserInfo.size(); j++)
			{
				
				std::map<int,CUserStatic>::iterator lIterOrgUser = mmapStatisticsItemUser.find(lVecOrgUserInfo[j]);
				if(lIterOrgUser != mmapStatisticsItemUser.end())
				{
					ldbInitFund += lIterOrgUser->second.GetInitFund();
					ldbPosiProfit += lIterOrgUser->second.GetPositionProfit();
					ldbStaticBalance += lIterOrgUser->second.GetStaticBalance();

					std::set<string> lSetInstruemnt;
					lIterOrgUser->second.GetInsturments(lSetInstruemnt);
					std::set<string>::iterator lsetIter = lSetInstruemnt.begin();
					for(;lsetIter != lSetInstruemnt.end();lsetIter++)
					{
						lStatic.PushInstrumentUserMargin(*lsetIter,lIterOrgUser->second.GetInstruemntUserMargin(*lsetIter));
						lStatic.PushInstrumentVolumn(*lsetIter,lIterOrgUser->second.GetInstruemntUserVolumn(*lsetIter));
					}				

				}				

			}
			lStatic.SetInitFund(ldbInitFund);
			lStatic.SetPositionProfit(ldbPosiProfit);
			lStatic.SetStaticBalance(ldbStaticBalance);

			mmapStatisticsItemFinicial[lVecFP[i].nID] = lStatic;
		}

	}

	//发送订阅消息	
	{
		std::map<AssetOrgProRelation,std::set<SOCKET>>::iterator lIterMap=m_SubscribeStatistics.begin();
		while(lIterMap!=m_SubscribeStatistics.end())
		{
			std::vector<StatisticsEntry> vec;
			GetStatisticsEntryList(lIterMap->first,vec);
			for(size_t i=0;i<vec.size();++i)
			{
				std::set<SOCKET>::iterator lIterSocket = lIterMap->second.begin();
				for(; lIterSocket != lIterMap->second.end() ;lIterSocket++)
				{
					CInterface_SvrTcp::getObj().SendPkgData(*lIterSocket,
						Cmd_RM_Statistics_Push,
						vec.size() == 0 ? NULL : &vec[0],
						sizeof(StatisticsEntry)*vec.size(),
						0,
						0,
						0,
						0,
						CF_ERROR_SUCCESS,0);					
				}				
			}
			lIterMap++;
		}
	}

	//发送订阅用户资金
	{

		std::map<int,std::set<SOCKET>>::iterator lIterMap=m_SubscribeFund.begin();
		while(lIterMap!=m_SubscribeFund.end())
		{
			std::vector<sFundInfoEx> vec;
			GetFundList(lIterMap->first,vec);
			for(size_t i=0;i<vec.size();++i)
			{
				std::set<SOCKET>::iterator lIterSocket = lIterMap->second.begin();
				for(; lIterSocket != lIterMap->second.end() ;lIterSocket++)
				{
					CInterface_SvrTcp::getObj().SendPkgData(*lIterSocket,
						Cmd_RM_UserFund_Push,
						vec.size() == 0 ? NULL : &vec[0],
						sizeof(sFundInfoEx)*vec.size(),
						0,
						0,
						0,
						0,
						CF_ERROR_SUCCESS,0);					
				}				
			}
			lIterMap++;
		}

	}
	return true;

}