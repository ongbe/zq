// clsInstrumentIDListDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "clsInstrumentIDListDialog.h"


// CInstrumentIDListDialog dialog

IMPLEMENT_DYNAMIC(CInstrumentIDListDialog, CDialog)

CInstrumentIDListDialog::CInstrumentIDListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInstrumentIDListDialog::IDD, pParent)
{
	m_nCodeHeadsCount = 0;
	m_ptCodeHeads = NULL;
	m_pButtonGroup = NULL;
	m_poOutputEditInstrumentID = NULL;
}

CInstrumentIDListDialog::~CInstrumentIDListDialog()
{
	for(int i=0; i<m_nCodeHeadsCount; i++) {
		if(m_pButtonGroup[i]!=NULL)
		{
			delete m_pButtonGroup[i];
			m_pButtonGroup[i] = NULL;
		}
	}
	if(m_pButtonGroup!=NULL)
	{
		delete m_pButtonGroup;
		m_pButtonGroup = NULL;
	}
	if(m_ptCodeHeads!=NULL)
	{
		delete m_ptCodeHeads;
		m_ptCodeHeads = NULL;
	}
}

void CInstrumentIDListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INSTRUMENTIDLIST, m_ctlInstrumentIDList);
}

BOOL CInstrumentIDListDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message == WM_LBUTTONUP) //WM_LBUTTONDOWN
	{ 
		if(m_ctlInstrumentIDList.IsWindowVisible())
		{
			CRect rect;                                                                
			m_ctlInstrumentIDList.GetWindowRect(&rect); 
			if(rect.PtInRect(pMsg->pt)) 
			{ 
				OnClick_InstrumentIDList();
			}
		} 
	}
	else if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->hwnd==m_ctlInstrumentIDList.GetSafeHwnd() 
				&& m_ctlInstrumentIDList.IsWindowVisible())
		{
			OnKeyPress_InstrumentIDList((UINT)pMsg->wParam);
		}
	}
	 
	return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CInstrumentIDListDialog, CDialog)
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_INSTRUMENTID_GROUP, &CInstrumentIDListDialog::OnBnClickedInstrumentidGroup)
	ON_LBN_SELCHANGE(IDC_INSTRUMENTIDLIST, &CInstrumentIDListDialog::OnLbnSelchangeInstrumentidlist)
END_MESSAGE_MAP()


// CInstrumentIDListDialog message handlers

