#include "stdafx.h"

#include "DataManager.h"
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
CDataManager* CDataManager::m_DataManager=NULL;
std::vector<struct tm> CDataManager::m_vecTimeScales;
CDataManager::CDataManager(void)
{
	std::string		UDPSvrIP;
	unsigned short	UDPSvrPort;
	std::string strTime;
	Load(UDPSvrIP, UDPSvrPort, strTime);
	SetTimeScales(strTime);
	m_hWorkerSaveThread =new CToolThread(ThreadSave, this);
}

CDataManager::~CDataManager(void)
{
	if(m_hWorkerSaveThread)
	{
		delete m_hWorkerSaveThread;
		m_hWorkerSaveThread = NULL;
	}	
}
void CDataManager::Init(void)
{
	if(m_DataManager)
	{
		Release();
		
	}

	m_DataManager=new CDataManager();

}
void CDataManager::Release(void)
{
	
	SAFE_DELETE(m_DataManager);
}

CDataManager* CDataManager::Get()
{
	return m_DataManager;
}
DWORD CDataManager::ThreadSave( void* pData )
{
	CDataManager* pWorker = (CDataManager*)pData;
	if(pWorker)
		pWorker->ProcessSave();
	return 0;
}
void CDataManager::GetFolderFileName(std::string& strFilePath,const std::string& folder)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
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
			CreateDirectory(strSystemPath.c_str(),NULL);
		}
		strFilePath = strSystemPath + strFilePath;
	}	
}
void CDataManager::ProcessSave()
{
	if (NULL==m_hWorkerSaveThread)
	{
		return;
	}
	while(!m_hWorkerSaveThread->IsNeedExit())
	{
		std::string strFilaName="save.txt";
		GetFolderFileName(strFilaName);
		if (DeleteFile(strFilaName.c_str()))
		{
			std::vector<StatisticsData> vecData;
			GetAllStatics_Min(vecData);
			for(int i =0; i< (int)vecData.size(); i++)
			{
				char BufHead[512];
				memset(BufHead, 0, sizeof(BufHead));					
				StatisticsData& data = vecData[i];
				time_t tm1 = (time_t)data.tmTime;
				struct tm dwTime = *localtime(&tm1);
				
				DWORD dwUpDelay   = data.dwUpDelay;
				DWORD dwDownDelay = data.dwDownDelay;
				sprintf(BufHead,"time:[%2d:%2d]; UpRecv:[%ld]; UpSend:[%ld]; DownRecv:[%ld]; DownSend:[%ld]; UpDelay:[%ld]; DownDelay:[%ld]", 
					(DWORD)dwTime.tm_hour, (DWORD)dwTime.tm_min, data.dwUpRecv,data.dwUpSend, data.dwDownRecv, data.dwDownSend, dwUpDelay, dwDownDelay);	
				LogData("%s",BufHead);				

			}
		}		
		else
		{
			//空闲时自动执行动作
			Sleep(60000);
			continue;
		}
	}

}
bool CDataManager::GetClinetUnConnectTime(Stru_IPAddress RemoteAddr, SOCKET hSocket, int& nTime)
{
	bool bFind = false;
	m_Mutex_mapClientUnConnectTime.write_lock();
	SClientKey clientKey(RemoteAddr, hSocket);
	std::map<SClientKey, int>::iterator it = m_mapClientUnConnectTime.find(clientKey);
	if(it != m_mapClientUnConnectTime.end())
	{
		nTime = it->second;
		bFind = true;		
	}		
	m_Mutex_mapClientUnConnectTime.write_unlock();
	return bFind;
}
void CDataManager::SetClinetUnConnectTime(Stru_IPAddress RemoteAddr, SOCKET hSocket, int& nTime)
{
	m_Mutex_mapClientUnConnectTime.write_lock();
	SClientKey clientKey(RemoteAddr, hSocket);
	m_mapClientUnConnectTime[clientKey] = nTime;
	m_Mutex_mapClientUnConnectTime.write_unlock();
}
void CDataManager::ChangeAllUnConnectTime()
{	
	m_Mutex_mapClientUnConnectTime.write_lock();
	std::map<SClientKey, int>::iterator it = m_mapClientUnConnectTime.begin();
	while(it != m_mapClientUnConnectTime.end())
	{
		int nTime = it->second;
		if(nTime <50000)
		{
			it->second++;
			it++;
		}
		else 
		{//5秒没检测到就是掉线了
			it = m_mapClientUnConnectTime.erase(it);
		}		
	}
	m_Mutex_mapClientUnConnectTime.write_unlock();
}
void CDataManager::GetAllUnconnetTime(std::map<SClientKey, int>& mapClientUnConnectTime)
{
	m_Mutex_mapClientUnConnectTime.write_lock();
	mapClientUnConnectTime = m_mapClientUnConnectTime;
	m_Mutex_mapClientUnConnectTime.write_unlock();
}
void CDataManager::AppendServerOrder(ServerOrder& order)
{
	m_Mutex_ServerOrder.write_lock();
	m_vecServerOrder.push_back(order); 
	m_Mutex_ServerOrder.write_unlock();
}
void CDataManager::AddStatisticsData(ServerOrder& order)
{
	m_Mutex_StatisticsData.write_lock();
	std::map<DWORD, StatisticsData>::iterator it = m_mapStatisticsData.find(order.nUTCTime);
	if(it == m_mapStatisticsData.end())
	{
		StatisticsData data;	
		data.tmTime = order.nUTCTime;
		switch(order.userdata2)
		{
			case 1://上行接收
				data.dwUpRecv += 1;
				break;
			case 2://上行发送
				data.dwUpSend += 1;
				break;
			case 3://下行接收
				data.dwDownRecv += 1;//接收
				break;
			case 4://下行发送
				data.dwDownSend += 1;//发送
				break;
			case 5:
				break;
			case 6:
				break;
			default:
				break;
		}		
		m_mapStatisticsData[order.nUTCTime] = data;
	}
	else
	{
		StatisticsData& data = it->second;
		data.tmTime = order.nUTCTime;
		switch(order.userdata2)
		{
		case 1://上行接收
			data.dwUpRecv += 1;
			break;
		case 2://上行发送
			data.dwUpSend += 1;
			break;
		case 3://下行接收
			data.dwDownRecv += 1;//接收
			break;
		case 4://下行发送
			data.dwDownSend += 1;//发送
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			break;
		}		
	}
	m_Mutex_StatisticsData.write_unlock();
}
void CDataManager::AddStatisticsData_Min(ServerOrder& order)
{
	time_t timeT = (time_t)order.nUTCTime;
	if(timeT < 0)
		return;
	m_Mutex_StatisticsData_Min.write_lock();	
	struct tm TM = *localtime(&timeT);	
	
	struct tm  tmTime = GetTimeInSpan(TM,  60);
	time_t t1 = mktime( &tmTime);
	std::map<DWORD, StatisticsData>::iterator it = m_mapStatisticsData_Min.find(t1);
	if(it == m_mapStatisticsData_Min.end())
	{
		StatisticsData data;	
		data.tmTime = t1;
		switch(order.userdata2)
		{
		case 1://上行接收
			data.dwUpRecv += 1;
			break;
		case 2://上行发送
			data.dwUpSend += 1;
			break;
		case 3://下行接收
			data.dwDownRecv += 1;//接收
			break;
		case 4://下行发送
			data.dwDownSend += 1;//发送
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			break;
		}		
		m_mapStatisticsData_Min[t1] = data;
	}
	else
	{
		StatisticsData& data = it->second;
		data.tmTime = t1;
		switch(order.userdata2)
		{
		case 1://上行接收
			data.dwUpRecv += 1;
			break;
		case 2://上行发送
			data.dwUpSend += 1;
			break;
		case 3://下行接收
			data.dwDownRecv += 1;//接收
			break;
		case 4://下行发送
			data.dwDownSend += 1;//发送
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			break;
		}		
	}
	m_Mutex_StatisticsData_Min.write_unlock();
}
void CDataManager::CalcDelay(ServerOrder& order)
{
	DWORD dwTimspan = 0;
	DWORD dwTime = 0;
	m_Mutex_mapSeq2ServerOrder.write_lock();
	std::map<int, ServerOrder>::iterator it = m_mapSeq2ServerOrder.find(order.userdata3);
	if(it == m_mapSeq2ServerOrder.end())
	{
		if(order.userdata2 <5)//不包含发送接收失败的
			m_mapSeq2ServerOrder[order.userdata3] = order;
	}
	else
	{
		ServerOrder& pre = it->second;
		if(order.userdata2 == UPRECEV || order.userdata2 == DOWNRECEV)
		{//接收
			dwTimspan = (pre.nUTCTime - order.nUTCTime)*1000 + (pre.nMiliSecond - order.nMiliSecond);
			dwTime    = pre.nUTCTime;
		}
		else if(order.userdata2 == UPSEND || order.userdata2 == DOWNSEND)
		{
			dwTimspan = (order.nUTCTime - pre.nUTCTime)*1000 + (order.nMiliSecond - pre.nMiliSecond);
			dwTime    = order.nUTCTime;
		}
		m_mapSeq2ServerOrder.erase(order.userdata3);//有进有出，完成统计延迟时间，就从数组里删除了
	}
	m_Mutex_mapSeq2ServerOrder.write_unlock();

//////////////////////////////////////////////////////////////////////////
	m_Mutex_StatisticsData.write_lock();
	std::map<DWORD, StatisticsData>::iterator itStatis = m_mapStatisticsData.find(order.nUTCTime);
	if(itStatis == m_mapStatisticsData.end())
	{
		StatisticsData data;	
		data.tmTime = order.nUTCTime;
		if(order.userdata2 == UPRECEV || order.userdata2 == UPSEND)
		{
			data.dwUpDelay += dwTimspan;
		}
		else if(order.userdata2 == DOWNRECEV || order.userdata2 == DOWNSEND)
		{
			data.dwDownDelay += dwTimspan;
		}		
		m_mapStatisticsData[ order.nUTCTime] = data;
	}
	else
	{
		StatisticsData& data = itStatis->second;
		data.tmTime = order.nUTCTime;
		if(order.userdata2 == UPRECEV || order.userdata2 == UPSEND)
		{
			data.dwUpDelay += dwTimspan;
		}
		else if(order.userdata2 == DOWNRECEV || order.userdata2 == DOWNSEND)
		{
			data.dwDownDelay += dwTimspan;
		}		
		m_mapStatisticsData[ order.nUTCTime] = data;
	}
	m_Mutex_StatisticsData.write_unlock();
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
	time_t timeT = (time_t)order.nUTCTime;
	if(timeT <0)
		return;
	m_Mutex_StatisticsData_Min.write_lock();	
	struct tm TM = *localtime(&timeT);	
	struct tm  tmTime = GetTimeInSpan(TM,  60);
	time_t t1 = mktime( &tmTime);
	std::map<DWORD, StatisticsData>::iterator itStatis_Min = m_mapStatisticsData_Min.find(t1);
	if(itStatis_Min == m_mapStatisticsData_Min.end())
	{
		StatisticsData data;	
		data.tmTime = t1;
		if(order.userdata2 == UPRECEV || order.userdata2 == UPSEND)
		{
			data.dwUpDelay += dwTimspan;
		}
		else if(order.userdata2 == DOWNRECEV || order.userdata2 == DOWNSEND)
		{
			data.dwDownDelay += dwTimspan;
		}		
		m_mapStatisticsData_Min[ t1] = data;
	}
	else
	{
		StatisticsData& data = itStatis_Min->second;
		//data.tmTime = order.nUTCTime;
		data.tmTime = t1;
		if(order.userdata2 == UPRECEV || order.userdata2 == UPSEND)
		{
			data.dwUpDelay += dwTimspan;
		}
		else if(order.userdata2 == DOWNRECEV || order.userdata2 == DOWNSEND)
		{
			data.dwDownDelay += dwTimspan;
		}		
		m_mapStatisticsData_Min[ t1] = data;
	}
	m_Mutex_StatisticsData_Min.write_unlock();
	//////////////////////////////////////////////////////////////////////////

	CalcCountDelay(order, dwTimspan);
}
void CDataManager::CalcCountDelay(ServerOrder& order, DWORD dwTimspan)
{	
	m_Mutex_Statistics.write_lock();	
	switch(order.userdata2)
	{
	case 1://上行接收
		m_mapStatistics.dwUpRecv += 1;
		break;
	case 2://上行发送
		m_mapStatistics.dwUpSend += 1;
		break;
	case 3://下行接收
		m_mapStatistics.dwDownRecv += 1;//接收
		break;
	case 4://下行发送
		m_mapStatistics.dwDownSend += 1;//发送
		break;
	case 5:
		break;
	case 6:
		break;
	default:
		break;
	}	
	if(order.userdata2 == UPRECEV || order.userdata2 == UPSEND)
	{
		m_mapStatistics.dwUpDelay += dwTimspan;
	}
	else if(order.userdata2 == DOWNRECEV || order.userdata2 == DOWNSEND)
	{
		m_mapStatistics.dwDownDelay += dwTimspan;
	}		
	m_Mutex_Statistics.write_unlock();
}
bool CDataManager::GetStatisticsData(DWORD& dwTime, StatisticsData& data)
{
	bool bFind = false;
	m_Mutex_StatisticsData.write_lock();
	std::map<DWORD, StatisticsData>::iterator itStatis = m_mapStatisticsData.find(dwTime);
	if(itStatis != m_mapStatisticsData.end())
	{
		data = itStatis->second;
		bFind = true;
	}
	m_Mutex_StatisticsData.write_unlock();

	return bFind;
}
bool CDataManager::GetStatisticsData_Min(DWORD& dwTime, StatisticsData& data)
{
	bool bFind = false;
	m_Mutex_StatisticsData_Min.write_lock();
	time_t timeT = (time_t)dwTime;
	struct tm TM = *localtime(&timeT);	
	struct tm  tmTime = GetTimeInSpan(TM,  60);
	time_t t1 = mktime( &tmTime);
	std::map<DWORD, StatisticsData>::iterator itStatis = m_mapStatisticsData_Min.find(t1);
	if(itStatis != m_mapStatisticsData_Min.end())
	{
		data = itStatis->second;
		bFind = true;
	}
	m_Mutex_StatisticsData_Min.write_unlock();

	return bFind;
}
void CDataManager::GetAllStatics(std::vector<StatisticsData>& vecData)
{
	m_Mutex_StatisticsData.write_lock();
	std::map<DWORD, StatisticsData>::iterator itStatis = m_mapStatisticsData.begin();
	while(itStatis != m_mapStatisticsData.end())
	{
		vecData.push_back(itStatis->second);
		itStatis++;
	}
	m_Mutex_StatisticsData.write_unlock();
	
}	

