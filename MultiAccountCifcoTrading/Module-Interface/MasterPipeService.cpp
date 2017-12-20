// MasterPipeService.cpp: implementation of the CMasterPipeService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MasterPipeService.h"
#include "ISvr\BusinessData.hpp"
#include "..\inc\Module-Misc\FieldValueTool.h"

class IPlatformSvr;
extern IPlatformSvr* g_pPlatform;

zqWriteLog* m_pWriteLog = NULL;

DWORD g_dwPushToAppPipeSuccessSum = 0;
DWORD g_dwPushToAppPipeFailSum = 0;//8765;


CMasterPipeService*	g_pMasterPipeService = NULL;
HANDLE g_hMasterPipe = NULL;

DWORD g_dwMasterPipeThreadId = 0; 
HANDLE g_hMasterPipeThread = NULL;
DWORD g_dwPushToAppPipeThreadId = 0;
HANDLE g_hPushToAppPipeThread = NULL;

// 用于推平台应答消息的互斥量
wxMutex	g_oMutexRspPlatformQueue;

// 用于缓存平台应答消息的队列
queue<BYTE*> g_queueRspPlatform;

void LockRspPlatformQueue() {
	g_oMutexRspPlatformQueue.Lock();
};

void UnlockRspPlatformQueue() {
	g_oMutexRspPlatformQueue.Unlock();
};

void PushRspPlatformQueue(BYTE* pBuff)
{
	g_queueRspPlatform.push(pBuff);
};

int CMasterPipeService::RspCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(!g_pMasterPipeService)
		return 0;

	BYTE* pTmp=new BYTE[data.Size];
	memcpy(pTmp, &data, data.Size);
	LockRspPlatformQueue();
	PushRspPlatformQueue(pTmp);
	UnlockRspPlatformQueue();
	return 0;
}

DWORD WINAPI CMasterPipeService::Proc_PushToAppPipe(LPVOID lpvParam)
{
	
	if(g_pMasterPipeService == NULL)
		return 0xffffffff;
	
	BYTE* pBuff = NULL;
	AbstractBusinessData* pAbData = NULL;
	BOOL bIsFound = FALSE;
	DWORD dwMsgType;
	DWORD dwCommand;
	BYTE chReply[IBUFSIZE];
	
	while(1) { 
		queue<BYTE*> queueTemp;
		LockRspPlatformQueue();
		while(!g_queueRspPlatform.empty()) {
			if(g_queueRspPlatform.front()!=NULL) {
				queueTemp.push(g_queueRspPlatform.front());
			}
			g_queueRspPlatform.pop();
		};
		UnlockRspPlatformQueue();

		while(!queueTemp.empty()) {
			pBuff = queueTemp.front();
			queueTemp.pop();
			bIsFound = FALSE;

			//if(pBuff != NULL) {
				pAbData = (AbstractBusinessData*)pBuff;

				if(pAbData->BID==BID_RtnDepthMarketData 
						&& pAbData->Size==sizeof(DataRtnDepthMarketData)) {
					dwMsgType = IMSGTYPE_F;
					dwCommand = ICMD_F_rspDepthMarket;
					bIsFound = TRUE;
					//DataRtnDepthMarketData* pTmp=(DataRtnDepthMarketData*)pBuff;
				}
				else if(pAbData->BID==BID_RtnOrder 
						&& pAbData->Size==sizeof(DataRtnOrder)) { 
					dwMsgType = IMSGTYPE_H;
					dwCommand = ICMD_H_rspOrderStatus;
					bIsFound = TRUE;
					//DataRtnOrder* pTmp=(DataRtnOrder*)pBuff;
				}
				else if(pAbData->BID==BID_RtnTrade 
						&& pAbData->Size==sizeof(DataRtnTrade)) {
					dwMsgType = IMSGTYPE_H;
					dwCommand = ICMD_H_rspTrade;
					bIsFound = TRUE;
					//DataRtnTrade* pTmp=(DataRtnTrade*)pBuff;
				}
				else if(pAbData->BID==BID_RspOrderInsert
						&& pAbData->Size==sizeof(DataRspOrderInsert)) { 
					dwMsgType = IMSGTYPE_H;
					dwCommand = ICMD_H_rspNewOrderError;
					bIsFound = TRUE;
					//DataRspOrderInsert* pTmp=(DataRspOrderInsert*)pBuff;
				}
				else if(pAbData->BID==BID_RspOrderAction1 
						&& pAbData->Size==sizeof(DataRspOrderAction1)) { 
					//报单操作回执(错误回执1，综合交易平台认为撤单无效)
					dwMsgType = IMSGTYPE_H;
					dwCommand = ICMD_H_rspCancelOrderError;
					bIsFound = TRUE;
					//DataRspOrderAction1* pTmp=(DataRspOrderAction1*)pBuff;
				}
				else if(pAbData->BID==BID_RspOrderAction2 
						&& pAbData->Size==sizeof(DataRspOrderAction2)) { 
					//报单操作回执(错误回执2，交易所认为撤单无效)
					dwMsgType = IMSGTYPE_H;
					dwCommand = ICMD_H_rspCancelOrderError;
					bIsFound = TRUE;
					//DataRspOrderAction2* pTmp=(DataRspOrderAction2*)pBuff;
				}
				if(bIsFound==FALSE)
					continue;

				vector<ISessionIDType> arrOut;
				string strAppPipe;
				ISessionIDType dwCurrSessionID;
				CAppPipeManager& appMng = g_pMasterPipeService->GetAppPipeManager();
				
				appMng.LockObject();
				if(appMng.GetAllSessionID(arrOut)>0) {
					for(int i=0; i<(int)arrOut.size(); i++) {
						dwCurrSessionID = arrOut[i];
						if(!appMng.FilterMessageType(dwCurrSessionID, dwMsgType)) {
							appMng.GetSessionConnectAppPipe(dwCurrSessionID, strAppPipe);
							ZeroMemory(chReply, IBUFSIZE);
							if(MergeAndSend(strAppPipe, chReply, IBUFSIZE, 
									dwCurrSessionID, dwMsgType, dwCommand, 0, 
									(BYTE*)pAbData, pAbData->Size)) {
								g_dwPushToAppPipeSuccessSum++;
							}
							else {
								g_dwPushToAppPipeFailSum++;
							}
						}
					}
				}
				appMng.UnlockObject();
			//}
			delete[] pBuff;
		}
		Sleep(5);
	}
	
	return 1;
}