BOOL CInstrumentIDListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_nCodeHeadsCount = 0;
	m_ptCodeHeads = new CODEHEADDATA[conMaxButtonNum];
	ZeroMemory(m_ptCodeHeads, sizeof(CODEHEADDATA)*conMaxButtonNum);

	CString strShortNames = GetProductShortNames();
	int nMaxShortNameLen = 0;
	
	char* pHead = strShortNames.GetBuffer(0);
	char* pHeadNext = NULL;
	BOOL bRetHasData = FALSE;
	do 
	{
		pHeadNext = GetProductSNameItem(pHead, m_ptCodeHeads[m_nCodeHeadsCount], bRetHasData);
		if(bRetHasData) {
			if(nMaxShortNameLen < strlen(m_ptCodeHeads[m_nCodeHeadsCount].strCodeName))
				nMaxShortNameLen = strlen(m_ptCodeHeads[m_nCodeHeadsCount].strCodeName);
		}
#ifdef _USE_COMBOINSTRUMENT
		if(bRetHasData)
#else
		if(bRetHasData && m_ptCodeHeads[m_nCodeHeadsCount].cType=='1')
#endif
		{
			m_nCodeHeadsCount++;
			if(m_nCodeHeadsCount>=conMaxButtonNum) 
			{
				m_nCodeHeadsCount=conMaxButtonNum;
				break;
			}
		}
		if(pHeadNext!=NULL)
			pHead = pHeadNext;
	} while(pHeadNext!=NULL);

	m_mapCodeHead2NameIndex.clear();
	for(int i=0; i<m_nCodeHeadsCount; i++)
	{
		for(int k=0; k<m_ptCodeHeads[i].nCmbCodeCount; k++)
		{
			CString strItem(m_ptCodeHeads[i].strCodeHeads[k]);
			strItem.MakeLower();
			m_mapCodeHead2NameIndex.insert(
					std::pair<std::string, int>(std::string(strItem), i));
		}
	}

	int nBWidth = nMaxShortNameLen*6;//52;//46;

	//if(g_pPlatform&&g_pPlatform->GetPlatformId()==PID_PATSAPI) {
	//	nBWidth = 72;
	//}

	m_pButtonGroup = new CButton*[m_nCodeHeadsCount];
	ZeroMemory(m_pButtonGroup, sizeof(CButton*)*m_nCodeHeadsCount);
	for(int i=0; i<m_nCodeHeadsCount; i++) {
		m_pButtonGroup[i] = new CButton();
		m_pButtonGroup[i]->Create(m_ptCodeHeads[i].strCodeName, 
				BS_NOTIFY, //BS_FLAT|
				CRect((i%conEachRowBtnNum)*nBWidth, (i/conEachRowBtnNum)*20, (i%conEachRowBtnNum+1)*nBWidth, (i/conEachRowBtnNum+1)*20), 
				this, IDC_INSTRUMENTID_GROUP);
		m_pButtonGroup[i]->SetFont(GetFont());
		m_pButtonGroup[i]->ShowWindow(SW_SHOW);
		//::SetWindowLong(m_pButtonGroup[i]->GetSafeHwnd(), GWL_STYLE, 
		//		::GetWindowLong(m_pButtonGroup[i]->GetSafeHwnd(), GWL_STYLE) | BS_OWNERDRAW);
	}

	CRect rect;
	GetWindowRect(&rect);
	rect.right = rect.left + nBWidth*conEachRowBtnNum + 2;
	rect.bottom = rect.top + 20*((m_nCodeHeadsCount+2)/conEachRowBtnNum) + 2;
	MoveWindow(&rect);

	rect.left = 0;
	rect.top = 0;
	rect.right = rect.left + nBWidth*conEachRowBtnNum - 2;
	rect.bottom = rect.top + 20*((m_nCodeHeadsCount+2)/conEachRowBtnNum) - 2;
	m_ctlInstrumentIDList.MoveWindow(&rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInstrumentIDListDialog::ShowView(CString& strKeyInputReal, BOOL bShowCmd)
{
	CString strKeyInput=strKeyInputReal.MakeLower();

	if(!bShowCmd) {

		CString strSrc;
		std::set<int> setHeads;
		
		for(int i=0; i<m_nCodeHeadsCount; i++)
		{
			strSrc = m_ptCodeHeads[i].strCodeNameJP;
			strSrc.MakeLower();
			if(strSrc.Find(strKeyInput)!=-1)
				setHeads.insert(i);
		}

		CString strItem;
		std::set<std::string>& setIDs = GetInstrumentIDList();
		std::set<std::string>::iterator itID;
		int nCurrIndex = -1;
		m_ctlInstrumentIDList.ResetContent();
		for(std::set<int>::iterator it = setHeads.begin();
				it != setHeads.end(); it++)
		{
			nCurrIndex = *it;
			itID = setIDs.begin();
			for(; itID!=setIDs.end(); itID++)
			{
				strSrc=(*itID).c_str();
				strItem = strSrc;
				strItem.MakeLower();
				for(int k=0; k<m_ptCodeHeads[nCurrIndex].nCmbCodeCount; k++)
				{
					int nHeadLen = strlen(m_ptCodeHeads[nCurrIndex].strCodeHeads[k]);
					if(strItem.GetLength()<=nHeadLen)
						continue;
					if(!(strItem[nHeadLen] >= '0' && strItem[nHeadLen] <= '9'))
						continue;
					if(strnicmp(strItem, m_ptCodeHeads[nCurrIndex].strCodeHeads[k], 
							strlen(m_ptCodeHeads[nCurrIndex].strCodeHeads[k]))==0)
					{
						m_ctlInstrumentIDList.AddString(
								strSrc + " (" + m_ptCodeHeads[nCurrIndex].strCodeName + ")");
					}
				}
			}
		}
		CString strNewHead;
		for(int i=0; i<strKeyInput.GetLength(); i++)
		{
			if(strKeyInput[i]>='0' && strKeyInput[i]<='9' && strKeyInput[i]!=' ')
				break;
			strNewHead += strKeyInput[i];
		}
		std::map<std::string, int>::iterator itIndex = 
				m_mapCodeHead2NameIndex.find(std::string(strNewHead));
		if(itIndex != m_mapCodeHead2NameIndex.end())
		{
			nCurrIndex = itIndex->second;
			std::set<int>::iterator it = setHeads.find(nCurrIndex);
			if(it==setHeads.end())
			{
				itID = setIDs.begin();
				for(; itID!=setIDs.end(); itID++)
				{
					strSrc=(*itID).c_str();
					strItem=strSrc;
					strItem.MakeLower();
					if(strItem.Find(strKeyInput)!=-1)
					{

						m_ctlInstrumentIDList.AddString(
								strSrc + " (" + m_ptCodeHeads[nCurrIndex].strCodeName + ")");
					}
				}
			}
		}
		for(int i=0; i<m_nCodeHeadsCount; i++)
		{
			m_pButtonGroup[i]->ShowWindow(SW_HIDE);
		}
		m_ctlInstrumentIDList.ShowWindow(SW_SHOW);
		m_bHasUserChange = TRUE;
	}
	else {
		for(int i=0; i<m_nCodeHeadsCount; i++)
		{
			m_pButtonGroup[i]->ShowWindow(SW_SHOW);
		}
		m_ctlInstrumentIDList.ShowWindow(SW_HIDE);
		m_bHasUserChange = FALSE;
	}

}

void CInstrumentIDListDialog::OnBnClickedInstrumentidGroup()
{
	// TODO: Add your control notification handler code here
	CWnd* pFocus = NULL;
	BOOL bFound = FALSE;
	int nCurrIndex = -1;
	pFocus = GetFocus();
	for(int i=0; i<m_nCodeHeadsCount; i++)
	{
		if(m_pButtonGroup[i]==pFocus)
		{
			nCurrIndex = i;
			bFound = TRUE;
			break;
		}
	}
	if(!bFound)
		return;

	CString strItem;
	std::set<std::string> setIDs = GetInstrumentIDList();
	std::set<std::string>::iterator it = setIDs.begin();
	m_ctlInstrumentIDList.ResetContent();
	for(; it!=setIDs.end(); it++)
	{
		strItem=(*it).c_str();
		for(int k=0; k<m_ptCodeHeads[nCurrIndex].nCmbCodeCount; k++)
		{
			int nHeadLen = strlen(m_ptCodeHeads[nCurrIndex].strCodeHeads[k]);
			if(strItem.GetLength()<=nHeadLen)
				continue;
			if(!(strItem[nHeadLen] >= '0' && strItem[nHeadLen] <= '9'))
				continue;
			if(strnicmp(strItem, m_ptCodeHeads[nCurrIndex].strCodeHeads[k], 
					strlen(m_ptCodeHeads[nCurrIndex].strCodeHeads[k]))==0)
			{
				m_ctlInstrumentIDList.AddString(
						strItem + " (" + m_ptCodeHeads[nCurrIndex].strCodeName + ")");
			}
		}
	}
	for(int i=0; i<m_nCodeHeadsCount; i++)
	{
		m_pButtonGroup[i]->ShowWindow(SW_HIDE);
	}
	m_ctlInstrumentIDList.ShowWindow(SW_SHOW);
	if(m_ctlInstrumentIDList.GetCount()>0)
		m_ctlInstrumentIDList.SetCurSel(0);
	m_ctlInstrumentIDList.SetFocus();
}

void CInstrumentIDListDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(!bShow)
		m_poOutputEditInstrumentID = NULL;
}

