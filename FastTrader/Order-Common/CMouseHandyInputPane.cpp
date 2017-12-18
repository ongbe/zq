
#include "StdAfx.h"
#include "CMouseHandyInputPane.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

set<CMouseHandyInputPane*> s_setMouseHandyInputPane;
HHOOK s_hMouseHook = NULL;
int s_nMouseHookCounter = 0;

LRESULT CALLBACK InputMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION) {
		MOUSEHOOKSTRUCT* pMouseHook = (MOUSEHOOKSTRUCT*)lParam;
		//wxMouseEvent evt;
		//evt.m_x = pMouseHook->pt.x;
		//evt.m_y = pMouseHook->pt.y;
		//s_pMouseHandyInputPane->OnMouseMove(evt);
		wxPoint pos;
		pos.x = pMouseHook->pt.x;
		pos.y = pMouseHook->pt.y;
		for(set<CMouseHandyInputPane*>::iterator it=s_setMouseHandyInputPane.begin(); 
				it!=s_setMouseHandyInputPane.end(); it++) {
			if(*it!=NULL && (*it)->IsShown())
				(*it)->TransMouseHook(pos, wParam);
		}
	}
	return CallNextHookEx(s_hMouseHook, nCode, wParam, lParam);
}

BOOL RegisterWindowMouseHook()
{
	if(s_nMouseHookCounter==0)
		s_hMouseHook = SetWindowsHookEx(WH_MOUSE, InputMouseProc, NULL, GetCurrentThreadId());
	s_nMouseHookCounter++;
	return TRUE;
}

BOOL UnregisterWindowMouseHook()
{
	if(s_nMouseHookCounter==0)
		return FALSE;
	s_nMouseHookCounter--;
	if(s_nMouseHookCounter==0)
	{
		UnhookWindowsHookEx(s_hMouseHook);
		s_hMouseHook=NULL;
	}
	return TRUE;
}

//lParam
//[in] Pointer to a MOUSEHOOKSTRUCT structure. 
//typedef struct {
//    POINT pt;
//    HWND hwnd;
//    UINT wHitTestCode;
//    ULONG_PTR dwExtraInfo;
//} MOUSEHOOKSTRUCT, *PMOUSEHOOKSTRUCT;


//HHOOK SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, 0);
//


IMPLEMENT_CLASS(CMouseHandyInputPane, wxDialog)

BEGIN_EVENT_TABLE(CMouseHandyInputPane, wxDialog)
EVT_SIZE( OnSize )
EVT_SET_FOCUS( OnSetFocus )
EVT_KILL_FOCUS( OnKillFocus )
EVT_COMMAND_RANGE(ID_PopupButton,ID_PopupButton+conMaxButtonNum-1,wxEVT_COMMAND_BUTTON_CLICKED,OnButton)
EVT_LIST_ITEM_SELECTED( ID_PopupList, OnListBoxSelected )
EVT_LIST_ITEM_SELECTED( ID_PopupListAll, OnListBoxSelected )
EVT_TIMER(wxID_ANY,OnTimer)
END_EVENT_TABLE()


