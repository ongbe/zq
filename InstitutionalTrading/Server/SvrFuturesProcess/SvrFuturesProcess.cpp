// SvrFuturesProcess.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SvrFuturesProcess.h"


SVRFUTURESPROCESS_API void InitFunc(void)
{   
	LoadConfig();

	hReadSQLDBThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ReadSQLDBThreadProc,0,0,&idReadSQLDBThread);
	hCaculateKLineThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)CaculateKLineThreadProc,0,0,&idCaculateKLineThread);
	hSaveDBThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)SaveDBThreadProc,0,0,&idSaveDBThread);
}


SVRFUTURESPROCESS_API void ReleaseFunc(void)
{ 
	if(hReadSQLDBThread)
	{
		//发送WM_QUIT通知线程结束
		PostThreadMessage(idReadSQLDBThread,WM_QUIT,0,0);

		//等待线程退出
		DWORD ExitCode;
		WaitForSingleObject(hReadSQLDBThread,8000);
		if(GetExitCodeThread(hReadSQLDBThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread(hReadSQLDBThread,0);
		CloseHandle(hReadSQLDBThread);
		hReadSQLDBThread=NULL;
		idReadSQLDBThread=0;

		WaitForSingleObject(hCaculateKLineThread,8000);
		if(GetExitCodeThread(hCaculateKLineThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread(hCaculateKLineThread,0);
		CloseHandle(hCaculateKLineThread);
		hCaculateKLineThread=NULL;
		idCaculateKLineThread=0;

		WaitForSingleObject(hSaveDBThread,8000);
		if(GetExitCodeThread(hSaveDBThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread(hSaveDBThread,0);
		CloseHandle(hSaveDBThread);
		hSaveDBThread=NULL;
		idSaveDBThread=0;
	}
}


/********************线程*********************************************/
DWORD ReadSQLDBThreadProc(void)
{
	STU student;
	::CoInitialize(NULL); // 初始化OLE/COM库环境 ，为访问ADO接口做准备
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_ConnectionPtr m_pConnection("ADODB.Connection");
	_bstr_t bstrSQL("select * from stu_info"); //查询语句 
	char * query_cmd = "DELETE FROM stu_info WHERE sname = '本拉登'";
	try
	{
		// 创建Connection对象
		m_pConnection.CreateInstance("ADODB.Connection");
		// 设置连接字符串，必须是BSTR型或者_bstr_t类型
		_bstr_t strConnect= "Provider=SQLOLEDB;Server=(local);Database=student; uid=sa; pwd=123;";
		//若数据库在网络上则Server为形如(192.168.1.5,3340)
		//用户sa和密码123只是针对我的库 
		m_pConnection->Open(strConnect,"","",adModeUnknown);
		if(m_pConnection==NULL)
			cerr<<"Lind data ERROR!\n";
		// 创建记录集对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// 取得表中的记录
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),
			adOpenDynamic,adLockOptimistic,adCmdText);
		_variant_t vsnum,vsname,vsage,vssex,vsmajor; //对应库中的snum,sname,sage,ssex,smajor
		cout << "学号 姓名 年龄 姓别 专业";
		cout << "\n----------------------------------------------------------------\n";
		while (!m_pRecordset->EndOfFile)
		{
			vsnum = m_pRecordset->GetCollect(_variant_t((long)0));//这儿给字段编号和字段名都可以 
			vsname = m_pRecordset->GetCollect("sname");
			vsage = m_pRecordset->GetCollect("sage");
			vssex = m_pRecordset->GetCollect("ssex");
			vsmajor = m_pRecordset->GetCollect("smajor");
			if (vsnum.vt != VT_NULL && vsname.vt != VT_NULL && vsage.vt != VT_NULL && vssex.vt != VT_NULL && vsmajor.vt != VT_NULL)
			{
				cout.setf(ios::left);
				cout << setw(14) << (char*)(_bstr_t)vsnum;
				cout << setw(14) << (char*)(_bstr_t)vsname;
				cout << setw(8) << vsage.lVal;
				cout << setw(8) << (char*)(_bstr_t)vssex;
				cout <<setw(20) << (char*)(_bstr_t)vsmajor;
				cout.unsetf(ios::left);
				cout << endl;
			} 
			m_pRecordset->MoveNext(); ///移到下一条记录
		}

		m_pRecordset->Close(); // 关闭记录集
	}
	catch(_com_error e)
	{    
		cerr << "\nERROR:" << (char*)e.Description();
	}

	if(m_pConnection->State)
		m_pConnection->Close();

	::CoUninitialize();

	return 0;
}

DWORD CaculateKLineThreadProc(void)
{	
	int nCount;
	PlatformStru_DepthMarketData data;

	while(!bQuit)
	{
		nCount = 0;

		// 用日KLine重算周KLine和月KLine
		RecalcWeekMonthKLine();

		//每处理100条数据后挂起
		while(GetQuotFromQueue(data)) 
		{
			SetDataInTime(&data);	

			nCount++;
			if(nCount>=100)
				break;
		}

		Sleep(100);
	}

	return 0;
}

DWORD SaveDBThreadProc(void)
{
	char strName[128];
	char strTime[128];

	int nErrorCode=CF_ERROR_SUCCESS;
	string szTableName = "AQUOTINFO";
	vector<KLine> vecData;

	while(!bQuit)
	{
		if(!GetKLineFromQueue(vecData))
		{
			Sleep(100);
			continue;
		}	

		for(vector<KLine>::iterator it=vecData.begin(); it!=vecData.end(); it++)
			SaveKline2DB(it->szInstrumentID,it->sKLine, it->ePhrase,it->szExchange);

		Sleep(100);
	}

	return 0;
}


/**************************************************************************/
void LoadConfig(void)
{
	CMyXML xml("ExchangeTradingTime.xml");
	xml.GetNodeContentInt(m_nTimeSpan,"root","Save2DBTimeSpan");


	if(m_strTradingDay.empty())
		return;


	m_tmCloseTime.tm_year = atoi(m_strTradingDay.substr(0, 4).c_str()) - 1900;
	m_tmCloseTime.tm_mon  = atoi(m_strTradingDay.substr(4, 2).c_str()) - 1;
	m_tmCloseTime.tm_mday = atoi(m_strTradingDay.substr(6, 2).c_str());
	m_tmCloseTime.tm_hour = 15;
	m_tmCloseTime.tm_min  = 0;
	m_tmCloseTime.tm_sec  = 0;	


	//各交易所交易时间配置文件 
	int i,nExchangeCount;
	xml.GetNodeContentInt(nExchangeCount,"root","Count");
	for(i=0;i<nExchangeCount;i++) 	
	{ 
		string szAttrId,szAttrTime;
		xml.GetNodeAttrValue(szAttrId,"id","root","tradingtime","group");
		xml.GetNodeAttrValue(szAttrTime,"time","root","tradingtime","group");

		vector<string> vecTime;
		vecTime = CTools_Ansi::SplitString(szAttrTime.c_str(),' ');

		vector<struct tm> vecRestTime;
		for(int i = 0; i < (int)vecTime.size(); i++)
		{
			string strTime = vecTime[i];
			vector<string> vecOne = CTools_Ansi::SplitString(strTime.c_str(),'-');
			if(vecOne.size() != 2)
				break;

			string strBegin = vecOne[0];
			string strEnd   = vecOne[1];	

			struct tm tmScale;
			tmScale.tm_year = atoi(m_strTradingDay.substr(0, 4).c_str()) - 1900;
			tmScale.tm_mon  = atoi(m_strTradingDay.substr(4, 2).c_str()) - 1;
			tmScale.tm_mday = atoi(m_strTradingDay.substr(6, 2).c_str());
			tmScale.tm_hour = atoi(strBegin.substr(0, 2).c_str());
			tmScale.tm_min  = atoi(strBegin.substr(3, 2).c_str());
			tmScale.tm_sec  = 0;		
			vecRestTime.push_back(tmScale);

			tmScale.tm_hour = atoi(strEnd.substr(0, 2).c_str());
			tmScale.tm_min  = atoi(strEnd.substr(3, 2).c_str());
			tmScale.tm_sec  = 0;		
			vecRestTime.push_back(tmScale);
		}

		m_mapExchangeTime.insert(make_pair(szAttrId,  vecRestTime));			
	}
}

bool GetQuotFromQueue(PlatformStru_DepthMarketData& outData)
{
	CeasymutexGuard guard(mutexQuot);

	if(!queueQuot.empty())
	{                
		outData = queueQuot.front();
		queueQuot.pop();
		return true;
	} 
	return false;
}

bool GetKLineFromQueue(vector<KLine>& vData)
{
	CeasymutexGuard guard(mutexKLine);

	KLine data;
	vData.clear();

	if(!queueKLine.empty())
	{                
		data = queueKLine.front();
		vData.push_back(data);
		queueKLine.pop();
	}

	return vData.size()>0;
}
bool RecalcWeekMonthKLine()
{
	if(m_strTradingDay.empty())
		return false;
	if(m_vmapKLineDay.empty())
		return false;

	time_t times = 0;
	int nDate = atoi(m_strTradingDay.c_str());
	struct tm tradingdayTime = {0};
	struct tm weekTime = {0};
	struct tm monthTime = {0};

	tradingdayTime.tm_year = nDate/10000 - 1900;
	tradingdayTime.tm_mon = ((nDate/100)%100)-1;
	tradingdayTime.tm_mday = nDate%100;

	for(int i=0; i<7; i++) {
		times = mktime(&tradingdayTime);
		times -= i*24*3600;
		weekTime = *localtime(&times);
		if(weekTime.tm_wday==1)
			break;
	}

	monthTime = tradingdayTime;
	monthTime.tm_mday = 1;

	{
		CeasymutexGuard guard(m_mutexKLineDay);
		CeasymutexGuard guard2(m_mutexKLineWeek);
		map<string,map<time_t, KLineBase>>::iterator it;
		map<time_t, KLineBase>::iterator itKLine;
		for(it = m_vmapKLineDay.begin(); it != m_vmapKLineDay.end(); it++) {

			bool bFirst = true;
			time_t startTime = mktime(&weekTime);
			KLineBase kLineSum;
			memset(&kLineSum, 0, sizeof(kLineSum));

			for(itKLine = it->second.begin(); itKLine != it->second.end(); itKLine++) {
				if(itKLine->first >= startTime)
					break;
			}
			for(; itKLine != it->second.end(); itKLine++) {
				if(bFirst) {
					kLineSum = itKLine->second;
					bFirst = false;
				}
				else {
					if(kLineSum.fHighPrice < itKLine->second.fHighPrice)
						kLineSum.fHighPrice = itKLine->second.fHighPrice;
					if(kLineSum.fLowPrice > itKLine->second.fLowPrice)
						kLineSum.fLowPrice = itKLine->second.fLowPrice;
					kLineSum.fClosePrice = itKLine->second.fClosePrice;
					kLineSum.fTurnover += itKLine->second.fTurnover;
					kLineSum.dwVolume += itKLine->second.dwVolume;
					kLineSum.dwOpenInterest = itKLine->second.dwOpenInterest;
				}
			}

			if(!bFirst) {
				map<string, vector<KLineBase>>::iterator itK = m_mapKLineWeek.find(it->first);
				if (itK == m_mapKLineWeek.end()) {
					vector<KLineBase> vecKLineMin;
					vecKLineMin.push_back(kLineSum);
					m_mapKLineWeek[it->first] = vecKLineMin;
				}
				else {
					itK->second.push_back(kLineSum);
				}
			}
		} 

	} 

	{
		CeasymutexGuard guard(m_mutexKLineDay);
		CeasymutexGuard guard2(m_mutexKLineMonth);
		map<string,map<time_t, KLineBase>>::iterator it;
		map<time_t, KLineBase>::iterator itKLine;
		for(it = m_vmapKLineDay.begin(); it != m_vmapKLineDay.end(); it++) {

			bool bFirst = true;
			time_t startTime = mktime(&monthTime);
			KLineBase kLineSum;
			memset(&kLineSum, 0, sizeof(kLineSum));

			for(itKLine = it->second.begin(); itKLine != it->second.end(); itKLine++) {
				if(itKLine->first >= startTime)
					break;
			}
			for(; itKLine != it->second.end(); itKLine++) {
				if(bFirst) {
					kLineSum = itKLine->second;
					bFirst = false;
				}
				else {
					if(kLineSum.fHighPrice < itKLine->second.fHighPrice)
						kLineSum.fHighPrice = itKLine->second.fHighPrice;
					if(kLineSum.fLowPrice > itKLine->second.fLowPrice)
						kLineSum.fLowPrice = itKLine->second.fLowPrice;
					kLineSum.fClosePrice = itKLine->second.fClosePrice;
					kLineSum.fTurnover += itKLine->second.fTurnover;
					kLineSum.dwVolume += itKLine->second.dwVolume;
					kLineSum.dwOpenInterest = itKLine->second.dwOpenInterest;
				}
			}

			if(!bFirst) {
				map<string, vector<KLineBase>>::iterator itK = m_mapKLineMonth.find(it->first);
				if (itK == m_mapKLineMonth.end()) {
					vector<KLineBase> vecKLineMin;
					vecKLineMin.push_back(kLineSum);
					m_mapKLineMonth[it->first] = vecKLineMin;
				}
				else {
					itK->second.push_back(kLineSum);
				}
			}
		} 
	}

	return true;
}

void SetDataInTime(PlatformStru_DepthMarketData* pData)
{

	string strDate = m_strTradingDay;//如果行情里面没有日期，照样不能解决跨日期的问题
	string strTime		= pData->UpdateTime;
	string strFutureID = pData->InstrumentID;
	if(util::isInvalidValue(pData->LastPrice)) 		
	{		
		return;//无效值
	}	

	if(util::isInvalidValue(pData->AskPrice1))
		pData->AskPrice1 = 0;
	if(util::isInvalidValue(pData->BidPrice1))
		pData->BidPrice1 = 0;
	if(util::isInvalidValue(pData->Volume))
		pData->Volume = 0;
	if(util::isInvalidValue(pData->Turnover))
		pData->Turnover = 0;
	if(util::isInvalidValue(pData->OpenInterest))
		pData->OpenInterest = 0;
	if(strDate.length() != 8)
	{		
		return;
	}

	double dcClose = pData->PreSettlementPrice;
	if(dcClose <0.00001)
		dcClose = pData->OpenPrice;//如果没有昨结算，则用今日开盘价	

	SetInTimeData_KLine(pData);
}

bool SetInTimeData_KLine(PlatformStru_DepthMarketData* pData)
{	
	if(m_mapExchangeTime.size() == 0)
		return false;

	string strTime		= pData->UpdateTime;
	string strFutureID = pData->InstrumentID;
	struct tm tempTime = {0};

	KLineBase pKLine;
	//strncpy(pKLine.InstrumentID,pData->InstrumentID,sizeof(pKLine.InstrumentID)-1);
	tempTime.tm_year	= atoi(m_strTradingDay.substr(0, 4).c_str())-1900;
	tempTime.tm_mon	= atoi(m_strTradingDay.substr(4, 2).c_str())-1;
	tempTime.tm_mday	= atoi(m_strTradingDay.substr(6, 2).c_str());
	tempTime.tm_hour	= atoi(strTime.substr(0, 2).c_str());
	tempTime.tm_min	= atoi(strTime.substr(3, 2).c_str());
	tempTime.tm_sec	= atoi(strTime.substr(6, 2).c_str());
	//pKLine.dwTime.tm_wday	= 0;
	//pKLine.dwTime.tm_yday	= 0;
	pKLine.dwTime			= mktime(&tempTime);
	pKLine.dwOpenInterest	= pData->OpenInterest;
	pKLine.dwVolume			= pData->Volume;
	pKLine.fClosePrice		= pData->LastPrice;
	pKLine.fHighPrice		= pData->HighestPrice;
	pKLine.fLowPrice		= pData->LowestPrice;
	pKLine.fOpenPrice		= pData->OpenPrice;
	pKLine.fTurnover		= pData->Turnover;
	//if( !util::isInvalidValue(pData->SettlementPrice))
	//{
	//	pKLine.dJieSuan = pData->SettlementPrice;
	//}

	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_1MIN,pData->ExchangeID);	
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_5MIN,pData->ExchangeID);	
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_15MIN,pData->ExchangeID);
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_30MIN,pData->ExchangeID);
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_60MIN,pData->ExchangeID);	
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_DAY,pData->ExchangeID);
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_WEEK,pData->ExchangeID);
	SetInTimeData_DiffSpan(string(pData->InstrumentID), pKLine, PHRASE_MONTH,pData->ExchangeID);

	m_mapKLineLast[strFutureID] = pKLine;
	return true;
}

