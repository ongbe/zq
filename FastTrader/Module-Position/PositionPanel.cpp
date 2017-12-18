#include "StdAfx.h"
#include "PositionPanel.h"
#include "PositionDetailInfoList.h"
#include "PositionInfoList.h"
#include "ComboPositionInfoList.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc2/EventParam.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

CPositionDetailInfoList* PositionPanel::m_PositionDetailList=NULL;
CPositionInfoList* PositionPanel::m_PositionList=NULL;
CComboPositionInfoList* PositionPanel::m_ComPositionList = NULL;



BEGIN_EVENT_TABLE(PositionPanel, wxPanel)
	EVT_SIZE(OnSize)
	EVT_CFG_CHANGE(OnCfgChanged)
    EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
    EVT_BUTTON(ID_BUTTON_REQRY,OnReQry)
	EVT_COMMAND(wxID_ANY,wxEVT_REQ_REQRY,OnReConnectQry)
    EVT_RADIOBUTTON(wxID_ANY,OnRadioButton)
    EVT_SUBSCRIBE(OnSubscrible)
	EVT_PANEL_FOCUS(OnPanelFocus)

	EVT_BUTTON(ID_BUTTON_MOVEPOSITION,OnMovePosition)
	EVT_BUTTON(ID_BUTTON_SWAPPOSITION,OnSwapPosition)
	EVT_COMMAND(ID_BUTTON_MOVEPOSITION,wxEVT_POS_MOVE_SWAP_UPDATE,OnUpdatePosMoveSwapSwap)
	EVT_COMMAND(ID_BUTTON_SWAPPOSITION,wxEVT_POS_MOVE_SWAP_UPDATE,OnUpdatePosMoveSwapSwap)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_ADD,OnMAPlatformAdd)
	EVT_COMMAND(wxID_ANY, wxEVT_MA_PLAT_DELETE, OnMAPlatformDelete)
END_EVENT_TABLE()



//----------下面是订阅事件通知----------


void PositionPanel::OnSubscrible(wxCommandEvent& evt)
{
    if(!g_pPlatformMgr) return;
    vector<IPlatformSingleSvr*> pSvrs=g_pPlatformMgr->GetLogonPlatformSvr();

    for(int i=0;i<(int)pSvrs.size();i++)
    {
	    if(evt.GetInt())
	    {
	        pSvrs[i]->SubscribeBusinessData(BID_RspQryInvestorPosition, GID_POSITIONALL, CallBackFunc_BID_RspQryInvestorPosition);
	        pSvrs[i]->SubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_POSITIONALL, CallBackFunc_BID_RspQryInvestorPositionDetail);
		    pSvrs[i]->SubscribeBusinessData(BID_RspQryInvestorPositionCombDetail, GID_POSITIONALL, CallBackFunc_BID_RspQryInvestorComboPositionDetail);
	        pSvrs[i]->SubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_POSITIONALL, CallBackFunc_BID_RspQryInstrumentMarginRate);
	        pSvrs[i]->SubscribeBusinessData(BID_RtnTrade, GID_POSITIONALL, CallBackFunc_BID_RtnTrade);
	        pSvrs[i]->SubscribeBusinessData(BID_RtnOrder, GID_POSITIONALL, CallBackFunc_BID_RtnOrder);
	        pSvrs[i]->SubscribeBusinessData(BID_RtnDepthMarketData, GID_POSITIONALL, CallBackFunc_BID_RtnDepthMarketData);
	    }
	    else
	    {
	        pSvrs[i]->UnSubscribeBusinessData(BID_RspQryInvestorPosition, GID_POSITIONALL);
	        pSvrs[i]->UnSubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_POSITIONALL);
		    pSvrs[i]->UnSubscribeBusinessData(BID_RspQryInvestorPositionCombDetail, GID_POSITIONALL);
	        pSvrs[i]->UnSubscribeBusinessData(BID_RspQryInstrumentMarginRate, GID_POSITIONALL);
	        pSvrs[i]->UnSubscribeBusinessData(BID_RtnTrade, GID_POSITIONALL);
	        pSvrs[i]->UnSubscribeBusinessData(BID_RtnOrder, GID_POSITIONALL);
	        pSvrs[i]->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_POSITIONALL);
	    }
    }
}

//----------下面是事件回调函数----------

