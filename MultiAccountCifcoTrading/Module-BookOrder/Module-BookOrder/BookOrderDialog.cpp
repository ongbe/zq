// BookOrderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "BookOrderDialog.h"
#include "SequenceManager.h"
#include "Module-Misc/tools_util.h"


#define QUERYMETIMER			0x7f998876
#define HIDEMETIMER				0x7f998877
#define TIMEOUTTIMER			0x7f998878
#define COLORGRADIENTTIMER		0x7f998879

// CBookOrderDialog dialog

IMPLEMENT_DYNAMIC(CBookOrderDialog, CDialog)

CBookOrderDialog::CBookOrderDialog(CWnd* pParent /*=NULL*/)
	: CBookOrderOperations(CBookOrderDialog::IDD, pParent)
{
	ZeroMemory(&m_tPriceGridIndex, sizeof(m_tPriceGridIndex));
	ZeroMemory(&m_tOldPriceGridIndex, sizeof(m_tOldPriceGridIndex));
	ZeroMemory(&m_tAccountInfo, sizeof(m_tAccountInfo));
	ZeroMemory(&m_tInstrumentInfo, sizeof(m_tInstrumentInfo));
	m_nPositionCount = 0;
	//ZeroMemory(m_tPositions, sizeof(m_tPositions));
	ZeroMemory(&m_tDepthMarketData, sizeof(m_tDepthMarketData));
	m_pdlgInstrumentIDList = NULL;
	m_pdlgQtyList = NULL;
	m_pDlgBrushBk = NULL;
	m_pdlgCancelConfirm = NULL;
	m_pButtonBrushBk = NULL;
	m_pEditBrushBk = NULL;
	ZeroMemory(&m_bookOrderDlgColor, sizeof(m_bookOrderDlgColor));
	m_nDlgMaxWidth = 40;
	m_ndlgMaxHeight = 20;
	m_bNeedInstrumentIDInputPanel = true;
	m_bNeedQtyInputPanel = true;
	m_nQueryOrderLastSeqNum = -1;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MOUSELEFT);
}

CBookOrderDialog::~CBookOrderDialog()
{
	//::DeleteObject(m_hIcon);
	//::CloseHandle(m_hIcon);
	::DeleteObject(m_hiconMouseLeft);
	::DeleteObject(m_hiconMouseRight);

	if(m_pdlgInstrumentIDList!=NULL)
	{
		delete m_pdlgInstrumentIDList;
		m_pdlgInstrumentIDList = NULL;
	}
	if(m_pdlgQtyList!=NULL)
	{
		delete m_pdlgQtyList;
		m_pdlgQtyList = NULL;
	}
	if(m_pDlgBrushBk!=NULL)
	{
		m_pDlgBrushBk->DeleteObject();
		delete m_pDlgBrushBk;
		m_pDlgBrushBk = NULL;
	}
	if(m_pButtonBrushBk!=NULL)
	{
		m_pButtonBrushBk->DeleteObject();
		delete m_pButtonBrushBk;
		m_pButtonBrushBk = NULL;
	}
	if(m_pEditBrushBk!=NULL)
	{
		m_pEditBrushBk->DeleteObject();
		delete m_pEditBrushBk;
		m_pEditBrushBk = NULL;
	}
}

void CBookOrderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDPRICE, m_gridPrice);             // associate the grid window with a C++ object
	DDX_Control(pDX, IDC_GRIDACCOUNT, m_gridAccountInfo);
	DDX_Control(pDX, IDC_GRIDPOSITION, m_gridPositionInfo);
	DDX_Control(pDX, IDC_CHECK_ALLOW_ORDER, m_CheckAllowOrder);
	DDX_Control(pDX, IDC_EDIT_INSTRUMENTID, m_EditInstrumentID);
	DDX_Control(pDX, IDC_COMBO_OC_LEFT, m_comboOCLeft);
	DDX_Control(pDX, IDC_COMBO_OC_RIGHT, m_comboOCRight);
	DDX_Control(pDX, IDC_EDIT_QTY_LEFT, m_editQtyLeft);
	DDX_Control(pDX, IDC_EDIT_QTY_RIGHT, m_editQtyRight);
	DDX_Control(pDX, IDC_CHECK_FIXPRICE, m_CheckFixPrice);
	DDX_Control(pDX, IDC_CHECK_CHANGEORDER, m_CheckChangeOrder);
	DDX_Control(pDX, IDC_SPIN_QTY_LEFT, m_SpinQtyLeft);
	DDX_Control(pDX, IDC_SPIN_QTY_RIGHT, m_SpinQtyRight);
	DDX_Control(pDX, ID_REFLASHPRICE, m_btnReflashPrice);
	DDX_Control(pDX, ID_CANCELALL, m_btnCancelAll);
	DDX_Control(pDX, ID_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_CHECK_TOMOSTTOP, m_CheckToMostTop);
	DDX_Control(pDX, IDC_BTN_COLOR1, m_btnColor1);
	DDX_Control(pDX, IDC_BTN_COLOR2, m_btnColor2);
	DDX_Control(pDX, IDC_BTN_COLOR3, m_btnColor3);
	DDX_Control(pDX, IDC_BTN_COLOR4, m_btnColor4);
}

BOOL CBookOrderDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	//if(m_NcButtonManager.PreTranslateMessage(pMsg))
	//	return TRUE;
	//else
	{
		//m_btnReflashPrice.PreTranslateMessage(pMsg);
		//m_btnCancelAll.PreTranslateMessage(pMsg);
		//m_btnClose.PreTranslateMessage(pMsg);
		if(pMsg->message == WM_LBUTTONDOWN) 
		{ 
			CRect rect;                                                                
			m_EditInstrumentID.GetWindowRect(&rect); 
			if(rect.PtInRect(pMsg->pt)) 
			{ 
				OnTextLeftClicked_InstrmentID();
			}
			m_editQtyLeft.GetWindowRect(&rect); 
			if(rect.PtInRect(pMsg->pt)) 
			{ 
				OnTextLeftClicked_QtyLeft();
			}
			m_editQtyRight.GetWindowRect(&rect); 
			if(rect.PtInRect(pMsg->pt)) 
			{ 
				OnTextLeftClicked_QtyRight();
			}
		} 
		else if(pMsg->message == WM_KEYDOWN)
		{
			if(pMsg->hwnd==m_EditInstrumentID.GetSafeHwnd())
			{
				OnTextKeyDown_InstrmentID((UINT)pMsg->wParam);
			}
		}
	 
		return CDialog::PreTranslateMessage(pMsg);
	}
}

LRESULT CBookOrderDialog::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	//LRESULT lResult = CDialog::DefWindowProc(message, wParam, lParam);
	//if(!IsWindow(this->m_hWnd))
	//	return lResult;

	//LRESULT lMyResult = m_NcButtonManager.DefWindowProc(GetSafeHwnd(), message, wParam, lParam);
	//if(lMyResult != 0)
	//	return lMyResult;
	//else
	//	return lResult;	
	LRESULT lResult = CDialog::DefWindowProc(message, wParam, lParam);
	//if(!IsWindow(this->m_hWnd))
	//	return lResult;

	//switch(message)
	//{
	//case WM_NCACTIVATE:
	//	OnNcPaint();
	//	break;
	//case WM_NCPAINT:
	//	OnNcPaint();
	//	break;
	//}
	return lResult;

	//return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL CBookOrderDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CBookOrderDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	// TODO:  Return a different brush if the default is not desired
	if(pWnd==this)
	{
		return m_brushWhite;
	}
	else if(pWnd->GetDlgCtrlID()==IDC_LABEL_TITLE
			|| pWnd->GetDlgCtrlID()==IDC_CHECK_TOMOSTTOP)
	{
        pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_bookOrderDlgColor.dwDlgBackColor);
        return *m_pDlgBrushBk;//套用视觉样式返回背景刷子，没有套用则返回空刷子//m_brushNull;
	}
	else if(pWnd->GetDlgCtrlID()==IDC_STATIC 
			|| pWnd->GetDlgCtrlID()==IDC_CHECK_ALLOW_ORDER 
			|| pWnd->GetDlgCtrlID()==IDC_CHECK_FIXPRICE 
			|| pWnd->GetDlgCtrlID()==IDC_CHECK_CHANGEORDER)
	{
        pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(255, 255, 255));
		return m_brushWhite;
	}
	return hbr;
}

void CBookOrderDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	lpMMI->ptMinTrackSize.x = m_nDlgMaxWidth;
	lpMMI->ptMinTrackSize.y = m_ndlgMaxHeight;

	CDialog::OnGetMinMaxInfo(lpMMI);
}


BEGIN_MESSAGE_MAP(CBookOrderDialog, CDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//ON_WM_NCPAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()

	ON_BN_CLICKED(ID_CLOSE, &CBookOrderDialog::OnBnClickedClose)
	ON_BN_CLICKED(ID_REFLASHPRICE, &CBookOrderDialog::OnBnClickedReflashprice)

	ON_BN_CLICKED(IDC_BTN_COLOR1, OnNcBtnColor1Clicked)
	ON_BN_CLICKED(IDC_BTN_COLOR2, OnNcBtnColor2Clicked)
	ON_BN_CLICKED(IDC_BTN_COLOR3, OnNcBtnColor3Clicked)
	ON_BN_CLICKED(IDC_BTN_COLOR4, OnNcBtnColor4Clicked)

    ON_NOTIFY(NM_CLICK, IDC_GRIDPRICE, OnGridLeftClick)
	ON_NOTIFY(NM_RCLICK, IDC_GRIDPRICE, OnGridRightClick)
    ON_NOTIFY(NM_DBLCLK, IDC_GRIDPRICE, OnGridLeftDblClick)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_QTY_LEFT, &CBookOrderDialog::OnDeltaposSpinQtyLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_QTY_RIGHT, &CBookOrderDialog::OnDeltaposSpinQtyRight)
	ON_BN_CLICKED(ID_CANCELALL, &CBookOrderDialog::OnBnClickedCancelall)
	ON_EN_CHANGE(IDC_EDIT_INSTRUMENTID, &CBookOrderDialog::OnEnChangeEditInstrumentid)
	ON_BN_CLICKED(IDC_CHECK_FIXPRICE, &CBookOrderDialog::OnBnClickedCheckFixprice)
	ON_BN_CLICKED(IDC_CHECK_TOMOSTTOP, &CBookOrderDialog::OnBnClickedCheckTomosttop)
	ON_BN_CLICKED(IDC_CHECK_ALLOW_ORDER, &CBookOrderDialog::OnBnClickedCheckAllowOrder)
END_MESSAGE_MAP()


// CBookOrderDialog message handlers


