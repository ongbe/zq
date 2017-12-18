
/////////////////////////////////////////////////////////////////////////
///@system 新一代交易所系统
///@company 上海期货信息技术有限公司
///@file FtdcStruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
///20070227	zhangjie		创建该文件
/////////////////////////////////////////////////////////////////////////

#if !defined(_USERAPISTRUCT_H)
#define _USERAPISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <math.h>
#include <string.h>
#include "UserApiDataType.h"

#ifndef SMALL_DOUBLE
#define SMALL_DOUBLE	0.0000001
#endif

#ifndef EQUALDOUBLE
#define EQUALDOUBLE(x,y)	(fabs((x)-(y)) < SMALL_DOUBLE)
#endif


///信息分发
struct DisseminationField
{
	///序列系列号
	TSequenceSeriesType	SequenceSeries;
	///序列号
	TSequenceNoType	SequenceNo;

bool operator==(const DisseminationField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceSeries == ct.SequenceSeries);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///用户登录请求
struct ReqUserLoginField
{
	///交易日
	TDateType	TradingDay;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///密码
	TPasswordType	Password;
	///用户端产品信息
	TProductInfoType	UserProductInfo;
	///接口端产品信息
	TProductInfoType	InterfaceProductInfo;
	///协议信息
	TProtocolInfoType	ProtocolInfo;

bool operator==(const ReqUserLoginField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InterfaceProductInfo, ct.InterfaceProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProtocolInfo, ct.ProtocolInfo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///用户登录应答
struct RspUserLoginField
{
	///交易日
	TDateType	TradingDay;
	///登录成功时间
	TTimeType	LoginTime;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///交易系统名称
	TSystemNameType	SystemName;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///最大报单引用
	TOrderRefType	MaxOrderRef;
	///上期所时间
	TTimeType	SHFETime;
	///大商所时间
	TTimeType	DCETime;
	///郑商所时间
	TTimeType	CZCETime;
	///中金所时间
	TTimeType	FFEXTime;

bool operator==(const RspUserLoginField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginTime, ct.LoginTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemName, ct.SystemName) == 0);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxOrderRef, ct.MaxOrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SHFETime, ct.SHFETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DCETime, ct.DCETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CZCETime, ct.CZCETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FFEXTime, ct.FFEXTime) == 0);
	if (!ret) return ret;

	return ret;
}
};

///用户登出请求
struct UserLogoutField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;

bool operator==(const UserLogoutField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///强制交易员退出
struct ForceUserLogoutField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;

bool operator==(const ForceUserLogoutField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///银期转帐报文头
struct TransferHeaderField
{
	///版本号，常量，1.0
	TVersionType	Version;
	///交易代码，必填
	TTradeCodeType	TradeCode;
	///交易日期，必填，格式：yyyymmdd
	TTradeDateType	TradeDate;
	///交易时间，必填，格式：hhmmss
	TTradeTimeType	TradeTime;
	///发起方流水号，N/A
	TTradeSerialType	TradeSerial;
	///期货公司代码，必填
	TFutureIDType	FutureID;
	///银行代码，根据查询银行得到，必填
	TBankIDType	BankID;
	///银行分中心代码，根据查询银行得到，必填
	TBankBrchIDType	BankBrchID;
	///操作员，N/A
	TOperNoType	OperNo;
	///交易设备类型，N/A
	TDeviceIDType	DeviceID;
	///记录数，N/A
	TRecordNumType	RecordNum;
	///会话编号，N/A
	TSessionIDType	SessionID;
	///请求编号，N/A
	TRequestIDType	RequestID;

bool operator==(const TransferHeaderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(Version, ct.Version) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeCode, ct.TradeCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeSerial, ct.TradeSerial) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureID, ct.FutureID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OperNo, ct.OperNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DeviceID, ct.DeviceID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(RecordNum, ct.RecordNum) == 0);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;

	return ret;
}
};

///银行资金转期货请求，TradeCode=202001
struct TransferBankToFutureReqField
{
	///期货资金账户
	TAccountIDType	FutureAccount;
	///密码标志
	TFuturePwdFlagType	FuturePwdFlag;
	///密码
	TFutureAccPwdType	FutureAccPwd;
	///转账金额
	TMoneyType	TradeAmt;
	///客户手续费
	TMoneyType	CustFee;
	///币种：RMB-人民币 USD-美圆 HKD-港元
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferBankToFutureReqField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (FuturePwdFlag == ct.FuturePwdFlag);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccPwd, ct.FutureAccPwd) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CustFee, ct.CustFee));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///银行资金转期货请求响应
struct TransferBankToFutureRspField
{
	///响应代码
	TRetCodeType	RetCode;
	///响应信息
	TRetInfoType	RetInfo;
	///资金账户
	TAccountIDType	FutureAccount;
	///转帐金额
	TMoneyType	TradeAmt;
	///应收客户手续费
	TMoneyType	CustFee;
	///币种
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferBankToFutureRspField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(RetCode, ct.RetCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(RetInfo, ct.RetInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CustFee, ct.CustFee));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///期货资金转银行请求，TradeCode=202002
struct TransferFutureToBankReqField
{
	///期货资金账户
	TAccountIDType	FutureAccount;
	///密码标志
	TFuturePwdFlagType	FuturePwdFlag;
	///密码
	TFutureAccPwdType	FutureAccPwd;
	///转账金额
	TMoneyType	TradeAmt;
	///客户手续费
	TMoneyType	CustFee;
	///币种：RMB-人民币 USD-美圆 HKD-港元
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferFutureToBankReqField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (FuturePwdFlag == ct.FuturePwdFlag);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccPwd, ct.FutureAccPwd) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CustFee, ct.CustFee));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///期货资金转银行请求响应
struct TransferFutureToBankRspField
{
	///响应代码
	TRetCodeType	RetCode;
	///响应信息
	TRetInfoType	RetInfo;
	///资金账户
	TAccountIDType	FutureAccount;
	///转帐金额
	TMoneyType	TradeAmt;
	///应收客户手续费
	TMoneyType	CustFee;
	///币种
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferFutureToBankRspField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(RetCode, ct.RetCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(RetInfo, ct.RetInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CustFee, ct.CustFee));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询银行资金请求，TradeCode=204002
struct TransferQryBankReqField
{
	///期货资金账户
	TAccountIDType	FutureAccount;
	///密码标志
	TFuturePwdFlagType	FuturePwdFlag;
	///密码
	TFutureAccPwdType	FutureAccPwd;
	///币种：RMB-人民币 USD-美圆 HKD-港元
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferQryBankReqField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (FuturePwdFlag == ct.FuturePwdFlag);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccPwd, ct.FutureAccPwd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询银行资金请求响应
struct TransferQryBankRspField
{
	///响应代码
	TRetCodeType	RetCode;
	///响应信息
	TRetInfoType	RetInfo;
	///资金账户
	TAccountIDType	FutureAccount;
	///银行余额
	TMoneyType	TradeAmt;
	///银行可用余额
	TMoneyType	UseAmt;
	///银行可取余额
	TMoneyType	FetchAmt;
	///币种
	TCurrencyCodeType	CurrencyCode;

bool operator==(const TransferQryBankRspField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(RetCode, ct.RetCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(RetInfo, ct.RetInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TradeAmt, ct.TradeAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseAmt, ct.UseAmt));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FetchAmt, ct.FetchAmt));
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询银行交易明细请求，TradeCode=204999
struct TransferQryDetailReqField
{
	///期货资金账户
	TAccountIDType	FutureAccount;

bool operator==(const TransferQryDetailReqField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询银行交易明细请求响应
struct TransferQryDetailRspField
{
	///交易日期
	TDateType	TradeDate;
	///交易时间
	TTradeTimeType	TradeTime;
	///交易代码
	TTradeCodeType	TradeCode;
	///期货流水号
	TTradeSerialNoType	FutureSerial;
	///期货公司代码
	TFutureIDType	FutureID;
	///资金帐号
	TFutureAccountType	FutureAccount;
	///银行流水号
	TTradeSerialNoType	BankSerial;
	///银行代码
	TBankIDType	BankID;
	///银行分中心代码
	TBankBrchIDType	BankBrchID;
	///银行账号
	TBankAccountType	BankAccount;
	///证件号码
	TCertCodeType	CertCode;
	///货币代码
	TCurrencyCodeType	CurrencyCode;
	///发生金额
	TMoneyType	TxAmount;
	///有效标志
	TTransferValidFlagType	Flag;

bool operator==(const TransferQryDetailRspField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeCode, ct.TradeCode) == 0);
	if (!ret) return ret;
	ret = ret && (FutureSerial == ct.FutureSerial);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureID, ct.FutureID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FutureAccount, ct.FutureAccount) == 0);
	if (!ret) return ret;
	ret = ret && (BankSerial == ct.BankSerial);
	if (!ret) return ret;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankAccount, ct.BankAccount) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CertCode, ct.CertCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CurrencyCode, ct.CurrencyCode) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TxAmount, ct.TxAmount));
	if (!ret) return ret;
	ret = ret && (Flag == ct.Flag);
	if (!ret) return ret;

	return ret;
}
};

///响应信息
struct RspInfoField
{
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const RspInfoField &ct) const
{
	bool ret = true;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///交易所
struct ExchangeField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///交易所名称
	TExchangeNameType	ExchangeName;
	///交易所属性
	TExchangePropertyType	ExchangeProperty;

bool operator==(const ExchangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeName, ct.ExchangeName) == 0);
	if (!ret) return ret;
	ret = ret && (ExchangeProperty == ct.ExchangeProperty);
	if (!ret) return ret;

	return ret;
}
};

///产品
struct ProductField
{
	///产品代码
	TInstrumentIDType	ProductID;
	///产品名称
	TProductNameType	ProductName;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///产品类型
	TProductClassType	ProductClass;
	///合约数量乘数
	TVolumeMultipleType	VolumeMultiple;
	///最小变动价位
	TPriceType	PriceTick;
	///市价单最大下单量
	TVolumeType	MaxMarketOrderVolume;
	///市价单最小下单量
	TVolumeType	MinMarketOrderVolume;
	///限价单最大下单量
	TVolumeType	MaxLimitOrderVolume;
	///限价单最小下单量
	TVolumeType	MinLimitOrderVolume;
	///持仓类型
	TPositionTypeType	PositionType;
	///持仓日期类型
	TPositionDateTypeType	PositionDateType;
	///平仓处理类型
	TCloseDealTypeType	CloseDealType;

bool operator==(const ProductField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductName, ct.ProductName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (ProductClass == ct.ProductClass);
	if (!ret) return ret;
	ret = ret && (VolumeMultiple == ct.VolumeMultiple);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PriceTick, ct.PriceTick));
	if (!ret) return ret;
	ret = ret && (MaxMarketOrderVolume == ct.MaxMarketOrderVolume);
	if (!ret) return ret;
	ret = ret && (MinMarketOrderVolume == ct.MinMarketOrderVolume);
	if (!ret) return ret;
	ret = ret && (MaxLimitOrderVolume == ct.MaxLimitOrderVolume);
	if (!ret) return ret;
	ret = ret && (MinLimitOrderVolume == ct.MinLimitOrderVolume);
	if (!ret) return ret;
	ret = ret && (PositionType == ct.PositionType);
	if (!ret) return ret;
	ret = ret && (PositionDateType == ct.PositionDateType);
	if (!ret) return ret;
	ret = ret && (CloseDealType == ct.CloseDealType);
	if (!ret) return ret;

	return ret;
}
};

///合约
struct InstrumentField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///合约名称
	TInstrumentNameType	InstrumentName;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///产品代码
	TInstrumentIDType	ProductID;
	///产品类型
	TProductClassType	ProductClass;
	///交割年份
	TYearType	DeliveryYear;
	///交割月
	TMonthType	DeliveryMonth;
	///市价单最大下单量
	TVolumeType	MaxMarketOrderVolume;
	///市价单最小下单量
	TVolumeType	MinMarketOrderVolume;
	///限价单最大下单量
	TVolumeType	MaxLimitOrderVolume;
	///限价单最小下单量
	TVolumeType	MinLimitOrderVolume;
	///合约数量乘数
	TVolumeMultipleType	VolumeMultiple;
	///最小变动价位
	TPriceType	PriceTick;
	///创建日
	TDateType	CreateDate;
	///上市日
	TDateType	OpenDate;
	///到期日
	TDateType	ExpireDate;
	///开始交割日
	TDateType	StartDelivDate;
	///结束交割日
	TDateType	EndDelivDate;
	///合约生命周期状态
	TInstLifePhaseType	InstLifePhase;
	///当前是否交易
	TBoolType	IsTrading;
	///持仓类型
	TPositionTypeType	PositionType;
	///持仓日期类型
	TPositionDateTypeType	PositionDateType;
	///多头保证金率
	TRatioType	LongMarginRatio;
	///空头保证金率
	TRatioType	ShortMarginRatio;

bool operator==(const InstrumentField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentName, ct.InstrumentName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (ProductClass == ct.ProductClass);
	if (!ret) return ret;
	ret = ret && (DeliveryYear == ct.DeliveryYear);
	if (!ret) return ret;
	ret = ret && (DeliveryMonth == ct.DeliveryMonth);
	if (!ret) return ret;
	ret = ret && (MaxMarketOrderVolume == ct.MaxMarketOrderVolume);
	if (!ret) return ret;
	ret = ret && (MinMarketOrderVolume == ct.MinMarketOrderVolume);
	if (!ret) return ret;
	ret = ret && (MaxLimitOrderVolume == ct.MaxLimitOrderVolume);
	if (!ret) return ret;
	ret = ret && (MinLimitOrderVolume == ct.MinLimitOrderVolume);
	if (!ret) return ret;
	ret = ret && (VolumeMultiple == ct.VolumeMultiple);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PriceTick, ct.PriceTick));
	if (!ret) return ret;
	ret = ret && (strcmp(CreateDate, ct.CreateDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExpireDate, ct.ExpireDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartDelivDate, ct.StartDelivDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EndDelivDate, ct.EndDelivDate) == 0);
	if (!ret) return ret;
	ret = ret && (InstLifePhase == ct.InstLifePhase);
	if (!ret) return ret;
	ret = ret && (IsTrading == ct.IsTrading);
	if (!ret) return ret;
	ret = ret && (PositionType == ct.PositionType);
	if (!ret) return ret;
	ret = ret && (PositionDateType == ct.PositionDateType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatio, ct.LongMarginRatio));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatio, ct.ShortMarginRatio));
	if (!ret) return ret;

	return ret;
}
};

///经纪公司
struct BrokerField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///经纪公司简称
	TBrokerAbbrType	BrokerAbbr;
	///经纪公司名称
	TBrokerNameType	BrokerName;
	///是否活跃
	TBoolType	IsActive;

bool operator==(const BrokerField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerAbbr, ct.BrokerAbbr) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerName, ct.BrokerName) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///交易所交易员
struct TraderField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///密码
	TPasswordType	Password;
	///安装数量
	TInstallCountType	InstallCount;
	///经纪公司代码
	TBrokerIDType	BrokerID;

bool operator==(const TraderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (InstallCount == ct.InstallCount);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者
struct InvestorField
{
	///投资者代码
	TInvestorIDType	InvestorID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者分组代码
	TInvestorIDType	InvestorGroupID;
	///投资者名称
	TPartyNameType	InvestorName;
	///证件类型
	TIdCardTypeType	IdentifiedCardType;
	///证件号码
	TIdentifiedCardNoType	IdentifiedCardNo;
	///是否活跃
	TBoolType	IsActive;
	///联系电话
	TTelephoneType	Telephone;
	///通讯地址
	TAddressType	Address;
	///开户日期
	TDateType	OpenDate;
	///手机
	TMobileType	Mobile;
	///手续费率模板代码
	TInvestorIDType	CommModelID;

bool operator==(const InvestorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorName, ct.InvestorName) == 0);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CommModelID, ct.CommModelID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///交易编码
struct TradingCodeField
{
	///投资者代码
	TInvestorIDType	InvestorID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///客户代码
	TClientIDType	ClientID;
	///是否活跃
	TBoolType	IsActive;
	///交易编码类型
	TClientIDTypeType	ClientIDType;

bool operator==(const TradingCodeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (ClientIDType == ct.ClientIDType);
	if (!ret) return ret;

	return ret;
}
};

///会员编码和经纪公司编码对照表
struct PartBrokerField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///是否活跃
	TBoolType	IsActive;

bool operator==(const PartBrokerField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///管理用户
struct SuperUserField
{
	///用户代码
	TUserIDType	UserID;
	///用户名称
	TUserNameType	UserName;
	///密码
	TPasswordType	Password;
	///是否活跃
	TBoolType	IsActive;

bool operator==(const SuperUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserName, ct.UserName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///管理用户功能权限
struct SuperUserFunctionField
{
	///用户代码
	TUserIDType	UserID;
	///功能代码
	TFunctionCodeType	FunctionCode;

bool operator==(const SuperUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (FunctionCode == ct.FunctionCode);
	if (!ret) return ret;

	return ret;
}
};

///投资者组
struct InvestorGroupField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者分组代码
	TInvestorIDType	InvestorGroupID;
	///投资者分组名称
	TInvestorGroupNameType	InvestorGroupName;

bool operator==(const InvestorGroupField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupName, ct.InvestorGroupName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///资金账户
struct TradingAccountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者帐号
	TAccountIDType	AccountID;
	///上次质押金额
	TMoneyType	PreMortgage;
	///上次信用额度
	TMoneyType	PreCredit;
	///上次存款额
	TMoneyType	PreDeposit;
	///上次结算准备金
	TMoneyType	PreBalance;
	///上次占用的保证金
	TMoneyType	PreMargin;
	///利息基数
	TMoneyType	InterestBase;
	///利息收入
	TMoneyType	Interest;
	///入金金额
	TMoneyType	Deposit;
	///出金金额
	TMoneyType	Withdraw;
	///冻结的保证金
	TMoneyType	FrozenMargin;
	///冻结的资金
	TMoneyType	FrozenCash;
	///冻结的手续费
	TMoneyType	FrozenCommission;
	///当前保证金总额
	TMoneyType	CurrMargin;
	///资金差额
	TMoneyType	CashIn;
	///手续费
	TMoneyType	Commission;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///持仓盈亏
	TMoneyType	PositionProfit;
	///期货结算准备金
	TMoneyType	Balance;
	///可用资金
	TMoneyType	Available;
	///可取资金
	TMoneyType	WithdrawQuota;
	///基本准备金
	TMoneyType	Reserve;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///信用额度
	TMoneyType	Credit;
	///质押金额
	TMoneyType	Mortgage;
	///交易所保证金
	TMoneyType	ExchangeMargin;
	///投资者交割保证金
	TMoneyType	DeliveryMargin;
	///交易所交割保证金
	TMoneyType	ExchangeDeliveryMargin;

bool operator==(const TradingAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMortgage, ct.PreMortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreCredit, ct.PreCredit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDeposit, ct.PreDeposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(InterestBase, ct.InterestBase));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Interest, ct.Interest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Deposit, ct.Deposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WithdrawQuota, ct.WithdrawQuota));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Credit, ct.Credit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Mortgage, ct.Mortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(DeliveryMargin, ct.DeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeDeliveryMargin, ct.ExchangeDeliveryMargin));
	if (!ret) return ret;

	return ret;
}
};

///投资者持仓
struct InvestorPositionField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///持仓多空方向
	TPosiDirectionType	PosiDirection;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///持仓日期
	TPositionDateType	PositionDate;
	///上日持仓
	TVolumeType	YdPosition;
	///今日持仓
	TVolumeType	Position;
	///多头冻结
	TVolumeType	LongFrozen;
	///空头冻结
	TVolumeType	ShortFrozen;
	///开仓冻结金额
	TMoneyType	LongFrozenAmount;
	///开仓冻结金额
	TMoneyType	ShortFrozenAmount;
	///开仓量
	TVolumeType	OpenVolume;
	///平仓量
	TVolumeType	CloseVolume;
	///开仓金额
	TMoneyType	OpenAmount;
	///平仓金额
	TMoneyType	CloseAmount;
	///持仓成本
	TMoneyType	PositionCost;
	///上次占用的保证金
	TMoneyType	PreMargin;
	///占用的保证金
	TMoneyType	UseMargin;
	///冻结的保证金
	TMoneyType	FrozenMargin;
	///冻结的资金
	TMoneyType	FrozenCash;
	///冻结的手续费
	TMoneyType	FrozenCommission;
	///资金差额
	TMoneyType	CashIn;
	///手续费
	TMoneyType	Commission;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///持仓盈亏
	TMoneyType	PositionProfit;
	///上次结算价
	TPriceType	PreSettlementPrice;
	///本次结算价
	TPriceType	SettlementPrice;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///开仓成本
	TMoneyType	OpenCost;
	///交易所保证金
	TMoneyType	ExchangeMargin;
	///组合成交形成的持仓
	TVolumeType	CombPosition;
	///组合多头冻结
	TVolumeType	CombLongFrozen;
	///组合空头冻结
	TVolumeType	CombShortFrozen;
	///逐日盯市平仓盈亏
	TMoneyType	CloseProfitByDate;
	///逐笔对冲平仓盈亏
	TMoneyType	CloseProfitByTrade;
	///今日持仓
	TVolumeType	TodayPosition;
	///保证金率
	TRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TRatioType	MarginRateByVolume;

bool operator==(const InvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongFrozen == ct.LongFrozen);
	if (!ret) return ret;
	ret = ret && (ShortFrozen == ct.ShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongFrozenAmount, ct.LongFrozenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortFrozenAmount, ct.ShortFrozenAmount));
	if (!ret) return ret;
	ret = ret && (OpenVolume == ct.OpenVolume);
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenAmount, ct.OpenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseMargin, ct.UseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenCost, ct.OpenCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (CombPosition == ct.CombPosition);
	if (!ret) return ret;
	ret = ret && (CombLongFrozen == ct.CombLongFrozen);
	if (!ret) return ret;
	ret = ret && (CombShortFrozen == ct.CombShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;

	return ret;
}
};

///合约保证金率
struct InstrumentMarginRateField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;
	///是否相对交易所收取
	TBoolType	IsRelative;

