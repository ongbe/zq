#include "stdafx.h"
#include <iostream>
#include "Tools/WriteLog.h"
#include "Tools/TcpServerWorker.h"
#include "ThreadSharedData.h"
#include "SocketCommon.h"
#include "RiskMsgQueue.h"
#include "Worker.h"
#include "DBAccess.h"
#include "DataCenter.h"
#define SubscribedDataSend(SID)\
do \
{\
	std::set<SOCKET>::const_iterator it;\
	std::set<SOCKET> sset;\
	m_pThreadSharedData->GetSubscribeData(AccIDOrInst,SubscribeID_##SID,sset);\
	for (it=sset.begin();it!=sset.end();++it)\
	{\
		RspResultData(*it, OPERATOR_SUCCESS, RspCmdID, (void*)&Data, sizeof(Data));\
	}	\
} while (0)
void CWorker::ProcessSubscribedData( const Risk2ClientData& reqData )
{
	switch(reqData.CmdID)
	{
	case R2CMSGID_INPUTORDER:
		{
			const InputOrderField& InputOrder=reqData.Main.InputOrder;			
			int RspCmdID=Cmd_RM_ForceCloseOrderInsert_Rsp;
			std::set<SOCKET> sset;
			std::set<SOCKET>::const_iterator it;
			if(m_pThreadSharedData->GetSocketsFromUserID(sset))
			{
				for (it=sset.begin();it!=sset.end();++it)
				{
					RspResultData(*it, OPERATOR_SUCCESS, RspCmdID, (void*)&InputOrder, sizeof(InputOrder));
				}
			}
		}
		break;
	case R2CMSGID_FUND://资金
		{
			int AccID=reqData.AccID;
			char AccIDOrInst[256];
			sprintf_s(AccIDOrInst,256,"%d",AccID);
			const RiskSyncAccountField& Data=reqData.Main.RiskSyncAccount;
			int RspCmdID=Cmd_RM_Fund_Push;
			SubscribedDataSend(Fund);
		}
		break;
	case R2CMSGID_POSITION://持仓
		{
			int AccID=reqData.AccID;
			char AccIDOrInst[256];
			sprintf_s(AccIDOrInst,256,"%d",AccID);
			const SequencialPositionField& Data=reqData.Main.Position;
			int RspCmdID=Cmd_RM_Position_Push;
			SubscribedDataSend(Position);
		}
		break;
	case R2CMSGID_TRADE://成交
		{
			int AccID=reqData.AccID;
			char AccIDOrInst[256];
			sprintf_s(AccIDOrInst,256,"%d",AccID);
			const SequencialTradeField& Data=reqData.Main.Trade;
			int RspCmdID=Cmd_RM_Trade_Push;
			SubscribedDataSend(Trade);
		}
		break;
	case R2CMSGID_ORDER://报单
		{
			int AccID=reqData.AccID;
			char AccIDOrInst[256];
			sprintf_s(AccIDOrInst,256,"%d",AccID);
			const SequencialOrderField& Data=reqData.Main.Order;
			int RspCmdID=Cmd_RM_Order_Push;
			SubscribedDataSend(Order);
		}
		break;
	case R2CMSGID_DEPOSIT://出入金
		{
			int AccID=reqData.AccID;
			char AccIDOrInst[256];
			sprintf_s(AccIDOrInst,256,"%d",AccID);
			const SeqDepositField& Data=reqData.Main.Deposit;
			int RspCmdID=Cmd_RM_Deposit_Push;
			SubscribedDataSend(Deposit);
		}
		break;
	case R2CMSGID_QUOT://行情
		{
			const DepthMarketDataField& Data=reqData.Main.MarketData;
			std::string AccIDOrInst=Data.InstrumentID;
			int RspCmdID=Cmd_RM_Quot_Push;
			SubscribedDataSend(Quot);
		}
		break;
	case R2CMSGID_RISKEVENT:
		{
			int AccID=reqData.AccID;
			char AccIDOrInst[256];
			sprintf_s(AccIDOrInst,256,"%d",AccID);
			const RiskEvent& Data=reqData.Main.Event;
			int RspCmdID=Cmd_RM_RiskEvent_Push;
			SubscribedDataSend(RiskEvent);
		}
		break;
	case R2CMSGID_EVENTHANDLING:
		{
			int AccID=reqData.AccID;
			char AccIDOrInst[256];
			sprintf_s(AccIDOrInst,256,"%d",AccID);
			const RiskEventHandling& Data=reqData.Main.Handling;
			int RspCmdID=Cmd_RM_RiskEvtHandling_Push;
			SubscribedDataSend(RiskEvtHandling);
		}
		break;
	case R2CMSGID_STATISTICS:
		{
			const StatisticsEntry& SingleData=reqData.Main.Statistics;
			if(reqData.AccID&(-STATISTICS_DATA_BEGIN))
				m_StatVec.clear();
			m_StatVec.push_back(SingleData);
			if(reqData.AccID&(-STATISTICS_DATA_END))
			{
				char AccIDOrInst[256];
				sprintf_s(AccIDOrInst,256,"%08X%08X",SingleData.nAssetMgmtOrgID,SingleData.nFinancialProductID);			
				int RspCmdID=Cmd_RM_Statistics_Push;
				std::set<SOCKET>::const_iterator it;
				std::set<SOCKET> sset;
				m_pThreadSharedData->GetSubscribeData(AccIDOrInst,SubscribeID_Statistics,sset);
				for (it=sset.begin();it!=sset.end();++it)
				{
					RspResultDataAutoDividePkg<StatisticsEntry>(*it, OPERATOR_SUCCESS, RspCmdID,m_StatVec);
				}
			}
		}
		break;
	case R2CMSGID_TARGETACCOUNT:
		m_TargetAccountVec.push_back(reqData.Main.Target);
		break;
	case R2CMSGID_MESSAGEINFO:
		{
			MessageInfo msginfo=reqData.Main.MsgInfo;
			SendEventMessage(msginfo,m_TargetAccountVec);
			m_TargetAccountVec.clear();
		}
		break;
	case R2CMSGID_NETVALUE:
		{
			int AccID=reqData.AccID;
			char AccIDOrInst[256];
			sprintf_s(AccIDOrInst,256,"%d",AccID);
			const NetValueInfo& Data=reqData.Main.netValueInfo;
			int RspCmdID=Cmd_RM_FundNetValue_push;
			SubscribedDataSend(NetValue);
		}
		break;
	default:
		break;
	}
}

//处理出入金
void CWorker::ProcessDepositRequest(const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_QryTraderDeposit_Req://查询账户出入金请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					std::vector<SeqDepositField> vec;
					SAFE_GET_DATACENTER()->GetDepositList(AccID,vec);
					RspResultDataAutoDividePkg<SeqDepositField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryTraderDeposit_Rsp, 
						vec, reqData.head.seq);
				}
			}
			else
			{
				OutputUnkownReqLog(reqData);

			}
		}
		break;
	case Cmd_RM_SubscribeDeposit_Req:
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<SeqDepositField> vec;
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_Deposit,AccID);
					SAFE_GET_DATACENTER()->SubscribeTrade(AccID);
					SAFE_GET_DATACENTER()->GetDepositList(AccID,vec);
				}
				RspResultDataAutoDividePkg<SeqDepositField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribeDeposit_Rsp, 
					vec, reqData.head.seq);	
			}
			else
			{
				OutputUnkownReqLog(reqData);

			}
		}
		break;
	case Cmd_RM_UnSubscribeDeposit_Req://退订出入金
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_Deposit,AccID);
					SAFE_GET_DATACENTER()->UnSubscribeDeposit(AccID);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribeDeposit_Rsp, reqData.pData,reqData.head.len, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessTradeRequest(const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{		
	case Cmd_RM_SubscribeTrade_Req://订阅成交请求
		{

			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<SequencialTradeField> vec;
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_Trade,AccID);
					SAFE_GET_DATACENTER()->SubscribeTrade(AccID);
					SAFE_GET_DATACENTER()->GetTradeList(AccID,vec);
				}
				RspResultDataAutoDividePkg<SequencialTradeField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribeTrade_Rsp, 
					vec, reqData.head.seq);	
			}
		}
		break;
	case Cmd_RM_UnSubscribeTrade_Req://退订成交请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_Trade,AccID);
					SAFE_GET_DATACENTER()->UnSubscribeTrade(AccID);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribeTrade_Rsp, reqData.pData,reqData.head.len, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryHistroyTrade_Req:
		{
			if ( reqData.head.len != sizeof(HisTradeDataQueryKey))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			HisTradeDataQueryKey key = *(HisTradeDataQueryKey*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t.BROKERID,t.INVESTORID,t.INSTRUMENTID,\
							  t.ORDERREF,t.USERID,t.EXCHANGEID,t.TRADEID,t.DIRECTION,t.ORDERSYSID,\
							  t.PARTICIPANTID,t.CLIENTID,t.TRADINGROLE,t.EXCHANGEINSTID,t.OFFSETFLAG,t.HEDGEFLAG,\
							  t.PRICE,t.VOLUME,t.TRADEDATE,t.TRADETIME,t.TRADETYPE,t.PRICESOURCE,t.TRADERID,\
							  t.ORDERLOCALID,t.CLEARINGPARTID,t.BUSINESSUNIT,t.SEQUENCENO,t.TRADINGDAY,\
							  t.SETTLEMENTID,t.BROKERORDERSEQ,t.TRADESOURCE,t.UNIQSEQUENCENO from SEQUENCIALTRADE t \
							  where to_date(t.TRADEDATE,'YYYYMMDD') >= to_date('%s','YYYY-MM-DD')\
							  and to_date(t.TRADEDATE,'YYYYMMDD') <= to_date('%s','YYYY-MM-DD')\
							  order by t.TRADEDATE",
							  key.TradingDayFrom, key.TradingDayTo);
			std::vector<SequencialTradeField> vec;
			if ( !GET_DBACCESS().QueryTradeRecord(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryHistroyTrade_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryHistroyTrade_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}
void CWorker::ProcessOrderRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{		
	case Cmd_RM_SubscribeOrder_Req://订阅报单请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<SequencialOrderField> vec;
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_Order,AccID);
					SAFE_GET_DATACENTER()->SubscribeOrder(AccID);
					SAFE_GET_DATACENTER()->GetOrderList(AccID,vec);
				}
				RspResultDataAutoDividePkg<SequencialOrderField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribeOrder_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_UnSubscribeOrder_Req://退订报单请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_Order,AccID);
					SAFE_GET_DATACENTER()->UnSubscribeOrder(AccID);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribeOrder_Rsp, reqData.pData,reqData.head.len, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryHistroyOrder_Req:
		{
			if ( reqData.head.len != sizeof(HisTradeDataQueryKey))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			HisTradeDataQueryKey key = *(HisTradeDataQueryKey*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from SEQUENCIALORDER t \
							  where to_date(t.TRADINGDAY,'YYYYMMDD') >= to_date('%s','YYYY-MM-DD')\
							  and to_date(t.TRADINGDAY,'YYYYMMDD') <= to_date('%s','YYYY-MM-DD')\
							  order by t.TRADINGDAY", 
							  key.TradingDayFrom, key.TradingDayTo);
			std::vector<SequencialOrderField> vec;
			if ( !GET_DBACCESS().QueryOrderRecord(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryHistroyOrder_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryHistroyOrder_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessFundRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{		
	case Cmd_RM_SubscribeFund_Req://订阅账户资金请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<RiskAllAccountField> vec;

				std::vector<NetValueInfo> vecNetValueInfo;
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_Fund,AccID);					
					SAFE_GET_DATACENTER()->SubscribeFund(AccID);
					SAFE_GET_DATACENTER()->GetAllAccountFundList(AccID,vec);

					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_NetValue,AccID);
					SAFE_GET_DATACENTER()->SubscribeNetValue(AccID);
					//SAFE_GET_DATACENTER()->GetAllNetValueInfo(AccID, vecNetValueInfo);	
					
					//计算基金净值
					std::string InvestorID = CDataCenter::Get()->GetTradeAccountID( AccID);					
					double dbP = CDataCenter::Get()->CalcFundNetValueRisk(InvestorID);	
					if(dbP +1 > 0.000001 || dbP +1 <-0.000001)
					{//无效的基金净值则不发送，基金参数表FUNDNETPARAM里面没有设置参数的
						NetValueInfo netValue;
						strcpy(netValue.szAccount, InvestorID.c_str());
						netValue.dbValue = dbP;
						vecNetValueInfo.push_back(netValue);
					}
				}
				RspResultDataAutoDividePkg<RiskAllAccountField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribeFund_Rsp, 
					vec, reqData.head.seq);

				RspResultDataAutoDividePkg<NetValueInfo>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_FundNetValue_push, 
					vecNetValueInfo, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_UnSubscribeFund_Req://退订账户资金请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_Fund,AccID);					
					SAFE_GET_DATACENTER()->UnSubscribeFund(AccID);

					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_NetValue,AccID);
					SAFE_GET_DATACENTER()->UnSubscribeNetValue(AccID);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribeFund_Rsp, reqData.pData,reqData.head.len, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryHistroyRiskFund_Req:
		{
			if ( reqData.head.len != sizeof(HisTradeDataQueryKey))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			HisTradeDataQueryKey key = *(HisTradeDataQueryKey*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t1.SEQUENCENO,t1.BROKERID,t1.INVESTORID,t1.ACCOUNTID,\
				t1.PREMORTGAGE,t1.PRECREDIT,t1.PREDEPOSIT,t1.PREBALANCE,t1.PREMARGIN,t1.RESERVE,\
				t1.PREEXCHMARGIN,t1.FORCECLOSESTAT,t1.DELIVERYMARGIN,t1.EXCHANGEDELIVERYMARGIN,\
				t1.RECVTIME,t2.BROKERID,t2.INVESTORID,t2.WITHDRAW,\
				t2.CURRMARGIN,t2.COMMISSION,t2.CLOSEPROFIT,t2.POSITIONPROFIT,\
				t2.BALANCE,t2.AVAILABLE,t2.MORTGAGE,t2.EXCHANGEMARGIN,\
				t2.WITHDRAWQUOTA,t2.CREDIT,t2.FROZENMARGIN,t2.FROZENCOMMISSION,\
				t2.RECVTIME from PRERISKACCOUNT t1 left join RISKSYNCACCOUNT t2\
				on t1.investorid = t2.investorid and t1.recvtime = t2.recvtime\
				where to_date(t1.RECVTIME,'YYYYMMDD') >= to_date('%s','YYYY-MM-DD')\
				and to_date(t1.RECVTIME,'YYYYMMDD') <= to_date('%s','YYYY-MM-DD')\
				order by t1.RECVTIME", 
				key.TradingDayFrom, key.TradingDayTo);
			vector<RiskAllAccountField> vec;
			if ( !GET_DBACCESS().QueryRiskAccount(szBuffer, vec))
			{
				std::string strError = "无法查到资金数据";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryHistroyRiskFund_Rsp, 
					(void*)strError.c_str(), 
					strError.length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryHistroyRiskFund_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessPositionRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{		
	case Cmd_RM_SubscribePosition_Req://订阅持仓请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<SequencialPositionField> vec;
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_Position,AccID);
					SAFE_GET_DATACENTER()->SubscribePosition(AccID);
					SAFE_GET_DATACENTER()->GetPositionList(AccID,vec);
				}
				RspResultDataAutoDividePkg<SequencialPositionField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribePosition_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_UnSubscribePosition_Req://退订持仓请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_Position,AccID);
					SAFE_GET_DATACENTER()->UnSubscribePosition(AccID);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribePosition_Rsp, reqData.pData,reqData.head.len, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryHistroyPosition_Req:
		{
			if ( reqData.head.len != sizeof(HisTradeDataQueryKey))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			HisTradeDataQueryKey key = *(HisTradeDataQueryKey*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from SEQUENCIALPOSITION t \
							  where to_date(t.TRADINGDAY,'YYYYMMDD') >= to_date('%s','YYYY-MM-DD')\
							  and to_date(t.TRADINGDAY,'YYYYMMDD') <= to_date('%s','YYYY-MM-DD')\
							  order by t.TRADINGDAY", 
							  key.TradingDayFrom, key.TradingDayTo);

			std::vector<SequencialPositionField> vec;
			if ( !GET_DBACCESS().QueryPositionRecord(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryHistroyPosition_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryHistroyPosition_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessQuotRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{		
	case Cmd_RM_SubscribeQuot_Req://订阅行情请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(TInstrumentIDType);
			int AccRem=pkgLen%sizeof(TInstrumentIDType);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<DepthMarketDataField> vec;
				TInstrumentIDType* pInstrumentID = (TInstrumentIDType*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					std::string strInstr=pInstrumentID[index];
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_Quot,strInstr);
					SAFE_GET_DATACENTER()->SubscribeQuot(strInstr);
					SAFE_GET_DATACENTER()->GetQuotList(strInstr,vec);
				}
				RspResultDataAutoDividePkg<DepthMarketDataField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribeQuot_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_UnSubscribeQuot_Req://退订行情请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(TInstrumentIDType);
			int AccRem=pkgLen%sizeof(TInstrumentIDType);
			if (pkgLen>0||AccRem==0)
			{
				TInstrumentIDType* pInstrumentID = (TInstrumentIDType*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					std::string strInstr=pInstrumentID[index];
					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_Quot,strInstr);
					SAFE_GET_DATACENTER()->UnSubscribeQuot(strInstr);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribeQuot_Rsp, reqData.pData,reqData.head.len, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}
void CWorker::ProcessInstrumentRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{		
	case Cmd_RM_InstrumentIDList_Req:
		{
			int pkgLen=reqData.head.len;
			if(pkgLen==0)
			{
				std::vector<std::string> vec;
				SAFE_GET_DATACENTER()->GetInstrumentList(vec);
				int bufferlength=sizeof(TInstrumentIDType)* vec.size();
				char* buffer=(char*)alloca(bufferlength);
				memset(buffer,0,bufferlength);
				for (size_t i=0;i<vec.size();++i)
				{
					safestrcpy(buffer+i*sizeof(TInstrumentIDType),sizeof(TInstrumentIDType),vec[i].c_str());
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_InstrumentIDList_Rsp, buffer, bufferlength,reqData.head.seq);
			}
			else
			{
				OutputUnkownReqLog(reqData);

			}
		}
		break;
	case Cmd_RM_InstrumentInfo_Req:
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(TInstrumentIDType);
			int AccRem=pkgLen%sizeof(TInstrumentIDType);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<InstrumentField> vec;
				TInstrumentIDType* pInstrumentID = (TInstrumentIDType*)reqData.pData;
				InstrumentField instr;
				for (int index=0;index<AccCnt;++index)
				{
					SAFE_GET_DATACENTER()->GetInstrumentInfo(pInstrumentID[index],instr);
					vec.push_back(instr);
				}
				RspResultDataAutoDividePkg<InstrumentField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_InstrumentInfo_Rsp, 
					vec, reqData.head.seq);

			}
		}
		break;
	case Cmd_RM_InstrumentInfoList_Req:
		{
			int pkgLen=reqData.head.len;
			if(pkgLen==0)
			{
				std::vector<InstrumentField> vec;
				SAFE_GET_DATACENTER()->GetInstrumentInfoList(vec);
				RspResultDataAutoDividePkg<InstrumentField>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_InstrumentInfoList_Rsp, 
					vec, reqData.head.seq);
			}
			else
			{
				OutputUnkownReqLog(reqData);

			}
		}
		break;
	case Cmd_RM_ProductIDList_Req:
		{
			int pkgLen=reqData.head.len;
			if(pkgLen==0)
			{
				std::vector<std::string> vec;
				SAFE_GET_DATACENTER()->GetProductIDList(vec);
				int bufferlength=sizeof(TInstrumentIDType)* vec.size();
				char* buffer=(char*)alloca(bufferlength);
				memset(buffer,0,bufferlength);
				for (size_t i=0;i<vec.size();++i)
				{
					safestrcpy(buffer+i*sizeof(TInstrumentIDType),sizeof(TInstrumentIDType),vec[i].c_str());
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_ProductIDList_Rsp, buffer, bufferlength,reqData.head.seq);
			}
			else
			{
				OutputUnkownReqLog(reqData);

			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessRiskEventRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{		
	case Cmd_RM_SubscribeRiskEvent_Req://订阅风险事件请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<RiskEvent> vec;
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_RiskEvent,AccID);
					SAFE_GET_DATACENTER()->SubscribeRiskEvent(AccID);
					SAFE_GET_DATACENTER()->GetRiskEventList(AccID,vec);
				}
				RspResultDataAutoDividePkg<RiskEvent>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribeRiskEvent_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_UnSubscribeRiskEvent_Req://退订风险事件请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_RiskEvent,AccID);
					SAFE_GET_DATACENTER()->UnSubscribeRiskEvent(AccID);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribeRiskEvent_Rsp, reqData.pData,reqData.head.len, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryRiskEvent_Req:
		{
			if ( reqData.head.len != sizeof(RiskEventQueryKey))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			RiskEventQueryKey key = *(RiskEventQueryKey*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from riskevent t where t.tradeaccountid = %d and \
				to_char(to_date('1970-01-01','yyyy-MM-dd')+t.eventtime/86400,'YYYY-MM-DD') >= '%s'\
				and to_char(to_date('1970-01-01','yyyy-MM-dd')+t.eventtime/86400,'YYYY-MM-DD') <= '%s'\
				and (t.riskeventid,t.riskeventsubid) in \
				(select t1.riskeventid,max(t1.riskeventsubid) from riskevent t1 group by t1.riskeventid)", 
				key.nTradeAccountID, key.szStartDate,key.szEndDate);
			vector<RiskEvent> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryRiskEvent_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryRiskEvent_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}

void CWorker::ProcessRiskEvtHandlingRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{	
	case Cmd_RM_AddRiskEvtHandling_Req:
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(RiskEventHandlingUserReq);
			int AccRem=pkgLen%sizeof(RiskEventHandlingUserReq);
			if (pkgLen>0||AccRem==0)
			{
				RiskEventHandlingUserReq* pHandlingID = (RiskEventHandlingUserReq*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					RiskEventHandlingUserReq& AccID=pHandlingID[index];
					SAFE_GET_DATACENTER()->AddHandling(AccID);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS,Cmd_RM_AddRiskEvtHandling_Rsp,NULL,0, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_SubscribeRiskEvtHandling_Req://订阅风险事件处理请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<RiskEventHandling> vec;
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_RiskEvtHandling,AccID);
					SAFE_GET_DATACENTER()->SubscribeRiskEvtHandling(AccID);
					SAFE_GET_DATACENTER()->GetRiskEvtHandlingList(AccID,vec);
				}
				RspResultDataAutoDividePkg<RiskEventHandling>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribeRiskEvtHandling_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_UnSubscribeRiskEvtHandling_Req://退订风险事件处理请求
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(int);
			int AccRem=pkgLen%sizeof(int);
			if (pkgLen>0||AccRem==0)
			{
				int* pAccountID = (int*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					int AccID=pAccountID[index];
					m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_RiskEvtHandling,AccID);
					SAFE_GET_DATACENTER()->UnSubscribeRiskEvtHandling(AccID);
				}
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribeRiskEvtHandling_Rsp,reqData.pData,reqData.head.len, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QryRiskEvtHandling_Req:
		{
			if ( reqData.head.len != sizeof(RiskEventHandlingQueryKey))
			{
				OutputUnkownReqLog(reqData);

				return;
			}

			RiskEventHandlingQueryKey key = *(RiskEventHandlingQueryKey*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t1.riskeventhandlingid,t1.riskeventid,t1.riskeventsubid,\
							  t1.handlingtime,t1.handler,t1.accepter,t1.actiontype,t1.processstatus,\
							  t1.eventcomment,t2.tradeaccountid,t2.riskindicatorid,t2.risklevelid,t2.calcresult,\
							  t2.instrumentid from RISKEVENTHANDLING t1 \
							  left join riskevent t2 on t1.riskeventid = t2.riskeventid \
							  and t1.riskeventsubid = t2.riskeventsubid \
							  where t1.riskeventid = %d", 
							  key.nRiskEventID);
			vector<RiskEventHandlingEx> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QryRiskEvtHandling_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QryRiskEvtHandling_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}
void CWorker::ProcessForceCloseRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{		
	case Cmd_RM_ForceCloseCalc_Req://请求强平计算
		{
		}
		break;
	case Cmd_RM_ForceCloseOrderInsert_Req://请求强平报单录入
		if (reqData.head.len==sizeof(RiskForceCloseOrderField))
		{
			bool bRet=false;
			int AccID;
			if(m_pThreadSharedData->FindUser(reqData.hSocket,&AccID))
			{
				if(CDataCenter::Get()&&CDataCenter::Get()->GetForceClosePermission(AccID))
				{
					Client2RiskData crdata;
					crdata.CmdID=C2RMSGID_ForceCloseOrderInsert;
					crdata.Main.ForceCloseOrder= *((RiskForceCloseOrderField*)reqData.pData);
					//补充强平用户UserID
					//ReqRiskUserLoginField login={0};
					//SAFE_GET_DATACENTER()->GetUserLoginField(login);
					//safestrcpy(crdata.Main.ForceCloseOrder.UserID,sizeof(TUserIDType),login.UserID);
					
					if(CClient2RiskMsgQueue::Get())
						CClient2RiskMsgQueue::Get()->AddMsg(crdata);
					bRet=true;
				}
			}
			if(bRet)
				RspResultData( reqData.hSocket, OPERATOR_SUCCESS,Cmd_RM_ForceCloseOrderInsert_Rsp,NULL,0, reqData.head.seq);
			else
			{
				char strError[]="账户没有下强平单权限";
				RspResultData( reqData.hSocket, OPERATOR_FAILED,Cmd_RM_ForceCloseOrderInsert_Rsp,strError,strlen(strError)+1, reqData.head.seq);
			}

		}
		break;
	case Cmd_RM_AddFundNetParam_Req:
		{
			if ( reqData.head.len != sizeof(NetFundParam))
			{
				OutputUnkownReqLog(reqData);
				return;
			}

			NetFundParam* pParam = (NetFundParam*)reqData.pData;
			int nNum = 0;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from FUNDNETPARAM t where t.tradeaccountid = %d",pParam->nTradeAccountID);
			if ( !GET_DBACCESS().ExcuteSelect(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddFundNetParam_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				if ( nNum == 0 )
				{
					//添加
					sprintf(szBuffer, "insert into FUNDNETPARAM values(%d,%f,%f,%f,%d)",
						pParam->nTradeAccountID, pParam->dInnerVolumn, pParam->dOuterVolumn,
						pParam->dOuterNetAsset, pParam->nInnerNetAssetOption);
				}
				else
				{
					//修改
					sprintf(szBuffer, "update FUNDNETPARAM t set t.innervolumn = %f, \
									  t.outervolumn = %f,t.outernetasset = %f,t.innernetassetoption = %d \
									  where t.tradeaccountid = %d",
									  pParam->dInnerVolumn, pParam->dOuterVolumn,
									  pParam->dOuterNetAsset, pParam->nInnerNetAssetOption,
									  pParam->nTradeAccountID);
				}
				if ( !GET_DBACCESS().Excute(szBuffer))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddFundNetParam_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddFundNetParam_Rsp, 
						&pParam->nTradeAccountID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_QueryFundNetParam_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}

			int nTradeAccountID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from FUNDNETPARAM t where t.tradeaccountid = %d",nTradeAccountID);
			vector<NetFundParam> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QueryFundNetParam_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QueryFundNetParam_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_QueryFundNetCalcResult_Req:
		{
			if ( reqData.head.len != sizeof(RiskEventQueryKey))
			{
				OutputUnkownReqLog(reqData);
				return;
			}

			RiskEventQueryKey key = *(RiskEventQueryKey*)reqData.pData;

			vector<NetFundCalcResult> vec;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select t.tradeaccountid,t.innervolumn,\
				t.outervolumn,t.innernetasset,t.outernetasset,t.innerpernet,\
				t.outernetasset,t.totalnetasset,to_char(t.updatedate,'YYYY-MM-DD') from FUNDNETCALCRESULT t\
				where t.tradeaccountid = %d and to_char(t.updatedate, 'YYYY-MM-DD') >= '%s'\
				and to_char(t.updatedate, 'YYYY-MM-DD') <= '%s'",
				key.nTradeAccountID, key.szStartDate, key.szEndDate);
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QueryFundNetCalcResult_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QueryFundNetCalcResult_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_AddForceCloseParam_Req:
		{
			if ( reqData.head.len != sizeof(ForceCloseParam))
			{
				OutputUnkownReqLog(reqData);
				return;
			}

			ForceCloseParam* pParam = (ForceCloseParam*)reqData.pData;
			int nNum = 0;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from FORCECLOSEPARAM t where t.tradeaccountid = %d",pParam->nTradeAccountID);
			if ( !GET_DBACCESS().ExcuteSelect(szBuffer, nNum))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddForceCloseParam_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				if ( nNum == 0 )
				{
					//添加
					sprintf(szBuffer, "insert into FORCECLOSEPARAM values(%d,%d,%f,%d,%d)",
						pParam->nTradeAccountID, pParam->nForceCloseInd, pParam->fThreshold,
						pParam->nCloseStrategy, pParam->nForceClosePriceOption);
				}
				else
				{
					//修改
					sprintf(szBuffer, "update FORCECLOSEPARAM t set t.forcecloseind = %d, \
									  t.threshold = %f,t.closestrategy = %d,t.forceclosepriceoption = %d \
									  where t.tradeaccountid = %d",
									  pParam->nForceCloseInd, pParam->fThreshold,
									  pParam->nCloseStrategy, pParam->nForceClosePriceOption,
									  pParam->nTradeAccountID);
				}
				if ( !GET_DBACCESS().Excute(szBuffer))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AddForceCloseParam_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
				}
				else
				{
					RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AddForceCloseParam_Rsp, 
						&pParam->nTradeAccountID, sizeof(int), reqData.head.seq);
				}
			}
		}
		break;
	case Cmd_RM_QueryForceCloseParam_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}

			int nTradeAccountID = *(int*)reqData.pData;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from FORCECLOSEPARAM t where t.tradeaccountid = %d",nTradeAccountID);
			vector<ForceCloseParam> vec;
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_QueryForceCloseParam_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_QueryForceCloseParam_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	default:
		break;
	}
}
void CWorker::ProcessStatisticsRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_SubscribeStatistics_Req://订阅统计请求	
		{
			int pkgLen=reqData.head.len;
			int AccCnt=pkgLen/sizeof(AssetOrgProRelation);
			int AccRem=pkgLen%sizeof(AssetOrgProRelation);
			if (pkgLen>0||AccRem==0)
			{
				std::vector<StatisticsEntry> vec;
				AssetOrgProRelation* pOrgID = (AssetOrgProRelation*)reqData.pData;
				for (int index=0;index<AccCnt;++index)
				{
					AssetOrgProRelation& OrgID=pOrgID[index];
					char buffer[256]={0};
					sprintf_s(buffer,256,"%08X%08X",OrgID.nAssetMgmtOrgID,OrgID.nFinancialProductID);
					m_pThreadSharedData->Subscribe(reqData.hSocket,SubscribeID_Statistics,buffer);
					SAFE_GET_DATACENTER()->SubscribeStatistics(OrgID);
					SAFE_GET_DATACENTER()->GetStatisticsEntryList(OrgID,vec);
				}
				RspResultDataAutoDividePkg<StatisticsEntry>( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_SubscribeStatistics_Rsp, 
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_UnSubscribeStatistics_Req://退订统计请求	
		int pkgLen=reqData.head.len;
		int AccCnt=pkgLen/sizeof(AssetOrgProRelation);
		int AccRem=pkgLen%sizeof(AssetOrgProRelation);
		if (pkgLen>0||AccRem==0)
		{
			AssetOrgProRelation* pOrgID = (AssetOrgProRelation*)reqData.pData;
			for (int index=0;index<AccCnt;++index)
			{
				AssetOrgProRelation& OrgID=pOrgID[index];
				char buffer[256]={0};
				sprintf_s(buffer,256,"%08X%08X",OrgID.nAssetMgmtOrgID,OrgID.nFinancialProductID);
				m_pThreadSharedData->UnSubscribe(reqData.hSocket,SubscribeID_Statistics,buffer);
				SAFE_GET_DATACENTER()->UnSubscribeStatistics(OrgID);
			}
			RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_UnSubscribeStatistics_Rsp, reqData.pData,reqData.head.len, reqData.head.seq);
		}
		break;
	}
}
void CWorker::ProcessGatherRequest( const RequestData& reqData )
{
	switch(reqData.head.cmdid)
	{
	case Cmd_RM_Gather_ByOrgID_Req://订阅（某部门）下所有帐户汇总信息请求
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nOrgID = *(int*)reqData.pData;
		
			std::vector<AssetMgmtOrganization> vOrg;
			if ( !GetLowerAssetOrg(nOrgID, vOrg))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_Gather_ByOrgID_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
				return;
			}

			vector<TrustTradeAccount> vec;
			char szBuffer[MAX_SQL_LENGTH];
			for( UINT i = 0; i < vOrg.size(); i++ )
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "select * from TRUSTTRADEACCOUNT t where t.assetmgmtorgid = %d", vOrg[i]);

				if ( !GET_DBACCESS().QueryData(szBuffer, vec))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_Gather_ByOrgID_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
					return;
				}
			}
			SGather sGather;
			memset(&sGather, 0, sizeof(sGather));
			ProcessGatherAccountVec(0, nOrgID, vec, sGather);
			RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_Gather_ByOrgID_Rsp, 
				&sGather, sizeof(SGather), reqData.head.seq);	
				
		}
		break;
	case Cmd_RM_Gather_ByFinancialProductID_Req://订阅（某理财产品）下所有帐户汇总信息请求		
		{			
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nFinancialProductID = *(int*)reqData.pData;

			std::map<std::string,TrustTradeAccount> AccountList;
			CDataCenter::Get()->GetAccountList(AccountList);
			vector<TrustTradeAccount> vec;
			std::map<std::string,TrustTradeAccount>::iterator it = AccountList.begin();
			for(; it != AccountList.end(); it++)
			{
				TrustTradeAccount account = it->second;
				if(account.nFinancialProductID == nFinancialProductID)
					vec.push_back(account);
			}
			if(vec.size()==0)
			{
				std::string  str = "该理财产品没有相对应得委托交易账户";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_Gather_ByFinancialProductID_Rsp, 
					(void *)str.c_str(), 
					str.length()+1, reqData.head.seq);
				return;
			}
			SGather sGather;
			memset(&sGather, 0, sizeof(sGather));
			ProcessGatherAccountVec(1, nFinancialProductID, vec, sGather);			
		
			RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_Gather_ByFinancialProductID_Rsp, 
				&sGather, sizeof(SGather), reqData.head.seq);	

		}
		break;
	case Cmd_RM_Gather_ByAccountID_Req://订阅（某交易账号）下所有帐户汇总信息请求			
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nAccountID = *(int*)reqData.pData;
			TrustTradeAccount account;
			std::string strID = CDataCenter::Get()->GetTradeAccountID( nAccountID);			
			if(!CDataCenter::Get()->GetTradeAccount(strID, account))
			{
				std::string  str = "没有该账户信息";
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_Gather_ByAccountID_Rsp, 
					(void *)str.c_str(), 
					str.length()+1, reqData.head.seq);
				return;
			}
			vector<TrustTradeAccount> vec;
			vec.push_back(account);
			SGather sGather;
			memset(&sGather, 0, sizeof(sGather));
			ProcessGatherAccountVec(2, nAccountID, vec, sGather);
			RspResultData( reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_Gather_ByAccountID_Rsp, 
				&sGather, sizeof(SGather), reqData.head.seq);	
		}
		break;
	case Cmd_RM_Rank_ByAccount_Req:
		{//昨日帐户收益Top10排行榜
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nTop = *(int*)reqData.pData;

			vector<SGather> vec;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from (select t1.id,t1.idtype,t1.initprofit,t1.curprofit,t1.offsetprofit,\
				t1.annualprofitrate,t1.marginrate,to_char(t1.updatedate, 'YYYY-MM-DD'), t1.rjvalue, t1.daycount\
				from GATHERDATA t1 where to_char(t1.updatedate, 'YYYY-MM-DD') = \
				(select max(to_char(t.updatedate, 'YYYY-MM-DD')) from GATHERDATA t ) and idtype = 2 order by offsetprofit DESC) where rownum<=%d",
							  nTop);
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_Rank_ByAccount_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_Rank_ByAccount_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_Rank_ByFinancialProductID_Req:
		{//昨日理财品种收益Top10排行榜
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nTop = *(int*)reqData.pData;

			vector<SGather> vec;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from (select t1.id,t1.idtype,t1.initprofit,t1.curprofit,t1.offsetprofit,\
							  t1.annualprofitrate,t1.marginrate,to_char(t1.updatedate, 'YYYY-MM-DD'), t1.rjvalue, t1.daycount\
							  from GATHERDATA t1 where to_char(t1.updatedate, 'YYYY-MM-DD') = \
							  (select max(to_char(t.updatedate, 'YYYY-MM-DD')) from GATHERDATA t ) and idtype = 1 order by offsetprofit DESC) where rownum<=%d",
							  nTop);
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_Rank_ByFinancialProductID_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_Rank_ByFinancialProductID_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_AnnualRate_ByAccount_Req:
		{//年化帐户Top10排行榜,以年化收益率为排序条件
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nTop = *(int*)reqData.pData;

			vector<SGather> vec;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from (select t1.id,t1.idtype,t1.initprofit,t1.curprofit,t1.offsetprofit,\
							  t1.annualprofitrate,t1.marginrate,to_char(t1.updatedate, 'YYYY-MM-DD'), t1.rjvalue, t1.daycount\
							  from GATHERDATA t1 where to_char(t1.updatedate, 'YYYY-MM-DD') = \
							  (select max(to_char(t.updatedate, 'YYYY-MM-DD')) from GATHERDATA t ) and idtype = 2 order by annualprofitrate DESC) where rownum<=%d",
							  nTop);
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AnnualRate_ByAccount_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AnnualRate_ByAccount_Rsp,
					vec, reqData.head.seq);
			}

		}
		break;
	case Cmd_RM_AnnualRate_ByFinancialProductID_Req:
		{//昨日理财品种收益Top10排行榜,以年化收益率为排序条件
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nTop = *(int*)reqData.pData;

			vector<SGather> vec;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from (select t1.id,t1.idtype,t1.initprofit,t1.curprofit,t1.offsetprofit,\
							  t1.annualprofitrate,t1.marginrate,to_char(t1.updatedate, 'YYYY-MM-DD'), t1.rjvalue, t1.daycount\
							  from GATHERDATA t1 where to_char(t1.updatedate, 'YYYY-MM-DD') = \
							  (select max(to_char(t.updatedate, 'YYYY-MM-DD')) from GATHERDATA t ) and idtype = 1 order by annualprofitrate DESC) where rownum<=%d",
							  nTop);
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AnnualRate_ByFinancialProductID_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AnnualRate_ByFinancialProductID_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_AnnualRate_ByFinancialProductID_Worst_Req:
		{//昨日理财品种收益最差Top10排行榜,以年化收益率为反向排序条件
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nTop = *(int*)reqData.pData;

			vector<SGather> vec;
			char szBuffer[MAX_SQL_LENGTH];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "select * from (select t1.id,t1.idtype,t1.initprofit,t1.curprofit,t1.offsetprofit,\
							  t1.annualprofitrate,t1.marginrate,to_char(t1.updatedate, 'YYYY-MM-DD'), t1.rjvalue, t1.daycount\
							  from GATHERDATA t1 where to_char(t1.updatedate, 'YYYY-MM-DD') = \
							  (select max(to_char(t.updatedate, 'YYYY-MM-DD')) from GATHERDATA t ) and idtype = 1 order by annualprofitrate ) where rownum<=%d",
							  nTop);
			if ( !GET_DBACCESS().QueryData(szBuffer, vec))
			{
				RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_RM_AnnualRate_ByFinancialProductID_Worst_Rsp, 
					(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
					GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
			}
			else
			{
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_AnnualRate_ByFinancialProductID_Worst_Rsp,
					vec, reqData.head.seq);
			}
		}
		break;
	case Cmd_RM_GetAccountList_ByFinancialProductID_Req:
		{
			if ( reqData.head.len != sizeof(int))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			int nProductID = *(int*)reqData.pData;		
			vector<TrustTradeAccount> vec;

			std::map<std::string,TrustTradeAccount> AccountList;
			CDataCenter::Get()->GetAccountList(AccountList);

			std::map<std::string,TrustTradeAccount>::iterator it = AccountList.begin();
			for(; it != AccountList.end(); it++)
			{
				TrustTradeAccount& tAccount = it->second;
				if(tAccount.nFinancialProductID == nProductID)
					vec.push_back(tAccount);
			}
			RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_RM_GetAccountList_ByFinancialProductID_Rsp,
				vec, reqData.head.seq);
		}
		break;
	case Cmd_Rm_GetProfitCurve_Req:
		{
			if ( reqData.head.len != sizeof(SProfitCurve))
			{
				OutputUnkownReqLog(reqData);
				return;
			}
			SProfitCurve* sProfitCurve = (SProfitCurve*)reqData.pData;	
			if(sProfitCurve == NULL)
				return;
			if(sProfitCurve->nIDType == 1)
			{//1 资管组织下面的交易账号
				int nOrgID = sProfitCurve->nID;

				std::vector<AssetMgmtOrganization> vOrg;
				if ( !GetLowerAssetOrg(nOrgID, vOrg))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_Rm_GetProfitCurve_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
					return;
				}

				std::multimap<std::string, SProfit> sProfit; 
				char szBuffer[MAX_SQL_LENGTH];
				for( UINT i = 0; i < vOrg.size(); i++ )
				{
					memset(szBuffer, 0, sizeof(szBuffer));
					sprintf(szBuffer, "select t1.tradeaccount,  t1.profit, t2.balance, t3.offsetprofit, to_char(t3.UPDATEDATE,'YYYYMMDD') \
						from TRUSTTRADEACCOUNT t1, RISKSYNCACCOUNT t2, Gatherdata t3 \
					where t1.tradeaccount = t2.investorid and t2.RECVTIME = to_char(t3.UPDATEDATE,'YYYYMMDD') \
						and t3.id = %d and t3.idtype = 0  and t1.assetmgmtorgid = %d and\
						to_char(t3.UPDATEDATE,'YYYYMMDD')  >='%s' and to_char(t3.UPDATEDATE,'YYYYMMDD') <='%s'\
						order by to_date(t2.RECVTIME,'YYYYMMDD') ", vOrg[i].nAssetMgmtOrgID, vOrg[i].nAssetMgmtOrgID, sProfitCurve->DateBegin, sProfitCurve->DateEnd);

					if ( !GET_DBACCESS().QueryData(szBuffer, sProfit))
					{
						RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_Rm_GetProfitCurve_Rsp, 
							(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
							GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
						return;
					}
				}


				vector<SProfitCurveReturn> sProfitCurveData;				
				while(1)
				{
					std::multimap<std::string, SProfit>::iterator it= sProfit.begin();
					if(it == sProfit.end())
						break;
					std::string strDate = it->first;
					int nCount	= sProfit.count(strDate);
					std::multimap<std::string, SProfit>::iterator itFind = sProfit.find(strDate);

					SProfitCurveReturn sData;
					memset(&sData, 0, sizeof(sData));
					sData.nIDType	= sProfitCurve->nIDType;
					sData.nID		= nOrgID;
					strcpy(sData.Date, strDate.c_str());
					for(int i=0; i< nCount; i++)
					{
						SProfit& sProfit = itFind->second;
						sData.dbSumProfit += sProfit.dbBalance - sProfit.dbProfit;
						sData.dbOffsetBalance += sProfit.dbOffsetProfit;
						itFind++;
					}
					sProfitCurveData.push_back(sData);
					sProfit.erase(strDate);
				}
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_Rm_GetProfitCurve_Rsp,
					sProfitCurveData, reqData.head.seq);
			}
			else if(sProfitCurve->nIDType == 0)
			{//0, 理财产品下面的交易账号
				int nFinancialProductID = sProfitCurve->nID;

				std::multimap<std::string, SProfit> sProfit; 
				char szBuffer[MAX_SQL_LENGTH];
				
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "select t1.tradeaccount,  t1.profit, t2.balance, t3.offsetprofit, to_char(t3.UPDATEDATE,'YYYYMMDD') \
								  from TRUSTTRADEACCOUNT t1, RISKSYNCACCOUNT t2, Gatherdata t3 \
								  where t1.tradeaccount = t2.investorid and t2.RECVTIME = to_char(t3.UPDATEDATE,'YYYYMMDD') \
								  and t3.id = %d and t3.idtype = 1  and t1.FINANCIALPRODUCTID = %d and\
								  to_char(t3.UPDATEDATE,'YYYYMMDD')  >='%s' and to_char(t3.UPDATEDATE,'YYYYMMDD') <='%s'\
								  order by to_date(t2.RECVTIME,'YYYYMMDD') ", nFinancialProductID, nFinancialProductID, sProfitCurve->DateBegin, sProfitCurve->DateEnd);

				if ( !GET_DBACCESS().QueryData(szBuffer, sProfit))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_Rm_GetProfitCurve_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
					return;
				}

				vector<SProfitCurveReturn> sProfitCurveData;				
				while(1)
				{
					std::multimap<std::string, SProfit>::iterator it= sProfit.begin();
					if(it == sProfit.end())
						break;
					std::string strDate = it->first;
					int nCount	= sProfit.count(strDate);
					std::multimap<std::string, SProfit>::iterator itFind = sProfit.find(strDate);

					SProfitCurveReturn sData;
					memset(&sData, 0, sizeof(sData));

					sData.nIDType	= sProfitCurve->nIDType;
					sData.nID		= nFinancialProductID;
					strcpy(sData.Date, strDate.c_str());
					for(int i=0; i< nCount; i++)
					{
						SProfit& sProfit = itFind->second;
						sData.dbSumProfit += sProfit.dbBalance - sProfit.dbProfit;
						sData.dbOffsetBalance += sProfit.dbOffsetProfit;
						itFind++;
					}
					sProfitCurveData.push_back(sData);
					sProfit.erase(strDate);
					
				}		
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_Rm_GetProfitCurve_Rsp,
					sProfitCurveData, reqData.head.seq);
			}
			else if(sProfitCurve->nIDType == 2)
			{//2 交易账号
				int nAccountID = sProfitCurve->nID;

				std::multimap<std::string, SProfit> sProfit; 
				char szBuffer[MAX_SQL_LENGTH];

				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "select t1.tradeaccount,  t1.profit, t2.balance, t3.offsetprofit, to_char(t3.UPDATEDATE,'YYYYMMDD') \
								  from TRUSTTRADEACCOUNT t1, RISKSYNCACCOUNT t2, Gatherdata t3 \
								  where t1.tradeaccount = t2.investorid and t2.RECVTIME = to_char(t3.UPDATEDATE,'YYYYMMDD') \
								  and t3.id = %d and t3.idtype = 2  and t1.TRADEACCOUNTID = %d and\
								  to_char(t3.UPDATEDATE,'YYYYMMDD')  >='%s' and to_char(t3.UPDATEDATE,'YYYYMMDD') <='%s'\
								  order by to_date(t2.RECVTIME,'YYYYMMDD') ", nAccountID, nAccountID, sProfitCurve->DateBegin, sProfitCurve->DateEnd);

				if ( !GET_DBACCESS().QueryData(szBuffer, sProfit))
				{
					RspResultData( reqData.hSocket, OPERATOR_FAILED, Cmd_Rm_GetProfitCurve_Rsp, 
						(void*)GET_DBACCESS().GetLastErrorString().c_str(), 
						GET_DBACCESS().GetLastErrorString().length()+1, reqData.head.seq);
					return;
				}

				vector<SProfitCurveReturn> sProfitCurveData;				
				while(1)
				{
					std::multimap<std::string, SProfit>::iterator it= sProfit.begin();
					if(it == sProfit.end())
						break;
					std::string strDate = it->first;
					int nCount	= sProfit.count(strDate);
					std::multimap<std::string, SProfit>::iterator itFind = sProfit.find(strDate);

					SProfitCurveReturn sData;
					memset(&sData, 0, sizeof(sData));

					sData.nIDType	= sProfitCurve->nIDType;
					sData.nID		= nAccountID;
					strcpy(sData.Date, strDate.c_str());
					for(int i=0; i< nCount; i++)
					{
						SProfit& sProfit = itFind->second;
						sData.dbSumProfit += sProfit.dbBalance - sProfit.dbProfit;
						sData.dbOffsetBalance += sProfit.dbOffsetProfit;
						itFind++;
					}
					sProfitCurveData.push_back(sData);
					sProfit.erase(strDate);

				}		
				RspResultDataAutoDividePkg(reqData.hSocket, OPERATOR_SUCCESS, Cmd_Rm_GetProfitCurve_Rsp,
					sProfitCurveData, reqData.head.seq);
			}

			
		}
		break;
		
	
	}	
}
void CWorker::ProcessGatherAccountVec(int nTypeID, int nID, vector<TrustTradeAccount>& vec, SGather& sGather)
{
	std::vector<SGatherData> vGatherData;
	CDataCenter::Get()->ProcessGatherAccountVec(nTypeID,  nID,  vec,  vGatherData);
	if(vGatherData.size() ==0)
		return;
	SGatherData& temp = vGatherData[0];

	sGather.nIDType				= temp.nIDType;
	sGather.nID					= temp.nID;
	strcpy(sGather.szUpdateDate, temp.szUpdateDate);
	sGather.SumPreBalance		= temp.SumPreBalance;
	sGather.SumBalance			= temp.SumBalance;
	sGather.OffBalance			= temp.OffBalance;
	sGather.YearEarnRate		= temp.YearEarnRate;
	sGather.MarginRate			= temp.MarginRate;
	
	/*
	if(vec.size() == 0)
		return;

	//SGather sGather;
	//memset(&sGather, 0, sizeof(sGather));
	sGather.nIDType = nTypeID;
	sGather.nID     = nID;

	double dbCurrMargin = 0.0;
	double dbSumWithDraw = 0.0;
	double dbSumPreBalance = 0.0;
	for(int k =0; k < (int)vec.size(); k++)
	{
		double dbWithDraw = 0.0;
		TrustTradeAccount tAccount = vec[k];
		if(!CDataCenter::Get()->GetWithDraw(tAccount.szAccount, dbWithDraw))
			continue;
		
		dbSumWithDraw += dbWithDraw;//总出入金相加
		sGather.SumPreBalance += tAccount.dProfit + dbWithDraw;			//初始权益和		
		double dbBalance =  CDataCenter::Get()->GetBalance(tAccount.szAccount);
		sGather.SumBalance += dbBalance;					//当前权益和

		double dbPreBalance = 0.0;
		CDataCenter::Get()->GetPreBalance(tAccount.szAccount, dbPreBalance);	
		dbSumPreBalance += dbPreBalance;
		sGather.OffBalance += dbBalance - dbPreBalance; //今日权益变化
		dbCurrMargin += CDataCenter::Get()->GetCurrMargin(tAccount.szAccount);//保证金比率
	}
	sGather.MarginRate = dbCurrMargin/sGather.SumBalance;

	double dbrj = 0;
	if(dbSumWithDraw > 0)
		dbrj = sGather.SumBalance/(dbSumPreBalance + dbSumWithDraw)-1;
	else
		dbrj = (sGather.SumBalance + dbSumWithDraw)/dbSumPreBalance-1;

	SGatherData sGathDataTemp;
	if(CDataCenter::Get()->GetGatherData(nTypeID, nID, sGathDataTemp ))
	{//得到昨日保存的“年华收益率”值			
		sGather.YearEarnRate = ((1 + sGathDataTemp.dbValue)*(1+dbrj) -1)/(sGathDataTemp.nDayCount +1)*220;			
	}
	else
	{			
		sGather.YearEarnRate = ((1+dbrj) -1)*220;						
	}
*/
	
}