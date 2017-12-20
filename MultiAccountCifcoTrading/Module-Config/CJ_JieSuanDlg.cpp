#include "stdafx.h"
#include "../inc/cfgMgr/CfgMgr.h"
#include "CJ_JieSuanDlg.h"
#include "wx/file.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
#define CJ_JieSuanDlg_WIDTH   800
#define CJ_JieSuanDlg_HEIGHT  450
#define CJ_JieSuanDlg_MARGIN_X 25
#define CJ_JieSuanDlg_MARGIN_Y 100

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

#include <wx/html/htmlwin.h>

static CJ_JieSuanDlg* pCJ_JieSuanDlg = NULL;

static int MyCallBack(const GUIModuleID GID,const AbstractBusinessData& data)
{

	if(data.BID!= BID_RspQrySettlementInfo ||data.Size!=sizeof(DataRspQrySettlementInfo))
		return 0;

	DataRspQrySettlementInfo* pTmp = new DataRspQrySettlementInfo();
	memcpy(pTmp, &data, sizeof(DataRspQrySettlementInfo));

	wxCommandEvent evt(wxEVT_HisCal, wxID_ANY);
	evt.SetClientData(pTmp);
	if( pCJ_JieSuanDlg )
		pCJ_JieSuanDlg->AddPendingEvent(evt);

	return 0;
}

BEGIN_EVENT_TABLE(CJ_JieSuanDlg,ConfigPanelBase)
	EVT_COMMAND(wxID_ANY, wxEVT_HisCal, CJ_JieSuanDlg::OnHisCal)
	EVT_BUTTON(ID_BTN_QUERY,OnQryDayInfo)
	EVT_BUTTON(ID_BTN_SAVE_AS_FILE,OnSaveFile)
	EVT_BUTTON(ID_BTN_CENTER_SETTLEMENT,OnSettle)
	EVT_SIZE( OnSize)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
	EVT_CHOICE(ID_ChoiceUser,OnUserChoice)
END_EVENT_TABLE()

CJ_JieSuanDlg::CJ_JieSuanDlg(wxWindow *parent,
							 wxWindowID winid ,
							 const wxPoint& pos ,
							 const wxSize& size ,
							 long style,
							 const wxString& name)
							 : ConfigPanelBase(parent,winid,pos,size,style,name)
{	
	InitionInterface(); 
	pCJ_JieSuanDlg = this;
	m_pSvr=NULL;
}

void CJ_JieSuanDlg::InitionInterface()
{
	ConfigPanelBase::CreateHeader("SETTLEMENT");
	ConfigPanelBase::EnableSaveAndReturn(false);

	int offset = 30;
	wxDateTime WxDatePickerCtrl1_Date = wxDateTime::Now();
	date = new wxDatePickerCtrl(this, ID_DATE, WxDatePickerCtrl1_Date, wxPoint(150, 63+offset), wxSize(107, 20) , wxDP_DROPDOWN);
	new wxStaticText( this, wxID_STATIC,"ÓÃ»§Ñ¡Ôñ", wxPoint(280,63+offset), wxDefaultSize, 0 );
	new wxChoice( this, ID_ChoiceUser, wxPoint(360,63+offset), wxSize(80,-1), MA_CORE()->GetAccountArray() );

	m_pRadioDate = new wxRadioButton(this, ID_RADIO_DATE, LOADSTRING(SETTLEMENT_FOR_DATE), wxPoint(26, 65+offset), wxSize(90, 17), 0);
	m_pRadioDate->SetValue(true);

	m_pBtnSaveAs= new wxButton(this, ID_BTN_SAVE_AS_FILE, LOADSTRING(SAVE_AS_FILE), wxPoint(135, 20+offset), wxSize(95, 25), 0);
	m_pBtnSettlement = new wxButton(this, ID_BTN_CENTER_SETTLEMENT, LOADSTRING(EXCHANGE_CENTER_SETTLEMENT), wxPoint(249, 20+offset), wxSize(150, 25), 0);

	m_pBtnPrint = new wxButton(this, ID_BTN_PRINT, LOADSTRING(PRINT), wxPoint(249, 20+offset), wxSize(95, 25), 0);
	m_pBtnPrint->Show(false);
	m_pBtnPreview = new wxButton(this, ID_BTN_PREVIEW, LOADSTRING(PREVIEW), wxPoint(135, 20+offset), wxSize(95, 25), 0);
	m_pBtnPreview->Show(false);

	m_pBtnQuery = new wxButton(this, ID_BTN_QUERY, LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(20, 20+offset), wxSize(95, 25), 0);

	wxTextCtrl* pEdit = new wxTextCtrl( this, ID_MyEdit, wxEmptyString,  wxPoint(17, 90+offset), wxSize(850, 331), wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL);
	pEdit->SetMaxLength(0);
	pEdit->SetFont(wxFont(8, wxDEFAULT ,wxNORMAL, wxNORMAL, FALSE,"Fixedsys")); 
}

