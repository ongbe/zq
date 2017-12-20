#include "stdafx.h"
#include "TradingDayManager.h"
#include "SafeLock.h"
#include "..\SvrDBOpr\Interface_SvrDBOpr.h"
#include "time.h"
CTradingDayManager::CTradingDayManager()
{
	LoadHistoryTradingDay();
	LoadHistorySettmentedDay();
}
CTradingDayManager::~CTradingDayManager()
{
	CSafeLock mLock(mMutex);
	//msetAllTradingDay.erase(msetAllTradingDay.begin(),msetAllTradingDay.end());
	mmapAllTradingDay.clear();
	msetAllSettlementedDay.clear();
};

//装置历史的交易日
void CTradingDayManager::LoadHistoryTradingDay()
{
	CSafeLock mLock(mMutex);
	//装置数据库
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[100] = {0};	
	strcpy_s(szBuffer, "select * from TradingDay t");
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return ;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			char szDay[11] = {0};
			std::vector<_variant_t> vValue = vNode[j];
			int i = 0;
			strcpy(szDay, vValue[i++].operator _bstr_t());		
			i++;
            eTradingDayInitStatus leInitStatus = static_cast<eTradingDayInitStatus>(vValue[i++].intVal);
			

			std::map<std::string,eTradingDayInitStatus>::iterator lIter = mmapAllTradingDay.find(szDay);
			if(lIter == mmapAllTradingDay.end() || 
				(lIter != mmapAllTradingDay.end() && lIter->second == TRADING_DAY_INIT_STATUS_NOTHING))
				mmapAllTradingDay[szDay] = leInitStatus;

		}
	}

	return;
}

//装置历史的结算日
void  CTradingDayManager::LoadHistorySettmentedDay()
{
	CSafeLock mLock(mMutex);
	//装置数据库
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[100] = {0};	
	strcpy_s(szBuffer, "select * from SETTLEMENTDAY t");
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return ;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			char szDay[11] = {0};
			std::vector<_variant_t> vValue = vNode[j];
			int i = 0;
			strcpy(szDay, vValue[i++].operator _bstr_t());			


			msetAllSettlementedDay.insert(szDay);

		}
	}

	return;
}

//判断是否有盘中初始化过的交易日未结算
bool CTradingDayManager::HasInitTradingDayWithNoSettlement(std::string & nsOldestInitTradingDayWithNoSettlement)
{
	nsOldestInitTradingDayWithNoSettlement.clear();
	std::map<std::string,eTradingDayInitStatus>::iterator  lIter = mmapAllTradingDay.begin();
	while(lIter != mmapAllTradingDay.end())
	{
		if(lIter->second == TRADING_DAY_INIT_STATUS_INSTRUMENT && !IsSetttemnted(lIter->first))
		{
			nsOldestInitTradingDayWithNoSettlement = lIter->first;			
			break;
		}
		lIter++;
	}
	if(nsOldestInitTradingDayWithNoSettlement.empty())
		return false;
	else
		return true;
}

//获取上一个交易日,如果一个交易日未初始化，则该交易日是无效的交易日
bool CTradingDayManager::GetLastTradingData(const std::string & nDay,std::string & nLastTradingDay)
{
	CSafeLock mLock(mMutex);

	if(nDay.empty())
		return false;

	std::map<std::string,eTradingDayInitStatus>::iterator  lIter = mmapAllTradingDay.find(nDay);
	while(lIter != mmapAllTradingDay.end())
	{
		std::map<std::string,eTradingDayInitStatus>::iterator lIterLast = --lIter;
		if(lIterLast != mmapAllTradingDay.end() && lIterLast->second == TRADING_DAY_INIT_STATUS_INSTRUMENT)
		{
			//nLastTradingDay == lIterLast->first;
			nLastTradingDay.assign(lIterLast->first);
			
			return true;
		}
		
	}
	return false;
	//nday 历史倒流，直到在历史列表找到为止
	//time_t lTime;
	//std::string lsTempStr;
	//struct tm * lOldTimeinfo; 
	//time(&lTime);
	//lOldTimeinfo = localtime(&lTime);
	//sscanf_s(nDay.c_str(),"%04d-%02d-%02d",&lOldTimeinfo->tm_year,&lOldTimeinfo->tm_mon,&lOldTimeinfo->tm_mday);

	//lOldTimeinfo->tm_year -= 1900;
	//lOldTimeinfo->tm_mon -= 1;
	//time_t lOldTime = mktime(lOldTimeinfo);

	////尝试倒流30天，历史的一个月内必有一天是交易日，如果没有则失败
	//bool lbRet = false;
	//time_t lYesTime = lOldTime ;
	//int i = 0;
	//do 
	//{
	//	lYesTime = lYesTime - 24* 3600;
	//	struct tm * ltmYestorday = localtime(&lYesTime);
	//	char ltempBuf[11] = {0};
	//	sprintf_s(ltempBuf,"%04d-%02d-%02d",ltmYestorday->tm_year+1900,ltmYestorday->tm_mon+1,ltmYestorday->tm_mday);
	//	std::map<std::string,eTradingDayInitStatus>::iterator  lIter = mmapAllTradingDay.find(ltempBuf);
	//	if(lIter != mmapAllTradingDay.end() && lIter->second == TRADING_DAY_INIT_STATUS_INSTRUMENT)
	//	{
	//		nLastTradingDay = ltempBuf;
	//		lbRet = true;
	//		break;
	//	}
	//	i++;
 //      
	//} while (i <= 30);



	//return lbRet;
}

