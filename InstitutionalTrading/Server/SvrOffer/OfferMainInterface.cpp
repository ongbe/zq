#include "stdafx.h"
#include "CTPAccount.h"
#include "ToolThread.h"
#include "OfferMainInterface.h"
#include <io.h>

COfferMainInterface::COfferMainInterface():m_IdleSleepTime(100)
{
	CSTransferMsgQueue::Init();
	m_Thread_OrderTransfer = NULL;
	m_Thread_OrderTransfer = new CToolThread(ThreadOrderTransfer, this);
}

COfferMainInterface::~COfferMainInterface()
{
	m_db.close();
	SAFE_DELETE(m_Thread_OrderTransfer);
}



void COfferMainInterface::QueryQuot(std::map<std::string, int>& mapInstrumentID)
{
	if(mapInstrumentID.size() == 0)
		return;
	if(CTCPDataService::GetInstance()->GetNetStatus() != enmNetConnectSuccess)
	{
	//	OFFER_INFO("行情服务器没有连接成功\n");		
	//	PUSH_LOG_ERROR(FirstLevelError,true,true,"行情服务器没有连接成功\n");		
		return;
	}	
	Sleep(5000);
	std::map<std::string, int>::iterator it = mapInstrumentID.begin();
	for(;it != mapInstrumentID.end(); it++)
	{
		std::string strInstrument = it->first;
		CTCPDataService::GetInstance()->SendData(Cmd_SubscribeQuotEvent_Req, (void *)strInstrument.c_str(), strlen(strInstrument.c_str())+1, 0);
		OFFER_INFO("报盘订阅行情 Instrument===%s\n", strInstrument.c_str());
		//PUSH_LOG_ERROR(Logger,false,true,"报盘订阅行情 Instrument===%s\n", strInstrument.c_str());		
	}
	CTCPDataService::GetInstance()->AddQutoInstruments(mapInstrumentID);
}
void COfferMainInterface::SetOrderTransferByAccount(std::vector<SOrderTransfer>& vecOrderTransfer)
{
	for(int i=0; i< (int)vecOrderTransfer.size(); i++)
	{
		SOrderTransfer sTransfer = vecOrderTransfer[i];
		m_mutex_mapAccountID2OrderTransfer.write_lock();
		BrokerAccountKey BAKey(sTransfer.szBrokerID, sTransfer.szAccountID);
		m_mapAccountID2OrderTransfer.insert(make_pair(BAKey, sTransfer));
		m_mutex_mapAccountID2OrderTransfer.write_unlock();
	}
}

