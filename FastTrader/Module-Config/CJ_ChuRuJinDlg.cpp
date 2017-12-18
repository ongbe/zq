#include "stdafx.h"
#include "../inc/cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "CJ_ChuRuJinDlg.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace std;

static CJ_ChuRuJinDlg* s_pCJ_ChuRuJinDlg=NULL;

static int rspOnTransferCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
    if(!s_pCJ_ChuRuJinDlg)
		return 0;
	wxCommandEvent evt(wxEVT_TRANSFER_RSP, wxID_ANY);

    switch (data.BID)
    {
    case BID_RspQryTransferSerial://请求查询转帐流水响应
        {
            if(data.Size!=sizeof(DataRspQryTransferSerial))
                return 0;
            DataRspQryTransferSerial* pTmp=new DataRspQryTransferSerial();
            memcpy(pTmp, &data, sizeof(DataRspQryTransferSerial));
            evt.SetClientData(pTmp);
            break;
        }
	default:
		break;
    }

	s_pCJ_ChuRuJinDlg->AddPendingEvent(evt);

	return 0;

}


BEGIN_EVENT_TABLE(CJ_ChuRuJinDlg,ConfigPanelBase)
	//EVT_CLOSE(CJ_ChuRuJinDlg::OnClose)
	EVT_COMBOBOX(ID_Com, OnCom) 
	EVT_BUTTON(ID_BTN_QUERY,OnQuery)
	EVT_SIZE( OnSize)
    EVT_COMMAND(wxID_ANY, wxEVT_TRANSFER_RSP, OnRcvRspTransfer)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
END_EVENT_TABLE()

CJ_ChuRuJinDlg::CJ_ChuRuJinDlg(wxWindow *parent,
                       wxWindowID winid ,
                       const wxPoint& pos ,
                       const wxSize& size ,
                       long style,
                       const wxString& name)
:ConfigPanelBase(parent,winid,pos,size,style,name)
{
    s_pCJ_ChuRuJinDlg=this;

	ConfigPanelBase::CreateHeader( "TRANSFER_FUNDS" );
	ConfigPanelBase::EnableSaveAndReturn( false );

	int offset = 40;
    m_pList = new wxExtListCtrl_Obsolete(this, wxID_ANY, wxPoint(21, 50+offset), wxSize(50, 50), wxNO_3D|wxLCEXT_REPORT|wxLC_SINGLE_SEL);
	//m_pBtnCancel = new wxButton(this, ID_BTN_CANCEL, LOADSTRING(CANCEL), wxPoint(305, 14+offset), wxSize(75, 25), 0);
    m_pBtnQuery = new wxButton(this, ID_BTN_QUERY, LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(219, 14+offset), wxSize(75, 25), 0);

	wxArrayString arr;
	arr.Add("");
	arr.Add(LOADSTRING(OUTPORT_FUNDS));
	arr.Add(LOADSTRING(IMPORT_FUNDS));
	m_pCombox = new wxComboBox(this, ID_Com, wxT(""), wxPoint(71, 16+offset), wxSize(92, 20), arr, wxCB_READONLY);

    m_pStaticDirection = new wxStaticText(this, wxID_ANY, LOADSTRING(DIRCTION), wxPoint(25, 19+offset), wxDefaultSize, 0);
	m_pStaticPrompt = new wxStaticText(this, wxID_ANY, LOADSTRING(QUERY_TODAY_ONLY), wxPoint(400, 19+offset), wxDefaultSize, 0);

	int index=0;
	m_pList->InsertColumn(index++,LOADSTRING(DATE));
	m_pList->InsertColumn(index++,LOADSTRING(SEQUENCE_NUMBER));
	m_pList->InsertColumn(index++,LOADSTRING(TRANSFER_DIRECTION));
	m_pList->InsertColumn(index++,LOADSTRING(OUTPORT_FUNDS));
	m_pList->InsertColumn(index++,LOADSTRING(IMPORT_FUNDS));
	m_pList->InsertColumn(index++,LOADSTRING(CURRENCY_NAME));
	m_pList->InsertColumn(index++,LOADSTRING(FUNDS_TYPE));
	m_pList->InsertColumn(index++,LOADSTRING(BT_DETAIL_BANKNAME));
	m_pList->InsertColumn(index++,LOADSTRING(SUMMARY));
	//订阅请求查询转帐流水响应
    if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTransferSerial, GID_CHURUJIN, rspOnTransferCallBackFunc, false);
	m_dwQueryTick = 0;
}

