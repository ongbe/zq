#pragma warning(disable : 4996)
#include "stdafx.h"
#include "SvrTradeExcute.h"
#include "easymutex.h"
#include "CommonPkg.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "CommonErrorCode.h"
#include "CommonDef.h"
#include "../SvrBrokerInfo/Interface_SvrBrokerInfo.h"
#include "../SvrUserOrg/Interface_SvrUserOrg.h"
//#include "../../FastTrader/inc/ISvr/UserApiDataType.h"
#include "../SvrOffer/Interface_SvrOffer.h"
#include "../SvrTradeData/Interface_SvrTradeData.h"

#include "SvrTradeExcuteImpl.h"
#include "ExeCmdDealFun.h"
#include "CommonMacro.h"
CSvrTradeExcuteImpl* CSvrTradeExcuteImpl::m_pObj=NULL;


CSvrTradeExcuteImpl::CSvrTradeExcuteImpl()
{
	g_mutex.lock();
	LoadDB();
	g_mutex.unlock();
}

CSvrTradeExcuteImpl::~CSvrTradeExcuteImpl()
{
}

CSvrTradeExcuteImpl& CSvrTradeExcuteImpl::getObj(void)
{
	g_mutex.lock();
	if(!m_pObj)
		m_pObj=new CSvrTradeExcuteImpl();
	g_mutex.unlock();
	return *m_pObj;
}

bool CSvrTradeExcuteImpl::LoadDB()
{

	return true;
}


//风控下强平单
CF_ERROR CSvrTradeExcuteImpl::RiskForceCloseOrder(PlatformStru_InputOrder & nInputOrder,
												  const InputOrderKey& lKey,
												  int nUserID,
												  const std::string & nsForceReason) 
{
	CF_ERROR nErrorCode = CF_ERROR_SUCCESS;
	do
	{
	
		PlatformStru_OrderInfo orderInfo;
		CInterface_SvrTradeData::getObj().GenerateOrderInfo(nInputOrder,orderInfo);
		orderInfo.FrontID = lKey.nFrontID;
		orderInfo.SessionID = lKey.nSessionID;
		memcpy(&orderInfo.OrderRef,lKey.szOrderRef,sizeof(TThostFtdcOrderRefType));

		//添加到交易数据管理模块缓存持久化
		CInterface_SvrTradeData::getObj().PushExecuteInOrder(orderInfo);	

		//下单到交易数据管理，执行冻结
		TRADE_EXT_LOG("Push Order to tradedata to freeze fund");
		orderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
		orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;	
		UpdateOrderDeal(orderInfo);
		strcpy(orderInfo.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_HAS_RISK_RECEVIED));
		CInterface_SvrTradeData::getObj().PushOrder(orderInfo,true);

		TRADE_EXT_LOG("OrderInsert:InvestID:%s InstrumentID:%s FrondID:%d SessionID:",
			orderInfo.InvestorID,
			orderInfo.InstrumentID,
			orderInfo.FrontID,
			orderInfo.SessionID);
		//判断该用户的准备好了吗？
		if(CInterface_SvrTradeData::getObj().GetUserInitStatus(orderInfo.InvestorID) != USER_STATUS_Inited)
		{	
			nErrorCode = CF_ERROR_TRADE_USER_TRADE_NOTREADY;
			TRADE_EXT_LOG("OrderInsert Fail:GetUserInitStatus Fail");	
			CancelOrderDeal(orderInfo,CF_ERROR_TRADE_USER_TRADE_NOTREADY,true);		
			break;
		}

		//判断是否对应的合约有费率,保证金率
		nErrorCode = CInterface_SvrTradeData::getObj().CheckUserInstrumentCommissionAndMaginExist(
			orderInfo.InvestorID,orderInfo.InstrumentID);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("OrderInsert Fail:CheckUserInstrumentCommissionAndMaginExist Fail");		
			CancelOrderDeal(orderInfo,nErrorCode,true);
			break;
		}

	


		//执行基础风控
		nErrorCode = CSvrTradeExcuteImpl::getObj().BaseFilterOrder(orderInfo.InvestorID, orderInfo, false);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("OrderInsert Fail:BaseFilterOrder Fail");		
			CancelOrderDeal(orderInfo,nErrorCode,true);
			break;
		}

	
	

		//下到报盘		
		nErrorCode = CInterface_SvrOffer::getObj().ReqOrderInsert(&nInputOrder,nUserID, lKey, 0);
		if ( nErrorCode != CF_ERROR_SUCCESS )
		{
			TRADE_EXT_LOG("OrderInsert Fail:ReqOrderInsert Fail");		
			CancelOrderDeal(orderInfo,nErrorCode,true);
			break;
		}


		break;
	}while(true);
	
	return nErrorCode;
}
//风控撤挂单
CF_ERROR CSvrTradeExcuteImpl::RiskForceActionOrder(PlatformStru_InputOrderAction & order,												  
												   int nUserID,
												   int nRequestID,
											 	   const std::string & nsActionReson)
{


	//CThostFtdcRspInfoField lRsp;
	OrderKey lKey;
	strcpy(lKey.Account,order.Thost.InvestorID);
	strcpy(lKey.InstrumentID,order.Thost.InstrumentID);
	lKey.FrontID = order.Thost.FrontID;
	lKey.SessionID = order.Thost.SessionID;
	strcpy(lKey.OrderRef,order.Thost.OrderRef);



	//判断该用户的准备好了吗？
	if(CInterface_SvrTradeData::getObj().GetUserInitStatus(order.Thost.InvestorID) != USER_STATUS_Inited)
	{			
		return CF_ERROR_TRADE_USER_TRADE_NOTREADY;
	}

	//判断该单是不是在审核状态
	PlatformStru_OrderInfo lOrderInfo;
	CF_ERROR nErrorCode = CInterface_SvrTradeData::getObj().GetUserOrderByKey(lKey.Account,lKey,lOrderInfo);
	if(nErrorCode != CF_ERROR_SUCCESS)
	{
		return nErrorCode;
	}

	//如果是审核状态，则内部撤单
	if(lOrderInfo.ExStatus == PlatformStru_OrderInfo::ExSta_approving)
	{
		nErrorCode = CF_ERROR_TRADE_ALREADY_CANCELED;

		TRADE_EXT_LOG("OrderInsert Fail:AfterFilterOrder Fail");
		CancelOrderDeal(lOrderInfo,nErrorCode,true);
		return CF_ERROR_SUCCESS;
	}
	else
	{
		InputOrderKey lInputKey;
		lInputKey.nFrontID = lKey.FrontID;
		lInputKey.nSessionID = lKey.SessionID;
		memcpy(&lInputKey.szOrderRef,lKey.OrderRef,sizeof(TThostFtdcOrderRefType));
		return CInterface_SvrOffer::getObj().ReqOrderAction(&order.Thost,nUserID,lInputKey,nRequestID);	

	}

	return CF_ERROR_SUCCESS;
}
//风控调用