void COfferMainInterface::InsertOrderRef(BrokerAccountKey& BAKey, CCTPAccount*& pCtpAccount)
{
	int nCTPOrderMax = 0;
	int nCount = m_mapAccountID2OrderTransfer.count(BAKey);
	std::multimap<BrokerAccountKey, SOrderTransfer>::iterator it = m_mapAccountID2OrderTransfer.find(BAKey);
	for(int i = 0; i< nCount; i++)
	{
		SOrderTransfer sData = it->second;
		STransfer keyClient, keyCtp;
		keyCtp.key.nFrontID		= sData.nCTPFrontID;
		keyCtp.key.nSessionID	= sData.nCTPSessionID;
		sprintf(keyCtp.key.szOrderRef, "%d", sData.nCTPOrderRef);
		keyCtp.strBrokerID = sData.szBrokerID;//经纪公司ID
		keyCtp.strID = sData.szAccountID;	  //交易账户ID	
		keyCtp.ForceCloseReason = '0';
		keyCtp.UserForceClose   = 0;

		keyClient.key.nFrontID		= sData.nFrontID;
		keyClient.key.nSessionID	= sData.nSessionID;
		sprintf(keyClient.key.szOrderRef, "%d", sData.nOrderRef);
		keyClient.strBrokerID.empty();
		keyClient.strID = sData.szInvestorID; //交易员ID	
		keyClient.ForceCloseReason = sData.ForceCloseReason;
		keyClient.UserForceClose   = sData.UserForceClose;

		pCtpAccount->InsertCTP2CleintKeyTransfer(keyCtp, keyClient);

		keyClient.ForceCloseReason = '0';
		keyClient.UserForceClose   = 0;//撤单的时候，最好强平单的标识别取消
		pCtpAccount->InsertCleint2CTPKeyTransfer(keyClient, keyCtp);


		STransfer ClientOrderRef;
		ClientOrderRef.strBrokerID.empty();
		ClientOrderRef.strID = sData.szInvestorID;
		ClientOrderRef.key.nFrontID = sData.nFrontID;
		ClientOrderRef.key.nSessionID = sData.nSessionID;
		sprintf(ClientOrderRef.key.szOrderRef, "%d", sData.nOrderRef);		

		SOrderRef CtpOrderRef; 
		CtpOrderRef.strBrokerID = sData.szBrokerID;
		CtpOrderRef.strID = sData.szAccountID;
		CtpOrderRef.nOrderRef = sData.nCTPOrderRef;

		pCtpAccount->InsertCtp2ClientOrderRef(CtpOrderRef, ClientOrderRef);		
		if(strcmp(sData.orderSysID, "")!=0)
		{
			SOrderSysIDRef orderSysIDRef;
			orderSysIDRef.strExchanggeID = sData.ExchangeID;
			orderSysIDRef.strOrderSysID  = sData.orderSysID;
			pCtpAccount->Insertordersysid2ClientOrderRef(orderSysIDRef, ClientOrderRef);
		}

		if(sData.nCTPOrderRef > nCTPOrderMax)//服务器重启后pCtpAccount 的ClientOrderRef要从最大的开始
			nCTPOrderMax = sData.nCTPOrderRef;

		it++;
	}
	pCtpAccount->SetCurrentOrderref(++nCTPOrderMax);	
}

DWORD WINAPI COfferMainInterface::ThreadOrderTransfer(PVOID pParam)
{
	COfferMain* pMgr=(COfferMain*)pParam;
	pMgr->doOrderTransfer(pMgr);
	return 0;
}
void COfferMainInterface::doOrderTransfer(COfferMainInterface* pMgr)
{	
	Sleep(1000);//可能线程比构造函数先完成m_Thread_OrderTransfer指针无效
	std::vector<STransferMsg> sTransferMsgVec;
	while (m_Thread_OrderTransfer && !m_Thread_OrderTransfer->IsNeedExit())
	{				
		if(CSTransferMsgQueue::Get()->GetMsgVector(sTransferMsgVec))
		{
			OFFER_INFO("Sqllite存储[%d]\n", sTransferMsgVec.size());
			//PUSH_LOG_ERROR(Logger,false,true,"Sqllite存储[%d]\n", sTransferMsgVec.size());
			m_db.execDML("begin transaction;");
			for(int i =0; i< (int)sTransferMsgVec.size(); i++)
			{
				STransferMsg& sTransferMsg = sTransferMsgVec[i];
				if(sTransferMsg.nProcess == 0)
				{//update
					pMgr->UpdateOrderTransfer(sTransferMsg.sOrderTransfer);
				}
				else if(sTransferMsg.nProcess == 1)
				{//add				
					int nError =0;
					pMgr->SaveOrderTransfer(sTransferMsg.sOrderTransfer, nError);
				}
				else
				{
					OFFER_INFO("Sqllite存储线程nProcess =[%d]\n", sTransferMsg.nProcess);
					//PUSH_LOG_ERROR(FirstLevelError,true,true,"Sqllite存储线程nProcess =[%d]\n", sTransferMsg.nProcess);
				}
			}
			m_db.execDML("commit transaction;");	
			sTransferMsgVec.clear();		
		}
		else
		{
			Sleep(m_IdleSleepTime);
			//OFFER_INFO("Sqllite sleep 100 \n");
		}
	}
	OFFER_INFO("Sqllite存储线程退出。\n");
	//PUSH_LOG_ERROR(Logger,false,true,"Sqllite存储线程退出。\n");
}

