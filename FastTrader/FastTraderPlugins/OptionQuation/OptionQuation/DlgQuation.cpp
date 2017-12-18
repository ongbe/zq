// DlgQuation.cpp : implementation file
//

#include "stdafx.h"
#include "OptionQuation.h"
#include "DlgQuation.h"
#include "TCPDataService.h"
#include "TCPHistoryService.h"
#include "TechIndexConfigDlg.h"
#include "TechIndexExpressDlg.h"
// CDlgQuation dialog

IMPLEMENT_DYNAMIC(CDlgQuation, CDialog)
CDlgQuation::CDlgQuation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuation::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_Quation);

	m_bConnectHQServer			= false;
	m_bConnectHQHistoryServer	= false;

	m_enumPhrase = PHRASE_DAY;
	m_strInstrument = "";
	m_strTradingDay ="20140828";
	m_strUpdateTime = "";

	m_callbackGet = NULL;
	nQueryIndex = 0;
}

CDlgQuation::~CDlgQuation()
{
}

void CDlgQuation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgQuation, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_MESSAGE(MESSAGE_QutoInTime, OnMessageQutoInTime)	
	ON_MESSAGE(MESSAGE_CONNECTSUCCESS, OnMessageConnectSuccess)
	ON_MESSAGE(MESSAGE_CONNECTFAIL, OnMessageConnectfail)
	ON_MESSAGE(MESSAGE_CHANGETimeSPAN, OnMessageChangeTimeSpan)
	ON_MESSAGE(MESSAGE_QutoTLineHistory, OnMessageQutoTLineHistory)	
	ON_MESSAGE(MESSAGE_QutoKLineHistory, OnMessageQutoKLineHistory)	
	ON_MESSAGE(MESSAGE_INDEX_EXPRESS, OnMessageIndexExpress)	
	ON_MESSAGE(MESSAGE_INDEX_CONFIG, OnMessageIndexConfig)	
	ON_MESSAGE(MESSAGE_CONFIG_CHANGE, OnMessageConfigChange)	
	
END_MESSAGE_MAP()
BOOL CDlgQuation::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//CHeaderCtrl::OnEraseBkgnd(pDC);
}

// CDlgQuation message handlers
BOOL CDlgQuation::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetWindowText("行情");

	CRect rtClient;
	GetClientRect(&rtClient);
	
	m_QView.Create(IDD_DIALOG_QView, this);	
	m_QView.MoveWindow(rtClient);	
	m_QView.ShowWindow(SW_SHOW);

	