//查询持仓响应的回调函数
int PositionPanel::CallBackFunc_BID_RspQryInvestorPosition(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryInvestorPosition || data.Size!=sizeof(DataRspQryInvestorPosition) || !m_PositionList)
		return 0;

	if(((DataRspQryInvestorPosition*)&data)->bIsLast)
	{
	    wxCommandEvent evt(wxEVT_RspQryPositionLast, wxID_ANY);
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInvestorPosition),0);
        evt.SetInt((int)EvtParamID);
		m_PositionList->AddPendingEvent(evt);
	}
    return 0;
}

//查询持仓明细响应的回调函数
int PositionPanel::CallBackFunc_BID_RspQryInvestorPositionDetail(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryInvestorPositionDetail || data.Size!=sizeof(DataRspQryInvestorPositionDetail) || !m_PositionDetailList)
		return 0;

	if(((DataRspQryInvestorPositionDetail*)&data)->bIsLast)
	{
	    wxCommandEvent evt(wxEVT_RspQryPositionDetailLast, wxID_ANY);
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInvestorPositionDetail),0);
        evt.SetInt((int)EvtParamID);
		m_PositionDetailList->AddPendingEvent(evt);
	}
    return 0;
}

//查询组合持仓响应的回调函数
int PositionPanel::CallBackFunc_BID_RspQryInvestorComboPositionDetail(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryInvestorPositionCombDetail || data.Size!=sizeof(DataRspQryInvestorPositionCombDetail) || !m_ComPositionList)
		return 0;

	if(((DataRspQryInvestorPositionCombDetail*)&data)->bIsLast)
	{
		wxCommandEvent evt(wxEVT_RspQryPositionDetailCombLast, wxID_ANY);
		DWORD EvtParamID;
		CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInvestorPositionCombDetail),0);
		evt.SetInt((int)EvtParamID);
		m_ComPositionList->AddPendingEvent(evt);

        //需要同时更新持仓明细里的组合合约字段
	    wxCommandEvent evt2(wxEVT_RspQryPositionDetailCombLast, wxID_ANY);
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInvestorPositionCombDetail),0);
        evt2.SetInt((int)EvtParamID);
		m_PositionDetailList->AddPendingEvent(evt2);
	}
	return 0;
}

//查询保证金率响应的回调函数
int PositionPanel::CallBackFunc_BID_RspQryInstrumentMarginRate(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RspQryInstrumentMarginRate || data.Size!=sizeof(DataRspQryInstrumentMarginRate))
		return 0;	

    if(m_PositionDetailList)
    {
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInstrumentMarginRate),0);

        wxCommandEvent evt(wxEVT_RspQryMarginRate, wxID_ANY);
        evt.SetInt((int)EvtParamID);
        m_PositionDetailList->AddPendingEvent(evt);
    }
    if(m_PositionList)
    {
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRspQryInstrumentMarginRate),0);

        wxCommandEvent evt(wxEVT_RspQryMarginRate, wxID_ANY);
        evt.SetInt((int)EvtParamID);
        m_PositionList->AddPendingEvent(evt);
    }
    return 0;
}

//成交回报的回调函数
int PositionPanel::CallBackFunc_BID_RtnTrade(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
		return 0;	

    if(m_PositionDetailList)
    {
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnTrade),0);

        wxCommandEvent evt(wxEVT_RtnTrade, wxID_ANY);
        evt.SetInt((int)EvtParamID);
        m_PositionDetailList->AddPendingEvent(evt);
    }
    if(m_PositionList)
    {
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnTrade),0);

        wxCommandEvent evt(wxEVT_RtnTrade, wxID_ANY);
        evt.SetInt((int)EvtParamID);
        m_PositionList->AddPendingEvent(evt);
    }

	if ( m_ComPositionList )
	{
		DWORD EvtParamID;
		CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnTrade),0);

		wxCommandEvent evt(wxEVT_RtnTrade, wxID_ANY);
		evt.SetInt((int)EvtParamID);
		m_ComPositionList->AddPendingEvent(evt);
	}

    return 0;
}

//报单回报的回调函数
int PositionPanel::CallBackFunc_BID_RtnOrder(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RtnOrder || data.Size!=sizeof(DataRtnOrder))
		return 0;	

    if(m_PositionList)
    {
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnOrder),0);

        wxCommandEvent evt(wxEVT_RtnOrder, wxID_ANY);
        evt.SetInt((int)EvtParamID);
        m_PositionList->AddPendingEvent(evt);
    }
    return 0;
}