bool operator==(const InstrumentMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///合约手续费率
struct InstrumentCommissionRateField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///开仓手续费率
	TRatioType	OpenRatioByMoney;
	///开仓手续费
	TRatioType	OpenRatioByVolume;
	///平仓手续费率
	TRatioType	CloseRatioByMoney;
	///平仓手续费
	TRatioType	CloseRatioByVolume;
	///平今手续费率
	TRatioType	CloseTodayRatioByMoney;
	///平今手续费
	TRatioType	CloseTodayRatioByVolume;

bool operator==(const InstrumentCommissionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenRatioByMoney, ct.OpenRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenRatioByVolume, ct.OpenRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseRatioByMoney, ct.CloseRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseRatioByVolume, ct.CloseRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseTodayRatioByMoney, ct.CloseTodayRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseTodayRatioByVolume, ct.CloseTodayRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///深度行情
struct DepthMarketDataField
{
	///交易日
	TDateType	TradingDay;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///最新价
	TPriceType	LastPrice;
	///上次结算价
	TPriceType	PreSettlementPrice;
	///昨收盘
	TPriceType	PreClosePrice;
	///昨持仓量
	TLargeVolumeType	PreOpenInterest;
	///今开盘
	TPriceType	OpenPrice;
	///最高价
	TPriceType	HighestPrice;
	///最低价
	TPriceType	LowestPrice;
	///数量
	TVolumeType	Volume;
	///成交金额
	TMoneyType	Turnover;
	///持仓量
	TLargeVolumeType	OpenInterest;
	///今收盘
	TPriceType	ClosePrice;
	///本次结算价
	TPriceType	SettlementPrice;
	///涨停板价
	TPriceType	UpperLimitPrice;
	///跌停板价
	TPriceType	LowerLimitPrice;
	///昨虚实度
	TRatioType	PreDelta;
	///今虚实度
	TRatioType	CurrDelta;
	///最后修改时间
	TTimeType	UpdateTime;
	///最后修改毫秒
	TMillisecType	UpdateMillisec;
	///申买价一
	TPriceType	BidPrice1;
	///申买量一
	TVolumeType	BidVolume1;
	///申卖价一
	TPriceType	AskPrice1;
	///申卖量一
	TVolumeType	AskVolume1;
	///申买价二
	TPriceType	BidPrice2;
	///申买量二
	TVolumeType	BidVolume2;
	///申卖价二
	TPriceType	AskPrice2;
	///申卖量二
	TVolumeType	AskVolume2;
	///申买价三
	TPriceType	BidPrice3;
	///申买量三
	TVolumeType	BidVolume3;
	///申卖价三
	TPriceType	AskPrice3;
	///申卖量三
	TVolumeType	AskVolume3;
	///申买价四
	TPriceType	BidPrice4;
	///申买量四
	TVolumeType	BidVolume4;
	///申卖价四
	TPriceType	AskPrice4;
	///申卖量四
	TVolumeType	AskVolume4;
	///申买价五
	TPriceType	BidPrice5;
	///申买量五
	TVolumeType	BidVolume5;
	///申卖价五
	TPriceType	AskPrice5;
	///申卖量五
	TVolumeType	AskVolume5;
	///当日均价
	TPriceType	AveragePrice;

bool operator==(const DepthMarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LastPrice, ct.LastPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreClosePrice, ct.PreClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreOpenInterest, ct.PreOpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenPrice, ct.OpenPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(HighestPrice, ct.HighestPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowestPrice, ct.LowestPrice));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Turnover, ct.Turnover));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenInterest, ct.OpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ClosePrice, ct.ClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UpperLimitPrice, ct.UpperLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowerLimitPrice, ct.LowerLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDelta, ct.PreDelta));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrDelta, ct.CurrDelta));
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (UpdateMillisec == ct.UpdateMillisec);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice1, ct.BidPrice1));
	if (!ret) return ret;
	ret = ret && (BidVolume1 == ct.BidVolume1);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice1, ct.AskPrice1));
	if (!ret) return ret;
	ret = ret && (AskVolume1 == ct.AskVolume1);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice2, ct.BidPrice2));
	if (!ret) return ret;
	ret = ret && (BidVolume2 == ct.BidVolume2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice2, ct.AskPrice2));
	if (!ret) return ret;
	ret = ret && (AskVolume2 == ct.AskVolume2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice3, ct.BidPrice3));
	if (!ret) return ret;
	ret = ret && (BidVolume3 == ct.BidVolume3);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice3, ct.AskPrice3));
	if (!ret) return ret;
	ret = ret && (AskVolume3 == ct.AskVolume3);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice4, ct.BidPrice4));
	if (!ret) return ret;
	ret = ret && (BidVolume4 == ct.BidVolume4);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice4, ct.AskPrice4));
	if (!ret) return ret;
	ret = ret && (AskVolume4 == ct.AskVolume4);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice5, ct.BidPrice5));
	if (!ret) return ret;
	ret = ret && (BidVolume5 == ct.BidVolume5);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice5, ct.AskPrice5));
	if (!ret) return ret;
	ret = ret && (AskVolume5 == ct.AskVolume5);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AveragePrice, ct.AveragePrice));
	if (!ret) return ret;

	return ret;
}
};

///投资者合约交易权限
struct InstrumentTradingRightField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///交易权限
	TTradingRightType	TradingRight;

bool operator==(const InstrumentTradingRightField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRight == ct.TradingRight);
	if (!ret) return ret;

	return ret;
}
};

///经纪公司用户
struct BrokerUserField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///用户名称
	TUserNameType	UserName;
	///用户类型
	TUserTypeType	UserType;
	///是否活跃
	TBoolType	IsActive;
	///是否使用令牌
	TBoolType	IsUsingOTP;

bool operator==(const BrokerUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserName, ct.UserName) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (IsUsingOTP == ct.IsUsingOTP);
	if (!ret) return ret;

	return ret;
}
};

///经纪公司用户口令
struct BrokerUserPasswordField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///密码
	TPasswordType	Password;

bool operator==(const BrokerUserPasswordField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;

	return ret;
}
};

///经纪公司用户功能权限
struct BrokerUserFunctionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///经纪公司功能代码
	TBrokerFunctionCodeType	BrokerFunctionCode;

bool operator==(const BrokerUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (BrokerFunctionCode == ct.BrokerFunctionCode);
	if (!ret) return ret;

	return ret;
}
};

///交易所交易员报盘机
struct TraderOfferField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///密码
	TPasswordType	Password;
	///安装编号
	TInstallIDType	InstallID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///交易所交易员连接状态
	TTraderConnectStatusType	TraderConnectStatus;
	///发出连接请求的日期
	TDateType	ConnectRequestDate;
	///发出连接请求的时间
	TTimeType	ConnectRequestTime;
	///上次报告日期
	TDateType	LastReportDate;
	///上次报告时间
	TTimeType	LastReportTime;
	///完成连接日期
	TDateType	ConnectDate;
	///完成连接时间
	TTimeType	ConnectTime;
	///启动日期
	TDateType	StartDate;
	///启动时间
	TTimeType	StartTime;
	///交易日
	TDateType	TradingDay;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///本席位最大成交编号
	TTradeIDType	MaxTradeID;
	///本席位最大报单备拷
	TReturnCodeType	MaxOrderMessageReference;

bool operator==(const TraderOfferField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (TraderConnectStatus == ct.TraderConnectStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectRequestDate, ct.ConnectRequestDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectRequestTime, ct.ConnectRequestTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportDate, ct.LastReportDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportTime, ct.LastReportTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectDate, ct.ConnectDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectTime, ct.ConnectTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartDate, ct.StartDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartTime, ct.StartTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxTradeID, ct.MaxTradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxOrderMessageReference, ct.MaxOrderMessageReference) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者结算结果
struct SettlementInfoField
{
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///序号
	TSequenceNoType	SequenceNo;
	///消息正文
	TContentType	Content;

bool operator==(const SettlementInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(Content, ct.Content) == 0);
	if (!ret) return ret;

	return ret;
}
};

///合约保证金率调整
struct InstrumentMarginRateAdjustField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;
	///是否相对交易所收取
	TBoolType	IsRelative;

bool operator==(const InstrumentMarginRateAdjustField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///交易所保证金率
struct ExchangeMarginRateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;

bool operator==(const ExchangeMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///交易所保证金率调整
struct ExchangeMarginRateAdjustField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///跟随交易所投资者多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///跟随交易所投资者多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///跟随交易所投资者空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///跟随交易所投资者空头保证金费
	TMoneyType	ShortMarginRatioByVolume;
	///交易所多头保证金率
	TRatioType	ExchLongMarginRatioByMoney;
	///交易所多头保证金费
	TMoneyType	ExchLongMarginRatioByVolume;
	///交易所空头保证金率
	TRatioType	ExchShortMarginRatioByMoney;
	///交易所空头保证金费
	TMoneyType	ExchShortMarginRatioByVolume;
	///不跟随交易所投资者多头保证金率
	TRatioType	NoLongMarginRatioByMoney;
	///不跟随交易所投资者多头保证金费
	TMoneyType	NoLongMarginRatioByVolume;
	///不跟随交易所投资者空头保证金率
	TRatioType	NoShortMarginRatioByMoney;
	///不跟随交易所投资者空头保证金费
	TMoneyType	NoShortMarginRatioByVolume;

bool operator==(const ExchangeMarginRateAdjustField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchLongMarginRatioByMoney, ct.ExchLongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchLongMarginRatioByVolume, ct.ExchLongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchShortMarginRatioByMoney, ct.ExchShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchShortMarginRatioByVolume, ct.ExchShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoLongMarginRatioByMoney, ct.NoLongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoLongMarginRatioByVolume, ct.NoLongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoShortMarginRatioByMoney, ct.NoShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoShortMarginRatioByVolume, ct.NoShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///结算引用
struct SettlementRefField
{
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;

bool operator==(const SettlementRefField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;

	return ret;
}
};

///当前时间
struct CurrentTimeField
{
	///当前日期
	TDateType	CurrDate;
	///当前时间
	TTimeType	CurrTime;
	///当前时间（毫秒）
	TMillisecType	CurrMillisec;

bool operator==(const CurrentTimeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(CurrDate, ct.CurrDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CurrTime, ct.CurrTime) == 0);
	if (!ret) return ret;
	ret = ret && (CurrMillisec == ct.CurrMillisec);
	if (!ret) return ret;

	return ret;
}
};

///通讯阶段
struct CommPhaseField
{
	///交易日
	TDateType	TradingDay;
	///通讯时段编号
	TCommPhaseNoType	CommPhaseNo;

bool operator==(const CommPhaseField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (CommPhaseNo == ct.CommPhaseNo);
	if (!ret) return ret;

	return ret;
}
};

///登录信息
struct LoginInfoField
{
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///登录日期
	TDateType	LoginDate;
	///登录时间
	TTimeType	LoginTime;
	///IP地址
	TIPAddressType	IPAddress;
	///用户端产品信息
	TProductInfoType	UserProductInfo;
	///接口端产品信息
	TProductInfoType	InterfaceProductInfo;
	///协议信息
	TProtocolInfoType	ProtocolInfo;
	///系统名称
	TSystemNameType	SystemName;
	///密码
	TPasswordType	Password;
	///最大报单引用
	TOrderRefType	MaxOrderRef;
	///上期所时间
	TTimeType	SHFETime;
	///大商所时间
	TTimeType	DCETime;
	///郑商所时间
	TTimeType	CZCETime;
	///中金所时间
	TTimeType	FFEXTime;
	///Mac地址
	TMacAddressType	MacAddress;
	///动态密码
	TPasswordType	OneTimePassword;

bool operator==(const LoginInfoField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginDate, ct.LoginDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginTime, ct.LoginTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(IPAddress, ct.IPAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InterfaceProductInfo, ct.InterfaceProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProtocolInfo, ct.ProtocolInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemName, ct.SystemName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxOrderRef, ct.MaxOrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SHFETime, ct.SHFETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DCETime, ct.DCETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CZCETime, ct.CZCETime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FFEXTime, ct.FFEXTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MacAddress, ct.MacAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OneTimePassword, ct.OneTimePassword) == 0);
	if (!ret) return ret;

	return ret;
}
};

///登录信息
struct LogoutAllField
{
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///系统名称
	TSystemNameType	SystemName;

bool operator==(const LogoutAllField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemName, ct.SystemName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///前置状态
struct FrontStatusField
{
	///前置编号
	TFrontIDType	FrontID;
	///上次报告日期
	TDateType	LastReportDate;
	///上次报告时间
	TTimeType	LastReportTime;
	///是否活跃
	TBoolType	IsActive;

bool operator==(const FrontStatusField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportDate, ct.LastReportDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportTime, ct.LastReportTime) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///用户口令变更
struct UserPasswordUpdateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///原来的口令
	TPasswordType	OldPassword;
	///新的口令
	TPasswordType	NewPassword;

bool operator==(const UserPasswordUpdateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OldPassword, ct.OldPassword) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(NewPassword, ct.NewPassword) == 0);
	if (!ret) return ret;

	return ret;
}
};

///输入报单
struct InputOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///用户强评标志
	TBoolType	UserForceClose;

bool operator==(const InputOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;

	return ret;
}
};

///报单
struct OrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///报单提交状态
	TOrderSubmitStatusType	OrderSubmitStatus;
	///报单提示序号
	TSequenceNoType	NotifySequence;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///报单来源
	TOrderSourceType	OrderSource;
	///报单状态
	TOrderStatusType	OrderStatus;
	///报单类型
	TOrderTypeType	OrderType;
	///今成交数量
	TVolumeType	VolumeTraded;
	///剩余数量
	TVolumeType	VolumeTotal;
	///报单日期
	TDateType	InsertDate;
	///委托时间
	TTimeType	InsertTime;
	///激活时间
	TTimeType	ActiveTime;
	///挂起时间
	TTimeType	SuspendTime;
	///最后修改时间
	TTimeType	UpdateTime;
	///撤销时间
	TTimeType	CancelTime;
	///最后修改交易所交易员代码
	TTraderIDType	ActiveTraderID;
	///结算会员编号
	TParticipantIDType	ClearingPartID;
	///序号
	TSequenceNoType	SequenceNo;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///用户端产品信息
	TProductInfoType	UserProductInfo;
	///状态信息
	TErrorMsgType	StatusMsg;
	///用户强评标志
	TBoolType	UserForceClose;
	///操作用户代码
	TUserIDType	ActiveUserID;
	///经纪公司报单编号
	TSequenceNoType	BrokerOrderSeq;
	///相关报单
	TOrderSysIDType	RelativeOrderSysID;
	///郑商所成交数量
	TVolumeType	ZCETotalTradedVolume;

bool operator==(const OrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (NotifySequence == ct.NotifySequence);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderSource == ct.OrderSource);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (OrderType == ct.OrderType);
	if (!ret) return ret;
	ret = ret && (VolumeTraded == ct.VolumeTraded);
	if (!ret) return ret;
	ret = ret && (VolumeTotal == ct.VolumeTotal);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertDate, ct.InsertDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTime, ct.InsertTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTime, ct.ActiveTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SuspendTime, ct.SuspendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CancelTime, ct.CancelTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTraderID, ct.ActiveTraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveUserID, ct.ActiveUserID) == 0);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (strcmp(RelativeOrderSysID, ct.RelativeOrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ZCETotalTradedVolume == ct.ZCETotalTradedVolume);
	if (!ret) return ret;

	return ret;
}
};

///交易所报单
struct ExchangeOrderField
{
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///报单提交状态
	TOrderSubmitStatusType	OrderSubmitStatus;
	///报单提示序号
	TSequenceNoType	NotifySequence;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///报单来源
	TOrderSourceType	OrderSource;
	///报单状态
	TOrderStatusType	OrderStatus;
	///报单类型
	TOrderTypeType	OrderType;
	///今成交数量
	TVolumeType	VolumeTraded;
	///剩余数量
	TVolumeType	VolumeTotal;
	///报单日期
	TDateType	InsertDate;
	///委托时间
	TTimeType	InsertTime;
	///激活时间
	TTimeType	ActiveTime;
	///挂起时间
	TTimeType	SuspendTime;
	///最后修改时间
	TTimeType	UpdateTime;
	///撤销时间
	TTimeType	CancelTime;
	///最后修改交易所交易员代码
	TTraderIDType	ActiveTraderID;
	///结算会员编号
	TParticipantIDType	ClearingPartID;
	///序号
	TSequenceNoType	SequenceNo;

bool operator==(const ExchangeOrderField &ct) const
{
	bool ret = true;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (NotifySequence == ct.NotifySequence);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderSource == ct.OrderSource);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (OrderType == ct.OrderType);
	if (!ret) return ret;
	ret = ret && (VolumeTraded == ct.VolumeTraded);
	if (!ret) return ret;
	ret = ret && (VolumeTotal == ct.VolumeTotal);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertDate, ct.InsertDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTime, ct.InsertTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTime, ct.ActiveTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SuspendTime, ct.SuspendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CancelTime, ct.CancelTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTraderID, ct.ActiveTraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///交易所报单插入失败
struct ExchangeOrderInsertErrorField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const ExchangeOrderInsertErrorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///输入报单操作
struct InputOrderActionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///报单操作引用
	TOrderActionRefType	OrderActionRef;
	///报单引用
	TOrderRefType	OrderRef;
	///请求编号
	TRequestIDType	RequestID;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///操作标志
	TActionFlagType	ActionFlag;
	///价格
	TPriceType	LimitPrice;
	///数量变化
	TVolumeType	VolumeChange;
	///用户代码
	TUserIDType	UserID;
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const InputOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionRef == ct.OrderActionRef);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///报单操作
struct OrderActionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///报单操作引用
	TOrderActionRefType	OrderActionRef;
	///报单引用
	TOrderRefType	OrderRef;
	///请求编号
	TRequestIDType	RequestID;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///操作标志
	TActionFlagType	ActionFlag;
	///价格
	TPriceType	LimitPrice;
	///数量变化
	TVolumeType	VolumeChange;
	///操作日期
	TDateType	ActionDate;
	///操作时间
	TTimeType	ActionTime;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///操作本地编号
	TOrderLocalIDType	ActionLocalID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///报单操作状态
	TOrderActionStatusType	OrderActionStatus;
	///用户代码
	TUserIDType	UserID;
	///状态信息
	TErrorMsgType	StatusMsg;
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const OrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionRef == ct.OrderActionRef);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionDate, ct.ActionDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionTime, ct.ActionTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionLocalID, ct.ActionLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionStatus == ct.OrderActionStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///交易所报单操作
struct ExchangeOrderActionField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///操作标志
	TActionFlagType	ActionFlag;
	///价格
	TPriceType	LimitPrice;
	///数量变化
	TVolumeType	VolumeChange;
	///操作日期
	TDateType	ActionDate;
	///操作时间
	TTimeType	ActionTime;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///操作本地编号
	TOrderLocalIDType	ActionLocalID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///报单操作状态
	TOrderActionStatusType	OrderActionStatus;
	///用户代码
	TUserIDType	UserID;

bool operator==(const ExchangeOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionDate, ct.ActionDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionTime, ct.ActionTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionLocalID, ct.ActionLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionStatus == ct.OrderActionStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///交易所报单操作失败
struct ExchangeOrderActionErrorField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///操作本地编号
	TOrderLocalIDType	ActionLocalID;
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const ExchangeOrderActionErrorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionLocalID, ct.ActionLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///交易所成交
struct ExchangeTradeField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///成交编号
	TTradeIDType	TradeID;
	///买卖方向
	TDirectionType	Direction;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///交易角色
	TTradingRoleType	TradingRole;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///开平标志
	TOffsetFlagType	OffsetFlag;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///价格
	TPriceType	Price;
	///数量
	TVolumeType	Volume;
	///成交时期
	TDateType	TradeDate;
	///成交时间
	TTimeType	TradeTime;
	///成交类型
	TTradeTypeType	TradeType;
	///成交价来源
	TPriceSourceType	PriceSource;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///结算会员编号
	TParticipantIDType	ClearingPartID;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///序号
	TSequenceNoType	SequenceNo;
	///成交来源
	TTradeSourceType	TradeSource;

bool operator==(const ExchangeTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRole == ct.TradingRole);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (PriceSource == ct.PriceSource);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (TradeSource == ct.TradeSource);
	if (!ret) return ret;

	return ret;
}
};

///成交
struct TradeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///成交编号
	TTradeIDType	TradeID;
	///买卖方向
	TDirectionType	Direction;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///交易角色
	TTradingRoleType	TradingRole;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///开平标志
	TOffsetFlagType	OffsetFlag;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///价格
	TPriceType	Price;
	///数量
	TVolumeType	Volume;
	///成交时期
	TDateType	TradeDate;
	///成交时间
	TTimeType	TradeTime;
	///成交类型
	TTradeTypeType	TradeType;
	///成交价来源
	TPriceSourceType	PriceSource;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///结算会员编号
	TParticipantIDType	ClearingPartID;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///序号
	TSequenceNoType	SequenceNo;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///经纪公司报单编号
	TSequenceNoType	BrokerOrderSeq;
	///成交来源
	TTradeSourceType	TradeSource;

bool operator==(const TradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRole == ct.TradingRole);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (PriceSource == ct.PriceSource);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (TradeSource == ct.TradeSource);
	if (!ret) return ret;

	return ret;
}
};

///用户会话
struct UserSessionField
{
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///登录日期
	TDateType	LoginDate;
	///登录时间
	TTimeType	LoginTime;
	///IP地址
	TIPAddressType	IPAddress;
	///用户端产品信息
	TProductInfoType	UserProductInfo;
	///接口端产品信息
	TProductInfoType	InterfaceProductInfo;
	///协议信息
	TProtocolInfoType	ProtocolInfo;
	///Mac地址
	TMacAddressType	MacAddress;

bool operator==(const UserSessionField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginDate, ct.LoginDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LoginTime, ct.LoginTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(IPAddress, ct.IPAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InterfaceProductInfo, ct.InterfaceProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProtocolInfo, ct.ProtocolInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MacAddress, ct.MacAddress) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询最大报单数量
struct QueryMaxOrderVolumeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///买卖方向
	TDirectionType	Direction;
	///开平标志
	TOffsetFlagType	OffsetFlag;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///最大允许报单数量
	TVolumeType	MaxVolume;

bool operator==(const QueryMaxOrderVolumeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (MaxVolume == ct.MaxVolume);
	if (!ret) return ret;

	return ret;
}
};

///投资者结算结果确认信息
struct SettlementInfoConfirmField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///确认日期
	TDateType	ConfirmDate;
	///确认时间
	TTimeType	ConfirmTime;

bool operator==(const SettlementInfoConfirmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConfirmDate, ct.ConfirmDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConfirmTime, ct.ConfirmTime) == 0);
	if (!ret) return ret;

	return ret;
}
};

///出入金同步
struct SyncDepositField
{
	///出入金流水号
	TDepositSeqNoType	DepositSeqNo;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///入金金额
	TMoneyType	Deposit;
	///是否强制进行
	TBoolType	IsForce;

bool operator==(const SyncDepositField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(DepositSeqNo, ct.DepositSeqNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Deposit, ct.Deposit));
	if (!ret) return ret;
	ret = ret && (IsForce == ct.IsForce);
	if (!ret) return ret;

	return ret;
}
};

///经纪公司同步
struct BrokerSyncField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;

bool operator==(const BrokerSyncField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///正在同步中的投资者
struct SyncingInvestorField
{
	///投资者代码
	TInvestorIDType	InvestorID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者分组代码
	TInvestorIDType	InvestorGroupID;
	///投资者名称
	TPartyNameType	InvestorName;
	///证件类型
	TIdCardTypeType	IdentifiedCardType;
	///证件号码
	TIdentifiedCardNoType	IdentifiedCardNo;
	///是否活跃
	TBoolType	IsActive;
	///联系电话
	TTelephoneType	Telephone;
	///通讯地址
	TAddressType	Address;
	///开户日期
	TDateType	OpenDate;
	///手机
	TMobileType	Mobile;
	///手续费率模板代码
	TInvestorIDType	CommModelID;

bool operator==(const SyncingInvestorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorName, ct.InvestorName) == 0);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CommModelID, ct.CommModelID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///正在同步中的交易代码
struct SyncingTradingCodeField
{
	///投资者代码
	TInvestorIDType	InvestorID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///客户代码
	TClientIDType	ClientID;
	///是否活跃
	TBoolType	IsActive;
	///交易编码类型
	TClientIDTypeType	ClientIDType;

bool operator==(const SyncingTradingCodeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (ClientIDType == ct.ClientIDType);
	if (!ret) return ret;

	return ret;
}
};

///正在同步中的投资者分组
struct SyncingInvestorGroupField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者分组代码
	TInvestorIDType	InvestorGroupID;
	///投资者分组名称
	TInvestorGroupNameType	InvestorGroupName;

bool operator==(const SyncingInvestorGroupField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupName, ct.InvestorGroupName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///正在同步中的交易账号
struct SyncingTradingAccountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者帐号
	TAccountIDType	AccountID;
	///上次质押金额
	TMoneyType	PreMortgage;
	///上次信用额度
	TMoneyType	PreCredit;
	///上次存款额
	TMoneyType	PreDeposit;
	///上次结算准备金
	TMoneyType	PreBalance;
	///上次占用的保证金
	TMoneyType	PreMargin;
	///利息基数
	TMoneyType	InterestBase;
	///利息收入
	TMoneyType	Interest;
	///入金金额
	TMoneyType	Deposit;
	///出金金额
	TMoneyType	Withdraw;
	///冻结的保证金
	TMoneyType	FrozenMargin;
	///冻结的资金
	TMoneyType	FrozenCash;
	///冻结的手续费
	TMoneyType	FrozenCommission;
	///当前保证金总额
	TMoneyType	CurrMargin;
	///资金差额
	TMoneyType	CashIn;
	///手续费
	TMoneyType	Commission;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///持仓盈亏
	TMoneyType	PositionProfit;
	///期货结算准备金
	TMoneyType	Balance;
	///可用资金
	TMoneyType	Available;
	///可取资金
	TMoneyType	WithdrawQuota;
	///基本准备金
	TMoneyType	Reserve;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///信用额度
	TMoneyType	Credit;
	///质押金额
	TMoneyType	Mortgage;
	///交易所保证金
	TMoneyType	ExchangeMargin;
	///投资者交割保证金
	TMoneyType	DeliveryMargin;
	///交易所交割保证金
	TMoneyType	ExchangeDeliveryMargin;

bool operator==(const SyncingTradingAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMortgage, ct.PreMortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreCredit, ct.PreCredit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDeposit, ct.PreDeposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(InterestBase, ct.InterestBase));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Interest, ct.Interest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Deposit, ct.Deposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WithdrawQuota, ct.WithdrawQuota));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Credit, ct.Credit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Mortgage, ct.Mortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(DeliveryMargin, ct.DeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeDeliveryMargin, ct.ExchangeDeliveryMargin));
	if (!ret) return ret;

	return ret;
}
};