//添加一个交易日
bool CTradingDayManager::AddTradingDay(const std::string & nDay)
{
	CSafeLock mLock(mMutex);
	if(mmapAllTradingDay.find(nDay) == mmapAllTradingDay.end())
	{
		mmapAllTradingDay[nDay] = TRADING_DAY_INIT_STATUS_NOTHING;
		
		if(mbNeedSaveDB != false)
		{
			//落地
			int nErrorCode;
			CInterface_SvrDBOpr::getObj().SaveOneTradingDay("TradingDay",nDay,TRADING_DAY_INIT_STATUS_NOTHING,nErrorCode);
		}
	}
	return true;
}


//设置一个交易日初始化完毕了
bool CTradingDayManager::SetTradingDayInit(const std::string & nDay)
{
	CSafeLock mLock(mMutex);
	std::map<std::string,eTradingDayInitStatus>::iterator  lIter = mmapAllTradingDay.find(nDay);
	if(lIter != mmapAllTradingDay.end())
	{
		mmapAllTradingDay[nDay] = TRADING_DAY_INIT_STATUS_INSTRUMENT;
		
		if(mbNeedSaveDB != false)
		{
			//落地
			int nErrorCode;
			CInterface_SvrDBOpr::getObj().UpdateTradingDayInitStatus("TradingDay",nDay,TRADING_DAY_INIT_STATUS_INSTRUMENT,nErrorCode);
		}
	}
	return true;
}
//添加一个结算日
bool CTradingDayManager::AddSettlementedDay(const std::string & nDay)
{
	CSafeLock mLock(mMutex);
	if(msetAllSettlementedDay.find(nDay) == msetAllSettlementedDay.end())
	{
		msetAllSettlementedDay.insert(nDay);
		
		if(mbNeedSaveDB != false)
		{
			//落地
			int nErrorCode;
			CInterface_SvrDBOpr::getObj().SaveOneSettlementDay("SETTLEMENTDAY",nDay,nErrorCode);
		}
	}
	return true;
}

bool CTradingDayManager::IsTradingDay(const std::string & nsDay)
{
	CSafeLock mLock(mMutex);
	if(mmapAllTradingDay.find(nsDay) == mmapAllTradingDay.end())
		return false;

	return true;
}

//是不是系统运行的第一天
bool CTradingDayManager::IsFirstDay()
{
	CSafeLock mLock(mMutex);
	return mmapAllTradingDay.size() == 0 ? true : false;
}

//是否结算过
bool CTradingDayManager::IsSetttemnted(const std::string & nsDay)
{
	CSafeLock mLock(mMutex);
	if(msetAllSettlementedDay.find(nsDay) == msetAllSettlementedDay.end())
		return false;

	return true;
}


//获取最新的已结算的交易日
bool CTradingDayManager::GetNewestSettlementDay( std::string & nDay)
{
	CSafeLock mLock(mMutex);
	std::set<std::string>::reverse_iterator lLast = msetAllSettlementedDay.rbegin();
	if(lLast == msetAllSettlementedDay.rend())
		return false;
	else
	{
		nDay.assign( *lLast);
		return true;
	}
}