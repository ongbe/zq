// BatchAddAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "RiskManage.h"
#include "BatchAddAccountDlg.h"
#include "ConfigMgr.h"
#include "TCPDataService.h"
#include "CryptoAPI.h"

// CBatchAddAccountDlg dialog

IMPLEMENT_DYNAMIC(CBatchAddAccountDlg, CDialog)

CBatchAddAccountDlg::CBatchAddAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchAddAccountDlg::IDD, pParent)
	, m_csTitle(_T("账户号,密码,用户名,联系方式,证件类型,证件号码,初始权益"))
{
	m_mapAccountInfo.clear();
	m_mapErrorInfo.clear();
	m_vSuccessedID.clear();
	m_tempAccountInfoMap.clear();

	m_mapAssetMgmtOrgTree.clear();
	m_mapFinancialProductTree.clear();
	m_mapIndModule.clear();
	m_mapTraderInfoEx.clear();
	m_mapFormatErrorLine.clear();
}

CBatchAddAccountDlg::~CBatchAddAccountDlg()
{
}

void CBatchAddAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_edtPath);
	DDX_Control(pDX, IDC_COMBO_BATCH_FIN_PRO, m_cbxFinPro);
	DDX_Control(pDX, IDC_COMBO_BATCH_TRADER, m_cbxTrader);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
	DDX_Control(pDX, IDC_COMBO_BATCH_MODULE, m_cbxModule);
}


BEGIN_MESSAGE_MAP(CBatchAddAccountDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CBatchAddAccountDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDOK, &CBatchAddAccountDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(RECV_DATA_MSG, &CBatchAddAccountDlg::OnRecvMessage)
	ON_CBN_SELENDOK(IDC_COMBO_BATCH_ORG, &CBatchAddAccountDlg::OnCbnSelchangeComboBatchOrg)
	ON_CBN_SELCHANGE(IDC_COMBO_BATCH_FIN_PRO, &CBatchAddAccountDlg::OnCbnSelchangeComboBatchFinPro)
END_MESSAGE_MAP()


// CBatchAddAccountDlg message handlers

void CBatchAddAccountDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL, _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"CSV files (*.csv)|*.csv||");
	if( IDOK == dlg.DoModal())
	{
		m_edtPath.SetWindowText(dlg.GetPathName());
	} 
}

void CBatchAddAccountDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csFilePath;
	m_edtPath.GetWindowText(csFilePath);
	if ( csFilePath.IsEmpty())
	{
		MessageBox(_T("请输入文件路径！"), lpszPrompt, MB_ICONERROR);
		m_edtPath.SetFocus();
		return;
	}

	csFilePath = csFilePath.Trim();
	CString csSuffix = csFilePath.Right(4);
	if ( csSuffix.MakeLower() != _T(".csv"))
	{
		MessageBox(_T("只能导入CSV文件！"), lpszPrompt, MB_ICONERROR);
		m_edtPath.SetFocus();
		return;
	}
	if ( -1 == _access(csFilePath, 0))
	{
		MessageBox(_T("文件路径不存在！"), lpszPrompt, MB_ICONERROR);
		m_edtPath.SetFocus();
		return;
	}

	if ( m_cbxModule.GetCurSel() == -1 )
	{
		MessageBox(_T("风险模型不能为空！"), lpszPrompt, MB_ICONERROR);
		m_cbxModule.SetFocus();
		return;
	}

	int nRiskModuleID = m_cbxModule.GetItemData(m_cbxModule.GetCurSel());

	if ( m_comboTree.GetCurSelItem() == NULL )
	{
		MessageBox(lpszEmptyOrg, lpszPrompt, MB_ICONERROR);
		m_comboTree.SetFocus();
		return;
	}

	int nAssetMgmtOrgID = m_comboTree.GetSelectItemData();
	int nFinancialProductSel = m_cbxFinPro.GetCurSel();
	int nFinProID = 0;
	if ( nFinancialProductSel != -1 )
	{
		nFinProID = m_cbxFinPro.GetItemData(nFinancialProductSel);
	}

	int nTraderSel = m_cbxTrader.GetCurSel();
	int nTraderID = 0;
	if ( nTraderSel != -1 )
	{
		nTraderID = m_cbxTrader.GetItemData(nTraderSel);
	}

	m_vSuccessedID.clear();
	m_mapErrorInfo.clear();
	if ( !LoadImportFile(csFilePath))
	{
		MessageBox(_T("无法打开文件，或文件内容格式错误！"), lpszPrompt, MB_ICONERROR);
		m_edtPath.SetFocus();
		return;
	}

	m_tempAccountInfoMap = m_mapAccountInfo;
	std::map<int, TrustTradeAccount>::iterator it = m_tempAccountInfoMap.begin();
	for ( it; it != m_tempAccountInfoMap.end(); it++ )
	{
		it->second.nRiskIndModuleID = nRiskModuleID;
		it->second.nAssetMgmtOrgID = nAssetMgmtOrgID;
		it->second.nFinancialProductID = nFinProID;
		it->second.nTraderID = nTraderID;
	}

	std::map<int, TrustTradeAccount>::iterator it_temp = m_tempAccountInfoMap.begin();
	if ( it_temp != m_tempAccountInfoMap.end() )
	{
		if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_AddTradeAccount_Req,
			&it_temp->second, sizeof(it_temp->second), it_temp->first))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}

		m_tempAccountInfoMap.erase(it_temp);

		//设置进度条
		m_progressCtrl.SetRange(0, m_tempAccountInfoMap.size());
		m_progressCtrl.SetStep(1);
		m_progressCtrl.ShowWindow(SW_SHOW);
		m_btnOK.EnableWindow(FALSE);
	}
	else
	{
		m_progressCtrl.ShowWindow(SW_HIDE);

		if ( !m_mapErrorInfo.empty())
		{
			WriteErrorFile();

			CString csMsg;
			csMsg.Format(_T("本次批量导入完成，成功%d条，失败%d条，是否查看失败文件？"),
				static_cast<int>(m_vSuccessedID.size()), 
				static_cast<int>(m_mapErrorInfo.size()));
			if ( IDYES == MessageBox(csMsg, lpszPrompt, MB_ICONQUESTION|MB_YESNO))
			{
				CString csSysPath = ConfigManager::GetInstance()->GetSystemPath();
				CString csErrorFile = csSysPath + _T("\\ImportError.csv");
				ShellExecute(NULL, _T("open"), csErrorFile, NULL,NULL,SW_SHOWNORMAL); 
			}
		}
		else
		{
			MessageBox(_T("没有可供导入的数据！"), lpszPrompt, MB_ICONERROR);
		}
	}

	//OnOK();
}

BOOL CBatchAddAccountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//取消组合框风格
	SetWindowTheme( m_cbxFinPro.m_hWnd, L"", L"");
	SetWindowTheme( m_cbxTrader.m_hWnd, L"", L"");
	SetWindowTheme( m_cbxModule.m_hWnd, L"", L"");

	//填充所属资管
	m_comboTree.SubclassDlgItem (IDC_COMBO_BATCH_ORG, this);
	m_comboTree.SetAssetMgmtOrg(m_mapAssetMgmtOrgTree);

	m_progressCtrl.ShowWindow(SW_HIDE);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_QryValidRiskIndModule_Req, NULL, 0))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}

	return TRUE;
}

bool CBatchAddAccountDlg::LoadImportFile( const CString& csFilePath )
{
	m_mapAccountInfo.clear();
	m_mapFormatErrorLine.clear();

	CStdioFile file;
	if ( !file.Open(csFilePath, CFile::modeRead))
	{
		return false;
	}

	int i = 0;
	CString csLine;
	while ( file.ReadString(csLine) )
	{
		if ( i == 0 )
		{
			if ( csLine.Trim() != m_csTitle )
			{
				file.Close();
				return false;
			}
		}
		else
		{
			TrustTradeAccount accountInfo;
			memset(&accountInfo, 0, sizeof(accountInfo));
			if ( !ParseLine(csLine, accountInfo))
			{
				m_mapFormatErrorLine[i] = csLine;
				m_mapErrorInfo[i] = _T("数据格式错误");
			}
			else
			{
				m_mapAccountInfo[i] = accountInfo;
			}
		}

		i++;
	}
	
	file.Close();
	return true;
}

