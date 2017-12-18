#include "StdAfx.h"
#include "DBAccess.h"
#include "DataCenter.h"
#include "ConfigManager.h"
#include "RiskMsgQueue.h"
#include "../CloseOrder/ThostFtdcUserApiDataType.h"
#include "Tools/WriteLog.h"
#include <time.h>

#pragma   warning (disable:4503)
#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("DataCenter", WriteLogLevel_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif
#define SUBSCRIBE_INFO(AccID,pmField)\
	do{\
	std::string strAcc=GetTradeAccountID(AccID);\
	if(!strAcc.empty())\
	{\
	m_Mutex_SubscribeInfo.write_lock();\
	std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);\
	if(it!=m_SubscribeInfo.end())\
		{\
		SubscribeType& st=it->second;\
		st.pmField=true;	\
		}\
		else\
		{\
		SubscribeType st;\
		ZeroMemory(&st,sizeof(st));\
		st.AccID=AccID;\
		st.pmField=true;\
		m_SubscribeInfo[strAcc]=st;\
		}\
	m_Mutex_SubscribeInfo.write_unlock();\
	}\
	}while(0)
#define UNSUBSCRIBE_INFO(AccID,pmField)\
	do{\
	std::string strAcc=GetTradeAccountID(AccID);\
	if(!strAcc.empty())\
	{\
	std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);\
	m_Mutex_SubscribeInfo.write_lock();\
	if(it!=m_SubscribeInfo.end())\
		{\
		SubscribeType& st=it->second;\
		st.pmField=false;	\
		}\
	m_Mutex_SubscribeInfo.write_unlock();\
	}\
	}while(0)
#define SendSubscribedInfoMsg(InvestorID,pmField,cmdid,maindata)\
	do{\
	CRisk2ClientMsgQueue* pMsgQueue=CRisk2ClientMsgQueue::Get();	\
	if(pMsgQueue)\
	{\
	std::map<std::string,SubscribeType>::const_iterator it;\
	m_Mutex_SubscribeInfo.read_lock();\
	it=m_SubscribeInfo.find(InvestorID);\
	if(it!=m_SubscribeInfo.end())\
		{\
		if(it->second.pmField)\
			{\
			Risk2ClientData data={0};\
			data.CmdID=cmdid;\
			data.AccID=it->second.AccID;\
			data.Main.maindata=maindata;\
			pMsgQueue->AddMsg(data);\
			}\
		}\
		m_Mutex_SubscribeInfo.read_unlock();\
	}\
	}while(0)
#define DATA_CLEAR(tag)\
	do{\
	m_Mutex_##tag.write_lock();\
	m_##tag.clear();	\
	m_Mutex_##tag.write_unlock();\
	}while(0)
#define DATA_CLEAR2(tag,mtx)\
	do{\
	m_Mutex_##mtx.write_lock();\
	m_##tag.clear();	\
	m_Mutex_##mtx.write_unlock();\
	}while(0)
#define STAT_DELAY_TIME 2
extern int g_nRiskEventHandlingID;
extern bool g_NeedSaveUserSpi;
CDataCenter* CDataCenter::m_DataCenter=NULL;
CDataCenter::CDataCenter(void)
:m_StatisticsUpdate(0)
,m_LoginTime(0)
,m_lastStatus(true)
,m_nRiskEventHandlingID(0)
,m_nRiskEventID(0)
{
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"DataCenter.log");
	m_dbAccess.InitDB(ConfigManager::Get()->GetDBUser(), 
		ConfigManager::Get()->GetDBPwd(), 
		ConfigManager::Get()->GetDBSID());
	Reset();
	m_bRiskWarningChanged=false;
	m_pWriteLogTest = NULL;
}
CDataCenter::~CDataCenter(void)
{
	//WriteTodayRiskEvent(m_HistoryEvent,0,m_Mutex_RiskEvent,"History.evt");
	//WriteTodayRiskEvent(m_ActiveEvent,m_nRiskEventID,m_Mutex_RiskEvent,"Active.evt");
	//WriteTodayRiskEvent(m_Handling,m_nRiskEventHandlingID,m_Mutex_RiskEvent,"Handling.evt");
	//WRITESIMPLECTPDATA(PreRiskFundAccount);
	//WRITESIMPLECTPDATA(RiskFundAccount);
	//WRITECOMPLEXCTPDATA(AllPosition);
	//WRITECOMPLEXCTPDATA(AllTrade);
	//WRITECOMPLEXCTPDATA(AllOrder);
	//WRITECOMPLEXCTPDATA(AllDeposit);
	//WRITESIMPLECTPDATA(AllProduct);
	//WRITESIMPLECTPDATA(AllInstrument);
	//WRITESIMPLECTPDATA(DepthMarketData);
	SAFE_DELETE(m_pWriteLog);
}
void CDataCenter::Init(void)
{
	if(m_DataCenter)
	{
		Release();
	}
	m_DataCenter=new CDataCenter();
}
void CDataCenter::Release(void)
{
	SAFE_DELETE(m_DataCenter);
}
CDataCenter* CDataCenter::Get()
{
	return m_DataCenter;
}

void CDataCenter::GetStatisticsEntryList(const AssetOrgProRelation& org, std::vector<StatisticsEntry>& vec ,bool bProtected) const
{
	int i;
	double dblValue;
	std::map<AssetOrgProRelation,double>::const_iterator  Total_it;
	if(bProtected) m_Mutex_Statistics.read_lock();
	for (i=0;i<3;++i)
	{
		Total_it=m_StatisticsTotalValue[i].find(org);
		if(Total_it!=m_StatisticsTotalValue[i].end())
			dblValue=Total_it->second;
		else
			dblValue=0;
		StatisticsEntry entry;
		memset(&entry,0,sizeof(StatisticsEntry));
		switch(i){
			case 0:entry.nStatisticsType=ST_PosProfit;break;
			case 1:entry.nStatisticsType=ST_InitFund;break;
			case 2:entry.nStatisticsType=ST_Balance;break;
		}
		entry.nAssetMgmtOrgID=org.nAssetMgmtOrgID;
		entry.nFinancialProductID=org.nFinancialProductID;
		entry.dblValue=dblValue;
		vec.push_back(entry);
	}

	std::map<AssetOrgProRelation,std::map<std::string,double>>::const_iterator  Instr_it;
	for (i=0;i<2;++i)
	{
		bool bEmpty=false;
		Instr_it=m_StatisticsInstrValue[i].find(org);
		if(Instr_it!=m_StatisticsInstrValue[i].end())
		{
			std::map<std::string,double>::const_iterator nestit;
			const std::map<std::string,double>& nestmap=Instr_it->second;
			if(nestmap.size()>0)
			{
				for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
				{
					StatisticsEntry entry;
					memset(&entry,0,sizeof(StatisticsEntry));
					switch(i){
					case 0:entry.nStatisticsType=ST_UseMargin;break;
					case 1:entry.nStatisticsType=ST_Position;break;
					}
					entry.nAssetMgmtOrgID=org.nAssetMgmtOrgID;
					entry.nFinancialProductID=org.nFinancialProductID;
					safestrcpy(entry.InstrumentID,sizeof(InstrumentIDType),nestit->first.c_str());
					entry.dblValue=nestit->second;
					vec.push_back(entry);
				}
			}
			else
				bEmpty=true;

		}
		else
			bEmpty=true;
		if(bEmpty)
		{
			StatisticsEntry entry;
			memset(&entry,0,sizeof(StatisticsEntry));
			switch(i){
					case 0:entry.nStatisticsType=ST_UseMargin;break;
					case 1:entry.nStatisticsType=ST_Position;break;
			}
			entry.nAssetMgmtOrgID=org.nAssetMgmtOrgID;
			entry.nFinancialProductID=org.nFinancialProductID;
			vec.push_back(entry);
		}
	}
	if(bProtected) m_Mutex_Statistics.read_unlock();
}
std::map<std::string,double>& CDataCenter::GetComplexMapIterator(
std::map<AssetOrgProRelation,std::map<std::string,double>>& itemmap,AssetOrgProRelation org)
{
	std::map<AssetOrgProRelation,std::map<std::string,double>>::iterator it;
	it=itemmap.find(org);
	if(it==itemmap.end())
	{
		std::map<std::string,double> nestmap;
		itemmap[org]=nestmap;
		it=itemmap.find(org);
	}
	return it->second;
}
void CDataCenter::OnTimerStatistics()
{
	if(InterlockedDecrement(&m_StatisticsUpdate)>0)
	{
		m_Mutex_Statistics.write_lock();
		std::map<AssetOrgProRelation,std::set<std::string>>::iterator it;
		std::set<std::string>::iterator ssit;
		m_StatisticsTotalValue[0].clear();//1.	持仓盈亏汇总
		m_StatisticsTotalValue[1].clear();//2.	账户初始权益汇总
		m_StatisticsTotalValue[2].clear();//3.	账户当前总权益汇总
		m_StatisticsInstrValue[0].clear();//4.	合约持仓保证金汇总
		m_StatisticsInstrValue[1].clear();//5.	合约的持仓量汇总
		for (it=m_StatisticsItem.begin();it!=m_StatisticsItem.end();++it)
		{
			std::set<std::string>& ress=it->second;
			for(ssit=ress.begin();ssit!=ress.end();++ssit)
			{
				m_StatisticsTotalValue[0][it->first]+=GetPositionProfit(*ssit);//1.	持仓盈亏汇总
				m_StatisticsTotalValue[1][it->first]+=GetInitFund(*ssit);//2.	账户初始权益汇总
				double dbValue = 0;
				GetBalance(*ssit, dbValue);
				m_StatisticsTotalValue[2][it->first]+=dbValue;//3.	账户当前总权益汇总
				std::map<std::string,double>& UseMarginMap=GetComplexMapIterator(m_StatisticsInstrValue[0],it->first);
				std::map<std::string,double>& PositionMap=GetComplexMapIterator(m_StatisticsInstrValue[1],it->first);
				std::vector<SequencialPositionField> vec;
				GetPositionList(GetTradeAccountID(*ssit),vec);
				for (size_t i=0;i<vec.size();++i)
				{
					SequencialPositionField& pos=vec[i];
					UseMarginMap[pos.InstrumentID]+=pos.UseMargin;//4.	合约持仓保证金汇总
					PositionMap[pos.InstrumentID]+=pos.Position;//5.	合约的持仓量汇总
				}
			}
			//发送订阅消息
			CRisk2ClientMsgQueue* pMsgQueue=CRisk2ClientMsgQueue::Get();
			if(pMsgQueue)
			{
				std::set<AssetOrgProRelation>::const_iterator setit=m_SubscribeStatistics.find(it->first);
				if(setit!=m_SubscribeStatistics.end())
				{
					std::vector<StatisticsEntry> vec;
					GetStatisticsEntryList(*setit,vec,false);
					for(size_t i=0;i<vec.size();++i)
					{
						Risk2ClientData data={0};
						if(i==0)
							data.AccID|=-STATISTICS_DATA_BEGIN;//开始
						if(i==vec.size()-1)
							data.AccID|=-STATISTICS_DATA_END;//结束
						data.CmdID=R2CMSGID_STATISTICS;
						data.Main.Statistics=vec[i];
						pMsgQueue->AddMsg(data);
					}
				}
			}
		}
		m_Mutex_Statistics.write_unlock();
	}
}
void CDataCenter::ReadAssetOrgProRelation()
{
//	DATA_CLEAR2(OrgIDMap,Statistics);
//	DATA_CLEAR2(StatisticsItem,Statistics);


	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf_s(szBuffer,MAX_SQL_LENGTH, "select * from PRODUCTANDORGRELATION t");
	std::set<std::string> ss;
	std::vector<AssetOrgProRelation> vec0;
	if (!m_dbAccess.QueryData(szBuffer, vec0))
		return;
	
	m_Mutex_Statistics.write_lock();
	m_StatisticsItem.clear();
	for (size_t i=0;i<vec0.size();++i)
	{
		m_StatisticsItem[vec0[i]]=ss;
	}
	m_Mutex_Statistics.write_unlock();
	
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf_s(szBuffer, MAX_SQL_LENGTH,"select * from FINANCIALPRODUCT t");
	std::vector<FinancialProduct> vec1;
	if (!m_dbAccess.QueryData(szBuffer, vec1))
		return;	
	m_Mutex_Statistics.write_lock();
	//m_StatisticsItem.clear();
	for (size_t i=0;i<vec1.size();++i)
	{
		AssetOrgProRelation re;
		re.nAssetMgmtOrgID=-1;
		re.nFinancialProductID=vec1[i].nFinancialProductID;
		m_StatisticsItem[re]=ss;
	}
	m_Mutex_Statistics.write_unlock();
	
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf_s(szBuffer,MAX_SQL_LENGTH, "select * from ASSETMGMTORGANIZATION t");
	std::vector<AssetMgmtOrganization> vec2;
	if (!m_dbAccess.QueryData(szBuffer, vec2))
		return;
	
	m_Mutex_Statistics.write_lock();
	m_OrgIDMap.clear();
	for (size_t i=0;i<vec2.size();++i)
	{
		AssetOrgProRelation re;
		re.nAssetMgmtOrgID=vec2[i].nAssetMgmtOrgID;
		re.nFinancialProductID=-1;
		m_StatisticsItem[re]=ss;
		m_OrgIDMap[vec2[i].nAssetMgmtOrgID]=vec2[i].nUpperLevelOrgID;
	}
	m_Mutex_Statistics.write_unlock();
	
	std::map<std::string,TrustTradeAccount>::const_iterator it;
	for (it=m_AccountList.begin();it!=m_AccountList.end();++it)
	{
		
		m_Mutex_AccountList.write_lock();
		TrustTradeAccount acc=it->second;
		m_Mutex_AccountList.write_unlock();
		InsertStatisticsItem(acc);
	}
}
void CDataCenter::ReadTrustTradeAccount()
{
	//DATA_CLEAR(AccountList);
	//DATA_CLEAR2(AccountID2Str,AccountList);
	//获取账户列表
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	strcpy_s(szBuffer,MAX_SQL_LENGTH, "select * from TRUSTTRADEACCOUNT t");
	std::vector<TrustTradeAccount> vec;
	if (!m_dbAccess.QueryData(szBuffer, vec))
		return;
	m_Mutex_AccountList.write_lock();
	m_AccountList.clear();
	m_AccountID2Str.clear();
	for (size_t i=0;i<vec.size();++i)
	{		
		m_AccountList[vec[i].szAccount]=vec[i];
		m_AccountID2Str[vec[i].nTradeAccountID]=vec[i].szAccount;		
	}
	m_Mutex_AccountList.write_unlock();
	for (size_t i=0;i<vec.size();++i)
	{
		InsertStatisticsItem(vec[i]);
	}
}
void CDataCenter::ReadNetFundParam()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	strcpy_s(szBuffer,MAX_SQL_LENGTH, "select * from FUNDNETPARAM t");
	std::vector<NetFundParam> vec;
	if (!m_dbAccess.QueryData(szBuffer, vec))
		return;
	m_Mutex_Account2NetFundParam.write_lock();
	m_mapAccount2NetFundParam.clear();	
	for (size_t i=0;i<vec.size();++i)
	{			
		const std::string strAccount = GetTradeAccountID(vec[i].nTradeAccountID);
		m_mapAccount2NetFundParam[strAccount]=vec[i];		
	}
	m_Mutex_Account2NetFundParam.write_unlock();
}
void CDataCenter::ReadForceCloseParam()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	strcpy_s(szBuffer,MAX_SQL_LENGTH, "select * from FORCECLOSEPARAM t");
	std::vector<ForceCloseParam> vec;
	if (!m_dbAccess.QueryData(szBuffer, vec))
		return;

	m_Mutex_Account2ForceCloseParam.write_lock();
	m_mapAccount2ForceCloseParam.clear();	
	for (size_t i=0;i<vec.size();++i)
	{		
		const std::string strAccount = GetTradeAccountID(vec[i].nTradeAccountID);
		m_mapAccount2ForceCloseParam[strAccount] = vec[i];		
	}
	m_Mutex_Account2ForceCloseParam.write_unlock();
}
void CDataCenter::ReadGatherData()
{
	char strTime2[64];
	memset(&strTime2, 0, sizeof(strTime2));
	SYSTEMTIME st;
	GetLocalTime(&st);	
	sprintf(strTime2,"%4d-%02d-%02d",st.wYear, st.wMonth, st.wDay);
	std::vector<SGatherData> vGatherData;
	m_dbAccess.QueryData(strTime2,  vGatherData);
	
	for(int i =0; i< (int)vGatherData.size(); i++)
	{
		SGatherData sData = vGatherData[i];		
		if(sData.nIDType == 0)
		{
			m_Mutex_OrgID2GatherData.read_lock();
			m_mapOrgID2GatherData[sData.nID] = sData;
			m_Mutex_OrgID2GatherData.read_unlock();
		}
		else if(sData.nIDType == 1)
		{
			m_Mutex_FinancialID2GatherData.read_lock();
			m_mapFinancialID2GatherData[sData.nID] = sData;
			m_Mutex_FinancialID2GatherData.read_unlock();
		}
		else if(sData.nIDType == 2)
		{
			m_Mutex_AccountID2GatherData.read_lock();
			m_mapAccountID2GatherData[sData.nID] = sData;
			m_Mutex_AccountID2GatherData.read_unlock();
		}
	}

	

}

