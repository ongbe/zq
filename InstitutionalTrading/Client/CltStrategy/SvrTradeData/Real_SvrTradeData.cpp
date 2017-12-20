// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#define WRITELOGID


#include "Real_SvrTradeData.h"
#include "SvrTradeData.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "FactoryAccountOp.h"
#include "FactoryUserOp.h"
#include "SvrTradeData.h"
#include "CommonMacro.h"
#include "SafeLock.h"
#include "SafeRwLock.h"
//#include "..\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"
//#include "..\SvrUserOrg\Interface_SvrUserOrg.h"
//#include "..\SvrBrokerInfo\Interface_SvrBrokerInfo.h"
//#include "..\SvrLogin\Interface_SvrLogin.h"
//#include "..\SvrTcp\Interface_SvrTcp.h"
//#include "Real_DBOpr.h"
#include "..\StrategyTester\tinyxml\tinyxml.h"
#include "MyXML.h"

#define  TRADEDATA_KEY         "CFG"
#define  TRADEDATA_CONFIG_FILE "TradeDataCfg.txt"

CReal_SvrTradeData* CReal_SvrTradeData::m_pObj=NULL;

struct  CTraderSorter
{
public:
	// Return whether first element is less than the second   
	bool operator () (const PlatformStru_TradeInfo a,const PlatformStru_TradeInfo b) const  
	{   
		return strncmp(a.TradeTime,b.TradeTime,sizeof(TThostFtdcTimeType)) < 0;   
	}; 
}TraderSorterObject;


void  BufferStoreCallback()
{
	//PostThreadMessage(g_idThread,TMESSAGE_TRADING_BUFFER_STORE,0,0);
}
CReal_SvrTradeData& CReal_SvrTradeData::getObj(void)
{
	if(!m_pObj)
		m_pObj=new CReal_SvrTradeData();
	return *m_pObj;
}

CReal_SvrTradeData::CReal_SvrTradeData()
{
	mpPlatformDataInstrument = NULL;
	mpPlatformDataQuots = NULL;

	//mpDBOpr = new CRealDBOpr();

	meTraderDataStatus = TradeDataStatusInitialing;

	mpPlatformDataInstrument = new CPlatformDataMgr_Instruments();
	mpPlatformDataQuots = new CPlatformDataMgr_Quots(*mpPlatformDataInstrument);

	meExchangeStatus = ExchangeStatus_PreOpen;
	uv__rwlock_fallback_init(&mrwLockMapAccount);
	uv__rwlock_fallback_init(&mrwLockMapUserTrading);

	mbDirty = false;

	//m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"TradeData.log");

	mMasterSocket = 0;

	//mpTradingDayManager.SetNeedSaveTODB(!CInterface_SvrLogin::getObj().IsSlaver());
	mbNeedCache = true;
    mpCache = NULL;
}

CReal_SvrTradeData::~CReal_SvrTradeData()
{
	ResetAll();

	if(mpPlatformDataInstrument)
		delete mpPlatformDataInstrument;
	if(mpPlatformDataQuots)
		delete mpPlatformDataQuots;

    uv__rwlock_fallback_destroy(&mrwLockMapUserTrading);
	uv__rwlock_fallback_destroy(&mrwLockMapAccount);

	/*if(m_pWriteLog!=NULL)
	{
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}*/

}
//获取交易数据管理模块状态
eExchangeStatus CReal_SvrTradeData::GetExchangeStatus() 
{
	eExchangeStatus leRet = ExchangeStatus_PreOpen;
	InterlockedExchange((volatile long*)(&leRet),meExchangeStatus);
	return leRet;
}
//获取交易数据管理模块状态
eTradeDataStatus CReal_SvrTradeData::GetTraderDataStatus()
{
	eTradeDataStatus leRet = TradeDataStatusInitialing;
	InterlockedExchange((volatile long*)(&leRet),meTraderDataStatus);
	return leRet;
}



//设置交易数据管理模块状态
void CReal_SvrTradeData::SetTraderDataStatus(eTradeDataStatus neStatus) 
{
	InterlockedExchange(reinterpret_cast<volatile long*>(&meTraderDataStatus),neStatus);
}
//设置交易所状态
void CReal_SvrTradeData::SetIntrumentsStatus(const CThostFtdcInstrumentStatusField & nStatus)
{

	//TRADE_LOG("Instrument status is %c",nStatus.InstrumentStatus);

	if(nStatus.InstrumentStatus == THOST_FTDC_IS_Continous ||
		nStatus.InstrumentStatus == THOST_FTDC_IS_AuctionOrdering ||
		nStatus.InstrumentStatus == THOST_FTDC_IS_AuctionBalance ||
		nStatus.InstrumentStatus == THOST_FTDC_IS_AuctionMatch)
	{
		TRADE_LOG("Instrument can order ");
		InterlockedExchange((volatile long*)(&meExchangeStatus),ExchangeStatus_ContinueTrade);
		
		//添加一个交易日
		//mpTradingDayManager.AddTradingDay(msCurrentTradingDay);
	}
	if(nStatus.InstrumentStatus == THOST_FTDC_IS_NoTrading ||
		nStatus.InstrumentStatus == THOST_FTDC_IS_Closed)
	{
		TRADE_LOG("Instrument status change to notrading");
		InterlockedExchange((volatile long*)(&meExchangeStatus),ExchangeStatus_UnableTrade);
	}

	return;
}

//判断是否是交易日
//字符串为日期格式2013-08-09
bool  CReal_SvrTradeData::IsTradingDay(const std::string & nsDay)
{
	return false/*mpTradingDayManager.IsTradingDay(nsDay)*/;
}


//判断交易日是否结算
//字符串为日期格式2013-08-09
bool  CReal_SvrTradeData::IsSettlemented(const std::string & nsDay) 
{
	return false/*mpTradingDayManager.IsSetttemnted(nsDay)*/;
}


//获取一个日期的上一个交易日
bool CReal_SvrTradeData::GetLastTrading(const std::string & nsDay, std::string & nsLastDay)
{
	return false/*mpTradingDayManager.GetLastTradingData(nsDay,nsLastDay)*/;
}
//设置当前交易日
void CReal_SvrTradeData::SetCurrentTradingDay(const std::string & nsCurrentTradingDay,bool nbSetByMySelf) 
{
	if(GetTraderDataStatus() != TradeDataStatusInitialing && GetTraderDataStatus() != TradeDataStatusSettlemtEnd)
		return;

	if(nsCurrentTradingDay.empty())
		return;

	CSafeLock lLock(mMutexTradingday);
	if(nbSetByMySelf)
	{
		msCurrentTradingDay = nsCurrentTradingDay;
		//return;
	}
	else
	{
		std::string lsTemp = nsCurrentTradingDay.substr(0,4) + "-" + nsCurrentTradingDay.substr(4,2) + "-" + nsCurrentTradingDay.substr(6,2);

		if(msCurrentTradingDay == lsTemp)
			return;

		msCurrentTradingDay = lsTemp;
	}
	
	
	//添加一个交易日
	//mpTradingDayManager.AddTradingDay(msCurrentTradingDay);

	//设置合约对象的交易日
	ExchangeTime lTime;
	lTime.CurTickCount = GetTickCount();
	std::string lsTempTradingDay = nsCurrentTradingDay.substr(0,4) + nsCurrentTradingDay.substr(5,2) + nsCurrentTradingDay.substr(8,2);
	//std::string lsTempTradingDay = nsCurrentTradingDay;
	strcpy(lTime.TradingDay,lsTempTradingDay.c_str());
	mpPlatformDataInstrument->SetExchangeTime(lTime);

	//发起获取交易日后的交易初始化
	//PostThreadMessage(g_idThread,TMESSAGE_TRADEINIT_AFTER_GET_TRADINGDAY,0,0);
	
}

//获取当前交易日
void CReal_SvrTradeData::GetCurrentTradingDay(std::string & nsCurrentTradingDay) 
{
	CSafeLock lLock(mMutexTradingday);
	nsCurrentTradingDay = msCurrentTradingDay;
}

//登陆和初始化一个委托交易账号
CF_ERROR CReal_SvrTradeData::LoginAdnInitTraderAccount(const BrokerAccountKey & nKey,int nAccountID)
{
	CSafeRwLockWrite lLock(mrwLockMapAccount);
	_MapAccountData::iterator lIterAccount = mMapAccountData.find(nKey);
	if(lIterAccount == mMapAccountData.end())
	{		
		//插入一条
		IInterfaceAccountOp * lpTemp =  IFactoryAccountOp::GetFactory()->CreateAccountOp();
		lpTemp->SetPlatFormInstrument(mpPlatformDataInstrument);
		lpTemp->SetPlatFormQutos(mpPlatformDataQuots);
		mMapAccountData[nKey] = lpTemp;
	}


	if(mMapAccountData[nKey]->GetCommissonMarginReady() == CommissionMargin_ComissionAndMarinReady)
	{
		TRADE_LOG("Has Found %s commission,we need check ctp status ",nKey.AccountID);

		double ldbTime ;
		if(mMapAccountData[nKey]->GetAccountCTPStatus(ldbTime) != ACCOUNT_STATUS_Login)
		{
			TRADE_LOG(" %s ctp status is not login,we need relogin ",nKey.AccountID);
			//通知登陆查询费率
			Stru_NotifyEvent lDataEvent(EventTypeNeedSingleLoginTrade,0);		
			lDataEvent.AddBufer((void*)&nAccountID,sizeof(int));	
			//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);

		}
		return CF_ERROR_SUCCESS;
	}
	TRADE_LOG("Has not Found %s commission,notify offer to login ",nKey.AccountID);

	//通知登陆查询费率
	Stru_NotifyEvent lDataEvent(EventTypeNeedSingleLoginTrade,0);		
	lDataEvent.AddBufer((void*)&nAccountID,sizeof(int));	
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);

	//产生一个需要查询合约费率的事件
	Stru_NotifyEvent lDataEvent1(EventTypeNeedQryCommission,0);		
	lDataEvent1.AddBufer((void*)&nKey,sizeof(nKey));	
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent1);

	//产生一个需要查询报证金率的事件
	Stru_NotifyEvent lDataEvent2(EventTypeNeedQryMargin,0);	
	lDataEvent2.AddBufer((void*)&nKey,sizeof(nKey));	
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent2);
	return CF_ERROR_SUCCESS;
}
//报盘可以工作了
void CReal_SvrTradeData::TradeDataInit(CMemCache* pCache,string szUser,string szBrokerCode,string szTradeAccount,bool nbForce ) 
{
	//mbForceInit = nbForce;
 //   mpCache = pCache;

	//TRADE_LOG("TradeDataInit with %s",mbForceInit ? "force" : "noforce");

 //   //清理内存
	//ResetAll();

	//msCurrentTradingDay.clear();
	//msInstrumentDay.clear();

	////为每个用户绑定一个空的用户对象
	//InitUserMapWithEmpty(szUser,szBrokerCode,szTradeAccount,mMapUser2TradingData);  
}

//报盘设置每个账户的链接CTP的状态
void CReal_SvrTradeData::SetAccountStatus(const std::string & nsAccoutnID,
										  const std::string & nsBrokerID,
							  const EnumAccountStatus& neUserStatu) 
{

	//if(GetTraderDataStatus() != TradeDataStatusInitialing 
	//	&& GetTraderDataStatus() != TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("not Init Status,discard this call");
	//	return ;
	//}
	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nsAccoutnID.c_str());
	_MapAccountData::iterator lIterAccount = mMapAccountData.find(lKey);
	if(lIterAccount != mMapAccountData.end())
	{
		lIterAccount->second->SetAccountCTPStatus(neUserStatu);
	}


	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	vector<UserInfo> lVecUser;
// 	CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nsAccoutnID,nsBrokerID,lVecUser);	
// 	vector<UserInfo>::iterator lIter = lVecUser.begin();
// 	for(;lIter != lVecUser.end();lIter++)
// 	{	
// 		//CSafeLock ls(m_mutexMapUserTrading);
 		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsAccoutnID);
 		if(lIterUser != mMapUser2TradingData.end())
 			lIterUser->second->SetUserCTPStatus(neUserStatu);		
// 	}
	
}


//获取每个账户的ctp状态
void CReal_SvrTradeData::GetAccountStatus(const std::string & nsAccoutnID,
							  const std::string & nsBrokerID,
							  EnumAccountStatus& neCTPStatus,
							  double & nStatusTime)
{
	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nsAccoutnID.c_str());
	_MapAccountData::iterator lIterAccount = mMapAccountData.find(lKey);
	if(lIterAccount != mMapAccountData.end())
	{		
		neCTPStatus = lIterAccount->second->GetAccountCTPStatus(nStatusTime);
	}
}

//获取所有委托交易账号的状态
void CReal_SvrTradeData::GetAllAccountStatus(std::vector<sAccountStatus> & nVecStatus)
{
	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);	
	_MapAccountData::iterator lIterAccount = mMapAccountData.begin();
	for(;lIterAccount != mMapAccountData.end();lIterAccount++)
	{		
		sAccountStatus lsStatus;
		lsStatus.mBrokerAccount = lIterAccount->first;	
		double ldbTime = 0.0f;
		EnumAccountStatus leCTPStatus = lIterAccount->second->GetAccountCTPStatus(ldbTime);
		lsStatus.meStatus = leCTPStatus;
		lsStatus.ldbDuration = ldbTime;
		lsStatus.meCommissionAndMarginReady = lIterAccount->second->GetCommissonMarginReady();
		nVecStatus.push_back(lsStatus);
	}
}

//获取所有用户的初始化状态
void CReal_SvrTradeData::GetAlUserInitStatus(std::vector<sUserInitStatus> & lVecStatus)
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.begin();
	for(;lIterUser != mMapUser2TradingData.end();lIterUser++)
	{
		sUserInitStatus ltemp;
		strcpy_s(ltemp.szUserAccount,lIterUser->first.c_str());
		ltemp.meStatus =  lIterUser->second->GetUserInitStatus();
		ltemp.meCTPStatus =  lIterUser->second->GetUserCTPStatus(ltemp.ldbDuration);
		lVecStatus.push_back(ltemp);
	}

}

bool CReal_SvrTradeData::IsUserSettlementConfirmed(const std::string & nsUserName,
														std::string & nsConfirmedDate,
														std::string & nsConfirmedTime) 
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	nsConfirmedDate.clear();
	nsConfirmedTime.clear();

	do 
	{		
// 		if(!msCurrentTradingDay.empty())
// 			mpDBOpr->LoadUserSettlementConfirm(msCurrentTradingDay,nsUserName,nsConfirmedDate,nsConfirmedTime);
	/*	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsUserName);
		if(lIterUser != mMapUser2TradingData.end())
			lbRet =  lIterUser->second->IsSettlementComfirmed(nsConfirmedDate,nsConfirmedTime);*/

		break;;

	} while (false);

	return !nsConfirmedDate.empty();
}


//用户确认了结算单
void CReal_SvrTradeData::SetUserSettlementConfirmed(const std::string & nsUserName,
										const std::string & nsConfirmedDate,
										const std::string & nsConfirmedTime) 
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
// 	if(!msCurrentTradingDay.empty())
// 		mpDBOpr->AddUserSettlementConfirm(msCurrentTradingDay,nsUserName,nsConfirmedDate,nsConfirmedTime);
	//_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsUserName);
	//if(lIterUser != mMapUser2TradingData.end())
	//	lIterUser->second->SetSettlementConfirmed(true,nsConfirmedDate,nsConfirmedTime);	
	
}

//获取用户的ctp链接状态
EnumAccountStatus  CReal_SvrTradeData::GetUserCTPStatus(int nUserID,double & nDifftime) 
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
	{
		TRADE_LOG("Init Status,discard this call");
		return ACCOUNT_STATUS_UnInit;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	EnumAccountStatus leRet = ACCOUNT_STATUS_UnInit;
	do 
	{
		UserInfo lUser;
// 		if(false == CInterface_SvrUserOrg::getObj().GetUserByID(nUserID,lUser))
// 			break;
// 
// 		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(lUser.szAccount);
// 		if(lIterUser != mMapUser2TradingData.end())
// 			leRet =  lIterUser->second->GetUserCTPStatus(nDifftime);

	} while (false);

	return leRet;
	
}
//获取用户的ctp链接状态
eUserInitStatus  CReal_SvrTradeData::GetUserInitStatus(int nUserID) 
{
	
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
	{
		TRADE_LOG("Init Status,discard this call");
		return USER_STATUS_UnInit;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	eUserInitStatus leRet = USER_STATUS_UnInit;
	do 
	{
// 		UserInfo lUser;
// 		if(false == CInterface_SvrUserOrg::getObj().GetUserByID(nUserID,lUser))
// 			break;
// 
// 		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(lUser.szAccount);
// 		if(lIterUser != mMapUser2TradingData.end())
// 			leRet =  lIterUser->second->GetUserInitStatus();
	
	} while (false);

	return leRet;
}



//获取用户的ctp链接状态
EnumAccountStatus CReal_SvrTradeData::GetUserCTPStatus(const std::string& nsUser,double & nDifftime) 
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
	{
		TRADE_LOG("Init Status,discard this call");
		return ACCOUNT_STATUS_UnInit;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	EnumAccountStatus leRet = ACCOUNT_STATUS_UnInit;
	do 
	{
		
		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsUser);
		if(lIterUser != mMapUser2TradingData.end())
			leRet =  lIterUser->second->GetUserCTPStatus(nDifftime);

	} while (false);

	return leRet;
}

//获取用户的初始化状态
eUserInitStatus CReal_SvrTradeData::GetUserInitStatus(const std::string& nsUser) 
{
	
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
	{
		TRADE_LOG("Init Status,discard this call");
		return USER_STATUS_UnInit;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	eUserInitStatus leRet = USER_STATUS_UnInit;
	do 
	{		
		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nsUser);
		if(lIterUser != mMapUser2TradingData.end())
			leRet =  lIterUser->second->GetUserInitStatus();

	} while (false);

	return leRet;
}

//获取交易日
void CReal_SvrTradeData::GetExchangeData(ExchangeTime & nData) 
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing || 
		GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK||
		mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return ;
	}*/

	return;
}

///获取合约列表
int CReal_SvrTradeData::GetInstrumentList(std::set<std::string> &outData,
										  std::string ExchangeID)
{
	//CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}

	mpPlatformDataInstrument->GetInstrumentList(outData,ExchangeID);
	return outData.size();
}
//添加主力合约列表
void CReal_SvrTradeData::AddMainInstrumentList(std::string instrument)
{
	//CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return ;
	}
	mpPlatformDataInstrument->AddMainInstrumentList(instrument);
}
///获取主力合约列表
int CReal_SvrTradeData::GetMainInstrumentList(std::set<std::string> &outData)
{
	//CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	return mpPlatformDataInstrument->GetMainInstrumentList(outData);
}
///设置合约容差列表
void CReal_SvrTradeData::SetInstrumentVarietyMap(map<string, string>& inData)
{
	//CSafeLock lLock(m_mutex);
	if(mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return ;
	}
	mpPlatformDataInstrument->SetInstrumentVarietyMap(inData);
}

// 获取合约容差列表
int CReal_SvrTradeData::GetInstrumentVarietyMap(map<string, string>& outData)
{
	//CSafeLock lLock(m_mutex);
	if(mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	mpPlatformDataInstrument->GetInstrumentVarietyMap(outData);
	return outData.size();
}

///获取指定合约信息
int CReal_SvrTradeData::GetInstrumentInfo(const std::string& InstrumentID, 
										  PlatformStru_InstrumentInfo& outData)
{
	//CSafeLock lLock(m_mutex);
	if(mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	bool brlt=mpPlatformDataInstrument->GetInstrumentInfo(InstrumentID,outData);
	return brlt?0:-1;
}

///设置指定合约信息
void CReal_SvrTradeData::SetInstrumentInfo(PlatformStru_InstrumentInfo& nInstrumentInfo)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Not Trading Status,discard this call");
		return ;
	}
	TRADE_LOG("%s",nInstrumentInfo.InstrumentID);
	mpPlatformDataInstrument->SetInstrumentInfo(nInstrumentInfo.InstrumentID,nInstrumentInfo,false);
}

///获取合约的产品类型，失败返回-1
char CReal_SvrTradeData::GetProductClassType(const std::string& InstrumentID)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	return mpPlatformDataInstrument->GetProductClassType(InstrumentID);
}

///获取包含此单腿合约的组合合约列表
bool CReal_SvrTradeData::GetCombInstrumentIDs_IncludeLeg(const std::string& LegInstrument,
														 std::vector<std::string>& vecCombInstruments)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	return mpPlatformDataInstrument->GetCombInstrumentIDs_IncludeLeg(LegInstrument,vecCombInstruments);
}

///获取合约品种代码
int CReal_SvrTradeData::GetProductID(std::map<std::string, std::set<std::string> >& outData,
									 std::string ExchangeID)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	mpPlatformDataInstrument->GetProductID(outData,ExchangeID);
	return outData.size();
}



///获取合约乘数，成功返回合约乘数，失败返回-1
int CReal_SvrTradeData::GetInstrumentMultiple(const std::string& InstrumentID)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	return mpPlatformDataInstrument->GetInstrumentMultiple(InstrumentID);
}

///获取指定品种的合约列表,返回合约个数
int CReal_SvrTradeData::GetInstrumentListByProductID(const std::string& ProductID,
													 std::set<std::string> &outData)
{
//	CSafeLock lLock(m_mutex);
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("Init Status,discard this call");
		return 0;
	}
	mpPlatformDataInstrument->GetInstrumentListByProductID(ProductID,outData);
	return outData.size();
}
//报盘调用，查询合约完毕
void CReal_SvrTradeData::EndQryInstrument()
{	
	if(	mpPlatformDataInstrument == NULL)
	{
		TRADE_LOG("EndQryInstrument Init Status,discard this call");
		return ;
	}
	//设置合约数据准备状态	//为用户账户设置合约
	if(DataReady_Ready != InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReady_Ready))
	{
		SetPlatformObjectToAllAccount();
		SetPlatformObjectToAllUser(mMapUser2TradingData);	
	}
	
	//获取所有合约
	std::vector<PlatformStru_InstrumentInfo> lVecInstruments;
	std::set<std::string> lInstrumentStringSet;
	CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
	TRADE_LOG("Instruments size %d",lInstrumentStringSet.size());
	std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
	while(lIterSet != lInstrumentStringSet.end())
	{
		PlatformStru_InstrumentInfo lTempInfo;
		if(0 == CReal_SvrTradeData::getObj().GetInstrumentInfo(*lIterSet,lTempInfo))
			lVecInstruments.push_back(lTempInfo);
		lIterSet++;
	}
	
    //写数据库 
	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	//mpDBOpr->WriteIntrument(lsTempTradingDay,lVecInstruments);

	//至今天的交易日初始化完成	
	//mpTradingDayManager.SetTradingDayInit(lsTempTradingDay);
	//PUSH_LOG_ERROR(Logger,false,true,"合约查询完毕");

	//检测过期合约的持仓
	if(CheckInitDataHasExpiredInstruementPosition())
	{
		SetTraderDataStatus(TradeDataStatusInitFail);
		//PUSH_LOG_ERROR(Logger,false,true,"发现日初持仓里面有过期的合约，设置服务器为初始化失败状态");
		//PUSH_LOG_ERROR(Logger,false,true,"-----------------初始化失败-----------------");
		//通知线程关闭定时器
		//PostThreadMessage(g_idThread,TMESSAGE_TRADING_END_USER_CHECK,0,0);

		return;
	}

	//处理查询保证金和费率
	SendQryCommissionAndMargin();
	
}

//获取合约数据是否可用
eDataReady  CReal_SvrTradeData::GetInstrumentDataReady()
{	

	eDataReady lRet = DataReday_NoReady;
	InterlockedExchange(reinterpret_cast<volatile long*>(&lRet),mbInstrumentsReady);
	return lRet;

}
///获取指定合约行情
int CReal_SvrTradeData::GetQuotInfo(const std::string& InstrumentID, 
									PlatformStru_DepthMarketData& outData)
{
	if(mpPlatformDataQuots == NULL)
	{
		TRADE_LOG("GetQuotInfo error Status,discard this call");
		return -1;
	}

//	CSafeLock lLock(m_mutex);
	return mpPlatformDataQuots->GetQuotInfo(InstrumentID,outData)?0:-1;
}


///获取指定合约旧行情
int CReal_SvrTradeData::GetOldQuotInfo(const std::string& InstrumentID, 
									   PlatformStru_DepthMarketData& outData)
{
	if(mpPlatformDataQuots == NULL)
	{
		TRADE_LOG("GetOldQuotInfo error Status,discard this call");
		return -1;
	}

//	CSafeLock lLock(m_mutex);
	return mpPlatformDataQuots->GetOldQuotInfo(InstrumentID,outData)?0:-1;
}

///获取指定合约的现价，失败返回无效值
double CReal_SvrTradeData::GetCurPrice(const std::string& InstrumentID)
{
//	CSafeLock lLock(m_mutex);
	if(mpPlatformDataQuots == NULL)
	{
		TRADE_LOG("GetCurPrice error Status,discard this call");
		return util::GetDoubleInvalidValue();
	}

	return mpPlatformDataQuots->GetCurPrice(InstrumentID);
}

//设置指定合约行情，设置的行情更新，则返回true；否则返回false（比原来的行情更老）
CF_ERROR CReal_SvrTradeData::SetQuotInfo( PlatformStru_DepthMarketData& inData 
									 ,bool bSettlementManul )
{	
    TRADE_LOG("收到深度行情数据inst=%s",inData.InstrumentID);
	
	//计算
	bool lbRet = mpPlatformDataQuots->SetQuotInfo(inData);
	if(lbRet)
	{

		CSafeRwLockRead lrlock(mrwLockMapUserTrading);
		//触发重新计算
		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.begin();
		while(lIterUser != mMapUser2TradingData.end())
		{
			if(lIterUser->second->CalulateByNewQuto(inData,bSettlementManul))
			{
				if(mbNeedCache)
				{
					//缓存修改的持仓，和资金
					PlatformStru_TradingAccountInfo lFundInfo;
					lIterUser->second->GetFundInfo(lFundInfo);
					mpCache->CacheFund(lFundInfo);
                    TRADE_LOG("SetQuotInfo CacheFund AccountID=%s Available=%.4f DynamicProfit=%.4f StaticProfit=%.4f ",
                        lFundInfo.AccountID,lFundInfo.Available,lFundInfo.DynamicProfit,lFundInfo.StaticProfit);

					vector<PlatformStru_Position> lvscPos;
					lIterUser->second->GetPositions(inData.InstrumentID,lvscPos);
					for(int i = 0 ; i < lvscPos.size(); i++)
                    {
						mpCache->CachePosition(lvscPos[i]);
                        TRADE_LOG("SetQuotInfo CachePosition InstrumentID=%s CanCloseTodayVolume=%d CanCloseVolume=%d CanCloseydVolume=%d\
                            CloseProfit=%.4f CloseTodayOrderVolume=%d CloseOrderVolume=%d CloseVolume=%d InvestorID=%s CanCloseydVolume=%d\
                            OpenVolume=%d Position=%d TodayPosition=%d YdPosition=%d",
                            lvscPos[i].InstrumentID,lvscPos[i].CanCloseTodayVolume,lvscPos[i].CanCloseVolume,lvscPos[i].CanCloseydVolume,
                            lvscPos[i].CloseProfit,lvscPos[i].CloseTodayOrderVolume,lvscPos[i].CloseOrderVolume,lvscPos[i].CloseVolume,
                            lvscPos[i].InvestorID,lvscPos[i].OpenVolume,lvscPos[i].Position,lvscPos[i].TodayPosition,lvscPos[i].YdPosition);

                    }
				}
			}
			lIterUser++;
		}

		CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
		//触发重新计算
		_MapAccountData::iterator  lIterAccount = mMapAccountData.begin();
		while(lIterAccount != mMapAccountData.end())
		{
			lIterAccount->second->CalulateByNewQuto(inData,bSettlementManul);
			lIterAccount++;
		}
	}

	return CF_ERROR_SUCCESS;
}


/***************************************************************************************************************************************/

