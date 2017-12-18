// RiskMsgTemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "RiskMsgTemplateDlg.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridColumnTraitMultilineEdit.h"
#include "CGridColumnTraitText.h"
#include "TCPDataService.h"


// CRiskMsgTemplateDlg dialog

IMPLEMENT_DYNAMIC(CRiskMsgTemplateDlg, CDialog)

CRiskMsgTemplateDlg::CRiskMsgTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRiskMsgTemplateDlg::IDD, pParent)
	, m_nSelIndID(-1)
{
	m_vIndicator.clear();
	m_mapMaxRiskLevel.clear();
	m_mapEvtMsgTemplate.clear();
}

CRiskMsgTemplateDlg::~CRiskMsgTemplateDlg()
{
}

void CRiskMsgTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG_IND, m_listInd);
	DDX_Control(pDX, IDC_BUTTON_MSG_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_LIST_MSG_CONTENT, m_listMsg);
}


BEGIN_MESSAGE_MAP(CRiskMsgTemplateDlg, CDialog)
	ON_MESSAGE(RECV_DATA_MSG, &CRiskMsgTemplateDlg::OnRecvMessage)
	ON_BN_CLICKED(IDOK, &CRiskMsgTemplateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRiskMsgTemplateDlg::OnBnClickedCancel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MSG_IND, &CRiskMsgTemplateDlg::OnLvnItemchangedListMsgInd)
	ON_BN_CLICKED(IDC_BUTTON_MSG_SAVE, &CRiskMsgTemplateDlg::OnBnClickedButtonMsgSave)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRiskMsgTemplateDlg message handlers

void CRiskMsgTemplateDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CRiskMsgTemplateDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CRiskMsgTemplateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle1 = m_listInd.GetExtendedStyle();
	dwStyle1 |= LVS_EX_FULLROWSELECT;
	m_listInd.SetExtendedStyle(dwStyle1);

	m_listInd.InsertHiddenLabelColumn();
	m_listInd.SetCellMargin(1.5);
	CGridColumnTraitText* pTextTrait = new CGridColumnTraitText;
	m_listInd.InsertColumnTrait(1, _T("风险指标"), LVCFMT_LEFT, 160, 0, pTextTrait);
	for ( UINT i = 0; i < m_vIndicator.size(); i++ )
	{
		int nCount = m_listInd.GetItemCount();
		m_listInd.InsertItem(nCount, _T(""));
		m_listInd.SetItemText(nCount, 1, m_vIndicator[i].szName);
		m_listInd.SetItemData(nCount, m_vIndicator[i].nRiskIndicatorID);
	}

	m_listMsg.InsertHiddenLabelColumn();
	m_listMsg.SetCellMargin(1.5);
	pTextTrait = new CGridColumnTraitText;
	m_listMsg.InsertColumnTrait(1, _T("风险等级"), LVCFMT_RIGHT, 80, 0, pTextTrait);
	CGridColumnTraitEdit* pEditTrait = new CGridColumnTraitEdit;
	m_listMsg.InsertColumnTrait(2, _T("标题"), LVCFMT_LEFT, 120, 0, pEditTrait);
	CGridColumnTraitMultilineEdit* pMultiLineEditTrait = new CGridColumnTraitMultilineEdit;
	DWORD dwStyle = pMultiLineEditTrait->GetStyle();
	dwStyle |= ES_MULTILINE;
	dwStyle |= ES_AUTOVSCROLL;
	pMultiLineEditTrait->SetStyle(dwStyle);
	m_listMsg.InsertColumnTrait(3, _T("内容"), LVCFMT_LEFT, 240, 0, pMultiLineEditTrait);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);
	//查询风险指标的最大级别
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryMaxRiskLevel_Req, NULL, 0);
	//查询消息模版列表
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryEvtMsgTemplate_Req, NULL, 0);

	return TRUE;
}

void CRiskMsgTemplateDlg::SetIndicator( const std::vector<RiskIndicator>& vIndicator )
{
	m_vIndicator = vIndicator;
}