CJ_ChuRuJinDlg::~CJ_ChuRuJinDlg()
{
    if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTransferSerial, GID_CHURUJIN);
}


void CJ_ChuRuJinDlg::OnCom(wxCommandEvent& event)	
{

}
//查询转账明细
void CJ_ChuRuJinDlg::OnQuery(wxCommandEvent& event)
{
    
	PlatformStru_QryTransferSerial QryTransferSerial;
    memset(&QryTransferSerial,0x00,sizeof(QryTransferSerial));

	m_vec.clear();
	m_InInfo.in = "";
	m_OutInfo.out = "";
	m_pList->DeleteAllItems();

	if(::GetTickCount() - m_dwQueryTick > 5000) {
		if(DEFAULT_SVR()) DEFAULT_SVR()->ReqQryTransferSerial(QryTransferSerial);
		m_dwQueryTick = ::GetTickCount();
	}

}

void CJ_ChuRuJinDlg::OnSize(wxSizeEvent& evt)
{
	wxRect rt = this->GetRect();
	m_pList->SetSize(  rt.width-150, rt.height-100);
	m_pList->SetFocus();
}

void CJ_ChuRuJinDlg::OnRcvRspTransfer(wxCommandEvent& event)
{
    bool b_end = false;
	m_dwQueryTick = 0;

	SerialInfo info;

	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(event.GetClientData());
    if(!pAbstractBusinessData)
    {
        wxASSERT(pAbstractBusinessData);
        return;
    }

    switch (pAbstractBusinessData->BID)
    {
    case   BID_RspQryTransferSerial://请求查询转帐流水响应
        {
            DataRspQryTransferSerial* pTmp= (DataRspQryTransferSerial*)(pAbstractBusinessData);
			if( pTmp->bIsLast)
			{
		        //if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTransferSerial, GID_TRANSFER);
                b_end = true;

			}

            CThostFtdcTransferSerialField* pRawData= &pTmp->TransferSerialField;
            CThostFtdcRspInfoField  *RspInfoField=&pTmp->RspInfoField;
           

            if(RspInfoField->ErrorID<0)
            {
                wxString strMsg;
                strMsg.Printf(_(" %s"),
                    pRawData->ErrorMsg
                    );
                wxMessageBox(strMsg,LOADSTRING(FAILED_TO_QUERY));
                return;
            }



            if(RspInfoField->ErrorID == CustomErrorID)//查询无记录
			{    
				wxString strMsg;
				strMsg.Printf(_(" %s"),
					RspInfoField->ErrorMsg
					);
				wxMessageBox(strMsg,LOADSTRING(QUERY_TRANSFER_FUNDS));
				return;
            }

            else
            {
				if( string(pRawData->ErrorMsg)== LOADSTRING(CHURUJIN_TRADE_SUCCESS))
				{             
					char buf[256];
					memset(buf,0x00,sizeof(buf));
					char strTradeTime[25];
					memcpy(strTradeTime,pRawData->TradingDay,sizeof(pRawData->TradingDay));
					strcat(strTradeTime,"  ");
					strcat(strTradeTime,pRawData->TradeTime);

					info.date = strTradeTime;

					//RowId= List->GetItemCount();

					sprintf(buf, "%d", pRawData->FutureSerial);
					//List->SetItem(count,1,buf);//期货流水号
					info.serial = buf;
					// List->SetItem(RowId,1,wxString(pRawData->BankAccount)); //银行账号
					//交易类型
					if((!strcmp(pRawData->TradeCode,"102001")) || (!strcmp(pRawData->TradeCode,"202001")))
					{
						info.direction = LOADSTRING(IMPORT_FUNDS);
						info.out = "0.00";

						char buf[50];
						sprintf(buf,"%g",pRawData->TradeAmount);
						info.in = buf;
						double a = atof(m_InInfo.in.c_str());
						a = a + pRawData->TradeAmount;

						sprintf(buf,"%g",a);
						m_InInfo.in = buf;
					}

					else if(!strcmp(pRawData->TradeCode,"102002") || !strcmp(pRawData->TradeCode,"202002"))
					{
						info.direction = LOADSTRING(OUTPORT_FUNDS);
						info.in = "0.00";

						char buf[50];
						sprintf(buf,"%g",pRawData->TradeAmount);
						info.out = buf;


						double a = atof(m_OutInfo.out.c_str());
						a = a + pRawData->TradeAmount;

						sprintf(buf,"%g",a);
						m_OutInfo.out = buf;
					}

					info.name = LOADSTRING(RMB);
					info.type = LOADSTRING(CASH);

					int c =atoi(pRawData->BankID);
					//c = 
					//memcpy(cc, pRawData->BankID,sizeof(pRawData->BankID));

					switch( c)
					{
					case 1:
						info.bankid = LOADSTRING(ICBC);
						break;
					case 2:
						info.bankid = LOADSTRING(ABC);
						break;

					case 3:
						info.bankid = LOADSTRING(CBC);
						break;

					case 4 :
						info.bankid = LOADSTRING(CCB);
						break;

					case 5:
						info.bankid = LOADSTRING(BC);
						break;

					case 12:
						info.bankid = LOADSTRING(CMBC);
						break;
					default:
						info.bankid = LOADSTRING(OTHER_BANK);
						break;
					}

					info.comment = pRawData->BrokerBranchID;
					m_vec.push_back( info );
				}
				break;
			}
		}

	}

	string str = m_pCombox->GetValue();
    //SerialInfo info;

	if( !b_end ) 
		return;

	if(str =="")
	{
		for(int unsigned i=0; i<m_vec.size(); i++)
		{
           info = m_vec[i];
		   m_pList->InsertItem(i,info.date);
		   m_pList->SetItem(i,1,info.serial);
		   m_pList->SetItem(i,2,info.direction);
		   m_pList->SetItem(i,3,info.out);
		   m_pList->SetItem(i,4,info.in);
		   m_pList->SetItem(i,5,info.name);
		   m_pList->SetItem(i,6,info.type);
		   m_pList->SetItem(i,7,info.bankid);
		   m_pList->SetItem(i,8,info.comment);
		}
		int count = m_pList->GetItemCount();
		if( count ==0)
			return;

	
			if( m_InInfo.in!= "")
			{
				m_pList->InsertItem(count, LOADSTRING(SUM_TEXT));
				m_pList->SetItem(count,1,"0");
				m_pList->SetItem(count,2, LOADSTRING(IMPORT_FUNDS));
				m_pList->SetItem(count,3,"0.00");
				m_pList->SetItem(count,4,m_InInfo.in);
				m_pList->SetItem(count,5,LOADSTRING(RMB));
			}
            
			if( m_OutInfo.out!="")
			{
				m_pList->InsertItem(count,LOADSTRING(SUM_TEXT));
				m_pList->SetItem(count,1,"0");
				m_pList->SetItem(count,2,LOADSTRING(OUTPORT_FUNDS));
				m_pList->SetItem(count,3,"0.00");
				m_pList->SetItem(count,4,m_OutInfo.out);
				m_pList->SetItem(count,5,LOADSTRING(RMB));
			}

	}


	else if(str == LOADSTRING(IMPORT_FUNDS))
	{
		int row =0;
		for( unsigned int i=0; i<m_vec.size(); i++)
		{
              info = m_vec[i];
			  if( info.direction ==  LOADSTRING(IMPORT_FUNDS))
			  {
				  m_pList->InsertItem(row,info.date);
				  m_pList->SetItem(row,1,info.serial);
				  m_pList->SetItem(row,2,info.direction);
				  m_pList->SetItem(row,3,info.out);
				  m_pList->SetItem(row,4,info.in);
				  m_pList->SetItem(row,5,info.name);
				  m_pList->SetItem(row,6,info.type);
				  m_pList->SetItem(row,7,info.bankid);
				  m_pList->SetItem(row,8,info.comment);
				  row ++;
			  }
		}

		int count = m_pList->GetItemCount();
		if( count ==0 )
			return;

				m_pList->InsertItem(count,LOADSTRING(SUM_TEXT));
				m_pList->SetItem(count,1,"0");
				m_pList->SetItem(count,2, LOADSTRING(IMPORT_FUNDS));
				m_pList->SetItem(count,3,"0.00");
				m_pList->SetItem(count,4,m_InInfo.in);
				m_pList->SetItem(count,5,LOADSTRING(RMB));
	}

	else
	{
		int row =0;
		for( int unsigned i=0; i<m_vec.size(); i++)
		{
              info = m_vec[i];
			  if( info.direction == LOADSTRING(OUTPORT_FUNDS))
			  {
                m_pList->InsertItem(row,info.date);
			   m_pList->SetItem(row,1,info.serial);
			   m_pList->SetItem(row,2,info.direction);
			   m_pList->SetItem(row,3,info.out);
			   m_pList->SetItem(row,4,info.in);
			   m_pList->SetItem(row,5,info.name);
			   m_pList->SetItem(row,6,info.type);
			   m_pList->SetItem(row,7,info.bankid);
			   m_pList->SetItem(row,8,info.comment);
			   row ++;
			  }
		}


		int count = m_pList->GetItemCount(); 
		if( count ==0)
			return;
		       m_pList->InsertItem(count,LOADSTRING(SUM_TEXT));
				m_pList->SetItem(count,1,"0");
				m_pList->SetItem(count,2,LOADSTRING(OUTPORT_FUNDS));
				m_pList->SetItem(count,3,"0.00");
				m_pList->SetItem(count,4,m_OutInfo.out);
				m_pList->SetItem(count,5,LOADSTRING(RMB));
	}
    delete (void*)pAbstractBusinessData;
	pAbstractBusinessData=NULL;
	event.SetClientData(NULL);



}
void CJ_ChuRuJinDlg::OnClose(wxCloseEvent& event)
{
  
}

