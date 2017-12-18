// AutoForceCloseParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "AutoForceCloseParamDlg.h"
#include "TCPDataService.h"


// CAutoForceCloseParamDlg dialog

IMPLEMENT_DYNAMIC(CAutoForceCloseParamDlg, CDialog)

CAutoForceCloseParamDlg::CAutoForceCloseParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoForceCloseParamDlg::IDD, pParent)
	, m_csThreshold(_T(""))
{

}

CAutoForceCloseParamDlg::~CAutoForceCloseParamDlg()
{
}

void CAutoForceCloseParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbxForceCloseInd);
	DDX_Text(pDX, IDC_EDIT2, m_csThreshold);
	DDX_Control(pDX, IDC_COMBO2, m_cbxCloseStrategy);
	DDX_Control(pDX, IDC_COMBO3, m_cbxClosePrice);
}


BEGIN_MESSAGE_MAP(CAutoForceCloseParamDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAutoForceCloseParamDlg::OnBnClickedOk)
	ON_MESSAGE(RECV_DATA_MSG, &CAutoForceCloseParamDlg::OnRecvMessage)
END_MESSAGE_MAP()

void CAutoForceCloseParamDlg::SetTradeAccountID( int nAccountID )
{
	m_nTradeAccountID = nAccountID;
}

BOOL CAutoForceCloseParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	m_cbxForceCloseInd.AddString(_T("资金净值"));
	m_cbxCloseStrategy.AddString(_T("全部平仓"));
	m_cbxClosePrice.AddString(_T("市价"));

	m_cbxForceCloseInd.SetCurSel(0);
	m_cbxCloseStrategy.SetCurSel(0);
	m_cbxClosePrice.SetCurSel(0);

	CTCPDataService::GetInstance()->SendData(Cmd_RM_QueryForceCloseParam_Req, 
		&m_nTradeAccountID, sizeof(m_nTradeAccountID));

	return TRUE;
}


// CAutoForceCloseParamDlg message handlers

void CAutoForceCloseParamDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	memset(&m_forceCloseParam, 0, sizeof(m_forceCloseParam));
	m_forceCloseParam.nTradeAccountID = m_nTradeAccountID;
	m_forceCloseParam.fThreshold = atof(m_csThreshold.GetBuffer(0));
	m_csThreshold.ReleaseBuffer();

	if( !CTCPDataService::GetInstance()->SendData(Cmd_RM_AddForceCloseParam_Req, 
		&m_forceCloseParam, sizeof(m_forceCloseParam)))
	{
		MessageBox(lpszQryError, lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableControls(FALSE);
	}

	//OnOK();
}

void CAutoForceCloseParamDlg::SetModifyData()
{
	m_csThreshold.Format(_T("%.2f"), m_forceCloseParam.fThreshold);
	UpdateData(FALSE);
}

void CAutoForceCloseParamDlg::EnableControls( BOOL bFlag )
{
	GetDlgItem(IDC_COMBO1)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT2)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO2)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO3)->EnableWindow(bFlag);
}

LRESULT CAutoForceCloseParamDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_QueryForceCloseParam_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			memset(&m_forceCloseParam, 0, sizeof(m_forceCloseParam));

			int nCount = pRecvData->head.len / sizeof(ForceCloseParam);
			if ( nCount == 0 )
			{
				//数据不存在
			}
			else if ( nCount == 1 )
			{
				//已经存在数据
				m_forceCloseParam = *(ForceCloseParam*)(pRecvData->pData);
				SetModifyData();
			}
			else
			{
				//此种情况不会出现
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_AddForceCloseParam_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			MessageBox(_T("操作成功！"), lpszPrompt, MB_ICONINFORMATION);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}

	return 0;
}