bool CDataCenter::HasForceClosePermission(const std::string& Investor)
{
	bool bHasRight = false;
	m_Mutex_Account2ForceCloseParam.write_lock();	
	const std::string strAccount = Investor;
	std::map<std::string,ForceCloseParam>::iterator it = m_mapAccount2ForceCloseParam.find(strAccount);
	if(it != m_mapAccount2ForceCloseParam.end())
		bHasRight = true;	
	m_Mutex_Account2ForceCloseParam.write_unlock();
	return bHasRight;
}
void CDataCenter::GetUpperLevelOrgID(AssetMgmtOrgIDType id,std::set<AssetMgmtOrgIDType>& ups)
{
	AssetMgmtOrgIDType org=id;
	ups.insert(org);
	std::map<AssetMgmtOrgIDType,AssetMgmtOrgIDType>::iterator it;
	it=m_OrgIDMap.find(id);	
	while (it!=m_OrgIDMap.end())
	{
		org=it->second;
		if(org>0)
		{
			ups.insert(org);
		}
		else
			break;
		it=m_OrgIDMap.find(org);
	}
}
void CDataCenter::InsertStatisticsItem( const TrustTradeAccount& acc )
{
	std::set<AssetMgmtOrgIDType> ss;
	std::set<AssetMgmtOrgIDType>::iterator it;
	GetUpperLevelOrgID(acc.nAssetMgmtOrgID,ss);
	AssetOrgProRelation re;
	std::map<AssetOrgProRelation,std::set<std::string>>::iterator reit;
	m_Mutex_Statistics.write_lock();
	for (it=ss.begin();it!=ss.end();++it)
	{

		re.nAssetMgmtOrgID=*it;
		re.nFinancialProductID=-1;
		reit=m_StatisticsItem.find(re);
		if(reit!=m_StatisticsItem.end())
		{
			std::set<std::string>& ress=reit->second;
			ress.insert(acc.szAccount);
		}
		re.nAssetMgmtOrgID=*it;
		re.nFinancialProductID=acc.nFinancialProductID;
		reit=m_StatisticsItem.find(re);
		if(reit!=m_StatisticsItem.end())
		{
			std::set<std::string>& ress=reit->second;
			ress.insert(acc.szAccount);
		}
	}
	re.nAssetMgmtOrgID=-1;
	re.nFinancialProductID=acc.nFinancialProductID;
	reit=m_StatisticsItem.find(re);
	if(reit!=m_StatisticsItem.end())
	{
		std::set<std::string>& ress=reit->second;
		ress.insert(acc.szAccount);
	}
	m_Mutex_Statistics.write_unlock();
}
void CDataCenter::MakeRiskWarningMap(RiskIndicatorType type,RiskIndModuleType module,
									 int level,const RiskResponse& warning,RISKWARNINGSTRUCT* pMapVec)
{
	RISKWARNINGSTRUCT& pMap=pMapVec[type];
	RISKWARNINGSTRUCT::iterator it;
	it=pMap.find(module);
	if(it!=pMap.end())
	{
		std::map<RiskLevelType,RiskResponse>& resmap=it->second;
		resmap[level]=warning;
	}
	else
	{	
		std::map<int,RiskResponse> resmap;
		resmap[level]=warning;
		pMap[module]=resmap;
	}
}
void CDataCenter::MakeRiskWarningMap(RiskIndicatorType type,std::string strAcc,
									 std::string ProductID,int level,const RiskResponse& warning,
									 std::map<std::string,std::map<RiskLevelType,RiskResponse>>& tmap)
{
	if(type==RI_MaxPosition)
	{
		std::map<std::string,std::map<RiskLevelType,RiskResponse>>::iterator it;
		std::string strAccAndProduct=GetSpecialIDFromProduct(strAcc,ProductID);
		it=tmap.find(strAccAndProduct);
		if(it!=tmap.end())
		{
			std::map<RiskLevelType,RiskResponse>& resmap=it->second;
			resmap[level]=warning;
		}
		else
		{
			std::map<int,RiskResponse> resmap;
			resmap[level]=warning;
			tmap[strAccAndProduct]=resmap;
		}
	}
	else if(type==RI_ProductMaxPositon)
	{
		std::map<std::string,std::map<RiskLevelType,RiskResponse>>::iterator it;
		std::string strAccAndProduct=GetSpecialIDFromProduct(strAcc,ProductID);
		it=tmap.find(strAccAndProduct);
		if(it!=tmap.end())
		{
			std::map<RiskLevelType,RiskResponse>& resmap=it->second;
			resmap[level]=warning;
		}
		else
		{
			std::map<int,RiskResponse> resmap;
			resmap[level]=warning;
			tmap[strAccAndProduct]=resmap;
		}
	}
}
template<typename K,typename V>
bool CDataCenter::IsSameMap(const std::map<K,V>& map1,const std::map<K,V>& map2)
{
	if(map1.size()!=map2.size())
		return false;
	std::set<K> keys1,keys2;
	std::map<K,V>::const_iterator it,it1,it2;
	std::set<K>::const_iterator keyit;
	for (it=map1.begin();it!=map1.end();++it) keys1.insert(it->first);
	for (it=map2.begin();it!=map2.end();++it) keys2.insert(it->first);
	if(keys1!=keys2)
		return false;
	for (keyit=keys1.begin();keyit!=keys1.end();++keyit)
	{
		it1=map1.find(*keyit);
		it2=map2.find(*keyit);
		if(it1->second!=it2->second)
			return false;
	}
	return true;
}
template<typename K>
bool CDataCenter::IsSameRiskWarning(const std::map<K,std::map<RiskLevelType,RiskResponse>>& map1,
									const std::map<K,std::map<RiskLevelType,RiskResponse>>& map2)
{
	if(map1.size()!=map2.size())
		return false;
	std::set<K> keys1,keys2;
	std::map<K,std::map<RiskLevelType,RiskResponse>>::const_iterator it,it1,it2;
	std::set<K>::const_iterator keyit;
	for (it=map1.begin();it!=map1.end();++it) keys1.insert(it->first);
	for (it=map2.begin();it!=map2.end();++it) keys2.insert(it->first);
	if(keys1!=keys2)
		return false;
	for (keyit=keys1.begin();keyit!=keys1.end();++keyit)
	{
		it1=map1.find(*keyit);
		it2=map2.find(*keyit);
		if(!IsSameMap(it1->second,it2->second))
			return false;
	}
	return true;
}
void CDataCenter::ReadRiskWarning()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	strcpy_s(szBuffer,MAX_SQL_LENGTH, "select * from PRODUCTTHRESHOLD t");
	std::vector<ProductRiskWarning> vec;
	if (m_dbAccess.QueryData(szBuffer, vec))
	{
		std::map<std::string,std::map<RiskLevelType,RiskResponse>> tmap;
		std::map<std::string,std::map<RiskLevelType,RiskResponse>> tmap_Product;
		for (size_t i=0;i<vec.size();++i)
		{
			ProductRiskWarning& ex=vec[i];
			RiskResponse	warning;
			warning.nRiskLevelID=ex.nRiskLevelID;
			
			warning.nColor=ex.nColor;
			warning.nResponseType=ex.nResponseType;
			std::string strAcc=GetTradeAccountID(ex.nTradeAccountID);
			if(ex.nRiskIndicatorID == RI_MaxPosition)
			{
				warning.fThresholdValue=ex.fThresholdValue/100;//产品最大持仓的时候，这个值是相对于市场总持仓的百分比
				MakeRiskWarningMap(ex.nRiskIndicatorID,strAcc,ex.szProductID,ex.nRiskLevelID,warning,tmap);
			}
			else if(ex.nRiskIndicatorID == RI_ProductMaxPositon)
			{
				warning.fThresholdValue=ex.fThresholdValue;//产品最大持仓的时候，这个值是手数
				MakeRiskWarningMap(ex.nRiskIndicatorID,strAcc,ex.szProductID,ex.nRiskLevelID,warning,tmap_Product);
			}

		}
		m_Mutex_RiskWarning.write_lock();
		if(!IsSameRiskWarning(m_SpecialMaxPositionRiskWarning,tmap))
		{
			m_SpecialMaxPositionRiskWarning=tmap;
			m_bRiskWarningChanged=true;
		}
		if(!IsSameRiskWarning(m_SpecialProductMaxPositionRiskWarning,tmap_Product))
		{
			m_SpecialProductMaxPositionRiskWarning=tmap_Product;
			m_bRiskWarningChanged=true;
		}
		m_Mutex_RiskWarning.write_unlock();
	}
	memset(szBuffer, 0, sizeof(szBuffer));
	strcpy_s(szBuffer,MAX_SQL_LENGTH, "select * from RISKWARNING t");
	std::vector<RiskWarning> vecWarning;
	if (m_dbAccess.QueryData(szBuffer, vecWarning))
	{
		RISKWARNINGSTRUCT MapVec[RI_Count];//这个数值是风控指标数目+1，每增加一个指标，这个数值加上1
		for (size_t i=0;i<vecWarning.size();++i)
		{
			RiskWarning& ex=vecWarning[i];
			RiskResponse	warning;
			warning.nRiskLevelID=ex.nRiskLevelID;
			if(ex.nRiskIndicatorID == RI_ProductMaxPositon)
				warning.fThresholdValue=ex.fThresholdValue;//产品最大持仓的时候，这个值是手数
			else if(ex.nRiskIndicatorID == RI_FundNetValue)
				warning.fThresholdValue=ex.fThresholdValue;//基金净值
			else
				warning.fThresholdValue=ex.fThresholdValue/100;
			warning.nColor=ex.nColor;
			warning.nResponseType=ex.nResponseType;
			MakeRiskWarningMap(ex.nRiskIndicatorID,ex.nRiskIndModuleID,ex.nRiskLevelID,warning,MapVec);
		}
		m_Mutex_RiskWarning.write_lock();
		if(!IsSameRiskWarning(m_MaxPositionRiskWarning,MapVec[RI_MaxPosition]))
		{
			m_MaxPositionRiskWarning=MapVec[RI_MaxPosition];
			m_bRiskWarningChanged=true;
		}
		if(!IsSameRiskWarning(m_FundLossRiskWarning,MapVec[RI_FundLoss]))
		{
			m_FundLossRiskWarning=MapVec[RI_FundLoss];
			m_bRiskWarningChanged=true;
		}
		if(!IsSameRiskWarning(m_GappedMarketRiskWarning,MapVec[RI_GappedMarket]))
		{
			m_GappedMarketRiskWarning=MapVec[RI_GappedMarket];
			m_bRiskWarningChanged=true;
		}
		if(!IsSameRiskWarning(m_MarginRatoRiskWarning,MapVec[RI_MarginRatio]))
		{
			m_MarginRatoRiskWarning=MapVec[RI_MarginRatio];
			m_bRiskWarningChanged=true;
		}
		if(!IsSameRiskWarning(m_ProductMaxPositionRiskWarning,MapVec[RI_ProductMaxPositon]))
		{
			m_ProductMaxPositionRiskWarning=MapVec[RI_ProductMaxPositon];
			m_bRiskWarningChanged=true;
		}
		if(!IsSameRiskWarning(m_FundNetRiskWarning,MapVec[RI_FundNetValue]))
		{
			m_FundNetRiskWarning=MapVec[RI_FundNetValue];
			m_bRiskWarningChanged=true;
		}
		m_Mutex_RiskWarning.write_unlock();
	}
}
void CDataCenter::ReadConfigXml(const std::string strfilename)
{
	memset(&m_LoginField,0,sizeof(ReqRiskUserLoginField));
	m_strFrontAddr.clear();
	m_LoginTime=0;
	m_lastStatus=true;
	TiXmlDocument myDocument;
	if(myDocument.LoadFile(strfilename))
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(XML_ELEMENTTEXT(RootElement)=="root")
		{
			TiXmlElement* pFrontElement=RootElement->FirstChildElement();
			if(XML_ELEMENTTEXT(pFrontElement)=="front")
			{
				TiXmlElement* p=pFrontElement->FirstChildElement();
				while(p)
				{
					std::string tag=XML_ELEMENTTEXT(p);
					if(tag=="ipaddress")
					{
						const char* pstr=p->Attribute("value");
						if(pstr) m_strFrontAddr.push_back(pstr);
					}
					else if(tag=="userid")
					{
						const char* pstr=p->Attribute("value");
						if(pstr) safestrcpy(m_LoginField.UserID,sizeof(TUserIDType),pstr);
					}
					else if(tag=="bookid")
					{
						const char* pstr=p->Attribute("value");
						if(pstr) safestrcpy(m_LoginField.BrokerID,sizeof(TBrokerIDType),pstr);
					}
					else if(tag=="password")
					{
						const char* pstr=p->Attribute("value");
						if(pstr) safestrcpy(m_LoginField.Password,sizeof(TPasswordType),pstr);
					}
					else if (tag=="tradeAddress")
					{
						const char* pstr = p->Attribute("value");
						if(pstr) m_strTradeAddr = pstr;
					}
					p=p->NextSiblingElement();
				}
			}
		}
	}
}
void CDataCenter::AddProduct(const ProductField& Product )
{
	m_Mutex_AllProduct.write_lock();
	m_AllProduct[Product.ProductID]=Product;
	m_Mutex_AllProduct.write_unlock();
}
void CDataCenter::AddInstrument(const InstrumentField& Instrument)
{
	m_AllInstrument[Instrument.InstrumentID]=Instrument;
}
void CDataCenter::AddMarketData(const DepthMarketDataField& MarketData)
{
	m_Mutex_DepthMarketData.write_lock();
	m_DepthMarketData[MarketData.InstrumentID]=MarketData;
	m_Mutex_DepthMarketData.write_unlock();
	//发送订阅消息
	CRisk2ClientMsgQueue* pMsgQueue=CRisk2ClientMsgQueue::Get();
	if(pMsgQueue)
	{
		m_Mutex_SubscribeQuot.read_lock();
		std::set<std::string>::const_iterator it=m_SubscribeQuot.find(MarketData.InstrumentID);
		if(it!=m_SubscribeQuot.end())
		{
			Risk2ClientData data={0};
			data.CmdID=R2CMSGID_QUOT;
			data.Main.MarketData=MarketData;
			pMsgQueue->AddMsg(data);
		}
		m_Mutex_SubscribeQuot.read_unlock();
	}
}
void CDataCenter::AddRiskFundAccount(const RiskSyncAccountField& RiskSyncAccount)
{
	InterlockedExchange(&m_StatisticsUpdate,STAT_DELAY_TIME);
	m_Mutex_RiskFundAccount.write_lock();
	m_RiskFundAccount[RiskSyncAccount.InvestorID]=RiskSyncAccount;

	LOG_INFO("总权益更新:账号：%s,  总权益：%.8f", RiskSyncAccount.InvestorID,RiskSyncAccount.Balance);
	m_Mutex_RiskFundAccount.write_unlock();	
	SendSubscribedInfoMsg(RiskSyncAccount.InvestorID,bFund,R2CMSGID_FUND,RiskSyncAccount);
}
void CDataCenter::AddAcoount2NetValue(NetValueInfo& netValueInfo)
{	
	m_Mutex_Accout2NetValue.write_lock();
	std::string strID = netValueInfo.szAccount;
	m_Accout2NetValue[strID]=netValueInfo;
	m_Mutex_Accout2NetValue.write_unlock();	
//	SendSubscribedInfoMsg(strID,bNetValue,R2CMSGID_NETVALUE,netValueInfo);

//#define SendSubscribedInfoMsg(InvestorID,pmField,cmdid,maindata)\

	CRisk2ClientMsgQueue* pMsgQueue=CRisk2ClientMsgQueue::Get();	
	if(pMsgQueue)
	{
		std::map<std::string,SubscribeType>::const_iterator it;
		m_Mutex_SubscribeInfo.read_lock();
		it=m_SubscribeInfo.find(strID);
		if(it!=m_SubscribeInfo.end())
		{
			if(it->second.bNetValue)
			{
				Risk2ClientData data={0};
				data.CmdID=R2CMSGID_NETVALUE;
				data.AccID=it->second.AccID;
				data.Main.netValueInfo=netValueInfo;
				pMsgQueue->AddMsg(data);
			}
		}
		m_Mutex_SubscribeInfo.read_unlock();
	}
}
void CDataCenter::AddPreRiskFundAccount(const SeqPreRiskAccountField& PreRiskAccount)
{
	m_Mutex_PreRiskFundAccount.write_lock();
	m_PreRiskFundAccount[PreRiskAccount.InvestorID]=PreRiskAccount;
	m_Mutex_PreRiskFundAccount.write_unlock();
}
void CDataCenter::AddPosition(const SequencialPositionField& Position)
{
	m_Mutex_AllPosition.write_lock();
	POSITIONSTRUCT::iterator it=m_AllPosition.find(Position.InvestorID);
	if(it!=m_AllPosition.end())
	{
		it->second[Position.UniqSequenceNo]=Position;
	}
	else
	{
		std::map<int,SequencialPositionField> pos_map;
		pos_map[Position.UniqSequenceNo]=Position;
		m_AllPosition[Position.InvestorID]=pos_map;
	}
	m_Mutex_AllPosition.write_unlock();
	InterlockedExchange(&m_StatisticsUpdate,STAT_DELAY_TIME);
	SendSubscribedInfoMsg(Position.InvestorID,bPosition,R2CMSGID_POSITION,Position);
}
void CDataCenter::AddTrade(const SequencialTradeField& Trade)
{
	m_Mutex_AllTrade.write_lock();
	TRADESTRUCT::iterator it=m_AllTrade.find(Trade.InvestorID);
	if(it!=m_AllTrade.end())
	{
		it->second[Trade.UniqSequenceNo]=Trade;
	}
	else
	{
		std::map<int,SequencialTradeField> trade_map;
		trade_map[Trade.UniqSequenceNo]=Trade;
		m_AllTrade[Trade.InvestorID]=trade_map;
	}
	m_Mutex_AllTrade.write_unlock();
	SendSubscribedInfoMsg(Trade.InvestorID,bTrade,R2CMSGID_TRADE,Trade);
}
void CDataCenter::AddOrder(const SequencialOrderField& Order)
{
	m_Mutex_AllOrder.write_lock();
	ORDERSTRUCT::iterator it=m_AllOrder.find(Order.InvestorID);
	if(it!=m_AllOrder.end())
	{
		it->second[Order.UniqSequenceNo]=Order;
	}
	else
	{
		std::map<int,SequencialOrderField> order_map;
		order_map[Order.UniqSequenceNo]=Order;
		m_AllOrder[Order.InvestorID]=order_map;
	}
	m_Mutex_AllOrder.write_unlock();
	SendSubscribedInfoMsg(Order.InvestorID,bOrder,R2CMSGID_ORDER,Order);
}
void CDataCenter::AddSeqDeposit(const SeqDepositField& Deposit)
{
	std::map<std::string,std::map<int,SeqDepositField>>::iterator it;
	m_Mutex_AllDeposit.write_lock();
	it=m_AllDeposit.find(Deposit.InvestorID);
	if(it!=m_AllDeposit.end())
	{
		it->second[Deposit.UniqSequenceNo]=Deposit;
	}
	else
	{
		std::map<int,SeqDepositField> deposit_map;
		deposit_map[Deposit.UniqSequenceNo]=Deposit;
		m_AllDeposit[Deposit.InvestorID]=deposit_map;
	}
	m_Mutex_AllDeposit.write_unlock();
	SendSubscribedInfoMsg(Deposit.InvestorID,bDeposit,R2CMSGID_DEPOSIT,Deposit);
}
void CDataCenter::GetUserLoginField(ReqRiskUserLoginField& LoginField) const
{
	LoginField=m_LoginField;
}
//获取前端服务器地址列表
void CDataCenter::GetrFrontAddr(std::vector<std::string>& strFrontAddr ) const
{
	strFrontAddr=m_strFrontAddr;
}
void CDataCenter::GetTradeAddr(std::string& strTradeAddr) const
{
	strTradeAddr = m_strTradeAddr;
}

