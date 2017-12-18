#include "StdAfx.h"
#include "COrderInsertOperations.h"
#include <math.h>
#include "../inc/Module-Misc2/tools_util.h"
#include "../inc/Module-Misc/zqControl.h"
#include "cfgMgr/CfgMgr.h"
#include "COrderBatchDailog.h"
#include "CEditOrderChangeDailog.h"
#include "CEditOrderReplaceDailog.h"
#include "COrderConfirmDlg.h"
#include "COrderParkedSelectDlg.h"
#include "../Module-Misc/LogDefine.h"
#include "Module-Misc2/globalDefines.h"
#include "Module-Misc2/GlobalConfigMgr.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


COrderInsertOperations::COrderInsertOperations(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos ,
            const wxSize& size ,
            long style,
            const wxString& name ):
	COrderInsertView(parent,winid,pos,size,style,name),
	m_currPriceTick(0.0),
	m_currUpperLimitPrice(0.0),
	m_currLowerLimitPrice(0.0),
	m_NeedUpdatePrice(false),
	m_bNoKeyChangeText(TRUE),
	m_bIsGridChangeInstrumentID(FALSE),
	m_bShowTradeInfo(true),
	m_poLocalOrderService(NULL),
	m_nMultiplyVolume(1)
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr != NULL) {
		LPORDER_CFG p = pMgr->GetOrderCfg();
		// 设置界面上的自动开平按钮
		//待处理，默认自动开平
		SetInputAutoOpenClose(p->bAutoOpenClose);

		// 这里从xml中读下单倍率数据
		m_KeyMultiplyVolumeMap.clear();
		for(int i=0; i<p->OrderMultiDataNum; i++) {
			std::string strKey(p->OrderMultiData[i].ShortKey);
			m_KeyMultiplyVolumeMap[strKey] = p->OrderMultiData[i].Volume;
		}
		
		//int accindex=0;
		//wxAcceleratorEntry entries[2];
		//entries[accindex++].Set(wxACCEL_ALT, (int)'a', ID_BUTTON_OK);
		//entries[accindex++].Set(wxACCEL_ALT, (int)'s', ID_BUTTON_PARKEDORDER);
		//wxAcceleratorTable accel(2, entries);
		//this->SetAcceleratorTable(accel);
		
	}

	m_poLocalOrderService = CLocalOrderService::GetInstance();
}

COrderInsertOperations::~COrderInsertOperations()
{
}

void COrderInsertOperations::FillInputOrderField(PlatformStru_InputOrder& req)
{
	BOOL bIsMarketPrice = FALSE;
	double fPrice = 0.0, fTriggerPrice = 0.0;
	string strPriceText = GetInputPriceText().c_str();
	string strLimitPriceText = GetInputLimitPriceText().c_str();
	string strName = GetInputInstrumentID().c_str();
	BOOL bIsBuy = GetInputDirection();
	int nOrderTypeFT = OrderType_FT_Immediately;
	switch(GetInputOrderType()) {
	case OrderType_PATS_LIMIT:						// 限价
		bIsMarketPrice = FALSE;
		nOrderTypeFT = OrderType_FT_Immediately;
		fPrice = atof(strPriceText.c_str());
		break;
	case OrderType_PATS_MARKET:						// 市价
		bIsMarketPrice = TRUE;
		nOrderTypeFT = OrderType_FT_Immediately;
		break;
	case OrderType_PATS_STOP:						// 停止单
		bIsMarketPrice = TRUE;
		nOrderTypeFT = OrderType_FT_Touch;
		fTriggerPrice = atof(strPriceText.c_str());
		break;
	case OrderType_PATS_STOPLIMIT:					// 停止限价单
		bIsMarketPrice = FALSE;
		nOrderTypeFT = OrderType_FT_Touch;
		fTriggerPrice = atof(strPriceText.c_str());
		fPrice = atof(strLimitPriceText.c_str());
		break;
	default:
		//Macro
		AUTOMARKETPRICE(strName, bIsBuy, strPriceText, bIsMarketPrice, fPrice)
		break;
	};
	
	MakeReq_Order(req, strName, GetInputAccount().c_str(), nOrderTypeFT, 
			bIsBuy, GetInputOCMode(), GetInputQty(), fPrice, fTriggerPrice, 
			GetInputHedge(), bIsMarketPrice, FALSE);
}

// 应该做的是价格有效性分析
double COrderInsertOperations::AnalysePrice(double fPrice, wxString& strInstrumentID)
{
	return fPrice;
}

BOOL COrderInsertOperations::NeedConfirm()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bOrderConfirm;
}

// 下单成功 是否 弹框
BOOL COrderInsertOperations::NeedOrderSuccessConfirm()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bOrderSuccessDlg;
}

// 下单失败 是否 弹框
BOOL COrderInsertOperations::NeedOrderFailConfirm()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bOrderFailDlg;
}

// 挂单成交 是否 弹框
BOOL COrderInsertOperations::NeedOrderTradeConfirm()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bTradeDlg;
}

// 下单成功 是否 发声
BOOL COrderInsertOperations::NeedOrderSuccessSound()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bOrderSuccessSound;
}

// 下单失败 是否 发声
BOOL COrderInsertOperations::NeedOrderFailSound()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bOrderFailSound;
}

// 挂单成交 是否 发声
BOOL COrderInsertOperations::NeedOrderTradeSound()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bTradeSound;
}

// 判读是否需要自动开平
BOOL COrderInsertOperations::NeedAutoOpenClose(int& nAutoMode)
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	nAutoMode = p->ProcessPositionType;
	return p->bAutoOpenClose;
}

// 判读是否需要自动开平
BOOL COrderInsertOperations::NeedAlwaysOpen()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bAlwaysOpen;
}

// 在跟盘状态 下单 后继续跟盘
BOOL COrderInsertOperations::NeedContinueTrackPrice()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bReTrack;
}

// 是否用涨跌停价模拟市价，当交易所不支持市价时
BOOL COrderInsertOperations::NeedSimulateMarketPrice()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bSimulate;
}

// 是否需要显示下单面板的5档价格列表
BOOL COrderInsertOperations::NeedFivePriceList()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bBuy5Sell5;
}

// 是否需要显示下单面板的5档价格列表
BOOL COrderInsertOperations::NeedReturnQuotList()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bReturnQuotTable;
}

// 设置是否需要显示下单面板的5档价格列表
void COrderInsertOperations::SaveFivePriceList(BOOL bHas)
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;
	LPORDER_CFG p = pMgr->GetOrderCfg();
    p->bBuy5Sell5 = bHas?true:false;
}

// 在跟盘状态 下单 后继续跟盘
int COrderInsertOperations::GetActionAfterOrder()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->ActionAfterOrder;
}

