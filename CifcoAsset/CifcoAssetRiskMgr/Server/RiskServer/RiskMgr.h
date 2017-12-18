#pragma once

#include "DataBase.h"
#include "MsgQueue.h"
#include "../CloseOrder/ThostFtdcUserApiStruct.h"
#include "FtdcRiskUserApiStruct.h"
class UserApi;
class CToolThread;
class CRiskMsgCalc;
class CRiskMgr:CDataBase
{
public:
	static void Init(void);
	static void Release(void);
	static DWORD WINAPI JoinThreadProc (PVOID pParam);

	static DWORD WINAPI RiskThreadProcMain (PVOID pParam);
	void doProcessBusinessDataMain();
	static DWORD WINAPI RiskThreadProcQuot (PVOID pParam);
	void doProcessBusinessDataQuot();
	static DWORD WINAPI RiskThreadProcSync (PVOID pParam);
	void doProcessBusinessDataSync();
	static DWORD WINAPI RiskThreadProcStatistics (PVOID pParam);
	void doProcessBusinessDataStatistics();
	static DWORD WINAPI RiskThreadProcClient (PVOID pParam);	
	void doProcessBusinessDataClient();
	static DWORD WINAPI RiskThreadProcForceClose (PVOID pParam);	
	void doProcessBusinessDataForceClose();
	int GetNextID();
	UserApi* GetApi();	
	void UpdateAllRiskEvent();
protected:
	UserApi* m_Api;
	CRiskMsgCalc*		  m_RiskMsgCalc;
	CToolThread*		  m_RiskMgrThread;
	CToolThread*		  m_RiskMgrThread_Quot;
	CToolThread*		  m_RiskMgrThread_Sync;
	CToolThread*		  m_RiskMgrThread_Stat;
	CToolThread*		  m_RiskMgrThread_AutoForceClose;
	CToolThread*		  m_Client2RiskThread;
	CToolThread*		  m_JoinThread;
	const int			  m_IdleSleepTime;
	int					  m_CtpNextIndex;
	CReadWriteLock		  m_Mutex_Hash;	
private:
	static CRiskMgr* m_pRiskMgr;
private:
	CRiskMgr(void);
	virtual ~CRiskMgr(void);
	DISALLOW_COPY_AND_ASSIGN(CRiskMgr);

private:
	//用未成交单生成撤单
	void GenerateOrderAction(const std::vector<SequencialOrderField>& vOrder, 
		std::vector<CThostFtdcInputOrderActionField>& vOrderAction);
	//用持仓生成平仓下单结构
	void GenerateInsertOrder(const std::vector<SequencialPositionField>& vPosition, 
		std::vector<CThostFtdcInputOrderField>& vInsertOrder);

	bool NewCloseOrder( const SequencialPositionField &position, CThostFtdcInputOrderField &inputOrder, 
		char Direction, int nCloseVolumn, char cCloseFlag );

	//将下单结构转化为ctp下单结构
	void Covent2CtpOrder(CShfeFtdcRiskForceCloseOrderField forceCloseOrder,
		CThostFtdcInputOrderField& ctpOrderField);
};