bool SetInTimeData_DiffSpan(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,string exchangeID)
{	
	if (exchangeID.empty())
		return false;

	map<string,vector<struct tm>> ::iterator itTime=m_mapExchangeTime.find(exchangeID);
	if (itTime == m_mapExchangeTime.end())
		return true;

	KLineBase pTemp = pKLine;
	KLineBase KlineRet;
	memset( &KlineRet, 0, sizeof(KLineBase));
	bool bRet = true;

	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{			
			map<string ,vector<KLineBase>>::iterator it = m_mapKLineMin1.find(strInstrumentID);
			if (it == m_mapKLineMin1.end())
			{
				vector<KLineBase>  vecKLineMin;
				m_mapKLineMin1[strInstrumentID] = vecKLineMin;
				bRet = SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 60,itTime->second);//1
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 60,itTime->second);//1	
				if (bRet)
				{
					KlineRet = it->second[0];
				}				
			}
		}
		break;
	case PHRASE_5MIN:
		{
			map<string ,vector<KLineBase>>::iterator it = m_mapKLineMin5.find(strInstrumentID);
			if (it == m_mapKLineMin5.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMin5[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 300,itTime->second);//5
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 300,itTime->second);//5	
				if (bRet)
				{
					KlineRet = (it->second)[0];
				}				
			}
		}
		break;
	case PHRASE_15MIN:
		{
			map<string ,vector<KLineBase>>::iterator it = m_mapKLineMin15.find(strInstrumentID);
			if (it == m_mapKLineMin15.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMin15[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 900,itTime->second);//15
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 900,itTime->second);//15
				if (bRet)
				{
					KlineRet = it->second[0];
				}				
			}
		}
		break;
	case PHRASE_30MIN:
		{
			map<string ,vector<KLineBase>>::iterator it = m_mapKLineMin30.find(strInstrumentID);
			if (it == m_mapKLineMin30.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMin30[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 1800,itTime->second);//30
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 1800,itTime->second);//30	
				if (bRet)
				{
					KlineRet = it->second[0];
				}				
			}
		}
		break;
	case PHRASE_60MIN:
		{
			map<string ,vector<KLineBase>>::iterator it = m_mapKLineMin60.find(strInstrumentID);
			if (it == m_mapKLineMin60.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMin60[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_MINS(strInstrumentID, vecKLineMin, pTemp, 3600,itTime->second);//60
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}				
			}
			else
			{
				bRet =SetInTimeData_MINS(strInstrumentID, it->second, pTemp, 3600,itTime->second);//60	
				if (bRet)
				{
					KlineRet = it->second[0];
				}				
			}
		}
		break;
	case PHRASE_DAY:
		{
			map<string ,vector<KLineBase>>::iterator it = m_mapKLineDay.find(strInstrumentID);
			if (it == m_mapKLineDay.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineDay[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_Day(strInstrumentID, vecKLineMin, pTemp,itTime->second);
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}
			}
			else
			{
				bRet =SetInTimeData_Day(strInstrumentID, it->second, pTemp,itTime->second);	
				if (bRet)
				{
					KlineRet = it->second[0];
				}
			}
			//日，周，月线不需要时,分,秒;统一处理				
			/*KlineRet.dwTime.tm_hour = 0;
			KlineRet.dwTime.tm_min  = 0;
			KlineRet.dwTime.tm_sec  = 0;*/
		}
		break;
	case PHRASE_WEEK:
		{
			map<string ,vector<KLineBase>>::iterator it = m_mapKLineWeek.find(strInstrumentID);
			if (it == m_mapKLineWeek.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineWeek[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_Week(strInstrumentID, vecKLineMin, pTemp,itTime->second);
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}
			}
			else
			{
				bRet =SetInTimeData_Week(strInstrumentID, it->second, pTemp,itTime->second);	
				if (bRet)
				{
					KlineRet = it->second[0];
				}
			}
			//日，周，月线不需要时,分,秒;统一处理				
			/*KlineRet.dwTime.tm_hour = 0;
			KlineRet.dwTime.tm_min  = 0;
			KlineRet.dwTime.tm_sec  = 0;*/
		}	

		break;
	case PHRASE_MONTH:
		{
			map<string ,vector<KLineBase>>::iterator it = m_mapKLineMonth.find(strInstrumentID);
			if (it == m_mapKLineMonth.end())
			{
				vector<KLineBase> vecKLineMin;
				m_mapKLineMonth[strInstrumentID] = vecKLineMin;
				bRet =SetInTimeData_Month(strInstrumentID, vecKLineMin, pTemp,itTime->second);
				if (bRet)
				{
					KlineRet = vecKLineMin[0];
				}
			}
			else
			{
				bRet =SetInTimeData_Month(strInstrumentID, it->second, pTemp,itTime->second);	
				if (bRet)
				{
					KlineRet = it->second[0];
				}
			}	
			//日，周，月线不需要时,分,秒;统一处理				
			/*KlineRet.dwTime.tm_hour = 0;
			KlineRet.dwTime.tm_min  = 0;
			KlineRet.dwTime.tm_sec  = 0;*/

		}
		break;
	}

	if (bRet)
	{
		//加到cache里
		SaveKline2Cache(strInstrumentID, KlineRet, enumPhrase);

		KLine lineData;
		lineData.szExchange=exchangeID;
		lineData.szInstrumentID=strInstrumentID;
		lineData.ePhrase=enumPhrase;
		lineData.sKLine=KlineRet;

		CeasymutexGuard guard(mutexKLine);
		queueKLine.push(lineData);
		//SaveKline2DB(strInstrumentID,KlineRet, enumPhrase,exchangeID);
	}	

	return bRet;
}

