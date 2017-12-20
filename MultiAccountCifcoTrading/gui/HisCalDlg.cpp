#include "stdafx.h"

#include "HisCalDlg.h"
#include "wx/file.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#define HISCALDLG_WIDTH   800
#define HISCALDLG_HEIGHT  450
#define HISCALDLG_MARGIN_X 25
#define HISCALDLG_MARGIN_Y 100
#include "../inc/MultiAccountCore/MultiAccountCore.h"	
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

static HisCalDlg* pHisCalDlg = NULL;



static int MyCallBack(const GUIModuleID GID,const AbstractBusinessData& data)
{

	if(data.BID!= BID_RspQrySettlementInfo ||data.Size!=sizeof(DataRspQrySettlementInfo))
	return 0;

	DataRspQrySettlementInfo* pTmp = new DataRspQrySettlementInfo();
	memcpy(pTmp, &data, sizeof(DataRspQrySettlementInfo));

	wxCommandEvent evt(wxEVT_HisCal, wxID_ANY);
	evt.SetClientData(pTmp);
	if( pHisCalDlg )
		 pHisCalDlg->AddPendingEvent(evt);

	 return 0;
}


HisCalDlg::HisCalDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,LOADSTRING(HISTORY_SETTLE_DLG_CAPTION),wxDefaultPosition, 
         wxSize(HISCALDLG_WIDTH,HISCALDLG_HEIGHT),
         wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxRESIZE_BORDER)

{
	m_pSvr=NULL;
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();	
	InitionInterface(); 
	pHisCalDlg = this;
}