void CBookOrderDialog::ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	if(m_bExiting)
		return;
	switch(nCmdId) {
		case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
		case CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP:
		case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
		case CMDID_INSTRUMENT_INFO_RSP:
			ParseCmd_Base(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
			break;
		case CMDID_ALL_ORDER_LIST_RSP:
		case CMDID_ALL_ORDER_INFO_RSP:
		case CMDID_POSITION_INFO_RSP:
		case CMDID_POSITION_INFO_LIST_RSP:
		case CMDID_QUOTE_SNAPSHOT_RSP:
		case CMDID_QUOTE_SEND_QUOTE:
		case CMDID_ORDER_ORDER_STATUS_CHANGE:
		case CMDID_ORDER_TRADE_RTN:

		case CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP:
		case CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP:
			ParseCmd_Business(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
			break;
		case CMDID_CONFIG_CHANGE:
		case CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP:
		case CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP:
		case CMDID_CONFIG_INPUT_VOLUMES_RSP:
			ParseCmd_Config(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
			break;
	};
}

void CBookOrderDialog::ParseCmd_Base(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen>0 && pData!=NULL) 
				{
					char strUserID[64];
					ZeroMemory(strUserID, sizeof(strUserID));
					strncpy(strUserID, (char*)pData, nDataLen>63 ? 63 : nDataLen);
					m_strCurrAccount = strUserID;
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得当前用户名", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen>0 && pData!=NULL)
				{
					char strSubAccount[64];
					ZeroMemory(strSubAccount, sizeof(strSubAccount));
					strncpy(strSubAccount, (char*)pData, nDataLen>63 ? 63 : nDataLen);
					m_strCurrSubAccount = strSubAccount;
					QueryCurrAccountInfo(CString(strSubAccount));
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得当前子帐号", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(PlatformStru_TradingAccountInfo))
				{
					memcpy(&m_tAccountInfo, pData, sizeof(PlatformStru_TradingAccountInfo));
					ShowCurrAccountInfo();
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得当前帐号信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_INSTRUMENT_INFO_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(PlatformStru_InstrumentInfo))
				{
					PlatformStru_InstrumentInfo* pInfo = (PlatformStru_InstrumentInfo*)pData;
					char strInstrmentID[sizeof(InstrumentIDType)+4];
					ZeroMemory(strInstrmentID, sizeof(strInstrmentID));
					strncpy(strInstrmentID, pInfo->InstrumentID, sizeof(InstrumentIDType));
					if(GetInstrumentID().CompareNoCase(strInstrmentID)==0) {
						memcpy(&m_tInstrumentInfo, pData, sizeof(PlatformStru_InstrumentInfo));
						QueryPositionInfo(CString(strInstrmentID));
					}
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法指定合约信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		default:
			break;
	}
}

void CBookOrderDialog::ParseCmd_Business(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_ALL_ORDER_LIST_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(int)==0) 
				{
					if(nDataLen==0)
					{
						::MessageBox(NULL, "当前合约无可撤报单", "提示", MB_ICONINFORMATION|MB_OK|MB_SYSTEMMODAL);
					}
					else
					{
						int nCount = nDataLen/sizeof(int);
						int* pnData = (int*)pData;
						for(int i=0; i<nCount; i++)
						{
							m_nQueryOrderLastSeqNum = QueryOrderInfo(pnData[i]);
						}
					}
				}
				else 
				{
					//AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_ALL_ORDER_INFO_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_OrderInfo)==0) 
				{
					PlatformStru_OrderInfo* pInfo = (PlatformStru_OrderInfo*)pData;
					if(m_nQueryOrderLastSeqNum>0)
					{
						if((pInfo->OrderStatus==THOST_FTDC_OST_PartTradedQueueing 
								|| pInfo->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)
								&& GetInstrumentID().CompareNoCase(pInfo->InstrumentID)==0)
							m_mapOrdersForCancel.insert(
								std::pair<int, PlatformStru_OrderInfo>(pInfo->FTID, *pInfo));
						if(nSeqNum==m_nQueryOrderLastSeqNum)
						{
							if(m_mapOrdersForCancel.size()>0)
							{
								CCancelConfirmDialog dlg(this);
								dlg.SetPriceTick(m_tInstrumentInfo.PriceTick);
								dlg.SetCanCancelOrder(m_mapOrdersForCancel);
								dlg.SetDlgBackColor(m_bookOrderDlgColor.dwDlgBackColor);
								dlg.SetDlgButtonBackColor(m_bookOrderDlgColor.dwDlgButtonBackColor);
								dlg.DoModal();
							}
							else
							{
								::MessageBox(NULL, "当前合约无可撤报单", "提示", MB_ICONINFORMATION|MB_OK|MB_SYSTEMMODAL);
							}
						}
					}
				}
				else 
				{
					//AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		//case CMDID_POSITION_INFO_RSP:
		case CMDID_POSITION_INFO_LIST_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_Position)==0) 
				{
					m_nPositionCount = 0;
					for(int i=0; i<conMaxPositionCount; i++)
					{
						m_tPositions[i].Clear();
					}
					int nCount = nDataLen/sizeof(PlatformStru_Position);
					for(int i=0; i<nCount && i<conMaxPositionCount; i++)
					{
						memcpy(&m_tPositions[i], (PlatformStru_Position*)pData+i, 
								sizeof(PlatformStru_Position));
					}
					m_nPositionCount = nCount>conMaxPositionCount 
							? conMaxPositionCount : nCount;
					ShowPositionInfos();
				}
				else 
				{
					//AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_QUOTE_SNAPSHOT_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_DepthMarketData)==0)
				{
					if(nDataLen>0)
					{
						memcpy(&m_tDepthMarketData, pData, sizeof(PlatformStru_DepthMarketData));
						m_tPriceGridIndex.bHasData = TRUE;
						InitPriceGrid(m_CheckFixPrice.GetCheck());
						ShowPriceGrid(m_CheckFixPrice.GetCheck());
						QueryOrderWaitVolumeList(CString(m_tDepthMarketData.InstrumentID), THOST_FTDC_D_Buy, TRUE);
						QueryOrderWaitVolumeList(CString(m_tDepthMarketData.InstrumentID), THOST_FTDC_D_Sell, FALSE);
					}
				}
				else 
				{
					//AfxMessageBox("无法获得行情快照", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_QUOTE_SEND_QUOTE:
			{
				if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_DepthMarketData)==0 && nDataLen>0)
				{
					memcpy(&m_tDepthMarketData, pData, sizeof(PlatformStru_DepthMarketData));
					if(!m_tPriceGridIndex.bHasData)
					{
						m_tPriceGridIndex.bHasData = TRUE;
						InitPriceGrid(m_CheckFixPrice.GetCheck());
					}
					ShowPriceGrid(m_CheckFixPrice.GetCheck());
					if(!util::equals(m_tPriceGridIndex.fTopPrice, m_tOldPriceGridIndex.fTopPrice))
					{
						QueryOrderWaitVolumeList(CString(m_tDepthMarketData.InstrumentID), THOST_FTDC_D_Buy, TRUE);
						QueryOrderWaitVolumeList(CString(m_tDepthMarketData.InstrumentID), THOST_FTDC_D_Sell, FALSE);
					}
					RecomputePositionProfit();
					ShowPositionInfos();
				}
			}
			break;
		case CMDID_ORDER_ORDER_STATUS_CHANGE:
			{
				if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_OrderInfo)==0 && nDataLen>0)
				{
					PlatformStru_OrderInfo* pOrderData = (PlatformStru_OrderInfo*)pData;
					//if(pOrderData->OrderStatus==THOST_FTDC_OST_AllTraded 
					//		|| pOrderData->OrderStatus==THOST_FTDC_OST_PartTradedQueueing 
					//		|| pOrderData->OrderStatus==THOST_FTDC_OST_PartTradedNotQueueing
					//		|| pOrderData->OrderStatus==THOST_FTDC_OST_NoTradeQueueing
					//		|| pOrderData->OrderStatus==THOST_FTDC_OST_NoTradeNotQueueing
					//		|| pOrderData->OrderStatus==THOST_FTDC_OST_Canceled)
					{
						CString strIntrumentID = GetInstrumentID();
						QueryCurrAccountInfo(m_strCurrSubAccount);
						QueryPositionInfo(strIntrumentID);
						int nRetSeqNum = QueryOrderWaitVolume(strIntrumentID, 
								pOrderData->LimitPrice, pOrderData->Direction);
						std::string strPrice(Price2String(pOrderData->LimitPrice, m_tInstrumentInfo.PriceTick));
						m_mapWaitRspOrderWait.insert(std::pair<int, std::string>(nRetSeqNum, strPrice));
					}
				}
			}
			break;
		case CMDID_ORDER_TRADE_RTN:
			if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_TradeInfo)==0 && nDataLen>0)
			{
				QueryCurrAccountInfo(GetInstrumentID());
				QueryPositionInfo(GetInstrumentID());
			}
			break;
		case CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP:
			if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlusinOpenOrderNumRsp)==0 && nDataLen>0)
			{
				PlusinOpenOrderNumRsp* pRsp = (PlusinOpenOrderNumRsp*)pData;
				if(pRsp->nNum==0)
				{
					std::string strPrice = Price2String(pRsp->queryCondition.dLimitPrice, 
							m_tInstrumentInfo.PriceTick);
					if(m_mapPriceCancel.find(strPrice)!=m_mapPriceCancel.end())
						m_mapPriceCancel.erase(strPrice);
				}
				std::map<int, std::string>::iterator it = 
						m_mapWaitRspOrderWait.find(nSeqNum);
				if(it!=m_mapWaitRspOrderWait.end())
				{
					std::string strPrice = it->second;
					m_mapWaitRspOrderWait.erase(it);
					BIDASKVOLUME tVol;
					ZeroMemory(&tVol, sizeof(tVol));
					if(m_mapOrderWaitVolume.find(strPrice)==m_mapOrderWaitVolume.end())
						m_mapOrderWaitVolume.insert(
							std::pair<std::string, BIDASKVOLUME>(strPrice, tVol));
					if(pRsp->queryCondition.cDirection==THOST_FTDC_D_Buy)
						m_mapOrderWaitVolume[strPrice].nBuy = pRsp->nNum;
					else
						m_mapOrderWaitVolume[strPrice].nSell = pRsp->nNum;
					tVol = m_mapOrderWaitVolume[strPrice];
					//TRACE("CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP return strPrice[%s], nVolume:=%u\n", strPrice.c_str(), nVolume);
					ShowOrderWaitVolume(atof(strPrice.c_str()), tVol);
				}
			}
			break;
		case CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP:
			if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(WaitOrderPrice)==0 && nDataLen>0)
			{
				m_tWaitOrderPrice= *(WaitOrderPrice*)pData;
				if(m_CheckFixPrice.GetCheck())
				{
					BOOL bOK = FALSE;
					if(!util::isInvalidValue(m_tWaitOrderPrice.dHighPrice) 
							&& util::greater(m_tWaitOrderPrice.dHighPrice, m_tOldWaitOrderPrice.dHighPrice))
						bOK = TRUE;
					if(!util::isInvalidValue(m_tWaitOrderPrice.dLowPrice) 
							&& util::isInvalidValue(m_tOldWaitOrderPrice.dLowPrice))
						bOK = TRUE;
					else if(!util::isInvalidValue(m_tWaitOrderPrice.dLowPrice) 
							&& !util::isInvalidValue(m_tOldWaitOrderPrice.dLowPrice)
							&& util::less(m_tWaitOrderPrice.dLowPrice, m_tOldWaitOrderPrice.dLowPrice))
						bOK = TRUE;
					if(bOK)
					{
						//InitPriceGrid(m_CheckFixPrice.GetCheck());
						ShowPriceGrid(m_CheckFixPrice.GetCheck());
						m_tOldWaitOrderPrice = m_tWaitOrderPrice;
						QueryOrderWaitVolumeList(CString(m_tDepthMarketData.InstrumentID), THOST_FTDC_D_Buy, TRUE);
						QueryOrderWaitVolumeList(CString(m_tDepthMarketData.InstrumentID), THOST_FTDC_D_Sell, FALSE);
					}
				}
			}
			break;
		default:
			break;
	};
}