// 风险控制：手数限制
int COrderInsertOperations::GetRaskVolumeLimit()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->VolumeLimit;
}

// 风险控制：偏离价位
int COrderInsertOperations::GetRaskDifferPrice()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->DifferPrice;
}

// 下单后，输入焦点定位
int COrderInsertOperations::GetAfterFocus()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return 0;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->nCursorPosAfterOrder;
}

int COrderInsertOperations::GetDefaultVolume(wxString& wxstrCode)
{
	int nDefaultVolume, nMultiplyVolume;
	return GetDefaultVolume(wxstrCode, nDefaultVolume, nMultiplyVolume);
}

int COrderInsertOperations::GetDefaultVolume(wxString& wxstrCode, int& nDefaultVolume, int& nMultiplyVolume)
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	//LPORDER_CFG p = pMgr->GetOrderCfg();
	std::vector<VarietyInfo>* pVInfo = pMgr->GetVarietyInfoVector();
	int nVolume = 1;
	VarietyInfo tvInfo;
	std::string strCode;
	std::string strTemp;
	BOOL bFound = FALSE;
	char strCodeHead[256];
	ZeroMemory(strCodeHead, 256);

	// 优先精确查找
	strcpy(strCodeHead, wxstrCode.c_str());
	strCode = strlwr(strCodeHead);
	for(int i=0; i<(int)pVInfo->size(); i++) {
		tvInfo = (*pVInfo)[i];
		wxString strValue = tvInfo.InstruName;
		strTemp = strlwr((char*)strValue.c_str());
		if(strTemp.compare(strCode) == 0) {
			nVolume = tvInfo.volume;
			bFound = TRUE;
			break;
		}
	}
	if(bFound) {
		nVolume = nVolume>0 ? nVolume :	1;
		nDefaultVolume = nVolume;
		nVolume = nVolume * (m_nMultiplyVolume>0 ? m_nMultiplyVolume : 1);
		nMultiplyVolume = m_nMultiplyVolume;
		return nVolume;
	}

	// 如精确查找不到，再使用模糊查找
	ZeroMemory(strCodeHead, 256);
	strncpy(strCodeHead, wxstrCode.c_str(), 2);
	if(strCodeHead[1] >= '0' && strCodeHead[1] <= '9')
		strCodeHead[1] = 0;
	strCode = strlwr(strCodeHead);
	for(int i=0; i<(int)pVInfo->size(); i++) {
		tvInfo = (*pVInfo)[i];
		wxString strValue = tvInfo.InstruName;
		strTemp = strlwr((char*)(strValue.c_str()));
		if(strTemp.compare(strCode) == 0) {
			nVolume = tvInfo.volume;
			break;
		}
	}
	nVolume = nVolume>0 ? nVolume :	1;
	nDefaultVolume = nVolume;
	nVolume = nVolume * (m_nMultiplyVolume>0 ? m_nMultiplyVolume : 1);
	nMultiplyVolume = m_nMultiplyVolume;
	return nVolume;
}

//根据报单回报显示 交易信息
void COrderInsertOperations::ShowTradeInfoDlg(const string& Title, const CThostFtdcOrderField& OrderInfo, BOOL bIsShow)
{
	if(!m_bShowTradeInfo)
		return;
    if(TRADEINFODLG(this)==NULL) return;
	//m_pTradeInfoDlg->Hide();

	wxString info;
    std::string stdstrprice=GlobalFunc::ConvertToString(OrderInfo.LimitPrice,4);
	info.reserve(512);
	wxString strStatusMsg=CFieldValueTool::ConvertStatusMsg(OrderInfo.StatusMsg);
	info.Printf(LOADFORMATSTRING(CDOCQPM_FORMAT,"%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s
					OrderInfo.InstrumentID,
					CFieldValueTool::Direction2String(OrderInfo.Direction,SVR_LANGUAGE).c_str(),
					CFieldValueTool::OffsetFlag2String(OrderInfo.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
					Number2String(OrderInfo.VolumeTotalOriginal).c_str(),
					stdstrprice.c_str(),
					strStatusMsg.c_str());
	TRADEINFODLG(this)->ShowTradeMsg(info, -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = "CDOCQPM_FORMAT";
	pData->vParam.push_back(LOG_PARAM(OrderInfo.InstrumentID));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.Direction, DIRECTION_TYPE));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.CombOffsetFlag[0], OFFSETFLAG_TYPE));
	pData->vParam.push_back(LOG_PARAM(Number2String(OrderInfo.VolumeTotalOriginal)));
	pData->vParam.push_back(LOG_PARAM(stdstrprice));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.StatusMsg, ORDER_STATUS_MSG_TYPE));
	TRADEINFODLG(this)->WriteLog( pData );
}

//根据ErrRtnOrderAction显示 撤单失败信息
void COrderInsertOperations::ShowTradeInfoDlg(const string& Title, const CThostFtdcOrderActionField& OrderActionInfo, BOOL bIsShow)
{
	if(!m_bShowTradeInfo)
		return;
    if(TRADEINFODLG(this)==NULL) return;
	//m_pTradeInfoDlg->Hide();

	wxString info;
	info.reserve(512);
	wxString strStatusMsg=CFieldValueTool::ConvertStatusMsg(OrderActionInfo.StatusMsg);
	info.Printf(LOADFORMATSTRING(CONMENO_FORMAT,"%s%s"),//文本必须包含%s,%s
					OrderActionInfo.InstrumentID,
					strStatusMsg.c_str());
	TRADEINFODLG(this)->ShowTradeMsg(info, -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = "CONMENO_FORMAT";
	pData->vParam.push_back(LOG_PARAM(OrderActionInfo.InstrumentID));
	pData->vParam.push_back(LOG_PARAM(OrderActionInfo.StatusMsg, ORDER_STATUS_MSG_TYPE));
	TRADEINFODLG(this)->WriteLog( pData );
}

//根据OnRspOrderAction显示 撤单失败信息
void COrderInsertOperations::ShowTradeInfoDlg(const string& Title, const DataRspOrderAction& InputOrderActionInfo, BOOL bIsShow)
{
	if(!m_bShowTradeInfo)
		return;
    if(TRADEINFODLG(this)==NULL) return;
	//m_pTradeInfoDlg->Hide();

	wxString info;
	info.reserve(512);
	info.Printf(LOADFORMATSTRING(CONMENO_FORMAT,"%s%s"),//文本必须包含%s,%s
					InputOrderActionInfo.InputOrderActionField.InstrumentID,
					InputOrderActionInfo.RspInfoField.ErrorMsg);
	TRADEINFODLG(this)->ShowTradeMsg(info, -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = "CONMENO_FORMAT";
	pData->vParam.push_back(LOG_PARAM(InputOrderActionInfo.InputOrderActionField.InstrumentID));
	pData->vParam.push_back(LOG_PARAM(InputOrderActionInfo.RspInfoField.ErrorMsg));
	TRADEINFODLG(this)->WriteLog( pData );
}

//根据报单回执显示 交易信息
void COrderInsertOperations::ShowTradeInfoDlg(const string& Title, const DataRspOrderInsert& OrderInfo, BOOL bIsShow)
{
	if(!m_bShowTradeInfo)
		return;

	//m_pTradeInfoDlg->Hide();
    if(TRADEINFODLG(this)==NULL) return;
	wxString info;
    std::string stdstrprice=GlobalFunc::ConvertToString(OrderInfo.InputOrderField.LimitPrice,4);
	info.reserve(512);
	info.Printf(LOADFORMATSTRING(CDOCQPM_FORMAT,"%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s
					OrderInfo.InputOrderField.InstrumentID,
					CFieldValueTool::Direction2String(OrderInfo.InputOrderField.Direction,SVR_LANGUAGE).c_str(),
					CFieldValueTool::OffsetFlag2String(OrderInfo.InputOrderField.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
					Number2String(OrderInfo.InputOrderField.VolumeTotalOriginal).c_str(),
					stdstrprice.c_str(),
					OrderInfo.RspInfoField.ErrorMsg);
	TRADEINFODLG(this)->ShowTradeMsg(info, -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = "CDOCQPM_FORMAT";
	pData->vParam.push_back(LOG_PARAM(OrderInfo.InputOrderField.InstrumentID));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.InputOrderField.Direction, DIRECTION_TYPE));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.InputOrderField.CombOffsetFlag[0], OFFSETFLAG_TYPE));
	pData->vParam.push_back(LOG_PARAM(Number2String(OrderInfo.InputOrderField.VolumeTotalOriginal)));
	pData->vParam.push_back(LOG_PARAM(stdstrprice));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.RspInfoField.ErrorMsg));
	TRADEINFODLG(this)->WriteLog( pData );
}