DWORD WINAPI CMasterPipeService::Proc_MasterPipe(LPVOID lpvParam) 
{ 
	
	if(g_pMasterPipeService == NULL)
		return 0xffffffff;

	DWORD cbBytesRead=0, cbReplyBytes=0, cbWritten=0; 
	BOOL fSuccess=FALSE; 
 
	DWORD dwTotalBytesAvail = 0;
	DWORD dwMode = PIPE_READMODE_BYTE; 
	BYTE* pReqBuff = NULL; 
	IPackageHeadStru tPackHead;
	IPackageHeadStru tPackHeadTemp;

	while(1) { 

		if(!ConnectNamedPipe(g_hMasterPipe, NULL)) {
			if(GetLastError() == ERROR_PIPE_CONNECTED)
				break;
			Sleep(1);
		}

		SetNamedPipeHandleState(g_hMasterPipe, &dwMode, NULL, NULL); 
		
		Sleep(5);
		dwTotalBytesAvail = 0;
		PeekNamedPipe(g_hMasterPipe, &tPackHeadTemp, sizeof(IPackageHeadStru), 
				&cbBytesRead, &dwTotalBytesAvail, NULL);
		if(tPackHeadTemp.Length!=dwTotalBytesAvail) {
			DisconnectNamedPipe(g_hMasterPipe);
			TRACE("Invalid Msg Length\n");
			USERLOG_INFO("CMasterPipeService::Proc_MasterPipe()\r\n无效的包长度，与Peek的数据长度不符");
			continue;
		}

		pReqBuff = new BYTE[dwTotalBytesAvail];
		ZeroMemory(pReqBuff, dwTotalBytesAvail);
		ReadFile(g_hMasterPipe, pReqBuff, dwTotalBytesAvail, &cbBytesRead, NULL);

		if(cbBytesRead != dwTotalBytesAvail) {
			delete[] pReqBuff;
			DisconnectNamedPipe(g_hMasterPipe);
			TRACE("Invalid Msg Read Length\n");
			USERLOG_INFO("CMasterPipeService::Proc_MasterPipe()\r\n读取的数据与包长度不符合");
			continue;
		}

		DWORD* pTail = (DWORD*)(pReqBuff+dwTotalBytesAvail-4);
		if(*pTail != ITailNum) {
			delete[] pReqBuff;
			DisconnectNamedPipe(g_hMasterPipe);
			TRACE("Invalid Msg Tail\n");
			USERLOG_INFO("CMasterPipeService::Proc_MasterPipe()\r\n无效的包尾");
			continue;
		}
		
		ZeroMemory(&tPackHead, sizeof(IPackageHeadStru));
		memcpy(&tPackHead, pReqBuff, sizeof(IPackageHeadStru));

		if(tPackHead.SyncID != ISynIDNum) {
			delete[] pReqBuff;
			DisconnectNamedPipe(g_hMasterPipe);
			USERLOG_INFO("CMasterPipeService::Proc_MasterPipe()\r\n无效的包头");
			continue;
		}
		if(tPackHead.CMDID==ICMD_A_Login && tPackHead.SessionID==0) {
			g_pMasterPipeService->ParseLogin(g_hMasterPipe, tPackHead, 
					pReqBuff+sizeof(IPackageHeadStru), 
					cbBytesRead-sizeof(IPackageHeadStru)-sizeof(PackageTailType)); 
			Sleep(20);
		}
		else {
			g_pMasterPipeService->ParseMsgType(tPackHead, 
					pReqBuff+sizeof(IPackageHeadStru), 
					cbBytesRead-sizeof(IPackageHeadStru)-sizeof(PackageTailType));
		}
		delete[] pReqBuff;

		DisconnectNamedPipe(g_hMasterPipe); 
	} 
 
	DisconnectNamedPipe(g_hMasterPipe); 

	return 1;
}

DWORD CMasterPipeService::MergePackage(BYTE* pDataBuff, DWORD dwDataBuffSize, 
									   ISessionIDType SessionID, 
									   DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum, 
									   BYTE* pAppendDataBuff, DWORD dwAppendDataLen)
{
	if(pDataBuff == NULL || dwDataBuffSize<=dwAppendDataLen)// || dwAppendDataLen>IBUFSIZE)
		return 0;
	
	DWORD dwPos = 0;
	PackageTailType tailType = ITailNum;
	IPackageHeadStru tPackHead;

	ZeroMemory(&tPackHead, sizeof(IPackageHeadStru));
	//ZeroMemory(pDataBuff, dwDataBuffSize);

	tPackHead.SyncID = ISynIDNum;
	tPackHead.SessionID = SessionID;
	tPackHead.MsgType = dwMsgType;
	tPackHead.CMDID = dwCommand;
	tPackHead.dwRspMsgNum = dwRspMsgNum;
//	if(pAppendDataBuff==NULL)
//		tPackHead.Length = sizeof(IPackageHeadStru)+sizeof(PackageTailType);
//	else
		tPackHead.Length = sizeof(IPackageHeadStru)+dwAppendDataLen+sizeof(PackageTailType);

	dwPos = 0;
	memcpy(pDataBuff+dwPos, &tPackHead, sizeof(IPackageHeadStru));
	dwPos += sizeof(IPackageHeadStru);
	if(pAppendDataBuff!=NULL) {
		memcpy(pDataBuff+dwPos, pAppendDataBuff, dwAppendDataLen);
//		dwPos += dwAppendDataLen;
	}
	// the temp
	dwPos += dwAppendDataLen;
	memcpy(pDataBuff+dwPos, &tailType, sizeof(PackageTailType));
	return tPackHead.Length;
}