void CBookOrderDialog::ParseCmd_Config(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_CONFIG_CHANGE:
			{
				if(nErrorCode == SUCCESS) 
				{
					QueryConfig_InstrumentIDInputPanelShow();
					QueryConfig_QtyInputPanelShow();
					QueryConfig_QtyInputPanelQtyDatas();
				}
				else 
				{
					//AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen==sizeof(bool) && pData!=NULL)
				{
					memcpy(&m_bNeedInstrumentIDInputPanel, pData, sizeof(bool));
				}
				else 
				{
					//AfxMessageBox("无法获得行情快照", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen==sizeof(bool) && pData!=NULL)
				{
					memcpy(&m_bNeedQtyInputPanel, pData, sizeof(bool));
				}
				else 
				{
					//AfxMessageBox("无法获得行情快照", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_CONFIG_INPUT_VOLUMES_RSP:
			{
				if(nErrorCode == SUCCESS && pData!=NULL && nDataLen==9*sizeof(int))
				{
					if(m_pdlgQtyList!=NULL)
					{
						m_pdlgQtyList->ResetQtyList((int*)pData, nDataLen/sizeof(int));
					}
				}
			}
			break;
		default:
			break;
	};
}

void CBookOrderDialog::ShowCurrAccountInfo()
{
	CString strText;
	strText.Format("%s", Money2String(m_tAccountInfo.DynamicProfit));
	m_gridAccountInfo.SetItemText(1, 0, strText);
	strText.Format("%s", Money2String(m_tAccountInfo.Available));
	m_gridAccountInfo.SetItemText(1, 1, strText);
	strText.Format("%s", Money2String(m_tAccountInfo.FrozenCash));
	m_gridAccountInfo.SetItemText(1, 2, strText);
	m_gridAccountInfo.Invalidate();
}

void CBookOrderDialog::ShowPositionInfos()
{
	int nFixedItemCount = m_gridPositionInfo.GetFixedRowCount();
	int nItemCount = m_gridPositionInfo.GetRowCount()-nFixedItemCount;
	if(nItemCount!=m_nPositionCount)
	{
		m_gridPositionInfo.SetRowCount(m_nPositionCount+nFixedItemCount);
	}
	CString strText;
	for(int i=0; i<m_nPositionCount; i++)
	{
		strText.Format("%s", m_tPositions[i].InstrumentID);
		m_gridPositionInfo.SetItemText(i+nFixedItemCount, 0, strText);
		strText.Format("%s", m_tPositions[i].PosiDirection==THOST_FTDC_PD_Long ? "买  " 
				: (m_tPositions[i].PosiDirection==THOST_FTDC_PD_Short ? "  卖" : "净持仓"));
		m_gridPositionInfo.SetItemText(i+nFixedItemCount, 1, strText);
		m_gridPositionInfo.SetItemFgColour(i+nFixedItemCount, 1, 
				m_tPositions[i].PosiDirection==THOST_FTDC_PD_Long ? RGB(255, 0, 0) : RGB(0, 160, 0));
		strText = Number2String(m_tPositions[i].Position);
		m_gridPositionInfo.SetItemText(i+nFixedItemCount, 2, strText);
		strText = Number2String(m_tPositions[i].CanCloseydVolume);
		m_gridPositionInfo.SetItemText(i+nFixedItemCount, 3, strText);
		strText = Number2String(m_tPositions[i].CanCloseTodayVolume);
		m_gridPositionInfo.SetItemText(i+nFixedItemCount, 4, strText);
		strText = Price2String(m_tPositions[i].AveragePositionPrice, m_tInstrumentInfo.PriceTick);
		m_gridPositionInfo.SetItemText(i+nFixedItemCount, 5, strText);
		strText.Format("%s", Money2String(m_tPositions[i].PositionProfit, 2));
		m_gridPositionInfo.SetItemText(i+nFixedItemCount, 6, strText);
		m_gridPositionInfo.SetItemFgColour(i+nFixedItemCount, 6, 
				util::equals(m_tPositions[i].PositionProfit, 0.0) ? RGB(0, 0, 0) : 
				(util::greater(m_tPositions[i].PositionProfit, 0.0) ? RGB(255, 0, 0) : RGB(0, 160, 0)));
		m_gridPositionInfo.SetRowHeight(i+nFixedItemCount, 20);
	}
	if(m_nPositionCount>1)
		m_gridPositionInfo.ExpandRowsToFit(FALSE);
	m_gridPositionInfo.Invalidate();
}

void CBookOrderDialog::SetItem_AskBid(CGridCtrl& gridPrice, double fPrice, int nVolume, double fPriceTick, double fTopPrice, BOOL bIsAsk)
{
	CString strText;
	int nFixedCount = gridPrice.GetFixedRowCount();
	int nItemCount = gridPrice.GetRowCount() - nFixedCount;
	int nColIndex = bIsAsk ? conColAskVolume : conColBidVolume;
	double fTickCount = ceil((fTopPrice - fPrice)/fPriceTick-0.00000009);
	int nRowIndex = (int)fTickCount;
	if(nRowIndex>=0 && nRowIndex<nItemCount)
	{
		strText = Number2String(nVolume);
#ifdef _DEBUG
		strText += " : ";
		strText += Price2String(fPrice, fPriceTick);
#endif
		gridPrice.SetItemText(nRowIndex+nFixedCount, nColIndex, strText);
	}
}

void CBookOrderDialog::SetItem_WaitVolume(CGridCtrl& gridPrice, double fPrice, int nVolume, 
										  double fPriceTick, double fTopPrice, BOOL bIsAsk)
{
	int nFixedCount = gridPrice.GetFixedRowCount();
	int nItemCount = gridPrice.GetRowCount() - nFixedCount;
	int nColIndex = bIsAsk ? conColSellVolumeWait : conColBuyVolumeWait;
	double fTickCount = ceil((fTopPrice - fPrice)/fPriceTick-0.00000009);
	int nRowIndex = (int)fTickCount;
	CString strText;
	if(nRowIndex>=0 && nRowIndex<nItemCount)
	{
		if(nVolume>0)
		{
			strText = Number2String(nVolume);
#ifdef _DEBUG
			strText += " : ";
			strText += Price2String(fPrice, fPriceTick);
#endif
			gridPrice.SetItemText(nRowIndex+nFixedCount, nColIndex, strText);
		}
		else 
		{
			gridPrice.SetItemText(nRowIndex+nFixedCount, nColIndex, "");
		}
	}
}

void SetItem_CenterAlign(CGridCtrl& gridPrice)
{
	CString strText;
	int nFixedCount = gridPrice.GetFixedRowCount();
	int nItemCount = gridPrice.GetRowCount() - nFixedCount;
	for(int i=0; i<nItemCount; i++)
	{
		for(int k=0; k<5; k++)
			gridPrice.SetItemFormat(i+nFixedCount, k, DT_CENTER);
	}
}

void CBookOrderDialog::InitPriceGrid(BOOL bFixedPrice, BOOL bNeedColorGradient)
{
	int nFixedCount = m_gridPrice.GetFixedRowCount();
	int nItemCount = m_gridPrice.GetRowCount()-m_gridPrice.GetFixedRowCount();

	CRect rectClient;
	//m_gridPrice.GetClientRect(&rectClient);
	m_gridPrice.GetWindowRect(&rectClient);
	int nItemAreaHeigh = rectClient.Height()-4-
			m_gridPrice.GetFixedRowCount()*m_gridPrice.GetFixedRowHeight();
	int nRealItemCount = nItemAreaHeigh/m_gridPrice.GetRowHeight(1);
#ifdef _DEBUG
	{
		CString strText;
		strText.Format("InitPriceGrid rectClient.Height():=%d, nRealItemCount:=%d, nItemAreaHeigh:=%d\n", rectClient.Height(), nRealItemCount, nItemAreaHeigh);
		TRACE(strText);
	}
#endif
	if(bFixedPrice)
	{
		if(nRealItemCount<11)
			nRealItemCount = 11;
		else
			nRealItemCount++;
	}
	if(nRealItemCount != nItemCount)
	{
		m_gridPrice.SetRowCount(nRealItemCount+nFixedCount);
		nItemCount = nRealItemCount;
	}

	BOOL bHasData = m_tPriceGridIndex.bHasData;
	double fTopPriceBak = m_tOldPriceGridIndex.fTopPrice;
	ZeroMemory(&m_tPriceGridIndex, sizeof(m_tPriceGridIndex));
	ZeroMemory(&m_tOldPriceGridIndex, sizeof(m_tOldPriceGridIndex));

	m_tPriceGridIndex.bHasData = bHasData;
	m_tPriceGridIndex.nGridRowCount = nItemCount;
	m_tPriceGridIndex.nLastPriceRowIndex = (nItemCount/2 + nItemCount%2)-1;
	m_tPriceGridIndex.fLastPrice = util::isInvalidValue(m_tDepthMarketData.LastPrice)
			? (util::isInvalidValue(m_tDepthMarketData.PreSettlementPrice) 
			? 0.0 : m_tDepthMarketData.PreSettlementPrice) 
			: m_tDepthMarketData.LastPrice;
	m_tPriceGridIndex.fTopPrice = m_tPriceGridIndex.fLastPrice
			+ m_tPriceGridIndex.nLastPriceRowIndex * m_tInstrumentInfo.PriceTick;
	m_tPriceGridIndex.fPriceTick = m_tInstrumentInfo.PriceTick;

	ResetPriceGridColor(m_bookOrderDlgColor.tPrice);
	
	if(!bFixedPrice && bNeedColorGradient && !util::equals(m_tPriceGridIndex.fTopPrice, fTopPriceBak))
	{
		if(util::greater(m_tPriceGridIndex.fTopPrice, fTopPriceBak))
			ResetColorGradient(m_bookOrderDlgColor.tPrice.tCellRisePrice.dwTextColor, 
					m_bookOrderDlgColor.tPrice.tPriceCol.tRow.dwTextColor);
		else
			ResetColorGradient(m_bookOrderDlgColor.tPrice.tCellFallPrice.dwTextColor, 
					m_bookOrderDlgColor.tPrice.tPriceCol.tRow.dwTextColor);
		m_nColorGradientIndex = conColorGradientNum;
		SetTimer(COLORGRADIENTTIMER, 500, NULL);
	}
	
	m_tOldPriceGridIndex = m_tPriceGridIndex;
}

void CBookOrderDialog::ShowPriceGrid_GradientColor()
{
	if(m_nColorGradientIndex>=0)
	{
		DWORD dwNewTextColor = GetGradientColor(m_nColorGradientIndex);
		int nFixedCount = m_gridPrice.GetFixedRowCount();
		int nItemCount = m_tPriceGridIndex.nGridRowCount;
		for(int i=nFixedCount; i<nItemCount+nFixedCount; i++) 
		{
			m_gridPrice.SetItemFgColour(i, conColPrice, dwNewTextColor);
		}
	}
}

void CBookOrderDialog::ShowPriceGrid(BOOL bFixedPrice, BOOL bReflash)
{

	if(!m_tPriceGridIndex.bHasData)
		return;

	int nFixedCount = m_gridPrice.GetFixedRowCount();
	int nItemCount = m_tPriceGridIndex.nGridRowCount;
	int nMidRowIndex = (nItemCount/2 + nItemCount%2)-1;

	if(m_tOldPriceGridIndex.bHasData)
	{
		int nRowIndex = m_tOldPriceGridIndex.nLastPriceRowIndex+nFixedCount;
		DWORD dwBackColor = 0, dwForeColor = 0;
		if(nRowIndex%2)
		{
			dwBackColor = m_bookOrderDlgColor.tPrice.tPriceCol.tRow.dwBackColor;
			dwForeColor = m_bookOrderDlgColor.tPrice.tPriceCol.tRow.dwTextColor;
		}
		else
		{
			dwBackColor = m_bookOrderDlgColor.tPrice.tPriceCol.tEvenRow.dwBackColor;
			dwForeColor = m_bookOrderDlgColor.tPrice.tPriceCol.tEvenRow.dwTextColor;
		}
		if(nRowIndex>=0 && nRowIndex<nItemCount)
		{
			m_gridPrice.SetItemBkColour(nRowIndex, conColPrice, dwBackColor);
			m_gridPrice.SetItemFgColour(nRowIndex, conColPrice, dwForeColor);
		}
	}

//	if(!bFixedPrice && nItemCount!=m_gridPrice.GetRowCount()-m_gridPrice.GetFixedRowCount())
//	{
//		InitPriceGrid(bFixedPrice);
//		nItemCount = m_tPriceGridIndex.nGridRowCount;
//		nMidRowIndex = (nItemCount/2 + nItemCount%2)-1;
//	}

	double fTopPrice = m_tPriceGridIndex.fTopPrice;

	m_tPriceGridIndex.fLastPrice = util::isInvalidValue(m_tDepthMarketData.LastPrice)
			? (util::isInvalidValue(m_tDepthMarketData.PreSettlementPrice) 
			? 0.0 : m_tDepthMarketData.PreSettlementPrice) 
			: m_tDepthMarketData.LastPrice;
	double fTickCount = ceil(
			(m_tPriceGridIndex.fTopPrice - m_tPriceGridIndex.fLastPrice)
			/ m_tPriceGridIndex.fPriceTick - 0.00000009);
	int nRowIndex = (int)fTickCount;

	if(!bFixedPrice)
	{
		if(nRowIndex < nMidRowIndex/2 || nRowIndex >= nItemCount - nMidRowIndex/2)
		{
			InitPriceGrid(bFixedPrice, TRUE);
			fTopPrice = m_tPriceGridIndex.fTopPrice;
			nRowIndex = nMidRowIndex;
		}
		else 
		{
			fTickCount = ceil(
					(m_tPriceGridIndex.fTopPrice - m_tDepthMarketData.BidPrice1)
					/ m_tPriceGridIndex.fPriceTick - 0.00000009);
			int nBid1RowIndex = (int)fTickCount;
			fTickCount = ceil(
					(m_tPriceGridIndex.fTopPrice - m_tDepthMarketData.AskPrice1)
					/ m_tPriceGridIndex.fPriceTick - 0.00000009);
			int nAsk1RowIndex = (int)fTickCount;
			if(((nBid1RowIndex<0 || nBid1RowIndex>nItemCount-1) 
					|| (nAsk1RowIndex<0 || nAsk1RowIndex>nItemCount-1)) 
					&& abs(nRowIndex-nBid1RowIndex)<nMidRowIndex
					&& abs(nRowIndex-nAsk1RowIndex)<nMidRowIndex)
			{
				InitPriceGrid(bFixedPrice);
				fTopPrice = m_tPriceGridIndex.fTopPrice;
				nRowIndex = nMidRowIndex;
			}
		}
	}
	else
	{
		int nInsertRowCount = 0;
		if(nRowIndex>(m_tPriceGridIndex.nGridRowCount-1)-5)
		{
			nInsertRowCount = 5 - ((m_tPriceGridIndex.nGridRowCount-1) - nRowIndex);
			//m_tPriceGridIndex.fTopPrice += nInsertRowCount*m_tPriceGridIndex.fPriceTick;
			m_gridPrice.SetRowCount(nFixedCount+m_tPriceGridIndex.nGridRowCount+nInsertRowCount);
		}
		else if(nRowIndex<5)
		{
			nInsertRowCount = 5 - nRowIndex;
			//for(int i=0; i<nInsertRowCount; i++)
			//	m_gridPrice.InsertRow("", 1);
			m_gridPrice.SetRowCount(nFixedCount+m_tPriceGridIndex.nGridRowCount+nInsertRowCount);
			m_tPriceGridIndex.fTopPrice += nInsertRowCount*m_tPriceGridIndex.fPriceTick;
			fTopPrice = m_tPriceGridIndex.fTopPrice;
			
			int nPos = m_gridPrice.GetScrollPos(SB_VERT);
			int nRealSubHeight = nInsertRowCount*m_gridPrice.GetRowHeight(1);
			m_gridPrice.SetScrollPos32(SB_VERT, nPos+nRealSubHeight);
		}

		if(util::less(m_tPriceGridIndex.fTopPrice, m_tWaitOrderPrice.dHighPrice) 
			&& !util::isInvalidValue(m_tWaitOrderPrice.dHighPrice))
		{
			double fTickCount = ceil(
					(m_tWaitOrderPrice.dHighPrice - m_tPriceGridIndex.fTopPrice)
					/ m_tPriceGridIndex.fPriceTick - 0.00000009);
			nInsertRowCount += (int)fTickCount;
			//for(int i=0; i<(int)fTickCount; i++)
			//	m_gridPrice.InsertRow("", 1);
			m_gridPrice.SetRowCount(nFixedCount+m_tPriceGridIndex.nGridRowCount+nInsertRowCount);
			m_tPriceGridIndex.fTopPrice = m_tWaitOrderPrice.dHighPrice;
			fTopPrice = m_tPriceGridIndex.fTopPrice;

			int nPos = m_gridPrice.GetScrollPos(SB_VERT);
			int nRealSubHeight = (int)fTickCount*m_gridPrice.GetRowHeight(1);
			m_gridPrice.SetScrollPos32(SB_VERT, nPos+nRealSubHeight);
		}
		if(util::greater(m_tPriceGridIndex.fTopPrice-
				(nItemCount+nInsertRowCount-1)*m_tPriceGridIndex.fPriceTick, 
				m_tWaitOrderPrice.dLowPrice) && !util::isInvalidValue(m_tWaitOrderPrice.dLowPrice))
		{
			double fTickCount = ceil(
					(m_tPriceGridIndex.fTopPrice-
					(nItemCount+nInsertRowCount-1)*m_tPriceGridIndex.fPriceTick
					- m_tWaitOrderPrice.dLowPrice)
					/ m_tPriceGridIndex.fPriceTick - 0.00000009);
			nInsertRowCount += (int)fTickCount;
			m_gridPrice.SetRowCount(nFixedCount+m_tPriceGridIndex.nGridRowCount+nInsertRowCount);
		}

		if(nInsertRowCount>0)
		{
			m_tPriceGridIndex.nGridRowCount += nInsertRowCount;
			nItemCount += nInsertRowCount;
			//m_gridPrice.SetRowCount(nFixedCount+m_tPriceGridIndex.nGridRowCount);
			//fTopPrice = m_tPriceGridIndex.fMidPrice + 
			//		m_tPriceGridIndex.fPriceTick * m_tPriceGridIndex.nMidRowIndex;
			fTickCount = ceil(
					(m_tPriceGridIndex.fTopPrice - m_tPriceGridIndex.fLastPrice)
					/m_tPriceGridIndex.fPriceTick-0.00000009);
			nRowIndex = (int)fTickCount;
			m_gridPrice.ExpandColumnsToFit();
			ResetPriceGridColor(m_bookOrderDlgColor.tPrice);
		}
	}

	m_tPriceGridIndex.nLastPriceRowIndex = nRowIndex;

	for(int i=nFixedCount; i<nItemCount+nFixedCount; i++) 
	{
		for(int k=0; k<5; k++)
			m_gridPrice.SetItemText(i, k, "");
	}

	if(!m_tPriceGridIndex.bHasData)
	{
		m_gridPrice.Invalidate();
		return;
	}

	CString strText;
	for(int i=0; i<nItemCount; i++)
	{
		strText = "";
		if(i==m_tPriceGridIndex.nLastPriceRowIndex)
			strText = "@";
		strText += Price2String(fTopPrice-i*m_tPriceGridIndex.fPriceTick, 
				m_tPriceGridIndex.fPriceTick);
		m_gridPrice.SetItemText(i+nFixedCount, conColPrice, strText);
	}
	
	if(m_tPriceGridIndex.nLastPriceRowIndex>=0 && m_tPriceGridIndex.nLastPriceRowIndex<nItemCount)
	{
		m_gridPrice.SetItemBkColour(m_tPriceGridIndex.nLastPriceRowIndex+nFixedCount, 
				conColPrice, m_bookOrderDlgColor.tPrice.tCellLastPrice.dwBackColor);
		m_gridPrice.SetItemFgColour(m_tPriceGridIndex.nLastPriceRowIndex+nFixedCount, 
				conColPrice, m_bookOrderDlgColor.tPrice.tCellLastPrice.dwTextColor);
	}
	
	for(int i=0; i<nItemCount; i++)
	{
		double fCurrPrice = fTopPrice-i*m_tPriceGridIndex.fPriceTick;
		std::string strPrice(Price2String(fCurrPrice, m_tPriceGridIndex.fPriceTick));
		std::map<std::string, BIDASKVOLUME>::iterator it = m_mapOrderWaitVolume.find(strPrice);
		if(it!=m_mapOrderWaitVolume.end())
		{
			SetItem_WaitVolume(m_gridPrice, fCurrPrice, it->second.nSell, 
					m_tPriceGridIndex.fPriceTick, fTopPrice, TRUE);
			SetItem_WaitVolume(m_gridPrice, fCurrPrice, it->second.nBuy, 
					m_tPriceGridIndex.fPriceTick, fTopPrice, FALSE);
		}
	}
		
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.AskPrice1, m_tDepthMarketData.AskVolume1, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, TRUE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.BidPrice1, m_tDepthMarketData.BidVolume1, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, FALSE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.AskPrice2, m_tDepthMarketData.AskVolume2, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, TRUE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.BidPrice2, m_tDepthMarketData.BidVolume2, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, FALSE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.AskPrice3, m_tDepthMarketData.AskVolume3, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, TRUE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.BidPrice3, m_tDepthMarketData.BidVolume3, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, FALSE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.AskPrice4, m_tDepthMarketData.AskVolume4, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, TRUE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.BidPrice4, m_tDepthMarketData.BidVolume4, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, FALSE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.AskPrice5, m_tDepthMarketData.AskVolume5, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, TRUE);
	SetItem_AskBid(m_gridPrice, 
			m_tDepthMarketData.BidPrice5, m_tDepthMarketData.BidVolume5, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, FALSE);

	ShowPriceGrid_GradientColor();
	
	m_tOldPriceGridIndex = m_tPriceGridIndex;

	if(bReflash)
		m_gridPrice.Invalidate();
}

void CBookOrderDialog::ShowLastPrice2GridCenter()
{
	int nItemCount = m_gridPrice.GetRowCount()-m_gridPrice.GetFixedRowCount();
	if(nItemCount==0)
		return;

	CRect rectCltGrid;
	m_gridPrice.GetClientRect(&rectCltGrid);

	int nRealItemCount = 0;
	int nFixedRowHeight = m_gridPrice.GetFixedRowHeight();
	nRealItemCount = (rectCltGrid.Height()-nFixedRowHeight)/m_gridPrice.GetRowHeight(1);

	if(nRealItemCount>=nItemCount)
		return;

	int nRealSubHeight = 0;
	int nMinPos = 0, nMaxPos = 0;
	m_gridPrice.GetScrollRange(SB_VERT, &nMinPos, &nMaxPos);

	int nLastPriceIndex = m_tPriceGridIndex.nLastPriceRowIndex;
	int nRealTopIndex = nLastPriceIndex-nRealItemCount/2;
	if(nRealTopIndex<0)
		nRealTopIndex=0;
	nRealSubHeight = nRealTopIndex*m_gridPrice.GetRowHeight(1);
	m_gridPrice.SetScrollPos32(SB_VERT, nRealSubHeight);
	//nRealSubHeight = ((nMaxPos+nMinPos)-rectCltGrid.Height()-nFixedRowHeight)/2;
	//if(nRealSubHeight>=8)
	//	m_gridPrice.SetScrollPos32(SB_VERT, nRealSubHeight);
}

void CBookOrderDialog::ShowOrderWaitVolume(double fPrice, BIDASKVOLUME& tVol)
{
	double fTopPrice = m_tPriceGridIndex.fTopPrice;
	SetItem_WaitVolume(m_gridPrice, fPrice, tVol.nSell, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, TRUE);
	SetItem_WaitVolume(m_gridPrice, fPrice, tVol.nBuy, 
			m_tPriceGridIndex.fPriceTick, fTopPrice, FALSE);
	m_gridPrice.Invalidate();
}

void CBookOrderDialog::ResetDlgColor(DWORD dwBackColor, 
									 DWORD dwButtonBackColor, 
									 DWORD dwEditBackColor)
{

	if(m_pDlgBrushBk!=NULL)
	{
		m_pDlgBrushBk->DeleteObject();
		m_pDlgBrushBk = NULL;
	}
	if(m_pButtonBrushBk!=NULL)
	{
		m_pButtonBrushBk->DeleteObject();
		m_pButtonBrushBk = NULL;
	}
	if(m_pEditBrushBk!=NULL)
	{
		m_pEditBrushBk->DeleteObject();
		m_pEditBrushBk = NULL;
	}

	m_pDlgBrushBk = new CBrush(dwBackColor);
	m_pButtonBrushBk = new CBrush(dwButtonBackColor);
	m_pEditBrushBk = new CBrush(dwEditBackColor);

	m_btnReflashPrice.SetBGColor(dwButtonBackColor);
	m_btnReflashPrice.Invalidate();
	m_btnCancelAll.SetBGColor(dwButtonBackColor);
	m_btnCancelAll.Invalidate();
	m_btnClose.SetBGColor(dwButtonBackColor);
	m_btnClose.Invalidate();

}

void CBookOrderDialog::ResetAccountGridColor(ROWCOLOR& rowColor)
{
	int nColCount = m_gridAccountInfo.GetColumnCount();
	int nItemCount = m_gridAccountInfo.GetRowCount();
	m_gridAccountInfo.SetBkColor(rowColor.tRow.dwBackColor);
	for(int i=0; i<nItemCount; i++)
	{
		for(int k=0; k<nColCount; k++)
		{
			m_gridAccountInfo.SetItemBkColour(i, k, rowColor.tRow.dwBackColor);
			m_gridAccountInfo.SetItemFgColour(i, k, rowColor.tRow.dwTextColor);
		}
	}
	m_gridAccountInfo.Invalidate();
}

void CBookOrderDialog::ResetPositionGridColor(ROWCOLOR& rowColor)
{
	int nFixedItemCount = m_gridPositionInfo.GetFixedRowCount();
	int nItemCount = m_gridPositionInfo.GetRowCount();
	int nColCount = m_gridPositionInfo.GetColumnCount();
	for(int i=0; i<nFixedItemCount; i++)
	{
		for(int k=0; k<nColCount; k++)
		{
			m_gridPositionInfo.SetItemBkColour(i, k, rowColor.tFixedRow.dwBackColor);
			m_gridPositionInfo.SetItemFgColour(i, k, rowColor.tFixedRow.dwTextColor);
		}
	}
	for(int i=nFixedItemCount; i<nItemCount; i++)
	{
		for(int k=0; k<nColCount; k++)
		{
			m_gridPositionInfo.SetItemBkColour(i, k, rowColor.tRow.dwBackColor);
			m_gridPositionInfo.SetItemFgColour(i, k, rowColor.tRow.dwTextColor);
		}
	}
	m_gridPositionInfo.Invalidate();
}

void CBookOrderDialog::ResetPriceGridColor(PRICEGRIDCOLOR& rowColor)
{
	int nFixedItemCount = m_gridPrice.GetFixedRowCount();
	int nItemCount = m_gridPrice.GetRowCount();
	ROWCOLOR rowColorCurr = {0};
	for(int k=0; k<=conColSellVolumeWait; k++)
	{
		switch(k)
		{
		case conColBuyVolumeWait:
			rowColorCurr = rowColor.tWaitBuyCol;
			break;
		case conColBidVolume:
			rowColorCurr = rowColor.tBidCol;
			break;
		case conColPrice: 
			rowColorCurr = rowColor.tPriceCol;
			break;
		case conColAskVolume: 
			rowColorCurr = rowColor.tAskCol;
			break;
		case conColSellVolumeWait:
			rowColorCurr = rowColor.tWaitSellCol;
			break;
		};

		for(int i=0; i<nFixedItemCount; i++)
		{
			m_gridPrice.SetItemBkColour(i, k, 
					rowColorCurr.tFixedRow.dwBackColor);
			m_gridPrice.SetItemFgColour(i, k, 
					rowColorCurr.tFixedRow.dwTextColor);
		}
		for(int i=nFixedItemCount; i<nItemCount; i++)
		{
			if(i%2)
			{
				m_gridPrice.SetItemBkColour(i, k, 
						rowColorCurr.tRow.dwBackColor);
				m_gridPrice.SetItemFgColour(i, k, 
						rowColorCurr.tRow.dwTextColor);
			}
			else
			{
				m_gridPrice.SetItemBkColour(i, k, 
						rowColorCurr.tEvenRow.dwBackColor);
				m_gridPrice.SetItemFgColour(i, k, 
						rowColorCurr.tEvenRow.dwTextColor);
			}
		}
	}
}

int CBookOrderDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	//POINT ptBtn1Offset = {46, 5};
	//POINT ptBtn2Offset = {66, 5};
	//POINT ptBtn3Offset = {86, 5};
	//POINT ptBtn4Offset = {106, 5};
	//SIZE sizeBtn1 = {16, 16};
	//SIZE sizeBtn2 = {16, 16};
	//SIZE sizeBtn3 = {16, 16};
	//SIZE sizeBtn4 = {16, 16};
	//
	//m_NcBtnColor1.Create("", m_hWnd, ptBtn1Offset, sizeBtn1, IDC_BTN_COLOR1);
	//m_NcBtnColor2.Create("", m_hWnd, ptBtn2Offset, sizeBtn2, IDC_BTN_COLOR2);
	//m_NcBtnColor3.Create("", m_hWnd, ptBtn3Offset, sizeBtn3, IDC_BTN_COLOR3);
	//m_NcBtnColor4.Create("", m_hWnd, ptBtn4Offset, sizeBtn4, IDC_BTN_COLOR4);
	//
	//m_NcBtnColor1.SetText("", "宋体", 12);
	//m_NcBtnColor1.SetButtonBitmap(IDB_BTNCOLOR1, BTNBMP_NORMAL);
	//m_NcBtnColor1.SetTooltip("窗体套色1");

	//m_NcBtnColor2.SetText("", "宋体", 12);
	//m_NcBtnColor2.SetButtonBitmap(IDB_BTNCOLOR2, BTNBMP_NORMAL);
	//m_NcBtnColor2.SetTooltip("窗体套色2");

	//m_NcBtnColor3.SetText("", "宋体", 12);
	//m_NcBtnColor3.SetButtonBitmap(IDB_BTNCOLOR3, BTNBMP_NORMAL);
	//m_NcBtnColor3.SetTooltip("窗体套色3");

	//m_NcBtnColor4.SetText("", "宋体", 12);
	//m_NcBtnColor4.SetButtonBitmap(IDB_BTNCOLOR4, BTNBMP_NORMAL);
	//m_NcBtnColor4.SetTooltip("窗体套色4");

	//m_NcButtonManager.AddButton(&m_NcBtnColor1);
	//m_NcButtonManager.AddButton(&m_NcBtnColor2);
	//m_NcButtonManager.AddButton(&m_NcBtnColor3);
	//m_NcButtonManager.AddButton(&m_NcBtnColor4);

    m_brushNull.CreateStockObject(NULL_BRUSH); 
	m_brushWhite.CreateSolidBrush(RGB(255, 255, 255)); 
	m_fontTitle.CreateFont(
		   16,                        // nHeight
		   8,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_BOLD,                   // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   DEFAULT_CHARSET,           // nCharSet
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   _T("宋体"));                 // lpszFacename

	return 0;
}

BOOL CBookOrderDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CWnd* pWnd = GetDlgItem(IDC_LABEL_TITLE);
	if(pWnd != NULL)
		pWnd->SetFont(&m_fontTitle);

	m_btnColor1.LoadBitmaps(IDB_BTNCLR1, IDB_BTNCLR1_DOWN);
	m_btnColor2.LoadBitmaps(IDB_BTNCLR2, IDB_BTNCLR2_DOWN);
	m_btnColor3.LoadBitmaps(IDB_BTNCLR3, IDB_BTNCLR3_DOWN);
	m_btnColor4.LoadBitmaps(IDB_BTNCLR4, IDB_BTNCLR4_DOWN);

	LONG rtn = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	rtn |= WS_EX_APPWINDOW;
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, rtn);

	// TODO:  Add extra initialization here
	m_hiconMouseLeft = ::LoadIcon(AfxFindResourceHandle(), MAKEINTRESOURCE(IDR_MOUSELEFT));
	m_hiconMouseRight = ::LoadIcon(AfxFindResourceHandle(), MAKEINTRESOURCE(IDR_MOUSERIGHT));

	m_pdlgInstrumentIDList = new CInstrumentIDListDialog();
	m_pdlgQtyList = new CQtyListDialog();
	m_pdlgInstrumentIDList->Create(CInstrumentIDListDialog::IDD, this);
	m_pdlgQtyList->Create(CQtyListDialog::IDD, this);
	//m_pdlgInstrumentIDList->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW);
	//m_pdlgQtyList->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW);

	//VERIFY(m_btnReflashPrice.Attach(ID_REFLASHPRICE, this)); 

	m_EditInstrumentID.SetLimitText(60);
	m_editQtyLeft.SetLimitText(4);
	m_editQtyRight.SetLimitText(4);

	m_editQtyLeft.SetSel(0, -1);
	m_editQtyLeft.ReplaceSel("1");
	m_editQtyRight.SetSel(0, -1);
	m_editQtyRight.ReplaceSel("1");

	m_comboOCLeft.InsertString(0, "");
	m_comboOCLeft.InsertString(1, "开仓");
	m_comboOCLeft.InsertString(2, "平今");
	m_comboOCLeft.InsertString(3, "平仓");
	m_comboOCLeft.SetCurSel(1);

	m_comboOCRight.InsertString(0, "");
	m_comboOCRight.InsertString(1, "开仓");
	m_comboOCRight.InsertString(2, "平今");
	m_comboOCRight.InsertString(3, "平仓");
	m_comboOCRight.SetCurSel(0);

	m_gridAccountInfo.SetEditable(FALSE);
	m_gridAccountInfo.SetGridLines(GVL_VERT);
	m_gridAccountInfo.EnableSelection(FALSE);
	m_gridAccountInfo.SetFixedColumnSelection(FALSE);
	m_gridAccountInfo.EnableDragAndDrop(FALSE);
	m_gridAccountInfo.SetTrackFocusCell(FALSE);
	m_gridAccountInfo.SetColumnCount(3);
	m_gridAccountInfo.SetRowCount(2);
	m_gridAccountInfo.ExpandRowsToFit(FALSE);
	m_gridAccountInfo.SetColumnWidth(0, 100);
	m_gridAccountInfo.SetColumnWidth(1, 100);
	m_gridAccountInfo.SetColumnWidth(2, 100);
	m_gridAccountInfo.SetItemText(0, 0, "动态权益");
	m_gridAccountInfo.SetItemText(0, 1, "可用资金");
	m_gridAccountInfo.SetItemText(0, 2, "冻结资金");
	m_gridAccountInfo.Invalidate();

	m_gridPositionInfo.SetDefCellHeight(20);
	m_gridPositionInfo.SetDefCellMargin(3);
	m_gridPositionInfo.SetEditable(FALSE);
	m_gridPositionInfo.EnableSelection(TRUE);
	m_gridPositionInfo.SetListMode(TRUE);
	m_gridPositionInfo.SetSingleRowSelection(TRUE);
	m_gridPositionInfo.SetFixedColumnSelection(FALSE);
	m_gridPositionInfo.EnableDragAndDrop(FALSE);
	m_gridPositionInfo.SetColumnResize(FALSE);
	m_gridPositionInfo.SetHeaderSort(FALSE);
	m_gridPositionInfo.SetTrackFocusCell(FALSE);
	m_gridPositionInfo.SetShowFocusRect(FALSE);
	m_gridPositionInfo.SetBkColor(RGB(255, 255, 255));
	m_gridPositionInfo.SetColumnCount(7);
	m_gridPositionInfo.SetRowCount(1);
	m_gridPositionInfo.SetFixedRowCount(1);
	m_gridPositionInfo.SetColumnWidth(0, 65);
	m_gridPositionInfo.SetColumnWidth(1, 42);
	m_gridPositionInfo.SetColumnWidth(2, 50);
	m_gridPositionInfo.SetColumnWidth(3, 68);
	m_gridPositionInfo.SetColumnWidth(4, 68);
	m_gridPositionInfo.SetColumnWidth(5, 81);
	m_gridPositionInfo.SetColumnWidth(6, 81+18);

	m_gridPositionInfo.SetItemText(0, 0, "合约");
	m_gridPositionInfo.SetItemText(0, 1, "买卖");
	m_gridPositionInfo.SetItemText(0, 2, "持仓");
	m_gridPositionInfo.SetItemText(0, 3, "可平昨量");
	m_gridPositionInfo.SetItemText(0, 4, "可平今量");
	m_gridPositionInfo.SetItemText(0, 5, "持仓均价");
	m_gridPositionInfo.SetItemText(0, 6, "持仓盈亏");
	m_gridPositionInfo.Invalidate();

	m_gridPrice.SetDefCellHeight(20);
	m_gridPrice.SetDefCellMargin(3);
	m_gridPrice.SetEditable(FALSE);
	m_gridPrice.EnableSelection(FALSE);
	m_gridPrice.SetFixedColumnSelection(FALSE);
	m_gridPrice.EnableDragAndDrop(FALSE);
	m_gridPrice.SetColumnResize(FALSE);
	m_gridPrice.SetHeaderSort(FALSE);
	m_gridPrice.SetTrackFocusCell(FALSE);
	m_gridPrice.SetBkColor(RGB(255, 255, 255));
	
	m_gridPrice.SetColumnCount(5);
	m_gridPrice.SetRowCount(20);
	m_gridPrice.SetFixedRowCount(1);
	m_gridPrice.SetItemText(0, conColBuyVolumeWait, "挂单买入量");
	m_gridPrice.SetItemText(0, conColBidVolume, "申买量");
	m_gridPrice.SetItemText(0, conColPrice, "价格");
	m_gridPrice.SetItemText(0, conColAskVolume, "申卖量");
	m_gridPrice.SetItemText(0, conColSellVolumeWait, "挂单卖出量");
	m_gridPrice.ExpandColumnsToFit();
	m_gridPrice.Invalidate();

	m_SpinQtyLeft.SetRange(1, 9999);
	m_SpinQtyRight.SetRange(1, 9999);

	QueryCurrUserID();
	QueryCurrAccountID();
	QueryConfig_InstrumentIDInputPanelShow();
	QueryConfig_QtyInputPanelShow();
	QueryConfig_QtyInputPanelQtyDatas();

	CRect rectWnd, rectClient;
	CRect gridRectWnd, gridRectClient;
	GetWindowRect(&rectWnd);
	GetClientRect(&rectClient);
	m_gridPrice.GetWindowRect(&gridRectWnd);
	m_gridPrice.GetClientRect(&gridRectClient);
	ScreenToClient(&gridRectWnd);

	m_nDlgMaxWidth = 460+rectWnd.Width()-rectClient.Width()+gridRectWnd.left*2;
	m_ndlgMaxHeight = m_gridPrice.GetFixedRowHeight() + m_gridPrice.GetRowHeight(1)*7
			+ gridRectWnd.top + 70
			+ gridRectWnd.Height()-gridRectClient.Height()+rectWnd.Height()-rectClient.Height();

	m_bookOrderDlgColor = GetBookOrderDlgColor();
	ResetDlgColor(m_bookOrderDlgColor.dwDlgBackColor, 
				m_bookOrderDlgColor.dwDlgButtonBackColor, 
				m_bookOrderDlgColor.dwDlgEditBackColor);
	ResetAccountGridColor(m_bookOrderDlgColor.tAccount);
	ResetPositionGridColor(m_bookOrderDlgColor.tPosition);
	ResetPriceGridColor(m_bookOrderDlgColor.tPrice);

	SetTimer(HIDEMETIMER, 50, NULL);
	SetTimer(QUERYMETIMER, 500, NULL);

	static int nShowCount = 0;

	SIZE size = GetBookDlgSize();
	if(size.cx>=m_nDlgMaxWidth && size.cy>=m_ndlgMaxHeight)
	{
		SetWindowPos(NULL, (nShowCount%10)*32, (nShowCount%10)*24, size.cx, size.cy, SWP_HIDEWINDOW);
	}
	else
	{
		SetWindowPos(NULL, (nShowCount%10)*32, (nShowCount%10)*24, 0, 0, SWP_NOSIZE|SWP_HIDEWINDOW);
	}
	nShowCount++;

	// 在Title上创建控件，有个方法就是通过EnumWindow来查找所有的子窗体
	// 找到属性为WS_CAPTION的子窗体即可。

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBookOrderDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent==HIDEMETIMER)
	{
		if(m_pdlgInstrumentIDList!=NULL)
		{
			CRect rect;
			CPoint point;
			m_pdlgInstrumentIDList->GetWindowRect(&rect);
			rect.left -= 5;
			rect.top -= 23;
			rect.bottom += 5;
			rect.right += 5;
			::GetCursorPos(&point);
			if(!rect.PtInRect(point))
				m_pdlgInstrumentIDList->ShowWindow(SW_HIDE);
		}
		if(m_pdlgQtyList!=NULL)
		{
			CRect rect;
			CPoint point;
			m_pdlgQtyList->GetWindowRect(&rect);
			rect.left -= 5;
			rect.top -= 23;
			rect.bottom += 5;
			rect.right += 5;
			::GetCursorPos(&point);
			if(!rect.PtInRect(point))
				m_pdlgQtyList->ShowWindow(SW_HIDE);
		}
	}
	else if(nIDEvent==TIMEOUTTIMER)
	{
		KillTimer(TIMEOUTTIMER);
		if(!m_tPriceGridIndex.bHasData)
			::MessageBox(NULL, "行情初始化超时，请手动刷新价格", "警告", MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL);
	}
	else if(nIDEvent==QUERYMETIMER)
	{
		QueryCurrAccountInfo(m_strCurrSubAccount);
		QueryOrderWaitMaxMinPrice(GetInstrumentID());
		
		DWORD dwCurrTime = ::GetTickCount();
		std::queue<std::string> queueDel;
		std::map<std::string, DWORD>::iterator it = m_mapPriceCancel.begin();
		for(; it!=m_mapPriceCancel.end(); it++)
		{
			if(dwCurrTime-it->second > 2000)
				queueDel.push(it->first);
		}
		while(!queueDel.empty())
		{
			m_mapPriceCancel.erase(queueDel.front());
			queueDel.pop();
		}
	}
	else if(nIDEvent==COLORGRADIENTTIMER)
	{
		m_nColorGradientIndex--;
		ShowPriceGrid(m_CheckFixPrice.GetCheck());
		if(m_nColorGradientIndex<0)
			KillTimer(COLORGRADIENTTIMER);
	}

	CDialog::OnTimer(nIDEvent);
}

