/***************************************************************************//**
*   @file         QuotSpi.hpp
*   @brief        综合交易平台(CTP)行情相关的回调对象
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/
#ifndef _QUOTSPI_HPP_
#define _QUOTSPI_HPP_

#include "ctp/patsApiSpi.h"
#include "ISvr/PlatformStruct.h"
#include "Patsystem/PatsystemApi.h"

class CPackageCache;
class zqWriteLog;
namespace Pats
{
	class CPlatFormParam;
	class CPlatFormService;
	class CQuotSpi
		:public CPatsQuotSpi
	{
	public:
		CQuotSpi(CPlatFormService& RefCPatsPlatFormService);
		~CQuotSpi();
	private:
		CQuotSpi(const CQuotSpi&);
		CQuotSpi& operator= (const CQuotSpi&);

		CPatsystemApi&       m_RefMdApi;
		CPackageCache&       m_RefPackageCache;
		CPlatFormParam&      m_PlatformParam;
		CPlatFormService&    m_RefCPatsPlatFormService;
		zqWriteLog*           m_pWriteLog;

	public:
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

		///登出请求响应
		virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///错误应答
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///订阅行情应答
		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///取消订阅行情应答
		virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///深度行情通知
		virtual void OnRtnDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData);

	};
}

#endif