void CInstrumentIDListDialog::OnLbnSelchangeInstrumentidlist()
{
	// TODO: Add your control notification handler code here
	//CListBox* poCurrList = &m_ctlInstrumentIDList;

	//int nFlag = 0;
	//long nSubItem = -1;
	//CString strItem;
	//poCurrList->GetText(poCurrList->GetCurSel(), strItem);
	//char strTemp[128];
	//ZeroMemory(strTemp, 128);
	//sscanf_s(strItem, "%[^ ]s", strTemp, sizeof(strTemp)-1);
	//if(m_poOutputEditInstrumentID!=NULL)
	//{
	//	m_poOutputEditInstrumentID->SetSel(0, -1);
	//	m_poOutputEditInstrumentID->ReplaceSel(strTemp);
	//}
	//ShowWindow(SW_HIDE);
}

void CInstrumentIDListDialog::OnClick_InstrumentIDList()
{
	CListBox* poCurrList = &m_ctlInstrumentIDList;
	int nSel = poCurrList->GetCurSel();
	if(nSel==-1)
		return;

	int nFlag = 0;
	long nSubItem = -1;
	CString strItem;
	poCurrList->GetText(nSel, strItem);
	char strTemp[128];
	ZeroMemory(strTemp, 128);
	sscanf_s(strItem, "%[^ ]s", strTemp, sizeof(strTemp)-1);
	if(m_poOutputEditInstrumentID!=NULL)
	{
		m_poOutputEditInstrumentID->SetSel(0, -1);
		m_poOutputEditInstrumentID->ReplaceSel(strTemp);
	}
	ShowWindow(SW_HIDE);
}

void CInstrumentIDListDialog::OnKeyPress_InstrumentIDList(UINT nKeyNum)
{
	if(nKeyNum==VK_RETURN)
	{
		OnClick_InstrumentIDList();
	}
}

void CInstrumentIDListDialog::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	if(m_ctlInstrumentIDList.IsWindowVisible())
	{
		m_ctlInstrumentIDList.SetFocus();
		if(m_ctlInstrumentIDList.GetCount()>0)
			m_ctlInstrumentIDList.SetCurSel(0);
	}
	else
	{
		m_pButtonGroup[0]->SetFocus();
		m_pButtonGroup[0]->SetButtonStyle(BS_DEFPUSHBUTTON);
	}
}

void CInstrumentIDListDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}

HBRUSH CInstrumentIDListDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	//if(pWnd->GetDlgCtrlID() == IDC_INSTRUMENTID_GROUP) {
	//	CString strText;
	//	CRect rect;
	//	DWORD dwColor = RGB(255, 0, 0);

	//	pWnd->GetWindowText(strText);
	//	pWnd->GetClientRect(&rect);
	//	pDC->SetTextColor(RGB(255, 255, 255));
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->DrawText(strText, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	////	//for(int i=0; i<m_nCodeHeadsCount; i++) {
	////	//	if(pWnd==m_pButtonGroup[i])
	////	//		dwColor = m_pButtonColor[i];
	////	//}
	//	return CreateSolidBrush(dwColor);
	//}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
