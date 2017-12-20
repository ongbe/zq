// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#if 0
#define OFFER_INFO(fmt, ...)  ;
#else
#define OFFER_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","Offer", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcTraderApi.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiDataType.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"


#include "..\SvrTradeData\Interface_SvrTradeData.h"
#include "..\SvrBrokerInfo\Interface_SvrBrokerInfo.h"
#include "..\SvrUserOrg\Interface_SvrUserOrg.h"
#include "..\SvrLogin\Interface_SvrLogin.h"
#include "..\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"
#include "..\SvrDBOpr\Interface_SvrDBOpr.h"
#include "..\SvrTcp\Interface_SvrTcp.h"
#ifdef  SIMU//Ä£Äâ½»Ò×
	#include "..\SvrSimulateTrade\Interface_SvrSimulateTrade.h"
	#include "textfile.h"
#endif

#include "TcpClientWorker.h"

#include "tinyxml.h"

#include <vector>
#include <map>
#include <set>
#include <queue>
#include "iostream"
#include "fstream"
#include "ostream"
#include "string"
#include "CommonStruct.h"
#include "ReadWriteLock.h"
#include "OfferMainInterface.h"
#include "OfferMain.h"
#include "OfferMain_Simu.h"
#include "UserTraderSpi.h"
#include "CTPAccount.h"
#include "MsgQueue.h"
#include "OfferMsgQueue.h"
#include "CommonMacro.h"
#include "TCPDataService.h"
#include "CommonDef.h"
#include "TCPDataService.h"
// TODO: reference additional headers your program requires here
void CopyOrderField(PlatformStru_OrderInfo& data,const CThostFtdcOrderField& orderField);	
void CopyTradeRecordField( PlatformStru_TradeInfo& data, const CThostFtdcTradeField& tradeField );

void CopyCTPOrderField(CThostFtdcOrderField& data, PlatformStru_OrderInfo& orderField);
void CopyCTPTradeRecordField( CThostFtdcTradeField& data,  PlatformStru_TradeInfo& tradeField );
#include "FileOpr.h "
#define INSTRUMENT_FILENAME "Instruments.txt"
#define COMMISSION_MARGIN_FILENAME "cam.xml"
