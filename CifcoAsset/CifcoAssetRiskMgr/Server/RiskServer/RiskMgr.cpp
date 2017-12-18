#include "StdAfx.h"
#include "RiskMgr.h"
#include "RiskMsgQueue.h"
#include "ToolThread.h"
#include "DBAccess.h"
#include "DataCenter.h"
#include "RiskMsgCalc.h"
#include "UserApi.h"
#include "Hash.h"
#include "../CloseOrder/CloseOrder.h"
#include "CryptoAPI.h"
#define  MainRiskThread  0
#define  QuotRiskThread  1
#define  SyncRiskThread  2
#define  ClientRiskThread  3
#define MAX_CAN_CLOSE_VOLUMN	100

CRiskMgr* CRiskMgr::m_pRiskMgr=NULL;
DWORD WINAPI CRiskMgr::RiskThreadProcMain (PVOID pParam)
{
	CRiskMgr* pMgr=(CRiskMgr*)pParam;
	pMgr->doProcessBusinessDataMain();
	return 0;
}
DWORD WINAPI CRiskMgr::RiskThreadProcQuot (PVOID pParam)
{
	CRiskMgr* pMgr=(CRiskMgr*)pParam;
	pMgr->doProcessBusinessDataQuot();
	return 0;
}
DWORD WINAPI CRiskMgr::RiskThreadProcSync (PVOID pParam)
{
	CRiskMgr* pMgr=(CRiskMgr*)pParam;
	pMgr->doProcessBusinessDataSync();
	return 0;
}
DWORD WINAPI CRiskMgr::RiskThreadProcStatistics (PVOID pParam)
{
	CRiskMgr* pMgr=(CRiskMgr*)pParam;
	pMgr->doProcessBusinessDataStatistics();
	return 0;
}
DWORD WINAPI CRiskMgr::RiskThreadProcClient (PVOID pParam)
{
	CRiskMgr* pMgr=(CRiskMgr*)pParam;
	pMgr->doProcessBusinessDataClient();
	return 0;
}

DWORD WINAPI CRiskMgr::RiskThreadProcForceClose (PVOID pParam)
{
	CRiskMgr* pMgr=(CRiskMgr*)pParam;
	pMgr->doProcessBusinessDataForceClose();
	return 0;
}
DWORD WINAPI CRiskMgr::JoinThreadProc(PVOID pParam)
{
	CRiskMgr* pMgr=(CRiskMgr*)pParam;
	while (!pMgr->m_JoinThread->IsNeedExit())
	{
		UserApi* pApi=pMgr->GetApi();
		if(pApi)
		{
			pApi->Init();
			pApi->Join();
		}
		Sleep(1000);
	}
	return 0;
}
CStringPair* g_StringPair=NULL;
CRiskMgr::CRiskMgr(void)
:m_IdleSleepTime(10)
,m_CtpNextIndex(0)
{
	g_StringPair=new CStringPair();
	m_Api=UserApi::CreateUserApi();
	m_Api->RegisterSpi();
	m_RiskMsgCalc=new CRiskMsgCalc(m_Api);
	m_RiskMgrThread=new CToolThread(RiskThreadProcMain,this);
	m_RiskMgrThread_Quot=new CToolThread(RiskThreadProcQuot,this);
	m_RiskMgrThread_Sync=new CToolThread(RiskThreadProcSync,this);
	m_RiskMgrThread_Stat=new CToolThread(RiskThreadProcStatistics,this);
	m_Client2RiskThread=new CToolThread(RiskThreadProcClient,this);
	m_RiskMgrThread_AutoForceClose = new CToolThread(RiskThreadProcForceClose, this);

	std::vector<std::string> strFrontAddr;
	SAFE_GET_DATACENTER()->GetrFrontAddr(strFrontAddr);
	for (size_t i=0;i<strFrontAddr.size();++i)
	{
		m_Api->RegisterFront((char*)strFrontAddr[i].c_str());
	}
	m_JoinThread=new CToolThread(JoinThreadProc,this);

	std::string strTradeAddr;
	SAFE_GET_DATACENTER()->GetTradeAddr(strTradeAddr);
	SetFrontAddr(strTradeAddr);
}
CRiskMgr::~CRiskMgr(void)
{
	if(m_Api)
	{
		m_Api->Release();
		m_Api=NULL;
	}
	SAFE_DELETE(m_JoinThread);
	SAFE_DELETE(m_RiskMgrThread);
	SAFE_DELETE(m_RiskMgrThread_Quot);
	SAFE_DELETE(m_RiskMgrThread_Sync);
	SAFE_DELETE(m_RiskMgrThread_Stat);
	SAFE_DELETE(m_Client2RiskThread);
	SAFE_DELETE(m_RiskMgrThread_AutoForceClose);
	SAFE_DELETE(m_RiskMsgCalc);
	SAFE_DELETE(g_StringPair);
}