bool CSvrTradeExcuteImpl::RiskControl(const std::string &nUserName, 
									  std::multimap<RiskWarningType, SRiskControl>& vecRiskControl)
{
	mMutexRisk.lock();
	do 
	{
		m_mapRiskLimited[nUserName] = vecRiskControl;
		break;;
	} while (true);
	mMutexRisk.unlock();
	return true;
}

//审核保单
CF_ERROR CSvrTradeExcuteImpl::SetVerifyOrder(OrderKey& orderKey, bool bPassVerify) 
{
	CF_ERROR nErrorCode = CF_ERROR_SUCCESS;
	do 
	{
		PlatformStru_OrderInfo lOrderInfo;
		nErrorCode = CInterface_SvrTradeData::getObj().GetUserOrderByKey(orderKey.Account,orderKey,lOrderInfo);
		if(nErrorCode != CF_ERROR_SUCCESS)
		{		
			break;
		}

		//如果该报单已审核
		if(lOrderInfo.OrderStatus != THOST_FTDC_OST_Unknown ||
			lOrderInfo.ExStatus != PlatformStru_OrderInfo::ExSta_approving)
		{		
			nErrorCode =  CF_ERROR_TRADE_HAS_VERIFYED;
			break;
		}


		//先撤单解冻资金
	/*	lOrderInfo.OrderStatus = THOST_FTDC_OST_Canceled;
		lOrderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
		CInterface_SvrTradeData::getObj().PushOrder(lOrderInfo,false);
		CancelOrderDeal(lOrderInfo,CF_ERROR_TRADE_VERIFY_FAILD,false);*/

		//审核不通过 直接返回
		if(bPassVerify == false)
		{
			//发送撤单原因
			TRADE_EXT_LOG("order Pass Verify fail");	
			CancelOrderDeal(lOrderInfo,CF_ERROR_TRADE_VERIFY_FAILD,true);
			break;
		}

		//下单到交易数据管理，执行冻结
	/*	TRADE_EXT_LOG("Push Order to tradedata to freeze fund");
		lOrderInfo.OrderStatus = THOST_FTDC_OST_Unknown;
		lOrderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;	
		CInterface_SvrTradeData::getObj().PushOrder(lOrderInfo,true);*/

		//执行风控和策略		
		nErrorCode = CSvrTradeExcuteImpl::getObj().AfterFilterOrder(lOrderInfo.InvestorID, lOrderInfo, false);
		if( nErrorCode != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("OrderInsert Fail:AfterFilterOrder Fail");	
			CancelOrderDeal(lOrderInfo,nErrorCode,true);
			break;
		}

		//执行基础风控		
		nErrorCode = CSvrTradeExcuteImpl::getObj().BaseFilterOrder(lOrderInfo.InvestorID, lOrderInfo, false);
		if(nErrorCode  != CF_ERROR_SUCCESS)
		{	
			TRADE_EXT_LOG("OrderInsert Fail:BaseFilterOrder Fail");		
			CancelOrderDeal(lOrderInfo,nErrorCode,true);
			break;;
		}

	



		//下到报盘
		InputOrderKey lKey;
		lKey.nFrontID = lOrderInfo.FrontID;
		lKey.nSessionID = lOrderInfo.SessionID;
		memcpy(&lKey.szOrderRef,lOrderInfo.OrderRef,sizeof(TThostFtdcOrderRefType));
		PlatformStru_InputOrder lInputOrder;
		CInterface_SvrTradeData::getObj().GenerateInputOrder(lOrderInfo,lInputOrder);
		int nUserID = -1;
		vector<UserInfo> lVecUeser;
		CInterface_SvrUserOrg::getObj().GetUserInfos(lVecUeser);
		for(unsigned int i = 0;i < lVecUeser.size();i++)
		{
			if(strcmp(lVecUeser[i].szAccount,lOrderInfo.InvestorID) == 0)
			{
				nUserID = lVecUeser[i].nUserID;
				break;
			}
		}
	
		nErrorCode = CInterface_SvrOffer::getObj().ReqOrderInsert(&lInputOrder,nUserID, lKey, 0);
		if (nErrorCode  != CF_ERROR_SUCCESS )
		{
			TRADE_EXT_LOG("OrderInsert Fail:ReqOrderInsert Fail");		
			CancelOrderDeal(lOrderInfo,nErrorCode,true);
			break;
		}

		break;
	} while (true);


	return nErrorCode;
}