void COfferMainInterface::InitDBData()
{
	char strTime[64];
	memset(&strTime, 0, sizeof(strTime));
	SYSTEMTIME st;
	GetLocalTime(&st);	
	sprintf(strTime,"%4d-%02d-%02d.db",st.wYear, st.wMonth, st.wDay);	

	std::string strFileFolder = "offer";
	std::string strFileName   = strTime;
	GetFolderFileName(strFileName, strFileFolder);
	try
	{
		if(access(strFileName.c_str(), 0)==-1)
		{
			m_db.open(strFileName.c_str());	
			std::string strSql = "create table offer_orderreftransfer(accountid varchar(16), \
								 frontid int, sessionid int, orderref int, ctp_orderref int, ctp_frontid int, ctp_sessionid int, Inserttime datetime,\
								 investorid varchar(16), brokerid varchar(16), ordersysid varchar(21), exchangeid varchar(9), userforceclose int, forceclosereason varchar(1));";
			m_db.execDML(strSql.c_str());

			std::string strSql2 =  "CREATE INDEX [oeder_index] ON [offer_orderreftransfer](\
								   [orderref]  DESC,	[frontid]  DESC,	[sessionid]  DESC,	[accountid]  DESC)";
			m_db.execDML(strSql2.c_str());
		}
		else
			m_db.open(strFileName.c_str());	

	}
	catch (CppSQLite3Exception& e)
	{
		OFFER_INFO("Sqllit打开数据库失败！%s \n", e.errorMessage());	
		PUSH_LOG_ERROR(FirstLevelError,true,true,"Sqllit打开数据库失败！\n");	
	}	
}
void COfferMainInterface::GetFolderFileName(std::string& strFilePath,const std::string& folder)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileNameA( NULL, szLocalPath, 256 );
	std::string strSystemPath( szLocalPath );
	int nPos = strSystemPath.rfind( '\\' );
	if ( -1 != nPos )
	{
		strSystemPath = strSystemPath.substr( 0, nPos + 1 );
		if(!folder.empty())
		{
			if(std::string::npos==folder.rfind("\\"))
				strSystemPath+=folder+"\\";
			else
				strSystemPath+=folder;
			CreateDirectoryA(strSystemPath.c_str(),NULL);
		}
		strFilePath = strSystemPath + strFilePath;
	}	
}
bool COfferMainInterface::GetOrderTransfer(std::vector<SOrderTransfer>& vecOrderTransfer)
{
	try
	{
		std::string strSql = "select * from offer_orderreftransfer;";

		CppSQLite3Table t = m_db.getTable(strSql.c_str());
		for (int row = 0; row < t.numRows(); row++)
		{
			t.setRow(row);
			SOrderTransfer sOrderTransfer;
			const char *value = NULL;
			if (!t.fieldIsNull(0))
			{
				value = t.fieldValue(0);
				strcpy(sOrderTransfer.szAccountID, value);
			}
			if (!t.fieldIsNull(1))
			{
				value = t.fieldValue(1);
				sOrderTransfer.nFrontID = atoi(value);
			}
			if (!t.fieldIsNull(2))
			{
				value = t.fieldValue(2);
				sOrderTransfer.nSessionID = atoi(value);
			}
			if (!t.fieldIsNull(3))
			{
				value = t.fieldValue(3);
				sOrderTransfer.nOrderRef = atoi(value);
			}
			if (!t.fieldIsNull(4))
			{
				value = t.fieldValue(4);
				sOrderTransfer.nCTPOrderRef = atoi(value);
			}
			if (!t.fieldIsNull(5))
			{
				value = t.fieldValue(5);
				sOrderTransfer.nCTPFrontID = atoi(value);
			}
			if (!t.fieldIsNull(6))
			{
				value = t.fieldValue(6);
				sOrderTransfer.nCTPSessionID = atoi(value);
			}
			if (!t.fieldIsNull(7))
			{
				value = t.fieldValue(7);
				strcpy(sOrderTransfer.szUpdateDate, value);
			}
			if (!t.fieldIsNull(8))
			{
				value = t.fieldValue(8);
				strcpy(sOrderTransfer.szInvestorID, value);
			}
			if (!t.fieldIsNull(9))
			{
				value = t.fieldValue(9);
				strcpy(sOrderTransfer.szBrokerID, value);
			}
			if (!t.fieldIsNull(10))
			{
				value = t.fieldValue(10);
				strcpy(sOrderTransfer.orderSysID, value);
			}
			if (!t.fieldIsNull(11))
			{
				value = t.fieldValue(11);
				strcpy(sOrderTransfer.ExchangeID, value);
			}
			if (!t.fieldIsNull(12))
			{
				value = t.fieldValue(12);
				sOrderTransfer.UserForceClose = atoi(value);
			}
			if (!t.fieldIsNull(13))
			{
				value = t.fieldValue(13);
				if(strlen(value) > 0)
					sOrderTransfer.ForceCloseReason = value[0];
			}

			vecOrderTransfer.push_back(sOrderTransfer);
		}	
	}
	catch (CppSQLite3Exception& e)
	{		
		OFFER_INFO("Sqllite读取报单失败%s \n", e.errorMessage());
		//PUSH_LOG_ERROR(Logger,false,true,"Sqllite读取报单失败\n");
		return false;
	}

	return true;
}
bool COfferMainInterface::SaveOrderTransfer(SOrderTransfer& order, int& nErrorCode)
{
	try
	{
		char strSql[MAX_SQL_LENGTH];
		memset(strSql, 0, sizeof(strSql));
		sprintf(strSql, "insert into OFFER_ORDERREFTRANSFER(accountid , \
						frontid, sessionid, orderref, ctp_orderref, ctp_frontid, ctp_sessionid, Inserttime,\
						investorid, brokerid, ordersysid, exchangeid, userforceclose, forceclosereason) \
						values('%s', %d, %d, %d, %d, %d, %d, datetime('now', 'localtime'), '%s', '%s', '%s', '%s', %d, '%c');",\
						order.szAccountID, order.nFrontID, order.nSessionID, order.nOrderRef, order.nCTPOrderRef,
						order.nCTPFrontID, order.nCTPSessionID, order.szInvestorID, order.szBrokerID, order.orderSysID, order.ExchangeID, order.UserForceClose, order.ForceCloseReason);

		m_db.execDML(strSql);		
	}
	catch (CppSQLite3Exception& e)
	{
		OFFER_INFO("Sqllite存储报单转化失败 %s\n, accountID=[%s], frontid = [%d], sessionid =[%d], orderref =[%d], ctp_orderref = [%d], ctp_frontid = [%d], ctp_sessionid = [%d],\
				   investorid =[%s], brokerid=[%s], ordersysid=[%s], exchangeid=[%s], userforceclose=[%d], forceclosereason=[%c] \n",e.errorMessage(), order.szAccountID, order.nFrontID, order.nSessionID, order.nOrderRef, order.nCTPOrderRef,
				   order.nCTPFrontID, order.nCTPSessionID, order.szInvestorID, order.szBrokerID, order.orderSysID, order.ExchangeID, order.UserForceClose, order.ForceCloseReason);

		/*	PUSH_LOG_ERROR(FirstLevelError,true,true,"Sqllite存储报单转化失败\n, accountID=[%s], frontid = [%d], sessionid =[%d], orderref =[%d], ctp_orderref = [%d], ctp_frontid = [%d], ctp_sessionid = [%d],\
		investorid =[%s], brokerid=[%s], ordersysid=[%s], exchangeid=[%s], userforceclose=[%d], forceclosereason=[%c] \n",order.szAccountID, order.nFrontID, order.nSessionID, order.nOrderRef, order.nCTPOrderRef,
		order.nCTPFrontID, order.nCTPSessionID, order.szInvestorID, order.szBrokerID, order.orderSysID, order.ExchangeID, order.UserForceClose, order.ForceCloseReason);
		*/	return false;
	}
	return true;
}
bool COfferMainInterface::UpdateOrderTransfer(SOrderTransfer& sOrderTransfer)
{
	try
	{
		char strSql[MAX_SQL_LENGTH];
		memset(strSql, 0, sizeof(strSql));
		sprintf(strSql, "update OFFER_ORDERREFTRANSFER  set ordersysid ='%s' , EXCHANGEID = '%s' where orderref = %d \
						and accountid = '%s' and frontid = %d and sessionid = %d",
						sOrderTransfer.orderSysID, sOrderTransfer.ExchangeID, sOrderTransfer.nOrderRef, 
						sOrderTransfer.szAccountID, sOrderTransfer.nFrontID,
						sOrderTransfer.nSessionID);
		m_db.execDML(strSql);		
	}
	catch (CppSQLite3Exception& e)
	{
		OFFER_INFO("Sqllite更新报单转化失败 %s\n, accountID=[%s], frontid = [%d], sessionid =[%d], orderref =[%d], ctp_orderref = [%d], ctp_frontid = [%d], ctp_sessionid = [%d],\
				   investorid =[%s], brokerid=[%s], ordersysid=[%s], exchangeid=[%s], userforceclose=[%d], forceclosereason=[%c] \n",e.errorMessage(), sOrderTransfer.szAccountID, sOrderTransfer.nFrontID, sOrderTransfer.nSessionID, sOrderTransfer.nOrderRef, sOrderTransfer.nCTPOrderRef,
				   sOrderTransfer.nCTPFrontID, sOrderTransfer.nCTPSessionID, sOrderTransfer.szInvestorID, sOrderTransfer.szBrokerID, sOrderTransfer.orderSysID, sOrderTransfer.ExchangeID, sOrderTransfer.UserForceClose, sOrderTransfer.ForceCloseReason);

		/*PUSH_LOG_ERROR(FirstLevelError,true,true,"Sqllite更新报单转化失败\n, accountID=[%s], frontid = [%d], sessionid =[%d], orderref =[%d], ctp_orderref = [%d], ctp_frontid = [%d], ctp_sessionid = [%d],\
		investorid =[%s], brokerid=[%s], ordersysid=[%s], exchangeid=[%s], userforceclose=[%d], forceclosereason=[%c] \n",sOrderTransfer.szAccountID, sOrderTransfer.nFrontID, sOrderTransfer.nSessionID, sOrderTransfer.nOrderRef, sOrderTransfer.nCTPOrderRef,
		sOrderTransfer.nCTPFrontID, sOrderTransfer.nCTPSessionID, sOrderTransfer.szInvestorID, sOrderTransfer.szBrokerID, sOrderTransfer.orderSysID, sOrderTransfer.ExchangeID, sOrderTransfer.UserForceClose, sOrderTransfer.ForceCloseReason);*/

		return false;
	}
	return true;
}
bool COfferMainInterface::GetTradeAccountByUserID(int nTraderID, std::string& strBrokerID, std::string& strInvestID)
{//根据交易员账号得到委托交易账号
	int nUserID = nTraderID;
	std::vector<UserAndTradeAccountRelation> vec;
	CInterface_SvrUserOrg::getObj().GetUserAndTradeAccountRelationByUserID(nUserID, vec);
	if(vec.size() == 0)
		return false;

	TradeAccount account;
	int nAccountID = vec[0].nTradeAccountID;
	if(!CInterface_SvrBrokerInfo::getObj().GetTradeAccount(nAccountID, account ))
		return false;	

	strInvestID = account.szTradeAccount;

	BrokerInfo brokerInfo;
	if(!CInterface_SvrBrokerInfo::getObj().GetBrokerInfoByID(account.nBrokerID, brokerInfo))
		return false;
	strBrokerID = brokerInfo.szBrokerCode;
	return true;
}