bool SetInTimeData_MINS(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine, int nTimeSpan ,vector<struct tm>& vecTimeScales)
{
	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		struct tm tm1 = vecTimeScales[0];
		struct tm tm2 = *localtime(&pKLine.dwTime);
		// 这个地方逻辑上是不是有点问题？时间相等不做任何处理？
		if(difftime(mktime(&tm1), mktime(&tm2)) != 0)
		{
			pKLine.dwVolume		= 0;
			pKLine.fTurnover	= 0;
			pKLine.fOpenPrice	= pKLine.fClosePrice;
			pKLine.fHighPrice	= pKLine.fClosePrice;
			pKLine.fLowPrice	= pKLine.fClosePrice;
		}

		struct tm TM = GetTimeInSpan(*localtime(&pKLine.dwTime), nTimeSpan,vecTimeScales);
		if ( TM.tm_year == 0)
		{
			return false;
		}
		TM.tm_sec = 0;
		pKLine.dwTime = mktime(&TM);

		vecKLinePara.push_back(pKLine);
		return true;

	}
	else
	{
		KLineBase sKLineLast;
		struct tm sKLineLastTime = {0};
		memset( &sKLineLast,0,sizeof(KLineBase));
		map<string,KLineBase>::iterator itLast =	m_mapKLineLast.find(strInstrumentID);
		if (itLast!= m_mapKLineLast.end())
		{
			sKLineLast =  itLast->second;
			sKLineLastTime = *localtime(&sKLineLast.dwTime);
		}

		KLineBase& pLast = vecKLinePara[0];
		struct tm pLastTime = {0};
		struct tm TM = GetTimeInSpan(*localtime(&pKLine.dwTime), nTimeSpan,vecTimeScales);//15:15:09 就可以转换为15:15
		if ( TM.tm_year == 0)
		{
			return false;
		}
		pLastTime = *localtime(&pLast.dwTime);
		if(pLastTime.tm_year == TM.tm_year
			&& pLastTime.tm_mon == TM.tm_mon 
			&& pLastTime.tm_mday == TM.tm_mday)
		{//
			struct tm tm1 = pLastTime;
			struct tm tm2 = TM;
			int dwTimeSpan = difftime(mktime(&tm1), mktime(&tm2));
			if(dwTimeSpan==0)
			{//同一分钟	
				bSameMin = true;
				pLast.fClosePrice  = pKLine.fClosePrice;
				pLast.dwOpenInterest = pKLine.dwOpenInterest;
				//pLast.dJieSuan     = pKLine.dJieSuan;
				if(pLast.fHighPrice < pKLine.fClosePrice  )
					pLast.fHighPrice	= pKLine.fClosePrice;
				if(pLast.fLowPrice > pKLine.fClosePrice)
					pLast.fLowPrice	= pKLine.fClosePrice;

				if(sKLineLastTime.tm_year != 0)
				{
					pLast.dwVolume += pKLine.dwVolume - sKLineLast.dwVolume;				
					pLast.fTurnover	+= pKLine.fTurnover - sKLineLast.fTurnover;
				}

				return bSameMin;
			}
			else if(dwTimeSpan >0)
			{		
				KLineBase& pLastFirst = vecKLinePara[vecKLinePara.size() - 1];
				if(difftime(pLastFirst.dwTime, mktime(&TM)) > 0)
				{//比第一个数据还要早，主要是正对模拟行情处理
					if(sKLineLastTime.tm_year != 0)
					{
						pKLine.dwVolume  = pKLine.dwVolume - sKLineLast.dwVolume;
						pKLine.fTurnover = pKLine.fTurnover - sKLineLast.fTurnover;
					}
					else
					{
						pKLine.dwVolume		= 0;
						pKLine.fTurnover   = 0;
					}
					vecKLinePara.push_back(pKLine);					
				}
				else 
				{//实时行情加到目前数组的中间去
					vector<KLineBase>::iterator itLast = vecKLinePara.end();
					for(vector<KLineBase>::iterator it = vecKLinePara.begin(); it!= vecKLinePara.end(); it++)
					{
						KLineBase& pLastTemp = *it;
						struct tm pLastTempTime = {0};
						pLastTempTime = *localtime(&pLastTemp.dwTime);
						if(pLastTempTime.tm_year == TM.tm_year
							&& pLastTempTime.tm_mon == TM.tm_mon 
							&& pLastTempTime.tm_mday == TM.tm_mday
							&& pLastTempTime.tm_hour == TM.tm_hour
							&& pLastTempTime.tm_min == TM.tm_min)
						{//如果是按时间先加进去的则有可能出现这种情况
							bSameMin = true;

							pLastTemp.fClosePrice = pKLine.fClosePrice;
							if(pLastTemp.fHighPrice < pKLine.fClosePrice  )
								pLastTemp.fHighPrice = pKLine.fClosePrice;
							if(pLastTemp.fLowPrice > pKLine.fClosePrice)
								pLastTemp.fLowPrice	= pKLine.fClosePrice;
							if(sKLineLastTime.tm_year != 0)
							{
								pLastTemp.dwVolume  += pKLine.dwVolume - sKLineLast.dwVolume;
								pLastTemp.fTurnover += pKLine.fTurnover - sKLineLast.fTurnover;
							}						
							break;
						}
						if(difftime(pLastTemp.dwTime, mktime(&TM)) < 0)
						{		
							TM.tm_sec   = 0;
							pKLine.dwTime = mktime(&TM);
							if(sKLineLastTime.tm_year != 0)
							{
								pKLine.dwVolume  = pKLine.dwVolume - sKLineLast.dwVolume;
								pKLine.fTurnover = pKLine.fTurnover - sKLineLast.fTurnover;
							}
							//////////////////////////////////////////////////////////////////////////
							//对于插入数组的类型，最低最高价都应该新的计，以开盘价为准
							pKLine.fOpenPrice      = pKLine.fClosePrice;
							pKLine.fHighPrice		= pKLine.fClosePrice;
							pKLine.fLowPrice       = pKLine.fClosePrice;
							//////////////////////////////////////////////////////////////////////////
							vecKLinePara.insert(it, pKLine);
							break;
						}
						itLast = it;
					}	
				}
				return bSameMin;
			}
		}
		if(!bSameMin)
		{
			if(sKLineLastTime.tm_year != 0)
			{//
				pKLine.dwVolume     = pKLine.dwVolume - sKLineLast.dwVolume;				

				pKLine.fTurnover	= pKLine.fTurnover - sKLineLast.fTurnover;
				/* 廖说开盘价不用上一时间周期的收盘价修改之前;以后如果换成上一分钟的收盘价则应该把注解的打开，把后面的注掉就可以了
				pKLine.fOpenPrice      = sKLineLast.fClosePrice;
				pKLine.fHighPrice		= sKLineLast.fClosePrice;
				pKLine.fLowPrice       = sKLineLast.fClosePrice;
				*/
				pKLine.fOpenPrice      = pKLine.fClosePrice;
				pKLine.fHighPrice		= pKLine.fClosePrice;
				pKLine.fLowPrice       = pKLine.fClosePrice;
			}
			else
			{//刚开盘的第一笔
				struct tm tm1 = vecTimeScales[0];
				struct tm tm2 = *localtime(&pKLine.dwTime);
				if(difftime(mktime(&tm1), mktime(&tm2)) != 0)
				{
					pKLine.dwVolume		= 0;
					pKLine.fTurnover   = 0;
					pKLine.fOpenPrice      = pKLine.fClosePrice;
					pKLine.fHighPrice		= pKLine.fClosePrice;
					pKLine.fLowPrice       = pKLine.fClosePrice;					
				}
			}				

			TM.tm_sec   = 0;
			pKLine.dwTime = mktime(&TM);
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
		}		
	}

	return true;
}