void CBookOrderDialog::OnBnClickedClose()
{
	// TODO: Add your control notification handler code here
	UnsubscribeQuot(GetInstrumentID());
	KillTimer(HIDEMETIMER);
	KillTimer(QUERYMETIMER);
	KillTimer(COLORGRADIENTTIMER);
	KillTimer(TIMEOUTTIMER);
	m_bExiting = true;
	if(m_pdlgInstrumentIDList!=NULL)
	{
		m_pdlgInstrumentIDList->ShowWindow(SW_HIDE);
	}
	if(m_pdlgQtyList!=NULL)
	{
		m_pdlgQtyList->ShowWindow(SW_HIDE);
	}

	::PostMessage(::AfxGetMainWnd()->GetSafeHwnd(), 
			GetBookOrderCloseMSGID(), 0, (LPARAM)this);
}

void CBookOrderDialog::OnBnClickedReflashprice()
{
	// TODO: Add your control notification handler code here
	InitPriceGrid(m_CheckFixPrice.GetCheck());
	ShowPriceGrid(m_CheckFixPrice.GetCheck());
	if(m_CheckFixPrice.GetCheck())
		ShowLastPrice2GridCenter();
}

void CBookOrderDialog::OnGridLeftClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if(!m_CheckAllowOrder.GetCheck())
		return;

	int nAction = m_comboOCLeft.GetCurSel();
	if(nAction==0)
		return;

	BOOL bDirection = (pItem->iColumn==conColBidVolume || pItem->iColumn==conColBuyVolumeWait);
	BOOL bIsChangeOrder = m_CheckChangeOrder.GetCheck();
	int nVolume = 0;
	char strQty[64];
	ZeroMemory(strQty, sizeof(strQty));
	m_editQtyLeft.GetLine(0, strQty, 63);
	nVolume = atoi(strQty);

	if(nVolume<=0 && nVolume>9999)
	{
		::MessageBox(NULL, "鼠标左键手数错误(需>0,<=9999)", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		return;
	}

	double fLimitPrice = m_tPriceGridIndex.fTopPrice 
			- m_tPriceGridIndex.fPriceTick * (pItem->iRow-1);
	std::string strPrice(Price2String(fLimitPrice, m_tPriceGridIndex.fPriceTick));
	std::map<std::string, BIDASKVOLUME>::iterator it;
	int nWaitVolume = 0;

	switch(pItem->iColumn)
	{
	case conColBidVolume:
	case conColAskVolume:
		if(bIsChangeOrder)
			ChangeOrder(GetInstrumentID(), bDirection, nAction, fLimitPrice, nVolume);
		else
			NewOrder(GetInstrumentID(), bDirection, nAction, fLimitPrice, nVolume);
		break;
	case conColBuyVolumeWait:
	{
		if(m_mapPriceCancel.find(strPrice)!=m_mapPriceCancel.end())
			return;
		it = m_mapOrderWaitVolume.find(strPrice);
		if(it != m_mapOrderWaitVolume.end())
		{
			nWaitVolume = it->second.nBuy;
			if(nWaitVolume>0)
			{
				m_mapPriceCancel[strPrice] = ::GetTickCount();
				CancelOrder(GetInstrumentID(), bDirection, fLimitPrice);
			}
		}
	}
		break;
	case conColSellVolumeWait:
	{
		if(m_mapPriceCancel.find(strPrice)!=m_mapPriceCancel.end())
			return;
		it = m_mapOrderWaitVolume.find(strPrice);
		if(it != m_mapOrderWaitVolume.end())
		{
			nWaitVolume = it->second.nSell;
			if(nWaitVolume>0)
			{
				m_mapPriceCancel[strPrice] = ::GetTickCount();
				CancelOrder(GetInstrumentID(), bDirection, fLimitPrice);
			}
		}
	}
		break;
	};
}

