
#include "StdAfx.h"
#include "MouseHandyInputPaneX.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/Module-Misc2/EventParam.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

DEFINE_EVENT_TYPE(wxEVT_CODE_MOUSEINPUT)

IMPLEMENT_CLASS(MouseHandyInputPaneX, wxPopupTransientWindow)

BEGIN_EVENT_TABLE(MouseHandyInputPaneX, wxPopupTransientWindow)
    EVT_MOUSE_EVENTS( MouseHandyInputPaneX::OnMouse )
    EVT_SIZE( MouseHandyInputPaneX::OnSize )
    EVT_SET_FOCUS( MouseHandyInputPaneX::OnSetFocus )
    EVT_KILL_FOCUS( MouseHandyInputPaneX::OnKillFocus )
	EVT_COMMAND_RANGE(ID_PopupButton,ID_PopupButton+conMaxButtonNum-1,wxEVT_COMMAND_BUTTON_CLICKED,OnButton)
	EVT_LIST_ITEM_SELECTED( ID_PopupList, MouseHandyInputPaneX::OnListBoxSelected )
END_EVENT_TABLE()


MouseHandyInputPaneX::MouseHandyInputPaneX( wxWindow *parent )
:wxPopupTransientWindow( parent ), m_ptCodeHeads(NULL), m_bKeyInput(FALSE)
{
	m_mapCodeToName.clear();
	m_nCodeHeadsCount = 0;
	m_ptCodeHeads = new CODEHEADDATA[conMaxButtonNum];
	ZeroMemory(m_ptCodeHeads, sizeof(CODEHEADDATA)*conMaxButtonNum);

	string strShortNames="";
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetContractShortNames(strShortNames);
	
	char* pHead = (char*)strShortNames.c_str();
	char* pHeadNext = NULL;
	BOOL bRetHasData = FALSE;
	do 
	{
		pHeadNext = GetProductSNameItem(pHead, m_ptCodeHeads[m_nCodeHeadsCount], bRetHasData);
        if(bRetHasData)
        {
//#ifdef _USE_COMBOINSTRUMENT
		    if(GlobalConfigManager::m_bUseComb||
                m_ptCodeHeads[m_nCodeHeadsCount].cType=='1')
//#else
//    		if(m_ptCodeHeads[m_nCodeHeadsCount].cType=='1')
//#endif
		    {
			    m_nCodeHeadsCount++;
			    if(m_nCodeHeadsCount>=conMaxButtonNum) 
			    {
				    m_nCodeHeadsCount=conMaxButtonNum;
				    break;
			    }
		    }
        }
		if(pHeadNext!=NULL)
			pHead = pHeadNext;
	} while(pHeadNext!=NULL);

	int nBWidth = 52;//46;

	if(DEFAULT_SVR() && DEFAULT_SVR()->GetPlatformType()==PTYPE_PATSAPI) {
		nBWidth = 72;
	}
	wxFont oldfont=GetFont();
	oldfont.SetPointSize(8);
	SetFont(oldfont);

	m_panel = new wxScrolledWindow( this, wxID_ANY );
    m_panel->SetBackgroundColour( *wxLIGHT_GREY );
	m_panel->SetAutoLayout(true);

    // Keep this code to verify if mouse events work, they're required if 
    // you're making a control like a combobox where the items are highlighted
    // under the cursor, the m_panel is set focus in the Popup() function
    m_panel->Connect(wxEVT_MOTION,
                     wxMouseEventHandler(MouseHandyInputPaneX::OnMouse),
                     NULL, this);

	
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);//wxBOTH);//wxVERTICAL );
	wxBoxSizer *subSizer = NULL;

	int nColWidth = 113;
	if(DEFAULT_SVR() && DEFAULT_SVR()->GetPlatformType()==PTYPE_PATSAPI) {
		nColWidth = 230;
	}
	m_listBox = new wxListCtrl(m_panel, ID_PopupList, 
			wxPoint(1,1), wxSize(nBWidth*3-2, 20*((m_nCodeHeadsCount+2)/3)-2),wxLC_REPORT|wxLC_NO_HEADER);
	m_listBox->Show(false);
	//m_listBox->AcceptsFocus();
	m_listBox->InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, nColWidth);

	topSizer->Add(m_listBox, 0, wxALL, 0 );

	for(int i=0; i<m_nCodeHeadsCount; i++) {
		if(i%3 == 0) {
			subSizer = new wxBoxSizer( wxHORIZONTAL );
		}
		wxButton* button = new wxButton(m_panel, ID_PopupButton+i, wxT(m_ptCodeHeads[i].strCodeName), 
							wxPoint(1,1), wxSize(nBWidth, 20));
		subSizer->Add(button, 0, wxALL, 0 );
		if(i%3 == 2 || i+1 == m_nCodeHeadsCount) {
			topSizer->Add(subSizer, 0, wxALL, 0 );
		}
	}

    m_panel->SetAutoLayout( true );
    m_panel->SetSizer( topSizer );
    topSizer->Fit(m_panel);
    topSizer->Fit(this);

	SetSize(wxSize(1+nBWidth*3, 1+20*((m_nCodeHeadsCount+2)/3)));

}

