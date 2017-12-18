#include "StdAfx.h"

#include "../../../inc/Quotation/MyFrame.h"



#define  ID_PHRASE_QUIT				23134

DEFINE_EVENT_TYPE(wxEVT_Data_InTime)
DEFINE_EVENT_TYPE(wxEVT_GetTime)
DEFINE_EVENT_TYPE(wxEVT_SetLineViewTime)
DEFINE_EVENT_TYPE(wxEVT_ReSend_Message)
DEFINE_EVENT_TYPE(wxEVT_MyFrame_AddMySelect)
DEFINE_EVENT_TYPE(wxEVT_MyFrame_MaxMize)
BEGIN_EVENT_TABLE(MyFrame, wxPanel)

 EVT_MENU(ID_PHRASE_QUIT,			MyFrame::Quit)
 EVT_SIZE(MyFrame::OnSize)
 EVT_CLOSE( MyFrame::OnClose)

 EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_LEFTDBCLICK,		MyFrame::OnFuncLDBClickKLine)
 EVT_COMMAND(wxID_ANY, wxEVT_KCtrl_KeyDown,			MyFrame::OnFuncKeyDownKLine)

 EVT_COMMAND(wxID_ANY, wxEVT_TCtrl_LEFTDBCLICK,		MyFrame::OnFuncLDBClickTLine)
 EVT_COMMAND(wxID_ANY, wxEVT_TCtrl_KeyDown,			MyFrame::OnFuncKeyDownTLine)

 EVT_COMMAND(wxID_ANY, wxEVT_RETURN_KLINE,	MyFrame::OnFuncReturnKLine)
 EVT_COMMAND(wxID_ANY, wxEVT_RETURN_TLINE,	MyFrame::OnFuncReturnTLine)

 EVT_COMMAND(wxID_ANY, wxEVT_GridEx_KeyDown,	MyFrame::OnFuncKeyQPanel)
 

 EVT_COMMAND(wxID_ANY, wxEVT_RETURN_BAOJIABIAO,	MyFrame::OnFuncReturnBaoJiaBiao)
 EVT_COMMAND(wxID_ANY, wxEVT_RETURN_BAOJIABIAO2,	MyFrame::OnFuncReturnBaoJiaBiao2)

 EVT_COMMAND(wxID_ANY, wxEVT_KLINEVIW_INFO, OnKLineViewInfo)
 EVT_COMMAND(wxID_ANY, wxEVT_TLINEVIW_INFO, OnKLineViewInfo)
 
END_EVENT_TABLE()

