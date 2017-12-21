#include "stdafx.h"
#include "FTTraderSpi.h"
#include "Interface_SvrTcp.h"
#include "fttcmddef.h"
#include "DataPackageForwarding.h"
#include "SvrTradeFront.h"
#include "LinkEntity.h"


static CThostFtdcRspInfoField BlankRspInfo;

CFTTraderSpi::CFTTraderSpi(void)
{
	 memset(&BlankRspInfo,0,sizeof(BlankRspInfo));
	 memset(&m_ReqUserLoginField,0,sizeof(m_ReqUserLoginField));
	 m_nRequestID =0;
	 m_pUserApi=NULL;
	m_pLinkEntity = NULL;
}

CFTTraderSpi::~CFTTraderSpi(void)
{
	m_pLinkEntity = NULL;
}

void CFTTraderSpi::SetSocket(int nSocket)
{
	m_nSocket = nSocket;

}

void CFTTraderSpi::SetLinkEntity(LPVOID pLinkEntity)
{
	m_pLinkEntity = pLinkEntity;
}

void CFTTraderSpi::SetCTPTraderApi(CThostFtdcTraderApi* pApi)
{
	m_pUserApi = pApi;

}
void CFTTraderSpi::LoginCtp(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
	//LOG_INFO("CFTTraderSpi::LoginCtp(交易客户端登录):\n"
	//	"\t\t\t\t\t\t\t\t\t BrokerID=[%s],ClientIPAddress=[%s],UserID=[%s]\n"
	//	"\t\t\t\t\t\t\t\t\t TradingDay=[%s]\n",
	//	pReqUserLoginField->BrokerID,pReqUserLoginField->ClientIPAddress,pReqUserLoginField->UserID,pReqUserLoginField->TradingDay);

	m_ReqUserLoginField = *pReqUserLoginField;
    m_nRequestID = nRequestID;
}
void CFTTraderSpi:: OnFrontConnected()
{
	//int nServSeq = _GetForwarding().GetNextSeqID();

	//_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnFrontConnected_RON,0,0,3,nServSeq,m_nSocket);

	LOG_INFO("OnFrontConnected(与交易所成功建立TCP连接)\n");

	//CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
	//	FTTCMD_OnFrontConnected_RON,
	//	NULL ,
	//	0,
	//	0,
	//	0,
	//	CF_ERROR_SUCCESS);
	if(m_pLinkEntity != NULL) {
		((CLinkEntity*)m_pLinkEntity)->SendPkgData(
				FTTCMD_OnFrontConnected_RON, NULL, 0, 0, 0);
	}

	//_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnFrontConnected_RON,0,0,4,nServSeq,m_nSocket);

}

void CFTTraderSpi::OnFrontDisconnected(int nReason)
{

	//int nServSeq = _GetForwarding().GetNextSeqID();

	//_GetForwarding().SendPackage(&nReason,sizeof(nReason),FTTCMD_OnFrontDisconnected_RON,0,0,3,nServSeq,m_nSocket);


	LOG_INFO("OnFrontDisconnected(交易所TCP失去连接)\n"
		"\t\t\t ret=[%d]",nReason);

	//CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
	//	FTTCMD_OnFrontDisconnected_RON,
	//	&nReason,
	//	sizeof(nReason),
	//	0,
	//	0,
	//	CF_ERROR_SUCCESS);
	if(m_pLinkEntity != NULL) {
		((CLinkEntity*)m_pLinkEntity)->SendPkgData(
				FTTCMD_OnFrontDisconnected_RON, NULL, 0, 0, 0);
	}

	//_GetForwarding().SendPackage(&nReason,sizeof(nReason),FTTCMD_OnFrontDisconnected_RON,0,0,4,nServSeq,m_nSocket);
}

///登录请求响应
void CFTTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	LOG_INFO("CFTTraderSpi::OnRspUserLogin(登录请求响应):\n"
		"\t\t\t\t\t\t\t\t\t ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]\n"
		"\t\t\t\t\t\t\t\t\t TradingDay=[%s],\t LoginTime=[%s],\t BrokerID=[%s],\t UserID=[%s],\t SystemName=[%s]\n"
		"\t\t\t\t\t\t\t\t\t FrontID=[%d],\t SessionID=[%#x],\t MaxOrderRef[%s],\t SHFETime=[%s],\t DCETime=[%s],\n"
		"\t\t\t\t\t\t\t\t\t CZCETime[%s],\t FFEXTime=[%s]",
		pRspInfo?pRspInfo->ErrorID:-1, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
		pRspUserLogin->TradingDay,pRspUserLogin->LoginTime,pRspUserLogin->BrokerID   , pRspUserLogin->UserID,pRspUserLogin->SystemName, 
		pRspUserLogin->FrontID  , pRspUserLogin->SessionID ,pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime , pRspUserLogin->DCETime, 
		pRspUserLogin->CZCETime   , pRspUserLogin->FFEXTime);

	DataRonUserLogin tmp(*pRspUserLogin,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	//CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
	//	FTTCMD_OnRspUserLogin_RON,
	//	&tmp ,
	//	sizeof(tmp),
	//	_GetNextSeq(), 0, nRequestID);
	if(m_pLinkEntity != NULL) {
		((CLinkEntity*)m_pLinkEntity)->SendPkgData(
				FTTCMD_OnRspUserLogin_RON, &tmp, sizeof(tmp), 
				_GetNextSeq(), 0, nRequestID);
	}

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pRspUserLogin)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);

	memset(pScrBuff, 0, nSrcDataLen);
	if(pRspUserLogin)
		memcpy(pScrBuff, pRspUserLogin, sizeof(*pRspUserLogin));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pRspUserLogin), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pRspUserLogin)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pRspUserLogin)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspUserLogin_RON,0,nRequestID,3,nServSeq,m_nSocket);
	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspUserLogin_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///登出请求响应
void CFTTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pUserLogout)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pUserLogout)
		memcpy(pScrBuff, pUserLogout, sizeof(*pUserLogout));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pUserLogout), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pUserLogout)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pUserLogout)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspUserLogout_RON,0,nRequestID,3,nServSeq,m_nSocket);

	CThostFtdcUserLogoutField UserLogout;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pUserLogout)
	{
		memset(&UserLogout,0,sizeof(UserLogout));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		UserLogout=*pUserLogout;
	}

	DataRonUserLogout tmp(UserLogout,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspUserLogout_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspUserLogout(登出请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t UserID=[%s],",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		tmp.UserLogoutField.BrokerID, tmp.UserLogoutField.UserID);

   _GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspUserLogout_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///用户口令更新请求响应
void CFTTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pUserPasswordUpdate)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pUserPasswordUpdate)
		memcpy(pScrBuff, pUserPasswordUpdate, sizeof(*pUserPasswordUpdate));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pUserPasswordUpdate), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pUserPasswordUpdate)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pUserPasswordUpdate)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspUserPasswordUpdate_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonUserPasswordUpdate tmp(*pUserPasswordUpdate,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspUserPasswordUpdate_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspUserPasswordUpdate(用户口令更新请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t UserID=[%s],",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pUserPasswordUpdate->BrokerID, pUserPasswordUpdate->UserID);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspUserPasswordUpdate_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///资金账户口令更新请求响应
void CFTTraderSpi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pTradingAccountPasswordUpdate)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pTradingAccountPasswordUpdate)
		memcpy(pScrBuff, pTradingAccountPasswordUpdate, sizeof(*pTradingAccountPasswordUpdate));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pTradingAccountPasswordUpdate), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pTradingAccountPasswordUpdate)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pTradingAccountPasswordUpdate)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspTradingAccountPasswordUpdate_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonTradingAccountPasswordUpdate tmp(*pTradingAccountPasswordUpdate,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspTradingAccountPasswordUpdate_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspTradingAccountPasswordUpdate(资金账户口令更新请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t AccountID=[%s],",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pTradingAccountPasswordUpdate->BrokerID, pTradingAccountPasswordUpdate->AccountID);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspTradingAccountPasswordUpdate_RON,0,nRequestID,4,nServSeq,m_nSocket);
}

///报单录入请求响应
void CFTTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInputOrder)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInputOrder)
		memcpy(pScrBuff, pInputOrder, sizeof(*pInputOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInputOrder), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInputOrder)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInputOrder)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspOrderInsert_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonOrderInsert tmp(*pInputOrder,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspOrderInsert_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspOrderInsert(报单录入请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
		"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
		"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
		"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
		"\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
		pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
		pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
		pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
		pInputOrder->RequestID, pInputOrder->UserForceClose);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspOrderInsert_RON,0,nRequestID,4,nServSeq,m_nSocket);
}

///预埋单录入请求响应
void CFTTraderSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pParkedOrder)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pParkedOrder)
		memcpy(pScrBuff, pParkedOrder, sizeof(*pParkedOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pParkedOrder), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pParkedOrder)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pParkedOrder)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspParkedOrderInsert_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonParkedOrderInsert tmp(*pParkedOrder,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspParkedOrderInsert_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);
	
	LOG_INFO("OnRspParkedOrderInsert(预埋单录入请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
		"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
		"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
		"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
		"\t\t\t RequestID=[%d],\t UserForceClose=[%d],\t ExchangeID=[%s],\t ParkedOrderID=[%s],\t UserType=[%c],\n"
		"\t\t\t Status=[%c],\t ErrorID=[%d],\t ErrorMsg=[%s]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pParkedOrder->BrokerID, pParkedOrder->InvestorID, pParkedOrder->InstrumentID, pParkedOrder->OrderRef,pParkedOrder->UserID,
		pParkedOrder->OrderPriceType, pParkedOrder->Direction, pParkedOrder->CombOffsetFlag, pParkedOrder->CombHedgeFlag,pParkedOrder->LimitPrice,
		pParkedOrder->VolumeTotalOriginal, pParkedOrder->TimeCondition, pParkedOrder->GTDDate, pParkedOrder->VolumeCondition,pParkedOrder->MinVolume,
		pParkedOrder->ContingentCondition, pParkedOrder->StopPrice, pParkedOrder->ForceCloseReason, pParkedOrder->IsAutoSuspend,pParkedOrder->BusinessUnit,
		pParkedOrder->RequestID, pParkedOrder->UserForceClose, pParkedOrder->ExchangeID, pParkedOrder->ParkedOrderID, pParkedOrder->UserType, 
		pParkedOrder->Status, pParkedOrder->ErrorID, pParkedOrder->ErrorMsg);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspParkedOrderInsert_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///预埋撤单录入请求响应
void CFTTraderSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pParkedOrderAction)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pParkedOrderAction)
		memcpy(pScrBuff, pParkedOrderAction, sizeof(*pParkedOrderAction));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pParkedOrderAction), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pParkedOrderAction)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pParkedOrderAction)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspParkedOrderAction_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonParkedOrderAction tmp(*pParkedOrderAction,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspParkedOrderAction_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspParkedOrderAction(预埋撤单录入请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
		"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
		"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s],\t ParkedOrderActionID=[%d],\n"
		"\t\t\t UserType=[%c],\t Status=[%c],\t ErrorID=[%d],\t ErrorMsg=[%s]", 
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pParkedOrderAction->BrokerID, pParkedOrderAction->InvestorID, pParkedOrderAction->OrderActionRef, pParkedOrderAction->OrderRef,pParkedOrderAction->RequestID,
		pParkedOrderAction->FrontID, pParkedOrderAction->SessionID, pParkedOrderAction->ExchangeID, pParkedOrderAction->OrderSysID,pParkedOrderAction->ActionFlag,
		pParkedOrderAction->LimitPrice, pParkedOrderAction->VolumeChange, pParkedOrderAction->UserID, pParkedOrderAction->InstrumentID, pParkedOrderAction->ParkedOrderActionID, 
		pParkedOrderAction->UserType, pParkedOrderAction->Status, pParkedOrderAction->ErrorID, pParkedOrderAction->ErrorMsg);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspParkedOrderAction_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///报单操作请求响应
void CFTTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInputOrderAction)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInputOrderAction)
		memcpy(pScrBuff, pInputOrderAction, sizeof(*pInputOrderAction));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInputOrderAction), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInputOrderAction)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInputOrderAction)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspOrderAction_RON,0,nRequestID,3,nServSeq,m_nSocket);

	PlatformStru_InputOrderAction field;
	field.Thost = *pInputOrderAction;

	DataRonOrderAction tmp(field,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspOrderAction_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);
 
	LOG_INFO("OnRspOrderAction(报单操作请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
		"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
		"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pInputOrderAction->BrokerID, pInputOrderAction->InvestorID, pInputOrderAction->OrderActionRef, pInputOrderAction->OrderRef,pInputOrderAction->RequestID,
		pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->ExchangeID, pInputOrderAction->OrderSysID,pInputOrderAction->ActionFlag,
		pInputOrderAction->LimitPrice, pInputOrderAction->VolumeChange, pInputOrderAction->UserID, pInputOrderAction->InstrumentID);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspOrderAction_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///查询最大报单数量响应
void CFTTraderSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pQueryMaxOrderVolume)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pQueryMaxOrderVolume)
		memcpy(pScrBuff, pQueryMaxOrderVolume, sizeof(*pQueryMaxOrderVolume));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pQueryMaxOrderVolume), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pQueryMaxOrderVolume)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pQueryMaxOrderVolume)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQueryMaxOrderVolume_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonQueryMaxOrderVolume tmp(*pQueryMaxOrderVolume,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQueryMaxOrderVolume_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspQueryMaxOrderVolume(查询最大报单数量响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pQueryMaxOrderVolume->BrokerID, pQueryMaxOrderVolume->InvestorID);


	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQueryMaxOrderVolume_RON,0,nRequestID,4,nServSeq,m_nSocket);
}

///投资者结算结果确认响应
void CFTTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pSettlementInfoConfirm)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pSettlementInfoConfirm)
		memcpy(pScrBuff, pSettlementInfoConfirm, sizeof(*pSettlementInfoConfirm));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pSettlementInfoConfirm), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pSettlementInfoConfirm)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pSettlementInfoConfirm)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspSettlementInfoConfirm_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonSettlementInfoConfirm tmp(*pSettlementInfoConfirm,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspSettlementInfoConfirm_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspSettlementInfoConfirm(投资者结算结果确认响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],ConfirmDate=[%s],ConfirmTime=[%s]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pSettlementInfoConfirm->BrokerID, pSettlementInfoConfirm->InvestorID,pSettlementInfoConfirm->ConfirmDate,pSettlementInfoConfirm->ConfirmTime);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspSettlementInfoConfirm_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///删除预埋单响应
void CFTTraderSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pRemoveParkedOrder)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pRemoveParkedOrder)
		memcpy(pScrBuff, pRemoveParkedOrder, sizeof(*pRemoveParkedOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pRemoveParkedOrder), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pRemoveParkedOrder)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pRemoveParkedOrder)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspRemoveParkedOrder_RON,0,nRequestID,3,nServSeq,m_nSocket);


	DataRonRemoveParkedOrder tmp(*pRemoveParkedOrder,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspRemoveParkedOrder_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspRemoveParkedOrder(删除预埋单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderID=[%s]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pRemoveParkedOrder->BrokerID, pRemoveParkedOrder->InvestorID, pRemoveParkedOrder->ParkedOrderID);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspRemoveParkedOrder_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///删除预埋撤单响应
void CFTTraderSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pRemoveParkedOrderAction)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pRemoveParkedOrderAction)
		memcpy(pScrBuff, pRemoveParkedOrderAction, sizeof(*pRemoveParkedOrderAction));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pRemoveParkedOrderAction), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pRemoveParkedOrderAction)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pRemoveParkedOrderAction)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspRemoveParkedOrderAction_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonRemoveParkedOrderAction tmp(*pRemoveParkedOrderAction,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspRemoveParkedOrderAction_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspRemoveParkedOrderAction(删除预埋撤单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderActionID=[%s]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pRemoveParkedOrderAction->BrokerID, pRemoveParkedOrderAction->InvestorID, pRemoveParkedOrderAction->ParkedOrderActionID);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspRemoveParkedOrderAction_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///执行宣告录入请求响应
void CFTTraderSpi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInputExecOrder)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInputExecOrder)
		memcpy(pScrBuff, pInputExecOrder, sizeof(*pInputExecOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInputExecOrder), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInputExecOrder)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInputExecOrder)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspExecOrderInsert_RON,0,nRequestID,3,nServSeq,m_nSocket);

	CThostFtdcInputExecOrderField InputExecOrder={0};
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(pInputExecOrder) InputExecOrder=*pInputExecOrder;
	else if(RspInfo.ErrorID==0)
	{
		RspInfo.ErrorID=CustomErrorID;
		strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
	}

	DataRonExecOrderInsert tmp(PlatformStru_InputExecOrderField(InputExecOrder),RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspExecOrderInsert_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	if(pInputExecOrder&&pRspInfo)
	{
		LOG_INFO("OnRspExecOrderInsert(执行宣告录入请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n%s",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID,bIsLast,tmp.InputExecOrderField.tostring().c_str());
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspExecOrderInsert_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///执行宣告操作请求响应
void CFTTraderSpi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInputExecOrderAction)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInputExecOrderAction)
		memcpy(pScrBuff, pInputExecOrderAction, sizeof(*pInputExecOrderAction));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInputExecOrderAction), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInputExecOrderAction)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInputExecOrderAction)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspExecOrderAction_RON,0,nRequestID,3,nServSeq,m_nSocket);

	CThostFtdcInputExecOrderActionField InputExecOrderAction={0};
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(pInputExecOrderAction) InputExecOrderAction=*pInputExecOrderAction;
	else if(RspInfo.ErrorID==0)
	{
		RspInfo.ErrorID=CustomErrorID;
		strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
	}
	DataRonExecOrderAction tmp(PlatformStru_InputExecOrderActionField(InputExecOrderAction),RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspExecOrderAction_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);


	if(pInputExecOrderAction&&pRspInfo)
	{
		LOG_INFO("OnRspExecOrderAction(执行宣告操作请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n%s",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID,bIsLast,tmp.InputExecOrderActionField.tostring().c_str());
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspExecOrderAction_RON,0,nRequestID,4,nServSeq,m_nSocket);
}

///询价录入请求响应
void CFTTraderSpi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInputForQuote)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInputForQuote)
		memcpy(pScrBuff, pInputForQuote, sizeof(*pInputForQuote));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInputForQuote), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInputForQuote)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInputForQuote)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspForQuoteInsert_RON,0,nRequestID,3,nServSeq,m_nSocket);

	CThostFtdcInputForQuoteField InputForQuote={0};
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(pInputForQuote) InputForQuote=*pInputForQuote;
	else if(RspInfo.ErrorID==0)
	{
		RspInfo.ErrorID=CustomErrorID;
		strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
	}

	DataRonForQuoteInsert tmp(PlatformStru_InputForQuoteField(InputForQuote),RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspForQuoteInsert_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	if(pInputForQuote&&pRspInfo)
	{
		LOG_INFO("OnRspForQuoteInsert(询价录入请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n%s",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID,bIsLast,tmp.InputForQuoteField.tostring().c_str());
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspForQuoteInsert_RON,0,nRequestID,4,nServSeq,m_nSocket);
}