//根据报单回执显示 交易信息
void COrderInsertOperations::ShowTradeInfoDlg(const string& Title, const COrderChangeEntity::CMBORDERINSERT& OrderInfo, BOOL bIsShow)
{
	if(!m_bShowTradeInfo)
		return;
    if(TRADEINFODLG(this)==NULL) return;

	wxString info;
    std::string stdstrprice=GlobalFunc::ConvertToString(OrderInfo.insert.LimitPrice,4);
	info.reserve(512);
	info.Printf(LOADFORMATSTRING(CDOCQPM_FORMAT,"%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s
					OrderInfo.insert.InstrumentID,
					CFieldValueTool::Direction2String(OrderInfo.insert.Direction,SVR_LANGUAGE).c_str(),
					CFieldValueTool::OffsetFlag2String(OrderInfo.insert.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
					Number2String(OrderInfo.insert.VolumeTotalOriginal).c_str(),
					stdstrprice.c_str(),
					OrderInfo.rspInfo.ErrorMsg);
	TRADEINFODLG(this)->ShowTradeMsg(info, -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = "CDOCQPM_FORMAT";
	pData->vParam.push_back(LOG_PARAM(OrderInfo.insert.InstrumentID));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.insert.Direction, DIRECTION_TYPE));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.insert.CombOffsetFlag[0], OFFSETFLAG_TYPE));
	pData->vParam.push_back(LOG_PARAM(Number2String(OrderInfo.insert.VolumeTotalOriginal)));
	pData->vParam.push_back(LOG_PARAM(stdstrprice));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.rspInfo.ErrorMsg));
	TRADEINFODLG(this)->WriteLog( pData );
}

//根据报单回执显示 交易信息
void COrderInsertOperations::ShowTradeInfoDlg(const string& Title, const COrderInsertEntity::CMBORDER& OrderInfo, BOOL bIsShow)
{
	if(!m_bShowTradeInfo)
		return;
    if(TRADEINFODLG(this)==NULL) return;
	//m_pTradeInfoDlg->Hide();

	wxString info;
	std::string stdstrprice=GlobalFunc::ConvertToString(OrderInfo.order.insert.LimitPrice,4);
	info.reserve(512);
	info.Printf(LOADFORMATSTRING(CDOCQPM_FORMAT,"%s%s%s%s%s%s"),//文本必须包含%s,%s,%s,%s,%s,%s
					OrderInfo.order.insert.InstrumentID,
					CFieldValueTool::Direction2String(OrderInfo.order.insert.Direction,SVR_LANGUAGE).c_str(),
					CFieldValueTool::OffsetFlag2String(OrderInfo.order.insert.CombOffsetFlag[0],SVR_LANGUAGE).c_str(),
					Number2String(OrderInfo.order.insert.VolumeTotalOriginal).c_str(),
					stdstrprice.c_str(),
					OrderInfo.rspInfo.ErrorMsg);
	TRADEINFODLG(this)->ShowTradeMsg(info, -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = "CDOCQPM_FORMAT";
	pData->vParam.push_back(LOG_PARAM(OrderInfo.order.insert.InstrumentID));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.order.insert.Direction, DIRECTION_TYPE));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.order.insert.CombOffsetFlag[0], OFFSETFLAG_TYPE));
	pData->vParam.push_back(LOG_PARAM(Number2String(OrderInfo.order.insert.VolumeTotalOriginal)));
	pData->vParam.push_back(LOG_PARAM(stdstrprice));
	pData->vParam.push_back(LOG_PARAM(OrderInfo.rspInfo.ErrorMsg));
	TRADEINFODLG(this)->WriteLog( pData );
}

//显示错误信息
void COrderInsertOperations::ShowTradeInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow)
{
	if(!m_bShowTradeInfo)
		return;
    if(TRADEINFODLG(this)==NULL) return;
	//m_pTradeInfoDlg->Hide();
	TRADEINFODLG(this)->ShowTradeMsg(LOADSTRING_TEXT(errormessage), -1, LOADSTRING_TEXT(Title), -1, bIsShow);

	LOG_DATA* pData = new LOG_DATA;
	pData->strTitle = Title;
	pData->strFormat = errormessage;
	TRADEINFODLG(this)->WriteLog( pData );
}

