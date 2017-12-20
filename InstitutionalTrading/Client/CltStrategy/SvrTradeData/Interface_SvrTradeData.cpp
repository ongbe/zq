// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Interface_SvrTradeData.h"
#include "Real_SvrTradeData.h"


CInterface_SvrTradeData& CInterface_SvrTradeData::getObj(void)
{
	return (CInterface_SvrTradeData&)CReal_SvrTradeData::getObj();
}


//模块初始化，所需的参数（用户，委托交易账号，经纪公司，初始资金，Redis对象指针,）
void CInterface_SvrTradeData::InitFunc(string szUser,string szBrokerCode,string szTradeAccount,
		CDataEntity* pDataEntity, CMemCache* pCmdCache, double dFund )
{

    CReal_SvrTradeData::getObj().SlaverInitAfterGetTradingDay(szUser,szBrokerCode,szTradeAccount,pDataEntity, pCmdCache, dFund);
/*
	// 初始化交易数据管理
	CReal_SvrTradeData::getObj().TradeDataInit(pCmdCache,szUser,szBrokerCode,szTradeAccount,false);



	//账户的链接CTP的状态
	CReal_SvrTradeData::getObj().SetAccountStatus(szUser,szBrokerCode,ACCOUNT_STATUS_Connected);

	// 设置当前交易日
	std::string szTradingDay = "20140613";
	CReal_SvrTradeData::getObj().SetCurrentTradingDay(szTradingDay);

	// 设置合约信息
	std::map<std::string, PlatformStru_InstrumentInfo>::const_iterator itInstr;
	const std::map<std::string, PlatformStru_InstrumentInfo>& mapInstrument
		= pDataEntity->GetInstrument();
	for(itInstr = mapInstrument.begin(); itInstr != mapInstrument.end(); itInstr++) {
		CReal_SvrTradeData::getObj().SetInstrumentInfo((PlatformStru_InstrumentInfo&)itInstr->second);
	}

	CReal_SvrTradeData::getObj().EndQryInstrument();

	// 设置保证金率信息
	std::map<std::string, PlatformStru_InstrumentMarginRate>::iterator itMG;
	std::map<std::string, PlatformStru_InstrumentMarginRate> mapMG
		= pDataEntity->GetMarginRate();
	for(itMG = mapMG.begin(); itMG != mapMG.end(); itMG++) {
		strncpy(itMG->second.BrokerID, szBrokerCode.c_str(), sizeof(itMG->second.BrokerID)-1);
		strncpy(itMG->second.InvestorID, szTradeAccount.c_str(), sizeof(itMG->second.InvestorID)-1);
		CReal_SvrTradeData::getObj().SetMarginRate((PlatformStru_InstrumentMarginRate&)itMG->second);
	}
	CReal_SvrTradeData::getObj().EndUserQryMargin(szBrokerCode, szTradeAccount);   

	// 设置手续费率信息
	std::map<std::string, PlatformStru_InstrumentCommissionRate>::iterator itCS;
	std::map<std::string, PlatformStru_InstrumentCommissionRate> mapCS
		= pDataEntity->GetCommissionRate();
	for(itCS = mapCS.begin(); itCS != mapCS.end(); itCS++) {
		strncpy(itCS->second.BrokerID, szBrokerCode.c_str(), sizeof(itCS->second.BrokerID)-1);
		strncpy(itCS->second.InvestorID, szTradeAccount.c_str(), sizeof(itCS->second.InvestorID)-1);
		CReal_SvrTradeData::getObj().SetCommissionRate((PlatformStru_InstrumentCommissionRate&)itCS->second);
	}

	CReal_SvrTradeData::getObj().EndUserQryCommission(szBrokerCode, szTradeAccount ,szUser); 

    //加载日出数据(mUserIniFundInfo mUserInitPosition mUserInitPositionDeatil)	
    //资金
	PlatformStru_TradingAccountInfo* inData = new PlatformStru_TradingAccountInfo;
    memset(inData,0,sizeof(PlatformStru_TradingAccountInfo));
    strcpy(inData->BrokerID,szBrokerCode.c_str());
    strcpy(inData->AccountID,szTradeAccount.c_str());
    inData->Available = 888888888888.0f;
    inData->DynamicProfit = 888888888888.0f;
    inData->StaticProfit = 888888888888.0f;
    CReal_SvrTradeData::getObj().LoadInitData(*inData,szTradingDay,szUser);


	CReal_SvrTradeData::getObj().QryFundReach(*inData,szUser);//SetUserInitFund(dFund);

	CReal_SvrTradeData::getObj().SetTraderDataStatus(TradeDataStatusTrading);
    */
}
 
//模块结束，释放资源，关闭工作线程
void CInterface_SvrTradeData::ReleaseFunc(void)
{ 
}