/////报价录入请求响应
//void CFTTraderSpi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}
//
/////报价操作请求响应
//void CFTTraderSpi::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询报单响应
void CFTTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pOrder)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pOrder)
		memcpy(pScrBuff, pOrder, sizeof(*pOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pOrder), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pOrder)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pOrder)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryOrder_RON,0,nRequestID,3,nServSeq,m_nSocket);

	PlatformStru_OrderInfo OrderField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pOrder)
	{
		OrderField.Clear();
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		OrderField=PlatformStru_OrderInfo(*pOrder);
	}

	DataRonQryOrder tmp(OrderField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryOrder_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);


	//log
	if(!pOrder)
	{
		LOG_INFO("OnRspQryOrder(请求查询报单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);
	}
	else
	{
		PlatformStru_OrderInfo* pOrder = &tmp.OrderField;
		LOG_INFO("OnRspQryOrder(请求查询报单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
			"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
			"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
			"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
			"\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
			"\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
			"\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
			"\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
			"\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
			"\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
			"\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
			pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
			pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
			pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
			pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
			pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ParticipantID,pOrder->ClientID,
			pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
			pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
			pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
			pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
			pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
			pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryOrder_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///请求查询成交响应
void CFTTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pTrade)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pTrade)
		memcpy(pScrBuff, pTrade, sizeof(*pTrade));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pTrade), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pTrade)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pTrade)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryTrade_RON,0,nRequestID,3,nServSeq,m_nSocket);

	PlatformStru_TradeInfo TradeField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pTrade)
	{
		TradeField.Clear();
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{    
		TradeField=PlatformStru_TradeInfo(*pTrade);
	}

	DataRonQryTrade tmp(TradeField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryTrade_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryTrade(请求查询成交响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else if(pTrade!=NULL)
	{
		LOG_INFO("OnRspQryTrade(请求查询成交响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
			"\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
			"\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
			"\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
			"\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
			"\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d],\t TradeSource=[%c]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
			pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
			pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
			pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
			pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
			pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
			pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq, pTrade->TradeSource
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryTrade_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///请求查询投资者持仓响应
void CFTTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInvestorPosition)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInvestorPosition)
		memcpy(pScrBuff, pInvestorPosition, sizeof(*pInvestorPosition));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInvestorPosition), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInvestorPosition)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInvestorPosition)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryInvestorPosition_RON,0,nRequestID,3,nServSeq,m_nSocket);

	PlatformStru_Position InvestorPositionField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	TThostFtdcInstrumentIDType	QryInstrumentID;
	memset(&QryInstrumentID,0,sizeof(QryInstrumentID));
	if(!pInvestorPosition)
	{
		InvestorPositionField.Clear();
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{    
		InvestorPositionField=PlatformStru_Position(*pInvestorPosition);
	}

	DataRonQryInvestorPosition tmp(InvestorPositionField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryInvestorPosition_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	
	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryInvestorPosition(请求查询投资者持仓响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else if(pInvestorPosition!=NULL)
	{
		LOG_INFO("OnRspQryInvestorPosition(请求查询投资者持仓响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t PosiDirection=[%d],\t HedgeFlag=[%d],\n"
			"\t\t\t PositionDate=[%d],\t YdPosition=[%d],\t Position=[%d],\t LongFrozen=[%d],\t ShortFrozen=[%d],\n"
			"\t\t\t LongFrozenAmount=[%g],\t  ShortFrozenAmount=[%g],\t OpenVolume=[%d],\t CloseVolume=[%d],\t OpenAmount=[%g],\n"
			"\t\t\t CloseAmount=[%g],\t PositionCost=[%g],\t PreMargin=[%g],\t UseMargin=[%g],\t FrozenMargin=[%g],\n"
			"\t\t\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CashIn=[%g],\t Commission=[%g],CloseProfit=[%g],\n"
			"\t\t\t PositionProfit=[%g],\t PreSettlementPrice=[%g],\t SettlementPrice=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n"
			"\t\t\t OpenCost=[%g],\t ExchangeMargin=[%g],\t CombPosition=[%d],\t CombLongFrozen=[%d],\t CombShortFrozen=[%d],\n"
			"\t\t\t CloseProfitByDate=[%g],\t CloseProfitByTrade=[%g],\t TodayPosition=[%d],\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			pInvestorPosition->InstrumentID, pInvestorPosition->BrokerID,pInvestorPosition->InvestorID,pInvestorPosition->PosiDirection,pInvestorPosition->HedgeFlag,
			pInvestorPosition->PositionDate, pInvestorPosition->YdPosition,pInvestorPosition->Position,pInvestorPosition->LongFrozen,pInvestorPosition->ShortFrozen,
			pInvestorPosition->LongFrozenAmount, pInvestorPosition->ShortFrozenAmount,pInvestorPosition->OpenVolume,pInvestorPosition->CloseVolume,pInvestorPosition->OpenAmount,
			pInvestorPosition->CloseAmount, pInvestorPosition->PositionCost,pInvestorPosition->PreMargin,pInvestorPosition->UseMargin,pInvestorPosition->FrozenMargin,
			pInvestorPosition->FrozenCash, pInvestorPosition->FrozenCommission,pInvestorPosition->CashIn,pInvestorPosition->Commission,pInvestorPosition->CloseProfit,
			pInvestorPosition->PositionProfit, pInvestorPosition->PreSettlementPrice,pInvestorPosition->SettlementPrice,pInvestorPosition->TradingDay,pInvestorPosition->SettlementID,
			pInvestorPosition->OpenCost, pInvestorPosition->ExchangeMargin,pInvestorPosition->CombPosition,pInvestorPosition->CombLongFrozen,pInvestorPosition->CombShortFrozen,
			pInvestorPosition->CloseProfitByDate, pInvestorPosition->CloseProfitByTrade,pInvestorPosition->TodayPosition,pInvestorPosition->MarginRateByMoney,pInvestorPosition->MarginRateByVolume
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryInvestorPosition_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///请求查询资金账户响应
void CFTTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pTradingAccount)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pTradingAccount)
		memcpy(pScrBuff, pTradingAccount, sizeof(*pTradingAccount));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pTradingAccount), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pTradingAccount)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pTradingAccount)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryTradingAccount_RON,0,nRequestID,3,nServSeq,m_nSocket);


	PlatformStru_TradingAccountInfo TradingAccountField;
	TradingAccountField.clear();
	strcpy(TradingAccountField.AccountID, pTradingAccount->AccountID);
	strcpy(TradingAccountField.Account, pTradingAccount->AccountID);
	strcpy(TradingAccountField.BrokerID, pTradingAccount->BrokerID);
	TradingAccountField.PreMortgage=pTradingAccount->PreMortgage;
	TradingAccountField.PreCredit=pTradingAccount->PreCredit;
	TradingAccountField.PreDeposit=pTradingAccount->PreDeposit;
	TradingAccountField.PreBalance=pTradingAccount->PreBalance;
	TradingAccountField.PreMargin=pTradingAccount->PreMargin;
	TradingAccountField.InterestBase=pTradingAccount->InterestBase;
	TradingAccountField.Interest=pTradingAccount->Interest;
	TradingAccountField.Deposit=pTradingAccount->Deposit;

	TradingAccountField.Withdraw=pTradingAccount->Withdraw;
	TradingAccountField.FrozenMargin=pTradingAccount->FrozenMargin;
	TradingAccountField.FrozenCommission=pTradingAccount->FrozenCommission;
	TradingAccountField.CurrMargin=pTradingAccount->CurrMargin;
	TradingAccountField.CashIn=pTradingAccount->CashIn;
	TradingAccountField.Commission=pTradingAccount->Commission;
	TradingAccountField.CloseProfit=pTradingAccount->CloseProfit;

	TradingAccountField.PositionProfit=pTradingAccount->PositionProfit;
	TradingAccountField.Balance=pTradingAccount->Balance;
	TradingAccountField.Available=pTradingAccount->Available;
	TradingAccountField.WithdrawQuota=pTradingAccount->WithdrawQuota;
	TradingAccountField.Reserve=pTradingAccount->Reserve;
	strcpy(TradingAccountField.TradingDay, pTradingAccount->TradingDay);
	TradingAccountField.SettlementID=pTradingAccount->SettlementID;
	TradingAccountField.Credit=pTradingAccount->Credit;
	TradingAccountField.Mortgage=pTradingAccount->Mortgage;
	TradingAccountField.ExchangeMargin=pTradingAccount->ExchangeMargin;
	TradingAccountField.DeliveryMargin=pTradingAccount->DeliveryMargin;
	TradingAccountField.ExchangeDeliveryMargin=pTradingAccount->ExchangeDeliveryMargin;

	DataRonQryTradingAccount tmp(TradingAccountField,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryTradingAccount_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspQryTradingAccount(请求查询资金账户响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t BrokerID=[%s],\t AccountID=[%s],\t PreMortgage=[%g],\t PreCredit=[%g],\t PreDeposit=[%g],\n"
		"\t\t\t PreBalance=[%g],\t PreMargin=[%g],\t InterestBase=[%g],\t Interest=[%g],\t Deposit=[%g],\n "
		"\t\t\t Withdraw=[%g],\t FrozenMargin=[%g],\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CurrMargin=[%g],\n "
		"\t\t\t CashIn=[%g],\t Commission=[%g],\t CloseProfit=[%g],\t PositionProfit=[%g],\t Balance=[%g],\n "
		"\t\t\t Available=[%g],\t WithdrawQuota=[%g],\t Reserve=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n "
		"\t\t\t Credit=[%g],\t Mortgage=[%g],\t ExchangeMargin=[%g],\t DeliveryMargin=[%g],\t ExchangeDeliveryMargin=[%g]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		pTradingAccount->BrokerID,pTradingAccount->AccountID,pTradingAccount->PreMortgage,pTradingAccount->PreCredit,pTradingAccount->PreDeposit, 
		pTradingAccount->PreBalance,pTradingAccount->PreMargin,pTradingAccount->InterestBase,pTradingAccount->Interest,pTradingAccount->Deposit, 
		pTradingAccount->Withdraw,pTradingAccount->FrozenMargin,pTradingAccount->FrozenCash,pTradingAccount->FrozenCommission,pTradingAccount->CurrMargin, 
		pTradingAccount->CashIn,pTradingAccount->Commission,pTradingAccount->CloseProfit,pTradingAccount->PositionProfit,pTradingAccount->Balance, 
		pTradingAccount->Available,pTradingAccount->WithdrawQuota,pTradingAccount->Reserve,pTradingAccount->TradingDay,pTradingAccount->SettlementID, 
		pTradingAccount->Credit,pTradingAccount->Mortgage,pTradingAccount->ExchangeMargin,pTradingAccount->DeliveryMargin,pTradingAccount->ExchangeDeliveryMargin
		);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryTradingAccount_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询投资者响应
void CFTTraderSpi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInvestor)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInvestor)
		memcpy(pScrBuff, pInvestor, sizeof(*pInvestor));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInvestor), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInvestor)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInvestor)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryInvestor_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcInvestorField Investor;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pInvestor)
	{
		memset(&Investor,0,sizeof(Investor));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		Investor = *pInvestor;
	}

	DataRonQryInvestor tmp(Investor,RspInfo,nRequestID,bIsLast);


	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryInvestor_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryInvestor(请求查询投资者响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		pInvestor= &tmp.InvestorField;
		LOG_INFO("OnRspQryInvestor(请求查询投资者响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t InvestorID=[%s],\t BrokerID=[%s],\t InvestorGroupID=[%s],\t InvestorName=[%s],\t IdentifiedCardType=[%d],\n"
			"\t\t\t IdentifiedCardNo=[%s],\t IsActive=[%d],\t Telephone=[%s],\t Address=[%s]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			pInvestor->InvestorID, pInvestor->BrokerID,pInvestor->InvestorGroupID,pInvestor->InvestorName,pInvestor->IdentifiedCardType,
			pInvestor->IdentifiedCardNo, pInvestor->IsActive,pInvestor->Telephone,pInvestor->Address
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryInvestor_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

/////请求查询交易编码响应
//void CFTTraderSpi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//{
//	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryTradingCode_RON,0,nRequestID,4,nServSeq,m_nSocket);
//
//
//	DataRonQryTradingCode tmp(*pTradingCode,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);
//
//
//	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
//		FTTCMD_OnRspQryTradingCode_RON,
//		&tmp ,
//		sizeof(tmp),
//		_GetNextSeq(), 0, nRequestID);
//
//	LOG_INFO("OnRspQryTradingCode(请求查询交易编码响应) : ErrorID=[%d],ErrorMsg=[%s],InvestorID=[%s],BrokerID=[%s],ClientID=[%s],nRequestID=[%d],bIsLast=[%d]",
//		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
//		pTradingCode->InvestorID, pTradingCode->BrokerID,pTradingCode->ClientID,nRequestID,bIsLast);
//
//	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryTradingCode_RON,0,nRequestID,4,nServSeq,m_nSocket);
//
//
//}

///请求查询合约保证金率响应
void CFTTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInstrumentMarginRate)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInstrumentMarginRate)
		memcpy(pScrBuff, pInstrumentMarginRate, sizeof(*pInstrumentMarginRate));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInstrumentMarginRate), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInstrumentMarginRate)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInstrumentMarginRate)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryInstrumentMarginRate_RON,0,nRequestID,3,nServSeq,m_nSocket);

	PlatformStru_InstrumentMarginRate InstrumentMarginRateField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pInstrumentMarginRate)
	{
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		InstrumentMarginRateField = PlatformStru_InstrumentMarginRate(*pInstrumentMarginRate);
	}

	DataRonQryInstrumentMarginRate tmp(InstrumentMarginRateField,RspInfo,nRequestID,bIsLast);


	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryInstrumentMarginRate_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryInstrumentMarginRate(请求查询合约保证金率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		pInstrumentMarginRate=(CThostFtdcInstrumentMarginRateField*)&tmp.InstrumentMarginRateField;
		LOG_INFO("OnRspQryInstrumentMarginRate(请求查询合约保证金率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\n"
			"\t\t\t LongMarginRatioByMoney=[%g],\t LongMarginRatioByVolume=[%g],\t ShortMarginRatioByMoney=[%g],\t ShortMarginRatioByVolume=[%g],\t IsRelative=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			InstrumentMarginRateField.InstrumentID,
			InstrumentMarginRateField.InvestorRange,
			InstrumentMarginRateField.BrokerID,
			InstrumentMarginRateField.InvestorID,
			InstrumentMarginRateField.HedgeFlag,
			InstrumentMarginRateField.LongMarginRatioByMoney,
			InstrumentMarginRateField.LongMarginRatioByVolume,
			InstrumentMarginRateField.ShortMarginRatioByMoney,
			InstrumentMarginRateField.ShortMarginRatioByVolume,
			InstrumentMarginRateField.IsRelative
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryInstrumentMarginRate_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询合约手续费率响应
void CFTTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInstrumentCommissionRate)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInstrumentCommissionRate)
		memcpy(pScrBuff, pInstrumentCommissionRate, sizeof(*pInstrumentCommissionRate));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInstrumentCommissionRate), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInstrumentCommissionRate)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInstrumentCommissionRate)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryInstrumentCommissionRate_RON,0,nRequestID,3,nServSeq,m_nSocket);


	PlatformStru_InstrumentCommissionRate InstrumentCommissionRateField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pInstrumentCommissionRate)
	{
		InstrumentCommissionRateField.clear();
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		InstrumentCommissionRateField = PlatformStru_InstrumentCommissionRate(*pInstrumentCommissionRate);
	}

	DataRonQryInstrumentCommissionRate tmp(InstrumentCommissionRateField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryInstrumentCommissionRate_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);
	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryInstrumentCommissionRate(请求查询合约手续费率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, 
			tmp.RspInfoField.ErrorMsg,
			nRequestID,bIsLast
			);

	}
	else
	{
		LOG_INFO("OnRspQryInstrumentCommissionRate(请求查询合约手续费率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t OpenRatioByMoney=[%g],\n"
			"\t\t\t OpenRatioByVolume=[%g],\t CloseRatioByMoney=[%g],\t CloseRatioByVolume=[%g],\t CloseTodayRatioByMoney=[%g],\t CloseTodayRatioByVolume=[%g]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			InstrumentCommissionRateField.InstrumentID,
			InstrumentCommissionRateField.InvestorRange,
			InstrumentCommissionRateField.BrokerID,
			InstrumentCommissionRateField.InvestorID,
			InstrumentCommissionRateField.OpenRatioByMoney,
			InstrumentCommissionRateField.OpenRatioByVolume,
			InstrumentCommissionRateField.CloseRatioByMoney,
			InstrumentCommissionRateField.CloseRatioByVolume,
			InstrumentCommissionRateField.CloseTodayRatioByMoney,
			InstrumentCommissionRateField.CloseTodayRatioByVolume
			);
	}


	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryInstrumentCommissionRate_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询交易所响应
void CFTTraderSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pExchange)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pExchange)
		memcpy(pScrBuff, pExchange, sizeof(*pExchange));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pExchange), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pExchange)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pExchange)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryExchange_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonQryExchange tmp(*pExchange,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryExchange_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	LOG_INFO("OnRspQryExchange(请求查询交易所响应) : ErrorID=[%d],ErrorMsg=[%s],ExchangeID=[%s],ExchangeName=[%s],ExchangeProperty=[%c],nRequestID=[%d],bIsLast=[%d]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
		pExchange->ExchangeID, pExchange->ExchangeName,pExchange->ExchangeProperty,nRequestID,bIsLast);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryExchange_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询产品响应
void CFTTraderSpi::OnRspQryProduct(CThostFtdcProductField *pProduct, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pProduct)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pProduct)
		memcpy(pScrBuff, pProduct, sizeof(*pProduct));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pProduct), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pProduct)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pProduct)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryProduct_RON,0,nRequestID,3,nServSeq,m_nSocket);

	PlatformStru_ProductInfo ProductField(*pProduct);
	DataRonQryProduct tmp(ProductField,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryProduct_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

#ifdef CTP060300
	LOG_INFO("OnRspQryProduct(请求查询产品响应) : ErrorID=[%d] ErrorMsg=[%s] RequestID=[%d] bIsLast=[%d] "
		"\n\t\t\t ProductID=[%s] \t ProductName=[%s] \t ExchangeID=[%s] "
		"\n\t\t\t ProductClass=[%d] \t VolumeMultiple=[%d] \t PriceTick=[%g] "
		"\n\t\t\t MaxMarketOrderVolume=[%d] \t MinMarketOrderVolume=[%d] \t MaxLimitOrderVolume=[%d] \t MinLimitOrderVolume=[%d] "
		"\n\t\t\t PositionType=[%d] \t PositionDateType=[%d] \t CloseDealType=[%d] \t TradeCurrencyID=[%s] \t MortgageFundUseRange=[%d] "
		"\n\t\t\t ExchangeProductID=[%s] \t UnderlyingMultiple=[%d] "
		,tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID, bIsLast
		,pProduct->ProductID, pProduct->ProductName, pProduct->ExchangeID
		,pProduct->ProductClass, pProduct->VolumeMultiple, pProduct->PriceTick
		,pProduct->MaxMarketOrderVolume, pProduct->MinMarketOrderVolume, pProduct->MaxLimitOrderVolume, pProduct->MinLimitOrderVolume
		,pProduct->PositionType, pProduct->PositionDateType, pProduct->CloseDealType, pProduct->TradeCurrencyID, pProduct->MortgageFundUseRange
		,pProduct->ExchangeProductID, pProduct->UnderlyingMultiple
		);
#else
	LOG_INFO("OnRspQryProduct(请求查询产品响应) : ErrorID=[%d] ErrorMsg=[%s] RequestID=[%d] bIsLast=[%d] "
		"\n\t\t\t ProductID=[%s] \t ProductName=[%s] \t ExchangeID=[%s] "
		"\n\t\t\t ProductClass=[%d] \t VolumeMultiple=[%d] \t PriceTick=[%g] "
		"\n\t\t\t MaxMarketOrderVolume=[%d] \t MinMarketOrderVolume=[%d] \t MaxLimitOrderVolume=[%d] \t MinLimitOrderVolume=[%d] "
		"\n\t\t\t PositionType=[%d] \t PositionDateType=[%d] \t CloseDealType=[%d] \t TradeCurrencyID=[%s] \t MortgageFundUseRange=[%d] "
		,tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID, bIsLast
		,pProduct->ProductID, pProduct->ProductName, pProduct->ExchangeID
		,pProduct->ProductClass, pProduct->VolumeMultiple, pProduct->PriceTick
		,pProduct->MaxMarketOrderVolume, pProduct->MinMarketOrderVolume, pProduct->MaxLimitOrderVolume, pProduct->MinLimitOrderVolume
		,pProduct->PositionType, pProduct->PositionDateType, pProduct->CloseDealType, pProduct->TradeCurrencyID, pProduct->MortgageFundUseRange
		);
#endif

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryProduct_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询合约响应
void CFTTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInstrument)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInstrument)
		memcpy(pScrBuff, pInstrument, sizeof(*pInstrument));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInstrument), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInstrument)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInstrument)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryInstrument_RON,0,nRequestID,3,nServSeq,m_nSocket);


	PlatformStru_InstrumentInfo InstrumentField(*pInstrument);
	DataRonQryInstrument tmp(InstrumentField,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryInstrument_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

#ifdef CTP060300
	LOG_INFO("OnRspQryInstrument(请求查询合约响应) : ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t InstrumentName=[%s],\t ExchangeInstID=[%s],\t ProductID=[%s], ProductClass=[%d],\n"
		"\t\t\t DeliveryYear=[%d],\t DeliveryMonth=[%d],\n"
		"\t\t\t MaxMarketOrderVolume=[%d],\t MinMarketOrderVolume=[%d],\t MaxLimitOrderVolume=[%d],\t MinLimitOrderVolume=[%d],\n"
		"\t\t\t VolumeMultiple=[%d],\t PriceTick=[%g],\n"
		"\t\t\t CreateDate=[%s],\t OpenDate=[%s],\t ExpireDate=[%s],\t StartDelivDate=[%s],\t EndDelivDate=[%s],\n"
		"\t\t\t InstLifePhase=[%c],\t IsTrading=[%d],\t PositionType=[%c],\t PositionDateType=[%c],\n"
		"\t\t\t LongMarginRatio=[%g],\t ShortMarginRatio=[%g],\t MaxMarginSideAlgorithm=[%d],\n"
		"\t\t\t UnderlyingInstrID=[%s],\t StrikePrice=[%g],\t OptionsType=[%d],\t UnderlyingMultiple=[%g],\t CombinationType=[%d]", 
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID, bIsLast,
		pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName, pInstrument->ExchangeInstID, pInstrument->ProductID, pInstrument->ProductClass, 
		pInstrument->DeliveryYear, pInstrument->DeliveryMonth, 
		pInstrument->MaxMarketOrderVolume, pInstrument->MinMarketOrderVolume, pInstrument->MaxLimitOrderVolume, pInstrument->MinLimitOrderVolume, 
		pInstrument->VolumeMultiple, pInstrument->PriceTick, 
		pInstrument->CreateDate, pInstrument->OpenDate, pInstrument->ExpireDate, pInstrument->StartDelivDate, pInstrument->EndDelivDate, 
		pInstrument->InstLifePhase, pInstrument->IsTrading, pInstrument->PositionType, pInstrument->PositionDateType, \
		pInstrument->LongMarginRatio, pInstrument->ShortMarginRatio,pInstrument->MaxMarginSideAlgorithm,
		pInstrument->UnderlyingInstrID,pInstrument->StrikePrice,pInstrument->OptionsType,pInstrument->UnderlyingMultiple,pInstrument->CombinationType
		);
#else
	LOG_INFO("OnRspQryInstrument(请求查询合约响应) : ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
		"\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t InstrumentName=[%s],\t ExchangeInstID=[%s],\t ProductID=[%s], \n"
		"\t\t\t ProductClass=[%c],\t DeliveryYear=[%u],\t DeliveryMonth=[%u],\t MaxMarketOrderVolume=[%d],\t MinMarketOrderVolume=[%d], \n"
		"\t\t\t MaxLimitOrderVolume=[%d],\t MinLimitOrderVolume=[%d],\t VolumeMultiple=[%d],\t PriceTick=[%g],\t CreateDate=[%s], \n"
		"\t\t\t OpenDate=[%s],\t ExpireDate=[%s],\t StartDelivDate=[%s],\t EndDelivDate=[%s],\t InstLifePhase=[%c], \n"
		"\t\t\t IsTrading=[%d],\t PositionType=[%c],\t PositionDateType=[%c],\t LongMarginRatio=[%g],\t ShortMarginRatio=[%g]\n"
		"\t\t\t MaxMarginSideAlgorithm=[%d]", 
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID, bIsLast,
		pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName, pInstrument->ExchangeInstID, pInstrument->ProductID, 
		pInstrument->ProductClass, pInstrument->DeliveryYear, pInstrument->DeliveryMonth, pInstrument->MaxMarketOrderVolume, pInstrument->MinMarketOrderVolume, 
		pInstrument->MaxLimitOrderVolume, pInstrument->MinLimitOrderVolume, pInstrument->VolumeMultiple, pInstrument->PriceTick, pInstrument->CreateDate, 
		pInstrument->OpenDate, pInstrument->ExpireDate, pInstrument->StartDelivDate, pInstrument->EndDelivDate, pInstrument->InstLifePhase, 
		pInstrument->IsTrading, pInstrument->PositionType, pInstrument->PositionDateType, pInstrument->LongMarginRatio, pInstrument->ShortMarginRatio,
		pInstrument->MaxMarginSideAlgorithm
		);
#endif


	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryInstrument_RON,0,nRequestID,4,nServSeq,m_nSocket);


}



