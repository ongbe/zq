
#include "DataCenter.h"
CDataCenter* CDataCenter::m_DataCenter=NULL;
CDataCenter::CDataCenter(void)
{
	m_nRiskEventID = GetMaxRiskEventID();

	m_nRiskEventHandlingID = 0;	
	ReadDB();
	LoadExchangeRestTime();
}

CDataCenter::~CDataCenter(void)
{
}
void CDataCenter::ClearInstrumentInfo()
{
	m_Mutex_ID2InstrumentInfo.write_lock();
	m_mapID2InstrumentInfo.clear();
	m_Mutex_ID2InstrumentInfo.write_unlock();
}
int CDataCenter::GetInstrumentInfo_DataCenter(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
	m_Mutex_ID2InstrumentInfo.write_lock();
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = m_mapID2InstrumentInfo.find(InstrumentID);
	if(it != m_mapID2InstrumentInfo.end())
	{
		outData = it->second;
		m_Mutex_ID2InstrumentInfo.write_unlock();
		return CF_ERROR_SUCCESS;
	}
	m_Mutex_ID2InstrumentInfo.write_unlock();

	if( CF_ERROR_SUCCESS == CInterface_SvrTradeData::getObj().GetInstrumentInfo(InstrumentID,outData))
	{
		m_mapID2InstrumentInfo.insert(make_pair(InstrumentID, outData));
		return CF_ERROR_SUCCESS;
	}
	return -1;
}
int CDataCenter::GetInstrumentInfos_ByProduct(const std::string& ProductID, std::set<std::string>& outData)
{
	m_Mutex_Product2Instruments.write_lock();
	std::map<std::string, std::set<std::string>>::iterator it = m_mapProduct2Instruments.find(ProductID);
	if(it != m_mapProduct2Instruments.end())
	{
		outData = it->second;
		m_Mutex_Product2Instruments.write_unlock();
		return CF_ERROR_SUCCESS;
	}
	m_Mutex_Product2Instruments.write_unlock();
	
	if( CF_ERROR_SUCCESS == CInterface_SvrTradeData::getObj().GetInstrumentListByProductID(ProductID,outData))
	{
		m_mapProduct2Instruments.insert(make_pair(ProductID, outData));
		return CF_ERROR_SUCCESS;
	}
	return -1;
}
void CDataCenter::Reset()
{
	ReadRiskWarning();
	ReadRiskIndicator();
	m_Mutex_RiskEvent.read_lock();
	m_HistoryEvent.clear();
	m_ActiveEvent.clear();
	m_Mutex_RiskEvent.read_unlock();
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
		if(st.wHour==17&&st.wMinute>30)//17:30以后存一次结算后数据
		{
			m_DayLoadSavedSet[1].insert(idays);
			SaveAllData();
		}
	}
	std::string strFilaName="SaveAllData.txt";
	GetFolderFileName(strFilaName);
	if (DeleteFile(strFilaName.c_str()))
	{
		SaveAllData();
	}
}

void CDataCenter::split(const string& s, char c, vector<string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);
	if(j == -1 && s.length() != 0)
	{
		v.push_back(s);
		return;
	}
	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}

}
void CDataCenter::ReadDB()
{
	//读取风控风险指标判断设置
	ReadRiskWarning();
	ReadRiskIndicator();
	ReadAccountLossDays();
}
void CDataCenter::ReadAccountLossDays()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select t.brokerid,t.accountid, t.dynamicprofit, t.lossdaycount, t.lastday, t.maxdynamicprofit from RISK_LOSSDAY t");
	std::vector<std::vector<_variant_t>> vNode;
	int nErrorCode = CF_ERROR_SUCCESS;
	if(!CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size()==0)
	{
		return;
	}
	else
	{
		AccountLossDay accountLossDay;
		for(UINT j = 0; j < vNode.size(); j++) 
		{
			memset(&accountLossDay, 0, sizeof(accountLossDay));
			std::vector<_variant_t> vValue = vNode[j];
			{
				int i = 0;	
				BrokerAccountKey bakey;
				strncpy(bakey.BrokerID, (_bstr_t)(vValue[i++]), sizeof(bakey.BrokerID)-1);
				strncpy(bakey.AccountID, (_bstr_t)(vValue[i++]), sizeof(bakey.AccountID)-1);				

				strncpy(accountLossDay.BrokerID, bakey.BrokerID, sizeof(bakey.BrokerID)-1);
				strncpy(accountLossDay.AccountID, bakey.AccountID, sizeof(bakey.AccountID)-1);					
				accountLossDay.dynamicProfit  = vValue[i++];	
				accountLossDay.nlossdaycount  = vValue[i++];	
				strncpy(accountLossDay.szLastDate, (_bstr_t)(vValue[i++]), sizeof(accountLossDay.szLastDate)-1);	
				accountLossDay.maxdynamicProfit  = vValue[i++];	

				m_Mutex_AccountDay.read_lock();
				m_mapAccountDay.insert(make_pair(bakey, accountLossDay));
				m_Mutex_AccountDay.read_unlock();
			}
		}
	}
}
void CDataCenter::ReadRiskWarning()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select t2.orgid,t2.riskindicatorid,t2.allproduct,t2.riskproduct,t2.use, t1.riskindicatorid, t1.risklevelid,t1.thresholdvalue, t1.responsetype, t1.color,t3.innervolumn,t3.outervolumn,t3.outernetasset,t3.innernetassetoption, t2.RISKTYPE, \
					  t2.BEGINTIME, t2.ENDTIME, t2.INSTRUMENTS, t2.MARGINDIVIDDYNAMIC, t2.MARGINUSE, t2.LOSSAMOUNT, t2.LOSSPERCENT, t2.planname  from RISK_RISKWARING t1, RISK_ORGIDPLANRELATION t2 left join  RISK_FUNDNETPARAM t3 on t2.orgid = t3.orgid and t2.riskindicatorid = %d where t1.RISKINDICATORID = t2.RISKINDICATORID and t1.orgid = t2.orgid  and t1.begintime = t2.begintime and t1.endtime = t2.endtime",
					  (int)RI_FundNetValue);

	std::vector<RiskPlan> vecRiskPlan;
	int  nErrorCode = CF_ERROR_SUCCESS;
	CInterface_SvrDBOpr::getObj().GetRiskPlan(szBuffer, vecRiskPlan, nErrorCode);
	m_Mutex_RiskWarning.write_lock();
	m_RiskWarning.clear();
	for(int i = 0; i< (int)vecRiskPlan.size(); i++)
	{	
		RiskPlan& plan = vecRiskPlan[i];
		if(plan.OrgIDPlanRelation.bUse == false)
			continue;//对于不启用的风险指标不进行风险计算判断

		RiskKey key;
		key.nOrgID		= plan.OrgIDPlanRelation.nOrgID;
		key.RiskType	= plan.OrgIDPlanRelation.nRiskIndicatorID;

		std::vector<std::string> vecLevels;	
		if(!plan.OrgIDPlanRelation.bAllProduct)
			split(plan.OrgIDPlanRelation.ProductID, ';', vecLevels);	

		std::vector<std::string> vecInstruments;	
		split(plan.OrgIDPlanRelation.cInstruments, ';', vecInstruments);	


		int nCount = m_RiskWarning.count(key);
		std::multimap<RiskKey, SResponse>::iterator it = m_RiskWarning.find(key);		
		if(it == m_RiskWarning.end())//m_RiskWarning高风险到低风险排列
		{
			SResponse sResponse;
			for(int k =0; k < (int)vecLevels.size(); k++)
			{
				std::string str = vecLevels[k];
				sResponse.setProducts.insert(str);
			}
			for(int k =0; k < (int)vecInstruments.size(); k++)
			{
				std::string str = vecInstruments[k];
				sResponse.setInstruments.insert(str);
			}
			sResponse.mapResponse.insert(make_pair(plan.WaringLevel.nRiskLevelID, plan));	
			m_RiskWarning.insert(make_pair(key, sResponse));
		}
		else
		{
			for(int i =0; i< nCount; i++)
			{
				std::string szBegin = "", szEnd = "";
				SResponse&  mapTemp = it->second;	
				std::map<int,RiskPlan>::iterator itPlan = mapTemp.mapResponse.begin();
				if(itPlan != mapTemp.mapResponse.end())
				{
					szBegin = itPlan->second.OrgIDPlanRelation.cTimeBegin;
					szEnd	= itPlan->second.OrgIDPlanRelation.cTimeEnd;					
				}
				if(strcmp(szBegin.c_str(), plan.OrgIDPlanRelation.cTimeBegin) == 0 
					&& strcmp(szEnd.c_str(), plan.OrgIDPlanRelation.cTimeEnd) == 0 
					&& szBegin != "" && szEnd != "")
				{					
					it->second.mapResponse.insert(make_pair(plan.WaringLevel.nRiskLevelID, plan));	
					break;
				}
				else if(i == nCount -1)
				{//说明是不同时间段的同一种风险事件
					SResponse sResponse;
					for(int k =0; k < (int)vecLevels.size(); k++)
					{
						std::string str = vecLevels[k];
						sResponse.setProducts.insert(str);
					}
					for(int k =0; k < (int)vecInstruments.size(); k++)
					{
						std::string str = vecInstruments[k];
						sResponse.setInstruments.insert(str);
					}
					sResponse.mapResponse.insert(make_pair(plan.WaringLevel.nRiskLevelID, plan));	
					m_RiskWarning.insert(make_pair(key, sResponse));
				}
				it++;
			}			
			
		}
	}
	m_Mutex_RiskWarning.write_unlock();

	m_Mutex_multimapOrgid2RiskPlan.write_lock();
	m_multimapOrgid2RiskPlan.clear();
	for(int i = 0; i< (int)vecRiskPlan.size(); i++)
	{	
		RiskPlan& plan = vecRiskPlan[i];
		m_multimapOrgid2RiskPlan.insert(make_pair(plan.OrgIDPlanRelation.nOrgID, plan));		
	}
	m_Mutex_multimapOrgid2RiskPlan.write_unlock();

	ReadNetFundParam();
}

