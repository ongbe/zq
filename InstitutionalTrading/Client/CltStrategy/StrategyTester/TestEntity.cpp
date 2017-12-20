
#include "stdafx.h"
#include "TestEntity.h"
#include "tinyxml\tinyxml.h"
#include "..\SvrTradeData\Interface_SvrTradeData.h"

using namespace System::Diagnostics;
using namespace System::Windows;

DWORD CTestEntity::Proc_Thread(LPVOID lpParam)
{
	if(lpParam==NULL)
		return -1;

	CTestEntity* pTest = (CTestEntity*)lpParam;
	Sleep(2000);
	//::MessageBox(NULL, "StrategyTester.exe", "提示", 0);
	pTest->Run();

    return 0; 
} 

CTestEntity::CTestEntity()
{
	m_hThread = NULL;
	m_nThreadID = 0;
	m_hRunStrategyProcess = NULL;
	m_pDataEntity = NULL;
	m_pMDApi = NULL;
	m_hQuotFile = NULL;
	m_pTradeData = NULL;
	m_nRunStatus = conRunNone;
	m_bQuotFromFile = true;
	m_nStartDate = 0;
	m_nEndDate = 0;
	m_nNextDate = -1;
	m_nNextMinute = -1;
	m_nCurrQuotPos = 0;
	m_nQuotUpdateTime = 0;
	m_bWaitQuotQuery = false;
	m_mutex = ::CreateMutex(NULL, FALSE, "");
}

CTestEntity::~CTestEntity()
{
	CloseHandle(m_mutex);
}

void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst )
{	
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
	{
		strBig.replace( pos, srclen, strdst );
		pos += dstlen;
	}
} 