void CRiskMgr::Init(void)
{
	if(m_pRiskMgr)
	{
		Release();
	}
	m_pRiskMgr=new CRiskMgr();
}

void CRiskMgr::Release(void)
{
	SAFE_DELETE(m_pRiskMgr);
}
UserApi* CRiskMgr::GetApi()
{
	return m_Api;
}
int CRiskMgr::GetNextID()
{
	return ++m_CtpNextIndex;
}
void CRiskMgr::doProcessBusinessDataMain()
{
	while (!m_RiskMgrThread->IsNeedExit())
	{
		SpiData data;
		if(CRiskMsgQueue::Get()->GetMsg(data))
		{
			switch(data.CmdID)
			{
			case MSGID_FrontConnected:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("前端连接成功");
					ReqRiskUserLoginField login={0};
					SAFE_GET_DATACENTER()->GetUserLoginField(login);
					m_Api->ReqRiskUserLogin(&login);						
				}
				break;
			case MSGID_FrontDisconnected:
				{
					SAFE_GET_DATACENTER()->SaveAllData();//断开之前存储一下所有数据
					SAFE_GET_DATACENTER()->FirstPrintf("前端连接断开",true);
					std::vector<std::string> strFrontAddr;
					SAFE_GET_DATACENTER()->GetrFrontAddr(strFrontAddr);
					for (size_t i=0;i<strFrontAddr.size();++i)
					{
						m_Api->RegisterFront((char*)strFrontAddr[i].c_str());
					}		
				}
				break;
			case MSGID_HeartBeatWarning:
				{
					int nTimeLapse=data.Main.nTimeLapse;
					nTimeLapse;
				}
				break;
			case MSGID_RspError:
				break;
			case MSGID_RtnBrokerDeposit:
				break;
			case MSGID_RtnInvestorSumInfo:
				{
				}
				break;
			case MSGID_RtnClientSGDataSyncStart:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("客户端结算组数据同步开始");
				}
				break;
			case MSGID_RtnClientSGDataSyncEnd:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("客户端结算组数据同步结束");
					RiskNtfSequenceField InvestorIDRange={0};
					m_Api->ReqSubPreRiskAccount(&InvestorIDRange);
					//订阅有序的出入金
					RiskNtfSequenceField NtfSequence={0};
					m_Api->ReqSubSeqDeposit(&NtfSequence);			
					NotifySequenceField  NotifySequence={0};
					m_Api->ReqSubscribeOrder(&NotifySequence);
					m_Api->ReqSubscribeTrade(&NotifySequence);	
					m_Api->ReqSubscribePosition(&NotifySequence);
					std::vector<std::string> instrumentlist;
					SAFE_GET_DATACENTER()->GetInstrumentList(instrumentlist);
					SubMarketDataField* pSubMarketData=new SubMarketDataField[instrumentlist.size()];
					for (size_t i=0;i<instrumentlist.size();++i)
					{
						safestrcpy(pSubMarketData[i].InstrumentID,sizeof(TInstrumentIDType),
							instrumentlist[i].c_str());
					}
					m_Api->ReqSubRiskMarketData(pSubMarketData,instrumentlist.size());
					delete[] pSubMarketData;
				}
				break;
			case MSGID_RspRiskUserLogin:
				{
					if(data.RspInfo.ErrorID==0)
					{
						SAFE_GET_DATACENTER()->FirstPrintf("登录成功");
						SAFE_GET_DATACENTER()->SuccessLogin();
					}
					else
					{
						SAFE_GET_DATACENTER()->FirstPrintf(data.RspInfo.ErrorMsg);
						//登录不成功就一直试着登录
						ReqRiskUserLoginField login={0};
						SAFE_GET_DATACENTER()->GetUserLoginField(login);
						m_Api->ReqRiskUserLogin(&login);
					}

				}
				break;
			case MSGID_RspQryInvestorMarginRate:
				break;
			case MSGID_RtnProduct:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("推送产品信息");
					ProductField& Product=data.Main.Product;
					SAFE_GET_DATACENTER()->AddProduct(Product);	
					
					SAFE_GET_DATACENTER()->WriteLogInfo("产品代码", Product.ProductID);
				}
				break;
			case MSGID_RtnInstrument:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("推送合约信息");
					InstrumentField& Instrument=data.Main.Instrument;
					SAFE_GET_DATACENTER()->AddInstrument(Instrument);

					SAFE_GET_DATACENTER()->WriteLogInfo("合约代码", Instrument.InstrumentID);
				}
				break;
			case MSGID_RspQryOrderStat:
				break;
			case MSGID_RtnExchange:
				break;
			case MSGID_RspInvestorPositionStatic:
				break;
			case MSGID_RspInvestorTradeStatic:
				break;
			case MSGID_RtnTimeSync:
				break;
			case MSGID_RspInstPositionRate:
				break;
			case MSGID_RspProductPositionRate:
				break;
			case MSGID_RtnInvestorGroup:
				break;
			case MSGID_RtnExchangeMarginRate:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("推送交易所保证金率");
				}
				break;
			case MSGID_RtnExchangeMarginRateAdjust:
				break;
			case MSGID_RspQryTradingCodeHash:
				break;
			case MSGID_RspQryTradingCode:
				break;
			case MSGID_RtnTradingCode:
				break;
			case MSGID_RtnDelTradingCode:
				break;
			case MSGID_RtnSequencialBrokerUserEvent:
				break;
			case MSGID_RtnSequencialTrade:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("返回查询成交回报");
					SequencialTradeField& SequencialTrade=data.Main.SequencialTrade;
					if(CDataCenter::Get()&&CDataCenter::Get()->AccountListIsExist(SequencialTrade.InvestorID))
					{
						SAFE_GET_DATACENTER()->AddTrade(SequencialTrade);
						if(CDataCenter::Get()->IsOK())
							m_RiskMsgCalc->NewTrade(SequencialTrade.InvestorID);
					}
					SAFE_GET_DATACENTER()->WriteLogInfo("成交投资者代码",  SequencialTrade.InvestorID);
				}
				break;
			case MSGID_RtnSequencialOrder:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("返回查询报单回报");
					SequencialOrderField& SequencialOrder=data.Main.SequencialOrder;
					if(CDataCenter::Get()&&CDataCenter::Get()->AccountListIsExist(SequencialOrder.InvestorID))
					{
						SAFE_GET_DATACENTER()->AddOrder(SequencialOrder);
						if(CDataCenter::Get()->IsOK())
							m_RiskMsgCalc->NewOrder(SequencialOrder.InvestorID);
					}
					SAFE_GET_DATACENTER()->WriteLogInfo("报单投资者代码",  SequencialOrder.InvestorID);
				}
				break;
			case MSGID_RspRiskOrderInsert:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("返回强平下单回报");
					InputOrderField& InputOrder=data.Main.InputOrder;
					if(CRisk2ClientMsgQueue::Get())
					{
						Risk2ClientData rcdata={0};
						rcdata.CmdID=R2CMSGID_INPUTORDER;
						rcdata.Main.InputOrder=InputOrder;
						CRisk2ClientMsgQueue::Get()->AddMsg(rcdata);
					}
					SAFE_GET_DATACENTER()->WriteLogInfo("报单经纪公司代码", InputOrder.BrokerID);
				}
				break;
			case MSGID_RspRiskOrderAction:
				break;
			case MSGID_RtnSequencialPosition:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("返回查询持仓回报");
					SequencialPositionField& SequencialPosition=data.Main.SequencialPosition;
					if(CDataCenter::Get()&&CDataCenter::Get()->AccountListIsExist(SequencialPosition.InvestorID))
					{
						SAFE_GET_DATACENTER()->AddPosition(SequencialPosition);
						if(CDataCenter::Get()->IsOK())
							m_RiskMsgCalc->NewPosition(SequencialPosition.InvestorID,SequencialPosition.InstrumentID);
					}
					SAFE_GET_DATACENTER()->WriteLogInfo("持仓投资者代码",  SequencialPosition.InvestorID);
				}
				break;
			case MSGID_RspRiskNotifyCommand:
				break;
			case MSGID_RspBatchForceCloseCalc:
				break;
			case MSGID_RspForceCloseCalc:
				break;
			case MSGID_RspSetIndexNPPPara:
				break;
			case MSGID_RtnIndexNPP:
				break;
			case MSGID_RspRiskDelIndexNPPPara:
				break;
			case MSGID_RspForceCloseAccount:
				break;
			case MSGID_RspQryLogin:
				break;
			case MSGID_RspQrySafePriceRange:
				break;
			case MSGID_RspSafePriceAccount:
				break;
			case MSGID_RspQryPriceVaryEffect:
				break;
			case MSGID_RtnDepartment:
				break;
			case MSGID_RspIndexNPP:
				break;
			case MSGID_RtnTradeParam:
				break;
			case MSGID_RspRiskParkedOrderInsert:
				break;
			case MSGID_RspRemoveRiskParkedOrder:
				break;
			case MSGID_RtnSeqRiskParkedOrder:
				break;
			case MSGID_RspRiskUserPasswordUpd:
				break;
			case MSGID_RtnSeqDeposit:
				{
					SeqDepositField& dep=data.Main.SeqDeposit;
					if(CDataCenter::Get()&&CDataCenter::Get()->AccountListIsExist(dep.InvestorID))
					{
						SAFE_GET_DATACENTER()->AddSeqDeposit(dep);
					}
					SAFE_GET_DATACENTER()->WriteLogInfo("出入金投资者代码", dep.InvestorID);
				}
				break;
			case MSGID_RspAddRiskUserEvent:
				break;
			case MSGID_RspPredictRiskAccount:
				break;
			case MSGID_RspPredictRiskPosition:
				break;
			case MSGID_RtnRiskInvestor:
				break;
			case MSGID_RspQryInvestorLinkMan:
				break;
			case MSGID_RtnDelExchangeMarginRate:
				break;
			case MSGID_RtnDelExchangeMarginRateAdjust:
				break;
			case MSGID_RspQryInvestorDepartment:
				break;
			case MSGID_RtnDelIndexNPP:
				break;
			case MSGID_RtnRiskUserFunction:
				{
				}
				break;
			case MSGID_RtnDelRiskUserFunction:
				break;
			case MSGID_RtnSeqPreRiskAccount:
				{
					SAFE_GET_DATACENTER()->FirstPrintf("返回投资者昨日资金");
					SeqPreRiskAccountField& PreRiskAccount=data.Main.SeqPreRiskAccount;
					if(CDataCenter::Get()&&CDataCenter::Get()->AccountListIsExist(PreRiskAccount.InvestorID))
					{
						SAFE_GET_DATACENTER()->AddPreRiskFundAccount(PreRiskAccount);
					}

					SAFE_GET_DATACENTER()->WriteLogInfo("风控资金账户昨日信息流中投资者代码", PreRiskAccount.InvestorID);
				}
				break;
			case MSGID_RtnNoticeToken:
				break;
			case MSGID_RtnNoticePattern:
				break;
			case MSGID_RspModNoticePattern:
				break;
			case MSGID_RtnVaryMarketData:
				break;
			case MSGID_RspAddRiskNotifyA:
				break;
			case MSGID_RspAddBizNotice:
				break;
			case MSGID_RtnSeqBizNotice:
				break;
			case MSGID_RspRiskQryBrokerDeposit:
				break;
			case MSGID_RtnRiskParamInfo:
				{
				}
				break;
			case MSGID_RspModRiskInvestorParam:
				break;
			case MSGID_RspRemRiskInvestorParam:
				break;
			case MSGID_RtnRiskInvestorParam:
				break;
			case MSGID_RtnDelRiskInvestorParam:
				break;
			case MSGID_RspForceRiskUserLogout:
				break;
			case MSGID_RtnForceRiskUserLogout:
				break;
			case MSGID_RspAddRiskPattern:
				break;
			case MSGID_RspModRiskPattern:
				break;
			case MSGID_RspRemRiskPattern:
				break;
			case MSGID_RtnRiskPattern:
				break;
			case MSGID_RtnDelRiskPattern:
				break;
			case MSGID_RspAddInvestorPattern:
				break;
			case MSGID_RspModInvestorPattern:
				break;
			case MSGID_RspRemInvestorPattern:
				break;
			case MSGID_RtnInvestorPattern:
				break;
			case MSGID_RtnDelInvestorPattern:
				break;
			case MSGID_RtnRiskNotifyToken:
				break;
			case MSGID_RtnSeqRiskNotifyB:
				break;
			case MSGID_RspQryPositionStat:
				break;
			case MSGID_RspQryTradeStat:
				break;
			case MSGID_RspQryInvestorLinkManHash:
				break;
			case MSGID_RspQryInvestorDepartmentHash:
				break;
			case MSGID_RspQryStressTest:
				break;
			case MSGID_RspQryLowMarginInvestorHash:
				break;
			case MSGID_RspQryLowMarginInvestor:
				break;
			case MSGID_RtnLowMarginInvestor:
				break;
			case MSGID_RtnDelLowMarginInvestor:
				break;
			}	
		}
		else
		{
			SAFE_GET_DATACENTER()->IdleBusinessDataMain();
			Sleep(m_IdleSleepTime);
		}
	}
}
void CRiskMgr::doProcessBusinessDataQuot()
{
	while (!m_RiskMgrThread_Quot->IsNeedExit())
	{
		SpiData data;
		if(CRiskMsgQueue_Quot::Get()->GetMsg(data))
		{
			if(data.CmdID==MSGID_RtnRiskDepthMarketData)
			{
				SAFE_GET_DATACENTER()->FirstPrintf("推送深度行情");
				DepthMarketDataField& DepthMarketData=data.Main.DepthMarketData;
				SAFE_GET_DATACENTER()->AddMarketData(DepthMarketData);
				if(CDataCenter::Get()->IsOK())
					m_RiskMsgCalc->NewDepthMarketData(DepthMarketData.InstrumentID);
			}
		}
		else
			Sleep(m_IdleSleepTime);
	}
}
void CRiskMgr::doProcessBusinessDataSync()
{
	while (!m_RiskMgrThread_Sync->IsNeedExit())
	{
		SpiData data;
		if(CRiskMsgQueue_Sync::Get()->GetMsg(data))
		{
			if(data.CmdID==MSGID_RtnRiskSyncAccount)
			{
				SAFE_GET_DATACENTER()->FirstPrintf("推送投资者当日资金回报");
				RiskSyncAccountField& RiskSyncAccount=data.Main.RiskSyncAccount;
				if(CDataCenter::Get()&&CDataCenter::Get()->AccountListIsExist(RiskSyncAccount.InvestorID))
				{
					SAFE_GET_DATACENTER()->AddRiskFundAccount(RiskSyncAccount);
					if(CDataCenter::Get()->IsOK())
					{
						double dbValue = m_RiskMsgCalc->CalcFundNetValueRisk(RiskSyncAccount.InvestorID);
						NetValueInfo netValueInfo;
						safestrcpy(netValueInfo.szAccount,sizeof(ACCOUNTSTRING), RiskSyncAccount.InvestorID);					
						netValueInfo.dbValue		= dbValue;

						TrustTradeAccount account;
						SAFE_GET_DATACENTER()->GetTradeAccount(RiskSyncAccount.InvestorID, account);
						SAFE_GET_DATACENTER()->SubscribeNetValue(account.nTradeAccountID);
						SAFE_GET_DATACENTER()->AddAcoount2NetValue(netValueInfo);
 
						m_RiskMsgCalc->NewFundAccount(RiskSyncAccount.InvestorID);
					}
				}
				SAFE_GET_DATACENTER()->WriteLogInfo("风控同步投资者资金账户", RiskSyncAccount.InvestorID);
			}
		}
		else
			Sleep(m_IdleSleepTime);
	}
}
void CRiskMgr::doProcessBusinessDataStatistics()
{
	while (!m_RiskMgrThread_Stat->IsNeedExit())
	{	
		if(CDataCenter::Get())
		SAFE_GET_DATACENTER()->OnTimerStatistics();
		Sleep(1000);
		if(CDataCenter::Get())
		{
			CDataCenter::Get()->UpdateBaseData();
			if(CDataCenter::Get()->IsRiskWarningChanged())
			{
				UpdateAllRiskEvent();
			}
		}
	}
}
void CRiskMgr::doProcessBusinessDataClient()
{
	while (!m_Client2RiskThread->IsNeedExit())
	{
		Client2RiskData data;
		if(CClient2RiskMsgQueue::Get()->GetMsg(data))
		{
			if(data.CmdID==C2RMSGID_ForceCloseOrderInsert)
			{
				RiskForceCloseOrderField& fc=data.Main.ForceCloseOrder;
				//m_Api->ReqRiskOrderInsert(&fc);

				//获取用户交易密码
				TrustTradeAccount account;
				memset(&account, 0, sizeof(account));
				if ( CDataCenter::Get() != NULL && 
					CDataCenter::Get()->GetTradeAccount(fc.InvestorID, account) )
				{
					CCryptoAPI::Decode3DES_ToString(account.szPwd);

					//启动交易软件下强平单
					CShfeFtdcRiskForceCloseOrderField orderField;
					memcpy(&orderField, &fc, sizeof(orderField));
					CThostFtdcInputOrderField ctpOrderField;
					Covent2CtpOrder(orderField, ctpOrderField);
					
					std::vector<CThostFtdcInputOrderField> vOrder;
					vOrder.clear();
					vOrder.push_back(ctpOrderField);
					std::vector<CThostFtdcInputOrderActionField> vOrderAction;
					vOrderAction.clear();
					InsertOrder(fc.InvestorID, account.szPwd, vOrderAction, vOrder);
				}
			}
		}
		else
			Sleep(m_IdleSleepTime);
	}
}

