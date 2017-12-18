// IndicatorTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "IndicatorTabDlg.h"
#include "CGridColumnTraitEdit.h"
#include "CGridColumnTraitCombo.h"
#include "CGridColumnTraitText.h"

// CIndicatorTabDlg dialog

IMPLEMENT_DYNAMIC(CIndicatorTabDlg, CDialog)

CIndicatorTabDlg::CIndicatorTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIndicatorTabDlg::IDD, pParent)
	, m_bValid(FALSE)
	, m_nIndicatorID(-1)
	, m_bInit(false)
	, m_bRiskThresholdSortedOrder(false)
{
	m_vCommitData.clear();
	m_vOriginalData.clear();
}

CIndicatorTabDlg::~CIndicatorTabDlg()
{
}

void CIndicatorTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADD_LEVEL, m_btnAddLevel);
	DDX_Control(pDX, IDC_BUTTON_DEL_LEVEL, m_btnDelLevel);
	DDX_Control(pDX, IDC_LIST_IND_LEVEL, m_list);
}


BEGIN_MESSAGE_MAP(CIndicatorTabDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CIndicatorTabDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CIndicatorTabDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LEVEL, &CIndicatorTabDlg::OnBnClickedButtonAddLevel)
	ON_BN_CLICKED(IDC_BUTTON_DEL_LEVEL, &CIndicatorTabDlg::OnBnClickedButtonDelLevel)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_IND_LEVEL, &CIndicatorTabDlg::OnNMDblclkList)
END_MESSAGE_MAP()


// CIndicatorTabDlg message handlers

void CIndicatorTabDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
}

void CIndicatorTabDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CIndicatorTabDlg::SetIndicatorID( int nIndID )
{
	m_nIndicatorID = nIndID;
}

int CIndicatorTabDlg::GetIndicatorID()
{
	return m_nIndicatorID;
}

void CIndicatorTabDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if ( m_bInit )
	{
		CRect rc;
		GetClientRect(&rc);

		CRect rcList(rc);
		rcList.left += 2;
		rcList.right -= 2;
		rcList.top += 2;
		rcList.bottom -= 30;
		m_list.MoveWindow(&rcList);

		CRect rcBtn;
		m_btnAddLevel.GetWindowRect(&rcBtn);
		CRect rcTemp;
		rcTemp.left = rc.left + 90;
		rcTemp.right = rcTemp.left + rcBtn.Width();
		rcTemp.top = rc.bottom - 25;
		rcTemp.bottom = rcTemp.top + rcBtn.Height();
		m_btnAddLevel.MoveWindow(&rcTemp);

		rcTemp.left = rcTemp.left + 130;
		rcTemp.right = rcTemp.left + rcBtn.Width();
		rcTemp.top = rc.bottom - 25;
		rcTemp.bottom = rcTemp.top + rcBtn.Height();
		m_btnDelLevel.MoveWindow(&rcTemp);
	}
}

BOOL CIndicatorTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bInit = true;

	m_list.InsertHiddenLabelColumn();
	CGridColumnTraitText* pTextTrait = new CGridColumnTraitText;
	m_list.InsertColumnTrait(1, _T("风险级别"), LVCFMT_RIGHT, 120, 0, pTextTrait);
	CGridColumnTraitEdit* pTrait = new CGridColumnTraitEdit;
	m_list.InsertColumnTrait(2, _T("阀值"), LVCFMT_RIGHT, 120, 0, pTrait);
	CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;
	pComboTrait->AddItem((DWORD_PTR)WarningType_Beep, lpszPromtAction[WarningType_Beep]);
	pComboTrait->AddItem((DWORD_PTR)WarningType_Vibrate, lpszPromtAction[WarningType_Vibrate]);
	pComboTrait->AddItem((DWORD_PTR)WarningType_Both, lpszPromtAction[WarningType_Both]);
	pComboTrait->SetStyle(pComboTrait->GetStyle()|CBS_DROPDOWNLIST);
	m_list.InsertColumnTrait(3, _T("提示方式"), LVCFMT_LEFT, 120, 0, pComboTrait);

	m_list.SetCellMargin(1.5);

	m_list.EnableWindow(m_bValid);
	m_btnAddLevel.EnableWindow(m_bValid);
	m_btnDelLevel.EnableWindow(m_bValid);

	return TRUE;
}