bool CBatchAddAccountDlg::ParseLine( const CString& csLine, TrustTradeAccount& accountInfo )
{
	CString csTemp = csLine;
	memset(&accountInfo, 0, sizeof(accountInfo));
	int nPos = csTemp.Find(',');
	//账户号
	if ( nPos == -1 )
	{
		return false;
	}

	CString csAccount = csTemp.Left(nPos);
	//去除前缀
	if (!RemovePrefix(csAccount))
	{
		return false;
	}
	if ( csAccount.GetLength() < MIN_ACCOUNT_LENGTH ||
		csAccount.GetLength() > MAX_ACCOUNT_LENGTH ||
		!IsValidString(csAccount))
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//密码
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}
	CString csPwd = csTemp.Left(nPos);
	if (!RemovePrefix(csPwd))
	{
		return false;
	}
	if ( csPwd.GetLength() > MAX_PWD_LENGTH ||
		!IsValidString(csPwd))
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//用户名
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}
	CString csUserName = csTemp.Left(nPos);
	if (!RemovePrefix(csUserName))
	{
		return false;
	}
	if ( csUserName.GetLength() > MAX_TRUST_ACCOUNT_NAME_LENGTH )
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//联系方式
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}

	CString csContactInfo = csTemp.Left(nPos);
	if (!RemovePrefix(csContactInfo))
	{
		return false;
	}
	if ( csContactInfo.GetLength() > MAX_CONTACTINFO_LENGTH )
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//证件类型
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}

	CString csIDCCardType = csTemp.Left(nPos);
	if (!RemovePrefix(csIDCCardType))
	{
		return false;
	}
	int nIDCardType = atoi(csIDCCardType.GetBuffer(0));
	csIDCCardType.ReleaseBuffer();
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//证件号码
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}

	CString csIDCardNo = csTemp.Left(nPos);
	if (!RemovePrefix(csIDCardNo))
	{
		return false;
	}
	if ( csIDCardNo.GetLength() > MAX_IDCARD_NO_LENGTH )
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//初始权益
	nPos = csTemp.Find(',');
	if ( nPos != -1 )
	{
		return false;
	}

	CString csProfit = csTemp;
	if (!RemovePrefix(csProfit))
	{
		return false;
	}
	double dProfit = atof(csProfit.GetBuffer(0));
	csProfit.ReleaseBuffer();

	strcpy(accountInfo.szAccount, csAccount.GetBuffer(0));
	csAccount.ReleaseBuffer();
	strcpy(accountInfo.szPwd, csPwd.GetBuffer(0));
	csPwd.ReleaseBuffer();
	if ( strlen(accountInfo.szPwd) != 0)
	{
		CCryptoAPI::Encode3DES_ToString(accountInfo.szPwd);
	}
	
	strcpy(accountInfo.szName, csUserName.GetBuffer(0));
	csUserName.ReleaseBuffer();
	strcpy(accountInfo.szContactInfo, csContactInfo.GetBuffer(0));
	csContactInfo.ReleaseBuffer();
	accountInfo.nIDCardType = (IDCardType)nIDCardType;
	strcpy(accountInfo.szIDCardNo, csIDCardNo.GetBuffer(0));
	csIDCardNo.ReleaseBuffer();
	accountInfo.dProfit = dProfit;
	
	return true;
}

bool CBatchAddAccountDlg::IsValidString( const CString& cs )
{
	if ( cs.IsEmpty() )
	{
		return true;
	}

	for ( int i = 0; i < cs.GetLength(); i++ )
	{
		UINT nChar = cs.GetAt(i);
		if ( !(
			( nChar >= _T( 'a' ) && nChar <= _T( 'z' ) ) ||
			( nChar >= _T( 'A' ) && nChar <= _T( 'Z' ) ) ||
			( nChar >= _T( '0' ) && nChar <= _T( '9' ) ))
			 )
		{
			return false;
		}
	}

	return true;
}