void CRiskMgr::doProcessBusinessDataForceClose()
{
	while (!m_RiskMgrThread_AutoForceClose->IsNeedExit())
	{
		std::string strAccount;
		if(CForceCloseMsgQueue::Get()->GetForceCloseMsg(strAccount))
		{
			//获取该账号的所有挂单
			std::vector<SequencialOrderField> vOrder;
			SAFE_GET_DATACENTER()->GetOpenOrder(strAccount, vOrder);

			//生成撤单结构
			std::vector<CThostFtdcInputOrderActionField> vOrderAction;
			GenerateOrderAction(vOrder, vOrderAction);

			//获取该账号所有持仓
			std::vector<SequencialPositionField> vPosition;
			SAFE_GET_DATACENTER()->GetPosition(strAccount, vPosition);
			if ( vPosition.empty() )
			{
				//已平完
				continue;
			}
			else
			{
				//生成平仓下单结构
				std::vector<CThostFtdcInputOrderField> vInsertOrder;
				GenerateInsertOrder(vPosition, vInsertOrder);

				//获取用户交易密码
				TrustTradeAccount account;
				memset(&account, 0, sizeof(account));
				if ( CDataCenter::Get() != NULL && 
					CDataCenter::Get()->GetTradeAccount(strAccount, account) )
				{
					CCryptoAPI::Decode3DES_ToString(account.szPwd);

					//启动交易软件下强平单
					InsertOrder(strAccount, account.szPwd, vOrderAction, vInsertOrder);
				}

				CForceCloseMsgQueue::Get()->AddForceCloseMsg(strAccount);
				Sleep(1000);
			}
		}
		else
			Sleep(m_IdleSleepTime);
	}
}

