#include "stdafx.h"
#include "RiskMgr.h"
#include "ToolThread.h"
#include "DataCenter.h"

extern bool g_bSendToExcute;//通过网络发送给交易执行命令
extern  int g_hSocket;      //主服务器套接字

#define MAX_CAN_CLOSE_VOLUMN	100
CRiskMgr* CRiskMgr::m_pRiskMgr=NULL;
CRiskMgr::CRiskMgr(void):m_IdleSleepTime(100)
{
	
	m_RiskMgrThread_AutoForceClose = new CToolThread(RiskThreadProcForceClose, this);

	//如果不是日内唯一，
	time_t lTime;
	struct tm * lTimeinfo; 
	time(&lTime);
	lTimeinfo = localtime(&lTime);
	char lcBuf[256] = {0};
	sprintf_s(lcBuf,"%04d-%02d-%02d",1900+lTimeinfo->tm_year,1+lTimeinfo->tm_mon,lTimeinfo->tm_mday);
	m_strTodayString = lcBuf;	
}
CRiskMgr::~CRiskMgr(void)
{	
	SAFE_DELETE(m_RiskMgrThread_AutoForceClose);
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
DWORD WINAPI CRiskMgr::RiskThreadProcForceClose (PVOID pParam)
{
	CRiskMgr* pMgr=(CRiskMgr*)pParam;
	pMgr->doProcessBusinessDataForceClose();
	return 0;
}
void CRiskMgr::GetInputOrderKey(int nTradeInvestorID, InputOrderKey& lKey)
{
	m_Mutex_Investor2OrderRef.write_lock();
	char szInvestor[256];
	sprintf(szInvestor, "%d", nTradeInvestorID);

	std::map<int, int>::iterator it = m_mapInvestor2OrderRef.find(nTradeInvestorID);
	if(it != m_mapInvestor2OrderRef.end())
	{
		int nOrderRef	= it->second;

		lKey.nFrontID	= nTradeInvestorID;
		lKey.nSessionID = 1;
		sprintf(lKey.szOrderRef, "%d", ++nOrderRef);

		m_mapInvestor2OrderRef[nTradeInvestorID] = nOrderRef;

		::WritePrivateProfileString(m_strTodayString.c_str(),szInvestor,lKey.szOrderRef,"orderref.txt"); 	
	}
	else
	{
		
		int nOrderRef = GetPrivateProfileInt(m_strTodayString.c_str(),szInvestor,1,"orderref.txt");

		lKey.nFrontID	= nTradeInvestorID;
		lKey.nSessionID = 1;
		sprintf(lKey.szOrderRef, "%d", nOrderRef);

		m_mapInvestor2OrderRef[nTradeInvestorID] = nOrderRef;
	}
	m_Mutex_Investor2OrderRef.write_unlock();
}
void CRiskMgr::doProcessBusinessDataForceClose()
{
	SForceClose sForceClose;
	while (!m_RiskMgrThread_AutoForceClose->IsNeedExit())//frontid userid；sessionid 1； orderref：从1开始自增长
	{		
		if(CForceCloseMsgQueue::Get()->GetForceCloseMsg(sForceClose))
		{
			if(sForceClose.bIsSigleHoldTime)
			{
				CloseOnePositonDetail(sForceClose);
			}
			else if(sForceClose.strInstrument.length() ==0)
			{//全部平仓
				CloseAllPositon(sForceClose);
			}	
			else
			{//单合约强平
				CloseOneInstrument(sForceClose);
			}
		}
		else
			Sleep(m_IdleSleepTime);
	}
}
void CRiskMgr::CloseAllPositon(SForceClose& sForceClose)
{
	//获取该账号的所有挂单
	std::vector<PlatformStru_OrderInfo> vOrder;
	CInterface_SvrTradeData::getObj().GetUserWaitOrders(sForceClose.strInvestID, vOrder);

	UserInfo userInfo;
	CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(sForceClose.strInvestID, userInfo);	
	int nTradeInvestorID = userInfo.nUserID;

	//生成撤单结构
	std::vector<PlatformStru_InputOrderAction> vOrderAction;
	GenerateOrderAction(vOrder, vOrderAction);
	for(int i = 0; i< (int)vOrderAction.size();i++)
	{
		PlatformStru_InputOrderAction& order = vOrderAction[i];
		std::string  nsForceReason;
		char buf[256];
		std::string strName;
		SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)sForceClose.nRiskIndicatorID , strName);
		sprintf(buf,"自动撤单原因：触发风险类型:%s; 风险等级:%d，需要先撤单后市价强平",  strName.c_str(), sForceClose.nRiskLevelID);
		nsForceReason = buf;
		//if(!g_bSendToExcute)
		//	CInterface_SvrTradeExcute::getObj().RiskForceActionOrder(order, userInfo.nUserID, 0, nsForceReason);	
		if(g_bSendToExcute)
		{
			int nSocket =0;
			InterlockedExchange((volatile long*)(&nSocket),g_hSocket);
			if(nSocket)
			{
				SRiskInputOrderAction sMClose;
				sMClose.nInputOrderAction = order;
				strcpy(sMClose.nsActionReson, buf);
				sMClose.nRequestID = 0;
				strcpy(sMClose.szRiskerName, "");
				CInterface_SvrTcp::getObj().SendPkgData(nSocket,Cmd_RM_ToExecute_OrderAction_Req,
					&sMClose, sizeof(SRiskInputOrderAction),0,0,CF_ERROR_SUCCESS);	
			}
			else
			{//输出socket无效
				RISK_LOGINFO("主从服务断开，自动强平失败\n");
			}
		}

		int nErrorCode = CF_ERROR_SUCCESS;
		CInterface_SvrDBOpr::getObj().SaveForceCloseOrderAction(order, true, "", nErrorCode);
		if(nErrorCode != CF_ERROR_SUCCESS)
		{
			CInterface_SvrDBOpr::getObj().SaveForceCloseOrderAction2File(order, true, "");
		}
	}	

	std::vector<PlatformStru_Position> vPosition;
	CInterface_SvrTradeData::getObj().GetUserPositions(sForceClose.strInvestID, "",	vPosition);
	if(vPosition.size() ==0)
	{
		CForceCloseMsgQueue::Get()->DeleteForceCloseCount(sForceClose);
		return;//已平完
	}
	else
	{
		//生成平仓下单结构
		std::vector<PlatformStru_InputOrder> vInsertOrder;
		GenerateInsertOrder(vPosition, vInsertOrder);

		for(int i = 0; i< (int)vInsertOrder.size();i++)
		{
			PlatformStru_InputOrder& order = vInsertOrder[i];
			//用UserForceClose = 1以及ForceCloseReason = THOST_FTDC_FCC_ForceClose代表强平单，不可撤销
			order.UserForceClose = 1;
			order.ForceCloseReason = THOST_FTDC_FCC_ForceClose;

			InputOrderKey lKey;
			GetInputOrderKey(nTradeInvestorID,  lKey);
			strcpy(order.OrderRef, lKey.szOrderRef);						

			std::string strReason;
			std::string strName;
			SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)sForceClose.nRiskIndicatorID , strName);
			char buf[256];
			sprintf(buf,"自动强平原因：触发风险类型:%s; 风险等级:%d",  strName.c_str(), sForceClose.nRiskLevelID);
			strReason = buf;

			//if(!g_bSendToExcute)
			//	CInterface_SvrTradeExcute::getObj().RiskForceCloseOrder(order, lKey, userInfo.nUserID, strReason);	
			if(g_bSendToExcute)
			{
				SManualForceClose sMClose;
				sMClose.nInputOrder = order;
				sMClose.nFrontID = lKey.nFrontID;
				sMClose.nSessionID = lKey.nSessionID;
				strcpy(sMClose.szOrderRef, lKey.szOrderRef);
				strcpy(sMClose.szReason, buf);
				strcpy(sMClose.szRiskerName, "");
				int nSocket =0;
				InterlockedExchange((volatile long*)(&nSocket),g_hSocket);
				if(nSocket)
					CInterface_SvrTcp::getObj().SendPkgData(nSocket,Cmd_RM_ToExecute_ForceCloseOrderInsert_Req,
						&sMClose, sizeof(SManualForceClose),0,0,CF_ERROR_SUCCESS,0,0,0);	
				else
				{
					RISK_LOGINFO("主从服务断开，自动强平失败\n");
				}

			}

			int nErrorCode = CF_ERROR_SUCCESS;
			strcpy(order.InvestorID , sForceClose.strInvestID.c_str());//改变回交易员
			order.OrderClass = '0';//数据库里面不显示赋值char类型，存不进去
			CInterface_SvrDBOpr::getObj().SaveForceCloseOrder(order, lKey, true, "", nErrorCode);
			if(nErrorCode != CF_ERROR_SUCCESS)
			{
				CInterface_SvrDBOpr::getObj().SaveForceCloseOrder2File(order, lKey, true, "");
			}
		}						
		if(CForceCloseMsgQueue::Get()->SetForceCloseCount(sForceClose))
			CForceCloseMsgQueue::Get()->AddForceCloseMsg(sForceClose.nRiskEventID, sForceClose.strInvestID, "", NULL, sForceClose.nRiskIndicatorID, sForceClose.nRiskLevelID);
		else
		{
			CForceCloseMsgQueue::Get()->DeleteForceCloseCount(sForceClose);
			SendForceCloseMessage(sForceClose);
		}
		Sleep(1000);
	}
}
void CRiskMgr::CloseOneInstrument(SForceClose& sForceClose)
{
	//获取该账号的所有挂单
	std::vector<PlatformStru_OrderInfo> vOrder;
	CInterface_SvrTradeData::getObj().GetUserWaitOrders(sForceClose.strInvestID, vOrder);

	UserInfo userInfo;
	CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(sForceClose.strInvestID, userInfo);	
	int nTradeInvestorID = userInfo.nUserID;

	//生成撤单结构
	std::vector<PlatformStru_InputOrderAction> vOrderAction;
	GenerateOrderAction(vOrder, vOrderAction);
	for(int i = 0; i< (int)vOrderAction.size();i++)
	{
		PlatformStru_InputOrderAction& order = vOrderAction[i];
		if(strcmp(order.Thost.InstrumentID,sForceClose.strInstrument.c_str()) != 0)
			continue;

		std::string  nsForceReason;
		char buf[256];
		std::string strName;
		SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)sForceClose.nRiskIndicatorID , strName);
		sprintf(buf,"自动撤单原因：触发风险类型:%s; 风险等级:%d，需要先撤单后市价强平",  strName.c_str(), sForceClose.nRiskLevelID);
		nsForceReason = buf;		
		//if(!g_bSendToExcute)
		//	CInterface_SvrTradeExcute::getObj().RiskForceActionOrder(order, userInfo.nUserID, 0, nsForceReason);	
		if(g_bSendToExcute)
		{
			int nSocket =0;
			InterlockedExchange((volatile long*)(&nSocket),g_hSocket);
			if(nSocket)
			{
				SRiskInputOrderAction sMClose;
				sMClose.nInputOrderAction = order;
				strcpy(sMClose.nsActionReson, buf);
				sMClose.nRequestID = 0;
				strcpy(sMClose.szRiskerName, "");
				CInterface_SvrTcp::getObj().SendPkgData(nSocket,Cmd_RM_ToExecute_OrderAction_Req,
					&sMClose, sizeof(SRiskInputOrderAction),0,0,CF_ERROR_SUCCESS);	
			}
			else
			{//输出socket无效
				RISK_LOGINFO("主从服务断开，自动强平失败\n");
			}
		}

		int nErrorCode = CF_ERROR_SUCCESS;
		CInterface_SvrDBOpr::getObj().SaveForceCloseOrderAction(order, true, "", nErrorCode);
		if(nErrorCode != CF_ERROR_SUCCESS)
		{
			CInterface_SvrDBOpr::getObj().SaveForceCloseOrderAction2File(order, true, "");
		}
	}	

	std::vector<PlatformStru_Position> vPosition;
	CInterface_SvrTradeData::getObj().GetUserPositions(sForceClose.strInvestID, sForceClose.strInstrument,	vPosition);
	if(vPosition.size() ==0)
	{
		CForceCloseMsgQueue::Get()->DeleteForceCloseCount(sForceClose);
		return;//已平完
	}
	else
	{
		//生成平仓下单结构
		std::vector<PlatformStru_InputOrder> vInsertOrder;
		GenerateInsertOrder(vPosition, vInsertOrder);
		UserInfo userInfo;
		CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(sForceClose.strInvestID, userInfo);	
		int nTradeInvestorID = userInfo.nUserID;					
		for(int i = 0; i< (int)vInsertOrder.size();i++)
		{
			PlatformStru_InputOrder& order = vInsertOrder[i];
			//用UserForceClose = 1以及ForceCloseReason = THOST_FTDC_FCC_ForceClose代表强平单，不可撤销
			order.UserForceClose = 1;
			order.ForceCloseReason = THOST_FTDC_FCC_ForceClose;

			InputOrderKey lKey;
			GetInputOrderKey(nTradeInvestorID,  lKey);

			std::string strReason;
			std::string strName;
			SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)sForceClose.nRiskIndicatorID , strName);
			char buf[256];
			sprintf(buf,"自动强平原因：\n触发风险类型:%s;\n存在风险的合约:%s;\n 风险等级:%d",  strName.c_str(), sForceClose.strInstrument.c_str(), sForceClose.nRiskLevelID);
			strReason = buf;

		//	if(!g_bSendToExcute)
		//		CInterface_SvrTradeExcute::getObj().RiskForceCloseOrder(order, lKey, userInfo.nUserID, strReason);	
			if(g_bSendToExcute)
			{
				SManualForceClose sMClose;
				sMClose.nInputOrder = order;
				sMClose.nFrontID = lKey.nFrontID;
				sMClose.nSessionID = lKey.nSessionID;
				strcpy(sMClose.szOrderRef, lKey.szOrderRef);
				strcpy(sMClose.szReason, buf);
				strcpy(sMClose.szRiskerName, "");
				
				int nSocket =0;
				InterlockedExchange((volatile long*)(&nSocket),g_hSocket);
				if(nSocket)
					CInterface_SvrTcp::getObj().SendPkgData(nSocket,Cmd_RM_ToExecute_ForceCloseOrderInsert_Req,
						&sMClose, sizeof(SManualForceClose),0,0,CF_ERROR_SUCCESS,0,0,0);	
				else
				{
					RISK_LOGINFO("主从服务断开，手动强平失败\n");
				}

			}

			int nErrorCode = CF_ERROR_SUCCESS;
			strcpy(order.InvestorID , sForceClose.strInvestID.c_str());//改变回交易员
			order.OrderClass = '0';//数据库里面不显示赋值char类型，存不进去
			CInterface_SvrDBOpr::getObj().SaveForceCloseOrder(order, lKey, true, "", nErrorCode);
			if(nErrorCode != CF_ERROR_SUCCESS)
			{
				CInterface_SvrDBOpr::getObj().SaveForceCloseOrder2File(order, lKey, true, "");
			}
		}						
		if(CForceCloseMsgQueue::Get()->SetForceCloseCount(sForceClose))
			CForceCloseMsgQueue::Get()->AddForceCloseMsg(sForceClose.nRiskEventID, sForceClose.strInvestID, sForceClose.strInstrument, NULL, sForceClose.nRiskIndicatorID, sForceClose.nRiskLevelID);
		else
		{
			SendForceCloseMessage(sForceClose);
		}
		Sleep(1000);
	}
}
void CRiskMgr::CloseOnePositonDetail(SForceClose& sForceClose)
{//目前的逻辑是撤改合约所有的单，然后强平该持仓明细
	//获取该账号的所有挂单
	//获取该账号的所有挂单

	PositionDetailKey  nKey = sForceClose.PostionDetail;
	PlatformStru_PositionDetail outData;
	int nError =  CInterface_SvrTradeData::getObj().GetUserPositionDetail(sForceClose.strInvestID,	nKey, outData);
	if(CF_ERROR_SUCCESS != nError || (outData.Volume == 0))
	{
		CForceCloseMsgQueue::Get()->DeleteForceCloseCount(sForceClose);
		return;
	}
	int nVolume = outData.Volume;

	UserInfo userInfo;
	CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(sForceClose.strInvestID, userInfo);	
	int nTradeInvestorID = userInfo.nUserID;

	std::vector<PlatformStru_OrderInfo> vOrder;
	CInterface_SvrTradeData::getObj().GetUserWaitOrders(sForceClose.strInvestID, vOrder);

	//生成撤单结构
	std::vector<PlatformStru_InputOrderAction> vOrderAction;
	GenerateOrderAction(vOrder, vOrderAction);
	for(int i = 0; i< (int)vOrderAction.size();i++)
	{
		PlatformStru_InputOrderAction& order = vOrderAction[i];
		if(strcmp(order.Thost.InstrumentID, sForceClose.PostionDetail.InstrumentID) != 0)
			continue;

		std::string  nsForceReason;
		char buf[256];
		std::string strName;
		SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)sForceClose.nRiskIndicatorID , strName);
		sprintf(buf,"自动撤单原因：触发风险类型:%s; 风险等级:%d，需要先撤单后市价强平",  strName.c_str(), sForceClose.nRiskLevelID);
		nsForceReason = buf;
		//if(!g_bSendToExcute)
		//	CInterface_SvrTradeExcute::getObj().RiskForceActionOrder(order, userInfo.nUserID, 0, nsForceReason);	
		if(g_bSendToExcute)
		{
			int nSocket =0;
			InterlockedExchange((volatile long*)(&nSocket),g_hSocket);
			if(nSocket)
			{
				SRiskInputOrderAction sMClose;
				sMClose.nInputOrderAction = order;
				strcpy(sMClose.nsActionReson, buf);
				sMClose.nRequestID = 0;
				strcpy(sMClose.szRiskerName, "");
				CInterface_SvrTcp::getObj().SendPkgData(nSocket,Cmd_RM_ToExecute_OrderAction_Req,
					&sMClose, sizeof(SRiskInputOrderAction),0,0,CF_ERROR_SUCCESS);	
			}
			else
			{//输出socket无效
				RISK_LOGINFO("主从服务断开，撤单失败\n");
			}
		}

		int nErrorCode = CF_ERROR_SUCCESS;
		CInterface_SvrDBOpr::getObj().SaveForceCloseOrderAction(order, true, "", nErrorCode);
		if(nErrorCode != CF_ERROR_SUCCESS)
		{
			CInterface_SvrDBOpr::getObj().SaveForceCloseOrderAction2File(order, true, "");
		}
	}	

	std::vector<PlatformStru_Position> vPosition;
	CInterface_SvrTradeData::getObj().GetUserPositions(sForceClose.strInvestID, sForceClose.PostionDetail.InstrumentID,	vPosition);
	if(vPosition.size() ==0)
	{
		CForceCloseMsgQueue::Get()->DeleteForceCloseCount(sForceClose);
		return;//已平完
	}
	else
	{
		//生成平仓下单结构
		std::vector<PlatformStru_InputOrder> vInsertOrder;
	//	GenerateInsertOrder(vPosition, vInsertOrder);
		GenerateInsertOrder_PositionDetail(outData, vInsertOrder);
		for(int i = 0; i< (int)vInsertOrder.size();i++)
		{
			PlatformStru_InputOrder& order = vInsertOrder[i];			
			//用UserForceClose = 1以及ForceCloseReason = THOST_FTDC_FCC_ForceClose代表强平单，不可撤销
			order.UserForceClose = 1;
			order.ForceCloseReason = THOST_FTDC_FCC_ForceClose;

			InputOrderKey lKey;
			GetInputOrderKey(nTradeInvestorID,  lKey);
			strcpy(order.OrderRef, lKey.szOrderRef);						

			std::string strReason;
			std::string strName;
			SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)sForceClose.nRiskIndicatorID , strName);
			char buf[256];
			sprintf(buf,"自动强平原因：触发风险类型:%s; 风险等级:%d",  strName.c_str(), sForceClose.nRiskLevelID);
			strReason = buf;

			//if(!g_bSendToExcute)
			//	CInterface_SvrTradeExcute::getObj().RiskForceCloseOrder(order, lKey, userInfo.nUserID, strReason);	
			if(g_bSendToExcute)
			{
				SManualForceClose sMClose;
				sMClose.nInputOrder = order;
				sMClose.nFrontID = lKey.nFrontID;
				sMClose.nSessionID = lKey.nSessionID;
				strcpy(sMClose.szOrderRef, lKey.szOrderRef);
				strcpy(sMClose.szReason, buf);
				strcpy(sMClose.szRiskerName, "");
				CInterface_SvrTcp::getObj().SendPkgData(g_hSocket,Cmd_RM_ToExecute_ForceCloseOrderInsert_Req,
					&sMClose, sizeof(SManualForceClose),0,0,CF_ERROR_SUCCESS,0,0,0);	

			}	

			int nErrorCode = CF_ERROR_SUCCESS;
			strcpy(order.InvestorID , sForceClose.strInvestID.c_str());//改变回交易员
			order.OrderClass = '0';//数据库里面不显示赋值char类型，存不进去
			CInterface_SvrDBOpr::getObj().SaveForceCloseOrder(order, lKey, true, "", nErrorCode);
			if(nErrorCode != CF_ERROR_SUCCESS)
			{
				CInterface_SvrDBOpr::getObj().SaveForceCloseOrder2File(order, lKey, true, "");
			}
		}						
		if(CForceCloseMsgQueue::Get()->SetForceCloseCount(sForceClose))
			CForceCloseMsgQueue::Get()->AddForceCloseMsg(sForceClose.nRiskEventID, sForceClose.strInvestID, "", &nKey, sForceClose.nRiskIndicatorID, sForceClose.nRiskLevelID);
		else
		{
			CForceCloseMsgQueue::Get()->DeleteForceCloseCount(sForceClose);
			SendForceCloseMessage(sForceClose);
		}
		Sleep(1000);
	}
}
void CRiskMgr::SendForceCloseMessage(SForceClose& sForceClose)
{
	int nOrgID = -1;
	if(!CInterface_SvrUserOrg::getObj().GetOrgIDByAccount(sForceClose.strInvestID, nOrgID))
		return;

	set<int> setRealUsers;
	std::vector<UserInfo> vecTemp;
	CInterface_SvrUserOrg::getObj().GetUserInfosByOrgID(nOrgID, vecTemp);
	for(int i =0; i< (int)vecTemp.size();i++)
	{
		UserInfo& userInfo = vecTemp[i];
		if(userInfo.nUserType == USER_TYPE_RISK_CONTROL)
		{
			setRealUsers.insert(userInfo.nUserID);			
		}
	}
	
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);		
	int nExpiredDate = sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;
	
	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"风险事件提示");

	std::string strName;
	SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)sForceClose.nRiskIndicatorID , strName);
	sprintf(MsgInfo.szContent,"自动强平失败，请风控员手动强平。\n存在风险的交易员：%s\n  触发风险类型:%s;\n 存在风险的合约:%s;\n 风险等级:%d",  sForceClose.strInvestID.c_str(), strName.c_str(),sForceClose.strInstrument.c_str(), sForceClose.nRiskLevelID);
	CInterface_SvrMsg::getObj().SendMsg(0, setRealUsers, conAutoMsg, nExpiredDate, 
		MsgInfo.szTitle, MsgInfo.szContent);
}
void CRiskMgr::GenerateInsertOrder( const std::vector<PlatformStru_Position>& vPosition, 
								   std::vector<PlatformStru_InputOrder>& vInsertOrder )
{
	for ( UINT i = 0; i < vPosition.size(); i++ )
	{
		PlatformStru_Position position = vPosition[i];
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
		int nTodayOpenOrderVolumn    = 0;
		//获取挂单量
	//	SAFE_GET_DATACENTER()->GetOpenOrderVolumn(position.InvestorID,position.InstrumentID, 
	//		Direction, nTodayOpenOrderVolumn, nYestoDayOpenOrderVolumn);
		std::vector<PlatformStru_OrderInfo> vOrder;
		CInterface_SvrTradeData::getObj().GetUserWaitOrders(position.InvestorID, vOrder);
		for (int k = 0; k < (int)vOrder.size(); k++)
		{
			PlatformStru_OrderInfo& order = vOrder[k];
			if(strcmp(position.InstrumentID, order.InstrumentID) !=0)
				continue;//只有要下的单和已经挂的单一样，才需要后面减去这个挂单数量
			if ( order.CombOffsetFlag[0] == THOST_FTDC_OF_Close )
			{
				nYestoDayOpenOrderVolumn += order.VolumeTotal;
			}
			else if ( order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
			{
				nTodayOpenOrderVolumn += order.VolumeTotal;
			}
			else if ( order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday )
			{
				nYestoDayOpenOrderVolumn += order.VolumeTotal;
			}
		}
		
		if ( position.TodayPosition != 0 )
		{
			int nCanCloseVolumn = position.TodayPosition - nTodayOpenOrderVolumn;
			if ( nCanCloseVolumn < 0 )
			{
				continue;
			}

			//平今
			PlatformStru_InputOrder  inputOrder;
			memset(&inputOrder, 0, sizeof(inputOrder));
			if( !NewCloseOrder(position, inputOrder, Direction, nCanCloseVolumn, THOST_FTDC_OF_CloseToday))
			{
				continue;
			}
			else
			{
				while ( inputOrder.VolumeTotalOriginal > MAX_CAN_CLOSE_VOLUMN )
				{
					PlatformStru_InputOrder order = inputOrder;
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
		//	if ( nCanCloseVolumn <= 0 )
		//	{
		//		continue;
		//	}

			//平仓
			PlatformStru_InputOrder inputOrder;
			memset(&inputOrder, 0, sizeof(inputOrder));
			if(!NewCloseOrder(position, inputOrder, Direction, nCanCloseVolumn, THOST_FTDC_OF_Close))
			{
				continue;
			}
			else
			{
				while ( inputOrder.VolumeTotalOriginal > MAX_CAN_CLOSE_VOLUMN )
				{
					PlatformStru_InputOrder order = inputOrder;
					order.VolumeTotalOriginal = MAX_CAN_CLOSE_VOLUMN;
					inputOrder.VolumeTotalOriginal -= MAX_CAN_CLOSE_VOLUMN;
					vInsertOrder.push_back(order);
				}

				vInsertOrder.push_back(inputOrder);
			}
		}
	}
}
void CRiskMgr::GenerateOrderAction( const std::vector<PlatformStru_OrderInfo>& vOrder, 
								   std::vector<PlatformStru_InputOrderAction>& vOrderAction )
{
	for ( UINT i = 0; i < vOrder.size(); i++ )
	{
		PlatformStru_OrderInfo order = vOrder[i];

		//涨跌停价限价单不撤
		//取涨跌停价
		double dUpperLimitPrice, dLowerLimitPrice;
		bool bFlag = false;
		PlatformStru_DepthMarketData Quot;		
		bFlag = CInterface_SvrTradeData::getObj().GetQuotInfo(order.InstrumentID,Quot)==CF_ERROR_SUCCESS;
		dUpperLimitPrice = Quot.UpperLimitPrice;
		dLowerLimitPrice = Quot.LowerLimitPrice;
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
		

		PlatformStru_InputOrderAction OrderAction;
		memset(&OrderAction, 0, sizeof(OrderAction));
		OrderAction.Thost.FrontID = order.FrontID;
		OrderAction.Thost.SessionID = order.SessionID;
		strcpy(OrderAction.Thost.OrderRef, order.OrderRef);
		strcpy(OrderAction.Thost.OrderSysID, order.OrderSysID);
		strcpy(OrderAction.Thost.ExchangeID, order.ExchangeID);
		strcpy(OrderAction.Thost.InstrumentID, order.InstrumentID);
		strcpy(OrderAction.Thost.BrokerID, order.BrokerID);
		strcpy(OrderAction.Thost.InvestorID, order.InvestorID);
		OrderAction.Thost.RequestID = i+1;
		OrderAction.Thost.ActionFlag = THOST_FTDC_AF_Delete;

		vOrderAction.push_back(OrderAction);
	}
}
bool CRiskMgr::NewCloseOrder( const PlatformStru_Position &position, PlatformStru_InputOrder &inputOrder, 
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

	PlatformStru_InstrumentInfo info;
	if(CDataCenter::Get()->GetInstrumentInfo_DataCenter(position.InstrumentID,info) != CF_ERROR_SUCCESS)
		return false;

	strExchangeID = info.ExchangeID;

/*
	if ( NULL != CDataCenter::Get() )
	{
		bFlag = CDataCenter::Get()->GetExchangeID(position.InstrumentID, strExchangeID);
	}

	if ( !bFlag )
	{
		return false;
	}
*/
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
		PlatformStru_DepthMarketData Quot;
		if(CInterface_SvrTradeData::getObj().GetQuotInfo(position.InstrumentID, Quot) != CF_ERROR_SUCCESS)
			return false;
		dUpperLimitPrice = Quot.UpperLimitPrice;
		dLowerLimitPrice = Quot.LowerLimitPrice;
/*
		if ( NULL != CDataCenter::Get() )
		{
			bFlag = CDataCenter::Get()->GetLimitPrice(position.InstrumentID, dUpperLimitPrice, dLowerLimitPrice);
		}

		if ( !bFlag )
		{
			return false;
		}
*/
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
void CRiskMgr::GenerateInsertOrder_PositionDetail(PlatformStru_PositionDetail& outData, std::vector<PlatformStru_InputOrder>& vInsertOrder)
{
	char Direction;
	if ( outData.Direction == THOST_FTDC_D_Buy )
	{
		Direction = THOST_FTDC_D_Sell;
	}
	else if ( outData.Direction == THOST_FTDC_D_Sell )
	{
		Direction = THOST_FTDC_D_Buy;
	}
	int nCanCloseVolumn  = outData.Volume;

	char cCloseFlag;
	if(outData.bToday)
		cCloseFlag = THOST_FTDC_OF_CloseToday;
	else
		cCloseFlag = THOST_FTDC_OF_Close;

	PlatformStru_InputOrder inputOrder;
	memset(&inputOrder, 0, sizeof(inputOrder));

	strcpy(inputOrder.BrokerID, outData.BrokerID);
	strcpy(inputOrder.InvestorID, outData.InvestorID);
	strcpy(inputOrder.InstrumentID, outData.InstrumentID);
	strcpy(inputOrder.UserID, outData.InvestorID);
	//买卖方向
	inputOrder.Direction = Direction;

	//价格类型、价格
	//郑商所支持市价下单
	std::string strExchangeID;
	bool bFlag = false;

	PlatformStru_InstrumentInfo info;
	if(CDataCenter::Get()->GetInstrumentInfo_DataCenter(outData.InstrumentID,info) != CF_ERROR_SUCCESS)
		return ;

	strExchangeID = info.ExchangeID;

	/*
	if ( NULL != CDataCenter::Get() )
	{
	bFlag = CDataCenter::Get()->GetExchangeID(position.InstrumentID, strExchangeID);
	}

	if ( !bFlag )
	{
	return false;
	}
	*/
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
		PlatformStru_DepthMarketData Quot;
		if(CInterface_SvrTradeData::getObj().GetQuotInfo(outData.InstrumentID, Quot) != CF_ERROR_SUCCESS)
			return ;
		dUpperLimitPrice = Quot.UpperLimitPrice;
		dLowerLimitPrice = Quot.LowerLimitPrice;
		/*
		if ( NULL != CDataCenter::Get() )
		{
		bFlag = CDataCenter::Get()->GetLimitPrice(position.InstrumentID, dUpperLimitPrice, dLowerLimitPrice);
		}

		if ( !bFlag )
		{
		return false;
		}
		*/
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

	inputOrder.VolumeTotalOriginal = nCanCloseVolumn;
	inputOrder.VolumeCondition = THOST_FTDC_VC_AV; 
	inputOrder.MinVolume = 1;
	inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;	//立即下单模式
	inputOrder.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	//开平标识
	inputOrder.CombOffsetFlag[0] = cCloseFlag;


	vInsertOrder.push_back(inputOrder);
}