///获取某个账户合约手续费率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
int CReal_SvrTradeData::GetCommissionRate(const string& nsBrokerID,const string& nsAccountID,
										  const string& InstrumentID, 
										  PlatformStru_InstrumentCommissionRate& outData)
{
	
	//if(GetTraderDataStatus() == TradeDataStatusInitialing
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("error Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}
	CSafeRwLockRead lLock(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nsAccountID.c_str());

	//获取状态
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return -1;

	if(	!(lIter->second->GetCommissonMarginReady() & CommissionMargin_ComissionReady))
	{
		TRADE_LOG("Comission not ready,discard this call");
		return 0;
	}

	return  lIter->second->GetCommissionRate(InstrumentID,outData);

}
///获取某个用户合约手续费率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
//因为一个用户绑定一个账号，所以目前可以用这种方式来返回费率
int CReal_SvrTradeData::GetCommissionRate(const std::string& nsUserID,	
							  const std::string& InstrumentID, 
							  PlatformStru_InstrumentCommissionRate& outData) 
{
	
	//if(GetTraderDataStatus() == TradeDataStatusInitialing
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("error Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserID);
	if(mMapUser2TradingData.end() != lIter)
	{
		IInterfaceUserOp * lpTemp = lIter->second;	
		return lpTemp->GetCommissionRate(InstrumentID,outData);		
	}


	return -1;
}
///设置某个账户合约手续费率
void CReal_SvrTradeData::SetCommissionRate( PlatformStru_InstrumentCommissionRate& nCommissionRate)
{
	//if(GetTraderDataStatus() != TradeDataStatusInitialing &&
	//	GetTraderDataStatus() != TradeDataStatusSettlemtIniting && 
	//	GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Init Status,discard this call");
	//	return ;
	//}

	TRADE_LOG("SetCommissionRate BrokerID:%s AccountID %s",nCommissionRate.BrokerID,nCommissionRate.InvestorID);
// 	if(GetTraderDataStatus() == TradeDataStatusTrading && !CInterface_SvrLogin::getObj().IsSlaver())
// 	{
		//如果是盘中交易状态查询回来的则推送给所有的从服务
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryCommission to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryCommission_Push,
// 				&nCommissionRate,
// 				sizeof(nCommissionRate),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}

// 	}

	CSafeRwLockRead lLock(mrwLockMapAccount);

	IInterfaceAccountOp * lpTemp = NULL;
	BrokerAccountKey lKey(nCommissionRate.BrokerID,nCommissionRate.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
	{
		//TRADE_LOG("SetCommissionRate No Rate Object,discard this call");
		//return ;
		lpTemp =  IFactoryAccountOp::GetFactory()->CreateAccountOp();
		mMapAccountData[lKey] = lpTemp;
		lpTemp->SetPlatFormInstrument(mpPlatformDataInstrument);
		lpTemp->SetPlatFormQutos(mpPlatformDataQuots);
	}
	else
	{
		lpTemp = lIter->second;
	}

	std::set<std::string> InstrumentList;
	mpPlatformDataInstrument->GetInstrumentListByProductID(nCommissionRate.InstrumentID,InstrumentList);
	if((int)InstrumentList.size()> 0)
	{//是品种代码
		for(std::set<std::string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
		{
			 strcpy_s(nCommissionRate.InstrumentID,sizeof(TThostFtdcInstrumentIDType),(*it2).c_str());
			 lpTemp->SetCommissionRate(*it2,nCommissionRate);
		}
	}
	else	
	  lpTemp->SetCommissionRate(nCommissionRate.InstrumentID,nCommissionRate);
}
//报盘调用，当某一个用户合约的保证金率查询完成后
void CReal_SvrTradeData::EndUserQryCommission(const std::string& nsBrokerID,
										  const std::string &nAccountID,
                                          const std::string &nUserID) 
{

	//if(GetTraderDataStatus() != TradeDataStatusInitialing &&
	//	GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Init Status,discard this call");
	//	return ;
	//}

	//CHECK instrument ready
	TRADE_LOG("EndUserQryCommission BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());


	CSafeRwLockRead lLock(mrwLockMapAccount);
	IInterfaceAccountOp * lpRate = NULL;
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)	
	{		
		lpRate = lIter->second;
	}
	else
		return;

	//置commissionRate
	lIter->second->SetCommissionMarginReady(CommissionMargin_ComissionReady);	
	//如果费率和保证金率都查了
// 	if(lIter->second->GetCommissonMarginReady() == CommissionMargin_ComissionAndMarinReady)
// 	{	
		//TRADE_LOG("EndUserQryCommission BrokerID:%s AccountID %s send qry trader",nsBrokerID.c_str(),nAccountID.c_str());
		//设置费率对象到对应的用户对象
		//SetRatesObjectToAccountRelationUserObject(nsBrokerID,nAccountID,nUserID);


		////PUSH_LOG_ERROR(Logger,false,true,"费率和保证金率查询完毕，装载委托交易帐号%s的日初数据",nAccountID.c_str());

		////装载日初的数据
		//DispathInitialDataToUserWithAccount(nsBrokerID,nAccountID,mMapUser2TradingData);

		////PUSH_LOG_ERROR(Logger,false,true,"发送委托交易帐号%s的查询资金命令",nAccountID.c_str());


		////发送查成交的命令
		//Stru_NotifyEvent lDataEvent(EventTypeNeedQryFund,0);
		//BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
		//lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
		////CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 	}


	

	//写数据库
// 	if(lpRate != NULL && !CInterface_SvrLogin::getObj().IsSlaver())	
// 	{
// 		std::vector<PlatformStru_InstrumentCommissionRate> lVecCommission;
// 		std::set<std::string> lInstrumentStringSet;
// 		CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
// 		std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
// 		while(lIterSet != lInstrumentStringSet.end())
// 		{
// 			PlatformStru_InstrumentCommissionRate lTempInfo;			
// 			if(0 == lpRate->GetCommissionRate(*lIterSet,lTempInfo))
// 				lVecCommission.push_back(lTempInfo);
// 			lIterSet++;
// 		}
// 
// 		//写数据库 
// 		std::string lsTempTradingDay;
// 		GetCurrentTradingDay(lsTempTradingDay);
// 		mpDBOpr->WriteCommission(nsBrokerID,nAccountID,lsTempTradingDay,lVecCommission);
// 	}
	return;
}

///获取合约保证金率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
int CReal_SvrTradeData::GetMarginRate(const string& nsBrokerID,
									  const string& nsAccountID,
									  const string& InstrumentID, 
									  PlatformStru_InstrumentMarginRate& outData)
{
	
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lLock(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nsAccountID.c_str());
	//获取状态	

	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return -1;

	if(!(lIter->second->GetCommissonMarginReady() & CommissionMargin_MarginReady))
	{
		TRADE_LOG("GetMarginRate Comission not ready,discard this call");
		return 0;
	}
	return lIter->second->GetMarginRate(InstrumentID,outData);
}


///获取合约保证金率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
int CReal_SvrTradeData::GetMarginRate(const string& nsUserID,	
						  const string& InstrumentID, 
						  PlatformStru_InstrumentMarginRate& outData) 
{
	

	//if(GetTraderDataStatus() == TradeDataStatusInitialing
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("error Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserID);
	if(mMapUser2TradingData.end() != lIter)
	{
		IInterfaceUserOp * lpTemp = lIter->second;	
		return lpTemp->GetMarginRate(InstrumentID,outData);		
	}


	return -1;
}

///设置合约保证金率
void CReal_SvrTradeData::SetMarginRate( PlatformStru_InstrumentMarginRate& nMarginRate)
{
	
	//if(GetTraderDataStatus() != TradeDataStatusInitialing &&
	//	GetTraderDataStatus() != TradeDataStatusSettlemtIniting && 
	//	GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Init Status,discard this call");
	//	return ;
	//}
	TRADE_LOG("SetMarginRate BrokerID:%s AccountID %s",nMarginRate.BrokerID,nMarginRate.InvestorID);
// 	if(GetTraderDataStatus() == TradeDataStatusTrading && !CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//如果是盘中交易状态查询回来的则推送给所有的从服务
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryMargin to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryMargin_Push,
// 				&nMarginRate,
// 				sizeof(nMarginRate),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 
// 	}
	CSafeRwLockRead lLock(mrwLockMapAccount);
	IInterfaceAccountOp * lpTemp = NULL;
	BrokerAccountKey lKey(nMarginRate.BrokerID,nMarginRate.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
	{
		TRADE_LOG("No Rate Object,discard this call");
		return ;
	}
	else
	{
		lpTemp = lIter->second;
	}

	return lpTemp->SetMarginRate(nMarginRate.InstrumentID,nMarginRate);

}
//报盘调用，当某一个用户合约的保证金率查询完成后
void CReal_SvrTradeData::EndUserQryMargin(const std::string& nsBrokerID,
													   const std::string &nAccountID) 
{
	/*if(GetTraderDataStatus() != TradeDataStatusInitialing &&
		GetTraderDataStatus() != TradeDataStatusTrading)
	{
		TRADE_LOG("Not Init Status,discard this call");
		return ;
	}*/


	TRADE_LOG("EndUserQryMargin BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lLock(mrwLockMapAccount);

	IInterfaceAccountOp * lpRate = NULL;
	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);	
	if(mMapAccountData.end() != lIter)	
	{		
		lpRate = lIter->second;
	}
	else
		return;
	//置commissionRate
	lIter->second->SetCommissionMarginReady(CommissionMargin_MarginReady);
	/*_MapAccountData::iterator lIterReady =  mMapAccountRateReady.find(lKey);
	if(mMapAccountRateReady.end() != lIterReady)
	{
		int lnTemp = (int)lIterReady->second;
		lnTemp |= CommissionMargin_MarginReady;
		lIterReady->second = (eCommissionMarginReady)lnTemp;
	}*/
	//如果费率和保证金率都查了
	if(lIter->second->GetCommissonMarginReady() == CommissionMargin_ComissionAndMarinReady)
	{	
		TRADE_LOG("EndUserQryMargin BrokerID:%s AccountID %s send qry trader",nsBrokerID.c_str(),nAccountID.c_str());
		//设置费率对象到对应的用户对象
		//SetRatesObjectToAccountRelationUserObject(nsBrokerID,nAccountID);

		

		////PUSH_LOG_ERROR(Logger,false,true,"保证金查询完毕，装载委托交易帐号%s的日初数据",nAccountID.c_str());

		////装载日初的数据
		//DispathInitialDataToUserWithAccount(nsBrokerID,nAccountID,mMapUser2TradingData);


		////PUSH_LOG_ERROR(Logger,false,true,"发送委托交易帐号%s的查询资金命令",nAccountID.c_str());

		////发送查成交的命令
		//Stru_NotifyEvent lDataEvent(EventTypeNeedQryFund,0);
		//BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
		//lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
		////CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	}

	

	//写数据库
	if(lpRate != NULL/* && !CInterface_SvrLogin::getObj().IsSlaver()*/)
	{
		std::vector<PlatformStru_InstrumentMarginRate> lVecMargin;
		std::set<std::string> lInstrumentStringSet;
		CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
		std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
		while(lIterSet != lInstrumentStringSet.end())
		{
			PlatformStru_InstrumentMarginRate lTempInfo;	
			memset(&lTempInfo,0,sizeof(PlatformStru_InstrumentMarginRate));
			if(0 == lpRate->GetMarginRate(*lIterSet,lTempInfo))	
				lVecMargin.push_back(lTempInfo);
			lIterSet++;
		}

		//写数据库 
		std::string lsTempTradingDay;
		GetCurrentTradingDay(lsTempTradingDay);
		//mpDBOpr->WriteMarginRate(nsBrokerID,nAccountID,lsTempTradingDay,lVecMargin);
	}

	return;
}

//报盘调用，当某一个主帐号资金查询完成
void CReal_SvrTradeData::QryFundReach(const PlatformStru_TradingAccountInfo& inData,const std::string & nUserID)
{
	TRADE_LOG("QryFundReach BrokerID:%s UserID %s",inData.BrokerID,inData.AccountID);

	CSafeRwLockRead lrlock(mrwLockMapAccount);

// 	if(GetTraderDataStatus() == TradeDataStatusTrading && !CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//如果是盘中交易状态查询回来的则推送给所有的从服务
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryFund to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryFund_Push,
// 				(void*)&inData,
// 				sizeof(inData),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}

// 	}
	


	//设置查询明细查询结束，
	BrokerAccountKey lKey(inData.BrokerID,inData.AccountID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{

// 		if(1/*GetTraderDataStatus() == TradeDataStatusTrading && CInterface_SvrLogin::getObj().IsSlaver()*/)
// 		{
			lIter->second->SetCommissionMarginReady(CommissionMargin_ComissionAndMarinReady);
			//说明是盘中增加的委托交易帐号
			TRADE_LOG("SetRatesObjectToAccountRelationUserObject BrokerID:%s AccountID %s",inData.BrokerID,inData.AccountID);
			//设置费率对象到对应的用户对象
			SetRatesObjectToAccountRelationUserObject(inData.BrokerID,inData.AccountID,nUserID);

			//装载日初的数据
			DispathInitialDataToUserWithAccount(inData.BrokerID,inData.AccountID,mMapUser2TradingData,nUserID);

// 		}
// 		IInterfaceAccountOp * lpTemp = lIter->second;	
// 		if(CF_ERROR_SUCCESS == lpTemp->SetQryFundInfo(inData))
// 		{

			if(mbNeedCache)
            {
				mpCache->CacheFund(inData);
                TRADE_LOG("QryFundReach CacheFund AccountID=%s Available=%.4f DynamicProfit=%.4f StaticProfit=%.4f ",
                    inData.AccountID,inData.Available,inData.DynamicProfit,inData.StaticProfit);

            }

			//产生一个需要查询报单的事件
			//PUSH_LOG_ERROR(Logger,false,true,"发送委托交易帐号%s的查询成交命令",inData.AccountID);
// 
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryTrader,0);		
// 			lDataEvent.AddBufer((void*)&lKey,sizeof(lKey));	
			//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 		}
		
	}
}


//查询到某一个主帐号的持仓
void CReal_SvrTradeData::QryPositionReach( PlatformStru_Position& PositionInfo, int ErrID, bool bIsLast)
{
	//添加到用户管理模块去
	TRADE_LOG("QryPositionReach BrokerID:%s UserID %s ErrID:%d,BLast:%d",PositionInfo.BrokerID,PositionInfo.InvestorID,ErrID,bIsLast);

	if(GetTraderDataStatus() == TradeDataStatusTrading /*&& !CInterface_SvrLogin::getObj().IsSlaver()*/)
	{
		//如果是盘中交易状态查询回来的则推送给所有的从服务
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryPosition to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryPosition_Push,
// 				&PositionInfo,
// 				sizeof(PositionInfo),
// 				0,
// 				0,
// 				ErrID,
// 				bIsLast,
// 				CF_ERROR_SUCCESS,0);
// 		}

	}
	CSafeRwLockRead lrlock(mrwLockMapAccount);


	//设置查询明细查询结束，
	BrokerAccountKey lKey(PositionInfo.BrokerID,PositionInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		if(CF_ERROR_SUCCESS == lpTemp->QryPositionReach(PositionInfo,ErrID,bIsLast) && ErrID == 0)
		{
			//添加到待存储对象中
			//mBufferDataStoreOrder.Push(OrderInfo,BufferStoreCallback);
			if(mbNeedCache)
            {
				mpCache->CachePosition(PositionInfo);
                TRADE_LOG("QryPositionReach CachePosition InstrumentID=%s CanCloseTodayVolume=%d CanCloseVolume=%d CanCloseydVolume=%d\
                          CloseProfit=%.4f CloseTodayOrderVolume=%d CloseOrderVolume=%d CloseVolume=%d InvestorID=%s CanCloseydVolume=%d\
                          OpenVolume=%d Position=%d TodayPosition=%d YdPosition=%d",
                          PositionInfo.InstrumentID,PositionInfo.CanCloseTodayVolume,PositionInfo.CanCloseVolume,PositionInfo.CanCloseydVolume,
                          PositionInfo.CloseProfit,PositionInfo.CloseTodayOrderVolume,PositionInfo.CloseOrderVolume,PositionInfo.CloseVolume,
                          PositionInfo.InvestorID,PositionInfo.OpenVolume,PositionInfo.Position,PositionInfo.TodayPosition,PositionInfo.YdPosition);
            }
		}	
	}
	return;
}


//报盘调用，当某一个持仓查询完成后
void CReal_SvrTradeData::EndUserQryPosition(const std::string& nsBrokerID,
						const std::string &nAccountID)
{
	TRADE_LOG("EndUserQryPosition BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lrlock(mrwLockMapAccount);


	//PUSH_LOG_ERROR(Logger,false,true,"委托交易帐号%s持仓查询完成",nAccountID.c_str());


	//设置查询明细查询结束，
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		PlatformStru_Position lPositionInfo;
		lIter->second->QryPositionReach(lPositionInfo,-1,true);
		lIter->second->SetAccountInitStatus(USER_STATUS_InitQryPositionReady);

		if(lIter->second->GetAccountInitStatus() == USER_STATUS_Inited)
		{
			//发送用户数据准备好了，可以进行风险运算等后续操作
		/*	Stru_NotifyEvent lsFund(EventTypeUserDataReady,0);
			lsFund.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsFund);	*/
		}
	}

	return;
}



//查询到某一个主帐号的持仓
void CReal_SvrTradeData::QryPositionDetailReach( PlatformStru_PositionDetail& PositionDetailInfo, int ErrID, bool bIsLast)
{
	//添加到用户管理模块去
	TRADE_LOG("QryPositionDetailReach BrokerID:%s UserID %s ErrID:%d,BLast:%d",PositionDetailInfo.BrokerID,PositionDetailInfo.InvestorID,ErrID,bIsLast);

// 	if(GetTraderDataStatus() == TradeDataStatusTrading && !CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//如果是盘中交易状态查询回来的则推送给所有的从服务
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a QryPositionDetail to SLaver %d",lVecSocket[i]);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterQryPositionDetail_Push,
// 				&PositionDetailInfo,
// 				sizeof(PositionDetailInfo),
// 				0,
// 				0,
// 				ErrID,
// 				bIsLast,
// 				CF_ERROR_SUCCESS,0);
// 		}

// 	}
	CSafeRwLockRead lrlock(mrwLockMapAccount);


	//设置查询明细查询结束，
	BrokerAccountKey lKey(PositionDetailInfo.BrokerID,PositionDetailInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		if(CF_ERROR_SUCCESS == lpTemp->QryPositionDetailReach(PositionDetailInfo,ErrID,bIsLast) && ErrID == 0)
		{
			//添加到待存储对象中
			//mBufferDataStoreOrder.Push(OrderInfo,BufferStoreCallback);
			if(mbNeedCache)
				mpCache->CachePositionDetail(PositionDetailInfo);
		}	
	}

	return;
}


//报盘调用，当某一个持仓明细查询完成后
void CReal_SvrTradeData::EndUserQryPositionDetail(const std::string& nsBrokerID,
							  const std::string &nAccountID) 
{

	TRADE_LOG("EndUserQryOrder BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lrlock(mrwLockMapAccount);


	//PUSH_LOG_ERROR(Logger,false,true,"委托交易帐号%s持仓明细查询完成",nAccountID.c_str());

	
	//设置查询明细查询结束，
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		PlatformStru_OrderInfo lOrderInfo;
		lIter->second->QryOrderReach(lOrderInfo,-1,true);
		lIter->second->SetAccountInitStatus(USER_STATUS_InitQryPositionDetailReady);

		//PUSH_LOG_ERROR(Logger,false,true,"发送委托交易帐号%s的查询持仓命令",nAccountID.c_str());

		//产生一个需要查询报单的事件
		Stru_NotifyEvent lDataEvent(EventTypeNeedQryPosition,0);
		BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
		lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	}

	return;
}






//查询报单到来
CF_ERROR CReal_SvrTradeData::QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
										   const int ErrID,
										   const bool bIsLast) 
{

	//添加到用户管理模块去
	TRADE_LOG("QryOrderReach BrokerID:%s UserID %s ErrID:%d,BLast:%d",OrderInfo.BrokerID,OrderInfo.InvestorID,ErrID,bIsLast);
	
	////在交易中的查询报单需要推送给从服务
	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//向从服务推送行情
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterQryOrder_Push,
	//			&OrderInfo,
	//			sizeof(OrderInfo),
	//			0,
	//			0,
	//			ErrID,
	//			bIsLast,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(OrderInfo.InvestorID);
	if(mMapUser2TradingData.end() != lIter)
	{
		IInterfaceUserOp * lpTemp = lIter->second;	
		if( ErrID == 0)
		{	
			lpTemp->QryOrderReach(OrderInfo,ErrID,bIsLast) ;
			//添加到待存储对象中
// 			if(!CInterface_SvrLogin::getObj().IsSlaver())	
// 			{
// 				TRADE_LOG("Push order to bufferdate store BrokerID:%s UserID %s ErrID:%d,BLast:%d",OrderInfo.BrokerID,OrderInfo.InvestorID,ErrID,bIsLast);
// 				mBufferDataStoreOrder.Push(OrderInfo,BufferStoreCallback);
// 			}
			if(mbNeedCache)
            {
				mpCache->CacheOrder(OrderInfo);
                TRADE_LOG("QryOrderReach CacheOrder InvestorID=%s InstrumentID=%s CombHedgeFlag=%c CombOffsetFlag=%c\
                          Direction=%c LimitPrice=%.4f VolumeTotalOriginal=%d StatusMsg=%c",
                          OrderInfo.InvestorID,OrderInfo.InstrumentID,OrderInfo.CombHedgeFlag,OrderInfo.CombOffsetFlag,
                          OrderInfo.Direction,OrderInfo.LimitPrice,OrderInfo.VolumeTotalOriginal,OrderInfo.StatusMsg);
            }

		}
	}
	return CF_ERROR_SUCCESS;
}

//报盘调用，当某一个账户的报单后
void CReal_SvrTradeData::EndUserQryOrder(const std::string& nsBrokerID,
							 const std::string &nAccountID)
{


	TRADE_LOG("EndUserQryOrder BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	
// 	PUSH_LOG_ERROR(Logger,false,true,"委托交易帐号%s报单查询完成",nAccountID.c_str());

	//调用该账号相关的交易员，全部结束查询报单
	std::vector<UserInfo> lVecUser;
// 	CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nAccountID,nsBrokerID,lVecUser);
// 	for(unsigned int i = 0; i < lVecUser.size() ;i++)
// 	{		
//         //设置查询报单结束，初始化过程结束
// 		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(lVecUser[i].szAccount);
// 		if(mMapUser2TradingData.end() != lIter)
// 		{
// 			PlatformStru_OrderInfo lOrderInfo;
// 			lIter->second->QryOrderReach(lOrderInfo,-1,true);
// 			lIter->second->SetUserInitStatus(USER_STATUS_InitQryOrdersReady);
// 
// 
// 			//if(lIter->second->GetUserInitStatus() == USER_STATUS_Inited)
// 			//{
// 			//	//发送用户数据准备好了，可以进行风险运算等后续操作
// 			//	Stru_NotifyEvent lsFund(EventTypeUserDataReady,0);
// 			//	lsFund.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
// 			//	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsFund);	
// 			//}
// 		}
// 		//
// 		////设置可以登录
// 		//CInterface_SvrLogin::getObj().SetUserDataReady(lVecUser[i].szAccount,true);
// 	}

	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
	_MapAccountData::iterator lIterAccount = mMapAccountData.find(lKey);
	if(lIterAccount != mMapAccountData.end())
	{
		lIterAccount->second->SetAccountInitStatus(USER_STATUS_InitQryOrdersReady);
	}

	//PUSH_LOG_ERROR(Logger,false,true,"发送委托交易帐号%s的查询持仓明细命令",nAccountID.c_str());
	//产生一个需要查询报单的事件
	Stru_NotifyEvent lDataEvent(EventTypeNeedQryPositionDetail,0);
	BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
	lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
}


////添加一个报单
CF_ERROR CReal_SvrTradeData::PushOrder(PlatformStru_OrderInfo & nOrderInfo,bool nbNeedRiseEvent)
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	TRADE_LOG("PushOrder BrokerID:%s UserID %s IId %s %c",nOrderInfo.BrokerID,nOrderInfo.InvestorID,nOrderInfo.InstrumentID,nOrderInfo.Direction);
	

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	//添加到用户管理模块去	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nOrderInfo.InvestorID);
	if(mMapUser2TradingData.end() != lIter)
	{
		IInterfaceUserOp * lpTemp = lIter->second;	
		lRet = lpTemp->PushOrder(nOrderInfo);	
		if(lRet != CF_ERROR_NOINITED && nbNeedRiseEvent)
		{
			//添加到待存储对象中
			if(lRet == CF_ERROR_SUCCESS /*&& !CInterface_SvrLogin::getObj().IsSlaver()*/)			
				mBufferDataStoreOrder.Push(nOrderInfo,&BufferStoreCallback);

			//cache
			if(mbNeedCache)
			{
				mpCache->CacheOrder(nOrderInfo);
				mpCache->PushOrder(nOrderInfo);

                TRADE_LOG("PushOrder CacheOrder InvestorID=%s InstrumentID=%s CombHedgeFlag=%c CombOffsetFlag=%c\
                          Direction=%c LimitPrice=%.4f VolumeTotalOriginal=%d StatusMsg=%c",
                          nOrderInfo.InvestorID,nOrderInfo.InstrumentID,nOrderInfo.CombHedgeFlag,nOrderInfo.CombOffsetFlag,
                          nOrderInfo.Direction,nOrderInfo.LimitPrice,nOrderInfo.VolumeTotalOriginal,nOrderInfo.StatusMsg);

                TRADE_LOG("PushOrder PushOrder InvestorID=%s InstrumentID=%s CombHedgeFlag=%c CombOffsetFlag=%c\
                          Direction=%c LimitPrice=%.4f VolumeTotalOriginal=%d StatusMsg=%c",
                          nOrderInfo.InvestorID,nOrderInfo.InstrumentID,nOrderInfo.CombHedgeFlag,nOrderInfo.CombOffsetFlag,
                          nOrderInfo.Direction,nOrderInfo.LimitPrice,nOrderInfo.VolumeTotalOriginal,nOrderInfo.StatusMsg);

			}

			//如果已初始化完毕，才发送报单事件
			Stru_NotifyEvent lEvent(EventTypeOrderRtn,0);
			lEvent.AddBufer(&nOrderInfo,sizeof(PlatformStru_OrderInfo));	
			//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);
		}
	}
	if(0/*!CInterface_SvrLogin::getObj().IsSlaver()*/)
	{
		//向从服务推送行情
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a order to SLaver %d ,order info %s %s Ftid:%d updateseq:%d",lVecSocket[i],nOrderInfo.InstrumentID,nOrderInfo.InvestorID,nOrderInfo.FTID,nOrderInfo.UpdateSeq);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterOrder_Push,
// 				&nOrderInfo,
// 				sizeof(nOrderInfo),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
	}

	return lRet;
}
// 报单录入应答
CF_ERROR CReal_SvrTradeData::PushRspOrderInsert(int nFrontID,
												int nSessionID,
												CThostFtdcInputOrderField *pInputOrder, 
											    CThostFtdcRspInfoField *pRspInfo,
										        int nRequestID, 
											    bool bIsLast)
{
	//if(GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Trading Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}


	if(pInputOrder)
		TRADE_LOG("PushRspOrderInsert BrokerID:%s UserID %s",pInputOrder->BrokerID,pInputOrder->InvestorID);

	//发送事件
	//Stru_NotifyEvent lEvent(EventTypeRspOrderInsert,0);
	PlatformStru_OrderInfo lOrder;
	GenerateOrderInfoFromCTPInputOrder(*pInputOrder,lOrder);
	lOrder.FrontID = nFrontID;
	lOrder.SessionID = nSessionID;
	
	if(pRspInfo)
		memcpy(lOrder.StatusMsg,pRspInfo->ErrorMsg,sizeof(TThostFtdcErrorMsgType));
	//lEvent.AddBufer(&lOrder,sizeof(PlatformStru_OrderInfo));
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);
	//TRADE_LOG("PushRspOrderInsert Fail");
	lOrder.troubleMoney = util::GetDoubleInvalidValue();	
	lOrder.freezeMargin = util::GetDoubleInvalidValue();
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(lOrder.CancelTime, sizeof(lOrder.CancelTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strncpy(lOrder.UpdateTime, lOrder.CancelTime, sizeof(lOrder.UpdateTime));

	lOrder.OrderStatus = THOST_FTDC_OST_Canceled;
	lOrder.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	CInterface_SvrTradeData::getObj().PushOrder(lOrder,true);

	return CF_ERROR_SUCCESS;
}

///报单录入错误回报
CF_ERROR CReal_SvrTradeData::PushErrRtnOrderInsert(int nFrontID,
												   int nSessionID,
												   CThostFtdcInputOrderField *pInputOrder,
												   CThostFtdcRspInfoField *pRspInfo)
{
	//if(GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Trading Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}


	if(pInputOrder)
		TRADE_LOG("PushErrRtnOrderInsert BrokerID:%s UserID %s",pInputOrder->BrokerID,pInputOrder->InvestorID);

	//发送事件
	//Stru_NotifyEvent lEvent(EventTypeErrOrderInsert,0);
	PlatformStru_OrderInfo lOrder;
	GenerateOrderInfoFromCTPInputOrder(*pInputOrder,lOrder);
	lOrder.FrontID = nFrontID;
	lOrder.SessionID = nSessionID;
	if(pRspInfo)
		memcpy(lOrder.StatusMsg,pRspInfo->ErrorMsg,sizeof(TThostFtdcErrorMsgType));
	lOrder.troubleMoney = util::GetDoubleInvalidValue();	
	lOrder.freezeMargin = util::GetDoubleInvalidValue();
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(lOrder.CancelTime, sizeof(lOrder.CancelTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strncpy(lOrder.UpdateTime, lOrder.CancelTime, sizeof(lOrder.UpdateTime));
	lOrder.OrderStatus = THOST_FTDC_OST_Canceled;
	lOrder.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	
	CInterface_SvrTradeData::getObj().PushOrder(lOrder,true);

	//if(mbNeedCache)
	//	mCache.PushOrderInputError(lOrder);
// 	if(!CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//向从服务推送行情
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a Error order insert to SLaver %d ,order info %s %s Ftid:%d updateseq:%d",lVecSocket[i],lOrder.InstrumentID,lOrder.InvestorID,lOrder.FTID,lOrder.UpdateSeq);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterErrOrderInsert_Push,
// 				&lOrder,
// 				sizeof(PlatformStru_OrderInfo),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 	}
	//需解冻资金
	return CF_ERROR_SUCCESS;

}
///报单操作请求响应
CF_ERROR CReal_SvrTradeData::PushRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, 
								 CThostFtdcRspInfoField *pRspInfo,
								 int nRequestID, 
								 bool bIsLast)
{
	//if(GetTraderDataStatus() != TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Not Trading Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}


	if(pInputOrderAction)
		TRADE_LOG("PushRspOrderAction BrokerID:%s UserID %s",pInputOrderAction->BrokerID,pInputOrderAction->InvestorID);

	//发送事件
	Stru_NotifyEvent lEvent(EventTypeRspOrderAction,0);		
	lEvent.AddBufer(pInputOrderAction,sizeof(CThostFtdcInputOrderActionField));
	lEvent.AddBufer(pRspInfo,sizeof(CThostFtdcRspInfoField));
	lEvent.AddBufer(&nRequestID,sizeof(int));
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);

	//需解冻资金
	return CF_ERROR_SUCCESS;
}

///报单操作错误回报
CF_ERROR CReal_SvrTradeData::PushErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction,
									CThostFtdcRspInfoField *pRspInfo)
{
	/*if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/

	if(pOrderAction)
		TRADE_LOG("PushErrRtnOrderAction BrokerID:%s UserID %s",pOrderAction->BrokerID,pOrderAction->InvestorID);

	//发送事件
	Stru_NotifyEvent lEvent(EventTypeErrOrderAction,0);
	lEvent.AddBufer(pOrderAction,sizeof(CThostFtdcOrderActionField));
	lEvent.AddBufer(pRspInfo,sizeof(CThostFtdcRspInfoField));
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);


// 	if(!CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//向从服务推送行情
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a error order action to SLaver %d ,order info %s %s ",lVecSocket[i],
// 				pOrderAction->InstrumentID,pOrderAction->InvestorID);
// 
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterErrOrderAction_Push,
// 				pOrderAction,
// 				sizeof(CThostFtdcOrderActionField),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 	}
	/*if(mbNeedCache)
	{
		mCache.PushOrderActionError(*pOrderAction);
	}*/
	//需解冻资金
	return CF_ERROR_SUCCESS;
}

///获取指定报单
CF_ERROR CReal_SvrTradeData::GetUserOrderByKey(const std::string & nUserID,
								   const OrderKey& nOrderKey,
								   PlatformStru_OrderInfo& outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("GetUserOrderByKey error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;


	return lpTemp->GetOrderByKey(nOrderKey,outData);
}
///获取全部报单
CF_ERROR CReal_SvrTradeData::GetUserOrders(const std::string & nUserID,
										   std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;


	return lpTemp->GetAllOrders(outData);

}

/////获取全部已触发的报单
CF_ERROR CReal_SvrTradeData::GetUserTriggerOrders(const std::string & nUserID,
												  std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	//
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetTriggerOrders(outData);
}

/////获取已成交报单
CF_ERROR CReal_SvrTradeData::GetUserTradedOrders(const std::string & nUserID,
												 std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetTradedOrders(outData);

}

/////获取已撤单和错误报单
CF_ERROR CReal_SvrTradeData::GetUserCanceledOrders(const std::string & nUserID,
												   std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	//
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetCanceledOrders(outData);
}

/////获取未成交报单
CF_ERROR CReal_SvrTradeData::GetUserWaitOrders(const std::string & nUserID,
											   std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetWaitOrders(outData);

}

///获取未知状态的报单
CF_ERROR CReal_SvrTradeData::GetUserUnkownOrders(const std::string & nUserID,
									 std::vector<PlatformStru_OrderInfo>& outData)
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetUnkownOrders(outData);
}


//
///获取指定报单
CF_ERROR CReal_SvrTradeData::GetAccountOrderByKey(const std::string & nsBrokerID,
									  const std::string & nInvestorID,
									  const OrderKey& nOrderKey,
									  PlatformStru_OrderInfo& outData)
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetOrderByKey(nOrderKey,outData);
}


///获取全部报单
CF_ERROR CReal_SvrTradeData::GetAccountOrders(const std::string & nsBrokerID,
								  const std::string & nInvestorID,
								  std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetAllOrders(outData);
}


/////获取全部已触发的报单
CF_ERROR CReal_SvrTradeData::GetAccountTriggerOrders(const std::string & nsBrokerID,
										 const std::string & nInvestorID,
										 std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetTriggerOrders(outData);
}

/////获取已成交报单
CF_ERROR CReal_SvrTradeData::GetAccountTradedOrders(const std::string & nsBrokerID,
										const std::string & nInvestorID,
										std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetTradedOrders(outData);
}

/////获取已撤单和错误报单
CF_ERROR CReal_SvrTradeData::GetAccountCanceledOrders(const std::string & nsBrokerID,
										  const std::string & nInvestorID,
										  std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetCanceledOrders(outData);
}

/////获取未成交报单
CF_ERROR CReal_SvrTradeData::GetAccountWaitOrders(const std::string & nsBrokerID,
									  const std::string & nInvestorID,
									  std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetWaitOrders(outData);
}

///获取未知状态的报单
CF_ERROR CReal_SvrTradeData::GetAccountUnkownOrders(const std::string & nsBrokerID,
										const std::string & nInvestorID,
										std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetUnkownOrders(outData);
}
//查询的成交回报到来
CF_ERROR CReal_SvrTradeData::QryTradeReach( PlatformStru_TradeInfo& TradeInfo,
										   int ErrID,
										   bool bIsLast)
{
	//添加到用户管理模块去
	TRADE_LOG("QryTradeReach BrokerID:%s UserID %s ErrID:%d,BLast:%d",TradeInfo.BrokerID,TradeInfo.InvestorID,ErrID,bIsLast);
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("QryTradeReach Settlement Status,dispatch this call to settlement data with ctp");
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(TradeInfo.InvestorID);
		if(mMapUser2TradingData.end() != lIter)
		{
			IInterfaceUserOp * lpTemp = lIter->second;	
			lpTemp->QryTradeReach(TradeInfo,ErrID,bIsLast);		
			
		}
		return CF_ERROR_SUCCESS;
	}
	else if(GetTraderDataStatus() == TradeDataStatusInitialing || GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("QryTradeReach Trading Status,dispatch this call to trading data");
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(TradeInfo.InvestorID);
		if(mMapUser2TradingData.end() != lIter)
		{
			IInterfaceUserOp * lpTemp = lIter->second;			
			if(ErrID == 0)
			{
				lpTemp->QryTradeReach(TradeInfo,ErrID,bIsLast);
				//添加到待存储对象中
// 				if(CInterface_SvrLogin::getObj().IsSlaver())
// 				{
// 					TRADE_LOG("QryTradeReach Trading Status,push this trade to buffer store data");
// 					mBufferDataStoreTrade.Push(TradeInfo,BufferStoreCallback);
// 				}
			}
		}
		return CF_ERROR_SUCCESS;
	}
	
	return CF_ERROR_NOINITED;
}


//报盘调用，当某一个账户的成交后
void CReal_SvrTradeData::EndUserQryTrade(const std::string& nsBrokerID,
							 const std::string &nAccountID)
{
	//调用该账号相关的交易员，全部结束查询成交
	//发送查询该账户的报单命令
	TRADE_LOG("EndUserQryTrade BrokerID:%s AccountID %s",nsBrokerID.c_str(),nAccountID.c_str());
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("EndUserQryTrade Settlement Status,dispatch this call to settlement data with ctp");
		std::vector<UserInfo> lVecUser;
// 		CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nAccountID,nsBrokerID,lVecUser);
// 		for(unsigned int i = 0; i < lVecUser.size() ;i++)
// 		{
// 			_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(lVecUser[i].szAccount);
// 			if(mMapUser2TradingData.end() != lIter)
// 			{
// 				PlatformStru_TradeInfo lTraderInfo;
// 				lIter->second->QryTradeReach(lTraderInfo,-1,true);
// 				
// 			}
// 		}
		
	}
	else if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("EndUserQryTrade Trading Status,dispatch this call to trading data");
		std::vector<UserInfo> lVecUser;
// 		CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nAccountID,nsBrokerID,lVecUser);
// 		for(unsigned int i = 0; i < lVecUser.size() ;i++)
// 		{
// 			_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(lVecUser[i].szAccount);
// 			if(mMapUser2TradingData.end() != lIter)
// 			{
// 				PlatformStru_TradeInfo lTraderInfo;
// 				lIter->second->QryTradeReach(lTraderInfo,-1,true);				
// 			}
// 		}

		CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
		BrokerAccountKey lKey(nsBrokerID.c_str(),nAccountID.c_str());
		_MapAccountData::iterator lIterAccount = mMapAccountData.find(lKey);
		if(lIterAccount != mMapAccountData.end())
		{
			lIterAccount->second->SetAccountInitStatus(USER_STATUS_InitQryTradesReady);
		}

	
		//PUSH_LOG_ERROR(Logger,false,true,"成交查询完成，发送委托交易帐号%s的查询报单命令",nAccountID.c_str());

		//产生一个需要查询报单的事件
		Stru_NotifyEvent lDataEvent(EventTypeNeedQryOrder,0);
		BrokerAccountKey lsBrokerAccount(nsBrokerID.c_str(),nAccountID.c_str());		
		lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
		
	}

	

}
//新的成交回报到来
CF_ERROR CReal_SvrTradeData::PushTrade(PlatformStru_TradeInfo& nTradeInfo)
{

	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	TRADE_LOG("推送成交PushTrade OrderRef=%s,  TradeID=%s,  OrderSysID=%s,Direction=%c, OffsetFlag=%c,Price=%f,Volume=%d,InstrumentID=%s,TradeTime=%s",nTradeInfo.OrderRef,nTradeInfo.TradeID,nTradeInfo.OrderSysID,
		nTradeInfo.Direction,nTradeInfo.OffsetFlag,nTradeInfo.Price,nTradeInfo.Volume,nTradeInfo.InstrumentID,nTradeInfo.TradeTime);
	
	

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	//添加到用户管理模块去
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nTradeInfo.InvestorID);
	if(mMapUser2TradingData.end() != lIter)
	{
		
		IInterfaceUserOp * lpTemp = lIter->second;	
		
		TradeKey lkey(nTradeInfo);
		if(!lpTemp->TestTradeInfoExist(lkey))
		{

			lRet = lpTemp->PushTrader(nTradeInfo);	
			if(lRet == CF_ERROR_SUCCESS)
			{

				//添加到待存储对象中
// 				if(CInterface_SvrLogin::getObj().IsSlaver())
// 					mBufferDataStoreTrade.Push(nTradeInfo,BufferStoreCallback);
// 
// 				//如果已初始化完毕，才发送报单事件
// 				Stru_NotifyEvent lEvent(EventTypeTradeRtn,0);
// 				lEvent.AddBufer(&nTradeInfo,sizeof(PlatformStru_TradeInfo));	
// 				CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lEvent);

				////产生持仓变化和资金变化事件		
				//Stru_NotifyEvent lsTrader(EventTypeTrade,0);
				//lsTrader.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
				//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsTrader);

				//产生持仓变化和资金变化事件		
				Stru_NotifyEvent ls(EventTypePosition,0);
				ls.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
				//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(ls);

				Stru_NotifyEvent lsFund(EventTypeFund,0);
				lsFund.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
				//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsFund);

				//cache
				if(mbNeedCache)
				{
					mpCache->PushTrader(nTradeInfo);
                    //TRADE_LOG("PushTrade PushTrader InvestorID=%s InstrumentID=%s CombHedgeFlag=%c CombOffsetFlag=%c\
                    //          Direction=%c LimitPrice=%.4f VolumeTotalOriginal=%d StatusMsg=%c",
                    //          nTradeInfo.InvestorID,nTradeInfo.InstrumentID,nTradeInfo.CombHedgeFlag,nTradeInfo.CombOffsetFlag,
                    //          nTradeInfo.Direction,nTradeInfo.LimitPrice,nTradeInfo.VolumeTotalOriginal,nTradeInfo.StatusMsg);


					//缓存修改的持仓，和资金
					PlatformStru_TradingAccountInfo lFundInfo;
					lpTemp->GetFundInfo(lFundInfo);
					mpCache->CacheFund(lFundInfo);

					vector<PlatformStru_Position> lvscPos;
					lpTemp->GetPositions(nTradeInfo.InstrumentID,lvscPos,true);
					for(int i = 0 ; i < lvscPos.size(); i++)
                    {
						mpCache->CachePosition(lvscPos[i]);
                        TRADE_LOG("PushTrade CachePosition InstrumentID=%s CanCloseTodayVolume=%d CanCloseVolume=%d CanCloseydVolume=%d\
                                  CloseProfit=%.4f CloseTodayOrderVolume=%d CloseOrderVolume=%d CloseVolume=%d InvestorID=%s CanCloseydVolume=%d\
                                  OpenVolume=%d Position=%d TodayPosition=%d YdPosition=%d",
                                  lvscPos[i].InstrumentID,lvscPos[i].CanCloseTodayVolume,lvscPos[i].CanCloseVolume,lvscPos[i].CanCloseydVolume,
                                  lvscPos[i].CloseProfit,lvscPos[i].CloseTodayOrderVolume,lvscPos[i].CloseOrderVolume,lvscPos[i].CloseVolume,
                                  lvscPos[i].InvestorID,lvscPos[i].OpenVolume,lvscPos[i].Position,lvscPos[i].TodayPosition,lvscPos[i].YdPosition);

                    }
				}
			}
		}
		else
		{
			TRADE_LOG("Push an Exsit Trader");
		}

	}
// 	if(!CInterface_SvrLogin::getObj().IsSlaver())
// 	{
// 		//向从服务推送行情
// 		vector<SOCKET> lVecSocket;
// 		CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
// 		for(unsigned int i = 0; i < lVecSocket.size() ; i++)
// 		{
// 			TRADE_LOG("Master push a trader to SLaver %d,trader info %s %s %d %d",lVecSocket[i],nTradeInfo.InstrumentID,nTradeInfo.InvestorID,nTradeInfo.FTID,nTradeInfo.UpdateSeq);
// 			CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
// 				Cmd_TradeDataManager_MasterTrader_Push,
// 				&nTradeInfo,
// 				sizeof(nTradeInfo),
// 				0,
// 				0,
// 				0,
// 				0,
// 				CF_ERROR_SUCCESS,0);
// 		}
// 	}
	return lRet;
}

//检查CTP成交里面的报单编号是不是我们的报单编号
bool CReal_SvrTradeData::CheckTradeOrderSysIDIsOwn(const std::string& nsUserID,
									   const std::string& nsOrderSysID) 
{
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return false;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserID);
	if(mMapUser2TradingData.end() == lIter)
		return false;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->CheckTradeOrderSysIDIsOwn(nsOrderSysID);
}