void CDataCenter::ReadRiskIndicator()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from RISK_RISKINDICATOR t order by t.riskindicatorid");
	vector<RiskIndicator> vec;
	int nErrorCode = CF_ERROR_SUCCESS;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vec, nErrorCode))
		return;

	m_Mutex_multimapRiskIndicator.write_lock();
	for(int i =0; i < (int)vec.size(); i++)
	{
		RiskIndicator& riskIndicator = vec[i];
		int nID = (int)riskIndicator.nRiskIndicatorID;
		m_mapRiskIndicator[nID] = riskIndicator;
	}
	m_Mutex_multimapRiskIndicator.write_unlock();
}
bool CDataCenter::GetRiskIndicatorName(int nID, std::string& strName)
{
	bool bFind = false;
	m_Mutex_multimapRiskIndicator.write_lock();
	std::map<int, RiskIndicator>::iterator it = m_mapRiskIndicator.find(nID);
	if(it != m_mapRiskIndicator.end())
	{
		strName = it->second.szName;
		bFind = true;
	}
	m_Mutex_multimapRiskIndicator.write_unlock();
	return true;
}
bool CDataCenter::GetRiskPlanByOrgID(int nOrgID, vector<RiskPlan>& vec)
{	
	int nCount = m_multimapOrgid2RiskPlan.count(nOrgID);
	std::multimap<int, RiskPlan>::iterator it = m_multimapOrgid2RiskPlan.find(nOrgID);
	for(int i =0; i<nCount; i++)
	{
		m_Mutex_multimapOrgid2RiskPlan.write_lock();
		RiskPlan& plan = it->second;
		vec.push_back(plan);
		m_Mutex_multimapOrgid2RiskPlan.write_unlock();
		it++;
	}	
	
	return true;
}
void CDataCenter::ReadEventMessageTemplate()
{	
	std::vector<EventMessageTemplate> vec;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from RISK_EVENTMESSAGETEMPLATE t");

	if (!CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vec))
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
void CDataCenter::ReadNetFundParam()
{
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	strcpy_s(szBuffer,MAX_SQL_LENGTH, "select * from RISK_FUNDNETPARAM t");
	std::vector<NetFundParam> vec;
	if (!CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vec))
		return;
	m_Mutex_OrgID2NetFundParam.write_lock();
	m_mapOrgID2NetFundParam.clear();	
	for (size_t i=0;i<vec.size();++i)
	{	
		NetFundParam& para = vec[i];
		int nID = para.nOrgID;
		m_mapOrgID2NetFundParam[nID]=vec[i];		
	}
	m_Mutex_OrgID2NetFundParam.write_unlock();
}
void CDataCenter::LoadExchangeRestTime()
{
	m_mapRestTime.clear();

	vector<EXCHANGERESTTIME> vecRestTime;
	int nStartTime = 0, nEndTime = 0, nSubNum = 0;
	EXCHANGERESTTIME subTime;

	ZeroMemory(&subTime, sizeof(subTime));

	subTime.nStart = 9*3600 + 15*60;
	subTime.nEnd = 9*3600 + 30*60;
	subTime.nSubNum = subTime.nEnd - subTime.nStart;
	vecRestTime.push_back(subTime);

	subTime.nStart = 11*3600 + 30*60;
	subTime.nEnd = 13*3600 + 30*60;
	subTime.nSubNum = subTime.nEnd - subTime.nStart;
	vecRestTime.push_back(subTime);

	m_mapRestTime.insert(std::make_pair("shfe", vecRestTime));
	m_mapRestTime.insert(std::make_pair("czce", vecRestTime));
	m_mapRestTime.insert(std::make_pair("dce", vecRestTime));

	vecRestTime.clear();
	subTime.nStart = 11*3600 + 30*60;
	subTime.nEnd = 13*3600;
	subTime.nSubNum = subTime.nEnd - subTime.nStart;
	vecRestTime.push_back(subTime);

	m_mapRestTime.insert(std::make_pair("cffex", vecRestTime));
}