void CRiskMgr::UpdateAllRiskEvent()
{
	std::vector<std::string> accounts,Instruments;
	SAFE_GET_DATACENTER()->GetAccountList(accounts);
	SAFE_GET_DATACENTER()->GetInstrumentList(Instruments);
	for(size_t i=0;i<accounts.size();++i)
	{
		m_RiskMsgCalc->ActiveCalcFundLossRisk(accounts[i]);
		m_RiskMsgCalc->ActiveCalcGappedMarketRisk(accounts[i]);
		for (size_t j=0;j<Instruments.size();++j)
		{
			m_RiskMsgCalc->ActiveCalcMaxPositionRisk(accounts[i],Instruments[j]);
		}
		m_RiskMsgCalc->ActiveCalcMarginRatoRisk(accounts[i]);
		m_RiskMsgCalc->ActiveCalcProductMaxPositionRisk(accounts[i]);	
		m_RiskMsgCalc->ActiveCalcFundNetValueRisk(accounts[i]);	
	}
}

void CRiskMgr::GenerateOrderAction( const std::vector<SequencialOrderField>& vOrder, 
								   std::vector<CThostFtdcInputOrderActionField>& vOrderAction )
{
	for ( UINT i = 0; i < vOrder.size(); i++ )
	{
		SequencialOrderField order = vOrder[i];
		
		//涨跌停价限价单不撤
		//取涨跌停价
		double dUpperLimitPrice, dLowerLimitPrice;
		bool bFlag = false;
		if ( NULL != CDataCenter::Get() )
		{
			bFlag = CDataCenter::Get()->GetLimitPrice(order.InstrumentID, dUpperLimitPrice, dLowerLimitPrice);
			if ( bFlag && 
				(order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday ||
				order.CombOffsetFlag[0] == THOST_FTDC_OF_Close))
			{
				if ( order.Direction == THOST_FTDC_D_Sell )
				{
					if ( (order.LimitPrice - dLowerLimitPrice) <= 0.00001 &&
						(order.LimitPrice - dLowerLimitPrice) >= -0.00001 )
					{
						continue;
					}
				}
				else
				{
					if ( (order.LimitPrice - dUpperLimitPrice) <= 0.00001 &&
						(order.LimitPrice - dUpperLimitPrice) >= -0.00001 )
					{
						continue;
					}
				}
			}
		}

		CThostFtdcInputOrderActionField OrderAction;
		memset(&OrderAction, 0, sizeof(OrderAction));
		OrderAction.FrontID = order.FrontID;
		OrderAction.SessionID = order.SessionID;
		strcpy(OrderAction.OrderRef, order.OrderRef);
		strcpy(OrderAction.OrderSysID, order.OrderSysID);
		strcpy(OrderAction.ExchangeID, order.ExchangeID);
		strcpy(OrderAction.InstrumentID, order.InstrumentID);
		strcpy(OrderAction.BrokerID, order.BrokerID);
		strcpy(OrderAction.InvestorID, order.InvestorID);
		OrderAction.RequestID = i+1;
		OrderAction.ActionFlag = THOST_FTDC_AF_Delete;

		vOrderAction.push_back(OrderAction);
	}
}