//获取全部成交单，返回数据项的数量
CF_ERROR  CReal_SvrTradeData::GetUserTradeInfo(const std::string & nUserID,											  
											   std::vector<PlatformStru_TradeInfo>& outData)
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetAllTrades(outData);

	return CF_ERROR_SUCCESS;

}

//获取全部成交单，返回数据项的数量
CF_ERROR  CReal_SvrTradeData::GetAccountTradeInfo(const std::string & nsBrokerID,
									  const std::string & nInvestorID, 
									  std::vector<PlatformStru_TradeInfo>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetAllTrades(outData);
}									


//获取主账户报单成交大于Seq
CF_ERROR  CReal_SvrTradeData::GetAccountOrderAndTraderGESeq(const std::string & nsBrokerID,
												const std::string & nInvestorID,
												int nOrderSeq,
												int nTraderSeq,
												std::vector<PlatformStru_OrderInfo> & nvecOrder,
												std::vector<PlatformStru_TradeInfo>& nvecTrader) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;
	lpTemp->GetOrderAndTradeGeSeq(nOrderSeq,nTraderSeq,nvecOrder,nvecTrader );
	return CF_ERROR_SUCCESS;
}


//获取主账户报单成交大于Seq
CF_ERROR  CReal_SvrTradeData::GetUserOrderAndTraderGESeq(const std::string & nInvestorID,
											 int nOrderSeq,
											 int nTraderSeq,
											 std::vector<PlatformStru_OrderInfo> & nvecOrder,
											 std::vector<PlatformStru_TradeInfo>& nvecTrader) 
{

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nInvestorID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	lpTemp->GetOrderAndTradeGeSeq(nOrderSeq,nTraderSeq,nvecOrder,nvecTrader );
	return CF_ERROR_SUCCESS;
}
//获取持仓汇总，返回数据项的数量
CF_ERROR  CReal_SvrTradeData::GetUserPositions(const std::string & nUserID,
											   const std::string & nInstrumentID,
											   std::vector<PlatformStru_Position>& outData,
											   bool bNeedZeroVolumePosition ) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositions(nInstrumentID,outData, bNeedZeroVolumePosition );
	
}


//获取总持仓量
CF_ERROR CReal_SvrTradeData::GetUserPositionVolume(const std::string & nUserID,
									   const std::string & nInstrumentID,
									   int & nVolume) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionVolume(nInstrumentID,nVolume);
}

//获取持仓汇总，
CF_ERROR  CReal_SvrTradeData::GetAccountPositions(const std::string & nsBrokerID,
									  const std::string & nInvestorID,
									  const std::string & nInstrumentID,
									  std::vector<PlatformStru_Position>& outData,
									  bool bNeedZeroVolumePosition ) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetPositions(nInstrumentID,outData, bNeedZeroVolumePosition );
}		


//获取持仓明细，返回数据项的数量
CF_ERROR   CReal_SvrTradeData::GetUserPositionDetail(const std::string & nUserID,
													 const std::string & nInstrumentID,
													 std::vector<PlatformStru_PositionDetail>& outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionDetail(nInstrumentID,outData);

	
}

//获取持仓明细，返回对应的key的数量
CF_ERROR   CReal_SvrTradeData::GetUserPositionDetail(const std::string & nUserID,
										const PositionDetailKey & nKey,
										PlatformStru_PositionDetail& outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionDetail(nKey,outData);
}
//获取持仓明细
//成功返回CF_ERROR_SUCCESS
CF_ERROR  CReal_SvrTradeData::GetAccountPositionDetail(const std::string & nsBrokerID,
										   const std::string & nInvestorID,
										   const std::string & nInstrumentID,
										   std::vector<PlatformStru_PositionDetail>& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetPositionDetail(nInstrumentID,outData);
}		


//获取持仓明细，返回对应的key的持仓明细
//成功返回CF_ERROR_SUCCESS
CF_ERROR  CReal_SvrTradeData::GetAccountPositionDetail(const std::string & nsBrokerID,
										   const std::string & nInvestorID,
										   const PositionDetailKey & nKey,
										   PlatformStru_PositionDetail& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;


	return lpTemp->GetPositionDetail(nKey,outData);
}		





//获取组合持仓，返回数据项的数量
CF_ERROR  CReal_SvrTradeData::GetUserPositionComb(const std::string & nUserID,
												  const std::string & nInstrumentID,
												  std::vector<PlatformStru_Position>& outData)
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionComb(nInstrumentID,outData);


}






//获取组合持仓明细，返回数据项的数量
CF_ERROR  CReal_SvrTradeData::GetUserPositionCombDetail(const std::string & nUserID,
														const std::string & nInstrumentID,
														std::vector<PlatformStru_PositionDetailComb>& outData)
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetPositionCombDetail(nInstrumentID,outData);

	
}

//获取交易员资金信息
CF_ERROR  CReal_SvrTradeData::GetUserFundInfo(const std::string & nUserID
											  ,PlatformStru_TradingAccountInfo& outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetFundInfo(outData);
}

//获取委托交易帐号资金信息
CF_ERROR   CReal_SvrTradeData::GetAccountFundInfo(const std::string & nsBrokerID,
									 const std::string & nInvestorID,
									 PlatformStru_TradingAccountInfo& outData) 
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	BrokerAccountKey lKey(nsBrokerID.c_str(),nInvestorID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceAccountOp * lpTemp = lIter->second;
	return lpTemp->GetFundInfo(outData);
}		

CF_ERROR  CReal_SvrTradeData::GetUserInitFund(const std::string & nUserID,
						  double& outData) 
{

	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;

	IInterfaceUserOp * lpTemp = lIter->second;
	return lpTemp->GetFirstFund(outData);
	
}

//获取当前交易日的合约
CF_ERROR  CReal_SvrTradeData::GetCurrentDayAlInstrumentInfo(
	std::vector<PlatformStru_InstrumentInfo> & outData) 
{
	//if(GetTraderDataStatus() == TradeDataStatusInitialing
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	//{
	//	TRADE_LOG("error Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}
	//获取当前交易日读数据库
	std::string lsTradingDay;
	GetCurrentTradingDay(lsTradingDay);
	if(lsTradingDay.empty())
	{
		TRADE_LOG("no tradingday,discard this call");
		return CF_ERROR_TRADEDATA_NOTRADINGDAY;
	}
	//装载结算行情

	return 0/*mpDBOpr->LoadTodayInstrument(lsTradingDay,outData)*/;
	
}


//获取所有委托交易账号的手续费率
CF_ERROR CReal_SvrTradeData::GetAllCommissionRate(
	std::vector<PlatformStru_InstrumentCommissionRate> & outData)
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	//m_mutexMapAccount.lock();
	CSafeRwLockRead lLock(mrwLockMapAccount);
	outData.clear();
	_MapAccountData::iterator lIter =  mMapAccountData.begin();

	while(mMapAccountData.end() != lIter)	
	{		
		std::set<std::string> lInstrumentStringSet;
		CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
		std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
		while(lIterSet != lInstrumentStringSet.end())
		{		
			PlatformStru_InstrumentCommissionRate lTempInfo;			
			if(lIter->second != NULL && 0 == lIter->second->GetCommissionRate(*lIterSet,lTempInfo))
				outData.push_back(lTempInfo);
			lIterSet++;
		}
		lIter++;
	}
	

	return CF_ERROR_SUCCESS;
}

//获取所有委托交易账号的保证金率
CF_ERROR CReal_SvrTradeData::GetAllMarginRate(
	std::vector<PlatformStru_InstrumentMarginRate>  & outData)
{

	//if(GetTraderDataStatus() == TradeDataStatusInitialing 
	//	|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
	//	|| GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Init Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lLock(mrwLockMapAccount);

	outData.clear();

	_MapAccountData::iterator lIter =  mMapAccountData.begin();

	while(mMapAccountData.end() != lIter)	
	{		
		std::set<std::string> lInstrumentStringSet;
		CReal_SvrTradeData::getObj().GetInstrumentList(lInstrumentStringSet);
		std::set<std::string>::iterator lIterSet = lInstrumentStringSet.begin();
		while(lIterSet != lInstrumentStringSet.end())
		{
			PlatformStru_InstrumentMarginRate lTempInfo;			
			if(lIter->second != NULL && 0 == lIter->second->GetMarginRate(*lIterSet,lTempInfo))
				outData.push_back(lTempInfo);
			lIterSet++;
		}
		lIter++;
	}

	return CF_ERROR_SUCCESS;
}
//获取所有的结算价
CF_ERROR  CReal_SvrTradeData::GetAllSettlementPrice(
	std::map<std::string,PlatformStru_DepthMarketData> & outData) 
{
	//if(GetTraderDataStatus() == TradeDataStatusInitialing 
	//	|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
	//	|| GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Init Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeLock lLock(mMutextSettlementQuot);
	outData.clear();
	outData = mmapSettlementQuot;
	return CF_ERROR_SUCCESS;
}

//获取当前交易日的结算价
CF_ERROR  CReal_SvrTradeData::GetCurrentDayAllSettlementPrice(
	std::map<std::string,PlatformStru_DepthMarketData> & outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("GetCurrentDayAllSettlementPrice error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	//获取当前交易日读数据库
	std::string lsTradingDay;
	GetCurrentTradingDay(lsTradingDay);
	if(lsTradingDay.empty())
	{
		TRADE_LOG("GetCurrentDayAllSettlementPrice no tradingday,discard this call");
		return CF_ERROR_TRADEDATA_NOTRADINGDAY;
	}
	//装载结算行情
	vector<PlatformStru_DepthMarketData> lVecQuto;	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadQutoInfo("TRADEDATA_Quto",lsTradingDay,lVecQuto))
// 	{
// 		TRADE_LOG("GetCurrentDayAllSettlementPrice LoadTodaySettlementQuto From DataBase Success  %s",lsTradingDay.c_str());
// 		vector<PlatformStru_DepthMarketData>::iterator lIte = lVecQuto.begin();
// 		for(;lIte != lVecQuto.end();lIte++)
// 		{				
// 			//插入到结算行情map中
// 			outData[lIte->InstrumentID] = *lIte;		
// 		}		
// 	}
	return CF_ERROR_SUCCESS;

}
//获取所有的成交
CF_ERROR  CReal_SvrTradeData::GetAllTradeInfo(
	std::map<std::string,std::vector<PlatformStru_TradeInfo>> & outData) 
{

	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("GetAllTradeInfo error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	//_MapUserTradeData * lpMap = GetMapUserDataWithOption(nOption);
	if(mMapUser2TradingData.size() == 0)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;


	outData.clear();
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter)	
	{		
		vector<PlatformStru_TradeInfo> lVec;
		if(lIter->second != NULL && 
			CF_ERROR_SUCCESS == lIter->second->GetAllTrades(lVec))
		{
			//if(lVec.size() > 0)
			{
				//按成交时间排序
				//std::sort(lVec.begin(),lVec.end(),TraderSorterObject);
				outData[lIter->first] = lVec;
				//outData.insert(std::make_pair(lIter->first,lVec));
			}
		}
		
		lIter++;
	}
	return CF_ERROR_SUCCESS;
}

//获取所有的持仓明细
CF_ERROR  CReal_SvrTradeData::GetAllPositionDetail(
	std::map<std::string,std::vector<PlatformStru_PositionDetail>> & outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(mMapUser2TradingData.size() == 0)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;


	outData.clear();

	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter)	
	{		
		vector<PlatformStru_PositionDetail> lVec;
		if(lIter->second != NULL && 
			CF_ERROR_SUCCESS == lIter->second->GetAllPositionDetail(lVec))
		{
			//if(lVec.size() > 0)
			outData[lIter->first] = lVec;
				//outData.insert(std::make_pair(lIter->first,lVec));
		}

		lIter++;
	}
	return CF_ERROR_SUCCESS;
}

//获取所有的持仓
CF_ERROR  CReal_SvrTradeData::GetAllPosition(
	std::map<std::string,std::vector<PlatformStru_Position>> & outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(GetTraderDataStatus() == TradeDataStatusInitialing 
		|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
		|| GetTraderDataStatus() == TradeDataStatusTrading)
	{
		TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}*/
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(mMapUser2TradingData.size() == 0)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;


	outData.clear();

	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter)	
	{		
		vector<PlatformStru_Position> lVec;
		if(lIter->second != NULL && 
			CF_ERROR_SUCCESS == lIter->second->GetAllPosition(lVec))
		{
			//if(lVec.size() > 0)
			outData[lIter->first] = lVec;
				//outData.insert(std::make_pair(lIter->first,lVec));
		}

		lIter++;
	}
	return CF_ERROR_SUCCESS;
}

//获取所有的资金
CF_ERROR  CReal_SvrTradeData::GetAllFundInfo(
	std::map<std::string,PlatformStru_TradingAccountInfo> & outData) 
{
	if(GetTraderDataStatus() == TradeDataStatusInitialing
		|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	//if(GetTraderDataStatus() == TradeDataStatusInitialing 
	//	|| GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK
	//	|| GetTraderDataStatus() == TradeDataStatusSettlemtIniting
	//	|| GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	TRADE_LOG("Init Status,discard this call");
	//	return CF_ERROR_NOINITED;
	//}

	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	if(mMapUser2TradingData.size() == 0)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;


	outData.clear();

	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter)	
	{		
		PlatformStru_TradingAccountInfo lTemp;
		if(lIter->second != NULL &&
			CF_ERROR_SUCCESS == lIter->second->GetFundInfo(lTemp))
		{
			//outData.insert(std::make_pair(lIter->first,lTemp));
			outData[lIter->first] = lTemp;
		}

		lIter++;
	}
	return CF_ERROR_SUCCESS;
}

