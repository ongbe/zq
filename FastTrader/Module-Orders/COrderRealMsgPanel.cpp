#include "StdAfx.h"
#include "COrderRealMsgPanel.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "../inc/Module-Orders/COrderInsertEntity.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;


IMPLEMENT_CLASS(COrderRealMsgPanel, wxPanel)


BEGIN_EVENT_TABLE(COrderRealMsgPanel, wxPanel)
	EVT_SIZE(OnSize)
//	EVT_PAINT(OnPaint)
//	EVT_LEFT_DOWN(OnLeftDown)
//	EVT_SET_CURSOR(OnSetCursor)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


COrderRealMsgPanel::COrderRealMsgPanel(wxWindow *parent,
                                       wxWindowID id,
                                       const wxPoint& pos,
                                       const wxSize& size,
                                       long style,
                                       const wxString& name):
wxPanel(parent,id,pos,size,style,name)
{

    SetBackgroundColour(DEFAULT_COLOUR);
    wxArrayString arrString;
	wxString strTemp;
	if(DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) 
	{
		strTemp = LOADSTRING(FUNDS_AVAILABLE);
		arrString.Add(strTemp);
		strTemp.Printf(LOADSTRING(OPEN_QTY));
		arrString.Add(strTemp);
		strTemp.Printf(LOADFORMATSTRING(CLOSE_OR_TODAY_F,"%d%d"), 0, 0);//文本必须包含%d,%d
		arrString.Add(strTemp);
	}
	strTemp.Printf(LOADFORMATSTRING(DEFAULTQTY_MUL,"%d%d"),1, 1); //文本必须包含%d,%d
	arrString.Add(strTemp);
    wxListBox* itemListBox1 = new wxListBox( this, ID_LISTORDERREALMSG, wxPoint(0, 0), wxSize(174, 64),arrString);



}

COrderRealMsgPanel::~COrderRealMsgPanel()
{
}

int COrderRealMsgPanel::GetMKTOpenVolumeMode() 
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->nMKTPriceUse;
}

