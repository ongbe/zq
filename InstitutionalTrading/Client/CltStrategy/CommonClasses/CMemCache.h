

#pragma once
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <winsock2.h>
#include "windows.h"

#include "hiredis.h"

#include <string>
#include "easymutex.h"
#include "KernelStruct.h"



/*指向类的非静态成员函数的指针*/
typedef void (*DealCommandFun)(const std::string & nsTableName);
										   
class CMemCache
{
private:
	redisContext* mConnect;
	redisReply* mReply;
	Ceasymutex mMutex;

public:
	CMemCache()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD( 2, 2 ), &wsaData);
		mConnect = NULL;
		mReply = NULL;
	}

	~CMemCache()
	{
        if(mReply!=NULL)
            freeReplyObject(mReply);
        if(mConnect!=NULL)
            redisFree(mConnect);

		this->mConnect = NULL;
		this->mReply = NULL;
		WSACleanup();
	}

	bool connect(const std::string& host, int port)
	{
		this->mConnect = redisConnect(host.c_str(), port);
		if (this->mConnect->err) {
			return false;
		}
		return true;
	}

    void disconnect()
    {
		if(this->mConnect!=NULL) {
            redisFree(this->mConnect);
			this->mConnect = NULL;
		}
    }

    bool getConnected()
    {
        return this->mConnect!=NULL;
    }

	std::string getError()
	{
		return this->mConnect->errstr;
	}

	void FlushAll()
	{
		if(mConnect)
		{
				redisReply* lp = (redisReply*)redisCommand(this->mConnect, "FlushAll");			
				freeReplyObject(lp);
		}
	}

	bool CachePosition(const PlatformStru_Position & nPos );
	bool CachePositionDetail(const PlatformStru_PositionDetail & nPosDetail );
	bool CacheFund(const PlatformStru_TradingAccountInfo & nFundInfo);
	bool CacheOrder(const PlatformStru_OrderInfo & nOrder);

	bool PushQuot(const PlatformStru_DepthMarketData& nQuot);
	bool PushOrder(const PlatformStru_OrderInfo& nOrder);
	bool PushTrader(const PlatformStru_TradeInfo& nOrder);
	bool PushOrderInputError(const PlatformStru_OrderInfo & nOrder);
	bool PushOrderActionError(const CThostFtdcOrderActionField & nOrder);
	bool PushStrategyStopOrRun(const std::string & nsName,bool nsStop);
	bool PushTraderUseroffLine(const std::string & nUserName,bool bOffline);
	bool PushStrategyInstanceStopOrRun(const std::string & nsUserName,const std::string& nStrategyName,bool nsStop);

	bool PushQuotTimer();

	bool SubscibleRead(DealCommandFun pFun);


	static std::string OrderToString(const PlatformStru_OrderInfo & nOrder);
	std::string OrderInputToString(const PlatformStru_OrderInfo & nOrder);
	static std::string TraderToString(const PlatformStru_TradeInfo & nOrder);
	static std::string PositionToString(const PlatformStru_Position & nOrder);
	static std::string FundToString(const PlatformStru_TradingAccountInfo & nOrder);
	static std::string OrderActionToString(const CThostFtdcOrderActionField & nOrder);
	static std::string PositionDetailToString(const PlatformStru_PositionDetail & nOrder);
	static std::string QuotToString(const PlatformStru_DepthMarketData & nOrder);
};