bool CDataCenter::AccountListIsExist(const std::string& InvestorID)
{
	bool bRet=false;
	std::map<std::string,TrustTradeAccount>::const_iterator it;
	m_Mutex_AccountList.read_lock();
	it=m_AccountList.find(InvestorID);
	bRet=(it!=m_AccountList.end());
	m_Mutex_AccountList.read_unlock();
	return bRet;
}
//获取管理的全部账户列表
void CDataCenter::GetAccountList( std::vector<std::string>& accounts )
{
	std::map<std::string,TrustTradeAccount>::const_iterator it;
	m_Mutex_AccountList.read_lock();
	for (it=m_AccountList.begin();it!=m_AccountList.end();++it)
	{
		accounts.push_back(it->first);

	}
	m_Mutex_AccountList.read_unlock();
}
void CDataCenter::GetAccountList( std::map<std::string,TrustTradeAccount>& AccountList)
{
	m_Mutex_AccountList.read_lock();
	AccountList = m_AccountList;
	m_Mutex_AccountList.read_unlock();
}
//获取合约列表
void CDataCenter::GetInstrumentList(std::vector<std::string>& vec) const
{
	std::map<std::string,InstrumentField>::const_iterator it;
	for (it=m_AllInstrument.begin();it!=m_AllInstrument.end();++it)
	{
		vec.push_back(it->first);
	}
}
//获取品种列表
void CDataCenter::GetProductIDList(std::vector<std::string>& vec)
{
	std::map<std::string,ProductField>::const_iterator it;
	m_Mutex_AllProduct.read_lock();
	for (it=m_AllProduct.begin();it!=m_AllProduct.end();++it)
	{
		vec.push_back(it->first);
	}
	m_Mutex_AllProduct.read_unlock();
}
//从深度行情获取合约的市场总持仓
double  CDataCenter::GetTotalPostion(const std::string& Instrument)
{
	double dblValue=0.0;
	std::map<std::string,DepthMarketDataField>::const_iterator it;
	m_Mutex_DepthMarketData.read_lock();
	it=m_DepthMarketData.find(Instrument);
	if(it!=m_DepthMarketData.end())
	{
		dblValue=it->second.OpenInterest;
	}
	m_Mutex_DepthMarketData.read_unlock();
	return dblValue;
}
//获取一个账户特定的合约下总持仓
int CDataCenter::GetInstrumentPostion(const std::string& Investor,const std::string& Instrument)
{
	int pos=0;
	POSITIONSTRUCT::const_iterator it;
	m_Mutex_AllPosition.read_lock();
	it=m_AllPosition.find(Investor);
	if(it!=m_AllPosition.end())
	{
		std::map<TSequenceNoType,SequencialPositionField>::const_iterator nest_it;
		const std::map<TSequenceNoType,SequencialPositionField>& tmap=it->second;
		for (nest_it=tmap.begin();nest_it!=tmap.end();++nest_it)
		{
			if(Instrument==nest_it->second.InstrumentID)
			{
				pos+=nest_it->second.Position;
			}
		}
	}
	m_Mutex_AllPosition.read_unlock();
	return pos;
}
//获取某账户下所有品种的总持仓
void  CDataCenter::GetProductPostion(const std::string& Investor, std::map<std::string, int>& mapProductCount)
{	
	POSITIONSTRUCT::const_iterator it;
	m_Mutex_AllPosition.read_lock();
	it=m_AllPosition.find(Investor);
	if(it!=m_AllPosition.end())
	{
		std::map<TSequenceNoType,SequencialPositionField>::const_iterator nest_it;
		const std::map<TSequenceNoType,SequencialPositionField>& tmap=it->second;
		for (nest_it=tmap.begin();nest_it!=tmap.end();++nest_it)
		{
			std::string strInstrumentID = nest_it->second.InstrumentID;		
			InstrumentField field;
			if(!GetInstrumentInfo(strInstrumentID,field))
				continue;
			std::string strProduct = field.ProductID;

			std::map<std::string, int>::iterator it = mapProductCount.find(strProduct);
			if(it == mapProductCount.end())
				mapProductCount.insert(make_pair(strProduct, nest_it->second.Position));
			else
			{
				int nPos = it->second + nest_it->second.Position;
				mapProductCount[strProduct] = nPos;
			}
		}
	}
	m_Mutex_AllPosition.read_unlock();
	
}
std::string CDataCenter::GetSpecialIDFromProduct(const std::string& Investor, const std::string& ProductID) const
{
	std::string str=Investor+"+"+ProductID;
	return	str;
}
std::string CDataCenter::GetSpecialIDFromInstrument(const std::string& Investor, const std::string& InstrumentID) const
{
	InstrumentField field;
	if(GetInstrumentInfo(InstrumentID,field))
	{
		return GetSpecialIDFromProduct(Investor,field.ProductID);
	}
	return "";
}
bool CDataCenter::GetWarningMapLevel(double value,std::map<int,RiskResponse>& warnings,int& LevelID) const
{
	std::map<RiskLevelType,RiskResponse>::const_iterator coreit;
	for (coreit=warnings.begin();coreit!=warnings.end();++coreit)
	{
		const RiskResponse& risk=coreit->second;
	//	if(value-risk.fThresholdValue>-0.00001 && value-risk.fThresholdValue < 0.00001)
	//		continue;//相等不触发
		if(value > risk.fThresholdValue)
		{
			LevelID=coreit->first;
			return true;
		}
	}
	return false;
}
bool CDataCenter::GetWarningMapLevel_FundNetValue(double value,std::map<int,RiskResponse>& warnings,int& LevelID) const
{
	std::map<RiskLevelType,RiskResponse>::const_iterator coreit;
	for (coreit=warnings.begin();coreit!=warnings.end();++coreit)
	{
		const RiskResponse& risk=coreit->second;
	//	if(value-risk.fThresholdValue>-0.00001 && value-risk.fThresholdValue < 0.00001)
	//		continue;//相等不触发
		if(value < risk.fThresholdValue)
		{//小于设定的基金净值
			LevelID=coreit->first;
			return true;
		}
	}
	return false;
}