void CJ_ChuRuJinDlg::OnLanguageChanged( wxCommandEvent& event )
{
	ConfigPanelBase::OnLanguageChanged();

	m_pBtnQuery->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));	
	//m_pBtnCancel->SetLabel(LOADSTRING(CANCEL));

	int nSel = m_pCombox->GetSelection();
	m_pCombox->Clear();
	m_pCombox->AppendString("");
	m_pCombox->AppendString(LOADSTRING(OUTPORT_FUNDS));
	m_pCombox->AppendString(LOADSTRING(IMPORT_FUNDS));
	m_pCombox->SetSelection(nSel);

	m_pStaticDirection->SetLabel(LOADSTRING(DIRCTION));
	m_pStaticPrompt->SetLabel(LOADSTRING(QUERY_TODAY_ONLY));

	m_pList->SetColumn(0,LOADSTRING(DATE));
	m_pList->SetColumn(1,LOADSTRING(SEQUENCE_NUMBER));
	m_pList->SetColumn(2,LOADSTRING(TRANSFER_DIRECTION));
	m_pList->SetColumn(3,LOADSTRING(OUTPORT_FUNDS));
	m_pList->SetColumn(4,LOADSTRING(IMPORT_FUNDS));
	m_pList->SetColumn(5,LOADSTRING(CURRENCY_NAME));
	m_pList->SetColumn(6,LOADSTRING(FUNDS_TYPE));
	m_pList->SetColumn(7,LOADSTRING(BANK_CODE));
	m_pList->SetColumn(8,LOADSTRING(SUMMARY));
}