void CDataCenter::SaveAllData()
{
	int step=0;
	char strTime[64];
	GetTodayString(strTime);
	//风控事件
	{
		std::vector<RiskEvent> vecEvent;
		std::map<RiskEventIDType,std::vector<RiskEvent>>::const_iterator it;
		m_Mutex_RiskEvent.read_lock();/*
		for (it=m_HistoryEvent.begin();it!=m_HistoryEvent.end();++it)
		{
			const std::vector<RiskEvent>& vec=it->second;
			vecEvent.insert(vecEvent.end(),vec.begin(),vec.end());
		}*/
		for (it=m_ActiveEvent.begin();it!=m_ActiveEvent.end();++it)
		{
			const std::vector<RiskEvent>& vec=it->second;
			vecEvent.insert(vecEvent.end(),vec.begin(),vec.end());
		}
		m_Mutex_RiskEvent.read_unlock();	
		
		if(vecEvent.size() >0)
		{//非交易时间重启后会删除已经存储的风险事件
			int nErrorCode = CF_ERROR_SUCCESS;
			CInterface_SvrDBOpr::getObj().SaveRiskEvent(strTime,vecEvent, nErrorCode, false);
			if(nErrorCode == CF_ERROR_SUCCESS)
			{
				OUTPUT_LOG("SaveAllData(第%d步):风控事件(共%d条记录)",++step,vecEvent.size());
			}
			else
			{
               CInterface_SvrDBOpr::getObj().SaveRiskEvent2File(vecEvent);
			}
		}
	}

	OUTPUT_LOG("基金净值数据保存日期: %s", strTime);
	char strTime2[64];
	memset(&strTime2, 0, sizeof(strTime2));
	{
		std::vector<NetFundCalcResult> vResult;

		std::vector<UserInfo> vec;
		CInterface_SvrUserOrg::getObj().GetUserInfos(vec);//得到所有交易员
		for(int i=0; i<(int)vec.size(); i++)
		{
			UserInfo& userInfo = vec[i];
			if(USER_TYPE_TRADER != userInfo.nUserType)
				continue;

			double  dblBalance = 0.0;
			PlatformStru_TradingAccountInfo outData;
			if(CF_ERROR_SUCCESS != CInterface_SvrTradeData::getObj().GetUserFundInfo(userInfo.szAccount, outData))			
				continue;			
			dblBalance	 = outData.DynamicProfit; //动态权益
			SaveAccountLossDays(outData.BrokerID, outData.AccountID, dblBalance);

			
			OUTPUT_LOG("基金净值:账号：%s,  总权益：%.8f", userInfo.szAccount ,dblBalance);
			int nOrgID = -1;
			CInterface_SvrUserOrg::getObj().GetOrgIDByUserID(userInfo.nUserID, nOrgID);
			NetFundParam netFundParam;
			if(!GetNetFundParam(nOrgID, netFundParam))
			{
				OUTPUT_LOG("基金净值配置表FUNDNETPARAM里没有该账户:%s",userInfo.szAccount);
				continue;
			}
			NetFundCalcResult sResult;
			sResult.nTradeAccountID = userInfo.nUserID;
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
			OUTPUT_LOG("委托交易账号ID:%d; 场内份额：%.2f  场外份额：%.6f 场内净值：%.6f 场外净值：%.6f 场内单位净值：%.6f 场外单位净值：%.6f 总资产净值：%.6f, 时间：%s",
				userInfo.szAccount, sResult.dInnerVolumn, sResult.dOuterVolumn, sResult.dInnerNetAsset, sResult.dOuterNetAsset,
				sResult.dInnerPerNet, sResult.dOuterPerNet, sResult.dTotalNetAsset, strTime2);
		}
	
		
		int nErrorCode = CF_ERROR_SUCCESS;
		CInterface_SvrDBOpr::getObj().SaveNetFundCalcResult(strTime2, vResult, nErrorCode);
		if(nErrorCode == CF_ERROR_SUCCESS)
		{
            OUTPUT_LOG("SaveAllData(第%d步):基金净值(共%d条记录)",++step,vResult.size());
		}
		else
		{
			CInterface_SvrDBOpr::getObj().SaveNetFundCalcResult2File(vResult);
		}
		

	}
}
void CDataCenter::SaveAccountLossDays(std::string strBrokerID, std::string strAccountID, double dblBalance)
{
	BrokerAccountKey bakey;
	strcpy(bakey.BrokerID, strBrokerID.c_str());
	strcpy(bakey.AccountID, strAccountID.c_str());
	
	AccountLossDay account;
	strcpy(account.BrokerID, strBrokerID.c_str());
	strcpy(account.AccountID, strAccountID.c_str());
	account.nlossdaycount = 0;
	account.dynamicProfit = 0;
	account.maxdynamicProfit = 0;

	m_Mutex_AccountDay.read_lock();
	std::map<BrokerAccountKey, AccountLossDay>::iterator it = m_mapAccountDay.find(bakey);
	if(it != m_mapAccountDay.end())
	{
		if(account.dynamicProfit < it->second.dynamicProfit)
		{
			account.nlossdaycount = it->second.nlossdaycount + 1;
		}
		account.dynamicProfit = it->second.dynamicProfit;

		if(account.maxdynamicProfit < it->second.maxdynamicProfit)
		{
			account.maxdynamicProfit = it->second.maxdynamicProfit;
		}
	}
	m_mapAccountDay[bakey] = account;//更新最新值

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "Insert into RISK_LOSSDAY(brokerid, accountid, dynamicprofit, lossdaycount, lastday, maxdynamicprofit) \
		values('%s', '%s', %f, %d, sysdate, %f, %f)", 
		account.BrokerID, account.AccountID, account.dynamicProfit, account.nlossdaycount, account.maxdynamicProfit);				
	int nErrorCode = CF_ERROR_SUCCESS;
	CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode);
	m_Mutex_AccountDay.read_unlock();
}
bool CDataCenter::GetAccountLossDays(BrokerAccountKey& bakey, AccountLossDay& accountLossDay)
{
	bool bGet= false;
	m_Mutex_AccountDay.read_lock();
	std::map<BrokerAccountKey, AccountLossDay>::iterator it = m_mapAccountDay.find(bakey);
	if(it != m_mapAccountDay.end())
	{		
		accountLossDay = it->second;
		bGet = true;
	}	
	m_Mutex_AccountDay.read_unlock();
	return bGet;
}
bool CDataCenter::SetAccountLossDays(BrokerAccountKey& bakey, AccountLossDay& accountLossDay)
{	
	m_Mutex_AccountDay.read_lock();
	m_mapAccountDay[bakey] = accountLossDay;
	m_Mutex_AccountDay.read_unlock();
	return true;
}
CDataCenter* CDataCenter::Get()
{
	return m_DataCenter;
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
bool CDataCenter::GetFundParam(RiskKey& rKey, std::string strTime, NetFundParam& netFundParam)
{
	m_Mutex_RiskWarning.write_lock();
	RISKWARNINGSTRUCT::iterator it = m_RiskWarning.find(rKey);
	if(it == m_RiskWarning.end())
	{
		m_Mutex_RiskWarning.write_unlock();
		return false;//找不到该组织机构的风险指标设置
	}
	SResponse warnings = it->second;
	m_Mutex_RiskWarning.write_unlock();

	std::map<int,RiskPlan>::const_iterator coreit;
	for (coreit=warnings.mapResponse.begin();coreit!=warnings.mapResponse.end();++coreit)
	{
		const RiskPlan& risk=coreit->second;
		netFundParam = risk.netFundParam;
		return true;
	}
	return false;	
}
int CDataCenter::GetWarningMapLevel(RiskKey& rKey, std::string strTime, double value, SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd)
{
	SResponse warnings;
	if(!GetResponseOfRiskWarning(rKey, strTime, warnings))
		return 0;

	std::map<int,RiskPlan>::const_iterator coreit;
	for (coreit=warnings.mapResponse.begin();coreit!=warnings.mapResponse.end();++coreit)
	{
		const RiskPlan& risk=coreit->second;
		if(rKey.RiskType == RI_FundNetValue)
		{
			if(value < risk.WaringLevel.fThresholdValue+0.00001)//加上0.00001主要是为了防止相等时候不触发风险
			{
				LevelID=coreit->first;
				sResponse.mapResponse[LevelID] = risk;
				strTimeBegin = risk.OrgIDPlanRelation.cTimeBegin;
				strTimeEnd   = risk.OrgIDPlanRelation.cTimeEnd;
				return 1;//有风险
			}
		}
		else
		{
			if(value > risk.WaringLevel.fThresholdValue -0.00001)//减去0.00001主要是为了防止相等时候不触发风险
			{
				LevelID=coreit->first;				
				sResponse.mapResponse[LevelID] = risk;	
				strTimeBegin = risk.OrgIDPlanRelation.cTimeBegin;
				strTimeEnd   = risk.OrgIDPlanRelation.cTimeEnd;
				return 1;//有风险
			}
		}
	}
	
	return -1;//没有风险
}
bool  CDataCenter::GetAllMaxPositonZeroWarningMapLevel(RiskKey& rKey, std::string strTime, SResponse& vecResponse, std::string& strTimeBegin, std::string& strTimeEnd)
{
	
/*	RISKWARNINGSTRUCT::iterator it = m_RiskWarning.find(rKey);
	if(it == m_RiskWarning.end())
	{
		m_Mutex_RiskWarning.write_unlock();
		return false;//找不到该组织机构的风险指标设置
	}
	
	SResponse warnings = it->second;*/

	SResponse warnings;
	if(!GetResponseOfRiskWarning(rKey, strTime, warnings))
		return 0;

	m_Mutex_RiskWarning.write_lock();
	std::map<int,RiskPlan>::const_iterator coreit;
	int nPrevLevel = -1;
	BOOL bHasRisk = FALSE;

	for(coreit = warnings.mapResponse.begin();coreit != warnings.mapResponse.end();++coreit) 
	{
		const RiskPlan& risk=coreit->second;
		if(risk.WaringLevel.fThresholdValue > -0.00001 && risk.WaringLevel.fThresholdValue < 0.00001) 
		{//有0值的最大风险事件方案设置
			vecResponse = warnings;
			strTimeBegin = risk.OrgIDPlanRelation.cTimeBegin;
			strTimeEnd   = risk.OrgIDPlanRelation.cTimeEnd;
			break;
		}
	}
	m_Mutex_RiskWarning.write_unlock();
	return true;
}
int CDataCenter::GetPosiDetailLevel(RiskKey& rKey, double value, const string& strInstID, 
									SResponse& retRsp, int& LevelID)
{
	m_Mutex_RiskWarning.write_lock();
	RISKWARNINGSTRUCT::iterator it = m_RiskWarning.find(rKey);
	if(it == m_RiskWarning.end())
	{
		m_Mutex_RiskWarning.write_unlock();
		return 0;
	}

	SResponse warnings = it->second;
	m_Mutex_RiskWarning.write_unlock();

	std::string strProduct;
	PlatformStru_InstrumentInfo info;	
	if(GetInstrumentInfo_DataCenter(strInstID,info) != CF_ERROR_SUCCESS)
		return -1;
	strProduct = info.ProductID;

	if(warnings.setProducts.size()>0 && warnings.setProducts.find(strProduct)==warnings.setProducts.end())
		return 0;	//设置了品种，但是没有找到；放弃判断风险则返回-1

	std::map<int,RiskPlan>::const_iterator coreit;
	int nPrevLevel = -1;
	BOOL bHasRisk = FALSE;

	for(coreit=warnings.mapResponse.begin();coreit!=warnings.mapResponse.end();++coreit) {
		const RiskPlan& risk=coreit->second;
		if(rKey.RiskType == RI_SingleHoldTime) {
			if(value < risk.WaringLevel.fThresholdValue-0.00001) {//减去0.00001主要是为了防止相等时候不触发风险 
				break;
			}
			else {
				bHasRisk = TRUE;
				nPrevLevel = coreit->first;
			}
		}
	}
	if(bHasRisk) {
		warnings.setProducts.clear();				//把里面品种相关的清除
		warnings.setProducts.insert(strInstID);		//针对具体某个合约的风险
		retRsp = warnings;
		LevelID = nPrevLevel;
		return 1;//有风险
	}
	else 	
		return -1;//没有风险
}

int CDataCenter::GetMaxPositionWarningMapLevel(RiskKey& rKey, std::string strTime, std::string InstrumentID, double value,  SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd)
{
	SResponse warnings;
	if(!GetResponseOfRiskWarning(rKey, strTime, warnings))
		return 0;

	//if(InstrumentID.length() !=0)
	{//主要针对最大持仓品种
		std::string strProduct;
		PlatformStru_InstrumentInfo info;	
		if(GetInstrumentInfo_DataCenter(InstrumentID,info) != CF_ERROR_SUCCESS)
			return -1;
		strProduct = info.ProductID;

		set<std::string>& setInstruments = warnings.setProducts;
		if(setInstruments.size() != 0)
		{
			set<std::string>::iterator result = setInstruments.find(strProduct);
				// find( vecInstruments.begin( ), vecInstruments.end( ), strProduct ); //查找strProduct

			if(result == setInstruments.end())
				return 0;//设置了品种，但是没有找到；放弃判断风险则返回-1
		}
	}
	
	std::map<int,RiskPlan>::const_iterator coreit;
	for (coreit=warnings.mapResponse.begin();coreit!=warnings.mapResponse.end();++coreit)
	{
		const RiskPlan& risk=coreit->second;
		if(value > risk.WaringLevel.fThresholdValue - 0.00001)//减去0.00001主要是为了防止相等时候不触发风险
		{
			LevelID=coreit->first;
			
			sResponse.mapResponse[LevelID] = risk;
			sResponse.setProducts.insert(InstrumentID);//针对具体某个合约的风险
			strTimeBegin = risk.OrgIDPlanRelation.cTimeBegin;
			strTimeEnd   = risk.OrgIDPlanRelation.cTimeEnd;
		
			return 1;//有风险
		}
	}

	return -1;//没有风险
}
bool CDataCenter::GetParentOrgID(int nOrgID, int& nParentID)
{
	Organization org;
	if(!CInterface_SvrUserOrg::getObj().GetOrgByID(nOrgID, org))
		return false;

	nParentID = org.nUpperLevelOrgID;
	return true;
}
void CDataCenter::GetRiskEventID( int& EventID )
{
	EventID=++m_nRiskEventID;
	EventID+=GetIDPre();
}
int CDataCenter::GetIDPre()
{
	struct tm *ptr;
	time_t lt;
	lt =time(NULL);
	ptr=localtime(&lt);
	int idays=(ptr->tm_year-113)*1000+ptr->tm_yday;
	return idays*100000;
}
int CDataCenter::GetMaxRiskEventID()
{
	int nMaxRiskEventID = 0;

	char strTime[64];
	GetTodayString(strTime);

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select max(riskeventid) from RISK_RISKEVENT t where	  \
					  to_char(to_date('1970-01-01','yyyy-MM-dd')+t.eventtime/86400,'YYYYMMDD') = '%s'", strTime);

	std::vector<std::vector<_variant_t>> vNode;
	int nErrorCode = CF_ERROR_SUCCESS;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode))
	{		
		return 0;
	}
	else
	{ 	
		if(vNode.size() >0)
		{
			std::vector<_variant_t> vValue = vNode[0];
			int nRiskEventID = vValue[0].intVal;
			if(nRiskEventID != 0)
				nMaxRiskEventID = nRiskEventID - GetIDPre()+1;//取当前数据库里面最大ID值
		}
	}
	return nMaxRiskEventID;
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
void CDataCenter::ChangeActiveEvent(const RiskEvent& evt )
{
	std::map<RiskEventIDType,std::vector<RiskEvent>>::iterator it;
	m_Mutex_RiskEvent.write_lock();
	it=m_ActiveEvent.find(evt.nRiskEventID);
	if(it!=m_ActiveEvent.end())
	{
		//加入历史
		std::vector<RiskEvent>& ss=it->second;		
		if(ss.size() > 0)
		{
			ss[ss.size()-1].dblIndicatorCurrentValue = evt.dblIndicatorCurrentValue;	
			ss[ss.size()-1].dblIndicatorCurrentValue2 = evt.dblIndicatorCurrentValue2;	
		}
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

		//保存刚消失的风险事件
		char strTime[64];
		GetTodayString(strTime);
		int nErrorCode = CF_ERROR_SUCCESS;
		CInterface_SvrDBOpr::getObj().SaveRiskEvent(strTime,ss, nErrorCode, false);
		if(nErrorCode != CF_ERROR_SUCCESS)
		
		{
			CInterface_SvrDBOpr::getObj().SaveRiskEvent2File(ss);
		}

		//删除激活
		m_ActiveEvent.erase(evt.nRiskEventID);
	}
	m_Mutex_RiskEvent.write_unlock();
}
bool CDataCenter::GetActiveEvent(int nRiskEventID, RiskEvent&	riskEvent)
{
	bool bGet = false;
	std::map<RiskEventIDType,std::vector<RiskEvent>>::iterator it;
	m_Mutex_RiskEvent.write_lock();
	it=m_ActiveEvent.find(nRiskEventID);
	if(it!=m_ActiveEvent.end())
	{
		//加入历史
		std::vector<RiskEvent>& ss=it->second;		
		if(ss.size() > 0)
		{
			riskEvent = ss[ss.size()-1];
			bGet = true;
		}
	}
	m_Mutex_RiskEvent.write_unlock();
	
	return bGet;
}
void CDataCenter::GetAllActiveEvent(std::map<RiskEventIDType,std::vector<RiskEvent>>&  ActiveEvent)
{
	m_Mutex_RiskEvent.write_lock();
	ActiveEvent = m_ActiveEvent;
	m_Mutex_RiskEvent.write_unlock();
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
void CDataCenter::GetRiskEventHandlingID( int& HandlingID )
{
	HandlingID=++m_nRiskEventHandlingID;
	HandlingID+=GetIDPre();
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
void CDataCenter::SubscribeRiskEvent(int AccID)
{	
	m_Mutex_SubscribeInfo.write_lock();
	UserInfo userInfo;	
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);  
	if(bFind)
	{
		std::string strAcc = userInfo.szAccount;
		std::map<std::string, SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bRiskEvent =true;	
		}
		else
		{
			SubscribeType st;
			ZeroMemory(&st,sizeof(st));
			st.AccID=AccID;
			st.bRiskEvent = true;
			m_SubscribeInfo[strAcc]=st;
		}
	}
	m_Mutex_SubscribeInfo.write_unlock();	
}
void CDataCenter::UnSubscribeRiskEvent(int AccID)
{
	UserInfo userInfo;
	std::string strAcc;
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);                                //GetTradeAccountID(AccID);
	if(bFind)
	{
		strAcc = userInfo.szAccount;
		std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		m_Mutex_SubscribeInfo.write_lock();
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bRiskEvent=false;	
		}
			m_Mutex_SubscribeInfo.write_unlock();
	}
	//UNSUBSCRIBE_INFO(AccID,bRiskEvent);
}
void CDataCenter::GetRiskEventList( int AccID, std::vector<RiskEvent>& vecEvent ) const
{
	std::map<RiskEventIDType,std::vector<RiskEvent>>::const_iterator it;
	m_Mutex_RiskEvent.read_lock();
/*	for (it=m_HistoryEvent.begin();it!=m_HistoryEvent.end();++it)
	{
		const std::vector<RiskEvent>& vec=it->second;
		for (size_t i=0;i<vec.size();++i)
		{
			if(vec[i].nTradeInvestorID==AccID)
				vecEvent.push_back(vec[i]);
		}
	}*/
	for (it=m_ActiveEvent.begin();it!=m_ActiveEvent.end();++it)
	{		
		const std::vector<RiskEvent>& vec=it->second;
		
		bool bFind =false;
		RSTIME timeLast = 0;
		RiskEvent risk;
		for(int k =0; k<(int)vec.size(); k++)
		{
			const RiskEvent& riskevent= vec[k];
			if(riskevent.lEventTime >timeLast && riskevent.nTradeInvestorID==AccID)
			{
				bFind = true;
				risk = riskevent;
				timeLast = riskevent.lEventTime;
			}
		}
		if(bFind)
			vecEvent.push_back(risk);		
	}
	std::sort(vecEvent.begin(),vecEvent.end());
	m_Mutex_RiskEvent.read_unlock();
}
bool CDataCenter::GetNetFundParam(int nID, NetFundParam& netFundParam)
{	
	m_Mutex_OrgID2NetFundParam.write_lock();
	int nOrgID = nID;
	while(1)
	{		
		std::map<int,NetFundParam>::const_iterator it=m_mapOrgID2NetFundParam.find(nOrgID);
		if(it == m_mapOrgID2NetFundParam.end())
		{
			Organization org;
			if(!CInterface_SvrUserOrg::getObj().GetOrgByID(nOrgID, org))
			{			
				m_Mutex_OrgID2NetFundParam.write_lock();
				return false;//如果没有父级，则查找失败
			}
			nOrgID = org.nUpperLevelOrgID;//从父级里面查找
		}
		else 		
		{
			netFundParam=it->second;
			m_Mutex_OrgID2NetFundParam.write_unlock();
			break;
		}
	}	
	return true;
}
double CDataCenter::CalcFundNetValueRisk( const string& InvestorID)
{
	double dblBalance= 0.0;
	PlatformStru_TradingAccountInfo outData;
	if(CF_ERROR_SUCCESS != CInterface_SvrTradeData::getObj().GetUserFundInfo(InvestorID, outData))
		return -1;
	dblBalance = outData.DynamicProfit;//动态权益

	int nOrgID = -1;
	CInterface_SvrUserOrg::getObj().GetOrgIDByAccount(InvestorID, nOrgID);
	NetFundParam netFundParam;
	if(!GetNetFundParam(nOrgID, netFundParam))
		return -1;

	double dbP = 0.0;
	if(netFundParam.dOuterVolumn + netFundParam.dInnerVolumn > 0)
		dbP = (netFundParam.dOuterNetAsset + dblBalance)/(netFundParam.dOuterVolumn + netFundParam.dInnerVolumn);
	return dbP;
}
void CDataCenter::SubscribeQuot(const std::string& strInstrument)
{
	m_Mutex_SubscribeQuot.write_lock();
	m_SubscribeQuot.insert(strInstrument);
	m_Mutex_SubscribeQuot.write_unlock();
}
void CDataCenter::UnSubscribeQuot(const std::string& strInstrument)
{
	m_Mutex_SubscribeQuot.write_lock();
	m_SubscribeQuot.erase(strInstrument);
	m_Mutex_SubscribeQuot.write_unlock();
}
bool CDataCenter::IsQutoSubscribed(PlatformStru_DepthMarketData& DepthMarketData)
{
	bool bIsSubscribed = false;
	m_Mutex_SubscribeQuot.read_lock();
	std::set<std::string>::const_iterator it=m_SubscribeQuot.find(DepthMarketData.InstrumentID);
	if(it!=m_SubscribeQuot.end())
	{	
		bIsSubscribed = true;
	}
	m_Mutex_SubscribeQuot.read_unlock();

	return bIsSubscribed;
}
void CDataCenter::SubscribeTrade(int AccID)
{
	m_Mutex_SubscribeInfo.write_lock();
	UserInfo userInfo;	
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);  
	if(bFind)
	{
		std::string strAcc = userInfo.szAccount;
		std::map<std::string, SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bTrade =true;	
		}
		else
		{
			SubscribeType st;
			ZeroMemory(&st,sizeof(st));
			st.AccID=AccID;
			st.bTrade = true;
			m_SubscribeInfo[strAcc]=st;
		}
	}
	m_Mutex_SubscribeInfo.write_unlock();	
}
void CDataCenter::UnSubscribeTrade(int AccID)
{
	UserInfo userInfo;
	std::string strAcc;
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);                                //GetTradeAccountID(AccID);
	if(bFind)
	{
		strAcc = userInfo.szAccount;
		std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		m_Mutex_SubscribeInfo.write_lock();
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bTrade=false;	
		}
		m_Mutex_SubscribeInfo.write_unlock();
	}
	//UNSUBSCRIBE_INFO(AccID,bTrade);
}
void CDataCenter::SubscribeFund(int AccID)
{
	m_Mutex_SubscribeInfo.write_lock();
	UserInfo userInfo;	
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);  
	if(bFind)
	{
		std::string strAcc = userInfo.szAccount;
		std::map<std::string, SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bFund =true;	
		}
		else
		{
			SubscribeType st;
			ZeroMemory(&st,sizeof(st));
			st.AccID=AccID;
			st.bFund = true;
			m_SubscribeInfo[strAcc]=st;
		}
	}
	m_Mutex_SubscribeInfo.write_unlock();	
}
void CDataCenter::UnSubscribeFund(int AccID)
{
	UserInfo userInfo;
	std::string strAcc;
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);                                //GetTradeAccountID(AccID);
	if(bFind)
	{
		strAcc = userInfo.szAccount;
		std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		m_Mutex_SubscribeInfo.write_lock();
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bFund=false;	
		}
		m_Mutex_SubscribeInfo.write_unlock();
	}
}
void CDataCenter::SubscribePosition(int AccID)
{
	m_Mutex_SubscribeInfo.write_lock();
	UserInfo userInfo;	
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);  
	if(bFind)
	{
		std::string strAcc = userInfo.szAccount;
		std::map<std::string, SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bPosition =true;	
		}
		else
		{
			SubscribeType st;
			ZeroMemory(&st,sizeof(st));
			st.AccID=AccID;
			st.bPosition = true;
			m_SubscribeInfo[strAcc]=st;
		}
	}
	m_Mutex_SubscribeInfo.write_unlock();	
}
void CDataCenter::UnSubscribePosition(int AccID)
{
	UserInfo userInfo;
	std::string strAcc;
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);                                //GetTradeAccountID(AccID);
	if(bFind)
	{
		strAcc = userInfo.szAccount;
		std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		m_Mutex_SubscribeInfo.write_lock();
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bPosition=false;	
		}
		m_Mutex_SubscribeInfo.write_unlock();
	}
		
}
void CDataCenter::SubscribeOrder(int AccID)
{
	m_Mutex_SubscribeInfo.write_lock();
	UserInfo userInfo;	
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);  
	if(bFind)
	{
		std::string strAcc = userInfo.szAccount;
		std::map<std::string, SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bOrder =true;	
		}
		else
		{
			SubscribeType st;
			ZeroMemory(&st,sizeof(st));
			st.AccID=AccID;
			st.bOrder = true;
			m_SubscribeInfo[strAcc]=st;
		}
	}
	m_Mutex_SubscribeInfo.write_unlock();	
}
void CDataCenter::UnSubscribeOrder(int AccID)
{
	UserInfo userInfo;
	std::string strAcc;
	bool bFind = CInterface_SvrUserOrg::getObj().GetUserByID(AccID, userInfo);                                //GetTradeAccountID(AccID);
	if(bFind)
	{
		strAcc = userInfo.szAccount;
		std::map<std::string,SubscribeType>::iterator it=m_SubscribeInfo.find(strAcc);
		m_Mutex_SubscribeInfo.write_lock();
		if(it!=m_SubscribeInfo.end())
		{
			SubscribeType& st=it->second;
			st.bOrder=false;	
		}
		m_Mutex_SubscribeInfo.write_unlock();
	}

}
//获得监控部门的单笔持仓时间风险风控配置
bool CDataCenter::GetPosiDetailWarning(int nOrgID, std::string strTime, SResponse& retWarning, std::string& strTimeBegin, std::string& strTimeEnd)
{
	bool bFound = false;
	RiskKey rKey;

	rKey.nOrgID = nOrgID;				//组织机构ID
	rKey.RiskType = RI_SingleHoldTime;	//风险类型

	m_Mutex_RiskWarning.write_lock();

	do {
			
		    if(GetResponseOfRiskWarning(rKey, strTime, retWarning))
			{			
				std::map<int,RiskPlan>::iterator itRiskPlan = retWarning.mapResponse.begin();
				if(itRiskPlan != retWarning.mapResponse.end())
				{
					strTimeBegin = itRiskPlan->second.OrgIDPlanRelation.cTimeBegin;
					strTimeEnd   = itRiskPlan->second.OrgIDPlanRelation.cTimeEnd;
				}
				
				bFound = true;
				break;
			}
		/*	RISKWARNINGSTRUCT::iterator it = m_RiskWarning.find(rKey);
			if(it != m_RiskWarning.end()) 
			{
				retWarning = it->second;
				bFound = true;
				break;
			}*/
			else 
			{
				Organization org;
				if(!CInterface_SvrUserOrg::getObj().GetOrgByID(rKey.nOrgID, org))
					break;	//如果没有父级，则查找失败
				rKey.nOrgID = org.nUpperLevelOrgID;	//从父级里面查找
			}
	} while(true);

	m_Mutex_RiskWarning.write_unlock();

	return bFound;
}
void CDataCenter::AddResponse(std::string strInvestor, RiskIndicatorType nRiskIndicatorID, std::string strInstrument, PositionDetailKey*  PostionDetail, SResponse& sResponse, bool bNewRiskLevel)
{
	m_Mutex_AccountRisk.write_lock();
	std::map<string, std::map<SRiskKey, SResponse>>::iterator it = m_mapAccountRisk.find(strInvestor);
	if(it != m_mapAccountRisk.end())
	{//得到每个账户下的所有风险动作	
		std::map<SRiskKey, SResponse>&  mapKey = it->second;
		PositionDetailKey PDetail;
		if(PostionDetail)
			PDetail = *PostionDetail;
		SRiskKey key(nRiskIndicatorID, strInstrument, PDetail);
		std::map<SRiskKey, SResponse>::iterator itKey = mapKey.find(key);
		
		if(bNewRiskLevel)
		{
			mapKey[key] = sResponse;
		}
		else
		{
			if(itKey != mapKey.end())
			{
				mapKey.erase(itKey);				
			}
		}		
	}
	else
	{
		if(bNewRiskLevel)
		{
			PositionDetailKey PDetail;
			if(PostionDetail)
				PDetail = *PostionDetail;
			SRiskKey key(nRiskIndicatorID, strInstrument, PDetail);
			std::map<SRiskKey, SResponse> mapKey;
			mapKey[key] = sResponse;
			m_mapAccountRisk[strInvestor] = mapKey;			
		}
	}
	m_Mutex_AccountRisk.write_unlock();
}
void CDataCenter::GetAccountRisk(std::map<string, std::map<SRiskKey, SResponse>>& mapAccountRisk)
{
	m_Mutex_AccountRisk.write_lock();
	mapAccountRisk = m_mapAccountRisk;
	m_Mutex_AccountRisk.write_unlock();
}
bool CDataCenter::EraseAccountRisk(std::string  strAccount)
{	
	m_Mutex_AccountRisk.write_lock();
	m_mapAccountRisk.erase(strAccount);
	m_Mutex_AccountRisk.write_unlock();
	return true;
}