void CJ_JieSuanDlg::OnHisCal( wxCommandEvent& evt )
{
    DataRspQrySettlementInfo* pTmp = (DataRspQrySettlementInfo*)(evt.GetClientData());
    if(pTmp)
    {
        if( pTmp->RspInfoField.ErrorID!=0  )
        {
            ((wxTextCtrl*)FindWindowById(ID_MyEdit,this))->AppendText(pTmp->RspInfoField.ErrorMsg);
            ((wxButton*)FindWindowById(ID_BTN_QUERY,this))->Enable();
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
                ((wxButton*)FindWindowById(ID_BTN_QUERY,this))->Enable();
            }
        }
        delete pTmp;
    }
    else
       ((wxButton*)FindWindowById(ID_BTN_QUERY,this))->Enable();

}


CJ_JieSuanDlg::~ CJ_JieSuanDlg()
{
	wxArrayString strArr=MA_CORE()->GetAccountArray();
	for (int i=0;i<(int)strArr.size();i++)
	{
		m_pSvr=g_pPlatformMgr->GetPlatformSvr(strArr[i].c_str());
		if(m_pSvr) 
			m_pSvr->UnSubscribeBusinessData(BID_RspQrySettlementInfo,GID_SettlementInfo);
	}
	pCJ_JieSuanDlg = NULL;
}

void  CJ_JieSuanDlg::OnQryDayInfo(wxCommandEvent& event)
{
    ((wxButton*)FindWindowById(ID_BTN_QUERY,this))->Disable();
	((wxTextCtrl*)FindWindowById(ID_MyEdit,this))->Clear();
	m_AllContent.Empty();
	if(m_pSvr) m_pSvr->SubscribeBusinessData( BID_RspQrySettlementInfo, GID_SettlementInfo, MyCallBack, true);

	PlatformStru_QrySettlementInfo temp;
	memset(&temp,0,sizeof(temp) );

	wxDateTime t = ((wxDatePickerCtrl*)FindWindowById(ID_DATE,this))->GetValue();
	int year = t.GetYear();
	int mon = t.GetMonth()+1;
	int day = t.GetDay();

	TThostFtdcDateType	TradingDay;
	m_timestring.Printf(DATE_FORMAT,year,mon,day);
	memcpy( &temp.Thost.TradingDay,m_timestring.c_str(),sizeof(TradingDay) );
	if(m_pSvr) m_pSvr->ReqQrySettlementInfo( temp ) ;
}

void CJ_JieSuanDlg::OnSize(wxSizeEvent& evt)
{
	wxTextCtrl* pEdit=((wxTextCtrl*)FindWindowById(ID_MyEdit,this));

	wxRect rt = this->GetRect();
	pEdit->SetSize( rt.width-100,rt.height-150);
}

void CJ_JieSuanDlg::OnSettle(wxCommandEvent& event)
{ 
	PlatformStru_QryCFMMCTradingAccountKey temp;
	memset(&temp,0,sizeof(temp) );

	if(m_pSvr) m_pSvr->ReqQryCFMMCTradingAccountKey( temp ) ;

//	ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("https://investorservice.cfmmc.com/loginByKey.do"), NULL, SW_SHOW);
}

void  CJ_JieSuanDlg::OnSaveFile(wxCommandEvent& event)
{
	if(m_timestring.IsEmpty()) return;
	wxString strFileName=LOADSTRING(SETTLEMENT);
	strFileName+="_";
	wxString wildcard =LOADSTRING(TEXT_OR_ALL_FILE_FILTER);
	wxFileDialog dlg(this,LOADSTRING(SAVE_AS),wxEmptyString,strFileName+m_timestring+".txt",wildcard,wxSAVE);
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

void CJ_JieSuanDlg::OnLanguageChanged( wxCommandEvent& event )
{
	ConfigPanelBase::OnLanguageChanged();

	m_pBtnQuery->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	m_pBtnPreview->SetLabel(LOADSTRING(PREVIEW));
	m_pBtnPrint->SetLabel(LOADSTRING(PRINT));
	m_pBtnSaveAs->SetLabel(LOADSTRING(SAVE_AS_FILE));
	m_pBtnSettlement->SetLabel(LOADSTRING(EXCHANGE_CENTER_SETTLEMENT));
	m_pRadioDate->SetLabel(LOADSTRING(SETTLEMENT_FOR_DATE));
}
void CJ_JieSuanDlg::OnUserChoice(wxCommandEvent& evt)
{
	int sel=evt.GetInt();
	wxArrayString items=MA_CORE()->GetAccountArray(); 
	if(sel>=0&&sel<(int)items.size())
	{
		m_pSvr=g_pPlatformMgr->GetPlatformSvr(items[sel].c_str());
	}
}