//开始结算
CF_ERROR  CReal_SvrTradeData::StartSettlement(eSettlementLoadOption nOption,
											  const std::string & nsDay ,
											  bool nbHistorySettlement) 
{

	TRADE_LOG("StartSettlement %s %d %s",nsDay.c_str(),nOption,nbHistorySettlement?"History":"Today");

	//PUSH_LOG_ERROR(Logger,false,true,"开始结算%s",nsDay.c_str());
	eTradeDataStatus  leStaus = GetTraderDataStatus();

	if(leStaus == TradeDataStatusInitialing)
	{
		//TRADE_LOG("this status ,no settlement");

		TRADE_LOG("StartSettlement this status ,no settlement");

		return CF_ERROR_TRADEDATA_SETTMENT_NOINIT;
	}
		
	


	//如果是今天的计算
	if(nbHistorySettlement == false)
	{
		if(GetTraderDataStatus() == TradeDataStatusNoTradingDay)
		{
			//TRADE_LOG("this status ,just history settlement");

			TRADE_LOG("StartSettlement this status ,just history settlement");
			//PUSH_LOG_ERROR(Logger,false,true,"没有交易日只能做历史结算");
			return CF_ERROR_TRADEDATA_SETTMENT_NOINIT;
		}

		////连续交易状态不能盘中结算
		//if(GetExchangeStatus() == ExchangeStatus_ContinueTrade )
		//{
		//	TRADE_LOG("EXCHANGE_ISTRADE,no settlement");
		//	return CF_ERROR_TRADEDATA_EXCHANGE_ISTRADE;
		//}
		//没有交易日，只能进行历史结算
		std::string lsTempTradingDay;
		GetCurrentTradingDay(lsTempTradingDay);
		if(lsTempTradingDay.empty())
		{
			TRADE_LOG("StartSettlement No TradingDay");
			//PUSH_LOG_ERROR(Logger,false,true,"没有获取到交易日只能做历史结算");
			return CF_ERROR_TRADEDATA_NOTRADINGDAY;
		}

		if(GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK)
		{
			TRADE_LOG("StartSettlement TradeDataStatusLastDaySettlemtNotOK");
			mbDirty = true;
		}
	}
	else
	{		

		//如果是连续交易状态，则不能结算
		/*if(GetExchangeStatus() == ExchangeStatus_ContinueTrade &&
				GetTraderDataStatus() == TradeDataStatusTrading)
		{
			TRADE_LOG("EXCHANGE_ISTRADE,no settlement");
			return CF_ERROR_TRADEDATA_EXCHANGE_ISTRADE;
		}*/
		if(!IsTradingDay(nsDay))
		{
			TRADE_LOG("StartSettlement History Settlement No traydingDay");
			//PUSH_LOG_ERROR(Logger,false,true,"该日不是交易日，不能进行结算");
			return CF_ERROR_TRADEDATA_NOTRADINGDAY;
		}

	}
	
	
	//装载成功，设置为结算状态		
	SetTraderDataStatus(TradeDataStatusSettlemtIniting);

    //向从服务推送结算事件
//     vector<SOCKET> lVecSocket;
//     CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
//     for(unsigned int i = 0; i < lVecSocket.size() ; i++)
//     {
//         TRADE_LOG("Master push a StartSettlement Msg to SLaver %d",lVecSocket[i]);
//         CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
//             CMDID_NotifyStartSettlementToSlave_Push,
//             NULL,
//             0,
//             0,
//             0,
//             0,
//             0,
//             CF_ERROR_SUCCESS,0);
//     }


	//发送同步通知其他模块停止工作
	Stru_NotifyEvent lDataEvent(EventTypeStartSettlement,0);
	//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);

	//是今天的盘中数据结算，
	if(nOption == SettlementLoadOption_TradingDataWithTodayDB 
		&& nbHistorySettlement == false)
	{	

		//如果当前是交易状态，数据是干净的，则直接返回使用
		if(mbDirty == false )
		{		
			TRADE_LOG("StartSettlement Today ettlement with Clean Data no need to load");
			//PUSH_LOG_ERROR(Logger,false,true,"直接使用今天的内存数据进行结算，服务器进入结算状态");
			TRADE_LOG("撤销所有未知和未成交报单");
			CReal_SvrTradeData::getObj().CancelAllUnkonwnOrder();
			CReal_SvrTradeData::getObj().SaveAllTradingData();

			SetTraderDataStatus(TradeDataStatusSettlemting);
			return CF_ERROR_SUCCESS;
		}
	}
	//清理内存
	ResetAll();	

	TRADE_LOG("StartSettlement Settlement Check End");

	//设置数据不干净
	mbDirty = true;

	//从盘中数据库中转载数据
	CF_ERROR lRet = SettlementInitTradeDataWithTradingDB(nOption,nsDay);
	if(lRet == CF_ERROR_SUCCESS)
	{
		TRADE_LOG("StartSettlement Settlement Load End");
		//PUSH_LOG_ERROR(Logger,false,true,"加载交易数据完成，服务器进入结算状态");
		//装载成功，设置为结算状态		
		SetTraderDataStatus(TradeDataStatusSettlemting);
	}
	else
	{
		TRADE_LOG("StartSettlement Settlement Load Faild");
		//PUSH_LOG_ERROR(Logger,false,true,"从数据库加载交易数据失败");
		//装置失败，设置为原来的状态
		SetTraderDataStatus(leStaus);
	}
	//if(nOption == SettlementLoadOption_TradingDataWithHistory ||
	//	nOption == SettlementLoadOption_TradingDataWithTodayDB)
	//{
	//	if(lRet == CF_ERROR_SUCCESS)
	//	{
	//		//装载成功，设置为结算状态		
	//		SetTraderDataStatus(TradeDataStatusSettlemting);
	//	}
	//	else
	//	{
	//		//装置失败，设置为原来的状态
	//		SetTraderDataStatus(leStaus);
	//	}
	//}
	//else
	//{
	//	if(lRet != CF_ERROR_SUCCESS)
	//	{
	//		//装置失败，设置为原来的状态
	//		SetTraderDataStatus(leStaus);
	//	}
	//}

	return lRet;
}



//以盘中数据库初始化交易数据
CF_ERROR CReal_SvrTradeData::SettlementInitTradeDataWithTradingDB(eSettlementLoadOption nOption,
																  const std::string & nsDay) 
{
	if(GetTraderDataStatus() != TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("SettlementInitTradeDataWithTradingDB Not Settlemnt Status,discard this call");
		
		return CF_ERROR_NOINITED;
	}

	//CSafeLock lLock(m_mutexMapUserTraded);

	//初始化所有用户的初始化对象
// 	InitUserMapWithEmpty(mMapUser2TradingData); 

	//获取上一个交易日
	std::string lsLastTradingDay;
	//mpTradingDayManager.GetLastTradingData(nsDay,lsLastTradingDay);

// 	if(!lsLastTradingDay.empty() &&
// 		!mpTradingDayManager.IsSetttemnted(lsLastTradingDay))
// 	{
// 		////设置为上一个交易日结算不完全状态，需要重新结算上一个交易日的数据	
// 		TRADE_LOG("LoadInitData Failed need resettlement");
// 		return CF_ERROR_TRADEDATA_SETTMENT_NOINIT;
// 	}
// 	else
// 	{
// 		//装载日初数据
// 		LoadInitData(lsLastTradingDay);
// 		////设置为上一个交易日结算不完全状态，需要重新结算上一个交易日的数据	
// 		TRADE_LOG("LoadInitData load day is %s",lsLastTradingDay.c_str() );		
// 	}


	//处理合约
	//如果数据库有改日的合约，则读取数据库
	//设置合约和费率的日期
	
	vector<PlatformStru_InstrumentInfo> lVecInstrument;	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayInstrument(nsDay,lVecInstrument))
// 	{
// 		TRADE_LOG("LoadTodayInstrument From DataBase Success  %s",nsDay.c_str());
// 		vector<PlatformStru_InstrumentInfo>::iterator lIte = lVecInstrument.begin();
// 		for(;lIte != lVecInstrument.end();lIte++)
// 		{
// 			SetInstrumentInfo(*lIte);
// 		}
// 		msInstrumentDay = nsDay;
// 		//设置所有用户的		
// 		InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReady_Ready);
// 
// 		//设置合约对象的交易日
// 		ExchangeTime lTime;
// 		lTime.CurTickCount = GetTickCount();
// 		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);
// 		strcpy(lTime.TradingDay,lsTempTradingDay.c_str());
// 		mpPlatformDataInstrument->SetExchangeTime(lTime);
// 
// 	}
// 	else
// 	{
// 		return CF_ERROR_TRADEDATA_SETTMENT_NOTRAING;
// 	}

	//装载结算行情
	vector<PlatformStru_DepthMarketData> lVecQuto;	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadQutoInfo("TRADEDATA_Quto",nsDay,lVecQuto))
// 	{
// 		TRADE_LOG("LoadTodaySettlementQuto From DataBase Success  %s",nsDay.c_str());
// 		vector<PlatformStru_DepthMarketData>::iterator lIte = lVecQuto.begin();
// 		for(;lIte != lVecQuto.end();lIte++)
// 		{
// 			mMutextSettlementQuot.lock();			
// 			//插入到结算行情map中
// 			mmapSettlementQuot[lIte->InstrumentID] = *lIte;
// 			mMutextSettlementQuot.unlock();
// 
// 			mpPlatformDataQuots->SetQuotInfo(*lIte);
// 		}		
// 	}

	//处理手续费率
	//如果数据库有今天的相关账户的手续费率，则读取数据库

	//处理保证金率
	//如果数据库有今天的相关账户的保证金率，则读取数据库

	{
		vector<TradeAccount> lVecAccount;
// 		CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
// 		vector<TradeAccount>::iterator lIter = lVecAccount.begin();
// 		for(;lIter != lVecAccount.end();lIter++)
// 		{
// 			//设置初始化数据
// 			bool lbCommisstionLoadSuccess = false;
// 			bool lbMarginRatesLoadSuccess = false;
// 			BrokerInfo lBroker;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID,lBroker);
// 			BrokerAccountKey lKey(lBroker.szBrokerCode,lIter->szTradeAccount);			
// 
// 			IInterfaceAccountOp * lpTemp = NULL;
// 			if(mMapAccountData.find(lKey) == mMapAccountData.end())
// 			{
// 				lpTemp = IFactoryAccountOp::GetFactory()->CreateAccountOp();
// 				mMapAccountData.insert(std::make_pair(lKey,lpTemp));
// 			}
// 			else
// 			{
// 				lpTemp = mMapAccountData.find(lKey)->second;
// 			}
// 
// 			vector<PlatformStru_InstrumentCommissionRate> lVecCommissionRate;	
// 			//如果装置成功，并且合约的个数不为0 并且费率的个数和合约的个数相等的情况下才算成功
// 			if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayCommission(lBroker.szBrokerCode, lIter->szTradeAccount,nsDay,lVecCommissionRate) )
// 			{
// 				TRADE_LOG("LoadTodayCommission From DataBase Success:%s AccountID %s",
// 					lIter->szTradeAccount,nsDay.c_str());
// 				lbCommisstionLoadSuccess = true;
// 				vector<PlatformStru_InstrumentCommissionRate>::iterator lIte = lVecCommissionRate.begin();
// 				for(;lIte != lVecCommissionRate.end();lIte++)
// 				{
// 					SetCommissionRate(*lIte);
// 				}		
// 				//设置状态
// 				_MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);
// 				if(mMapAccountData.end() != lIterReady)
// 				{
// 					lIterReady->second->SetCommissionMarginReady(CommissionMargin_ComissionReady);
// 				/*	int lnTemp = (int)lIterReady->second;
// 					lnTemp |= CommissionMargin_ComissionReady;
// 					lIterReady->second = (eCommissionMarginReady)lnTemp;*/
// 				}
// 			}
// 			else
// 			{
// 				TRADE_LOG("LoadTodayCommission From DataBase Faild %s AccountID %s",
// 					lIter->szTradeAccount,nsDay.c_str());
// 			}
// 
// 
// 			//去装载保证金率
// 			//如果装置成功，并且合约的个数不为0 并且费率的个数和合约的个数相等的情况下才算成功
// 			vector<PlatformStru_InstrumentMarginRate> lVecMarginRate;	
// 			if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayMarginRate(lBroker.szBrokerCode,lIter->szTradeAccount,nsDay,lVecMarginRate) )
// 			{
// 				TRADE_LOG("LoadTodayMarginRate From DataBase Success:%s AccountID %s",lIter->szTradeAccount,nsDay.c_str());
// 				lbMarginRatesLoadSuccess = true;
// 				vector<PlatformStru_InstrumentMarginRate>::iterator lIte = lVecMarginRate.begin();
// 				for(;lIte != lVecMarginRate.end();lIte++)
// 				{
// 					SetMarginRate(*lIte);
// 				}	
// 				//设置状态
// 				_MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);
// 				if(mMapAccountData.end() != lIterReady)
// 				{
// 					lIterReady->second->SetCommissionMarginReady(CommissionMargin_MarginReady);
// 				/*	int lnTemp = (int)lIterReady->second;
// 					lnTemp |= CommissionMargin_MarginReady;
// 					lIterReady->second = (eCommissionMarginReady)lnTemp;*/
// 				}
// 				
// 			}
// 			else
// 			{
// 				TRADE_LOG("LoadTodayCommission From DataBase Faild %s AccountID %s",
// 					lIter->szTradeAccount,nsDay.c_str());
// 			}			
// 
// 		}
	}

	//分派合约和费率对象到每个用户对象中
	{
		//设置合约到主帐号对象中
		SetPlatformObjectToAllAccount();

		//设置合约对象给所有用户的对象
		SetPlatformObjectToAllUser(mMapUser2TradingData);

		//指派帐号对应的费率对象所有用户的对象
		DispathRateToAllUser(mMapUser2TradingData);

	}

	//分派日初数据到每个用户对象中
	{
		
// 		vector<TradeAccount> lVecAccount;
// 		CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
// 		vector<TradeAccount>::iterator lIterAcc = lVecAccount.begin();
// 		for(;lIterAcc != lVecAccount.end();lIterAcc++)
// 		{
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIterAcc->nBrokerID, lBrokeInfo);		
// 			DispathInitialDataToUserWithAccount(lBrokeInfo.szBrokerCode,lIterAcc->szTradeAccount,mMapUser2TradingData);
// 		
// 		}	
	}

	


	//分派当天的出入金到用户对象
	//DispathFundInOut(nsDay,mMapUser2TradingData);

	//分派盘中的数据到每个用户对象中
// 	if(nOption != SettlementLoadOption_TradingDataWithCTPSett)
// 	{
// 		CSafeRwLockRead lrlock(mrwLockMapUserTrading);
// 
// 		vector<UserInfo> lVecUser;
// 		CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUser);
// 		vector<UserInfo>::iterator lIter = lVecUser.begin();
// 		for(;lIter != lVecUser.end();lIter++)
// 		{
// 			if(lIter->nUserType == USER_TYPE_TRADER)
// 			{
// 				_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(lIter->szAccount);
// 				if(lIterUser != mMapUser2TradingData.end())
// 				{
// 					//装载成交
// 					vector<PlatformStru_TradeInfo> lTradeVec;
// 					mpDBOpr->LoadTodayUserTrades("TRADEDATA_TRADES",lIter->szAccount,nsDay,lTradeVec);
// 					if(lTradeVec.size() == 0)
// 					{
// 						PlatformStru_TradeInfo lTemp;
// 						lIterUser->second->QryTradeReach(lTemp,-1,true);
// 					}
// 					else
// 					{
// 						bool bLast = false;
// 						for(unsigned int i = 0;i < lTradeVec.size();i++ )
// 						{	
// 							if(i == lTradeVec.size() - 1) bLast = true;
// 							strcpy(lTradeVec[i].Account,lTradeVec[i].InvestorID);
// 							lIterUser->second->QryTradeReach(lTradeVec[i],0,bLast);
// 						}
// 					}
// 					////装载报单
// 					//vector<PlatformStru_OrderInfo> lOrderVec;
// 					//mpDBOpr->LoadTodayUserOrders("TRADEDATA_ORDERS",lIter->szAccount,nsDay,lOrderVec);
// 					//if(lOrderVec.size() == 0)
// 					//{
// 					//	PlatformStru_OrderInfo lTemp;
// 					//	lIterUser->second->QryOrderReach(lTemp,-1,true);
// 					//}
// 					//else
// 					//{
// 					//	bool bLast = false;
// 					//	for(unsigned int i = 0;i < lOrderVec.size();i++ )
// 					//	{	
// 					//		if(i == lOrderVec.size() - 1) bLast = true;
// 					//		strcpy(lOrderVec[i].szAccount,lOrderVec[i].InvestorID);
// 					//		lIterUser->second->QryOrderReach(lOrderVec[i],0,bLast);
// 					//	}
// 					//}//else
// 
// 				}//if(lIterUser != mMapUser2TradedData.end())
// 			}//if(lIter->nUserType == USER_TYPE_TRADER)
// 		}//for(;lIter != lVecUser.end();lIter++)
// 	}//if(nOption != SettlementLoadOption_TradingDataWithCTPSett)
// 	else
// 	{	
// 		//从ctp结算后，数据就脏了
// 		mbDirty = true;
// 
// 		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
// 		for(;lIter != mMapUser2TradingData.end();lIter++)
// 		{
// 			lIter->second->SetUserCTPStatus(ACCOUNT_STATUS_UnInit);
// 			//lIter->second->SetEqualUserInitStatus(USER_STATUS_UnInit);
// 		}
// 		//发送登录结算服务器命令
// 		Stru_NotifyEvent lDataEvent(EventTypeNeedAllLoginSettlement,0);		
// 		CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 
// 		vector<TradeAccount> lVecAccount;
// 		CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
// 		vector<TradeAccount>::iterator lIterAcc = lVecAccount.begin();
// 		for(;lIterAcc != lVecAccount.end();lIterAcc++)
// 		{
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIterAcc->nBrokerID, lBrokeInfo);		
// 
// 			//发送查成交的命令
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryTrader,0);
// 			BrokerAccountKey lsBrokerAccount(lBrokeInfo.szBrokerCode,lIterAcc->szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 			
// 		}	
// 		//ctp查成交的情况，设置定时check状态
// 		int lnCount = 0;
// 		
// 		do 
// 		{
// 			int lnFinishNum = 0;
// 			Sleep(1000);
// 			_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
// 			for(;lIter != mMapUser2TradingData.end();lIter++)
// 			{
// 				int nUserID = -1;
// 				vector<UserInfo> lVecUeser;
// 				CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 				for(unsigned int i = 0;i < lVecUeser.size();i++)
// 				{
// 					if(strcmp(lVecUeser[i].szAccount,lIter->first.c_str()) == 0)
// 					{
// 						nUserID = lVecUeser[i].nUserID;
// 						break;
// 					}
// 				}
// 				TradeAccount tradeAccount;
// 				if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))			
// 				{
// 					TRADE_LOG("SettlementInitTradeDataWithTradingDB User %s Finished with Empty Trader Account",lIter->first.c_str());
// 					lnFinishNum++;
// 				}
// 				else if(lIter->second->GetUserInitStatus() & USER_STATUS_InitQryTradesReady)
// 				{
// 					TRADE_LOG("SettlementInitTradeDataWithTradingDB Uesr %s qry trader success,we need success it",lIter->first.c_str());
// 					//一个用户的查完了
// 					lnFinishNum++;
// 					PushEventToAccountWithUser(EventTypeNeedLogoffSettlement,lIter->first);					
// 				}
// 				else
// 				{
// 					//在任何一个状态停留20s以上，则认为结束
// 					double ldifftime = 0.0f;
// 					EnumAccountStatus lAcStatus = lIter->second->GetUserCTPStatus(ldifftime);
// 					//在这3个状态上停留20s的时间，就认为是固定状态
// 					if( (lAcStatus== ACCOUNT_STATUS_CannotConnect ||
// 						lAcStatus == ACCOUNT_STATUS_LoginFailure ||
// 						lAcStatus == ACCOUNT_STATUS_Disconnected) &&
// 						ldifftime > 20)
// 					{
// 						TRADE_LOG("SettlementInitTradeDataWithTradingDB Uesr %s sit this status %d too long,we need failed it",lIter->first.c_str(),lAcStatus);
// 						lnFinishNum++;
// 						PushEventToAccountWithUser(EventTypeNeedLogoffSettlement,lIter->first);		
// 					}
// 					/*if( ldifftime > 20)
// 					{		
// 						lnFinishNum++;
// 						PushEventToAccountWithUser(EventTypeNeedLogoffSettlement,lIter->first);			
// 					}*/
// 				
// 				}
// 			}
// 
// 			//所有的用户都查完了
// 			if(lnFinishNum == mMapUser2TradingData.size())
// 				break;
// 
// 			lnCount++;
// 		} while (lnCount <= 30*60);
// 
// 	}

	

	//meUserTradedDataWithDBReady = DataReady_Ready;
	return CF_ERROR_SUCCESS;
}

//结束结算
CF_ERROR  CReal_SvrTradeData::EndSettlement(const std::string & nsDay)
{
	if(GetTraderDataStatus() != TradeDataStatusSettlemting)
	{
		TRADE_LOG("SettlementInitTradeDataWithTradingDB Not Settlemnt Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	/*_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
	for(;lIter != mMapUser2TradingData.end();lIter++)
		lIter->second->;*/

	//PUSH_LOG_ERROR(Logger,false,true,"结算完成，服务器进入结算完成状态");
	SetTraderDataStatus(TradeDataStatusSettlemtEnd);

	//结算日期落地
// 	if(!nsDay.empty())
// 		mpTradingDayManager.AddSettlementedDay(nsDay);
// 	
// 
// 	//发送同步通知其他模块停止工作
// 	Stru_NotifyEvent lDataEvent(EventTypeEndSettlement,0);
// 	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 
// 	////检查是否还有未结算的交易日
// 	std::string lsOldestInitDayWithNoSett;
// 	mpTradingDayManager.HasInitTradingDayWithNoSettlement(lsOldestInitDayWithNoSett);
// 	if(!lsOldestInitDayWithNoSett.empty())
// 	{		
// 		SetCurrentTradingDay(lsOldestInitDayWithNoSett,true);
// 		PUSH_LOG_ERROR(Logger,false,true,"结算完成，仍然有待结算的交易日");
// 	//	SetTraderDataStatus(TradeDataStatusLastDaySettlemtNotOK);
// 		TRADE_LOG("EndSettlement with no settlement is %s, set trading status to TradeDataStatusLastDaySettlemtNotOK ",\
// 			lsOldestInitDayWithNoSett.c_str());
// 	}
// 	else
// 	{
// 		std::string lsNewestSettlementedDay;
// 		mpTradingDayManager.GetNewestSettlementDay(lsNewestSettlementedDay);
// 		SetCurrentTradingDay(lsNewestSettlementedDay,true);
// 		//	SetTraderDataStatus(TradeDataStatusLastDaySettlemtNotOK);
// 		TRADE_LOG("EndSettlement with no day need to settlement");
// 	}


	return CF_ERROR_SUCCESS;
}

//设置修改后的手续费率和保证金率，并触发成交和持仓和资金的计算
CF_ERROR  CReal_SvrTradeData::SetModifiedCommissionAndMargin( std::vector<PlatformStru_InstrumentCommissionRate> & inCommssion,
												  std::vector<PlatformStru_InstrumentMarginRate> & inMargin) 
{
	if(GetTraderDataStatus() != TradeDataStatusSettlemting)
	{
		TRADE_LOG("Not Settlemnt Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	//界面修改了费率后，数据就脏了
	mbDirty = true;

	CSafeRwLockRead lwlock(mrwLockMapAccount);
	//定义记录修改的委托交易账号
	std::set<BrokerAccountKey> lModifySet;
	//重新设置费率
	for(unsigned int i = 0; i < inCommssion.size();i++)
	{
		BrokerAccountKey lKey(inCommssion[i].BrokerID,inCommssion[i].InvestorID);
		_MapAccountData::iterator lIter = mMapAccountData.find(lKey);
		if(lIter != mMapAccountData.end())
			lIter->second->SetCommissionRate(inCommssion[i].InstrumentID,inCommssion[i]);
		lModifySet.insert(lKey);
	}
	for(unsigned int i = 0; i < inMargin.size();i++)
	{
		BrokerAccountKey lKey(inMargin[i].BrokerID,inMargin[i].InvestorID);
		_MapAccountData::iterator lIter = mMapAccountData.find(lKey);
		if(lIter != mMapAccountData.end())
			lIter->second->SetMarginRate(inMargin[i].InstrumentID,inMargin[i]);
		lModifySet.insert(lKey);
	}

	////重新计算修改的委托交易账号的对应的用户的成交数据，做一次全出全入的动作
	//std::set<BrokerAccountKey>::iterator lIterAccount = lModifySet.begin();
	//for(;lIterAccount != lModifySet.end();lIterAccount++)
	//{
	//	vector<UserInfo> lvecUser;
	//	CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(lIterAccount->AccountID,
	//		lIterAccount->BrokerID,lvecUser);
	//	vector<UserInfo>::iterator lIter = lvecUser.begin();
	//	for(;lIter != lvecUser.end();lIter++)
	//	{
	//		_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(lIter->szAccount);
	//		if(lIterUser != mMapUser2TradingData.end())
	//		{
	//			std::vector<PlatformStru_TradeInfo> lTemp;
	//			lIterUser->second->RecalculateTrade(lTemp,false);
	//		}
	//	}	

	//}


	return CF_ERROR_SUCCESS;
}

//设置修改后的成交，并触发成交和持仓和资金的计算
CF_ERROR  CReal_SvrTradeData::SeUserNewTrades(const std::string & nUserName,
											  const std::vector<PlatformStru_TradeInfo> & inTradeInfo)
{
	if(GetTraderDataStatus() != TradeDataStatusSettlemting)
	{
		TRADE_LOG("SeUserNewTrades Not Settlemnt Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	//界面修改了成交后，数据就脏了
	mbDirty = true;
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);

	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nUserName);
	if(lIterUser != mMapUser2TradingData.end())
	{
		//清除改用户的持仓信息
		lIterUser->second->ClearTraderAndPosition();
	
		//重新加入日初持仓
		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);	
        DispathInitialDataToUser(lIterUser->first,lsTempTradingDay,mMapUser2TradingData);

		//重新加入成交
		lIterUser->second->RecalculateTrade(inTradeInfo,true);
	}

	return CF_ERROR_SUCCESS;
}




//下单用的函数，需要在交易状态下执行
CF_ERROR CReal_SvrTradeData::CheckUserInstrumentCommissionAndMaginExist(const std::string & nUserName,
															const std::string & nIstrumentID) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	int lnRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	do 
	{
		CSafeRwLockRead lrlock(mrwLockMapUserTrading);
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserName);
		if(lIter ==  mMapUser2TradingData.end())
			break;
		
		PlatformStru_InstrumentCommissionRate lTempCom;
		if(0 != lIter->second->GetCommissionRate(nIstrumentID,lTempCom))
		{
			lnRet = CF_ERROR_TRADEDATA_NO_COMMISSION;
			break;
		}

		PlatformStru_InstrumentMarginRate lTempMar;
		if(0 != lIter->second->GetMarginRate(nIstrumentID,lTempMar))
		{
			lnRet = CF_ERROR_TRADEDATA_NO_MARGIN;
			break;
		}

		lnRet = CF_ERROR_SUCCESS;

		
	} while (false);
	
	return lnRet;
}

//判断是否可开仓
CF_ERROR  CReal_SvrTradeData::CanOrderOpen(const PlatformStru_OrderInfo & nOrder) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
	//	TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	IInterfaceUserOp * lpUserOp = NULL;
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nOrder.InvestorID);
	if(mMapUser2TradingData.end() != lIter)
	{	
		lpUserOp = lIter->second;
	}

	if(lpUserOp == NULL)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;		

	//如果是开仓，判断可用资金
	PlatformStru_InstrumentInfo lInstruemntInfo;
	if(mpPlatformDataInstrument == NULL ||
		false == mpPlatformDataInstrument->GetInstrumentInfo(nOrder.InstrumentID,lInstruemntInfo))
		return CF_ERROR_TRADEDATA_NO_INSTRUMENTS;

	if(lInstruemntInfo.VolumeMultiple == 0 )
		return CF_ERROR_TRADEDATA_NO_INSTRUMENTS;

	//获取手续费率
	PlatformStru_InstrumentCommissionRate lCommission;
	if(	0 != lpUserOp->GetCommissionRate(nOrder.InstrumentID,lCommission))
		return CF_ERROR_TRADEDATA_NO_COMMISSION;

	if(util::isInvalidValue(lCommission.OpenRatioByMoney) ||
		util::isInvalidValue(lCommission.OpenRatioByVolume))
		return CF_ERROR_TRADEDATA_NO_COMMISSION;

	//获取保证金率
	PlatformStru_InstrumentMarginRate lMargin;
	if(0 != lpUserOp->GetMarginRate(nOrder.InstrumentID,lMargin))
		return CF_ERROR_TRADEDATA_NO_MARGIN;

	if(util::isInvalidValue(lMargin.LongMarginRatioByMoney) ||
		util::isInvalidValue(lMargin.LongMarginRatioByVolume) ||
		util::isInvalidValue(lMargin.ShortMarginRatioByMoney) ||
		util::isInvalidValue(lMargin.ShortMarginRatioByVolume) )
		return CF_ERROR_TRADEDATA_NO_MARGIN;

	//如果是市价，取行情
	double ldbOrderPrice = 0.0;
	if(nOrder.OrderPriceType == THOST_FTDC_OPT_AnyPrice)
	{
		PlatformStru_DepthMarketData lQuto;
		GetQuotInfo(nOrder.InstrumentID,lQuto);
		if(util::isInvalidValue(lQuto.LastPrice))
			return CF_ERROR_TRADEDATA_NO_QUTO;

		ldbOrderPrice = lQuto.LastPrice;
	}
	else
		ldbOrderPrice = nOrder.LimitPrice;

	//判断多头和空头
	double ldbNeedMoney = 0.0;
	if(nOrder.Direction == THOST_FTDC_D_Sell)
	{
		ldbNeedMoney = ldbOrderPrice * lInstruemntInfo.VolumeMultiple * lMargin.ShortMarginRatioByMoney+
			lMargin.ShortMarginRatioByVolume +
			ldbOrderPrice * lInstruemntInfo.VolumeMultiple * lCommission.OpenRatioByMoney +
			lCommission.OpenRatioByVolume;


	}
	else
	{
		ldbNeedMoney = ldbOrderPrice * lInstruemntInfo.VolumeMultiple * lMargin.LongMarginRatioByMoney+
			lMargin.LongMarginRatioByVolume +
			ldbOrderPrice * lInstruemntInfo.VolumeMultiple * lCommission.OpenRatioByMoney +
			lCommission.OpenRatioByVolume;

	}
	ldbNeedMoney *= nOrder.VolumeTotalOriginal;


	//取可用资金
	PlatformStru_TradingAccountInfo lAccountInfo;
    lpUserOp->GetFundInfo(lAccountInfo); 
	if(util::isInvalidValue(lAccountInfo.Available) ||
		lAccountInfo.Available <= 0.0 ||
		lAccountInfo.Available < ldbNeedMoney)
		return CF_ERROR_TRADEDATA_NOT_ENOUGH_FUND;

	return CF_ERROR_SUCCESS;
}