void HisCalDlg::InitionInterface()
{

	wxBoxSizer*  BoxSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(BoxSizer);

    wxBoxSizer* itemSizer1 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer->Add(itemSizer1, 0, wxALIGN_LEFT|wxALL, 5);
	m_items=MA_CORE()->GetAccountArray();
	wxChoice* m_ChoiceCtrl = new wxChoice( this, ID_UserChoice, wxDefaultPosition, wxSize(80,-1), m_items  );
	itemSizer1->Add(m_ChoiceCtrl,0,wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxDatePickerCtrl* m_TimeCtrl = new wxDatePickerCtrl( this, ID_CALCTRL, wxDateTime(), wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN  );
    itemSizer1->Add(m_TimeCtrl,0,wxALIGN_CENTER_VERTICAL|wxALL, 5);

    
	wxButton* m_BtDay   =  new wxButton( this, ID_BtDay, LOADSTRING(GUI_SEARCH_DAILY_STATEMENT),  wxDefaultPosition, wxDefaultSize, 0 );
    itemSizer1->Add(m_BtDay,0,wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* m_BtMon   =  new wxButton( this, ID_BtMon, LOADSTRING(GUI_SEARCH_MONTHLY_STATEMENT),  wxDefaultPosition, wxDefaultSize, 0 );
	itemSizer1->Add(m_BtMon ,0,wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_BtMon->Hide();
	wxButton* m_BtSave   =  new wxButton( this, ID_BtSave, LOADSTRING(GUI_SAVE_TO_FILE),  wxDefaultPosition, wxDefaultSize, 0 );
	itemSizer1->Add(m_BtSave,0,wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* m_BtPrint  =  new wxButton( this, ID_BtPrint, LOADSTRING(PRINT),  wxDefaultPosition, wxDefaultSize, 0 );
	itemSizer1->Add(m_BtPrint,0,wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_BtPrint->Hide();
	wxButton* m_BtClose   =  new wxButton( this, wxID_OK, LOADSTRING(VK_ESC_TEXT),  wxDefaultPosition, wxDefaultSize, 0 );
	itemSizer1->Add(m_BtClose,0,wxALIGN_CENTER_VERTICAL|wxALL, 5);


    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer->Add(itemBoxSizer2, 1, wxALL, 5);
	wxTextCtrl* m_edit   =  new wxTextCtrl( this, ID_MyEdit, wxEmptyString,  wxDefaultPosition, 
        wxSize(HISCALDLG_WIDTH-HISCALDLG_MARGIN_X,HISCALDLG_HEIGHT-HISCALDLG_MARGIN_Y), wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL);
    itemBoxSizer2->Add(m_edit, 1, wxALIGN_LEFT|wxALL, 5);
	m_edit->SetMaxLength(0);
    m_edit->SetFont(wxFont(8, wxDEFAULT ,wxNORMAL, wxNORMAL, FALSE,"Fixedsys")); 
    SetMinSize(wxSize(HISCALDLG_WIDTH,HISCALDLG_HEIGHT));
}
void HisCalDlg::OnHisCal( wxCommandEvent& evt )
{
    DataRspQrySettlementInfo* pTmp = (DataRspQrySettlementInfo*)(evt.GetClientData());
    if(pTmp)
    {
        if( pTmp->RspInfoField.ErrorID!=0  )
        {
            ((wxTextCtrl*)FindWindowById(ID_MyEdit,this))->AppendText(pTmp->RspInfoField.ErrorMsg);
            ((wxButton*)FindWindowById(ID_BtDay,this))->Enable();
        }
        else
        {
            CThostFtdcSettlementInfoField & rawData= pTmp->SettlementInfoField;
            m_AllContent+=rawData.Content;
            if( pTmp->bIsLast == true)
            {
                if(m_pSvr)
                    m_pSvr->UnSubscribeBusinessData(BID_RspQrySettlementInfo, GID_SettlementInfo);
                ((wxTextCtrl*)FindWindowById(ID_MyEdit,this))->AppendText(m_AllContent);
                ((wxButton*)FindWindowById(ID_BtDay,this))->Enable();
            }
        }
        delete pTmp;
    }
    else
       ((wxButton*)FindWindowById(ID_BtDay,this))->Enable();
}

	
 HisCalDlg::~ HisCalDlg()
 {

     if(m_pSvr)
	    m_pSvr->UnSubscribeBusinessData(BID_RspQrySettlementInfo,GID_SettlementInfo);
     pHisCalDlg = NULL;
 }

 void  HisCalDlg::OnQryDayInfo(wxCommandEvent& event)
 {
     ((wxButton*)FindWindowById(ID_BtDay,this))->Disable();
	 ((wxTextCtrl*)FindWindowById(ID_MyEdit,this))->Clear();
	 m_AllContent.Empty();

     if(m_pSvr)
	    m_pSvr->SubscribeBusinessData( BID_RspQrySettlementInfo, GID_SettlementInfo, MyCallBack, true);

	 PlatformStru_QrySettlementInfo temp;
	 memset( &temp,0,sizeof(temp) );

	 wxDateTime t = ((wxDatePickerCtrl*)FindWindowById(ID_CALCTRL,this))->GetValue();
	 int year = t.GetYear();
	 int mon = t.GetMonth()+1;
	 int day = t.GetDay();

	 TThostFtdcDateType	TradingDay;
     m_timestring.Printf(DATE_FORMAT,year,mon,day);
     memcpy( &temp.Thost.TradingDay,m_timestring.c_str(),sizeof(TradingDay) );
     if(m_pSvr)
         m_pSvr->ReqQrySettlementInfo( temp ) ;
 }
BEGIN_EVENT_TABLE(HisCalDlg,wxDialog)


 EVT_COMMAND(wxID_ANY, wxEVT_HisCal, HisCalDlg::OnHisCal)
//EVT_COMMAND(wxID_ANY, wxEVT_FundPwdChange,  ModifyPwdDlg::OnRcvTFundPwdChange)
 EVT_BUTTON(ID_BtDay,OnQryDayInfo)
 EVT_BUTTON(ID_BtSave,OnSaveFile)
 EVT_SIZE(HisCalDlg::OnSize)
 EVT_CHOICE(ID_UserChoice,OnUserSel)

END_EVENT_TABLE()

void HisCalDlg::OnSize(wxSizeEvent& evt)
{
    wxTextCtrl* pEdit=((wxTextCtrl*)FindWindowById(ID_MyEdit,this));
    if(pEdit)
    {
        int width=evt.GetSize().x-HISCALDLG_MARGIN_X;
        int height=evt.GetSize().y-HISCALDLG_MARGIN_Y;
        pEdit->SetSize(width,height);
    }


}
void  HisCalDlg::OnSaveFile(wxCommandEvent& event)
{
    if(m_timestring.IsEmpty()) return;
    wxString strFileName;
	strFileName.Printf("%s_", LOADSTRING(SETTLEMENT));
    wxString wildcard = LOADSTRING(TEXT_OR_ALL_FILE_FILTER);
    wxFileDialog dlg(this, LOADSTRING(SAVE_AS),wxEmptyString,strFileName+m_timestring+".txt",wildcard,wxSAVE);
    if(dlg.ShowModal()==wxID_OK)
    {
        wxString path=dlg.GetPath();
        wxFile file(path,wxFile::write);
        wxTextCtrl* pEdit=((wxTextCtrl*)FindWindowById(ID_MyEdit,this));
        if(file.IsOpened())
        {
			wxString strValue=pEdit->GetValue();
			if(strValue.Find("\r\n")<0)
			{
				char endl1[4]="\n",end2[4]="\r\n";
				strValue.Replace(endl1,end2);
			}
            file.Write(strValue.c_str(),strValue.size());
            file.Close();
        }
    }

}
void HisCalDlg::OnUserSel(wxCommandEvent& evt)
{
	int sel=evt.GetInt();
	if(sel>=0&&sel<(int)m_items.size())
	{
		m_pSvr=g_pPlatformMgr->GetPlatformSvr(m_items[sel].c_str());
	}
}