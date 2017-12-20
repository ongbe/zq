#include "StdAfx.h"
#include "CZQEditTextCtrl.h"
#include "../inc/Module-Misc/tools_util.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

#define ROW_HEIGHT  13
#define ROW_WIDTH   12
#define WDPANEL_WIDTH 146



BEGIN_EVENT_TABLE(CZQEditTextCtrl, wxStaticText)
	EVT_PAINT(OnPaint)
	EVT_LEFT_DOWN(OnLeftDown)
	EVT_SET_CURSOR(OnSetCursor)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CZQEditTextCtrl::CZQEditTextCtrl(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style,
                 const wxString& name):
	wxStaticText(parent,id,label,pos,size,style,name),
	m_fPriceTick(1.0f)
{

	ZeroMemory(&m_tDepthMarketData, sizeof(PlatformStru_DepthMarketData));
	
	SetBackgroundColour(DEFAULT_COLOUR);

}

CZQEditTextCtrl::~CZQEditTextCtrl()
{
}

wxString CZQEditTextCtrl::Number2String(int num)
{
    wxString str="-";
    if(num!=-1) 
       str.Printf("%d",num);
    return str;
}

wxString CZQEditTextCtrl::Double2String(double num)
{
    wxString strPriceText="-";
    if(num!=-1.0f&&(!util::isInvalidValue(num))&&num!=0.0f) 
    {
        //一位，二位，没有小数位
        int percentnum=(int)(m_fPriceTick*100);
        if(percentnum%10!=0)//二位小数位
        {
            strPriceText.Printf("%.2f",num);
        }
        else if(percentnum%100!=0)//一位小数位
        {
            strPriceText.Printf("%.1f",num);
        }
        else//没有小数位
        {
            strPriceText.Printf("%.0f",num);
        }
    }
    return strPriceText;
}