void CDataManager::GetAllStatics_Min(std::vector<StatisticsData>& vecData)
{
	m_Mutex_StatisticsData_Min.write_lock();
	std::map<DWORD, StatisticsData>::iterator itStatis = m_mapStatisticsData_Min.begin();
	while(itStatis != m_mapStatisticsData_Min.end())
	{
		vecData.push_back(itStatis->second);
		itStatis++;
	}
	m_Mutex_StatisticsData_Min.write_unlock();
}
void CDataManager::GetCountStatics(StatisticsData& data)
{
	m_Mutex_Statistics.write_lock();
	data = m_mapStatistics;
	m_Mutex_Statistics.write_unlock();
}
void CDataManager::SetTimeScales(std::string strTimeSpan)
{
	char strTime[128];
	SYSTEMTIME st;
	GetSystemTime(&st);
	memset(strTime, 0, sizeof(strTime));
	sprintf(strTime,"%.4d%.2d%.2d",st.wYear,st.wMonth,st.wDay);

	std::string m_strTradingDay = strTime;
	std::vector<std::pair<std::string,std::string>> TradingTime;

	vector<string> vecOne;
	split(strTimeSpan, ' ', vecOne);
	for(int k =0; k< (int)vecOne.size(); k++)
	{
		std::string strText = vecOne[k];
		vector<string> vec;
		split(strText, '-', vec);
		if(vec.size() ==2)
		{
			TradingTime.push_back(std::make_pair(vec[0], vec[1]));
		}
	}
	//TradingTime.push_back(std::make_pair("09:15", "11:30"));//第一交易时段的起止时间
	//TradingTime.push_back(std::make_pair("13:00", "15:15"));//第二交易时段的起止时间
	std::vector<struct tm> vecTimeScales;
	for(std::vector<std::pair<std::string, std::string>>::const_iterator it = TradingTime.begin(); it != TradingTime.end(); it++)
	{
		std::string strBegin = it->first;
		std::string strEnd   = it->second;		

		struct tm tmScale;
		tmScale.tm_year = atoi(m_strTradingDay.substr(0, 4).c_str()) - 1900;
		tmScale.tm_mon  = atoi(m_strTradingDay.substr(4, 2).c_str()) - 1;
		tmScale.tm_mday = atoi(m_strTradingDay.substr(6, 2).c_str());
		tmScale.tm_hour = atoi(strBegin.substr(0, 2).c_str());
		tmScale.tm_min  = atoi(strBegin.substr(3, 2).c_str());
		tmScale.tm_sec  = 0;		
		vecTimeScales.push_back(tmScale);

		tmScale.tm_hour = atoi(strEnd.substr(0, 2).c_str());
		tmScale.tm_min  = atoi(strEnd.substr(3, 2).c_str());
		tmScale.tm_sec  = 0;		
		vecTimeScales.push_back(tmScale);
	}
	m_vecTimeScales = vecTimeScales;
}
struct tm CDataManager::GetTimeInSpan(struct tm tmPara, int nSpan)
{
	vector<struct tm> vecTimeScales;
	vecTimeScales = m_vecTimeScales;