void MyFrame::OnClose(wxCloseEvent& evt)
{
	if(m_pKLineView)
	{
		delete m_pKLineView;
		m_pKLineView = NULL;
	}
	if(m_pTLineView)
	{
		delete m_pTLineView;
		m_pTLineView = NULL;
	}
	
	if(m_pQPanel)
	{
		delete m_pQPanel;
		m_pQPanel = NULL;
	}
	if(m_splitter)
	{
		delete m_splitter;
		m_splitter = NULL;
	}
	evt.Skip();	
//	Close(true);
	delete this;
	exit(1);	
}
MyFrame::MyFrame(wxWindow *parent,
		int x, int y, int width, int height,
		long style,
		const wxString& name)
	:wxPanel(parent, x,  y,  width,  height, style,	name)
{		
	m_pKLineView = NULL;	
	m_pTLineView = NULL;
	m_splitter	 = NULL;
	m_pQPanel    = NULL;
	m_pFData	 = NULL;

	wxRect rtClient = GetClientRect();
	m_splitter = new KSplitterWindow(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxSP_3D|wxNO_BORDER);
	m_splitter->SetSashGravity(1.0);	
	m_splitter->SetSashSize(1);
	::SetWindowPos((HWND)m_splitter->GetHWND(), HWND_BOTTOM, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE );
	
	
	int nQPanel = 250;
	m_pKLineView = new CKLineView(this);
	m_pKLineView->CreateDlg(m_splitter, 11121, wxPoint(0,0), wxSize(rtClient.GetWidth()-nQPanel, rtClient.GetHeight()) );
	m_pKLineView->CfgShow(KTYPE_KLINE|KTYPE_VOLUME|KTYPE_SCALES|KTYPE_TECHINDEX);

	m_pTLineView = new CTLineView(this);
	m_pTLineView->CreateDlg(m_splitter, 11122, wxPoint(0,0), wxSize(rtClient.GetWidth()-nQPanel, rtClient.GetHeight()) );
	m_pTLineView->CfgShow(TTYPE_TLINE|TTYPE_VOLUME|TTYPE_SCALES);
	m_pTLineView->Show();

	m_pQPanel = new CQPanel;
	m_pQPanel->CreateDlg(m_splitter, 11128, wxPoint(rtClient.GetWidth()-nQPanel,0), wxSize(nQPanel, rtClient.GetHeight()) );	
	m_pQPanel->Show();	
	
	m_splitter->SetSize(rtClient.width , rtClient.height);
	
	
//	InitFromCFG();
	m_nTrace = 0;
	GetRegIntValue(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\zq"),_T("trace"), m_nTrace);

	m_nRightSpanMiniWidth = 200;
	m_splitter->SetMinimumPaneSize(m_nRightSpanMiniWidth);
	m_bShowRightPanel = true;
	m_nWidthRightPanel = -300;
	//_asm int 3
}

MyFrame::~MyFrame()
{
	map<string, vector<SSpan*>*>::iterator it = m_mapTransTime.begin();
	while(it != m_mapTransTime.end())
	{
		map<string, vector<SSpan*>*>::iterator itTemp = it;
		it++;
		vector<SSpan*> *pTemp = itTemp->second;

		for(vector<SSpan *>::iterator it = pTemp->begin(); it != pTemp->end(); it++)
		{
			SSpan *p = *it;
			delete p;
			p= NULL;
		}
		pTemp->erase(pTemp->begin(), pTemp->end());	
		delete pTemp;
		pTemp = NULL;
		m_mapTransTime.erase(itTemp);
	}

	if(m_pTLineView)
	{		
		delete m_pTLineView;		
		m_pTLineView = NULL;
	}
	if(m_pKLineView)
	{		
		delete m_pKLineView;		
		m_pKLineView = NULL;
	}
	if(m_pQPanel)
	{		
		delete m_pQPanel;
		m_pQPanel = NULL;
	}
	if(m_splitter)
	{		
		delete m_splitter;
		m_splitter = NULL;
	}
}

void MyFrame::Quit(wxCommandEvent& WXUNUSED(event) )
{	
	//ClearFData();
	Close(true);
}
void MyFrame::OnSize(wxSizeEvent& event)
{
	if(m_splitter == NULL || !::IsWindow((HWND)m_splitter->GetHWND()))
		return;
	SizeWindow();
}
void MyFrame::OnDataInTime(PlatformStru_DepthMarketData* pData, CFData* pFData, bool bUpdate)
{
	if(pFData == NULL || pData == NULL)
		return;	
	
	//wxString strDate = pFData->TradingDay;
	wxString strDate = pFData->m_strTradingDay;//如果行情里面没有日期，照样不能解决跨日期的问题
	wxString strFutureID = pData->InstrumentID;
	if(isInvalidValue(pData->LastPrice)) 
	//	||isInvalidValue(pData->OpenPrice)
	//	||isInvalidValue(pData->HighestPrice)
	//	||isInvalidValue(pData->LowestPrice)
	//	||isInvalidValue(pData->Volume))
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
		m_pQPanel->SetInTimeData(*pData);
		m_pQPanel->Refresh(false);		

		if(isInvalidValue(pData->BidPrice1))
			pData->BidPrice1 = 0;
		if(isInvalidValue(pData->AskPrice1))
			pData->AskPrice1 = 0;
    //无效数据暂时不弹出提示框
		if(m_nTrace)
			AppendLog_DepthMarketData(pData);
		return;//无效值
	}	
	if(isInvalidValue(pData->BidPrice1))
		pData->BidPrice1 = 0;
	if(isInvalidValue(pData->AskPrice1))
		pData->AskPrice1 = 0;
 //无效数据暂时不弹出提示框
	if(m_nTrace)
		AppendLog_DepthMarketData(pData);
	pFData->SetDataInTime(pData);


	if(bUpdate)
	{
		if(m_pTLineView)
		{
			m_pTLineView->SetLastClosePrice(pFData->m_PreSettlementPrice);			
			m_pTLineView->SetInTimeData();
		}
		if(m_pKLineView)
			m_pKLineView->SetInTimeData();		
		if(m_pQPanel)
		{
			m_pQPanel->SetGridLastItem();
			m_pQPanel->SetInTimeData(*pData);
			m_pQPanel->Refresh(false);		
		}
	}
}
void MyFrame::OnFuncLDBClickKLine(wxCommandEvent& event)
{
	wxWindow* winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	if(window)
		winSend = window->GetParent();//chqframe
	wxCommandEvent myEvent(wxEVT_KCtrl_LEFTDBCLICK);
	myEvent.SetClientData(this);
	if(winSend)
		winSend->ProcessEvent(myEvent);

//	m_pKLineView->Hide();
//	m_pApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, GetNextRequestId());
}
void MyFrame::OnFuncLDBClickTLine(wxCommandEvent& event)
{
	OnReturnKLine();	
}
void MyFrame::OnFuncKeyDownTLine(wxCommandEvent& event)
{
	wxWindow* winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	if(window)
		winSend = window->GetParent();//chqframe
	
	if(winSend)
		winSend->ProcessEvent(event);
}
void MyFrame::OnFuncKeyQPanel(wxCommandEvent& event)
{
	wxWindow* winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	if(window)
		winSend = window->GetParent();//chqframe

	if(winSend)
		winSend->ProcessEvent(event);
}
void MyFrame::OnFuncKeyDownKLine(wxCommandEvent& event)
{
	wxWindow* winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	if(window)
		winSend = window->GetParent();//chqframe

	if(winSend)
		winSend->ProcessEvent(event);
}