bool  CDataCenter::GetMaxPositionRiskLevel(const std::string& Investor, const std::string& InstrumentID,double value,
										   std::map<int,RiskResponse>& warnings,int& LevelID) const
{
	bool bRet=false;
	LevelID=0;
	std::map<std::string,std::map<RiskLevelType,RiskResponse>>::const_iterator sit;
	m_Mutex_RiskWarning.read_lock();
	sit=m_SpecialMaxPositionRiskWarning.find(GetSpecialIDFromInstrument(Investor,InstrumentID));
	if(sit!=m_SpecialMaxPositionRiskWarning.end())
	{
		warnings=sit->second;
		if(GetWarningMapLevel(value,warnings,LevelID))
			bRet=true;
	}
	else
	{
		RISKWARNINGSTRUCT::const_iterator it;
		int nIndModuleID=GetRiskIndModuleID(Investor);
		if(nIndModuleID>0)
		{
			it=m_MaxPositionRiskWarning.find(nIndModuleID);
			if(it!=m_MaxPositionRiskWarning.end())
			{
				warnings=it->second;
				if(GetWarningMapLevel(value,warnings,LevelID))
					bRet=true;
			}
		}
	}
	m_Mutex_RiskWarning.read_unlock();
	return bRet;
}
bool  CDataCenter::GetFundLossRiskLevel(const std::string& Investor, double value,
										std::map<RiskLevelType,RiskResponse>& warnings,int& LevelID) const
{
	bool bRet=false;
	LevelID=0;

	RISKWARNINGSTRUCT::const_iterator it;
	int nIndModuleID=GetRiskIndModuleID(Investor);
	if(nIndModuleID>0)
	{
		m_Mutex_RiskWarning.read_lock();
		it=m_FundLossRiskWarning.find(nIndModuleID);
		if(it!=m_FundLossRiskWarning.end())
		{
			warnings=it->second;
			if(GetWarningMapLevel(value,warnings,LevelID))
				bRet=true;	
		}
		m_Mutex_RiskWarning.read_unlock();
	}
	return bRet;
}
bool  CDataCenter::GetGappedMarketRiskLevel(const std::string& Investor,double value,
											std::map<RiskLevelType,RiskResponse>& warnings,int& LevelID) const
{
	bool bRet=false;
	LevelID=0;
	RISKWARNINGSTRUCT::const_iterator it;
	int nIndModuleID=GetRiskIndModuleID(Investor);
	if(nIndModuleID>0)
	{
		m_Mutex_RiskWarning.read_lock();
		it=m_GappedMarketRiskWarning.find(nIndModuleID);
		if(it!=m_GappedMarketRiskWarning.end())
		{
			warnings=it->second;
			if(GetWarningMapLevel(value,warnings,LevelID))
				bRet=true;
		}
		m_Mutex_RiskWarning.read_unlock();
	}

	return bRet;
}
bool  CDataCenter::GetMarginRatioRiskLevel(const std::string& Investor,double value,
							  std::map<int,RiskResponse>& warnings,int& LevelID) const
{
	bool bRet=false;
	LevelID=0;
	RISKWARNINGSTRUCT::const_iterator it;
	int nIndModuleID=GetRiskIndModuleID(Investor);
	if(nIndModuleID>0)
	{
		m_Mutex_RiskWarning.read_lock();
		it=m_MarginRatoRiskWarning.find(nIndModuleID);
		if(it!=m_MarginRatoRiskWarning.end())
		{
			warnings=it->second;
			if(GetWarningMapLevel(value,warnings,LevelID))
				bRet=true;
		}
		m_Mutex_RiskWarning.read_unlock();
	}

	return bRet;
}
bool  CDataCenter::GetMaxProductPositionRiskLevel(const std::string& Investor, const std::string& ProductID,double value,
									 std::map<int,RiskResponse>& warnings,int& LevelID) const
{
	bool bRet=false;
	LevelID=0;
	std::map<std::string,std::map<RiskLevelType,RiskResponse>>::const_iterator sit;
	m_Mutex_RiskWarning.read_lock();
	sit=m_SpecialProductMaxPositionRiskWarning.find(GetSpecialIDFromProduct(Investor,ProductID));
	if(sit!=m_SpecialProductMaxPositionRiskWarning.end())
	{
		warnings=sit->second;
		if(GetWarningMapLevel(value,warnings,LevelID))
			bRet=true;
	}
	else
	{
		RISKWARNINGSTRUCT::const_iterator it;
		int nIndModuleID=GetRiskIndModuleID(Investor);
		if(nIndModuleID>0)
		{
			it=m_ProductMaxPositionRiskWarning.find(nIndModuleID);
			if(it!=m_ProductMaxPositionRiskWarning.end())
			{
				warnings=it->second;
				if(GetWarningMapLevel(value,warnings,LevelID))
					bRet=true;
			}
		}
	}
	m_Mutex_RiskWarning.read_unlock();
	return bRet;
}
bool  CDataCenter::GetFundNetValueLevel(const std::string& Investor, double value,
						   std::map<int,RiskResponse>& warnings,int& LevelID)const
{
	bool bRet=false;
	LevelID=0;
	RISKWARNINGSTRUCT::const_iterator it;
	int nIndModuleID=GetRiskIndModuleID(Investor);
	if(nIndModuleID>0)
	{
		m_Mutex_RiskWarning.read_lock();
		it=m_FundNetRiskWarning.find(nIndModuleID);
		if(it!=m_FundNetRiskWarning.end())
		{
			warnings=it->second;
			if(GetWarningMapLevel_FundNetValue(value,warnings,LevelID))
				bRet=true;
		}
		m_Mutex_RiskWarning.read_unlock();
	}

	return bRet;
}
bool CDataCenter::GetWithDraw( const std::string& InvestorID, double& dbWithDraw)
{//得到当天出入金
	bool bFind = false;
	m_Mutex_RiskFundAccount.read_lock();
	std::map<std::string,RiskSyncAccountField>::const_iterator it=m_RiskFundAccount.find(InvestorID);
	if(it!=m_RiskFundAccount.end())
	{
		dbWithDraw=it->second.Withdraw;
		bFind = true;
	}
	m_Mutex_RiskFundAccount.read_unlock();
	return bFind;
}
double CDataCenter::GetCurrMargin( const std::string& InvestorID )
{
	double dblValue=0.0;
	m_Mutex_RiskFundAccount.read_lock();
	std::map<std::string,RiskSyncAccountField>::const_iterator it=m_RiskFundAccount.find(InvestorID);
	if(it!=m_RiskFundAccount.end())
	{
		dblValue=it->second.CurrMargin;
	}
	m_Mutex_RiskFundAccount.read_unlock();
	return dblValue;
}
bool CDataCenter::GetBalance( const std::string& InvestorID, double& dbValue)
{
	bool bGet = false;
	m_Mutex_RiskFundAccount.read_lock();
	std::map<std::string,RiskSyncAccountField>::const_iterator it=m_RiskFundAccount.find(InvestorID);
	if(it!=m_RiskFundAccount.end())
	{
		dbValue=it->second.Balance;
		bGet = true;
	}
	m_Mutex_RiskFundAccount.read_unlock();
	return bGet;
}
bool CDataCenter::GetPreBalance(const std::string& InvestorID, double& dbValue)
{
	bool bReturn = false;
	m_Mutex_PreRiskFundAccount.read_lock();
	std::map<std::string,SeqPreRiskAccountField>::const_iterator it=m_PreRiskFundAccount.find(InvestorID);
	if(it!=m_PreRiskFundAccount.end())
	{
		dbValue=it->second.PreBalance;
		bReturn = true;
	}
	m_Mutex_PreRiskFundAccount.read_unlock();
	return bReturn;
}
//获得账户的初始权益
double CDataCenter::GetInitFund( const std::string& InvestorID )
{
	double val=0.0;
	m_Mutex_AccountList.read_lock();
	std::map<std::string,TrustTradeAccount>::const_iterator it=m_AccountList.find(InvestorID);
	if(it!=m_AccountList.end())
		val=it->second.dProfit;
	m_Mutex_AccountList.read_unlock();
	return val;
}
double CDataCenter::GetPositionProfit( const std::string& InvestorID )
{
	double dblValue=0.0;
	m_Mutex_RiskFundAccount.read_lock();
	std::map<std::string,RiskSyncAccountField>::const_iterator it=m_RiskFundAccount.find(InvestorID);
	if(it!=m_RiskFundAccount.end())
	{
		dblValue=it->second.PositionProfit;
	}
	m_Mutex_RiskFundAccount.read_unlock();
	return dblValue;
}
double CDataCenter::GetSimulateGappedLoss( const std::string& InvestorID )
{
	double dblLoss=0.0;
	POSITIONSTRUCT::const_iterator it;
	m_Mutex_AllPosition.read_lock();
	it=m_AllPosition.find(InvestorID);
	if(it!=m_AllPosition.end())
	{
		const std::map<TSequenceNoType,SequencialPositionField>& nestmap=it->second;
		std::map<TSequenceNoType,SequencialPositionField>::const_iterator nestit;
		for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
		{
			const SequencialPositionField& PosField=nestit->second;
			DepthMarketDataField Quot={0};
			InstrumentField info={0};
			if(GetDepthMarketData(PosField.InstrumentID,Quot)&&GetInstrumentInfo(PosField.InstrumentID,info))
			{

				if(PosField.PosiDirection==T_PD_Long)
				{
					double NewLowerLimitPrice=Quot.LowerLimitPrice/Quot.PreSettlementPrice*Quot.LastPrice;
					dblLoss+=PosField.PositionCost-NewLowerLimitPrice*PosField.Position*info.VolumeMultiple;
				}
				else if(PosField.PosiDirection==T_PD_Short)
				{
					double NewUpperLimitPrice=Quot.UpperLimitPrice/Quot.PreSettlementPrice*Quot.LastPrice;
					dblLoss+=NewUpperLimitPrice*PosField.Position*info.VolumeMultiple-PosField.PositionCost;
				}
			}

		}
	}
	m_Mutex_AllPosition.read_unlock();
	return dblLoss;
}
bool CDataCenter::GetDepthMarketData(const std::string& Instrument, DepthMarketDataField& MarketDataField)
{
	bool bRet=false;
	std::map<std::string,DepthMarketDataField>::const_iterator it;
	m_Mutex_DepthMarketData.read_lock();
	it=m_DepthMarketData.find(Instrument);
	if (it!=m_DepthMarketData.end())
	{
		MarketDataField=it->second;
		bRet=true;
	}
	m_Mutex_DepthMarketData.read_unlock();
	return bRet;
}
bool CDataCenter::GetInstrumentInfo(const std::string& strInstrument,InstrumentField& InstrumentInfo) const
{
	m_Mutex_AllInstrument.read_lock();
	std::map<std::string,InstrumentField>::const_iterator it=m_AllInstrument.find(strInstrument);
	if(it!=m_AllInstrument.end())
	{
		InstrumentInfo=it->second;
		m_Mutex_AllInstrument.read_unlock();
		return true;
	}
	m_Mutex_AllInstrument.read_unlock();
	return false;
}

void CDataCenter::GetInstrumentInfoList( std::vector<InstrumentField>& vec )
{
	m_Mutex_AllInstrument.read_lock();
	std::map<std::string,InstrumentField>::const_iterator it;
	for (it=m_AllInstrument.begin();it!=m_AllInstrument.end();++it)
	{
		vec.push_back(it->second);
	}
	m_Mutex_AllInstrument.read_unlock();
}
TradeAccountIDType CDataCenter::GetTradeAccountID( const std::string& InvestorID )
{
	TradeAccountIDType id=0;
	m_Mutex_AccountList.read_lock();
	std::map<std::string,TrustTradeAccount>::const_iterator it=m_AccountList.find(InvestorID);	
	if(it!=m_AccountList.end())
	{
		id=it->second.nTradeAccountID;
	}
	else
		TRACE("GetTradeAccountID-%s\n",InvestorID.c_str());
	m_Mutex_AccountList.read_unlock();
	return id;
}
AssetMgmtOrgIDType CDataCenter::GetAssetMgmtOrgID( const std::string& InvestorID )
{
	AssetMgmtOrgIDType id=0;
	m_Mutex_AccountList.read_lock();
	std::map<std::string,TrustTradeAccount>::const_iterator it=m_AccountList.find(InvestorID);	
	if(it!=m_AccountList.end())
	{
		id=it->second.nAssetMgmtOrgID;
	}
	else
		TRACE("GetTradeAccountID-%s\n",InvestorID.c_str());
	m_Mutex_AccountList.read_unlock();
	return id;
}
FinancialProductIDType CDataCenter::GetFinancialProductID( const std::string& InvestorID )
{
	FinancialProductIDType id=0;
	m_Mutex_AccountList.read_lock();
	std::map<std::string,TrustTradeAccount>::const_iterator it=m_AccountList.find(InvestorID);	
	if(it!=m_AccountList.end())
	{
		id=it->second.nFinancialProductID;
	}
	else
		TRACE("GetFinancialProductID-%s\n",InvestorID.c_str());
	m_Mutex_AccountList.read_unlock();
	return id;
}
std::string CDataCenter::GetTradeAccountID( TradeAccountIDType  aID) const
{
	std::string str;
	m_Mutex_AccountList.read_lock();
	std::map<int,std::string>::const_iterator it=m_AccountID2Str.find(aID);
	if(it!=m_AccountID2Str.end())
	{
		str=it->second;
	}
	else
		TRACE("GetTradeAccountID-%d\n",aID);
	m_Mutex_AccountList.read_unlock();
	return str;
}

