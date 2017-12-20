/***************************************************************************//**
*   @file         FieldValueTool.h
*   @brief        将各个业务消息的字段转为用于GUI显示的字符串
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/10/11
********************************************************************************/
#ifndef _FIELDVALUETOOL_H_
#define _FIELDVALUETOOL_H_

#include "../inc/ISvr/PlatformSvrMgr.h"

class CFieldValueTool
{
public:
	///买卖方向
	static const wxString Direction2String(char param,int language=LANGUAGE_CHINESE);	
	static const char String2Direction(wxString Dir,int* language=NULL);	
	///持仓多空方向
	static const wxString PosiDirection2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2PosiDirection(wxString Type,int* language=NULL);
	///投机套保标志
	static const wxString HedgeFlag2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2HedgeFlag(wxString Dir,int* language=NULL);
	///开平标志
	static const wxString OffsetFlag2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OffsetFlag(wxString flag,int* language=NULL);
	///报单状态
	static const wxString OrderStatus2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OrderStatus(wxString Type,int* language=NULL);
	///报单提交状态
	static const wxString OrderSubmitStatus2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OrderSubmitStatus(wxString Type,int* language=NULL);
	///报单价格条件
	static const wxString OrderPriceType2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OrderPriceType(wxString Type,int* language=NULL);
	///用户强平标志
	static const wxString UserForceClose2String(int UserForceClose,int language=LANGUAGE_CHINESE);
	static const int String2UserForceClose(wxString Type,int* language=NULL);
	///强平原因
	static const wxString ForceCloseReason2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2ForceCloseReason(wxString Type,int* language=NULL);
	///报单类型
	static const wxString OrderType2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2OrderType(wxString Type,int* language=NULL);
	///有效期类型
	static const wxString TimeCondition2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2TimeCondition(wxString Type,int* language=NULL);
	///成交量类型
	static const wxString VolumeCondition2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2VolumeCondition(wxString Type,int* language=NULL);
	///业务请求函数返回结果
	static const wxString ReturnCode2String(int param,int language=LANGUAGE_CHINESE);
	static const int String2ReturnCode(wxString Type,int* language=NULL);
    ///品种类型
    static const wxString ProductType2String(char param,int language=LANGUAGE_CHINESE);
	static const char String2ProductType(wxString Type,int* language=NULL);
	///成交类型
	static const wxString TradeType2String(char TradeType,int language=LANGUAGE_CHINESE);
	static const char String2TradeType(wxString Type,int* language=NULL);
	//
	static const wxString TodayPosition2String(int param,int language=LANGUAGE_CHINESE);
	static const int String2TodayPosition(wxString Type,int* language=NULL);
	//
	static const wxString OrderStatusMsg2String(int param,int language=LANGUAGE_CHINESE);
	static const int String2OrderStatusMsg(wxString Type,int* language=NULL);

	static const wxString ConvertStatusMsg(wxString strStatusMsg);

	static const wxString ExchangeID2Name(char* ExchangeID,int language=LANGUAGE_CHINESE);

private:
	CFieldValueTool();
};

#endif