BOOL CMasterPipeService::MergeAndSend(string& strAppPipe, BYTE* pDataBuff, DWORD dwDataBuffSize, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum, 
									  BYTE* pAppendDataBuff, DWORD dwAppendDataLen)
{
	
	DWORD dwPackLen = MergePackage(pDataBuff, dwDataBuffSize, SessionID, 
			dwMsgType, dwCommand, dwRspMsgNum, pAppendDataBuff, dwAppendDataLen);
	
	if(WaitNamedPipe((LPCSTR)strAppPipe.c_str(), NMPWAIT_NOWAIT)) {
		
		BOOL fSuccess = FALSE; 
		DWORD cbWritten, dwMode; 
	 
		HANDLE hPipe = CreateFile((LPCSTR)strAppPipe.c_str(), GENERIC_READ | GENERIC_WRITE, 
			 0, NULL, OPEN_EXISTING, 0, NULL);
		if(hPipe == INVALID_HANDLE_VALUE) {
			USERLOG_INFO("CMasterPipeService::MergeAndSend()\r\n连接AppPipe[%s]失败，原因码[%u]", strAppPipe.c_str(), GetLastError());
			//if(GetLastError() != ERROR_PIPE_BUSY) 
				return FALSE;
		}

		dwMode = PIPE_READMODE_BYTE; 
		SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL); 

		DWORD dwStep = 0, dwLen = 0, dwSumLen = 0;
		DWORD dwStartTime = ::GetTickCount();
//		while(dwSumLen<dwPackLen && ::GetTickCount()-dwStartTime<30) {
		while(dwSumLen<dwPackLen) {
			if(::GetTickCount()-dwStartTime>=30) {
				USERLOG_INFO("CMasterPipeService::MergeAndSend()\r\n向AppPipe[%s]发送长度[%u]数据超时，已发送长度[%u]", strAppPipe.c_str(), dwPackLen, dwSumLen);
				break;
			}
			dwStep++;
			dwLen = dwStep*0x8000 < dwPackLen ? 
					0x8000 : dwPackLen-(dwStep-1)*0x8000;
			fSuccess = WriteFile(hPipe, pDataBuff+dwSumLen, dwLen, &cbWritten, NULL); 
			dwSumLen += cbWritten;
		}
		Sleep(10);
		CloseHandle(hPipe); 
		return TRUE;
	} 
	else {
		TRACE("Send Msg to client fail\n");
		USERLOG_INFO("CMasterPipeService::MergeAndSend()\r\nAppPipe[%s]忙或不存在，发送数据失败", strAppPipe.c_str());
		return FALSE;
	}
}

BOOL CMasterPipeService::MergeAndSend_Accounts(string& strAppPipe, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum)
{
	vector<string> accountList;

	accountList.clear();
	if(g_pPlatform) g_pPlatform->GetAccountList(accountList);

	const int nAccountMaxLen = 31;
	DWORD nCount = accountList.size();
	int nLen = sizeof(IPackageHeadStru)+sizeof(DWORD)+(nAccountMaxLen+1)*nCount+sizeof(PackageTailType);
	if(nLen>IBUFMAXSIZE)
		return FALSE;
	BYTE* pBuff = new BYTE[nLen];
	ZeroMemory(pBuff, nLen);
	memcpy(pBuff+sizeof(IPackageHeadStru), &nCount, sizeof(DWORD));
	for(int i=0; i<(int)accountList.size(); i++) {
		memcpy(pBuff+sizeof(IPackageHeadStru)+sizeof(DWORD)+i*(nAccountMaxLen+1), 
				accountList[i].c_str(), accountList[i].size()>nAccountMaxLen ? 
				nAccountMaxLen:accountList[i].size());
	}
	BOOL bRet = MergeAndSend(strAppPipe, pBuff, nLen, SessionID, 
			dwMsgType, dwCommand, dwRspMsgNum, NULL, sizeof(DWORD)+(nAccountMaxLen+1)*nCount);
	delete[] pBuff;
	return bRet;
}

BOOL CMasterPipeService::MergeAndSend_ContractIDInfos(string& strAppPipe, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum)
{
	set<string> contractIDList;
	contractIDList.clear();
	if(g_pPlatform) g_pPlatform->GetInstrumentList(contractIDList);

	set<string>::iterator it;
	DWORD nCount = contractIDList.size();
	int nLen = sizeof(IPackageHeadStru)+sizeof(DWORD)+sizeof(PlatformStru_InstrumentInfo)*nCount+sizeof(PackageTailType);
	if(nLen>IBUFMAXSIZE)
		return FALSE;
	PlatformStru_InstrumentInfo outData;
	ZeroMemory(&outData, sizeof(PlatformStru_InstrumentInfo));
	BYTE* pBuff = new BYTE[nLen];
	ZeroMemory(pBuff, nLen);
	int i=0;

	memcpy(pBuff+sizeof(IPackageHeadStru), &nCount, sizeof(DWORD));
	for(it=contractIDList.begin(); it!=contractIDList.end(); it++,i++) {
		ZeroMemory(&outData, sizeof(PlatformStru_InstrumentInfo));
		if(g_pPlatform) g_pPlatform->GetInstrumentInfo(*it, outData);
		memcpy(pBuff+sizeof(IPackageHeadStru)+sizeof(DWORD)+i*sizeof(PlatformStru_InstrumentInfo), 
				&outData, sizeof(PlatformStru_InstrumentInfo));
	}
	BOOL bRet = MergeAndSend(strAppPipe, pBuff, nLen, SessionID, 
			dwMsgType, dwCommand, dwRspMsgNum, NULL, sizeof(DWORD)+sizeof(PlatformStru_InstrumentInfo)*nCount);
	delete[] pBuff;
	return bRet;
}

