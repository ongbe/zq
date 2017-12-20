
#ifndef _FIELDDETAIL_H_
#define _FIELDDETAIL_H_

namespace CltPresentation {
class CFieldDetail
{
public:
	///买卖方向
	static const char* Direction2String(char param);	
	static const char String2Direction(char* Dir);	
	///持仓多空方向
	static const char* PosiDirection2String(char param);
	static const char String2PosiDirection(char* Type);
	///投机套保标志
	static const char* HedgeFlag2String(char param);
	static const char String2HedgeFlag(char* Dir);
	///开平标志
	static const char* OffsetFlag2String(char param);
	static const char String2OffsetFlag(char* flag);
	///报单状态
	static const char* OrderStatus2String(char param);
	static const char String2OrderStatus(char* Type);
	///报单提交状态
	static const char* OrderSubmitStatus2String(char param);
	static const char String2OrderSubmitStatus(char* Type);
	///报单价格条件
	static const char* OrderPriceType2String(char param);
	static const char String2OrderPriceType(char* Type);
	///用户强平标志
	static const char* UserForceClose2String(int UserForceClose);
	static const int String2UserForceClose(char* Type);
	///强平原因
	static const char* ForceCloseReason2String(char param);
	static const char String2ForceCloseReason(char* Type);
	///报单类型
	static const char* OrderType2String(char param);
	static const char String2OrderType(char* Type);
	///有效期类型
	static const char* TimeCondition2String(char param);
	static const char String2TimeCondition(char* Type);
	///成交量类型
	static const char* VolumeCondition2String(char param);
	static const char String2VolumeCondition(char* Type);
	///业务请求函数返回结果
	static const char* ReturnCode2String(int param);
	static const int String2ReturnCode(char* Type);
    ///品种类型
    static const char* ProductType2String(char param);
	static const char String2ProductType(char* Type);
	///成交类型
	static const char* TradeType2String(char TradeType);
	static const char String2TradeType(char* Type);
	//
	static const char* TodayPosition2String(int param);
	static const int String2TodayPosition(char* Type);
	//
	static const char* OrderStatusMsg2String(int param);
	static const int String2OrderStatusMsg(char* Type);

	static const char* ConvertStatusMsg(char* strStatusMsg);

	static const char* ExchangeID2Name(char* ExchangeID);

	//交易角色
	static const char* TraderRole2String(int param);


	//成交来源
	static const char* TradeSourceString(int param);


	//成交价来源
	static const char* PriceType2String(int param);

private:
	CFieldDetail();
	~CFieldDetail();
};
}
#endif