void CBookOrderDialog::OnGridRightClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if(!m_CheckAllowOrder.GetCheck())
		return;

	int nAction = m_comboOCRight.GetCurSel();
	if(nAction==0)
		return;

	BOOL bDirection = (pItem->iColumn==conColBidVolume || pItem->iColumn==conColBuyVolumeWait);
	BOOL bIsChangeOrder = m_CheckChangeOrder.GetCheck();
	int nVolume = 0;
	char strQty[64];
	ZeroMemory(strQty, sizeof(strQty));
	m_editQtyRight.GetLine(0, strQty, 63);
	nVolume = atoi(strQty);

	if(nVolume<=0 && nVolume>9999)
	{
		::MessageBox(NULL, "鼠标左键手数错误(需>0,<=9999)", "错误提示", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		return;
	}

	double fLimitPrice = m_tPriceGridIndex.fTopPrice 
			- m_tPriceGridIndex.fPriceTick * (pItem->iRow-1);
	std::string strPrice(Price2String(fLimitPrice, m_tPriceGridIndex.fPriceTick));
	std::map<std::string, BIDASKVOLUME>::iterator it;
	int nWaitVolume = 0;

	switch(pItem->iColumn)
	{
	case conColBidVolume:
	case conColAskVolume:
		if(bIsChangeOrder)
			ChangeOrder(GetInstrumentID(), bDirection, nAction, fLimitPrice, nVolume);
		else
			NewOrder(GetInstrumentID(), bDirection, nAction, fLimitPrice, nVolume);
		break;
	case conColBuyVolumeWait:
	{
		if(m_mapPriceCancel.find(strPrice)!=m_mapPriceCancel.end())
			return;
		it = m_mapOrderWaitVolume.find(strPrice);
		if(it != m_mapOrderWaitVolume.end())
		{
			nWaitVolume = it->second.nBuy;
			if(nWaitVolume>0)
			{
				m_mapPriceCancel[strPrice] = ::GetTickCount();
				CancelOrder(GetInstrumentID(), bDirection, fLimitPrice);
			}
		}
	}
		break;
	case conColSellVolumeWait:
	{
		if(m_mapPriceCancel.find(strPrice)!=m_mapPriceCancel.end())
			return;
		it = m_mapOrderWaitVolume.find(strPrice);
		if(it != m_mapOrderWaitVolume.end())
		{
			nWaitVolume = it->second.nSell;
			if(nWaitVolume>0)
			{
				m_mapPriceCancel[strPrice] = ::GetTickCount();
				CancelOrder(GetInstrumentID(), bDirection, fLimitPrice);
			}
		}
	}
		break;
	};
}