void CZQEditTextCtrl::OnPaint(wxPaintEvent& event)
{
	wxWindow* window=FindWindowById(event.GetId(),this);
    wxPaintDC dc(window);
    wxRect rect(wxPoint(0,0),dc.GetSize());
    dc.GradientFillLinear(rect,GetBackgroundColour() ,GetBackgroundColour());

	dc.SetPen(wxPen(wxColour(172,168,153)));

    wxString str;
	int i=0, nRow=0;

 	wxFont fontLarge(wxSize(12, 24), wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL, false,
			wxT("宋体"), wxFONTENCODING_ISO8859_1);
	wxFont fontSmall(wxSize(6, 12), wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL, false,
			wxT("宋体"), wxFONTENCODING_ISO8859_1);

	dc.SetFont(fontSmall);
	dc.SetTextForeground(wxColour(0, 0, 0));

	PlatformStru_InstrumentInfo outData;
    memset(&outData,0,sizeof(outData));
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(m_tDepthMarketData.InstrumentID, outData);
	str.Printf("%s", outData.InstrumentName);
    dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,nRow*ROW_HEIGHT+ROW_HEIGHT/2));
	nRow++;

	dc.SetFont(fontLarge);
	dc.SetTextForeground(wxColour(255, 0, 0));

	dc.DrawText(m_tDepthMarketData.InstrumentID,wxPoint(0+ROW_WIDTH/2,nRow*ROW_HEIGHT+ROW_HEIGHT/2));
	nRow++;

	dc.SetFont(fontSmall);
	dc.SetTextForeground(wxColour(0, 0, 0));

	//// 画卖一量
	//str=Double2String(m_DynamicItem.SellQty);
 //   dc.DrawText(str,wxPoint(ROW_WIDTH*8+ROW_WIDTH/2,nRow*ROW_HEIGHT+ROW_HEIGHT/2));
	nRow++;

	//画分隔先
	dc.DrawLine(wxPoint(1,ROW_HEIGHT*(nRow+1)-1), wxPoint(rect.GetWidth()-2,ROW_HEIGHT*(nRow+1)-1));
	nRow++;

 	int nRowCount = 5;
	//if(sm_nViewStyle != 3) nRowCount = 5;

	std::string strSell = LOADSTRING(GUI_SELL);
	for(i=0; i<nRowCount; i++) {
		if(i==4 || i+1==nRowCount) {
			str.Printf("%s%d  ", strSell.c_str(), nRowCount==5 ? 5-i : 1);
			dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
			str=Double2String(m_tDepthMarketData.AskPrice1);
			if(m_tDepthMarketData.AskPrice1 - m_tDepthMarketData.PreSettlementPrice > 0)
				dc.SetTextForeground(wxColour(255,0,0));
			else if(m_tDepthMarketData.AskPrice1 - m_tDepthMarketData.PreSettlementPrice == 0)
				dc.SetTextForeground(wxColour(0,0,0));
			else
				dc.SetTextForeground(wxColour(0,128,0));
			dc.DrawText(str,wxPoint(ROW_WIDTH*3,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
			str=Number2String(m_tDepthMarketData.AskVolume1);
			dc.SetTextForeground(wxColour(0,0,0));
			dc.DrawText(str,wxPoint(ROW_WIDTH*8+ROW_WIDTH/2,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
		}
		else {
			str.Printf("%s%d  -          -", strSell.c_str(),5-i);
			dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
		}
	}
	nRow+=nRowCount;

	//画分隔先
	dc.DrawLine(wxPoint(1,ROW_HEIGHT*(nRow+1)-1), wxPoint(rect.GetWidth()-2,ROW_HEIGHT*(nRow+1)-1));
	nRow++;

	//画最新价
	str = LOADSTRING(OLS_LAST);
	dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,ROW_HEIGHT*nRow+ROW_HEIGHT/2));

	str=Double2String(m_tDepthMarketData.LastPrice);    
	if(m_tDepthMarketData.LastPrice - m_tDepthMarketData.PreSettlementPrice > 0)
		dc.SetTextForeground(wxColour(255,0,0));
	else if(m_tDepthMarketData.LastPrice - m_tDepthMarketData.PreSettlementPrice == 0)
		dc.SetTextForeground(wxColour(0,0,0));
	else
		dc.SetTextForeground(wxColour(0,128,0));
	dc.DrawText(str,wxPoint(ROW_WIDTH*3,ROW_HEIGHT*nRow+ROW_HEIGHT/2));
	
	if(m_tDepthMarketData.LastPrice >= m_tDepthMarketData.LowerLimitPrice)
		str=Double2String(m_tDepthMarketData.LastPrice - m_tDepthMarketData.PreSettlementPrice);//PreClosePrice);
	else
		str=wxT("-");
	dc.DrawText(str,wxPoint(ROW_WIDTH*8+ROW_WIDTH/2,ROW_HEIGHT*nRow+ROW_HEIGHT/2));
	nRow++;

	dc.SetTextForeground(wxColour(0,0,0));

	//画分隔先
	dc.DrawLine(wxPoint(1,ROW_HEIGHT*(nRow+1)-1), wxPoint(rect.GetWidth()-2,ROW_HEIGHT*(nRow+1)-1));
	nRow++;

	std::string strBuy = LOADSTRING(GUI_BUY);
	for(i=0; i<nRowCount; i++) {
		if(i==0) {
			str.Printf("%s%d  ", strBuy.c_str(),i+1);
			dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
			str=Double2String(m_tDepthMarketData.BidPrice1);    
			if(m_tDepthMarketData.BidPrice1 - m_tDepthMarketData.PreSettlementPrice > 0)
				dc.SetTextForeground(wxColour(255,0,0));
			else if(m_tDepthMarketData.BidPrice1 - m_tDepthMarketData.PreSettlementPrice == 0)
				dc.SetTextForeground(wxColour(0,0,0));
			else
				dc.SetTextForeground(wxColour(0,128,0));
			dc.DrawText(str,wxPoint(ROW_WIDTH*3,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
			str=Number2String(m_tDepthMarketData.BidVolume1);
			dc.SetTextForeground(wxColour(0,0,0));
			dc.DrawText(str,wxPoint(ROW_WIDTH*8+ROW_WIDTH/2,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
		}
		else {
			str.Printf("%s%d  -          -", strBuy.c_str(),i+1);
			dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
		}
//		str.Printf("买%d -        -", i+1);
//		dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,ROW_HEIGHT*(nRow+i)+ROW_HEIGHT/2));
	}
	nRow+=nRowCount;

	//画分隔先
	dc.DrawLine(wxPoint(1,ROW_HEIGHT*(nRow+1)-1), wxPoint(rect.GetWidth()-2,ROW_HEIGHT*(nRow+1)-1));
	nRow++;

	////画买一量
	//str=Double2String(m_DynamicItem.BuyQty);
 //   dc.DrawText(str,wxPoint(ROW_WIDTH*8+ROW_WIDTH/2,nRow*ROW_HEIGHT+ROW_HEIGHT/2));
	//nRow++;

	//画涨跌停
	str=LOADSTRING(OLS_UPPER)+Double2String(m_tDepthMarketData.UpperLimitPrice);//.Printf("涨停 % 5.02f", m_DynamicItem.UpperLimitPrice);
	dc.SetTextForeground(wxColour(255,0,0));
	dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,nRow*ROW_HEIGHT+ROW_HEIGHT/2));
	nRow++;

	str=LOADSTRING(OLS_LOWER)+Double2String(m_tDepthMarketData.LowerLimitPrice);//.Printf("跌停 % 5.02f", m_DynamicItem.LowerLimitPrice);
	dc.SetTextForeground(wxColour(0,128,0));
	dc.DrawText(str,wxPoint(0+ROW_WIDTH/2,nRow*ROW_HEIGHT+ROW_HEIGHT/2));
	nRow++;

	int nRowTotel = (nRowCount==5 ? 21 : 21-8);

	dc.DrawLine(wxPoint(0,0), wxPoint(0,ROW_HEIGHT*nRowTotel-1));
	dc.DrawLine(wxPoint(0,ROW_HEIGHT*nRowTotel-1), wxPoint(rect.GetWidth()-1,ROW_HEIGHT*nRowTotel-1));
	dc.DrawLine(wxPoint(rect.GetWidth()-1,ROW_HEIGHT*nRowTotel-1), wxPoint(rect.GetWidth()-1,0));
	dc.DrawLine(wxPoint(rect.GetWidth()-1,0), wxPoint(0,0));
	dc.SetPen(wxPen(wxColour(255,255,255)));
	dc.DrawLine(wxPoint(1,1), wxPoint(1,ROW_HEIGHT*nRowTotel-2));

}

void CZQEditTextCtrl::OnLeftDown(wxMouseEvent& event)
{
	wxStaticText* window=(wxStaticText*)FindWindowById(event.GetId(),this);
	wxRect rt1(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*8+ROW_HEIGHT/2,
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	wxRect rt2(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*10+ROW_HEIGHT/2,
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	wxRect rt3(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*12+ROW_HEIGHT/2,
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	wxRect rt4(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*18+ROW_HEIGHT/2,
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	wxRect rt5(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*19+ROW_HEIGHT/2,
			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	wxCoord x=event.GetX();
	wxCoord y=event.GetY();
	double val=-1.0f;
	if(rt1.Contains(x,y)) val=m_tDepthMarketData.AskPrice1;
	if(rt2.Contains(x,y)) val=m_tDepthMarketData.LastPrice;
	if(rt3.Contains(x,y)) val=m_tDepthMarketData.BidPrice1;
	if(rt4.Contains(x,y)) val=m_tDepthMarketData.UpperLimitPrice;
	if(rt5.Contains(x,y)) val=m_tDepthMarketData.LowerLimitPrice;
	//if(val!=-1)
	if(val > -1.0f) {
		wxCommandEvent evtNew(wxEVT_FIVEPRICE_PRICESELECTED, 0);
//		evtNew.SetString(Double2String(val).c_str());
        if(1)
        {
	        DWORD EvtParamID;
	        std::string EvtParamStr(Double2String(val).c_str());
	        if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
	        {
		        evtNew.SetInt((int)EvtParamID);
	        }
        }
		::wxPostEvent(GetParent(), evtNew);
		event.Skip();
		//SetContractPrice(val);	// 用发送点击价格相消息代替
	}
}

void CZQEditTextCtrl::OnSetCursor(wxSetCursorEvent& event)
{
    wxStaticText* window=(wxStaticText*)FindWindowById(event.GetId(),this);
//	if(sm_nViewStyle == 3) {
		wxRect rt1(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*8+ROW_HEIGHT/2,
				ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
		wxRect rt2(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*10+ROW_HEIGHT/2,
				ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
		wxRect rt3(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*12+ROW_HEIGHT/2,
				ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
		wxRect rt4(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*18+ROW_HEIGHT/2,
				ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
		wxRect rt5(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*19+ROW_HEIGHT/2,
				ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
		wxCoord x=event.GetX();
		wxCoord y=event.GetY();
		if(rt1.Contains(x,y)||rt2.Contains(x,y)||rt3.Contains(x,y)||rt4.Contains(x,y)||rt5.Contains(x,y))
		{
			window->SetCursor( wxCursor( wxCURSOR_HAND ) );
			event.Skip();
		}
	//}
	//else {
	//	wxRect rt1(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*4+ROW_HEIGHT/2,
	//			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	//	wxRect rt2(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*6+ROW_HEIGHT/2,
	//			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	//	wxRect rt3(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*8+ROW_HEIGHT/2,
	//			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	//	wxRect rt4(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*10+ROW_HEIGHT/2,
	//			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	//	wxRect rt5(ROW_WIDTH*2+ROW_WIDTH/2,ROW_HEIGHT*11+ROW_HEIGHT/2,
	//			ROW_WIDTH*5+ROW_WIDTH/2,ROW_HEIGHT);
	//	wxCoord x=event.GetX();
	//	wxCoord y=event.GetY();
	//	if(rt1.Contains(x,y)||rt2.Contains(x,y)||rt3.Contains(x,y)||rt4.Contains(x,y)||rt5.Contains(x,y))
	//	{
	//		window->SetCursor( wxCursor( wxCURSOR_HAND ) );
	//		event.Skip();
	//	}
	//}

}
