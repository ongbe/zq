// OrderItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "OrderItemDlg.h"
#include "OrderFunc.h"
#include "CommPublic.h"


// COrderItemDlg dialog

IMPLEMENT_DYNAMIC(COrderItemDlg, CDialog)

COrderItemDlg::COrderItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderItemDlg::IDD, pParent)
    , m_dPrice(0)
    , m_sDirect(_T(""))
    , m_sInstID(_T(""))
    , m_sOffsetFlag(_T(""))
    , m_iCount(0)
{

	EnableAutomation();

}

COrderItemDlg::COrderItemDlg(double dPrice, CString sDirect, CString sInstID, CString offSetFlag, int nCount, CWnd* pParent /*=NULL*/)
: CDialog(COrderItemDlg::IDD, pParent)
, m_dPrice(dPrice)
, m_sDirect(sDirect)
, m_sInstID(sInstID)
, m_sOffsetFlag(offSetFlag)
, m_iCount(nCount)

{

}

COrderItemDlg::~COrderItemDlg()
{
}

void COrderItemDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void COrderItemDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PRICE, m_dPrice);
    DDX_Text(pDX, IDC_STATIC_DIRECT, m_sDirect);
    DDX_Text(pDX, IDC_STATIC_INSTID, m_sInstID);
    DDX_Text(pDX, IDC_STATIC_OFFSETFLAG, m_sOffsetFlag);
    DDX_Text(pDX, IDC_EDIT_COUNT, m_iCount);
    DDX_Control(pDX, IDC_SPIN_PRICE, m_spinPrice);
    DDX_Control(pDX, IDC_SPIN_COUNT, m_spinCount);
    DDX_Control(pDX, IDC_EDIT_PRICE, m_editPrice);
    DDX_Control(pDX, IDC_EDIT_COUNT, m_editCount);
}


BEGIN_MESSAGE_MAP(COrderItemDlg, CDialog)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PRICE, &COrderItemDlg::OnDeltaposSpinPrice)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COUNT, &COrderItemDlg::OnDeltaposSpinCount)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COrderItemDlg, CDialog)
END_DISPATCH_MAP()

// Note: we add support for IID_IOrderItemDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {075C8114-129D-4301-8756-588D6C059EE9}
static const IID IID_IOrderItemDlg =
{ 0x75C8114, 0x129D, 0x4301, { 0x87, 0x56, 0x58, 0x8D, 0x6C, 0x5, 0x9E, 0xE9 } };

BEGIN_INTERFACE_MAP(COrderItemDlg, CDialog)
	INTERFACE_PART(COrderItemDlg, IID_IOrderItemDlg, Dispatch)
END_INTERFACE_MAP()


// COrderItemDlg message handlers

void COrderItemDlg::OnDeltaposSpinPrice(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    double fPrice,fTickPrice,fUpperPrice,fLowerPrice, dVolume = 0.0;
    char strValue[64];
    ZeroMemory(strValue, sizeof(strValue));
    m_editPrice.GetLine(0, strValue, 63);
    dVolume = atof(strValue);

    fTickPrice = CDataInfo::NewInstance()->GetInstPriceTick((string)m_sInstID);
    fUpperPrice = CDataInfo::NewInstance()->GetInstUpperPrice((string)m_sInstID);
    fLowerPrice = CDataInfo::NewInstance()->GetInstLowerPrice((string)m_sInstID);

    CheckSumPrice(dVolume, fTickPrice,0);
    dVolume -= pNMUpDown->iDelta * fTickPrice;

    fPrice = AnalysePrice(dVolume,fUpperPrice,fLowerPrice);

    sprintf_s(strValue, "%.4f", fPrice);
    m_editPrice.SetSel(0, -1);
    m_editPrice.ReplaceSel(strValue);

    *pResult = 0;
}

void COrderItemDlg::OnDeltaposSpinCount(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int nVolume;
    char strValue[64];
    ZeroMemory(strValue, sizeof(strValue));
    m_editCount.GetLine(0, strValue, 63);
    nVolume = atoi(strValue);

    nVolume -= pNMUpDown->iDelta;

    if(nVolume<=1)
        nVolume = 1;
    
    if(nVolume>=10000)
        nVolume = 10000;

    sprintf_s(strValue, "%d", nVolume);
    m_editCount.SetSel(0, -1);
    m_editCount.ReplaceSel(strValue);

    *pResult = 0;
}