bool MyFrame::InitFromCFG(TiXmlElement *root)
{
	if(!InitCfg(root))
	{
		return false;
	}
	if(m_pKLineView)
	{
		if(!m_pKLineView->InitCfg(root))
		{
			return false;
		}
	}
	if(m_pTLineView)
	{
		if(!m_pTLineView->InitCfg(root))
		{
			return false;
		}
	}
	if(m_pQPanel)
	{
		if(!m_pQPanel->InitCfg(root))
		{
			return false;
		}
	}
	return true;
}
void MyFrame::SetMiniWidth(unsigned long value)
{
	m_nRightSpanMiniWidth = value;
}
bool MyFrame::InitCfg(TiXmlElement *root)
{//各交易所交易时间配置文件

	TiXmlElement *tList2 = root->FirstChildElement("qpanel");
	if(tList2 == NULL)
		return false;

	TiXmlNode *pMiniWidth = tList2->FirstChild("MiniWidth");
	if(pMiniWidth == NULL)
		return false;
	const char *strValue = pMiniWidth->ToElement()->GetText();
	unsigned long value = ColorStr2Long(strValue);
	SetMiniWidth(value);


	TiXmlElement *tList = root->FirstChildElement("exchange");
	if(tList == NULL)
		return false;

	for(TiXmlElement *GroupItem = tList->FirstChildElement("group");
		GroupItem; 
		GroupItem = GroupItem->NextSiblingElement("group"))
	{
		const char *pGroupid = GroupItem->Attribute("id");
		if(!pGroupid) 
		{
			continue;
		}
		string strID = pGroupid;
		const char *pGrouptime = GroupItem->Attribute("time");
		if(!pGrouptime)
		{
			continue;
		}
		vector<SSpan*> *pVector = new vector<SSpan*>;
		string str = pGrouptime;
		vector<string> vecTime;
		split(str, ' ',vecTime);
		for(int i =0; i< (int)vecTime.size(); i++)
		{
			string strTime = vecTime[i];
			vector<string> vecOne;
			split(strTime, '-',vecOne);
			if(vecOne.size() < 2)
				break;
			wxString strSpan = vecOne[0];
			int nPos = strSpan.find(':');
			if(nPos == -1)
				break;
			SSpan *span = new SSpan;
			wxString strLeft  = strSpan.Left(nPos);
			wxString strRight =  strSpan.Right(strSpan.Length()- nPos -1);
			span->tmBegin.tm_hour = wxAtoi(strLeft);
			span->tmBegin.tm_min  = wxAtoi(strRight);

			strSpan = vecOne[1];
			nPos = strSpan.find(':');
			if(nPos == -1)
				break;
			strLeft  = strSpan.Left(nPos);
			strRight =  strSpan.Right(strSpan.Length()- nPos -1);
			span->tmEnd.tm_hour = wxAtoi(strLeft);
			span->tmEnd.tm_min  = wxAtoi(strRight);

			pVector->push_back(span);
		}
		m_mapTransTime.insert(map<string, vector<SSpan*>*>::value_type(strID,  pVector));			
	}

	return true;
}
void  MyFrame::InitGUIData(CFData *pFData, PlatformStru_DepthMarketData *pQutoData)
{
	m_pFData = pFData;
	pFData->m_UpperLimitPrice    = pQutoData->UpperLimitPrice;
	pFData->m_LowerLimitPrice    = pQutoData->LowerLimitPrice;
	pFData->m_PreSettlementPrice = pQutoData->PreSettlementPrice;
	if(isInvalidValue(pQutoData->HighestPrice))
	{//如果最高最低价无效，则以这单为第一笔数据，兼容组合单
		double dbMaxSpan = abs(pQutoData->LastPrice - pQutoData->PreSettlementPrice);		
		if(abs(pFData->m_fMaxValue	 - MIN_VALUE_DEFAULT)<0.00001 && abs(pFData->m_fMinValue-MAX_VALUE_DEFAULT)<0.00001)
		{
			pFData->m_fMaxValue = pQutoData->PreSettlementPrice + dbMaxSpan;
			pFData->m_fMinValue = pQutoData->PreSettlementPrice - dbMaxSpan;
		}
		else
		{
			double fMaxValue = pQutoData->PreSettlementPrice + dbMaxSpan;
			double fMinValue = pQutoData->PreSettlementPrice - dbMaxSpan;
			if(pFData->m_fMaxValue < fMaxValue)
				pFData->m_fMaxValue = fMaxValue;
			if(pFData->m_fMinValue > fMinValue)
				pFData->m_fMinValue = fMinValue;
		}
	}
	else
	{
		pFData->m_fMaxValue			 = pQutoData->HighestPrice;
		pFData->m_fMinValue			 = pQutoData->LowestPrice;
	}
	
	if(pFData)
	{
		wxWindow *pWnd = m_splitter->GetWindow1();
		if(pWnd == m_pTLineView)
			pFData->SetFuture();	
		else
		{
			EnumPhrase enumPhrase = m_pKLineView->GetPhrase();
			pFData->SetPhrase(enumPhrase);			
			//pFData->SetFuture(pFData->m_ExchangeName.c_str(), pFData->m_ContractName.c_str(), wxT(""), enumPhrase);
		}
		pFData->SetFuture_QIntime();		
	
	}
}
void MyFrame::InitGUI(CFData *pData, PlatformStru_DepthMarketData *pQutoData, bool bRemain)
{//两次调用该函数，一次是用于及时显示，第二次请求到合约详情以后才能真正显示正确的信心
	m_pFData = pData;
	wxWindow *pWnd = m_splitter->GetWindow1();
	if(m_pKLineView)
	{		
		EnumPhrase enumPhrase = m_pKLineView->GetPhrase();
		m_pKLineView->SetFData(pData);
		if(pWnd == m_pKLineView && pQutoData)//行情快照之后才会有交易所名称之类的
			m_pKLineView->SetPhrase(enumPhrase);			
	}		
	if(m_pTLineView)
	{
		m_pTLineView->SetFData(pData);		
		if(!bRemain)
		{			
			if(pWnd == m_pKLineView)
				m_splitter->ReplaceWindow(m_pKLineView, m_pTLineView);
			m_pKLineView->Show(false);
			m_pTLineView->Refresh(false);	
			m_pTLineView->Show();	
			m_pTLineView->SetFocus();
		}	
		else
		{
			if(pQutoData)
				pWnd->Refresh(false);
		}
	}	
	if(m_pQPanel)
	{			
		m_pQPanel->Init();
		m_pQPanel->SetFuture(pData->m_ExchangeName.c_str(), pData->m_ContractName.c_str(), wxT(""), pData->m_InstrumentName.c_str());
		m_pQPanel->SetFData(pData);
		if(pQutoData)
			m_pQPanel->InitGrid();//不要重复初始化
		if(pQutoData)
			m_pQPanel->SetInTimeData(*pQutoData);

		if(m_bShowRightPanel)
		{
			m_pQPanel->Refresh(false);	
			m_pQPanel->Show();
		}
	}
	m_splitter->Show();
	
}
BOOL MyFrame::GetRegIntValue(HKEY hRoot,wxString strKeyName,wxString strValueName,int& nValue)
{
	if(!hRoot)
		return FALSE;

	HKEY hDYXH = NULL;
	long l = ERROR_SUCCESS; 
	strKeyName.Trim(false);
	strKeyName.Trim();
	BOOL bFlag = TRUE;
	if(strKeyName.Length())
		l = RegOpenKeyEx(hRoot,strKeyName.c_str(),NULL,KEY_READ,&hDYXH);
	else
	{
		hDYXH = hRoot;
		bFlag = FALSE;
	}

	if(l == ERROR_SUCCESS && hDYXH)
	{
		DWORD dwType =  REG_DWORD;
		DWORD dwValue = 0;
		DWORD dwSize = sizeof(DWORD);
		l = RegQueryValueEx(hDYXH,strValueName,NULL,&dwType,(LPBYTE)&dwValue,&dwSize);

		if(bFlag)
			RegCloseKey(hDYXH);
		if(ERROR_SUCCESS != l)
			return FALSE;

		nValue = dwValue;
		return TRUE;
	}
	else
		return FALSE;
}
void MyFrame::OnFuncReturnKLine(wxCommandEvent& event)
{
	OnReturnKLine();		
}
void MyFrame::OnFuncReturnTLine(wxCommandEvent& event)
{
	OnReturnTLine();
}
void MyFrame::OnFuncReturnBaoJiaBiao(wxCommandEvent& event)
{
	wxWindow* winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	if(window)
		winSend = window->GetParent();//chqframe
	wxCommandEvent myEvent(wxEVT_RETURN_BAOJIABIAO);
	myEvent.SetClientData(this);
	if(winSend)
		winSend->ProcessEvent(myEvent);

	//OnReturnBaoJiaoBiao();
}
void MyFrame::OnFuncReturnBaoJiaBiao2(wxCommandEvent& event)
{
	wxWindow* winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	if(window)
		winSend = window->GetParent();//chqframe
	wxCommandEvent myEvent(wxEVT_RETURN_BAOJIABIAO);
	myEvent.SetClientData(this);
	if(winSend)
		winSend->ProcessEvent(myEvent);
	//OnReturnBaoJiaoBiao();
}
void MyFrame::OnReturnKLine()
{
	if(m_splitter && m_pTLineView && m_pKLineView)
	{
		wxWindow *pWnd = m_splitter->GetWindow1();
		if(pWnd == m_pTLineView)
			m_splitter->ReplaceWindow(m_pTLineView, m_pKLineView);

		m_pTLineView->Hide();

		EnumPhrase emPhrase = m_pKLineView->GetPhrase();
		m_pKLineView->SetPhrase(emPhrase);//初始化默认为日k线
		wxPanel *pKLine = m_pKLineView->GetControl(KTYPE_KLINE);
		if(pKLine)
			pKLine->SetFocus();

		m_pKLineView->Show();
	}	
}
void MyFrame::OnReturnTLine()
{
	if(m_splitter && m_pTLineView && m_pKLineView)
	{
		wxWindow *pWnd = m_splitter->GetWindow1();
		if(pWnd == m_pKLineView)
			m_splitter->ReplaceWindow(m_pKLineView, m_pTLineView);

		m_pKLineView->Hide();

		wxPanel *pTLine = m_pTLineView->GetControl(TTYPE_TLINE);
		if(pTLine)
			pTLine->SetFocus();
		m_pTLineView->Show();
	}	
}
void MyFrame::OnKLineViewInfo(wxCommandEvent& event)
{
	m_bShowRightPanel = !m_bShowRightPanel;
	SizeWindow();
}
void MyFrame::SizeWindow()
{	
	wxRect rtClient = GetClientRect();
	rtClient = GetClientRect();
	if(rtClient.GetHeight() == 0 || rtClient.GetWidth() ==0)
		return;

	wxWindow *pWnd = m_splitter->GetWindow2();
	if(pWnd != NULL && pWnd == m_pQPanel)
	{
		wxSize szWnd = pWnd->GetSize();
		if(szWnd.GetX() != 0)
			m_nWidthRightPanel = - szWnd.GetWidth();
	}

	m_splitter->SetSize(rtClient.width , rtClient.height);
	m_pKLineView->SetShowRightPanel(m_bShowRightPanel);
	m_pTLineView->SetShowRightPanel(m_bShowRightPanel);

	if( m_bShowRightPanel)
	{
		wxWindow *pWnd = m_splitter->GetWindow1();
		if(pWnd == m_pKLineView)
		{
			m_splitter->SplitVertically(m_pKLineView, m_pQPanel,m_nWidthRightPanel);	
			m_splitter->SetSashPosition(m_nWidthRightPanel);
		}
		else
		{
			m_splitter->SplitVertically(m_pTLineView, m_pQPanel,m_nWidthRightPanel);	
			m_splitter->SetSashPosition(m_nWidthRightPanel);
		}
	}
	else
	{
		m_splitter->Unsplit(m_pQPanel);
	}
	m_splitter->UpdateSize();
}

void MyFrame::SetAddMySelect()
{
	wxWindow* winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	if(window)
		winSend = window->GetParent();//chqframe
	wxCommandEvent myEvent(wxEVT_MyFrame_AddMySelect);
	myEvent.SetClientData(this);
	if(winSend)
		winSend->ProcessEvent(myEvent);
}
void MyFrame::SendMaxMize()
{
	int nValue = m_nMaxMize;
	if(nValue == 0)
		nValue = 1;
	else
		nValue = 0;

	wxWindow* winSend = NULL;
	wxWindow* window= GetParent();//CHILDFrame
	if(window)
		winSend = window->GetParent();//chqframe
	wxCommandEvent myEvent(wxEVT_MyFrame_MaxMize);
	myEvent.SetInt(nValue);
	if(winSend)
		winSend->ProcessEvent(myEvent);
}
int MyFrame::GetMaxSize()
{
	return m_nMaxMize;
}
CFData*  MyFrame::GetFData()
{
	return m_pFData;
}