//判断是否可平今
CF_ERROR  CReal_SvrTradeData::CanOrderColseToday(const PlatformStru_OrderInfo & nOrder)
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
		//TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	IInterfaceUserOp * lpUserOp = NULL;
	_MapUserTradeData::iterator lIterUser =  mMapUser2TradingData.find(nOrder.InvestorID);
	if(mMapUser2TradingData.end() != lIterUser)
	{	
		lpUserOp = lIterUser->second;
	}

	if(lpUserOp == NULL)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;	

	std::vector<PlatformStru_Position> lVecPosition;
	TThostFtdcVolumeType lVol = 0;
	lpUserOp->GetPositions(nOrder.InstrumentID,lVecPosition);
	std::vector<PlatformStru_Position>::iterator lIter = lVecPosition.begin();
	while(lIter != lVecPosition.end())
	{
		if( lIter->HedgeFlag == nOrder.CombHedgeFlag[0] &&
			( ( lIter->PosiDirection == THOST_FTDC_PD_Long && nOrder.Direction == THOST_FTDC_D_Sell) ||
			( lIter->PosiDirection == THOST_FTDC_PD_Short && nOrder.Direction == THOST_FTDC_D_Buy)) )
		{
			lVol = lIter->CanCloseTodayVolume ;
			break;
		}
		lIter++;
	}

	if(lVol < nOrder.VolumeTotalOriginal)
		return CF_ERROR_TRADEDATA_NOT_ENOUGH_VOLOMN; //

	return CF_ERROR_SUCCESS;

}

//判断是否可平仓
CF_ERROR  CReal_SvrTradeData::CanOrderColse(const PlatformStru_OrderInfo & nOrder)
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
		//TRADE_LOG("Init Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	IInterfaceUserOp * lpUserOp = NULL;
	_MapUserTradeData::iterator lIterUser =  mMapUser2TradingData.find(nOrder.InvestorID);
	if(mMapUser2TradingData.end() != lIterUser)
	{	
		lpUserOp = lIterUser->second;
	}

	if(lpUserOp == NULL)
		return CF_ERROR_TRADEDATA_NO_USER_DATA;	

	std::vector<PlatformStru_Position> lVecPosition;
	TThostFtdcVolumeType lVol = 0;
	lpUserOp->GetPositions(nOrder.InstrumentID,lVecPosition);
	std::vector<PlatformStru_Position>::iterator lIter = lVecPosition.begin();
	while(lIter != lVecPosition.end())
	{
		if( lIter->HedgeFlag == nOrder.CombHedgeFlag[0] &&
			( ( lIter->PosiDirection == THOST_FTDC_PD_Long && nOrder.Direction == THOST_FTDC_D_Sell) ||
			( lIter->PosiDirection == THOST_FTDC_PD_Short && nOrder.Direction == THOST_FTDC_D_Buy)) )
		{
			lVol = lIter->CanCloseVolume ;
			break;
		}
		lIter++;
	}

	if(lVol < nOrder.VolumeTotalOriginal)
		return CF_ERROR_TRADEDATA_NOT_ENOUGH_VOLOMN; //

	return CF_ERROR_SUCCESS;
}




//出入金
CF_ERROR CReal_SvrTradeData::FundInOut(const sFundInOut& nFundInOut) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading )
	{
	//	TRADE_LOG("error Status,discard this call");
		return CF_ERROR_TRADEDATA_FUNDINOUT_DENY;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nFundInOut.mUserID);
	if(mMapUser2TradingData.end() == lIter)
		return CF_ERROR_TRADEDATA_FUNDIN_DENY_NOINIT;

	IInterfaceUserOp * lpTemp = lIter->second;
	CF_ERROR lRet = lpTemp->UserFundInOut(nFundInOut);
// 	if(lRet == CF_ERROR_SUCCESS && mpDBOpr)
// 	{
// 
// 		//产生资金变化的事件
// 		Stru_NotifyEvent lsFund(EventTypeFund,0);
// 		lsFund.AddBufer((void*)lIter->first.c_str(),lIter->first.length() );	
// 		CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lsFund);			
// 
// 		if(!CInterface_SvrLogin::getObj().IsSlaver())
// 		{
// 			double ldbVal = 0.0f;
// 			lpTemp->GetFirstFund(ldbVal);
// 			//存用户的初始资金
// 			mpDBOpr->WriteInitFund("TradeData_InitFund",lIter->first,ldbVal);
// 
// 			//存出入金
// 			mpDBOpr->WriteFundInOut(nFundInOut);
// 			//存资金		
// 			PlatformStru_TradingAccountInfo lTempTrade;
// 			if(CF_ERROR_SUCCESS == lpTemp->GetFundInfo(lTempTrade))
// 			{			
// 				std::string lsTempTradingDay;
// 				GetCurrentTradingDay(lsTempTradingDay);
// 				mpDBOpr->WriteFundInfos("TradeData_FundInfo",lIter->first,lsTempTradingDay,lTempTrade);
// 			}
// 		}


// 	}
	return lRet;


}
//查询历史出入金
CF_ERROR CReal_SvrTradeData::QryHistroyFundInOut(const sQryFundInOut & nQryFund,
									 std::vector<sFundInOut> & nvecFund) 
{
	return 0/*mpDBOpr->LoadQryFundInOut(nQryFund,nvecFund)*/;
}
//设置用户初始资金 策略本地调试 
CF_ERROR CReal_SvrTradeData::SetUserInitFund(const double dInitFund)
{
    _MapUserTradeData::iterator lIterUser =  mMapUser2TradingData.begin();
    for(;lIterUser != mMapUser2TradingData.end();lIterUser++)
    {
        lIterUser->second->SetInitFund(dInitFund);
    }

    return CF_ERROR_SUCCESS;
}


//问询函数
//check 用户是否能够进行登录和注销
CF_ERROR CReal_SvrTradeData::ResponseToAsk(const eAskType& neDataType,
								 const eAskOperationType& neChangeType,
								 const int nllDataID,
								 const char * npDataBuf)
{
	CF_ERROR lRet = CF_ERROR_LOGIN_USER_DATA_NOTREADY;
	
// 	if(neDataType == AskType_Login)
// 	{
// 		//登录事件
// 		if(neChangeType == AskOperationTypeAdd)
// 		{  
// 			int lnUserID = (int)npDataBuf;
// 			if(lnUserID == SLAVE_USER_ID)
// 			{
// 				//从服务器登陆主服务，主服务必须在交易状态
// 				if(GetTraderDataStatus() != TradeDataStatusTrading )
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INIT;
// 
// 				return CF_ERROR_SUCCESS;
// 			}
// 
// 			UserInfo lUserInfo;
// 			if(false == CInterface_SvrUserOrg::getObj().GetUserByID(lnUserID,lUserInfo) )
// 				return CF_ERROR_LOGIN_INVALIDE_USERID;
// 			if(lUserInfo.nUserType == USER_TYPE_TRADER)
// 			{				
// 
// 				if(GetTraderDataStatus() == TradeDataStatusInitialing ||
// 					GetTraderDataStatus() == TradeDataStatusNoTradingDay)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INIT;
// 				if(GetTraderDataStatus() == TradeDataStatusLastDaySettlemtNotOK )
// 					return CF_ERROR_LOGIN_TADRE_STATUS_NOSETT;
// 				if(GetTraderDataStatus() == TradeDataStatusSettlemting ||
// 					GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_SETT;
// 
// 				if(GetTraderDataStatus() == TradeDataStatusSettlemtEnd)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_END;
// 
// 				if(GetTraderDataStatus() == TradeDataStatusInitFail)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INITFAIL;
// 
// 				if(npDataBuf)
// 				{					
// 					if(GetUserInitStatus(lnUserID) == USER_STATUS_Inited)
// 					{
// 						lRet = CF_ERROR_SUCCESS;
// 					}
// 				}
// 			}
// 			else if(lUserInfo.nUserType == USER_TYPE_MANAGE)
// 			{
// 				if(GetTraderDataStatus() == TradeDataStatusInitialing)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INIT;
// 
// 
// 				lRet = CF_ERROR_SUCCESS;
// 			}
// 			else if(lUserInfo.nUserType == USER_TYPE_RISK_CONTROL ||
// 				lUserInfo.nUserType == USER_TYPE_VIEWER)
// 			{
// 				if(GetTraderDataStatus() == TradeDataStatusInitialing)
// 					return CF_ERROR_LOGIN_TADRE_STATUS_INIT;
// 
// 				lRet = CF_ERROR_SUCCESS;
// 			}
// 			
// 		}

// 	}	
// 	else if(neDataType == AskType_UserAndTradeAccount)
// 	{
// 		//删除和修改一个子账户
// 		if(neChangeType == AskOperationTypeDelete || neChangeType == AskOperationTypeUpdate)
// 		{
// 			UserAndTradeAccountRelation * ltempRelation = (UserAndTradeAccountRelation*)(npDataBuf);
// 			if(GetTraderDataStatus() != TradeDataStatusSettlemtEnd && 
// 				GetTraderDataStatus() != TradeDataStatusNoTradingDay &&
// 				GetTraderDataStatus() != TradeDataStatusInitFail &&
// 				GetTraderDataStatus() != TradeDataStatusLastDaySettlemtNotOK)
// 				return CF_ERROR_TRADEDATA_NOTMATCH_STATUS;
// 			else 
// 			{
				//结算后有持仓不能改
// 				UserInfo lInfo;
// 				CInterface_SvrUserOrg::getObj().GetUserByID(ltempRelation->nUserID,lInfo);
// 				if(TestUserHasPosition(lInfo.szAccount))
// 					return CF_ERROR_TRADEDATA_HAS_POSITION;
// 				return CF_ERROR_SUCCESS;
// 			}
// 		}
// 		if(neChangeType == AskOperationTypeAdd)
// 		{
// 			UserAndTradeAccountRelation * ltempRelation = (UserAndTradeAccountRelation*)( npDataBuf);
// 			if(ltempRelation->nTradeAccountID == -1)
// 				return CF_ERROR_SUCCESS;
// 			//初始化状态下不能新增
// 			if(GetTraderDataStatus() == TradeDataStatusInitialing ||
//                GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
// 					return CF_ERROR_TRADEDATA_INITING;
// 
// 			if(GetTraderDataStatus() == TradeDataStatusSettlemting)
// 				return CF_ERROR_LOGIN_TADRE_STATUS_SETT;
// 
// 			lRet = CF_ERROR_SUCCESS;
// 		}
// 	
// 	}
// 	else if(neDataType == AskType_OrgAndStrategy || 
// 			neDataType == AskType_TraderAndProduct ||
// 			neDataType == AskType_UserAndOrg ||
// 			neDataType == AskType_BrokerAndServerGroup ||
// 			neDataType == AskType_BrokerAndTradeAccount ||
// 			neDataType == AskType_BrokerAndServerGroupAddress ||
// 			neDataType == AskType_User ||
// 			neDataType == AskType_Orgnazation ||
// 			neDataType == AskType_Broker ||
// 			neDataType == AskType_ServerGroup ||
// 			neDataType == AskType_ServerAddress ||
// 			neDataType == AskType_FinacialProduct ||
// 			neDataType == AskType_TradeStrategy)
// 	{
// 		//删除和修改一个子账户
// 		if(neChangeType == AskOperationTypeDelete || neChangeType == AskOperationTypeUpdate)
// 		{
// 			
// 			if(GetTraderDataStatus() != TradeDataStatusSettlemtEnd && 
// 				GetTraderDataStatus() != TradeDataStatusNoTradingDay &&
// 				GetTraderDataStatus() != TradeDataStatusInitFail &&
// 				GetTraderDataStatus() != TradeDataStatusLastDaySettlemtNotOK)
// 				return CF_ERROR_TRADEDATA_NOTMATCH_STATUS;
// 			return CF_ERROR_SUCCESS;			
// 		}
// 		if(neChangeType == AskOperationTypeAdd)
// 		{			
// 			//初始化状态下不能新增
// 			if(GetTraderDataStatus() == TradeDataStatusInitialing ||
// 				GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
// 				return CF_ERROR_TRADEDATA_INITING;
// 
// 			if(GetTraderDataStatus() == TradeDataStatusSettlemting)
// 				return CF_ERROR_LOGIN_TADRE_STATUS_SETT;
// 
// 			lRet = CF_ERROR_SUCCESS;
// 		}
// 
// 	}
// 	else if (neDataType == AskType_Account)
// 	{
// 		//删除和修改一个子账户
// 		if(neChangeType == AskOperationTypeDelete || neChangeType == AskOperationTypeUpdate)
// 		{
// 			//int lAccountID = nllDataID;		
			//TradeAccount lAccountInfo;
			//CInterface_SvrBrokerInfo::getObj().GetTradeAccount(lAccountID,lAccountInfo);

			//BrokerInfo lBroker;
			//CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lAccountInfo.nBrokerID,lBroker);

			//BrokerAccountKey  lpKey(lBroker.szBrokerCode,lAccountInfo.szTradeAccount);
			//EnumAccountStatus lsStatus = ACCOUNT_STATUS_UnInit;
			//double ldbTime;
			////只有该账户登陆失败或者不能连接的时候才能删除和修改
			//GetAccountStatus(lAccountInfo.szTradeAccount,lBroker.szBrokerCode,lsStatus,ldbTime);
			//if(lsStatus == ACCOUNT_STATUS_Login )
			//	return CF_ERROR_TRADEDATA_ACCOUNT_ISLOGIN;		
			//else
			//	return CF_ERROR_SUCCESS;
// 		}
// 		if(neChangeType == AskOperationTypeAdd)
// 		{			
// 			//初始化状态下不能新增
// 			if(GetTraderDataStatus() == TradeDataStatusInitialing ||
// 				GetTraderDataStatus() == TradeDataStatusSettlemtIniting)
// 				return CF_ERROR_TRADEDATA_INITING;
// 
// 			if(GetTraderDataStatus() == TradeDataStatusSettlemting)
// 				return CF_ERROR_LOGIN_TADRE_STATUS_SETT;
// 
// 			lRet = CF_ERROR_SUCCESS;
// 		}
// 
// 	}
// 	else if(AskType_UserLoginCheck == neDataType)
// 	{

		/*lRet = CF_ERROR_TRADEDATA_USER_LOGIN;
		int lUserID = nllDataID;
		std::vector<SOCKET> lvecSocket;
		CInterface_SvrLogin::getObj().GetUserSockets(lUserID,lvecSocket);
		if(lvecSocket.size() == 0)
			lRet = CF_ERROR_SUCCESS;*/
// 	}
	return lRet;

}

//获取初始化时间
CF_ERROR CReal_SvrTradeData::GetReinitTime(sReinitTime & nsReinitTime)
{
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);
	
	GetPrivateProfileString(TRADEDATA_KEY,"NightTime","20:30:30",nsReinitTime.mNightTime,sizeof(nsReinitTime.mNightTime),lsPath);
	GetPrivateProfileString(TRADEDATA_KEY,"DayTime","08:30:30",nsReinitTime.mDayTime,sizeof(nsReinitTime.mNightTime),lsPath);
	return CF_ERROR_SUCCESS;
}

//获取初始化时间
CF_ERROR CReal_SvrTradeData::GetSaveTime(sSaveTime & nsSaveTime)
{
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);

	GetPrivateProfileString(TRADEDATA_KEY,"SaveTime1","11:30:30",nsSaveTime.mSaveTime1,sizeof(nsSaveTime.mSaveTime1),lsPath);
	GetPrivateProfileString(TRADEDATA_KEY,"SaveTime2","15:20:30",nsSaveTime.mSaveTime2,sizeof(nsSaveTime.mSaveTime2),lsPath);
	return CF_ERROR_SUCCESS;
}

//设置初始化时间
CF_ERROR CReal_SvrTradeData::SetReinitTime(const sReinitTime & nsReinitTime)
{

	std::string lsNightTime = nsReinitTime.mNightTime;
	std::string lsDayTime = nsReinitTime.mDayTime;
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);

	WritePrivateProfileString(TRADEDATA_KEY,"NightTime",lsNightTime.c_str(),lsPath);
	WritePrivateProfileString(TRADEDATA_KEY,"DayTime",lsDayTime.c_str(),lsPath);
	return CF_ERROR_SUCCESS;
}

//获取交易时间
CF_ERROR CReal_SvrTradeData::GetTradingTime(sTradingTime & nsTradingTime) 
{
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);

	GetPrivateProfileString(TRADEDATA_KEY,"TradingDayStart","08:55:00",nsTradingTime.mTradingDayStart,sizeof(nsTradingTime.mTradingDayStart),lsPath);
	GetPrivateProfileString(TRADEDATA_KEY,"TradingDayEnd","15:15:00",nsTradingTime.mTradingDayEnd,sizeof(nsTradingTime.mTradingDayEnd),lsPath);

	GetPrivateProfileString(TRADEDATA_KEY,"TradingNigthStart","20:00:00",nsTradingTime.mTradingNightStart,sizeof(nsTradingTime.mTradingNightStart),lsPath);
	GetPrivateProfileString(TRADEDATA_KEY,"TradingNightEnd","02:00:00",nsTradingTime.mTradingNightEnd,sizeof(nsTradingTime.mTradingNightEnd),lsPath);
	
	
	return CF_ERROR_SUCCESS;
}


//设置初始化时间
CF_ERROR CReal_SvrTradeData::SetTradingTime(const sTradingTime & nsTradingTime)
{

	std::string lsTradingDayStart = nsTradingTime.mTradingDayStart;
	std::string lsTradingDayEnd = nsTradingTime.mTradingDayEnd;
	std::string lsTradingNightStart = nsTradingTime.mTradingNightStart;
	std::string lsTradingNightEnd = nsTradingTime.mTradingNightEnd;	
	
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,TRADEDATA_CONFIG_FILE);

	WritePrivateProfileString(TRADEDATA_KEY,"TradingDayStart",lsTradingDayStart.c_str(),lsPath);
	WritePrivateProfileString(TRADEDATA_KEY,"TradingDayEnd",lsTradingDayEnd.c_str(),lsPath);
	WritePrivateProfileString(TRADEDATA_KEY,"TradingNigthStart",lsTradingNightStart.c_str(),lsPath);
	WritePrivateProfileString(TRADEDATA_KEY,"TradingNightEnd",lsTradingNightEnd.c_str(),lsPath);

	return CF_ERROR_SUCCESS;
}

////添加一个CTP报单
CF_ERROR CReal_SvrTradeData::QryCTPOrder(PlatformStru_OrderInfo & nOrderInfo,const int ErrID,const bool bIsLast) 
{
	if(GetTraderDataStatus() != TradeDataStatusInitialing)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//向从服务推送行情
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		TRADE_LOG("Master push a QryCTPOrder to SLaver %d",lVecSocket[i]);
	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterQryCTPOrder_Push,
	//			&nOrderInfo,
	//			sizeof(nOrderInfo),
	//			0,
	//			0,
	//			ErrID,
	//			bIsLast,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}
	CSafeRwLockRead lrlock(mrwLockMapAccount);

	//添加到用户管理模块去
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	BrokerAccountKey lKey(nOrderInfo.BrokerID,nOrderInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		lRet = lpTemp->QryOrderReach(nOrderInfo,ErrID,bIsLast);	
	}

	/*if(CInterface_SvrLogin::getObj().IsSlaver())
		mBufferDataStoreCTPOrder.Push(nOrderInfo,BufferStoreCallback);*/
	return CF_ERROR_SUCCESS;
}

//添加一个CTP成交
CF_ERROR CReal_SvrTradeData::QryCTPTrader(PlatformStru_TradeInfo & nTraderInfo,const int ErrID,const bool bIsLast) 
{
	if(GetTraderDataStatus() != TradeDataStatusInitialing)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}

	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//向从服务推送行情
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		TRADE_LOG("Master push a QryCTPTrader to SLaver %d",lVecSocket[i]);
	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterQryCTPTrader_Push,
	//			&nTraderInfo,
	//			sizeof(nTraderInfo),
	//			0,
	//			0,
	//			ErrID,
	//			bIsLast,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}

	CSafeRwLockRead lrlock(mrwLockMapAccount);

	//添加到用户管理模块去
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	BrokerAccountKey lKey(nTraderInfo.BrokerID,nTraderInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		lRet = lpTemp->QryTradeReach(nTraderInfo,ErrID,bIsLast);	
	}


	/*if(!CInterface_SvrLogin::getObj().IsSlaver())
		mBufferDataStoreCTPTrade.Push(nTraderInfo,BufferStoreCallback);*/
	return CF_ERROR_SUCCESS;
}


////添加一个CTP报单
CF_ERROR CReal_SvrTradeData::PushCTPOrder(PlatformStru_OrderInfo & nOrderInfo) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	

	//添加到用户管理模块去
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	BrokerAccountKey lKey(nOrderInfo.BrokerID,nOrderInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		lRet = lpTemp->PushOrder(nOrderInfo);	
	}
	//if(CInterface_SvrLogin::getObj().IsSlaver())
	//	mBufferDataStoreCTPOrder.Push(nOrderInfo,BufferStoreCallback);
	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//向从服务推送行情
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		TRADE_LOG("Master push a CTPOrder to SLaver %d",lVecSocket[i]);

	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterCTPOrder_Push,
	//			&nOrderInfo,
	//			sizeof(nOrderInfo),
	//			0,
	//			0,
	//			0,
	//			0,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}

	return CF_ERROR_SUCCESS;
}

//添加一个CTP成交
CF_ERROR CReal_SvrTradeData::PushCTPTrader(PlatformStru_TradeInfo & nTraderInfo) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	
	CSafeRwLockRead lrlock(mrwLockMapAccount);

	//添加到用户管理模块去
	CF_ERROR lRet = CF_ERROR_TRADEDATA_NO_USER_DATA;
	BrokerAccountKey lKey(nTraderInfo.BrokerID,nTraderInfo.InvestorID);
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);
	if(mMapAccountData.end() != lIter)
	{
		IInterfaceAccountOp * lpTemp = lIter->second;	
		lRet = lpTemp->PushTrader(nTraderInfo);	
	}

	//if(!CInterface_SvrLogin::getObj().IsSlaver())
	//	mBufferDataStoreCTPTrade.Push(nTraderInfo,BufferStoreCallback);

	//if(!CInterface_SvrLogin::getObj().IsSlaver() && GetTraderDataStatus() == TradeDataStatusTrading)
	//{
	//	//向从服务推送行情
	//	vector<SOCKET> lVecSocket;
	//	CInterface_SvrLogin::getObj().GetUserTypeSocket(USER_TYPE_SLAVESERVER,lVecSocket);
	//	for(unsigned int i = 0; i < lVecSocket.size() ; i++)
	//	{
	//		TRADE_LOG("Master push a CTPTrader to SLaver %d",lVecSocket[i]);

	//		CInterface_SvrTcp::getObj().SendPkgData(lVecSocket[i],
	//			Cmd_TradeDataManager_MasterCTPTrader_Push,
	//			&nTraderInfo,
	//			sizeof(nTraderInfo),
	//			0,
	//			0,
	//			0,
	//			0,
	//			CF_ERROR_SUCCESS,0);
	//	}
	//}
	return CF_ERROR_SUCCESS;
}


////添加一个ExecuteIn报单
CF_ERROR CReal_SvrTradeData::PushExecuteInOrder(PlatformStru_OrderInfo & nOrderInfo) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(CInterface_SvrLogin::getObj().IsSlaver())
		mBufferDataStoreExecuteInOrder.Push(nOrderInfo,BufferStoreCallback);*/
	return CF_ERROR_SUCCESS;
}

////添加一个ExecuteOut报单
CF_ERROR CReal_SvrTradeData::PushExecuteOutOrder(PlatformStru_OrderInfo & nOrderInfo) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("Not Trading Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	/*if(CInterface_SvrLogin::getObj().IsSlaver())
		mBufferDataStoreExecuteOutOrder.Push(nOrderInfo,BufferStoreCallback);*/
	return CF_ERROR_SUCCESS;
}


//获取操作数据库的接口
//IInterfaceDBOpr * CReal_SvrTradeData::GetInterfaceDBOpr()
//{
//	return mpDBOpr;
//}
//报盘查询不成功调用
void CReal_SvrTradeData::QryFailed(const BrokerAccountKey & nBrokerAccount,eEventType nEvent)
{
	TRADE_LOG("QryFailed event is %d",(int)nEvent);
	if(nEvent == EventTypeNeedQryInstrument)
	{
		Stru_NotifyEvent lDataEvent(EventTypeNeedQryInstrument,0);
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	}
	else
	{
		Stru_NotifyEvent lDataEvent(nEvent,0);			
		BrokerAccountKey lsBrokerAccount(nBrokerAccount.BrokerID,nBrokerAccount.AccountID);		
		lDataEvent.AddBufer((void*)&nBrokerAccount,sizeof(nBrokerAccount));	
		//CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	}
	return;
}

//获取对应用户对应时间段的持仓明细
CF_ERROR CReal_SvrTradeData::GetPositionDetail_MaxTime(const  std::vector<std::string> nvecAccount, 
										   int LowerOpenTime,int UpperOpenTime,
										   std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail) 
{
	if(GetTraderDataStatus() != TradeDataStatusTrading)
	{
		//TRADE_LOG("error Status,discard this call");
		return CF_ERROR_NOINITED;
	}
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	for(unsigned int i = 0; i < nvecAccount.size();i++)
	{		
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nvecAccount[i]);
		if(mMapUser2TradingData.end() != lIter)
		{
			std::vector<PlatformStru_PositionDetail> lvecTemp;
			IInterfaceUserOp * lpTemp = lIter->second;
			lpTemp->GetPositionDetail(LowerOpenTime,UpperOpenTime,lvecTemp);
			if(lvecTemp.size() != 0)
				mapDetail[nvecAccount[i]] = lvecTemp;
		}	
	}
	return CF_ERROR_SUCCESS;

}
///***********************************内部操作******************************************************************************/
//
///***********************************内部操作******************************************************************************/
//处理发送查询费率和保证金率
void CReal_SvrTradeData::SendQryCommissionAndMargin()
{
//处理手续费率
	//如果数据库有今天的相关账户的手续费率，则读取数据库
	//遍历有哪些账户费率没有准备好，则发送费率查询事件

	//处理保证金率
	//如果数据库有今天的相关账户的保证金率，则读取数据库
	//遍历有哪些账户费率没有准备好，则发送费率查询事件
	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	TRADE_LOG("SendQryCommissionAndMargin %s",lsTempTradingDay.c_str());

	//去掉可能重复的合约
	std::set<string> lSetInstrumentId;
	mpPlatformDataInstrument->GetInstrumentList(lSetInstrumentId);
// 
// 	vector<TradeAccount> lVecAccount;
// 	CInterface_SvrBrokerInfo::getObj().GetTradeAccounts(lVecAccount);
// 	vector<TradeAccount>::iterator lIter = lVecAccount.begin();
// 	for(;lIter != lVecAccount.end();lIter++)
// 	{
// 		//设置初始化数据
// 		bool lbCommisstionLoadSuccess = false;
// 		bool lbMarginRatesLoadSuccess = false;
// 		BrokerInfo lBroker;
// 		CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID,lBroker);
// 		BrokerAccountKey lKey(lBroker.szBrokerCode,lIter->szTradeAccount);
// 		
// 
// 		vector<PlatformStru_InstrumentCommissionRate> lVecCommissionRate;	
// 		//如果装置成功，并且合约的个数不为0 并且费率的个数和合约的个数相等的情况下才算成功
// 		if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayCommission(lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lVecCommissionRate) &&
// 			lSetInstrumentId.size() != 0 &&
// 			lVecCommissionRate.size() == lSetInstrumentId.size() &&
// 			mbForceInit == false )
// 		{
// 			TRADE_LOG("LoadTodayCommission From DataBase Success:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 		
// 			PUSH_LOG_ERROR(Logger,false,true,"从数据库中加载委托交易帐号%s日期为%s的费率成功",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 
// 			
// 			lbCommisstionLoadSuccess = true;
// 			vector<PlatformStru_InstrumentCommissionRate>::iterator lIte = lVecCommissionRate.begin();
// 			for(;lIte != lVecCommissionRate.end();lIte++)
// 			{
// 				SetCommissionRate(*lIte);
// 			}		
// 			//设置状态
// 			_MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);
// 			if(mMapAccountData.end() != lIterReady)
// 			{
// 				lIterReady->second->SetCommissionMarginReady(CommissionMargin_ComissionReady);
// 		/*		int lnTemp = (int)lIterReady->second;
// 				lnTemp |= CommissionMargin_ComissionReady;
// 				lIterReady->second = (eCommissionMarginReady)lnTemp;*/
// 			}
// 		}
// 		
// 		
// 
// 		//去装载保证金率
// 		//如果装置成功，并且合约的个数不为0 并且费率的个数和合约的个数相等的情况下才算成功
// 		vector<PlatformStru_InstrumentMarginRate> lVecMarginRate;	
// 		if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayMarginRate(lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lVecMarginRate) &&
// 			lSetInstrumentId.size() != 0 &&
// 			lVecMarginRate.size() == lSetInstrumentId.size() &&
// 			mbForceInit == false )
// 		{
// 			TRADE_LOG("LoadTodayMarginRate From DataBase Success:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 		
// 			PUSH_LOG_ERROR(Logger,false,true,"从数据库中加载委托交易帐号%s日期为%s的保证金率成功",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 
// 			lbMarginRatesLoadSuccess = true;
// 			vector<PlatformStru_InstrumentMarginRate>::iterator lIte = lVecMarginRate.begin();
// 			for(;lIte != lVecMarginRate.end();lIte++)
// 			{
// 				SetMarginRate(*lIte);
// 			}	
// 			//设置状态
// 			_MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);
// 			if(mMapAccountData.end() != lIterReady)
// 			{
// 				lIterReady->second->SetCommissionMarginReady(CommissionMargin_MarginReady);
// 				/*int lnTemp = (int)lIterReady->second;
// 				lnTemp |= CommissionMargin_MarginReady;
// 				lIterReady->second = (eCommissionMarginReady)lnTemp;*/
// 			}
// 		}
// 		//else
// 		
// 
// 		//如果保证金率装置成功，需要将费率对象指派到用户对象中去
// 		if(lbMarginRatesLoadSuccess && lbCommisstionLoadSuccess)
// 		{		
// 			TRADE_LOG("LoadToday Commission And Margin From DataBase Success,and send Qry Trade Cmd:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID, lBrokeInfo);
// 
// 
// 			//设置费率对象到对应的用户对象
// 			SetRatesObjectToAccountRelationUserObject(lBrokeInfo.szBrokerCode,lIter->szTradeAccount);
// 
// 
// 			PUSH_LOG_ERROR(Logger,false,true,"装载委托交易帐号%s的日初数据",lIter->szTradeAccount);
// 
// 			//装载日初的数据
// 			DispathInitialDataToUserWithAccount(lBrokeInfo.szBrokerCode,lIter->szTradeAccount,mMapUser2TradingData);
// 
// 
// 			PUSH_LOG_ERROR(Logger,false,true,"发送委托交易帐号%s的查询资金命令",lIter->szTradeAccount);
// 
// 			//发送查询成交的事件
// 			//发送查成交的命令
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryFund,0);
// 			BrokerAccountKey lsBrokerAccount(lBrokeInfo.szBrokerCode,lIter->szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 		}
// 
// 		if(!lbCommisstionLoadSuccess)
// 		{
// 			TRADE_LOG("LoadTodayCommission From DataBase Faild send Qry Commission Cmd:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 			PUSH_LOG_ERROR(Logger,false,true,"从数据库中加载委托交易帐号%s日期为%s的费率失败，发送查询该委托交易帐号的费率查询指令",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID, lBrokeInfo);
// 
// 			//产生一个需要查询合约费率的事件
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryCommission,0);			
// 			BrokerAccountKey lsBrokerAccount(lBrokeInfo.szBrokerCode,lIter->szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 
// 		}
// 		if(!lbMarginRatesLoadSuccess)
// 		{
// 			//产生一个需要查询合约保证金率的事件			
// 			TRADE_LOG("LoadTodayMarginRate From DataBase Faild send Qry Margin Cmd:%s AccountID %s",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 			PUSH_LOG_ERROR(Logger,false,true,"从数据库中加载委托交易帐号%s日期为%s的保证金率失败，发送查询该委托交易帐号的保证金率查询指令",lIter->szTradeAccount,lsTempTradingDay.c_str());
// 
// 			BrokerInfo lBrokeInfo;
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(lIter->nBrokerID, lBrokeInfo);
// 
// 			//产生一个需要查询合约费率的事件
// 			Stru_NotifyEvent lDataEvent(EventTypeNeedQryMargin,0);	
// 			BrokerAccountKey lsBrokerAccount(lBrokeInfo.szBrokerCode,lIter->szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 
// 		}
// 
// 
// 
// 	}
}
_MapUserTradeData* CReal_SvrTradeData::GetMapUserDataWithOption(eSettlementLoadOption nOption)
{
	_MapUserTradeData * lRet = NULL;
//	switch(nOption)
//	{
////	case SettlementLoadOption_TradingDataFormMorning : { lRet = & mMapUser2TradingData;break;}
//	case SettlementLoadOption_TradingDataWithTodayDB : { lRet = & mMapUser2TradedDataWithDB;break;}
//	case SettlementLoadOption_TradingDataWithCTPSett : { lRet = & mMapUser2TradedDataWithCTP;break;}
//	//case SettlementLoadOption_TradingDataWithInit : lRet = & mMapUserInitialData;break;
//	}

	return lRet;
}