LRESULT CBatchAddAccountDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_AddTradeAccount_Rsp )
	{
		int nID = pRecvData->head.seq;
		m_progressCtrl.SetPos(nID);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_vSuccessedID.push_back(nID);	
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			m_mapErrorInfo[nID] = ConventErrorMsg(csMsg);
		}

		std::map<int, TrustTradeAccount>::iterator it = m_tempAccountInfoMap.begin();
		if ( it != m_tempAccountInfoMap.end() )
		{
			if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_AddTradeAccount_Req,
				&it->second, sizeof(it->second), it->first))
			{
				std::string strError = CTCPDataService::GetInstance()->GetLastError();
				m_mapErrorInfo[it->first] = strError.c_str();
			}
			
			m_tempAccountInfoMap.erase(it);
		}
		else
		{
			//导入完成
			m_progressCtrl.ShowWindow(SW_HIDE);
			m_btnOK.EnableWindow(TRUE);

			WriteErrorFile();

			CString csMsg;
			csMsg.Format(_T("本次批量导入完成，成功%d条，失败%d条，是否查看失败文件？"),
				static_cast<int>(m_vSuccessedID.size()), 
				static_cast<int>(m_mapErrorInfo.size()));
			if ( IDYES == MessageBox(csMsg, lpszPrompt, MB_ICONQUESTION|MB_YESNO))
			{
				CString csSysPath = ConfigManager::GetInstance()->GetSystemPath();
				CString csErrorFile = csSysPath + _T("\\ImportError.csv");
				ShellExecute(NULL, _T("open"), csErrorFile, NULL,NULL,SW_SHOWNORMAL); 
			}
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryValidRiskIndModule_Rsp )
	{
		m_mapIndModule.clear();
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(RiskIndicatorModule);
			for ( int i = 0; i < nCount; i++ )
			{
				RiskIndicatorModule* pRiskModule = (RiskIndicatorModule*)((char*)pRecvData->pData + i*sizeof(RiskIndicatorModule));
				m_mapIndModule[pRiskModule->nRiskIndModuleID] = *pRiskModule;
			}

			//填充风险模型列表
			m_cbxModule.ResetContent();
			int i = 0;
			std::map<int, RiskIndicatorModule>::iterator it_module = m_mapIndModule.begin();
			for ( ; it_module != m_mapIndModule.end(); ++it_module )
			{
				m_cbxModule.AddString(it_module->second.szName);
				m_cbxModule.SetItemData(i, it_module->second.nRiskIndModuleID);
				i++;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else
	{

	}

	return 0;
}

void CBatchAddAccountDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}

