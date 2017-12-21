#pragma once

class CSendData
{
public:
	CSendData(void);
	~CSendData(void);

	static void Init(void);
	static void Release(void);
	static CSendData* Get();
	static CSendData* m_SendData;


	void AddMsg( const RecvData& data);
	bool GetMsg( RecvData& data);

	void AddClientMsg( const RecvDataMsg& data );
	bool GetClientMsg( RecvDataMsg& data );




private:
	CCmdMsgQueue m_cmdMsgQueue;
	CRITICAL_SECTION m_CS;

	CCmdClinetMsgQueue m_cmdClientMsgQueue;
	CRITICAL_SECTION m_CSClient;

	CToolThread*	m_hWorkerThread;
	static DWORD WINAPI ThreadFunc(void* pData);
	void Process();

	void SendData(RecvData& cData);
	void writeLog(RecvData& cData);
	void CalcData(RecvData& cData);
	void SendCaclData(RecvData& cData);
	void SendCountData();

	bool CmdID2String(int nCmdID, std::string& str);
	void AppendErrorString(char* str,  CThostFtdcRspInfoField* pData, char* strOutPut);


	void split(const string& s, char c, vector<string>& v);
	void ReadTodaySaveData();



	void SendClientData(RecvDataMsg& reqDataMsg);
	void SendUDPData(int nCmdID, SOCKET hSocket,Stru_IPAddress RemoteAddr,const void* pData,int DataLen);
	
};