///正在同步中的投资者持仓
struct SyncingInvestorPositionField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///持仓多空方向
	TPosiDirectionType	PosiDirection;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///持仓日期
	TPositionDateType	PositionDate;
	///上日持仓
	TVolumeType	YdPosition;
	///今日持仓
	TVolumeType	Position;
	///多头冻结
	TVolumeType	LongFrozen;
	///空头冻结
	TVolumeType	ShortFrozen;
	///开仓冻结金额
	TMoneyType	LongFrozenAmount;
	///开仓冻结金额
	TMoneyType	ShortFrozenAmount;
	///开仓量
	TVolumeType	OpenVolume;
	///平仓量
	TVolumeType	CloseVolume;
	///开仓金额
	TMoneyType	OpenAmount;
	///平仓金额
	TMoneyType	CloseAmount;
	///持仓成本
	TMoneyType	PositionCost;
	///上次占用的保证金
	TMoneyType	PreMargin;
	///占用的保证金
	TMoneyType	UseMargin;
	///冻结的保证金
	TMoneyType	FrozenMargin;
	///冻结的资金
	TMoneyType	FrozenCash;
	///冻结的手续费
	TMoneyType	FrozenCommission;
	///资金差额
	TMoneyType	CashIn;
	///手续费
	TMoneyType	Commission;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///持仓盈亏
	TMoneyType	PositionProfit;
	///上次结算价
	TPriceType	PreSettlementPrice;
	///本次结算价
	TPriceType	SettlementPrice;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///开仓成本
	TMoneyType	OpenCost;
	///交易所保证金
	TMoneyType	ExchangeMargin;
	///组合成交形成的持仓
	TVolumeType	CombPosition;
	///组合多头冻结
	TVolumeType	CombLongFrozen;
	///组合空头冻结
	TVolumeType	CombShortFrozen;
	///逐日盯市平仓盈亏
	TMoneyType	CloseProfitByDate;
	///逐笔对冲平仓盈亏
	TMoneyType	CloseProfitByTrade;
	///今日持仓
	TVolumeType	TodayPosition;
	///保证金率
	TRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TRatioType	MarginRateByVolume;

bool operator==(const SyncingInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongFrozen == ct.LongFrozen);
	if (!ret) return ret;
	ret = ret && (ShortFrozen == ct.ShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongFrozenAmount, ct.LongFrozenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortFrozenAmount, ct.ShortFrozenAmount));
	if (!ret) return ret;
	ret = ret && (OpenVolume == ct.OpenVolume);
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenAmount, ct.OpenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseMargin, ct.UseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenCost, ct.OpenCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (CombPosition == ct.CombPosition);
	if (!ret) return ret;
	ret = ret && (CombLongFrozen == ct.CombLongFrozen);
	if (!ret) return ret;
	ret = ret && (CombShortFrozen == ct.CombShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;

	return ret;
}
};

///正在同步中的合约保证金率
struct SyncingInstrumentMarginRateField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;
	///是否相对交易所收取
	TBoolType	IsRelative;

bool operator==(const SyncingInstrumentMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///正在同步中的合约手续费率
struct SyncingInstrumentCommissionRateField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///开仓手续费率
	TRatioType	OpenRatioByMoney;
	///开仓手续费
	TRatioType	OpenRatioByVolume;
	///平仓手续费率
	TRatioType	CloseRatioByMoney;
	///平仓手续费
	TRatioType	CloseRatioByVolume;
	///平今手续费率
	TRatioType	CloseTodayRatioByMoney;
	///平今手续费
	TRatioType	CloseTodayRatioByVolume;

bool operator==(const SyncingInstrumentCommissionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenRatioByMoney, ct.OpenRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenRatioByVolume, ct.OpenRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseRatioByMoney, ct.CloseRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseRatioByVolume, ct.CloseRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseTodayRatioByMoney, ct.CloseTodayRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseTodayRatioByVolume, ct.CloseTodayRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///正在同步中的合约交易权限
struct SyncingInstrumentTradingRightField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///交易权限
	TTradingRightType	TradingRight;

bool operator==(const SyncingInstrumentTradingRightField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRight == ct.TradingRight);
	if (!ret) return ret;

	return ret;
}
};

///查询报单
struct QryOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///开始时间
	TTimeType	InsertTimeStart;
	///结束时间
	TTimeType	InsertTimeEnd;

bool operator==(const QryOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTimeStart, ct.InsertTimeStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTimeEnd, ct.InsertTimeEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询成交
struct QryTradeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///成交编号
	TTradeIDType	TradeID;
	///开始时间
	TTimeType	TradeTimeStart;
	///结束时间
	TTimeType	TradeTimeEnd;

bool operator==(const QryTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTimeStart, ct.TradeTimeStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTimeEnd, ct.TradeTimeEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询投资者持仓
struct QryInvestorPositionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const QryInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询资金账户
struct QryTradingAccountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const QryTradingAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询投资者
struct QryInvestorField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const QryInvestorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询交易编码
struct QryTradingCodeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///客户代码
	TClientIDType	ClientID;
	///交易编码类型
	TClientIDTypeType	ClientIDType;

bool operator==(const QryTradingCodeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (ClientIDType == ct.ClientIDType);
	if (!ret) return ret;

	return ret;
}
};

///查询交易编码
struct QryInvestorGroupField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;

bool operator==(const QryInvestorGroupField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询交易编码
struct QryInstrumentMarginRateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;

bool operator==(const QryInstrumentMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;

	return ret;
}
};

///查询交易编码
struct QryInstrumentCommissionRateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const QryInstrumentCommissionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询交易编码
struct QryInstrumentTradingRightField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const QryInstrumentTradingRightField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询经纪公司
struct QryBrokerField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;

bool operator==(const QryBrokerField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询交易员
struct QryTraderField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///交易所交易员代码
	TTraderIDType	TraderID;

bool operator==(const QryTraderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询经纪公司会员代码
struct QryPartBrokerField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///会员代码
	TParticipantIDType	ParticipantID;

bool operator==(const QryPartBrokerField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询管理用户功能权限
struct QrySuperUserFunctionField
{
	///用户代码
	TUserIDType	UserID;

bool operator==(const QrySuperUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询用户会话
struct QryUserSessionField
{
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;

bool operator==(const QryUserSessionField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询前置状态
struct QryFrontStatusField
{
	///前置编号
	TFrontIDType	FrontID;

bool operator==(const QryFrontStatusField &ct) const
{
	bool ret = true;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;

	return ret;
}
};

///查询交易所报单
struct QryExchangeOrderField
{
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///交易所交易员代码
	TTraderIDType	TraderID;

bool operator==(const QryExchangeOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询报单操作
struct QryOrderActionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///交易所代码
	TExchangeIDType	ExchangeID;

bool operator==(const QryOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询交易所报单操作
struct QryExchangeOrderActionField
{
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///交易所交易员代码
	TTraderIDType	TraderID;

bool operator==(const QryExchangeOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询管理用户
struct QrySuperUserField
{
	///用户代码
	TUserIDType	UserID;

bool operator==(const QrySuperUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询交易所
struct QryExchangeField
{
	///交易所代码
	TExchangeIDType	ExchangeID;

bool operator==(const QryExchangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询产品
struct QryProductField
{
	///产品代码
	TInstrumentIDType	ProductID;

bool operator==(const QryProductField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询合约
struct QryInstrumentField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///产品代码
	TInstrumentIDType	ProductID;

bool operator==(const QryInstrumentField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询行情
struct QryDepthMarketDataField
{
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const QryDepthMarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询经纪公司用户
struct QryBrokerUserField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;

bool operator==(const QryBrokerUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询经纪公司用户权限
struct QryBrokerUserFunctionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;

bool operator==(const QryBrokerUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询交易员报盘机
struct QryTraderOfferField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///交易所交易员代码
	TTraderIDType	TraderID;

bool operator==(const QryTraderOfferField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询出入金流水
struct QrySyncDepositField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///出入金流水号
	TDepositSeqNoType	DepositSeqNo;

bool operator==(const QrySyncDepositField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DepositSeqNo, ct.DepositSeqNo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询投资者结算结果
struct QrySettlementInfoField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///交易日
	TDateType	TradingDay;

bool operator==(const QrySettlementInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询报单
struct QryHisOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///开始时间
	TTimeType	InsertTimeStart;
	///结束时间
	TTimeType	InsertTimeEnd;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;

bool operator==(const QryHisOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTimeStart, ct.InsertTimeStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTimeEnd, ct.InsertTimeEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;

	return ret;
}
};

///组织架构投资者对应关系扁平表
struct InvestorDepartmentFlatField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///组织架构代码
	TInvestorIDType	DepartmentID;

bool operator==(const InvestorDepartmentFlatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DepartmentID, ct.DepartmentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///操作员组织架构关系
struct DepartmentUserField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///投资者范围
	TDepartmentRangeType	InvestorRange;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const DepartmentUserField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///组织架构
struct DepartmentField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///组织架构代码
	TInvestorIDType	DepartmentID;
	///组织架构名称
	TPartyNameType	DepartmentName;

bool operator==(const DepartmentField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DepartmentID, ct.DepartmentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(DepartmentName, ct.DepartmentName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询经纪公司资金
struct QueryBrokerDepositField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所代码
	TExchangeIDType	ExchangeID;

bool operator==(const QueryBrokerDepositField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///经纪公司资金
struct BrokerDepositField
{
	///交易日期
	TTradeDateType	TradingDay;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///上次结算准备金
	TMoneyType	PreBalance;
	///当前保证金总额
	TMoneyType	CurrMargin;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///期货结算准备金
	TMoneyType	Balance;
	///入金金额
	TMoneyType	Deposit;
	///出金金额
	TMoneyType	Withdraw;
	///可提资金
	TMoneyType	Available;
	///基本准备金
	TMoneyType	Reserve;
	///冻结的保证金
	TMoneyType	FrozenMargin;

bool operator==(const BrokerDepositField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Deposit, ct.Deposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;

	return ret;
}
};

///市场行情
struct MarketDataField
{
	///交易日
	TDateType	TradingDay;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///最新价
	TPriceType	LastPrice;
	///上次结算价
	TPriceType	PreSettlementPrice;
	///昨收盘
	TPriceType	PreClosePrice;
	///昨持仓量
	TLargeVolumeType	PreOpenInterest;
	///今开盘
	TPriceType	OpenPrice;
	///最高价
	TPriceType	HighestPrice;
	///最低价
	TPriceType	LowestPrice;
	///数量
	TVolumeType	Volume;
	///成交金额
	TMoneyType	Turnover;
	///持仓量
	TLargeVolumeType	OpenInterest;
	///今收盘
	TPriceType	ClosePrice;
	///本次结算价
	TPriceType	SettlementPrice;
	///涨停板价
	TPriceType	UpperLimitPrice;
	///跌停板价
	TPriceType	LowerLimitPrice;
	///昨虚实度
	TRatioType	PreDelta;
	///今虚实度
	TRatioType	CurrDelta;
	///最后修改时间
	TTimeType	UpdateTime;
	///最后修改毫秒
	TMillisecType	UpdateMillisec;

bool operator==(const MarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LastPrice, ct.LastPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreClosePrice, ct.PreClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreOpenInterest, ct.PreOpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenPrice, ct.OpenPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(HighestPrice, ct.HighestPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowestPrice, ct.LowestPrice));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Turnover, ct.Turnover));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenInterest, ct.OpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ClosePrice, ct.ClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UpperLimitPrice, ct.UpperLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowerLimitPrice, ct.LowerLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDelta, ct.PreDelta));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrDelta, ct.CurrDelta));
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (UpdateMillisec == ct.UpdateMillisec);
	if (!ret) return ret;

	return ret;
}
};

///行情基础属性
struct MarketDataBaseField
{
	///交易日
	TDateType	TradingDay;
	///上次结算价
	TPriceType	PreSettlementPrice;
	///昨收盘
	TPriceType	PreClosePrice;
	///昨持仓量
	TLargeVolumeType	PreOpenInterest;
	///昨虚实度
	TRatioType	PreDelta;

bool operator==(const MarketDataBaseField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreClosePrice, ct.PreClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreOpenInterest, ct.PreOpenInterest));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDelta, ct.PreDelta));
	if (!ret) return ret;

	return ret;
}
};

///行情静态属性
struct MarketDataStaticField
{
	///今开盘
	TPriceType	OpenPrice;
	///最高价
	TPriceType	HighestPrice;
	///最低价
	TPriceType	LowestPrice;
	///今收盘
	TPriceType	ClosePrice;
	///涨停板价
	TPriceType	UpperLimitPrice;
	///跌停板价
	TPriceType	LowerLimitPrice;
	///本次结算价
	TPriceType	SettlementPrice;
	///今虚实度
	TRatioType	CurrDelta;

bool operator==(const MarketDataStaticField &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(OpenPrice, ct.OpenPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(HighestPrice, ct.HighestPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowestPrice, ct.LowestPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ClosePrice, ct.ClosePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UpperLimitPrice, ct.UpperLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LowerLimitPrice, ct.LowerLimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrDelta, ct.CurrDelta));
	if (!ret) return ret;

	return ret;
}
};

///行情最新成交属性
struct MarketDataLastMatchField
{
	///最新价
	TPriceType	LastPrice;
	///数量
	TVolumeType	Volume;
	///成交金额
	TMoneyType	Turnover;
	///持仓量
	TLargeVolumeType	OpenInterest;

bool operator==(const MarketDataLastMatchField &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(LastPrice, ct.LastPrice));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Turnover, ct.Turnover));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenInterest, ct.OpenInterest));
	if (!ret) return ret;

	return ret;
}
};

///行情最优价属性
struct MarketDataBestPriceField
{
	///申买价一
	TPriceType	BidPrice1;
	///申买量一
	TVolumeType	BidVolume1;
	///申卖价一
	TPriceType	AskPrice1;
	///申卖量一
	TVolumeType	AskVolume1;

bool operator==(const MarketDataBestPriceField &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(BidPrice1, ct.BidPrice1));
	if (!ret) return ret;
	ret = ret && (BidVolume1 == ct.BidVolume1);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice1, ct.AskPrice1));
	if (!ret) return ret;
	ret = ret && (AskVolume1 == ct.AskVolume1);
	if (!ret) return ret;

	return ret;
}
};

///行情申买二、三属性
struct MarketDataBid23Field
{
	///申买价二
	TPriceType	BidPrice2;
	///申买量二
	TVolumeType	BidVolume2;
	///申买价三
	TPriceType	BidPrice3;
	///申买量三
	TVolumeType	BidVolume3;

bool operator==(const MarketDataBid23Field &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(BidPrice2, ct.BidPrice2));
	if (!ret) return ret;
	ret = ret && (BidVolume2 == ct.BidVolume2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice3, ct.BidPrice3));
	if (!ret) return ret;
	ret = ret && (BidVolume3 == ct.BidVolume3);
	if (!ret) return ret;

	return ret;
}
};

///行情申卖二、三属性
struct MarketDataAsk23Field
{
	///申卖价二
	TPriceType	AskPrice2;
	///申卖量二
	TVolumeType	AskVolume2;
	///申卖价三
	TPriceType	AskPrice3;
	///申卖量三
	TVolumeType	AskVolume3;

bool operator==(const MarketDataAsk23Field &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(AskPrice2, ct.AskPrice2));
	if (!ret) return ret;
	ret = ret && (AskVolume2 == ct.AskVolume2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice3, ct.AskPrice3));
	if (!ret) return ret;
	ret = ret && (AskVolume3 == ct.AskVolume3);
	if (!ret) return ret;

	return ret;
}
};

///行情申买四、五属性
struct MarketDataBid45Field
{
	///申买价四
	TPriceType	BidPrice4;
	///申买量四
	TVolumeType	BidVolume4;
	///申买价五
	TPriceType	BidPrice5;
	///申买量五
	TVolumeType	BidVolume5;

bool operator==(const MarketDataBid45Field &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(BidPrice4, ct.BidPrice4));
	if (!ret) return ret;
	ret = ret && (BidVolume4 == ct.BidVolume4);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BidPrice5, ct.BidPrice5));
	if (!ret) return ret;
	ret = ret && (BidVolume5 == ct.BidVolume5);
	if (!ret) return ret;

	return ret;
}
};

///行情申卖四、五属性
struct MarketDataAsk45Field
{
	///申卖价四
	TPriceType	AskPrice4;
	///申卖量四
	TVolumeType	AskVolume4;
	///申卖价五
	TPriceType	AskPrice5;
	///申卖量五
	TVolumeType	AskVolume5;

bool operator==(const MarketDataAsk45Field &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(AskPrice4, ct.AskPrice4));
	if (!ret) return ret;
	ret = ret && (AskVolume4 == ct.AskVolume4);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(AskPrice5, ct.AskPrice5));
	if (!ret) return ret;
	ret = ret && (AskVolume5 == ct.AskVolume5);
	if (!ret) return ret;

	return ret;
}
};

///行情更新时间属性
struct MarketDataUpdateTimeField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///最后修改时间
	TTimeType	UpdateTime;
	///最后修改毫秒
	TMillisecType	UpdateMillisec;

bool operator==(const MarketDataUpdateTimeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (UpdateMillisec == ct.UpdateMillisec);
	if (!ret) return ret;

	return ret;
}
};

///指定的合约
struct SpecificInstrumentField
{
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const SpecificInstrumentField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///合约状态
struct InstrumentStatusField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///结算组代码
	TSettlementGroupIDType	SettlementGroupID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///合约交易状态
	TInstrumentStatusType	InstrumentStatus;
	///交易阶段编号
	TTradingSegmentSNType	TradingSegmentSN;
	///进入本状态时间
	TTimeType	EnterTime;
	///进入本状态原因
	TInstStatusEnterReasonType	EnterReason;

bool operator==(const InstrumentStatusField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementGroupID, ct.SettlementGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InstrumentStatus == ct.InstrumentStatus);
	if (!ret) return ret;
	ret = ret && (TradingSegmentSN == ct.TradingSegmentSN);
	if (!ret) return ret;
	ret = ret && (strcmp(EnterTime, ct.EnterTime) == 0);
	if (!ret) return ret;
	ret = ret && (EnterReason == ct.EnterReason);
	if (!ret) return ret;

	return ret;
}
};

///查询合约状态
struct QryInstrumentStatusField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;

bool operator==(const QryInstrumentStatusField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者账户
struct InvestorAccountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投资者帐号
	TAccountIDType	AccountID;

bool operator==(const InvestorAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///浮动盈亏算法
struct PositionProfitAlgorithmField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者帐号
	TAccountIDType	AccountID;
	///盈亏算法
	TAlgorithmType	Algorithm;
	///备注
	TMemoType	Memo;

bool operator==(const PositionProfitAlgorithmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (Algorithm == ct.Algorithm);
	if (!ret) return ret;
	ret = ret && (strcmp(Memo, ct.Memo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///会员资金折扣
struct DiscountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///投资者代码
	TInvestorIDType	InvestorID;
	///资金折扣比例
	TRatioType	Discount;

bool operator==(const DiscountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Discount, ct.Discount));
	if (!ret) return ret;

	return ret;
}
};

///查询转帐银行
struct QryTransferBankField
{
	///银行代码
	TBankIDType	BankID;
	///银行分中心代码
	TBankBrchIDType	BankBrchID;

bool operator==(const QryTransferBankField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///转帐银行
struct TransferBankField
{
	///银行代码
	TBankIDType	BankID;
	///银行分中心代码
	TBankBrchIDType	BankBrchID;
	///银行名称
	TBankNameType	BankName;
	///是否活跃
	TBoolType	IsActive;

bool operator==(const TransferBankField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankName, ct.BankName) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///查询投资者持仓明细
struct QryInvestorPositionDetailField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const QryInvestorPositionDetailField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者持仓明细
struct InvestorPositionDetailField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///买卖
	TDirectionType	Direction;
	///开仓日期
	TDateType	OpenDate;
	///成交编号
	TTradeIDType	TradeID;
	///数量
	TVolumeType	Volume;
	///开仓价
	TPriceType	OpenPrice;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///成交类型
	TTradeTypeType	TradeType;
	///组合合约代码
	TInstrumentIDType	CombInstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///逐日盯市平仓盈亏
	TMoneyType	CloseProfitByDate;
	///逐笔对冲平仓盈亏
	TMoneyType	CloseProfitByTrade;
	///逐日盯市持仓盈亏
	TMoneyType	PositionProfitByDate;
	///逐笔对冲持仓盈亏
	TMoneyType	PositionProfitByTrade;
	///投资者保证金
	TMoneyType	Margin;
	///交易所保证金
	TMoneyType	ExchMargin;
	///保证金率
	TRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TRatioType	MarginRateByVolume;
	///昨结算价
	TPriceType	LastSettlementPrice;
	///结算价
	TPriceType	SettlementPrice;
	///平仓量
	TVolumeType	CloseVolume;
	///平仓金额
	TMoneyType	CloseAmount;

bool operator==(const InvestorPositionDetailField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenPrice, ct.OpenPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfitByDate, ct.PositionProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfitByTrade, ct.PositionProfitByTrade));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Margin, ct.Margin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchMargin, ct.ExchMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LastSettlementPrice, ct.LastSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;

	return ret;
}
};

///资金账户口令域
struct TradingAccountPasswordField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者帐号
	TAccountIDType	AccountID;
	///密码
	TPasswordType	Password;

bool operator==(const TradingAccountPasswordField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;

	return ret;
}
};

///交易所行情报盘机
struct MDTraderOfferField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///密码
	TPasswordType	Password;
	///安装编号
	TInstallIDType	InstallID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///交易所交易员连接状态
	TTraderConnectStatusType	TraderConnectStatus;
	///发出连接请求的日期
	TDateType	ConnectRequestDate;
	///发出连接请求的时间
	TTimeType	ConnectRequestTime;
	///上次报告日期
	TDateType	LastReportDate;
	///上次报告时间
	TTimeType	LastReportTime;
	///完成连接日期
	TDateType	ConnectDate;
	///完成连接时间
	TTimeType	ConnectTime;
	///启动日期
	TDateType	StartDate;
	///启动时间
	TTimeType	StartTime;
	///交易日
	TDateType	TradingDay;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///本席位最大成交编号
	TTradeIDType	MaxTradeID;
	///本席位最大报单备拷
	TReturnCodeType	MaxOrderMessageReference;

bool operator==(const MDTraderOfferField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (TraderConnectStatus == ct.TraderConnectStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectRequestDate, ct.ConnectRequestDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectRequestTime, ct.ConnectRequestTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportDate, ct.LastReportDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LastReportTime, ct.LastReportTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectDate, ct.ConnectDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ConnectTime, ct.ConnectTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartDate, ct.StartDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StartTime, ct.StartTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxTradeID, ct.MaxTradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MaxOrderMessageReference, ct.MaxOrderMessageReference) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询行情报盘机
struct QryMDTraderOfferField
{
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///交易所交易员代码
	TTraderIDType	TraderID;

bool operator==(const QryMDTraderOfferField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询客户通知
struct QryNoticeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;

bool operator==(const QryNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///客户通知
struct NoticeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///消息正文
	TContentType	Content;
	///经纪公司通知内容序列号
	TSequenceLabelType	SequenceLabel;

bool operator==(const NoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Content, ct.Content) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SequenceLabel, ct.SequenceLabel) == 0);
	if (!ret) return ret;

	return ret;
}
};

///用户权限
struct UserRightField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///客户权限类型
	TUserRightTypeType	UserRightType;
	///是否禁止
	TBoolType	IsForbidden;

bool operator==(const UserRightField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (UserRightType == ct.UserRightType);
	if (!ret) return ret;
	ret = ret && (IsForbidden == ct.IsForbidden);
	if (!ret) return ret;

	return ret;
}
};

///查询结算信息确认域
struct QrySettlementInfoConfirmField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const QrySettlementInfoConfirmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///装载结算信息
struct LoadSettlementInfoField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;