void CSvrTradeExcuteImpl::SetOneStrategyOrderLimit(const std::string &nUserName, 
												   const std::string & nInstrumentID,
												   StrategyOrderLimit nCtrl)
{
	//CSafeLock lLock(mMutexStategy);
	mMutexStategy.lock();
	std::map<std::string, std::map<std::string, StrategyOrderLimit>>::iterator lIter;
	lIter = m_mapStrategyOrderLimit.find(nUserName);
	if(lIter == m_mapStrategyOrderLimit.end())
	{
		std::map<std::string, StrategyOrderLimit> lTempMap;
		lTempMap[nInstrumentID] = nCtrl;

		m_mapStrategyOrderLimit[nUserName] = lTempMap;
	}
	else
	{	
		m_mapStrategyOrderLimit[nUserName][nInstrumentID] = nCtrl;
	}
	mMutexStategy.unlock();

}



//预处理下单，判断用户启用，用户合约限制，用户是否需要手工审核 
CF_ERROR CSvrTradeExcuteImpl::PreFilterOrder( int nUserId,
										const PlatformStru_OrderInfo& order, 
										bool& bNeedVerify)
{
	//判断账户启用
	UserInfo lUser;
	memset(&lUser,0,sizeof(lUser));
	CInterface_SvrUserOrg::getObj().GetUserByID(nUserId,lUser);
	if(lUser.nStatus == 0)
		return CF_ERROR_USER_NOT_ENABLE;

	//判断账户下单限制
	std::vector<UserAndTradeAccountRelation> lvec;
	CInterface_SvrUserOrg::getObj().GetUserAndTradeAccountRelationByUserID(nUserId, lvec);
	bool bEnableOrder = false;
	for ( UINT i = 0; i < lvec.size(); i++ )
	{
		if ( lvec[i].nUserID == nUserId )
		{
			bNeedVerify = lvec[i].nNeedVerify > 0;
			bEnableOrder = lvec[i].nCanInputOrder > 0;
			break;
		}
	}
	if(!bEnableOrder)
		return CF_ERROR_TRADE_FORBIDDEN;


	////合约限制
	//if ( RiskInstumentLimited(order.InvestorID, order.InstrumentID))
	//{
	//	return CF_ERROR_TRADE_LIMITED;
	//}

	return CF_ERROR_SUCCESS;

}