bool CDataCenter::GetTradeAccount(const std::string& InvestorID, TrustTradeAccount& account)
{
	bool bRet = false;
	m_Mutex_AccountList.read_lock();
	std::map<std::string,TrustTradeAccount>::const_iterator it=m_AccountList.find(InvestorID);	
	if(it!=m_AccountList.end())
	{
		account = it->second;
		bRet = true;
	}
	else
	{
		TRACE("GetTradeAccountID-%s\n",InvestorID.c_str());
		bRet = false;
	}
	m_Mutex_AccountList.read_unlock();

	return bRet;
}

RiskIndModuleType CDataCenter::GetRiskIndModuleID( const std::string& InvestorID ) const
{
	RiskIndModuleType id=0;
	m_Mutex_AccountList.read_lock();
	std::map<std::string,TrustTradeAccount>::const_iterator it= m_AccountList.find(InvestorID);
	if(it!=m_AccountList.end())
	{
		id=it->second.nRiskIndModuleID;
	}
	m_Mutex_AccountList.read_unlock();
	return id;
}
void CDataCenter::GetDepositList( int AccID, std::vector<SeqDepositField>& vec ) const
{
	const std::string strAccount=GetTradeAccountID(AccID);
	std::map<std::string,std::map<int,SeqDepositField>>::const_iterator it;
	m_Mutex_AllDeposit.read_lock();
	it=m_AllDeposit.find(strAccount);
	if(it!=m_AllDeposit.end())
	{
		const std::map<int,SeqDepositField>& nestmap=it->second;
		std::map<int,SeqDepositField>::const_iterator nestit;
		for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
		{
			vec.push_back(nestit->second);
		}
	}
	m_Mutex_AllDeposit.read_unlock();
}
void CDataCenter::GetTradeList( int AccID, std::vector<SequencialTradeField>& vec) const
{
	const std::string strAccount=GetTradeAccountID(AccID);
	m_Mutex_AllTrade.read_lock();
	std::map<std::string,std::map<int,SequencialTradeField>>::const_iterator it=m_AllTrade.find(strAccount);
	if(it!=m_AllTrade.end())
	{
		const std::map<int,SequencialTradeField>& nestmap=it->second;
		std::map<int,SequencialTradeField>::const_iterator nestit;
		for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
		{
			vec.push_back(nestit->second);
		}
	}
	m_Mutex_AllTrade.read_unlock();
}
//获取报单
void CDataCenter::GetOrderList( int AccID, std::vector<SequencialOrderField>& vec ) const
{
	const std::string strAccount=GetTradeAccountID(AccID);
	m_Mutex_AllOrder.read_lock();
	std::map<std::string,std::map<int,SequencialOrderField>>::const_iterator it=m_AllOrder.find(strAccount);
	if(it!=m_AllOrder.end())
	{
		const std::map<int,SequencialOrderField>& nestmap=it->second;
		std::map<int,SequencialOrderField>::const_iterator nestit;
		for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
		{
			vec.push_back(nestit->second);
		}
	}
	m_Mutex_AllOrder.read_unlock();
}
//获取持仓
void CDataCenter::GetPositionList( int AccID, std::vector<SequencialPositionField>& vec) const
{
	const std::string strAccount=GetTradeAccountID(AccID);
	m_Mutex_AllPosition.read_lock();
	std::map<std::string,std::map<int,SequencialPositionField>>::const_iterator it=m_AllPosition.find(strAccount);
	if(it!=m_AllPosition.end())
	{
		const std::map<int,SequencialPositionField>& nestmap=it->second;
		std::map<int,SequencialPositionField>::const_iterator nestit;
		for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
		{
			vec.push_back(nestit->second);
		}
	}
	m_Mutex_AllPosition.read_unlock();
}
void CDataCenter::GetAllNetValueInfo( int AccID, std::vector<NetValueInfo>& vec )const
{
	std::string strAccount=GetTradeAccountID(AccID);
	m_Mutex_Accout2NetValue.write_lock();
	
	std::map<std::string, NetValueInfo>::const_iterator it = m_Accout2NetValue.find(strAccount);
	if(it != m_Accout2NetValue.end())
	{
		NetValueInfo net;
		strcpy(net.szAccount, it->second.szAccount);
		net.dbValue = it->second.dbValue;
		vec.push_back(net);
	}	
	m_Mutex_Accout2NetValue.write_unlock();	

}
void CDataCenter::GetAllAccountFundList( int AccID, std::vector<RiskAllAccountField>& vec ) const
{
	const std::string strAccount=GetTradeAccountID(AccID);
	int flag=0;
	RiskAllAccountField all;	
	m_Mutex_PreRiskFundAccount.read_lock();
	std::map<std::string,SeqPreRiskAccountField>::const_iterator it1=m_PreRiskFundAccount.find(strAccount);
	if(it1!=m_PreRiskFundAccount.end())
	{
		flag|=1;
		all.pre=it1->second;
	}
	m_Mutex_PreRiskFundAccount.read_unlock();
	m_Mutex_RiskFundAccount.read_lock();
	std::map<std::string,RiskSyncAccountField>::const_iterator it2=m_RiskFundAccount.find(strAccount);
	if(it2!=m_RiskFundAccount.end())
	{
		flag|=2;
		all.cur=it2->second;
	}
	m_Mutex_RiskFundAccount.read_unlock();
	if(flag==3)
		vec.push_back(all);
}
void CDataCenter::GetQuotList( std::string InstrumentID, std::vector<DepthMarketDataField>& vec ) const
{
	std::map<std::string,DepthMarketDataField>::const_iterator it;
	m_Mutex_DepthMarketData.read_lock();
	it=m_DepthMarketData.find(InstrumentID);
	if(it!=m_DepthMarketData.end())
	{
		vec.push_back(it->second);
	}
	m_Mutex_DepthMarketData.read_unlock();
}
void CDataCenter::SubscribeFund(int AccID)
{
	SUBSCRIBE_INFO(AccID,bFund);	
}
void CDataCenter::SubscribeNetValue(int AccID)
{
	std::string strAcc=GetTradeAccountID(AccID);
	if(!strAcc.empty())
	{
		m_Mutex_SubscribeInfo.write_lock();
		std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bNetValue=true;	
		}
		else
		{
			SubscribeType st;
			ZeroMemory(&st,sizeof(st));
			st.AccID=AccID;
			st.bNetValue=true;
			m_SubscribeInfo[strAcc]=st;
		}
		m_Mutex_SubscribeInfo.write_unlock();
	}
}
void CDataCenter::SubscribePosition(int AccID)
{
	SUBSCRIBE_INFO(AccID,bPosition);
}

void CDataCenter::SubscribeTrade(int AccID)
{
	SUBSCRIBE_INFO(AccID,bTrade);
}

void CDataCenter::SubscribeOrder(int AccID)
{
	SUBSCRIBE_INFO(AccID,bOrder);
}

void CDataCenter::SubscribeDeposit(int AccID)
{
	SUBSCRIBE_INFO(AccID,bDeposit);
}
void CDataCenter::SubscribeRiskEvent(int AccID)
{
	SUBSCRIBE_INFO(AccID,bRiskEvent);
}
void CDataCenter::SubscribeRiskEvtHandling(int AccID)
{
	SUBSCRIBE_INFO(AccID,bRiskEvtHandling);
}
void CDataCenter::SubscribeQuot(const std::string& strInstrument)
{
	m_Mutex_SubscribeQuot.write_lock();
	m_SubscribeQuot.insert(strInstrument);
	m_Mutex_SubscribeQuot.write_unlock();
}
void CDataCenter::SubscribeStatistics(const AssetOrgProRelation& org)
{
	m_SubscribeStatistics.insert(org);
}
void CDataCenter::UnSubscribeFund(int AccID)
{
	UNSUBSCRIBE_INFO(AccID,bFund);
	UNSUBSCRIBE_INFO(AccID,bNetValue);
}

void CDataCenter::UnSubscribePosition(int AccID)
{
	UNSUBSCRIBE_INFO(AccID,bPosition);
}

void CDataCenter::UnSubscribeTrade(int AccID)
{
	UNSUBSCRIBE_INFO(AccID,bTrade);
}

void CDataCenter::UnSubscribeOrder(int AccID)
{
	UNSUBSCRIBE_INFO(AccID,bOrder);
}

void CDataCenter::UnSubscribeDeposit(int AccID)
{
	UNSUBSCRIBE_INFO(AccID,bDeposit);
}
void CDataCenter::UnSubscribeRiskEvent(int AccID)
{
	UNSUBSCRIBE_INFO(AccID,bRiskEvent);
}
void CDataCenter::UnSubscribeRiskEvtHandling(int AccID)
{
	UNSUBSCRIBE_INFO(AccID,bRiskEvtHandling);
}
void CDataCenter::UnSubscribeQuot(const std::string& strInstrument)
{
	m_Mutex_SubscribeQuot.write_lock();
	m_SubscribeQuot.erase(strInstrument);
	m_Mutex_SubscribeQuot.write_unlock();
}
void CDataCenter::UnSubscribeStatistics(const AssetOrgProRelation& org)
{
	m_SubscribeStatistics.erase(org);
}
void CDataCenter::UnSubscribeNetValue(int AccID)
{
	std::string strAcc=GetTradeAccountID(AccID);
	if(!strAcc.empty())
	{
		std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		m_Mutex_SubscribeInfo.write_lock();
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bNetValue=false;	
		}
		m_Mutex_SubscribeInfo.write_unlock();
	}
}
void CDataCenter::GetRiskEventList( int AccID, std::vector<RiskEvent>& vecEvent ) const
{
	std::map<RiskEventIDType,std::vector<RiskEvent>>::const_iterator it;
	m_Mutex_RiskEvent.read_lock();
	for (it=m_HistoryEvent.begin();it!=m_HistoryEvent.end();++it)
	{
		const std::vector<RiskEvent>& vec=it->second;
		for (size_t i=0;i<vec.size();++i)
		{
			if(vec[i].nTradeAccountID==AccID)
				vecEvent.push_back(vec[i]);
		}
	}
	for (it=m_ActiveEvent.begin();it!=m_ActiveEvent.end();++it)
	{
		const std::vector<RiskEvent>& vec=it->second;
		for (size_t i=0;i<vec.size();++i)
		{
			if(vec[i].nTradeAccountID==AccID)
				vecEvent.push_back(vec[i]);
		}
	}
	std::sort(vecEvent.begin(),vecEvent.end());
	m_Mutex_RiskEvent.read_unlock();

}

void CDataCenter::GetRiskEvtHandlingList( int AccID, std::vector<RiskEventHandling>& vec ) const
{
	std::map<RiskEventIDType,std::vector<RiskEventHandling>>::const_iterator it;
	m_Mutex_RiskEvent.read_lock();
	for (it=m_Handling.begin();it!=m_Handling.end();++it)
	{
		const std::vector<RiskEventHandling>& vecHandling=it->second;
		for(size_t i=0;i<vecHandling.size();++i)
		{
			if(vecHandling[i].nTradeAccountID==AccID)
			{
				vec.push_back(vecHandling[i]);
			}
		}
	}
	std::sort(vec.begin(),vec.end());
	m_Mutex_RiskEvent.read_unlock();
}
void CDataCenter::AddHandling(const RiskEventHandlingUserReq& r)
{
	RiskEventHandling handling={0};
	GetRiskEventHandlingID(handling.nRiskEventHandlingID);
	handling.lHandlingTime=time(NULL);
	handling.nTradeAccountID=r.nTradeAccountID;
	handling.nRiskEventID=r.nRiskEventID;
	handling.nRiskEventSubID=r.nRiskEventSubID;
	safestrcpy(handling.szHandler,sizeof(USERNAMESTRING),r.szHandler);
	safestrcpy(handling.szAccepter,sizeof(USERNAMESTRING),r.szAccepter);
	handling.nAction=Action_ClientRecord;
	handling.nProcessStatus=r.nProcessStatus;
	safestrcpy(handling.szContent,sizeof(CommentType),r.szContent);
	//推送风险事件处理
	CRisk2ClientMsgQueue* pMsgQueue=CRisk2ClientMsgQueue::Get();
	Risk2ClientData data={0};
	memset(&data,0,sizeof(data));
	data.CmdID=R2CMSGID_EVENTHANDLING;
	data.AccID=handling.nTradeAccountID;
	data.Main.Handling=handling;
	pMsgQueue->AddMsg(data);
	//添加到内存
	AddHandling(handling);
}
void CDataCenter::AddHandling(const RiskEventHandling& r)
{
	std::map<RiskEventIDType,std::vector<RiskEventHandling>>::iterator it;
	m_Mutex_RiskEvent.write_lock();
	it=m_Handling.find(r.nRiskEventID);
	if(it!=m_Handling.end())
	{
		std::vector<RiskEventHandling>& vec=it->second;
		vec.push_back(r);
	}
	else
	{
		std::vector<RiskEventHandling> vec;
		vec.push_back(r);
		m_Handling[r.nRiskEventID]=vec;
	}
	m_Mutex_RiskEvent.write_unlock();
}

