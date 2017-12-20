#pragma once

#include <vector>
#include <map>
#include <set>
#include "DBAccess.h"

class CWriteLog;
struct RequestData;
struct Stru_UniPkgHead;

class Worker
{
public:
	Worker(const char* pUser, const char* pPwd, const char* pSID );
	~Worker(void);

	static DWORD ThreadFunc(void* pData);

private:
	void OnIdle();

	void ProcessRequest( RequestData& reqData);

	void ProcessLogonRequest( RequestData& reqData );
	void ProcessAccountRequest( RequestData& reqData );

	bool DelAccount(int nAccountID, bool& bCaseCadeDeleted);
	bool DelAccountCaseCade( int nAccountID );

	void ProcessIDCardRequest( RequestData& reqData );
	void ProcessServerRequest( RequestData& reqData );
	void ProcessBrokerRequest( RequestData& reqData );
	void ProcessTradeAccountRequest( RequestData& reqData );
	void ProcessPermissionModuleRequest( RequestData& reqData );
	void ProcessPermissionRequest( RequestData& reqData );
	void ProcessMessageRequest( RequestData& reqData );

	void SendMessage( std::vector<TargetAccount> &vForeAccount, MessageInfo msgInfo );

	void RspResultData( SOCKET hSocket, int nOperateRet, int nCmdID, 
		void* pData, int nLen, int nSeq = 0, 
		int nCurPkgIndex = 0, int nTotalPkg = 1 );

	template<typename T>
	void RspResultDataAutoDividePkg( SOCKET hSocket, int nOperateRet, int nCmdID, 
		std::vector<T>& vData, int nSeq = 0 );

	void OutputUnkownReqLog( int nModuleID, int nCmdID, int nLen );

	void FilterAccount( int nCurAccountID, std::vector<AccountInfoEx>& vAccount, 
		std::vector<AccountInfoEx>& vOut);

	void CalcShowData(std::map<int, std::map<int, AccountInfoEx>>& mapAccountTree,
		int nOwnerID, int nRootViewPermission2, int nParentViewPermission1,
		std::vector<AccountInfoEx>& vOutData);

	void GetDelAccounts(int nCurAccountID, std::vector<AccountInfo>& vAccount, 
		std::set<int>& setOut);

	void GetChildAccounts(int nCurAccountID, 
		std::map<int, std::map<int, AccountInfo>>& mapAccountTree,
		std::set<int>& setOut);

	int GetOnlineStatus(int nAccountID);
	bool IsLogon(SOCKET hSocket);
	void ForceCloseClient(int nAccountID);

	void DisableExpiredAccounts();
	void DeleteExpiredWaitForSendAccounts();

private:
	CWriteLog* m_pWriteLog;
	HANDLE	m_hWorkerThread;
	bool m_bExit;
	DBAccess m_dbAccess;

	SYSTEMTIME m_backSysTime;
	bool m_bFirstIdle;
};