void CRiskMsgTemplateDlg::OnLvnItemchangedListMsgInd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		m_listMsg.DeleteAllItems();

		m_nSelIndID = m_listInd.GetItemData(pNMLV->iItem);

		std::map<int, int>::iterator it_ind = m_mapMaxRiskLevel.find(m_nSelIndID);
		if ( it_ind != m_mapMaxRiskLevel.end())
		{
			int nMaxLevel = it_ind->second;
			for ( int nLevel = 1; nLevel <= nMaxLevel; nLevel++ )
			{
				m_listMsg.InsertItem(nLevel-1, _T(""));
				CString csLevel;
				csLevel.Format(_T("第%d级"), nLevel);
				m_listMsg.SetItemText(nLevel-1, 1, csLevel);
				m_listMsg.SetItemData(nLevel-1, nLevel);

				EventMessageTemplate msg;
				if ( FindEvtMsgTemplate(m_nSelIndID, nLevel, msg))
				{
					m_listMsg.SetItemText(nLevel-1, 2, msg.szTitle);
					m_listMsg.SetItemText(nLevel-1, 3, msg.szContent);
				}
				else
				{
					m_listMsg.SetItemText(nLevel-1, 2, _T(""));
					m_listMsg.SetItemText(nLevel-1, 3, _T(""));
				}
			}
		}
	}
}