void CReal_SvrTradeData::GenerateOrderInfoFromCTPInputOrder(const CThostFtdcInputOrderField& inputOrder,
											PlatformStru_OrderInfo & orderInfo)
{
	memset(&orderInfo, 0, sizeof(orderInfo));
	strcpy(orderInfo.BrokerID, inputOrder.BrokerID);
	strcpy(orderInfo.InvestorID, inputOrder.InvestorID);
	strcpy(orderInfo.InstrumentID, inputOrder.InstrumentID);
	strcpy(orderInfo.OrderRef, inputOrder.OrderRef);
	strcpy(orderInfo.UserID, inputOrder.UserID);
	orderInfo.OrderPriceType = inputOrder.OrderPriceType;
	orderInfo.Direction = inputOrder.Direction;
	strcpy(orderInfo.CombOffsetFlag, inputOrder.CombOffsetFlag);
	strcpy(orderInfo.CombHedgeFlag, inputOrder.CombHedgeFlag);
	orderInfo.LimitPrice = inputOrder.LimitPrice;
	orderInfo.VolumeTotalOriginal = inputOrder.VolumeTotalOriginal;
	orderInfo.TimeCondition = inputOrder.TimeCondition;
	strcpy(orderInfo.GTDDate, inputOrder.GTDDate);
	orderInfo.VolumeCondition = inputOrder.VolumeCondition;
	orderInfo.MinVolume = inputOrder.MinVolume;
	orderInfo.ContingentCondition = inputOrder.ContingentCondition;
	orderInfo.StopPrice = inputOrder.StopPrice;
	orderInfo.ForceCloseReason = inputOrder.ForceCloseReason;
	orderInfo.IsAutoSuspend = inputOrder.IsAutoSuspend;
	strcpy(orderInfo.BusinessUnit, inputOrder.BusinessUnit);
	orderInfo.MinVolume = inputOrder.MinVolume;
	strcpy(orderInfo.Account, inputOrder.InvestorID);
	orderInfo.RequestID = inputOrder.RequestID;
	orderInfo.UserForceClose = inputOrder.UserForceClose;
	
	orderInfo.OrderSubmitStatus = '0';
	orderInfo.bInvalid = false;

}

void CReal_SvrTradeData::GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,PlatformStru_OrderInfo & orderInfo)
{
	memset(&orderInfo, 0, sizeof(orderInfo));
	strcpy(orderInfo.BrokerID, inputOrder.BrokerID);
	strcpy(orderInfo.InvestorID, inputOrder.InvestorID);
	strcpy(orderInfo.InstrumentID, inputOrder.InstrumentID);
	strcpy(orderInfo.OrderRef, inputOrder.OrderRef);
	strcpy(orderInfo.UserID, inputOrder.UserID);
	orderInfo.OrderPriceType = inputOrder.OrderPriceType;
	orderInfo.Direction = inputOrder.Direction;
	strcpy(orderInfo.CombOffsetFlag, inputOrder.CombOffsetFlag);
	strcpy(orderInfo.CombHedgeFlag, inputOrder.CombHedgeFlag);
	orderInfo.LimitPrice = inputOrder.LimitPrice;
	orderInfo.VolumeTotalOriginal = inputOrder.VolumeTotalOriginal;
	orderInfo.TimeCondition = inputOrder.TimeCondition;
	strcpy(orderInfo.GTDDate, inputOrder.GTDDate);
	orderInfo.VolumeCondition = inputOrder.VolumeCondition;
	orderInfo.MinVolume = inputOrder.MinVolume;
	orderInfo.ContingentCondition = inputOrder.ContingentCondition;
	orderInfo.StopPrice = inputOrder.StopPrice;
	orderInfo.ForceCloseReason = inputOrder.ForceCloseReason;
	orderInfo.UserForceClose = inputOrder.UserForceClose;
	orderInfo.IsAutoSuspend = inputOrder.IsAutoSuspend;
	strcpy(orderInfo.BusinessUnit, inputOrder.BusinessUnit);
	orderInfo.MinVolume = inputOrder.MinVolume;
	strcpy(orderInfo.Account, inputOrder.InvestorID);
	orderInfo.RequestID = inputOrder.RequestID;
	orderInfo.OrderSubmitStatus = '0';
	orderInfo.bInvalid = false;
	orderInfo.VolumeTotal = inputOrder.VolumeTotalOriginal;
	orderInfo.troubleMoney = util::GetDoubleInvalidValue();	
	orderInfo.freezeMargin = util::GetDoubleInvalidValue();
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(orderInfo.UpdateTime, sizeof(orderInfo.UpdateTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
}

void CReal_SvrTradeData::GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,PlatformStru_InputOrder & inputOrder)
{
	memset(&inputOrder, 0, sizeof(inputOrder));
	strcpy(inputOrder.BrokerID, orderInfo.BrokerID);
	strcpy(inputOrder.InvestorID, orderInfo.InvestorID);
	strcpy(inputOrder.InstrumentID, orderInfo.InstrumentID);
	strcpy(inputOrder.OrderRef, orderInfo.OrderRef);
	strcpy(inputOrder.UserID, orderInfo.UserID);
	inputOrder.OrderPriceType = orderInfo.OrderPriceType;
	inputOrder.Direction = orderInfo.Direction;
	strcpy(inputOrder.CombOffsetFlag, orderInfo.CombOffsetFlag);
	strcpy(inputOrder.CombHedgeFlag, orderInfo.CombHedgeFlag);
	inputOrder.LimitPrice = orderInfo.LimitPrice;
	inputOrder.VolumeTotalOriginal = orderInfo.VolumeTotalOriginal;
	inputOrder.TimeCondition = orderInfo.TimeCondition;
	strcpy(inputOrder.GTDDate, orderInfo.GTDDate);
	inputOrder.VolumeCondition = orderInfo.VolumeCondition;
	inputOrder.MinVolume = orderInfo.MinVolume;
	inputOrder.ContingentCondition = orderInfo.ContingentCondition;
	inputOrder.StopPrice = orderInfo.StopPrice;
	inputOrder.ForceCloseReason = orderInfo.ForceCloseReason;
	inputOrder.IsAutoSuspend = orderInfo.IsAutoSuspend;
	inputOrder.RequestID = orderInfo.RequestID;
	inputOrder.UserForceClose = orderInfo.UserForceClose;
	strcpy(inputOrder.BusinessUnit, orderInfo.BusinessUnit);
	inputOrder.MinVolume = orderInfo.MinVolume;
	strcpy(inputOrder.strAccount, orderInfo.InvestorID);
	
}
//创建空的子对象并加入map
void CReal_SvrTradeData::CreateEmptyAccountOpAndAddMap(const string & nUserId,_MapUserTradeData & mapUserTradeData)
{
	//CSafeRwLockWrite lwlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mapUserTradeData.find(nUserId);
	if(mapUserTradeData.end() == lIter)
	{
		IInterfaceUserOp * lp = IFactoryUserOp::GetFactory()->CreateUserOp();
		mapUserTradeData.insert(std::make_pair(nUserId,lp));
        lp->SetEqualUserInitStatus(USER_STATUS_Inited);
	}
}

void CReal_SvrTradeData::SetPlatformObjectToAllUser(_MapUserTradeData & mapUserTradeData)
{

	//为用户账户设置合约
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIter =  mapUserTradeData.begin();
	for(;lIter != mapUserTradeData.end();lIter++)
	{
		lIter->second->SetPlatFormInstrument(mpPlatformDataInstrument);
		lIter->second->SetPlatFormQutos(mpPlatformDataQuots);
	}

}
//将合约对象分派给各个主账户的对象
void CReal_SvrTradeData::SetPlatformObjectToAllAccount()
{

	//为用户账户设置合约
	CSafeRwLockRead lrlock(mrwLockMapAccount);
	_MapAccountData::iterator lIter =  mMapAccountData.begin();
	for(;lIter != mMapAccountData.end();lIter++)
	{
		lIter->second->SetPlatFormInstrument(mpPlatformDataInstrument);
		lIter->second->SetPlatFormQutos(mpPlatformDataQuots);
	}

}

//将合约对象分派给各个用户的对象
void CReal_SvrTradeData::DispathRateToAllUser(_MapUserTradeData & mapUserTradeData)
{
	
	CPlatformDataMgr_Rates * lpRate = NULL;
	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	_MapAccountData::iterator lIter =  mMapAccountData.begin();	
	while(mMapAccountData.end() != lIter)	
	{		
		lpRate = lIter->second->GetPlatFormRates();
		if(lpRate != NULL)
		{
			//反向根据nBrokerID 和 nAccountID查用户		//			
			vector<UserInfo> lVecUser;
// 			CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(lIter->first.AccountID,lIter->first.BrokerID,lVecUser);	
// 			vector<UserInfo>::iterator lIter = lVecUser.begin();
// 			for(;lIter != lVecUser.end();lIter++)
// 			{
// 				CSafeRwLockRead lrlock(mrwLockMapUserTrading);
// 				_MapUserTradeData::iterator  lIterUser = mapUserTradeData.find(lIter->szAccount);
// 				if(lIterUser != mapUserTradeData.end())
// 					lIterUser->second->SetPlatFormRates(lpRate);
// 			}		

		}
		lIter++;
	}


	
}
//分派今天的出入金给用户对象
void CReal_SvrTradeData::DispathFundInOut(const std::string & nsDate,_MapUserTradeData & nmapUserTradeData)
{
	//装载出入金
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator lIteFundInOut = nmapUserTradeData.begin();
// 	while(lIteFundInOut != nmapUserTradeData.end())
// 	{
// 		TRADE_LOG("LoadQryFundInOut From DataBase CMD:day %s",nsDate.c_str());
// 		vector<sFundInOut> lvecFundInOut;
// 		sQryFundInOut ls;
// 		strcpy(ls.msStartDay,nsDate.c_str());
// 		strcpy(ls.msEndDay,nsDate.c_str());
// 		strcpy(ls.mUserID,lIteFundInOut->first.c_str());
// 		mpDBOpr->LoadQryFundInOut(ls,lvecFundInOut);
// 		for(unsigned int i = 0; i < lvecFundInOut.size();i++)
// 		{
// 			lIteFundInOut->second->UserFundInOut(lvecFundInOut[i]);
// 		}
// 		lIteFundInOut++;
// 	}


}


//分派用户的初始数据到交易数据中
void CReal_SvrTradeData::DispathInitialDataToUser(const std::string & nUserId, 
												  const std::string & nsInstruemntDay,
												  _MapUserTradeData & nmapUserTradeData)
{
	CSafeLock lLock(m_mutexInitData);

	TRADE_LOG("DispathInitialDataToUser %s",nUserId.c_str());
	_MapUserTradeData::iterator  lIterUser = nmapUserTradeData.find(nUserId);
	if(lIterUser != nmapUserTradeData.end())
	{


		//获取该用户的初始权益				
		std::map<std::string,double>::iterator lIteInitFund = mUserInitInitFund.find(nUserId);
		if(lIteInitFund != mUserInitInitFund.end())
		{
			lIterUser->second->SetInitFund(lIteInitFund->second);
		}

		//获取该用户的资金				
		std::map<std::string,PlatformStru_TradingAccountInfo>::iterator lIteFund = mUserIniFundInfo.find(nUserId);
		if(lIteFund != mUserIniFundInfo.end())
		{
			strcpy(lIteFund->second.TradingDay,nsInstruemntDay.c_str());
			lIterUser->second->SetQryFundInfo(lIteFund->second);
		}
		else
		{
			//设置空资金和accountid
			PlatformStru_TradingAccountInfo lFundInfo;
			memset(&lFundInfo,0,sizeof(lFundInfo));
			strcpy(lFundInfo.BrokerID,"0000");
			strcpy(lFundInfo.AccountID,lIterUser->first.c_str());	
			strcpy(lFundInfo.TradingDay,nsInstruemntDay.c_str());
			lIterUser->second->SetQryFundInfo(lFundInfo);
		}

		//如果日初数据的日期和盘中数据的日期是一天，则不用加载出入金
		if(msInstrumentDay != msUserInitDay)
		{
			//获取用户今天的出入金
			vector<sFundInOut> lvecFundInOut;
			sQryFundInOut ls;
			strcpy(ls.msStartDay,msInstrumentDay.c_str());
			strcpy(ls.msEndDay,msInstrumentDay.c_str());
			strcpy(ls.mUserID,lIterUser->first.c_str());
// 			mpDBOpr->LoadQryFundInOut(ls,lvecFundInOut);
// 			for(unsigned int i = 0; i < lvecFundInOut.size();i++)
// 			{
// 				lIterUser->second->UserFundInOut(lvecFundInOut[i]);
// 			}
		}


		//获取该用户的持仓
		std::map<std::string,vector<PlatformStru_Position>>::iterator lIteVecPos = mUserInitPosition.find(nUserId);
		if(lIteVecPos != mUserInitPosition.end())
		{

			if(lIteVecPos->second.size() == 0)
			{
				PlatformStru_Position lTemp;
				//lIterUser->second->QryPositionReach(lTemp,-1,true);
			}
			else
			{
				bool bLast = false;
				for(unsigned int i = 0;i < lIteVecPos->second.size();i++ )
				{	
					strcpy(lIteVecPos->second[i].TradingDay,nsInstruemntDay.c_str());
					PlatformStru_InstrumentMarginRate lMargin;
					lMargin.clear();
					if(0 == lIterUser->second->GetMarginRate(lIteVecPos->second[i].InstrumentID,lMargin))
					{
						lIteVecPos->second[i].MarginRateByMoney = 
							lIteVecPos->second[i].PosiDirection == THOST_FTDC_PD_Long ? lMargin.LongMarginRatioByMoney : lMargin.ShortMarginRatioByMoney;
						lIteVecPos->second[i].MarginRateByMoney = 
							  lIteVecPos->second[i].PosiDirection == THOST_FTDC_PD_Long ? lMargin.LongMarginRatioByVolume : lMargin.ShortMarginRatioByVolume;

					}
					//lIteVecPos->second[i].MarginRateByMoney = lIterUser->
					//lIteVecPos->second[i].MarginRateByMoney

					if(i == lIteVecPos->second.size() - 1) 
						bLast = true;
					//判断该持仓的合约是否存在
					 PlatformStru_InstrumentInfo InstrumentInfo;
					if(mpPlatformDataInstrument != NULL &&
						false == mpPlatformDataInstrument->GetInstrumentInfo(lIteVecPos->second[i].InstrumentID,InstrumentInfo))
					{
						sErrorInfo lsError;
						memset(&lsError,0,sizeof(lsError));
						lsError.meErrorType = FirstLevelError;
						sprintf(lsError.msError,
							ERROR_POSITION_FORMAT,
							lIteVecPos->second[i].InvestorID,
							lIteVecPos->second[i].InstrumentID,
							lIteVecPos->second[i].PosiDirection == THOST_FTDC_PD_Long ? "多头" : "空头",
							lIteVecPos->second[i].Position);
						//TRADE_LOG(lsError.msError);
						//CInterface_SvrNotifyAndAsk::getObj().PushErrorInfo(lsError);
					}
					strcpy(lIteVecPos->second[i].Account,lIteVecPos->second[i].InvestorID);
					lIterUser->second->QryPositionReach(lIteVecPos->second[i],0,bLast);
				}
			}				
		}
		else
		{
			//设置一条错误的持仓
			PlatformStru_Position lTemp;
			memset(&lTemp,0,sizeof(lTemp));
			lIterUser->second->QryPositionReach(lTemp,-1,true);
		}

		//获取该用户的持仓明细
		std::map<std::string,vector<PlatformStru_PositionDetail>>::iterator lIteVecPosDetail = mUserInitPositionDeatil.find(nUserId);
		if(lIteVecPosDetail != mUserInitPositionDeatil.end())
		{

			if(lIteVecPosDetail->second.size() == 0)
			{
				PlatformStru_PositionDetail lTemp;
				memset(&lTemp,0,sizeof(lTemp));
				//lIterUser->second->QryPositionDetailReach(lTemp,-1,true);
			}
			else
			{
				bool bLast = false;
				for(unsigned int i = 0;i < lIteVecPosDetail->second.size();i++ )
				{	
					strcpy(lIteVecPosDetail->second[i].TradingDay,nsInstruemntDay.c_str());
					PlatformStru_InstrumentMarginRate lMargin;
					lMargin.clear();
					if(0 == lIterUser->second->GetMarginRate(lIteVecPosDetail->second[i].InstrumentID,lMargin))
					{
						lIteVecPosDetail->second[i].MarginRateByMoney = 
							lIteVecPosDetail->second[i].Direction == THOST_FTDC_D_Buy ? lMargin.LongMarginRatioByMoney : lMargin.ShortMarginRatioByMoney;
						lIteVecPosDetail->second[i].MarginRateByMoney = 
						    lIteVecPosDetail->second[i].Direction == THOST_FTDC_D_Buy ? lMargin.LongMarginRatioByVolume : lMargin.ShortMarginRatioByVolume;

					}
					if(i == lIteVecPosDetail->second.size() - 1) bLast = true;
					strcpy(lIteVecPosDetail->second[i].Account,lIteVecPosDetail->second[i].InvestorID);
					lIterUser->second->QryPositionDetailReach(lIteVecPosDetail->second[i],0,bLast);
				}
			}				
		}
		else
		{
			//设置一条错误的持仓明细
			PlatformStru_PositionDetail lTemp;
			memset(&lTemp,0,sizeof(lTemp));
			lIterUser->second->QryPositionDetailReach(lTemp,-1,true);
		}
	}			


	return;

}
//分派用户的初始数据到交易数据中
void CReal_SvrTradeData::DispathInitialDataToUserWithAccount(const std::string & nBrokerID, 
									  const std::string & nAccountID,
									  _MapUserTradeData & nmapUserTradeData,
                                      const std::string & nUserID)
{

    std::string lsTempTradingDay = "20140618";//msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);
    DispathInitialDataToUser(nUserID,lsTempTradingDay,nmapUserTradeData);	

	//CSafeLock lLock(m_mutexInitData);
	//填充资金
	//CPlatformDataMgr_Rates * lpRate = NULL;
	////m_mutexMapAccount.lock();
	//BrokerAccountKey lKey(nBrokerID.c_str(),nAccountID.c_str());
	//_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);	
	//if(mMapAccountData.end() != lIter)	
	//{		
	//	lpRate = lIter->second;
	//}
	////m_mutexMapAccount.unlock();

	//if(lpRate != NULL)
// 	{
		//反向根据nBrokerID 和 nAccountID查用户		//	
// 		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);
// 		vector<UserInfo> lVecUser;
//  		CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nAccountID,nBrokerID,lVecUser);	
//  		vector<UserInfo>::iterator lIter = lVecUser.begin();
//  		for(;lIter != lVecUser.end();lIter++)
//  		{
//  			DispathInitialDataToUser(nUserID,lsTempTradingDay,nmapUserTradeData);	
//  		}	

// 	}
	
	return;

}

void CReal_SvrTradeData::SetRatesObjectToAccountRelationUserObject(const std::string & nsBrokerId,
													   const std::string & nsAccountID,
                                                       const std::string & nUserID)
{

	

	CPlatformDataMgr_Rates * lpRate = NULL;

	BrokerAccountKey lKey(nsBrokerId.c_str(),nsAccountID.c_str());
	_MapAccountData::iterator lIter =  mMapAccountData.find(lKey);	
	if(mMapAccountData.end() != lIter)	
	{		
		lpRate = lIter->second->GetPlatFormRates();
	}
	

	if(lpRate != NULL)
	{
		//反向根据nBrokerID 和 nAccountID查用户		//		
		//vector<UserInfo> lVecUser;
// 		CInterface_SvrUserOrg::getObj().GetUserIDsByTradeAccount(nsAccountID,nsBrokerId,lVecUser);	
// 		vector<UserInfo>::iterator lIter = lVecUser.begin();
// 		for(;lIter != lVecUser.end();lIter++)
// 		{
 			_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nUserID);
 			if(lIterUser != mMapUser2TradingData.end())
 				lIterUser->second->SetPlatFormRates(lpRate);
// 		}
		

	}
}