void CRiskMgr::GenerateInsertOrder( const std::vector<SequencialPositionField>& vPosition, 
								   std::vector<CThostFtdcInputOrderField>& vInsertOrder )
{
	for ( UINT i = 0; i < vPosition.size(); i++ )
	{
		SequencialPositionField position = vPosition[i];
		if ( position.Position == 0 )
		{
			continue;
		}

		char Direction;
		if ( position.PosiDirection == THOST_FTDC_PD_Long )
		{
			Direction = THOST_FTDC_D_Sell;
		}
		else if ( position.PosiDirection == THOST_FTDC_PD_Short )
		{
			Direction = THOST_FTDC_D_Buy;
		}

		int nYestoDayOpenOrderVolumn = 0;
		int nTodayOpenOrderVolumn = 0;
		//获取挂单量
		SAFE_GET_DATACENTER()->GetOpenOrderVolumn(position.InvestorID,position.InstrumentID, 
			Direction, nTodayOpenOrderVolumn, nYestoDayOpenOrderVolumn);

		if ( position.TodayPosition != 0 )
		{
			int nCanCloseVolumn = position.TodayPosition - nTodayOpenOrderVolumn;
			if ( nCanCloseVolumn <= 0 )
			{
				continue;
			}

			//平今
			CThostFtdcInputOrderField inputOrder;
			memset(&inputOrder, 0, sizeof(inputOrder));
			if( !NewCloseOrder(position, inputOrder, Direction, nCanCloseVolumn, THOST_FTDC_OF_CloseToday))
			{
				continue;
			}
			else
			{
				while ( inputOrder.VolumeTotalOriginal > MAX_CAN_CLOSE_VOLUMN )
				{
					CThostFtdcInputOrderField order = inputOrder;
					order.VolumeTotalOriginal = MAX_CAN_CLOSE_VOLUMN;
					inputOrder.VolumeTotalOriginal -= MAX_CAN_CLOSE_VOLUMN;
					vInsertOrder.push_back(order);
				}
				
				vInsertOrder.push_back(inputOrder);
			}
		}

		if ( position.Position - position.TodayPosition != 0 )
		{
			int nCanCloseVolumn = position.Position - position.TodayPosition - nYestoDayOpenOrderVolumn;
			if ( nCanCloseVolumn <= 0 )
			{
				continue;
			}

			//平仓
			CThostFtdcInputOrderField inputOrder;
			memset(&inputOrder, 0, sizeof(inputOrder));
			if(!NewCloseOrder(position, inputOrder, Direction, nCanCloseVolumn, THOST_FTDC_OF_Close))
			{
				continue;
			}
			else
			{
				while ( inputOrder.VolumeTotalOriginal > MAX_CAN_CLOSE_VOLUMN )
				{
					CThostFtdcInputOrderField order = inputOrder;
					order.VolumeTotalOriginal = MAX_CAN_CLOSE_VOLUMN;
					inputOrder.VolumeTotalOriginal -= MAX_CAN_CLOSE_VOLUMN;
					vInsertOrder.push_back(order);
				}

				vInsertOrder.push_back(inputOrder);
			}
		}
	}
}

