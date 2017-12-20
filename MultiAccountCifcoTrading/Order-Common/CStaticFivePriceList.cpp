#include "StdAfx.h"
#include "CStaticFivePriceList.h"
#include "../inc/Module-Misc/tools_util.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

IMPLEMENT_CLASS(CStaticFivePriceList, wxPanel)


BEGIN_EVENT_TABLE(CStaticFivePriceList, wxPanel)
	EVT_PAINT(OnPaint)
	EVT_LEFT_DOWN(OnLeftDown)
	EVT_SET_CURSOR(OnSetCursor)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CStaticFivePriceList::CStaticFivePriceList(wxWindow *parent,
                 wxWindowID id,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style,
                 const wxString& name):
	wxPanel(parent,id,pos,size,style,name)
{

	ZeroMemory(&m_tDepthMarketData, sizeof(PlatformStru_DepthMarketData));
	
	SetBackgroundColour(DEFAULT_COLOUR);

}

CStaticFivePriceList::~CStaticFivePriceList()
{
}

void CStaticFivePriceList::OnPaint(wxPaintEvent& event)
{
	wxWindow* window=FindWindowById(event.GetId(),this);
    wxPaintDC dc(window);
    wxRect rect(wxPoint(0,0),dc.GetSize());
    dc.GradientFillLinear(rect,GetBackgroundColour() ,GetBackgroundColour());

	dc.SetPen(wxPen(wxColour(172,168,153)));

    wxString str;
	int i=0, nRow=0;

 	wxFont fontLarge(wxSize(10, 24), wxFONTFAMILY_DEFAULT, wxNORMAL, wxNORMAL, false,
			wxT("Times New Roman"), wxFONTENCODING_ISO8859_1);
	wxFont fontSmall(wxSize(6, 12), wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL, false,
			LOADSTRING(DEFAULT_FONT), wxFONTENCODING_ISO8859_1);

	dc.SetFont(fontSmall);
	dc.SetTextForeground(wxColour(0, 0, 0));

    int Poy=5;

	IPlatformSingleSvr* pSvr=NULL;
	//if(g_pPlatformMgr->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetCurrentPlatformSvr();
	//else
	//	pSvr = DEFAULT_SVR();

	PlatformStru_InstrumentInfo outData;
    memset(&outData,0,sizeof(outData));
	if(pSvr) pSvr->GetInstrumentInfo(m_tDepthMarketData.InstrumentID, outData);
	str.Printf("%s", outData.InstrumentName);
    dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,Poy));
	nRow++;
    Poy += ROW_HEIGHT;

	dc.SetFont(fontLarge);
	dc.SetTextForeground(wxColour(255, 0, 0));

	dc.DrawText(m_tDepthMarketData.InstrumentID,wxPoint(0+ROW_WIDTH/2,Poy));
	nRow++;
    Poy += ROW_HEIGHT*2;

    //Poy += 4;

	dc.SetFont(fontSmall);
	dc.SetTextForeground(wxColour(0, 0, 0));

	nRow++;

	//画分隔先
	dc.DrawLine(wxPoint(1,Poy-2), wxPoint(rect.GetWidth()-2,Poy-2));
	nRow++;
    //Poy += 4;

 	int nRowCount = 5;
	//if(sm_nViewStyle != 3) nRowCount = 5;

	CfgMgr* pCfgMgr=CFG_MGR_DEFAULT();
	if(pCfgMgr==NULL)
		return;

	for(i=0; i<nRowCount; i++) 
    {
		str.Printf(LOADFORMATSTRING(OLS_ASK_FD,"%d"), nRowCount==5 ? 5-i : 1);//文本必须包含%d
		dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,Poy));
		switch(i) {
		case 4:
			str=Price2String(m_tDepthMarketData.AskPrice1, m_fPriceTick);
			break;
		case 3:
			str=Price2String(m_tDepthMarketData.AskPrice2, m_fPriceTick);
			break;
		case 2:
			str=Price2String(m_tDepthMarketData.AskPrice3, m_fPriceTick);
			break;
		case 1:
			str=Price2String(m_tDepthMarketData.AskPrice4, m_fPriceTick);
			break;
		case 0:
			str=Price2String(m_tDepthMarketData.AskPrice5, m_fPriceTick);
			break;
		};
		if(m_tDepthMarketData.AskPrice1 - m_tDepthMarketData.PreSettlementPrice > 0)
			dc.SetTextForeground(wxColour(255,0,0));
		else if(m_tDepthMarketData.AskPrice1 - m_tDepthMarketData.PreSettlementPrice == 0)
			dc.SetTextForeground(wxColour(0,0,0));
		else
			dc.SetTextForeground(wxColour(0,128,0));
		dc.DrawText(str,wxPoint(ROW_WIDTH*3,Poy));
		switch(i) {
		case 4:
			str=Number2String(m_tDepthMarketData.AskVolume1);
			break;
		case 3:
			str=Number2String(m_tDepthMarketData.AskVolume2);
			break;
		case 2:
			str=Number2String(m_tDepthMarketData.AskVolume3);
			break;
		case 1:
			str=Number2String(m_tDepthMarketData.AskVolume4);
			break;
		case 0:
			str=Number2String(m_tDepthMarketData.AskVolume5);
			break;
		};
		dc.SetTextForeground(wxColour(0,0,0));
		dc.DrawText(str,wxPoint(ROW_WIDTH*8+ROW_WIDTH/2,Poy));

        Poy += ROW_HEIGHT;
	}
	nRow+=nRowCount;
    //Poy += 4;

	//画分隔先
	dc.DrawLine(wxPoint(1,Poy-2), wxPoint(rect.GetWidth()-2,Poy-2));
	nRow++;
    //Poy += 4;

	//画最新价
	str=LOADSTRING(OLS_LAST);
	dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,Poy));

	str=Price2String(m_tDepthMarketData.LastPrice, m_fPriceTick);    
	if(m_tDepthMarketData.LastPrice - m_tDepthMarketData.PreSettlementPrice > 0)
		dc.SetTextForeground(wxColour(255,0,0));
	else if(m_tDepthMarketData.LastPrice - m_tDepthMarketData.PreSettlementPrice == 0)
		dc.SetTextForeground(wxColour(0,0,0));
	else
		dc.SetTextForeground(wxColour(0,128,0));
	dc.DrawText(str,wxPoint(ROW_WIDTH*3,Poy));
	
	str=Price2String(m_tDepthMarketData.LastPrice - m_tDepthMarketData.PreSettlementPrice, m_fPriceTick);
	dc.DrawText(str,wxPoint(ROW_WIDTH*8+ROW_WIDTH/2,Poy));
	nRow++;

    Poy += ROW_HEIGHT;
    //Poy += 4;


	dc.SetTextForeground(wxColour(0,0,0));

	//画分隔先
	dc.DrawLine(wxPoint(1,Poy-2), wxPoint(rect.GetWidth()-2,Poy-2));
	nRow++;
    //Poy +=4; 

	
	for(i=0; i<nRowCount; i++) {
		str.Printf(LOADFORMATSTRING(OLS_BID_FD,"%d"), i+1);//文本必须包含%d
		dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,Poy));
		switch(i) {
		case 0:
			str=Price2String(m_tDepthMarketData.BidPrice1, m_fPriceTick);
			break;
		case 1:
			str=Price2String(m_tDepthMarketData.BidPrice2, m_fPriceTick);
			break;
		case 2:
			str=Price2String(m_tDepthMarketData.BidPrice3, m_fPriceTick);
			break;
		case 3:
			str=Price2String(m_tDepthMarketData.BidPrice4, m_fPriceTick);
			break;
		case 4:
			str=Price2String(m_tDepthMarketData.BidPrice5, m_fPriceTick);
			break;
		};
		if(m_tDepthMarketData.BidPrice1 - m_tDepthMarketData.PreSettlementPrice > 0)
			dc.SetTextForeground(wxColour(255,0,0));
		else if(m_tDepthMarketData.BidPrice1 - m_tDepthMarketData.PreSettlementPrice == 0)
			dc.SetTextForeground(wxColour(0,0,0));
		else
			dc.SetTextForeground(wxColour(0,128,0));
		dc.DrawText(str,wxPoint(ROW_WIDTH*3,Poy));
		switch(i) {
		case 0:
			str=Number2String(m_tDepthMarketData.BidVolume1);
			break;
		case 1:
			str=Number2String(m_tDepthMarketData.BidVolume2);
			break;
		case 2:
			str=Number2String(m_tDepthMarketData.BidVolume3);
			break;
		case 3:
			str=Number2String(m_tDepthMarketData.BidVolume4);
			break;
		case 4:
			str=Number2String(m_tDepthMarketData.BidVolume5);
			break;
		};
		dc.SetTextForeground(wxColour(0,0,0));
		dc.DrawText(str,wxPoint(ROW_WIDTH*8+ROW_WIDTH/2,Poy));

        Poy += ROW_HEIGHT;
	}
	nRow+=nRowCount;
    //Poy += 4;

	//画分隔先
	dc.DrawLine(wxPoint(1,Poy-2), wxPoint(rect.GetWidth()-2,Poy-2));
	nRow++;
    //Poy += 4;

	//画涨跌停
	str=LOADSTRING(OLS_UPPER)+Price2String(m_tDepthMarketData.UpperLimitPrice, m_fPriceTick);
	dc.SetTextForeground(wxColour(255,0,0));
	dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,Poy));
	nRow++;
    Poy += ROW_HEIGHT;

	str=LOADSTRING(OLS_LOWER)+Price2String(m_tDepthMarketData.LowerLimitPrice, m_fPriceTick);
	dc.SetTextForeground(wxColour(0,128,0));
	dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,Poy));
	nRow++;
    Poy += ROW_HEIGHT;
    //Poy += 4;

	int nRowTotel = (nRowCount==5 ? 21 : 21-8);

	dc.DrawLine(wxPoint(0,0), wxPoint(0,Poy-1));
	dc.DrawLine(wxPoint(0,Poy-1), wxPoint(rect.GetWidth()-1,Poy-1));
	dc.DrawLine(wxPoint(rect.GetWidth()-1,Poy-1), wxPoint(rect.GetWidth()-1,0));
	dc.DrawLine(wxPoint(rect.GetWidth()-1,0), wxPoint(0,0));
	dc.SetPen(wxPen(wxColour(255,255,255)));
