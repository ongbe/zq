/***************************************************************************//**
*   @file          PlatformCTP.h
*   @brief        综合交易平台(CTP)交易接口
*
********************************************************************************/
#ifndef _PLATFORMCTP_H_
#define _PLATFORMCTP_H_

#include <string>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::multimap;


#include "VariablePackage.h"
#include "easymutex.h"
#include "easytradingtimespan.hpp"


#pragma managed(push,off)

#include "ctp/ThostFtdcTraderApi.h"
#include "ctp/ThostFtdcMdApi.h"


//下面定义交易所连接状态
enum eCTPCONNSTATUS
{    
    CTPCONNSTATUS_Disconnected   =      0       ,           //未连接
    CTPCONNSTATUS_Connecting     =      1       ,           //正在连接
    CTPCONNSTATUS_Logining       =      2       ,           //正在登录
    CTPCONNSTATUS_LoginFailure   =      3       ,           //登录失败
    CTPCONNSTATUS_Connected      =      6       ,           //连接登录成功
    CTPCONNSTATUS_Disconnecting  =      7       ,           //正在断开连接
    CTPCONNSTATUS_UnInit         =      8                   //未初始化
};


//包命令字
enum ePkgCmd
{
    CmdID_Quot_FrontConnected           =13000,			    //与行情后台通信连接成功
    CmdID_Quot_FrontDisconnected,                           //与行情后台通信连接断开
    CmdID_Quot_RspUserLogin,                                //登录行情响应包
    CmdID_Quot_RspUserLogout,                               //登出行情响应包
    CmdID_Quot_RtnDepthMarketData,                          //深度行情包

    CmdID_Trade_FrontConnected          =13200,             //与交易ctp通信连接成功
    CmdID_Trade_FrontDisconnected,                          //与交易ctp通信连接断开
    CmdID_Trade_RspUserLogin,                               //登录交易响应包
    CmdID_Trade_RspUserLogout,                              //登出交易响应包
    CmdID_Trade_RspQrySettlementInfoConfirm,                //请求查询结算信息确认响应
    CmdID_Trade_RspSettlementInfoConfirm,                   //请求结算信息确认响应

    CmdID_Trade_RspUserPasswordUpdate,                      //用户口令更新请求响应
    CmdID_Trade_RspTradingAccountPasswordUpdate,            //资金账户口令更新请求响应
    CmdID_Trade_RspOrderInsert,                             //报单录入请求响应
    CmdID_Trade_RspOrderAction,                             //报单操作请求响应
    CmdID_Trade_RspParkedOrderInsert,                       //预埋单录入请求响应
    CmdID_Trade_RspParkedOrderAction,                       //预埋撤单录入请求响应
    CmdID_Trade_RspRemoveParkedOrder,                       //删除预埋单响应    
    CmdID_Trade_RspRemoveParkedOrderAction,                 //删除预埋撤单响应
    CmdID_Trade_RspQryParkedOrder,                          //请求查询预埋单响应
    CmdID_Trade_RspQryParkedOrderAction,                    //请求查询预埋撤单响应
    CmdID_Trade_RspQryOrder,                                //请求查询报单响应
    CmdID_Trade_RspQryTrade,                                //请求查询成交响应
    CmdID_Trade_RspQryTradingAccount,                       //请求查询资金账户响应
    CmdID_Trade_RspQryInvestor,                             //请求查询投资者响应
    CmdID_Trade_RspQryInstrumentMarginRate,                 //请求查询合约保证金率响应   
    CmdID_Trade_RspQryInstrumentCommissionRate,             //请求查询合约手续费率响应
    CmdID_Trade_RspQryInstrument,                           //请求查询合约响应
    CmdID_Trade_RspQrySettlementInfo,                       //请求查询投资者结算结果响应
    CmdID_Trade_RspQryInvestorPositionDetail,               //请求查询投资者持仓明细响应
    CmdID_Trade_RspQryInvestorPosition,                     //请求查询投资者持仓响应
    CmdID_Trade_RspQryInvestorPositionCombDetail,           //请求查询投资者持仓明细响应
    CmdID_Trade_RspQryNotice,                               //请求查询客户通知响应
    CmdID_Trade_RtnOrder,                                   //报单回报(报单录入与报单操作都会产生该回报)
    CmdID_Trade_RtnTrade,                                   //成交回报
    CmdID_Trade_ErrRtnOrderInsert,                          //报单录入回执((错误回执，交易所认为录入无效)
    CmdID_Trade_ErrRtnOrderAction,                          //报单操作回执(错误回执，交易所认为撤单无效)
    CmdID_Trade_RtnInstrumentStatus,                        //合约交易状态通知
    CmdID_Trade_RtnTradingNotice                            //交易通知
};



//CTP连接参数
struct Stru_CTPConnectionParam
{
public:
    Stru_CTPConnectionParam()
    {
    };
    Stru_CTPConnectionParam(const vector<string>& frontAddrs,const string& brokerID,const string& userID,const string& userPW)
    {
        FrontAddrs=frontAddrs;
        BrokerID=brokerID;
        UserID=userID;
        UserPW=userPW;
		InvestorID=userID;
    };
    Stru_CTPConnectionParam(const Stru_CTPConnectionParam& r)
    {
        *this=r;
    };
	Stru_CTPConnectionParam& operator= (const Stru_CTPConnectionParam& r)
    {
        ProxyConnectStr=r.ProxyConnectStr;
        FrontAddrs=r.FrontAddrs;
        BrokerID=r.BrokerID;
        UserID=r.UserID;
        UserPW=r.UserPW;
        InvestorID=r.InvestorID;
        OneTimePassword=r.OneTimePassword;
        return *this;
    };
    