//装载某一天所有用户的的日初数据
CF_ERROR CReal_SvrTradeData::LoadInitData(PlatformStru_TradingAccountInfo& inData, const std::string & nsTime,const std::string & szUser)
{
	if(GetTraderDataStatus() != TradeDataStatusInitialing &&
		GetTraderDataStatus() != TradeDataStatusSettlemtIniting)
	{
		TRADE_LOG("Not Initing,discard this call");
		return CF_ERROR_NOINITED;
	}
	
	/*if(nsTime.empty())
		return CF_ERROR_SUCCESS;*/

	bool lbLoad = true;
	CSafeLock lLock(m_mutexInitData);

	msUserInitDay = nsTime;
	//获取用户列表
	//vector<UserInfo> lVecUser;
// 	CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUser);
// 	vector<UserInfo>::iterator lIter = lVecUser.begin();
// 	for(;lIter != lVecUser.end();lIter++)
// 	{
// 		if(lIter->nUserType == USER_TYPE_TRADER)
// 		{
 			vector<PlatformStru_PositionDetail> lVecPosionDeatil;
 			vector<PlatformStru_Position> lVecPosition;
 			//PlatformStru_TradingAccountInfo lFundInfo;
 			//lFundInfo.clear();
 			
 			
 			
 			//if(CF_ERROR_SUCCESS == LoadInitialPositionFromDB(lIter->szAccount,nsTime,lVecPosition))
 			//	lbLoad = true;
 
 			//if(CF_ERROR_SUCCESS == LoadInitialPositionDetailFromDB(lIter->szAccount,nsTime,lVecPosionDeatil))
 			//	lbLoad = true;
 
 			//if(CF_ERROR_SUCCESS == LoadInitialFundFromDB(lIter->szAccount,nsTime,lFundInfo))
 			//	lbLoad = true;
 
 			//double ldbVal = inData.Available;
 			//LoadInitialInitFundFromDB(lIter->szAccount,ldbVal);
 
 			//strcpy(inData.AccountID,szUser.c_str());
 			mUserIniFundInfo.insert(std::make_pair(szUser,inData));
 			mUserInitPosition.insert(std::make_pair(szUser,lVecPosition));
 			mUserInitPositionDeatil.insert(std::make_pair(szUser,lVecPosionDeatil));
 			mUserInitInitFund[szUser] = inData.Available;
// 		}
// 	}
	if(lbLoad )
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_DATABASE_OTHER_ERROR;
}
//从结算数据库里面装置日初的持仓
CF_ERROR CReal_SvrTradeData::LoadInitialPositionFromDB(const std::string & nUserName,
													   const std::string & nsTime,
													   vector<PlatformStru_Position> & nVec)
{
	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserPositions("SETTLEMENT_POSITIONINIT",
// 		nUserName,
// 		nsTime,
// 		nVec))
// 	{
// 		return CF_ERROR_SUCCESS;
// 	}
	return CF_ERROR_DATABASE_OTHER_ERROR;
}

//从结算数据库里面装置日初的持仓明细
CF_ERROR CReal_SvrTradeData::LoadInitialPositionDetailFromDB(const std::string & nUserName,
															 const std::string & nsTime,
															 vector<PlatformStru_PositionDetail>& nVec)
{
	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserPositionDetails("SETTLEMENT_POSITIONDETAILINIT",
// 		nUserName,
// 		nsTime,
// 		nVec))
// 	{
// 		return CF_ERROR_SUCCESS;
// 	}
	return CF_ERROR_DATABASE_OTHER_ERROR;
}

//从结算数据库里面装置日初的资金
CF_ERROR CReal_SvrTradeData::LoadInitialFundFromDB(const std::string & nUserName,
												   const std::string & nsTime,
												   PlatformStru_TradingAccountInfo & nFundIndo)
{

	//PlatformStru_TradingAccountInfo lInfo;
	memset(&nFundIndo,0,sizeof(nFundIndo));
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserFundInfo("SETTLEMENT_FUNDINIT",
// 		nUserName,
// 		nsTime,
// 		nFundIndo))
// 	{
// 		//修改每日的出入金为0
// 		nFundIndo.Withdraw = 0.0;
// 		nFundIndo.Deposit = 0.0;
// 		return CF_ERROR_SUCCESS;
// 	}
	return CF_ERROR_DATABASE_OTHER_ERROR;	
}

//从初始权益表里面表里面转载初始资金
CF_ERROR CReal_SvrTradeData::LoadInitialInitFundFromDB(const std::string & nUserName,		
										   double & nFundIndo)
{
	
// 	if(CF_ERROR_SUCCESS == mpDBOpr->LoadUserInitFund("TradeData_InitFund",
// 		nUserName,		
// 		nFundIndo))
// 	{
// 		
// 		return CF_ERROR_SUCCESS;
// 	}
	return CF_ERROR_DATABASE_OTHER_ERROR;	
}
//初始化用户对象
CF_ERROR CReal_SvrTradeData::InitUserMapWithEmpty(string szUser,string szBrokerCode,string szTradeAccount,_MapUserTradeData & nmapUserData)
{

	{
		CSafeRwLockWrite lwlockAccount(mrwLockMapAccount);

        BrokerAccountKey lKey(szBrokerCode.c_str(),szTradeAccount.c_str());

        IInterfaceAccountOp * lpTemp = NULL;
		if(mMapAccountData.find(lKey) == mMapAccountData.end())
		{
			lpTemp = IFactoryAccountOp::GetFactory()->CreateAccountOp();
			mMapAccountData.insert(std::make_pair(lKey,lpTemp));
		}
	}

	{

		//获取用户列表
		CSafeRwLockWrite lwlock(mrwLockMapUserTrading);
        if(nmapUserData.find(szUser) == nmapUserData.end())
            CreateEmptyAccountOpAndAddMap(szUser,nmapUserData);
	}

	return CF_ERROR_SUCCESS;
}
//全部数据重置
//初始化状态下的函数，其他状态下无效
CF_ERROR CReal_SvrTradeData::ResetAll()
{
	//清除结算价行情
	mMutextSettlementQuot.lock();
	mmapSettlementQuot.clear();
	mMutextSettlementQuot.unlock();

	//清除日初用户信息
	{
		m_mutexInitData.lock();		
		mUserInitPosition.erase(mUserInitPosition.begin(),mUserInitPosition.end());
		mUserInitPositionDeatil.erase(mUserInitPositionDeatil.begin(),mUserInitPositionDeatil.end());
		mUserIniFundInfo.erase(mUserIniFundInfo.begin(),mUserIniFundInfo.end());
		m_mutexInitData.unlock();

	}	
	
	//清除用户信息
	{
		CSafeRwLockWrite lwlock(mrwLockMapUserTrading);
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();	
		while(mMapUser2TradingData.end() != lIter)	
		{		
			if(lIter->second != NULL)
			{
				lIter->second->ClearAll();					
			}
			lIter++;
		}		

	}

	//清除委托交易账号相关的费率保证金率
	{
		//m_mutexMapAccount.lock();
		CSafeRwLockWrite lwlock(mrwLockMapAccount);
		_MapAccountData::iterator lIter =  mMapAccountData.begin();	
		while(mMapAccountData.end() != lIter)	
		{		
			if(lIter->second != NULL)
			{
				lIter->second->ClearAll();
			}
			lIter++;
		}
		//mMapAccountData.erase(mMapAccountData.begin(),mMapAccountData.end());
		//mMapAccountRateReady.erase(mMapAccountRateReady.begin(),mMapAccountRateReady.end());
		//m_mutexMapAccount.unlock();
	}


	//清除合约
	InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReday_NoReady);
	if(mpPlatformDataQuots)
	{
		mpPlatformDataQuots->clear();	
	}

	if(mpPlatformDataInstrument != NULL)
	{
		mpPlatformDataInstrument->clear();	
	}

	return CF_ERROR_SUCCESS;
}


//定时存储
CF_ERROR  CReal_SvrTradeData::TimerSaveTradingData(bool bTimer)
{
// 	if(CInterface_SvrLogin::getObj().IsSlaver())
// 		return CF_ERROR_SUCCESS;

	if(GetTraderDataStatus() != TradeDataStatusTrading && GetTraderDataStatus() != TradeDataStatusInitialing)
		return CF_ERROR_NOINITED;

	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	//if(bTimer != false || mBufferDataStoreCTPOrder.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreCTPOrder.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteOrderInfos,"CTP_ORDERS","",lsTempTradingDay);

	//if(bTimer != false || mBufferDataStoreCTPTrade.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreCTPTrade.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteTradeInfos,"CTP_TRADES","",lsTempTradingDay);

	//if(bTimer != false || mBufferDataStoreOrder.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreOrder.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteOrderInfos,"TRADEDATA_ORDERS","",lsTempTradingDay);

	//if(bTimer != false || mBufferDataStoreTrade.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreTrade.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteTradeInfos,"TRADEDATA_TRADES","",lsTempTradingDay);

	//if(bTimer != false || mBufferDataStoreExecuteInOrder.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreExecuteInOrder.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteOrderInfos,"EXECUTEIN_ORDERS","",lsTempTradingDay,false);
	//	
	//if(bTimer != false || mBufferDataStoreExecuteOutOrder.Size() >= LIST_MAX_SIZE)
	//	mBufferDataStoreExecuteOutOrder.SaveToDB(mpDBOpr,&IInterfaceDBOpr::WriteOrderInfos,"EXECUTEOUT_ORDERS","",lsTempTradingDay,false);

	//获取增加修改的报单，成交，修改的持仓，持仓明细，定时save

	return CF_ERROR_SUCCESS;
}


//盘中数据落地
CF_ERROR  CReal_SvrTradeData::SaveAllTradingData(bool nbInitFirst)
{
// 	if(CInterface_SvrLogin::getObj().IsSlaver())
// 		return CF_ERROR_SUCCESS;

	//
	//CSafeLock lLock(m_mutexMapUserTrading);

	////存结算价行情
	//mMutextSettlementQuot.lock();
	//vector<PlatformStru_DepthMarketData> lVecQuto;
	//std::map<std::string,PlatformStru_DepthMarketData>::iterator lIterQuto =  mmapSettlementQuot.begin();
	//while(lIterQuto != mmapSettlementQuot.end())
	//{
	//	lVecQuto.push_back(lIterQuto->second);
	//	lIterQuto++;
	//}
	//mMutextSettlementQuot.unlock();
	//if(lVecQuto.size() > 0)
	//	mpDBOpr->WriteQutoInfo("TradeData_Quto",msCurrentTradingDay,lVecQuto);

	//	
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	while(mMapUser2TradingData.end() != lIter && !lsTempTradingDay.empty())	
	{			
// 		if(lIter->second != NULL && lIter->second->GetUserInitStatus() == USER_STATUS_Inited)
// 		{
// 			//清除缓存中的保单和成交
// 			mBufferDataStoreOrder.Clear();
// 			mBufferDataStoreTrade.Clear();
// 			////存报单
// 			std::vector<PlatformStru_OrderInfo> lVecOrder;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetAllOrders(lVecOrder) && lVecOrder.size() != 0)
// 				mpDBOpr->WriteOrderInfos("TradeData_Orders",lIter->first,lsTempTradingDay,lVecOrder);
// 
// 			//存成交
// 			std::vector<PlatformStru_TradeInfo> lVecTrade;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetAllTrades(lVecTrade) && lVecTrade.size() != 0)
// 				mpDBOpr->WriteTradeInfos("TradeData_Trades",lIter->first,lsTempTradingDay,lVecTrade);
// 
// 			//存持仓
// 			std::vector<PlatformStru_Position> lVecPosition;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetAllPosition(lVecPosition) && lVecPosition.size() != 0)
// 				mpDBOpr->WritePositionInfos("TradeData_Position",lIter->first,lsTempTradingDay,lVecPosition);
// 
// 			//存持仓
// 			std::vector<PlatformStru_PositionDetail> lVecPositionDetail;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetAllPositionDetail(lVecPositionDetail) && lVecPositionDetail.size() != 0)
// 				mpDBOpr->WritePositionDetailInfos("TradeData_PositionDetail",lIter->first,lsTempTradingDay,lVecPositionDetail);
// 	
// 			//存资金		
// 			PlatformStru_TradingAccountInfo lTempTrade;
// 			if(CF_ERROR_SUCCESS == lIter->second->GetFundInfo(lTempTrade))
// 			{				
// 				mpDBOpr->WriteFundInfos("TradeData_FundInfo",lIter->first,lsTempTradingDay,lTempTrade);
// 			}
// 
// 
// 		}

		lIter++;
	}

	CSafeRwLockRead lrlockAccount(mrwLockMapAccount);
	_MapAccountData::iterator lIterAccount =  mMapAccountData.begin();

	while(mMapAccountData.end() != lIterAccount && !lsTempTradingDay.empty())	
	{			
		if(lIterAccount->second != NULL && lIterAccount->second->GetAccountInitStatus() == USER_STATUS_Inited)
		{
		
			////存报单
// 			std::vector<PlatformStru_OrderInfo> lVecOrder;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetAllOrders(lVecOrder) && lVecOrder.size() != 0)
// 			{
// 				if(nbInitFirst)
// 					mpDBOpr->WriteAccountOrderInfos("TRADEDATA_A_O_INIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecOrder);
// 
// 				mpDBOpr->WriteAccountOrderInfos("TRADEDATA_A_ORDERS",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecOrder);
// 			}
// 
// 			//存成交
// 			std::vector<PlatformStru_TradeInfo> lVecTrade;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetAllTrades(lVecTrade) && lVecTrade.size() != 0)
// 			{
// 				if(nbInitFirst)
// 					mpDBOpr->WriteAccountTradeInfos("TRADEDATA_A_T_INIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecTrade);
// 
// 				mpDBOpr->WriteAccountTradeInfos("TRADEDATA_A_TRADES",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecTrade);
// 			}
// 
// 			//存持仓
// 			std::vector<PlatformStru_Position> lVecPosition;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetAllPosition(lVecPosition) && lVecPosition.size() != 0)
// 			{
// 				if(nbInitFirst)
// 					mpDBOpr->WriteAccountPositionInfos("TradeData_A_P_INIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecPosition);
// 
// 				mpDBOpr->WriteAccountPositionInfos("TradeData_A_P",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecPosition);
// 			}
// 
// 			//存持仓
// 			std::vector<PlatformStru_PositionDetail> lVecPositionDetail;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetAllPositionDetail(lVecPositionDetail) && lVecPositionDetail.size() != 0)
// 			{
// 				if(nbInitFirst)
// 					mpDBOpr->WriteAccountPositionDetailInfos("TradeData_A_PD_INIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecPositionDetail);
// 
// 				mpDBOpr->WriteAccountPositionDetailInfos("TradeData_A_PD",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lVecPositionDetail);
// 			}
// 
// 			//存资金		
// 			PlatformStru_TradingAccountInfo lTempTrade;
// 			if(CF_ERROR_SUCCESS == lIterAccount->second->GetFundInfo(lTempTrade))
// 			{		
// 				if(nbInitFirst)
// 						mpDBOpr->WriteAccountFundInfos("TRADEDATA_A_FUNDINIT",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lTempTrade);
// 
// 				mpDBOpr->WriteAccountFundInfos("TRADEDATA_A_FUNDINFO",lIterAccount->first.BrokerID,lIterAccount->first.AccountID,lsTempTradingDay,lTempTrade);
// 			}


		}

		lIterAccount++;
	}
	
	return CF_ERROR_SUCCESS;
}


//推送一个事件用户相关的账户为参数
CF_ERROR CReal_SvrTradeData::PushEventToAccountWithUser(eEventType nEve,const std::string & nUserName)
{
	int nUserID = -1;
	vector<UserInfo> lVecUeser;
// 	CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 	for(unsigned int i = 0;i < lVecUeser.size();i++)
// 	{
// 		if(strcmp(lVecUeser[i].szAccount,nUserName.c_str()) == 0)
// 		{
// 			nUserID = lVecUeser[i].nUserID;
// 			break;
// 		}
// 	}
// 	if(nUserID != -1)
// 	{
// 		TradeAccount tradeAccount;
// 		if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))
// 		{
// 			BrokerInfo lBroker;							
// 			CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(tradeAccount.nBrokerID,lBroker);
// 			//发送logoff的事件					
// 			Stru_NotifyEvent lDataEvent(nEve,0);
// 			BrokerAccountKey lsBrokerAccount(lBroker.szBrokerCode,tradeAccount.szTradeAccount);		
// 			lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
// 			CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 			return CF_ERROR_SUCCESS;
// 		}
// 	}
	return CF_ERROR_TRADEDATA_NO_USER_DATA;
	
}

void CReal_SvrTradeData::TradeDataInitAfterNoTradingDay()
{
	//没有获取到交易日
	//
	TRADE_LOG("TradeDataInitAfterNoTradingDay");
	
	std::string lsOldestInitDayWithNoSett;
// 	mpTradingDayManager.HasInitTradingDayWithNoSettlement(lsOldestInitDayWithNoSett);
// 
// 	if(lsOldestInitDayWithNoSett.empty())
// 	{
// 		PUSH_LOG_ERROR(Logger,false,true,"没有交易日，也没有待结算的交易日，设置服务器为无交易日状态");
// 		PUSH_LOG_ERROR(Logger,false,true,"-----------------初始化完成-----------------");
// 		TRADE_LOG("can not find a inited trading day with no settlemnt,we do nothing");
// 		SetTraderDataStatus(TradeDataStatusNoTradingDay);
// 	}
// 	else
// 	{
// 		PUSH_LOG_ERROR(Logger,false,true,"有待结算的交易日，设置服务器为有待结算交易日状态");
// 		PUSH_LOG_ERROR(Logger,false,true,"-----------------初始化完成-----------------");
// 
// 		SetCurrentTradingDay(lsOldestInitDayWithNoSett,true);
// 		SetTraderDataStatus(TradeDataStatusLastDaySettlemtNotOK);
// 		TRADE_LOG("Inittradingday with no settlement is %s, set trading status to TradeDataStatusLastDaySettlemtNotOK ",\
// 			lsOldestInitDayWithNoSett.c_str());
// 	
// 	}
	return;
}


//获取交易日后，进行的交易初始化动作
void CReal_SvrTradeData::TradeDataInitAfterGetTradingDay() 
{
	std::string lsTempTradingDay;
	GetCurrentTradingDay(lsTempTradingDay);
	TRADE_LOG("TradeDataInitAfterGetTradingDay %s",lsTempTradingDay.c_str());
	//查看今天是否结算过,直接设置为结算完成状态
// 	if(mpTradingDayManager.IsSetttemnted(lsTempTradingDay))
// 	{		
// 		TRADE_LOG("Today %s has settlemented,only need to set status to EndSettlemented",lsTempTradingDay.c_str());	
// 		PUSH_LOG_ERROR(Logger,false,true,"今日已结算，设置服务器进入结算完成状态");
// 		CReal_SvrTradeData::getObj().SetTraderDataStatus(TradeDataStatusSettlemtEnd);
// 		return;
// 	}
// 	else
// 	{
// 		std::string lsLastTradingDay;
// 		mpTradingDayManager.GetLastTradingData(lsTempTradingDay,lsLastTradingDay);
// 
// 		//检查上一个交易日是否结算
// 		if(!lsLastTradingDay.empty () &&
// 			!mpTradingDayManager.IsSetttemnted(lsLastTradingDay))
// 		{
// 			TRADE_LOG("LoadInitData Failed need resettlement day is %s",lsLastTradingDay.c_str());
// 			PUSH_LOG_ERROR(Logger,false,true,"上一个交易日未结算");
// 			TradeDataInitAfterNoTradingDay();
// 			return;				
// 		}
// 		else
// 		{
// 			PUSH_LOG_ERROR(Logger,false,true,"从数据库中加载日初数据");
// 			TRADE_LOG("LoadInitData Load last day %s",lsLastTradingDay.c_str());
// 			LoadInitData(lsLastTradingDay);
// 		}
// 
// 
// 	}


	//通知模块线程不停地check 用户的状态，
	//PostThreadMessage(g_idThread,TMESSAGE_TRADEINIT_CHECK_USER_STATUS,0,0);


	//如果装载日初数据成功，设置为交易状态
	//SetTraderDataStatus(TradeDataStatusTrading);



	//设置合约和费率的日期
	msInstrumentDay = lsTempTradingDay;

	//处理合约
	//如果数据库有今天的合约，则读取数据库
	//如果数据库里没有今天的合约，则向报盘发送查询合约的事件
	vector<PlatformStru_InstrumentInfo> lVecInstrument;	
	std::set<std::string> lSetInstrumentId;
// 	if(mbForceInit == false && CF_ERROR_SUCCESS == mpDBOpr->LoadTodayInstrument(lsTempTradingDay,lVecInstrument))
// 	{
// 		TRADE_LOG("LoadTodayInstrument From DataBase Success Send Get Instrument CMD:AccountID %s",lsTempTradingDay.c_str());
// 		vector<PlatformStru_InstrumentInfo>::iterator lIte = lVecInstrument.begin();
// 		for(;lIte != lVecInstrument.end();lIte++)
// 		{
// 			SetInstrumentInfo(*lIte);
// 		}
// 		//去掉可能重复的合约
// 		mpPlatformDataInstrument->GetInstrumentList(lSetInstrumentId);
// 
// 		//设置所有用户的		
// 		InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReady_Ready);
// 
// 		//为主帐号设置合约
// 		SetPlatformObjectToAllAccount();
// 		//为用户账户设置合约
// 		SetPlatformObjectToAllUser(mMapUser2TradingData);
// 
// 		//至今天的交易日初始化完成	
// 		mpTradingDayManager.SetTradingDayInit(lsTempTradingDay);
// 
// 		
// 		//通知报盘来获取合约
// 		Stru_NotifyEvent lDataEvent(EventTypeNeedGetInstrument,0);
// 		CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 		PUSH_LOG_ERROR(Logger,false,true,"从数据库加载合约信息成功，发送获取合约指令");
// 
// 		//检测过期合约的持仓
// 		if(CheckInitDataHasExpiredInstruementPosition())
// 		{
// 			SetTraderDataStatus(TradeDataStatusInitFail);
// 			PUSH_LOG_ERROR(Logger,false,true,"发现日初持仓里面有过期的合约，服务器初始化失败");
// 			//通知线程关闭定时器
// 			PostThreadMessage(g_idThread,TMESSAGE_TRADING_END_USER_CHECK,0,0);
// 			return;
// 		}
// 
// 		
// 
// 		//处理查询保证金和费率
// 		SendQryCommissionAndMargin();
// 	}
// 	else
// 	{
// 		TRADE_LOG("LoadTodayInstrument From DataBase Faild Send Qry Instrument CMD:date is %s",lsTempTradingDay.c_str());
// 		PUSH_LOG_ERROR(Logger,false,true,"发送查询合约指令");
// 		//产生一个需要查询合约的事件
// 		Stru_NotifyEvent lDataEvent(EventTypeNeedQryInstrument,0);
// 		CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
// 	}
	

}
//检查每个用户ctp登陆情况
bool CReal_SvrTradeData::CheckAllUserFinishedCTP(int & nLoginCount)
{
	bool lbRet = false;
	nLoginCount = 0;
	do 
	{
		TRADE_LOG("Start Check All User CTP")
        CSafeRwLockRead lrlock(mrwLockMapUserTrading);
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
        int lnFinishCount = 0;
		while(mMapUser2TradingData.end() != lIter)	
		{		
			//判断用户是否是空用户，没有绑定委托交易账号
			int nUserID = -1;
			vector<UserInfo> lVecUeser;
// 			CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 			for(unsigned int i = 0;i < lVecUeser.size();i++)
// 			{
// 				if(strcmp(lVecUeser[i].szAccount,lIter->first.c_str()) == 0)
// 				{
// 					nUserID = lVecUeser[i].nUserID;
// 					break;
// 				}
// 			}
// 			TradeAccount tradeAccount;
// 			if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))
// 			{
// 				TRADE_LOG("User %s Finished CTP with Empty Trader Account",lIter->first.c_str());
// 				lnFinishCount++;
// 			}
// 			else
// 			{
// 				double lnDifferTime;
// 				EnumAccountStatus  lnCtpStatus = lIter->second->GetUserCTPStatus(lnDifferTime);
// 
// 				if(lnCtpStatus == ACCOUNT_STATUS_Login ||
// 					lnCtpStatus == ACCOUNT_STATUS_CannotConnect ||
// 					lnCtpStatus == ACCOUNT_STATUS_LoginFailure)
// 				{
// 					TRADE_LOG("User %s Finished with Account CTP Status %d",lIter->first.c_str(),lnCtpStatus);
// 					//nLoginCount++;
// 					lnFinishCount++;
// 				}
// 				////在这 断开120s的时间，就认为是固定状态
// 				else if(lnCtpStatus ==  ACCOUNT_STATUS_Disconnected && lnDifferTime > 120)
// 				{
// 					TRADE_LOG("User %s Finished with Account CTP Status %d",lIter->first.c_str(),lnCtpStatus);
// 					lnFinishCount++;
// 				}
// 			
// 			}
			lIter++;
		}

		if(lnFinishCount == mMapUser2TradingData.size())
		{
			lbRet = true;
			TRADE_LOG("Exit Check All User CTP Finished")
	

			break;
		}

		TRADE_LOG("End Check All User CTP  Finished")
		break;
		//Sleep(1000);
	} while (true);


	return lbRet;

}
//检查每个用户是否都做完了动作，切入到交易状态
bool CReal_SvrTradeData::CheckAllUserFinishedInit()
{
	bool lbRet = false;
	do 
	{
		TRADE_LOG("Start Check All User Finished")
     
        CSafeRwLockRead lrlock(mrwLockMapUserTrading);
		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
        int lnFinishCount = 0;
		while(mMapUser2TradingData.end() != lIter)	
		{		
			//判断用户是否是空用户，没有绑定委托交易账号
			int nUserID = -1;
			vector<UserInfo> lVecUeser;
// 			CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 			for(unsigned int i = 0;i < lVecUeser.size();i++)
// 			{
// 				if(strcmp(lVecUeser[i].szAccount,lIter->first.c_str()) == 0)
// 				{
// 					nUserID = lVecUeser[i].nUserID;
// 					break;
// 				}
// 			}
// 			TradeAccount tradeAccount;
// 			if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))			
// 			{
// 				TRADE_LOG("User %s Finished with Empty Trader Account",lIter->first.c_str());
// 				lnFinishCount++;
// 			}
// 			else
// 			{
// 				if( lIter->second->GetUserInitStatus() == USER_STATUS_Inited )
// 				{
// 					TRADE_LOG("User %s Finished with Qry End",lIter->first.c_str());
// 					lnFinishCount++;
// 				}
// 				else 			 
// 				{
// 					double lnDifferTime;
// 					EnumAccountStatus  lnCtpStatus = lIter->second->GetUserCTPStatus(lnDifferTime);
// 
// 					//在这3个状态上停留8s的时间，就认为是固定状态
// 					if( (lnCtpStatus== ACCOUNT_STATUS_CannotConnect ||
// 						lnCtpStatus == ACCOUNT_STATUS_LoginFailure ||
// 						lnCtpStatus == ACCOUNT_STATUS_Disconnected) &&
// 						lnDifferTime > 8)
// 					{
// 						TRADE_LOG("User %s Finished with Account CTP Status %d",lIter->first.c_str(),lnCtpStatus);
// 						lnFinishCount++;
// 					}
// 				}
// 			}
			lIter++;
		}

		if(lnFinishCount == mMapUser2TradingData.size())
		{
			lbRet = true;
			TRADE_LOG("Exit Check All User Finished")
			break;
		}

		TRADE_LOG("End Check All User Finished with %d/%d",lnFinishCount,mMapUser2TradingData.size())
		break;
		//Sleep(1000);
	} while (true);


	return lbRet;

}


//测试一个用户是否有持仓
bool CReal_SvrTradeData::TestUserHasPosition(const std::string & nUserName)
{
	bool lbRet = false;
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nUserName);
	if(lIterUser != mMapUser2TradingData.end())
	{	
		lbRet = lIterUser->second->HasPositions();
	}
	return lbRet;
}


//测试一个用户是否有该成交
bool CReal_SvrTradeData::TestUserTraderExist(const PlatformStru_TradeInfo & nTraderInfo)
{
	bool lbRet = false;
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);
	_MapUserTradeData::iterator  lIterUser = mMapUser2TradingData.find(nTraderInfo.InvestorID);
	if(lIterUser != mMapUser2TradingData.end())
	{	
		TradeKey lKey(nTraderInfo);
		lbRet = lIterUser->second->TestTradeInfoExist(lKey);
	}
	return lbRet;
}
//盘中新增加一个交易员，其委托交易账号也已查完费率
CF_ERROR CReal_SvrTradeData::AddTraderUserWhenTrading(const std::string & nUserName,const std::string & nsAccountName,const std::string & nsBrokerCode)
{

	if(nUserName.empty() || nsAccountName.empty() || nsBrokerCode.empty())
		return CF_ERROR_COMMON_INPUT_PARAM;

	TRADE_LOG("param is %s ",nUserName.c_str());
	//新增一个对象的内存
	CSafeRwLockWrite lwlock(mrwLockMapUserTrading);
	
	CreateEmptyAccountOpAndAddMap(nUserName,mMapUser2TradingData);

	//分配合约和行情对象
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserName);
	if(lIter != mMapUser2TradingData.end())
	{
		lIter->second->SetPlatFormInstrument(mpPlatformDataInstrument);
		lIter->second->SetPlatFormQutos(mpPlatformDataQuots);
	}
	else
	{
		TRADE_LOG("No Data %s",nUserName.c_str());
		return CF_ERROR_TRADEDATA_NO_USER_DATA;
	}

	//查看是否已初始化完成
	if(lIter->second->GetUserInitStatus() == USER_STATUS_Inited)
	{
		TRADE_LOG("Has Init %s",nUserName.c_str());
		return CF_ERROR_TRADEDATA_HAS_INITED;
	}
	

	//查看是否有费率对象，如果有直接赋值，如果没有通知报盘查询费率和保证金率
	CSafeRwLockWrite lwlock2(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerCode.c_str(),nsAccountName.c_str());
	_MapAccountData::iterator lIterAccount= mMapAccountData.find(lKey);
    if(lIterAccount == mMapAccountData.end())
	{
		return CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOLOGIN; 
	}

	if(	lIterAccount->second->GetCommissonMarginReady() != CommissionMargin_ComissionAndMarinReady)
	{
		return CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOCOMM;
	}


	TRADE_LOG("Has Found %s commission ",nsAccountName.c_str());

	lIter->second->SetPlatFormRates(lIterAccount->second->GetPlatFormRates());

	//如果没有分配日初数据，则分派空的日初数据

	if(!(lIter->second->GetUserInitStatus() & USER_STATUS_InitQryPositionReady) )
	{
		TRADE_LOG("DispathInitialDataToUser %s  ",nUserName.c_str());
		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);	
		DispathInitialDataToUser(nUserName,lsTempTradingDay,mMapUser2TradingData);
	}

	//分派空的报单和成交
	lIter->second->SetEqualUserInitStatus(USER_STATUS_Inited);

    //设置用户同步完成，否则报单、成交到来时无法推送给新建用户
    CReal_SvrTradeData::getObj().SetUserSynFinished(nUserName);

	
	return CF_ERROR_SUCCESS;

	//else
	//{
	//	CPlatformDataMgr_Rates * lpTemp = new CPlatformDataMgr_Rates();
	//	mMapAccountData.insert(std::make_pair(nsAccountName,lpTemp));

	//	TRADE_LOG("Has not Found %s commission %s",nsAccountName.c_str,lsTempTradingDay.c_str());
	//
	//	//通知登陆查询费率
	//	Stru_NotifyEvent lDataEvent(EventTypeNeedSingleLoginTrade,0);			
	//	BrokerAccountKey lsBrokerAccount(nsBrokerCode.c_str(),nsAccountName.c_str());		
	//	lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
	//	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);

	//	//产生一个需要查询合约费率的事件
	//	Stru_NotifyEvent lDataEvent(EventTypeNeedQryCommission,0);			
	//	BrokerAccountKey lsBrokerAccount(nsBrokerCode.c_str(),nsAccountName.c_str());		
	//	lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
	//	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	//
	//	//产生一个需要查询报证金率的事件
	//	Stru_NotifyEvent lDataEvent(EventTypeNeedQryMargin,0);			
	//	BrokerAccountKey lsBrokerAccount(nsBrokerCode.c_str(),nsAccountName.c_str());		
	//	lDataEvent.AddBufer((void*)&lsBrokerAccount,sizeof(lsBrokerAccount));	
	//	CInterface_SvrNotifyAndAsk::getObj().NotifyEvent(lDataEvent);
	//}	

	
}
//盘中新增加一个交易员，其委托交易账号也已查完费率
CF_ERROR CReal_SvrTradeData::AddInitTraderUser(const std::string & nUserName,
											   const std::string & nsAccountName,
											   const std::string & nsBrokerCode)
{

	if(nUserName.empty() )
		return CF_ERROR_COMMON_INPUT_PARAM;

	TRADE_LOG("AddInitTraderUser param is %s ",nUserName.c_str());
	//新增一个对象的内存
	CSafeRwLockWrite lwlock(mrwLockMapUserTrading);

	CreateEmptyAccountOpAndAddMap(nUserName,mMapUser2TradingData);

	//分配合约和行情对象
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nUserName);
	if(lIter != mMapUser2TradingData.end())
	{
		lIter->second->SetPlatFormInstrument(mpPlatformDataInstrument);
		lIter->second->SetPlatFormQutos(mpPlatformDataQuots);
	}
	else
	{
		TRADE_LOG("No instrument Data %s",nUserName.c_str());
		return CF_ERROR_TRADEDATA_NO_USER_DATA;
	}


	//查看是否有费率对象，如果有直接赋值，如果没有通知报盘查询费率和保证金率
	CSafeRwLockWrite lwlock2(mrwLockMapAccount);
	BrokerAccountKey lKey(nsBrokerCode.c_str(),nsAccountName.c_str());
	_MapAccountData::iterator lIterAccount= mMapAccountData.find(lKey);
	if(lIterAccount == mMapAccountData.end())
	{
		return CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOLOGIN; 
	}

	if(	lIterAccount->second->GetCommissonMarginReady() != CommissionMargin_ComissionAndMarinReady)
	{
		return CF_ERROR_TRADEDATA_INIT_ACCOUNT_NOCOMM;
	}


	TRADE_LOG("Has Found %s commission ",nsAccountName.c_str());

	lIter->second->SetPlatFormRates(lIterAccount->second->GetPlatFormRates());

	//如果没有分配日初数据，则分派空的日初数据

	if(!(lIter->second->GetUserInitStatus() & USER_STATUS_InitQryPositionReady) )
	{
		TRADE_LOG("DispathInitialDataToUser %s  ",nUserName.c_str());
		std::string lsTempTradingDay = msInstrumentDay.substr(0,4) + msInstrumentDay.substr(5,2) + msInstrumentDay.substr(8,2);	
		DispathInitialDataToUser(nUserName,lsTempTradingDay,mMapUser2TradingData);
	}

	//分派空的报单和成交
	lIter->second->SetEqualUserInitStatus(USER_STATUS_Inited);

	
	return CF_ERROR_SUCCESS;

	


}