bool CBatchAddAccountDlg::WriteErrorFile()
{
	CString csSysPath = ConfigManager::GetInstance()->GetSystemPath();
	CString csErrorFile = csSysPath + _T("\\ImportError.csv");
	CStdioFile file;
	if ( !file.Open(csErrorFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		return false;
	}

	file.WriteString(m_csTitle);
	file.WriteString(_T("\n"));

	std::map<int, CString>::iterator it = m_mapErrorInfo.begin();
	for ( ; it != m_mapErrorInfo.end(); it++ )
	{
		std::map<int, TrustTradeAccount>::iterator it_account = m_mapAccountInfo.find(it->first);
		if ( it_account != m_mapAccountInfo.end() )
		{
			CString csLine = CombineLine(it_account->second);
			csLine += _T(",");
			csLine += it->second;
			file.WriteString(csLine);
			file.WriteString(_T("\n"));
		}
		else
		{
			std::map<int, CString>::iterator it_formatErrorLine = m_mapFormatErrorLine.find(it->first);
			if ( it_formatErrorLine != m_mapFormatErrorLine.end())
			{
				CString csLine = it_formatErrorLine->second;
				csLine += _T(",");
				csLine += it->second;
				file.WriteString(csLine);
				file.WriteString(_T("\n"));
			}
		}
	}

	file.Close();
	return true;
}

CString CBatchAddAccountDlg::CombineLine( const TrustTradeAccount& accountInfo )
{
	//账户号,密码,用户名,联系方式,证件类型,证件号码,初始权益
	char szPwd[33];
	memset(szPwd, 0, sizeof(szPwd));
	strcpy(szPwd, accountInfo.szPwd);
	if ( strlen(szPwd) != 0 )
	{
		CCryptoAPI::Decode3DES_ToString(szPwd);
	}

	CString csLine;
	csLine.Format(_T("T%s,T%s,T%s,T%s,T%d,T%s,T%.2f"),
		accountInfo.szAccount, szPwd, accountInfo.szName,
		accountInfo.szContactInfo, accountInfo.nIDCardType, accountInfo.szIDCardNo, accountInfo.dProfit);
	return csLine;
}

void CBatchAddAccountDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

CString CBatchAddAccountDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("账户号重复！");
	}
	else if ( -1 != csMsg.Find(_T("ORA-00291")) )
	{
		csTempMsg = _T("所选择的组织架构或者理财产品已被删除！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}

void CBatchAddAccountDlg::SetAssetMgmtOrgTree(std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetMgmtOrgTree)
{
	m_mapAssetMgmtOrgTree = mapAssetMgmtOrgTree;
}

void CBatchAddAccountDlg::SetFinancialProduct( std::map<int, std::map<int, FinancialProduct>>& mapFinancialProductTree )
{
	m_mapFinancialProductTree = mapFinancialProductTree;
}

void CBatchAddAccountDlg::SetTrader( std::map<int, TraderInfoEx>& mapTraderInfoEx )
{
	m_mapTraderInfoEx = mapTraderInfoEx;
}

void CBatchAddAccountDlg::FillFinancialProduct( int nAssetMgmtOrgID )
{
	m_cbxFinPro.ResetContent();
	std::map<int, std::map<int, FinancialProduct>>::iterator it =
		m_mapFinancialProductTree.find(nAssetMgmtOrgID);
	if ( it == m_mapFinancialProductTree.end())
	{
		return;
	}

	std::map<int, FinancialProduct>::iterator it_fin = it->second.begin();
	int i = 0;
	for ( ; it_fin != it->second.end(); ++it_fin )
	{
		m_cbxFinPro.AddString(it_fin->second.szName);
		m_cbxFinPro.SetItemData(i, it_fin->second.nFinancialProductID);
		i++;
	}
}

void CBatchAddAccountDlg::FillTrader( int nAssetMgmtOrgID, int nFinancialProductID )
{
	m_cbxTrader.ResetContent();

	int i = 0;
	std::map<int, TraderInfoEx>::iterator it = m_mapTraderInfoEx.begin();
	for ( ; it != m_mapTraderInfoEx.end(); ++it )
	{
		if ( it->second.traderInfo.nAssetMgmtOrgID == nAssetMgmtOrgID &&
			it->second.traderInfo.nFinancialProductID == nFinancialProductID )
		{
			m_cbxTrader.AddString(it->second.traderInfo.szName);
			m_cbxTrader.SetItemData(i, it->second.traderInfo.nTraderID);
			i++;
		}
	}
}

void CBatchAddAccountDlg::OnCbnSelchangeComboBatchOrg()
{
	// TODO: Add your control notification handler code here
	int nSelOrgID = m_comboTree.GetSelectItemData();
	FillFinancialProduct(nSelOrgID);

	FillTrader(nSelOrgID, 0);
}

void CBatchAddAccountDlg::OnCbnSelchangeComboBatchFinPro()
{
	// TODO: Add your control notification handler code here
	int nOrgID = m_comboTree.GetSelectItemData();
	int nFinSel = m_cbxFinPro.GetCurSel();
	int nFinProductID = m_cbxFinPro.GetItemData(nFinSel);
	FillTrader(nOrgID, nFinProductID);
}

int CBatchAddAccountDlg::GetImportedDataCount()
{
	return static_cast<int>(m_vSuccessedID.size());
}

bool CBatchAddAccountDlg::RemovePrefix( CString& cs )
{
	if ( cs.IsEmpty() )
	{
		return true;
	}

	if ( cs.GetAt(0) != _T('T'))
	{
		return false;
	}

	cs = cs.Right(cs.GetLength()-1);
	return true;
}