void CBookOrderDialog::OnGridLeftDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if(!m_CheckAllowOrder.GetCheck())
		return;

	switch(pItem->iColumn)
	{
	case conColBidVolume:
		break;
	case conColAskVolume:
		break;
	case conColBuyVolumeWait:
	case conColSellVolumeWait:
		break;
	};
}

void CBookOrderDialog::OnDeltaposSpinQtyLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	int nVolume = 0;
	char strQty[64];
	ZeroMemory(strQty, sizeof(strQty));
	m_editQtyLeft.GetLine(0, strQty, 63);
	nVolume = atoi(strQty);

	nVolume += pNMUpDown->iDelta;
	if(nVolume<=0)
		nVolume = 1;
	if(nVolume>9999)
		nVolume = 9999;
	sprintf(strQty, "%d", nVolume);
	m_editQtyLeft.SetSel(0, -1);
	m_editQtyLeft.ReplaceSel(strQty);

	*pResult = 0;
}

void CBookOrderDialog::OnDeltaposSpinQtyRight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nVolume = 0;
	char strQty[64];
	ZeroMemory(strQty, sizeof(strQty));
	m_editQtyRight.GetLine(0, strQty, 63);
	nVolume = atoi(strQty);

	nVolume += pNMUpDown->iDelta;
	if(nVolume<=0)
		nVolume = 1;
	if(nVolume>9999)
		nVolume = 9999;
	sprintf(strQty, "%d", nVolume);
	m_editQtyRight.SetSel(0, -1);
	m_editQtyRight.ReplaceSel(strQty);

	*pResult = 0;
}