bool operator==(const LoadSettlementInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///经纪公司可提资金算法表
struct BrokerWithdrawAlgorithmField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///可提资金算法
	TAlgorithmType	WithdrawAlgorithm;
	///资金使用率
	TRatioType	UsingRatio;
	///可提是否包含平仓盈利
	TIncludeCloseProfitType	IncludeCloseProfit;
	///本日无仓且无成交客户是否受可提比例限制
	TAllWithoutTradeType	AllWithoutTrade;
	///可用是否包含平仓盈利
	TIncludeCloseProfitType	AvailIncludeCloseProfit;
	///是否启用用户事件
	TBoolType	IsBrokerUserEvent;

bool operator==(const BrokerWithdrawAlgorithmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (WithdrawAlgorithm == ct.WithdrawAlgorithm);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UsingRatio, ct.UsingRatio));
	if (!ret) return ret;
	ret = ret && (IncludeCloseProfit == ct.IncludeCloseProfit);
	if (!ret) return ret;
	ret = ret && (AllWithoutTrade == ct.AllWithoutTrade);
	if (!ret) return ret;
	ret = ret && (AvailIncludeCloseProfit == ct.AvailIncludeCloseProfit);
	if (!ret) return ret;
	ret = ret && (IsBrokerUserEvent == ct.IsBrokerUserEvent);
	if (!ret) return ret;

	return ret;
}
};

///资金账户口令变更域
struct TradingAccountPasswordUpdateV1Field
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///原来的口令
	TPasswordType	OldPassword;
	///新的口令
	TPasswordType	NewPassword;

bool operator==(const TradingAccountPasswordUpdateV1Field &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OldPassword, ct.OldPassword) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(NewPassword, ct.NewPassword) == 0);
	if (!ret) return ret;

	return ret;
}
};

///资金账户口令变更域
struct TradingAccountPasswordUpdateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者帐号
	TAccountIDType	AccountID;
	///原来的口令
	TPasswordType	OldPassword;
	///新的口令
	TPasswordType	NewPassword;

bool operator==(const TradingAccountPasswordUpdateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OldPassword, ct.OldPassword) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(NewPassword, ct.NewPassword) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询组合合约分腿
struct QryCombinationLegField
{
	///组合合约代码
	TInstrumentIDType	CombInstrumentID;
	///单腿编号
	TLegIDType	LegID;
	///单腿合约代码
	TInstrumentIDType	LegInstrumentID;

bool operator==(const QryCombinationLegField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (LegID == ct.LegID);
	if (!ret) return ret;
	ret = ret && (strcmp(LegInstrumentID, ct.LegInstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询组合合约分腿
struct QrySyncStatusField
{
	///交易日
	TDateType	TradingDay;

bool operator==(const QrySyncStatusField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;

	return ret;
}
};

///组合交易合约的单腿
struct CombinationLegField
{
	///组合合约代码
	TInstrumentIDType	CombInstrumentID;
	///单腿编号
	TLegIDType	LegID;
	///单腿合约代码
	TInstrumentIDType	LegInstrumentID;
	///买卖方向
	TDirectionType	Direction;
	///单腿乘数
	TLegMultipleType	LegMultiple;
	///派生层数
	TImplyLevelType	ImplyLevel;

bool operator==(const CombinationLegField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (LegID == ct.LegID);
	if (!ret) return ret;
	ret = ret && (strcmp(LegInstrumentID, ct.LegInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (LegMultiple == ct.LegMultiple);
	if (!ret) return ret;
	ret = ret && (ImplyLevel == ct.ImplyLevel);
	if (!ret) return ret;

	return ret;
}
};

///数据同步状态
struct SyncStatusField
{
	///交易日
	TDateType	TradingDay;
	///数据同步状态
	TDataSyncStatusType	DataSyncStatus;

bool operator==(const SyncStatusField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (DataSyncStatus == ct.DataSyncStatus);
	if (!ret) return ret;

	return ret;
}
};

///查询联系人
struct QryLinkManField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const QryLinkManField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///联系人
struct LinkManField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///联系人类型
	TPersonTypeType	PersonType;
	///证件类型
	TIdCardTypeType	IdentifiedCardType;
	///证件号码
	TIdentifiedCardNoType	IdentifiedCardNo;
	///名称
	TPartyNameType	PersonName;
	///联系电话
	TTelephoneType	Telephone;
	///通讯地址
	TAddressType	Address;
	///邮政编码
	TZipCodeType	ZipCode;
	///优先级
	TPriorityType	Priority;

bool operator==(const LinkManField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PersonType == ct.PersonType);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(PersonName, ct.PersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ZipCode, ct.ZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (Priority == ct.Priority);
	if (!ret) return ret;

	return ret;
}
};

///查询经纪公司用户事件
struct QryBrokerUserEventField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///用户事件类型
	TUserEventTypeType	UserEventType;

bool operator==(const QryBrokerUserEventField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (UserEventType == ct.UserEventType);
	if (!ret) return ret;

	return ret;
}
};

///查询经纪公司用户事件
struct BrokerUserEventField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///用户事件类型
	TUserEventTypeType	UserEventType;
	///用户事件序号
	TSequenceNoType	EventSequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///用户事件信息
	TUserEventInfoType	UserEventInfo;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const BrokerUserEventField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (UserEventType == ct.UserEventType);
	if (!ret) return ret;
	ret = ret && (EventSequenceNo == ct.EventSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserEventInfo, ct.UserEventInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询签约银行请求
struct QryContractBankField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///银行代码
	TBankIDType	BankID;
	///银行分中心代码
	TBankBrchIDType	BankBrchID;

bool operator==(const QryContractBankField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询签约银行响应
struct ContractBankField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///银行代码
	TBankIDType	BankID;
	///银行分中心代码
	TBankBrchIDType	BankBrchID;
	///银行名称
	TBankNameType	BankName;

bool operator==(const ContractBankField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankID, ct.BankID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankBrchID, ct.BankBrchID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BankName, ct.BankName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者组合持仓明细
struct InvestorPositionCombineDetailField
{
	///交易日
	TDateType	TradingDay;
	///开仓日期
	TDateType	OpenDate;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///结算编号
	TSettlementIDType	SettlementID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///组合编号
	TTradeIDType	ComTradeID;
	///撮合编号
	TTradeIDType	TradeID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///买卖
	TDirectionType	Direction;
	///持仓量
	TVolumeType	TotalAmt;
	///投资者保证金
	TMoneyType	Margin;
	///交易所保证金
	TMoneyType	ExchMargin;
	///保证金率
	TRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TRatioType	MarginRateByVolume;
	///单腿编号
	TLegIDType	LegID;
	///单腿乘数
	TLegMultipleType	LegMultiple;
	///组合持仓合约编码
	TInstrumentIDType	CombInstrumentID;

bool operator==(const InvestorPositionCombineDetailField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ComTradeID, ct.ComTradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (TotalAmt == ct.TotalAmt);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Margin, ct.Margin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchMargin, ct.ExchMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;
	ret = ret && (LegID == ct.LegID);
	if (!ret) return ret;
	ret = ret && (LegMultiple == ct.LegMultiple);
	if (!ret) return ret;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///预埋单
struct ParkedOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///用户强评标志
	TBoolType	UserForceClose;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///预埋报单编号
	TParkedOrderIDType	ParkedOrderID;
	///用户类型
	TUserTypeType	UserType;
	///预埋单状态
	TParkedOrderStatusType	Status;
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const ParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///输入预埋单操作
struct ParkedOrderActionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///报单操作引用
	TOrderActionRefType	OrderActionRef;
	///报单引用
	TOrderRefType	OrderRef;
	///请求编号
	TRequestIDType	RequestID;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///操作标志
	TActionFlagType	ActionFlag;
	///价格
	TPriceType	LimitPrice;
	///数量变化
	TVolumeType	VolumeChange;
	///用户代码
	TUserIDType	UserID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///预埋撤单单编号
	TParkedOrderActionIDType	ParkedOrderActionID;
	///用户类型
	TUserTypeType	UserType;
	///预埋撤单状态
	TParkedOrderStatusType	Status;
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const ParkedOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionRef == ct.OrderActionRef);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderActionID, ct.ParkedOrderActionID) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询预埋单
struct QryParkedOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;

bool operator==(const QryParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询预埋撤单
struct QryParkedOrderActionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///交易所代码
	TExchangeIDType	ExchangeID;

bool operator==(const QryParkedOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///删除预埋单
struct RemoveParkedOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///预埋报单编号
	TParkedOrderIDType	ParkedOrderID;

bool operator==(const RemoveParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///删除预埋撤单
struct RemoveParkedOrderActionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///预埋撤单编号
	TParkedOrderActionIDType	ParkedOrderActionID;

bool operator==(const RemoveParkedOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderActionID, ct.ParkedOrderActionID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///经纪公司可提资金算法表
struct InvestorWithdrawAlgorithmField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///投资者代码
	TInvestorIDType	InvestorID;
	///可提资金比例
	TRatioType	UsingRatio;

bool operator==(const InvestorWithdrawAlgorithmField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UsingRatio, ct.UsingRatio));
	if (!ret) return ret;

	return ret;
}
};

///查询组合持仓明细
struct QryInvestorPositionCombineDetailField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///组合持仓合约编码
	TInstrumentIDType	CombInstrumentID;

bool operator==(const QryInvestorPositionCombineDetailField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CombInstrumentID, ct.CombInstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///成交均价
struct MarketDataAveragePriceField
{
	///当日均价
	TPriceType	AveragePrice;

bool operator==(const MarketDataAveragePriceField &ct) const
{
	bool ret = true;
	ret = ret && (EQUALDOUBLE(AveragePrice, ct.AveragePrice));
	if (!ret) return ret;

	return ret;
}
};

///校验投资者密码
struct VerifyInvestorPasswordField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///密码
	TPasswordType	Password;

bool operator==(const VerifyInvestorPasswordField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;

	return ret;
}
};

///用户IP
struct UserIPField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///IP地址
	TIPAddressType	IPAddress;
	///IP地址掩码
	TIPAddressType	IPMask;
	///Mac地址
	TMacAddressType	MacAddress;

bool operator==(const UserIPField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(IPAddress, ct.IPAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(IPMask, ct.IPMask) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(MacAddress, ct.MacAddress) == 0);
	if (!ret) return ret;

	return ret;
}
};

///用户事件通知信息
struct TradingNoticeInfoField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///发送时间
	TTimeType	SendTime;
	///消息正文
	TContentType	FieldContent;
	///序列系列号
	TSequenceSeriesType	SequenceSeries;
	///序列号
	TSequenceNoType	SequenceNo;

bool operator==(const TradingNoticeInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SendTime, ct.SendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FieldContent, ct.FieldContent) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceSeries == ct.SequenceSeries);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///用户事件通知
struct TradingNoticeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///投资者代码
	TInvestorIDType	InvestorID;
	///序列系列号
	TSequenceSeriesType	SequenceSeries;
	///用户代码
	TUserIDType	UserID;
	///发送时间
	TTimeType	SendTime;
	///序列号
	TSequenceNoType	SequenceNo;
	///消息正文
	TContentType	FieldContent;

bool operator==(const TradingNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceSeries == ct.SequenceSeries);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SendTime, ct.SendTime) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(FieldContent, ct.FieldContent) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询交易事件通知
struct QryTradingNoticeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const QryTradingNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询错误报单
struct QryErrOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const QryErrOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///错误报单
struct ErrOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///用户强评标志
	TBoolType	UserForceClose;
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const ErrOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询错误报单操作
struct QryErrOrderActionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const QryErrOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///错误报单操作
struct ErrOrderActionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///报单操作引用
	TOrderActionRefType	OrderActionRef;
	///报单引用
	TOrderRefType	OrderRef;
	///请求编号
	TRequestIDType	RequestID;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///操作标志
	TActionFlagType	ActionFlag;
	///价格
	TPriceType	LimitPrice;
	///数量变化
	TVolumeType	VolumeChange;
	///操作日期
	TDateType	ActionDate;
	///操作时间
	TTimeType	ActionTime;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///操作本地编号
	TOrderLocalIDType	ActionLocalID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///报单操作状态
	TOrderActionStatusType	OrderActionStatus;
	///用户代码
	TUserIDType	UserID;
	///状态信息
	TErrorMsgType	StatusMsg;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const ErrOrderActionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionRef == ct.OrderActionRef);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ActionFlag == ct.ActionFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeChange == ct.VolumeChange);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionDate, ct.ActionDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionTime, ct.ActionTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActionLocalID, ct.ActionLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (OrderActionStatus == ct.OrderActionStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者信息摘要
struct InvestorSumInfoField
{
	///投资者人数
	TVolumeType	Count;
	///MD5校验码1
	TMD5Type	md5_1;
	///MD5校验码2
	TMD5Type	md5_2;
	///MD5校验码3
	TMD5Type	md5_3;
	///MD5校验码4
	TMD5Type	md5_4;
	///MD5校验码5
	TMD5Type	md5_5;
	///MD5校验码6
	TMD5Type	md5_6;
	///MD5校验码7
	TMD5Type	md5_7;
	///MD5校验码8
	TMD5Type	md5_8;
	///MD5校验码9
	TMD5Type	md5_9;
	///MD5校验码10
	TMD5Type	md5_10;
	///MD5校验码11
	TMD5Type	md5_11;
	///MD5校验码12
	TMD5Type	md5_12;
	///MD5校验码13
	TMD5Type	md5_13;
	///MD5校验码14
	TMD5Type	md5_14;
	///MD5校验码15
	TMD5Type	md5_15;
	///MD5校验码16
	TMD5Type	md5_16;

bool operator==(const InvestorSumInfoField &ct) const
{
	bool ret = true;
	ret = ret && (Count == ct.Count);
	if (!ret) return ret;
	ret = ret && (md5_1 == ct.md5_1);
	if (!ret) return ret;
	ret = ret && (md5_2 == ct.md5_2);
	if (!ret) return ret;
	ret = ret && (md5_3 == ct.md5_3);
	if (!ret) return ret;
	ret = ret && (md5_4 == ct.md5_4);
	if (!ret) return ret;
	ret = ret && (md5_5 == ct.md5_5);
	if (!ret) return ret;
	ret = ret && (md5_6 == ct.md5_6);
	if (!ret) return ret;
	ret = ret && (md5_7 == ct.md5_7);
	if (!ret) return ret;
	ret = ret && (md5_8 == ct.md5_8);
	if (!ret) return ret;
	ret = ret && (md5_9 == ct.md5_9);
	if (!ret) return ret;
	ret = ret && (md5_10 == ct.md5_10);
	if (!ret) return ret;
	ret = ret && (md5_11 == ct.md5_11);
	if (!ret) return ret;
	ret = ret && (md5_12 == ct.md5_12);
	if (!ret) return ret;
	ret = ret && (md5_13 == ct.md5_13);
	if (!ret) return ret;
	ret = ret && (md5_14 == ct.md5_14);
	if (!ret) return ret;
	ret = ret && (md5_15 == ct.md5_15);
	if (!ret) return ret;
	ret = ret && (md5_16 == ct.md5_16);
	if (!ret) return ret;

	return ret;
}
};

///结算会话
struct SettlementSessionField
{
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;

bool operator==(const SettlementSessionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;

	return ret;
}
};

///用户登录请求
struct ReqRiskUserLoginField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///密码
	TPasswordType	Password;
	///客户端版本,20091230版Version=1,之前版本Version=0
	TVolumeType	Version;
	///本地前置中客户端连接的SessionID
	TSessionIDType	LocalSessionID;

bool operator==(const ReqRiskUserLoginField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Password, ct.Password) == 0);
	if (!ret) return ret;
	ret = ret && (Version == ct.Version);
	if (!ret) return ret;
	ret = ret && (LocalSessionID == ct.LocalSessionID);
	if (!ret) return ret;

	return ret;
}
};

///用户登录应答
struct RspRiskUserLoginField
{
	///交易日
	TDateType	TradingDay;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///客户端版本,20091230版Version=1,之前版本Version=0
	TVolumeType	Version;
	///前置类型
	TFrontTypeType	FrontType;

bool operator==(const RspRiskUserLoginField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (Version == ct.Version);
	if (!ret) return ret;
	ret = ret && (FrontType == ct.FrontType);
	if (!ret) return ret;

	return ret;
}
};

///特定范围投资者信息摘要请求
struct InvestorRangeSumInfoField
{
	///投资者起始下标
	TVolumeType	startindex;
	///投资者结束下标
	TVolumeType	endindex;
	///经纪公司代码
	TBrokerIDType	BrokerID;

bool operator==(const InvestorRangeSumInfoField &ct) const
{
	bool ret = true;
	ret = ret && (startindex == ct.startindex);
	if (!ret) return ret;
	ret = ret && (endindex == ct.endindex);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者资金查询请求
struct ReqInvestorAccountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///起始交易日期
	TDateType	TradingDayStart;
	///结束交易日期
	TDateType	TradingDayEnd;
	///起始投资者代码
	TInvestorIDType	InvestorIDStart;
	///结束投资者代码
	TInvestorIDType	InvestorIDEnd;
	///资金排序方法
	TAccountSortTypeType	SortType;
	///按排名数返回结果
	TVolumeType	ResultCount;
	///按比例返回结果
	TRatioType	ResultRatio;

bool operator==(const ReqInvestorAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayStart, ct.TradingDayStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayEnd, ct.TradingDayEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDStart, ct.InvestorIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (SortType == ct.SortType);
	if (!ret) return ret;
	ret = ret && (ResultCount == ct.ResultCount);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ResultRatio, ct.ResultRatio));
	if (!ret) return ret;

	return ret;
}
};

///投资者资金查询响应
struct InvestorRiskAccountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投资者帐号
	TAccountIDType	AccountID;
	///上次质押金额
	TMoneyType	PreMortgage;
	///上次信用额度
	TMoneyType	PreCredit;
	///上次存款额
	TMoneyType	PreDeposit;
	///上次总权益
	TMoneyType	PreBalance;
	///上次占用的保证金
	TMoneyType	PreMargin;
	///出入金金额
	TMoneyType	Withdraw;
	///当前保证金总额
	TMoneyType	CurrMargin;
	///手续费
	TMoneyType	Commission;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///持仓盈亏
	TMoneyType	PositionProfit;
	///总权益
	TMoneyType	Balance;
	///可用资金
	TMoneyType	Available;
	///质押金额
	TMoneyType	Mortgage;
	///交易所保证金
	TMoneyType	ExchangeMargin;
	///基本准备金
	TMoneyType	Reserve;
	///可取资金
	TMoneyType	WithdrawQuota;
	///信用额度
	TMoneyType	Credit;
	///上次交易所保证金
	TMoneyType	PreExchMargin;
	///历史强平次数
	TVolumeType	ForceCloseStat;
	///投资者交割保证金
	TMoneyType	DeliveryMargin;
	///交易所交割保证金
	TMoneyType	ExchangeDeliveryMargin;
	///保证金冻结
	TMoneyType	FrozenMargin;
	///手续费冻结
	TMoneyType	FrozenCommission;

bool operator==(const InvestorRiskAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMortgage, ct.PreMortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreCredit, ct.PreCredit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDeposit, ct.PreDeposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Mortgage, ct.Mortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WithdrawQuota, ct.WithdrawQuota));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Credit, ct.Credit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreExchMargin, ct.PreExchMargin));
	if (!ret) return ret;
	ret = ret && (ForceCloseStat == ct.ForceCloseStat);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(DeliveryMargin, ct.DeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeDeliveryMargin, ct.ExchangeDeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;

	return ret;
}
};

///投资者持仓查询请求
struct ReqInvestorPositionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///起始交易日期
	TDateType	TradingDayStart;
	///结束交易日期
	TDateType	TradingDayEnd;
	///起始投资者代码
	TInvestorIDType	InvestorIDStart;
	///结束投资者代码
	TInvestorIDType	InvestorIDEnd;
	///起始合约代码
	TInstrumentIDType	InstIDStart;
	///结束合约代码
	TInstrumentIDType	InstIDEnd;
	///起始产品代码
	TInstrumentIDType	ProductIDStart;
	///结束产品代码
	TInstrumentIDType	ProductIDEnd;
	///持仓排序方法
	TPositionSortTypeType	SortType;
	///按排名数返回结果
	TVolumeType	ResultCount;
	///按比例返回结果
	TRatioType	ResultRatio;

bool operator==(const ReqInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayStart, ct.TradingDayStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayEnd, ct.TradingDayEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDStart, ct.InvestorIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDStart, ct.InstIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDEnd, ct.InstIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDStart, ct.ProductIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDEnd, ct.ProductIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (SortType == ct.SortType);
	if (!ret) return ret;
	ret = ret && (ResultCount == ct.ResultCount);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ResultRatio, ct.ResultRatio));
	if (!ret) return ret;

	return ret;
}
};

///投资者持仓查询响应
struct RspInvestorPositionField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///持仓多空方向
	TPosiDirectionType	PosiDirection;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///持仓日期
	TPositionDateType	PositionDate;
	///上日持仓
	TVolumeType	YdPosition;
	///今日持仓
	TVolumeType	Position;
	///多头冻结
	TVolumeType	LongFrozen;
	///空头冻结
	TVolumeType	ShortFrozen;
	///开仓冻结金额
	TMoneyType	LongFrozenAmount;
	///开仓冻结金额
	TMoneyType	ShortFrozenAmount;
	///开仓量
	TVolumeType	OpenVolume;
	///平仓量
	TVolumeType	CloseVolume;
	///开仓金额
	TMoneyType	OpenAmount;
	///平仓金额
	TMoneyType	CloseAmount;
	///持仓成本
	TMoneyType	PositionCost;
	///上次占用的保证金
	TMoneyType	PreMargin;
	///占用的保证金
	TMoneyType	UseMargin;
	///冻结的保证金
	TMoneyType	FrozenMargin;
	///冻结的资金
	TMoneyType	FrozenCash;
	///冻结的手续费
	TMoneyType	FrozenCommission;
	///资金差额
	TMoneyType	CashIn;
	///手续费
	TMoneyType	Commission;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///持仓盈亏
	TMoneyType	PositionProfit;
	///上次结算价
	TPriceType	PreSettlementPrice;
	///本次结算价
	TPriceType	SettlementPrice;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///开仓成本
	TMoneyType	OpenCost;
	///交易所保证金
	TMoneyType	ExchangeMargin;
	///组合成交形成的持仓
	TVolumeType	CombPosition;
	///组合多头冻结
	TVolumeType	CombLongFrozen;
	///组合空头冻结
	TVolumeType	CombShortFrozen;
	///逐日盯市平仓盈亏
	TMoneyType	CloseProfitByDate;
	///逐笔对冲平仓盈亏
	TMoneyType	CloseProfitByTrade;
	///今日持仓
	TVolumeType	TodayPosition;
	///保证金率
	TRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TRatioType	MarginRateByVolume;

bool operator==(const RspInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongFrozen == ct.LongFrozen);
	if (!ret) return ret;
	ret = ret && (ShortFrozen == ct.ShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongFrozenAmount, ct.LongFrozenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortFrozenAmount, ct.ShortFrozenAmount));
	if (!ret) return ret;
	ret = ret && (OpenVolume == ct.OpenVolume);
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenAmount, ct.OpenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseMargin, ct.UseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenCost, ct.OpenCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (CombPosition == ct.CombPosition);
	if (!ret) return ret;
	ret = ret && (CombLongFrozen == ct.CombLongFrozen);
	if (!ret) return ret;
	ret = ret && (CombShortFrozen == ct.CombShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;

	return ret;
}
};

///投资者交易查询请求
struct ReqInvestorTradeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///起始交易日期
	TDateType	TradingDayStart;
	///结束交易日期
	TDateType	TradingDayEnd;
	///起始投资者代码
	TInvestorIDType	InvestorIDStart;
	///结束投资者代码
	TInvestorIDType	InvestorIDEnd;
	///起始合约代码
	TInstrumentIDType	InstIDStart;
	///结束合约代码
	TInstrumentIDType	InstIDEnd;
	///起始产品代码
	TInstrumentIDType	ProductIDStart;
	///结束产品代码
	TInstrumentIDType	ProductIDEnd;
	///交易排序方法
	TTradeSortTypeType	SortType;
	///按排名数返回结果
	TVolumeType	ResultCount;
	///按比例返回结果
	TRatioType	ResultRatio;

bool operator==(const ReqInvestorTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayStart, ct.TradingDayStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDayEnd, ct.TradingDayEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDStart, ct.InvestorIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDStart, ct.InstIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDEnd, ct.InstIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDStart, ct.ProductIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDEnd, ct.ProductIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (SortType == ct.SortType);
	if (!ret) return ret;
	ret = ret && (ResultCount == ct.ResultCount);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ResultRatio, ct.ResultRatio));
	if (!ret) return ret;

	return ret;
}
};