BOOL CMasterPipeService::MergeAndSend_AllOrders(string& strAppPipe, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum)
{
	vector<PlatformStru_OrderInfo> allOrders;
	allOrders.clear();
	if(g_pPlatform) g_pPlatform->GetAllOrders(allOrders);

	map<OrderKey, PlatformStru_OrderInfo>::iterator it;
	DWORD nCount = allOrders.size();
	int nLen = sizeof(IPackageHeadStru)+sizeof(DWORD)+sizeof(PlatformStru_OrderInfo)*nCount+sizeof(PackageTailType);
	if(nLen>IBUFMAXSIZE)
		return FALSE;
	BYTE* pBuff = new BYTE[nLen];
	ZeroMemory(pBuff, nLen);
	int i=0;
	PlatformStru_OrderInfo tInfo;
	memcpy(pBuff+sizeof(IPackageHeadStru), &nCount, sizeof(DWORD));
    for(int j=0;j<(int)allOrders.size();j++,i++)
    {
		tInfo = allOrders[j];
		memcpy(pBuff+sizeof(IPackageHeadStru)+sizeof(DWORD)+i*sizeof(PlatformStru_OrderInfo), 
				&tInfo, sizeof(PlatformStru_OrderInfo));
	}
	//{
	//	std::string strAppPath("C:\\Temp\\");
	//	char strTemp[24];
	//	ZeroMemory(strTemp, 24);
	//	sprintf_s(strTemp, 23, "%08x.Dat", ::GetTickCount());
	//	strAppPath+=strTemp;
	//	HANDLE hFile = ::CreateFile(strAppPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 
	//			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//	DWORD dwRetWrite = 0;
	//	if(hFile!=INVALID_HANDLE_VALUE) {
	//		::WriteFile(hFile, pBuff, nLen, &dwRetWrite, 0);
	//		::CloseHandle(hFile);
	//	}
	//}
	BOOL bRet = MergeAndSend(strAppPipe, pBuff, nLen, SessionID, 
			dwMsgType, dwCommand, dwRspMsgNum, NULL, sizeof(DWORD)+sizeof(PlatformStru_OrderInfo)*nCount);
	delete[] pBuff;
	return bRet;
}

BOOL CMasterPipeService::MergeAndSend_AllPositions(string& strAppPipe, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum)
{
    set<PositionKey> allKeySet;
	allKeySet.clear();
    if(g_pPlatform) g_pPlatform->GetPositionKeySet(allKeySet);

	set<PositionKey>::iterator it;
	DWORD nCount = allKeySet.size();
	int nLen = sizeof(IPackageHeadStru)+sizeof(DWORD)+sizeof(PlatformStru_Position)*nCount+sizeof(PackageTailType);
	if(nLen>IBUFMAXSIZE)
		return FALSE;
    PlatformStru_Position outData;
	BYTE* pBuff = new BYTE[nLen];
	ZeroMemory(pBuff, nLen);
	int i=0;
	memcpy(pBuff+sizeof(IPackageHeadStru), &nCount, sizeof(DWORD));
	for(it=allKeySet.begin(); it!=allKeySet.end(); it++,i++) {
		ZeroMemory(&outData, sizeof(outData));
		if(g_pPlatform) g_pPlatform->GetPositionData((*it).Account, (*it).InstrumentID, (*it).PosiDirection,(*it).HedgeFlag, outData);
		memcpy(pBuff+sizeof(IPackageHeadStru)+sizeof(DWORD)+i*sizeof(outData), 
				&outData, sizeof(outData));
	}
	BOOL bRet = MergeAndSend(strAppPipe, pBuff, nLen, SessionID, 
			dwMsgType, dwCommand, dwRspMsgNum, NULL, sizeof(DWORD)+sizeof(outData)*nCount);
	delete[] pBuff;
	return bRet;
}

BOOL CMasterPipeService::MergeAndSend_ExchangeDateTime(string& strAppPipe, string& strExchangeID, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum)
{
    string strExchDate;
	string strExchTime;
	unsigned long dwTickCount = 0;
	
    strExchDate = g_pPlatform?g_pPlatform->GetTradingDay(strExchangeID):"";
    dwTickCount = g_pPlatform?g_pPlatform->GetExchangeTime(strExchangeID, strExchTime):0;

	char cExchDataTime[32];
	int nLen = sizeof(IPackageHeadStru)+sizeof(cExchDataTime)+sizeof(PackageTailType);
	BYTE* pBuff = new BYTE[nLen];
	ZeroMemory(pBuff, nLen);

	ZeroMemory(cExchDataTime, sizeof(cExchDataTime));
	sprintf(cExchDataTime, "%s %s %ld", strExchDate.c_str(), strExchTime.c_str(), dwTickCount);
	BOOL bRet = MergeAndSend(strAppPipe, pBuff, nLen, SessionID, 
			dwMsgType, dwCommand, dwRspMsgNum, (BYTE*)cExchDataTime, sizeof(cExchDataTime));
	delete[] pBuff;
	return bRet;
}

BOOL CMasterPipeService::MergeAndSend_DepthMarket(string& strAppPipe, string& strInstrumentID, 
									  ISessionIDType SessionID, 
									  DWORD dwMsgType, DWORD dwCommand, DWORD dwRspMsgNum)
{

	PlatformStru_DepthMarketData depthMarket;
	BOOL bRet = FALSE;
    int nRetCode = g_pPlatform->GetQuotInfo(strInstrumentID.c_str(),depthMarket);
	int nLen = sizeof(IPackageHeadStru)+(nRetCode==0?sizeof(PlatformStru_DepthMarketData):0)+sizeof(PackageTailType);
	BYTE* pBuff = new BYTE[nLen];
	ZeroMemory(pBuff, nLen);
	if(nRetCode==0)
	{
		bRet = MergeAndSend(strAppPipe, pBuff, nLen, SessionID, 
				dwMsgType, dwCommand, 0, (BYTE*)&depthMarket, sizeof(PlatformStru_DepthMarketData));
	}
	else 
	{
		bRet = MergeAndSend(strAppPipe, pBuff, nLen, SessionID, 
				dwMsgType, dwCommand, 999, NULL, 0);
	}
	delete[] pBuff;
	return bRet;
}