void COrderRealMsgPanel::ResetMsg(wxString& strCode, BOOL bIsBuy, BOOL bIsHedge, 
                                  double fPrice, int nDefaultVolume, int nMultiplyVolume,
								  bool bIsMarketPrice, wxString strAccount, 
								  BOOL bUpdatePreClose)
{
    if(DEFAULT_SVR()==NULL) return;
    PlatformStru_DepthMarketData field;
    DEFAULT_SVR()->GetQuotInfo(strCode.c_str(),field);
    wxString strTemp;
    wxListBox* pListBox = (wxListBox*)FindWindowById(ID_LISTORDERREALMSG, this);
    if(pListBox==NULL)  return;
    //pListBox->Clear();
    //设置第四行
	strTemp.Printf(LOADFORMATSTRING(DEFAULTQTY_MUL,"%d%d"),nDefaultVolume, nMultiplyVolume); //文本必须包含%d,%d
	if(DEFAULT_SVR()->GetPlatformType()!=PTYPE_CTP) 
	{
		if(strTemp!=pListBox->GetString(0))
			pListBox->SetString(0, strTemp); 
	}
	else
	{
		if(strTemp!=pListBox->GetString(3))
			pListBox->SetString(3, strTemp); 
	}
    // 非综合交易平台，无可用资金、可开手数等提示
	if(DEFAULT_SVR()->GetPlatformType()!=PTYPE_CTP) 
		return;

    //设置第一行
	double fMargin = -1.0;

	if(DEFAULT_SVR()->GetTradingAccountAvailable(fMargin)==0&&(!util::isInvalidValue(fMargin))) 
		strTemp.Printf(LOADFORMATSTRING(FUNDS_AVAILABLE_2F,"%f"), fMargin);//文本必须包含%f
	else
		strTemp = LOADSTRING(FUNDS_AVAILABLE);

	if(strTemp!=pListBox->GetString(0))
		pListBox->SetString(0, strTemp); 

    //设置第二行
    string strCmbSubCode1, strCmbSubCode2;
	PlatformStru_InstrumentInfo tRetInstrumentInfo;
	ZeroMemory(&tRetInstrumentInfo, sizeof(tRetInstrumentInfo));
	if(DEFAULT_SVR()->GetInstrumentInfo(string(strCode.c_str()), tRetInstrumentInfo)!=0)
		return;
	BOOL bIsComboCode = tRetInstrumentInfo.ProductClass==THOST_FTDC_PC_Combination;
    //BOOL bIsComboCode = DEFAULT_SVR()->GetProductClassType(strCode.c_str()) == THOST_FTDC_PC_Combination;
	int nVolumeMultiple = tRetInstrumentInfo.VolumeMultiple;

    // 可开仓数
    int nOpenVolume = 0;
	double fTemp = 0.0;


	if(bIsMarketPrice) {
		switch(GetMKTOpenVolumeMode()) {
		case 1:
			fPrice = field.LastPrice;
			break;
		case 2:
			fPrice = bIsBuy ? field.AskPrice1 : field.BidPrice1;
			break;
		case 0:
		default:
			fPrice = util::GetDoubleInvalidValue();
			break;
		};
	}

    PlatformStru_InstrumentInfo InstrumentData,InstrumentData2;
    PlatformStru_InstrumentMarginRate InstrumentMarginRateData,InstrumentMarginRateData2;
    PlatformStru_InstrumentCommissionRate InstrumentCommissionRateData,InstrumentCommissionRateData2;
    strTemp = LOADSTRING(OPEN_QTY);
    //按照新的手续费计算公式计算20111222
	if(fMargin>0.001 &&
		(util::greaterOrEqual(fPrice, field.LowerLimitPrice) && util::lessOrEqual(fPrice, field.UpperLimitPrice)) ||
			(util::equals(field.LowerLimitPrice, 0.0)||util::equals(field.UpperLimitPrice, 0.0))||
            (util::isInvalidValue(field.LowerLimitPrice)||util::isInvalidValue(field.UpperLimitPrice))) 
    {
		if(bIsComboCode) {
			if(ConvertComboInstrumentID(string(strCode), strCmbSubCode1, strCmbSubCode2)
					&&GetIMC(strCmbSubCode1,InstrumentData,InstrumentMarginRateData,InstrumentCommissionRateData)
					&&GetIMC(strCmbSubCode2,InstrumentData2,InstrumentMarginRateData2,InstrumentCommissionRateData2)) {
        
				PlatformStru_DepthMarketData fieldSub1, fieldSub2;
				if(DEFAULT_SVR()->GetQuotInfo(strCmbSubCode1, fieldSub1)==0
					&& DEFAULT_SVR()->GetQuotInfo(strCmbSubCode2, fieldSub2)==0) {
					// 计算最大可开仓数
					fTemp = nVolumeMultiple*(InstrumentMarginRateData.LongMarginRatioByMoney*InstrumentData.VolumeMultiple*fieldSub1.LastPrice
							+InstrumentData.VolumeMultiple*fieldSub1.LastPrice*InstrumentCommissionRateData.OpenRatioByMoney
							+InstrumentCommissionRateData.OpenRatioByVolume 
							+InstrumentMarginRateData2.LongMarginRatioByMoney*InstrumentData2.VolumeMultiple*fieldSub2.LastPrice
							+InstrumentData2.VolumeMultiple*fieldSub2.LastPrice*InstrumentCommissionRateData2.OpenRatioByMoney
							+InstrumentCommissionRateData2.OpenRatioByVolume);
                    if(fTemp!=0.0&&(!util::isInvalidValue(fMargin)))
                    {
					    nOpenVolume=(int)(fMargin/fTemp);
					    strTemp.Printf(LOADFORMATSTRING(OPEN_QTY_F,"%d"), nOpenVolume);//文本必须包含%d
                    }
                    else
                    {
                        strTemp.Printf(LOADSTRING(OPEN_QTY));
                    }
				}
			}
			else
				nOpenVolume = -1;
        }
		else {
			if(GetIMC(strCode.c_str(),InstrumentData,InstrumentMarginRateData,InstrumentCommissionRateData)) {
				fTemp = InstrumentMarginRateData.LongMarginRatioByMoney*InstrumentData.VolumeMultiple*fPrice
						+InstrumentData.VolumeMultiple*fPrice*InstrumentCommissionRateData.OpenRatioByMoney
						+InstrumentCommissionRateData.OpenRatioByVolume;
                if(fTemp!=0.0&&(!util::isInvalidValue(fMargin)))
                {
				    nOpenVolume=(int)(fMargin/fTemp);
				    strTemp.Printf(LOADFORMATSTRING(OPEN_QTY_F,"%d"), nOpenVolume);//文本必须包含%d
                }
                else
                {
                    strTemp.Printf(LOADSTRING(OPEN_QTY));
                }
			}
			else
				nOpenVolume = -1;
        }
    }
	if(strTemp!=pListBox->GetString(1))
		pListBox->SetString(1, strTemp); 



	//设置第三行 可平数 总仓/今仓
	int nSumVolume = 0, nSumTodayVolume=0;

	if(bUpdatePreClose) {
		//if(bIsComboCode) {
		//	int nSub1SumVolume=0, nSub1SumTodayVolume=0;
		//	int nSub2SumVolume=0, nSub2SumTodayVolume=0;
		//	ConvertComboInstrumentID(string(strCode), strCmbSubCode1, strCmbSubCode2);
		//	COrderInsertEntity::GetPreVolume(strCmbSubCode1.c_str(), !bIsBuy, bIsHedge, nSub1SumVolume, nSub1SumTodayVolume, strAccount.c_str());
		//	COrderInsertEntity::GetPreVolume(strCmbSubCode2.c_str(), bIsBuy, bIsHedge, nSub2SumVolume, nSub2SumTodayVolume, strAccount.c_str());
		//	nSumVolume = min(nSub1SumVolume, nSub2SumVolume);
		//	nSumTodayVolume = min(nSub1SumTodayVolume, nSub2SumTodayVolume);
		//}
		//else {
		//	COrderInsertEntity::GetPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, strAccount.c_str());
		//}
		if(bIsComboCode) {
			GetCmbPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, strAccount.c_str());
		}
		else {
			COrderInsertEntity::GetPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, strAccount.c_str());
		}

		strTemp.Printf(LOADFORMATSTRING(CLOSE_OR_TODAY_F,"%d%d"), nSumVolume, nSumTodayVolume);//文本必须包含%d,%d
		if(strTemp!=pListBox->GetString(2))
			pListBox->SetString(2, strTemp); 
	}
}