    string          ProxyConnectStr;                        //代理服务器地址
    vector<string>  FrontAddrs;                             //行情前置机地址组，类似多个"58.246.49.130:41213"
    string          BrokerID;
    string          UserID;
    string          UserPW;
    string          InvestorID;
	string          OneTimePassword;                        //客户端增强认证时的单次密码
};


//CTP基本接口
class CPlatformCTP
{
public:
	enum eMsgID
	{
		QuotConnMsg=1500,		//Quot连接状态改变, lParam为EventID, EventParam的string为: Quot: ConnStr, UserInt为eCTPCONNSTATUS
		TradeConnMsg,			//Trade连接状态改变, lParam为EventID, EventParam的string为: Trade: ConnStr, UserInt为eCTPCONNSTATUS
		DataArrivedMsg			//数据(如行情、成交等)到达, lParam为EventParamID, EvtID为包命令字(ePkgCmd), EventParam的pEventData和pEventData2为包数据, userInt1和userInt2为RequestID和bLast
	};

public:
    //ConnParam: 连接参数
    //bPushCache: 是否需要将ctp返回的数据放到m_Cache中，等待其它线程处理(类似于FastTrader处理方式)
    //bUserComb: 是否使用组合单功能
    //bCalcuCombQuotOnLegQuot: 针对组合单，是否根据单腿合约行情自动生成组合合约行情
    CPlatformCTP(const Stru_CTPConnectionParam& ConnParam=Stru_CTPConnectionParam(),bool bPushCache=true,bool bUseComb=false,bool bCalcuCombQuotOnLegQuot=false);

	//获取/设置连接状态
	static char* GetConnStatusName(eCTPCONNSTATUS sta);
    eCTPCONNSTATUS GetConnStatus(void) const { return m_ConnStatus; }
	const char* GetConnStatus2(void) const;
	time_t GetConnStatusTime(void) const { return m_ConnStatusTime; }
    void SetConnStatus(eCTPCONNSTATUS connsta,bool bQuot);

	//获取/设置连接参数
    Stru_CTPConnectionParam GetConnParam(void) const { CeasymutexGuard guard(m_mutex);  return m_ConnParam; }
    void SetConnParam(Stru_CTPConnectionParam& ConnParam) { CeasymutexGuard guard(m_mutex);  m_ConnParam=ConnParam; }

    //从Cache中获取数据包
    bool IsEmptyCache(void) { CeasymutexGuard guard(m_mutex);  return m_Cache.IsEmpty(); }
	CVariablePackage::HeadStru PeekFromCache(void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0) { CeasymutexGuard guard(m_mutex);  return m_Cache.PeekFrom(pData1,DataSize1,pData2,DataSize2); }
    void DeleteCurPackageFromCache() { CeasymutexGuard guard(m_mutex);  m_Cache.DeleteCurPackage(); }
    bool IsPushCache(void) { return m_bPushCache; }

	//取指定交易所的交易时间
	static CEasyTradingTimespan GetTradingTimespan(const string& strExchangeID)
	{
		CEasyTimespan<int> ts_IntraDay;
		if(strExchangeID==string("CFFEX"))
		{	
			//中金所合约 9:15-11:30 13:00-15:15, total 4.5 hours (2h15'+2h15')
			ts_IntraDay.insert(9*3600+15*60,11*3600+30*60);
			ts_IntraDay.insert(13*3600,15*3600+15*60);
		}
		else 
		{
			//商品期货 9:00-10:15 10:30-11:30 13:00-15:00, total 4h15'(1h15'+1h+2h)
			ts_IntraDay.insert(9*3600,10*3600+15*60);
			ts_IntraDay.insert(10*3600+30*60,11*3600+30*60);
			ts_IntraDay.insert(13*3600+30*60,15*3600);
		}
		CEasyTradingTimespan tts(true,ts_IntraDay);
		return tts;
	}


protected:
    //生成连接字符串
    //正常情况：tcp://...
    //使用代理：socks5:// asp-sim2-front1.financial-trading-platform.com:26205/user:pass@127.0.0.1:10001
    string CPlatformCTP::MakeConnectStr(const string& addr,const string& ProxyStr);

public:
    Ceasymutex              m_mutex;

protected:
    Stru_CTPConnectionParam m_ConnParam;

    CVariablePackage        m_Cache;                        //交易所的返回的数据保存到此cache中，
    bool                    m_bPushCache;                   //ctp返回的数据是否需要push到cache中

	bool                    m_bUseComb;						//是否处理组合合约的行情
	bool                    m_bCalcuCombQuotOnLegQuot;		//是否根据分腿行情计算组合合约的行情

private:
    eCTPCONNSTATUS          m_ConnStatus;					//连接状态
	time_t					m_ConnStatusTime;				//连接状态的时间

public:
    int                     m_RequestID;                    //一个递增的ID

protected:
	multimap<ePkgCmd,int>	m_subscribePkg;					//订阅指定消息的窗口句柄
public:
	void SubscribePkg(ePkgCmd pkgID,int hWnd);
	void UnsubscribePkg(ePkgCmd pkgID,int hWnd);
	void UnsubscribePkg(int hWnd);
	void UnsubscribePkgAll();

protected:
	//这个只能在派生类里使用，由使用者负责加锁
	void PostPkg_Internal(ePkgCmd pkgID,void* pdata,int datalen,void* pdata2=NULL,int datalen2=0,int userInt1=0,int userInt2=0);



private:
    CPlatformCTP(const CPlatformCTP&);
	CPlatformCTP& operator= (const CPlatformCTP&);
};


#pragma managed(pop)

#endif