/*
//////////////////////////////////////////////////////////////////////////
	ReadTLineFileTest();
	//string strTimes = "201408120900201408121015 201408121030201408121130 201408121315201408121500";
	string strTimes = "201408120915201408121130  201408121300201408121515";
	vector<struct tm> vecTimeScales;
	SetTodayScales(strTimes,  vecTimeScales);
	m_FData.SetScales(vecTimeScales);//分时交易区间
//////////////////////////////////////////////////////////////////////////

	m_QView.m_TlineView.SetLastClosePrice(2373.60);//必须最先调用
	m_QView.m_TlineView.SetVolumeMultiple(300);
	m_QView.m_TlineView.SetScales(vecTimeScales);
	std::sort(m_FData.m_vecTLine.begin(),m_FData.m_vecTLine.end());
	m_QView.m_TlineView.SetFuture("zjs", "IF1410");
	m_QView.m_TlineView.SetVectorData(m_FData.m_vecTLine);
	
	ReadFileTest_KLineDay();
	std::sort(m_FData.m_vecKLineDay.begin(),m_FData.m_vecKLineDay.end());
	
	m_QView.m_KlineView.SetFuture("zjs", "IF1410");
	m_QView.m_KlineView.SetPhrase(PHRASE_DAY);
	m_QView.m_KlineView.SetVectorData(m_FData.m_vecKLineDay);

	ReadFileTest_KLine60();
	std::sort(m_FData.m_vecKLineMin60.begin(),m_FData.m_vecKLineMin60.end());
	m_QView.m_KlineView.SetFuture("zjs", "IF1410");
	m_QView.m_KlineView.SetPhrase(PHRASE_60MIN);
	m_QView.m_KlineView.SetVectorData(m_FData.m_vecKLineMin60);
*/
	Load(m_HisSvrIP, m_HisSvrPort, m_IntimeQuotSvrIP, m_IntimeQuotSvrPort);
	CTCPDataService::GetInstance()->SetMessWnd(m_hWnd);
	CTCPDataService::GetInstance()->Connect(m_IntimeQuotSvrIP, m_IntimeQuotSvrPort);	
	
	CTCPHistoryService::GetInstance()->SetMessWnd(m_hWnd);
	CTCPHistoryService::GetInstance()->Connect(m_HisSvrIP, m_HisSvrPort);	

	return TRUE; 
}
void CDlgQuation::Load(std::string& HisSvrIP, unsigned short& HisSvrPort, std::string& IntimeQuotSvrIP, unsigned short& IntimeQuotSvrPort)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath)-1 );

	string strPathFileName( szLocalPath );
	int nPos = strPathFileName.rfind( '\\' );
	if ( nPos == -1) return;
		strPathFileName = strPathFileName.substr( 0, nPos + 1 ) + string("QuotServerConfig.xml");
	string strValue;	

	TiXmlDocument* pDocument=new TiXmlDocument(strPathFileName.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			const char* pRootElement = RootElement->Value();
			if(strcmp(pRootElement, "root") ==0)
			{
				TiXmlNode* pNode=RootElement->FirstChild("QuotServer_InTime");
				if(pNode)
				{
					const char* pElement = pNode->Value();
					if(strcmp(pElement, "QuotServer_InTime") ==0)
					{
						TiXmlElement* pChild=pNode->FirstChildElement("IP");
						if(pChild)
						{
							std::string str = pChild->GetText();
							IntimeQuotSvrIP = str;							
						}
						TiXmlElement* pChild2=pNode->FirstChildElement("Port");
						if(pChild)
						{
							std::string str = pChild2->GetText();
							IntimeQuotSvrPort = atoi(str.c_str());							
						}
					}
				}
				TiXmlNode* pNodeHistory=RootElement->FirstChild("QuotServer_History");
				if(pNodeHistory)
				{
					const char* pElement = pNodeHistory->Value();
					if(strcmp(pElement, "QuotServer_History") ==0)
					{
						TiXmlElement* pChild=pNodeHistory->FirstChildElement("IP");
						if(pChild)
						{
							std::string str = pChild->GetText();
							m_HisSvrIP = str;							
						}
						TiXmlElement* pChild2=pNodeHistory->FirstChildElement("Port");
						if(pChild)
						{
							std::string str = pChild2->GetText();
							m_HisSvrPort = atoi(str.c_str());							
						}
					}
				}
			}			
		}
	}
}
void CDlgQuation::SetTodayScales(string strTimes, vector<struct tm>& vecTimeScales)
{//201408120915201408121130 201408121300201408121515
	vector<string> vecTime;
	split(strTimes, ' ',vecTime);
	for(int i=0; i<(int)vecTime.size(); i++)
	{
		string strTime = vecTime[i];
		if(strTime.length() != 24)
			continue;

		struct tm tmScale;
		tmScale.tm_year = atoi(strTime.substr(0, 4).c_str()) - 1900;
		tmScale.tm_mon  = atoi(strTime.substr(4, 2).c_str()) - 1;
		tmScale.tm_mday = atoi(strTime.substr(6, 2).c_str());
		tmScale.tm_hour = atoi(strTime.substr(8, 2).c_str());
		tmScale.tm_min  = atoi(strTime.substr(10, 2).c_str());
		tmScale.tm_sec  = 0;		
		vecTimeScales.push_back(tmScale);

		tmScale.tm_year = atoi(strTime.substr(12, 4).c_str()) - 1900;
		tmScale.tm_mon  = atoi(strTime.substr(16, 2).c_str()) - 1;
		tmScale.tm_mday = atoi(strTime.substr(18, 2).c_str());
		tmScale.tm_hour = atoi(strTime.substr(20, 2).c_str());
		tmScale.tm_min  = atoi(strTime.substr(22, 2).c_str());
		vecTimeScales.push_back(tmScale);
	}
}/*
void CDlgQuation::ReadTLineFileTest()
{
	const FILENAMECHAR* filename = "c:\\T.txt";
	CTextFileRead myfile(filename);
	if(!myfile.IsOpen())
	{		
		return;
	}
	std::string strAll;
	myfile.Read(strAll);
	vector<string> vec;
	split(strAll, '\n', vec);
	for(int i =0; i < (int)vec.size(); i++)
	{		
		std::string strOne = vec[i];
		
		vector<string> vecOne;
		split(strOne, '\t', vecOne);
		if(vecOne.size() != 4)
			continue;

		std::string str = vecOne[0];
		if(str.length() < 14)
			continue;

		if( i >100)
		{
			int o=0;
		}
		STLine TLine;
		TLine.dwTime.tm_year = atoi(str.substr(0, 2).c_str()) +2000- 1900;
		TLine.dwTime.tm_mon  = atoi(str.substr(3, 2).c_str()) - 1;
		TLine.dwTime.tm_mday = atoi(str.substr(6, 2).c_str());
		TLine.dwTime.tm_hour = atoi(str.substr(9, 2).c_str());
		TLine.dwTime.tm_min  = atoi(str.substr(12, 2).c_str());
		TLine.dwTime.tm_sec  = 0; 
		TLine.fLastPrice = atoi(vecOne[1].c_str());
		TLine.dwVolume   = atoi(vecOne[2].c_str());
		TLine.dwVolumeMoney   = atoi(vecOne[3].c_str());
		m_FData.m_vecTLine.push_back(TLine);	
	}

}
void CDlgQuation::ReadFileTest_KLineDay()
{
	const FILENAMECHAR* filename = "c:\\kDay.txt";
	CTextFileRead myfile(filename);
	if(!myfile.IsOpen())
	{		
		return;
	}
	std::string strAll;
	myfile.Read(strAll);
	vector<string> vec;
	split(strAll, '\n', vec);
	for(int i =0; i < (int)vec.size(); i++)
	{		
		std::string strOne = vec[i];

		vector<string> vecOne;
		split(strOne, '\t', vecOne);
		if(vecOne.size() != 14)
			continue;

		std::string str = vecOne[0];
		if(str.length() < 10)
			continue;

		if( i >100)
		{
			int o=0;
		}
		SKLine KLine;
		KLine.dwTime.tm_year = atoi(str.substr(0, 4).c_str()) - 1900;
		KLine.dwTime.tm_mon  = atoi(str.substr(5, 2).c_str()) - 1;
		KLine.dwTime.tm_mday = atoi(str.substr(8, 2).c_str());
		KLine.dwTime.tm_hour = 0;
		KLine.dwTime.tm_min  = 0;
		KLine.dwTime.tm_sec  = 0; 
		KLine.fOpenPrice = atoi(vecOne[1].c_str());
		KLine.fHighPrice = atoi(vecOne[2].c_str());
		KLine.fLowPrice  = atoi(vecOne[3].c_str());
		KLine.fClosePrice= atoi(vecOne[4].c_str());
		KLine.dwVolume   = atoi(vecOne[5].c_str());
		KLine.fSumTradeVolume  = atoi(vecOne[6].c_str());
		m_FData.m_vecKLineDay.push_back(KLine);	
	}
}
void CDlgQuation::ReadFileTest_KLine60()
{
	const FILENAMECHAR* filename = "c:\\k60.txt";
	CTextFileRead myfile(filename);
	if(!myfile.IsOpen())
	{		
		return;
	}
	std::string strAll;
	myfile.Read(strAll);
	vector<string> vec;
	split(strAll, '\n', vec);
	for(int i =0; i < (int)vec.size(); i++)
	{		
		std::string strOne = vec[i];

		vector<string> vecOne;
		split(strOne, '\t', vecOne);
		if(vecOne.size() != 14)
			continue;

		std::string str = vecOne[0];
		if(str.length() < 14)
			continue;

		if( i >100)
		{
			int o=0;
		}
		SKLine KLine;
		
		KLine.dwTime.tm_year = atoi(str.substr(0, 2).c_str()) +2000- 1900;
		KLine.dwTime.tm_mon  = atoi(str.substr(3, 2).c_str()) - 1;
		KLine.dwTime.tm_mday = atoi(str.substr(6, 2).c_str());
		KLine.dwTime.tm_hour = atoi(str.substr(9, 2).c_str());
		KLine.dwTime.tm_min  = atoi(str.substr(12, 2).c_str());
		KLine.dwTime.tm_sec  = 0; 
		KLine.fOpenPrice = atoi(vecOne[1].c_str());
		KLine.fHighPrice = atoi(vecOne[2].c_str());
		KLine.fLowPrice  = atoi(vecOne[3].c_str());
		KLine.fClosePrice= atoi(vecOne[4].c_str());
		KLine.dwVolume   = atoi(vecOne[5].c_str());
		KLine.fSumTradeVolume  = atoi(vecOne[6].c_str());
		m_FData.m_vecKLineMin60.push_back(KLine);	
	}

}
*/
void CDlgQuation::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rtClient;
	GetClientRect(&rtClient);
	if(::IsWindow(m_QView.GetSafeHwnd()))	
		m_QView.MoveWindow(rtClient);
}
LRESULT CDlgQuation::OnMessageQutoInTime(WPARAM wP,LPARAM lP)
{
	PlatformStru_DepthMarketData* pMarketData = (PlatformStru_DepthMarketData*)wP;
	if(pMarketData == NULL)
		return 0;

	string strID = pMarketData->InstrumentID;
	map<string,  CFData>::iterator itFData = m_mapID2FData.find(strID);
	
	if(itFData ==  m_mapID2FData.end())
		return 0;

	CFData& fdata = itFData->second;
	bool bUpdate = false;
	if(m_strSelFData == fdata.m_strInstrument)
		bUpdate = true;
	OnDataInTime(pMarketData, &fdata, bUpdate);		
	
	return 1L;
}
void CDlgQuation::OnDataInTime(PlatformStru_DepthMarketData* pData, CFData* fdata, bool bUpdate)
{
	if(fdata == NULL || pData == NULL)
		return;	

	CString strDate = m_strTradingDay.c_str();
	CString strFutureID = pData->InstrumentID;
	if(isInvalidValue(pData->LastPrice)) 
	{		
		if(isInvalidValue(pData->ClosePrice))
			pData->ClosePrice = 0;
		if(isInvalidValue(pData->LastPrice))
			pData->LastPrice = 0;
		if(isInvalidValue(pData->OpenPrice))
			pData->OpenPrice = 0;
		if(isInvalidValue(pData->HighestPrice))
			pData->HighestPrice = 0;
		if(isInvalidValue(pData->LowestPrice))
			pData->LowestPrice = 0;
		if(isInvalidValue(pData->Volume))
			pData->Volume = 0;
		if(isInvalidValue(pData->BidPrice1))
			pData->BidPrice1 = 0;
		if(isInvalidValue(pData->AskPrice1))
			pData->AskPrice1 = 0;
		return;
	}	
	if(isInvalidValue(pData->BidPrice1))
		pData->BidPrice1 = 0;
	if(isInvalidValue(pData->AskPrice1))
		pData->AskPrice1 = 0;
	AppendLog_DepthMarketData(pData);
	fdata->SetDataInTime(pData);
	
	if(bUpdate)
	{
		m_QView.m_TlineView.SetLastClosePrice(fdata->m_PreSettlementPrice);		
		m_QView.m_TlineView.SetInTimeData(*fdata);		
		m_QView.m_KlineView.SetInTimeData(*fdata);			
	}
}
void CDlgQuation::SetTradingDay(std::string strTradingDay)
{
	m_strTradingDay = strTradingDay;
}
void CDlgQuation::InitFinish(std::string strInstrument)
{
	map<string,  CFData>::iterator itFData = m_mapID2FData.find(strInstrument);
	if(itFData ==  m_mapID2FData.end())
	{		
		return;
	}
	CFData& fdata = itFData->second;

	m_QView.m_TlineView.SetLastClosePrice(fdata.m_PreSettlementPrice);//必须最先调用
	m_QView.m_TlineView.SetVolumeMultiple(fdata.m_VolumeMultiple);
	m_QView.m_TlineView.SetScales(fdata.m_vecTimeScales);
	//std::sort(m_FData.m_vecTLine.begin(),m_FData.m_vecTLine.end());
	m_QView.m_TlineView.SetFuture("", strInstrument.c_str());
	
	vector<STLine> vec;
	bool bGet = fdata.GetVecTLine(vec);
	m_QView.m_TlineView.SetVectorData(vec);


	m_QView.m_KlineView.SetFuture("", strInstrument.c_str());
	m_QView.m_KlineView.SetPhrase(m_enumPhrase);	
	SetPhraseVector(m_enumPhrase, fdata);	

	if(m_bConnectHQServer)
	{
		SubscribleInstrument(strInstrument);
	}
	else
	{
		m_strInstrument = strInstrument;
	}
}
void CDlgQuation::ShowInstrument(std::string strInstrument)
{		
	m_strSelFData = strInstrument;
	m_queueInstrument.push(strInstrument);
	if(m_queueInstrument.size() >1)
	{
		std::string str = m_queueInstrument.front();
		m_queueInstrument.pop();
		DelFData(str);
	}
	CFData fdata;
	fdata.m_strTradingDay = m_strTradingDay;
	fdata.m_strInstrument = strInstrument;	
	//std::sort(fdata.m_vecTLine.begin(),fdata.m_vecTLine.end());
	m_mapID2FData.insert(make_pair(strInstrument, fdata));
}
bool CDlgQuation::SetInstrumentInfo(PlatformStru_InstrumentInfo& InstrumentData)
{
	map<string,  CFData>::iterator itFData = m_mapID2FData.find(InstrumentData.InstrumentID);
	if(itFData !=  m_mapID2FData.end())
	{		
		return false;
	}

	CFData& fData = itFData->second;
	fData.m_ExchangeName	= InstrumentData.ExchangeID;
	fData.m_strInstrument   = InstrumentData.InstrumentName;	
	fData.m_VolumeMultiple	= InstrumentData.VolumeMultiple;	
	fData.m_PriceTick       = InstrumentData.PriceTick;

	return true;
}
void CDlgQuation::SetTradingScales(std::string& strInstrument, std::string& strTime)
{
	map<string,  CFData>::iterator itFData = m_mapID2FData.find(strInstrument);
	if(itFData ==  m_mapID2FData.end())
	{		
		return;
	}
	vector<struct tm> vecTimeScales;
	SetTodayScales(strTime,  vecTimeScales);

	CFData& fData = itFData->second;
	fData.SetScales(vecTimeScales);//分时交易区间
	fData.ReadFile_TLine();//readfile 要在得到刻度之后
	fData.ReadFile(m_enumPhrase);
	QueryHistoryTData(fData);

}
void CDlgQuation::QueryHistoryTData(CFData& fData)
{
	std::string strBeginTime = "00:00:00";
	vector<STLine> vec;
	bool bGet = fData.GetVecTLine(vec);
	if(vec.size())
	{
		STLine& tLine = vec[0];	

		time_t t1 = mktime( &tLine.dwTime);
		struct tm tmReturn = *localtime(&t1);

		struct tm tmPre = fData.GetTimeInSpan_TLine(tmReturn, 60);//最后一分钟

		char szTime[256];
		memset(szTime, 0 , sizeof(szTime));
		sprintf(szTime,"%2d:%2d:%2d", tmPre.tm_hour, tmPre.tm_min, tmPre.tm_sec);
		strBeginTime = szTime;		
	}
	
	SGetInfo sGetInfo;
	if(m_callbackGet)
		m_callbackGet(sGetInfo);
	if(sGetInfo.str == "")
		sGetInfo.str= "23:59:59";
	std::string strEndTime = sGetInfo.str;

	KLineQuery kQuery;
	memset(&kQuery, 0, sizeof(KLineQuery));
	sprintf(kQuery.szStartTime,"%s %s", m_strTradingDay.c_str(),  strBeginTime.c_str());
	sprintf(kQuery.szEndTime,"%s %s", m_strTradingDay.c_str(),  strEndTime.c_str());
	strcpy(kQuery.InstrumentID, fData.m_strInstrument.c_str());
	kQuery.nDataType = 2;

	nQueryIndex++;
	m_mapIndex2QueryInfo.insert(make_pair(nQueryIndex, kQuery));
	CTCPHistoryService::GetInstance()->SendData(Cmd_QueryQuotTimeLineData_Req, (void *)&kQuery, sizeof(KLineQuery), nQueryIndex);	
}
void CDlgQuation::SetDepthMarketData(std::string& strInstrument, PlatformStru_DepthMarketData* QutoData)
{
	if(!QutoData)
		return;
	map<string,  CFData>::iterator itFData = m_mapID2FData.find(strInstrument);
	if(itFData ==  m_mapID2FData.end())
	{		
		return;
	}
	CFData& fData = itFData->second;
	fData.m_UpperLimitPrice    = QutoData->UpperLimitPrice;
	fData.m_LowerLimitPrice    = QutoData->LowerLimitPrice;
	fData.m_PreSettlementPrice = QutoData->PreSettlementPrice;
	if(isInvalidValue(QutoData->HighestPrice))
	{//如果最高最低价无效，则以这单为第一笔数据，兼容组合单
		double dbMaxSpan = abs(QutoData->LastPrice - QutoData->PreSettlementPrice);		
		if(abs(fData.m_fMaxValue	 - MIN_VALUE_DEFAULT)<0.00001 && abs(fData.m_fMinValue-MAX_VALUE_DEFAULT)<0.00001)
		{
			fData.m_fMaxValue = QutoData->PreSettlementPrice + dbMaxSpan;
			fData.m_fMinValue = QutoData->PreSettlementPrice - dbMaxSpan;
		}
		else
		{
			double fMaxValue = QutoData->PreSettlementPrice + dbMaxSpan;
			double fMinValue = QutoData->PreSettlementPrice - dbMaxSpan;
			if(fData.m_fMaxValue < fMaxValue)
				fData.m_fMaxValue = fMaxValue;
			if(fData.m_fMinValue > fMinValue)
				fData.m_fMinValue = fMinValue;
		}
	}
	else
	{
		fData.m_fMaxValue			 = QutoData->HighestPrice;
		fData.m_fMinValue			 = QutoData->LowestPrice;
	}

}
void CDlgQuation::DelFData(std::string strInstrument)
{
	map<string, CFData>::iterator it = m_mapID2FData.find(strInstrument);
	if(it != m_mapID2FData.end())
	{
		CFData& fData = it->second;
		fData.SaveFile();
		fData.SaveFile_TLine();
		
		m_mapID2FData.erase(strInstrument);
		UnSubscribleInstrument(strInstrument);
	}		
}
void  CDlgQuation::UnSubscribleInstrument(std::string strInstrument)
{
	CTCPDataService::GetInstance()->SendData(Cmd_UnSubscribeQuotEvent_Req, (void *)strInstrument.c_str(), strlen(strInstrument.c_str())+1, 0);
}
LRESULT CDlgQuation::OnMessageConnectSuccess(WPARAM wP,LPARAM lP)
{
	int& n = *(int *)wP;
	if(n == 0)
	{
		m_bConnectHQServer = true;
		SubscribleInstrument(m_strInstrument);
		m_strInstrument = "";
	}
	else if(n ==1)
	{
		m_bConnectHQHistoryServer = true;
	}
	return 1L;
}
LRESULT CDlgQuation::OnMessageConnectfail(WPARAM wP,LPARAM lP)
{
	int& n = *(int *)wP;
	if(n == 0)
		m_bConnectHQServer = false;
	else if(n ==1)
		m_bConnectHQHistoryServer = false;

	return 1L;
}
void  CDlgQuation::SubscribleInstrument(std::string strInstrument)
{
	if(strInstrument == "")
		return ;
	map<string,  CFData>::iterator itFData = m_mapID2FData.find(strInstrument);
	if(itFData ==  m_mapID2FData.end())
	{		
		return;
	}
	CTCPDataService::GetInstance()->SendData(Cmd_SubscribeQuotEvent_Req, (void *)strInstrument.c_str(), strlen(strInstrument.c_str())+1, 0);

}
LRESULT CDlgQuation::OnMessageChangeTimeSpan(WPARAM wP,LPARAM lP)
{
	m_enumPhrase = *(EnumPhrase*)wP;
	m_QView.m_KlineView.SetPhrase(m_enumPhrase);

	map<string,  CFData>::iterator itFData = m_mapID2FData.find(m_strSelFData);
	if(itFData ==  m_mapID2FData.end())
		return 0;

	CFData& fdata = itFData->second;	
	fdata.ReadFile(m_enumPhrase);
	SetPhraseVector(m_enumPhrase, itFData->second);	
	return 1L;
}
void CDlgQuation::SetPhraseVector(EnumPhrase enumPhrase, CFData& fdata)
{
	m_QView.m_KlineView.SetPhrase(enumPhrase);
	fdata.ReadFile(enumPhrase);
	switch(enumPhrase)
	{
		case PHRASE_1MIN:
			{	
				vector<SKLine> vec;
				bool bGet = fdata.GetVecKLine(PHRASE_1MIN, vec);	
				if(bGet)
				{
					std::vector<std::string> vecTechNames;
					EnumPhrase enumPhrase;
					m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
					for(int i =0; i< (int)vecTechNames.size(); i++)
					{
						STechCalcPara sTCPara;
						sTCPara.enumPhrase = m_enumPhrase;
						sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
						fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
					}
					m_QView.m_KlineView.SetVectorData(vec);
					
				}
			}
			break;
		case PHRASE_5MIN:
			{	
				vector<SKLine> vec;
				bool bGet = fdata.GetVecKLine(PHRASE_5MIN, vec);				
				if(bGet)
				{
					std::vector<std::string> vecTechNames;
					EnumPhrase enumPhrase;
					m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
					for(int i =0; i< (int)vecTechNames.size(); i++)
					{
						STechCalcPara sTCPara;
						sTCPara.enumPhrase = m_enumPhrase;
						sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
						fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
					}
					m_QView.m_KlineView.SetVectorData(vec);
				}
			}
			break;
		case PHRASE_15MIN:
			{
				vector<SKLine> vec;
				bool bGet = fdata.GetVecKLine(PHRASE_15MIN, vec);					
				if(bGet)
				{
					std::vector<std::string> vecTechNames;
					EnumPhrase enumPhrase;
					m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
					for(int i =0; i< (int)vecTechNames.size(); i++)
					{
						STechCalcPara sTCPara;
						sTCPara.enumPhrase = m_enumPhrase;
						sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
						fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
					}
					m_QView.m_KlineView.SetVectorData(vec);
				}
			}
			break;
		case PHRASE_30MIN:
			{
				vector<SKLine> vec;
				bool bGet = fdata.GetVecKLine(PHRASE_30MIN, vec);				
				if(bGet)
				{
					std::vector<std::string> vecTechNames;
					EnumPhrase enumPhrase;
					m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
					for(int i =0; i< (int)vecTechNames.size(); i++)
					{
						STechCalcPara sTCPara;
						sTCPara.enumPhrase = m_enumPhrase;
						sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
						fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
					}
					m_QView.m_KlineView.SetVectorData(vec);
				}
			}
			break;
		case PHRASE_60MIN:
			{
				vector<SKLine> vec;
				bool bGet = fdata.GetVecKLine(PHRASE_60MIN, vec);				
				if(bGet)
				{
					std::vector<std::string> vecTechNames;
					EnumPhrase enumPhrase;
					m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
					for(int i =0; i< (int)vecTechNames.size(); i++)
					{
						STechCalcPara sTCPara;
						sTCPara.enumPhrase = m_enumPhrase;
						sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
						fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
					}
					m_QView.m_KlineView.SetVectorData(vec);
				}
			}
			break;
		case PHRASE_DAY:
			{		
				vector<SKLine> vec;
				bool bGet = fdata.GetVecKLine(PHRASE_DAY, vec);						
				if(bGet)
				{
					std::vector<std::string> vecTechNames;
					EnumPhrase enumPhrase;
					m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
					for(int i =0; i< (int)vecTechNames.size(); i++)
					{
						STechCalcPara sTCPara;
						sTCPara.enumPhrase = m_enumPhrase;
						sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
						fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
					}
					m_QView.m_KlineView.SetVectorData(vec);
				}
			}
			break;
		case PHRASE_WEEK:
			{	
				vector<SKLine> vec;
				bool bGet = fdata.GetVecKLine(PHRASE_WEEK, vec);				
				if(bGet)
				{
					std::vector<std::string> vecTechNames;
					EnumPhrase enumPhrase;
					m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
					for(int i =0; i< (int)vecTechNames.size(); i++)
					{
						STechCalcPara sTCPara;
						sTCPara.enumPhrase = m_enumPhrase;
						sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
						fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
					}
					m_QView.m_KlineView.SetVectorData(vec);
				}		
			}
			break;
		case PHRASE_MONTH:
			{			
				vector<SKLine> vec;
				bool bGet = fdata.GetVecKLine(PHRASE_MONTH, vec);						
				if(bGet)
				{
					std::vector<std::string> vecTechNames;
					EnumPhrase enumPhrase;
					m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
					for(int i =0; i< (int)vecTechNames.size(); i++)
					{
						STechCalcPara sTCPara;
						sTCPara.enumPhrase = m_enumPhrase;
						sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
						fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
					}
					m_QView.m_KlineView.SetVectorData(vec);
				}
			}
			break;		
	}
	QueryHistoryKLine(fdata, enumPhrase);
}	
void CDlgQuation::SetCallBackFunction(CallBackGetTime callbackGet)
{
	m_callbackGet = callbackGet;
}
LRESULT CDlgQuation::OnMessageQutoTLineHistory(WPARAM wP,LPARAM lP)
{//获取分时历史数据
	std::string strInstrument ="";
	int n = lP;
	if(n ==0)
		return 0;
	std::vector<TimeLineData>& vecTimeLineData = *(std::vector<TimeLineData>*)wP;
	if(vecTimeLineData.size())
	{
		TimeLineData& timeLineData = vecTimeLineData[0];
		strInstrument = timeLineData.InstrumentID;
	}
	else
	{//如果没有返回，则意味着服务端没有数据了，请求结束了
		int nQueryInfo = (int)lP;
		map<int, KLineQuery>::iterator it =	m_mapIndex2QueryInfo.find(nQueryInfo);
		if(it == m_mapIndex2QueryInfo.end())
			return 0;

		KLineQuery& kLineQuery = it->second;
		strInstrument = kLineQuery.InstrumentID;		
	}
	map<string,  CFData>::iterator itFData = m_mapID2FData.find(strInstrument);
	if(itFData ==  m_mapID2FData.end())
		return 0;

	CFData& fdata = itFData->second;	
	fdata.AppendHistoryTLineData(vecTimeLineData);
	fdata.AppendRealTimeTLineData();

	m_QView.m_TlineView.SetInTimeData(fdata);		
	return 1L;
}
void CDlgQuation::QueryHistoryKLine(CFData& fData, EnumPhrase enumPhrase)
{
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{	
			QueryHistoryKLine_Mins(fData, enumPhrase, QUERY_MIN1_DAYCOUNT);
			//QueryHistoryKLine_Min1(fData);
		}
		break;
	case PHRASE_5MIN:
		{			
			QueryHistoryKLine_Mins(fData, enumPhrase, QUERY_MIN5_DAYCOUNT);
		}
		break;
	case PHRASE_15MIN:
		{
			QueryHistoryKLine_Mins(fData, enumPhrase, QUERY_MIN15_DAYCOUNT);
		}
		break;
	case PHRASE_30MIN:
		{
			QueryHistoryKLine_Mins(fData, enumPhrase, QUERY_MIN30_DAYCOUNT);
		}
		break;
	case PHRASE_60MIN:
		{
			QueryHistoryKLine_Mins(fData, enumPhrase, QUERY_MIN60_DAYCOUNT);
		}
		break;
	case PHRASE_DAY:
		{				
			QueryHistoryKLine_Mins(fData, enumPhrase, QUERY_DAY_DAYCOUNT);
		}
		break;
	case PHRASE_WEEK:
		{	
			QueryHistoryKLine_Mins(fData, enumPhrase, QUERY_WEEK_DAYCOUNT);	
		}
		break;
	case PHRASE_MONTH:
		{				
			QueryHistoryKLine_Mins(fData, enumPhrase, QUERY_MONTH_DAYCOUNT);	
		}
		break;		
	}
	
}/*
void CDlgQuation::QueryHistoryKLine_Min1(CFData& fData)
{
	struct tm  dwTimeBein; 
	vector<SKLine> vec = fData.GetVecKLine(PHRASE_1MIN);
	if(vec.size())
	{
		SKLine& sKine = vec[0];	
		dwTimeBein = sKine.dwTime;		
	}
	struct tm  dwTimeEnd; 
	dwTimeEnd.tm_year = atoi(m_strTradingDay.substr(0, 4).c_str()) - 1900;
	dwTimeEnd.tm_mon  = atoi(m_strTradingDay.substr(4, 2).c_str()) - 1;
	dwTimeEnd.tm_mday = atoi(m_strTradingDay.substr(6, 2).c_str());
	dwTimeEnd.tm_hour = 0;
	dwTimeEnd.tm_min  = 0;
	dwTimeEnd.tm_sec  = 0;	
	int nTimeSpanTemp = difftime(mktime(&dwTimeBein), mktime(&dwTimeEnd));
	if(nTimeSpanTemp >QUERY_MIN1_DAYCOUNT*3600*24)
	{//如果超过7天的数据，则取今天之前7天的数据
		time_t t1 = mktime( &dwTimeEnd);
		t1 = t1 - QUERY_MIN1_DAYCOUNT*3600*24;
		dwTimeBein = *localtime(&t1);
	}
		
	SGetInfo sGetInfo;
	if(m_callbackGet)
		m_callbackGet(sGetInfo);
	if(sGetInfo.str == "")
		sGetInfo.str= "23:59:59";
	std::string strEndTime = sGetInfo.str;

	KLineQuery kQuery;
	memset(&kQuery, 0, sizeof(KLineQuery));
	sprintf(kQuery.szStartTime,"%2d%2d%2d %2d:%2d:%2d",  dwTimeBein.tm_year+1900,  dwTimeBein.tm_mon +1, dwTimeBein.tm_mday, dwTimeBein.tm_hour, dwTimeBein.tm_min, dwTimeBein.tm_sec);
	sprintf(kQuery.szEndTime,"%s %s", m_strTradingDay.c_str(),  strEndTime.c_str());
	strcpy(kQuery.InstrumentID, fData.m_strInstrument.c_str());
	kQuery.nCycle =	Minute_1;
	kQuery.nDataType =1;
	
	CTCPHistoryService::GetInstance()->SendData(Cmd_QueryQuotKLineData_Req, (void *)&kQuery, sizeof(KLineQuery), 0);	

}*/
void CDlgQuation::QueryHistoryKLine_Mins(CFData& fData, EnumPhrase	enumPhrase, int nDayCount)
{
	if(fData.IsKLineQueryFinish(enumPhrase))
		return;//已经请求过历史数据了
	struct tm  dwTimeBein; 
	vector<SKLine> vec;
	fData.GetVecKLine(enumPhrase, vec);
	if(vec.size())
	{
		SKLine& sKine = vec[0];	
		dwTimeBein = sKine.dwTime;		
	}
	struct tm  dwTimeEnd; 
	dwTimeEnd.tm_year = atoi(m_strTradingDay.substr(0, 4).c_str()) - 1900;
	dwTimeEnd.tm_mon  = atoi(m_strTradingDay.substr(4, 2).c_str()) - 1;
	dwTimeEnd.tm_mday = atoi(m_strTradingDay.substr(6, 2).c_str());
	dwTimeEnd.tm_hour = 0;
	dwTimeEnd.tm_min  = 0;
	dwTimeEnd.tm_sec  = 0;	
	int nTimeSpanTemp = difftime(mktime(&dwTimeBein), mktime(&dwTimeEnd));
	if(nTimeSpanTemp >nDayCount*3600*24 || vec.size()==0)
	{//如果超过nDayCount天的数据，则取今天之前nDayCount天的数据
		if(enumPhrase == PHRASE_1MIN 
			|| enumPhrase == PHRASE_5MIN 
			||enumPhrase == PHRASE_15MIN 
			||enumPhrase == PHRASE_30MIN
			||enumPhrase == PHRASE_60MIN)
		{
			time_t t1 = mktime( &dwTimeEnd);
			t1 = t1 - nDayCount*3600*24;
			dwTimeBein = *localtime(&t1);
		}
		else if(enumPhrase == PHRASE_DAY 
			|| enumPhrase == PHRASE_WEEK 
			||enumPhrase == PHRASE_MONTH)
		{//如果是年，则所有上市以来的数据，默认从1900年开始
			dwTimeBein.tm_year = 90;
			dwTimeBein.tm_mon  = 0;
			dwTimeBein.tm_mday = 1;
			dwTimeBein.tm_hour = 0;
			dwTimeBein.tm_min  = 0;
			dwTimeBein.tm_sec  = 0;	
		}
	}

	SGetInfo sGetInfo;
	if(m_callbackGet)
		m_callbackGet(sGetInfo);
	if(sGetInfo.str == "")
		sGetInfo.str= "23:59:59";
	std::string strEndTime = sGetInfo.str;

	KLineQuery kQuery;
	memset(&kQuery, 0, sizeof(KLineQuery));
	sprintf(kQuery.szStartTime,"%04d%02d%02d %02d:%02d:%02d",  dwTimeBein.tm_year+1900,  dwTimeBein.tm_mon +1, dwTimeBein.tm_mday, dwTimeBein.tm_hour, dwTimeBein.tm_min, dwTimeBein.tm_sec);
	sprintf(kQuery.szEndTime,"%s %s", m_strTradingDay.c_str(),  strEndTime.c_str());
	strcpy(kQuery.InstrumentID, fData.m_strInstrument.c_str());
	kQuery.nCycle =	GetKLineDataCycleValue(enumPhrase);
	kQuery.nDataType =1;

	nQueryIndex++;
	m_mapIndex2QueryInfo.insert(make_pair(nQueryIndex, kQuery));
	CTCPHistoryService::GetInstance()->SendData(Cmd_QueryQuotKLineData_Req, (void *)&kQuery, sizeof(KLineQuery), nQueryIndex);	
}
LRESULT CDlgQuation::OnMessageQutoKLineHistory(WPARAM wP,LPARAM lP)
{
	std::string strInstrument ="";
	EnumPhrase  enumPhrase;
	int n = lP;
	std::vector<KLineData>& vecData = *(std::vector<KLineData>*)wP;
	if(vecData.size())
	{
		KLineData& Data = vecData[0];
		strInstrument = Data.InstrumentID;
		enumPhrase = GetEnumPhraseValue(Data.eCycle);	
	}
	else
	{//如果没有返回，则意味着服务端没有数据了，请求结束了
		int nQueryInfo = (int)lP;
		map<int, KLineQuery>::iterator it =	m_mapIndex2QueryInfo.find(nQueryInfo);
		if(it == m_mapIndex2QueryInfo.end())
			return 0;

		KLineQuery& kLineQuery = it->second;
		strInstrument = kLineQuery.InstrumentID;
		enumPhrase	  = GetEnumPhraseValue(kLineQuery.nCycle);	
	}
	map<string,  CFData>::iterator itFData = m_mapID2FData.find(strInstrument);
	if(itFData ==  m_mapID2FData.end())
		return 0;

	CFData& fdata = itFData->second;	

	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{	
			fdata.AppendHistoryKLineData_Min1(vecData);
			if(vecData.size() < QUERY_MAX_RETURN)
			{//少于1000条的返回则视为请求已经结束
				fdata.AppendRealTimeKLineData_Min1();
				if(m_enumPhrase == PHRASE_1MIN && m_strSelFData == fdata.m_strInstrument)
				{
					vector<SKLine> vec;
					bool bGet = fdata.GetVecKLine(PHRASE_1MIN, vec);
					if(bGet)
					{
						std::vector<std::string> vecTechNames;
						EnumPhrase enumPhrase;
						m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
						for(int i =0; i< (int)vecTechNames.size(); i++)
						{
							STechCalcPara sTCPara;
							sTCPara.enumPhrase = m_enumPhrase;
							sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
							fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
						}
						m_QView.m_KlineView.SetVectorData(vec);
					}
				}
			}
			else
			{//继续请求，直到所有请求结束为止
				QueryHistoryKLine_Mins(fdata, PHRASE_1MIN, QUERY_MIN1_DAYCOUNT);
			}
		}
		break;
	case PHRASE_5MIN:
		{			
			fdata.AppendHistoryKLineData_Min5(vecData);
			if(vecData.size() < QUERY_MAX_RETURN)
			{//少于1000条的返回则视为请求已经结束
				fdata.AppendRealTimeKLineData_Min5();
				if(m_enumPhrase == PHRASE_5MIN && m_strSelFData == fdata.m_strInstrument)
				{
					vector<SKLine> vec;
					bool bGet = fdata.GetVecKLine(PHRASE_5MIN, vec);
					if(bGet)
					{
						std::vector<std::string> vecTechNames;
						EnumPhrase enumPhrase;
						m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
						for(int i =0; i< (int)vecTechNames.size(); i++)
						{
							STechCalcPara sTCPara;
							sTCPara.enumPhrase = m_enumPhrase;
							sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
							fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
						}
						m_QView.m_KlineView.SetVectorData(vec);
					}
				}
			}
			else
			{//继续请求，直到所有请求结束为止
				QueryHistoryKLine_Mins(fdata, PHRASE_5MIN, QUERY_MIN5_DAYCOUNT);
			}
		}
		break;
	case PHRASE_15MIN:
		{
			fdata.AppendHistoryKLineData_Min15(vecData);
			if(vecData.size() < QUERY_MAX_RETURN)
			{//少于1000条的返回则视为请求已经结束
				fdata.AppendRealTimeKLineData_Min15();
				if(m_enumPhrase == PHRASE_15MIN && m_strSelFData == fdata.m_strInstrument)
				{
					vector<SKLine> vec;
					bool bGet = fdata.GetVecKLine(PHRASE_15MIN, vec);
					if(bGet)
					{
						std::vector<std::string> vecTechNames;
						EnumPhrase enumPhrase;
						m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
						for(int i =0; i< (int)vecTechNames.size(); i++)
						{
							STechCalcPara sTCPara;
							sTCPara.enumPhrase = m_enumPhrase;
							sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
							fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
						}
						m_QView.m_KlineView.SetVectorData(vec);
					}
				}
			}
			else
			{//继续请求，直到所有请求结束为止
				QueryHistoryKLine_Mins(fdata, PHRASE_15MIN, QUERY_MIN15_DAYCOUNT);
			}
		}
		break;
	case PHRASE_30MIN:
		{
			fdata.AppendHistoryKLineData_Min30(vecData);
			if(vecData.size() < QUERY_MAX_RETURN)
			{//少于1000条的返回则视为请求已经结束
				fdata.AppendRealTimeKLineData_Min30();
				if(m_enumPhrase == PHRASE_30MIN && m_strSelFData == fdata.m_strInstrument)
				{
					vector<SKLine> vec;
					bool bGet = fdata.GetVecKLine(PHRASE_30MIN, vec);
					if(bGet)
					{
						std::vector<std::string> vecTechNames;
						EnumPhrase enumPhrase;
						m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
						for(int i =0; i< (int)vecTechNames.size(); i++)
						{
							STechCalcPara sTCPara;
							sTCPara.enumPhrase = m_enumPhrase;
							sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
							fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
						}
						m_QView.m_KlineView.SetVectorData(vec);
					}
				}
			}
			else
			{//继续请求，直到所有请求结束为止
				QueryHistoryKLine_Mins(fdata, PHRASE_30MIN, QUERY_MIN30_DAYCOUNT);
			}
		}
		break;
	case PHRASE_60MIN:
		{
			fdata.AppendHistoryKLineData_Min60(vecData);
			if(vecData.size() < QUERY_MAX_RETURN)
			{//少于1000条的返回则视为请求已经结束
				fdata.AppendRealTimeKLineData_Min60();
				if(m_enumPhrase == PHRASE_60MIN && m_strSelFData == fdata.m_strInstrument)
				{
					vector<SKLine> vec;
					bool bGet = fdata.GetVecKLine(PHRASE_60MIN, vec);
					if(bGet)
					{
						std::vector<std::string> vecTechNames;
						EnumPhrase enumPhrase;
						m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
						for(int i =0; i< (int)vecTechNames.size(); i++)
						{
							STechCalcPara sTCPara;
							sTCPara.enumPhrase = m_enumPhrase;
							sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
							fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
						}
						m_QView.m_KlineView.SetVectorData(vec);
					}
				}
			}
			else
			{//继续请求，直到所有请求结束为止
				QueryHistoryKLine_Mins(fdata, PHRASE_60MIN, QUERY_MIN60_DAYCOUNT);
			}
		}
		break;
	case PHRASE_DAY:
		{
			fdata.AppendHistoryKLineData_Day(vecData);
			if(vecData.size() < QUERY_MAX_RETURN)
			{//少于1000条的返回则视为请求已经结束
				fdata.AppendRealTimeKLineData_Day();
				if(m_enumPhrase == PHRASE_DAY && m_strSelFData == fdata.m_strInstrument)
				{
					vector<SKLine> vec;
					bool bGet = fdata.GetVecKLine(PHRASE_DAY, vec);
					if(bGet)
					{
						std::vector<std::string> vecTechNames;
						EnumPhrase enumPhrase;
						m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
						for(int i =0; i< (int)vecTechNames.size(); i++)
						{
							STechCalcPara sTCPara;
							sTCPara.enumPhrase = m_enumPhrase;
							sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
							fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
						}
						m_QView.m_KlineView.SetVectorData(vec);
					}
				}
			}
			else
			{//继续请求，直到所有请求结束为止
				QueryHistoryKLine_Mins(fdata, PHRASE_DAY, QUERY_DAY_DAYCOUNT);
			}
		}
		break;
	case PHRASE_WEEK:
		{	
			fdata.AppendHistoryKLineData_Week(vecData);
			if(vecData.size() < QUERY_MAX_RETURN)
			{//少于1000条的返回则视为请求已经结束
				fdata.AppendRealTimeKLineData_Week();
				if(m_enumPhrase == PHRASE_WEEK && m_strSelFData == fdata.m_strInstrument)
				{
					vector<SKLine> vec;
					bool bGet = fdata.GetVecKLine(PHRASE_WEEK, vec);
					if(bGet)
					{
						std::vector<std::string> vecTechNames;
						EnumPhrase enumPhrase;
						m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
						for(int i =0; i< (int)vecTechNames.size(); i++)
						{
							STechCalcPara sTCPara;
							sTCPara.enumPhrase = m_enumPhrase;
							sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
							fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
						}
						m_QView.m_KlineView.SetVectorData(vec);
					}
				}
			}
			else
			{//继续请求，直到所有请求结束为止
				QueryHistoryKLine_Mins(fdata, PHRASE_WEEK, QUERY_WEEK_DAYCOUNT);
			}
		}
		break;
	case PHRASE_MONTH:
		{				
			fdata.AppendHistoryKLineData_Month(vecData);
			if(vecData.size() < QUERY_MAX_RETURN)
			{//少于1000条的返回则视为请求已经结束
				fdata.AppendRealTimeKLineData_Month();
				if(m_enumPhrase == PHRASE_MONTH && m_strSelFData == fdata.m_strInstrument)
				{
					vector<SKLine> vec;
					bool bGet = fdata.GetVecKLine(PHRASE_MONTH, vec);
					if(bGet)
					{
						std::vector<std::string> vecTechNames;
						EnumPhrase enumPhrase;
						m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);					
						for(int i =0; i< (int)vecTechNames.size(); i++)
						{
							STechCalcPara sTCPara;
							sTCPara.enumPhrase = m_enumPhrase;
							sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
							fdata.CalcKTechIndex(vec,"", vecTechNames[i], sTCPara, m_QView.m_KlineView.m_pTechIndex[i]);
						}
						m_QView.m_KlineView.SetVectorData(vec);
					}
				}
			}
			else
			{//继续请求，直到所有请求结束为止
				QueryHistoryKLine_Mins(fdata, PHRASE_MONTH, QUERY_MONTH_DAYCOUNT);
			}
		}
		break;		
	}

	
	return 1L;
}
void CDlgQuation::OnDestroy()
{	
	map<string, CFData>::iterator it = m_mapID2FData.begin();
	for(; it != m_mapID2FData.end(); it++)
	{
		CFData& fData = it->second;
		fData.SaveFile();
		fData.SaveFile_TLine();
		UnSubscribleInstrument(it->first);
	}		
	CDialog::OnDestroy();
}
LRESULT CDlgQuation::OnMessageIndexExpress(WPARAM wP,LPARAM lP)
{
	CString strTechName = *(CString *)wP;
	CTechIndexExpressDlg dlg;
	dlg.SetTechIndex(strTechName);
	dlg.DoModal();
	return 1L;
}
LRESULT CDlgQuation::OnMessageIndexConfig(WPARAM wP,LPARAM lP)
{
	vector<int> vecConfig;

	CString str = *(CString*)wP;
	EnumPhrase  enumPhrase = m_QView.m_KlineView.GetPhrase();
	int nIndex = (int)lP;
	
	map<string, CFData>::iterator it = m_mapID2FData.begin();
	for(; it != m_mapID2FData.end(); it++)
	{
		CFData& fData = it->second;
		fData.GetTechIndexConfig(str.GetBuffer(str.GetLength()), enumPhrase, vecConfig);
	}		

	CTechIndexConfigDlg dlg;
	dlg.SetMsgWnd(this);
	dlg.SetTechIndex(str, nIndex, m_enumPhrase, vecConfig);
	
	dlg.DoModal();
	return 1L;
}
LRESULT CDlgQuation::OnMessageConfigChange(WPARAM wP,LPARAM lP)
{
	CInitConfig& cInitConfig = *(CInitConfig*)wP;
	vector<int> vecCofig;
	for(int i=0; i<MAX_CONFIG_ITEMS; i++)
	{
		if(cInitConfig.bConfigShow[i])
			vecCofig.push_back(cInitConfig.nConfig[i]);		
	}
	string strID;
	m_QView.m_KlineView.GetFuture(strID);

	map<string,  CFData>::iterator itFData = m_mapID2FData.find(strID);

	if(itFData ==  m_mapID2FData.end())
		return 0;

	CFData& fdata = itFData->second;
	
	std::vector<std::string> vecTechNames;
	EnumPhrase enumPhrase;
	m_QView.m_KlineView.GetTechIndexNames(vecTechNames, enumPhrase);
	
	vector<SKLine> vec;
	bool bGet = fdata.GetVecKLine(enumPhrase, vec);
	if(bGet)
	{
		STechCalcPara sTCPara;
		sTCPara.enumPhrase = m_enumPhrase;
		sTCPara.m_VolumeMultiple = fdata.m_VolumeMultiple;
		if(cInitConfig.nIndex <TECHINDEX_MAX && cInitConfig.nIndex>=0)
			fdata.CalcKTechIndex(vec,"", cInitConfig.strTechIndex.GetBuffer(cInitConfig.strTechIndex.GetLength()), 
			sTCPara, m_QView.m_KlineView.m_pTechIndex[cInitConfig.nIndex], &vecCofig);

	//	vector<SKLine> vec2;
	//	bool bGet = fdata.GetVecKLine(enumPhrase, vec2);
	//	m_QView.m_KlineView.SetVectorData(vec2);
	}

	return 1L;
}