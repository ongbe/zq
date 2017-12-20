#pragma once

#include <vector>
#include <map>
#include <set>

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

	void ProcessRequest( RequestData& reqData);

	void ProcessLogonRequest( RequestData& reqData );
	void ProcessAccountRequest( RequestData& reqData );

	void ProcessServerRequest( RequestData& reqData );
	void ProcessBrokerRequest( RequestData& reqData );
	void ProcessTradeAccountRequest( RequestData& reqData );

	void RspResultData( SOCKET hSocket, int nOperateRet, int nCmdID, 
		void* pData, int nLen, int nSeq = 0, 
		int nCurPkgIndex = 0, int nTotalPkg = 1 );

	template<typename T>
	void RspResultDataAutoDividePkg( SOCKET hSocket, int nOperateRet, int nCmdID, 
		std::vector<T>& vData, int nSeq = 0 );

	void OutputUnkownReqLog( int nModuleID, int nCmdID, int nLen );

	bool IsLogon(SOCKET hSocket);

private:
	CWriteLog* m_pWriteLog;
	HANDLE	m_hWorkerThread;
	bool m_bExit;
};