	time_t tmMax =   mktime(&vecTimeScales[vecTimeScales.size()-1]);
	time_t tReturn;

	//用这个值判断成功否
	time_t tm11 =   mktime(&tmPara);
	int nIndex = 0;	
	for(int i= 0; i< (int)vecTimeScales.size(); i++)
	{
		if(difftime(mktime(&vecTimeScales[i]), mktime(&tmPara)) >0) 
		{
			nIndex = i;
			break;
		}

		if(i == vecTimeScales.size() -1)
			return vecTimeScales[i];//15:00:07 算成 15:00:00
	}

	if(nIndex == 0)
	{		
		time_t t1 = mktime( &vecTimeScales[0]);
		t1 += nSpan;
		struct tm tmReturn = *localtime(&t1);
		return tmReturn; 
	}
	else if( nIndex%2==0)
	{
		time_t t1 = mktime( &vecTimeScales[nIndex-1]);
		t1 -= 1;//
		struct tm tmReturn = *localtime(&t1);
		return GetTimeInSpan(tmReturn, nSpan);
		//	return vecTimeScales[nIndex-1];//11:30:10 算成11:30分
	}
	DWORD	dwCount = 0;
	for(int i = 0; i<nIndex-1; i++)
	{		
		time_t t1 = mktime( &vecTimeScales[i] );
		time_t t2 = mktime( &vecTimeScales[i+1]);
		DWORD dw = abs(difftime(t1, t2));
		dwCount += dw;
		i++;
	}
	if(nIndex != 0)
	{
		DWORD dwn = dwCount%nSpan;
		time_t t1 = mktime( &vecTimeScales[nIndex -1]);
		time_t t3 = t1-dwn;
		while(t3 <= tm11)
		{
			t3 += nSpan;
		}
		if(nIndex + 2 < (int)vecTimeScales.size())
		{
			time_t tNext = mktime( &vecTimeScales[nIndex]);
			if(t3 > tNext)
			{
				t3 = t3- tNext + mktime(&vecTimeScales[nIndex+1]);//11:29:00在30分钟是应该是13:15（9:15开盘）
			}			
		}
		tReturn = t3;	
	}
	if(tReturn > tmMax)
		tReturn = tmMax;

	struct tm tmReturn;
	tmReturn.tm_year = 0;
	tmReturn = *localtime(&tReturn);

	return tmReturn;
}