///投资者交易查询响应
struct RspInvestorTradeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///成交编号
	TTradeIDType	TradeID;
	///买卖方向
	TDirectionType	Direction;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///交易角色
	TTradingRoleType	TradingRole;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///开平标志
	TOffsetFlagType	OffsetFlag;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///价格
	TPriceType	Price;
	///数量
	TVolumeType	Volume;
	///成交时期
	TDateType	TradeDate;
	///成交时间
	TTimeType	TradeTime;
	///成交类型
	TTradeTypeType	TradeType;
	///成交价来源
	TPriceSourceType	PriceSource;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///结算会员编号
	TParticipantIDType	ClearingPartID;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///序号
	TSequenceNoType	SequenceNo;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///经纪公司报单编号
	TSequenceNoType	BrokerOrderSeq;
	///成交来源
	TTradeSourceType	TradeSource;

bool operator==(const RspInvestorTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRole == ct.TradingRole);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (PriceSource == ct.PriceSource);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (TradeSource == ct.TradeSource);
	if (!ret) return ret;

	return ret;
}
};

///投资者报单查询请求
struct ReqInvestorOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易日期
	TDateType	TradingDay;
	///起始投资者代码
	TInvestorIDType	InvestorIDStart;
	///结束投资者代码
	TInvestorIDType	InvestorIDEnd;
	///起始合约代码
	TInstrumentIDType	InstIDStart;
	///结束合约代码
	TInstrumentIDType	InstIDEnd;
	///起始产品代码
	TInstrumentIDType	ProductIDStart;
	///结束产品代码
	TInstrumentIDType	ProductIDEnd;

bool operator==(const ReqInvestorOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDStart, ct.InvestorIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDStart, ct.InstIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDEnd, ct.InstIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDStart, ct.ProductIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDEnd, ct.ProductIDEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者报单查询响应
struct RspInvestorOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///报单提交状态
	TOrderSubmitStatusType	OrderSubmitStatus;
	///报单提示序号
	TSequenceNoType	NotifySequence;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///报单来源
	TOrderSourceType	OrderSource;
	///报单状态
	TOrderStatusType	OrderStatus;
	///报单类型
	TOrderTypeType	OrderType;
	///今成交数量
	TVolumeType	VolumeTraded;
	///剩余数量
	TVolumeType	VolumeTotal;
	///报单日期
	TDateType	InsertDate;
	///委托时间
	TTimeType	InsertTime;
	///激活时间
	TTimeType	ActiveTime;
	///挂起时间
	TTimeType	SuspendTime;
	///最后修改时间
	TTimeType	UpdateTime;
	///撤销时间
	TTimeType	CancelTime;
	///最后修改交易所交易员代码
	TTraderIDType	ActiveTraderID;
	///结算会员编号
	TParticipantIDType	ClearingPartID;
	///序号
	TSequenceNoType	SequenceNo;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///用户端产品信息
	TProductInfoType	UserProductInfo;
	///状态信息
	TErrorMsgType	StatusMsg;
	///用户强评标志
	TBoolType	UserForceClose;
	///操作用户代码
	TUserIDType	ActiveUserID;
	///经纪公司报单编号
	TSequenceNoType	BrokerOrderSeq;
	///相关报单
	TOrderSysIDType	RelativeOrderSysID;
	///郑商所成交数量
	TVolumeType	ZCETotalTradedVolume;

bool operator==(const RspInvestorOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (NotifySequence == ct.NotifySequence);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderSource == ct.OrderSource);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (OrderType == ct.OrderType);
	if (!ret) return ret;
	ret = ret && (VolumeTraded == ct.VolumeTraded);
	if (!ret) return ret;
	ret = ret && (VolumeTotal == ct.VolumeTotal);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertDate, ct.InsertDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTime, ct.InsertTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTime, ct.ActiveTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SuspendTime, ct.SuspendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CancelTime, ct.CancelTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTraderID, ct.ActiveTraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveUserID, ct.ActiveUserID) == 0);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (strcmp(RelativeOrderSysID, ct.RelativeOrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ZCETotalTradedVolume == ct.ZCETotalTradedVolume);
	if (!ret) return ret;

	return ret;
}
};

///投资者持仓统计查询响应
struct InvestorPositionStaticField
{
	///产品代码
	TInstrumentIDType	ProductID;
	///合约编号
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///昨持仓
	TVolumeType	YdPosition;
	///总持仓
	TVolumeType	Position;
	///多头持仓
	TVolumeType	LongPosition;
	///空头持仓
	TVolumeType	ShortPosition;
	///净持仓
	TVolumeType	NetPosition;
	///投机多头持仓
	TVolumeType	SpecuLongPosi;
	///投机空头持仓
	TVolumeType	SpecuShortPosi;
	///保值多头持仓
	TVolumeType	HedgeLongPosi;
	///保值空头持仓
	TVolumeType	HedgeShortPosi;
	///今仓
	TVolumeType	TodayPosition;
	///总持仓成本
	TMoneyType	PositionCost;

bool operator==(const InvestorPositionStaticField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongPosition == ct.LongPosition);
	if (!ret) return ret;
	ret = ret && (ShortPosition == ct.ShortPosition);
	if (!ret) return ret;
	ret = ret && (NetPosition == ct.NetPosition);
	if (!ret) return ret;
	ret = ret && (SpecuLongPosi == ct.SpecuLongPosi);
	if (!ret) return ret;
	ret = ret && (SpecuShortPosi == ct.SpecuShortPosi);
	if (!ret) return ret;
	ret = ret && (HedgeLongPosi == ct.HedgeLongPosi);
	if (!ret) return ret;
	ret = ret && (HedgeShortPosi == ct.HedgeShortPosi);
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;

	return ret;
}
};

///投资者成交统计查询响应
struct InvestorTradeStaticField
{
	///产品代码
	TInstrumentIDType	ProductID;
	///合约编号
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///总成交量
	TVolumeType	TotalVol;
	///总成交额
	TMoneyType	TotalAmt;
	///买成交量
	TVolumeType	BuyVol;
	///买成交额
	TMoneyType	BuyAmt;
	///卖成交量
	TVolumeType	SellVol;
	///卖成交额
	TMoneyType	SellAmt;
	///净买入成交量
	TVolumeType	NetVol;
	///净买入成交额
	TMoneyType	NetAmt;

bool operator==(const InvestorTradeStaticField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TotalVol == ct.TotalVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TotalAmt, ct.TotalAmt));
	if (!ret) return ret;
	ret = ret && (BuyVol == ct.BuyVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BuyAmt, ct.BuyAmt));
	if (!ret) return ret;
	ret = ret && (SellVol == ct.SellVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SellAmt, ct.SellAmt));
	if (!ret) return ret;
	ret = ret && (NetVol == ct.NetVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NetAmt, ct.NetAmt));
	if (!ret) return ret;

	return ret;
}
};

///订阅实时行情
struct SubMarketDataField
{
	///合约编号
	TInstrumentIDType	InstrumentID;

bool operator==(const SubMarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///合约持仓比例查询
struct QryInstPositionRateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///起始合约代码
	TInstrumentIDType	InstIDStart;
	///结束合约代码
	TInstrumentIDType	InstIDEnd;
	///散户保值买入持仓量定义
	TLargeVolumeType	hbtotal_little;
	///中户保值买入持仓量定义
	TLargeVolumeType	hbtotal_medium;
	///散户保值卖出持仓量定义
	TLargeVolumeType	hstotal_little;
	///中户保值卖出持仓量定义
	TLargeVolumeType	hstotal_medium;
	///散户保值持仓量定义
	TLargeVolumeType	htotal_little;
	///中户保值持仓量定义
	TLargeVolumeType	htotal_medium;
	///散户投机买入持仓量定义
	TLargeVolumeType	sbtotal_little;
	///中户投机买入持仓量定义
	TLargeVolumeType	sbtotal_medium;
	///散户投机卖出持仓量定义
	TLargeVolumeType	sstotal_little;
	///中户投机卖出持仓量定义
	TLargeVolumeType	sstotal_medium;
	///散户投机持仓量定义
	TLargeVolumeType	stotal_little;
	///中户投机持仓量定义
	TLargeVolumeType	stotal_medium;
	///散户买入持仓量定义
	TLargeVolumeType	buytotal_little;
	///中户买入持仓量定义
	TLargeVolumeType	buytotal_medium;
	///散户卖出持仓量定义
	TLargeVolumeType	selltotal_little;
	///中户卖出持仓量定义
	TLargeVolumeType	selltotal_medium;
	///散户总持仓量定义
	TLargeVolumeType	total_little;
	///中户总持仓量定义
	TLargeVolumeType	total_medium;
	///取值方式
	TValueModeType	ValueMode;

bool operator==(const QryInstPositionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDStart, ct.InstIDStart) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstIDEnd, ct.InstIDEnd) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hbtotal_little, ct.hbtotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hbtotal_medium, ct.hbtotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hstotal_little, ct.hstotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hstotal_medium, ct.hstotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(htotal_little, ct.htotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(htotal_medium, ct.htotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sbtotal_little, ct.sbtotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sbtotal_medium, ct.sbtotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sstotal_little, ct.sstotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sstotal_medium, ct.sstotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(stotal_little, ct.stotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(stotal_medium, ct.stotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(buytotal_little, ct.buytotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(buytotal_medium, ct.buytotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(selltotal_little, ct.selltotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(selltotal_medium, ct.selltotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(total_little, ct.total_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(total_medium, ct.total_medium));
	if (!ret) return ret;
	ret = ret && (ValueMode == ct.ValueMode);
	if (!ret) return ret;

	return ret;
}
};

///合约持仓比例应答
struct RspInstPositionRateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///散户保值买入持仓量
	TVolumeType	hbtotal_amt_little;
	///中户保值买入持仓量
	TVolumeType	hbtotal_amt_medium;
	///大户保值买入持仓量
	TVolumeType	hbtotal_amt_large;
	///散户保值卖出持仓量
	TVolumeType	hstotal_amt_little;
	///中户保值卖出持仓量
	TVolumeType	hstotal_amt_medium;
	///大户保值卖出持仓量
	TVolumeType	hstotal_amt_large;
	///散户保值持仓量
	TVolumeType	htotal_amt_little;
	///中户保值持仓量
	TVolumeType	htotal_amt_medium;
	///大户保值持仓量
	TVolumeType	htotal_amt_large;
	///散户投机买入持仓量
	TVolumeType	sbtotal_amt_little;
	///中户投机买入持仓量
	TVolumeType	sbtotal_amt_medium;
	///大户投机买入持仓量
	TVolumeType	sbtotal_amt_large;
	///散户投机卖出持仓量
	TVolumeType	sstotal_amt_little;
	///中户投机卖出持仓量
	TVolumeType	sstotal_amt_medium;
	///大户投机卖出持仓量
	TVolumeType	sstotal_amt_large;
	///散户投机持仓量
	TVolumeType	stotal_amt_little;
	///中户投机持仓量
	TVolumeType	stotal_amt_medium;
	///大户投机持仓量
	TVolumeType	stotal_amt_large;
	///散户买入持仓量
	TVolumeType	buytotal_amt_little;
	///中户买入持仓量
	TVolumeType	buytotal_amt_medium;
	///大户买入持仓量
	TVolumeType	buytotal_amt_large;
	///散户卖出持仓量
	TVolumeType	selltotal_amt_little;
	///中户卖出持仓量
	TVolumeType	selltotal_amt_medium;
	///大户卖出持仓量
	TVolumeType	selltotal_amt_large;
	///散户总持仓量
	TVolumeType	total_amt_little;
	///中户总持仓量
	TVolumeType	total_amt_medium;
	///大户总持仓量
	TVolumeType	total_amt_large;

bool operator==(const RspInstPositionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_little == ct.hbtotal_amt_little);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_medium == ct.hbtotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_large == ct.hbtotal_amt_large);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_little == ct.hstotal_amt_little);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_medium == ct.hstotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_large == ct.hstotal_amt_large);
	if (!ret) return ret;
	ret = ret && (htotal_amt_little == ct.htotal_amt_little);
	if (!ret) return ret;
	ret = ret && (htotal_amt_medium == ct.htotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (htotal_amt_large == ct.htotal_amt_large);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_little == ct.sbtotal_amt_little);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_medium == ct.sbtotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_large == ct.sbtotal_amt_large);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_little == ct.sstotal_amt_little);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_medium == ct.sstotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_large == ct.sstotal_amt_large);
	if (!ret) return ret;
	ret = ret && (stotal_amt_little == ct.stotal_amt_little);
	if (!ret) return ret;
	ret = ret && (stotal_amt_medium == ct.stotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (stotal_amt_large == ct.stotal_amt_large);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_little == ct.buytotal_amt_little);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_medium == ct.buytotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_large == ct.buytotal_amt_large);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_little == ct.selltotal_amt_little);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_medium == ct.selltotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_large == ct.selltotal_amt_large);
	if (!ret) return ret;
	ret = ret && (total_amt_little == ct.total_amt_little);
	if (!ret) return ret;
	ret = ret && (total_amt_medium == ct.total_amt_medium);
	if (!ret) return ret;
	ret = ret && (total_amt_large == ct.total_amt_large);
	if (!ret) return ret;

	return ret;
}
};

///产品持仓比例查询
struct QryProductPositionRateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///产品代码
	TInstrumentIDType	ProductID;
	///散户保值买入持仓量定义
	TLargeVolumeType	hbtotal_little;
	///中户保值买入持仓量定义
	TLargeVolumeType	hbtotal_medium;
	///散户保值卖出持仓量定义
	TLargeVolumeType	hstotal_little;
	///中户保值卖出持仓量定义
	TLargeVolumeType	hstotal_medium;
	///散户保值持仓量定义
	TLargeVolumeType	htotal_little;
	///中户保值持仓量定义
	TLargeVolumeType	htotal_medium;
	///散户投机买入持仓量定义
	TLargeVolumeType	sbtotal_little;
	///中户投机买入持仓量定义
	TLargeVolumeType	sbtotal_medium;
	///散户投机卖出持仓量定义
	TLargeVolumeType	sstotal_little;
	///中户投机卖出持仓量定义
	TLargeVolumeType	sstotal_medium;
	///散户投机持仓量定义
	TLargeVolumeType	stotal_little;
	///中户投机持仓量定义
	TLargeVolumeType	stotal_medium;
	///散户买入持仓量定义
	TLargeVolumeType	buytotal_little;
	///中户买入持仓量定义
	TLargeVolumeType	buytotal_medium;
	///散户卖出持仓量定义
	TLargeVolumeType	selltotal_little;
	///中户卖出持仓量定义
	TLargeVolumeType	selltotal_medium;
	///散户总持仓量定义
	TLargeVolumeType	total_little;
	///中户总持仓量定义
	TLargeVolumeType	total_medium;
	///取值方式
	TValueModeType	ValueMode;

bool operator==(const QryProductPositionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hbtotal_little, ct.hbtotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hbtotal_medium, ct.hbtotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hstotal_little, ct.hstotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(hstotal_medium, ct.hstotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(htotal_little, ct.htotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(htotal_medium, ct.htotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sbtotal_little, ct.sbtotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sbtotal_medium, ct.sbtotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sstotal_little, ct.sstotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(sstotal_medium, ct.sstotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(stotal_little, ct.stotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(stotal_medium, ct.stotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(buytotal_little, ct.buytotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(buytotal_medium, ct.buytotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(selltotal_little, ct.selltotal_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(selltotal_medium, ct.selltotal_medium));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(total_little, ct.total_little));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(total_medium, ct.total_medium));
	if (!ret) return ret;
	ret = ret && (ValueMode == ct.ValueMode);
	if (!ret) return ret;

	return ret;
}
};

///产品持仓比例
struct RspProductPositionRateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///产品代码
	TInstrumentIDType	ProductID;
	///散户保值买入持仓量
	TVolumeType	hbtotal_amt_little;
	///中户保值买入持仓量
	TVolumeType	hbtotal_amt_medium;
	///大户保值买入持仓量
	TVolumeType	hbtotal_amt_large;
	///散户保值卖出持仓量
	TVolumeType	hstotal_amt_little;
	///中户保值卖出持仓量
	TVolumeType	hstotal_amt_medium;
	///大户保值卖出持仓量
	TVolumeType	hstotal_amt_large;
	///散户保值持仓量
	TVolumeType	htotal_amt_little;
	///中户保值持仓量
	TVolumeType	htotal_amt_medium;
	///大户保值持仓量
	TVolumeType	htotal_amt_large;
	///散户投机买入持仓量
	TVolumeType	sbtotal_amt_little;
	///中户投机买入持仓量
	TVolumeType	sbtotal_amt_medium;
	///大户投机买入持仓量
	TVolumeType	sbtotal_amt_large;
	///散户投机卖出持仓量
	TVolumeType	sstotal_amt_little;
	///中户投机卖出持仓量
	TVolumeType	sstotal_amt_medium;
	///大户投机卖出持仓量
	TVolumeType	sstotal_amt_large;
	///散户投机持仓量
	TVolumeType	stotal_amt_little;
	///中户投机持仓量
	TVolumeType	stotal_amt_medium;
	///大户投机持仓量
	TVolumeType	stotal_amt_large;
	///散户买入持仓量
	TVolumeType	buytotal_amt_little;
	///中户买入持仓量
	TVolumeType	buytotal_amt_medium;
	///大户买入持仓量
	TVolumeType	buytotal_amt_large;
	///散户卖出持仓量
	TVolumeType	selltotal_amt_little;
	///中户卖出持仓量
	TVolumeType	selltotal_amt_medium;
	///大户卖出持仓量
	TVolumeType	selltotal_amt_large;
	///散户总持仓量
	TVolumeType	total_amt_little;
	///中户总持仓量
	TVolumeType	total_amt_medium;
	///大户总持仓量
	TVolumeType	total_amt_large;

bool operator==(const RspProductPositionRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_little == ct.hbtotal_amt_little);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_medium == ct.hbtotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (hbtotal_amt_large == ct.hbtotal_amt_large);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_little == ct.hstotal_amt_little);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_medium == ct.hstotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (hstotal_amt_large == ct.hstotal_amt_large);
	if (!ret) return ret;
	ret = ret && (htotal_amt_little == ct.htotal_amt_little);
	if (!ret) return ret;
	ret = ret && (htotal_amt_medium == ct.htotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (htotal_amt_large == ct.htotal_amt_large);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_little == ct.sbtotal_amt_little);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_medium == ct.sbtotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (sbtotal_amt_large == ct.sbtotal_amt_large);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_little == ct.sstotal_amt_little);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_medium == ct.sstotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (sstotal_amt_large == ct.sstotal_amt_large);
	if (!ret) return ret;
	ret = ret && (stotal_amt_little == ct.stotal_amt_little);
	if (!ret) return ret;
	ret = ret && (stotal_amt_medium == ct.stotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (stotal_amt_large == ct.stotal_amt_large);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_little == ct.buytotal_amt_little);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_medium == ct.buytotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (buytotal_amt_large == ct.buytotal_amt_large);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_little == ct.selltotal_amt_little);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_medium == ct.selltotal_amt_medium);
	if (!ret) return ret;
	ret = ret && (selltotal_amt_large == ct.selltotal_amt_large);
	if (!ret) return ret;
	ret = ret && (total_amt_little == ct.total_amt_little);
	if (!ret) return ret;
	ret = ret && (total_amt_medium == ct.total_amt_medium);
	if (!ret) return ret;
	ret = ret && (total_amt_large == ct.total_amt_large);
	if (!ret) return ret;

	return ret;
}
};

///历史模拟法压力测试只返回一次结果
struct IsWriteOnceField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///是否只返回一次结果
	TBoolType	IsWriteOnce;

bool operator==(const IsWriteOnceField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (IsWriteOnce == ct.IsWriteOnce);
	if (!ret) return ret;

	return ret;
}
};