CMouseHandyInputPane::CMouseHandyInputPane(wxWindow *parent, wxWindowID id,
										   const wxString& title,
										   const wxPoint& pos,
										   const wxSize& size,
										   long style,
										   const wxString& name)
										   :wxDialog(parent, id, title, pos, size, style, name), 
										   m_timer(this),
										   m_ptCodeHeads(NULL), 
										   m_bHasUserChange(FALSE), 
										   m_nViewStyle(0)//STYLE_SunGard)
{
	DIALOG_CREATE_LOG();
	s_setMouseHandyInputPane.insert(this);

	int nMaxShortNameLen = 0;
	std::map<std::string, PlatformStru_ProductInfo> mapProduct;
	std::map<std::string, PlatformStru_ProductInfo>::iterator itP;
	m_nCodeHeadsCount = 0;
	m_ptCodeHeads = new CODEHEADDATA[conMaxButtonNum];
	ZeroMemory(m_ptCodeHeads, sizeof(CODEHEADDATA)*conMaxButtonNum);

	string strShortNames="";
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetContractShortNames(strShortNames);

	vector<pair<string,vector<PlatformStru_ProductInfo>>> vecExProd;
	if(DEFAULT_SVR()) {
		DEFAULT_SVR()->GetExchangeID_ProductInfos(vecExProd);
		for(int i = 0; i < (int)vecExProd.size(); i++) {
			for(int k=0; k < (int)vecExProd[i].second.size(); k++) {
				mapProduct.insert(make_pair(vecExProd[i].second[k].ProductID, vecExProd[i].second[k]));
				if(nMaxShortNameLen < strlen(vecExProd[i].second[k].ProductShortName))
					nMaxShortNameLen = strlen(vecExProd[i].second[k].ProductShortName);
			}
		}
	}
	
	char* pHead = (char*)strShortNames.c_str();
	char* pHeadNext = NULL;
	BOOL bRetHasData = FALSE;
	do 
	{
		pHeadNext = GetProductSNameItem(pHead, m_ptCodeHeads[m_nCodeHeadsCount], bRetHasData);
        if(GlobalConfigManager::m_bUseComb&&bRetHasData||
            !GlobalConfigManager::m_bUseComb&&bRetHasData&&m_ptCodeHeads[m_nCodeHeadsCount].cType=='1')
		{
			m_nCodeHeadsCount++;
			if(m_nCodeHeadsCount>=conMaxButtonNum) 
			{
				m_nCodeHeadsCount=conMaxButtonNum;
				break;
			}
		}
		if(pHeadNext!=NULL)
			pHead = pHeadNext;
	} while(pHeadNext!=NULL);

//	SetSellBuyColor();
//	SetBackgroundColour(DEFAULT_COLOUR);
	wxFont oldfont=GetFont();
//	oldfont.SetWeight(wxBOLD);
	oldfont.SetPointSize(8);
	SetFont(oldfont);

	int nBWidth = nMaxShortNameLen*6;//52;//46;

	//if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_PATSAPI) {
	//	nBWidth = 72;
	//}
	m_listBox = new wxListCtrl(this, ID_PopupList, 
		wxPoint(0,0), wxSize(nBWidth*conEachRowBtnNum, 20*((m_nCodeHeadsCount+2)/conEachRowBtnNum)),wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL);
	m_listBox->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CMouseHandyInputPane::OnListKeyDown), NULL, this);
	m_listBox->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CMouseHandyInputPane::OnListItemMouseClick), NULL, this);
	m_listBox->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CMouseHandyInputPane::OnListItemMouseClick_Up), NULL, this);
	m_listBox->Connect(wxEVT_MOTION, wxMouseEventHandler(CMouseHandyInputPane::OnListMouseMove), NULL, this);
	m_listBox->Show(false);
	int nColWidth = nMaxShortNameLen*7*2;
	//if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_PATSAPI) {
	//	nColWidth = 230;
	//}
	m_listBox->InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, nColWidth);

	m_listBox_All = new wxListCtrl(this, ID_PopupListAll, 
		wxPoint(0,0), wxSize(nBWidth*conEachRowBtnNum, 20*((m_nCodeHeadsCount+2)/conEachRowBtnNum)),wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL);
	m_listBox_All->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CMouseHandyInputPane::OnListKeyDown), NULL, this);
	m_listBox_All->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CMouseHandyInputPane::OnListItemMouseClick), NULL, this);
	m_listBox_All->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CMouseHandyInputPane::OnListItemMouseClick_Up), NULL, this);
	m_listBox_All->Connect(wxEVT_MOTION, wxMouseEventHandler(CMouseHandyInputPane::OnListMouseMove), NULL, this);
	m_listBox_All->Show(false);
	nColWidth = 180;
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_PATSAPI) {
		nColWidth = 230;
	}
	m_listBox_All->InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, nColWidth);

	std::set<std::string> outData;
	// 获取合约列表
    outData.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(int i=1; it!=outData.end(); ++it,i++) {
		m_listBox_All->InsertItem(i, *it);
	}

	wxString strPrevExchangeID;
	wxString strExchangeID;
	int nSumChange = 0;
	wxButton* pButton = NULL;
	for(int i=0; i<m_nCodeHeadsCount; i++) {
		pButton = new wxButton(this, ID_PopupButton+i, wxT(m_ptCodeHeads[i].strCodeName), 
				wxPoint(1+(i%conEachRowBtnNum)*nBWidth, 1+(i/conEachRowBtnNum)*20), wxSize(nBWidth-1, 20));
		if(pButton != NULL) {
			itP = mapProduct.find(std::string(m_ptCodeHeads[i].strCodeHeads[0]));
			if(itP != mapProduct.end()) {
				strExchangeID = wxString(itP->second.ExchangeID);
				if(strPrevExchangeID.Cmp(strExchangeID)!=0) {
					nSumChange += 1;
					strPrevExchangeID = strExchangeID;
				}
			}
			if(nSumChange%2==0)//(i/conEachRowBtnNum/4)%2) 
				pButton->SetBackgroundColour(wxColour(RGB(201, 201, 201)));
			else
				pButton->SetBackgroundColour(wxColour(RGB(239, 239, 239)));
		}
	}

	SetSize(wxSize(1+nBWidth*conEachRowBtnNum, 1+20*((m_nCodeHeadsCount+2)/conEachRowBtnNum)));

	m_mapCodeToName.clear();
	InitCodeToNameMap();
	m_timer.Start(500);
}

