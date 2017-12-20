#include "stdafx.h"
#include "Real_DBOpr.h"
// #include "..\SvrDBOpr\Interface_SvrDBOpr.h"
#include "commonSqlScript.h"

//创建表或者分区
//如果表不存在则建表和今日的分区
//如果表存在，并且今日的分区不存在，则追加今日的分区
CF_ERROR CRealDBOpr::CreateMyTableAndPartitions(const std::string & strDate)
{



	char  lps[4000] = {0};
	sprintf_s(lps,gSqlScriptOrderFormat,TABLENAME_TRADEDATA_ORDERS,
		TABLENAME_TRADEDATA_ORDERS,
		TABLENAME_TRADEDATA_ORDERS,
		TABLENAME_TRADEDATA_ORDERS,
		TABLENAME_TRADEDATA_ORDERS,
		TABLENAME_TRADEDATA_ORDERS,
		TABLENAME_TRADEDATA_ORDERS,
		TABLENAME_TRADEDATA_ORDERS
		);
	int nErrorCode = CF_ERROR_SUCCESS;
	if ( !CInterface_SvrDBOpr::getObj().Excute(lps,nErrorCode))
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	return CF_ERROR_SUCCESS;
}


CF_ERROR CRealDBOpr::LoadTodayInstrument(const std::string & strTime, std::vector<PlatformStru_InstrumentInfo> & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from \"TRADEDATA_INSTRUEMENTS\" t where t.ValidateDate = \'%s\'",strTime.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_InstrumentInfo lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
			/*if ( vValue.size() != 26)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				/*
				"INSTRUMENTID"       VARCHAR2(31),
				"EXCHANGEID"         VARCHAR2(9),
				"INSTRUMENTNAME"     VARCHAR2(21),
				"EXCHANGEINSTID"     VARCHAR2(31),
				"PRODUCTID"          VARCHAR2(31),
				"PRODUCTCLASS"       CHAR,
				"DELIVERYYEAR"       INTEGER,
				"DELIVERYMONTH"      INTEGER,
				"MAXMARKETORDERVOLUME" INTEGER,
				"MINMARKETORDERVOLUME" INTEGER,
				"MAXLIMITORDERVOLUME" INTEGER,
				"MINLIMITORDERVOLUME" INTEGER,
				"VOLUMEMULTIPLE"     INTEGER,
				"PRICETICK"          DOUBLE PRECISION,
				"CREATEDATE"         VARCHAR2(9),
				"OPENDATE"           VARCHAR2(9),
				"EXPIREDATE"         VARCHAR2(9),
				"STARTDELIVDATE"     VARCHAR2(9),
				"ENDDELIVDATE"       VARCHAR2(9),
				"INSTLIFEPHASE"      CHAR,
				"ISTRADING"          INTEGER,
				"POSITIONTYPE"       CHAR,
				"POSITIONDATETYPE"   CHAR,
				"LONGMARGINRATIO"    DOUBLE PRECISION,
				"SHORTMARGINRATIO"   DOUBLE PRECISION,
				*/
				int i = 0;
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());			
				strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.InstrumentName, vValue[i++].operator _bstr_t());			
				strcpy(lTempInfp.ExchangeInstID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ProductID, vValue[i++].operator _bstr_t());
				lTempInfp.ProductClass = vValue[i++].cVal;
				lTempInfp.DeliveryYear = vValue[i++].intVal;
				lTempInfp.DeliveryMonth = vValue[i++].intVal;
				lTempInfp.MaxMarketOrderVolume = vValue[i++].intVal;
				lTempInfp.MinMarketOrderVolume = vValue[i++].intVal;
				lTempInfp.MaxLimitOrderVolume = vValue[i++].intVal;
				lTempInfp.MinLimitOrderVolume = vValue[i++].intVal;
				lTempInfp.VolumeMultiple = vValue[i++].intVal;
				lTempInfp.PriceTick = vValue[i++].dblVal;
				strcpy(lTempInfp.CreateDate, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.OpenDate, vValue[i++].operator _bstr_t());			
				strcpy(lTempInfp.ExpireDate, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.StartDelivDate, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.EndDelivDate, vValue[i++].operator _bstr_t());
				lTempInfp.InstLifePhase = vValue[i++].cVal;
				lTempInfp.IsTrading = vValue[i++].intVal;
				lTempInfp.PositionType = vValue[i++].cVal;
				lTempInfp.PositionDateType = vValue[i++].cVal;
				lTempInfp.LongMarginRatio = vValue[i++].dblVal;
				lTempInfp.ShortMarginRatio = vValue[i++].dblVal;
				lTempInfp.MaxMarginSideAlgorithm = vValue[i++].cVal;
				lTempInfp.UnwindPriorities = vValue[i++].intVal;
				lTempInfp.PriceForMarginOfTodayPosition = vValue[i++].intVal;
				lTempInfp.CloseTodayInstructionSupport = vValue[i++].intVal;
				lTempInfp.CloseInstructionSupport = vValue[i++].intVal;
				strcpy(lTempInfp.Currency, vValue[i++].operator _bstr_t());
				lTempInfp.TicksPerPoint = vValue[i++].intVal;
				strcpy(lTempInfp.TickSize, vValue[i++].operator _bstr_t());

				nInfo.push_back(lTempInfp);
			}
		}
	}


	return CF_ERROR_SUCCESS;
}
CF_ERROR CRealDBOpr::WriteIntrument(const std::string & strTime, const std::vector<PlatformStru_InstrumentInfo> & nInfo) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveInstruments(strTime,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}



CF_ERROR CRealDBOpr::LoadTodayCommission(const std::string & nsBrokerID,
										 const std::string & nsAccountID,
										 const std::string & strTime, 
										 std::vector<PlatformStru_InstrumentCommissionRate> & nInfo) 
{
    int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from \"TRADEDATA_COMMISSION\" t where t.ValidateDate = \'%s\' AND BrokerID = \'%s\' AND InvestorID = \'%s\'",
		strTime.c_str(),
		nsBrokerID.c_str(),
		nsAccountID.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_InstrumentCommissionRate lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				/*
				"INSTRUMENTID"       VARCHAR2(31),
				"INVESTORRANGE"      CHAR,
				"BROKERID"           VARCHAR2(11),
				"INVESTORID"         VARCHAR2(13),
				"OPENRATIOBYMONEY"   DOUBLE PRECISION,
				"OPENRATIOBYVOLUME"  DOUBLE PRECISION,
				"CLOSERATIOBYMONEY"  DOUBLE PRECISION,
				"CLOSERATIOBYVOLUME" DOUBLE PRECISION,
				"CLOSETODAYRATIOBYMONEY" DOUBLE PRECISION,
				"CLOSETODAYRATIOBYVOLUME" DOUBLE PRECISION,
				*/
				int i = 0;
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());			
				lTempInfp.InvestorRange = vValue[i++].cVal;
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());					
				lTempInfp.OpenRatioByMoney = vValue[i++].dblVal;
				lTempInfp.OpenRatioByVolume = vValue[i++].dblVal;
				lTempInfp.CloseRatioByMoney = vValue[i++].dblVal;
				lTempInfp.CloseRatioByVolume = vValue[i++].dblVal;
				lTempInfp.CloseTodayRatioByMoney = vValue[i++].dblVal;
				lTempInfp.CloseTodayRatioByVolume = vValue[i++].dblVal;
				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::WriteCommission(const std::string & nsBrokerID,
									 const std::string & nsAccountID,
									 const std::string & strTime, 
									 const std::vector<PlatformStru_InstrumentCommissionRate> & nInfo) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveAccountCommissionRate("TRADEDATA_COMMISSION",strTime,nsBrokerID,nsAccountID,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}