///压力测试结算价格(或浮动)
struct STSettlePriceField
{
	///第几天
	TDayNumberType	Day;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///结算价格类型(Day=1时有效)
	TSTPriceTypeType	PriceType;
	///结算价格(或浮动)
	TPriceType	Price;

bool operator==(const STSettlePriceField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (PriceType == ct.PriceType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;

	return ret;
}
};

///压力测试投资者保证金率
struct STInstrumentMarginRateField
{
	///第几天
	TDayNumberType	Day;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;
	///是否相对交易所收取
	TBoolType	IsRelative;

bool operator==(const STInstrumentMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///压力测试投资者保证金率调整
struct STInstrumentMarginRateAdjustField
{
	///第几天
	TDayNumberType	Day;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;
	///是否相对交易所收取
	TBoolType	IsRelative;

bool operator==(const STInstrumentMarginRateAdjustField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (IsRelative == ct.IsRelative);
	if (!ret) return ret;

	return ret;
}
};

///压力测试交易所保证金率
struct STExchangeMarginRateField
{
	///第几天
	TDayNumberType	Day;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;

bool operator==(const STExchangeMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///压力测试交易所保证金率调整
struct STExchangeMarginRateAdjustField
{
	///第几天
	TDayNumberType	Day;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///跟随交易所投资者多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///跟随交易所投资者多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///跟随交易所投资者空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///跟随交易所投资者空头保证金费
	TMoneyType	ShortMarginRatioByVolume;
	///交易所多头保证金率
	TRatioType	ExchLongMarginRatioByMoney;
	///交易所多头保证金费
	TMoneyType	ExchLongMarginRatioByVolume;
	///交易所空头保证金率
	TRatioType	ExchShortMarginRatioByMoney;
	///交易所空头保证金费
	TMoneyType	ExchShortMarginRatioByVolume;
	///不跟随交易所投资者多头保证金率
	TRatioType	NoLongMarginRatioByMoney;
	///不跟随交易所投资者多头保证金费
	TMoneyType	NoLongMarginRatioByVolume;
	///不跟随交易所投资者空头保证金率
	TRatioType	NoShortMarginRatioByMoney;
	///不跟随交易所投资者空头保证金费
	TMoneyType	NoShortMarginRatioByVolume;

bool operator==(const STExchangeMarginRateAdjustField &ct) const
{
	bool ret = true;
	ret = ret && (Day == ct.Day);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchLongMarginRatioByMoney, ct.ExchLongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchLongMarginRatioByVolume, ct.ExchLongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchShortMarginRatioByMoney, ct.ExchShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchShortMarginRatioByVolume, ct.ExchShortMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoLongMarginRatioByMoney, ct.NoLongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoLongMarginRatioByVolume, ct.NoLongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoShortMarginRatioByMoney, ct.NoShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NoShortMarginRatioByVolume, ct.NoShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///投资者与联系人信息
struct InvestorLinkManField
{
	///投资者代码
	TInvestorIDType	InvestorID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者分组代码
	TInvestorIDType	InvestorGroupID;
	///投资者名称
	TPartyNameType	InvestorName;
	///证件类型
	TIdCardTypeType	IdentifiedCardType;
	///证件号码
	TIdentifiedCardNoType	IdentifiedCardNo;
	///是否活跃
	TBoolType	IsActive;
	///联系电话
	TTelephoneType	Telephone;
	///通讯地址
	TAddressType	Address;
	///指定下单人证件类型
	TIdCardTypeType	OrderManIdentifiedCardType;
	///指定下单人证件号码
	TIdentifiedCardNoType	OrderManIdentifiedCardNo;
	///指定下单人名称
	TPartyNameType	OrderManPersonName;
	///指定下单人联系电话
	TTelephoneType	OrderManTelephone;
	///指定下单人通讯地址
	TAddressType	OrderManAddress;
	///指定下单人邮政编码
	TZipCodeType	OrderManZipCode;
	///资金调拨人证件类型
	TIdCardTypeType	FundManIdentifiedCardType;
	///资金调拨人证件号码
	TIdentifiedCardNoType	FundManIdentifiedCardNo;
	///资金调拨人名称
	TPartyNameType	FundManPersonName;
	///资金调拨人联系电话
	TTelephoneType	FundManTelephone;
	///资金调拨人通讯地址
	TAddressType	FundManAddress;
	///资金调拨人邮政编码
	TZipCodeType	FundManZipCode;
	///开户授权人证件类型
	TIdCardTypeType	OpenManIdentifiedCardType;
	///开户授权人证件号码
	TIdentifiedCardNoType	OpenManIdentifiedCardNo;
	///开户授权人名称
	TPartyNameType	OpenManPersonName;
	///开户授权人联系电话
	TTelephoneType	OpenManTelephone;
	///开户授权人通讯地址
	TAddressType	OpenManAddress;
	///开户授权人邮政编码
	TZipCodeType	OpenManZipCode;
	///结算单确认人证件类型
	TIdCardTypeType	SettlementManIdentifiedCardType;
	///结算单确认人证件号码
	TIdentifiedCardNoType	SettlementManIdentifiedCardNo;
	///结算单确认人名称
	TPartyNameType	SettlementManPersonName;
	///结算单确认人联系电话
	TTelephoneType	SettlementManTelephone;
	///结算单确认人通讯地址
	TAddressType	SettlementManAddress;
	///结算单确认人邮政编码
	TZipCodeType	SettlementManZipCode;
	///开户日期
	TDateType	OpenDate;
	///手机
	TMobileType	Mobile;
	///电子邮件
	TEMailType	EMail;
	///投资者类型
	TInvestorTypeType	InvestorType;
	///国家代码
	TUOACountryCodeType	PhoneCountryCode;
	///区号
	TAreaCodeType	PhoneAreaCode;
	///开户授权人国家代码
	TUOACountryCodeType	OpenPhoneCountryCode;
	///开户授权人区号
	TAreaCodeType	OpenPhoneAreaCode;
	///指定下单人国家代码
	TUOACountryCodeType	OrderPhoneCountryCode;
	///指定下单人区号
	TAreaCodeType	OrderPhoneAreaCode;
	///资金调拨人国家代码
	TUOACountryCodeType	FundPhoneCountryCode;
	///资金调拨人区号
	TAreaCodeType	FundPhoneAreaCode;
	///结算单确认人国家代码
	TUOACountryCodeType	SettlePhoneCountryCode;
	///结算单确认人区号
	TAreaCodeType	SettlePhoneAreaCode;

bool operator==(const InvestorLinkManField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorName, ct.InvestorName) == 0);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (OrderManIdentifiedCardType == ct.OrderManIdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManIdentifiedCardNo, ct.OrderManIdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManPersonName, ct.OrderManPersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManTelephone, ct.OrderManTelephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManAddress, ct.OrderManAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderManZipCode, ct.OrderManZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (FundManIdentifiedCardType == ct.FundManIdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManIdentifiedCardNo, ct.FundManIdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManPersonName, ct.FundManPersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManTelephone, ct.FundManTelephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManAddress, ct.FundManAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundManZipCode, ct.FundManZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (OpenManIdentifiedCardType == ct.OpenManIdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManIdentifiedCardNo, ct.OpenManIdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManPersonName, ct.OpenManPersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManTelephone, ct.OpenManTelephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManAddress, ct.OpenManAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenManZipCode, ct.OpenManZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementManIdentifiedCardType == ct.SettlementManIdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManIdentifiedCardNo, ct.SettlementManIdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManPersonName, ct.SettlementManPersonName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManTelephone, ct.SettlementManTelephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManAddress, ct.SettlementManAddress) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlementManZipCode, ct.SettlementManZipCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EMail, ct.EMail) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorType == ct.InvestorType);
	if (!ret) return ret;
	ret = ret && (strcmp(PhoneCountryCode, ct.PhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(PhoneAreaCode, ct.PhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenPhoneCountryCode, ct.OpenPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenPhoneAreaCode, ct.OpenPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderPhoneCountryCode, ct.OrderPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderPhoneAreaCode, ct.OrderPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundPhoneCountryCode, ct.FundPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundPhoneAreaCode, ct.FundPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlePhoneCountryCode, ct.SettlePhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlePhoneAreaCode, ct.SettlePhoneAreaCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///订阅投资者成交
struct SubInvestorTradeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const SubInvestorTradeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///订阅投资者持仓
struct SubInvestorPositionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const SubInvestorPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///序列号
struct NotifySequenceField
{
	///序号
	TSequenceNoType	SequenceNo;

bool operator==(const NotifySequenceField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///有序的成交
struct SequencialTradeField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///成交编号
	TTradeIDType	TradeID;
	///买卖方向
	TDirectionType	Direction;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///交易角色
	TTradingRoleType	TradingRole;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///开平标志
	TOffsetFlagType	OffsetFlag;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///价格
	TPriceType	Price;
	///数量
	TVolumeType	Volume;
	///成交时期
	TDateType	TradeDate;
	///成交时间
	TTimeType	TradeTime;
	///成交类型
	TTradeTypeType	TradeType;
	///成交价来源
	TPriceSourceType	PriceSource;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///结算会员编号
	TParticipantIDType	ClearingPartID;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///序号
	TSequenceNoType	SequenceNo;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///经纪公司报单编号
	TSequenceNoType	BrokerOrderSeq;
	///成交来源
	TTradeSourceType	TradeSource;

bool operator==(const SequencialTradeField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeID, ct.TradeID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (TradingRole == ct.TradingRole);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (OffsetFlag == ct.OffsetFlag);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;
	ret = ret && (Volume == ct.Volume);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeDate, ct.TradeDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeTime, ct.TradeTime) == 0);
	if (!ret) return ret;
	ret = ret && (TradeType == ct.TradeType);
	if (!ret) return ret;
	ret = ret && (PriceSource == ct.PriceSource);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (TradeSource == ct.TradeSource);
	if (!ret) return ret;

	return ret;
}
};

///有序的报单
struct SequencialOrderField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///本地报单编号
	TOrderLocalIDType	OrderLocalID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///会员代码
	TParticipantIDType	ParticipantID;
	///客户代码
	TClientIDType	ClientID;
	///合约在交易所的代码
	TExchangeInstIDType	ExchangeInstID;
	///交易所交易员代码
	TTraderIDType	TraderID;
	///安装编号
	TInstallIDType	InstallID;
	///报单提交状态
	TOrderSubmitStatusType	OrderSubmitStatus;
	///报单提示序号
	TSequenceNoType	NotifySequence;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///报单编号
	TOrderSysIDType	OrderSysID;
	///报单来源
	TOrderSourceType	OrderSource;
	///报单状态
	TOrderStatusType	OrderStatus;
	///报单类型
	TOrderTypeType	OrderType;
	///今成交数量
	TVolumeType	VolumeTraded;
	///剩余数量
	TVolumeType	VolumeTotal;
	///报单日期
	TDateType	InsertDate;
	///委托时间
	TTimeType	InsertTime;
	///激活时间
	TTimeType	ActiveTime;
	///挂起时间
	TTimeType	SuspendTime;
	///最后修改时间
	TTimeType	UpdateTime;
	///撤销时间
	TTimeType	CancelTime;
	///最后修改交易所交易员代码
	TTraderIDType	ActiveTraderID;
	///结算会员编号
	TParticipantIDType	ClearingPartID;
	///序号
	TSequenceNoType	SequenceNo;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;
	///用户端产品信息
	TProductInfoType	UserProductInfo;
	///状态信息
	TErrorMsgType	StatusMsg;
	///用户强评标志
	TBoolType	UserForceClose;
	///操作用户代码
	TUserIDType	ActiveUserID;
	///经纪公司报单编号
	TSequenceNoType	BrokerOrderSeq;
	///相关报单
	TOrderSysIDType	RelativeOrderSysID;
	///郑商所成交数量
	TVolumeType	ZCETotalTradedVolume;

bool operator==(const SequencialOrderField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderLocalID, ct.OrderLocalID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParticipantID, ct.ParticipantID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeInstID, ct.ExchangeInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(TraderID, ct.TraderID) == 0);
	if (!ret) return ret;
	ret = ret && (InstallID == ct.InstallID);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (NotifySequence == ct.NotifySequence);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderSysID, ct.OrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderSource == ct.OrderSource);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (OrderType == ct.OrderType);
	if (!ret) return ret;
	ret = ret && (VolumeTraded == ct.VolumeTraded);
	if (!ret) return ret;
	ret = ret && (VolumeTotal == ct.VolumeTotal);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertDate, ct.InsertDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InsertTime, ct.InsertTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTime, ct.ActiveTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SuspendTime, ct.SuspendTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UpdateTime, ct.UpdateTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(CancelTime, ct.CancelTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveTraderID, ct.ActiveTraderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClearingPartID, ct.ClearingPartID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (strcmp(UserProductInfo, ct.UserProductInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (strcmp(ActiveUserID, ct.ActiveUserID) == 0);
	if (!ret) return ret;
	ret = ret && (BrokerOrderSeq == ct.BrokerOrderSeq);
	if (!ret) return ret;
	ret = ret && (strcmp(RelativeOrderSysID, ct.RelativeOrderSysID) == 0);
	if (!ret) return ret;
	ret = ret && (ZCETotalTradedVolume == ct.ZCETotalTradedVolume);
	if (!ret) return ret;

	return ret;
}
};

///有序的经纪公司用户事件
struct SequencialBrokerUserEventField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///用户事件类型
	TUserEventTypeType	UserEventType;
	///用户事件序号
	TSequenceNoType	EventSequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///用户事件信息
	TUserEventInfoType	UserEventInfo;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;

bool operator==(const SequencialBrokerUserEventField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (UserEventType == ct.UserEventType);
	if (!ret) return ret;
	ret = ret && (EventSequenceNo == ct.EventSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserEventInfo, ct.UserEventInfo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///有序的持仓
struct SequencialPositionField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///持仓多空方向
	TPosiDirectionType	PosiDirection;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///持仓日期
	TPositionDateType	PositionDate;
	///上日持仓
	TVolumeType	YdPosition;
	///今日持仓
	TVolumeType	Position;
	///多头冻结
	TVolumeType	LongFrozen;
	///空头冻结
	TVolumeType	ShortFrozen;
	///开仓冻结金额
	TMoneyType	LongFrozenAmount;
	///开仓冻结金额
	TMoneyType	ShortFrozenAmount;
	///开仓量
	TVolumeType	OpenVolume;
	///平仓量
	TVolumeType	CloseVolume;
	///开仓金额
	TMoneyType	OpenAmount;
	///平仓金额
	TMoneyType	CloseAmount;
	///持仓成本
	TMoneyType	PositionCost;
	///上次占用的保证金
	TMoneyType	PreMargin;
	///占用的保证金
	TMoneyType	UseMargin;
	///冻结的保证金
	TMoneyType	FrozenMargin;
	///冻结的资金
	TMoneyType	FrozenCash;
	///冻结的手续费
	TMoneyType	FrozenCommission;
	///资金差额
	TMoneyType	CashIn;
	///手续费
	TMoneyType	Commission;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///持仓盈亏
	TMoneyType	PositionProfit;
	///上次结算价
	TPriceType	PreSettlementPrice;
	///本次结算价
	TPriceType	SettlementPrice;
	///交易日
	TDateType	TradingDay;
	///结算编号
	TSettlementIDType	SettlementID;
	///开仓成本
	TMoneyType	OpenCost;
	///交易所保证金
	TMoneyType	ExchangeMargin;
	///组合成交形成的持仓
	TVolumeType	CombPosition;
	///组合多头冻结
	TVolumeType	CombLongFrozen;
	///组合空头冻结
	TVolumeType	CombShortFrozen;
	///逐日盯市平仓盈亏
	TMoneyType	CloseProfitByDate;
	///逐笔对冲平仓盈亏
	TMoneyType	CloseProfitByTrade;
	///今日持仓
	TVolumeType	TodayPosition;
	///保证金率
	TRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TRatioType	MarginRateByVolume;

bool operator==(const SequencialPositionField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongFrozen == ct.LongFrozen);
	if (!ret) return ret;
	ret = ret && (ShortFrozen == ct.ShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongFrozenAmount, ct.LongFrozenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortFrozenAmount, ct.ShortFrozenAmount));
	if (!ret) return ret;
	ret = ret && (OpenVolume == ct.OpenVolume);
	if (!ret) return ret;
	ret = ret && (CloseVolume == ct.CloseVolume);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenAmount, ct.OpenAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseAmount, ct.CloseAmount));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(UseMargin, ct.UseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCash, ct.FrozenCash));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CashIn, ct.CashIn));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreSettlementPrice, ct.PreSettlementPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SettlementPrice, ct.SettlementPrice));
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SettlementID == ct.SettlementID);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OpenCost, ct.OpenCost));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (CombPosition == ct.CombPosition);
	if (!ret) return ret;
	ret = ret && (CombLongFrozen == ct.CombLongFrozen);
	if (!ret) return ret;
	ret = ret && (CombShortFrozen == ct.CombShortFrozen);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByDate, ct.CloseProfitByDate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfitByTrade, ct.CloseProfitByTrade));
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByMoney, ct.MarginRateByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRateByVolume, ct.MarginRateByVolume));
	if (!ret) return ret;

	return ret;
}
};

///查询客户风险通知模版
struct QryRiskNotifyPatternField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///保留字段
	TCommentType	Reserve;

bool operator==(const QryRiskNotifyPatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///客户风险通知模版
struct RiskNotifyPatternField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码，请求修改模版时有效
	TUserIDType	UserID;
	///通知类型
	TNotifyClassType	NotifyClass;
	///是否启用
	TBoolType	IsActive;
	///是否自动系统通知
	TBoolType	IsAutoSystem;
	///是否自动短信通知
	TBoolType	IsAutoSMS;
	///是否自动邮件通知
	TBoolType	IsAutoEmail;
	///系统通知模版内容
	TRiskNofityInfoType	SystemPattern;
	///短信通知模版内容
	TRiskNofityInfoType	SMSPattern;
	///邮件通知模版内容
	TRiskNofityInfoType	EmailPattern;
	///预留参数(暂时只用作warnlevel)
	TAddressType	Reserve;

bool operator==(const RiskNotifyPatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (IsAutoSystem == ct.IsAutoSystem);
	if (!ret) return ret;
	ret = ret && (IsAutoSMS == ct.IsAutoSMS);
	if (!ret) return ret;
	ret = ret && (IsAutoEmail == ct.IsAutoEmail);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemPattern, ct.SystemPattern) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SMSPattern, ct.SMSPattern) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EmailPattern, ct.EmailPattern) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询客户风险通知模版中自动替换字段
struct QryRiskNotifyTokenField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;

bool operator==(const QryRiskNotifyTokenField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///客户风险通知模版中自动替换字段
struct RiskNotifyTokenField
{
	///自动替换字段
	TExchangeNameType	Token;
	///自动替换字段的描述
	TBrokerNameType	Description;

bool operator==(const RiskNotifyTokenField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(Token, ct.Token) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Description, ct.Description) == 0);
	if (!ret) return ret;

	return ret;
}
};

///请求发送客户风险通知的命令
struct RiskNotifyCommandField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///通知类型
	TNotifyClassType	NotifyClass;
	///投资者代码
	TInvestorIDType	InvestorID;
	///是否发送系统通知
	TBoolType	IsAutoSystem;
	///是否发送短信通知
	TBoolType	IsAutoSMS;
	///是否发送邮件通知
	TBoolType	IsAutoEmail;
	///保留字段
	TCommentType	Reserve;
	///通知模版内容
	TRiskNofityInfoType	Pattern;
	///是否允许发送正常通知
	TBoolType	IsNormal;
	///是否允许发送警示通知
	TBoolType	IsWarn;

bool operator==(const RiskNotifyCommandField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (IsAutoSystem == ct.IsAutoSystem);
	if (!ret) return ret;
	ret = ret && (IsAutoSMS == ct.IsAutoSMS);
	if (!ret) return ret;
	ret = ret && (IsAutoEmail == ct.IsAutoEmail);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Pattern, ct.Pattern) == 0);
	if (!ret) return ret;
	ret = ret && (IsNormal == ct.IsNormal);
	if (!ret) return ret;
	ret = ret && (IsWarn == ct.IsWarn);
	if (!ret) return ret;

	return ret;
}
};

///有序的客户风险通知事件
struct SequencialRiskNotifyField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///请求发送该风险通知的用户代码
	TUserIDType	UserID;
	///该用户代码的风险通知事件在当天的序号
	TSequenceNoType	SequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///通知类型
	TNotifyClassType	NotifyClass;
	///投资者代码
	TInvestorIDType	InvestorID;
	///是否发送系统通知
	TBoolType	IsAutoSystem;
	///是否发送短信通知
	TBoolType	IsAutoSMS;
	///是否发送邮件通知
	TBoolType	IsAutoEmail;
	///系统通知内容
	TRiskNofityInfoType	SystemMsg;
	///短信通知内容
	TRiskNofityInfoType	SMSMsg;
	///邮件通知内容
	TRiskNofityInfoType	EmailMsg;

bool operator==(const SequencialRiskNotifyField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (IsAutoSystem == ct.IsAutoSystem);
	if (!ret) return ret;
	ret = ret && (IsAutoSMS == ct.IsAutoSMS);
	if (!ret) return ret;
	ret = ret && (IsAutoEmail == ct.IsAutoEmail);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemMsg, ct.SystemMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SMSMsg, ct.SMSMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EmailMsg, ct.EmailMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///客户风险通知事件
struct RiskNotifyField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///请求发送该风险通知的用户代码
	TUserIDType	UserID;
	///该用户代码的风险通知事件在当天的序号
	TSequenceNoType	SequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///通知类型
	TNotifyClassType	NotifyClass;
	///投资者代码
	TInvestorIDType	InvestorID;
	///是否发送系统通知
	TBoolType	IsAutoSystem;
	///是否发送短信通知
	TBoolType	IsAutoSMS;
	///是否发送邮件通知
	TBoolType	IsAutoEmail;
	///系统通知内容
	TRiskNofityInfoType	SystemMsg;
	///短信通知内容
	TRiskNofityInfoType	SMSMsg;
	///邮件通知内容
	TRiskNofityInfoType	EmailMsg;

bool operator==(const RiskNotifyField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (IsAutoSystem == ct.IsAutoSystem);
	if (!ret) return ret;
	ret = ret && (IsAutoSMS == ct.IsAutoSMS);
	if (!ret) return ret;
	ret = ret && (IsAutoEmail == ct.IsAutoEmail);
	if (!ret) return ret;
	ret = ret && (strcmp(SystemMsg, ct.SystemMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SMSMsg, ct.SMSMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EmailMsg, ct.EmailMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///强平标准
struct ForceCloseStandardField
{
	///强平标准
	TForceCloseLevelType	ForceCloseLevel;
	///强平资金释放标准
	TForceCloseReleaseType	ForceCloseRelease;
	///非停板方向持仓优先
	TBoolType	FCNonLimitFirst;

bool operator==(const ForceCloseStandardField &ct) const
{
	bool ret = true;
	ret = ret && (ForceCloseLevel == ct.ForceCloseLevel);
	if (!ret) return ret;
	ret = ret && (ForceCloseRelease == ct.ForceCloseRelease);
	if (!ret) return ret;
	ret = ret && (FCNonLimitFirst == ct.FCNonLimitFirst);
	if (!ret) return ret;

	return ret;
}
};

///强平持仓规则
struct ForceClosePositionRuleField
{
	///产品或合约代码
	TInstrumentIDType	ProductInstrumentID;
	///强平持仓方向
	TForceClosePosiDirectionType	FCPosiDirection;
	///强平投机套保标志
	TForceCloseHedgeFlagType	FCHedgeFlag;
	///强平组合持仓标志
	TForceCloseCombPosiFlagType	FCCombPosiFlag;
	///强平历史持仓顺序
	TForceCloseHistoryPosiOrderType	FCHistoryPosiOrder;
	///强平价格类型
	TForceClosePriceTypeType	FCPrice;
	///限价调整点数
	TVolumeType	PriceTick;
	///批量强平计算规则优先级
	TFCRulePriorityType	FCRulePriority;

bool operator==(const ForceClosePositionRuleField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductInstrumentID, ct.ProductInstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (FCPosiDirection == ct.FCPosiDirection);
	if (!ret) return ret;
	ret = ret && (FCHedgeFlag == ct.FCHedgeFlag);
	if (!ret) return ret;
	ret = ret && (FCCombPosiFlag == ct.FCCombPosiFlag);
	if (!ret) return ret;
	ret = ret && (FCHistoryPosiOrder == ct.FCHistoryPosiOrder);
	if (!ret) return ret;
	ret = ret && (FCPrice == ct.FCPrice);
	if (!ret) return ret;
	ret = ret && (PriceTick == ct.PriceTick);
	if (!ret) return ret;
	ret = ret && (strcmp(FCRulePriority, ct.FCRulePriority) == 0);
	if (!ret) return ret;

	return ret;
}
};

///强平名单
struct ForceCloseListField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const ForceCloseListField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者强平持仓
struct ForceClosePositionField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///持仓多空方向
	TPosiDirectionType	PosiDirection;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///持仓日期
	TPositionDateType	PositionDate;
	///当前持仓数量
	TVolumeType	Position;
	///需强平持仓数量
	TVolumeType	FCPosition;
	///强平价格类型
	TForceClosePriceTypeType	FCPriceType;
	///限价调整点数
	TVolumeType	PriceTick;
	///强平价格
	TPriceType	FCPrice;
	///平仓释放的保证金
	TMoneyType	ReleaseMargin;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///平仓释放的交易所保证金
	TMoneyType	ExchReleaseMargin;

bool operator==(const ForceClosePositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (FCPosition == ct.FCPosition);
	if (!ret) return ret;
	ret = ret && (FCPriceType == ct.FCPriceType);
	if (!ret) return ret;
	ret = ret && (PriceTick == ct.PriceTick);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FCPrice, ct.FCPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ReleaseMargin, ct.ReleaseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchReleaseMargin, ct.ExchReleaseMargin));
	if (!ret) return ret;

	return ret;
}
};

///强平应答
struct RspForceClosePositionField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///持仓多空方向
	TPosiDirectionType	PosiDirection;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///持仓日期
	TPositionDateType	PositionDate;
	///当前持仓数量
	TVolumeType	Position;
	///需强平持仓数量
	TVolumeType	FCPosition;
	///强平价格类型
	TForceClosePriceTypeType	FCPriceType;
	///限价调整点数
	TVolumeType	PriceTick;
	///强平价格
	TPriceType	FCPrice;
	///平仓释放的保证金
	TMoneyType	ReleaseMargin;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///强平场景编号
	TForceCloseSceneIdType	FCID;
	///辅助强平单的生成时间
	TTimeType	Time;
	///当前时间（毫秒）
	TMillisecType	CurrMillisec;
	///平仓释放的交易所保证金
	TMoneyType	ExchReleaseMargin;

bool operator==(const RspForceClosePositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PosiDirection == ct.PosiDirection);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (PositionDate == ct.PositionDate);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (FCPosition == ct.FCPosition);
	if (!ret) return ret;
	ret = ret && (FCPriceType == ct.FCPriceType);
	if (!ret) return ret;
	ret = ret && (PriceTick == ct.PriceTick);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FCPrice, ct.FCPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ReleaseMargin, ct.ReleaseMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (strcmp(FCID, ct.FCID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Time, ct.Time) == 0);
	if (!ret) return ret;
	ret = ret && (CurrMillisec == ct.CurrMillisec);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchReleaseMargin, ct.ExchReleaseMargin));
	if (!ret) return ret;

	return ret;
}
};

///风控强平报单输入
struct RiskForceCloseOrderField
{
	///风控强平类型
	TForceCloseTypeType	FCType;
	///辅助强平单的生成时间
	TTimeType	Time1;
	///辅助强平单的生成时间（毫秒）
	TMillisecType	Millisec1;
	///强平单的提交时间
	TTimeType	Time2;
	///强平单的提交时间（毫秒）
	TMillisecType	Millisec2;
	///强平场景编号
	TForceCloseSceneIdType	FCSceneId;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///用户强评标志
	TBoolType	UserForceClose;
	///前置编号
	TFrontIDType	FrontID;
	///会话编号
	TSessionIDType	SessionID;

bool operator==(const RiskForceCloseOrderField &ct) const
{
	bool ret = true;
	ret = ret && (FCType == ct.FCType);
	if (!ret) return ret;
	ret = ret && (strcmp(Time1, ct.Time1) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec1 == ct.Millisec1);
	if (!ret) return ret;
	ret = ret && (strcmp(Time2, ct.Time2) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec2 == ct.Millisec2);
	if (!ret) return ret;
	ret = ret && (strcmp(FCSceneId, ct.FCSceneId) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;

	return ret;
}
};

///前置状态报告
struct FrontStatusReportField
{
	///前置time(NULL)
	TSessionIDType	Seconds;

bool operator==(const FrontStatusReportField &ct) const
{
	bool ret = true;
	ret = ret && (Seconds == ct.Seconds);
	if (!ret) return ret;

	return ret;
}
};

///净持仓保证金指标参数
struct IndexNPPField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///产品代码
	TInstrumentIDsType	ProductIDs;
	///报警值(在参数中是警示值,在指标FullIndexNPP中是指标值)
	TRatioType	WarnLevel;

bool operator==(const IndexNPPField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDs, ct.ProductIDs) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WarnLevel, ct.WarnLevel));
	if (!ret) return ret;

	return ret;
}
};