///请求查询投资者结算结果响应
void CFTTraderSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pSettlementInfo)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pSettlementInfo)
		memcpy(pScrBuff, pSettlementInfo, sizeof(*pSettlementInfo));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pSettlementInfo), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pSettlementInfo)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pSettlementInfo)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQrySettlementInfo_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcSettlementInfoField SettlementInfoField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pSettlementInfo)
	{
		memset(&SettlementInfoField,0,sizeof(SettlementInfoField));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		SettlementInfoField = *pSettlementInfo;
	}

	DataRonQrySettlementInfo tmp(SettlementInfoField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQrySettlementInfo_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQrySettlementInfo(请求查询投资者结算结果响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);
		
	}
	else
	{
		pSettlementInfo= &tmp.SettlementInfoField;
		LOG_INFO("OnRspQrySettlementInfo(请求查询投资者结算结果响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t SequenceNo=[%d],\n"
			"\t\t\t Content=[%s]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			pSettlementInfo->TradingDay, pSettlementInfo->SettlementID,pSettlementInfo->BrokerID,pSettlementInfo->InvestorID,pSettlementInfo->SequenceNo,
			pSettlementInfo->Content
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQrySettlementInfo_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

/////请求查询转帐银行响应
//void CFTTraderSpi::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询投资者持仓明细响应
void CFTTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInvestorPositionDetail)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInvestorPositionDetail)
		memcpy(pScrBuff, pInvestorPositionDetail, sizeof(*pInvestorPositionDetail));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInvestorPositionDetail), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInvestorPositionDetail)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInvestorPositionDetail)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryInvestorPositionDetail_RON,0,nRequestID,3,nServSeq,m_nSocket);


	PlatformStru_PositionDetail InvestorPositionDetailField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	TThostFtdcInstrumentIDType	QryInstrumentID;
	memset(&QryInstrumentID,0,sizeof(QryInstrumentID));
	if(!pInvestorPositionDetail)
	{
		InvestorPositionDetailField.Clear();
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		InvestorPositionDetailField=PlatformStru_PositionDetail(*pInvestorPositionDetail);
	}

	DataRonQryInvestorPositionDetail tmp(InvestorPositionDetailField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryInvestorPositionDetail_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);
	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryInvestorPositionDetail(请求查询投资者持仓明细响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		//pInvestorPositionDetail=&tmp.InvestorPositionDetailField;
		LOG_INFO("OnRspQryInvestorPositionDetail(请求查询投资者持仓明细响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\t Direction=[%d],\n"
			"\t\t\t OpenDate=[%s],\t TradeID=[%s],\t Volume=[%d],\t OpenPrice=[%g],\t TradingDay=[%s],\n"
			"\t\t\t SettlementID=[%d],\t  TradeType=[%d],\t CombInstrumentID=[%s],\t ExchangeID=[%s],\t CloseProfitByDate=[%g],\n"
			"\t\t\t CloseProfitByTrade=[%g],\t PositionProfitByDate=[%g],\t PositionProfitByTrade=[%g],\t Margin=[%g],\t ExchMargin=[%g],\n"
			"\t\t\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g],\t LastSettlementPrice=[%g],\t SettlementPrice=[%g],CloseVolume=[%d],\n"
			"\t\t\t CloseAmount=[%g]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			pInvestorPositionDetail->InstrumentID, pInvestorPositionDetail->BrokerID,pInvestorPositionDetail->InvestorID,pInvestorPositionDetail->HedgeFlag,pInvestorPositionDetail->Direction,
			pInvestorPositionDetail->OpenDate, pInvestorPositionDetail->TradeID,pInvestorPositionDetail->Volume,pInvestorPositionDetail->OpenPrice,pInvestorPositionDetail->TradingDay,
			pInvestorPositionDetail->SettlementID, pInvestorPositionDetail->TradeType,pInvestorPositionDetail->CombInstrumentID,pInvestorPositionDetail->ExchangeID,pInvestorPositionDetail->CloseProfitByDate,
			pInvestorPositionDetail->CloseProfitByTrade, pInvestorPositionDetail->PositionProfitByDate,pInvestorPositionDetail->PositionProfitByTrade,pInvestorPositionDetail->Margin,pInvestorPositionDetail->ExchMargin,
			pInvestorPositionDetail->MarginRateByMoney, pInvestorPositionDetail->MarginRateByVolume,pInvestorPositionDetail->LastSettlementPrice,pInvestorPositionDetail->SettlementPrice,pInvestorPositionDetail->CloseVolume,
			pInvestorPositionDetail->CloseAmount
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryInvestorPositionDetail_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询客户通知响应
void CFTTraderSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pNotice)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pNotice)
		memcpy(pScrBuff, pNotice, sizeof(*pNotice));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pNotice), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pNotice)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pNotice)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryNotice_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcNoticeField NoticeField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pNotice)
	{
		memset(&NoticeField,0,sizeof(NoticeField));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		NoticeField=*pNotice;
	}

	DataRonQryNotice tmp(NoticeField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryNotice_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	if(pNotice)
	{
		LOG_INFO("OnRspQryNotice(请求查询客户通知响应)\n\t\t\t BrokerID=%s Content=%s SequenceLabel=%s",pNotice->BrokerID,pNotice->Content,pNotice->SequenceLabel);
	}
	else
	{
		LOG_INFO("OnRspQryNotice(请求查询客户通知响应)");
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryNotice_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询结算信息确认响应
void CFTTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pSettlementInfoConfirm)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pSettlementInfoConfirm)
		memcpy(pScrBuff, pSettlementInfoConfirm, sizeof(*pSettlementInfoConfirm));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pSettlementInfoConfirm), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pSettlementInfoConfirm)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pSettlementInfoConfirm)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQrySettlementInfoConfirm_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcSettlementInfoConfirmField SettlementInfoConfirm;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pSettlementInfoConfirm)
	{
		memset(&SettlementInfoConfirm,0,sizeof(SettlementInfoConfirm));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		SettlementInfoConfirm=*pSettlementInfoConfirm;
	}

	DataRonQrySettlementInfoConfirm tmp(SettlementInfoConfirm,RspInfo,nRequestID,bIsLast);
	
	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQrySettlementInfoConfirm_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);


	LOG_INFO("OnRspQrySettlementInfoConfirm(请求查询投资者结算结果确认响应)\n"
		"\t\t\t m_bSettlementInfoConfirmed=%d\n"
		"\t\t\t BrokerID=%s InvestorID=%s ConfirmDate=%s ConfirmTime=%s",
		pSettlementInfoConfirm,
		pSettlementInfoConfirm?pSettlementInfoConfirm->BrokerID:"-",
		pSettlementInfoConfirm?pSettlementInfoConfirm->InvestorID:"-",
		pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmDate:"-",
		pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmTime:"-");

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQrySettlementInfoConfirm_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询投资者持仓明细响应
void CFTTraderSpi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInvestorPositionCombineDetail)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInvestorPositionCombineDetail)
		memcpy(pScrBuff, pInvestorPositionCombineDetail, sizeof(*pInvestorPositionCombineDetail));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInvestorPositionCombineDetail), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pInvestorPositionCombineDetail)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pInvestorPositionCombineDetail)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryInvestorPositionCombineDetail_RON,0,nRequestID,3,nServSeq,m_nSocket);


	PlatformStru_PositionDetailComb InvestorPositionCombineDetailField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	TThostFtdcInstrumentIDType	QryInstrumentID;
	memset(&QryInstrumentID,0,sizeof(QryInstrumentID));
	if(!pInvestorPositionCombineDetail)
	{
		InvestorPositionCombineDetailField.Clear();
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		InvestorPositionCombineDetailField=PlatformStru_PositionDetailComb(*pInvestorPositionCombineDetail);
	}

	DataRonQryInvestorPositionCombDetail tmp(InvestorPositionCombineDetailField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryInvestorPositionCombineDetail_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);


	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryInvestorPositionCombineDetail(请求查询投资者组合持仓明细响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);
	}
	else
	{
		CThostFtdcInvestorPositionCombineDetailField* pDetail = pInvestorPositionCombineDetail;
		LOG_INFO("OnRspQryInvestorPositionCombineDetail(请求查询投资者组合持仓明细响应) : ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t TradingDay=[%s],\t OpenDate=[%s],\t ExchangeID=[%s],\t SettlementID=[%d],\t BrokerID=[%s], \n"
			"\t\t\t InvestorID=[%s],\t ComTradeID=[%s],\t TradeID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%c], \n"
			"\t\t\t Direction=[%c],\t TotalAmt=[%d],\t Margin=[%g],\t ExchMargin=[%g],\t MarginRateByMoney=[%g], \n"
			"\t\t\t MarginRateByVolume=[%g],\t LegID=[%d],\t LegMultiple=[%d],\t CombInstrumentID=[%s]", 
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			pDetail->TradingDay, pDetail->OpenDate, pDetail->ExchangeID, pDetail->SettlementID, pDetail->BrokerID, 
			pDetail->InvestorID, pDetail->ComTradeID, pDetail->TradeID, pDetail->InstrumentID, pDetail->HedgeFlag, 
			pDetail->Direction, pDetail->TotalAmt, pDetail->Margin, pDetail->ExchMargin, pDetail->MarginRateByMoney, 
			pDetail->MarginRateByVolume, pDetail->LegID, pDetail->LegMultiple, pDetail->CombInstrumentID
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryInvestorPositionCombineDetail_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///查询保证金监管系统经纪公司资金账户密钥响应
void CFTTraderSpi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pCFMMCTradingAccountKey)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pCFMMCTradingAccountKey)
		memcpy(pScrBuff, pCFMMCTradingAccountKey, sizeof(*pCFMMCTradingAccountKey));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pCFMMCTradingAccountKey), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pCFMMCTradingAccountKey)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pCFMMCTradingAccountKey)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryCFMMCTradingAccountKey_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcCFMMCTradingAccountKeyField KeyField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pCFMMCTradingAccountKey)
	{
		memset(&KeyField,0,sizeof(KeyField));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		KeyField=*pCFMMCTradingAccountKey;
	}

	DataRonTradingAccountKey tmp(KeyField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryCFMMCTradingAccountKey_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	if(pCFMMCTradingAccountKey)
	{
		LOG_INFO("OnRspQryCFMMCTradingAccountKey(查询保证金监管系统经纪公司资金账户密钥响应)\n\t\t\t BrokerID=%s AccountID=%s KeyID=%s CurrentKey=%s",
		pCFMMCTradingAccountKey->BrokerID,pCFMMCTradingAccountKey->AccountID,pCFMMCTradingAccountKey->KeyID,pCFMMCTradingAccountKey->CurrentKey);
	}
	else
	{
		LOG_INFO("OnRspQryCFMMCTradingAccountKey(查询保证金监管系统经纪公司资金账户密钥响应)");
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryCFMMCTradingAccountKey_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

/////请求查询仓单折抵信息响应
//void CFTTraderSpi::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}
//
/////请求查询投资者品种/跨品种保证金响应
//void CFTTraderSpi::OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}
//
/////请求查询交易所保证金率响应
//void CFTTraderSpi::OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}
//
/////请求查询交易所调整保证金率响应
//void CFTTraderSpi::OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}
//
/////请求查询汇率响应
//void CFTTraderSpi::OnRspQryExchangeRate(CThostFtdcExchangeRateField *pExchangeRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}
//
/////请求查询二级代理操作员银期权限响应
//void CFTTraderSpi::OnRspQrySecAgentACIDMap(CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询期权交易成本响应
void CFTTraderSpi::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pOptionInstrTradeCost)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pOptionInstrTradeCost)
		memcpy(pScrBuff, pOptionInstrTradeCost, sizeof(*pOptionInstrTradeCost));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pOptionInstrTradeCost), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pOptionInstrTradeCost)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pOptionInstrTradeCost)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryOptionInstrTradeCost_RON,0,nRequestID,3,nServSeq,m_nSocket);


	PlatformStru_InstrumentMarginRate InstrumentMarginRateField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pOptionInstrTradeCost)
	{
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		InstrumentMarginRateField = PlatformStru_InstrumentMarginRate(*pOptionInstrTradeCost);
	}

	DataRonQryOptionInstrTradeCost tmp(InstrumentMarginRateField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryOptionInstrTradeCost_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryOptionInstrTradeCost(请求查询期权交易成本响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, 
			tmp.RspInfoField.ErrorMsg,
			nRequestID,bIsLast
			);

	}
	else
	{
		LOG_INFO("OnRspQryOptionInstrTradeCost(请求查询期权交易成本响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\n"
			"\t\t\t FixedMargin=[%.16g],\t MiniMargin=[%.16g],\t Royalty=[%.16g],\t ExchFixedMargin=[%.16g],\t ExchMiniMargin=[%.16g]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			InstrumentMarginRateField.InstrumentID,
			InstrumentMarginRateField.BrokerID,
			InstrumentMarginRateField.InvestorID,
			InstrumentMarginRateField.HedgeFlag,
			InstrumentMarginRateField.FixedMargin,
			InstrumentMarginRateField.MiniMargin,
			InstrumentMarginRateField.Royalty,
			InstrumentMarginRateField.ExchFixedMargin,
			InstrumentMarginRateField.ExchMiniMargin
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryOptionInstrTradeCost_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询期权合约手续费响应
void CFTTraderSpi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pOptionInstrCommRate)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pOptionInstrCommRate)
		memcpy(pScrBuff, pOptionInstrCommRate, sizeof(*pOptionInstrCommRate));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pOptionInstrCommRate), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pOptionInstrCommRate)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pOptionInstrCommRate)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryOptionInstrCommRate_RON,0,nRequestID,3,nServSeq,m_nSocket);

	PlatformStru_InstrumentCommissionRate InstrumentCommissionRateField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pOptionInstrCommRate)
	{
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		InstrumentCommissionRateField = PlatformStru_InstrumentCommissionRate(*pOptionInstrCommRate);
	}

	DataRonQryOptionInstrCommRate tmp(InstrumentCommissionRateField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryOptionInstrCommRate_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryOptionInstrCommRate(请求查询期权合约手续费率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, 
			tmp.RspInfoField.ErrorMsg,
			nRequestID,bIsLast
			);

	}
	else
	{
		LOG_INFO("OnRspQryOptionInstrCommRate(请求查询期权合约手续费率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t OpenRatioByMoney=[%.16g],\n"
			"\t\t\t OpenRatioByVolume=[%.16g],\t CloseRatioByMoney=[%.16g],\t CloseRatioByVolume=[%.16g],\t CloseTodayRatioByMoney=[%.16g],\t CloseTodayRatioByVolume=[%.16g],\n"
			"\t\t\t StrikeRatioByMoney=[%.16g],\t StrikeRatioByVolume=[%.16g]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			InstrumentCommissionRateField.InstrumentID,
			InstrumentCommissionRateField.InvestorRange,
			InstrumentCommissionRateField.BrokerID,
			InstrumentCommissionRateField.InvestorID,
			InstrumentCommissionRateField.OpenRatioByMoney,
			InstrumentCommissionRateField.OpenRatioByVolume,
			InstrumentCommissionRateField.CloseRatioByMoney,
			InstrumentCommissionRateField.CloseRatioByVolume,
			InstrumentCommissionRateField.CloseTodayRatioByMoney,
			InstrumentCommissionRateField.CloseTodayRatioByVolume,
			InstrumentCommissionRateField.StrikeRatioByMoney,
			InstrumentCommissionRateField.StrikeRatioByVolume
			);
	}


    _GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryOptionInstrCommRate_RON,0,nRequestID,4,nServSeq,m_nSocket);

}