bool SetInTimeData_Day(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine ,vector<struct tm>& vecTimeScales)
{
	if(difftime(mktime(&vecTimeScales[0]), pKLine.dwTime) > 0) 
		return false;
	if(util::isInvalidValue(pKLine.fHighPrice)
		||util::isInvalidValue(pKLine.fOpenPrice)
		||util::isInvalidValue(pKLine.fLowPrice))
		return false;

	struct tm pKLineTime = *localtime(&pKLine.dwTime);

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);	
		//return true;
	}
	else
	{
		KLineBase& pLast = vecKLinePara[0];		
		struct tm pLastTime = *localtime(&pLast.dwTime);
		if(pLastTime.tm_year == pKLineTime.tm_year
			&& pLastTime.tm_mon == pKLineTime.tm_mon 
			&& pLastTime.tm_mday == pKLineTime.tm_mday)
		{			
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwOpenInterest = pKLine.dwOpenInterest;
			//pLast.dJieSuan     = pKLine.dJieSuan;
			pLast.dwVolume     = pKLine.dwVolume;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;
			pLast.fOpenPrice	= pLast.fOpenPrice;
			pLast.fTurnover		= pLast.fTurnover;
			bSameMin = true;
		}
		if(!bSameMin)
		{			
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);				
		}
	}
	return true;
}