bool CReal_SvrTradeData::CheckInitDataHasExpiredInstruementPosition()
{
	CSafeLock lLock(m_mutexInitData);
    bool lbRet = false;
	//获取该用户的持仓
	std::map<std::string,vector<PlatformStru_Position>>::iterator lIteVecPos = mUserInitPosition.begin();
	for(;lIteVecPos != mUserInitPosition.end();lIteVecPos++)
	{		

		for(unsigned int i = 0;i < lIteVecPos->second.size();i++ )
		{						
			//判断该持仓的合约是否存在
			PlatformStru_InstrumentInfo InstrumentInfo;
			if(mpPlatformDataInstrument != NULL &&
				false == mpPlatformDataInstrument->GetInstrumentInfo(lIteVecPos->second[i].InstrumentID,InstrumentInfo))
			{
				sErrorInfo lsError;
				memset(&lsError,0,sizeof(lsError));
				lsError.meErrorType = FirstLevelError;
				sprintf(lsError.msError,
					ERROR_POSITION_FORMAT,
					lIteVecPos->second[i].InvestorID,
					lIteVecPos->second[i].InstrumentID,
					lIteVecPos->second[i].PosiDirection == THOST_FTDC_PD_Long ? "多头" : "空头",
					lIteVecPos->second[i].Position);
				//TRADE_LOG(lsError.msError);
				//CInterface_SvrNotifyAndAsk::getObj().PushErrorInfo(lsError);
				lbRet = true;
			}				

		}
	}	
	return lbRet;

}


//结算时将所有的未成交，部分成交，待审核的报单全部改为已撤单
CF_ERROR CReal_SvrTradeData::CancelAllUnkonwnOrder()
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();

	for(;mMapUser2TradingData.end() != lIter ;lIter++)	
	{
		//lIter->second->CancelAllUnkonwnOrders();
		//需调用外侧的pushorder，以便推送到从服务器
		std::vector<PlatformStru_OrderInfo> lvecOrder;
		lIter->second->GetAllOrders(lvecOrder);
		for(unsigned int i = 0; i < lvecOrder.size(); i++)
		{
			if(lvecOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
				lvecOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeQueueing||
				lvecOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing||
				lvecOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing ||
				lvecOrder[i].OrderStatus == THOST_FTDC_OST_Unknown )
			{
				lvecOrder[i].OrderStatus = THOST_FTDC_OST_Canceled;
				lvecOrder[i].ExStatus = PlatformStru_OrderInfo::ExSta_none;
				lvecOrder[i].freezeMargin = util::GetDoubleInvalidValue();
				lvecOrder[i].freezeExchMargin = util::GetDoubleInvalidValue();
				lvecOrder[i].troubleMoney = util::GetDoubleInvalidValue();
				lvecOrder[i].freezeExchMargin = util::GetDoubleInvalidValue();
				lvecOrder[i].Leg1FreezeMargin = util::GetDoubleInvalidValue();       //组合合约报单的第一腿冻结保证金
				lvecOrder[i].Leg1FreezeExchMargin = util::GetDoubleInvalidValue();   //组合合约报单的第一腿冻结交易所保证金
				lvecOrder[i].Leg1TroubleMoney = util::GetDoubleInvalidValue();       //组合合约报单的第一腿冻结手续费
				lvecOrder[i].Leg2FreezeMargin = util::GetDoubleInvalidValue();      //组合合约报单的第二腿冻结保证金
				lvecOrder[i].Leg2FreezeExchMargin = util::GetDoubleInvalidValue();   //组合合约报单的第二腿冻结交易所保证金
				lvecOrder[i].Leg2TroubleMoney = util::GetDoubleInvalidValue();      //组合合约报单的第二腿冻结手续费
				SYSTEMTIME sysTime;
				::GetLocalTime(&sysTime);
				sprintf_s(lvecOrder[i].CancelTime, sizeof(lvecOrder[i].CancelTime), "%02d:%02d:%02d", 
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
				strncpy(lvecOrder[i].UpdateTime, lvecOrder[i].CancelTime, sizeof(lvecOrder[i].UpdateTime));
				strcpy(lvecOrder[i].StatusMsg,"服务器撤单");
				PushOrder(lvecOrder[i]);
			}
		}
		
	}
	return CF_ERROR_SUCCESS;
}

void CReal_SvrTradeData::SetUserSynFinished(const std::string & nsUserName)
{
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserName);
	if(lIter != mMapUser2TradingData.end())
		lIter->second->SetSyncFinished(true);

}
bool CReal_SvrTradeData::GetUserSynFinished(const std::string & nsUserName)
{
	bool lbRet = false;
	CSafeRwLockRead lrlock(mrwLockMapUserTrading);	
	_MapUserTradeData::iterator lIter =  mMapUser2TradingData.find(nsUserName);
	if(lIter != mMapUser2TradingData.end())
		lbRet = lIter->second->IsSyncFinished();

	return lbRet;

}
void CReal_SvrTradeData::SetAccountSynFinished(const BrokerAccountKey & nKey)
{
	CSafeRwLockRead lrlock(mrwLockMapAccount);	
	_MapAccountData::iterator lIter =  mMapAccountData.find(nKey);
	if(lIter != mMapAccountData.end())
		lIter->second->SetSyncFinished(true);
}
bool  CReal_SvrTradeData::SlaverInitAfterGetTradingDay(std::string szUser,std::string szBrokerCode,
                                                       std::string szTradeAccount,CDataEntity* pDataEntity, 
                                                       CMemCache* pCmdCache, double dFund )
{
    mpCache = pCmdCache;

    //情况缓存
    mpCache->FlushAll();

    InitUserMapWithEmpty(szUser,szBrokerCode,szTradeAccount,mMapUser2TradingData);  

    std::string szTradingDay = "20140613";
    SetCurrentTradingDay(szTradingDay);

    //转载日初数据
    std::string lsTempTradingDay;
    std::string lsLastTradingDay;
    GetCurrentTradingDay(lsTempTradingDay);
    TRADE_LOG("SlaverInitAfterGetTradingDay %s",lsTempTradingDay.c_str());

    mpTradingDayManager.LoadHistoryTradingDay();
    mpTradingDayManager.LoadHistorySettmentedDay();

    mpTradingDayManager.GetLastTradingData(lsTempTradingDay,lsLastTradingDay);
    //设置合约和费率的日期
    msInstrumentDay = lsTempTradingDay;

    //从数据库转载合约
    //处理合约
    //如果数据库有今天的合约，则读取数据库
    //如果数据库里没有今天的合约，则向报盘发送查询合约的事件
    vector<PlatformStru_InstrumentInfo> lVecInstrument;	
    std::set<std::string> lSetInstrumentId;
    TRADE_LOG("LoadTodayInstrument From DataBase Faild Send Qry Instrument CMD:date is %s",lsTempTradingDay.c_str());
          
    std::map<std::string, PlatformStru_InstrumentInfo>::const_iterator itInstr;
    const std::map<std::string, PlatformStru_InstrumentInfo>& mapInstrument = pDataEntity->GetInstrument();
    for(itInstr = mapInstrument.begin(); itInstr != mapInstrument.end(); itInstr++) 
    {
        CReal_SvrTradeData::getObj().SetInstrumentInfo((PlatformStru_InstrumentInfo&)itInstr->second);
    }

    //去掉可能重复的合约
    mpPlatformDataInstrument->GetInstrumentList(lSetInstrumentId);

    //设置所有用户的		
    InterlockedExchange(reinterpret_cast<volatile long*>(&mbInstrumentsReady),DataReady_Ready);

    //为主帐号设置合约
    SetPlatformObjectToAllAccount();
    //为用户账户设置合约
    SetPlatformObjectToAllUser(mMapUser2TradingData);
  	

    //从数据库转载保证金
    //从数据库转载费率
    //去掉可能重复的合约
    bool lbCommisstionLoadSuccess = false;
    bool lbMarginRatesLoadSuccess = false;

    BrokerAccountKey lKey(szBrokerCode.c_str(),szTradeAccount.c_str());	
    _MapAccountData::iterator lIterReady =  mMapAccountData.find(lKey);

    lbCommisstionLoadSuccess = true;
    std::map<std::string, PlatformStru_InstrumentCommissionRate>::iterator itCS;
    std::map<std::string, PlatformStru_InstrumentCommissionRate> mapCS = pDataEntity->GetCommissionRate();
    for(itCS = mapCS.begin(); itCS != mapCS.end(); itCS++)
    {
        strncpy(itCS->second.BrokerID, szBrokerCode.c_str(), sizeof(itCS->second.BrokerID)-1);
        strncpy(itCS->second.InvestorID, szTradeAccount.c_str(), sizeof(itCS->second.InvestorID)-1);
        SetCommissionRate((PlatformStru_InstrumentCommissionRate&)itCS->second);
    }

    if(mMapAccountData.end() != lIterReady)
    {
        lIterReady->second->SetCommissionMarginReady(CommissionMargin_ComissionReady);	
    }


    lbMarginRatesLoadSuccess = true;
    std::map<std::string, PlatformStru_InstrumentMarginRate>::iterator itMG;
    std::map<std::string, PlatformStru_InstrumentMarginRate> mapMG = pDataEntity->GetMarginRate();
    for(itMG = mapMG.begin(); itMG != mapMG.end(); itMG++) 
    {
        strncpy(itMG->second.BrokerID, szBrokerCode.c_str(), sizeof(itMG->second.BrokerID)-1);
        strncpy(itMG->second.InvestorID, szTradeAccount.c_str(), sizeof(itMG->second.InvestorID)-1);
        SetMarginRate((PlatformStru_InstrumentMarginRate&)itMG->second);
    }

    //设置状态		
    if(mMapAccountData.end() != lIterReady)
    {
        lIterReady->second->SetCommissionMarginReady(CommissionMargin_MarginReady);			
    }


    if(lbMarginRatesLoadSuccess && lbCommisstionLoadSuccess)
    {		
        TRADE_LOG("LoadToday Commission And Margin From DataBase Success,and send Qry Trade Cmd:%s AccountID %s",szTradeAccount,lsTempTradingDay.c_str());

        //设置费率对象到对应的用户对象
        SetRatesObjectToAccountRelationUserObject(szBrokerCode,szTradeAccount,szUser);
    }

    //装载主帐号的资金
    PlatformStru_TradingAccountInfo lAccountInfo;
    memset(&lAccountInfo,0,sizeof(PlatformStru_TradingAccountInfo));
    lIterReady->second->SetQryFundInfo(lAccountInfo);

    //装载主帐号的持仓
    vector<PlatformStru_Position> lvecPosition;
    if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountPositions("TRADEDATA_A_P_INIT",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecPosition)*/)
    {
        //设置状态
        if(mMapAccountData.end() != lIterReady)
        {
            vector<PlatformStru_Position>::iterator lIte = lvecPosition.begin();
            for(;lIte != lvecPosition.end();lIte++)
            {		
                strcpy(lIte->Account,lIte->InvestorID);
                lIterReady->second->QryPositionReach(*lIte,0,false);	
            }	
            PlatformStru_Position lPostion;
            lIterReady->second->QryPositionReach(lPostion,-100,true);
        }			
    }
    else
    {
        //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该主帐号%s的持仓",szTradeAccount);
        if(mMapAccountData.end() != lIterReady)
        {			
            PlatformStru_Position lPostion;
            lIterReady->second->QryPositionReach(lPostion,-100,true);
        }		
    }

    //装载主帐号的持仓明细
    vector<PlatformStru_PositionDetail> lvecPositionDetail;
    if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountPositionDetails("TRADEDATA_A_PD_INIT",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecPositionDetail)*/)
    {
        //设置状态
        if(mMapAccountData.end() != lIterReady)
        {
            vector<PlatformStru_PositionDetail>::iterator lIte = lvecPositionDetail.begin();
            for(;lIte != lvecPositionDetail.end();lIte++)
            {			
                strcpy(lIte->Account,lIte->InvestorID);
                lIterReady->second->QryPositionDetailReach(*lIte,0,false);	
            }	
            PlatformStru_PositionDetail lPostionDetail;
            lIterReady->second->QryPositionDetailReach(lPostionDetail,-100,true);
        }			
    }
    else
    {
        //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该主帐号%s的持仓明细",szTradeAccount);
        if(mMapAccountData.end() != lIterReady)
        {			
            PlatformStru_PositionDetail lPostionDetail;
            lIterReady->second->QryPositionDetailReach(lPostionDetail,-100,true);
        }		
    }


    //装载主帐号的成交
    vector<PlatformStru_TradeInfo> lvecTrader;
    if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountTrades("TRADEDATA_A_T_INIT",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecTrader)*/)
    {
        //设置状态
        if(mMapAccountData.end() != lIterReady)
        {
            vector<PlatformStru_TradeInfo>::iterator lIte = lvecTrader.begin();
            for(;lIte != lvecTrader.end();lIte++)
            {				
                strcpy(lIte->Account,lIte->InvestorID);
                lIterReady->second->QryTradeReach(*lIte,0,false);	
                lIterReady->second->SetLoadTraderUpdateSeq(lIte->UpdateSeq);
            }	
            PlatformStru_TradeInfo lTrader;
            lIterReady->second->QryTradeReach(lTrader,-100,true);	

        }			
    }
    else
    {
        //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该主帐号%s的成交",szTradeAccount);
        if(mMapAccountData.end() != lIterReady)
        {			
            PlatformStru_TradeInfo lTrader;
            lIterReady->second->QryTradeReach(lTrader,-100,true);	
        }			
    }

    //装载主帐号的报单
    vector<PlatformStru_OrderInfo> lvecOrder;
    if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountOrders("TRADEDATA_A_O_INIT",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecOrder)*/)
    {
        //设置状态
        if(mMapAccountData.end() != lIterReady)
        {
            vector<PlatformStru_OrderInfo>::iterator lIte = lvecOrder.begin();
            for(;lIte != lvecOrder.end();lIte++)
            {		
                strcpy(lIte->Account,lIte->InvestorID);			
                lIte->troubleMoney = util::GetDoubleInvalidValue();	
                lIte->freezeMargin = util::GetDoubleInvalidValue();
                lIterReady->second->QryOrderReach(*lIte,0,false);
                lIterReady->second->SetLoadOrderUpdateSeq(lIte->UpdateSeq);
            }	
            PlatformStru_OrderInfo lOrder;
            lIterReady->second->QryOrderReach(lOrder,-100,true);
        }			
    }
    else
    {
        //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该主帐号%s的报单",szTradeAccount);
        if(mMapAccountData.end() != lIterReady)
        {			
            PlatformStru_OrderInfo lOrder;
            lIterReady->second->QryOrderReach(lOrder,-100,true);
        }			
    }

//     //装载主帐号的报单
//     vector<PlatformStru_OrderInfo> lvecSecondOrder;
//     if(CF_ERROR_SUCCESS == mpDBOpr->LoadTodayAccountOrders("CTP_ORDERS",lBroker.szBrokerCode,lIter->szTradeAccount,lsTempTradingDay,lvecSecondOrder))
//     {
//         //设置状态		
//         if(mMapAccountData.end() != lIterReady)
//         {	
//             //大于前面转载的updateseq的报单才会被push
//             int lnLastSeq=0;
//             lIterReady->second->GetLoadOrderUpdateSeq(lnLastSeq);
//             vector<PlatformStru_OrderInfo>::iterator lIte = lvecSecondOrder.begin();
//             for(;lIte != lvecSecondOrder.end();lIte++)
//             {	
// 
//                 if(lIte->UpdateSeq > lnLastSeq)
//                 {
//                     lIte->troubleMoney = util::GetDoubleInvalidValue();	
//                     lIte->freezeMargin = util::GetDoubleInvalidValue();
//                     lIterReady->second->PushOrder(*lIte);
//                     lIterReady->second->SetLoadOrderUpdateSeq(lIte->UpdateSeq);
//                 }
//             }	
// 
//         }	
//     }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //装载子帐号的数据
    std::string lsTempDay = msCurrentTradingDay.substr(0,4) + msCurrentTradingDay.substr(5,2) + msCurrentTradingDay.substr(8,2);	
    _MapUserTradeData::iterator lIterUser =  mMapUser2TradingData.begin();
    for(;lIterUser != mMapUser2TradingData.end();lIterUser++)
    {
        //装载子帐号的资金
        PlatformStru_TradingAccountInfo lAccountInfo;	
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserFundInfo("SETTLEMENT_FUNDINIT",lIterUser->first,lsLastTradingDay,lAccountInfo)*/)
        {
            //设置状态
            lIterUser->second->SetQryFundInfo(lAccountInfo);
            if(mbNeedCache)
                pCmdCache->CacheFund(lAccountInfo);
        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该用户%s的资金",lIterUser->first.c_str());

            //设置空资金和accountid
            PlatformStru_TradingAccountInfo lFundInfo;    
            lFundInfo.clear();
            strcpy(lFundInfo.BrokerID,"0000");
            strcpy(lFundInfo.AccountID,lIterUser->first.c_str());	
            strcpy(lFundInfo.TradingDay,lsTempDay.c_str());
            //lFundInfo.Available = dFund;
            //lFundInfo.StaticProfit = dFund;
            //lFundInfo.DynamicProfit = dFund;
            lFundInfo.PreBalance = dFund;
            lIterUser->second->SetQryFundInfo(lFundInfo);
            if(mbNeedCache)
                pCmdCache->CacheFund(lAccountInfo);
        }


        //设置初始资金
        double ldbVal = dFund;
        LoadInitialInitFundFromDB(lIterUser->first,ldbVal);
        lIterUser->second->SetInitFund(ldbVal);

        //装载出入金
        //获取用户今天的出入金
        vector<sFundInOut> lvecFundInOut;        
        sQryFundInOut ls;
        strcpy(ls.msStartDay,msInstrumentDay.c_str());
        strcpy(ls.msEndDay,msInstrumentDay.c_str());
        strcpy(ls.mUserID,lIterUser->first.c_str());
//         sFundInOut inout;
//         strcpy(inout.mUserID,szUser.c_str());     //交易员账户
//         eInOut       meInOut;     //出或入
//         double       mdbVolume;   //金额
//         char         msDesc[200]; //备注
//         _UserIDType  mOpAdminID;  //操作的管理员账户
//         char		 msDay[11];   //操作日期
//         char		 msTime[9];   //操作时间
//         lvecFundInOut.push_back(ls);
        //mpDBOpr->LoadQryFundInOut(ls,lvecFundInOut);
        for(unsigned int i = 0; i < lvecFundInOut.size();i++)
        {
            lIterUser->second->UserFundInOut(lvecFundInOut[i]);
        }

        //缓存资金
        if(mbNeedCache)
        {
            PlatformStru_TradingAccountInfo lFundInfo;
            lIterUser->second->GetFundInfo(lFundInfo);
            pCmdCache->CacheFund(lAccountInfo);
        }

        //装载主帐号的持仓
        vector<PlatformStru_Position> lvecPosition;
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserPositions("SETTLEMENT_POSITIONINIT",lIterUser->first,lsLastTradingDay,lvecPosition)*/)
        {
            //设置状态
            vector<PlatformStru_Position>::iterator lIte = lvecPosition.begin();
            for(;lIte != lvecPosition.end();lIte++)
            {	
                strcpy(lIte->TradingDay,lsTempDay.c_str());
                PlatformStru_InstrumentMarginRate lMargin;
                lMargin.clear();
                if(0 == lIterUser->second->GetMarginRate(lIte->InstrumentID,lMargin))
                {
                    lIte->MarginRateByMoney = 
                        lIte->PosiDirection == THOST_FTDC_PD_Long ? lMargin.LongMarginRatioByMoney : lMargin.ShortMarginRatioByMoney;
                    lIte->MarginRateByMoney = 
                        lIte->PosiDirection == THOST_FTDC_PD_Long ? lMargin.LongMarginRatioByVolume : lMargin.ShortMarginRatioByVolume;

                }			


                strcpy(lIte->Account,lIte->InvestorID);
                lIterUser->second->QryPositionReach(*lIte,0,false);	
                if(mbNeedCache)
                {
                    pCmdCache->CachePosition(*lIte);
                    TRADE_LOG("PushTrade CachePosition InstrumentID=%s CanCloseTodayVolume=%d CanCloseVolume=%d CanCloseydVolume=%d\
                              CloseProfit=%.4f CloseTodayOrderVolume=%d CloseOrderVolume=%d CloseVolume=%d InvestorID=%s CanCloseydVolume=%d\
                              OpenVolume=%d Position=%d TodayPosition=%d YdPosition=%d",
                              lIte->InstrumentID,lIte->CanCloseTodayVolume,lIte->CanCloseVolume,lIte->CanCloseydVolume,
                              lIte->CloseProfit,lIte->CloseTodayOrderVolume,lIte->CloseOrderVolume,lIte->CloseVolume,
                              lIte->InvestorID,lIte->OpenVolume,lIte->Position,lIte->TodayPosition,lIte->YdPosition);

                }
            }	
            PlatformStru_Position lPostion;
            lIterUser->second->QryPositionReach(lPostion,-100,true);
        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该用户%s的持仓",lIterUser->first.c_str());
            PlatformStru_Position lTemp;
            lIterUser->second->QryPositionReach(lTemp,-100,true);
        }

        //装载主帐号的持仓明细
        vector<PlatformStru_PositionDetail> lvecPositionDetail;
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserPositionDetails("SETTLEMENT_POSITIONDETAILINIT",lIterUser->first,lsLastTradingDay,lvecPositionDetail)*/)
        {
            //设置状态
            vector<PlatformStru_PositionDetail>::iterator lIte = lvecPositionDetail.begin();
            for(;lIte != lvecPositionDetail.end();lIte++)
            {				
                strcpy(lIte->Account,lIte->InvestorID);
                strcpy(lIte->TradingDay,lsTempDay.c_str());
                PlatformStru_InstrumentMarginRate lMargin;
                lMargin.clear();
                if(0 == lIterUser->second->GetMarginRate(lIte->InstrumentID,lMargin))
                {
                    lIte->MarginRateByMoney = 
                        lIte->Direction == THOST_FTDC_D_Buy ? lMargin.LongMarginRatioByMoney : lMargin.ShortMarginRatioByMoney;
                    lIte->MarginRateByMoney = 
                        lIte->Direction == THOST_FTDC_D_Buy ? lMargin.LongMarginRatioByVolume : lMargin.ShortMarginRatioByVolume;
                }			

                lIterUser->second->QryPositionDetailReach(*lIte,0,false);
                if(mbNeedCache)
                    pCmdCache->CachePositionDetail(*lIte);
            }	
            PlatformStru_PositionDetail lPostionDetail;		
            lIterUser->second->QryPositionDetailReach(lPostionDetail,-100,true);

        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该用户%s的持仓明细",lIterUser->first.c_str());
            PlatformStru_PositionDetail lPostionDetail;		
            lIterUser->second->QryPositionDetailReach(lPostionDetail,-100,true);
        }


        //装载子帐号的成交
        vector<PlatformStru_TradeInfo> lvecTrader;
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserTrades("TRADEDATA_TRADES",lIterUser->first,lsTempTradingDay,lvecTrader)*/)
        {
            //设置状态
            vector<PlatformStru_TradeInfo>::iterator lIte = lvecTrader.begin();
            for(;lIte != lvecTrader.end();lIte++)
            {
                strcpy(lIte->Account,lIte->InvestorID);
                lIterUser->second->SetLoadTraderUpdateSeq(lIte->UpdateSeq);				
                lIterUser->second->QryTradeReach(*lIte,0,false);	

            }	
            PlatformStru_TradeInfo lTrader;
            lIterUser->second->QryTradeReach(lTrader,-100,true);			
        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该用户%s的成交",lIterUser->first.c_str());
            PlatformStru_TradeInfo lTrader;
            lIterUser->second->QryTradeReach(lTrader,-100,true);	
            //continue;
        }

        //装载主帐号的报单
        vector<PlatformStru_OrderInfo> lvecOrder;
        if(false/*CF_ERROR_SUCCESS == mpDBOpr->LoadTodayUserOrders("TRADEDATA_ORDERS",lIterUser->first,lsTempTradingDay,lvecOrder)*/)
        {
            //设置状态
            vector<PlatformStru_OrderInfo>::iterator lIte = lvecOrder.begin();
            for(;lIte != lvecOrder.end();lIte++)
            {		
                strcpy(lIte->Account,lIte->InvestorID);			
                lIte->troubleMoney = util::GetDoubleInvalidValue();	
                lIte->freezeMargin = util::GetDoubleInvalidValue();
                lIterUser->second->SetLoadOrderUpdateSeq(lIte->UpdateSeq);
                lIterUser->second->QryOrderReach(*lIte,0,false);

                if(mbNeedCache)
                    pCmdCache->CacheOrder(*lIte);
            }	
            PlatformStru_OrderInfo lOrder;
            lIterUser->second->QryOrderReach(lOrder,-100,true);

        }
        else
        {
            //PUSH_LOG_ERROR(Logger,false,true,"数据库中没有该用户%s的报单",lIterUser->first.c_str());
            PlatformStru_OrderInfo lOrder;
            lIterUser->second->QryOrderReach(lOrder,-100,true);
        }
    }

    SetTraderDataStatus(TradeDataStatusTrading);

    return true;
}

//检查每个用户和主服务是否同步完成
bool CReal_SvrTradeData::CheckAllUserAccountSyn()
{
	bool lbRet = false;
	do 
	{
		TRADE_LOG("Start Check All User Finished")

// 		CSafeRwLockRead lrlock(mrwLockMapUserTrading);
// 		_MapUserTradeData::iterator lIter =  mMapUser2TradingData.begin();
// 		int lnFinishCount = 0;
// 		while(mMapUser2TradingData.end() != lIter)	
// 		{		
// 			//判断用户是否是空用户，没有绑定委托交易账号
// 			int nUserID = -1;
// 			vector<UserInfo> lVecUeser;
// 			CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
// 			for(unsigned int i = 0;i < lVecUeser.size();i++)
// 			{
// 				if(strcmp(lVecUeser[i].szAccount,lIter->first.c_str()) == 0)
// 				{
// 					nUserID = lVecUeser[i].nUserID;
// 					break;
// 				}
// 			}
// 			TradeAccount tradeAccount;
// 			if(!CInterface_SvrUserOrg::getObj().GetTradeAccountByUserID(nUserID,tradeAccount))			
// 			{
// 				TRADE_LOG("User %s Finished with Empty Trader Account",lIter->first.c_str());
// 				lnFinishCount++;
// 			}
// 			else
// 			{
// 				if( lIter->second->IsSyncFinished()  )
// 				{
// 					TRADE_LOG("User %s Finished with Qry End",lIter->first.c_str());
// 					lnFinishCount++;
// 				}
// 				
// 			}
// 			lIter++;
// 		}
// 
// 		if(lnFinishCount == mMapUser2TradingData.size())
// 		{
// 			lbRet = true;
// 			TRADE_LOG("Exit Check All User Finished")
// 				break;
// 		}

		//TRADE_LOG("End Check All User Finished with %d/%d",lnFinishCount,mMapUser2TradingData.size())
			break;
		//Sleep(1000);
	} while (true);


	return lbRet;

}

void CReal_SvrTradeData::AddCommand(const std::string & nsCommand)
{
	CSafeLock ls(mCommandMutex);
	if(!nsCommand.empty())
	{
		mCommandList.push(nsCommand);	
		//PostThreadMessage(g_idThread,TMESSAGE_START_DEAL_CACHE_CHANNEL_COMMAND,0,0);
	}
}

void CReal_SvrTradeData::DealCommand()
{
	CSafeLock ls(mCommandMutex);
	while(mCommandList.size() != 0)
	{
		std::string lCommandStr = mCommandList.front();
		//if(lCommandStr is orderInput)
		//  send master a orderinput 
		//if(lCommandStr is orderAction)
		//  send master a orderaction
		//if(lCommandStr is heartbeat)
		// update user:strategy updatetime
		 std::size_t found = std::string::npos;
		 if( (found = lCommandStr.find(ORDERINPUT_CHANNEL_NAME)) != std::string::npos)
		 {
			 //获取数据库读取最大的updateseq
// 			 CInterface_SvrTcp::getObj().SendPkgData(mMasterSocket,
// 				 Cmd_TradeExcute_StategyInputOrder_Req,
// 				 (void*)lCommandStr.c_str(),
// 				 lCommandStr.size()+1,
// 				 0,
// 				 0,
// 				 0,
// 				 0,
// 				 0,0);
		 }
		 else if((found = lCommandStr.find(ORDERACTION_CHANNEL_NAME)) != std::string::npos) 
		 {
			 //获取数据库读取最大的updateseq
// 			 CInterface_SvrTcp::getObj().SendPkgData(mMasterSocket,
// 				 Cmd_TradeExcute_StategyActionOrder_Req,
// 				 (void*)lCommandStr.c_str(),
// 				 lCommandStr.size()+1,
// 				 0,
// 				 0,
// 				 0,
// 				 0,
// 				 0,0);
		 }
		 else  if((found = lCommandStr.find(HEARTBEAT_CHANNEL_NAME)) != std::string::npos) 
		 {
			 //set heartbeat updatetime
			/* time_t curtime=time(NULL);
			 CSafeLock llock(mUpdateTimeMapMutex);
			 bool lbRet = false;
			 std::string lsKey;
			 mStrategyInstanceUpdateTimeMap[lsKey] = curtime;*/
		 }
		mCommandList.pop();
	}
	return;
}

void CReal_SvrTradeData::AddCommandCallback(const std::string & nsCommand)
{
	CReal_SvrTradeData::getObj().AddCommand(nsCommand);
}
bool CReal_SvrTradeData::GetStategyUserInstanceUpdateTime(const std::string & nsUserName,
											  const std::string & nsStategyName,
											  time_t & nsUpdateTime) 
{
	CSafeLock llock(mUpdateTimeMapMutex);
	bool lbRet = false;
	std::string lsKey = nsUserName + ":"+nsStategyName;
	std::map<std::string ,time_t>::iterator lIter = mStrategyInstanceUpdateTimeMap.find(lsKey);
	if(lIter != mStrategyInstanceUpdateTimeMap.end())
	{
		nsUpdateTime = lIter->second;
		lbRet = true;
	}
	return lbRet;		
}