bool CTestEntity::loadXMLString(std::string& strStraID, std::string& strStraParams, 
								std::string& strInstruments, std::string& strStraRunPath, 
								StrategyTestParams& testParams, std::string& strXML)
{

	TiXmlDocument *myDocument = new TiXmlDocument();
//	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","gb2312","no");  
//	myDocument->LinkEndChild(decl);  

	//创建一个根元素并连接。
	TiXmlElement *RootElement = new TiXmlElement("strategy");

	myDocument->LinkEndChild(RootElement);

	//创建一个Person元素并连接。
	TiXmlElement *strategyName = new TiXmlElement("strategyName");
	RootElement->LinkEndChild(strategyName);
	TiXmlText *strategyNameContent = new TiXmlText(strStraID.c_str());
	strategyName->LinkEndChild(strategyNameContent);

	TiXmlElement *traderName = new TiXmlElement("traderName");
	RootElement->LinkEndChild(traderName);
	TiXmlText *traderNameContent = new TiXmlText("localtestor");
	traderName->LinkEndChild(traderNameContent);

	TiXmlElement *para = new TiXmlElement("parameter");
	RootElement->LinkEndChild(para);

	TiXmlText *paraContent = new TiXmlText(strStraParams.c_str());	
	para->LinkEndChild(paraContent);
	
	
	TiXmlElement *instrument = new TiXmlElement("instrument");
	RootElement->LinkEndChild(instrument);
	TiXmlText *instrumentContent = new TiXmlText(strInstruments.c_str());
	instrument->LinkEndChild(instrumentContent);

	int nFrontID =0, nSessionID =0;
	char szFrontID[55];
	sprintf(szFrontID,"%d",nFrontID);
	char szSessionID[55];
	sprintf(szSessionID,"%d",nSessionID);

	TiXmlElement *sessionID = new TiXmlElement("sessionID");
	RootElement->LinkEndChild(sessionID);
	TiXmlText *sessionIDContent = new TiXmlText(szSessionID);
	sessionID->LinkEndChild(sessionIDContent);

	TiXmlElement *FrontID = new TiXmlElement("frontID");
	RootElement->LinkEndChild(FrontID);
	TiXmlText *szFrontIDContent = new TiXmlText(szFrontID);
	FrontID->LinkEndChild(szFrontIDContent);

	TiXmlElement *orderRefID = new TiXmlElement("orderRef");
	RootElement->LinkEndChild(orderRefID);
	TiXmlText *orderRefIDContent = new TiXmlText("1");
	orderRefID->LinkEndChild(orderRefIDContent);
//////////////////////////////////////////////////////////////////////////
	char szStartTimer[55];
	//sprintf(szStartTimer,"%d",testParams.bStartTimer);
	sprintf(szStartTimer,"0");

	TiXmlElement *StartTimer = new TiXmlElement("startTimer");
	RootElement->LinkEndChild(StartTimer);
	TiXmlText *sessionStartTimer = new TiXmlText(szStartTimer);
	StartTimer->LinkEndChild(sessionStartTimer);

	char szTimerSpan[55];
	//sprintf(szTimerSpan,"%d",testParams.nTimerSpan);
	sprintf(szTimerSpan,"0");

	TiXmlElement *TimerSpan = new TiXmlElement("timerSpan");
	RootElement->LinkEndChild(TimerSpan);
	TiXmlText *sessionTimerSpan = new TiXmlText(szTimerSpan);
	TimerSpan->LinkEndChild(sessionTimerSpan);

	TiXmlElement *RunAfterOffline = new TiXmlElement("RunAfterOffline");//0为空转，1为运行
	RootElement->LinkEndChild(RunAfterOffline);
	TiXmlText *ttszRunAfterOffline = new TiXmlText("1");
	RunAfterOffline->LinkEndChild(ttszRunAfterOffline);

	//TiXmlElement *runPath = new TiXmlElement("runpath");
	//RootElement->LinkEndChild(runPath);
	//TiXmlText *runPathContent = new TiXmlText(strStraRunPath.c_str());
	//runPath->LinkEndChild(runPathContent);

	char csText[16] = {0};
	sprintf(csText,"%d", testParams.bSaveHistory);
	TiXmlElement *saveHistory = new TiXmlElement("savehistory");
	RootElement->LinkEndChild(saveHistory);
	TiXmlText *ttszsaveHistory = new TiXmlText(csText);
	saveHistory->LinkEndChild(ttszsaveHistory);

	TiXmlElement *historyPath = new TiXmlElement("historypath");
	RootElement->LinkEndChild(historyPath);
	TiXmlText *ttszhistoryPath = new TiXmlText(_GetHistoryDataPath().c_str());
	historyPath->LinkEndChild(ttszhistoryPath);

//////////////////////////////////////////////////////////////////////////
	TiXmlPrinter printer;
	myDocument->Accept(&printer);
	strXML =	printer.CStr();
	string_replace(strXML, "&lt;", "<");
	string_replace(strXML, "&gt;", ">");
	string_replace(strXML, "&apos;", "'");
	string_replace(strXML, "&#x0D;&#x0A;", "");
	string_replace(strXML, "&#x0A;", "");//只有换行符就直接去掉
	string_replace(strXML, "&#x0D;", "");//c#代码不支持回车换行符
	//string_replace(strXML, ">\n;", ">");//c#代码不支持回车换行符
	//myDocument->SaveFile("c:\\1.xml");
	return true;
}

bool CTestEntity::CreateRunStrategyExe()
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si,0,sizeof(si));
	si.cb=sizeof(si);
	si.wShowWindow=SW_SHOW;
	si.dwFlags=STARTF_USESHOWWINDOW;

	std::string strXML;
	std::string strStraParams;
	m_pDataEntity->GetStrategy().MakeXml(strStraParams);
	loadXMLString((std::string&)m_pDataEntity->GetStrategy().GetStrategyName(), 
			strStraParams, m_strInstrumentIDs, _GetRunPath(), m_testParams, strXML);
	std::string strFileName   = m_strRunPath + "RunSingleStrategy.exe";

	std::string strCmd = strFileName + " \"" + strXML + "\"";	
	
	bool bRet = CreateProcess(NULL,(LPSTR)strCmd.c_str(),NULL, NULL, FALSE, 0, NULL, (LPSTR)m_strRunPath.c_str(), &si,&pi);
	m_hRunStrategyProcess = pi.hProcess;
	//::MessageBox(NULL, "Create RunSingleStrategy.exe", "提示", 0);
	return bRet;
}