///请求查询执行宣告响应
void CFTTraderSpi::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pExecOrder)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pExecOrder)
		memcpy(pScrBuff, pExecOrder, sizeof(*pExecOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pExecOrder), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pExecOrder)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pExecOrder)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryExecOrder_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcExecOrderField ExecOrder={0};
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(pExecOrder) ExecOrder=*pExecOrder;
	else if(RspInfo.ErrorID==0)
	{
		RspInfo.ErrorID=CustomErrorID;
		strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
	}

	DataRonQryExecOrder tmp(PlatformStru_ExecOrderField(ExecOrder),RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryExecOrder_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);

	if(pExecOrder&&pRspInfo)
	{
		LOG_INFO("OnRspQryExecOrder(请求查询执行宣告响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n%s",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID,bIsLast,tmp.ExecOrderField.tostring().c_str());
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryExecOrder_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

/////请求查询询价响应
//void CFTTraderSpi::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}
//
/////请求查询报价响应
//void CFTTraderSpi::OnRspQryQuote(CThostFtdcQuoteField *pQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}

///请求查询转帐流水响应
void CFTTraderSpi::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pTransferSerial)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pTransferSerial)
		memcpy(pScrBuff, pTransferSerial, sizeof(*pTransferSerial));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pTransferSerial), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pTransferSerial)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pTransferSerial)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryTransferSerial_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcTransferSerialField TransferSerialField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pTransferSerial)
	{
		memset(&TransferSerialField,0,sizeof(TransferSerialField));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		TransferSerialField=*pTransferSerial;
	}

	DataRonQryTransferSerial tmp(TransferSerialField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryTransferSerial_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);


	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryTransferSerial(请求查询转帐流水响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		pTransferSerial=& tmp.TransferSerialField;
		LOG_INFO("OnRspQryTransferSerial(请求查询转帐流水响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t PlateSerial=[%d],\t TradeDate=[%s],\t TradingDay=[%s],\t TradeTime=[%s],\t TradeCode=[%s],\n"
			"\t\t\t SessionID=[%#x],\t BankID=[%s],\t BankBranchID=[%s],\t BankAccType=[%d],\t BankAccount=[%s],\n"
			"\t\t\t BankSerial=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\t FutureAccType=[%d],\t AccountID=[%s],\n"
			"\t\t\t InvestorID=[%s],\t FutureSerial=[%d],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\t CurrencyID=[%s],\n"
			"\t\t\t TradeAmount=[%g],\t CustFee=[%g],BrokerFee=[%g],\t AvailabilityFlag=[%d],\t OperatorCode=[%s],\n"
			"\t\t\t BankNewAccount=[%s],\t --ErrorID=[%d],\t --ErrorMsg=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			pTransferSerial->PlateSerial, pTransferSerial->TradeDate,pTransferSerial->TradingDay,pTransferSerial->TradeTime,pTransferSerial->TradeCode,
			pTransferSerial->SessionID, pTransferSerial->BankID,pTransferSerial->BankBranchID,pTransferSerial->BankAccType,pTransferSerial->BankAccount,
			pTransferSerial->BankSerial, pTransferSerial->BrokerID,pTransferSerial->BrokerBranchID,pTransferSerial->FutureAccType,pTransferSerial->AccountID,
			pTransferSerial->InvestorID, pTransferSerial->FutureSerial,pTransferSerial->IdCardType,pTransferSerial->IdentifiedCardNo,pTransferSerial->CurrencyID,
			pTransferSerial->TradeAmount,pTransferSerial->CustFee, pTransferSerial->BrokerFee,pTransferSerial->AvailabilityFlag,pTransferSerial->OperatorCode,
			pTransferSerial->BankNewAccount, pTransferSerial->ErrorID, pTransferSerial->ErrorMsg
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryTransferSerial_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询银期签约关系响应
void CFTTraderSpi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pAccountregister)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pAccountregister)
		memcpy(pScrBuff, pAccountregister, sizeof(*pAccountregister));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pAccountregister), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pAccountregister)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pAccountregister)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryAccountregister_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcAccountregisterField Accountregister;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pAccountregister)
	{
		memset(&Accountregister,0,sizeof(Accountregister));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		Accountregister=*pAccountregister;
	}

	DataRonQryAccountregister tmp(Accountregister,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryAccountregister_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);
	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryTransferSerial(请求查询银期签约关系响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		LOG_INFO("OnRspQryTransferSerial(请求查询银期签约关系响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t TradeDay=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BankAccount=[%s],\n"
			"\t\t\t BrokerID=[%s],\t BrokerBranchID=[%s],\t AccountID=[%s],\t IdCardType=[%d],\n"
			"\t\t\t IdentifiedCardNo=[%s],\t CustomerName=[%s],\t CurrencyID=[%s],\t OpenOrDestroy=[%d],\n"
			"\t\t\t RegDate=[%s],\t OutDate=[%s],\t TID=[%d],\t CustType=[%d],\n"
			"\t\t\t BankAccType=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			pAccountregister->TradeDay,pAccountregister->BankID,pAccountregister->BankBranchID,pAccountregister->BankAccount,
			pAccountregister->BrokerID,pAccountregister->BrokerBranchID,pAccountregister->AccountID,pAccountregister->IdCardType,
			pAccountregister->IdentifiedCardNo,pAccountregister->CustomerName,pAccountregister->CurrencyID,pAccountregister->OpenOrDestroy,
			pAccountregister->RegDate,pAccountregister->OutDate,pAccountregister->TID,pAccountregister->CustType,
			pAccountregister->BankAccType);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryAccountregister_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///错误应答
void CFTTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = +sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	
	if(pRspInfo)
		memcpy(pScrBuff, pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspError_RON,0,nRequestID,3,nServSeq,m_nSocket);

	DataRonError tmp(pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspError_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, nRequestID);


	LOG_INFO("CTraderSpi::OnRspError,ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
		pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, int(bIsLast));

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspError_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///报单通知
void CFTTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pOrder);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pOrder)
		memcpy(pScrBuff, pOrder, sizeof(*pOrder));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRtnOrder_RON,0,0,3,nServSeq,m_nSocket);

	PlatformStru_OrderInfo tmp(*pOrder);


	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRtnOrder_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnRtnOrder(报单通知) : \n\t\t\t "
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
		"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
		"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
		"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
		"\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
		"\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
		"\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
		"\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
		"\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
		"\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
		"\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
		pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
		pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
		pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
		pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
		pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ParticipantID,pOrder->ClientID,
		pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
		pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
		pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
		pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
		pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
		pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
		);


	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRtnOrder_RON,0,0,4,nServSeq,m_nSocket);


}

///成交通知
void CFTTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pTrade);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pTrade)
		memcpy(pScrBuff, pTrade, sizeof(*pTrade));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRtnTrade_RON,0,0,3,nServSeq,m_nSocket);


	PlatformStru_TradeInfo  tmp(*pTrade);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRtnTrade_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnRtnTrade(成交通知) : \n\t\t\t"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
		"\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
		"\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
		"\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
		"\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
		"\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d]",
		pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
		pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
		pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
		pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
		pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
		pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq,pTrade->TradeSource
		);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRtnTrade_RON,0,0,4,nServSeq,m_nSocket);


}

///报单录入错误回报
void CFTTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInputOrder)+sizeof(*pRspInfo);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInputOrder)
		memcpy(pScrBuff, pInputOrder, sizeof(*pInputOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInputOrder), pRspInfo, sizeof(*pRspInfo));
	
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnErrRtnOrderInsert_RON,0,0,3,nServSeq,m_nSocket);

	DataErrRonOrderInsert tmp(*pInputOrder,pRspInfo?(*pRspInfo):BlankRspInfo);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnErrRtnOrderInsert_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnErrRtnOrderInsert(报单录入错误回报) : ErrorID=[%d],ErrorMsg=[%s],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
		"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
		"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
		"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
		"\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
		pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
		pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
		pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
		pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
		pInputOrder->RequestID, pInputOrder->UserForceClose);


	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnErrRtnOrderInsert_RON,0,0,4,nServSeq,m_nSocket);


}

///报单操作错误回报
void CFTTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pOrderAction)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pOrderAction)
		memcpy(pScrBuff, pOrderAction, sizeof(*pOrderAction));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pOrderAction), pRspInfo, sizeof(*pRspInfo));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnErrRtnOrderAction_RON,0,0,3,nServSeq,m_nSocket);


	DataErrRonOrderAction tmp(*pOrderAction,pRspInfo?(*pRspInfo):BlankRspInfo);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnErrRtnOrderAction_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnErrRtnOrderAction(报单操作错误回报) : ErrorID=[%d],ErrorMsg=[%s],\n"
		"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
		"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
		"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t ActionDate=[%s],\t ActionTime=[%s],\t TraderID=[%s],\n"
		"\t\t\t InstallID=[%d],\t OrderLocalID=[%s],\t ActionLocalID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
		"\t\t\t BusinessUnit=[%s],\t OrderActionStatus=[%d],\t UserID=[%s],\t StatusMsg=[%s],\t InstrumentID=[%s]",
		tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,
		pOrderAction->BrokerID, pOrderAction->InvestorID, pOrderAction->OrderActionRef, pOrderAction->OrderRef,pOrderAction->RequestID,
		pOrderAction->FrontID, pOrderAction->SessionID, pOrderAction->ExchangeID, pOrderAction->OrderSysID,pOrderAction->ActionFlag,
		pOrderAction->LimitPrice, pOrderAction->VolumeChange, pOrderAction->ActionDate, pOrderAction->ActionTime,pOrderAction->TraderID,
		pOrderAction->InstallID, pOrderAction->OrderLocalID, pOrderAction->ActionLocalID, pOrderAction->ParticipantID,pOrderAction->ClientID,
		pOrderAction->BusinessUnit, pOrderAction->OrderActionStatus, pOrderAction->UserID, pOrderAction->StatusMsg,pOrderAction->InstrumentID
		);


	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnErrRtnOrderAction_RON,0,0,4,nServSeq,m_nSocket);

}

///合约交易状态通知
void CFTTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInstrumentStatus);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInstrumentStatus)
		memcpy(pScrBuff, pInstrumentStatus, sizeof(*pInstrumentStatus));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRtnInstrumentStatus_RON,0,0,3,nServSeq,m_nSocket);


	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRtnInstrumentStatus_RON,
		pInstrumentStatus ,
		sizeof(CThostFtdcInstrumentStatusField),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnRtnInstrumentStatus(合约交易状态通知):\n"
		"\t\t\t ExchangeID=[%s],\t ExchangeInstID=[%s],\t SettlementGroupID=[%s],\t InstrumentID=[%s],\t InstrumentStatus=[%d],\n"
		"\t\t\t TradingSegmentSN=[%d],\t EnterTime=[%s],\t EnterReason=[%d]",
		pInstrumentStatus->ExchangeID,
		pInstrumentStatus->ExchangeInstID,
		pInstrumentStatus->SettlementGroupID,
		pInstrumentStatus->InstrumentID,
		pInstrumentStatus->InstrumentStatus,
		pInstrumentStatus->TradingSegmentSN,
		pInstrumentStatus->EnterTime,
		pInstrumentStatus->EnterReason);

	_GetForwarding().SendPackage(pInstrumentStatus,sizeof(CThostFtdcInstrumentStatusField),FTTCMD_OnRtnInstrumentStatus_RON,0,0,4,nServSeq,m_nSocket);


}

///交易通知
void CFTTraderSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pTradingNoticeInfo);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pTradingNoticeInfo)
		memcpy(pScrBuff, pTradingNoticeInfo, sizeof(*pTradingNoticeInfo));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRtnTradingNotice_RON,0,0,3,nServSeq,m_nSocket);


	CThostFtdcTradingNoticeInfoField tmp(*pTradingNoticeInfo);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRtnTradingNotice_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnRtnTradingNotice(交易通知):\n"
		"\t\t\t BrokerID=[%s],\tInvestorID=[%s],SendTime=[%s],\n"
		"\t\t\t FieldContent=[%s],\n"
		"\t\t\t SequenceSeries=[%d],SequenceNo=[%d]",
		pTradingNoticeInfo->BrokerID,
		pTradingNoticeInfo->InvestorID,
		pTradingNoticeInfo->SendTime,
		pTradingNoticeInfo->FieldContent,
		pTradingNoticeInfo->SequenceSeries,
		pTradingNoticeInfo->SequenceNo);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRtnTradingNotice_RON,0,0,4,nServSeq,m_nSocket);


}

/////提示条件单校验错误
//void CFTTraderSpi::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) {}

///执行宣告通知
void CFTTraderSpi::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder)
{
	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pExecOrder);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pExecOrder)
		memcpy(pScrBuff, pExecOrder, sizeof(*pExecOrder));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRtnExecOrder_RON,0,0,3,nServSeq,m_nSocket);

	PlatformStru_ExecOrderField tmp(*pExecOrder);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRtnExecOrder_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);
	
	LOG_INFO("OnRtnExecOrder(执行宣告通知) ");


	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRtnExecOrder_RON,0,0,4,nServSeq,m_nSocket);


}