void CBookOrderDialog::OnBnClickedCancelall()
{
	// TODO: Add your control notification handler code here
	QueryOrderWaits();
//	if(::AfxMessageBox("即将进行全部撤单操作，请您确认？", MB_YESNO|MB_ICONQUESTION)==IDYES)
//		CancelAllOrder();
}

void CBookOrderDialog::OnTextLeftClicked_InstrmentID()
{
	if(m_pdlgInstrumentIDList && m_bNeedInstrumentIDInputPanel)
	{
		CRect rect, rectParent;
		m_EditInstrumentID.GetWindowRect(&rectParent);
		//m_EditInstrumentID.ClientToScreen(&rectParent);
		m_pdlgInstrumentIDList->GetWindowRect(&rect);
		rect.MoveToXY(rectParent.left, rectParent.bottom);
		m_pdlgInstrumentIDList->ShowView(CString(""), TRUE);
		m_pdlgInstrumentIDList->MoveWindow(rect);
		m_pdlgInstrumentIDList->SetOutputEditInstrumentID(&m_EditInstrumentID);
		m_pdlgInstrumentIDList->SetWindowPos(NULL, 0, 0, 0, 0, 
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW);
		::SetWindowLong(m_pdlgInstrumentIDList->GetSafeHwnd(), 
				GWL_HWNDPARENT, (long)GetSafeHwnd());
	}
}

void CBookOrderDialog::OnTextKeyDown_InstrmentID(UINT nKeyNum)
{
	if(nKeyNum==VK_DOWN && m_pdlgInstrumentIDList->IsWindowVisible())
	{
		m_pdlgInstrumentIDList->SetFocus();
	}
}

void CBookOrderDialog::OnTextLeftClicked_QtyLeft()
{
	if(m_pdlgQtyList && m_bNeedQtyInputPanel)
	{
		CRect rect, rectParent;
		m_editQtyLeft.GetWindowRect(&rectParent);
		//m_editQtyLeft.ClientToScreen(&rectParent);
		m_pdlgQtyList->GetWindowRect(&rect);
		rect.MoveToXY(rectParent.left, rectParent.bottom);
		m_pdlgQtyList->MoveWindow(rect);
		m_pdlgQtyList->SetOutputEditQty(&m_editQtyLeft);
		m_pdlgQtyList->ShowWindow(SW_SHOW);
		::SetWindowLong(m_pdlgQtyList->GetSafeHwnd(), 
				GWL_HWNDPARENT, (long)GetSafeHwnd());
		m_editQtyLeft.SetFocus();
	}
}

void CBookOrderDialog::OnTextLeftClicked_QtyRight()
{
	if(m_pdlgQtyList && m_bNeedQtyInputPanel)
	{
		CRect rect, rectParent;
		m_editQtyRight.GetWindowRect(&rectParent);
		//m_editQtyRight.ClientToScreen(&rectParent);
		m_pdlgQtyList->GetWindowRect(&rect);
		rect.MoveToXY(rectParent.left, rectParent.bottom);
		m_pdlgQtyList->MoveWindow(rect);
		m_pdlgQtyList->SetOutputEditQty(&m_editQtyRight);
		m_pdlgQtyList->ShowWindow(SW_SHOW);
		::SetWindowLong(m_pdlgQtyList->GetSafeHwnd(), 
				GWL_HWNDPARENT, (long)GetSafeHwnd());
		m_editQtyRight.SetFocus();
	}
}

void CBookOrderDialog::OnEnChangeEditInstrumentid()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	m_tWaitOrderPrice.dHighPrice = util::GetDoubleInvalidValue();
	m_tWaitOrderPrice.dLowPrice = util::GetDoubleInvalidValue();
	m_tOldWaitOrderPrice.dHighPrice = util::GetDoubleInvalidValue();
	m_tOldWaitOrderPrice.dLowPrice = util::GetDoubleInvalidValue();

	m_mapPriceCancel.clear();

	if(!m_bUserChangeInstrumentID)
		return;
	CString strItem;
	char strInstrumentID[64];
	ZeroMemory(strInstrumentID, sizeof(strInstrumentID));
	m_EditInstrumentID.GetLine(0, strInstrumentID, sizeof(strInstrumentID)-1);
	strItem.Format("一键下单 -- %s", strInstrumentID);
	SetTilte(strItem);
	m_mapOrderWaitVolume.clear();
	QueryInstrumentInfo(CString(strInstrumentID));
	SubscribeQuot(CString(strInstrumentID));
	QueryQuotSnapshot(CString(strInstrumentID));
	UnsubscribeQuot(m_strOldInstrumentID);
	if(m_pdlgInstrumentIDList)
	{
		CRect rect, rectParent;
		m_EditInstrumentID.GetWindowRect(&rectParent);
		//m_EditInstrumentID.ClientToScreen(&rectParent);
		m_pdlgInstrumentIDList->GetWindowRect(&rect);
		rect.MoveToXY(rectParent.left, rectParent.bottom);
		m_pdlgInstrumentIDList->ShowView(CString(strInstrumentID), FALSE);
		m_pdlgInstrumentIDList->MoveWindow(rect);
		m_pdlgInstrumentIDList->SetOutputEditInstrumentID(&m_EditInstrumentID);
		m_pdlgInstrumentIDList->SetWindowPos(NULL, 0, 0, 0, 0, 
				SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_SHOWWINDOW);
		::SetWindowLong(m_pdlgInstrumentIDList->GetSafeHwnd(), 
				GWL_HWNDPARENT, (long)GetSafeHwnd());
	}
	m_strOldInstrumentID = strInstrumentID;

	m_tPriceGridIndex.bHasData = FALSE;
	ShowPriceGrid(m_CheckFixPrice.GetCheck());

	KillTimer(TIMEOUTTIMER);
	std::set<std::string>& setIDs = GetInstrumentIDList();
	if(setIDs.find(strInstrumentID)!=setIDs.end())
	{
		SetTimer(TIMEOUTTIMER, 3000, NULL);
	}
}

void CBookOrderDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}

void CBookOrderDialog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	OnBnClickedClose();
	//CDialog::OnCancel();
}

void CBookOrderDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	CRect rect;
	CWnd* pWnd = NULL;
	
	{
		if(m_pDlgBrushBk!=NULL)
		{
			CRgn rgnClt, rgnCmb;
			CRect rect;
			GetClientRect(&rect);
			rgnClt.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);

			CRgn rgn1, rgn2, rgn3;

			m_gridAccountInfo.GetWindowRect(&rect);
			ScreenToClient(&rect);
			rgn1.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);

			m_gridPositionInfo.GetWindowRect(&rect);
			ScreenToClient(&rect);
			rgn2.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);

			rgn3.CreateRectRgn(0, 0, 0, 0);
			pWnd = GetDlgItem(IDC_FRAME_ORDER);
			if(pWnd != NULL)
			{
				pWnd->GetWindowRect(&rect);
				ScreenToClient(&rect);
				rgn3.SetRectRgn(&rect);
			}
			rgnCmb.CreateRectRgn(0, 0, 0, 0);
			rgnCmb.CombineRgn(&rgn1, &rgn2, RGN_OR);
			rgnCmb.CombineRgn(&rgnCmb, &rgn3, RGN_OR);
			rgnCmb.CombineRgn(&rgnClt, &rgnCmb, RGN_DIFF);
			
			dc.FillRgn(&rgnCmb, m_pDlgBrushBk);
		}
	}
	pWnd = GetDlgItem(IDC_FRAME_ORDER);
	if(pWnd != NULL)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		CBrush brush;
		brush.CreateSolidBrush(RGB(192, 192, 192));
		dc.FrameRect(&rect, &brush);
		brush.DeleteObject();
	}
	pWnd = GetDlgItem(IDC_MOUSELEFT);
	if(pWnd)
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		dc.DrawIcon(rect.left, rect.top, m_hiconMouseLeft);
	}
	pWnd = GetDlgItem(IDC_MOUSERIGHT);
	if(pWnd)
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		dc.DrawIcon(rect.left, rect.top, m_hiconMouseRight);
	}
}

void CBookOrderDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(nFlags&MK_LBUTTON) {
		ReleaseCapture();
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}

	CBookOrderOperations::OnMouseMove(nFlags, point);
}

void CBookOrderDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
	PostMessage(WM_COMMAND, MAKEWPARAM(ID_CLOSE, BN_CLICKED), NULL);

	//CBookOrderOperations::OnClose();
}

void CBookOrderDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	ResetDlgSize(cx, cy);
	Invalidate();
	RedrawDlgItem();
}

void CBookOrderDialog::ResetDlgSize(int cx, int cy)
{
	// TODO: Add your message handler code here

	if(!IsWindowVisible())
		return;

	CRect rect, rectClient;
	CRect gridRect, gridRectClient;
	CRect rectMe;

	GetWindowRect(&rect);
	GetClientRect(&rectClient);
	m_gridPrice.GetWindowRect(&gridRect);
	m_gridPrice.GetClientRect(&gridRectClient);
	ScreenToClient(&gridRect);

	int nMinGridHeight = m_gridPrice.GetFixedRowHeight() + m_gridPrice.GetRowHeight(1)*7 +4;
	int nMinGridWidth = 460;//518-4-20;//462;
	int nSysSideHeight = gridRect.Height()-gridRectClient.Height();
	int nMinWidth = nMinGridWidth + 2*gridRect.left + rect.Width()-rectClient.Width();
	int nMinHeight = rect.top + nMinGridHeight + nSysSideHeight + 40;

	int nGridWidth = cx - 2*gridRect.left;
	nGridWidth = nGridWidth>nMinGridWidth ? nGridWidth : nMinGridWidth;
	int nGridHeight = cy - gridRect.top - 70;
	nGridHeight = nGridHeight>nMinGridHeight ? nGridHeight : nMinGridHeight;

	int nRealGridHeight = nGridHeight - nSysSideHeight 
			- m_gridPrice.GetFixedRowHeight();
	nRealGridHeight = nRealGridHeight / m_gridPrice.GetRowHeight(1);
	nRealGridHeight = nSysSideHeight + m_gridPrice.GetFixedRowHeight()
			+ nRealGridHeight*m_gridPrice.GetRowHeight(1);

	//::SetWindowPos(m_gridPrice.GetSafeHwnd(), HWND_TOP, gridRect.left, 
	//		gridRect.top, nGridWidth, nGridHeight, SWP_NOMOVE|SWP_NOREDRAW);
	::SetWindowPos(m_gridPrice.GetSafeHwnd(), HWND_TOP, gridRect.left, 
			gridRect.top, nGridWidth, nRealGridHeight, SWP_NOMOVE);
	//m_gridPrice.Invalidate();
	//m_gridPrice.MoveWindow(gridRect.left, 
	//		gridRect.top, nGridWidth, nGridHeight);

	m_gridAccountInfo.GetWindowRect(&rectMe);
	ScreenToClient(&rectMe);
	::SetWindowPos(m_gridAccountInfo.GetSafeHwnd(), HWND_TOP, rectMe.left, 
			rectMe.top, rectClient.Width()-2*rectMe.left, rectMe.Height(), SWP_NOMOVE);
	
	m_gridPositionInfo.GetWindowRect(&rectMe);
	ScreenToClient(&rectMe);
	::SetWindowPos(m_gridPositionInfo.GetSafeHwnd(), HWND_TOP, rectMe.left, 
			rectMe.top, rectClient.Width()-2*rectMe.left, rectMe.Height(), SWP_NOMOVE);
	m_gridPositionInfo.SetColumnWidth(m_gridPositionInfo.GetColumnCount()-1, 1);
	m_gridPositionInfo.ExpandLastColumn();

	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_FRAME_ORDER);
	if(pWnd != NULL)
	{
		pWnd->GetWindowRect(&rectMe);
		ScreenToClient(&rectMe);
		::SetWindowPos(pWnd->GetSafeHwnd(), HWND_TOP, rectMe.left, 
				rectMe.top, rectClient.Width()-2*rectMe.left, 
				rectClient.Height()-rectMe.top-rectMe.left, SWP_NOMOVE);
	}

	int nNextX = 0, nNextY = 0;
	{
		m_CheckToMostTop.GetWindowRect(&rectMe);
		ScreenToClient(&rectMe);
		nNextY = rectMe.top;
		nNextX = rectClient.Width()-rectMe.Width()-7-25;
		::SetWindowPos(m_CheckToMostTop.GetSafeHwnd(), HWND_TOP, 
				nNextX, nNextY, 0, 0, SWP_NOSIZE);
	}

	{
		m_btnColor1.GetWindowRect(&rectMe);
		ScreenToClient(&rectMe);
		nNextY = rectMe.top;
		nNextX -= rectMe.Width()+10;
		::SetWindowPos(m_btnColor1.GetSafeHwnd(), HWND_TOP, 
				nNextX, nNextY, 0, 0, SWP_NOSIZE);
	}
	{
		m_btnColor2.GetWindowRect(&rectMe);
		ScreenToClient(&rectMe);
		nNextY = rectMe.top;
		nNextX -= rectMe.Width()+3;
		::SetWindowPos(m_btnColor2.GetSafeHwnd(), HWND_TOP, 
				nNextX, nNextY, 0, 0, SWP_NOSIZE);
	}
	{
		m_btnColor3.GetWindowRect(&rectMe);
		ScreenToClient(&rectMe);
		nNextY = rectMe.top;
		nNextX -= rectMe.Width()+3;
		::SetWindowPos(m_btnColor3.GetSafeHwnd(), HWND_TOP, 
				nNextX, nNextY, 0, 0, SWP_NOSIZE);
	}
	{
		m_btnColor4.GetWindowRect(&rectMe);
		ScreenToClient(&rectMe);
		nNextY = rectMe.top;
		nNextX -= rectMe.Width()+3;
		::SetWindowPos(m_btnColor4.GetSafeHwnd(), HWND_TOP, 
				nNextX, nNextY, 0, 0, SWP_NOSIZE);
	}

	pWnd = GetDlgItem(IDC_CHECK_FIXPRICE);
	if(pWnd!=NULL)
	{
		//nNextY = rectClient.Height()-2*20+4;
		nNextY = rectClient.Height()-1*20-35;
		::SetWindowPos(pWnd->GetSafeHwnd(), HWND_TOP, gridRect.left, 
				nNextY, 0, 0, SWP_NOSIZE);
	}