//行情回调函数
int PositionPanel::CallBackFunc_BID_RtnDepthMarketData(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!=BID_RtnDepthMarketData || data.Size!=sizeof(DataRtnDepthMarketData))
		return 0;

    DataRtnDepthMarketData& NewBusiness= (DataRtnDepthMarketData&)data;  
    //行情合约没有持仓，不管
    if(!NewBusiness.bHavePosition)
        return 0;

    if(m_PositionDetailList)
    {
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnDepthMarketData),0);

        wxCommandEvent evt(wxEVT_RtnDepthMarketData, wxID_ANY);
        evt.SetInt((int)EvtParamID);
        m_PositionDetailList->AddPendingEvent(evt);
    }
    if(m_PositionList)
    {
        DWORD EvtParamID;
        CFTEventParam::CreateEventParam(EvtParamID,NULL,NULL,&data,sizeof(DataRtnDepthMarketData),0);

        wxCommandEvent evt(wxEVT_RtnDepthMarketData, wxID_ANY);
        evt.SetInt((int)EvtParamID);
        m_PositionList->AddPendingEvent(evt);
    }
    return 0;
}





PositionPanel::PositionPanel(wxWindow *parent,
			wxWindowID winid,
			const wxPoint& pos,
			const wxSize& size,
			long style,
			const wxString& name):
	    wxPanel(parent,winid,pos,size,style,name),
        m_HaveComb(false),
        m_HaveMoveSwap(false),
        m_WxRadioButton_Position(NULL),
        m_WxRadioButton_PositionDetail(NULL),
		m_wxRadioButton_PositionCombo(NULL),
        m_pButtonMovePosition(NULL),
        m_pButtonSwapPosition(NULL),
        m_pButtonReQry(NULL)
{
	if(DEFAULT_SVR()->SupportPlugins())
		m_HaveMoveSwap=true;

	SetBackgroundColour(DEFAULT_COLOUR);
	int module_count=0;
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_POSITION ))
	{
		module_count++;
        m_PositionList=new CPositionInfoList(this,wxID_ANY);
		m_PositionList->Show(false);
		m_WxRadioButton_Position = new wxRadioButton(this, ID_WXRADIOBUTTON_Position, LOADSTRING(Name_Position), wxPoint(), wxSize(60, CommonBtnHeight));
	}

	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_POSITIONDETAIL ))
	{
		module_count++;
        m_PositionDetailList=new CPositionDetailInfoList(this,wxID_ANY);
		m_PositionDetailList->Show(false);
		m_WxRadioButton_PositionDetail = new wxRadioButton(this, ID_WXRADIOBUTTON_PositionDetail, LOADSTRING(Name_PositionDetail), wxPoint(), wxSize(80, CommonBtnHeight));
	}

	if ( GlobalConfigManager::m_bUseComb&&
        DEFAULT_SVR() && DEFAULT_SVR()->IsModuleValid( GID_POSITIONCOMB ))
	{
		module_count++;
		m_ComPositionList = new CComboPositionInfoList(this, wxID_ANY);
		m_ComPositionList->Show(false);
		m_wxRadioButton_PositionCombo = new wxRadioButton(this, ID_WXRADIOBUTTON_PositionComb, LOADSTRING(Name_PositionComb), wxPoint(), wxSize(80, CommonBtnHeight));
	}

	if ( NULL != m_PositionList )
	{
		m_WxRadioButton_Position->SetValue(true);
		m_PositionList->Show(true);
		m_ShowType = PositionShowType_Position;
	}
	else if ( NULL != m_PositionDetailList)
	{
		m_WxRadioButton_PositionDetail->SetValue(true);
		m_PositionDetailList->Show(true);
		m_ShowType = PositionShowType_PositionDetail;
	}
	else if ( NULL != m_ComPositionList )
	{
		m_wxRadioButton_PositionCombo->SetValue(true);
		m_ComPositionList->Show(true);
		m_ShowType = PositionShowType_PositionComb;
	}
	if(module_count==1)
	{
		if(m_WxRadioButton_Position)	m_WxRadioButton_Position->Hide();
		if(m_WxRadioButton_PositionDetail)	m_WxRadioButton_PositionDetail->Hide();
		if(m_wxRadioButton_PositionCombo)	m_wxRadioButton_PositionCombo->Hide();
	}
    m_pButtonReQry = new wxButton(this, ID_BUTTON_REQRY, LOADSTRING(BUTTON_REQRY_CAPTION), wxPoint(), wxSize(80,CommonBtnHeight));

    if(m_HaveMoveSwap&&m_PositionList)
    {
        m_pButtonMovePosition = new wxButton( this, ID_BUTTON_MOVEPOSITION, LOADSTRING(MOVE_POSITION), wxPoint(), wxSize(60,CommonBtnHeight), 0 );
        m_pButtonSwapPosition = new wxButton( this, ID_BUTTON_SWAPPOSITION, LOADSTRING(SWAP_POSITION), wxPoint(), wxSize(60,CommonBtnHeight), 0 );
    }

    if(g_pPlatformMgr)
    {
        vector<IPlatformSingleSvr*> pSvrs=g_pPlatformMgr->GetLogonPlatformSvr();

        for(int i=0;i<(int)pSvrs.size();i++)
        {
            if(m_ShowType==PositionShowType_Position)
                pSvrs[i]->SetCurrentPositionContent(1);
            else if(m_ShowType==PositionShowType_PositionDetail)
                pSvrs[i]->SetCurrentPositionContent(2);
            else if(m_ShowType==PositionShowType_PositionComb)
                pSvrs[i]->SetCurrentPositionContent(3);
        }
    }

    //直接订阅事件，避免初始化不同步而丢数据
	wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(1);
    OnSubscrible(myEvent);

}