bool SetInTimeData_Week(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine ,vector<struct tm>& vecTimeScales)
{	
	if(difftime(mktime(&vecTimeScales[0]), pKLine.dwTime) > 0) 
		return false;
	if(util::isInvalidValue(pKLine.fHighPrice)
		||util::isInvalidValue(pKLine.fOpenPrice)
		||util::isInvalidValue(pKLine.fLowPrice))
		return false;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);		
	}
	else
	{
		bool hasFound = false;
		KLineBase sKLineLast;
		memset(&sKLineLast,0,sizeof(KLineBase));
		map<string,KLineBase>::iterator itLast =	m_mapKLineLast.find(strInstrumentID);
		if (itLast!= m_mapKLineLast.end())
		{
			sKLineLast =  itLast->second;
			hasFound = true;
		}

		KLineBase& pLast = vecKLinePara[0];	
		if(IsOneWeek(*localtime(&pLast.dwTime), *localtime(&pKLine.dwTime)))
		{			
			pLast.dwTime = pKLine.dwTime;
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwOpenInterest = pKLine.dwOpenInterest;
			//pLast.dJieSuan     = pKLine.dJieSuan;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;

			if(hasFound)
			{
				pLast.dwVolume  += pKLine.dwVolume - sKLineLast.dwVolume;				
				pLast.fTurnover	+= pKLine.fTurnover - sKLineLast.fTurnover;
			}
			bSameMin = true;			
		}
		if(!bSameMin)
		{
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);				
		}
	}
	return true;
}