void CRiskMgr::Covent2CtpOrder( CShfeFtdcRiskForceCloseOrderField forceCloseOrder, 
							   CThostFtdcInputOrderField& ctpOrderField )
{
	memset(&ctpOrderField, 0, sizeof(ctpOrderField));
	strcpy(ctpOrderField.BrokerID, forceCloseOrder.BrokerID);
	strcpy(ctpOrderField.InvestorID, forceCloseOrder.InvestorID);
	strcpy(ctpOrderField.InstrumentID, forceCloseOrder.InstrumentID);
	strcpy(ctpOrderField.OrderRef, forceCloseOrder.OrderRef);
	strcpy(ctpOrderField.UserID, forceCloseOrder.UserID);
	ctpOrderField.OrderPriceType = forceCloseOrder.OrderPriceType;
	ctpOrderField.Direction = forceCloseOrder.Direction;
	strcpy(ctpOrderField.CombOffsetFlag, forceCloseOrder.CombOffsetFlag);
	strcpy(ctpOrderField.CombHedgeFlag, forceCloseOrder.CombHedgeFlag);
	ctpOrderField.LimitPrice = forceCloseOrder.LimitPrice;
	ctpOrderField.VolumeTotalOriginal = forceCloseOrder.VolumeTotalOriginal;
	ctpOrderField.TimeCondition = forceCloseOrder.TimeCondition;
	strcpy(ctpOrderField.GTDDate, forceCloseOrder.GTDDate);
	ctpOrderField.VolumeCondition = forceCloseOrder.VolumeCondition;
	ctpOrderField.MinVolume = forceCloseOrder.MinVolume;
	ctpOrderField.ContingentCondition = forceCloseOrder.ContingentCondition;
	ctpOrderField.StopPrice = forceCloseOrder.StopPrice;
	ctpOrderField.IsAutoSuspend = forceCloseOrder.IsAutoSuspend;
	strcpy(ctpOrderField.BusinessUnit, forceCloseOrder.BusinessUnit);
	ctpOrderField.RequestID = forceCloseOrder.RequestID;
	ctpOrderField.UserForceClose = forceCloseOrder.UserForceClose;
	ctpOrderField.ForceCloseReason = forceCloseOrder.ForceCloseReason;
}