///执行宣告录入错误回报
void CFTTraderSpi::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, CThostFtdcRspInfoField *pRspInfo)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInputExecOrder);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInputExecOrder)
		memcpy(pScrBuff, pInputExecOrder, sizeof(*pInputExecOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInputExecOrder), pRspInfo, sizeof(*pRspInfo));

	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnErrRtnExecOrderInsert_RON,0,0,3,nServSeq,m_nSocket);


	CThostFtdcInputExecOrderField InputExecOrder={0};
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(pInputExecOrder) InputExecOrder=*pInputExecOrder;
	else if(RspInfo.ErrorID==0)
	{
		RspInfo.ErrorID=CustomErrorID;
		strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
	}

	DataErrRonExecOrderInsert tmp(PlatformStru_InputExecOrderField(InputExecOrder),RspInfo);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnErrRtnExecOrderInsert_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);
	if(pInputExecOrder&&pRspInfo)
	{
		LOG_INFO("OnErrRtnExecOrderInsert(执行宣告录入错误回报) : ErrorID=[%d],ErrorMsg=[%s]\n%s",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, tmp.InputExecOrderField.tostring().c_str());
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnErrRtnExecOrderInsert_RON,0,0,4,nServSeq,m_nSocket);


}

///执行宣告操作错误回报
void CFTTraderSpi::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField *pExecOrderAction, CThostFtdcRspInfoField *pRspInfo)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pExecOrderAction)+sizeof(*pRspInfo);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pExecOrderAction)
		memcpy(pScrBuff, pExecOrderAction, sizeof(*pExecOrderAction));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pExecOrderAction), pRspInfo, sizeof(*pRspInfo));
	
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnErrRtnExecOrderAction_RON,0,0,3,nServSeq,m_nSocket);

	CThostFtdcExecOrderActionField ExecOrderAction={0};
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(pExecOrderAction) ExecOrderAction=*pExecOrderAction;
	else if(RspInfo.ErrorID==0)
	{
		RspInfo.ErrorID=CustomErrorID;
		strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
	}

	DataErrRonExecOrderAction tmp(PlatformStru_ExecOrderActionField(ExecOrderAction),RspInfo);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnErrRtnExecOrderAction_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	if(pExecOrderAction&&pRspInfo)
	{
		LOG_INFO("OnErrRtnExecOrderAction(执行宣告操作错误回报) : ErrorID=[%d],ErrorMsg=[%s]\n%s",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, tmp.ExecOrderActionField.tostring().c_str());
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnErrRtnExecOrderAction_RON,0,0,4,nServSeq,m_nSocket);


}

///询价录入错误回报
void CFTTraderSpi::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, CThostFtdcRspInfoField *pRspInfo)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pInputForQuote);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pInputForQuote)
		memcpy(pScrBuff, pInputForQuote, sizeof(*pInputForQuote));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pInputForQuote), pRspInfo, sizeof(*pRspInfo));
	
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnErrRtnForQuoteInsert_RON,0,0,3,nServSeq,m_nSocket);

	CThostFtdcInputForQuoteField InputForQuote={0};
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(pInputForQuote) InputForQuote=*pInputForQuote;
	else if(RspInfo.ErrorID==0)
	{
		RspInfo.ErrorID=CustomErrorID;
		strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
	}

	DataErrRonForQuoteInsert tmp(PlatformStru_InputForQuoteField(InputForQuote),RspInfo);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnErrRtnForQuoteInsert_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);
	if(pInputForQuote&&pRspInfo)
	{
		LOG_INFO("OnErrRtnForQuoteInsert(询价录入错误回报) : ErrorID=[%d],ErrorMsg=[%s]\n%s",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg, tmp.InputForQuoteField.tostring().c_str());
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnErrRtnForQuoteInsert_RON,0,0,4,nServSeq,m_nSocket);


}

/////报价通知
//void CFTTraderSpi::OnRtnQuote(CThostFtdcQuoteField *pQuote) {}
//
/////报价录入错误回报
//void CFTTraderSpi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo) {}
//
/////报价操作错误回报
//void CFTTraderSpi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo) {}
//
/////询价通知
//void CFTTraderSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {}

///请求查询签约银行响应
void CFTTraderSpi::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pContractBank)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pContractBank)
		memcpy(pScrBuff, pContractBank, sizeof(*pContractBank));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pContractBank), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pContractBank)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pContractBank)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryContractBank_RON,0,nRequestID,3,nServSeq,m_nSocket);


	DataRonQryContractBank tmp(*pContractBank,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryContractBank_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnRspQryContractBank(请求查询签约银行响应):  bIsLast=%d\n"
		"\t\t\t BrokerID=[%s],BankID=[%s],BankBrchID=[%s],BankName=[%s]",
		bIsLast,
		pContractBank->BrokerID,
		pContractBank->BankID,
		pContractBank->BankBrchID,
		pContractBank->BankName);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryContractBank_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询预埋单响应
void CFTTraderSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pParkedOrder)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pParkedOrder)
		memcpy(pScrBuff, pParkedOrder, sizeof(*pParkedOrder));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pParkedOrder), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pParkedOrder)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pParkedOrder)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryParkedOrder_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcParkedOrderField ParkedOrder;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pParkedOrder)//查询结果为空时，pParkedOrder的值将为NULL
	{
		memset(&ParkedOrder,0,sizeof(ParkedOrder));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		ParkedOrder = *pParkedOrder;
	}

	DataRonQryParkedOrder tmp(ParkedOrder,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryParkedOrder_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryParkedOrder(请求查询预埋单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		LOG_INFO("OnRspQryParkedOrder(请求查询预埋单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
			"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
			"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
			"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
			"\t\t\t RequestID=[%d],\t UserForceClose=[%d],\t ExchangeID=[%s],\t ParkedOrderID=[%s],\t UserType=[%c], \n"
			"\t\t\t Status=[%c],\t ErrorID=[%d],\t ErrorMsg=[%s]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
			pParkedOrder->BrokerID,pParkedOrder->InvestorID,pParkedOrder->InstrumentID,pParkedOrder->OrderRef,pParkedOrder->UserID,
			pParkedOrder->OrderPriceType,pParkedOrder->Direction,pParkedOrder->CombOffsetFlag,pParkedOrder->CombHedgeFlag,pParkedOrder->LimitPrice,
			pParkedOrder->VolumeTotalOriginal,pParkedOrder->TimeCondition,pParkedOrder->GTDDate,pParkedOrder->VolumeCondition,pParkedOrder->MinVolume,
			pParkedOrder->ContingentCondition,pParkedOrder->StopPrice,pParkedOrder->ForceCloseReason,pParkedOrder->IsAutoSuspend,pParkedOrder->BusinessUnit,
			pParkedOrder->RequestID, pParkedOrder->UserForceClose,pParkedOrder->ExchangeID, pParkedOrder->ParkedOrderID, pParkedOrder->UserType, 
			pParkedOrder->Status, pParkedOrder->ErrorID, pParkedOrder->ErrorMsg
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryParkedOrder_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询预埋撤单响应
void CFTTraderSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pParkedOrderAction)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pParkedOrderAction)
		memcpy(pScrBuff, pParkedOrderAction, sizeof(*pParkedOrderAction));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pParkedOrderAction), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pParkedOrderAction)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pParkedOrderAction)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryParkedOrderAction_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcParkedOrderActionField ParkedOrderAction;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pParkedOrderAction)//查询结果为空时，pParkedOrderAction的值将为NULL
	{
		memset(&ParkedOrderAction,0,sizeof(ParkedOrderAction));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		ParkedOrderAction = *pParkedOrderAction;
	}

	DataRonQryParkedOrderAction tmp(ParkedOrderAction,RspInfo,nRequestID,bIsLast);


	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryParkedOrderAction_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryParkedOrderAction(请求查询预埋撤单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		LOG_INFO("OnRspQryParkedOrderAction(请求查询预埋撤单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%s],\t OrderRef=[%s],\t RequestID=[%d],\n"
			"\t\t\t FrontID=[%d],\t SessionID=[%d],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
			"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s],\t ParkedOrderActionID=[%s],\n"
			"\t\t\t UserType=[%c],\t Status=[%c],\t ErrorID=[%d],\t ErrorMsg=[%s]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
			pParkedOrderAction->BrokerID, pParkedOrderAction->InvestorID, pParkedOrderAction->OrderActionRef, pParkedOrderAction->OrderRef, pParkedOrderAction->RequestID, 
			pParkedOrderAction->FrontID, pParkedOrderAction->SessionID, pParkedOrderAction->ExchangeID, pParkedOrderAction->OrderSysID, pParkedOrderAction->ActionFlag, 
			pParkedOrderAction->LimitPrice, pParkedOrderAction->VolumeChange, pParkedOrderAction->UserID, pParkedOrderAction->InstrumentID, pParkedOrderAction->ParkedOrderActionID,
			pParkedOrderAction->UserType, pParkedOrderAction->Status, pParkedOrderAction->ErrorID, pParkedOrderAction->ErrorMsg
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryParkedOrderAction_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询交易通知响应
void CFTTraderSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pTradingNotice)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pTradingNotice)
		memcpy(pScrBuff, pTradingNotice, sizeof(*pTradingNotice));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pTradingNotice), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pTradingNotice)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pTradingNotice)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryTradingNotice_RON,0,nRequestID,3,nServSeq,m_nSocket);

	CThostFtdcTradingNoticeField TradingNoticeField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pTradingNotice)//查询结果为空时，pParkedOrderAction的值将为NULL
	{
		memset(&TradingNoticeField,0,sizeof(TradingNoticeField));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		TradingNoticeField = *pTradingNotice;
	}

	DataRonQryTradingNotice tmp(TradingNoticeField,RspInfo,nRequestID,bIsLast);


	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryTradingNotice_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQryTradingNotice(请求查询交易通知响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		LOG_INFO("OnRspQryTradingNotice(请求查询交易通知响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t UserID=[%s],\t SendTime=[%s],\t SequenceNo=[%d],\n"
			"\t\t\t  FieldContent=[%s]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
			pTradingNotice->BrokerID, pTradingNotice->InvestorID, pTradingNotice->UserID, pTradingNotice->SendTime, pTradingNotice->SequenceNo, 
			pTradingNotice->FieldContent);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryTradingNotice_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///请求查询经纪公司交易参数响应
void CFTTraderSpi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pBrokerTradingParams)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pBrokerTradingParams)
		memcpy(pScrBuff, pBrokerTradingParams, sizeof(*pBrokerTradingParams));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pBrokerTradingParams), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pBrokerTradingParams)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pBrokerTradingParams)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQryBrokerTradingParams_RON,0,nRequestID,3,nServSeq,m_nSocket);


	PlatformStru_BrokerTradingParams BrokerTradingParams(*pBrokerTradingParams);
	DataRonQryBrokerTradingParams tmp(BrokerTradingParams,pRspInfo?(*pRspInfo):BlankRspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQryBrokerTradingParams_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnRspQryBrokerTradingParams(请求查询经纪公司交易参数响应):  bIsLast=%d\n"
		"\t\t\t BrokerID=[%s],InvestorID=[%s],MarginPriceType=[%d],Algorithm=[%d],AvailIncludeCloseProfit=[%d],CurrencyID=[%s],OptionRoyaltyPriceType=[%d]"
		,bIsLast
		,pBrokerTradingParams->BrokerID
		,pBrokerTradingParams->InvestorID
		,pBrokerTradingParams->MarginPriceType
		,pBrokerTradingParams->Algorithm
		,pBrokerTradingParams->AvailIncludeCloseProfit
		,pBrokerTradingParams->CurrencyID
		,BrokerTradingParams.OptionRoyaltyPriceType
		);

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQryBrokerTradingParams_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

/////请求查询经纪公司交易算法响应
//void CFTTraderSpi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {}
//
/////银行发起银行资金转期货通知
//void CFTTraderSpi::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) {}
//
/////银行发起期货资金转银行通知
//void CFTTraderSpi::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) {}
//
/////银行发起冲正银行转期货通知
//void CFTTraderSpi::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) {}
//
/////银行发起冲正期货转银行通知
//void CFTTraderSpi::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) {}

///期货发起银行资金转期货通知
void CFTTraderSpi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pRspTransfer);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pRspTransfer)
		memcpy(pScrBuff, pRspTransfer, sizeof(*pRspTransfer));
	
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRtnFromBankToFutureByFuture_RON,0,0,3,nServSeq,m_nSocket);


	CThostFtdcRspTransferField tmp =  *pRspTransfer;

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRtnFromBankToFutureByFuture_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	LOG_INFO("OnRtnFromBankToFutureByFuture(期货发起银行资金转期货通知)");

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRtnFromBankToFutureByFuture_RON,0,0,4,nServSeq,m_nSocket);


}