//基础分控，
CF_ERROR CSvrTradeExcuteImpl::BaseFilterOrder(const std::string &nUserName,
						 const PlatformStru_OrderInfo& order, 
						 const bool nbNeedPushOrderInfoToTraderData)
{
	
	if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
	{
		
		return CInterface_SvrTradeData::getObj().CanOrderOpen(order);

	}
	else if(order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
	{
		//如果是平今，判断持仓里面的可平今量
		return CInterface_SvrTradeData::getObj().CanOrderColseToday(order);

	}
	else if(order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday ||
		order.CombOffsetFlag[0] == THOST_FTDC_OF_Close)
	{
		//如果是平仓，判断可平量
		//如果是平今，判断持仓里面的可平今量
		return CInterface_SvrTradeData::getObj().CanOrderColse(order);
			
	}
	return CF_ERROR_SUCCESS;
}


//分控和交易策略
CF_ERROR CSvrTradeExcuteImpl::AfterFilterOrder(const std::string &nUserName,
						  const PlatformStru_OrderInfo& order, 
						  const bool nbNeedPushOrderInfoToTraderData)
{
	//order.nOrderStatus = InputOrder_Verify_End;
	//风险控制
	CF_ERROR nError = CF_ERROR_SUCCESS;
	if ( RiskIndLimited(nUserName, order,nError))
	{
		return nError;
		//return CF_ERROR_TRADE_RISK_LIMITED;
	}

	//开仓交易辅助过滤
	if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open && 
		StrategyLimited(nUserName, order.InstrumentID, order.Direction))
	{
		return CF_ERROR_TRADE_STRATEGY_LIMITED;
	}

	return CF_ERROR_SUCCESS;
}
////
//int CSvrTradeExcuteImpl::InputOrder( int nUserID, 
//									const InputOrderKey& key, 
//									const PlatformStru_InputOrder& order, 
//									bool& bNeedVerify )
//{
//	//存储下单请求
//	//order.nOrderStatus = InputOrder_Receive;
//	UserInfo lUser;
//	CInterface_SvrUserOrg::getObj().g
//
//	
//	//判断是否允许下单
//	if ( !EnableOrder(nTraderID, nTradeAccountID))
//	{
//		return CF_ERROR_TRADE_FORBIDDEN;
//	}
//
//	//if ( RiskInstumentLimited(nTraderID, order.InstrumentID))
//	//{
//	//	return CF_ERROR_TRADE_LIMITED;
//	//}
//
//	//if ( NeedVerify(nTraderID, nTradeAccountID))
//	//{
//	//	//需要发送审核请求
//	//	bNeedVerify = true;
//	//	
//	//	return CF_ERROR_SUCCESS;
//	//}
//
//	////order.nOrderStatus = InputOrder_Verify_End;
//	//if ( RiskIndLimited(nTraderID, order))
//	//{
//	//	return CF_ERROR_TRADE_RISK_LIMITED;
//	//}
//
//	//if ( StrategyLimited(nTraderID, order.InstrumentID, order.Direction))
//	//{
//	//	return CF_ERROR_TRADE_STRATEGY_LIMITED;
//	//}
//
//	//验证资金
//	//冻结资金
//	//调用报盘下单接口
//	int nErrorCode = CInterface_SvrOffer::getObj().ReqOrderInsert(&order,nUserID, key, 0);
//	if ( nErrorCode != CF_ERROR_SUCCESS )
//	{
//		return nErrorCode;
//	}
//
//	//m_mapInputOrder[key] = order;
//	return CF_ERROR_SUCCESS;
//}

bool CSvrTradeExcuteImpl::EnableOrder( int nTraderID, int nTradeAccountID )
{
	//UserAndTradeAccountRelation
	std::vector<UserAndTradeAccountRelation> vec;
	CInterface_SvrUserOrg::getObj().GetUserAndTradeAccountRelationByUserID(nTraderID, vec);
	bool bEnableOrder = false;
	for ( UINT i = 0; i < vec.size(); i++ )
	{
		if ( vec[i].nTradeAccountID == nTradeAccountID )
		{
			bEnableOrder = vec[i].nCanInputOrder > 0;
		}
	}

	return bEnableOrder;
}


