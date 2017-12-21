#pragma once
#include "SocketBase.h"
#include "ReadWriteLock.h"
#include <map>

class CDataManager
{
public:
	CDataManager(void);
	~CDataManager(void);
	static void Init(void);
	static void Release(void);
	static CDataManager* Get();
	static CDataManager* m_DataManager;
private:
	std::map<SClientKey, int>			m_mapClientUnConnectTime;
	CReadWriteLock						m_Mutex_mapClientUnConnectTime;

	//所有的报单信息
	std::vector<ServerOrder>			m_vecServerOrder;
	CReadWriteLock						m_Mutex_ServerOrder;

	//对所有报单统计后的信息(每秒)
	std::map<DWORD, StatisticsData>		m_mapStatisticsData;
	CReadWriteLock						m_Mutex_StatisticsData;
	//对所有报单统计后的信息(每分)
	std::map<DWORD, StatisticsData>		m_mapStatisticsData_Min;
	CReadWriteLock						m_Mutex_StatisticsData_Min;
	

	std::map<int, ServerOrder>			m_mapSeq2ServerOrder;
	CReadWriteLock						m_Mutex_mapSeq2ServerOrder;

   //针对所有的统计信息
	StatisticsData						m_mapStatistics;
	CReadWriteLock						m_Mutex_Statistics;

public:
	bool GetClinetUnConnectTime(Stru_IPAddress RemoteAddr, SOCKET hSocket, int& nTime);
	void SetClinetUnConnectTime(Stru_IPAddress RemoteAddr, SOCKET hSocket, int& nTime);
	void ChangeAllUnConnectTime();
	void GetAllUnconnetTime(std::map<SClientKey, int>& mapClientUnConnectTime);
	

	void AppendServerOrder(ServerOrder& order); 

	void AddStatisticsData(ServerOrder& order);
	bool GetStatisticsData(DWORD& dwTime, StatisticsData& data);
	void GetAllStatics(std::vector<StatisticsData>& vecData);//


	void AddStatisticsData_Min(ServerOrder& order);
	bool GetStatisticsData_Min(DWORD& dwTime, StatisticsData& data);
	void GetAllStatics_Min(std::vector<StatisticsData>& vecData);//画线用


	void GetCountStatics(StatisticsData& data);				 //版面显示用



	void CalcDelay(ServerOrder& order);
	void CalcCountDelay(ServerOrder& order, DWORD dwTimspan);

	static void SetTimeScales(std::string strTimeSpan);
	static std::vector<struct tm> m_vecTimeScales;
	static tm GetTimeInSpan(struct tm tmPara, int nSpan);


	void GetFolderFileName(std::string& strFileName,const std::string& folder="");
	CToolThread*	m_hWorkerSaveThread;
	static DWORD WINAPI ThreadSave(void* pData);
	void ProcessSave();
};