BOOL COrderRealMsgPanel::GetCmbHoldingVolume(string strCode, BOOL bIsBuy, BOOL bIsHedge, 
										  int& nSumVolume, int& nSumTodayVolume, string strAccount)
{
	std::vector<PlatformStru_Position> outData;
	char cDirect = bIsBuy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
	char cHedge = bIsHedge ? THOST_FTDC_HF_Hedge : THOST_FTDC_HF_Speculation;
	nSumVolume=0;
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPositionCombs(outData)>0) 
    {
		for(int i=0; i<(int)outData.size(); i++) 
        {
			if(strAccount.compare(outData[i].Account)==0 && 
                strCode.compare(outData[i].InstrumentID)==0 && 
                outData[i].PosiDirection==cDirect && 
                outData[i].HedgeFlag==cHedge)
            {
                nSumVolume=outData[i].Position;
                nSumTodayVolume=outData[i].TodayPosition;
				break;
			}
		}
	}
	return nSumVolume>0;
}

BOOL COrderRealMsgPanel::GetCmbAskCloseVolume(string strCode, BOOL bIsBuy, BOOL bIsHedge, 
										   int& nAskVolume, int& nAskTodayVolume, string strAccount)
{
	char cDirection = bIsBuy ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
	char cHedge = bIsHedge ? THOST_FTDC_HF_Hedge : THOST_FTDC_HF_Speculation;
	int nCloseVolume = 0, nCloseYesterdayVolume = 0;

	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetCloseOrderVolume(strAccount,strCode, cDirection, 
		nCloseVolume, nAskTodayVolume, nCloseYesterdayVolume, cHedge) == 0) {
		if(util::isInvalidValue(nAskTodayVolume))
			nAskTodayVolume = 0;
		nAskVolume = nAskTodayVolume+util::isInvalidValue(nCloseVolume)?0:nCloseVolume;
	}
	else {
		nAskVolume = 0;
		nAskTodayVolume = 0;
	}
	return TRUE;
}

BOOL COrderRealMsgPanel::GetCmbPreVolume(string strCode, BOOL bIsBuy, BOOL bIsHedge, 
									  int& nPreVolume, int& nPreTodayVolume, string strAccount)
{
	int nSumVolume = 0, nSumTodayVolume = 0;
	int nAskVolume = 0, nAskTodayVolume = 0;

	if(!GetCmbHoldingVolume(strCode, bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, strAccount)) return FALSE;
	if(!GetCmbAskCloseVolume(strCode, !bIsBuy, bIsHedge, nAskVolume, nAskTodayVolume, strAccount)) return FALSE;
	nPreVolume = nSumVolume-nAskVolume;
	nPreTodayVolume = nSumTodayVolume-nAskTodayVolume;
	return TRUE;
}

bool COrderRealMsgPanel::GetIMC(const std::string& InstrumentID,PlatformStru_InstrumentInfo& InstrumentData,
                                  PlatformStru_InstrumentMarginRate& InstrumentMarginRateData,
                                  PlatformStru_InstrumentCommissionRate& InstrumentCommissionRateData)
{
    if(InstrumentID.empty())
        return false;
    if(DEFAULT_SVR()->GetInstrumentInfo(InstrumentID, InstrumentData)<0) 
        return false;
    if(DEFAULT_SVR()->GetMarginRate(InstrumentID, InstrumentMarginRateData)<0)
    {
        DEFAULT_SVR()->GetMarginRate_TriggerReq(InstrumentID);
        return false;
    }
    if(DEFAULT_SVR()->GetCommissionRate(InstrumentID, InstrumentCommissionRateData)<0)
    {
        DEFAULT_SVR()->GetCommissionRate_TriggerReq(InstrumentID);
        return false;
    }
    return true;
}

void COrderRealMsgPanel::OnSize(wxSizeEvent& event)
{
    wxListBox* pListBox = (wxListBox*)FindWindowById(ID_LISTORDERREALMSG, this);
    if(pListBox) {
        pListBox->SetSize(event.GetSize());
    }
}
