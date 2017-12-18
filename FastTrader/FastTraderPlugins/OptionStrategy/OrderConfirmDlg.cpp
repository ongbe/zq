// OrderConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "OrderConfirmDlg.h"
#include "OrderItemDlg.h"


// COrderConfirmDlg dialog

IMPLEMENT_DYNAMIC(COrderConfirmDlg, CDialog)

COrderConfirmDlg::COrderConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderConfirmDlg::IDD, pParent)
{

	EnableAutomation();

}

COrderConfirmDlg::~COrderConfirmDlg()
{
}

void COrderConfirmDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void COrderConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COrderConfirmDlg, CDialog)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDOK, &COrderConfirmDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COrderConfirmDlg, CDialog)
END_DISPATCH_MAP()

// Note: we add support for IID_IOrderConfirmDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {4CBCC554-33A7-4320-8921-20638F333099}
static const IID IID_IOrderConfirmDlg =
{ 0x4CBCC554, 0x33A7, 0x4320, { 0x89, 0x21, 0x20, 0x63, 0x8F, 0x33, 0x30, 0x99 } };

BEGIN_INTERFACE_MAP(COrderConfirmDlg, CDialog)
	INTERFACE_PART(COrderConfirmDlg, IID_IOrderConfirmDlg, Dispatch)
END_INTERFACE_MAP()


// COrderConfirmDlg message handlers

void COrderConfirmDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    //根据报单个数自动调整窗口大小
    int nItem = m_vOrderData.size();
    int nSpace = 10;
    this->MoveWindow(0,0,500,85+nSpace+nItem*23);

}

BOOL COrderConfirmDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here  
    CRect rc;
    GetClientRect(&rc);
    int nLeftSpace = 5, nRightSpace = 5, nTopSpace = 10, nBottomSpace = 10;
    
    CString sTemp, sOrderDirect, sOrderInstID, sOrderOffsetFlag;
    double dOrderPrice;
    int i=0,iOrderCount,iCount = m_vOrderData.size();
    COrderItemDlg *pItemDlg = NULL;
    PlatformStru_OrderInfo orderInfo;
    vector<PlatformStru_OrderInfo>::const_iterator it = m_vOrderData.begin();
    for (;it!=m_vOrderData.end();it++)
    {
        orderInfo = *it;
        sOrderDirect = (orderInfo.Direction==THOST_FTDC_D_Buy)?"买":"卖";  
        sOrderInstID.Format("%s",orderInfo.InstrumentID);
        sOrderOffsetFlag = (orderInfo.CombOffsetFlag[0]==THOST_FTDC_OF_Open)?"开仓":
                           (orderInfo.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday)?"平今":"平仓";
        dOrderPrice = orderInfo.LimitPrice;
        iOrderCount = orderInfo.VolumeTotalOriginal;
        
        pItemDlg = new COrderItemDlg(dOrderPrice,sOrderDirect,sOrderInstID,sOrderOffsetFlag,iOrderCount,this);
        pItemDlg->Create(IDD_ORDERITEMDLG,this);
        pItemDlg->ShowWindow(SW_SHOW);

        pItemDlg->MoveWindow(nLeftSpace,nTopSpace+i*25,480,25);

        i++;
    }


    GetDlgItem(IDOK)->MoveWindow(rc.right-200,rc.bottom-25,75,23);
    GetDlgItem(IDCANCEL)->MoveWindow(rc.right-120,rc.bottom-25,75,23);

    SetFocus();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void COrderConfirmDlg::SetData(vector<PlatformStru_OrderInfo>& vData)
{
    m_vOrderData.clear();
    m_vOrderData = vData;
}

void COrderConfirmDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}