void CDataCenter::ReadForceClosePermission()
{
	//DATA_CLEAR(ForceClosePermission);
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf_s(szBuffer, "select t1.riskmgmtuserid  from RISKMGMTUSERINFO t1 \
					  left join permissionvalue t2 on t1.permissionid = t2.permissionid \
					  where t2.actionid = 21 and t2.value = 1");
	std::vector<int> vec;
	if (!m_dbAccess.QueryData(szBuffer, vec))
		return;
	
	m_Mutex_ForceClosePermission.write_lock();
	m_ForceClosePermission.clear();
	for ( UINT i = 0; i < vec.size(); i++ )
	{
		m_ForceClosePermission.insert(vec[i]);
	}
	m_Mutex_ForceClosePermission.write_unlock();
	
}
void CDataCenter::ReadEventMessageTemplate()
{
	//DATA_CLEAR(EventMessageTemplate);
	std::vector<EventMessageTemplate> vec;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from EVENTMESSAGETEMPLATE t");

	if (!m_dbAccess.QueryData(szBuffer, vec))
		return;
	
	m_Mutex_EventMessageTemplate.write_lock();
	m_EventMessageTemplate.clear();
	for ( UINT i = 0; i < vec.size(); i++ )
	{
		int key=vec[i].nRiskLevelID*10+vec[i].nRiskIndicatorID;
		m_EventMessageTemplate[key]=vec[i];
	}
	m_Mutex_EventMessageTemplate.write_unlock();
	

}
void CDataCenter::ReadRiskMgmtUserInfo()
{
//	DATA_CLEAR(RiskMgmtUserInfo);
	std::vector<RiskMgmtUserInfo> vec;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from RISKMGMTUSERINFO t where t.roletype = 1");

	if (!m_dbAccess.QueryData(szBuffer, vec))
		return;

	m_Mutex_RiskMgmtUserInfo.write_lock();
	m_RiskMgmtUserInfo.clear();
	for ( UINT i = 0; i < vec.size(); i++ )
	{
		TargetAccount target;
		target.nRiskMgmtUserID=vec[i].nRiskMgmtUserID;
		safestrcpy(target.szAccount,sizeof(ACCOUNTSTRING),vec[i].szAccount);
		AssetOrgProRelation org;
		org.nAssetMgmtOrgID=vec[i].nAssetMgmtOrgID;
		org.nFinancialProductID=vec[i].nFinancialProductID;
		std::vector<TargetAccount>& acc=m_RiskMgmtUserInfo[org];
		acc.push_back(target);
	}
	m_Mutex_RiskMgmtUserInfo.write_unlock();
	

}
bool CDataCenter::GetEventMessageTemplate(RiskIndicatorType nRiskIndicatorID,
										  RiskLevelType nRiskLevelID,EventMessageTemplate& msgtemplate) const
{
	bool bRet=false;
	m_Mutex_EventMessageTemplate.read_lock();
	int key=nRiskLevelID*10+nRiskIndicatorID;
	std::map<int,EventMessageTemplate>::const_iterator it;
	it=m_EventMessageTemplate.find(key);
	if(it!=m_EventMessageTemplate.end())
	{
		msgtemplate=it->second;
		bRet=true;
	}
	m_Mutex_EventMessageTemplate.read_unlock();
	return bRet;
}
bool CDataCenter::GetRiskMgmtUserInfo(AssetOrgProRelation org,std::vector<TargetAccount>& Accounts)
{
	bool bRet=false;
	m_Mutex_RiskMgmtUserInfo.read_lock();
	std::map<AssetOrgProRelation,std::vector<TargetAccount>>::const_iterator it;
	it=m_RiskMgmtUserInfo.find(org);
	if(it!=m_RiskMgmtUserInfo.end())
	{
		Accounts=it->second;
		bRet=true;
	}
	m_Mutex_RiskMgmtUserInfo.read_unlock();
	return bRet;
}
bool CDataCenter::GetForceClosePermission(int AccID) const
{
	m_Mutex_ForceClosePermission.read_lock();
	std::set<int>::const_iterator it=m_ForceClosePermission.find(AccID);
	bool bret=(it!=m_ForceClosePermission.end());
	m_Mutex_ForceClosePermission.read_unlock();
	return bret;
}
int CDataCenter::getExpoBase2(const double& d)
{
	int i = 0;
	((short *)(&i))[0] = (((short *)(&d))[3] & (short)0x7ff0);
	return (i >> 4) - 1023;
}
bool CDataCenter::equals(const double& d1, const double& d2)
{
	if (d1 == d2)
		return true;
	int e1 = getExpoBase2(d1);
	int e2 = getExpoBase2(d2);
	int e3 = getExpoBase2(d1 - d2);
	if ((e3 - e2 < -48) && (e3 - e1 < -48))
		return true;
	return false;
}
void CDataCenter::SaveAllData()
{	
	int step=0;
	char strTime[64];
	GetTodayString(strTime);
	//昨日资金
	{
		std::vector<SeqPreRiskAccountField> vec;
		std::map<std::string,SeqPreRiskAccountField>::const_iterator it;
		m_Mutex_PreRiskFundAccount.read_lock();;
		for (it=m_PreRiskFundAccount.begin();it!=m_PreRiskFundAccount.end();++it)
		{
			vec.push_back(it->second);
		}
		m_Mutex_PreRiskFundAccount.read_unlock();
		LOG_INFO("SaveAllData(第%d步):昨日资金(共%d条记录)",++step,vec.size());
		m_dbAccess.SavePreRiskAccount(strTime,vec);	
	}
	//当日资金
	{
		std::vector<RiskSyncAccountField> vec;
		std::map<std::string,RiskSyncAccountField>::const_iterator it;
		m_Mutex_RiskFundAccount.read_lock();
		for (it=m_RiskFundAccount.begin();it!=m_RiskFundAccount.end();++it)
		{
			vec.push_back(it->second);
		}
		m_Mutex_RiskFundAccount.read_unlock();
		LOG_INFO("SaveAllData(第%d步):当日资金(共%d条记录)",++step,vec.size());
		m_dbAccess.SaveRiskSyncAccount(strTime,vec);	
	}
	//保存成交
	{
		std::vector<SequencialTradeField> vec;
		TRADESTRUCT::const_iterator it;
		std::map<TSequenceNoType,SequencialTradeField>::const_iterator nestit;
		m_Mutex_AllTrade.read_lock();
		for (it=m_AllTrade.begin();it!=m_AllTrade.end();++it)
		{	
			const std::map<TSequenceNoType,SequencialTradeField>& nestmap=it->second;
			for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
			{
				vec.push_back(nestit->second);
			}
		}
		m_Mutex_AllTrade.read_unlock();
		LOG_INFO("SaveAllData(第%d步):保存成交(共%d条记录)",++step,vec.size());
		m_dbAccess.SaveTradeRecord(strTime,vec);
	}
	//报单
	{
		std::vector<SequencialOrderField> vec;
		ORDERSTRUCT::const_iterator it;
		std::map<TSequenceNoType,SequencialOrderField>::const_iterator nestit;
		m_Mutex_AllOrder.read_lock();
		for (it=m_AllOrder.begin();it!=m_AllOrder.end();++it)
		{	
			const std::map<TSequenceNoType,SequencialOrderField>& nestmap=it->second;
			for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
			{
				vec.push_back(nestit->second);
			}
		}
		m_Mutex_AllOrder.read_unlock();
		LOG_INFO("SaveAllData(第%d步):报单(共%d条记录)",++step,vec.size());	
		m_dbAccess.SaveOrderRecord(strTime,vec);
	}

	//出入金
	{
		std::vector<SeqDepositField> vec;
		m_Mutex_AllDeposit.read_lock();
		std::map<std::string,std::map<int,SeqDepositField>>::iterator it;
		for (it=m_AllDeposit.begin();it!=m_AllDeposit.end();++it)
		{	
			std::map<int,SeqDepositField>& itSeqDepositField = it->second;
			
			std::map<int,SeqDepositField>::iterator itSecond = itSeqDepositField.begin();
			for(; itSecond != itSeqDepositField.end(); itSecond++)
			{
				SeqDepositField& seq = itSecond->second;
				vec.push_back(seq);
			}
		}
		m_Mutex_AllDeposit.read_unlock();
		LOG_INFO("SaveAllData(第%d步):出入金(共%d条记录)",++step,vec.size());	
		m_dbAccess.SaveSeqDepositField(strTime,vec);
	}
//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//持仓
	{
		std::vector<SequencialPositionField> vec;
		POSITIONSTRUCT::iterator it;
		std::map<TSequenceNoType,SequencialPositionField>::iterator nestit;
		m_Mutex_AllPosition.read_lock();
		for (it=m_AllPosition.begin();it!=m_AllPosition.end();++it)
		{	
			std::map<TSequenceNoType,SequencialPositionField>& nestmap=it->second;
			for (nestit=nestmap.begin();nestit!=nestmap.end();++nestit)
			{
				 SequencialPositionField& pf = nestit->second;
				if(equals(fabs(pf.PreSettlementPrice), 1.7976931348623158e+308))
					nestit->second.PreSettlementPrice = 0;
				vec.push_back(nestit->second);

				LOG_INFO("UniqSequenceNo:%d,\
						    InstrumentID:%s,\
							BrokerID:%s,\
							InvestorID:%s,\
							PosiDirection:%c,\
							HedgeFlag:%c,\
							PositionDate:%c,\
							YdPosition:%d,\
							Position:%d,\
							LongFrozen:%d,\
							ShortFrozen:%d,\
							LongFrozenAmount:%.2f,\
							ShortFrozenAmount:%.2f,\
							OpenVolume:%d,\
							CloseVolume:%d,\
							PositionCost:%.2f,\
							PreMargin:%.2f,\
						    UseMargin:%.2f,\
						    FrozenMargin:%.2f,\
						    FrozenCash:%.2f,\
							FrozenCommission:%.2f,\
							CashIn:%.2f,\
							Commission:%.2f,\
							CloseProfit:%.2f,\
							PositionProfit:%.2f,\
							PreSettlementPrice:%.2f,\
							SettlementPrice:%.2f,\
							TradingDay:%s,\
							SettlementID:%d,\
							OpenCost:%.2f,\
							ExchangeMargin:%.2f,\
						    CombPosition:%d,\
						    CombLongFrozen:%d,\
						    CombShortFrozen:%d,\
							CloseProfitByDate:%.2f,\
						    CloseProfitByTrade:%.2f,\
						    TodayPosition:%d,\
						    MarginRateByMoney:%.2f,\
							MarginRateByVolume:%.2f\n",	

							pf.UniqSequenceNo,
							pf.InstrumentID,
							pf.BrokerID,
							pf.InvestorID,
							pf.PosiDirection,
							pf.HedgeFlag,
							pf.PositionDate,
							pf.YdPosition,
							pf.Position,
							pf.LongFrozen,
							pf.ShortFrozen,
							pf.LongFrozenAmount,
							pf.ShortFrozenAmount,
							pf.OpenVolume,
							pf.CloseVolume,
							pf.PositionCost,
							pf.PreMargin,
							pf.UseMargin,
							pf.FrozenMargin,
							pf.FrozenCash,
							pf.FrozenCommission,
							pf.CashIn,
							pf.Commission,
							pf.CloseProfit,
							pf.PositionProfit,
							pf.PreSettlementPrice,
							pf.SettlementPrice,
							pf.TradingDay,
							pf.SettlementID,
							pf.OpenCost,
							pf.ExchangeMargin,
							pf.CombPosition,
							pf.CombLongFrozen,
							pf.CombShortFrozen,
							pf.CloseProfitByDate,
							pf.CloseProfitByTrade,
							pf.TodayPosition,
							pf.MarginRateByMoney,
							pf.MarginRateByVolume);
			}
		}
		m_Mutex_AllPosition.read_unlock();		
		if(m_dbAccess.SavePositionRecord(strTime,vec))
		{
			LOG_INFO("SaveAllData:持仓存入成功");
			LOG_INFO("SaveAllData(第%d步):持仓(共%d条记录)",++step,vec.size());
		}
		else
		{
			LOG_INFO("SaveAllData:持仓存入失败");
		}
	}

	//风控事件
	{
		std::vector<RiskEvent> vecEvent;
		std::map<RiskEventIDType,std::vector<RiskEvent>>::const_iterator it;
		m_Mutex_RiskEvent.read_lock();
		for (it=m_HistoryEvent.begin();it!=m_HistoryEvent.end();++it)
		{
			const std::vector<RiskEvent>& vec=it->second;
			vecEvent.insert(vecEvent.end(),vec.begin(),vec.end());
		}
		for (it=m_ActiveEvent.begin();it!=m_ActiveEvent.end();++it)
		{
			const std::vector<RiskEvent>& vec=it->second;
			vecEvent.insert(vecEvent.end(),vec.begin(),vec.end());
		}
		m_Mutex_RiskEvent.read_unlock();	
		LOG_INFO("SaveAllData(第%d步):风控事件(共%d条记录)",++step,vecEvent.size());
		m_dbAccess.SaveRiskEvent(strTime,vecEvent);
	}
	//风控事件处理
	{
		std::vector<RiskEventHandling> vecEvent;
		std::map<RiskEventIDType,std::vector<RiskEventHandling>>::const_iterator it;
		m_Mutex_RiskEvent.read_lock();
		for (it=m_Handling.begin();it!=m_Handling.end();++it)
		{
			const std::vector<RiskEventHandling>& vec=it->second;
			vecEvent.insert(vecEvent.end(),vec.begin(),vec.end());
		}
		m_Mutex_RiskEvent.read_unlock();
		LOG_INFO("SaveAllData(第%d步):风控事件处理(共%d条记录)",++step,vecEvent.size());
		m_dbAccess.SaveRiskEventHandling(strTime,vecEvent);
	}

	LOG_INFO("基金净值数据保存日期: %s", strTime);
	char strTime2[64];
	memset(&strTime2, 0, sizeof(strTime2));
	{
		std::vector<NetFundCalcResult> vResult;
		m_Mutex_AccountList.write_lock();
		std::map<std::string,TrustTradeAccount>::iterator it = m_AccountList.begin();
		for(; it != m_AccountList.end(); it++)
		{
			std::string InvestorID = it->first;
			double dblBalance= 0.0;
			if(!GetBalance(InvestorID, dblBalance))
				continue;
			LOG_INFO("基金净值:账号：%s,  总权益：%.8f", InvestorID.c_str() ,dblBalance);
			NetFundParam netFundParam;
			if(!GetNetFundParam(InvestorID, netFundParam))
			{
				LOG_INFO("基金净值配置表FUNDNETPARAM里没有该账户:%s",InvestorID.c_str());
				continue;
			}

			NetFundCalcResult sResult;
			sResult.nTradeAccountID = netFundParam.nTradeAccountID;
			sResult.dInnerVolumn	= netFundParam.dInnerVolumn;
			sResult.dOuterVolumn	= netFundParam.dOuterVolumn;
			sResult.dOuterNetAsset  = netFundParam.dOuterNetAsset;
			sResult.dInnerNetAsset	= dblBalance;
			if(sResult.dInnerVolumn <0.00001)
				sResult.dInnerPerNet  = 0.0;
			else
				sResult.dInnerPerNet    = sResult.dInnerNetAsset/sResult.dInnerVolumn;
			if(sResult.dOuterVolumn <0.00001)
				sResult.dOuterPerNet  = 0.0;
			else
				sResult.dOuterPerNet    = sResult.dOuterNetAsset/sResult.dOuterVolumn;

			if(sResult.dOuterVolumn + sResult.dInnerVolumn < 0.00001)
				sResult.dTotalNetAsset  =  0.0;
			else
				sResult.dTotalNetAsset  = (sResult.dOuterNetAsset + dblBalance)/(sResult.dOuterVolumn + sResult.dInnerVolumn);		
		
			
			SYSTEMTIME st;
			GetLocalTime(&st);	
			sprintf(strTime2,"%4d-%02d-%02d",st.wYear, st.wMonth, st.wDay);
			strcpy(sResult.szUpdateDate, strTime2);
			vResult.push_back(sResult);			
			LOG_INFO("委托交易账号ID:%d; 场内份额：%.2f  场外份额：%.6f 场内净值：%.6f 场外净值：%.6f 场内单位净值：%.6f 场外单位净值：%.6f 总资产净值：%.6f, 时间：%s",
				InvestorID.c_str(), sResult.dInnerVolumn, sResult.dOuterVolumn, sResult.dInnerNetAsset, sResult.dOuterNetAsset,
				sResult.dInnerPerNet, sResult.dOuterPerNet, sResult.dTotalNetAsset, strTime2);
		}
		m_Mutex_AccountList.write_unlock();
		m_dbAccess.SaveNetFundCalcResult(strTime2, vResult);
		LOG_INFO("SaveAllData(第%d步):基金净值(共%d条记录)",++step,vResult.size());

	}
	{
		LOG_INFO("绩效统计数据保存日期: %s", strTime2);
		SaveGatherData();		
	}
	LOG_INFO("SaveAllData(完成)");
}
void CDataCenter::SaveGatherData()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf_s(szBuffer,MAX_SQL_LENGTH, "select * from ASSETMGMTORGANIZATION t");
	std::vector<AssetMgmtOrganization> vec2;
	if (!m_dbAccess.QueryData(szBuffer, vec2))
		return;
	std::vector<SGatherData> vGatherData;
	for(int i =0; i<(int)vec2.size(); i++)
	{
		AssetMgmtOrganization assetOrg = vec2[i];
		int nID = assetOrg.nAssetMgmtOrgID;

		std::vector<AssetMgmtOrganization> vOrg;
		if ( !GetLowerAssetOrg(nID, vOrg))
		{		
			continue;
		}

		std::vector<TrustTradeAccount> vec;
		char szBuffer[MAX_SQL_LENGTH];
		for( UINT i = 0; i < vOrg.size(); i++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from TRUSTTRADEACCOUNT t where t.assetmgmtorgid = %d", vOrg[i]);

			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{				
				return;
			}
		}		
		ProcessGatherAccountVec(0, nID, vec, vGatherData);		
	}



	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf_s(szBuffer, MAX_SQL_LENGTH,"select * from FINANCIALPRODUCT t");
	std::vector<FinancialProduct> vec1;
	if (!m_dbAccess.QueryData(szBuffer, vec1))
		return;	
	for(int i=0; i<(int)vec1.size(); i++)
	{
		FinancialProduct fp = vec1[i];
		
		std::map<std::string,TrustTradeAccount> AccountList;
		CDataCenter::Get()->GetAccountList(AccountList);
		std::vector<TrustTradeAccount> vec;
		std::map<std::string,TrustTradeAccount>::iterator it = AccountList.begin();
		for(; it != AccountList.end(); it++)
		{
			TrustTradeAccount account = it->second;
			if(account.nFinancialProductID == fp.nFinancialProductID)
				vec.push_back(account);
		}
		ProcessGatherAccountVec(1, fp.nFinancialProductID, vec, vGatherData);	
	}

	std::map<std::string,TrustTradeAccount> AccountList;
	CDataCenter::Get()->GetAccountList(AccountList);
	
	std::map<std::string,TrustTradeAccount>::iterator it = AccountList.begin();
	for(; it != AccountList.end(); it++)
	{
		TrustTradeAccount account = it->second;
		std::vector<TrustTradeAccount> vec;		
		vec.push_back(account);
		ProcessGatherAccountVec(2, account.nTradeAccountID, vec, vGatherData);	
	}
	char strTime2[64];
	memset(&strTime2, 0, sizeof(strTime2));
	SYSTEMTIME st;
	GetLocalTime(&st);	
	sprintf(strTime2,"%4d-%02d-%02d",st.wYear, st.wMonth, st.wDay);
	m_dbAccess.SaveGatherData(strTime2, vGatherData);
	LOG_INFO("SaveAllData(第%d步):基金净值(共%d条记录)",9,vGatherData.size());

}
bool CDataCenter::ProcessGatherAccountVec(int nTypeID, int nID, std::vector<TrustTradeAccount>& vec, std::vector<SGatherData>& vGatherData)
{
	if(vec.size() == 0)
		return false;

	SGatherData sGather;
	memset(&sGather, 0, sizeof(sGather));
	sGather.nIDType = nTypeID;
	sGather.nID     = nID;

	int nUnGetInfoCount =0;//如果没有当日资金回报，则不存历史数据
	double dbSumCurrMargin = 0.0;
	double dbSumWithDraw = 0.0;
	double dbSumPreBalance = 0.0;
	for(int k =0; k < (int)vec.size(); k++)
	{
		double dbWithDraw = 0.0;
		TrustTradeAccount tAccount = vec[k];
		if(!CDataCenter::Get()->GetWithDraw(tAccount.szAccount, dbWithDraw))
		{
			nUnGetInfoCount++;
			LOG_INFO("没有该账户出入金信息:%s",tAccount.szAccount);
			continue;
		}
		dbSumWithDraw += dbWithDraw;//总出入金相加
		sGather.SumPreBalance += tAccount.dProfit + dbWithDraw;			//初始权益和				
		
		double dbBalance =  0.0;
		CDataCenter::Get()->GetBalance(tAccount.szAccount, dbBalance);
		sGather.SumBalance += dbBalance;					//当前权益和		

		double dbPreBalance = 0.0;
		CDataCenter::Get()->GetPreBalance(tAccount.szAccount, dbPreBalance);	
		dbSumPreBalance += dbPreBalance;

		sGather.OffBalance += dbBalance - dbPreBalance; //今日权益变化

		dbSumCurrMargin += CDataCenter::Get()->GetCurrMargin(tAccount.szAccount);//保证金比率

		
	}
	if(nUnGetInfoCount == (int)vec.size())
		return false;//没有任何账户的当日资金信息

	sGather.MarginRate = dbSumCurrMargin/sGather.SumBalance;

	double dbrj = 0;
	if(dbSumWithDraw > 0)
		dbrj = sGather.SumBalance/(dbSumPreBalance + dbSumWithDraw)-1;
	else
		dbrj = (sGather.SumBalance + dbSumWithDraw)/dbSumPreBalance-1;

	SGatherData sGathDataTemp;
	if(CDataCenter::Get()->GetGatherData(nTypeID, nID, sGathDataTemp ))
	{//得到昨日保存的“年华收益率”值			
		sGather.YearEarnRate	=  ((1 + sGathDataTemp.YearEarnRate* sGathDataTemp.nDayCount/220)*(1+dbrj) -1)/(sGathDataTemp.nDayCount +1)*220;	
	//	sGather.YearEarnRate	= ((1 + sGathDataTemp.dbValue)*(1+dbrj) -1)/(sGathDataTemp.nDayCount +1)*220;	
		sGather.dbValue			= (1 + sGathDataTemp.dbValue)*(1+dbrj) -1;
		sGather.nDayCount		= sGathDataTemp.nDayCount + 1;
	}
	else
	{			
		sGather.YearEarnRate = ((1+dbrj) -1)*220;
		sGather.dbValue			= ((1+dbrj) -1);
		sGather.nDayCount		= 1;
	}
	vGatherData.push_back(sGather);

	if(sGather.nIDType == 0)
		LOG_INFO("ID类型：%d; 组织机构ID:%d; 初始权益和:%.2f; 当前权益和:%.2f; 今日权益变化:%.2f; 年化收益率:%.2f; 保证金比率:%.2f; 指标值:%.2f; 总共多少天:%d",
		sGather.nIDType, sGather.nID, sGather.SumPreBalance, sGather.SumBalance, sGather.OffBalance, sGather.YearEarnRate, sGather.MarginRate, sGather.dbValue, sGather.nDayCount);
	else if(sGather.nIDType == 1)
			LOG_INFO("ID类型：%d; 理财产品ID:%d; 初始权益和:%.2f; 当前权益和:%.2f; 今日权益变化:%.2f; 年化收益率:%.2f; 保证金比率:%.2f; 指标值:%.2f; 总共多少天:%d",
			sGather.nIDType, sGather.nID, sGather.SumPreBalance, sGather.SumBalance, sGather.OffBalance, sGather.YearEarnRate, sGather.MarginRate, sGather.dbValue, sGather.nDayCount);
	else if(sGather.nIDType == 2)
		LOG_INFO("ID类型：%d; 账户ID:%d; 初始权益和:%.2f; 当前权益和:%.2f; 今日权益变化:%.2f; 年化收益率:%.2f; 保证金比率:%.2f; 指标值:%.2f; 总共多少天:%d",
		sGather.nIDType, sGather.nID, sGather.SumPreBalance, sGather.SumBalance, sGather.OffBalance, sGather.YearEarnRate, sGather.MarginRate, sGather.dbValue, sGather.nDayCount);

	return true;

}
void CDataCenter::FilterAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vAssetOrg, 
								 std::vector<AssetMgmtOrganization>& vOut )
{
	vOut.clear();
	bool bFind = false;
	std::map<int, std::map<int, AssetMgmtOrganization>> mapAssetOrgTree;
	for ( UINT i = 0; i < vAssetOrg.size(); i++ )
	{
		AssetMgmtOrganization assetOrg = vAssetOrg[i];
		if ( assetOrg.nAssetMgmtOrgID == nCurOrgID )
		{
			vOut.push_back(assetOrg);
			bFind = true;
		}

		std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it = 
			mapAssetOrgTree.find(assetOrg.nUpperLevelOrgID);
		if ( it != mapAssetOrgTree.end() )
		{
			it->second.insert(std::make_pair(assetOrg.nAssetMgmtOrgID, assetOrg));
		}
		else
		{
			std::map<int, AssetMgmtOrganization> tempOrgMap;
			tempOrgMap.clear();
			tempOrgMap[assetOrg.nAssetMgmtOrgID] = assetOrg;
			mapAssetOrgTree[assetOrg.nUpperLevelOrgID] = tempOrgMap;
		}	
	}

	if ( bFind )
	{
		CalcLowerData(mapAssetOrgTree, nCurOrgID, vOut);
	}
}