///净持仓保证金指标值
struct FullIndexNPPField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///产品代码
	TInstrumentIDsType	ProductIDs;
	///报警值(在参数中是警示值,在指标FullIndexNPP中是指标值)
	TRatioType	WarnLevel;
	///多头保证金
	TMoneyType	LongMargin;
	///空头保证金
	TMoneyType	ShortMargin;
	///总权益
	TMoneyType	Balance;
	///报警阈值
	TRatioType	RealWarnLevel;

bool operator==(const FullIndexNPPField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ProductIDs, ct.ProductIDs) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WarnLevel, ct.WarnLevel));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMargin, ct.LongMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMargin, ct.ShortMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(RealWarnLevel, ct.RealWarnLevel));
	if (!ret) return ret;

	return ret;
}
};

///一般查询请求
struct NormalRiskQueryField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;

bool operator==(const NormalRiskQueryField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询安全价格波动范围请求
struct QrySafePriceRangeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约价格波动方法
	TPriceVaryAlgoType	PriceVaryAlgo;
	///价格波动的基准价类型(v5.1.2之后废弃)
	TPriceTypeType	BasePriceType;
	///风险类型
	TNotifyClassType	RiskLevel;
	///按合约顺序波动时的最大允许停板个数
	TVolumeType	MaxLimitDay;

bool operator==(const QrySafePriceRangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (PriceVaryAlgo == ct.PriceVaryAlgo);
	if (!ret) return ret;
	ret = ret && (BasePriceType == ct.BasePriceType);
	if (!ret) return ret;
	ret = ret && (RiskLevel == ct.RiskLevel);
	if (!ret) return ret;
	ret = ret && (MaxLimitDay == ct.MaxLimitDay);
	if (!ret) return ret;

	return ret;
}
};

///价格波动参数
struct PriceVaryParamField
{
	///合约编号
	TInstrumentIDType	InstrumentID;
	///价格波动方向
	TPriceVaryDirType	Direction;
	///价格波动幅度(>=0有效)
	TRatioType	Pecent;
	///价格波动的基准价类型
	TPriceTypeType	BasePriceType;
	///自定义基准价
	TPriceType	BasePrice;

bool operator==(const PriceVaryParamField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Pecent, ct.Pecent));
	if (!ret) return ret;
	ret = ret && (BasePriceType == ct.BasePriceType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BasePrice, ct.BasePrice));
	if (!ret) return ret;

	return ret;
}
};

///查询安全价格波动范围应答
struct RspSafePriceRangeField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///基准价格
	TPriceType	BasePrice;
	///波动价格
	TPriceType	LimitPrice;
	///总波动幅度
	TRatioType	VaryPecent;
	///停板个数
	TVolumeType	LimitDayCnt;
	///总波动点数
	TVolumeType	VaryTickCnt;
	///停板点数
	TVolumeType	LimitTickCnt;
	///是否有波动限制
	TBoolType	HasLimit;
	///涨跌停
	TRatioType	PecentPerLimit;
	///停板波动
	TRatioType	InLimitVaryPct;
	///停板外波动
	TRatioType	OutLimitVaryPct;
	///总买持
	TVolumeType	LongVol;
	///总卖持
	TVolumeType	ShortVol;
	///波动方向
	TPriceVaryDirType	Direction;
	///不利方向
	TPriceVaryDirType	BadDir;

bool operator==(const RspSafePriceRangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BasePrice, ct.BasePrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(VaryPecent, ct.VaryPecent));
	if (!ret) return ret;
	ret = ret && (LimitDayCnt == ct.LimitDayCnt);
	if (!ret) return ret;
	ret = ret && (VaryTickCnt == ct.VaryTickCnt);
	if (!ret) return ret;
	ret = ret && (LimitTickCnt == ct.LimitTickCnt);
	if (!ret) return ret;
	ret = ret && (HasLimit == ct.HasLimit);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PecentPerLimit, ct.PecentPerLimit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(InLimitVaryPct, ct.InLimitVaryPct));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(OutLimitVaryPct, ct.OutLimitVaryPct));
	if (!ret) return ret;
	ret = ret && (LongVol == ct.LongVol);
	if (!ret) return ret;
	ret = ret && (ShortVol == ct.ShortVol);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (BadDir == ct.BadDir);
	if (!ret) return ret;

	return ret;
}
};

///投资者查分层
struct FullInvestorGroupField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者分组代码
	TInvestorIDType	InvestorGroupID;
	///投资者分组名称
	TInvestorGroupNameType	InvestorGroupName;

bool operator==(const FullInvestorGroupField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupName, ct.InvestorGroupName) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者查询分类
struct InvestorGroupMapField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投资者分组代码
	TInvestorIDType	InvestorGroupID;

bool operator==(const InvestorGroupMapField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询价格波动对投资者风险的影响
struct QryPriceVaryEffectField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///风险类型
	TNotifyClassType	RiskLevel;

bool operator==(const QryPriceVaryEffectField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (RiskLevel == ct.RiskLevel);
	if (!ret) return ret;

	return ret;
}
};

///经纪公司投资者
struct BrokerInvestorField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;

bool operator==(const BrokerInvestorField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///交易系统参数
struct TradeParamField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///参数代码
	TTradeParamIDType	TradeParamID;
	///参数代码值
	TSettlementParamValueType	TradeParamValue;
	///备注
	TMemoType	Memo;

bool operator==(const TradeParamField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (TradeParamID == ct.TradeParamID);
	if (!ret) return ret;
	ret = ret && (strcmp(TradeParamValue, ct.TradeParamValue) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Memo, ct.Memo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///风控预埋单
struct RiskParkedOrderField
{
	///预埋报单编号
	TParkedOrderIDType	ParkedOrderID;
	///预埋单本地编号
	TParkedOrderIDType	LocalID;
	///风控用户类型
	TRiskUserTypeType	UserType;
	///预埋单状态
	TRiskParkedOrderStatusType	Status;
	///预埋单状态信息
	TErrorMsgType	StatusMsg;
	///触发类型
	TOrderTriggerTypeType	TriggerType;
	///交易阶段
	TVolumeType	TradeSegment;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///风控强平类型
	TForceCloseTypeType	FCType;
	///辅助强平单的生成时间
	TTimeType	Time1;
	///辅助强平单的生成时间（毫秒）
	TMillisecType	Millisec1;
	///强平单的提交时间
	TTimeType	Time2;
	///强平单的提交时间（毫秒）
	TMillisecType	Millisec2;
	///强平场景编号
	TForceCloseSceneIdType	FCSceneId;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///用户强评标志
	TBoolType	UserForceClose;
	///报单提交状态
	TOrderSubmitStatusType	OrderSubmitStatus;
	///报单状态
	TOrderStatusType	OrderStatus;
	///报单状态信息
	TErrorMsgType	OrderStatusMsg;
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;
	///预埋时间
	TTimeType	ParkedTime;
	///预埋量
	TVolumeType	OriginalParkedVol;
	///预埋时可平量
	TVolumeType	MaxCloseVol1;
	///触发时可平量
	TVolumeType	MaxCloseVol2;
	///预埋时追保
	TMoneyType	Call1;
	///触发时追保
	TMoneyType	Call2;
	///预埋时出入金
	TMoneyType	MoneyIO1;
	///触发时出入金
	TMoneyType	MoneyIO2;
	///删除原因
	TCommentType	DeleteReason;
	///强平资金释放标准
	TForceCloseReleaseType	ForceCloseRelease;

bool operator==(const RiskParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LocalID, ct.LocalID) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (TriggerType == ct.TriggerType);
	if (!ret) return ret;
	ret = ret && (TradeSegment == ct.TradeSegment);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (FCType == ct.FCType);
	if (!ret) return ret;
	ret = ret && (strcmp(Time1, ct.Time1) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec1 == ct.Millisec1);
	if (!ret) return ret;
	ret = ret && (strcmp(Time2, ct.Time2) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec2 == ct.Millisec2);
	if (!ret) return ret;
	ret = ret && (strcmp(FCSceneId, ct.FCSceneId) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderStatusMsg, ct.OrderStatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedTime, ct.ParkedTime) == 0);
	if (!ret) return ret;
	ret = ret && (OriginalParkedVol == ct.OriginalParkedVol);
	if (!ret) return ret;
	ret = ret && (MaxCloseVol1 == ct.MaxCloseVol1);
	if (!ret) return ret;
	ret = ret && (MaxCloseVol2 == ct.MaxCloseVol2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Call1, ct.Call1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Call2, ct.Call2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MoneyIO1, ct.MoneyIO1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MoneyIO2, ct.MoneyIO2));
	if (!ret) return ret;
	ret = ret && (strcmp(DeleteReason, ct.DeleteReason) == 0);
	if (!ret) return ret;
	ret = ret && (ForceCloseRelease == ct.ForceCloseRelease);
	if (!ret) return ret;

	return ret;
}
};

///删除风控预埋单
struct RemoveRiskParkedOrderField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///预埋报单编号
	TParkedOrderIDType	ParkedOrderID;

bool operator==(const RemoveRiskParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;

	return ret;
}
};

///有序的风控预埋单
struct SeqRiskParkedOrderField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///预埋报单编号
	TParkedOrderIDType	ParkedOrderID;
	///预埋单本地编号
	TParkedOrderIDType	LocalID;
	///风控用户类型
	TRiskUserTypeType	UserType;
	///预埋单状态
	TRiskParkedOrderStatusType	Status;
	///预埋单状态信息
	TErrorMsgType	StatusMsg;
	///触发类型
	TOrderTriggerTypeType	TriggerType;
	///交易阶段
	TVolumeType	TradeSegment;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///风控强平类型
	TForceCloseTypeType	FCType;
	///辅助强平单的生成时间
	TTimeType	Time1;
	///辅助强平单的生成时间（毫秒）
	TMillisecType	Millisec1;
	///强平单的提交时间
	TTimeType	Time2;
	///强平单的提交时间（毫秒）
	TMillisecType	Millisec2;
	///强平场景编号
	TForceCloseSceneIdType	FCSceneId;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///报单引用
	TOrderRefType	OrderRef;
	///用户代码
	TUserIDType	UserID;
	///报单价格条件
	TOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TDirectionType	Direction;
	///组合开平标志
	TCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TCombHedgeFlagType	CombHedgeFlag;
	///价格
	TPriceType	LimitPrice;
	///数量
	TVolumeType	VolumeTotalOriginal;
	///有效期类型
	TTimeConditionType	TimeCondition;
	///GTD日期
	TDateType	GTDDate;
	///成交量类型
	TVolumeConditionType	VolumeCondition;
	///最小成交量
	TVolumeType	MinVolume;
	///触发条件
	TContingentConditionType	ContingentCondition;
	///止损价
	TPriceType	StopPrice;
	///强平原因
	TForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TBoolType	IsAutoSuspend;
	///业务单元
	TBusinessUnitType	BusinessUnit;
	///请求编号
	TRequestIDType	RequestID;
	///用户强评标志
	TBoolType	UserForceClose;
	///报单提交状态
	TOrderSubmitStatusType	OrderSubmitStatus;
	///报单状态
	TOrderStatusType	OrderStatus;
	///报单状态信息
	TErrorMsgType	OrderStatusMsg;
	///错误代码
	TErrorIDType	ErrorID;
	///错误信息
	TErrorMsgType	ErrorMsg;
	///预埋时间
	TTimeType	ParkedTime;
	///预埋量
	TVolumeType	OriginalParkedVol;
	///预埋时可平量
	TVolumeType	MaxCloseVol1;
	///触发时可平量
	TVolumeType	MaxCloseVol2;
	///预埋时追保
	TMoneyType	Call1;
	///触发时追保
	TMoneyType	Call2;
	///预埋时出入金
	TMoneyType	MoneyIO1;
	///触发时出入金
	TMoneyType	MoneyIO2;
	///删除原因
	TCommentType	DeleteReason;
	///强平资金释放标准
	TForceCloseReleaseType	ForceCloseRelease;

bool operator==(const SeqRiskParkedOrderField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedOrderID, ct.ParkedOrderID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(LocalID, ct.LocalID) == 0);
	if (!ret) return ret;
	ret = ret && (UserType == ct.UserType);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (strcmp(StatusMsg, ct.StatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (TriggerType == ct.TriggerType);
	if (!ret) return ret;
	ret = ret && (TradeSegment == ct.TradeSegment);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (FCType == ct.FCType);
	if (!ret) return ret;
	ret = ret && (strcmp(Time1, ct.Time1) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec1 == ct.Millisec1);
	if (!ret) return ret;
	ret = ret && (strcmp(Time2, ct.Time2) == 0);
	if (!ret) return ret;
	ret = ret && (Millisec2 == ct.Millisec2);
	if (!ret) return ret;
	ret = ret && (strcmp(FCSceneId, ct.FCSceneId) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderRef, ct.OrderRef) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (OrderPriceType == ct.OrderPriceType);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombOffsetFlag[i] == ct.CombOffsetFlag[i]);
		if (!ret) return ret;
	}
	for (int i = 0; i < 4; ++i) {
		ret = ret && (CombHedgeFlag[i] == ct.CombHedgeFlag[i]);
		if (!ret) return ret;
	}
	ret = ret && (EQUALDOUBLE(LimitPrice, ct.LimitPrice));
	if (!ret) return ret;
	ret = ret && (VolumeTotalOriginal == ct.VolumeTotalOriginal);
	if (!ret) return ret;
	ret = ret && (TimeCondition == ct.TimeCondition);
	if (!ret) return ret;
	ret = ret && (strcmp(GTDDate, ct.GTDDate) == 0);
	if (!ret) return ret;
	ret = ret && (VolumeCondition == ct.VolumeCondition);
	if (!ret) return ret;
	ret = ret && (MinVolume == ct.MinVolume);
	if (!ret) return ret;
	ret = ret && (ContingentCondition == ct.ContingentCondition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(StopPrice, ct.StopPrice));
	if (!ret) return ret;
	ret = ret && (ForceCloseReason == ct.ForceCloseReason);
	if (!ret) return ret;
	ret = ret && (IsAutoSuspend == ct.IsAutoSuspend);
	if (!ret) return ret;
	ret = ret && (strcmp(BusinessUnit, ct.BusinessUnit) == 0);
	if (!ret) return ret;
	ret = ret && (RequestID == ct.RequestID);
	if (!ret) return ret;
	ret = ret && (UserForceClose == ct.UserForceClose);
	if (!ret) return ret;
	ret = ret && (OrderSubmitStatus == ct.OrderSubmitStatus);
	if (!ret) return ret;
	ret = ret && (OrderStatus == ct.OrderStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderStatusMsg, ct.OrderStatusMsg) == 0);
	if (!ret) return ret;
	ret = ret && (ErrorID == ct.ErrorID);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParkedTime, ct.ParkedTime) == 0);
	if (!ret) return ret;
	ret = ret && (OriginalParkedVol == ct.OriginalParkedVol);
	if (!ret) return ret;
	ret = ret && (MaxCloseVol1 == ct.MaxCloseVol1);
	if (!ret) return ret;
	ret = ret && (MaxCloseVol2 == ct.MaxCloseVol2);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Call1, ct.Call1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Call2, ct.Call2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MoneyIO1, ct.MoneyIO1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MoneyIO2, ct.MoneyIO2));
	if (!ret) return ret;
	ret = ret && (strcmp(DeleteReason, ct.DeleteReason) == 0);
	if (!ret) return ret;
	ret = ret && (ForceCloseRelease == ct.ForceCloseRelease);
	if (!ret) return ret;

	return ret;
}
};

///风控中投资者消息
struct RiskInvestorInfoField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///手机
	TTelephoneType	Mobile;
	///电子邮件
	TEMailType	EMail;
	///是否允许发送邮件
	TBoolType	IsEMail;
	///是否允许发送邮件
	TBoolType	IsSMS;
	///投资者类型
	TInvestorTypeType	InvestorType;
	///国家代码
	TUOACountryCodeType	PhoneCountryCode;
	///区号
	TAreaCodeType	PhoneAreaCode;
	///开户授权人国家代码
	TUOACountryCodeType	OpenPhoneCountryCode;
	///开户授权人区号
	TAreaCodeType	OpenPhoneAreaCode;
	///指定下单人国家代码
	TUOACountryCodeType	OrderPhoneCountryCode;
	///指定下单人区号
	TAreaCodeType	OrderPhoneAreaCode;
	///资金调拨人国家代码
	TUOACountryCodeType	FundPhoneCountryCode;
	///资金调拨人区号
	TAreaCodeType	FundPhoneAreaCode;
	///结算单确认人国家代码
	TUOACountryCodeType	SettlePhoneCountryCode;
	///结算单确认人区号
	TAreaCodeType	SettlePhoneAreaCode;

bool operator==(const RiskInvestorInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EMail, ct.EMail) == 0);
	if (!ret) return ret;
	ret = ret && (IsEMail == ct.IsEMail);
	if (!ret) return ret;
	ret = ret && (IsSMS == ct.IsSMS);
	if (!ret) return ret;
	ret = ret && (InvestorType == ct.InvestorType);
	if (!ret) return ret;
	ret = ret && (strcmp(PhoneCountryCode, ct.PhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(PhoneAreaCode, ct.PhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenPhoneCountryCode, ct.OpenPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenPhoneAreaCode, ct.OpenPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderPhoneCountryCode, ct.OrderPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OrderPhoneAreaCode, ct.OrderPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundPhoneCountryCode, ct.FundPhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(FundPhoneAreaCode, ct.FundPhoneAreaCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlePhoneCountryCode, ct.SettlePhoneCountryCode) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SettlePhoneAreaCode, ct.SettlePhoneAreaCode) == 0);
	if (!ret) return ret;

	return ret;
}
};

///客户风险通知版本A
struct RiskNotifyAField
{
	///风险通知事件在当天的序号
	TSequenceNoType	SequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///请求发送该风险通知的用户代码
	TUserIDType	UserID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///通知类型
	TNotifyClassType	NotifyClass;
	///风险通知途径
	TRiskNotifyMethodType	NotifyMethod;
	///风险通知状态
	TRiskNotifyStatusType	NotifyStatus;
	///通知内容
	TRiskNofityInfoType	Message;
	///预留字段(人工通知-通知操作员，否则为错误原因)
	TCommentType	Reserve;

bool operator==(const RiskNotifyAField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (NotifyStatus == ct.NotifyStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///有序的客户风险通知版本A
struct SeqRiskNotifyAField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///风险通知事件在当天的序号
	TSequenceNoType	SequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///请求发送该风险通知的用户代码
	TUserIDType	UserID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///通知类型
	TNotifyClassType	NotifyClass;
	///风险通知途径
	TRiskNotifyMethodType	NotifyMethod;
	///风险通知状态
	TRiskNotifyStatusType	NotifyStatus;
	///通知内容
	TRiskNofityInfoType	Message;
	///预留字段(人工通知-通知操作员，否则为错误原因)
	TCommentType	Reserve;

bool operator==(const SeqRiskNotifyAField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (NotifyStatus == ct.NotifyStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///有序的出入金
struct SeqDepositField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///出入金类型
	TFundIOTypeType	Type;
	///有效标志-有效或冲正
	TAvailabilityFlagType	Flag;
	///出入金方向
	TFundDirectionType	Direction;
	///交易日
	TDateType	TradingDay;
	///流水号
	TDepositSeqNoType	SequenceNo;
	///时间
	TTimeType	Time;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///出入金金额
	TMoneyType	Amount;

bool operator==(const SeqDepositField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (Type == ct.Type);
	if (!ret) return ret;
	ret = ret && (Flag == ct.Flag);
	if (!ret) return ret;
	ret = ret && (Direction == ct.Direction);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(SequenceNo, ct.SequenceNo) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Time, ct.Time) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Amount, ct.Amount));
	if (!ret) return ret;

	return ret;
}
};

///有序的交易编码
struct SeqTradingCodeField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///投资者代码
	TInvestorIDType	InvestorID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所代码
	TExchangeIDType	ExchangeID;
	///客户代码
	TClientIDType	ClientID;
	///是否活跃
	TBoolType	IsActive;
	///操作标志(修改或删除)
	TActionFlagType	Action;
	///交易编码类型
	TClientIDTypeType	ClientIDType;

bool operator==(const SeqTradingCodeField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeID, ct.ExchangeID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ClientID, ct.ClientID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (Action == ct.Action);
	if (!ret) return ret;
	ret = ret && (ClientIDType == ct.ClientIDType);
	if (!ret) return ret;

	return ret;
}
};

///风控用户操作事件
struct RiskUserEventField
{
	///用户事件当天的序号
	TSequenceNoType	SequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///事件类型
	TRiskUserEventType	EventType;
	///事件信息
	TUserEventInfoType	EventInfo;

bool operator==(const RiskUserEventField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (EventType == ct.EventType);
	if (!ret) return ret;
	ret = ret && (strcmp(EventInfo, ct.EventInfo) == 0);
	if (!ret) return ret;

	return ret;
}
};

///风控订阅序列号
struct RiskNtfSequenceField
{
	///序号
	TSequenceNoType	SequenceNo;
	///业务数据类型
	TBusinessUnitType	DataType;

bool operator==(const RiskNtfSequenceField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(DataType, ct.DataType) == 0);
	if (!ret) return ret;

	return ret;
}
};

///客户风险通知模版版本A
struct RiskNotifyPatternAField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码，请求修改模版时有效
	TUserIDType	UserID;
	///通知类型
	TNotifyClassType	NotifyClass;
	///风险通知途径
	TRiskNotifyMethodType	NotifyMethod;
	///是否启用
	TBoolType	IsActive;
	///通知模版内容
	TRiskNofityInfoType	Pattern;
	///预留参数(暂时只用作warnlevel)
	TCommentType	Reserve;

bool operator==(const RiskNotifyPatternAField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Pattern, ct.Pattern) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///品种停板涨跌幅
struct ProductLimitsField
{
	///品种或合约代码
	TInstrumentIDType	ProductID;
	///D1涨跌幅
	TRatioType	Limit1;
	///D2涨跌幅
	TRatioType	Limit2;
	///D3涨跌幅
	TRatioType	Limit3;
	///D4涨跌幅
	TRatioType	Limit4;
	///最大保证金率
	TRatioType	MaxMarginRate1;
	///结算价
	TPriceType	Price;

bool operator==(const ProductLimitsField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(ProductID, ct.ProductID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit1, ct.Limit1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit2, ct.Limit2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit3, ct.Limit3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit4, ct.Limit4));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MaxMarginRate1, ct.MaxMarginRate1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;

	return ret;
}
};

///风险预算资金账户
struct PredictRiskAccountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///初始权益
	TMoneyType	Balance0;
	///D1亏损
	TMoneyType	Lose1;
	///D1权益
	TMoneyType	Balance1;
	///D2亏损
	TMoneyType	Lose2;
	///D2权益
	TMoneyType	Balance2;
	///D3亏损
	TMoneyType	Lose3;
	///D3权益
	TMoneyType	Balance3;
	///D4亏损
	TMoneyType	Lose4;
	///D4权益
	TMoneyType	Balance4;

bool operator==(const PredictRiskAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance0, ct.Balance0));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose1, ct.Lose1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance1, ct.Balance1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose2, ct.Lose2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance2, ct.Balance2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose3, ct.Lose3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance3, ct.Balance3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose4, ct.Lose4));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance4, ct.Balance4));
	if (!ret) return ret;

	return ret;
}
};