//	dc.DrawLine(wxPoint(1,1), wxPoint(1,ROW_HEIGHT*nRowTotel-2));

}

#define DEFCOORRECT 	wxRect rt1(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*3+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt2(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*4+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt3(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*5+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt4(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*6+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt5(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*7+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt6(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*8+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt7(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*9+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt8(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*10+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt9(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*11+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt10(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*12+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt11(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*13+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt12(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*14+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);\
	wxRect rt13(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*15+ROW_HEIGHT/2,\
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);


void CStaticFivePriceList::OnLeftDown(wxMouseEvent& event)
{
	wxStaticText* window=(wxStaticText*)FindWindowById(event.GetId(),this);
	DEFCOORRECT
	wxCoord x=event.GetX();
	wxCoord y=event.GetY();
	double val=-1.0;
	BOOL bHaveFound = TRUE;
	if(rt1.Contains(x,y)) val=m_tDepthMarketData.AskPrice5;
	else if(rt2.Contains(x,y)) val=m_tDepthMarketData.AskPrice4;
	else if(rt3.Contains(x,y)) val=m_tDepthMarketData.AskPrice3;
	else if(rt4.Contains(x,y)) val=m_tDepthMarketData.AskPrice2;
	else if(rt5.Contains(x,y)) val=m_tDepthMarketData.AskPrice1;
	else if(rt6.Contains(x,y)) val=m_tDepthMarketData.LastPrice;
	else if(rt7.Contains(x,y)) val=m_tDepthMarketData.BidPrice1;
	else if(rt8.Contains(x,y)) val=m_tDepthMarketData.BidPrice2;
	else if(rt9.Contains(x,y)) val=m_tDepthMarketData.BidPrice3;
	else if(rt10.Contains(x,y)) val=m_tDepthMarketData.BidPrice4;
	else if(rt11.Contains(x,y)) val=m_tDepthMarketData.BidPrice5;
	else if(rt12.Contains(x,y)) val=m_tDepthMarketData.UpperLimitPrice;
	else if(rt13.Contains(x,y)) val=m_tDepthMarketData.LowerLimitPrice;
	else bHaveFound=FALSE;
	if(bHaveFound) {
		wxCommandEvent evtNew(wxEVT_FIVEPRICE_PRICESELECTED, 0);
        if(1)
        {
	        DWORD EvtParamID;
	        std::string EvtParamStr(Price2String(val, m_fPriceTick).c_str());
	        if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
	        {
		        evtNew.SetInt((int)EvtParamID);
	        }
        }
//        evtNew.SetString(Price2String(val, m_fPriceTick).c_str());
		::wxPostEvent(GetParent(), evtNew);
	}
}