bool SetInTimeData_Month(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine ,vector<struct tm>& vecTimeScales)
{
	if(difftime(mktime(&vecTimeScales[0]), pKLine.dwTime) > 0) 
		return false;

	if(util::isInvalidValue(pKLine.fHighPrice)
		||util::isInvalidValue(pKLine.fOpenPrice)
		||util::isInvalidValue(pKLine.fLowPrice))
		return false;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{		
		vecKLinePara.push_back(pKLine);		
	}
	else
	{
		bool hasFound = false;
		KLineBase sKLineLast;
		memset(&sKLineLast,0,sizeof(KLineBase));
		map<string,KLineBase>::iterator itLast =	m_mapKLineLast.find(strInstrumentID);
		if (itLast!= m_mapKLineLast.end())
		{
			sKLineLast =  itLast->second;
			hasFound = true;
		}

		KLineBase& pLast = vecKLinePara[0];	
		if(IsOneMonth(*localtime(&pLast.dwTime), *localtime(&pKLine.dwTime)))
		{
			pLast.dwTime = pKLine.dwTime;
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwOpenInterest = pKLine.dwOpenInterest;
			//pLast.dJieSuan     = pKLine.dJieSuan;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;
			if(hasFound)
			{
				pLast.dwVolume  += pKLine.dwVolume - sKLineLast.dwVolume;				
				pLast.fTurnover	+= pKLine.fTurnover - sKLineLast.fTurnover;
			}
			bSameMin = true;
		}
		if(!bSameMin)
		{
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
		}
	}

	return true;
}

