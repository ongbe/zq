// stdafx.cpp : source file that includes just the standard includes
// testtcpsvr.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
void CopyOrderField(PlatformStru_OrderInfo& data,const CThostFtdcOrderField& orderField)
{
	///经纪公司代码
	strcpy_s(data.BrokerID, sizeof(orderField.BrokerID), orderField.BrokerID);
	///投资者代码
	strcpy_s(data.InvestorID, sizeof(orderField.InvestorID), orderField.InvestorID);
	///合约代码
	strcpy_s(data.InstrumentID, sizeof(orderField.InstrumentID), orderField.InstrumentID);
	///报单引用
	strcpy_s(data.OrderRef, sizeof(orderField.OrderRef), orderField.OrderRef);
	///用户代码
	strcpy_s(data.UserID, sizeof(orderField.UserID), orderField.UserID);
	///报单价格条件
	data.OrderPriceType = orderField.OrderPriceType;
	///买卖方向
	data.Direction = orderField.Direction;
	///组合开平标志
	data.CombOffsetFlag[0] = orderField.CombOffsetFlag[0];
	///组合投机套保标志
	strcpy_s(data.CombHedgeFlag, sizeof(orderField.CombHedgeFlag), orderField.CombHedgeFlag);
	///价格
	data.LimitPrice = orderField.LimitPrice;
	///数量
	data.VolumeTotalOriginal = orderField.VolumeTotalOriginal;
	///有效期类型
	data.TimeCondition = orderField.TimeCondition;
	///GTD日期
	strcpy_s(data.GTDDate, sizeof(orderField.GTDDate), orderField.GTDDate);
	///成交量类型
	data.VolumeCondition = orderField.VolumeCondition;
	///最小成交量
	data.MinVolume = orderField.MinVolume;
	///触发条件
	data.ContingentCondition = orderField.ContingentCondition;
	///止损价
	data.StopPrice = orderField.StopPrice;
	///强平原因
	data.ForceCloseReason = orderField.ForceCloseReason;
	///自动挂起标志
	data.IsAutoSuspend = orderField.IsAutoSuspend;
	///业务单元
	strcpy_s(data.BusinessUnit, sizeof(orderField.BusinessUnit), orderField.BusinessUnit);
	///请求编号
	data.RequestID = orderField.RequestID;
	///本地报单编号
	strcpy_s(data.OrderLocalID, sizeof(orderField.OrderLocalID), orderField.OrderLocalID);
	///交易所代码
	strcpy_s(data.ExchangeID, sizeof(orderField.ExchangeID), orderField.ExchangeID);
	///会员代码
	strcpy_s(data.ParticipantID, sizeof(orderField.ParticipantID), orderField.ParticipantID);
	///客户代码
	strcpy_s(data.ClientID, sizeof(orderField.ClientID), orderField.ClientID);
	///合约在交易所的代码
	strcpy_s(data.ExchangeInstID, sizeof(orderField.ExchangeInstID), orderField.ExchangeInstID);
	///交易所交易员代码
	strcpy_s(data.TraderID, sizeof(orderField.TraderID), orderField.TraderID);
	///安装编号
	data.InstallID = orderField.InstallID;
	///报单提交状态
	data.OrderSubmitStatus = orderField.OrderSubmitStatus;
	///报单提示序号
	data.NotifySequence = orderField.NotifySequence;
	///交易日
	strcpy_s(data.TradingDay, sizeof(orderField.TradingDay), orderField.TradingDay);
	///结算编号
	data.SettlementID = orderField.SettlementID;
	///报单编号
	strcpy_s(data.OrderSysID, sizeof(orderField.OrderSysID), orderField.OrderSysID);
	///报单来源
	data.OrderSource = orderField.OrderSource;
	///报单状态
	data.OrderStatus = orderField.OrderStatus;
	data.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	///报单类型
	data.OrderType = orderField.OrderType;
	///今成交数量
	data.VolumeTraded = orderField.VolumeTraded;
	///剩余数量
	data.VolumeTotal = orderField.VolumeTotal;
	///报单日期
	strcpy_s(data.InsertDate, sizeof(orderField.InsertDate), orderField.InsertDate);
	///委托时间
	strcpy_s(data.InsertTime, sizeof(orderField.InsertTime), orderField.InsertTime);
	///激活时间
	strcpy_s(data.ActiveTime, sizeof(orderField.ActiveTime), orderField.ActiveTime);
	///挂起时间
	strcpy_s(data.SuspendTime, sizeof(orderField.SuspendTime), orderField.SuspendTime);
	///最后修改时间
	strcpy_s(data.UpdateTime, sizeof(orderField.UpdateTime), orderField.UpdateTime);
	///撤销时间
	strcpy_s(data.CancelTime, sizeof(orderField.CancelTime), orderField.CancelTime);
	///最后修改交易所交易员代码
	strcpy_s(data.ActiveTraderID, sizeof(orderField.ActiveTraderID), orderField.ActiveTraderID);
	///结算会员编号
	strcpy_s(data.ClearingPartID, sizeof(orderField.ClearingPartID), orderField.ClearingPartID);
	///序号
	data.SequenceNo = orderField.SequenceNo;
	///前置编号
	data.FrontID = orderField.FrontID;
	///会话编号
	data.SessionID = orderField.SessionID;
	///用户端产品信息
	strcpy_s(data.UserProductInfo, sizeof(orderField.UserProductInfo), orderField.UserProductInfo);
	///状态信息
	strcpy_s(data.StatusMsg, sizeof(orderField.StatusMsg), orderField.StatusMsg);
	///用户强评标志
	data.UserForceClose = orderField.UserForceClose;
	///操作用户代码
	strcpy_s(data.ActiveUserID, sizeof(orderField.ActiveUserID), orderField.ActiveUserID);
	///经纪公司报单编号
	data.BrokerOrderSeq = orderField.BrokerOrderSeq;
	///相关报单
	strcpy_s(data.RelativeOrderSysID, sizeof(orderField.RelativeOrderSysID), orderField.RelativeOrderSysID);


	strncpy(data.Account,orderField.InvestorID,sizeof(data.Account)-1);

}
void CopyCTPOrderField(CThostFtdcOrderField& data, PlatformStru_OrderInfo& orderField)
{
	///经纪公司代码
	strcpy_s(data.BrokerID, sizeof(orderField.BrokerID), orderField.BrokerID);
	///投资者代码
	strcpy_s(data.InvestorID, sizeof(orderField.InvestorID), orderField.InvestorID);
	///合约代码
	strcpy_s(data.InstrumentID, sizeof(orderField.InstrumentID), orderField.InstrumentID);
	///报单引用
	strcpy_s(data.OrderRef, sizeof(orderField.OrderRef), orderField.OrderRef);
	///用户代码
	strcpy_s(data.UserID, sizeof(orderField.UserID), orderField.UserID);
	///报单价格条件
	data.OrderPriceType = orderField.OrderPriceType;
	///买卖方向
	data.Direction = orderField.Direction;
	///组合开平标志
	data.CombOffsetFlag[0] = orderField.CombOffsetFlag[0];
	///组合投机套保标志
	strcpy_s(data.CombHedgeFlag, sizeof(orderField.CombHedgeFlag), orderField.CombHedgeFlag);
	///价格
	data.LimitPrice = orderField.LimitPrice;
	///数量
	data.VolumeTotalOriginal = orderField.VolumeTotalOriginal;
	///有效期类型
	data.TimeCondition = orderField.TimeCondition;
	///GTD日期
	strcpy_s(data.GTDDate, sizeof(orderField.GTDDate), orderField.GTDDate);
	///成交量类型
	data.VolumeCondition = orderField.VolumeCondition;
	///最小成交量
	data.MinVolume = orderField.MinVolume;
	///触发条件
	data.ContingentCondition = orderField.ContingentCondition;
	///止损价
	data.StopPrice = orderField.StopPrice;
	///强平原因
	data.ForceCloseReason = orderField.ForceCloseReason;
	///自动挂起标志
	data.IsAutoSuspend = orderField.IsAutoSuspend;
	///业务单元
	strcpy_s(data.BusinessUnit, sizeof(orderField.BusinessUnit), orderField.BusinessUnit);
	///请求编号
	data.RequestID = orderField.RequestID;
	///本地报单编号
	strcpy_s(data.OrderLocalID, sizeof(orderField.OrderLocalID), orderField.OrderLocalID);
	///交易所代码
	strcpy_s(data.ExchangeID, sizeof(orderField.ExchangeID), orderField.ExchangeID);
	///会员代码
	strcpy_s(data.ParticipantID, sizeof(orderField.ParticipantID), orderField.ParticipantID);
	///客户代码
	strcpy_s(data.ClientID, sizeof(orderField.ClientID), orderField.ClientID);
	///合约在交易所的代码
	strcpy_s(data.ExchangeInstID, sizeof(orderField.ExchangeInstID), orderField.ExchangeInstID);
	///交易所交易员代码
	strcpy_s(data.TraderID, sizeof(orderField.TraderID), orderField.TraderID);
	///安装编号
	data.InstallID = orderField.InstallID;
	///报单提交状态
	data.OrderSubmitStatus = orderField.OrderSubmitStatus;
	///报单提示序号
	data.NotifySequence = orderField.NotifySequence;
	///交易日
	strcpy_s(data.TradingDay, sizeof(orderField.TradingDay), orderField.TradingDay);
	///结算编号
	data.SettlementID = orderField.SettlementID;
	///报单编号
	strcpy_s(data.OrderSysID, sizeof(orderField.OrderSysID), orderField.OrderSysID);
	///报单来源
	data.OrderSource = orderField.OrderSource;
	///报单状态
	data.OrderStatus = orderField.OrderStatus;
//	data.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	///报单类型
	data.OrderType = orderField.OrderType;
	///今成交数量
	data.VolumeTraded = orderField.VolumeTraded;
	///剩余数量
	data.VolumeTotal = orderField.VolumeTotal;
	///报单日期
	strcpy_s(data.InsertDate, sizeof(orderField.InsertDate), orderField.InsertDate);
	///委托时间
	strcpy_s(data.InsertTime, sizeof(orderField.InsertTime), orderField.InsertTime);
	///激活时间
	strcpy_s(data.ActiveTime, sizeof(orderField.ActiveTime), orderField.ActiveTime);
	///挂起时间
	strcpy_s(data.SuspendTime, sizeof(orderField.SuspendTime), orderField.SuspendTime);
	///最后修改时间
	strcpy_s(data.UpdateTime, sizeof(orderField.UpdateTime), orderField.UpdateTime);
	///撤销时间
	strcpy_s(data.CancelTime, sizeof(orderField.CancelTime), orderField.CancelTime);
	///最后修改交易所交易员代码
	strcpy_s(data.ActiveTraderID, sizeof(orderField.ActiveTraderID), orderField.ActiveTraderID);
	///结算会员编号
	strcpy_s(data.ClearingPartID, sizeof(orderField.ClearingPartID), orderField.ClearingPartID);
	///序号
	data.SequenceNo = orderField.SequenceNo;
	///前置编号
	data.FrontID = orderField.FrontID;
	///会话编号
	data.SessionID = orderField.SessionID;
	///用户端产品信息
	strcpy_s(data.UserProductInfo, sizeof(orderField.UserProductInfo), orderField.UserProductInfo);
	///状态信息
	strcpy_s(data.StatusMsg, sizeof(orderField.StatusMsg), orderField.StatusMsg);
	///用户强评标志
	data.UserForceClose = orderField.UserForceClose;
	///操作用户代码
	strcpy_s(data.ActiveUserID, sizeof(orderField.ActiveUserID), orderField.ActiveUserID);
	///经纪公司报单编号
	data.BrokerOrderSeq = orderField.BrokerOrderSeq;
	///相关报单
	strcpy_s(data.RelativeOrderSysID, sizeof(orderField.RelativeOrderSysID), orderField.RelativeOrderSysID);

//
//	strncpy(data.szAccount,orderField.InvestorID,sizeof(data.szAccount)-1);

}