bool CIndicatorTabDlg::RetrieveData( bool bPrompt )
{
	// TODO: Add your control notification handler code here
	m_vCommitData.clear();
	int nCount = m_list.GetItemCount();
	float fTempValue = 0;
	for ( int i = 0; i < nCount; i++ )
	{
		CString csValue = m_list.GetItemText(i, 2);
		if ( csValue.IsEmpty() )
		{
			if ( bPrompt )
			{
				MessageBox(_T("风险阀值不能为空！"), lpszPrompt, MB_ICONERROR);
				m_list.SelectRow(i, true);
				m_list.EditCell(i, 2);
			}

			return false;
		}

		float fValue = (float)atof(csValue.GetBuffer(0));
		csValue.ReleaseBuffer();
		if ( fValue < 0.00001 || fValue > 100 )
		{
			if ( bPrompt )
			{
				MessageBox(_T("风险阀值范围应为0-100之间！"), lpszPrompt, MB_ICONERROR);
				m_list.SelectRow(i, true);
				m_list.EditCell(i, 2);
			}

			return false;
		}

		//检查风险阀值
		if ( i != 0 )
		{
			if ( m_bRiskThresholdSortedOrder )
			{
				if ( fValue <= fTempValue )
				{
					if ( bPrompt )
					{
						MessageBox(_T("风险阀值必须按风险等级递增！"), lpszPrompt, MB_ICONERROR);
						m_list.SelectRow(i, true);
						m_list.EditCell(i, 2);
					}

					return false;
				}
			}
			else
			{
				if ( fValue >= fTempValue )
				{
					if ( bPrompt )
					{
						MessageBox(_T("风险阀值必须按风险等级递减！"), lpszPrompt, MB_ICONERROR);
						m_list.SelectRow(i, true);
						m_list.EditCell(i, 2);
					}

					return false;
				}
			}
		}

		fTempValue = fValue;

		IndLevelWarning levelWarning;
		memset(&levelWarning, 0, sizeof(levelWarning));
		levelWarning.nRiskIndicatorID = (RiskIndicatorType)m_nIndicatorID;
		levelWarning.nRiskLevelID = i+1;
		levelWarning.fThresholdValue = fValue;
		levelWarning.nColor = m_list.GetCellTextColor(i, 1);
		
		CString csPrompt = m_list.GetItemText(i, 3);
		if ( csPrompt == lpszPromtAction[WarningType_Beep])
		{
			levelWarning.nResponseType = WarningType_Beep;
		}
		else if ( csPrompt == lpszPromtAction[WarningType_Vibrate])
		{
			levelWarning.nResponseType = WarningType_Vibrate;
		}
		else if ( csPrompt == lpszPromtAction[WarningType_Both])
		{
			levelWarning.nResponseType = WarningType_Both;
		}
		else
		{

		}

		m_vCommitData.push_back(levelWarning);
	}

	return true;
}

void CIndicatorTabDlg::OnNMDblclkList( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( pNMItemActivate->iItem != -1 && pNMItemActivate->iSubItem == 1)
	{
		CColorDialog dlg;
		if( IDOK == dlg.DoModal())
		{
			COLORREF clr = dlg.GetColor();
			m_list.SetCellTextColor(pNMItemActivate->iItem, pNMItemActivate->iSubItem, clr);
		}
	}
}

void CIndicatorTabDlg::SetWarningData( const std::vector<IndLevelWarning>& vRiskWarning )
{
	m_vOriginalData = vRiskWarning;
	m_list.DeleteAllItems();
	m_list.ResetCellTextColor();

	for ( UINT i = 0; i < vRiskWarning.size(); i++ )
	{
		IndLevelWarning levelWarning = vRiskWarning[i];
		int nCount = m_list.GetItemCount();
		m_list.InsertItem(nCount, _T(""));
		CString csLevel;
		csLevel.Format(_T("第%d级"), levelWarning.nRiskLevelID);
		m_list.SetItemText(nCount, 1,csLevel);
		m_list.SetCellTextColor(nCount, 1, levelWarning.nColor);
		CString csValue;
		csValue.Format(_T("%.2f"), levelWarning.fThresholdValue);
		m_list.SetItemText(nCount, 2, csValue);
		m_list.SetItemText(nCount, 3, lpszPromtAction[levelWarning.nResponseType]);
	}
}

const std::vector<IndLevelWarning>& CIndicatorTabDlg::GetWarningData()
{
	return m_vCommitData;
}

void CIndicatorTabDlg::SetValid( BOOL bValid )
{
	m_bValid = bValid;
	m_list.EnableWindow(m_bValid);
	m_btnAddLevel.EnableWindow(m_bValid);
	m_btnDelLevel.EnableWindow(m_bValid);
}

BOOL CIndicatorTabDlg::IsValid()
{
	return m_bValid;
}

void CIndicatorTabDlg::AddEmptyRow()
{
	//默认添加一行数据
	int nCount = m_list.GetItemCount();
	m_list.InsertItem(nCount, _T(""));
	CString csLevel;
	csLevel.Format(_T("第%d级"), nCount+1);
	m_list.SetItemText(nCount, 1,csLevel);
	m_list.SetItemText(nCount, 2, _T(""));
	m_list.SetItemText(nCount, 3, lpszPromtAction[WarningType_Both]);

	if ( nCount == 0 )
	{
		m_list.SetCellTextColor(nCount, 1, RGB(255, 0, 0));
	}
}

void CIndicatorTabDlg::OnBnClickedButtonAddLevel()
{
	AddEmptyRow();
}

void CIndicatorTabDlg::OnBnClickedButtonDelLevel()
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_list.GetNextSelectedItem(pos);
	m_list.DeleteItem(nSel);
	for ( int i = nSel; i < m_list.GetItemCount(); i++ )
	{
		CString csLevel;
		csLevel.Format(_T("第%d级"), i+1);
		m_list.SetItemText(i, 1,csLevel);
	}
}

void CIndicatorTabDlg::ResetData()
{
	m_bValid = FALSE;
	m_list.EnableWindow(m_bValid);
	m_btnAddLevel.EnableWindow(m_bValid);
	m_btnDelLevel.EnableWindow(m_bValid);

	m_vOriginalData.clear();

	m_list.DeleteAllItems();
	m_list.ResetCellTextColor();
}

bool CIndicatorTabDlg::IsDataChanged()
{
	if ( m_vOriginalData.size() != m_vCommitData.size() )
	{
		return true;
	}

	for ( UINT i = 0; i < m_vOriginalData.size(); i++ )
	{
		if ( 0 != memcmp(&m_vOriginalData[i], &m_vCommitData[i], sizeof(IndLevelWarning)))
		{
			return true;
		}
	}

	return false;
}

void CIndicatorTabDlg::EnableControls( BOOL bFlag )
{
	m_list.EnableWindow(bFlag);
	m_btnAddLevel.EnableWindow(bFlag);
	m_btnDelLevel.EnableWindow(bFlag);
}

void CIndicatorTabDlg::SetSortedOrder( bool bSortedOrder )
{
	m_bRiskThresholdSortedOrder = bSortedOrder;
}