//	pWnd = GetDlgItem(IDC_CHECK_CHANGEORDER);
//	if(pWnd!=NULL)
//	{
//		nNextY += 20;
//		::SetWindowPos(pWnd->GetSafeHwnd(), HWND_TOP, gridRect.left, 
//				nNextY, 0, 0, SWP_NOSIZE);
//	}
	pWnd = GetDlgItem(ID_CLOSE);
	if(pWnd!=NULL)
	{
		pWnd->GetWindowRect(&rectMe);
		nNextY = rectClient.Height()-rectMe.Height()-25;
		nNextX = rectClient.Width()-rectMe.Width()-gridRect.left-10;
		::SetWindowPos(pWnd->GetSafeHwnd(), HWND_TOP, nNextX, 
			nNextY, 0, 0, SWP_NOSIZE);
	}
	pWnd = GetDlgItem(ID_CANCELALL);
	if(pWnd!=NULL)
	{
		CRect rectMe;
		pWnd->GetWindowRect(&rectMe);
		nNextX -= (rectMe.Width()+10);
		::SetWindowPos(pWnd->GetSafeHwnd(), HWND_TOP, nNextX, 
			nNextY, 0, 0, SWP_NOSIZE);
	}
	pWnd = GetDlgItem(ID_REFLASHPRICE);
	if(pWnd!=NULL)
	{
		CRect rectMe;
		pWnd->GetWindowRect(&rectMe);
		nNextX -= (rectMe.Width()+10);
		::SetWindowPos(pWnd->GetSafeHwnd(), HWND_TOP, nNextX, 
			nNextY, 0, 0, SWP_NOSIZE);
	}

	InitPriceGrid(m_CheckFixPrice.GetCheck());
	ShowPriceGrid(m_CheckFixPrice.GetCheck(), FALSE);
	for(int i=0; i<m_gridPrice.GetColumnCount(); i++)
		m_gridPrice.SetColumnWidth(i, 1);
	m_gridPrice.ExpandColumnsToFit(TRUE);
	//m_gridPrice.Invalidate();
	QueryOrderWaitVolumeList(CString(m_tDepthMarketData.InstrumentID), THOST_FTDC_D_Buy, TRUE);
	QueryOrderWaitVolumeList(CString(m_tDepthMarketData.InstrumentID), THOST_FTDC_D_Sell, FALSE);
}

void CBookOrderDialog::RedrawDlgItem()
{
	// TODO: Add your message handler code here

	if(!IsWindowVisible())
		return;

	CWnd* pWnd = NULL;

	m_CheckToMostTop.Invalidate(FALSE);
	m_btnColor1.Invalidate(FALSE);
	m_btnColor2.Invalidate(FALSE);
	m_btnColor3.Invalidate(FALSE);
	m_btnColor4.Invalidate(FALSE);

	pWnd = GetDlgItem(IDC_CHECK_ALLOW_ORDER);
	if(pWnd!=NULL)
	{
		pWnd->Invalidate(FALSE);
	}
	pWnd = GetDlgItem(IDC_CHECK_FIXPRICE);
	if(pWnd!=NULL)
	{
		pWnd->Invalidate(FALSE);
	}
//	pWnd = GetDlgItem(IDC_CHECK_CHANGEORDER);
//	if(pWnd!=NULL)
//	{
//		pWnd->Invalidate(FALSE);
//	}
	pWnd = GetDlgItem(ID_CLOSE);
	if(pWnd!=NULL)
	{
		pWnd->Invalidate(FALSE);
	}
	pWnd = GetDlgItem(ID_CANCELALL);
	if(pWnd!=NULL)
	{
		pWnd->Invalidate(FALSE);
	}
	pWnd = GetDlgItem(ID_REFLASHPRICE);
	if(pWnd!=NULL)
	{
		pWnd->Invalidate(FALSE);
	}
}

void CBookOrderDialog::OnNcBtnColor1Clicked()
{
	GetColorStyle(1, m_bookOrderDlgColor);
	SetBookOrderDlgColor(m_bookOrderDlgColor, 1);

	ResetDlgColor(m_bookOrderDlgColor.dwDlgBackColor, 
				m_bookOrderDlgColor.dwDlgButtonBackColor, 
				m_bookOrderDlgColor.dwDlgEditBackColor);
	Invalidate();
	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_DRAWFRAME);
	ResetAccountGridColor(m_bookOrderDlgColor.tAccount);
	ResetPositionGridColor(m_bookOrderDlgColor.tPosition);
	ResetPriceGridColor(m_bookOrderDlgColor.tPrice);
	ShowPositionInfos();
	ShowPriceGrid(m_CheckFixPrice.GetCheck());
}

void CBookOrderDialog::OnNcBtnColor2Clicked()
{
	GetColorStyle(2, m_bookOrderDlgColor);
	SetBookOrderDlgColor(m_bookOrderDlgColor, 2);

	ResetDlgColor(m_bookOrderDlgColor.dwDlgBackColor, 
				m_bookOrderDlgColor.dwDlgButtonBackColor, 
				m_bookOrderDlgColor.dwDlgEditBackColor);
	Invalidate();
	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_DRAWFRAME);
	ResetAccountGridColor(m_bookOrderDlgColor.tAccount);
	ResetPositionGridColor(m_bookOrderDlgColor.tPosition);
	ResetPriceGridColor(m_bookOrderDlgColor.tPrice);
	ShowPositionInfos();
	ShowPriceGrid(m_CheckFixPrice.GetCheck());
}

void CBookOrderDialog::OnNcBtnColor3Clicked()
{
	GetColorStyle(3, m_bookOrderDlgColor);
	SetBookOrderDlgColor(m_bookOrderDlgColor, 3);

	ResetDlgColor(m_bookOrderDlgColor.dwDlgBackColor, 
				m_bookOrderDlgColor.dwDlgButtonBackColor, 
				m_bookOrderDlgColor.dwDlgEditBackColor);
	Invalidate();
	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_DRAWFRAME);
	ResetAccountGridColor(m_bookOrderDlgColor.tAccount);
	ResetPositionGridColor(m_bookOrderDlgColor.tPosition);
	ResetPriceGridColor(m_bookOrderDlgColor.tPrice);
	ShowPositionInfos();
	ShowPriceGrid(m_CheckFixPrice.GetCheck());
}

void CBookOrderDialog::OnNcBtnColor4Clicked()
{
	GetColorStyle(4, m_bookOrderDlgColor);
	SetBookOrderDlgColor(m_bookOrderDlgColor, 4);

	ResetDlgColor(m_bookOrderDlgColor.dwDlgBackColor, 
				m_bookOrderDlgColor.dwDlgButtonBackColor, 
				m_bookOrderDlgColor.dwDlgEditBackColor);
	Invalidate();
	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_DRAWFRAME);
	ResetAccountGridColor(m_bookOrderDlgColor.tAccount);
	ResetPositionGridColor(m_bookOrderDlgColor.tPosition);
	ResetPriceGridColor(m_bookOrderDlgColor.tPrice);
	ShowPositionInfos();
	ShowPriceGrid(m_CheckFixPrice.GetCheck());
}

void CBookOrderDialog::OnBnClickedCheckFixprice()
{
	// TODO: Add your control notification handler code here
	//InitPriceGrid(m_CheckFixPrice.GetCheck());
	//ShowPriceGrid(m_CheckFixPrice.GetCheck());
	//m_gridPrice.ExpandColumnsToFit();
	OnBnClickedReflashprice();
	for(int i=0; i<m_gridPrice.GetColumnCount(); i++)
		m_gridPrice.SetColumnWidth(i, 1);
	m_gridPrice.ExpandColumnsToFit(TRUE);
	m_tOldWaitOrderPrice.dHighPrice = util::GetDoubleInvalidValue();
	m_tOldWaitOrderPrice.dLowPrice = util::GetDoubleInvalidValue();
}

void CBookOrderDialog::OnBnClickedCheckTomosttop()
{
	// TODO: Add your control notification handler code here
	if(m_CheckToMostTop.GetCheck())
	{
		::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, 
				SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW); 
	}
	else
	{
		::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, 
				SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW); 
	}
}

void CBookOrderDialog::OnBnClickedCheckAllowOrder()
{
	// TODO: Add your control notification handler code here
	
	if(!GetIsConfirmRaskInfo() && m_CheckAllowOrder.GetCheck())
	{
		MessageBox("一键下单功能已激活，在相关区域的点击会直接生成报单或撤单，\n"
				   "不会再弹出确认提示。请确认您已了解相关使用说明及风险揭示。\n"
				   "如有疑问，请联系客服\n", "一键下单 提示", 
				   MB_OK|MB_ICONWARNING|MB_APPLMODAL);
		SetIsConfirmRaskInfo(TRUE);
	}
}

void CBookOrderDialog::DrawRect(CDC* pDc, CRect& rect, CPen& pen)
{
	CPen* pOldPen = pDc->SelectObject(&pen);
	pDc->MoveTo(rect.left, rect.top);
	pDc->LineTo(rect.left, rect.bottom);
	pDc->MoveTo(rect.left, rect.bottom);
	pDc->LineTo(rect.right, rect.bottom);
	pDc->MoveTo(rect.right, rect.bottom);
	pDc->LineTo(rect.right, rect.top);
	pDc->MoveTo(rect.right, rect.top);
	pDc->LineTo(rect.left, rect.top);
	pDc->SelectObject(pOldPen);
}

void CBookOrderDialog::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CBookOrderOperations::OnNcPaint() for painting messages
	CDC* pWinDC=GetWindowDC();
	if(pWinDC) 
	{
		CRect rect, rectClt;
		GetClientRect(&rectClt);
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.OffsetRect(CPoint(-rect.left, -rect.top));
		rect.DeflateRect(0, 0, 1, 1);
		CPen penCurr;
		penCurr.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
		DrawRect(pWinDC, rect, penCurr);
		penCurr.DeleteObject();
		penCurr.CreatePen(PS_SOLID, 1, m_bookOrderDlgColor.dwDlgBackColor);
		for(int i=0; i<(rect.Width()+rectClt.Width()+2)/2; i++)
		{
			rect.DeflateRect(1, 1, 1, 1);
			DrawRect(pWinDC, rect, penCurr);
		}
		penCurr.DeleteObject();
		ReleaseDC(pWinDC);
	}
}