bool CTestEntity::Init(const std::vector<std::string>& vecInstrument, 
			CStrategyDataEx& strategy, const StrategyTestParams& testParams)
{ 
    
    //CInterface_SvrTradeData::getObj().InitFunc("0003","8090","01159539933","IF","IF1406",10000.0f);

	if(m_pDataEntity==NULL)
		return false;
	if(vecInstrument.size()==0)
		return false;




	//std::string strXml;
	//strategy.MakeXml(strXml);
	//m_strategy.ImportFromXml(strXml);
	m_strategy = strategy;
	m_testParams = testParams;
	m_vecInstrument = vecInstrument;

	if(strncmp(_GetRedisServer().c_str(), "tcp://", strlen("tcp://"))!=0)
		return false;

	char strAddr[128];
	char strPort[8];

	ZeroMemory(strAddr, sizeof(strAddr));
	ZeroMemory(strPort, sizeof(strPort));
	sscanf_s(_GetRedisServer().c_str()+strlen("tcp://"), "%[^:]:%s", strAddr, sizeof(strAddr)-1, 
			strPort, sizeof(strPort)-1);

	if(m_memCache.connect(std::string(strAddr), atoi(strPort))==false || 
			m_memSubCache.connect(std::string(strAddr), atoi(strPort))==false)
	{return false;}
	else
	{
		PostThreadMessage(g_idThreadSubcribe,TMESSAGE_START_SUBCRIBE_CACHE_CHANNEL,0,0);
	}


	if(m_bQuotFromFile) {
		m_hQuotFile = ::CreateFile(m_strQuotFile.c_str(), 	// file to open
                       GENERIC_READ,						// open for reading
                       FILE_SHARE_READ,						// share for reading
                       NULL,								// default security
                       OPEN_EXISTING,						// existing file only
                       FILE_ATTRIBUTE_NORMAL,				// normal file
                       NULL);								// no attr. template
		if(m_hQuotFile==INVALID_HANDLE_VALUE) 
			return FALSE;
	}
	else {
		m_pMDApi = _GetMDApi();
		m_pMDApi->RegisterFront((LPSTR)_GetQuotServer().c_str());
		m_pMDApi->SetBuzSpi(this);
		m_pMDApi->Init();
	}

	std::string strPath;
	SYSTEMTIME sysTime;
	char csDate[64] = {0};
	char csTime[64] = {0};

	::GetLocalTime(&sysTime);
	sprintf(csDate, "%04u%02u%02u\\", sysTime.wYear, sysTime.wMonth, 
			sysTime.wDay);
	sprintf(csTime, "%02u%02u%02u\\", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strPath = _GetHistoryDataPath();
	strPath += csDate;
	::CreateDirectory(strPath.c_str(), NULL);
	strPath += csTime;
	::CreateDirectory(strPath.c_str(), NULL);
	_SetHistoryDataPath(strPath);
	m_pDataEntity->SetSavePath(strPath);
	// m_pDataEntity->SetDate();
	// m_pDataEntity->SetTime();

	std::string szBrokerCode("0000"); 
	std::string szAccount("000001");
	std::string szUserID("localtestor");

	CInterface_SvrTradeData::getObj().InitFunc(szUserID, 
			szBrokerCode, szAccount, m_pDataEntity, &m_memCache, m_testParams.fInitFee);

	// 初始化模拟成功
	if(!m_oTradeSimu.init(szUserID, m_pDataEntity, m_testParams.nTradeRule, &m_memSubCache)) {
		return false;
	}

	// 创建策略实例进程
	CreateRunStrategyExe();

	// 创建策略行情推送进程
	m_nRunStatus = conRunPause;
	m_hThread = ::CreateThread(NULL, 0, Proc_Thread, this, 0, &m_nThreadID);
	if(m_hThread == NULL)
		return false;

	return true;
}

void CTestEntity::Start()
{
	if(m_nRunStatus==conRunPause)
		m_nRunStatus = conRunning;
}

void CTestEntity::Pause()
{
	if(m_nRunStatus==conRunning)
		m_nRunStatus = conRunPause;
}

void CTestEntity::Stop()
{
	m_nRunStatus = conRunFinish;
}

void CTestEntity::Release()
{
	m_memCache.disconnect();
	m_memSubCache.disconnect();
	CloseHandle(m_hQuotFile);
	m_hQuotFile = NULL;
	::TerminateProcess(m_hRunStrategyProcess, 1);
	m_hRunStrategyProcess = NULL;
	m_oTradeSimu.Release();
	if(m_pTradeData != NULL) {
		m_pTradeData->ReleaseFunc();
	}
	if(m_pMDApi != NULL) {
		m_pMDApi->Release();
		m_pMDApi = NULL;
	}
	// 在最外层释放
	//if(m_pDataEntity != NULL) {
	//	delete m_pDataEntity;
		m_pDataEntity = NULL;
	//}
}

void CTestEntity::Run()
{
	if(m_pDataEntity==NULL)
		return;

	bool bEnd = false;
	int nQuotTime = 0;
	DWORD dwCurrTick = ::GetTickCount();
	PlatformStru_DepthMarketData quot;
	SYSTEMTIME sysTime;
	char csText[64] = {0};
	//m_itDMQuotList = ((std::list<PlatformStru_DepthMarketData>&)m_pDataEntity->GetMDQuotList()).begin();

	::GetLocalTime(&sysTime);
	sprintf_s(csText, sizeof(csText)-1, "%04u-%02u-%02u %02u:%02u:%02u", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	m_pDataEntity->SetTestStartTime(std::string(csText));
	while(m_nRunStatus != conRunFinish) {
		if(m_nRunStatus != conRunning) {
			Sleep(5);
			continue;
		}
		if(!m_bQuotFromFile && !m_bIsConnectedQuot) {
			Sleep(5);
			continue;
		}
#if 0
		DWORD dwTick = ::GetTickCount();
		if(dwTick - dwCurrTick >= 100) {
			Sleep(10);
			dwCurrTick = dwTick;
		}
#else
		Sleep(_GetStrategyTestDelay());
#endif
		if(GetNextQuotItem(quot, bEnd)) {
			if(m_pDataEntity->GetInstrument().find(quot.InstrumentID)
					!=m_pDataEntity->GetInstrument().end()) {
				m_nCurrQuotPos = (m_nCurrQuotPos+1)%32400;
				m_pDataEntity->AppendQuot(quot);
				PushQuotToSimulate(quot);
				m_memCache.PushQuot(quot);
				if(m_testParams.bStartTimer) {
					nQuotTime = ConvTime(quot.UpdateTime);
					if(abs(nQuotTime-m_nQuotUpdateTime) >= m_testParams.nTimerSpan) {
						m_memCache.PushQuotTimer();
						m_nQuotUpdateTime = nQuotTime;
					}
				}
			}
		}
		if(bEnd)
			break;
	}
	::GetLocalTime(&sysTime);
	sprintf_s(csText, sizeof(csText)-1, "%04u-%02u-%02u %02u:%02u:%02u", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	m_pDataEntity->SetTestEndTime(std::string(csText));
	if(m_pDataEntity!=NULL) {
		m_pDataEntity->SaveAll();
	}
	m_nRunStatus = conRunFinish;
}

void CTestEntity::GetNextDateTime(int& nNextDate, int& nNextMinute)
{
	int nYear = 0, nMonth = 0, nDay = 0;
	int nHour = 0, nMinute = 0;
	
	nYear = nNextDate/10000;
	nMonth = (nNextDate/100)%100;
	nDay = nNextDate%100;

	nHour = nNextMinute/100;
	nMinute = nNextMinute%100;

	nMinute++;
	if(nMinute>59) {
		nMinute = 0;
		nHour++;
		if(nHour>23) {
			nHour = 0;
			nDay++;
			if(nDay>31) {
				nDay = 1;
				nMonth++;
				if(nMonth>12) {
					nMonth = 1;
					nYear++;
				}
			}
		}
	}

	nNextDate = nYear*10000 + nMonth*100 + nDay;
	nNextMinute = nHour*100 + nMinute;
}

int CTestEntity::ConvTime(char* pcsTime)
{
	if(pcsTime==NULL)
		return 0;
	int nHour = 0, nMinute = 0, nSecond = 0;
	sscanf(pcsTime, "%d:%02d:%02d", &nHour, &nMinute, &nSecond);
	return nHour*3600 + nMinute*60 + nSecond;
}

const bool CTestEntity::GetNextQuotItem(PlatformStru_DepthMarketData& quot, bool& bEnd)
{
	bool bRet = false;
	if(m_bQuotFromFile) {
		int nLineLen = 0;
		DWORD nRealRead = 0;
		char csLine[4096] = {0};
		while(::ReadFile(m_hQuotFile, csLine+nLineLen, 1, &nRealRead, NULL) && nLineLen<sizeof(csLine)-1) {
			nLineLen++;
			if(csLine[nLineLen-1]=='\n') 
				break;
		}
		if(nLineLen>0) {
			bRet = _ConvToQuot(std::string(csLine), quot);
			if(!bRet)
				bEnd = true;
		}
		else 
			bEnd = true;
	}
	else {
		LockObject();
		if(!m_queueQuot.empty()) {
			quot = m_queueQuot.front();
			m_queueQuot.pop();
			bRet = true;
		}
		bool bIsEmpty = m_queueQuot.empty();
		UnlockObject();
		if(bIsEmpty && !m_bWaitQuotQuery) {
			if(m_nNextDate == -1) {
				m_nNextDate = m_nStartDate;
				m_nNextMinute = 0;
			}
			else {
				GetNextDateTime(m_nNextDate, m_nNextMinute);
			}

			if(m_nNextDate>m_nEndDate) {
				bEnd = true;
			}
			else {
				char csText[256] = {0};
				sprintf(csText, "%08u %02u:%02u:00", m_nNextDate, m_nNextMinute/100, m_nNextMinute%100);
				//sprintf(csText, "%08u 11:09:00", m_nNextDate, m_nNextMinute/100, m_nNextMinute%100);
				m_pMDApi->ReqQryDepthMarketData(std::string(csText), 0);
				m_bWaitQuotQuery = true;
				//if(m_nNextMinute%100 == 0)
				//	Sleep(2000);
				//else if(m_nNextDate == m_nEndDate && m_nNextMinute/100 == 23 && m_nNextMinute%100 > 55)
				//	Sleep(2000);
			}
		}
	}
	return bRet;

	//if(m_itDMQuotList != ((std::list<PlatformStru_DepthMarketData>&)m_pDataEntity->GetMDQuotList()).end()) {
	//	quot = *m_itDMQuotList;
	//	m_itDMQuotList++;
	//	return true;
	//}
	//else
	//	return false;
}

void CTestEntity::PushQuotToSimulate(const PlatformStru_DepthMarketData& quot)
{
	CInterface_SvrTradeData::getObj().SetQuotInfo((PlatformStru_DepthMarketData&)quot);
	m_oTradeSimu.Quot(quot);
    m_oTradeSimu.DealCommand();
	//StrategyTester::MsChartForm^ form = (StrategyTester::MsChartForm^)
	//		System::Runtime::InteropServices::Marshal::GetObjectForIUnknown(
	//		IntPtr(_GetMsChartFormHandle()));
	//form->QuotProcess((PlatformStru_DepthMarketData&)quot);
}

void CTestEntity::OnQuotConnected()
{
	m_bIsConnectedQuot = true;
	m_bWaitQuotQuery = false;
}

void CTestEntity::OnQuotDisconnected()
{
	m_bIsConnectedQuot = false;
	m_bWaitQuotQuery = true;
}

void CTestEntity::OnTraderConnected()
{
}

void CTestEntity::OnTraderDisconnected()
{
}

void CTestEntity::OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)
{
}

void CTestEntity::OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)
{
}

void CTestEntity::OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum)
{
}

void CTestEntity::OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum)
{
	//m_mapMDQuot[rsp.InstrumentID] = rsp;
	//m_lstQuots.push_back(rsp);
	//PostEventToWnd(conDepthMarketData);
}

void CTestEntity::OnQueryQuotData(const vector<PlatformStru_DepthMarketData>& vec, int nErrorNum)
{
	if(nErrorNum==0) {
		LockObject();
		for(int i=0; i<vec.size(); i++) {
			m_queueQuot.push(vec[i]);
			Debug::Write(gcnew String(vec[i].InstrumentID));
			Debug::Write(", ");
			Debug::WriteLine(gcnew String(vec[i].UpdateTime));
		}
		UnlockObject();
	}
	m_bWaitQuotQuery = false;
}

void CTestEntity::OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum)
{
}

void CTestEntity::OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum)
{
}
