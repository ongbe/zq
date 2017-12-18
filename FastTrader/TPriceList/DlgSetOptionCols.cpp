// DlgSetOptionCols.cpp : implementation file
//

#include "stdafx.h"
#include "TPriceList.h"
#include "DlgSetOptionCols.h"


// CDlgSetOptionCols dialog

IMPLEMENT_DYNAMIC(CDlgSetOptionCols, CDialog)

CDlgSetOptionCols::CDlgSetOptionCols(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetOptionCols::IDD, pParent)
{

}

CDlgSetOptionCols::~CDlgSetOptionCols()
{
}

void CDlgSetOptionCols::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COL, m_ocxListCols);
}


BEGIN_MESSAGE_MAP(CDlgSetOptionCols, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSetOptionCols::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSetOptionCols::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_UP, &CDlgSetOptionCols::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWM, &CDlgSetOptionCols::OnBnClickedBtnDowm)
END_MESSAGE_MAP()


// CDlgSetOptionCols message handlers

BOOL CDlgSetOptionCols::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_ocxListCols.SetExtendedStyle(LVS_EX_GRIDLINES | 
			LVS_EX_FULLROWSELECT | LVS_EX_SIMPLESELECT | LVS_EX_HIDELABELS 
			| LVS_EX_CHECKBOXES);// | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);

	m_ocxListCols.InsertColumn(0, "", 0, 20);
	m_ocxListCols.InsertColumn(1, "列名", 0, 81);

	CenterWindow();

	ShowCols();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetOptionCols::ShowCols()
{
	std::vector<int> vecCol;
	std::map<int, std::string> mapColName;
	std::set<int> setShowCol;
	std::map<int, std::string>::iterator it;
	CString strText;
	int nRetItem = -1;
	
	LockGlobalObject();
	vecCol = GetTargetShowCol();
	mapColName = GetTargetColMap();
	UnlockGlobalObject();

	for(int i=0; i<(int)vecCol.size(); i++) {
		setShowCol.insert(vecCol[i]);
		if((it = mapColName.find(vecCol[i])) == mapColName.end())
			continue;
		nRetItem = m_ocxListCols.InsertItem(m_ocxListCols.GetItemCount(), "");
		strText.Format(TEXT("%s"), it->second.c_str());
		m_ocxListCols.SetItemText(nRetItem, 1, strText);
		m_ocxListCols.SetItemData(nRetItem, it->first);
		m_ocxListCols.SetCheck(nRetItem);
	}

	for(it = mapColName.begin(); it != mapColName.end(); it++) {
		if(setShowCol.find(it->first)!=setShowCol.end()) 
			continue;
		nRetItem = m_ocxListCols.InsertItem(m_ocxListCols.GetItemCount(), "");
		strText.Format(TEXT("%s"), it->second.c_str());
		m_ocxListCols.SetItemText(nRetItem, 1, strText);
		m_ocxListCols.SetItemData(nRetItem, it->first);
	}
}

void CDlgSetOptionCols::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	std::vector<int> vecCol;

	for(int i=0; i<m_ocxListCols.GetItemCount(); i++) {
		if(m_ocxListCols.GetCheck(i)!=0 && m_ocxListCols.GetItemData(i)!=0)
			vecCol.push_back(m_ocxListCols.GetItemData(i));
	}

	std::vector<int> vecTargetCol;

	//vecTargetCol.push_back(QUOT_InstrumentID);
	for(int i=0; i<(int)vecCol.size(); i++) {
		vecTargetCol.push_back(vecCol[i]);
	}

	std::vector<int> vecOptionCol;
	int nNewHead = QUOTNEWHEAD;

	//for(int i=(int)vecCol.size()-1; i>=0; i--) {
	for(int i=0; i<(int)vecCol.size(); i++) {
		vecOptionCol.push_back(vecCol[i]);
	}
	vecOptionCol.push_back(QUOT_OptionPrice);
	//for(int i=0; i<(int)vecCol.size(); i++) {
	for(int i=(int)vecCol.size()-1; i>=0; i--) {
		vecOptionCol.push_back(nNewHead + vecCol[i]);
	}

	LockGlobalObject();
	SetTargetShowCol(vecTargetCol);
	SetOptionShowCol(vecOptionCol);
	UnlockGlobalObject();

	//GetParent()->PostMessage(GetColSetChangedMSGID(), 0, NULL);

	OnOK();
}

void CDlgSetOptionCols::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDlgSetOptionCols::SwapTwoRow(int nItem, int nItemTow)
{
	BOOL bCheckBak = FALSE;
	CString strTextBak;
	int nItemDataBak = -1;

	bCheckBak = m_ocxListCols.GetCheck(nItem);
	m_ocxListCols.SetCheck(nItem, m_ocxListCols.GetCheck(nItemTow));
	m_ocxListCols.SetCheck(nItemTow, bCheckBak);

	nItemDataBak = m_ocxListCols.GetItemData(nItem);
	m_ocxListCols.SetItemData(nItem, m_ocxListCols.GetItemData(nItemTow));
	m_ocxListCols.SetItemData(nItemTow, nItemDataBak);

	strTextBak = m_ocxListCols.GetItemText(nItem, 1);
	m_ocxListCols.SetItemText(nItem, 1, m_ocxListCols.GetItemText(nItemTow, 1));
	m_ocxListCols.SetItemText(nItemTow, 1, strTextBak);
}

void CDlgSetOptionCols::OnBnClickedBtnUp()
{
	// TODO: Add your control notification handler code here

	POSITION pos = m_ocxListCols.GetFirstSelectedItemPosition();
	if(pos == NULL) {
		AfxMessageBox("请选择一行");
		return;
	}
	int nItem = m_ocxListCols.GetNextSelectedItem(pos);
	if(nItem <= 1) // 第一行、第二行不处理
		return;

	SwapTwoRow(nItem, nItem-1);
	m_ocxListCols.SetItemState(nItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_ocxListCols.SetItemState(nItem-1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CDlgSetOptionCols::OnBnClickedBtnDowm()
{
	// TODO: Add your control notification handler code here

	POSITION pos = m_ocxListCols.GetFirstSelectedItemPosition();
	if(pos == NULL) {
		AfxMessageBox("请选择一行");
		return;
	}
	int nItem = m_ocxListCols.GetNextSelectedItem(pos);
	if(nItem == m_ocxListCols.GetItemCount()-1) // 最后一行不处理
		return;

	SwapTwoRow(nItem, nItem+1);
	m_ocxListCols.SetItemState(nItem, 0, LVIS_SELECTED | LVIS_FOCUSED);
	m_ocxListCols.SetItemState(nItem+1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	//int nDownItem = nItem+1;
	//BOOL bCheckBak = FALSE;
	//CString strTextBak;
	//int nItemDataBak = -1;

	//bCheckBak = m_ocxListCols.GetCheck(nItem);
	//m_ocxListCols.SetCheck(nItem, m_ocxListCols.GetCheck(nDownItem));
	//m_ocxListCols.SetCheck(nDownItem, bCheckBak);

	//nItemDataBak = m_ocxListCols.GetItemData(nItem);
	//m_ocxListCols.SetItemData(nItem, m_ocxListCols.GetItemData(nDownItem));
	//m_ocxListCols.SetItemData(nDownItem, nItemDataBak);

	//strTextBak = m_ocxListCols.GetItemText(nItem, 1);
	//m_ocxListCols.SetItemText(nItem, m_ocxListCols.GetItemText(nDownItem, 1));
	//m_ocxListCols.SetItemText(nDownItem, strTextBak);
}