// 带风险管理、带Action处理、带是否弹出对话框处理
// 按OK键下单时，参数nAction=0
BOOL COrderInsertOperations::DoOrderInsert(COrderInsertEntity::ORIORDERREQ& tOriReq, 
							int nAction, BOOL& bHasOrderInsert, 
							CConfirmItemChange& confirmChg, BOOL bIsAsyncView)
{
	//LOG_INFO("COrderInsertOperations::DoOrderInsert\n"
 //            "\t\t\t InstrumentID=[%s],\t strAccount=[%s],\n"
 //            "\t\t\t bIsParketOrder=[%s],\t bDirection=[%s],\t nOCMode=[%d],\t fPrice=[%f],\t nVolume=[%d],\n"
 //            "\t\t\t Hedge=[%s],\t MarketPrice=[%s],\t bSimulate=[%s],\t bIsAuto=[%s],\t nAutoMode=[%d],\n"
 //            "\t\t\t bIsPreClose=[%s],\t nDefaultVolume=[%d]\n",
	//		 tOriReq.strInstrumentID,
	//		 tOriReq.strAccount,
	//		 tOriReq.bIsParketOrder?"TRUE":"FALSE",
	//		 tOriReq.bDirection?"TRUE":"FALSE",
	//		 tOriReq.nOCMode,
	//		 tOriReq.fPrice,
	//		 tOriReq.nVolume,
	//		 tOriReq.bIsHedge?"HEDG":"SPEC",
	//		 tOriReq.bIsMarketPrice?"MARKET":"LIMIT",
	//		 tOriReq.bSimulate?"TRUE":"FALSE",				// 是否用市价模拟
	//		 tOriReq.bIsAuto?"TRUE":"FALSE",
	//		 tOriReq.nAutoMode,
	//		 tOriReq.bIsPreClose?"TRUE":"FALSE",			// 是否按可平仓数处理
	//		 tOriReq.nDefaultVolume);
	
	if(nAction==0 || nAction==1 || nAction==2) {

		COrderInsertEntity::ORDERREQBREAK tOrderReqBreak={0};
		COrderInsertEntity::AnalyseOrderReq(tOriReq, tOrderReqBreak);
		
		int nRealVolume = tOrderReqBreak.dwVolume_Open + tOrderReqBreak.dwVolume_Close 
				+ tOrderReqBreak.dwVolume_CloseToday + tOrderReqBreak.dwVolume_ReverseOpen;

		//// 处理风险管理，手数
		//if(nRealVolume > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
		//	wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY),LOADSTRING(OIP_FASTTRADER), 
		//			wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		//	if(dialog.ShowModal()!=wxID_YES)
		//		return FALSE;
		//}

		//PlatformStru_DepthMarketData field;
		//ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

		//// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
		//memset(&field,0,sizeof(field));
		//if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(tOriReq.strInstrumentID,field);

		//// 处理风险管理，价格
		//if(!tOriReq.bIsMarketPrice) {
		//	BOOL bIsPriceLimit = tOriReq.fPrice>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
		//			|| tOriReq.fPrice<field.LastPrice - m_currPriceTick*GetRaskDifferPrice();
		//	bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
		//	if(bIsPriceLimit) {
		//		wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), 
		//				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
		//		if(dialog.ShowModal()!=wxID_YES) 
		//			return FALSE;
		//	}
		//}

		wxString strRetRevoke;
		wxString strMsg;
		wxString strCode = tOriReq.strInstrumentID;
		BOOL bIsAuto = tOriReq.bIsAuto;

		COrderInsertEntity* poEntity = COrderInsertEntity::CreateOrderInsert(tOriReq);
		// 先生成预撤单表表，然后调用DoRevokeOrder执行实际撤单
		if(bIsAuto) {
			poEntity->RevokeOrder(strCode, tOriReq.bDirection, tOriReq.nOCMode, strRetRevoke);
			strMsg = strRetRevoke;
		}
		strMsg += poEntity->GetOrderDesciption();

		if(bIsAsyncView) {
			// 处理界面同步显示
			SetInputDirection(tOriReq.bDirection);
			if(tOrderReqBreak.bIsOpen) 
			{
				SetInputQty(tOrderReqBreak.dwVolume_Open);
				SetInputOCMode(0);
			}
			else if(tOrderReqBreak.bIsCloseToday)
			{
				SetInputQty(tOrderReqBreak.dwVolume_CloseToday);
				SetInputOCMode(1);
			}
			else if(tOrderReqBreak.bIsClose) 
			{
				SetInputQty(tOrderReqBreak.dwVolume_Close);
				SetInputOCMode(2);
			}
			else if(tOrderReqBreak.bIsReverseOpen) 
			{
				SetInputQty(tOrderReqBreak.dwVolume_ReverseOpen);
				SetInputOCMode(0);
			}
		}

		BOOL bOK = TRUE;
		int nErrPos = -1;
		wxString strErrMsg;
		//if(PLATFORM_SVR().GetPlatformType()==PTYPE_CTP) {
			if((NeedConfirm()&&nAction==0) || nAction==1) {
				COrderConfirmDlg dlg(NULL);
				dlg.SetPriceTick(m_currPriceTick);
				dlg.SetUpperLimitPrice(m_currUpperLimitPrice);
				dlg.SetLowerLimitPrice(m_currLowerLimitPrice);
				dlg.SetIsAuto(bIsAuto);
				dlg.SetIsMarketPrice(tOriReq.bIsMarketPrice);
				dlg.SetConfirmItemChange(confirmChg);
				if(tOriReq.bIsParketOrder) 
					dlg.SetTitle(LOADSTRING(OIP_PARKCONF));
				else
					dlg.SetTitle(LOADSTRING(OIP_ORDERCONF));
				dlg.SetOrderInfo(strRetRevoke);
				for(int i=0; i<poEntity->GetOrderReqCount(); i++) {
					dlg.AddCmbOrder(&(poEntity->GetOrderReq(i)));
				}
				bOK = (dlg.ShowModal()==wxID_YES);
				USERLOG_INFO("提示选择，标题[%s]，\n用户选择[%s]\n%s\n",dlg.GetTitle().c_str(),bOK?"是":"否", dlg.GetUserInputText().c_str());
	//			wxMessageDialog dialog( NULL,strMsg , _("委托确认"), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
	//			bOK = dialog.ShowModal()==wxID_YES;
			}
		//}
		//else {
		//	strMsg = Printf_StdOrderReq(tOriReq, LOADSTRING(Printf_AccountTitle), LOADSTRING(Printf_OrderTypeTitle), 
		//			LOADSTRING(Printf_InstrumentIDTitle), LOADSTRING(Printf_DirectionTitle), 
		//			LOADSTRING(Printf_OpenCloseTitle), LOADSTRING(Printf_QtyTitle), 
		//			LOADSTRING(Printf_PriceTitle), LOADSTRING(Printf_TriggerPriceTitle), 
		//			LOADSTRING(Printf_HedgeTitle), m_currPriceTick, PLATFORM_SVR().HaveOrderType(UIOT_ACCOUNT),
		//			PLATFORM_SVR().HaveOrderType(UIOT_OPENCLOSE), PLATFORM_SVR().HaveOrderType(UIOT_HEDGE));

		//	BOOL bOK = TRUE;
		//	if(NeedConfirm()) {
		//		wxMessageDialog dialog( NULL,strMsg ,LOADSTRING(OIP_ORDERCONF), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
		//		bOK = dialog.ShowModal()==wxID_YES;
		//	}
		//}
		if(bOK) {
			//int nRealVolume = tOrderReqBreak.dwVolume_Open + tOrderReqBreak.dwVolume_Close 
			//		+ tOrderReqBreak.dwVolume_CloseToday + tOrderReqBreak.dwVolume_ReverseOpen;
			nRealVolume = 0;
			for(int i=0; i<poEntity->GetOrderReqCount(); i++) {
				COrderInsertEntity::CMBORDER& tOrder = poEntity->GetOrderReq(i);
				if(tOrder.bIsParked) {	// 是否为预埋单或正常报单
					nRealVolume += tOrder.order.parked.VolumeTotalOriginal;	// 预埋单域
				}
				else {
					nRealVolume += tOrder.order.insert.VolumeTotalOriginal;	// 正常单域
				}
			}

			// 处理风险管理，手数
			if(nRealVolume > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) {
				wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITQTY),LOADSTRING(OIP_FASTTRADER), 
						wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
				if(dialog.ShowModal()!=wxID_YES)
					return FALSE;
			}

			bool bHasQuot = false;
			PlatformStru_DepthMarketData field;

			//// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
			if(DEFAULT_SVR()) bHasQuot=DEFAULT_SVR()->GetQuotInfo(tOriReq.strInstrumentID,field)==0;

			// 处理风险管理，价格
			BOOL bPriceOver = FALSE;
			for(int i=0; i<poEntity->GetOrderReqCount(); i++) {
				COrderInsertEntity::CMBORDER& tOrder = poEntity->GetOrderReq(i);
				if(tOrder.bIsParked) {	// 是否为预埋单或正常报单
					bPriceOver = bPriceOver || 
							tOrder.order.parked.OrderPriceType==THOST_FTDC_OPT_LimitPrice && GetRaskDifferPrice() && 
							(tOrder.order.parked.LimitPrice>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
							 || tOrder.order.parked.LimitPrice<field.LastPrice - m_currPriceTick*GetRaskDifferPrice());
				}
				else {
					bPriceOver = bPriceOver || 
							tOrder.order.insert.OrderPriceType==THOST_FTDC_OPT_LimitPrice && GetRaskDifferPrice() && 
							(tOrder.order.insert.LimitPrice>field.LastPrice + m_currPriceTick*GetRaskDifferPrice() 
							 || tOrder.order.insert.LimitPrice<field.LastPrice - m_currPriceTick*GetRaskDifferPrice());
				}
			}

			if(bPriceOver) {
				wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), 
						wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
				if(dialog.ShowModal()!=wxID_YES) 
					return FALSE;
			}

			// 超过涨跌停价提示
			// 当无行情时不判断是否此项。见FASTTRADE-109
			if(bHasQuot 
					&& !util::isInvalidValue(field.UpperLimitPrice) && !util::isInvalidValue(field.LowerLimitPrice)
					&& !tOriReq.bIsMarketPrice) {
				bool bOverLimit = util::greater(tOriReq.fPrice, field.UpperLimitPrice) 
					|| util::less(tOriReq.fPrice, field.LowerLimitPrice);
				if(bOverLimit) {
					wxMessageDialog dialog(NULL, LOADSTRING(OIP_OVERLIMITPRICE_UL), LOADSTRING(OIP_FASTTRADER), 
							wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
					if(dialog.ShowModal()!=wxID_YES) 
						return FALSE;
				}
			}

			if(bIsAuto && poEntity->GetFtdcOrderActionCount()>0) {
				// 如果有撤单，着把实体放到等待队列里，在确定所有撤单成功后，在执行下单
				if(poEntity->DoRevokeOrder(nErrPos, strErrMsg))
					m_mapOrderInsertWaitForCancel[poEntity] = 3;
			}
			else {
				poEntity->DoOrder(nErrPos, strErrMsg);
			}
			if(GetInputAutoTrackPrice()) {
				SetInputAutoTrackPrice(NeedContinueTrackPrice());
			}

			bHasOrderInsert = TRUE;
		}
		wxString strCaption;
		if(nErrPos==-1) {
			m_bWaitingRsp_OrderInsert=true;

			//wxWindow* window = NULL;
			//switch(GetAfterFocus()) {
			//case 0:		// 不懂填单后光标不动
			//	break;
			//case 1:		// 填单后光标到合约代码
			//	SetFocus();
			//	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
			//		window=FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
			//	}
			//	else {
			//		window=FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT, this);
			//	}
			//	window->SetFocus();
			//	break;
			//case 2:		// 填单后光标到买卖
			//	SetFocus();
			//	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
			//		window=FindWindowById(ID_OISTD_BUYSELL_COMBOBOX, this);
			//	}
			//	else {
			//		window=FindWindowById(ID_OIPATS_BUYSELL_COMBOBOX, this);
			//	}
			//	window->SetFocus();
			//	break;
			//case 3:		// 填单后光标开平			
			//	SetFocus();
			//	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
			//		window=FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX, this);
			//	}
			//	else {
			//		window=FindWindowById(ID_OIPATS_OPENCLOSE_COMBOBOX, this);
			//	}
			//	window->SetFocus();
			//	break;
			//case 5:		// 填单后光标到数量
			//	SetFocus();
			//	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
			//		window=FindWindowById(ID_OISTD_VOLUME_TEXT, this);
			//	}
			//	else {
			//		window=FindWindowById(ID_OIPATS_VOLUME_TEXT, this);
			//	}
			//	window->SetFocus();
			//	break;
			//case 4:		// 填单后光标到价格
			//	SetFocus();
			//	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
			//		window=FindWindowById(ID_OISTD_PRICE_TEXT, this);
			//	}
			//	else {
			//		window=FindWindowById(ID_OIPATS_PRICE_TEXT, this);
			//	}
			//	window->SetFocus();
			//	break;
			//};

		}
		else {
			//设置错误信息
			//strcpy(tmpData.RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(ret).c_str());
			//ShowTradeInfoDlg(wxString("下单失败"), tmpData);
			//if(NeedOrderFailConfirm())
			ShowTradeInfoDlg("OIP_SOF", poEntity->GetOrderReq(nErrPos), NeedOrderFailConfirm()); // title: 中国国际期货网上交易终端
		}
		
		if(!(bIsAuto && poEntity->GetFtdcOrderActionCount()>0)) {
			try {
			delete poEntity;
			poEntity = NULL;
			}
			catch(...) {}
		}
		
		if(bOK) {
			if(bIsAsyncView) {
				//1 下单后不清空下单板, 2 全部清空, 3 清空价格和手数, 4 只清空手数
				m_poOrderInputPanel->ClearViewData(GetActionAfterOrder());
			}
		}

	}
	else {

		COrderInsertEntity::ORDERREQBREAK tOrderReqBreak={0};
		COrderInsertEntity::AnalyseOrderReq(tOriReq, tOrderReqBreak);
		
		int nRealVolume = tOrderReqBreak.dwVolume_Open + tOrderReqBreak.dwVolume_Close 
				+ tOrderReqBreak.dwVolume_CloseToday + tOrderReqBreak.dwVolume_ReverseOpen;

		if(bIsAsyncView) {
			// 处理界面同步显示
			SetInputDirection(tOriReq.bDirection);
			if(tOrderReqBreak.bIsOpen) 
			{
				SetInputQty(tOrderReqBreak.dwVolume_Open);
				SetInputOCMode(0);
			}
			else if(tOrderReqBreak.bIsCloseToday)
			{
				SetInputQty(tOrderReqBreak.dwVolume_CloseToday);
				SetInputOCMode(1);
			}
			else if(tOrderReqBreak.bIsClose) 
			{
				SetInputQty(tOrderReqBreak.dwVolume_Close);
				SetInputOCMode(2);
			}
			else if(tOrderReqBreak.bIsReverseOpen) 
			{
				SetInputQty(tOrderReqBreak.dwVolume_ReverseOpen);
				SetInputOCMode(0);
			}
		}

		wxWindow* window = NULL;
		switch(nAction) {
		case 3:		// 填单后光标不动
			break;
		case 4:		// 填单后光标到合约代码
			SetFocus();
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
				window=FindWindowById(ID_OISTD_INSTRUMENTID_TEXT, this);
			}
			else {
				window=FindWindowById(ID_OIPATS_INSTRUMENTID_TEXT, this);
			}
			window->SetFocus();
			break;
		case 5:		// 填单后光标到买卖
			SetFocus();
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
				window=FindWindowById(ID_OISTD_BUYSELL_COMBOBOX, this);
			}
			else {
				window=FindWindowById(ID_OIPATS_BUYSELL_COMBOBOX, this);
			}
			window->SetFocus();
			break;
		case 6:		// 填单后光标开平			
			SetFocus();
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
				window=FindWindowById(ID_OISTD_OPENCLOSE_COMBOBOX, this);
			}
			else {
				window=FindWindowById(ID_OIPATS_OPENCLOSE_COMBOBOX, this);
			}
			window->SetFocus();
			break;
		case 7:		// 填单后光标到数量
			SetFocus();
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
				window=FindWindowById(ID_OISTD_VOLUME_TEXT, this);
			}
			else {
				window=FindWindowById(ID_OIPATS_VOLUME_TEXT, this);
			}
			window->SetFocus();
			break;
		case 8:		// 填单后光标到价格
			SetFocus();
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
				window=FindWindowById(ID_OISTD_PRICE_TEXT, this);
			}
			else {
				window=FindWindowById(ID_OIPATS_PRICE_TEXT, this);
			}
			window->SetFocus();
			break;
		case 9:		// 填单后光标到下单按钮
			SetFocus();
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPlatformType()==PTYPE_CTP) {
				window=FindWindowById(ID_BUTTON_OK, this);
			}
			else {
				window=FindWindowById(ID_BUTTON_PATSORDER, this);
			}
			window->SetFocus();
			break;
		};
	}
	return TRUE;
}