LRESULT CRiskMsgTemplateDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_QryMaxRiskLevel_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapMaxRiskLevel.clear();
			int nCount = pRecvData->head.len / sizeof(MaxRiskLevel);
			for ( int i = 0; i < nCount; i++ )
			{
				MaxRiskLevel* pMaxRiskLevle = (MaxRiskLevel*)((char*)pRecvData->pData + i*sizeof(MaxRiskLevel));
				m_mapMaxRiskLevel[pMaxRiskLevle->nRiskIndicatorID] = pMaxRiskLevle->nMaxLevel;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryEvtMsgTemplate_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapEvtMsgTemplate.clear();
			int nCount = pRecvData->head.len / sizeof(EventMessageTemplate);
			for ( int i = 0; i < nCount; i++ )
			{
				EventMessageTemplate* pMsgTemplate = (EventMessageTemplate*)((char*)pRecvData->pData + i*sizeof(EventMessageTemplate));

				std::map<int, std::map<int, EventMessageTemplate>>::iterator it_ind
					= m_mapEvtMsgTemplate.find(pMsgTemplate->nRiskIndicatorID);
				if ( it_ind != m_mapEvtMsgTemplate.end())
				{
					(it_ind->second)[pMsgTemplate->nRiskLevelID] = *pMsgTemplate;
				}
				else
				{
					std::map<int, EventMessageTemplate> mapEvtMsg;
					mapEvtMsg[pMsgTemplate->nRiskLevelID] = *pMsgTemplate;
					m_mapEvtMsgTemplate[pMsgTemplate->nRiskIndicatorID] = mapEvtMsg;
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_SaveEvtMsgTemplate_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nIndID = *(int*)pRecvData->pData;

			std::map<int, EventMessageTemplate> mapEvtMsg;
			for ( UINT i = 0; i < m_vCurEvtMsg.size(); i++ )
			{
				EventMessageTemplate msg = m_vCurEvtMsg[i];
				mapEvtMsg[msg.nRiskLevelID] = msg;
			}

			m_mapEvtMsgTemplate[nIndID] = mapEvtMsg;
			MessageBox(_T("保存成功！"), lpszPrompt, MB_ICONINFORMATION);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else
	{

	}

	return 0;
}

bool CRiskMsgTemplateDlg::FindEvtMsgTemplate( int nIndicatorID, int nLevelID, EventMessageTemplate& msg )
{
	std::map<int, std::map<int, EventMessageTemplate>>::iterator it_ind
		= m_mapEvtMsgTemplate.find(m_nSelIndID);
	if ( it_ind == m_mapEvtMsgTemplate.end())
	{
		return false;
	}
	else
	{
		std::map<int, EventMessageTemplate>::iterator it_level 
			= it_ind->second.find(nLevelID);
		if ( it_level == it_ind->second.end())
		{
			return false;
		}
		else
		{
			msg = it_level->second;
			return true;
		}
	}
}

void CRiskMsgTemplateDlg::OnBnClickedButtonMsgSave()
{
	// TODO: Add your control notification handler code here
	if ( m_nSelIndID < 0 )
	{
		MessageBox(_T("请先选择一个风险指标！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	m_vCurEvtMsg.clear();
	int nCount = m_listMsg.GetItemCount();
	bool bHasEmptyRow = false;
	for ( int i = 0; i < nCount; i++ )
	{
		CString csTitle = m_listMsg.GetItemText(i, 2);
		CString csContent = m_listMsg.GetItemText(i, 3);
		if ( csTitle.IsEmpty() || csContent.IsEmpty() )
		{
			bHasEmptyRow = true;
			continue;
		}

		if ( csTitle.GetLength() > sizeof(MsgTitle)-1)
		{
			MessageBox(_T("标题内容过长，请重新输入！"), lpszPrompt, MB_ICONERROR);
			m_listMsg.SelectRow(i, true);
			m_listMsg.EditCell(i, 2);
			return;
		}

		if ( csContent.GetLength() > sizeof(MsgContent)-1)
		{
			MessageBox(_T("消息内容过长！请重新输入！"), lpszPrompt, MB_ICONERROR);
			m_listMsg.SelectRow(i, true);
			m_listMsg.EditCell(i, 3);
			return;
		}

		EventMessageTemplate msg;
		memset(&msg, 0, sizeof(msg));
		msg.nRiskIndicatorID = (RiskIndicatorType)m_nSelIndID;
		msg.nRiskLevelID = m_listMsg.GetItemData(i);
		strcpy(msg.szTitle, csTitle.GetBuffer(0));
		strcpy(msg.szContent, csContent.GetBuffer(0));
		csTitle.ReleaseBuffer();
		csContent.ReleaseBuffer();

		m_vCurEvtMsg.push_back(msg);
	}

	if ( bHasEmptyRow )
	{
		int nRet = MessageBox(_T("部分未完整填写的消息将不会被保存，是否继续？"), lpszPrompt, MB_ICONQUESTION|MB_YESNO);
		if ( nRet != IDYES )
		{
			return;
		}
	}

	//与原来的数据比较，看是否有变化，有变化则提交
	if ( IsDataChanged())
	{
		int nDataLen = sizeof(int)+sizeof(EventMessageTemplate)*m_vCurEvtMsg.size();
		char* pBuffer = new char[nDataLen];
		memset(pBuffer, 0, nDataLen);
		memcpy(pBuffer, &m_nSelIndID, sizeof(EventMessageTemplate));
		if ( !m_vCurEvtMsg.empty() )
		{
			memcpy(pBuffer+sizeof(int), &m_vCurEvtMsg[0], sizeof(EventMessageTemplate)*m_vCurEvtMsg.size());
		}

		if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_SaveEvtMsgTemplate_Req,
			pBuffer, nDataLen))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
			return;
		}
		else
		{
			EnableControls(FALSE);
		}

		delete [] pBuffer;
		pBuffer = NULL;
	}
}

void CRiskMsgTemplateDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

bool CRiskMsgTemplateDlg::IsDataChanged()
{
	std::map<int, EventMessageTemplate> mapEvtMsgTemplate;
	mapEvtMsgTemplate.clear();
	std::map<int, std::map<int, EventMessageTemplate>>::iterator it_ind
		= m_mapEvtMsgTemplate.find(m_nSelIndID);
	if ( it_ind != m_mapEvtMsgTemplate.end())
	{
		mapEvtMsgTemplate = it_ind->second;
	}

	if ( m_vCurEvtMsg.size() != mapEvtMsgTemplate.size() )
	{
		return true;
	}
	else
	{
		for ( UINT i = 0; i < m_vCurEvtMsg.size(); i++ )
		{
			EventMessageTemplate newMsg = m_vCurEvtMsg[i];
			EventMessageTemplate oldMsg;
			if ( !FindEvtMsgTemplate(newMsg.nRiskIndicatorID, newMsg.nRiskLevelID, oldMsg))
			{
				return true;
			}
			else
			{
				if ( 0 != memcmp(&newMsg, &oldMsg, sizeof(EventMessageTemplate)))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}

		return false;
	}
}

void CRiskMsgTemplateDlg::EnableControls( BOOL bFlag )
{
	m_btnSave.EnableWindow(bFlag);
}

CString CRiskMsgTemplateDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-02292")))
	{
		csTempMsg = _T("该消息模版正在被使用，无法删除！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}