PositionPanel::~PositionPanel()
{
    if(m_PositionList) { delete m_PositionList;  m_PositionList=NULL; }
    if(m_PositionDetailList) { delete m_PositionDetailList;  m_PositionDetailList=NULL; }
    if(m_WxRadioButton_Position) { delete m_WxRadioButton_Position;  m_WxRadioButton_Position=NULL; }
    if(m_WxRadioButton_PositionDetail) { delete m_WxRadioButton_PositionDetail;  m_WxRadioButton_PositionDetail=NULL; }
    if(m_pButtonReQry) { delete m_pButtonReQry;  m_pButtonReQry=NULL; }
    if(m_pButtonMovePosition) { delete m_pButtonMovePosition;  m_pButtonMovePosition=NULL; }
    if(m_pButtonSwapPosition) { delete m_pButtonSwapPosition;  m_pButtonSwapPosition=NULL; }
}

void PositionPanel::OnSize(wxSizeEvent& evt)
{
	wxRect rt=GetRect();
    int Pox,Poy;

	if(m_PositionDetailList)
		m_PositionDetailList->SetSize(0,0,rt.width,rt.height-CommonBtnHeight-2);
	if(m_PositionList)
		m_PositionList->SetSize(0,0,rt.width,rt.height-CommonBtnHeight-2);
	if (m_ComPositionList)
		m_ComPositionList->SetSize(0,0,rt.width,rt.height-CommonBtnHeight-2);

    Pox=8;
    Poy=rt.height-2-CommonBtnHeight;
    if(m_WxRadioButton_Position&&m_WxRadioButton_Position->IsShown())
    {
        m_WxRadioButton_Position->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_WxRadioButton_Position->GetRect().GetWidth()+8;
    }

    if(m_WxRadioButton_PositionDetail&&m_WxRadioButton_PositionDetail->IsShown())
    {
        m_WxRadioButton_PositionDetail->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_WxRadioButton_PositionDetail->GetRect().GetWidth()+8;
    }

	if(m_wxRadioButton_PositionCombo&&m_wxRadioButton_PositionCombo->IsShown())
	{
		m_wxRadioButton_PositionCombo->SetPosition(wxPoint(Pox,Poy));
		Pox+=m_wxRadioButton_PositionCombo->GetRect().GetWidth()+8;
	}

    Pox+=8;

    if(m_pButtonReQry)
    {
        m_pButtonReQry->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pButtonReQry->GetRect().GetWidth()+8;
    }

    Pox+=8;

    if(m_pButtonMovePosition)
    {
        m_pButtonMovePosition->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pButtonMovePosition->GetRect().GetWidth()+8;
    }
    if(m_pButtonSwapPosition)
    {
        m_pButtonSwapPosition->SetPosition(wxPoint(Pox,Poy));
        Pox+=m_pButtonSwapPosition->GetRect().GetWidth()+8;
    }
}