MouseHandyInputPaneX::~MouseHandyInputPaneX()
{
	if(m_ptCodeHeads) {
		delete m_ptCodeHeads;
		m_ptCodeHeads = NULL;
	}
}

void MouseHandyInputPaneX::Popup(wxWindow *focus)
{

#ifdef __WXMSW__
// MSW doesn't allow to set focus to the popup window, but we need to
// subclass the window which has the focus, and not winFocus passed in or
// otherwise everything else breaks down

	//m_focus = FindFocus();
	//if(m_focus)
	//{
	//	delete m_handlerFocus;
	//	m_handlerFocus = new wxPopupFocusHandler();
	//	m_focus->PushEventHandler(m_handlerFocus);
	//}

#else
#endif

//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::Popup"), long(this) );

	m_listBox->Show(false);
	for(int i=0; i<m_nCodeHeadsCount; i++) {
		wxWindow* pWin = FindWindowById(ID_PopupButton, this);
		if(pWin)
			pWin->Show(true);
	}

    wxPopupTransientWindow::Popup(focus ? focus : m_panel);
}

void MouseHandyInputPaneX::OnDismiss()
{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::OnDismiss"), long(this) );
	m_bKeyInput = FALSE;
	wxPopupTransientWindow::OnDismiss();
}

bool MouseHandyInputPaneX::ProcessLeftDown(wxMouseEvent& event)
{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::ProcessLeftDown pos(%d, %d)"), long(this), event.GetX(), event.GetY());
    return wxPopupTransientWindow::ProcessLeftDown(event);
}
bool MouseHandyInputPaneX::Show( bool show )
{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::Show %d"), long(this), int(show));
	if(show) {
		if(m_bKeyInput) {
			std::string strCodeHead = m_strKeyInput;
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
				wxWindow* pWin = FindWindowById(ID_PopupButton, this);
				if(pWin)
					pWin->Show(false);
			}

			m_listBox->AcceptsFocus();
			m_listBox->AcceptsFocusFromKeyboard();
			m_listBox->SetFocus();
			m_listBox->SetFocusFromKbd();
		}
	}
	else {
		m_bKeyInput = FALSE;
	}
    return wxPopupTransientWindow::Show(show);
}

void MouseHandyInputPaneX::OnSize(wxSizeEvent &event)
{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::OnSize"), long(this) );
    event.Skip();
}

void MouseHandyInputPaneX::OnSetFocus(wxFocusEvent &event)
{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::OnSetFocus"), long(this) );
    event.Skip();
}

void MouseHandyInputPaneX::OnKillFocus(wxFocusEvent &event)
{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::OnKillFocus"), long(this) );
    event.Skip();
}

void MouseHandyInputPaneX::OnMouse(wxMouseEvent &event)
{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::OnMouse ID %d"), long(this), event.GetId());
    event.Skip();
}

void MouseHandyInputPaneX::OnButton(wxCommandEvent& event)
{
    //event.GetId() 获得按钮的ID号
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::OnButton ID %d"), long(this), event.GetId());

	std::vector<std::string> tmpList;
	int nIndex = event.GetId() - ID_PopupButton;

	std::string strCodeHead;
	std::string strText;
	std::string strTmpText;

	// 获取合约列表
	vector<InstrumentGroupInfo>::iterator itgroup;
	vector<SimpleInstrumentInfo> vec;
	vector<SimpleInstrumentInfo>::iterator itvec;
	InstrumentGroupInfo tGroup;
	SimpleInstrumentInfo tInfo;

	for(int i=0; i<m_ptCodeHeads[nIndex].nCmbCodeCount; i++)
	{

		strCodeHead = m_ptCodeHeads[nIndex].strCodeHeads[i];

		//itgroup = m_poGroup->begin();
		for(unsigned int i=0; i < m_poGroup->size(); i++) {
			tGroup = (*m_poGroup)[i];
			vec = tGroup.InstrVec;
			//itvec = vec.begin();
			for(unsigned int k=0; k < vec.size(); k++) {
				tInfo = vec[k];
				std::string strTmp = tInfo.id;
				std::string strLwrName = strlwr((char*)strTmp.c_str());
				std::string strLwrCodeHead = strlwr((char*)(strCodeHead.c_str()));
				size_t pos = strLwrName.find(strLwrCodeHead);
				if(pos != std::string::npos) {
					strText = strLwrName;
					strText.replace(pos,strLwrCodeHead.size(),"");
					if(strText[0] >= '0' && strText[0] <= '9' || strText[0] == '-') {
						strTmpText = tInfo.id+"  (";
						strTmpText += m_ptCodeHeads[nIndex].strCodeName;
						strTmpText += ")";
						tmpList.push_back(strTmpText);
					}
				}
			}
		}
	}

	m_listBox->DeleteAllItems();

	std::vector<std::string>::iterator it;

	it=tmpList.begin();
	for(int i=0; it!=tmpList.end(); ++it, i++) {
		m_listBox->InsertItem(i+1, *it);
	}

	m_listBox->Show(true);

	for(int i=0; i<m_nCodeHeadsCount; i++) {
		wxWindow* pWin = FindWindowById(ID_PopupButton, this);
		if(pWin)
			pWin->Show(false);
	}

	m_listBox->AcceptsFocus();
	m_listBox->AcceptsFocusFromKeyboard();
	m_listBox->SetFocus();
	m_listBox->SetFocusFromKbd();

    event.Skip();
}