// 带风险管理、带是否弹出对话框处理
BOOL COrderInsertOperations::DoOrderChange(PlatformStru_OrderInfo& ftdcField, BOOL bIsAsyncView)
{
    if(!g_pPlatformMgr) return FALSE;

	// 当鼠标快速平仓时，自动功能优先于盈损单状态
	if(m_nViewSubStyle==conSubStyle_Position) 
    {
		SetStopPositionSelect(FALSE);
		m_poOrderPositionPanel->InitPositionPanel();
		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
		evt.SetInt(0);
		ProcessEvent(evt);
	}

    IPlatformSingleSvr* pSvr = g_pPlatformMgr->GetPlatformSvr(string(ftdcField.Account));
    IPlatformSingleSvr* pQuotSvr = IS_MULTIACCOUNT_VERSION ? CURRENT_SVR() : pSvr;
	if(pSvr==NULL || pQuotSvr==NULL) 
		return FALSE;

	int nSourceVolume = ftdcField.VolumeTotal;
	double fSourcePrice = ftdcField.LimitPrice;
	BOOL bDirection = (ftdcField.Direction==THOST_FTDC_D_Buy ? 1 : 0);
	wxString strName = ftdcField.InstrumentID;

    PlatformStru_DepthMarketData field;
    memset(&field,0,sizeof(field));

    pQuotSvr->GetQuotInfo(strName.c_str(),field);
    PlatformStru_InstrumentInfo insInfo;
    if(pSvr->GetInstrumentInfo(strName.c_str(),insInfo)==0) 
		m_currPriceTick=insInfo.PriceTick;
	if(m_poStaticFivePriceList->ResetDepthMarketData(field)) 
		m_poStaticFivePriceList->Refresh();

	int nNewVolume = 0;
	double fNewPrice = 0.0;
	BOOL bNewIsMarketPrice = FALSE;

	// 这里一个对话框，用于显示带输入框的窗体，方便用户修改价格和数量
	// ...
	CEditOrderChangeDailog* pDlg = new CEditOrderChangeDailog(this, wxID_ANY, LOADSTRING(OIP_FRORDER), wxDefaultPosition, wxSize(310, 330));
	pDlg->SetPriceTick(m_currPriceTick);
	pDlg->SetFtdcOrderField(ftdcField);
	if(pDlg->ShowModal()==wxID_OK) 
    {
		fNewPrice = pDlg->GetNewPrice();
		bNewIsMarketPrice = pDlg->GetNewIsMarketPrice();
		nNewVolume = pDlg->GetNewVolume();
		pDlg->Destroy();
	}
	else 
    {
		pDlg->Destroy();
		return FALSE;
	}

	int nOCMode = 0;
	switch(ftdcField.CombOffsetFlag[0]) 
    {
		case THOST_FTDC_OF_Open:        nOCMode = 0;  break; 
        case THOST_FTDC_OF_CloseToday:  nOCMode = 1;  break;
		default:                        nOCMode = 2;  break;
	};
	
    BOOL bOK = TRUE;
    COrderChangeEntity::ORIORDERREQ tOriReq={0};
    strcpy(tOriReq.strInstrumentID, ftdcField.InstrumentID);
    strcpy(tOriReq.strAccount, ftdcField.Account);
    tOriReq.bDirection = bDirection;
    tOriReq.nOCMode = nOCMode;
    tOriReq.fPrice = fNewPrice;
    tOriReq.bIsMarketPrice = bNewIsMarketPrice;
    tOriReq.bSimulate = NeedSimulateMarketPrice();
    tOriReq.nVolume = nNewVolume;
    tOriReq.nDefaultVolume = nNewVolume;
    tOriReq.bIsAuto = FALSE;
    tOriReq.nAutoMode = 0;
	tOriReq.nHedgeStatus = ftdcField.CombHedgeFlag[0]-THOST_FTDC_HF_Speculation;

    // 处理风险管理，手数
    if(tOriReq.nVolume > GetRaskVolumeLimit() && GetRaskVolumeLimit()>0) 
    {
	    wxMessageDialog dialog( NULL, LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	    if(dialog.ShowModal()!=wxID_YES)
		    return FALSE;
    }

    // 处理风险管理，价格
    BOOL bIsPriceLimit = tOriReq.fPrice>field.LastPrice+m_currPriceTick*GetRaskDifferPrice() || tOriReq.fPrice<field.LastPrice-m_currPriceTick*GetRaskDifferPrice();
    bIsPriceLimit = bIsPriceLimit && GetRaskDifferPrice();
    if(bIsPriceLimit) 
    {
	    wxMessageDialog dialog( NULL,LOADSTRING(OIP_OVERLIMITPRICE), LOADSTRING(OIP_FASTTRADER), wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	    if(dialog.ShowModal()!=wxID_YES) 
		    return FALSE;
    }

    wxString strRetRevoke;
    wxString strMsg;

    COrderChangeEntity* poEntity = COrderChangeEntity::CreateOrderChange(tOriReq);
    // 先生成预撤单列表，然后调用DoRevokeOrder执行实际撤单
    poEntity->RevokeOrder(ftdcField, strRetRevoke);
    strMsg = strRetRevoke;
    strMsg += poEntity->GetOrderDesciption();

    // 处理界面同步显示
    if(bIsAsyncView) 
    {
	    SetInputQty(tOriReq.nVolume);
	    SetInputOCMode(nOCMode);
    }

    int nErrPos = -1;
    wxString strErrMsg;
    if(NeedConfirm()) 
    {
	    wxMessageDialog dialog( NULL,strMsg , LOADSTRING(OLS_ORDER_CONFIRM), wxYES_DEFAULT|wxYES_NO|wxICON_QUESTION);
	    bOK = dialog.ShowModal()==wxID_YES;
	    USERLOG_INFO("提示选择，标题[%s]，\n用户选择%s\n",dialog.GetTitle().c_str(),bOK?"是":"否");
    }
    if(bOK) 
    {
	    // 如果有撤单，着把实体放到等待队列里，在确定所有撤单成功后，在执行下单
	    if(poEntity->DoRevokeOrder(nErrPos, strErrMsg))
		    m_mapOrderChangeWaitForCancel[poEntity] = 3;
    }
    wxString strCaption;
    if(nErrPos==-1) m_bWaitingRsp_OrderInsert=true;
    else ShowTradeInfoDlg("OIP_SOF", poEntity->GetOrderReq(nErrPos), NeedOrderFailConfirm()); // title: 中国国际期货网上交易终端

    if(bOK) 
    {
	    //1 下单后不清空下单板, 2 全部清空, 3 清空价格和手数, 4 只清空手数
	    if(bIsAsyncView) 
		    m_poOrderInputPanel->ClearViewData(GetActionAfterOrder());
    }
    return TRUE;
}

// 键盘快速下单
void COrderInsertOperations::DoKeyOrder(wxString& strCode, KeyOrderInfo& keyOrder)
{
	//USERLOG_INFO("COrderInsertOperations::DoKeyOrder\n"
	//		 "InstrumentID=[%s],\t Action=[%d],\t ActionArea=[%d],\t ShortKey=[%s],\t BuySell=[%d],\t Price=[%d],\t OverPrice=[%d]\n", 
	//		 strCode.c_str(), 
	//		 keyOrder.nAction, 
	//		 keyOrder.nActionArea,
 //			 keyOrder.ShortKey,
	//		 keyOrder.nBuySell,
	//		 keyOrder.nPrice,
	//		 keyOrder.nOverPrice);

	if ( keyOrder.nAction == 0)
	{
		return;
	}

	// 当键盘快速下单时，自动功能优先于盈损单状态
	if(m_nViewSubStyle==conSubStyle_Position) {
		SetStopPositionSelect(FALSE);
		m_poOrderPositionPanel->InitPositionPanel();
		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
		evt.SetInt(0);
		ProcessEvent(evt);
	}

	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT(); 
	LPORDER_CFG pOrderCfg = NULL;
	if(pMgr == NULL) return;
	pOrderCfg = pMgr->GetOrderCfg();

	set<string> InstrumentIDArray;
	InstrumentIDArray.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray);
	if(InstrumentIDArray.find(strCode.c_str())==InstrumentIDArray.end()) {
		ShowTradeInfoDlg("OIP_ERROR","OIP_CONTRERR",TRUE);
        return;
    }

	// 首先把合约代码写到界面上
	m_bNoKeyChangeText = FALSE;
	m_bIsGridChangeInstrumentID = TRUE;
	SetInputInstrumentID(strCode);
	m_bIsGridChangeInstrumentID = FALSE;
	m_bNoKeyChangeText = TRUE;

	// 设置手数
	int nDefaultVolume = GetDefaultVolume(strCode);
//	SetInputQty(nDefaultVolume);
	SetInputOrderType(OrderType_PATS_LIMIT);

	double fPrice = 0.0;
	int nOverPrice = 0;
	BOOL bIsBuy = FALSE;
	double fPriceTick = 0.0;

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strCode.c_str(),field);

	// 获取市价
	nOverPrice = keyOrder.nOverPrice;
	if( keyOrder.nPrice == 0) {
		fPrice = field.LastPrice;
	}
	else if( keyOrder.nPrice == 1) {
		fPrice = field.BidPrice1;
	}
	else if( keyOrder.nPrice == 2) {
		fPrice = field.AskPrice1;
	}

	fPriceTick=m_currPriceTick;
	if(keyOrder.nBuySell==0) {
		bIsBuy = TRUE;
		if(!util::isInvalidValue(fPrice)) {
			fPrice += (nOverPrice*fPriceTick);
		}
	}
	else {
		bIsBuy = FALSE;
		if(!util::isInvalidValue(fPrice)) {
			fPrice -= (nOverPrice*fPriceTick);
		}
	}
	// 设置方向及价格
	SetInputPrice(fPrice);
	SetInputDirection(bIsBuy);
	//SetInputAutoOpenClose(TRUE);

	BOOL bIsAuto = TRUE;
	int nOCMode = 0;
	int nVolume = nDefaultVolume;
	int nSumVolume = 0, nSumTodayVolume=0;
	COrderInsertEntity::ORIORDERREQ tOriReq={0};
	NeedAutoOpenClose(tOriReq.nAutoMode);

//	SetInputAutoOpenClose(TRUE);
	int nHedgeStatus = GetInputHedge();
	// 可平数 总仓/今仓
	//COrderInsertEntity::GetPreVolume(strCode.c_str(), !bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()));
	if(COrderInsertEntity::GetHoldingVolume(strCode.c_str(), !bIsBuy, nHedgeStatus, nSumVolume, nSumTodayVolume, string(GetInputAccount().c_str()))) {
		nVolume = nSumVolume;
	}
	if(nSumTodayVolume>0) {
		nOCMode=1;
		//SetInputQty(nSumTodayVolume);
	}
	else if(nSumVolume>0){
		nOCMode=2;
		//SetInputQty(nSumVolume-nSumTodayVolume);
	}
	else {
		nOCMode=0;
		//SetInputQty(GetDefaultVolume(strCode));
	}
	SetInputOCMode(nOCMode);

	if(nSumVolume==0) 
	{
		nVolume = nDefaultVolume;
		SetInputQty(nDefaultVolume);
		SetInputOCMode(0);
		bIsAuto = FALSE;
	}
	else 
	{
		if(tOriReq.nAutoMode==1)
			SetInputQty(nSumVolume);
		else 
			SetInputQty(nDefaultVolume);
	}

//	// 读取开平仓
//	int nOCMode = GetInputOCMode();
//	//bool bHedge=!((wxCheckBox*)FindWindowById(ID_CHECKBOX1,this))->GetValue();
//	BOOL bHedge = GetInputHedge();

	wxString strAccount=GetInputAccount();

	strcpy(tOriReq.strInstrumentID, strCode.c_str());
	strcpy(tOriReq.strAccount, strAccount.c_str());
	tOriReq.bIsParketOrder = FALSE;
	tOriReq.bDirection = bIsBuy;
	tOriReq.nOCMode = nOCMode;
	tOriReq.fPrice = fPrice;
	tOriReq.bIsMarketPrice = FALSE;
	tOriReq.bSimulate = NeedSimulateMarketPrice();
	tOriReq.nVolume = nVolume;
	tOriReq.nDefaultVolume = nDefaultVolume;//GetDefaultVolume(strCode);		// 在OK按钮下，使用输入手数作为默认手数
	tOriReq.bIsAuto = bIsAuto;
	tOriReq.nHedgeStatus = nHedgeStatus;

	BOOL bHasOrderInsert = FALSE;
	CConfirmItemChange confirmChg;
	confirmChg.SetItemNum(CConfirmItemChange::conAutoKeyPressConfirm);
	confirmChg.SetItemParam(std::string(keyOrder.ShortKey));
	DoOrderInsert(tOriReq, keyOrder.nAction, bHasOrderInsert, confirmChg, TRUE);

	if(bHasOrderInsert && NeedReturnQuotList() && (keyOrder.nAction==1 || keyOrder.nAction==2)) {
		wxCommandEvent evtnew(wxEVT_MAINFRAME_QUOT_SETFOCUS, GetId());
		GETTOPWINDOW()->AddPendingEvent(evtnew);
	}

}