void PositionPanel::OnCfgChanged( wxCommandEvent& evt )
{
    if(m_PositionList)
        m_PositionList->OnCfgChanged();
    if(m_PositionDetailList)
	    m_PositionDetailList->OnCfgChanged();
	if (m_ComPositionList)
		m_ComPositionList->OnCfgChanged();
}

void PositionPanel::OnLanguageChanged( wxCommandEvent& event )
{
    if(m_PositionList)
	    m_PositionList->OnLanguageChanged(event.GetInt());
    if(m_PositionDetailList)
	    m_PositionDetailList->OnLanguageChanged(event.GetInt());
	if (m_ComPositionList)
		m_ComPositionList->OnLanguageChanged(event);
	if ( m_WxRadioButton_Position )
		m_WxRadioButton_Position->SetLabel(LOADSTRING(Name_Position));
	if ( m_WxRadioButton_PositionDetail )
		m_WxRadioButton_PositionDetail->SetLabel(LOADSTRING(Name_PositionDetail));
	if ( m_wxRadioButton_PositionCombo )
		m_wxRadioButton_PositionCombo->SetLabel(LOADSTRING(Name_PositionComb));
    if(m_pButtonMovePosition)
        m_pButtonMovePosition->SetLabel(LOADSTRING(MOVE_POSITION));
    if(m_pButtonSwapPosition)
        m_pButtonSwapPosition->SetLabel(LOADSTRING(SWAP_POSITION));
	if(m_pButtonReQry)
		m_pButtonReQry->SetLabel(LOADSTRING(BUTTON_REQRY_CAPTION));
}

void PositionPanel::OnReQry(wxCommandEvent& evt)
{
    if(!g_pPlatformMgr) return;
    vector<IPlatformSingleSvr*> pSvrs=g_pPlatformMgr->GetLogonPlatformSvr();

    for(int i=0;i<(int)pSvrs.size();i++)
    {
        if(m_ShowType==PositionShowType_Position||m_ShowType == PositionShowType_PositionComb)
        {
            PlatformStru_QryInvestorPosition QryInvestorPosition;
            pSvrs[i]->ReqQryInvestorPosition(QryInvestorPosition);
        }
        else if(m_ShowType==PositionShowType_PositionDetail)
        {
            bool bHaveCombPositionDetail = pSvrs[i]->HaveCombPositionDetail();

            PlatformStru_QryInvestorPositionDetail QryInvestorPositionDetail;
            pSvrs[i]->ReqQryInvestorPositionDetail(QryInvestorPositionDetail);

            if(bHaveCombPositionDetail)
            {
		        PlatformStru_QryInvestorPositionCombineDetail QryInvestorPositionDetail;
			    pSvrs[i]->ReqQryInvestorPositionCombineDetail(QryInvestorPositionDetail);
            }
        }
    }
}
void PositionPanel::OnReConnectQry(wxCommandEvent& evt)
{
	if(evt.GetInt()==1)//获取需要刷新的总数
	{
		long index=0,count=0;
		if(m_PositionList)
		{
			index++;
			count+=m_PositionList->GetItemCount();
		}
		if(m_PositionDetailList)
		{
			index++;
			count+=m_PositionDetailList->GetItemCount();
		}
		if (m_ComPositionList)
		{
			index++;
			count+=m_ComPositionList->GetItemCount();
		}
		evt.SetExtraLong(count/index+1);
	}
	else
	{
		OnReQry(evt);
	}

}
void PositionPanel::OnPanelFocus(wxCommandEvent& evt)
{
    if(m_ShowType==PositionShowType_Position&&m_PositionList)
	    m_PositionList->SetFocus();
    if(m_ShowType==PositionShowType_PositionDetail&&m_PositionDetailList)
	    m_PositionDetailList->SetFocus();
	if (m_ShowType == PositionShowType_PositionComb && m_ComPositionList)
		m_ComPositionList->SetFocus();
}
void PositionPanel::OnMovePosition(wxCommandEvent& evt)
{
    if(m_HaveMoveSwap&&m_PositionList)
	    m_PositionList->OnMovePosition();
}
void PositionPanel::OnSwapPosition(wxCommandEvent& evt)
{
    if(m_HaveMoveSwap&&m_PositionList)
	    m_PositionList->OnSwapPosition();
}

