// FundNetParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "FundNetParamDlg.h"
#include "TCPDataService.h"


// CFundNetParamDlg dialog

IMPLEMENT_DYNAMIC(CFundNetParamDlg, CDialog)

CFundNetParamDlg::CFundNetParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFundNetParamDlg::IDD, pParent)
	, m_nTradeAccountID(-1)
	, m_csPerNet(_T(""))
{
	memset(&m_netFundParam, 0, sizeof(m_netFundParam));
}

CFundNetParamDlg::~CFundNetParamDlg()
{
}

void CFundNetParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INNER_VOLUMN, m_edtInnerVolumn);
	DDX_Control(pDX, IDC_COMBO_INNER_FUND_NET, m_cbxInnerFundNet);
	DDX_Text(pDX, IDC_EDIT_PER_NET, m_csPerNet);
	DDX_Control(pDX, IDC_EDIT_OUTER_VOLUMN, m_edtOuterVolumn);
	DDX_Control(pDX, IDC_EDIT_OUTER_FUND_NET, m_edtOuterFundNet);
}


BEGIN_MESSAGE_MAP(CFundNetParamDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFundNetParamDlg::OnBnClickedOk)
	ON_MESSAGE(RECV_DATA_MSG, &CFundNetParamDlg::OnRecvMessage)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	//WM_COMMAND()
END_MESSAGE_MAP()


// CFundNetParamDlg message handlers

void CFundNetParamDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	memset(&m_netFundParam, 0, sizeof(m_netFundParam));
	m_netFundParam.nTradeAccountID = m_nTradeAccountID;
	m_netFundParam.dInnerVolumn = m_edtInnerVolumn.GetData();
	m_netFundParam.dOuterVolumn = m_edtOuterVolumn.GetData();
	m_netFundParam.dOuterNetAsset = m_edtOuterFundNet.GetData();
	m_netFundParam.nInnerNetAssetOption = m_cbxInnerFundNet.GetCurSel();

	if( !CTCPDataService::GetInstance()->SendData(Cmd_RM_AddFundNetParam_Req, 
		&m_netFundParam, sizeof(m_netFundParam)))
	{
		MessageBox(lpszQryError, lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableControls(FALSE);
	}

}

BOOL CFundNetParamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cbxInnerFundNet.AddString(_T("动态权益"));
	m_cbxInnerFundNet.SetCurSel(0);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QueryFundNetParam_Req, 
		&m_nTradeAccountID, sizeof(m_nTradeAccountID));

	return TRUE;
}

void CFundNetParamDlg::SetTradeAccountID( int nAccountID )
{
	m_nTradeAccountID = nAccountID;
}

void CFundNetParamDlg::SetModifyData()
{
	m_edtInnerVolumn.SetData(m_netFundParam.dInnerVolumn);
	m_edtOuterVolumn.SetData(m_netFundParam.dOuterVolumn);
	m_edtOuterFundNet.SetData(m_netFundParam.dOuterNetAsset);
	m_cbxInnerFundNet.SetCurSel(m_netFundParam.nInnerNetAssetOption);

	if ( m_netFundParam.dOuterVolumn > -0.001 && m_netFundParam.dOuterVolumn < 0.001 )
	{
		m_csPerNet = _T("0");
	}
	else
	{
		m_csPerNet.Format(_T("%.2f"), m_netFundParam.dOuterNetAsset/m_netFundParam.dOuterVolumn);
	}

	UpdateData(FALSE);
}

void CFundNetParamDlg::EnableControls( BOOL bFlag )
{
	GetDlgItem(IDC_EDIT_INNER_VOLUMN)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_OUTER_VOLUMN)->EnableWindow(bFlag);
	GetDlgItem(IDC_EDIT_OUTER_FUND_NET)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_INNER_FUND_NET)->EnableWindow(bFlag);
}

LRESULT CFundNetParamDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_QueryFundNetParam_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			memset(&m_netFundParam, 0, sizeof(m_netFundParam));

			int nCount = pRecvData->head.len / sizeof(NetFundParam);
			if ( nCount == 0 )
			{
				//数据不存在
			}
			else if ( nCount == 1 )
			{
				//已经存在数据
				m_netFundParam = *(NetFundParam*)(pRecvData->pData);
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
	else if ( pRecvData->head.cmdid == Cmd_RM_AddFundNetParam_Rsp )
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

void CFundNetParamDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CFundNetParamDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ( ::GetFocus() == m_edtInnerVolumn.GetSafeHwnd() )
	{
		m_edtInnerVolumn.OnEnKillfocus();
	}
	else if ( ::GetFocus() == m_edtOuterVolumn.GetSafeHwnd() )
	{
		m_edtOuterVolumn.OnEnKillfocus();
		CalcPerNet();
	}
	else if ( ::GetFocus() == m_edtOuterFundNet.GetSafeHwnd() )
	{
		m_edtOuterFundNet.OnEnKillfocus();

		CalcPerNet();
	}
	else
	{

	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CFundNetParamDlg::CalcPerNet()
{
	UpdateData(TRUE);
	double dOuterVolumn = m_edtOuterVolumn.GetData();
	if ( dOuterVolumn > -0.001 && dOuterVolumn < 0.001 )
	{
		m_csPerNet = _T("0");
	}
	else
	{
		m_csPerNet.Format(_T("%.2f"), m_edtOuterFundNet.GetData()/dOuterVolumn);
	}

	UpdateData(FALSE);
}

BOOL CFundNetParamDlg::OnCommand( WPARAM wParam, LPARAM lParam )
{
	if ( HIWORD(wParam) == EN_KILLFOCUS )
	{
		WORD wID = LOWORD(wParam);
		if ( wID == IDC_EDIT_OUTER_VOLUMN ||
			wID == IDC_EDIT_OUTER_FUND_NET)
		{
			CalcPerNet();
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}