void CopyTradeRecordField( PlatformStru_TradeInfo& data, const CThostFtdcTradeField& tradeField )
{
	strcpy(data.BrokerID, tradeField.BrokerID);
	strcpy(data.InvestorID, tradeField.InvestorID);
	strcpy(data.InstrumentID, tradeField.InstrumentID);
	strcpy(data.OrderRef, tradeField.OrderRef);
	strcpy(data.UserID, tradeField.UserID);
	strcpy(data.ExchangeID, tradeField.ExchangeID);
	strcpy(data.TradeID, tradeField.TradeID);
	data.Direction = tradeField.Direction;
	strcpy(data.OrderSysID, tradeField.OrderSysID);
	strcpy(data.ParticipantID, tradeField.ParticipantID);
	strcpy(data.ClientID, tradeField.ClientID);
	data.TradingRole = tradeField.TradingRole;
	strcpy(data.ExchangeInstID, tradeField.ExchangeInstID);
	data.OffsetFlag = tradeField.OffsetFlag;
	data.HedgeFlag = tradeField.HedgeFlag;
	data.Price = tradeField.Price;
	data.Volume = tradeField.Volume;
	strcpy(data.TradeDate, tradeField.TradeDate);
	strcpy(data.TradeTime, tradeField.TradeTime);
	data.TradeType = tradeField.TradeType;
	data.PriceSource = tradeField.PriceSource;
	strcpy(data.TraderID, tradeField.TraderID);
	strcpy(data.OrderLocalID, tradeField.OrderLocalID);
	strcpy(data.ClearingPartID, tradeField.ClearingPartID);
	strcpy(data.BusinessUnit, data.BusinessUnit);
	data.SequenceNo = tradeField.SequenceNo;
	strcpy(data.TradingDay, tradeField.TradingDay);
	data.SettlementID = tradeField.SettlementID;
	data.BrokerOrderSeq = tradeField.BrokerOrderSeq;
	data.TradeSource = tradeField.TradeSource;

	strncpy(data.Account,tradeField.InvestorID,sizeof(data.Account)-1);
}
void CopyCTPTradeRecordField( CThostFtdcTradeField& data,  PlatformStru_TradeInfo& tradeField )
{
	strcpy(data.BrokerID, tradeField.BrokerID);
	strcpy(data.InvestorID, tradeField.InvestorID);
	strcpy(data.InstrumentID, tradeField.InstrumentID);
	strcpy(data.OrderRef, tradeField.OrderRef);
	strcpy(data.UserID, tradeField.UserID);
	strcpy(data.ExchangeID, tradeField.ExchangeID);
	strcpy(data.TradeID, tradeField.TradeID);
	data.Direction = tradeField.Direction;
	strcpy(data.OrderSysID, tradeField.OrderSysID);
	strcpy(data.ParticipantID, tradeField.ParticipantID);
	strcpy(data.ClientID, tradeField.ClientID);
	data.TradingRole = tradeField.TradingRole;
	strcpy(data.ExchangeInstID, tradeField.ExchangeInstID);
	data.OffsetFlag = tradeField.OffsetFlag;
	data.HedgeFlag = tradeField.HedgeFlag;
	data.Price = tradeField.Price;
	data.Volume = tradeField.Volume;
	strcpy(data.TradeDate, tradeField.TradeDate);
	strcpy(data.TradeTime, tradeField.TradeTime);
	data.TradeType = tradeField.TradeType;
	data.PriceSource = tradeField.PriceSource;
	strcpy(data.TraderID, tradeField.TraderID);
	strcpy(data.OrderLocalID, tradeField.OrderLocalID);
	strcpy(data.ClearingPartID, tradeField.ClearingPartID);
	strcpy(data.BusinessUnit, data.BusinessUnit);
	data.SequenceNo = tradeField.SequenceNo;
	strcpy(data.TradingDay, tradeField.TradingDay);
	data.SettlementID = tradeField.SettlementID;
	data.BrokerOrderSeq = tradeField.BrokerOrderSeq;
	data.TradeSource = tradeField.TradeSource;

	//strncpy(data.szAccount,tradeField.InvestorID,sizeof(data.szAccount)-1);
}