void PositionPanel::OnUpdatePosMoveSwapSwap(wxCommandEvent& evt)
{
    if(m_HaveMoveSwap&&m_PositionList)
	m_PositionList->OnUpdatePosMoveSwapSwap(evt);
}


void PositionPanel::OnRadioButton(wxCommandEvent& event)
{
	int id=event.GetId();
    EnumPositionShowType newShowType=m_ShowType;

	switch(id)
	{
	    case ID_WXRADIOBUTTON_Position:
            newShowType=PositionShowType_Position;
		    break;
	    case ID_WXRADIOBUTTON_PositionDetail:
            newShowType=PositionShowType_PositionDetail;
		    break;
		case ID_WXRADIOBUTTON_PositionComb:
			newShowType = PositionShowType_PositionComb;
			break;
	    default:
		    break;
	}
    if(newShowType!=m_ShowType)
    {
        m_ShowType=newShowType;
        if(m_ShowType==PositionShowType_Position)
        {
            if(m_pButtonMovePosition) m_pButtonMovePosition->Show(true);
            if(m_pButtonSwapPosition) m_pButtonSwapPosition->Show(true);
            if(m_PositionList) m_PositionList->Show(true);
            if(m_PositionDetailList) m_PositionDetailList->Show(false);
			if(m_ComPositionList) m_ComPositionList->Show(false);
        }
        else if (m_ShowType==PositionShowType_PositionDetail)
        { 
            if(m_pButtonMovePosition) m_pButtonMovePosition->Show(false);
            if(m_pButtonSwapPosition) m_pButtonSwapPosition->Show(false); 

            if(m_PositionList) m_PositionList->Show(false);
            if(m_PositionDetailList) m_PositionDetailList->Show(true);
			if(m_ComPositionList) m_ComPositionList->Show(false);
        }
		else
		{
			if(m_pButtonMovePosition) m_pButtonMovePosition->Show(false);
			if(m_pButtonSwapPosition) m_pButtonSwapPosition->Show(false); 

			if(m_PositionList) m_PositionList->Show(false);
			if(m_PositionDetailList) m_PositionDetailList->Show(false);
			if(m_ComPositionList) m_ComPositionList->Show(true);
		}

        if(g_pPlatformMgr)
        {
            vector<IPlatformSingleSvr*> pSvrs=g_pPlatformMgr->GetLogonPlatformSvr();

            for(int i=0;i<(int)pSvrs.size();i++)
            {
                if(m_ShowType==PositionShowType_Position)
                    pSvrs[i]->SetCurrentPositionContent(1);
                else if(m_ShowType==PositionShowType_PositionDetail)
                    pSvrs[i]->SetCurrentPositionContent(2);
                else if(m_ShowType==PositionShowType_PositionComb)
                    pSvrs[i]->SetCurrentPositionContent(3);
            }
        }
    }
}
void PositionPanel::OnMAPlatformAdd(wxCommandEvent& event)
{
	if(event.GetId()!=GetId()) return;

    wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(1);
	ProcessEvent(myEvent);

    wxCommandEvent PlatEvent(wxEVT_MA_PLAT_ADD);
	PlatEvent.SetInt(event.GetInt());
	if(m_PositionDetailList)
		m_PositionDetailList->ProcessEvent(PlatEvent);
	if(m_PositionList)
		m_PositionList->ProcessEvent(PlatEvent);
	if(m_ComPositionList)
		m_ComPositionList->ProcessEvent(PlatEvent);
}
void PositionPanel::OnMAPlatformDelete(wxCommandEvent& event)
{
	if(event.GetId()!=GetId()) return;

    wxCommandEvent myEvent(wxEVT_SUBSCRIBE);
	myEvent.SetInt(0);
	ProcessEvent(myEvent);
	myEvent.SetInt(1);
	ProcessEvent(myEvent);

	wxCommandEvent PlatEvent(wxEVT_MA_PLAT_DELETE);
	PlatEvent.SetInt(event.GetInt());
	if(m_PositionDetailList)
		m_PositionDetailList->ProcessEvent(PlatEvent);
	if(m_PositionList)
		m_PositionList->ProcessEvent(PlatEvent);
	if(m_ComPositionList)
		m_ComPositionList->ProcessEvent(PlatEvent);
}
