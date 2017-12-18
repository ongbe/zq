// FeedbackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "FeedbackDlg.h"
#include "AlgoAutoOrderDlg.h"


// CFeedbackDlg dialog

IMPLEMENT_DYNAMIC(CFeedbackDlg, CDialog)

CFeedbackDlg::CFeedbackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeedbackDlg::IDD, pParent)
    , m_pAlgoAutoOrderDlg(NULL)
    , m_NewVolume(0)
    , m_MarketOrder(0)
{
    
}

CFeedbackDlg::~CFeedbackDlg()
{
}

void CFeedbackDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CUSTOMTIME, m_customTime);
    DDX_Control(pDX, IDC_CUSTOMCOUNT, m_customCount);
    DDX_Control(pDX, IDC_STATIC_CONTRACT, m_staticContract);
    DDX_Control(pDX, IDC_STATIC_DIRECTION, m_staticDirection);
    DDX_Control(pDX, IDC_STATIC_COUNT, m_staticCount);
    DDX_Control(pDX, IDC_STATIC_COUNTTOTAL, m_staticCountTotal);
    DDX_Control(pDX, IDC_STATIC_PRICELIMIT, m_staticPriceLimit);
    DDX_Control(pDX, IDC_STATIC_AVERAGEPRIVE, m_staticAveragePirce);
    DDX_Control(pDX, IDC_STATIC_PRICE1, m_staticPrice1);
    DDX_Control(pDX, IDC_STATIC_PRICE2, m_staticPrice2);
    DDX_Control(pDX, IDC_STATIC_PRICE3, m_staticPrice3);
    DDX_Control(pDX, IDC_STATIC_PRICE4, m_staticPrice4);
    DDX_Control(pDX, IDC_STATIC_PRICE5, m_staticPrice5);
    DDX_Control(pDX, IDC_STATIC_VOLUME1, m_staticVolume1);
    DDX_Control(pDX, IDC_STATIC_VOLUME2, m_staticVolume2);
    DDX_Control(pDX, IDC_STATIC_VOLUME3, m_staticVolume3);
    DDX_Control(pDX, IDC_STATIC_VOLUME4, m_staticVolume4);
    DDX_Control(pDX, IDC_STATIC_VOLUME5, m_staticVolume5);
    DDX_Control(pDX, IDC_STATIC_AVERAGEPRIVE2, m_staticAveragePirce2);
}


BEGIN_MESSAGE_MAP(CFeedbackDlg, CDialog)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CFeedbackDlg message handlers
BOOL CFeedbackDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_customTime.m_nType = 0;
    m_customTime.CreateChildren(0);

    m_customTime.m_nType = 1;
    m_customCount.CreateChildren(1);    

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CFeedbackDlg::UpdataFeedbackData(BOOL bOrderRsp)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(!pAlgo)
        return;

    CString strD,strO,strDO;


    if (!bOrderRsp)
	{   
		switch(pAlgo->m_pAlgoAutoData->InstrumentDirection)
		{
		case 0:
			strD = "买入";
			break;
		case 1:
			strD = "卖出";
			break;
		}

		switch(pAlgo->m_pAlgoAutoData->InstrumentOffsetID)
		{
		case 0:
			strO = "开仓";
			break;
		case 1:
			strO = "平仓";
			break;
		}

		strDO = strD+strO;

		CTools_AlgoTrading::SetStringToCStatic(m_staticContract,pAlgo->m_pAlgoAutoData->InstrumentName);
		CTools_AlgoTrading::SetStringToCStatic(m_staticDirection,strDO.GetBuffer(0));
		CTools_AlgoTrading::SetStringToCStatic(m_staticCount,pAlgo->m_pAlgoAutoData->InstrumentCount);
		CTools_AlgoTrading::SetStringToCStatic(m_staticPriceLimit,pAlgo->m_pAlgoAutoData->LimitPrice);


        CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice1,0);
        CTools_AlgoTrading::SetIntToCStatic(m_staticVolume1,0);
        CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice2,0);
        CTools_AlgoTrading::SetIntToCStatic(m_staticVolume2,0);
        CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice3,0);
        CTools_AlgoTrading::SetIntToCStatic(m_staticVolume3,0);
        CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice4,0);
        CTools_AlgoTrading::SetIntToCStatic(m_staticVolume4,0);
        CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice5,0);
        CTools_AlgoTrading::SetIntToCStatic(m_staticVolume5,0);

		CTools_AlgoTrading::SetIntToCStatic(m_staticCountTotal,0);
		CTools_AlgoTrading::SetdoubleToCStatic(m_staticAveragePirce,0.0);

    }else
    {
		CTools_AlgoTrading::SetIntToCStatic(m_staticCountTotal,pAlgo->m_pAlgoAutoData->TotalVolume);
        CTools_AlgoTrading::SetdoubleToCStatic(m_staticAveragePirce,pAlgo->m_pAlgoAutoData->AverageQuan/pAlgo->m_pAlgoAutoData->TotalVolume);
    }
}

void CFeedbackDlg::UpdataHistogram()
{
    m_customTime.RefreshTradedVolume(0);
    m_customCount.RefreshTradedVolume(1);
}

void CFeedbackDlg::UpdateDepthMarketContent()
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(!pAlgo)
        return;

    m_NewVolume += pAlgo->m_pAlgoAutoData->NewVolume;
    m_MarketOrder += pAlgo->m_pAlgoAutoData->NewVolume * pAlgo->m_pAlgoAutoData->LatestPrice;
    if(m_NewVolume != 0)
        CTools_AlgoTrading::SetdoubleToCStatic(m_staticAveragePirce2,m_MarketOrder/m_NewVolume);
}   

void CFeedbackDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

	if(m_customTime.GetSafeHwnd()!=NULL && m_customCount.GetSafeHwnd()!=NULL)
	{
		CRect   rect; 
		m_customTime.GetWindowRect(&rect); 
		ScreenToClient(&rect);
        m_customTime.MoveWindow(rect.left+4,rect.top,rect.Width()-10,rect.Height());
		m_customCount.MoveWindow(rect.left+4,rect.top+175,rect.Width()-10,rect.Height());
	}

    // TODO: Add your message handler code here
}

void CFeedbackDlg::UpdataWuDangData(BOOL bFlag)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(!pAlgo)
        return;

	CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice1,0);
	CTools_AlgoTrading::SetIntToCStatic(m_staticVolume1,0);
	CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice2,0);
	CTools_AlgoTrading::SetIntToCStatic(m_staticVolume2,0);
	CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice3,0);
	CTools_AlgoTrading::SetIntToCStatic(m_staticVolume3,0);
	CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice4,0);
	CTools_AlgoTrading::SetIntToCStatic(m_staticVolume4,0);
	CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice5,0);
	CTools_AlgoTrading::SetIntToCStatic(m_staticVolume5,0);


	if (!bFlag)
    {    
        vector<ORDERINFO> OrderActiveOpt = pAlgo->m_pAlgoAutoData->vecOrderActiveInfo;
        vector<ORDERINFO>::iterator iter;

        for (iter = OrderActiveOpt.begin();iter!=OrderActiveOpt.end();iter++)
        {
            if (iter->index == 0)
            {
                CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice1,(iter->count>0)?iter->price:0);
                CTools_AlgoTrading::SetIntToCStatic(m_staticVolume1,iter->count); 
            }else if (iter->index == 1)
            {
                CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice2,(iter->count>0)?iter->price:0);
                CTools_AlgoTrading::SetIntToCStatic(m_staticVolume2,iter->count); 
            }else if (iter->index == 2)
            {
                CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice3,(iter->count>0)?iter->price:0);
                CTools_AlgoTrading::SetIntToCStatic(m_staticVolume3,iter->count); 
            }else if (iter->index == 3)
            {
                CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice4,(iter->count>0)?iter->price:0);
                CTools_AlgoTrading::SetIntToCStatic(m_staticVolume4,iter->count); 
            }else if (iter->index == 4)
            {
                CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice5,(iter->count>0)?iter->price:0);
                CTools_AlgoTrading::SetIntToCStatic(m_staticVolume5,iter->count); 
            }
        } 

    }else 
    {
        vector<ORDERINFO> OrderInfo = pAlgo->m_pAlgoAutoData->vecOrderInfo;
        vector<ORDERINFO>::iterator iter;

        for (iter = OrderInfo.begin();iter!=OrderInfo.end();iter++)
        {
//             if (iter->index == 0)
//             {
                CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice1,(iter->count>0)?iter->price:0);
                CTools_AlgoTrading::SetIntToCStatic(m_staticVolume1,iter->count); 
//             }else if (iter->index == 1)
//             {
//                 CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice2,(iter->count>0)?iter->price:0);
//                 CTools_AlgoTrading::SetIntToCStatic(m_staticVolume2,iter->count); 
//             }else if (iter->index == 2)
//             {
//                 CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice3,(iter->count>0)?iter->price:0);
//                 CTools_AlgoTrading::SetIntToCStatic(m_staticVolume3,iter->count); 
//             }else if (iter->index == 3)
//             {
//                 CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice4,(iter->count>0)?iter->price:0);
//                 CTools_AlgoTrading::SetIntToCStatic(m_staticVolume4,iter->count); 
//             }else if (iter->index == 4)
//             {
//                 CTools_AlgoTrading::SetdoubleToCStatic(m_staticPrice5,(iter->count>0)?iter->price:0);
//                 CTools_AlgoTrading::SetIntToCStatic(m_staticVolume5,iter->count); 
//             }
        } 
    }
}

void CFeedbackDlg::UpdataTitleData(void)
{
    CAlgoAutoOrderDlg *pAlgo = (CAlgoAutoOrderDlg*)m_pAlgoAutoOrderDlg;
    if(!pAlgo)
        return;

    CString strD,strO,strDO,strLimit;
    int nIndex = 1;

    switch(pAlgo->m_pAlgoAutoData->InstrumentDirection)
    {
    case 0:
        strD = "买入";
        break;
    case 1:
        strD = "卖出";
        break;
    }

    switch(pAlgo->m_pAlgoAutoData->InstrumentOffsetID)
    {
    case 0:
        strO = "开仓";
        break;
    case 1:
        strO = "平今";
        break;
	case 2:
		strO = "平仓";
    }

    strDO = strD+strO;
    
    m_staticContract.SetWindowText(pAlgo->m_pAlgoAutoData->InstrumentName.c_str());
    m_staticDirection.SetWindowText(strDO);
    CTools_AlgoTrading::SetStringToCStatic(m_staticCount,pAlgo->m_pAlgoAutoData->InstrumentCount);
    CTools_AlgoTrading::SetStringToCStatic(m_staticPriceLimit,pAlgo->m_pAlgoAutoData->LimitPrice);
}