bool SaveKline2Cache(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase)
{
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM1);
			map<string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin1.find(strInstrumentID);
			if(it!=m_vmapKLineMin1.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin1.insert(make_pair(strInstrumentID,mapTemp));
			}

			break;
		}			
	case PHRASE_5MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM5);
			map<string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin5.find(strInstrumentID);
			if(it!=m_vmapKLineMin5.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin5.insert(make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_15MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM15);
			map<string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin15.find(strInstrumentID);
			if(it!=m_vmapKLineMin15.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin15.insert(make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_30MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM30);
			map<string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin30.find(strInstrumentID);
			if(it!=m_vmapKLineMin30.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin30.insert(make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_60MIN:
		{
			CeasymutexGuard guard(m_mutexKLineM60);
			map<string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMin60.find(strInstrumentID);
			if(it!=m_vmapKLineMin60.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMin60.insert(make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_DAY:
		{
			CeasymutexGuard guard(m_mutexKLineDay);
			map<string,map<time_t,KLineBase>>::iterator it = m_vmapKLineDay.find(strInstrumentID);
			if(it!=m_vmapKLineDay.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineDay.insert(make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_WEEK:
		{
			CeasymutexGuard guard(m_mutexKLineWeek);
			map<string,map<time_t,KLineBase>>::iterator it = m_vmapKLineWeek.find(strInstrumentID);
			if(it!=m_vmapKLineWeek.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineWeek.insert(make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	case PHRASE_MONTH:
		{
			CeasymutexGuard guard(m_mutexKLineMonth);
			map<string,map<time_t,KLineBase>>::iterator it = m_vmapKLineMonth.find(strInstrumentID);
			if(it!=m_vmapKLineMonth.end())
			{
				map<time_t,KLineBase>& mapTime2SKLine = it->second;
				mapTime2SKLine[pKLine.dwTime]=pKLine;
			}
			else
			{
				map<time_t,KLineBase> mapTemp;
				mapTemp.insert(make_pair(pKLine.dwTime,pKLine));
				m_vmapKLineMonth.insert(make_pair(strInstrumentID,mapTemp));
			}

			break;
		}
	}

	return true;
}
bool SaveKline2DB(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase, string& exchangeID)
{

	int nErrorCode=CF_ERROR_SUCCESS;
	string lsDeleteSql = "";
	string szTableName = "";

	char strName[64];
	memset(strName,0,sizeof(strName));

	if (exchangeID.empty())
		return false;
	map<string,vector<struct tm>> ::iterator itTime=m_mapExchangeTime.find(exchangeID);
	if (itTime == m_mapExchangeTime.end())
		return true;

	KLineBase pTemp = pKLine;
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{			
			map<string,KLineBase >::iterator itLast = m_maptmLast1.find(strInstrumentID);
			if (itLast == m_maptmLast1.end() ) {
				m_maptmLast1[strInstrumentID] = pKLine;
				UpdateKLineData(strInstrumentID,PHRASE_1MIN,pKLine,true,nErrorCode);
				return true;
			}

			if( difftime(pKLine.dwTime, mktime(&(m_tmCloseTime ))) >=0 )
				UpdateKLineData(strInstrumentID,PHRASE_1MIN,pKLine,false,nErrorCode);
			else {
				//if( itLast->second.dwTime.tm_year == pKLine.dwTime.tm_year && itLast->second.dwTime.tm_mon == pKLine.dwTime.tm_mon && itLast->second.dwTime.tm_mday == pKLine.dwTime.tm_mday
				//	&& itLast->second.dwTime.tm_hour == pKLine.dwTime.tm_hour && itLast->second.dwTime.tm_min == pKLine.dwTime.tm_min )
				//if( itLast->second.dwTime/60 == pKLine.dwTime/60 ) {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,false,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_1MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast1[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_5MIN:
		{
			map<string,KLineBase >::iterator itLast = m_maptmLast5.find(strInstrumentID);
			if (itLast == m_maptmLast5.end()) {
				m_maptmLast5[strInstrumentID] = pKLine;
				UpdateKLineData(strInstrumentID,PHRASE_5MIN,pKLine,true,nErrorCode);
				return true;
			}			

			if( difftime(pKLine.dwTime, mktime(&(m_tmCloseTime)) ) >=0 ) {
				UpdateKLineData(strInstrumentID,PHRASE_5MIN,pKLine,false,nErrorCode);
			}
			else {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,false,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_5MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast5[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_15MIN:
		{
			map<string,KLineBase >::iterator itLast = m_maptmLast15.find(strInstrumentID);
			if (itLast == m_maptmLast15.end() ) {
				m_maptmLast15[strInstrumentID] = pKLine;
				UpdateKLineData(strInstrumentID,PHRASE_15MIN,pKLine,true,nErrorCode);
				return true;
			}

			if( difftime(pKLine.dwTime, mktime(&(m_tmCloseTime))) >=0 ) {
				UpdateKLineData(strInstrumentID,PHRASE_15MIN,pKLine,false,nErrorCode);
			}
			else {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_15MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast15[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_30MIN:
		{
			map<string,KLineBase >::iterator itLast = m_maptmLast30.find(strInstrumentID);
			if (itLast == m_maptmLast30.end()) {
				m_maptmLast30[strInstrumentID] = pKLine; 
				UpdateKLineData(strInstrumentID,PHRASE_30MIN,pKLine,true,nErrorCode);
				return true;
			}

			if( difftime(pKLine.dwTime, mktime(&(m_tmCloseTime) )) >=0 ) {
				UpdateKLineData(strInstrumentID,PHRASE_30MIN,pKLine,false,nErrorCode);
			}
			else {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_30MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast30[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_60MIN:
		{
			map<string,KLineBase >::iterator itLast = m_maptmLast60.find(strInstrumentID);
			if (itLast == m_maptmLast60.end()) {
				m_maptmLast60[strInstrumentID] = pKLine;
				UpdateKLineData(strInstrumentID,PHRASE_60MIN,pKLine,true,nErrorCode);
				return true;
			}

			if( difftime(pKLine.dwTime,  mktime(&(m_tmCloseTime) )) >=0 )
				UpdateKLineData(strInstrumentID,PHRASE_60MIN,pKLine,false,nErrorCode);
			else {
				if( difftime(pKLine.dwTime, itLast->second.dwTime) == 0) {
					//UpdateKLineData(strName,pKLine,nErrorCode,itLast->second);
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_60MIN,itLast->second,true,nErrorCode);
				}
			}
			m_maptmLast60[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_DAY:
		{
			map<string,KLineBase >::iterator itLast = m_maptmLastDay.find(strInstrumentID);
			if(itLast != m_maptmLastDay.end() && itLast->second.dwTime/(24*3600) == pKLine.dwTime/(24*3600)) {
				if(pKLine.dwTime - mktime(&m_LastDBTimeDay) > m_nTimeSpan ) {
					UpdateKLineData(strInstrumentID,PHRASE_DAY,pKLine,false,nErrorCode); 
					m_LastDBTimeDay = *localtime(&pKLine.dwTime);
				}
			}
			else {
				UpdateKLineData(strInstrumentID,PHRASE_DAY,pKLine,true,nErrorCode);
				m_LastDBTimeDay = *localtime(&pKLine.dwTime);
			}
			m_maptmLastDay[strInstrumentID] = pKLine;
		}
		break;
	case PHRASE_WEEK:
		{
			map<string,KLineBase >::iterator itLast = m_maptmLastWeek.find(strInstrumentID);
			if(itLast != m_maptmLastWeek.end()) {
				struct tm timeCurr = {0};
				struct tm timeLast = {0};
				timeLast = *localtime(&itLast->second.dwTime);
				timeCurr = *localtime(&pKLine.dwTime);
				if(IsOneWeek(timeLast, timeCurr)) {
					if(pKLine.dwTime - mktime(&m_LastDBTimeWeek) > m_nTimeSpan ) {
						UpdateKLineData(strInstrumentID,PHRASE_WEEK,pKLine,false,nErrorCode);
						m_LastDBTimeWeek = *localtime(&pKLine.dwTime);
					}
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_WEEK,pKLine,true,nErrorCode);
					m_LastDBTimeWeek = *localtime(&pKLine.dwTime);
				}
			}
			else {
				UpdateKLineData(strInstrumentID,PHRASE_WEEK,pKLine,true,nErrorCode);
				m_LastDBTimeWeek = *localtime(&pKLine.dwTime);
			}
			m_maptmLastWeek[strInstrumentID] = pKLine;
		}	
		break;
	case PHRASE_MONTH:
		{
			map<string,KLineBase >::iterator itLast = m_maptmLastMonth.find(strInstrumentID);
			if(itLast != m_maptmLastMonth.end()) {
				struct tm timeCurr = {0};
				struct tm timeLast = {0};
				timeLast = *localtime(&itLast->second.dwTime);
				timeCurr = *localtime(&pKLine.dwTime);
				if(IsOneMonth(timeLast, timeCurr)) {
					if(pKLine.dwTime - mktime(&m_LastDBTimeMonth) > m_nTimeSpan ) {
						UpdateKLineData(strInstrumentID,PHRASE_MONTH,pKLine,false,nErrorCode);
						m_LastDBTimeMonth = *localtime(&pKLine.dwTime);
					}
				}
				else {
					UpdateKLineData(strInstrumentID,PHRASE_MONTH,pKLine,true,nErrorCode);
					m_LastDBTimeMonth = *localtime(&pKLine.dwTime);
				}
			}
			else {
				UpdateKLineData(strInstrumentID,PHRASE_MONTH,pKLine,true,nErrorCode);
				m_LastDBTimeMonth = *localtime(&pKLine.dwTime);
			}
			m_maptmLastMonth[strInstrumentID] = pKLine;
		}	
		break;
	}

	return true;
}

bool UpdateKLineData(string& strInstrumentID, EnumPhrase phrase, KLineBase& pKLine, bool bInsert, int& nErrorCode)
{
	char szTableName[64];
	char szSQL[260];

	memset(szTableName, 0, sizeof(szTableName));

	strncpy(szTableName, MakeTableName(strInstrumentID, phrase).c_str(), sizeof(szTableName)-1);

	set<string>::iterator it = m_setTableName.find(szTableName);
	if(it == m_setTableName.end()) {
		memset(szSQL, 0, sizeof(szSQL));
		sprintf(szSQL,	"create table %s"
			"(KLineTime          INTEGER PRIMARY KEY,"
			" OpenPrice          binary_double,"
			" HighestPrice       binary_double,"
			" LowestPrice        binary_double,"
			" Closeprice         binary_double,"
			" Turnover           binary_double,"
			" Volume             INTEGER,"
			" OpenInterest       INTEGER,"
			" InsertDBTime       DATE"
			") NOLOGGING",
			szTableName);
		CInterface_SvrDBOpr::getObj().Excute( szSQL, nErrorCode );
		m_setTableName.insert(szTableName);
	}

	memset(szSQL, 0, sizeof(szSQL));
	if(bInsert) {
		sprintf(szSQL,	"Insert Into %s Values(%I64d," 
			"TO_BINARY_DOUBLE('%g'), "
			"TO_BINARY_DOUBLE('%g'), "
			"TO_BINARY_DOUBLE('%g'), "
			"TO_BINARY_DOUBLE('%g'), "
			"TO_BINARY_DOUBLE('%g'), "
			"%u, %u, sysdate)",
			szTableName, pKLine.dwTime, 
			pKLine.fOpenPrice, pKLine.fHighPrice, 
			pKLine.fLowPrice, pKLine.fClosePrice, 
			pKLine.fTurnover, pKLine.dwVolume, pKLine.dwOpenInterest);
	}
	else {
		sprintf(szSQL,	"Update %s t Set "
			"OpenPrice = TO_BINARY_DOUBLE('%g'), "
			"HighestPrice = TO_BINARY_DOUBLE('%g'), "
			"LowestPrice = TO_BINARY_DOUBLE('%g'), "
			"Closeprice = TO_BINARY_DOUBLE('%g'), "
			"Turnover = TO_BINARY_DOUBLE('%g'), "
			"Volume = %u, OpenInterest = %u Where t.KLineTime = %I64d",
			szTableName, 
			pKLine.fOpenPrice, pKLine.fHighPrice, 
			pKLine.fLowPrice, pKLine.fClosePrice, 
			pKLine.fTurnover, pKLine.dwVolume, pKLine.dwOpenInterest, 
			pKLine.dwTime);
	}
	bool brlt=CInterface_SvrDBOpr::getObj().Excute( szSQL, nErrorCode );
	return brlt;
}

string MakeTableName(string& strInstrumentID, EnumPhrase phrase)
{
	char szTableName[64];

	memset(szTableName, 0, sizeof(szTableName));

	if(phrase==PHRASE_1MIN || phrase==PHRASE_5MIN || phrase==PHRASE_15MIN || phrase==PHRASE_30MIN || phrase==PHRASE_60MIN) {
		sprintf_s(szTableName, sizeof(szTableName)-1, "KLineM%d_%s", phrase, strInstrumentID.c_str());
	}
	else if(phrase==PHRASE_DAY) {
		sprintf_s(szTableName, sizeof(szTableName)-1, "KLineDay_%s", strInstrumentID.c_str());
	}
	else if(phrase==PHRASE_WEEK) {
		sprintf_s(szTableName, sizeof(szTableName)-1, "KLineWeek_%s", strInstrumentID.c_str());
	}
	else if(phrase==PHRASE_MONTH) {
		sprintf_s(szTableName, sizeof(szTableName)-1, "KLineMonth_%s", strInstrumentID.c_str());
	}
	return szTableName;
}
struct tm GetTimeInSpan(struct tm tmPara, int nSpan ,vector<struct tm>& vecTimeScales)
{
	time_t tmMax =   mktime(&vecTimeScales[vecTimeScales.size()-1]);
	time_t tReturn;

	//用这个值判断成功否e
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
	{	//原代码，行情时间小于开盘时间时处理为开盘后第一段时间内行情	
		/*time_t t1 = mktime( &vecTimeScales[0]);
		t1 += nSpan;
		struct tm tmReturn = *localtime(&t1);
		return tmReturn; */

		//修改为，行情时间小于开盘时间 剔除掉
		struct tm Return;
		memset( &Return,0,sizeof(tm));
		return Return;

	}
	else if( nIndex%2==0)
	{
		time_t t1 = mktime( &vecTimeScales[nIndex-1]);
		t1 -= 1;//
		struct tm tmReturn = *localtime(&t1);
		return GetTimeInSpan(tmReturn, nSpan,vecTimeScales);
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

bool IsOneWeek(struct tm tm1, struct tm tm2)
{

	if(tm1.tm_year != tm2.tm_year)
		return false;
	int nDay1 = GetDayOfYear(tm1);
	int nDay2 = GetDayOfYear(tm2);

	if(nDay1/7 +1 != nDay2/7+1)
		return false;

	return true;
}

bool IsOneMonth(struct tm tm1, struct tm tm2)
{
	if((tm1.tm_year == tm2.tm_year) && (tm1.tm_mon == tm2.tm_mon))
		return true;
	return false;
}

int GetDayOfYear(struct tm tm1)
{
	int day=0;

	int month[13] = { 0, 31, 28, 31, 30, 31, 30,31, 31, 30, 31, 30, 31 };
	if(tm1.tm_year % 400 == 0 || tm1.tm_year % 100 != 0 && tm1.tm_year % 4 == 0) 
	{
		month[2] = 29;
	}
	if(tm1.tm_mon >= 1 && 
		tm1.tm_mon <= 12 && 
		tm1.tm_mday >= 1 && 
		tm1.tm_mday <= month[tm1.tm_mon]) 
	{
		day = tm1.tm_mday;
		for(int i = 1; i < tm1.tm_mon; ++i) 
		{
			day += month[i];
		}
	}
	return day;
}