CMasterPipeService* CMasterPipeService::GetOnlyOneInstance()
{
	if(g_pMasterPipeService == NULL) {
		g_pMasterPipeService = new CMasterPipeService();
	}
	return g_pMasterPipeService;
}

void CMasterPipeService::ReleaseOnlyOneInstance()
{
	if(g_pMasterPipeService != NULL) {
		CMasterPipeService* poTmp = g_pMasterPipeService;
		g_pMasterPipeService = NULL;
		delete poTmp;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMasterPipeService::CMasterPipeService() : 
	m_lpOwner(NULL)
{
	m_bInitOK = FALSE;
	m_bHasSubscribeResponse = FALSE;

	// 创建目录
	std::string strAppPath;
	GlobalFunc::GetPath(strAppPath);
	strAppPath+="NamedPipe\\";
	if(!wxDirExists(strAppPath.c_str())) 
		wxMkdir(strAppPath);

	if(m_pWriteLog==NULL) {

		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		char strFileName[1024];
		ZeroMemory(strFileName, 1024);

		sprintf(strFileName, "NamedPipe\\Service%04d%02d%02d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
		m_pWriteLog = new zqWriteLog(LOGMODE_LOCALFILE, strFileName);
	}
}

CMasterPipeService::~CMasterPipeService()
{
	if(g_pPlatform) g_pPlatform->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_MASTERPIPE_SERVICETHREAD);

	if(g_pPlatform) g_pPlatform->UnSubscribeBusinessData(BID_RtnTrade, GID_MASTERPIPE_SERVICETHREAD);	//订阅成交回报

	if(g_pPlatform) g_pPlatform->UnSubscribeBusinessData(BID_RspOrderInsert, GID_MASTERPIPE_SERVICETHREAD);
	if(g_pPlatform) g_pPlatform->UnSubscribeBusinessData(BID_RtnOrder, GID_MASTERPIPE_SERVICETHREAD);
	if(g_pPlatform) g_pPlatform->UnSubscribeBusinessData(BID_RspOrderAction1, GID_MASTERPIPE_SERVICETHREAD);
	if(g_pPlatform) g_pPlatform->UnSubscribeBusinessData(BID_RspOrderAction2, GID_MASTERPIPE_SERVICETHREAD);

	TerminateThread(g_hMasterPipeThread, 0);
	TerminateThread(g_hPushToAppPipeThread, 0);
	CloseHandle(g_hMasterPipe);
	g_hMasterPipe = NULL;
	if(m_pWriteLog!=NULL) {
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}
}

BOOL CMasterPipeService::Init(LPVOID lpOwner)
{

	LPTSTR lpszPipename = (LPTSTR)IMASTERPIPENAME; 
	string strPipe = lpszPipename;
	DWORD dwPID = GetCurrentProcessId();
	char strPID[64];
	ZeroMemory(strPID, sizeof(strPID));
	sprintf(strPID, "\\%x", dwPID);
	strPipe+=strPID;
  
	g_hMasterPipe = CreateNamedPipe( 
			strPipe.c_str(), PIPE_ACCESS_DUPLEX, 
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
			1, IBUFSIZE, IBUFSIZE, 3000, NULL); 
	//g_hMasterPipe = CreateNamedPipe( 
	//		lpszPipename, PIPE_ACCESS_DUPLEX, 
	//		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
	//		1, IBUFSIZE, IBUFSIZE, 3000, NULL); 
	if(g_hMasterPipe==INVALID_HANDLE_VALUE) {
		g_hMasterPipe = NULL;
//		TRACE("CreatePipe failed\n"); 
		USERLOG_INFO("CMasterPipeService::Init()\r\n初始化失败，MasterPipe[%s]创建不成功", strPipe.c_str());
		return FALSE;
	}

	g_hMasterPipeThread = CreateThread(NULL, 0, Proc_MasterPipe, 
			NULL, 0, &g_dwMasterPipeThreadId); 

	if(g_hMasterPipeThread == NULL) {
//		TRACE("CreateThread failed\n"); 
		USERLOG_INFO("CMasterPipeService::Init()\r\n初始化失败，为MasterPipe[%s]服务的线程创建不成功", strPipe.c_str());
		return FALSE;
	}
	if((g_hPushToAppPipeThread=CreateThread(NULL, 0, Proc_PushToAppPipe, NULL, 0, &g_dwPushToAppPipeThreadId))==NULL) {
//		TRACE("CreateThread failed\n"); 
		USERLOG_INFO("CMasterPipeService::Init()\r\n初始化失败，为推送数据给客户端的线程创建不成功");
		return FALSE;
	}

	//g_pMasterPipeService = this;

	m_appPipeManager.Init();

	USERLOG_INFO("CMasterPipeService::Init()\r\n初始化成功，本实例最大注册客户端上限为[%d]", IMAXSESSIOMNUM);

	m_bInitOK = TRUE;
	return m_bInitOK;
}

void CMasterPipeService::SubscribeResponse()
{
	////定制业务信息
	if(g_pPlatform) g_pPlatform->SubscribeBusinessData(BID_RtnDepthMarketData, GID_MASTERPIPE_SERVICETHREAD, CMasterPipeService::RspCallBackFunc);

	if(g_pPlatform) g_pPlatform->SubscribeBusinessData(BID_RtnTrade, GID_MASTERPIPE_SERVICETHREAD, CMasterPipeService::RspCallBackFunc);	//订阅成交回报

	if(g_pPlatform) g_pPlatform->SubscribeBusinessData(BID_RspOrderInsert, GID_MASTERPIPE_SERVICETHREAD, CMasterPipeService::RspCallBackFunc);
	if(g_pPlatform) g_pPlatform->SubscribeBusinessData(BID_RtnOrder, GID_MASTERPIPE_SERVICETHREAD, CMasterPipeService::RspCallBackFunc);
	if(g_pPlatform) g_pPlatform->SubscribeBusinessData(BID_RspOrderAction1, GID_MASTERPIPE_SERVICETHREAD, CMasterPipeService::RspCallBackFunc);
	if(g_pPlatform) g_pPlatform->SubscribeBusinessData(BID_RspOrderAction2, GID_MASTERPIPE_SERVICETHREAD, CMasterPipeService::RspCallBackFunc);
	m_bHasSubscribeResponse = TRUE;
}

// 用于处理登录的过程，解析用户名、密码等
BOOL CMasterPipeService::Login(ILOGINDATA& tLogin)
{
	return TRUE;
}

BOOL CMasterPipeService::ParseLogin(HANDLE hMasterPipe, IPackageHeadStru& tPackHead, 
									BYTE* pDataBuff, DWORD dwDataLen)
{
	
	LPTSTR lpszPipename = (LPTSTR)IAPPPIPENAMEBODY; 
	string strAppPipe;
	ISessionIDType NextSessionID;
	string strNextSessionID;
	DWORD dwRetLen = 0, cbWritten = 0;
	BOOL bRet = FALSE;
	ILOGINDATA tLogin;
	BYTE chReply[IBUFSIZE];

	ZeroMemory(&tLogin, sizeof(ILOGINDATA)); 
	ZeroMemory(&chReply, IBUFSIZE); 

	memcpy(&tLogin, pDataBuff, dwDataLen>sizeof(ILOGINDATA) ? sizeof(ILOGINDATA) : dwDataLen);
	if(Login(tLogin)) {
		
		DWORD dwSessionCount = 0;
		m_appPipeManager.LockObject();
		dwSessionCount = m_appPipeManager.GetSessionCount();
		m_appPipeManager.UnlockObject();
		if(dwSessionCount < IMAXSESSIOMNUM) {
			m_appPipeManager.GetNextSessionID(NextSessionID);
			
			char strText[1024];
			ZeroMemory(strText, 0);
			sprintf(strText, "%x", NextSessionID);
			strNextSessionID = strText;
			
			ISESSION tSession;
			ZeroMemory(&tSession, sizeof(ISESSION));

			tSession.tLoginData = tLogin;
			tSession.SessionID = NextSessionID;
			strAppPipe="\\\\";
			strAppPipe+=tLogin.strHostName;
			strAppPipe+=(char*)(lpszPipename);
			strAppPipe+=strNextSessionID;
			strncpy(tSession.strConnectAppName, strAppPipe.c_str(), 
					sizeof(tSession.strCreateAppName)-1 > strAppPipe.size() ? 
					strAppPipe.size() : sizeof(tSession.strCreateAppName)-1);
			strAppPipe="\\\\.";
			strAppPipe+=(char*)(lpszPipename);
			strAppPipe+=strNextSessionID;
			strncpy(tSession.strCreateAppName, strAppPipe.c_str(), 
					sizeof(tSession.strConnectAppName)-1 > strAppPipe.size() ? 
					strAppPipe.size() : sizeof(tSession.strConnectAppName)-1);

			tSession.dwRemainTime = IMAXREMAINTIME;

			m_appPipeManager.LockObject();
			m_appPipeManager.RegisterSession(NextSessionID, tSession);
			m_appPipeManager.UnlockObject();

			ILOGINRETURN tLoginReturn;
			ZeroMemory(&tLoginReturn, sizeof(ILOGINRETURN));
			strcpy(tLoginReturn.strMessage, IYOUWELCOME);
			strcpy(tLoginReturn.strAppPipeName, tSession.strCreateAppName);

			dwRetLen = MergePackage(chReply, IBUFSIZE, 
					NextSessionID, IMSGTYPE_A, ICMD_A_LoginReturn, 0, 
					(BYTE*)(&tLoginReturn), sizeof(ILOGINRETURN));
			bRet = WriteFile(hMasterPipe, chReply, dwRetLen, &cbWritten, NULL);
			TRACE("WriteFile to Client: %d, WriteLen %d\n", bRet, cbWritten);
			if(bRet) {
				USERLOG_INFO("CMasterPipeService::ParseLogin()\r\n客户端注册成功，Session[%x]，AppPipe[%s]，已注册客户端数[%u]", 
						NextSessionID, tLoginReturn.strAppPipeName, dwSessionCount+1);
			}
			else {
				USERLOG_INFO("CMasterPipeService::ParseLogin()\r\n客户端注册失败，原因向MasterPipe写数据失败");
			}
			return bRet;
		}
		else {
			dwRetLen = MergePackage(chReply, IBUFSIZE, 0, IMSGTYPE_A, ICMD_A_LoginReturn, 2);
			bRet = WriteFile(hMasterPipe, chReply, dwRetLen, &cbWritten, NULL);
			USERLOG_INFO("CMasterPipeService::ParseLogin()\r\n客户端注册失败，注册的客户端达到上限[%d]", IMAXSESSIOMNUM);
			return bRet;
		}
	}
	else {
		USERLOG_INFO("CMasterPipeService::ParseLogin()\r\n一个客户端登录效验失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CMasterPipeService::ParseMsgType(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen)
{
	BOOL bRet = FALSE;

	m_appPipeManager.LockObject();
	bRet = m_appPipeManager.FindSession(tPackHead.SessionID);
	m_appPipeManager.UnlockObject();
	if(!bRet)
		return FALSE;

	m_appPipeManager.LockObject();
	bRet = m_appPipeManager.FilterMessageType(tPackHead.SessionID, tPackHead.MsgType);
	m_appPipeManager.UnlockObject();
	if(bRet)
		return FALSE;

	if(tPackHead.MsgType == IMSGTYPE_A) {
		return ParseMsgType_A(tPackHead, pDataBuff, dwDataLen);
	}
	else if(tPackHead.MsgType == IMSGTYPE_C) {
		return ParseMsgType_C(tPackHead, pDataBuff, dwDataLen);
	}
	else if(tPackHead.MsgType == IMSGTYPE_E) {
		return ParseMsgType_E(tPackHead, pDataBuff, dwDataLen);
	}
	else if(tPackHead.MsgType == IMSGTYPE_G) {
		return ParseMsgType_G(tPackHead, pDataBuff, dwDataLen);
	}
	return FALSE;
}

BOOL CMasterPipeService::ParseMsgType_A(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen)
{
	string strAppPipe;
	BOOL bRet = FALSE;

	m_appPipeManager.LockObject();
	bRet = m_appPipeManager.GetSessionConnectAppPipe(tPackHead.SessionID, strAppPipe); 
	m_appPipeManager.UnlockObject();
	if(!bRet)
		return FALSE;

	DWORD dwRetLen = 0;
	BYTE chReply[IBUFSIZE];

	ZeroMemory(chReply, IBUFSIZE);
	if(tPackHead.CMDID == ICMD_A_Hello) {
		// 这里处理ICMD_A_Hello尝试握手命令
		// 并回复一个ICMD_B_IamFine愿意握手命令给第三方程序
		MergeAndSend(strAppPipe, chReply, IBUFSIZE, 
				tPackHead.SessionID, IMSGTYPE_B, ICMD_B_IamFine, 0);
		USERLOG_INFO("CMasterPipeService::ParseMsgType_A()\r\n客户端Session[%x]，发来握手命令，FastTrader返回同意握手命令IamFine", tPackHead.SessionID);
	}
	else if(tPackHead.CMDID == ICMD_A_HeartBeat) {
		// 这里处理ICMD_A_HeartBeat心跳命令
		// 并回复一个ICMD_B_rspHeartBeat心跳应答命令给第三方程序
		m_appPipeManager.LockObject();
		bRet = m_appPipeManager.HeartBeatSession(tPackHead.SessionID);
		m_appPipeManager.UnlockObject();
		if(!bRet)
			return FALSE;
		BYTE csBuff[32];
		ZeroMemory(csBuff, sizeof(csBuff));
		memcpy(csBuff, &g_dwPushToAppPipeSuccessSum, sizeof(DWORD));
		memcpy(csBuff+sizeof(DWORD), &g_dwPushToAppPipeFailSum, sizeof(DWORD));
		MergeAndSend(strAppPipe, chReply, IBUFSIZE, 
				tPackHead.SessionID, IMSGTYPE_B, ICMD_B_rspHeartBeat, 0, csBuff, 2*sizeof(DWORD));
	}
	else if(tPackHead.CMDID == ICMD_A_Logout) {
		m_appPipeManager.LockObject();
		BOOL bRet = m_appPipeManager.UnregisterSession(tPackHead.SessionID);
		if(bRet) {
			USERLOG_INFO("CMasterPipeService::ParseMsgType_A()\r\n客户端Session[%x]，发来注销命令，FastTrader成功注销客户端", tPackHead.SessionID);
		}
		else {
			USERLOG_INFO("CMasterPipeService::ParseMsgType_A()\r\n客户端Session[%x]，发来注销命令，FastTrader不能注销客户端", tPackHead.SessionID);
		}
		m_appPipeManager.UnlockObject();
		//return TRUE;
	}
	else {
		return FALSE;
	}

	return TRUE;
}

BOOL CMasterPipeService::ParseMsgType_C(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen)
{

	string strAppPipe;
	BOOL bRet = FALSE;

	m_appPipeManager.LockObject();
	bRet = m_appPipeManager.GetSessionConnectAppPipe(tPackHead.SessionID, strAppPipe); 
	m_appPipeManager.UnlockObject();
	if(!bRet)
		return FALSE;

	DWORD dwRetLen = 0;
	char chReply[IBUFSIZE];

	ZeroMemory(chReply, IBUFSIZE);
	if(tPackHead.CMDID == ICMD_C_QueryAccountInfo) {
		MergeAndSend_Accounts(strAppPipe, 
				tPackHead.SessionID, IMSGTYPE_D, ICMD_D_ReturnAccountInfo, 0);
	}
	else if(tPackHead.CMDID == ICMD_C_QueryContractIDInfo) {
		MergeAndSend_ContractIDInfos(strAppPipe, 
				tPackHead.SessionID, IMSGTYPE_D, ICMD_D_ReturnContractIDInfo, 0);
	}
	else if(tPackHead.CMDID == ICMD_C_QueryOrderInfo) {
		MergeAndSend_AllOrders(strAppPipe, 
				tPackHead.SessionID, IMSGTYPE_D, ICMD_D_ReturnOrderInfo, 0);
	}
	else if(tPackHead.CMDID == ICMD_C_QueryPositionInfo) {
		MergeAndSend_AllPositions(strAppPipe, 
				tPackHead.SessionID, IMSGTYPE_D, ICMD_D_ReturnPositionInfo, 0);
	}
	else if(tPackHead.CMDID == ICMD_C_QueryExchangeDateTime) {
		string strExchID;
		char cExchangeID[128];
		ZeroMemory(cExchangeID, sizeof(cExchangeID));
		if(dwDataLen!=32) {
			MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
					tPackHead.SessionID, IMSGTYPE_D, ICMD_D_ReturnExchangeDateTime, 999, 
					NULL, 0);
		}
		else {
			strncpy(cExchangeID, (char*)pDataBuff, 32);
			strExchID = cExchangeID;
			MergeAndSend_ExchangeDateTime(strAppPipe, strExchID, 
					tPackHead.SessionID, IMSGTYPE_D, ICMD_D_ReturnExchangeDateTime, 0);
		}
	}
	else if(tPackHead.CMDID == ICMD_C_QueryDepthMarket) {
		if(dwDataLen!=64) {
			MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
					tPackHead.SessionID, IMSGTYPE_D, ICMD_D_ReturnDepthMarket, 999, 
					NULL, 0);
		}
		else {
			string strInstrumentID;
			char cInstrumentID[128];
			ZeroMemory(cInstrumentID, sizeof(cInstrumentID));
			strncpy(cInstrumentID, (char*)pDataBuff, 64);
			strInstrumentID = cInstrumentID;
			MergeAndSend_DepthMarket(strAppPipe, strInstrumentID, 
					tPackHead.SessionID, IMSGTYPE_D, ICMD_D_ReturnDepthMarket, 0);
		}
	}
	else {
		return FALSE;
	}

	return TRUE;

}

BOOL CMasterPipeService::ParseMsgType_E(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen)
{

	string strAppPipe;
	BOOL bRet = FALSE;

	m_appPipeManager.LockObject();
	bRet = m_appPipeManager.GetSessionConnectAppPipe(tPackHead.SessionID, strAppPipe); 
	m_appPipeManager.UnlockObject();
	if(!bRet)
		return FALSE;

	DWORD dwRetLen = 0;
	char chReply[IBUFSIZE];

	ZeroMemory(chReply, IBUFSIZE);
	if(tPackHead.CMDID == ICMD_E_SubscribeDepthMarket) {
		const int nMaxContractIDLen=64;
		if(dwDataLen<=0 || dwDataLen>=nMaxContractIDLen) {
			MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
					tPackHead.SessionID, IMSGTYPE_F, ICMD_F_ReturnDepthMarket, 999);
		}
		else {
			pDataBuff[dwDataLen-1]='\0';
			string strName((char*)pDataBuff);
			if(g_pPlatform) g_pPlatform->SetSubscribeStatus(strName, S_STATUS_TEMP);
			MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
					tPackHead.SessionID, IMSGTYPE_F, ICMD_F_ReturnDepthMarket, 0);
		}
	}
	else {
		return FALSE;
	}
	return TRUE;
}

BOOL CMasterPipeService::ParseMsgType_G(IPackageHeadStru& tPackHead, BYTE* pDataBuff, DWORD dwDataLen)
{

	string strAppPipe;
	BOOL bRet = FALSE;

	m_appPipeManager.LockObject();
	bRet = m_appPipeManager.GetSessionConnectAppPipe(tPackHead.SessionID, strAppPipe); 
	m_appPipeManager.UnlockObject();
	if(!bRet)
		return FALSE;

	DWORD dwRetLen = 0;
	char chReply[IBUFSIZE];

	ZeroMemory(chReply, IBUFSIZE);
	if(tPackHead.CMDID == ICMD_G_NewOrder) {
		PlatformStru_InputOrder inputOrder;
		CThostFtdcRspInfoField rspInfo;
		ZeroMemory(&rspInfo, sizeof(CThostFtdcRspInfoField));
		if(dwDataLen!=sizeof(PlatformStru_InputOrder)) {
			rspInfo.ErrorID=999;
			strcpy(rspInfo.ErrorMsg, LOADSTRING(DATA_LENGTH_ERROR));
			MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
					tPackHead.SessionID, IMSGTYPE_H, ICMD_H_rspNewOrderApiFail, 999, 
					(BYTE*)(&rspInfo), sizeof(CThostFtdcRspInfoField));
		}
		else {
			memcpy(&inputOrder, pDataBuff, sizeof(PlatformStru_InputOrder));
            int nRet = g_pPlatform?g_pPlatform->ReqOrderInsert(inputOrder, g_pPlatform->GetReqID()):-1;
			if(nRet==0) {
				MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
						tPackHead.SessionID, IMSGTYPE_H, ICMD_H_rspNewOrderApiFail, 0, 
						(BYTE*)(&inputOrder), sizeof(PlatformStru_InputOrder));
			}
			else {
				rspInfo.ErrorID=nRet;
				strcpy(rspInfo.ErrorMsg, 
						CFieldValueTool::ReturnCode2String(nRet, 
						SVR_LANGUAGE).c_str());
				MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
						tPackHead.SessionID, IMSGTYPE_H, ICMD_H_rspNewOrderApiFail, 999, 
						(BYTE*)(&rspInfo), sizeof(CThostFtdcRspInfoField));
			}
		}
	}
	else if(tPackHead.CMDID == ICMD_G_CancelOrder) {
		PlatformStru_InputOrder inputOrder;
		CThostFtdcRspInfoField rspInfo;
		ZeroMemory(&rspInfo, sizeof(CThostFtdcRspInfoField));
		if(dwDataLen!=sizeof(PlatformStru_InputOrder)) {
			rspInfo.ErrorID=999;
			strcpy(rspInfo.ErrorMsg, LOADSTRING(DATA_LENGTH_ERROR));
			MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
					tPackHead.SessionID, IMSGTYPE_H, ICMD_H_rspCancelOrderApiFail, 999, 
					(BYTE*)(&rspInfo), sizeof(CThostFtdcRspInfoField));
		}
		else {
			memcpy(&inputOrder, pDataBuff, sizeof(PlatformStru_InputOrder));
            int nRet = g_pPlatform?g_pPlatform->ReqOrderInsert(inputOrder, g_pPlatform->GetReqID()):-1;
			if(nRet==0) {
				MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
						tPackHead.SessionID, IMSGTYPE_H, ICMD_H_rspCancelOrderApiFail, 0, 
						(BYTE*)(&inputOrder), sizeof(PlatformStru_InputOrder));
			}
			else {
				rspInfo.ErrorID=nRet;
				strcpy(rspInfo.ErrorMsg, 
						CFieldValueTool::ReturnCode2String(nRet, 
						SVR_LANGUAGE).c_str());
				MergeAndSend(strAppPipe, (BYTE*)chReply, IBUFSIZE, 
						tPackHead.SessionID, IMSGTYPE_H, ICMD_H_rspCancelOrderApiFail, 999, 
						(BYTE*)(&rspInfo), sizeof(CThostFtdcRspInfoField));
			}
		}
	}
	else {
		return FALSE;
	}
	return TRUE;

}