///风险预算持仓
struct PredictRiskPositionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///多头持仓
	TVolumeType	LongVol;
	///空头持仓
	TVolumeType	ShortVol;
	///净持仓
	TVolumeType	NetVol;
	///初始价格
	TPriceType	Price0;
	///D1结算价
	TPriceType	Price1;
	///D1涨跌幅
	TRatioType	Limit1;
	///D2涨跌幅
	TRatioType	Limit2;
	///D3涨跌幅
	TRatioType	Limit3;
	///D4涨跌幅
	TRatioType	Limit4;
	///合约乘数
	TVolumeMultipleType	Multiple;
	///D1亏损
	TMoneyType	Lose1;
	///D2亏损
	TMoneyType	Lose2;
	///D3亏损
	TMoneyType	Lose3;
	///D4亏损
	TMoneyType	Lose4;
	///可留手数
	TVolumeType	LeftVol;
	///D1应强平
	TVolumeType	ShouldClose;
	///调整后可强平
	TVolumeType	CanClose;
	///当前保证金率
	TRatioType	CurrMarginRate;
	///调整后保证金率
	TRatioType	MarginRate1;
	///净持仓保证金
	TMoneyType	NetMargin1;
	///D1可用
	TMoneyType	Available1;
	///1手保证金
	TMoneyType	Margin1PerVol;

bool operator==(const PredictRiskPositionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (LongVol == ct.LongVol);
	if (!ret) return ret;
	ret = ret && (ShortVol == ct.ShortVol);
	if (!ret) return ret;
	ret = ret && (NetVol == ct.NetVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price0, ct.Price0));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price1, ct.Price1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit1, ct.Limit1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit2, ct.Limit2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit3, ct.Limit3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Limit4, ct.Limit4));
	if (!ret) return ret;
	ret = ret && (Multiple == ct.Multiple);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose1, ct.Lose1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose2, ct.Lose2));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose3, ct.Lose3));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Lose4, ct.Lose4));
	if (!ret) return ret;
	ret = ret && (LeftVol == ct.LeftVol);
	if (!ret) return ret;
	ret = ret && (ShouldClose == ct.ShouldClose);
	if (!ret) return ret;
	ret = ret && (CanClose == ct.CanClose);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMarginRate, ct.CurrMarginRate));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(MarginRate1, ct.MarginRate1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NetMargin1, ct.NetMargin1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available1, ct.Available1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Margin1PerVol, ct.Margin1PerVol));
	if (!ret) return ret;

	return ret;
}
};

///风险预算参数
struct PredictRiskParamField
{
	///D1日期
	TVolumeType	D1;
	///是否过滤无效记录
	TBoolType	IsFilter;

bool operator==(const PredictRiskParamField &ct) const
{
	bool ret = true;
	ret = ret && (D1 == ct.D1);
	if (!ret) return ret;
	ret = ret && (IsFilter == ct.IsFilter);
	if (!ret) return ret;

	return ret;
}
};

///风控中同步投资者
struct RiskSyncInvestorField
{
	///操作标志(修改或删除)
	TActionFlagType	Action;
	///投资者代码
	TInvestorIDType	InvestorID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者分组代码
	TInvestorIDType	InvestorGroupID;
	///投资者名称
	TPartyNameType	InvestorName;
	///证件类型
	TIdCardTypeType	IdentifiedCardType;
	///证件号码
	TIdentifiedCardNoType	IdentifiedCardNo;
	///是否活跃
	TBoolType	IsActive;
	///联系电话
	TTelephoneType	Telephone;
	///通讯地址
	TAddressType	Address;
	///开户日期
	TDateType	OpenDate;
	///手机
	TMobileType	Mobile;

bool operator==(const RiskSyncInvestorField &ct) const
{
	bool ret = true;
	ret = ret && (Action == ct.Action);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorGroupID, ct.InvestorGroupID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorName, ct.InvestorName) == 0);
	if (!ret) return ret;
	ret = ret && (IdentifiedCardType == ct.IdentifiedCardType);
	if (!ret) return ret;
	ret = ret && (strcmp(IdentifiedCardNo, ct.IdentifiedCardNo) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Telephone, ct.Telephone) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Address, ct.Address) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(OpenDate, ct.OpenDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Mobile, ct.Mobile) == 0);
	if (!ret) return ret;

	return ret;
}
};

///合约价格
struct InstrumentPriceField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///价格类型
	TPriceTypeType	PriceType;
	///价格
	TPriceType	Price;

bool operator==(const InstrumentPriceField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (PriceType == ct.PriceType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;

	return ret;
}
};

///投资者与联系人信息的Hash值
struct InvestorHashField
{
	///请求中是业务类型，否则是数据MD5值
	TFullMD5Type	MD5;
	///投资者代码(为空代表所有投资者代码)
	TInvestorIDType	InvestorIDBeg;
	///投资者代码(为空代表所有投资者代码)
	TInvestorIDType	InvestorIDEnd;

bool operator==(const InvestorHashField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(MD5, ct.MD5) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDBeg, ct.InvestorIDBeg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者代码
struct InvestorIDRangeField
{
	///投资者代码(为空代表所有投资者代码)
	TInvestorIDType	InvestorIDBeg;
	///投资者代码(为空代表所有投资者代码)
	TInvestorIDType	InvestorIDEnd;

bool operator==(const InvestorIDRangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InvestorIDBeg, ct.InvestorIDBeg) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorIDEnd, ct.InvestorIDEnd) == 0);
	if (!ret) return ret;

	return ret;
}
};

///带通讯时段编号的流序号
struct CommPhaseSequenceField
{
	///通讯时段编号
	TCommPhaseNoType	CommPhaseNo;
	///序列系列号
	TSequenceSeriesType	SequenceSeries;
	///序号
	TSequenceNoType	SequenceNo;

bool operator==(const CommPhaseSequenceField &ct) const
{
	bool ret = true;
	ret = ret && (CommPhaseNo == ct.CommPhaseNo);
	if (!ret) return ret;
	ret = ret && (SequenceSeries == ct.SequenceSeries);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;

	return ret;
}
};

///风控用户权限
struct RiskUserFunctionField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///权限名称
	TFunctionIDType	Function;

bool operator==(const RiskUserFunctionField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Function, ct.Function) == 0);
	if (!ret) return ret;

	return ret;
}
};

///连接状态
struct LinkStatusField
{
	///连接状态
	TLinkStatusType	LinkStatus;

bool operator==(const LinkStatusField &ct) const
{
	bool ret = true;
	ret = ret && (LinkStatus == ct.LinkStatus);
	if (!ret) return ret;

	return ret;
}
};

///风控同步投资者资金账户
struct RiskSyncAccountField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///出入金金额
	TMoneyType	Withdraw;
	///当前保证金总额
	TMoneyType	CurrMargin;
	///手续费
	TMoneyType	Commission;
	///平仓盈亏
	TMoneyType	CloseProfit;
	///持仓盈亏
	TMoneyType	PositionProfit;
	///总权益
	TMoneyType	Balance;
	///可用资金
	TMoneyType	Available;
	///质押金额
	TMoneyType	Mortgage;
	///交易所保证金
	TMoneyType	ExchangeMargin;
	///可取资金
	TMoneyType	WithdrawQuota;
	///信用额度
	TMoneyType	Credit;
	///保证金冻结
	TMoneyType	FrozenMargin;
	///手续费冻结
	TMoneyType	FrozenCommission;
	///接收时间
	TTimeType RecvTime;

bool operator==(const RiskSyncAccountField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Withdraw, ct.Withdraw));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CurrMargin, ct.CurrMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Commission, ct.Commission));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(CloseProfit, ct.CloseProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionProfit, ct.PositionProfit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Balance, ct.Balance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Available, ct.Available));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Mortgage, ct.Mortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeMargin, ct.ExchangeMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(WithdrawQuota, ct.WithdrawQuota));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Credit, ct.Credit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenMargin, ct.FrozenMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(FrozenCommission, ct.FrozenCommission));
	if (!ret) return ret;

	return ret;
}
};

///风控资金账户昨日信息
struct SeqPreRiskAccountField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投资者帐号
	TAccountIDType	AccountID;
	///上次质押金额
	TMoneyType	PreMortgage;
	///上次信用额度
	TMoneyType	PreCredit;
	///上次存款额
	TMoneyType	PreDeposit;
	///上次总权益
	TMoneyType	PreBalance;
	///上次占用的保证金
	TMoneyType	PreMargin;
	///基本准备金
	TMoneyType	Reserve;
	///上次交易所保证金
	TMoneyType	PreExchMargin;
	///历史强平次数
	TVolumeType	ForceCloseStat;
	///投资者交割保证金
	TMoneyType	DeliveryMargin;
	///交易所交割保证金
	TMoneyType	ExchangeDeliveryMargin;
	///接收时间
	TTimeType RecvTime;

bool operator==(const SeqPreRiskAccountField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(AccountID, ct.AccountID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMortgage, ct.PreMortgage));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreCredit, ct.PreCredit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreDeposit, ct.PreDeposit));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreBalance, ct.PreBalance));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreMargin, ct.PreMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Reserve, ct.Reserve));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PreExchMargin, ct.PreExchMargin));
	if (!ret) return ret;
	ret = ret && (ForceCloseStat == ct.ForceCloseStat);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(DeliveryMargin, ct.DeliveryMargin));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ExchangeDeliveryMargin, ct.ExchangeDeliveryMargin));
	if (!ret) return ret;

	return ret;
}
};
struct RiskAllAccountField
{
	//昨日
	SeqPreRiskAccountField pre;
	//今日
	RiskSyncAccountField   cur;
};
///查询服务器登录应答
struct RspLocalFrontLoginField
{
	///通讯时段编号
	TCommPhaseNoType	CommPhaseNo;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///前置编号
	TFrontIDType	FrontID;
	///风控前置中连接的SessionID
	TSessionIDType	FrontSessionID;

bool operator==(const RspLocalFrontLoginField &ct) const
{
	bool ret = true;
	ret = ret && (CommPhaseNo == ct.CommPhaseNo);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;
	ret = ret && (FrontSessionID == ct.FrontSessionID);
	if (!ret) return ret;

	return ret;
}
};

///模板替换字段
struct NoticeTokenField
{
	///自动替换字段
	TExchangeNameType	Token;
	///自动替换字段的描述
	TBrokerNameType	Description;

bool operator==(const NoticeTokenField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(Token, ct.Token) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Description, ct.Description) == 0);
	if (!ret) return ret;

	return ret;
}
};

///通知模板
struct NoticePatternField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///业务类型
	TParamNameType	BizType;
	///通知途径
	TRiskNotifyMethodType	Method;
	///业务名称
	TParamNameType	BizName;
	///最近修改模版的用户代码
	TUserIDType	UserID;
	///是否启用
	TBoolType	IsActive;
	///通知模版内容
	TContentType	Pattern;

bool operator==(const NoticePatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (Method == ct.Method);
	if (!ret) return ret;
	ret = ret && (strcmp(BizName, ct.BizName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;
	ret = ret && (strcmp(Pattern, ct.Pattern) == 0);
	if (!ret) return ret;

	return ret;
}
};

///业务通知
struct BizNoticeField
{
	///事件发生日期
	TDateType	TradingDay;
	///通知事件在当天的序号
	TSequenceNoType	SequenceNo;
	///风险通知途径
	TRiskNotifyMethodType	Method;
	///事件发生时间
	TTimeType	EventTime;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///业务类型
	TParamNameType	BizType;
	///通知状态
	TRiskNotifyStatusType	Status;
	///通知内容
	TContentType	Message;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const BizNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (Method == ct.Method);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者参数
struct RiskInvestorParamField
{
	///参数代码
	TParamIDType	ParamID;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///参数值
	TParamValueType	ParamValue;

bool operator==(const RiskInvestorParamField &ct) const
{
	bool ret = true;
	ret = ret && (ParamID == ct.ParamID);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ParamValue, ct.ParamValue) == 0);
	if (!ret) return ret;

	return ret;
}
};

///变化行情
struct VaryMarketDataField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///最新价
	TPriceType	LastPrice;

bool operator==(const VaryMarketDataField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LastPrice, ct.LastPrice));
	if (!ret) return ret;

	return ret;
}
};

///合约价格区间
struct PriceRangeField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///价格1
	TPriceType	Price1;
	///价格2
	TPriceType	Price2;

bool operator==(const PriceRangeField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price1, ct.Price1));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price2, ct.Price2));
	if (!ret) return ret;

	return ret;
}
};

///有序的业务通知
struct SeqBizNoticeField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///事件发生日期
	TDateType	TradingDay;
	///通知事件在当天的序号
	TSequenceNoType	SequenceNo;
	///风险通知途径
	TRiskNotifyMethodType	Method;
	///事件发生时间
	TTimeType	EventTime;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///业务类型
	TParamNameType	BizType;
	///通知状态
	TRiskNotifyStatusType	Status;
	///通知内容
	TContentType	Message;
	///错误信息
	TErrorMsgType	ErrorMsg;

bool operator==(const SeqBizNoticeField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(TradingDay, ct.TradingDay) == 0);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (Method == ct.Method);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (Status == ct.Status);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ErrorMsg, ct.ErrorMsg) == 0);
	if (!ret) return ret;

	return ret;
}
};

///风控参数信息
struct RiskParamInfoField
{
	///参数代码
	TParamIDType	ParamID;
	///参数说明
	TDescriptionType	Description;

bool operator==(const RiskParamInfoField &ct) const
{
	bool ret = true;
	ret = ret && (ParamID == ct.ParamID);
	if (!ret) return ret;
	ret = ret && (strcmp(Description, ct.Description) == 0);
	if (!ret) return ret;

	return ret;
}
};

///风控登录信息
struct RiskLoginInfoField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///用户代码
	TUserIDType	UserID;
	///本地前置会话编号
	TSessionIDType	LocalSessionID;
	///风控前置会话编号
	TSessionIDType	SessionID;
	///风控前置编号
	TFrontIDType	FrontID;

bool operator==(const RiskLoginInfoField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (LocalSessionID == ct.LocalSessionID);
	if (!ret) return ret;
	ret = ret && (SessionID == ct.SessionID);
	if (!ret) return ret;
	ret = ret && (FrontID == ct.FrontID);
	if (!ret) return ret;

	return ret;
}
};

///风控通知模板
struct RiskPatternField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///业务类型
	TParamNameType	BizType;
	///模板代码
	TParamIDType	PatternID;
	///模板名称
	TParamNameType	PatternName;
	///通知模版内容
	TContentType	Pattern;

bool operator==(const RiskPatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (PatternID == ct.PatternID);
	if (!ret) return ret;
	ret = ret && (strcmp(PatternName, ct.PatternName) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Pattern, ct.Pattern) == 0);
	if (!ret) return ret;

	return ret;
}
};

///投资者通知模板
struct InvestorPatternField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///业务类型
	TParamNameType	BizType;
	///通知途径
	TRiskNotifyMethodType	Method;
	///模板代码
	TParamIDType	PatternID;
	///是否启用
	TBoolType	IsActive;

bool operator==(const InvestorPatternField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BizType, ct.BizType) == 0);
	if (!ret) return ret;
	ret = ret && (Method == ct.Method);
	if (!ret) return ret;
	ret = ret && (PatternID == ct.PatternID);
	if (!ret) return ret;
	ret = ret && (IsActive == ct.IsActive);
	if (!ret) return ret;

	return ret;
}
};

///客户风险通知版本B
struct RiskNotifyBField
{
	///风险通知事件在当天的序号
	TSequenceNoType	SequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///请求发送该风险通知的用户代码
	TUserIDType	UserID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///通知类型
	TNotifyClassType	NotifyClass;
	///风险通知途径
	TRiskNotifyMethodType	NotifyMethod;
	///风险通知状态
	TRiskNotifyStatusType	NotifyStatus;
	///通知内容
	TContentType	Message;
	///预留字段(人工通知-通知操作员，否则为错误原因)
	TCommentType	Reserve;

bool operator==(const RiskNotifyBField &ct) const
{
	bool ret = true;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (NotifyStatus == ct.NotifyStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///有序的客户风险通知版本B
struct SeqRiskNotifyBField
{
	///流中唯一的序列号
	TSequenceNoType	UniqSequenceNo;
	///风险通知事件在当天的序号
	TSequenceNoType	SequenceNo;
	///事件发生日期
	TDateType	EventDate;
	///事件发生时间
	TTimeType	EventTime;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///请求发送该风险通知的用户代码
	TUserIDType	UserID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///通知类型
	TNotifyClassType	NotifyClass;
	///风险通知途径
	TRiskNotifyMethodType	NotifyMethod;
	///风险通知状态
	TRiskNotifyStatusType	NotifyStatus;
	///通知内容
	TContentType	Message;
	///预留字段(人工通知-通知操作员，否则为错误原因)
	TCommentType	Reserve;

bool operator==(const SeqRiskNotifyBField &ct) const
{
	bool ret = true;
	ret = ret && (UniqSequenceNo == ct.UniqSequenceNo);
	if (!ret) return ret;
	ret = ret && (SequenceNo == ct.SequenceNo);
	if (!ret) return ret;
	ret = ret && (strcmp(EventDate, ct.EventDate) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(EventTime, ct.EventTime) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(UserID, ct.UserID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (NotifyClass == ct.NotifyClass);
	if (!ret) return ret;
	ret = ret && (NotifyMethod == ct.NotifyMethod);
	if (!ret) return ret;
	ret = ret && (NotifyStatus == ct.NotifyStatus);
	if (!ret) return ret;
	ret = ret && (strcmp(Message, ct.Message) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(Reserve, ct.Reserve) == 0);
	if (!ret) return ret;

	return ret;
}
};

///查询持仓成交报排名的参数
struct QryStatField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所品种合约代码组合(交易所代码使用前缀e:，格式如cu,e:SHFE,cu1105)
	TString100Type	ExchangeProductInstID;
	///排名类型
	TStatSortTypeType	SortType;
	///按排名数返回结果
	TVolumeType	ResultCount;
	///按比例返回结果
	TRatioType	ResultRatio;

bool operator==(const QryStatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeProductInstID, ct.ExchangeProductInstID) == 0);
	if (!ret) return ret;
	ret = ret && (SortType == ct.SortType);
	if (!ret) return ret;
	ret = ret && (ResultCount == ct.ResultCount);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ResultRatio, ct.ResultRatio));
	if (!ret) return ret;

	return ret;
}
};

///持仓排名统计
struct PositionStatField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所品种合约代码组合(交易所代码使用前缀e:，格式如cu,e:SHFE,cu1105)
	TString100Type	ExchangeProductInstID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///昨持仓
	TVolumeType	YdPosition;
	///总持仓
	TVolumeType	Position;
	///多头持仓
	TVolumeType	LongPosition;
	///空头持仓
	TVolumeType	ShortPosition;
	///净持仓
	TVolumeType	NetPosition;
	///投机多头持仓
	TVolumeType	SpecuLongPosi;
	///投机空头持仓
	TVolumeType	SpecuShortPosi;
	///保值多头持仓
	TVolumeType	HedgeLongPosi;
	///保值空头持仓
	TVolumeType	HedgeShortPosi;
	///今仓
	TVolumeType	TodayPosition;
	///总持仓成本
	TMoneyType	PositionCost;

bool operator==(const PositionStatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeProductInstID, ct.ExchangeProductInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (YdPosition == ct.YdPosition);
	if (!ret) return ret;
	ret = ret && (Position == ct.Position);
	if (!ret) return ret;
	ret = ret && (LongPosition == ct.LongPosition);
	if (!ret) return ret;
	ret = ret && (ShortPosition == ct.ShortPosition);
	if (!ret) return ret;
	ret = ret && (NetPosition == ct.NetPosition);
	if (!ret) return ret;
	ret = ret && (SpecuLongPosi == ct.SpecuLongPosi);
	if (!ret) return ret;
	ret = ret && (SpecuShortPosi == ct.SpecuShortPosi);
	if (!ret) return ret;
	ret = ret && (HedgeLongPosi == ct.HedgeLongPosi);
	if (!ret) return ret;
	ret = ret && (HedgeShortPosi == ct.HedgeShortPosi);
	if (!ret) return ret;
	ret = ret && (TodayPosition == ct.TodayPosition);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(PositionCost, ct.PositionCost));
	if (!ret) return ret;

	return ret;
}
};

///成交排名统计
struct TradeStatField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所品种合约代码组合(交易所代码使用前缀e:，格式如cu,e:SHFE,cu1105)
	TString100Type	ExchangeProductInstID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///总成交量
	TVolumeType	TotalVol;
	///总成交额
	TMoneyType	TotalAmt;
	///买成交量
	TVolumeType	BuyVol;
	///买成交额
	TMoneyType	BuyAmt;
	///卖成交量
	TVolumeType	SellVol;
	///卖成交额
	TMoneyType	SellAmt;
	///净买入成交量
	TVolumeType	NetVol;
	///净买入成交额
	TMoneyType	NetAmt;

bool operator==(const TradeStatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeProductInstID, ct.ExchangeProductInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TotalVol == ct.TotalVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(TotalAmt, ct.TotalAmt));
	if (!ret) return ret;
	ret = ret && (BuyVol == ct.BuyVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(BuyAmt, ct.BuyAmt));
	if (!ret) return ret;
	ret = ret && (SellVol == ct.SellVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(SellAmt, ct.SellAmt));
	if (!ret) return ret;
	ret = ret && (NetVol == ct.NetVol);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(NetAmt, ct.NetAmt));
	if (!ret) return ret;

	return ret;
}
};

///报单排名统计
struct OrderStatField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///交易所品种合约代码组合(交易所代码使用前缀e:，格式如cu,e:SHFE,cu1105)
	TString100Type	ExchangeProductInstID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///总报单量
	TVolumeType	TotalVol;
	///买报单量
	TVolumeType	BuyVol;
	///卖报单量
	TVolumeType	SellVol;
	///净买入报单量
	TVolumeType	NetVol;
	///有成交报单数
	TVolumeType	TradeCount;
	///总报单数
	TVolumeType	TotalCount;

bool operator==(const OrderStatField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(ExchangeProductInstID, ct.ExchangeProductInstID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (TotalVol == ct.TotalVol);
	if (!ret) return ret;
	ret = ret && (BuyVol == ct.BuyVol);
	if (!ret) return ret;
	ret = ret && (SellVol == ct.SellVol);
	if (!ret) return ret;
	ret = ret && (NetVol == ct.NetVol);
	if (!ret) return ret;
	ret = ret && (TradeCount == ct.TradeCount);
	if (!ret) return ret;
	ret = ret && (TotalCount == ct.TotalCount);
	if (!ret) return ret;

	return ret;
}
};

///灾备子系统编号
struct DRSysIDField
{
	///灾备子系统编号(每个子系统中心一个编号)
	TDRSysIDType	DRSysID;

bool operator==(const DRSysIDField &ct) const
{
	bool ret = true;
	ret = ret && (DRSysID == ct.DRSysID);
	if (!ret) return ret;

	return ret;
}
};

///查询投资者保证金率
struct QryInvestorMarginRateField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;

bool operator==(const QryInvestorMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;

	return ret;
}
};

///投资者保证金率
struct InvestorMarginRateField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;

bool operator==(const InvestorMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};

///压力测试结算价格
struct STPriceField
{
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///投资者代码或模板代码
	TInvestorIDType	InvestorID;
	///合约代码
	TInstrumentIDType	InstrumentID;
	///价格类型
	TPriceTypeType	PriceType;
	///价格
	TPriceType	Price;

bool operator==(const STPriceField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (PriceType == ct.PriceType);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(Price, ct.Price));
	if (!ret) return ret;

	return ret;
}
};

///压力测试保证金率
struct STMarginRateField
{
	///合约代码
	TInstrumentIDType	InstrumentID;
	///投资者范围
	TInvestorRangeType	InvestorRange;
	///经纪公司代码
	TBrokerIDType	BrokerID;
	///投资者代码
	TInvestorIDType	InvestorID;
	///投机套保标志
	THedgeFlagType	HedgeFlag;
	///多头保证金率
	TRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TMoneyType	ShortMarginRatioByVolume;

bool operator==(const STMarginRateField &ct) const
{
	bool ret = true;
	ret = ret && (strcmp(InstrumentID, ct.InstrumentID) == 0);
	if (!ret) return ret;
	ret = ret && (InvestorRange == ct.InvestorRange);
	if (!ret) return ret;
	ret = ret && (strcmp(BrokerID, ct.BrokerID) == 0);
	if (!ret) return ret;
	ret = ret && (strcmp(InvestorID, ct.InvestorID) == 0);
	if (!ret) return ret;
	ret = ret && (HedgeFlag == ct.HedgeFlag);
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByMoney, ct.LongMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(LongMarginRatioByVolume, ct.LongMarginRatioByVolume));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByMoney, ct.ShortMarginRatioByMoney));
	if (!ret) return ret;
	ret = ret && (EQUALDOUBLE(ShortMarginRatioByVolume, ct.ShortMarginRatioByVolume));
	if (!ret) return ret;

	return ret;
}
};



#endif