bool CSvrTradeExcuteImpl::NeedVerify( int nTraderID, int nTradeAccountID )
{
	//UserAndTradeAccountRelation
	std::vector<UserAndTradeAccountRelation> vec;
	CInterface_SvrUserOrg::getObj().GetUserAndTradeAccountRelationByUserID(nTraderID, vec);
	bool bNeedVerify = false;
	for ( UINT i = 0; i < vec.size(); i++ )
	{
		if ( vec[i].nTradeAccountID == nTradeAccountID )
		{
			bNeedVerify = vec[i].nNeedVerify > 0;
		}
	}

	return bNeedVerify;
}

bool CSvrTradeExcuteImpl::RiskIndLimited( const std::string &nUserName,
										 const PlatformStru_OrderInfo& order ,
										 CF_ERROR & nErrorCode)
{
	mMutexRisk.lock();
	bool lbRet = false;
	do 
	{
		std::map<std::string,std::multimap<RiskWarningType, SRiskControl>>::iterator it_trader = m_mapRiskLimited.find(nUserName);
		if ( it_trader == m_mapRiskLimited.end() )
		{
			break;
		}
		else
		{
			//判断所有合约下单限制
			std::multimap<RiskWarningType, SRiskControl>::iterator it_inst = it_trader->second.find(WarningType_ForbidOrder);
			if ( it_inst != it_trader->second.end() )
			{
				nErrorCode = CF_ERROR_RISK_ALL_INSTRUMENT_ORDER;
				lbRet = true;						
			}
			else
			{
				//如果所有合约下单限制没有，则判断单合约下单限制
				int nCount = it_trader->second.count(WarningType_ForbidOrder_Single);
				it_inst = it_trader->second.find(WarningType_ForbidOrder_Single);
				for(int i =0; i<nCount; i++)
				{					
					if(it_inst != it_trader->second.end() && 
						it_inst->second.setInstrument.find(order.InstrumentID) != it_inst->second.setInstrument.end())
					{
						nErrorCode = CF_ERROR_RISK_SINGLE_INSTRUMENT_ORDER;
						lbRet = true;		
						break;
					}					
					it_inst++;
				}

				if(!lbRet)
				{
					//如果单合约下单限制没有，则判断是否是开仓，如果是开仓则判断是否限制开仓
					if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
					{
						//判断所有合约开仓限制
						it_inst = it_trader->second.find(WarningType_ForbidOpen);
						if(it_inst != it_trader->second.end() )
						{
							nErrorCode = CF_ERROR_RISK_ALL_INSTRUMENT_OPEN;
							lbRet = true;						
						}
						else
						{
							//如果所有合约的开仓限制没有，则判断单合约的开仓限制
							int nCount = it_trader->second.count(WarningType_ForbidOpen_Single);
							it_inst = it_trader->second.find(WarningType_ForbidOpen_Single);
							for(int i =0; i<nCount; i++)
							{					
								if(it_inst != it_trader->second.end() && 
									it_inst->second.setInstrument.find(order.InstrumentID) != it_inst->second.setInstrument.end())
								{
									nErrorCode = CF_ERROR_RISK_SINGLE_INSTRUMENT_OPEN;
									lbRet = true;		
									break;
								}					
								it_inst++;
							}							
						}
					}				


				}
		/*		if(it_inst != it_trader->second.end() && 
					it_inst->second.setInstrument.find(order.InstrumentID) != it_inst->second.setInstrument.end())
				{
					nErrorCode = CF_ERROR_RISK_SINGLE_INSTRUMENT_ORDER;
					lbRet = true;					
				}
				else
				{
					//如果单合约下单限制没有，则判断是否是开仓，如果是开仓则判断是否限制开仓
					if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
					{
						//判断所有合约开仓限制
						it_inst = it_trader->second.find(WarningType_ForbidOpen);
						if(it_inst != it_trader->second.end() )
						{
							nErrorCode = CF_ERROR_RISK_ALL_INSTRUMENT_OPEN;
							lbRet = true;						
						}
						else
						{
							//如果所有合约的开仓限制没有，则判断单合约的开仓限制
							it_inst = it_trader->second.find(WarningType_ForbidOpen_Single);
							if(it_inst != it_trader->second.end() && 
								it_inst->second.setInstrument.find(order.InstrumentID) != it_inst->second.setInstrument.end())
							{
								nErrorCode = CF_ERROR_RISK_SINGLE_INSTRUMENT_OPEN;
								lbRet = true;							
							}
						}
					}				
				}*/
			}
		}
		break;
	} while (true);
	mMutexRisk.unlock();
	return lbRet;
}

