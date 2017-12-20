
#include "StdAfx.h"
//#include "MouseHandyInputPane.h"
//
//
////class IPlatformSvr;
//extern PlatformSvrMgr* g_pPlatformMgr;
//
//
//IMPLEMENT_CLASS(MouseHandyInputPane, wxPopupTransientWindow)
//
//BEGIN_EVENT_TABLE(MouseHandyInputPane, wxPopupTransientWindow)
//    EVT_MOUSE_EVENTS( MouseHandyInputPane::OnMouse )
//    EVT_SIZE( MouseHandyInputPane::OnSize )
//    EVT_SET_FOCUS( MouseHandyInputPane::OnSetFocus )
//    EVT_KILL_FOCUS( MouseHandyInputPane::OnKillFocus )
//	EVT_COMMAND_RANGE(ID_PopupButton,ID_PopupButton+49,wxEVT_COMMAND_BUTTON_CLICKED,OnButton)
//	EVT_LIST_ITEM_SELECTED( ID_PopupList, MouseHandyInputPane::OnListBoxSelected )
//END_EVENT_TABLE()
//
//
//MouseHandyInputPane::MouseHandyInputPane( wxWindow *parent )
//:wxPopupTransientWindow( parent ), m_ptCodeHeads(NULL), m_bKeyInput(FALSE)
//{
//	
//	m_nCodeHeadsCount = 0;
//	m_ptCodeHeads = new CODEHEADDATA[100];
//	ZeroMemory(m_ptCodeHeads, sizeof(CODEHEADDATA)*100);
//
//	int nStep=0, i=0;
//	wxString strHead=LOADSTRING(MHIP_CODEHEADS);
//	char* pHead = (char*)strHead.c_str();
//	while(pHead != NULL) {
//		i=0;
//		while(pHead[i]!='\0' && pHead[i] != '#') {
//			i++;
//		}
//		switch(nStep%3) {
//		case 0:
//			strncpy(m_ptCodeHeads[nStep/3].strCodeHead, pHead, i);
//			break;
//		case 1:
//			strncpy(m_ptCodeHeads[nStep/3].strCodeName, pHead, i);
//			break;
//		case 2:
//			strncpy(m_ptCodeHeads[nStep/3].strCodeNameJP, pHead, i);
//			break;
//		}
//		if(pHead[i]!='\0') {
//			pHead = pHead + i + 1;
//		}
//		else {
//			pHead = NULL;
//		}
//		nStep++;
//	};
//	m_nCodeHeadsCount = (nStep+2)/3;
//
//	
//	m_panel = new wxScrolledWindow( this, wxID_ANY );
//    m_panel->SetBackgroundColour( *wxLIGHT_GREY );
//	m_panel->SetAutoLayout(true);
//
//    // Keep this code to verify if mouse events work, they're required if 
//    // you're making a control like a combobox where the items are highlighted
//    // under the cursor, the m_panel is set focus in the Popup() function
//    m_panel->Connect(wxEVT_MOTION,
//                     wxMouseEventHandler(MouseHandyInputPane::OnMouse),
//                     NULL, this);
//
//	
//	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);//wxBOTH);//wxVERTICAL );
//	wxBoxSizer *subSizer = NULL;
//
//	m_listBox = new wxListCtrl(m_panel, ID_PopupList, 
//			wxPoint(1,1), wxSize(45*3-2, (20+1)*9-2),wxLC_REPORT|wxLC_NO_HEADER);
//	m_listBox->Show(false);
//	//m_listBox->AcceptsFocus();
//	m_listBox->InsertColumn(0, wxEmptyString, wxLIST_FORMAT_LEFT, 120);
//
//	topSizer->Add(m_listBox, 0, wxALL, 0 );
//
//	for(i=0; i<conMaxButtonNum; i++) {
//		m_button[i] = NULL;
//	}
//	for(i=0; i<m_nCodeHeadsCount; i++) {
//		if(i%3 == 0) {
//			subSizer = new wxBoxSizer( wxHORIZONTAL );
//		}
//		m_button[i] = new wxButton(m_panel, ID_PopupButton+i, wxT(m_ptCodeHeads[i].strCodeName), 
//							wxPoint(1,1), wxSize(45, 20));
//		subSizer->Add( m_button[i], 0, wxALL, 0 );
//		if(i%3 == 2 || i+1 == m_nCodeHeadsCount) {
//			topSizer->Add(subSizer, 0, wxALL, 0 );
//		}
//	}
//
//    m_panel->SetAutoLayout( true );
//    m_panel->SetSizer( topSizer );
//    topSizer->Fit(m_panel);
//    topSizer->Fit(this);
//
//}
//
//MouseHandyInputPane::~MouseHandyInputPane()
//{
//	wxDELETE(m_ptCodeHeads)
//}
//
//void MouseHandyInputPane::Popup(wxWindow *focus)
//{
//
//#ifdef __WXMSW__
//// MSW doesn't allow to set focus to the popup window, but we need to
//// subclass the window which has the focus, and not winFocus passed in or
//// otherwise everything else breaks down
//
//	//m_focus = FindFocus();
//	//if(m_focus)
//	//{
//	//	delete m_handlerFocus;
//	//	m_handlerFocus = new wxPopupFocusHandler();
//	//	m_focus->PushEventHandler(m_handlerFocus);
//	//}
//
//#else
//#endif
//
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::Popup"), long(this) );
//
//	AcceptsFocus();
//	AcceptsFocusFromKeyboard();
//	SetFocus();
//	SetFocusFromKbd();
//	m_listBox->Show(false);
//	for(int i=0; i<conMaxButtonNum; i++) {
//		if(m_button[i])
//			m_button[i]->Show(true);
//	}
//
//    wxPopupTransientWindow::Popup(focus ? focus : m_panel);
//}
//
//void MouseHandyInputPane::OnDismiss()
//{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::OnDismiss"), long(this) );
//	m_bKeyInput = FALSE;
//	wxPopupTransientWindow::OnDismiss();
//}
//
//bool MouseHandyInputPane::ProcessLeftDown(wxMouseEvent& event)
//{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::ProcessLeftDown pos(%d, %d)"), long(this), event.GetX(), event.GetY());
//    return wxPopupTransientWindow::ProcessLeftDown(event);
//}
//bool MouseHandyInputPane::Show( bool show )
//{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::Show %d"), long(this), int(show));
//	AcceptsFocus();
//	AcceptsFocusFromKeyboard();
//	SetFocus();
//	SetFocusFromKbd();
//	if(show) {
//		if(m_bKeyInput) {
//
//			std::set<std::string> outData;
//			std::set<std::string> tmpList;
//
//			wxString strCodeHead;
//			wxString strCodeName;
//			std::string strText;
//			std::string strTmpText;
//
//			BOOL bJPFound = FALSE;
//			int nJPCodeHeadCount = 0;
//			wxString strJPCodeHeads[100];
//			wxString strJPNameHeads[100];
//
//			if(m_strKeyInput.Length()>0) {
//
//			strCodeHead = m_strKeyInput.MakeLower();
//
//			// 代码头搜索
//			for(int i=0; i<m_nCodeHeadsCount; i++) {
//				if(strCodeHead.CmpNoCase(m_ptCodeHeads[i].strCodeHead) == 0) {
//					strCodeName = m_ptCodeHeads[i].strCodeName;
//					break;
//				}
//			}
//
//			wxString strTmpCodeNameJP;
//			// 品种中文简拼搜索
//			for(int i=0; i<m_nCodeHeadsCount; i++) {
//				strTmpCodeNameJP = m_ptCodeHeads[i].strCodeNameJP;
//				strTmpCodeNameJP.LowerCase();
//				if(strTmpCodeNameJP.find(strCodeHead) == 0) {
//					strJPCodeHeads[nJPCodeHeadCount] = m_ptCodeHeads[i].strCodeHead;
//					strJPNameHeads[nJPCodeHeadCount] = m_ptCodeHeads[i].strCodeName;
//					nJPCodeHeadCount++;
//					bJPFound = TRUE;
//					break;
//				}
//			}
//			if(bJPFound) {
//				strCodeHead = strJPCodeHeads[0].MakeLower();
//				strCodeName = strJPNameHeads[0];
//			}
//
//			// 获取合约列表
//            outData.clear();
//			if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
//			std::set<std::string>::iterator it=outData.begin();
//			for(; it!=outData.end(); ++it) {
//				wxString strTmp = (*it);
//				strTmp.LowerCase();
//				size_t pos = strTmp.find(strCodeHead);
//				if(pos != std::string::npos) {
//					strText = *it;
//					strText.replace(pos,strCodeHead.size(),"");
//					if(strText[0] >= '0' && strText[0] <= '9') {
//						strTmpText = *it+"  (";
//						strTmpText += strCodeName;
//						strTmpText += ")";
//						tmpList.insert(strTmpText);
//					}
//				}
//			}
//
//			m_listBox->DeleteAllItems();
//			it=tmpList.begin();
//			for(int i=0; it!=tmpList.end(); ++it, i++) {
//				m_listBox->InsertItem(i+1, *it);
//			}
//
//			m_listBox->Show(true);
//
//			for(int i=0; i<conMaxButtonNum; i++) {
//				if(m_button[i])
//					m_button[i]->Show(false);
//			}
//
//			//m_listBox->AcceptsFocus();
//			//m_listBox->AcceptsFocusFromKeyboard();
//			//m_listBox->SetFocus();
//			//m_listBox->SetFocusFromKbd();
//			}
//		}
//
//	}
//	else {
//	}
//	m_bKeyInput = FALSE;
//    return wxPopupTransientWindow::Show(show);
//}
//
//void MouseHandyInputPane::OnSize(wxSizeEvent &event)
//{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::OnSize"), long(this) );
//    event.Skip();
//}
//
//void MouseHandyInputPane::OnSetFocus(wxFocusEvent &event)
//{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::OnSetFocus"), long(this) );
//    event.Skip();
//}
//
//void MouseHandyInputPane::OnKillFocus(wxFocusEvent &event)
//{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::OnKillFocus"), long(this) );
//    event.Skip();
//}
//
//void MouseHandyInputPane::OnMouse(wxMouseEvent &event)
//{
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::OnMouse ID %d"), long(this), event.GetId());
//	//wxRect rect(m_mouseText->GetRect());
//	//rect.SetX(-100000);
//	//rect.SetWidth(1000000);
//	//wxColour colour(*wxLIGHT_GREY);
//
//	//if (rect.Contains(event.GetPosition()))
//	//{       
//	//	colour = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
//	//wxLogMessage( wxT("0x%lx SimpleTransientPopup::OnMouse pos(%d, %d)"), long(event.GetEventObject()), event.GetX(), event.GetY());
//	//}
//
//	//if (colour != m_mouseText->GetBackgroundColour())
//	//{
//	//	m_mouseText->SetBackgroundColour(colour);
//	//	m_mouseText->Refresh();
//	//}
//    event.Skip();
//}
//
//void MouseHandyInputPane::OnButton(wxCommandEvent& event)
//{
//    //event.GetId() 获得按钮的ID号
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::OnButton ID %d"), long(this), event.GetId());
//
//	std::set<std::string> outData;
//	std::set<std::string> tmpList;
//	int nIndex = event.GetId() - ID_PopupButton;
//
//	std::string strCodeHead;
//	std::string strText;
//	std::string strTmpText;
//	strCodeHead = m_ptCodeHeads[nIndex].strCodeHead;
//
//	// 获取合约列表
//    outData.clear();
//	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
//    std::set<std::string>::iterator it=outData.begin();
//	for(; it!=outData.end(); ++it) {
//		size_t pos = (*it).find(strCodeHead);
//		if(pos != std::string::npos) {
//			strText = *it;
//			strText.replace(pos,strCodeHead.size(),"");
//			if(strText[0] >= '0' && strText[0] <= '9') {
//				strTmpText = *it+"  (";
//				strTmpText += m_ptCodeHeads[nIndex].strCodeName;
//				strTmpText += ")";
//				tmpList.insert(strTmpText);
//			}
//		}
//	}
//
//	m_listBox->DeleteAllItems();
//	it=tmpList.begin();
//	for(int i=0; it!=tmpList.end(); ++it, i++) {
//		m_listBox->InsertItem(i+1, *it);
//	}
//
//	m_listBox->Show(true);
//
//	for(int i=0; i<conMaxButtonNum; i++) {
//		if(m_button[i])
//			m_button[i]->Show(false);
//	}
//
//	m_listBox->AcceptsFocus();
//	m_listBox->AcceptsFocusFromKeyboard();
//	m_listBox->SetFocus();
//	m_listBox->SetFocusFromKbd();
//
//    event.Skip();
//}
//
//void MouseHandyInputPane::OnListBoxSelected(wxListEvent& event)
//{
//	Dismiss();
//	WXLOG_INFO( wxT("0x%lx MouseHandyInputPane::OnSpinCtrl ID %d Value %d"), long(this), event.GetId(), event.GetInt());
//	int nIndex = event.m_itemIndex;
//	wxString strText = event.GetText();
//	//char strTemp[128];
//	//ZeroMemory(strTemp, 128);
//	//sscanf_s(strText.c_str(), "%[^ ]s", strTemp, sizeof(strTemp)-1);
//	vector<string> outData;
//	if(SplitString(strText.c_str(), outData)<1) 
//		return;
//	wxCommandEvent evt(wxEVT_MOUSEINPUT_CODE, 0);
////    evt.SetString(outData[0].c_str());
//    if(1)
//    {
//	    DWORD EvtParamID;
//	    std::string EvtParamStr(outData[0].c_str());
//	    if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
//	    {
//		    evt.SetInt((int)EvtParamID);
//	    }
//    }
//    ::wxPostEvent(GetParent(),evt);
//	event.Skip();
//}