CMouseHandyInputPane::~CMouseHandyInputPane()
{
	m_timer.Stop();
	s_setMouseHandyInputPane.erase(this);
	if(s_setMouseHandyInputPane.size()==0)
	{
		UnhookWindowsHookEx(s_hMouseHook);
		s_hMouseHook = NULL;
	}

	wxDELETE(m_ptCodeHeads)
}

int CMouseHandyInputPane::GetContractListStyle()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->nContractListStyle;
//	return 0;
}

void CMouseHandyInputPane::ShowView(wxString& strKeyInputReal, BOOL bShowCmd)
{
	wxString strKeyInput=strKeyInputReal;

	int nStyle = GetContractListStyle();

	if(nStyle == 0) {
		if(m_nViewStyle != STYLE_SunGard)
			nStyle = 1;
		else 
			nStyle = 2;
	}
	if(nStyle==1) {
		m_listBox_All->Show(false);
		if(!bShowCmd) {
			std::string strCodeHead = strKeyInputReal;
			//匹配合约
			std::set<std::string> setShowCode;
			setShowCode.clear();
			MatchInstrumentByCode(strCodeHead, false, setShowCode);
			MatchInstrumentBySimpleSpell(strCodeHead, setShowCode);

			m_listBox->DeleteAllItems();
			std::set<std::string>::iterator it = setShowCode.begin();
			for( int i = 0; it != setShowCode.end(); ++it, i++) {
				m_listBox->InsertItem( i+1, *it );
			}

			m_listBox->Show(true);

			for(int i=0; i<m_nCodeHeadsCount; i++) {
				wxWindow* pWin = FindWindowById(ID_PopupButton+i, this);
				if(pWin)
					pWin->Show(false);
			}

			m_listBox->SetFocus();

			m_bHasUserChange = TRUE;
		}
		else {
			m_listBox->Show(false);
			for(int i=0; i<m_nCodeHeadsCount; i++) {
				wxWindow* pWin = FindWindowById(ID_PopupButton+i, this);
				if(pWin)
					pWin->Show(true);
			}
			wxWindow* pWin = FindWindowById(ID_PopupButton, this);
			if(pWin)
				pWin->SetFocus();
			m_bHasUserChange = FALSE;
		}
	}
	else {
		m_listBox->Show(false);
		for(int i=0; i<conMaxButtonNum; i++) {
			wxWindow* pWin = FindWindowById(ID_PopupButton+i, this);
			if(pWin)
				pWin->Show(false);
		}
		m_listBox_All->Show(true);
		m_listBox_All->SetFocus();
	}

}