bool CSvrTradeExcuteImpl::StrategyLimited( const std::string &nUserName, 
										  std::string strInstrumentID,
										  char Direction )
{
	//CSafeLock lLock(mMutexStategy);
	mMutexStategy.lock();
	bool lbRet = false;
	do 
	{
		std::map<std::string, std::map<std::string, StrategyOrderLimit>>::iterator it_trader = m_mapStrategyOrderLimit.find(nUserName);
		if ( it_trader == m_mapStrategyOrderLimit.end() )
		{
			break;
		}
		else
		{
			std::map<std::string, StrategyOrderLimit>::iterator it_inst = it_trader->second.find(strInstrumentID);
			if ( it_inst == it_trader->second.end() )
			{
				break;
			}
			else
			{


				if ( it_inst->second == STRATEGY_FORBID_BOTH_SIDE || 
					(it_inst->second == STRATEGY_FORBID_BUY && Direction == THOST_FTDC_D_Buy) ||
					(it_inst->second == STRATEGY_FORBID_SELL && Direction == THOST_FTDC_D_Sell))
				{
					lbRet=true;
					break;
				}
				else
				{
					break;
				}
			}
		}
		break;
	} while (true);
	mMutexStategy.unlock();
	return lbRet;
}



int CSvrTradeExcuteImpl::CancelOrder( InputOrderKey key )
{
	/*std::map<InputOrderKey, PlatformStru_InputOrder>::iterator it = m_mapInputOrder.find(key);
	if ( it == m_mapInputOrder.end() )
	{
		return CF_ERROR_TRADE_ALREADY_CANCELED;
	}*/

	//if ( it->second.nOrderStatus < InputOrder_CTP_Received)
	//{
	//	m_mapInputOrder.erase(it);
	//	return CF_ERROR_SUCCESS;
	//}
	//else
	//{
	//	//生成撤单结构，向ctp撤单
	//	//ReqOrderAction
	//	m_mapInputOrder.erase(it);
	//	return CF_ERROR_SUCCESS;
	//}
	return CF_ERROR_SUCCESS;

}

int CSvrTradeExcuteImpl::CancelOrderReturn( InputOrderKey key )
{
	return CF_ERROR_SUCCESS;
}
//
//void CSvrTradeExcuteImpl::AddVerifyingOrder( InputOrderKey key, PlatformStru_InputOrder order )
//{
////	order.nOrderStatus = InputOrder_Verifying;
//	m_mapInputOrder[key] = order;
//}
//
//bool CSvrTradeExcuteImpl::GetInputOrder( InputOrderKey key, PlatformStru_InputOrder& order )
//{
//	std::map<InputOrderKey, PlatformStru_InputOrder>::iterator it = m_mapInputOrder.find(key);
//	if ( it == m_mapInputOrder.end() )
//	{
//		return false;
//	}
//	else
//	{
//		order = it->second;
//		return true;
//	}
//}
void CSvrTradeExcuteImpl::CancelOrderDeal(PlatformStru_OrderInfo & orderInfo,CF_ERROR nErrorCode,bool nbRiskEvent)
{
	orderInfo.OrderStatus = THOST_FTDC_OST_Canceled;
	orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	strcpy(orderInfo.StatusMsg,FormatErrorCode(nErrorCode));
	orderInfo.troubleMoney = util::GetDoubleInvalidValue();	
	orderInfo.freezeMargin = util::GetDoubleInvalidValue();
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(orderInfo.CancelTime, sizeof(orderInfo.CancelTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strncpy(orderInfo.UpdateTime, orderInfo.CancelTime, sizeof(orderInfo.UpdateTime));
	CInterface_SvrTradeData::getObj().PushOrder(orderInfo,nbRiskEvent);
}

void CSvrTradeExcuteImpl::UpdateOrderDeal(PlatformStru_OrderInfo & orderInfo)
{
	orderInfo.troubleMoney = util::GetDoubleInvalidValue();	
	orderInfo.freezeMargin = util::GetDoubleInvalidValue();
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(orderInfo.CancelTime, sizeof(orderInfo.CancelTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
}