void CDataCenter::AddAccountForbid(std::string strInvestor, RiskIndicatorType nRiskIndicatorID, std::string strInstrument, PositionDetailKey*  PostionDetail, SResponse& sResponse, bool bNewRiskLevel)
{
	m_Mutex_AccountForbid.write_lock();
	std::map<string, std::map<SRiskKey, SResponse>>::iterator it = m_mapAccountForbid.find(strInvestor);
	if(it != m_mapAccountForbid.end())
	{//得到每个账户下的所有风险动作	
		std::map<SRiskKey, SResponse>&  mapKey = it->second;

		PositionDetailKey PDetail;
		if(PostionDetail)
			PDetail = *PostionDetail;
		SRiskKey key(nRiskIndicatorID, strInstrument, PDetail);
		std::map<SRiskKey, SResponse>::iterator itKey = mapKey.find(key);

		if(bNewRiskLevel)
		{
			mapKey[key] = sResponse;
		}
		else
		{
			if(itKey != mapKey.end())
			{
				mapKey.erase(itKey);				
			}
		}		
	}
	else
	{
		if(bNewRiskLevel)
		{
			PositionDetailKey PDetail;
			if(PostionDetail)
				PDetail = *PostionDetail;
			SRiskKey key(nRiskIndicatorID, strInstrument, PDetail);
			std::map<SRiskKey, SResponse> mapKey;
			mapKey[key] = sResponse;
			m_mapAccountForbid[strInvestor] = mapKey;			
		}
	}
	m_Mutex_AccountForbid.write_unlock();
}
void CDataCenter::GetAccountForbid(std::map<string, std::map<SRiskKey, SResponse>>& mapAccountRisk)
{
	m_Mutex_AccountForbid.write_lock();
	mapAccountRisk = m_mapAccountForbid;
	m_Mutex_AccountForbid.write_unlock();
}
bool CDataCenter::EraseAccountForbid(std::string  strAccount)
{	
	m_Mutex_AccountForbid.write_lock();
	m_mapAccountForbid.erase(strAccount);
	m_Mutex_AccountForbid.write_unlock();
	return true;
}
bool CDataCenter::GetResponseOfRiskWarning(RiskKey& rKey, std::string strTime, SResponse& warnings)
{
	bool bFind = false;	
	m_Mutex_RiskWarning.write_lock();
	int nCount = m_RiskWarning.count(rKey);
	if(nCount == 0)
	{
		m_Mutex_RiskWarning.write_unlock();
		return false;
	}
	RISKWARNINGSTRUCT::iterator it = m_RiskWarning.find(rKey);
	for(int k =0; k < nCount; k++)
	{
		SResponse temp = it->second;
		std::map<int,RiskPlan>::iterator itRiskPlan = temp.mapResponse.begin();
		if(itRiskPlan != temp.mapResponse.end())
		{
			std::string strBegin = itRiskPlan->second.OrgIDPlanRelation.cTimeBegin;
			std::string strEnd   = itRiskPlan->second.OrgIDPlanRelation.cTimeEnd;

			if(strcmp(strBegin.c_str(), strTime.c_str()) <= 0 
				&& strcmp(strEnd.c_str(), strTime.c_str()) >= 0)
			{
				warnings = it->second;
				bFind = true;
				break;
			}
		}
		it++;
	}
	m_Mutex_RiskWarning.write_unlock();
	if(!bFind)
		return false;	

	return true;
}
int CDataCenter::GetTradeForbidMapLevel(RiskKey& rKey, std::string strTime, std::string InstrumentID, double value,  SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd)
{
	SResponse warnings;
	if(!GetResponseOfRiskWarning(rKey, strTime, warnings))
		return 0;

	std::set<std::string>   setInstruments = warnings.setInstruments;
	std::set<std::string>::iterator lIterSet = setInstruments.begin();
	while(lIterSet != setInstruments.end())
	{
		if(*lIterSet == InstrumentID)
		{
			LevelID = 1;
			std::map<int,RiskPlan>::const_iterator coreit = warnings.mapResponse.begin();
			if(coreit != warnings.mapResponse.end())
				sResponse.mapResponse[LevelID] = coreit->second;
			sResponse.setProducts.insert(InstrumentID);//针对具体某个合约的风险
			strTimeBegin = coreit->second.OrgIDPlanRelation.cTimeBegin;
			strTimeEnd   = coreit->second.OrgIDPlanRelation.cTimeEnd;
			return 1;
		}				
		lIterSet++;
	}

	std::string strProduct;
	PlatformStru_InstrumentInfo info;	
	if(GetInstrumentInfo_DataCenter(InstrumentID,info) != CF_ERROR_SUCCESS)
		return -1;
	strProduct = info.ProductID;

	std::set<std::string>   setProducts = warnings.setProducts;
	std::set<std::string>::iterator lSet = setProducts.begin();
	while(lSet != setProducts.end())
	{
		if(*lSet == strProduct)
		{
			LevelID = 1;
			std::map<int,RiskPlan>::const_iterator coreit = warnings.mapResponse.begin();
			if(coreit != warnings.mapResponse.end())
				sResponse.mapResponse[LevelID] = coreit->second;
			sResponse.setProducts.insert(InstrumentID);//针对具体某个合约的风险

			strTimeBegin = coreit->second.OrgIDPlanRelation.cTimeBegin;
			strTimeEnd   = coreit->second.OrgIDPlanRelation.cTimeEnd;
			return 1;
		}				
		lSet++;
	}
	return -1;//没有风险
}
int CDataCenter::GetMarginForbidMapLevel(RiskKey& rKey, std::string strTime, double value, double dbMargin, SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd)
{
	SResponse warnings;
	if(!GetResponseOfRiskWarning(rKey, strTime, warnings))
		return 0;

	std::map<int,RiskPlan>::const_iterator coreit;
	for (coreit=warnings.mapResponse.begin();coreit!=warnings.mapResponse.end();++coreit)
	{
		const RiskPlan& risk=coreit->second;
		if(value > risk.OrgIDPlanRelation.dbMarginDividDynamic -0.00001 
			|| dbMargin >risk.OrgIDPlanRelation.dbMarginUse - 0.00001)//减去0.00001主要是为了防止相等时候不触发风险
		{
			LevelID=coreit->first;				
			sResponse.mapResponse[LevelID] = risk;	
			strTimeBegin = risk.OrgIDPlanRelation.cTimeBegin;
			strTimeEnd   = risk.OrgIDPlanRelation.cTimeEnd;
			return 1;//有风险
		}		
	}	
	return -1;//没有风险
}
int  CDataCenter::GetLossForbidMapLevel(RiskKey& rKey, std::string strTime, double value, double dbLoss, SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd)
{
	SResponse warnings;
	if(!GetResponseOfRiskWarning(rKey, strTime, warnings))
		return 0;

	std::map<int,RiskPlan>::const_iterator coreit;
	for (coreit=warnings.mapResponse.begin();coreit!=warnings.mapResponse.end();++coreit)
	{
		const RiskPlan& risk=coreit->second;
		if(value > risk.OrgIDPlanRelation.dbLossPercent -0.00001 
			|| dbLoss > risk.OrgIDPlanRelation.dbLossAmount - 0.00001)//减去0.00001主要是为了防止相等时候不触发风险
		{
			LevelID=coreit->first;				
			sResponse.mapResponse[LevelID] = risk;	
			strTimeBegin = risk.OrgIDPlanRelation.cTimeBegin;
			strTimeEnd   = risk.OrgIDPlanRelation.cTimeEnd;
			return 1;//有风险
		}		
	}	
	return -1;//没有风险
}
int CDataCenter::GetLossPercentMapLevel(RiskKey& rKey, std::string strTime, double dynamic, double dbInit, SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd)
{
	SResponse warnings;
	if(!GetResponseOfRiskWarning(rKey, strTime, warnings))
		return 0;

	std::map<int,RiskPlan>::const_iterator coreit;
	for (coreit=warnings.mapResponse.begin();coreit!=warnings.mapResponse.end();++coreit)
	{
		const RiskPlan& risk=coreit->second;
		double value = (dbInit- dynamic)/(dbInit*risk.OrgIDPlanRelation.dbLossPercent);
		if(value > risk.WaringLevel.fThresholdValue -0.00001)//减去0.00001主要是为了防止相等时候不触发风险
		{
			LevelID=coreit->first;				
			sResponse.mapResponse[LevelID] = risk;	
			strTimeBegin = risk.OrgIDPlanRelation.cTimeBegin;
			strTimeEnd   = risk.OrgIDPlanRelation.cTimeEnd;
			return 1;//有风险
		}		
	}
	return -1;//没有风险
}