//市价反手
void COrderInsertOperations::DoBackHand(wxString strAccount, wxString strInstrumentID, char PosiDirection, char Hedge, int nVolume, BOOL bOnlyOpen)
{
	//USERLOG_INFO("COrderInsertOperations::DoBackHand\n"
	//		 "InstrumentID=[%s],\t Direction=[%c],\t Hedge=[%c],\t nVolume=[%d],\t OnlyOpen=[%s]\n", 
	//		 strInstrumentID.c_str(), PosiDirection, Hedge, nVolume, bOnlyOpen?"TRUE":"FALSE");
	
	// 当鼠标快速平仓时，自动功能优先于盈损单状态
	if(m_nViewSubStyle==conSubStyle_Position) {
		SetStopPositionSelect(FALSE);
		m_poOrderPositionPanel->InitPositionPanel();
		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED, ID_DOSTOPPOSITION_CHECK);
		evt.SetInt(0);
		ProcessEvent(evt);
	}

	int nDefaultVolume = nVolume*2;					// 获得合约的默认量
	double fPrice = 0.0;

	m_bNoKeyChangeText = FALSE;
    SetInputInstrumentID(strInstrumentID);
	m_bNoKeyChangeText = TRUE;

	// 读取方向
	BOOL bDirection = (PosiDirection==THOST_FTDC_PD_Long ? 0 : 1);

	SetInputAccount(strAccount);
	SetInputOrderType(OrderType_PATS_MARKET);
	SetInputDirection(bDirection);
	SetInputQty(nVolume);
	wxString strPriceText(LOADSTRING(OLS_MARKET));
	SetInputPriceText(strPriceText);