bool CRiskMgr::NewCloseOrder( const SequencialPositionField &position, CThostFtdcInputOrderField &inputOrder, 
							 char Direction, int nCloseVolumn, char cCloseFlag )
{
	strcpy(inputOrder.BrokerID, position.BrokerID);
	strcpy(inputOrder.InvestorID, position.InvestorID);
	strcpy(inputOrder.InstrumentID, position.InstrumentID);
	strcpy(inputOrder.UserID, position.InvestorID);

	//买卖方向
	inputOrder.Direction = Direction;

	//价格类型、价格
	//郑商所支持市价下单
	std::string strExchangeID;
	bool bFlag = false;
	if ( NULL != CDataCenter::Get() )
	{
		bFlag = CDataCenter::Get()->GetExchangeID(position.InstrumentID, strExchangeID);
	}

	if ( !bFlag )
	{
		return false;
	}

	if( strExchangeID == "CZCE") 
	{
		//价格类型
		inputOrder.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		//价格
		inputOrder.LimitPrice = 0;
		//有效期类型
		inputOrder.TimeCondition = THOST_FTDC_TC_IOC;
	}
	else 
	{
		//取涨跌停价
		double dUpperLimitPrice, dLowerLimitPrice;
		bFlag = false;
		if ( NULL != CDataCenter::Get() )
		{
			bFlag = CDataCenter::Get()->GetLimitPrice(position.InstrumentID, dUpperLimitPrice, dLowerLimitPrice);
		}

		if ( !bFlag )
		{
			return false;
		}

		//价格类型
		inputOrder.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		//限价价格
		if ( inputOrder.Direction == THOST_FTDC_D_Sell )
		{
			inputOrder.LimitPrice = dLowerLimitPrice;
		}
		else
		{
			inputOrder.LimitPrice = dUpperLimitPrice;
		}

		//有效期类型
		inputOrder.TimeCondition = THOST_FTDC_TC_GFD;
	}

	inputOrder.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;

	inputOrder.VolumeTotalOriginal = nCloseVolumn;
	inputOrder.VolumeCondition = THOST_FTDC_VC_AV; 
	inputOrder.MinVolume = 1;
	inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;	//立即下单模式
	inputOrder.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	//开平标识
	inputOrder.CombOffsetFlag[0] = cCloseFlag;

	return true;
}