void CDataCenter::CalcLowerData( std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetOrgTree, 
								int nOwnerID, std::vector<AssetMgmtOrganization>& vOutData )
{
	std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it_owner = 
		mapAssetOrgTree.find(nOwnerID);
	if ( it_owner == mapAssetOrgTree.end() )
	{
		return;
	}
	else
	{
		std::map<int, AssetMgmtOrganization> tempAssetOrgMap = it_owner->second;
		if ( tempAssetOrgMap.empty())
		{
			return;
		}

		std::map<int, AssetMgmtOrganization>::iterator it_asset = tempAssetOrgMap.begin();
		for ( ; it_asset != tempAssetOrgMap.end(); it_asset++ )
		{
			AssetMgmtOrganization info = it_asset->second;
			vOutData.push_back(info);

			CalcLowerData(mapAssetOrgTree, info.nAssetMgmtOrgID, vOutData);
		}
	}
}

bool CDataCenter::GetLowerAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vOut )
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from ASSETMGMTORGANIZATION t where t.assetmgmtorgid >= %d", nCurOrgID);
	std::vector<AssetMgmtOrganization> vec;
	if ( !GET_DBACCESS().QueryData(szBuffer, vec))
	{
		return false;
	}
	else
	{
		FilterAssetOrg(nCurOrgID, vec, vOut);
		return true;
	}
}
RiskEventSubIDType CDataCenter::GetRiskEventSubID(RiskEventIDType EventID)
{
	RiskEventSubIDType subID=0;
	std::map<RiskEventIDType,std::vector<RiskEvent>>::iterator it;
	m_Mutex_RiskEvent.write_lock();
	it=m_ActiveEvent.find(EventID);
	if(it!=m_ActiveEvent.end())
	{
		subID=it->second.size();
	}
	m_Mutex_RiskEvent.write_unlock();
	return subID;
}
void CDataCenter::NewActiveEvent( const RiskEvent& evt )
{
	std::map<RiskEventIDType,std::vector<RiskEvent>>::iterator it;
	m_Mutex_RiskEvent.write_lock();
	it=m_ActiveEvent.find(evt.nRiskEventID);
	if(it!=m_ActiveEvent.end())
	{
		std::vector<RiskEvent>& ss=it->second;
		ss.push_back(evt);
	}
	else
	{
		std::vector<RiskEvent> ss;
		ss.push_back(evt);
		m_ActiveEvent[evt.nRiskEventID]=ss;
	}
	m_Mutex_RiskEvent.write_unlock();
}
void CDataCenter::DeleteActiveEvent( const RiskEvent& evt )
{
	std::map<RiskEventIDType,std::vector<RiskEvent>>::iterator it;
	m_Mutex_RiskEvent.write_lock();
	it=m_ActiveEvent.find(evt.nRiskEventID);
	if(it!=m_ActiveEvent.end())
	{
		//加入历史
		std::vector<RiskEvent>& ss=it->second;
		ss.push_back(evt);
		for (size_t i=0;i<ss.size();++i)
		{
			ss[i].nIsValid=RiskEvent_History;
		}
		m_HistoryEvent[evt.nRiskEventID]=ss;
		//删除激活
		m_ActiveEvent.erase(evt.nRiskEventID);
	}
	m_Mutex_RiskEvent.write_unlock();
}