void CStaticFivePriceList::OnSetCursor(wxSetCursorEvent& event)
{
    wxStaticText* window=(wxStaticText*)FindWindowById(event.GetId(),this);
	DEFCOORRECT
	wxCoord x=event.GetX();
	wxCoord y=event.GetY();
	if(rt1.Contains(x,y)||rt2.Contains(x,y)||rt3.Contains(x,y)||rt4.Contains(x,y)||rt5.Contains(x,y)
			||rt6.Contains(x,y)||rt7.Contains(x,y)||rt8.Contains(x,y)||rt9.Contains(x,y)||rt10.Contains(x,y)
			||rt11.Contains(x,y)||rt12.Contains(x,y)||rt13.Contains(x,y))
	{
		window->SetCursor( wxCursor( wxCURSOR_HAND ) );
		event.Skip();
	}
}
BOOL CStaticFivePriceList::ResetDepthMarketData(PlatformStru_DepthMarketData& tDepthMarketData) 
{

	BOOL bChanged=FALSE;

#define COMP_EVALUATE(a,b) if(abs(a-b) >= numeric_limits<double>::epsilon()){bChanged=TRUE;}
#define COMP_EVALUATE_INT(a,b) if(a!=b){bChanged=TRUE;}

	if(strncmp(tDepthMarketData.InstrumentID, m_tDepthMarketData.InstrumentID, 
			sizeof(m_tDepthMarketData.InstrumentID))!=0)
		bChanged=TRUE;
	COMP_EVALUATE(m_tDepthMarketData.LowerLimitPrice,tDepthMarketData.LowerLimitPrice)
	COMP_EVALUATE(m_tDepthMarketData.UpperLimitPrice,tDepthMarketData.UpperLimitPrice)
	COMP_EVALUATE(m_tDepthMarketData.AskPrice1,tDepthMarketData.AskPrice1)
	COMP_EVALUATE_INT(m_tDepthMarketData.AskVolume1,tDepthMarketData.AskVolume1)
	COMP_EVALUATE(m_tDepthMarketData.BidPrice1,tDepthMarketData.BidPrice1)
	COMP_EVALUATE_INT(m_tDepthMarketData.BidVolume1,tDepthMarketData.BidVolume1)
	COMP_EVALUATE(m_tDepthMarketData.AskPrice2,tDepthMarketData.AskPrice2)
	COMP_EVALUATE_INT(m_tDepthMarketData.AskVolume2,tDepthMarketData.AskVolume2)
	COMP_EVALUATE(m_tDepthMarketData.BidPrice2,tDepthMarketData.BidPrice2)
	COMP_EVALUATE_INT(m_tDepthMarketData.BidVolume2,tDepthMarketData.BidVolume2)
	COMP_EVALUATE(m_tDepthMarketData.AskPrice3,tDepthMarketData.AskPrice3)
	COMP_EVALUATE_INT(m_tDepthMarketData.AskVolume3,tDepthMarketData.AskVolume3)
	COMP_EVALUATE(m_tDepthMarketData.BidPrice3,tDepthMarketData.BidPrice3)
	COMP_EVALUATE_INT(m_tDepthMarketData.BidVolume3,tDepthMarketData.BidVolume3)
	COMP_EVALUATE(m_tDepthMarketData.AskPrice4,tDepthMarketData.AskPrice4)
	COMP_EVALUATE_INT(m_tDepthMarketData.AskVolume4,tDepthMarketData.AskVolume4)
	COMP_EVALUATE(m_tDepthMarketData.BidPrice4,tDepthMarketData.BidPrice4)
	COMP_EVALUATE_INT(m_tDepthMarketData.BidVolume4,tDepthMarketData.BidVolume4)
	COMP_EVALUATE(m_tDepthMarketData.AskPrice5,tDepthMarketData.AskPrice5)
	COMP_EVALUATE_INT(m_tDepthMarketData.AskVolume5,tDepthMarketData.AskVolume5)
	COMP_EVALUATE(m_tDepthMarketData.BidPrice5,tDepthMarketData.BidPrice5)
	COMP_EVALUATE_INT(m_tDepthMarketData.BidVolume5,tDepthMarketData.BidVolume5)
	COMP_EVALUATE(m_tDepthMarketData.LastPrice,tDepthMarketData.LastPrice)
	COMP_EVALUATE(m_tDepthMarketData.PreSettlementPrice,tDepthMarketData.PreSettlementPrice)
	
	if(bChanged) {
		m_tDepthMarketData = tDepthMarketData;
	}

	return bChanged;
}