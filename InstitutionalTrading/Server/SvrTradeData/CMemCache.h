

#pragma once
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "windows.h"

#include "..\OpenSource\Hiredis\include\hiredis.h"
//#include "..\OpenSource\Hiredis\include\async.h"
//#include "..\OpenSource\Hiredis\include\adapters\libevent.h"



//
//#ifdef _DEBUG
//#pragma comment(lib,"..\\OpenSource\\Hiredis\\lib\\HiredisD.lib")
////#pragma comment(lib,"..\\OpenSource\\libevent\\lib\\libevent.lib")
//#else
//#pragma comment(lib,"..\\OpenSource\\Hiredis\\lib\\Hiredis.lib")
//#endif

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
    bool mbSeverQuit;

public:
	CMemCache()
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD( 2, 2 ), &wsaData);
		mConnect = NULL;
		mReply = NULL;
        mbSeverQuit=false;
	}

	~CMemCache()
	{
        mbSeverQuit=true;
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
            redisFree(this->mConnect);
            printf("Connect to redis server failed\n");
			return false;
		}
        printf("Connect to redisServer Success\n");  
		return true;
	}

    bool disconnect()
    {
        if(mConnect == NULL || 1==mConnect->err)
            return false;

        mbSeverQuit=true;

        redisReply* lp = (redisReply*)redisCommand(mConnect,"QUIT");
        if(lp)
        {
            freeReplyObject(lp);
        }        

        //get client list        
        //std::vector<std::string> vIp_Port;
        //redisReply* lp = (redisReply*)redisCommand(mConnect,"CLIENT LIST");
        //if(lp)
        //{
        //    getClientList(lp->str,vIp_Port);
        //    freeReplyObject(lp);
        //}

        //if(!vIp_Port.empty())
        //{
        //    std::string IP_PORT;
        //    for (std::vector<std::string>::iterator it = vIp_Port.begin();it!=vIp_Port.end();it++)
        //    {
        //        IP_PORT= *it;
        //        lp = (redisReply*)redisCommand(mConnect,"CLIENT KILL %s", IP_PORT.c_str());
        //        if(lp)
        //            freeReplyObject(lp);
        //    }
        //}

        return true;
    }

    bool shutdownRedisServer()
    {
        if(mConnect == NULL || 1==mConnect->err)
            return false;

        redisReply* lp = (redisReply*)redisCommand(mConnect,"SHUTDOWN");
        if(lp)
        {
            freeReplyObject(lp);
        }

        return true;
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

    void getClientList(std::string szText,std::vector<std::string>& vList)
    {
        if(szText.empty())
            return;

        std::string szLien,szDest,szTextSrc = szText;
        const char* pData = szText.c_str();


        int nLen = 0;
        int nPos = 0;
        int nCount = strlen(pData);


        int nLFCount;
        int nNull;   
        
        nLFCount = szText.find("addr");
        nNull = szText.find(' ');
        while(nLen<nCount)
        {
            if(nLFCount==-1 || nNull==-1) break;

            szLien = szText.substr(5,nNull-5);
            szText = szText.substr(nNull+1,nCount-nNull+1);

            vList.push_back(szLien);

            nLFCount = szText.find("addr");
            if(nLFCount==-1) break;

            szText = szText.substr(nLFCount,nCount-nLFCount+1);
            nNull = szText.find(' ');
            if(nNull==-1) break;

            nCount = strlen(pData);
        }


//         while(nLen<nCount)
//         {
//             int nLFCount = szText.find("\r");
//             if(nLFCount==-1) 
//                 break;
//             szLien = szText.substr(nLen,nLFCount);
// 
//             nPos = szLien.find(' ');
//             szDest = szLien.substr(5,nPos-5);
// 
//             vList.push_back(szDest);
// 
//             szText = szText.substr(nLFCount+1,nCount-nLFCount+1);
//             nCount = strlen(pData);
//         }
    }
	//std::string get(std::string key)
	//{
	//	this->mReply = (redisReply*)redisCommand(this->mConnect, "GET %s", key.c_str());
	//	std::string str = this->mReply->str;
	//	freeReplyObject(this->mReply);

	//	return str;
	//}

	//void set(std::string key, std::string value)
	//{
	//	redisCommand(this->mConnect, "SET %s %s", key.c_str(), value.c_str());
	//}

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
	//bool PushTraderUseroffLine(const std::string & nUserName,bool bOffline);
	bool PushStrategyInstanceStopOrRun(const std::string & nsUserName,const std::string& nStrategyName,bool nsStop);
    bool PushStrategyTimer(const std::string & nsName,bool nsStop);


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

//
//
//class CAsynMemCache
//{
//private:
//	redisContext* mConnect;
//	redisReply* mReply;
//	Ceasymutex mMutex;
//
//public:
//	CAsynMemCache()
//	{
//		WSADATA wsaData;
//		WSAStartup(MAKEWORD( 2, 2 ), &wsaData);
//		mConnect = NULL;
//		mReply = NULL;
//	}
//
//	~CAsynMemCache()
//	{
//		this->mConnect = NULL;
//		this->mReply = NULL;
//		WSACleanup();
//	}
//
//	bool connect(const std::string& host, int port)
//	{
//		signal(13, SIG_IGN);  
//		struct event_base *base = event_base_new();  
//
//		redisAsyncContext *c = redisAsyncConnect(host.c_str(), port);
//		if (c->err) {  
//			/* Let *c leak for now... */  
//			//printf("Error: %s\n", c->errstr);  
//			return false;  
//		}  
//
//		redisLibeventAttach(c,base);  
//		redisAsyncSetConnectCallback(c,CAsynMemCache::connectCallback);  
//		redisAsyncSetDisconnectCallback(c,CAsynMemCache::disconnectCallback);  
//		redisAsyncCommand(c, (redisCallbackFn *)CAsynMemCache::SubCallback, (char*) "sub", "SUBSCRIBE OrderInput");  
//
//		event_base_dispatch(base);  
//		return true;
//	}
//
//	
//
//public:
//	//callback
//	static void connectCallback(const redisAsyncContext *c, int status) {
//		if (status != REDIS_OK) {
//			printf("Error: %s\n", c->errstr);
//			return;
//		}
//		printf("Connected...\n");
//	}
//
//	static void disconnectCallback(const redisAsyncContext *c, int status) {
//		if (status != REDIS_OK) {
//			printf("Error: %s\n", c->errstr);
//			return;
//		}
//		printf("Disconnected...\n");
//	}
//
//	static void SubCallback(const redisAsyncContext *c,void *r, void *priv) {
//		redisReply *reply = (redisReply*)r;  
//		if (reply == NULL) return;  
//		if ( reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 )
//		{  
//			if ( strcmp( reply->element[0]->str, "subscribe" ) != 0 ) {  
//				printf( "Received[%s] channel %s: %s\n",  
//					(char*)priv,  
//					reply->element[1]->str,  
//					reply->element[2]->str );  
//			}  
//		}  
//	}
//	//std::string get(std::string key)
//	//{
//	//	this->mReply = (redisReply*)redisCommand(this->mConnect, "GET %s", key.c_str());
//	//	std::string str = this->mReply->str;
//	//	freeReplyObject(this->mReply);
//
//	//	return str;
//	//}
//
//	//void set(std::string key, std::string value)
//	//{
//	//	redisCommand(this->mConnect, "SET %s %s", key.c_str(), value.c_str());
//	//}
//
//	
//
//};