void MouseHandyInputPaneX::OnListBoxSelected(wxListEvent& event)
{
	Dismiss();
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneX::OnSpinCtrl ID %d Value %d"), long(this), event.GetId(), event.GetInt());
	//int nIndex = event.m_itemIndex;
	wxString strText = event.GetText();
	//char strTemp[128];
	//ZeroMemory(strTemp, 128);
	//sscanf(strText.c_str(), "%[^ ]s", strTemp);
	vector<string> outData;
	if(SplitString(strText.c_str(), outData, ' ')>0) {
		wxCommandEvent evt(wxEVT_CODE_MOUSEINPUT, 0);
//		evt.SetString(outData[0].c_str());
        if(1)
        {
	        DWORD EvtParamID;
	        std::string EvtParamStr(outData[0].c_str());
	        if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
	        {
		        evt.SetInt((int)EvtParamID);
	        }
        }
		::wxPostEvent(GetParent(),evt);
	}
	event.Skip();
}

void MouseHandyInputPaneX::MatchInstrumentByCode( const std::string &strCode, bool bExactMatch, std::set<std::string>& setShowText )
{
	//代码匹配
	for(unsigned int i=0; i < m_poGroup->size(); i++) 
	{
		InstrumentGroupInfo tGroup = (*m_poGroup)[i];
		vector<SimpleInstrumentInfo> vec = tGroup.InstrVec;
		for(unsigned int j=0; j < vec.size(); j++) 
		{
			SimpleInstrumentInfo tInfo = vec[j];
			std::string strValue = tInfo.id;
			std::string strLwrName = strlwr((char*)(strValue.c_str()));
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
				if ( strText[0] >= '0' && strText[0] <= '9')
				{
					std::string strTmpText = tInfo.id+"  (";
					strTmpText += m_mapCodeToName[strValue];
					strTmpText += ")";
					setShowText.insert(strTmpText);
				}
			}
			else
			{
				//模糊匹配
				std::string strTmpText = tInfo.id+"  (";
				strTmpText += m_mapCodeToName[strValue];
				strTmpText += ")";
				setShowText.insert(strTmpText);
			}
		}
	}
}

void MouseHandyInputPaneX::MatchInstrumentBySimpleSpell( const std::string& strSimpleSpell, std::set<std::string>& setShowText )
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

void MouseHandyInputPaneX::SetCodeGroup( vector<InstrumentGroupInfo>* poGroup )
{
	m_poGroup = poGroup;
	for(unsigned int i=0; i < m_poGroup->size(); i++) 
	{
		InstrumentGroupInfo tGroup = (*m_poGroup)[i];
		vector<SimpleInstrumentInfo> vec = tGroup.InstrVec;
		for(unsigned int j=0; j < vec.size(); j++) 
		{
			SimpleInstrumentInfo tInfo = vec[j];
			std::string strLwrName = strlwr((char*)(tInfo.id.c_str()));
			for ( size_t k = 0; k < strLwrName.length(); k++)
			{
				if ( strLwrName[k] >= '0' && strLwrName[k] <= '9' )
				{
					strLwrName = strLwrName.substr(0, k );
					break;
				}
			}

			for ( int k = 0; k < m_nCodeHeadsCount; k++)
			{
				BOOL bHasFound = FALSE;
				for(int i=0; i<m_ptCodeHeads[k].nCmbCodeCount; i++)
				{
					char strText[128];
					ZeroMemory(strText, 128);
					strcpy(strText, m_ptCodeHeads[k].strCodeHeads[i]);
					std::string strCodeHead = strlwr(strText);
					if ( strLwrName.compare( strCodeHead ) == 0)
					{
						m_mapCodeToName[tInfo.id] = m_ptCodeHeads[k].strCodeName;
						bHasFound = TRUE;
						break;
					}
				}
				if(bHasFound)
					break;
			}
		}
	}
}