CF_ERROR CRealDBOpr::LoadTodayMarginRate(const std::string & nsBrokerID,
										 const std::string & nsAccountID,
										 const std::string & strTime, 
										 std::vector<PlatformStru_InstrumentMarginRate> & nInfo) 
{
	 int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from \"TRADEDATA_MARGINRATE\" t where t.ValidateDate = \'%s\'\
		AND BrokerID = \'%s\' AND InvestorID = \'%s\' ",strTime.c_str(),nsBrokerID.c_str(),nsAccountID.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_InstrumentMarginRate lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				/*
				"VALIDATEDATE"       DATE,
				"INSTRUMENTID"       VARCHAR2(31),
				"INVESTORRANGE"      CHAR,
				"BROKERID"           VARCHAR2(11),
				"INVESTORID"         VARCHAR2(13),
				"HEDGEFLAG"          CHAR,
				"LONGMARGINRATIOBYMONEY" DOUBLE PRECISION,
				"LONGMARGINRATIOBYVOLUME" DOUBLE PRECISION,
				"SHORTMARGINRATIOBYMONEY" DOUBLE PRECISION,
				"SHORTMARGINRATIOBYVOLUME" DOUBLE PRECISION,
				"ISRELATIVE"         INTEGER,
				*/
				int i = 0;
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());			
				lTempInfp.InvestorRange = vValue[i++].cVal;
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());					
				lTempInfp.HedgeFlag = vValue[i++].cVal;
				lTempInfp.LongMarginRatioByMoney = vValue[i++].dblVal;
				lTempInfp.LongMarginRatioByVolume = vValue[i++].dblVal;
				lTempInfp.ShortMarginRatioByMoney = vValue[i++].dblVal;
				lTempInfp.ShortMarginRatioByVolume = vValue[i++].dblVal;
				lTempInfp.IsRelative = vValue[i++].intVal;
				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::WriteMarginRate(const std::string & nsBrokerID,
									 const std::string & nsAccountID,
									 const std::string & strTime,
									 const std::vector<PlatformStru_InstrumentMarginRate> & nInfo) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveAccountMarginRate("TRADEDATA_MARGINRATE",strTime,nsBrokerID,nsAccountID,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::WriteOrderInfos(const std::string & nsTableName,
									 const std::string & nsUserName,
									 const std::string & strTime,
									 const std::vector<PlatformStru_OrderInfo> & nInfo,
									 bool nbDelete) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveUserOrderInfos(nsTableName,nbDelete,strTime,nsUserName,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::WriteTradeInfos(const std::string & nsTableName,
								 const std::string & nsUserName,
								 const std::string & strTime,
								 const std::vector<PlatformStru_TradeInfo> & nInfo,
								 bool nbDelete) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveUserTraderInfos(nsTableName,nbDelete,strTime,nsUserName,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::WritePositionInfos(const std::string & nsTableName,
									const std::string & nsUserName,
									const std::string & strTime,
									const std::vector<PlatformStru_Position> & nInfo,
									bool nbDelete) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveUserPositionInfos(nsTableName,nbDelete,strTime,nsUserName,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::WritePositionDetailInfos(const std::string & nsTableName,
										  const std::string & nsUserName,
										  const std::string & strTime,
										  const std::vector<PlatformStru_PositionDetail> & nInfo,
										  bool nbDelete) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveUserPositionDetailInfos(nsTableName,nbDelete,strTime,nsUserName,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::WriteFundInfos(const std::string & nsTableName,
									  const std::string & nsUserName,
									  const std::string & strTime,
									  const PlatformStru_TradingAccountInfo & nInfo) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveUserFundInfos(nsTableName,strTime,nsUserName,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}

//写行情
CF_ERROR CRealDBOpr::WriteQutoInfo(const std::string & nsTableName,
							   const std::string & strTime,
							   const std::vector<PlatformStru_DepthMarketData> & nInfo) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveDepthMarketDataWithDelete(nsTableName,strTime,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}

//追加一条结算行情
CF_ERROR CRealDBOpr::AddQutoInfo(const std::string & nsTableName,
							 const std::string & strTime,
							 const PlatformStru_DepthMarketData & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "update %s t set t.SETTLEMENTPRICE = %f where t.ValidateDate = \'%s\' AND t.instrumentid =  \'%s\' ",
		nsTableName.c_str(),nInfo.SettlementPrice, strTime.c_str(),nInfo.InstrumentID);
	int nNum = 0;
	CInterface_SvrDBOpr::getObj().ExcuteUpdate(szBuffer, nNum, nErrorCode);
	
	{
		if(nNum == 0)
		{
			//更新失败则追加
			std::vector<PlatformStru_DepthMarketData> lVecinfo;
			lVecinfo.push_back(nInfo);
			int nErrorCode = 0;
			if ( !CInterface_SvrDBOpr::getObj().SaveDepthMarketData(nsTableName,strTime,lVecinfo,nErrorCode))
				return CF_ERROR_DATABASE_NOT_CONNECT;

			return CF_ERROR_SUCCESS;
		}
		return CF_ERROR_SUCCESS;
	}
}


//写出入金
CF_ERROR CRealDBOpr::WriteFundInOut(const sFundInOut & s) 
{
	if ( !CInterface_SvrDBOpr::getObj().SaveFundInOut(s))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}

//写初始资金
CF_ERROR CRealDBOpr::WriteInitFund(const std::string & nsTableName,
							   const std::string & nsUserName,
							   const double & ndbVal)
{
	if ( !CInterface_SvrDBOpr::getObj().SaveInitFund(nsTableName,nsUserName,ndbVal))
		return CF_ERROR_DATABASE_NOT_CONNECT;

	return CF_ERROR_SUCCESS;
}



//装载用户的报单
CF_ERROR CRealDBOpr::LoadQutoInfo(const std::string & nsTableName,										
										 const std::string & strTime, 
										 std::vector<PlatformStru_DepthMarketData> & nInfo) 
{
    int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\' ",
		nsTableName.c_str(), strTime.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_DepthMarketData lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));
			std::vector<_variant_t> vValue = vNode[j];
			
			{
				
				int i = 0;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.ExchangeInstID, vValue[i++].operator _bstr_t());	
			
				lTempInfp.LastPrice = vValue[i++].dblVal;
				lTempInfp.PreSettlementPrice = vValue[i++].dblVal;
				lTempInfp.PreClosePrice = vValue[i++].dblVal;
				lTempInfp.PreOpenInterest = vValue[i++].dblVal;
				lTempInfp.OpenPrice = vValue[i++].dblVal;
				lTempInfp.HighestPrice = vValue[i++].dblVal;
				lTempInfp.LowestPrice = vValue[i++].dblVal;
				lTempInfp.Volume = vValue[i++].intVal;
				lTempInfp.Turnover = vValue[i++].dblVal;
				lTempInfp.OpenInterest = vValue[i++].dblVal;
				lTempInfp.ClosePrice = vValue[i++].dblVal;
				lTempInfp.SettlementPrice = vValue[i++].dblVal;
				lTempInfp.UpperLimitPrice = vValue[i++].dblVal;
				lTempInfp.LowerLimitPrice = vValue[i++].dblVal;
				lTempInfp.PreDelta = vValue[i++].dblVal;
				lTempInfp.CurrDelta = vValue[i++].dblVal;

				strcpy(lTempInfp.UpdateTime, vValue[i++].operator _bstr_t());	
				lTempInfp.UpdateMillisec = vValue[i++].intVal;

				lTempInfp.BidPrice1 = vValue[i++].dblVal;
				lTempInfp.BidVolume1 = vValue[i++].intVal;
				lTempInfp.AskPrice1 = vValue[i++].dblVal;
				lTempInfp.AskVolume1 = vValue[i++].intVal;
				lTempInfp.BidPrice2 = vValue[i++].dblVal;
				lTempInfp.BidVolume2 = vValue[i++].intVal;
				lTempInfp.AskPrice2 = vValue[i++].dblVal;
				lTempInfp.AskVolume2 = vValue[i++].intVal;
				lTempInfp.BidPrice3 = vValue[i++].dblVal;
				lTempInfp.BidVolume3 = vValue[i++].intVal;
				lTempInfp.AskPrice4 = vValue[i++].dblVal;
				lTempInfp.AskVolume3 = vValue[i++].intVal;
				lTempInfp.BidPrice4 = vValue[i++].dblVal;
				lTempInfp.BidVolume4 = vValue[i++].intVal;
				lTempInfp.AskPrice4 = vValue[i++].dblVal;
				lTempInfp.AskVolume4 = vValue[i++].intVal;
				lTempInfp.BidPrice5 = vValue[i++].dblVal;
				lTempInfp.BidVolume5 = vValue[i++].intVal;
				lTempInfp.AskPrice5 = vValue[i++].dblVal;
				lTempInfp.AskVolume5 = vValue[i++].intVal;

				lTempInfp.AveragePrice = vValue[i++].dblVal;

				lTempInfp.BidPrice6 = vValue[i++].dblVal;
				lTempInfp.BidVolume6 = vValue[i++].intVal;
				lTempInfp.AskPrice6 = vValue[i++].dblVal;
				lTempInfp.AskVolume6 = vValue[i++].intVal;
				lTempInfp.BidPrice7 = vValue[i++].dblVal;
				lTempInfp.BidVolume7 = vValue[i++].intVal;
				lTempInfp.AskPrice7 = vValue[i++].dblVal;
				lTempInfp.AskVolume7 = vValue[i++].intVal;
				lTempInfp.BidPrice8 = vValue[i++].dblVal;
				lTempInfp.BidVolume8 = vValue[i++].intVal;
				lTempInfp.AskPrice8 = vValue[i++].dblVal;
				lTempInfp.AskVolume8 = vValue[i++].intVal;
				lTempInfp.BidPrice9 = vValue[i++].dblVal;
				lTempInfp.BidVolume9 = vValue[i++].intVal;
				lTempInfp.AskPrice9 = vValue[i++].dblVal;
				lTempInfp.AskVolume9 = vValue[i++].intVal;
				lTempInfp.BidPrice10 = vValue[i++].dblVal;
				lTempInfp.BidVolume10 = vValue[i++].intVal;
				lTempInfp.AskPrice10 = vValue[i++].dblVal;
				lTempInfp.AskVolume10 = vValue[i++].intVal;
				lTempInfp.NewVolume = vValue[i++].intVal;				
				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

//装载用户的报单
CF_ERROR CRealDBOpr::LoadTodayUserOrders(const std::string & nsTableName,
									 const std::string & nsUserName,
									 const std::string & strTime, 
									 std::vector<PlatformStru_OrderInfo> & nInfo) 
{
	 int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\'\
					  AND InvestorID = \'%s\' order by updateseq",nsTableName.c_str(), strTime.c_str(),nsUserName.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_OrderInfo lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
			/*if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				
				int i = 0;
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.OrderRef, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.UserID, vValue[i++].operator _bstr_t()); 
				lTempInfp.OrderPriceType = vValue[i++].cVal;
				lTempInfp.Direction = vValue[i++].cVal;
				strcpy(lTempInfp.CombOffsetFlag, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.CombHedgeFlag, vValue[i++].operator _bstr_t());
				lTempInfp.LimitPrice = vValue[i++].dblVal;
				lTempInfp.VolumeTotalOriginal = vValue[i++].intVal;
				lTempInfp.TimeCondition = vValue[i++].cVal;
				strcpy(lTempInfp.GTDDate, vValue[i++].operator _bstr_t());
				lTempInfp.VolumeCondition = vValue[i++].cVal;
				lTempInfp.MinVolume = vValue[i++].intVal;
				lTempInfp.ContingentCondition = vValue[i++].cVal;
				lTempInfp.StopPrice = vValue[i++].dblVal;
				lTempInfp.ForceCloseReason = vValue[i++].cVal;
				lTempInfp.IsAutoSuspend = vValue[i++].intVal;
				strcpy(lTempInfp.BusinessUnit, vValue[i++].operator _bstr_t());
				lTempInfp.RequestID = vValue[i++].intVal;
				strcpy(lTempInfp.OrderLocalID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ParticipantID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ClientID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ExchangeInstID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.TraderID, vValue[i++].operator _bstr_t());
				lTempInfp.InstallID = vValue[i++].intVal;
				lTempInfp.OrderSubmitStatus = vValue[i++].cVal;
				lTempInfp.NotifySequence = vValue[i++].intVal;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t());
				lTempInfp.SettlementID = vValue[i++].intVal;
				strcpy(lTempInfp.OrderSysID, vValue[i++].operator _bstr_t());
				lTempInfp.OrderSource = vValue[i++].cVal;
				lTempInfp.OrderStatus = vValue[i++].cVal;
				lTempInfp.OrderType = vValue[i++].cVal;
				lTempInfp.VolumeTraded = vValue[i++].intVal;
				lTempInfp.VolumeTotal = vValue[i++].intVal;
				strcpy(lTempInfp.InsertDate, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.InsertTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ActiveTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.SuspendTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.UpdateTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.CancelTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ActiveTraderID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ClearingPartID, vValue[i++].operator _bstr_t());
				lTempInfp.SequenceNo = vValue[i++].intVal;
				lTempInfp.FrontID = vValue[i++].intVal;
				lTempInfp.SessionID = vValue[i++].intVal;
				strcpy(lTempInfp.UserProductInfo, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.StatusMsg, vValue[i++].operator _bstr_t());
				lTempInfp.UserForceClose = vValue[i++].intVal;
				strcpy(lTempInfp.ActiveUserID, vValue[i++].operator _bstr_t());
				lTempInfp.BrokerOrderSeq = vValue[i++].intVal;
				strcpy(lTempInfp.RelativeOrderSysID, vValue[i++].operator _bstr_t());
				lTempInfp.AvgPrice = vValue[i++].dblVal;
				lTempInfp.ExStatus = (PlatformStru_OrderInfo::EnumExStatus)vValue[i++].intVal;	
				lTempInfp.FTID = vValue[i++].intVal;	
				lTempInfp.UpdateSeq = vValue[i++].intVal;	

				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;

}

//装载用户的成交
CF_ERROR CRealDBOpr::LoadTodayUserTrades(const std::string & nsTableName,
									 const std::string & nsUserName,
									 const std::string & strTime, 
									 std::vector<PlatformStru_TradeInfo> & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\' \
					  AND InvestorID = \'%s\' order by tradetime",nsTableName.c_str(), strTime.c_str(),nsUserName.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_TradeInfo lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.OrderRef, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.UserID, vValue[i++].operator _bstr_t()); 				
				strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.TradeID, vValue[i++].operator _bstr_t());
				lTempInfp.Direction = vValue[i++].cVal;
				strcpy(lTempInfp.OrderSysID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ParticipantID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ClientID, vValue[i++].operator _bstr_t());
				lTempInfp.TradingRole= vValue[i++].cVal;
				strcpy(lTempInfp.ExchangeInstID, vValue[i++].operator _bstr_t());
				lTempInfp.OffsetFlag= vValue[i++].cVal;
				lTempInfp.HedgeFlag= vValue[i++].cVal;
				lTempInfp.Price = vValue[i++].dblVal;
				lTempInfp.Volume = vValue[i++].intVal;
				strcpy(lTempInfp.TradeDate, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.TradeTime, vValue[i++].operator _bstr_t());
				lTempInfp.TradeType= vValue[i++].cVal;
				lTempInfp.PriceSource= vValue[i++].cVal;
				strcpy(lTempInfp.TraderID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.OrderLocalID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ClearingPartID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.BusinessUnit, vValue[i++].operator _bstr_t());
				lTempInfp.SequenceNo = vValue[i++].intVal;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t());
				lTempInfp.SettlementID = vValue[i++].intVal;
				lTempInfp.BrokerOrderSeq = vValue[i++].intVal;
				lTempInfp.TradeSource= vValue[i++].cVal;	
				lTempInfp.CloseProfitByDate = vValue[i++].dblVal;
				lTempInfp.CloseProfitByTrade = vValue[i++].dblVal;
				lTempInfp.TradeCommission = vValue[i++].dblVal;
				lTempInfp.FTID = vValue[i++].intVal;	
				lTempInfp.UpdateSeq = vValue[i++].intVal;	

				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

//装载用户的持仓
CF_ERROR CRealDBOpr::LoadTodayUserPositions(const std::string & nsTableName,
										const std::string & nsUserName,
										const std::string & strTime, 
										std::vector<PlatformStru_Position> & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\' \
					  AND InvestorID = \'%s\'",nsTableName.c_str(), strTime.c_str(),nsUserName.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_Position lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());					
				lTempInfp.PosiDirection = vValue[i++].cVal;
				lTempInfp.HedgeFlag= vValue[i++].cVal;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t());				
				lTempInfp.PositionDate= vValue[i++].cVal;
				lTempInfp.SettlementID = vValue[i++].intVal;
				lTempInfp.Position = vValue[i++].intVal;
				lTempInfp.TodayPosition = vValue[i++].intVal;
				lTempInfp.YdPosition = vValue[i++].intVal;
				lTempInfp.OpenVolume = vValue[i++].intVal;
				lTempInfp.CloseVolume = vValue[i++].intVal;
				lTempInfp.OpenAmount = vValue[i++].dblVal;
				lTempInfp.CloseAmount = vValue[i++].dblVal;
				lTempInfp.PositionCost = vValue[i++].dblVal;
				lTempInfp.OpenCost = vValue[i++].dblVal;
				lTempInfp.LongFrozen = vValue[i++].intVal;
				lTempInfp.ShortFrozen = vValue[i++].intVal;
				lTempInfp.LongFrozenAmount = vValue[i++].dblVal;
				lTempInfp.ShortFrozenAmount = vValue[i++].dblVal;
				lTempInfp.FrozenMargin = vValue[i++].dblVal;
				lTempInfp.FrozenCommission = vValue[i++].dblVal;
				lTempInfp.FrozenCash = vValue[i++].dblVal;
				lTempInfp.Commission = vValue[i++].dblVal;
				lTempInfp.PreMargin = vValue[i++].dblVal;
				lTempInfp.UseMargin = vValue[i++].dblVal;
				lTempInfp.ExchangeMargin = vValue[i++].dblVal;
				lTempInfp.MarginRateByMoney = vValue[i++].dblVal;
				lTempInfp.MarginRateByVolume = vValue[i++].dblVal;
				lTempInfp.CashIn = vValue[i++].dblVal;
				lTempInfp.PositionProfit = vValue[i++].dblVal;
				lTempInfp.CloseProfit = vValue[i++].dblVal;
				lTempInfp.CloseProfitByDate = vValue[i++].dblVal;
				lTempInfp.CloseProfitByTrade = vValue[i++].dblVal;
				lTempInfp.PreSettlementPrice = vValue[i++].dblVal;
				lTempInfp.SettlementPrice = vValue[i++].dblVal;
				lTempInfp.CombPosition = vValue[i++].intVal;
				lTempInfp.CombLongFrozen = vValue[i++].intVal;
				lTempInfp.CombShortFrozen = vValue[i++].intVal;
				lTempInfp.PositionProfitByTrade = vValue[i++].dblVal;
				lTempInfp.TotalPositionProfitByDate = vValue[i++].dblVal;
				lTempInfp.FTID = vValue[i++].intVal;	
				lTempInfp.UpdateSeq = vValue[i++].intVal;	

				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

//装载用户的持仓明细
CF_ERROR CRealDBOpr::LoadTodayUserPositionDetails(const std::string & nsTableName,
											  const std::string & nsUserName,
											  const std::string & strTime, 
											  std::vector<PlatformStru_PositionDetail> & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\' \
					  AND InvestorID = \'%s\'",nsTableName.c_str(), strTime.c_str(),nsUserName.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_PositionDetail lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());			
				lTempInfp.HedgeFlag= vValue[i++].cVal;	
				lTempInfp.Direction = vValue[i++].cVal;
				strcpy(lTempInfp.OpenDate, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.TradeID, vValue[i++].operator _bstr_t()); 
				lTempInfp.Volume = vValue[i++].intVal;			
				lTempInfp.OpenPrice = vValue[i++].dblVal;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t()); 
				lTempInfp.SettlementID = vValue[i++].intVal;
				lTempInfp.TradeType = vValue[i++].cVal;
				strcpy(lTempInfp.CombInstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t()); 

				lTempInfp.CloseProfitByDate = vValue[i++].dblVal;
				lTempInfp.CloseProfitByTrade = vValue[i++].dblVal;
				lTempInfp.PositionProfitByDate = vValue[i++].dblVal;
				lTempInfp.PositionProfitByTrade = vValue[i++].dblVal;

				lTempInfp.Margin = vValue[i++].dblVal;
				lTempInfp.ExchMargin = vValue[i++].dblVal;
				lTempInfp.MarginRateByMoney = vValue[i++].dblVal;
				lTempInfp.MarginRateByVolume = vValue[i++].dblVal;
				lTempInfp.LastSettlementPrice = vValue[i++].dblVal;
				lTempInfp.SettlementPrice = vValue[i++].dblVal;			
				lTempInfp.CloseVolume = vValue[i++].intVal;			
				lTempInfp.CloseAmount = vValue[i++].dblVal;	
				lTempInfp.FTID = vValue[i++].intVal;	
				lTempInfp.UpdateSeq = vValue[i++].intVal;	

				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

//装载用户的资金
CF_ERROR CRealDBOpr::LoadTodayUserFundInfo(const std::string & nsTableName,
									   const std::string & nsUserName,
									   const std::string & strTime, 
									   PlatformStru_TradingAccountInfo & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\'\
					  AND AccountID = \'%s\'",nsTableName.c_str(), strTime.c_str(),nsUserName.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			
			memset(&nInfo, 0, sizeof(nInfo));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;			
				strcpy(nInfo.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(nInfo.AccountID, vValue[i++].operator _bstr_t());			
				nInfo.PreMortgage = vValue[i++].dblVal;
				nInfo.PreCredit = vValue[i++].dblVal;
				nInfo.PreDeposit = vValue[i++].dblVal;
				nInfo.PreBalance = vValue[i++].dblVal;
				nInfo.PreMargin = vValue[i++].dblVal;
				nInfo.InterestBase = vValue[i++].dblVal;
				nInfo.Interest = vValue[i++].dblVal;
				nInfo.Deposit = vValue[i++].dblVal;
				nInfo.Withdraw = vValue[i++].dblVal;
				nInfo.FrozenMargin = vValue[i++].dblVal;			
				nInfo.FrozenCash = vValue[i++].dblVal;			
				nInfo.FrozenCommission = vValue[i++].dblVal;	
				nInfo.CurrMargin = vValue[i++].dblVal;
				nInfo.CashIn = vValue[i++].dblVal;
				nInfo.Commission = vValue[i++].dblVal;
				nInfo.CloseProfit = vValue[i++].dblVal;
				nInfo.PositionProfit = vValue[i++].dblVal;			
				nInfo.Balance = vValue[i++].dblVal;			
				nInfo.Available = vValue[i++].dblVal;	
				nInfo.WithdrawQuota = vValue[i++].dblVal;			
				nInfo.Reserve = vValue[i++].dblVal;	
				strcpy(nInfo.TradingDay, vValue[i++].operator _bstr_t());	
				nInfo.SettlementID = vValue[i++].intVal;	
				nInfo.Credit = vValue[i++].dblVal;
				nInfo.Mortgage = vValue[i++].dblVal;
				nInfo.ExchangeMargin = vValue[i++].dblVal;			
				nInfo.DeliveryMargin = vValue[i++].dblVal;			
				nInfo.ExchangeDeliveryMargin = vValue[i++].dblVal;	
				nInfo.StaticProfit = vValue[i++].dblVal;			
				nInfo.DynamicProfit = vValue[i++].dblVal;	
				nInfo.RiskDegree = vValue[i++].dblVal;	

			
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

//查询出入金
CF_ERROR CRealDBOpr::LoadQryFundInOut(const sQryFundInOut & nQryFund,
									  std::vector<sFundInOut> & nvecFund) 
{

	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	if(strcmp(nQryFund.mUserID,"*") == 0)
		sprintf(szBuffer, "select * from TradeData_FundInOut t where \
						  OPDATE between \'%s\' and \'%s\' order by OPDATE desc",
						  nQryFund.msStartDay,
						  nQryFund.msEndDay);
	else
		sprintf(szBuffer, "select * from TradeData_FundInOut t where \
						  (OPDATE between \'%s\' and \'%s\') \
						  AND UserID=\'%s\' order by OPDATE desc",
						  nQryFund.msStartDay,
						  nQryFund.msEndDay,
						  nQryFund.mUserID);
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			sFundInOut nInfo;
			memset(&nInfo, 0, sizeof(nInfo));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;			
				strcpy(nInfo.mUserID, vValue[i++].operator _bstr_t());	
				nInfo.meInOut = (eInOut)vValue[i++].intVal;	
				nInfo.mdbVolume = vValue[i++].dblVal;
				strcpy(nInfo.mOpAdminID, vValue[i++].operator _bstr_t());			
				strcpy(nInfo.msDesc, vValue[i++].operator _bstr_t());	
				strcpy(nInfo.msDay, vValue[i++].operator _bstr_t());	
				strcpy(nInfo.msTime, vValue[i++].operator _bstr_t());
			
			}
			nvecFund.push_back(nInfo);
		}
	}
	return CF_ERROR_SUCCESS;
}

//查询初始总权益
CF_ERROR CRealDBOpr::LoadUserInitFund(const std::string & nsTableName,
								  const std::string & nsUserName,
								  double  & ndbVal)
{
    int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where UserID=\'%s\' ",
					  nsTableName.c_str(),
					  nsUserName.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{		
			std::vector<_variant_t> vValue = vNode[j];
			ndbVal = vValue[1].dblVal;				
		}
	}
	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::LoadUserSettlementConfirm(const std::string & strValidateTime,
										   const std::string & nsUserName,
										   std::string & nsConfirmDate,
										   std::string & nsConfirmTime)
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from TRADEDATA_SETTLEMENTCONFIRM t where UserID=\'%s\' and ValidateDate = \'%s\' ",
		nsUserName.c_str(),
		strValidateTime.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{		
			std::vector<_variant_t> vValue = vNode[j];
			int i = 1;	
			nsConfirmDate =  vValue[i++].operator _bstr_t();
			nsConfirmTime = vValue[i++].operator _bstr_t();		
		}
	}
	return CF_ERROR_SUCCESS;
}

CF_ERROR CRealDBOpr::AddUserSettlementConfirm(const std::string & strValidateTime,
										  const std::string & nsUserName,
										  const std::string & nsConfirmDate,
										  const std::string & nsConfirmTime) 
{

	bool lbRet = false;
	char szBuffer[MAX_SQL_LENGTH];	
	sprintf(szBuffer, "insert into TRADEDATA_SETTLEMENTCONFIRM values(\
					  \'%s\',\'%s\',\'%s\',\'%s\',sysdate)",
					 nsUserName.c_str(), 
					 nsConfirmDate.c_str(),
					 nsConfirmTime.c_str(),
					 strValidateTime.c_str());
	int nErrorCode;
	if( CInterface_SvrDBOpr::getObj().Excute(szBuffer, nErrorCode))
	{
		lbRet = true;
	}


	return lbRet;
}


//装载用户的报单
CF_ERROR CRealDBOpr::LoadTodayAccountOrders(const std::string & nsTableName,
									 const std::string & nsBrokerID,
									 const std::string & nsInvestorID,
									 const std::string & strTime, 
									 std::vector<PlatformStru_OrderInfo> & nInfo) 
{
	 int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\'\
					  AND InvestorID = \'%s\'  AND BrokerID = \'%s\'",nsTableName.c_str(), strTime.c_str(),nsInvestorID.c_str(),nsBrokerID.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_OrderInfo lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
			/*if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{
				
				int i = 0;
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.OrderRef, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.UserID, vValue[i++].operator _bstr_t()); 
				lTempInfp.OrderPriceType = vValue[i++].cVal;
				lTempInfp.Direction = vValue[i++].cVal;
				strcpy(lTempInfp.CombOffsetFlag, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.CombHedgeFlag, vValue[i++].operator _bstr_t());
				lTempInfp.LimitPrice = vValue[i++].dblVal;
				lTempInfp.VolumeTotalOriginal = vValue[i++].intVal;
				lTempInfp.TimeCondition = vValue[i++].cVal;
				strcpy(lTempInfp.GTDDate, vValue[i++].operator _bstr_t());
				lTempInfp.VolumeCondition = vValue[i++].cVal;
				lTempInfp.MinVolume = vValue[i++].intVal;
				lTempInfp.ContingentCondition = vValue[i++].cVal;
				lTempInfp.StopPrice = vValue[i++].dblVal;
				lTempInfp.ForceCloseReason = vValue[i++].cVal;
				lTempInfp.IsAutoSuspend = vValue[i++].intVal;
				strcpy(lTempInfp.BusinessUnit, vValue[i++].operator _bstr_t());
				lTempInfp.RequestID = vValue[i++].intVal;
				strcpy(lTempInfp.OrderLocalID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ParticipantID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ClientID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ExchangeInstID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.TraderID, vValue[i++].operator _bstr_t());
				lTempInfp.InstallID = vValue[i++].intVal;
				lTempInfp.OrderSubmitStatus = vValue[i++].cVal;
				lTempInfp.NotifySequence = vValue[i++].intVal;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t());
				lTempInfp.SettlementID = vValue[i++].intVal;
				strcpy(lTempInfp.OrderSysID, vValue[i++].operator _bstr_t());
				lTempInfp.OrderSource = vValue[i++].cVal;
				lTempInfp.OrderStatus = vValue[i++].cVal;
				lTempInfp.OrderType = vValue[i++].cVal;
				lTempInfp.VolumeTraded = vValue[i++].intVal;
				lTempInfp.VolumeTotal = vValue[i++].intVal;
				strcpy(lTempInfp.InsertDate, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.InsertTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ActiveTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.SuspendTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.UpdateTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.CancelTime, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ActiveTraderID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ClearingPartID, vValue[i++].operator _bstr_t());
				lTempInfp.SequenceNo = vValue[i++].intVal;
				lTempInfp.FrontID = vValue[i++].intVal;
				lTempInfp.SessionID = vValue[i++].intVal;
				strcpy(lTempInfp.UserProductInfo, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.StatusMsg, vValue[i++].operator _bstr_t());
				lTempInfp.UserForceClose = vValue[i++].intVal;
				strcpy(lTempInfp.ActiveUserID, vValue[i++].operator _bstr_t());
				lTempInfp.BrokerOrderSeq = vValue[i++].intVal;
				strcpy(lTempInfp.RelativeOrderSysID, vValue[i++].operator _bstr_t());
				lTempInfp.AvgPrice = vValue[i++].dblVal;
				lTempInfp.ExStatus = (PlatformStru_OrderInfo::EnumExStatus)vValue[i++].intVal;	
				lTempInfp.FTID = vValue[i++].intVal;	
				lTempInfp.UpdateSeq = vValue[i++].intVal;	

				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;

}

//装载用户的成交
CF_ERROR CRealDBOpr::LoadTodayAccountTrades(const std::string & nsTableName,
									 const std::string & nsBrokerID,
									 const std::string & nsInvestorID,
									 const std::string & strTime, 
									 std::vector<PlatformStru_TradeInfo> & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\' \
					  AND InvestorID = \'%s\' AND BrokerID = \'%s\' order by tradetime",nsTableName.c_str(), strTime.c_str(),nsInvestorID.c_str(),nsBrokerID.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_TradeInfo lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.OrderRef, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.UserID, vValue[i++].operator _bstr_t()); 				
				strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.TradeID, vValue[i++].operator _bstr_t());
				lTempInfp.Direction = vValue[i++].cVal;
				strcpy(lTempInfp.OrderSysID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ParticipantID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ClientID, vValue[i++].operator _bstr_t());
				lTempInfp.TradingRole= vValue[i++].cVal;
				strcpy(lTempInfp.ExchangeInstID, vValue[i++].operator _bstr_t());
				lTempInfp.OffsetFlag= vValue[i++].cVal;
				lTempInfp.HedgeFlag= vValue[i++].cVal;
				lTempInfp.Price = vValue[i++].dblVal;
				lTempInfp.Volume = vValue[i++].intVal;
				strcpy(lTempInfp.TradeDate, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.TradeTime, vValue[i++].operator _bstr_t());
				lTempInfp.TradeType= vValue[i++].cVal;
				lTempInfp.PriceSource= vValue[i++].cVal;
				strcpy(lTempInfp.TraderID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.OrderLocalID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.ClearingPartID, vValue[i++].operator _bstr_t());
				strcpy(lTempInfp.BusinessUnit, vValue[i++].operator _bstr_t());
				lTempInfp.SequenceNo = vValue[i++].intVal;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t());
				lTempInfp.SettlementID = vValue[i++].intVal;
				lTempInfp.BrokerOrderSeq = vValue[i++].intVal;
				lTempInfp.TradeSource= vValue[i++].cVal;	
				lTempInfp.CloseProfitByDate = vValue[i++].dblVal;
				lTempInfp.CloseProfitByTrade = vValue[i++].dblVal;
				lTempInfp.TradeCommission = vValue[i++].dblVal;
				lTempInfp.FTID = vValue[i++].intVal;	
				lTempInfp.UpdateSeq = vValue[i++].intVal;	

				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

//装载用户的持仓
CF_ERROR CRealDBOpr::LoadTodayAccountPositions(const std::string & nsTableName,
										const std::string & nsBrokerID,
										const std::string & nsInvestorID,
										const std::string & strTime, 
										std::vector<PlatformStru_Position> & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\' \
					  AND InvestorID = \'%s\' AND BrokerID = \'%s\'",nsTableName.c_str(), strTime.c_str(),nsInvestorID.c_str(),nsBrokerID.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_Position lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());					
				lTempInfp.PosiDirection = vValue[i++].cVal;
				lTempInfp.HedgeFlag= vValue[i++].cVal;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t());				
				lTempInfp.PositionDate= vValue[i++].cVal;
				lTempInfp.SettlementID = vValue[i++].intVal;
				lTempInfp.Position = vValue[i++].intVal;
				lTempInfp.TodayPosition = vValue[i++].intVal;
				lTempInfp.YdPosition = vValue[i++].intVal;
				lTempInfp.OpenVolume = vValue[i++].intVal;
				lTempInfp.CloseVolume = vValue[i++].intVal;
				lTempInfp.OpenAmount = vValue[i++].dblVal;
				lTempInfp.CloseAmount = vValue[i++].dblVal;
				lTempInfp.PositionCost = vValue[i++].dblVal;
				lTempInfp.OpenCost = vValue[i++].dblVal;
				lTempInfp.LongFrozen = vValue[i++].intVal;
				lTempInfp.ShortFrozen = vValue[i++].intVal;
				lTempInfp.LongFrozenAmount = vValue[i++].dblVal;
				lTempInfp.ShortFrozenAmount = vValue[i++].dblVal;
				lTempInfp.FrozenMargin = vValue[i++].dblVal;
				lTempInfp.FrozenCommission = vValue[i++].dblVal;
				lTempInfp.FrozenCash = vValue[i++].dblVal;
				lTempInfp.Commission = vValue[i++].dblVal;
				lTempInfp.PreMargin = vValue[i++].dblVal;
				lTempInfp.UseMargin = vValue[i++].dblVal;
				lTempInfp.ExchangeMargin = vValue[i++].dblVal;
				lTempInfp.MarginRateByMoney = vValue[i++].dblVal;
				lTempInfp.MarginRateByVolume = vValue[i++].dblVal;
				lTempInfp.CashIn = vValue[i++].dblVal;
				lTempInfp.PositionProfit = vValue[i++].dblVal;
				lTempInfp.CloseProfit = vValue[i++].dblVal;
				lTempInfp.CloseProfitByDate = vValue[i++].dblVal;
				lTempInfp.CloseProfitByTrade = vValue[i++].dblVal;
				lTempInfp.PreSettlementPrice = vValue[i++].dblVal;
				lTempInfp.SettlementPrice = vValue[i++].dblVal;
				lTempInfp.CombPosition = vValue[i++].intVal;
				lTempInfp.CombLongFrozen = vValue[i++].intVal;
				lTempInfp.CombShortFrozen = vValue[i++].intVal;
				lTempInfp.PositionProfitByTrade = vValue[i++].dblVal;
				lTempInfp.TotalPositionProfitByDate = vValue[i++].dblVal;
				lTempInfp.FTID = vValue[i++].intVal;	
				lTempInfp.UpdateSeq = vValue[i++].intVal;	

				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

//装载用户的持仓明细
CF_ERROR CRealDBOpr::LoadTodayAccountPositionDetails(const std::string & nsTableName,
											  const std::string & nsBrokerID,
											  const std::string & nsInvestorD,
											  const std::string & strTime, 
											  std::vector<PlatformStru_PositionDetail> & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\' \
					  AND InvestorID = \'%s\' AND BrokerID = \'%s\'",nsTableName.c_str(), strTime.c_str(),nsInvestorD.c_str(),nsBrokerID.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			PlatformStru_PositionDetail lTempInfp;
			memset(&lTempInfp, 0, sizeof(lTempInfp));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;
				strcpy(lTempInfp.InstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.InvestorID, vValue[i++].operator _bstr_t());			
				lTempInfp.HedgeFlag= vValue[i++].cVal;	
				lTempInfp.Direction = vValue[i++].cVal;
				strcpy(lTempInfp.OpenDate, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.TradeID, vValue[i++].operator _bstr_t()); 
				lTempInfp.Volume = vValue[i++].intVal;			
				lTempInfp.OpenPrice = vValue[i++].dblVal;
				strcpy(lTempInfp.TradingDay, vValue[i++].operator _bstr_t()); 
				lTempInfp.SettlementID = vValue[i++].intVal;
				lTempInfp.TradeType = vValue[i++].cVal;
				strcpy(lTempInfp.CombInstrumentID, vValue[i++].operator _bstr_t());	
				strcpy(lTempInfp.ExchangeID, vValue[i++].operator _bstr_t()); 

				lTempInfp.CloseProfitByDate = vValue[i++].dblVal;
				lTempInfp.CloseProfitByTrade = vValue[i++].dblVal;
				lTempInfp.PositionProfitByDate = vValue[i++].dblVal;
				lTempInfp.PositionProfitByTrade = vValue[i++].dblVal;

				lTempInfp.Margin = vValue[i++].dblVal;
				lTempInfp.ExchMargin = vValue[i++].dblVal;
				lTempInfp.MarginRateByMoney = vValue[i++].dblVal;
				lTempInfp.MarginRateByVolume = vValue[i++].dblVal;
				lTempInfp.LastSettlementPrice = vValue[i++].dblVal;
				lTempInfp.SettlementPrice = vValue[i++].dblVal;			
				lTempInfp.CloseVolume = vValue[i++].intVal;			
				lTempInfp.CloseAmount = vValue[i++].dblVal;	
				lTempInfp.FTID = vValue[i++].intVal;	
				lTempInfp.UpdateSeq = vValue[i++].intVal;	

				nInfo.push_back(lTempInfp);
			}
		}
	}
	return CF_ERROR_SUCCESS;
}

//装载用户的资金
CF_ERROR CRealDBOpr::LoadTodayAccountFundInfo(const std::string & nsTableName,
									   const std::string & nsBrokerID,
									   const std::string & nsInvestorID,
									   const std::string & strTime, 
									   PlatformStru_TradingAccountInfo & nInfo) 
{
	int nErrorCode = CF_ERROR_SUCCESS;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "select * from %s t where t.ValidateDate = \'%s\'\
					  AND AccountID = \'%s\' AND BrokerID = \'%s\' ",nsTableName.c_str(), strTime.c_str(),nsInvestorID.c_str(),nsBrokerID.c_str());
	std::vector<std::vector<_variant_t>> vNode;
	if ( !CInterface_SvrDBOpr::getObj().QueryData(szBuffer, vNode, nErrorCode) || vNode.size() == 0)
	{
		return CF_ERROR_DATABASE_NOT_CONNECT;
	}
	else
	{
		for ( UINT j = 0; j < vNode.size(); j++ )
		{
			
			memset(&nInfo, 0, sizeof(nInfo));

			std::vector<_variant_t> vValue = vNode[j];
		/*	if ( vValue.size() != 11)
			{
				return CF_ERROR_DATABASE_NOT_CONNECT;
			}
			else*/
			{

				int i = 0;			
				strcpy(nInfo.BrokerID, vValue[i++].operator _bstr_t());	
				strcpy(nInfo.AccountID, vValue[i++].operator _bstr_t());			
				nInfo.PreMortgage = vValue[i++].dblVal;
				nInfo.PreCredit = vValue[i++].dblVal;
				nInfo.PreDeposit = vValue[i++].dblVal;
				nInfo.PreBalance = vValue[i++].dblVal;
				nInfo.PreMargin = vValue[i++].dblVal;
				nInfo.InterestBase = vValue[i++].dblVal;
				nInfo.Interest = vValue[i++].dblVal;
				nInfo.Deposit = vValue[i++].dblVal;
				nInfo.Withdraw = vValue[i++].dblVal;
				nInfo.FrozenMargin = vValue[i++].dblVal;			
				nInfo.FrozenCash = vValue[i++].dblVal;			
				nInfo.FrozenCommission = vValue[i++].dblVal;	
				nInfo.CurrMargin = vValue[i++].dblVal;
				nInfo.CashIn = vValue[i++].dblVal;
				nInfo.Commission = vValue[i++].dblVal;
				nInfo.CloseProfit = vValue[i++].dblVal;
				nInfo.PositionProfit = vValue[i++].dblVal;			
				nInfo.Balance = vValue[i++].dblVal;			
				nInfo.Available = vValue[i++].dblVal;	
				nInfo.WithdrawQuota = vValue[i++].dblVal;			
				nInfo.Reserve = vValue[i++].dblVal;	
				strcpy(nInfo.TradingDay, vValue[i++].operator _bstr_t());	
				nInfo.SettlementID = vValue[i++].intVal;	
				nInfo.Credit = vValue[i++].dblVal;
				nInfo.Mortgage = vValue[i++].dblVal;
				nInfo.ExchangeMargin = vValue[i++].dblVal;			
				nInfo.DeliveryMargin = vValue[i++].dblVal;			
				nInfo.ExchangeDeliveryMargin = vValue[i++].dblVal;	
				nInfo.StaticProfit = vValue[i++].dblVal;			
				nInfo.DynamicProfit = vValue[i++].dblVal;	
				nInfo.RiskDegree = vValue[i++].dblVal;	

			
			}
		}
	}
	return CF_ERROR_SUCCESS;
}



//写报单
CF_ERROR CRealDBOpr::WriteAccountOrderInfos(const std::string & nsTableName,
										const std::string & nsBrokerID,
										const std::string & nsInvestorID,
										const std::string & strTime,
										const std::vector<PlatformStru_OrderInfo> & nInfo,
										bool nbDelete ) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveAccountOrderInfos(nsTableName,nbDelete,strTime,nsBrokerID,nsInvestorID,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;
	return CF_ERROR_SUCCESS;
}

//写成交
CF_ERROR CRealDBOpr::WriteAccountTradeInfos(const std::string & nsTableName,
										const std::string & nsBrokerID,
										const std::string & nsInvestorID,
										const std::string & strTime,
										const std::vector<PlatformStru_TradeInfo> & nInfo,
										bool nbDelete ) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveAccountTraderInfos(nsTableName,nbDelete,strTime,nsBrokerID,nsInvestorID,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;
	return CF_ERROR_SUCCESS;
}

//写持仓
CF_ERROR CRealDBOpr::WriteAccountPositionInfos(const std::string & nsTableName,
										   const std::string & nsBrokerID,
										   const std::string & nsInvestorID,
										   const std::string & strTime,
										   const std::vector<PlatformStru_Position> & nInfo,
										   bool nbDelete ) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveAccountPositionInfos(nsTableName,nbDelete,strTime,nsBrokerID,nsInvestorID,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;
	return CF_ERROR_SUCCESS;
}

//写持仓明细
CF_ERROR CRealDBOpr::WriteAccountPositionDetailInfos(const std::string & nsTableName,
												 const std::string & nsBrokerID,
												 const std::string & nsInvestorID,
												 const std::string & strTime,
												 const std::vector<PlatformStru_PositionDetail> & nInfo,
												 bool nbDelete) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveAccountPositionDetailInfos(nsTableName,nbDelete,strTime,nsBrokerID,nsInvestorID,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;
	return CF_ERROR_SUCCESS;
}

//写资金
CF_ERROR CRealDBOpr::WriteAccountFundInfos(const std::string & nsTableName,
									   const std::string & nsBrokerID,
									   const std::string & nsInvestorID,
									   const std::string & strTime,
									   const PlatformStru_TradingAccountInfo & nInfo) 
{
	int nErrorCode = 0;
	if ( !CInterface_SvrDBOpr::getObj().SaveAccountFundInfos(nsTableName,strTime,nsBrokerID,nsInvestorID,nInfo,nErrorCode))
		return CF_ERROR_DATABASE_NOT_CONNECT;
	return CF_ERROR_SUCCESS;

}
