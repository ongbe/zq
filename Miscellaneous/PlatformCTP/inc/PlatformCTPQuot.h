/***************************************************************************//**
*   @file          PlatformCTPQuot.h
*   @brief        综合交易平台(CTP)交易接口
*
********************************************************************************/
#ifndef _PLATFORMCTPQUOT_H_
#define _PLATFORMCTPQUOT_H_

#include <string>
#include <set>
#include <map>
#include <vector>
using std::vector;
using std::string;
using std::map;
using std::multimap;
using std::set;




#include "PlatformCTP.h"


#pragma managed(push,off)



//行情接口
class CPlatformCTPQuot : public CThostFtdcMdSpi, public CPlatformCTP
{
public:
    //******************************************************************************************
    //ConnParam:                连接参数
    //bPushCache:               是否需要将ctp返回的数据放到m_Cache中，等待其它线程处理(类似于FastTrader处理方式)
    //bUserComb:                是否使用组合单功能
    //bCalcuCombQuotOnLegQuot:  针对组合单，是否根据单腿合约行情自动生成组合合约行情
    //******************************************************************************************
    CPlatformCTPQuot(const Stru_CTPConnectionParam& ConnParam=Stru_CTPConnectionParam(),
        bool bPushCache=true,
        bool bUseComb=false,
        bool bCalcuCombQuotOnLegQuot=false);

    virtual ~CPlatformCTPQuot();

    //******************************************************************************************
    //  pSpi:                   spi回调处理对象的指针。一般为NULL，使用本对象进行默认处理即可；
    //                          如果实例化的是CPlatformCTPTrade的子类，pMdSpi应该等于子类对象的指针，
    //                          使子类对象可以进行回调处理
    //******************************************************************************************
    virtual bool Login(const CThostFtdcMdSpi* pSpi=NULL);
    virtual void Logout(void);

    ///订阅行情
    int SubscribeMarketData(const string& InstrumentID);
    ///退订行情
    int UnSubscribeMarketData(const string& InstrumentID);
    ///退订全部行情
    int UnSubscribeAllMarketData(void);
    ///获取行情订阅列表
    vector<string> GetSubscribedInstrumentIDs(void) const;

	void SetInstrumentInfo(const map<string,CThostFtdcInstrumentField>& InstrumentID2Info);


///****************************************************************************
/// 下面是ctp回调函数
///****************************************************************************

protected:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);
	

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);


private:
    //设置指定合约行情，设置的行情更新，则返回true；否则返回false（比原来的行情更老）
    //仅用于OnRtnDepthMarketData()中
    bool SetQuotInfo_Internal(const CThostFtdcDepthMarketDataField& inData);
    //获取指定合约行情。仅用于OnRtnDepthMarketData()中
    bool GetQuotInfo_Internal(const std::string& InstrumentID, CThostFtdcDepthMarketDataField& outData);
	///获取包含此单腿合约的组合合约列表
	bool GetCombInstrumentIDs_IncludeLeg_Subscribed_Internal(const string& LegInstrument,vector<string>& vecCombInstruments);


    //判断一个合约是否是组合单合约。如果是，返回单腿合约名称。
    //目前仅能判断大商和郑商的组合合约
    //判断依据是前有空格后有&，如SP c1305&c1309
    static bool IsCombInstrument(const string& strInstrument,string& LegInstrument1,string& LegInstrument2);

    //判断一个合约是否是组合单合约。
    //目前仅能判断大商和郑商的组合合约
    //判断依据是前有空格后有&，如SP c1305&c1309
    static bool IsCombInstrument2(const string& strInstrument);


///****************************************************************************
/// 下面是成员变量
///****************************************************************************

private:

    map<string,int>         m_SubscribedInstrumentID;       //当前订阅行情的合约 -> 订阅次数
    multimap<string,string>	m_LegInstrument2CombSubscribed; //单腿合约到组合合约的对应表
	CThostFtdcMdApi*        m_pQuotApi;						//行情接口对象，由综合交易平台的接口提供
    map<string,CThostFtdcDepthMarketDataField> m_lastQuot;	//这里保存每个合约的最新行情

	//合约信息，由外部从交易接口取到后设置过来
	map<string,CThostFtdcInstrumentField> m_InstrumentID2Info;


private:
    CPlatformCTPQuot(const CPlatformCTPQuot&);
	CPlatformCTPQuot& operator= (const CPlatformCTPQuot&);
};


#pragma managed(pop)


#endif