///期货发起期货资金转银行通知
void CFTTraderSpi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pRspTransfer);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pRspTransfer)
		memcpy(pScrBuff, pRspTransfer, sizeof(*pRspTransfer));
	
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRtnFromFutureToBankByFuture_RON,0,0,3,nServSeq,m_nSocket);

	CThostFtdcRspTransferField tmp =  *pRspTransfer;

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRtnFromFutureToBankByFuture_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);
	LOG_INFO("OnRtnFromFutureToBankByFuture(期货发起期货资金转银行通知) ");

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRtnFromFutureToBankByFuture_RON,0,0,4,nServSeq,m_nSocket);


}

/////系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
//void CFTTraderSpi::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {}
//
/////系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
//void CFTTraderSpi::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) {}

///期货发起查询银行余额通知
void CFTTraderSpi::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) 
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pNotifyQueryAccount);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pNotifyQueryAccount)
		memcpy(pScrBuff, pNotifyQueryAccount, sizeof(*pNotifyQueryAccount));
	
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRtnQueryBankBalanceByFuture_RON,0,0,3,nServSeq,m_nSocket);


	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRtnQueryBankBalanceByFuture_RON,
		pNotifyQueryAccount ,
		sizeof(CThostFtdcNotifyQueryAccountField),
		_GetNextSeq(), 0, 0);
	LOG_INFO("OnRtnQueryBankBalanceByFuture(期货发起查询银行余额通知) ");

	_GetForwarding().SendPackage(pNotifyQueryAccount,sizeof(CThostFtdcNotifyQueryAccountField),FTTCMD_OnRtnQueryBankBalanceByFuture_RON,0,0,4,nServSeq,m_nSocket);


}

/////期货发起银行资金转期货错误回报
//void CFTTraderSpi::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {}
//
/////期货发起期货资金转银行错误回报
//void CFTTraderSpi::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) {}
//
/////系统运行时期货端手工发起冲正银行转期货错误回报
//void CFTTraderSpi::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {}
//
/////系统运行时期货端手工发起冲正期货转银行错误回报
//void CFTTraderSpi::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {}
//
/////期货发起查询银行余额错误回报
//void CFTTraderSpi::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) {}
//
/////期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
//void CFTTraderSpi::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) {}
//
/////期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
//void CFTTraderSpi::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) {}

///期货发起银行资金转期货应答
void CFTTraderSpi::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pReqTransfer)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pReqTransfer)
		memcpy(pScrBuff, pReqTransfer, sizeof(*pReqTransfer));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pReqTransfer), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pReqTransfer)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pReqTransfer)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspFromBankToFutureByFuture_RON,0,nRequestID,3,nServSeq,m_nSocket);


	CThostFtdcReqTransferField TransferField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pReqTransfer)
	{
		memset(&TransferField,0,sizeof(TransferField));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		TransferField=*pReqTransfer;
	}

	DataRonFromBankToFutureByFuture tmp(TransferField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspFromBankToFutureByFuture_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspFromBankToFutureByFuture(期货发起银行资金转期货应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		pReqTransfer=& tmp.TransferField;
		//LOG_INFO("OnRspFromBankToFutureByFuture(期货发起银行资金转期货应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		//                "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
		//                "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
		//                "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
		//                "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[******],\t AccountID=[%s],\t Password=[******],\n"
		//                "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
		//                "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
		//                "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
		//                "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
		//                "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
		//                tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		//                pReqTransfer->TradeCode, pReqTransfer->BankID,pReqTransfer->BankBranchID,pReqTransfer->BrokerID,pReqTransfer->BrokerBranchID,
		//                pReqTransfer->TradeDate, pReqTransfer->TradeTime,pReqTransfer->BankSerial,pReqTransfer->TradingDay,pReqTransfer->PlateSerial,
		//                pReqTransfer->LastFragment, pReqTransfer->SessionID,pReqTransfer->CustomerName,pReqTransfer->IdCardType,pReqTransfer->IdentifiedCardNo,
		//                pReqTransfer->CustType, pReqTransfer->BankAccount,pReqTransfer->AccountID,
		//                pReqTransfer->InstallID,pReqTransfer->FutureSerial, pReqTransfer->UserID,pReqTransfer->VerifyCertNoFlag,pReqTransfer->CurrencyID,
		//                pReqTransfer->TradeAmount,pReqTransfer->FutureFetchAmount, pReqTransfer->FeePayFlag,pReqTransfer->CustFee,pReqTransfer->BrokerFee,
		//                pReqTransfer->Message, pReqTransfer->Digest, pReqTransfer->BankAccType,pReqTransfer->DeviceID,pReqTransfer->BankSecuAccType,
		//                pReqTransfer->BrokerIDByBank, pReqTransfer->BankSecuAcc, pReqTransfer->BankPwdFlag,pReqTransfer->SecuPwdFlag,pReqTransfer->OperNo,
		//                pReqTransfer->RequestID, pReqTransfer->TID, pReqTransfer->TransferStatus
		//                );
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspFromBankToFutureByFuture_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///期货发起期货资金转银行应答
void CFTTraderSpi::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pReqTransfer)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pReqTransfer)
		memcpy(pScrBuff, pReqTransfer, sizeof(*pReqTransfer));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pReqTransfer), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pReqTransfer)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pReqTransfer)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspFromFutureToBankByFuture_RON,0,nRequestID,3,nServSeq,m_nSocket);

	CThostFtdcReqTransferField TransferField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pReqTransfer)
	{
		memset(&TransferField,0,sizeof(TransferField));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		TransferField=*pReqTransfer;
	}

	DataRonFromFutureToBankByFuture tmp(TransferField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspFromFutureToBankByFuture_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);
	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspFromFutureToBankByFuture(期货发起期货资金转银行应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		pReqTransfer=& tmp.TransferField;
		//LOG_INFO("OnRspFromFutureToBankByFuture(期货发起期货资金转银行应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
		//                "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
		//                "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
		//                "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
		//                "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[******],\t AccountID=[%s],\t Password=[******],\n"
		//                "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
		//                "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
		//                "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
		//                "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
		//                "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
		//                tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
		//                pReqTransfer->TradeCode, pReqTransfer->BankID,pReqTransfer->BankBranchID,pReqTransfer->BrokerID,pReqTransfer->BrokerBranchID,
		//                pReqTransfer->TradeDate, pReqTransfer->TradeTime,pReqTransfer->BankSerial,pReqTransfer->TradingDay,pReqTransfer->PlateSerial,
		//                pReqTransfer->LastFragment, pReqTransfer->SessionID,pReqTransfer->CustomerName,pReqTransfer->IdCardType,pReqTransfer->IdentifiedCardNo,
		//                pReqTransfer->CustType, pReqTransfer->BankAccount,pReqTransfer->AccountID,
		//                pReqTransfer->InstallID,pReqTransfer->FutureSerial, pReqTransfer->UserID,pReqTransfer->VerifyCertNoFlag,pReqTransfer->CurrencyID,
		//                pReqTransfer->TradeAmount,pReqTransfer->FutureFetchAmount, pReqTransfer->FeePayFlag,pReqTransfer->CustFee,pReqTransfer->BrokerFee,
		//                pReqTransfer->Message, pReqTransfer->Digest, pReqTransfer->BankAccType,pReqTransfer->DeviceID,pReqTransfer->BankSecuAccType,
		//                pReqTransfer->BrokerIDByBank, pReqTransfer->BankSecuAcc, pReqTransfer->BankPwdFlag,pReqTransfer->SecuPwdFlag,pReqTransfer->OperNo,
		//                pReqTransfer->RequestID, pReqTransfer->TID, pReqTransfer->TransferStatus
		//                );
	}



	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspFromFutureToBankByFuture_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

///期货发起查询银行余额应答
void CFTTraderSpi::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	int nServSeq = _GetForwarding().GetNextSeqID();
	int nSrcDataLen = sizeof(*pReqQueryAccount)+sizeof(*pRspInfo)+sizeof(int)+sizeof(bool);
	char* pScrBuff = new char[nSrcDataLen];
	std::auto_ptr<char> ptr(pScrBuff);
	memset(pScrBuff, 0, nSrcDataLen);
	if(pReqQueryAccount)
		memcpy(pScrBuff, pReqQueryAccount, sizeof(*pReqQueryAccount));
	if(pRspInfo)
		memcpy(pScrBuff+sizeof(*pReqQueryAccount), pRspInfo, sizeof(*pRspInfo));
	memcpy(pScrBuff+sizeof(*pReqQueryAccount)+sizeof(*pRspInfo), &nRequestID, sizeof(int));
	memcpy(pScrBuff+sizeof(*pReqQueryAccount)+sizeof(*pRspInfo)+sizeof(int), &bIsLast, sizeof(bool));
	_GetForwarding().SendPackage(pScrBuff,nSrcDataLen,FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON,0,nRequestID,3,nServSeq,m_nSocket);

	CThostFtdcReqQueryAccountField QueryAccountField;
	CThostFtdcRspInfoField RspInfo=pRspInfo?(*pRspInfo):BlankRspInfo;
	if(!pReqQueryAccount)
	{
		memset(&QueryAccountField,0,sizeof(QueryAccountField));
		if(RspInfo.ErrorID==0)
		{
			RspInfo.ErrorID=CustomErrorID;
			strcpy(RspInfo.ErrorMsg, CustomErrorMsg);
		}
	}
	else
	{
		QueryAccountField=*pReqQueryAccount;
	}

	DataRonQueryBankAccountMoneyByFuture tmp(QueryAccountField,RspInfo,nRequestID,bIsLast);

	CInterface_SvrTcp::getObj().SendPkgData(m_nSocket,
		FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON,
		&tmp ,
		sizeof(tmp),
		_GetNextSeq(), 0, 0);

	//log
	if(tmp.RspInfoField.ErrorID!=0)
	{
		LOG_INFO("OnRspQueryBankAccountMoneyByFuture(期货发起查询银行余额应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
			);

	}
	else
	{
		pReqQueryAccount=& tmp.QueryAccountField;
		LOG_INFO("OnRspQueryBankAccountMoneyByFuture(期货发起查询银行余额应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
			"\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
			"\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
			"\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[******],\t AccountID=[%s],\t Password=[******],\n"
			"\t\t\t FutureSerial=[%d],\t InstallID=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
			"\t\t\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],BrokerIDByBank=[%s],\n"
			"\t\t\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],RequestID=[%d],\n"
			"\t\t\t TID=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
			pReqQueryAccount->TradeCode, pReqQueryAccount->BankID,pReqQueryAccount->BankBranchID,pReqQueryAccount->BrokerID,pReqQueryAccount->BrokerBranchID,
			pReqQueryAccount->TradeDate, pReqQueryAccount->TradeTime,pReqQueryAccount->BankSerial,pReqQueryAccount->TradingDay,pReqQueryAccount->PlateSerial,
			pReqQueryAccount->LastFragment, pReqQueryAccount->SessionID,pReqQueryAccount->CustomerName,pReqQueryAccount->IdCardType,pReqQueryAccount->IdentifiedCardNo,
			pReqQueryAccount->CustType, pReqQueryAccount->BankAccount,pReqQueryAccount->AccountID,
			pReqQueryAccount->FutureSerial, pReqQueryAccount->InstallID,pReqQueryAccount->UserID,pReqQueryAccount->VerifyCertNoFlag,pReqQueryAccount->CurrencyID,
			pReqQueryAccount->Digest, pReqQueryAccount->BankAccType,pReqQueryAccount->DeviceID,pReqQueryAccount->BankSecuAccType,pReqQueryAccount->BrokerIDByBank,
			pReqQueryAccount->BankSecuAcc, pReqQueryAccount->BankPwdFlag,pReqQueryAccount->SecuPwdFlag,pReqQueryAccount->OperNo,pReqQueryAccount->RequestID,
			pReqQueryAccount->TID
			);
	}

	_GetForwarding().SendPackage(&tmp,sizeof(tmp),FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON,0,nRequestID,4,nServSeq,m_nSocket);


}

/////银行发起银期开户通知
//void CFTTraderSpi::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) {}
//
/////银行发起银期销户通知
//void CFTTraderSpi::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) {}
//
/////银行发起变更银行账号通知
//void CFTTraderSpi::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) {}