//	// 读取开平仓
//	int nOCMode = GetInputOCMode();
//	//获取是否自动
//	BOOL bIsAuto = TRUE;

	COrderInsertEntity::ORIORDERREQ tOriReq={0};
	strcpy(tOriReq.strInstrumentID, strInstrumentID.c_str());
	strcpy(tOriReq.strAccount, strAccount.c_str());
	tOriReq.bIsParketOrder = FALSE;
	tOriReq.bDirection = bDirection;
	if(bOnlyOpen) {
		tOriReq.nOCMode = 0;//nOCMode; // 快捷锁仓，直接使用对开处理
	}
	else {
		tOriReq.nOCMode = 2;//nOCMode; // 市价反手，直接使用平仓处理
	}
	tOriReq.fPrice = fPrice;
	tOriReq.bIsMarketPrice = TRUE;
	tOriReq.bSimulate = NeedSimulateMarketPrice();
	tOriReq.nVolume = nVolume;
	tOriReq.nDefaultVolume = nDefaultVolume;		// 在OK按钮下，使用输入手数作为默认手数
	if(bOnlyOpen) {
		tOriReq.bIsAuto = FALSE;
		tOriReq.nAutoMode = 0;
	}
	else {
		tOriReq.bIsAuto = TRUE;
		tOriReq.nAutoMode = 3;
	}
	tOriReq.nHedgeStatus = Hedge-THOST_FTDC_HF_Speculation;

	//int nAction = NeedConfirm() ? 1 : 2;
	//DoOrderInsert(tOriReq, nAction, FALSE);
	BOOL bHasOrderInsert = FALSE;
	CConfirmItemChange confirmChg;
	confirmChg.SetItemNum(CConfirmItemChange::conStdOrderConfirm);
	DoOrderInsert(tOriReq, 0, bHasOrderInsert, confirmChg, TRUE);

}