void CDataCenter::FrontDisconnected( int nReason )
{

}

void CDataCenter::SuccessLogin()
{
	m_LoginTime=time(NULL);
}
bool CDataCenter::IsOK()
{
	//最大间隔启动时间设置为10S
	bool curstatus=((time(NULL)-m_LoginTime)>30);
	if(!m_lastStatus&&curstatus)
		m_bRiskWarningChanged=true;
	m_lastStatus=curstatus;
	return curstatus;
}
void CDataCenter::Reset()
{
	DATA_CLEAR(SubscribeInfo);
	DATA_CLEAR(SubscribeQuot);
	DATA_CLEAR(SubscribeStatistics);
	//读取config.xml
	std::string str="config.xml";
	GetFolderFileName(str);
	ReadConfigXml(str);
	//获取账户列表
	ReadTrustTradeAccount();
	//读取RiskWarning
	ReadRiskWarning();
	//理财产品与资管组织关系
	ReadAssetOrgProRelation();
	//获取强平权限
	ReadForceClosePermission();
	//
	ReadEventMessageTemplate();
	//
	ReadRiskMgmtUserInfo();

	ReadNetFundParam();
	ReadForceCloseParam();
	ReadGatherData();
	//读取今日风险事件和处理
	SAFE_CLEAR2(m_HistoryEvent,m_Mutex_RiskEvent);
	SAFE_CLEAR2(m_ActiveEvent,m_Mutex_RiskEvent);
	SAFE_CLEAR2(m_Handling,m_Mutex_RiskEvent);
	SAFE_CLEAR(PreRiskFundAccount);
	SAFE_CLEAR(RiskFundAccount);
	SAFE_CLEAR(AllPosition);
	SAFE_CLEAR(AllTrade);
	SAFE_CLEAR(AllOrder);
	SAFE_CLEAR(AllDeposit);
	SAFE_CLEAR(AllProduct);
	SAFE_CLEAR(AllInstrument);
	SAFE_CLEAR(DepthMarketData);
	InterlockedExchange(&m_StatisticsUpdate,STAT_DELAY_TIME);
}
void CDataCenter::IdleBusinessDataMain()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	int idays=GetTodayString();
	if(m_DayLoadSavedSet[0].count(idays)==0)
	{

		if(st.wHour==0&&st.wMinute>30)//00:30以后重新载入一次数据
		{
			m_DayLoadSavedSet[0].insert(idays);
			Reset();
		}
	}
	if(m_DayLoadSavedSet[1].count(idays)==0)
	{
		if(st.wHour==15&&st.wMinute>30)//15:30以后存一次
		{
			m_DayLoadSavedSet[1].insert(idays);
			SaveAllData();
		}
	}
}

void CDataCenter::UpdateBaseData()
{
	static int count=0;
	if(count==60)//60秒检查一次
	{
		count=0;
		//获取账户列表
		//if(GetTrustTradeAccountDirty())
		ReadTrustTradeAccount();
		//读取RiskWarning
		//if(GetRiskWarningDirty())
		ReadRiskWarning();
		//理财产品与资管组织关系
		//if(GetAssetOrgProRelationDirty())
		ReadAssetOrgProRelation();
		//获取强平权限
		//if(GetForceClosePermissionDirty())
		ReadForceClosePermission();
		//
		ReadEventMessageTemplate();
		//
		ReadRiskMgmtUserInfo();

		ReadNetFundParam();
		ReadForceCloseParam();

		std::string strFilaName="SaveAllData.txt";
		GetFolderFileName(strFilaName);
		if (DeleteFile(strFilaName.c_str()))
		{

			SaveAllData();
		}
		std::string strUserSpi="SaveUserSpi.txt";
		GetFolderFileName(strUserSpi);
		g_NeedSaveUserSpi=(_access(strUserSpi.c_str(),0)!=-1);
	}
	else
		count++;
}

void CDataCenter::GetRiskEventID( int& EventID )
{
	EventID=++m_nRiskEventID;
	EventID+=GetIDPre();
}

void CDataCenter::GetRiskEventHandlingID( int& HandlingID )
{
	HandlingID=++m_nRiskEventHandlingID;
	HandlingID+=GetIDPre();
}
bool CDataCenter::IsRiskWarningChanged()
{
	if(m_bRiskWarningChanged)
	{
		m_bRiskWarningChanged=false;
		return true;
	}
	return false;
}
void CDataCenter::FirstPrintf(const char* pMsg,bool bClear)
{
	static 	std::set<std::string> FirstPrintfString;
	if(FirstPrintfString.count(pMsg)==0)
	{
		LOG_INFO("FirstPrintf(%s)",pMsg);
		FirstPrintfString.insert(pMsg);
	}
	if(bClear)
	{
		FirstPrintfString.clear();
	}
}
void CDataCenter::WriteLogInfo(const char* pMsg, const char* pContent,int nID)
{	
	std::string strUserSpi="test.log";
	GetFolderFileName(strUserSpi);
	bool bExist = _access(strUserSpi.c_str(),0)!=-1;
	if(bExist)
	{
		if(m_pWriteLogTest == NULL)
			m_pWriteLogTest = new CWriteLog(WriteLogMode_LOCALFILE,"test.log");
		if(nID == -1)
			m_pWriteLogTest->WriteLog_Fmt("test.log", WriteLogLevel_DEBUGINFO, "[%s:%s]",pMsg, pContent);
		else
			m_pWriteLogTest->WriteLog_Fmt("test.log", WriteLogLevel_DEBUGINFO, "[%s:%s%d]",pMsg, pContent, nID);
	}
}
bool CDataCenter::GetNetFundParam(const std::string& Investor, NetFundParam& netFundParam)
{	
	m_Mutex_Account2NetFundParam.write_lock();
	std::map<std::string,NetFundParam>::const_iterator it=m_mapAccount2NetFundParam.find(Investor);
	if(it!=m_mapAccount2NetFundParam.end())
	{
		netFundParam=it->second;
		m_Mutex_Account2NetFundParam.write_unlock();
		return true;
	}
	m_Mutex_Account2NetFundParam.write_unlock();
	return false;
}
bool CDataCenter::GetForceCloseParam(const std::string& Investor, ForceCloseParam& forceCloseParam)
{
	m_Mutex_Account2ForceCloseParam.write_lock();
	std::map<std::string,ForceCloseParam>::const_iterator it=m_mapAccount2ForceCloseParam.find(Investor);
	if(it!=m_mapAccount2ForceCloseParam.end())
	{
		forceCloseParam=it->second;
		m_Mutex_Account2ForceCloseParam.write_unlock();
		return true;
	}
	m_Mutex_Account2ForceCloseParam.write_unlock();
	return false;
}

void CDataCenter::GetOpenOrder( std::string strInvestor, std::vector<SequencialOrderField>& vec )
{
	vec.clear();
	m_Mutex_AllOrder.write_lock();
	ORDERSTRUCT::iterator it = m_AllOrder.find(strInvestor);
	if ( it != m_AllOrder.end())
	{
		std::map<TSequenceNoType,SequencialOrderField>::iterator it_order
			= it->second.begin();
		for ( ; it_order != it->second.end(); it_order++ )
		{
			SequencialOrderField order = it_order->second;
			if ( order.OrderStatus == T_OST_PartTradedQueueing ||
				order.OrderStatus == T_OST_PartTradedNotQueueing ||
				order.OrderStatus == T_OST_NoTradeQueueing ||
				order.OrderStatus == T_OST_NoTradeNotQueueing )
			{
				vec.push_back(order);
			}
		}
	}
	m_Mutex_AllOrder.write_unlock();
}

void CDataCenter::GetPosition( std::string strInvestor, std::vector<SequencialPositionField>& vec )
{
	vec.clear();
	m_Mutex_AllPosition.write_lock();
	POSITIONSTRUCT::iterator it = m_AllPosition.find(strInvestor);
	if ( it != m_AllPosition.end())
	{
		std::map<TSequenceNoType,SequencialPositionField>::iterator it_position
			= it->second.begin();
		for ( ; it_position != it->second.end(); it_position++ )
		{
			if ( it_position->second.Position > 0 )
			{
				vec.push_back(it_position->second);
			}
		}
	}
	m_Mutex_AllPosition.write_unlock();
}

bool CDataCenter::GetExchangeID( std::string strInstrumentID, std::string& strExchangeID )
{
	bool bFlag = false;
	m_Mutex_AllInstrument.write_lock();
	std::map<std::string,InstrumentField>::iterator it = m_AllInstrument.find(strInstrumentID);
	if ( it != m_AllInstrument.end())
	{
		strExchangeID = it->second.ExchangeID;
		bFlag = true;
	}
	m_Mutex_AllInstrument.write_unlock();
	
	return bFlag;
}

bool CDataCenter::GetLimitPrice( std::string strInstrumentID, double& dUpperLimit, double& dLowerLimit )
{
	bool bFlag = false;
	m_Mutex_DepthMarketData.write_lock();
	std::map<std::string,DepthMarketDataField>::iterator it = m_DepthMarketData.find(strInstrumentID);
	if ( it != m_DepthMarketData.end() )
	{
		bFlag = true;
		dUpperLimit = it->second.UpperLimitPrice;
		dLowerLimit = it->second.LowerLimitPrice;
	}
	m_Mutex_DepthMarketData.write_unlock();

	return bFlag;
}

void CDataCenter::GetOpenOrderVolumn( std::string strInvestor, std::string strInstrumentID, char Direction,
									 int& nTodayVolumn, int& nYestodayVolumn )
{
	nTodayVolumn = 0;
	nYestodayVolumn = 0;
	m_Mutex_AllOrder.write_lock();
	ORDERSTRUCT::iterator it = m_AllOrder.find(strInvestor);
	if ( it != m_AllOrder.end())
	{
		std::map<TSequenceNoType,SequencialOrderField>::iterator it_order
			= it->second.begin();
		for ( ; it_order != it->second.end(); it_order++ )
		{
			SequencialOrderField order = it_order->second;
			if ( order.InstrumentID == strInstrumentID && 
				(order.OrderStatus == T_OST_PartTradedQueueing ||
				order.OrderStatus == T_OST_PartTradedNotQueueing ||
				order.OrderStatus == T_OST_NoTradeQueueing ||
				order.OrderStatus == T_OST_NoTradeNotQueueing) &&
				order.Direction == Direction )
			{
				if ( order.CombOffsetFlag[0] == THOST_FTDC_OF_Close )
				{
					nYestodayVolumn += order.VolumeTotal;
				}
				else if ( order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
				{
					nTodayVolumn += order.VolumeTotal;
				}
				else if ( order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday )
				{
					nYestodayVolumn += order.VolumeTotal;
				}
				else
				{

				}
			}
		}
	}
	m_Mutex_AllOrder.write_unlock();
}
bool CDataCenter::GetGatherData(int nIDType, int nID, SGatherData& sGathData )
{
	bool bFlag = false;
	if(nIDType == 0)
	{//组织机构
		m_Mutex_OrgID2GatherData.write_lock();
		std::map<int,  SGatherData>::iterator it = m_mapOrgID2GatherData.find(nID);
		if ( it != m_mapOrgID2GatherData.end())
		{
			sGathData = it->second;
			bFlag = true;
		}

		m_Mutex_OrgID2GatherData.write_unlock();
	}
	else if(nIDType == 1)
	{//理财产品
		m_Mutex_FinancialID2GatherData.write_lock();
		std::map<int,  SGatherData>::iterator it = m_mapFinancialID2GatherData.find(nID);
		if ( it != m_mapFinancialID2GatherData.end())
		{
			sGathData = it->second;
			bFlag = true;
		}

		m_Mutex_FinancialID2GatherData.write_unlock();
	}
	else if(nIDType == 2)
	{//委托交易账号
		m_Mutex_AccountID2GatherData.write_lock();
		std::map<int,  SGatherData>::iterator it = m_mapAccountID2GatherData.find(nID);
		if ( it != m_mapAccountID2GatherData.end())
		{
			sGathData = it->second;
			bFlag = true;
		}

		m_Mutex_AccountID2GatherData.write_unlock();
	}

	return bFlag;

}
double CDataCenter::CalcFundNetValueRisk( const std::string& InvestorID)
{
	double dblBalance= 0.0;
	GetBalance(InvestorID, dblBalance);//账户总权益		
	NetFundParam netFundParam;
	if(!GetNetFundParam(InvestorID, netFundParam))
		return -1;

	double dbP = 0.0;
	if(netFundParam.dOuterVolumn + netFundParam.dInnerVolumn > 0)
		dbP = (netFundParam.dOuterNetAsset + dblBalance)/(netFundParam.dOuterVolumn + netFundParam.dInnerVolumn);
	return dbP;
}