bool CMouseHandyInputPane::Show(bool show)
{
	m_bHasUserChange = FALSE;
	WXLOG_INFO( wxT("0x%lx CMouseHandyInputPane::Show %d"), long(this), int(show));
	if(show) {
		////SetCapture((HWND)m_hWnd);
		////DWORD dwThreadId = GetCurrThreadId();
		//s_hMouseHook = SetWindowsHookEx(WH_MOUSE, InputMouseProc, NULL, GetCurrentThreadId());
		RegisterWindowMouseHook();
		CfgMgr* pMgr = CFG_MGR_DEFAULT();
		if (pMgr)
		{
			LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
			if(pMainWindowCfg->bIsTop)
				SetWindowPos((HWND)GETTOPWINDOW()->GetHWND(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
		}
	}
	else {
		////ReleaseCapture();
		//UnhookWindowsHookEx(s_hMouseHook);
		//s_hMouseHook = NULL;
		UnregisterWindowMouseHook();
		CfgMgr* pMgr = CFG_MGR_DEFAULT();
		if (pMgr)
		{
			LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
			if(pMainWindowCfg->bIsTop)
				SetWindowPos((HWND)GETTOPWINDOW()->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
		}
	}
	return wxDialog::Show(show);
}

void CMouseHandyInputPane::TransMouseHook(wxPoint mousePos, UINT nMouseMSG)
{

	WXLOG_INFO( wxT("0x%lx CMouseHandyInputPane::TransMouseHook nMouseMSG %u"), long(this), nMouseMSG);

	if(!IsShown()) return;

	wxRect oriRect = GetClientRect();
	wxRect extRect = GetClientRect();
	wxPoint pos = ScreenToClient(mousePos);
	extRect.x-=2;
	extRect.y-=22;
	extRect.width+=4;
	extRect.height+=24;
	if(!extRect.Contains(pos.x, pos.y)) {
		if(!m_bHasUserChange)
			Show(false);
		else {
			if(nMouseMSG>=WM_LBUTTONDOWN && nMouseMSG<=WM_MBUTTONDBLCLK)
				Show(false);
		}
	}
	//	else {
	//		if(oriRect.Contains(pos.x, pos.y)) {
	//			if(FindFocus() != this)
	//				SetFocus();
	//		}
	//		else {
	//		}
	//	}
}

void CMouseHandyInputPane::OnSize(wxSizeEvent &event)
{
	WXLOG_INFO( wxT("0x%lx CMouseHandyInputPane::OnSize"), long(this) );
	event.Skip();
}

void CMouseHandyInputPane::OnSetFocus(wxFocusEvent &event)
{
	WXLOG_INFO( wxT("0x%lx CMouseHandyInputPane::OnSetFocus"), long(this) );
	event.Skip();
}

void CMouseHandyInputPane::OnKillFocus(wxFocusEvent &event)
{
	WXLOG_INFO( wxT("0x%lx CMouseHandyInputPane::OnKillFocus"), long(this) );
	event.Skip();
}

void CMouseHandyInputPane::OnButton(wxCommandEvent& event)
{
	//event.GetId() 获得按钮的ID号
	WXLOG_INFO( wxT("0x%lx CMouseHandyInputPane::OnButton ID %d"), long(this), event.GetId());

	std::set<std::string> outData;
	std::set<std::string> tmpList;
	int nIndex = event.GetId() - ID_PopupButton;

	std::string strCodeHead;
	std::string strText;
	std::string strTmpText;

	// 获取合约列表
    outData.clear();
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->GetInstrumentList(outData);
	
	std::set<std::string>::iterator it;

	for(int i=0; i<m_ptCodeHeads[nIndex].nCmbCodeCount; i++)
	{
		strCodeHead = m_ptCodeHeads[nIndex].strCodeHeads[i];

		std::set<std::string>::iterator it=outData.begin();
		for(; it!=outData.end(); ++it) {
			size_t pos = (*it).find(strCodeHead);
			if(pos != std::string::npos) {
				strText = *it;
				strText.replace(pos,strCodeHead.size(),"");
				if(strText[0] >= '0' && strText[0] <= '9' || strText[0] == '-' || strText[0] == ' ') {
					strTmpText = *it+"  (";
					strTmpText += m_ptCodeHeads[nIndex].strCodeName;
					strTmpText += ")";
					tmpList.insert(strTmpText);
				}
			}
		}
	}

	m_listBox->DeleteAllItems();
	it=tmpList.begin();
	for(int i=0; it!=tmpList.end(); ++it, i++) {
		m_listBox->InsertItem(i+1, *it);
	}

	m_listBox->Show(true);

	for(int i=0; i<conMaxButtonNum; i++) {
		wxWindow* pWin = FindWindowById(ID_PopupButton+i, this);
		if(pWin)
			pWin->Show(false);
	}
	m_listBox->SetFocus();

	m_bHasUserChange = TRUE;

}

wxString s_strSelectText;
DWORD s_dwMouseClickTickCount=0;

void CMouseHandyInputPane::OnListBoxSelected(wxListEvent& event)
{
}

void CMouseHandyInputPane::OnListKeyDown(wxKeyEvent& event)
{
	if(event.GetRawKeyCode() == VK_RETURN) {
		Show(false);
		wxCommandEvent evt(wxEVT_MOUSEINPUT_CODE, 0);
//		evt.SetString(s_strSelectText.c_str());
        if(1)
        {
	        DWORD EvtParamID;
	        std::string EvtParamStr(s_strSelectText.c_str());
	        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
	        {
		        evt.SetInt((int)EvtParamID);
	        }
        }
		::wxPostEvent(GetParent(),evt);
	}
	else {
		event.Skip();
	}
}

void CMouseHandyInputPane::OnListItemMouseClick_Up(wxMouseEvent &event)
{
	wxListCtrl* poCurrList = NULL;
	if(m_listBox_All->IsShown()) 
	{
		poCurrList = m_listBox_All;
	}
	else 
	{
		poCurrList = m_listBox;
	}

	int nFlag = 0;
	long nSubItem = -1;
	int nIndex = poCurrList->HitTest(event.GetPosition(), nFlag, &nSubItem);
	if(nIndex==-1)
		return;
	wxString strText = poCurrList->GetItemText(nIndex);
//	char strTemp[128];
//	ZeroMemory(strTemp, 128);
//	sscanf_s(strText.c_str(), "%[^ ]s", strTemp, sizeof(strTemp)-1);
//	s_strSelectText = strTemp;
	vector<string> outData;
	if(SplitString(strText.c_str(), outData, '(')<1) 
		return;
	s_strSelectText = outData[0];
	s_strSelectText.Trim();
	wxCommandEvent evt(wxEVT_MOUSEINPUT_CODE, 0);
//		evt.SetString(s_strSelectText.c_str());
    if(1)
    {
        DWORD EvtParamID;
        std::string EvtParamStr(s_strSelectText.c_str());
        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
        {
	        evt.SetInt((int)EvtParamID);
        }
    }
	::wxPostEvent(GetParent(),evt);
}

void CMouseHandyInputPane::OnListItemMouseClick(wxMouseEvent &event)
{
}

void CMouseHandyInputPane::OnListMouseMove(wxMouseEvent &event)
{
	wxListCtrl* poCurrList = NULL;
	if(m_listBox_All->IsShown()) 
	{
		poCurrList = m_listBox_All;
	}
	else 
	{
		poCurrList = m_listBox;
	}
	if(poCurrList==NULL)
		return;

	int nFlag = 0;
	long nSubItem = -1;
	int nIndex = poCurrList->HitTest(event.GetPosition(), nFlag, &nSubItem);
	if(nIndex==-1)
		return;
	poCurrList->SetItemState(nIndex, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    poCurrList->SetItemState(nIndex, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
}

void CMouseHandyInputPane::MatchInstrumentByCode( const std::string &strCode, bool bExactMatch, std::set<std::string>& setShowText )
{
	// 获取合约列表
	std::set<std::string> outData;
    outData.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(; it!=outData.end(); ++it) {
		std::string strTmp = (*it);
		std::string strLwrName = strlwr((char*)(strTmp.c_str()));
		std::string strLwrCode = strlwr((char*)(strCode.c_str()));
		size_t pos = strLwrName.find(strLwrCode);
		if(pos != 0 ) 
		{
			continue;
		}

		if ( bExactMatch )
		{
			//精确匹配
			std::string strText = strLwrName;
			strText.replace(pos,strLwrCode.size(),"");
			if ( strText[0] >= '0' && strText[0] <= '9' || strText[0] == ' ')
			{
				std::string strTmpText = strTmp+"  (";
				strTmpText += m_mapCodeToName[strTmp];
				strTmpText += ")";
				setShowText.insert(strTmpText);
			}
		}
		else
		{
			//模糊匹配
			std::string strTmpText = strTmp+"  (";
			strTmpText += m_mapCodeToName[strTmp];
			strTmpText += ")";
			setShowText.insert(strTmpText);
		}
	}
}

void CMouseHandyInputPane::MatchInstrumentBySimpleSpell( const std::string& strSimpleSpell, std::set<std::string>& setShowText )
{
	//简拼匹配
	for( int k = 0; k < m_nCodeHeadsCount; k++ )
	{
		std::string strTempSimpleSpell = m_ptCodeHeads[k].strCodeNameJP;
		if ( strTempSimpleSpell.find( strSimpleSpell ) == 0 )
		{
			for(int i=0; i<m_ptCodeHeads[k].nCmbCodeCount; i++)
				MatchInstrumentByCode(m_ptCodeHeads[k].strCodeHeads[i], true, setShowText );
		}
	}
}

void CMouseHandyInputPane::InitCodeToNameMap()
{
	std::set<std::string> outData;
    outData.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(; it!=outData.end(); ++it) 
	{
		std::string strTmp = (*it);
		std::string strLwrName = strlwr((char*)(strTmp.c_str()));
		for ( size_t k = 0; k < strLwrName.length(); k++)
		{
			if ( strLwrName[k] >= '0' && strLwrName[k] <= '9' || strLwrName[k] == ' ')
			{
				strLwrName = strLwrName.substr(0, k );
				TRACE(strLwrName.c_str());
				TRACE("\n");
				break;
			}
		}

		for ( int k = 0; k < m_nCodeHeadsCount; k++)
		{
			BOOL bHasFound = FALSE;
			for(int i=0; i<m_ptCodeHeads[k].nCmbCodeCount; i++)
			{
				wxString strCodeTmp(m_ptCodeHeads[k].strCodeHeads[i]);
				strCodeTmp.MakeLower();
				std::string strCodeHead = strCodeTmp.c_str();
				if ( strLwrName.compare( strCodeHead ) == 0)
				{
					m_mapCodeToName[strTmp] = m_ptCodeHeads[k].strCodeName;
					bHasFound = TRUE;
					break;
				}
			}
			if(bHasFound)
				break;
		}
	}
}

void CMouseHandyInputPane::ResetContractList()
{
	m_listBox_All->DeleteAllItems();
	std::set<std::string> outData;
	// 获取合约列表
    outData.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
	std::set<std::string>::iterator it=outData.begin();
	for(int i=1; it!=outData.end(); ++it,i++) {
		m_listBox_All->InsertItem(i, *it);
	}
}
void CMouseHandyInputPane::OnTimer(wxTimerEvent& event)
{
	HWND hwnd =::GetActiveWindow();
	DWORD PsId=0;
	GetWindowThreadProcessId(hwnd,&PsId);
	if(PsId!=GetCurrentProcessId()&&IsShown())
		Hide();
}