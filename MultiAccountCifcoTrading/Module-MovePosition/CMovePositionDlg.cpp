
#include "stdafx.h"
#include "CMovePositionDlg.h"
#include "../inc/Module-Misc/tools_util.h"
#include "CNumericTextCtrl.h"
#include "CRadioButtonGroup.h"
#include <wx/spinctrl.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include "../inc/Module-Misc/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;


void tokenize_str(const std::string & str, std::vector<std::string> &vecResult, const std::string & delims=", \t")
{
	using namespace std;
	// Skip delims at beginning, find start of first token
	string::size_type lastPos = str.find_first_not_of(delims, 0);
	// Find next delimiter @ end of token
	string::size_type pos     = str.find_first_of(delims, lastPos);

	// output vector
	//vector<string> tokens;

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		vecResult.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delims.  Note the "not_of". this is beginning of token
		lastPos = str.find_first_not_of(delims, pos);
		// Find next delimiter at end of token.
		pos     = str.find_first_of(delims, lastPos);
	}

	//return tokens;
}

BEGIN_EVENT_TABLE(CMovePositionDlg, wxDialog)
	EVT_CLOSE(OnClose)
//	EVT_CHAR_HOOK(OnHookKeyPress)
    EVT_COMMAND(wxID_ANY, wxEVT_MOVEPOSITION_TIMER, OnRcvTimer)
	EVT_COMMAND(wxID_ANY, wxEVT_MOVEPOSITION_ORDERLOG, OnRcvOrderLog)
	EVT_COMMAND(wxID_ANY, wxEVT_MOVEPOSITION_TRADELOG, OnRcvTradeLog)
	EVT_COMMAND(wxID_ANY, wxEVT_MOVEPOSITION_ACTION, OnRcvAction)
	EVT_BUTTON(ID_WXBUTTON1,OnSetDefault)
	EVT_BUTTON(ID_WXBUTTON2,OnStart)
	EVT_BUTTON(ID_WXBUTTONEDIT,OnEdit)
	EVT_BUTTON(ID_WXBUTTON3,OnCancel)
	EVT_BUTTON(ID_WXBUTTONCLOSE,OnBtnClose)
	EVT_COMBOBOX(ID_WXCOMBOBOX1, OnComboboxSelectChange)
	EVT_COMBOBOX(ID_WXCOMBOBOX3, OnComboboxSelectChange)
	EVT_COMBOBOX(ID_WXCOMBOBOX4, OnComboboxSelectChange)
	EVT_CHILD_FOCUS(OnChildFocus)
	EVT_SPIN_UP(ID_WXSPINCTRL1, OnSpinUpPress)
	EVT_SPIN_DOWN(ID_WXSPINCTRL1, OnSpinDownPress)
	EVT_SPIN_UP(ID_WXSPINCTRL2, OnSpinUpPress)
	EVT_SPIN_DOWN(ID_WXSPINCTRL2, OnSpinDownPress)
	EVT_COMMAND_RANGE(ID_WXRADIOBUTTON1,ID_WXRADIOBUTTON8,wxEVT_COMMAND_RADIOBUTTON_SELECTED,OnRadioButton)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////////////////
CMovePositionDlg::CMovePositionDlg(wxWindow *parent)
	:wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(212,438)),m_pModifyOrderDlg(NULL),
	m_pCustomSpinCtrl1(NULL),m_pCustomSpinCtrl2(NULL),m_pRadioGroupOriginal(NULL),m_pRadioGroupDest(NULL),
	m_pCNumericTextCtrl1(NULL),m_pCNumericTextCtrl2(NULL),m_pCNumericTextCtrl3(NULL),m_pCNumericTextCtrl4(NULL),
	m_pCNumericTextCtrl5(NULL),m_pCNumericTextCtrl6(NULL),m_pComboBox1(NULL),m_pComboBox2(NULL),m_pComboBox3(NULL),
	m_pComboBox4(NULL),m_bRunning(FALSE)
{
	DIALOG_CREATE_LOG();
	m_pModifyOrderDlg = new CModifyOrderDlg(NULL);
	m_pModifyOrderDlg->Show(false);

	m_pRadioGroupOriginal = new CRadioButtonGroup;
	m_pRadioGroupDest = new CRadioButtonGroup;

	ZeroMemory(&m_tMove, sizeof(m_tMove));
	ZeroMemory(&m_curOrderOption, sizeof(m_curOrderOption));
	m_poMovePositionServiceThread = NULL;
	
	AUTO_SETSIZE();
	CreateGUIControls();
	SetIcon(wxNullIcon);
	Center();
}

CMovePositionDlg::~CMovePositionDlg()
{
	if (m_pRadioGroupDest)
	{
		delete m_pRadioGroupDest;
		m_pRadioGroupDest = NULL;
	}
	if (m_pRadioGroupOriginal)
	{
		delete m_pRadioGroupOriginal;
		m_pRadioGroupOriginal = NULL;
	}
	if (m_pModifyOrderDlg)
		m_pModifyOrderDlg->Destroy();
}

// 是否用涨跌停价模拟市价，当交易所不支持市价时
BOOL CMovePositionDlg::NeedSimulateMarketPrice()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bSimulate;
}

void CMovePositionDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

    int nMH = 24;

    wxString strText;
	strText.Printf("%s", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
	wxStaticText* pStaticText0 = new wxStaticText(this, ID_MARKETTYPE_CONFIRMMSG_STATIC, 
			strText, wxPoint(5, 5));

	wxNotebook * WxNotebook1 = new wxNotebook(this, ID_WXNOTEBOOK1, wxPoint(465, 86+nMH), wxSize(322, 125), wxNB_DEFAULT);
	wxPanel * WxNoteBookPage1 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE1, wxPoint(4, 23+nMH), wxSize(322, 125));
	WxNotebook1->AddPage(WxNoteBookPage1, wxT("成交情况"));
	wxPanel * WxNoteBookPage3 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE3, wxPoint(4, 23+nMH), wxSize(322, 125));
	WxNotebook1->AddPage(WxNoteBookPage3, wxT("委托状态"));

	wxNotebook * WxNotebook3 = new wxNotebook(this, ID_WXNOTEBOOK3, wxPoint(465, 281+nMH), wxSize(322, 125), wxNB_DEFAULT);
	wxPanel * WxNoteBookPage4 = new wxPanel(WxNotebook3, ID_WXNOTEBOOKPAGE4, wxPoint(4, 23+nMH), wxSize(322, 125));
	WxNotebook3->AddPage(WxNoteBookPage4, wxT("成交情况"));
	wxPanel * WxNoteBookPage5 = new wxPanel(WxNotebook3, ID_WXNOTEBOOKPAGE5, wxPoint(4, 23+nMH), wxSize(322, 125));
	WxNotebook3->AddPage(WxNoteBookPage5, wxT("委托状态"));

	wxListCtrl * WxListCtrl1 = new wxListCtrl(WxNoteBookPage1, ID_WXLISTCTRL1, wxPoint(0, 7), wxSize(310, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl1"));
	wxListCtrl * WxListCtrl2 = new wxListCtrl(WxNoteBookPage4, ID_WXLISTCTRL2, wxPoint(0, 7), wxSize(310, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl3"));
	new wxListCtrl(WxNoteBookPage3, ID_WXLISTCTRL3, wxPoint(0, 7), wxSize(310, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl2"));
	new wxListCtrl(WxNoteBookPage5, ID_WXLISTCTRL4, wxPoint(0, 7), wxSize(310, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl4"));

 	WxListCtrl2->InsertColumn(0,wxT("状态"),wxLIST_FORMAT_LEFT,50 );
 	WxListCtrl2->InsertColumn(0,wxT("剩余数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("成交数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("成交价"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("报单数"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("委托价格"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("成交编号"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("报单编号"),wxLIST_FORMAT_LEFT,67 );
 
 	WxListCtrl1->InsertColumn(0,wxT("状态"),wxLIST_FORMAT_LEFT,50 );
 	WxListCtrl1->InsertColumn(0,wxT("剩余数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("成交数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("成交价"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("报单数"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("委托价格"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("成交编号"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("报单编号"),wxLIST_FORMAT_LEFT,67 );

	new wxButton(this, ID_WXBUTTON3, wxT("取消"), wxPoint(736, 423+nMH), wxSize(70, 24), 0, wxDefaultValidator, wxT("WxButton3"));
	new wxButton(this, ID_WXBUTTONCLOSE, wxT("关闭"), wxPoint(736, 423+nMH), wxSize(70, 24), 0, wxDefaultValidator, wxT("WxButton3"));
	new wxButton(this, ID_WXBUTTON2, wxT("开始"), wxPoint(370, 423+nMH), wxSize(70, 24), 0, wxDefaultValidator, wxT("WxButton2"));
	new wxButton(this, ID_WXBUTTONEDIT, wxT("编辑"), wxPoint(370, 423+nMH), wxSize(70, 24), 0, wxDefaultValidator, wxT("WxButton2"));
	new wxButton(this, ID_WXBUTTON1, wxT("重置"), wxPoint(261, 423+nMH), wxSize(70, 24), 0, wxDefaultValidator, wxT("WxButton1"));

	new wxStaticBox(this, ID_WXSTATICBOX4, wxT("执行情况（移仓期间请不要到本界面外执行其他操作）"), wxPoint(453, 6+nMH), wxSize(346, 405));
	new wxStaticBox(this, ID_WXSTATICBOX3, wxT("超时设置"), wxPoint(7, 240+nMH), wxSize(436, 170));
	new wxStaticBox(this, ID_WXSTATICBOX2, wxT("限价价格设置"), wxPoint(7, 135+nMH), wxSize(434, 55));
	new wxStaticBox(this, ID_WXSTATICBOX1, wxT("限价价格设置"), wxPoint(6, 37+nMH), wxSize(434, 53));

	new wxStaticText(this, ID_WXSTATICTEXT15, wxT("原始合约__秒后，按最新价浮动__个价位改单"), wxPoint(465, 46+nMH), wxDefaultSize, 0, wxT("WxStaticText26"));
	new wxStaticText(this, ID_WXSTATICTEXT16, wxT("原始合约__平仓情况"), wxPoint(465, 66+nMH), wxDefaultSize, 0, wxT("WxStaticText32"));
	new wxStaticText(this, ID_WXSTATICTEXT17, wxT("执行时间__秒，已重试__次，剩余重试机会__次"), wxPoint(465, 26+nMH), wxDefaultSize, 0, wxT("WxStaticText17"));
	new wxStaticText(this, ID_WXSTATICTEXT18, wxT("目标合约__秒后，按最新价浮动__个价位改单"), wxPoint(465, 241+nMH), wxDefaultSize, 0, wxT("WxStaticText23"));
	new wxStaticText(this, ID_WXSTATICTEXT19, wxT("目标合约__开仓情况"), wxPoint(465, 261+nMH), wxDefaultSize, 0, wxT("WxStaticText31"));
	new wxStaticText(this, ID_WXSTATICTEXT20, wxT("执行时间__秒，已重试__次，剩余重试机会__次"), wxPoint(465, 221+nMH), wxDefaultSize, 0, wxT("WxStaticText33"));
	new wxStaticText(this, ID_WXSTATICTEXT14, wxT("滑动价差"), wxPoint(299, 305+nMH), wxDefaultSize, 0, wxT("WxStaticText14"));
	new wxStaticText(this, ID_WXSTATICTEXT13, wxT("原始合约未成交处理"), wxPoint(15, 305+nMH), wxDefaultSize, 0, wxT("WxStaticText13"));
	new wxStaticText(this, ID_WXSTATICTEXT12, wxT("次"), wxPoint(301, 269+nMH), wxDefaultSize, 0, wxT("WxStaticText12"));
	new wxStaticText(this, ID_WXSTATICTEXT11, wxT("重试次数"), wxPoint(180, 269+nMH), wxDefaultSize, 0, wxT("WxStaticText11"));
	new wxStaticText(this, ID_WXSTATICTEXT10, wxT("秒"), wxPoint(142, 269+nMH), wxDefaultSize, 0, wxT("WxStaticText10"));
	new wxStaticText(this, ID_WXSTATICTEXT9, wxT("超时设置"), wxPoint(15, 269+nMH), wxDefaultSize, 0, wxT("WxStaticText9"));
	new wxStaticText(this, ID_WXSTATICTEXT8, wxT("操作顺序"), wxPoint(11, 209+nMH), wxDefaultSize, 0, wxT("WxStaticText8"));
	new wxStaticText(this, ID_WXSTATICTEXT7, wxT("滑动价差"), wxPoint(215, 161+nMH), wxDefaultSize, 0, wxT("WxStaticText7"));
	new wxStaticText(this, ID_WXSTATICTEXT6, wxT("开仓买入__手，按以下价格下单"), wxPoint(197, 106+nMH), wxDefaultSize, 0, wxT("WxStaticText6"));
	new wxStaticText(this, ID_WXSTATICTEXT5, wxT("目标合约"), wxPoint(11, 107+nMH), wxDefaultSize, 0, wxT("WxStaticText5"));
	new wxStaticText(this, ID_WXSTATICTEXT4, wxT("滑动价差"), wxPoint(215, 63+nMH), wxDefaultSize, 0, wxT("WxStaticText4"));
	new wxStaticText(this, ID_WXSTATICTEXT3, wxT("目标合约未成交处理"), wxPoint(15, 345+nMH), wxDefaultSize, 0, wxT("WxStaticText15"));
	new wxStaticText(this, ID_WXSTATICTEXT2, wxT("滑动价差"), wxPoint(300, 343+nMH), wxDefaultSize, 0, wxT("WxStaticText16"));
	new wxStaticText(this, ID_WXSTATICTEXT1, wxT("原始合约：__平仓卖出__手按以下价格下单"), wxPoint(14, 14+nMH), wxDefaultSize, 0, wxT("WxStaticText1"));

	m_pCNumericTextCtrl1 = new CNumericTextCtrl(this, ID_WXEDIT1, wxT("0"), wxPoint(273, 60+nMH), wxSize(40, 21), 0, wxDefaultValidator, wxT("WxEdit1"));
	m_pCNumericTextCtrl2 = new CNumericTextCtrl(this, ID_WXEDIT2, wxT("0"), wxPoint(273, 158+nMH), wxSize(40, 21), 0, wxDefaultValidator, wxT("WxEdit2"));
	m_pCNumericTextCtrl3 = new CNumericTextCtrl(this, ID_WXEDIT3, wxT("0"), wxPoint(82, 265+nMH), wxSize(48, 21), 0, wxDefaultValidator, wxT("WxEdit3"));
	m_pCNumericTextCtrl4 = new CNumericTextCtrl(this, ID_WXEDIT4, wxT("0"), wxPoint(244, 265+nMH), wxSize(48, 21), 0, wxDefaultValidator, wxT("WxEdit4"));
	m_pCNumericTextCtrl5 = new CNumericTextCtrl(this, ID_WXEDIT5, wxT("0"), wxPoint(356, 302+nMH), wxSize(73, 21), 0, wxDefaultValidator, wxT("WxEdit5"));
	m_pCNumericTextCtrl6 = new CNumericTextCtrl(this, ID_WXEDIT6, wxT("0"), wxPoint(356, 340+nMH), wxSize(73, 21), 0, wxDefaultValidator, wxT("WxEdit6"));

	m_pCNumericTextCtrl1->SetFilter(g_slide);
	m_pCNumericTextCtrl2->SetFilter(g_slide);
	m_pCNumericTextCtrl5->SetFilter(g_slide);
	m_pCNumericTextCtrl6->SetFilter(g_slide);

	wxArrayString arrayStringFor_WxComboBox1;
	m_pComboBox1 = new wxComboBox(this, ID_WXCOMBOBOX1, wxT(""), wxPoint(75, 103+nMH), wxSize(105, 20), arrayStringFor_WxComboBox1, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox1"));

	wxArrayString arrayStringFor_WxComboBox2;
	arrayStringFor_WxComboBox2.Add(wxT("先平后开"));
	arrayStringFor_WxComboBox2.Add(wxT("同时平开"));
	m_pComboBox2 = new wxComboBox(this, ID_WXCOMBOBOX2, wxT("先平后开"), wxPoint(81, 206+nMH), wxSize(117, 20), arrayStringFor_WxComboBox2, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox2"));
	m_pComboBox2->SetSelection(0);

	wxArrayString arrayStringFor_WxComboBox3;
	arrayStringFor_WxComboBox3.Add(wxT("(无动作)"));
	arrayStringFor_WxComboBox3.Add(wxT("按最新价浮动改单"));
	arrayStringFor_WxComboBox3.Add(wxT("按买卖价浮动改单"));
	arrayStringFor_WxComboBox3.Add(wxT("按市价改单"));
	arrayStringFor_WxComboBox3.Add(wxT("撤单并撤销"));
	m_pComboBox3 = new wxComboBox(this, ID_WXCOMBOBOX3, wxT(""), wxPoint(130, 302+nMH), wxSize(152, 20), arrayStringFor_WxComboBox3, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox3"));
	m_pComboBox3->SetSelection(0);

	wxArrayString arrayStringFor_WxComboBox4;
	arrayStringFor_WxComboBox4.Add(wxT("(无动作)"));
	arrayStringFor_WxComboBox4.Add(wxT("按最新价浮动改单"));
	arrayStringFor_WxComboBox4.Add(wxT("按买卖价浮动改单"));
	arrayStringFor_WxComboBox4.Add(wxT("按市价改单"));
	arrayStringFor_WxComboBox4.Add(wxT("撤单并撤销"));
	m_pComboBox4 = new wxComboBox(this, ID_WXCOMBOBOX4, wxT(""), wxPoint(130, 340+nMH), wxSize(152, 20), arrayStringFor_WxComboBox4, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox4"));
	m_pComboBox4->SetSelection(0);

	m_pCustomSpinCtrl1 = new CCustomSpinCtrl(this, ID_WXEDIT7, ID_WXSPINCTRL1, wxT("0"), wxPoint(375, 60+nMH), wxSize(43, 21), wxSP_ARROW_KEYS, 0, 999999, 0);
	m_pCustomSpinCtrl2 = new CCustomSpinCtrl(this, ID_WXEDIT8, ID_WXSPINCTRL2, wxT("0"), wxPoint(375, 158+nMH), wxSize(43, 21), wxSP_ARROW_KEYS, 0, 999999, 0);

	m_pCustomSpinCtrl1->SetFilter(g_assign);
	m_pCustomSpinCtrl2->SetFilter(g_assign);

	wxRadioButton *p = new wxRadioButton(this, ID_WXRADIOBUTTON1, wxT("市价"), wxPoint(22, 62+nMH), wxSize(44, 17), wxRB_GROUP, wxDefaultValidator, wxT("WxRadioButton1"));
	m_pRadioGroupOriginal->InsertGroup(p);
	p = new wxRadioButton(this, ID_WXRADIOBUTTON2, wxT("最新价"), wxPoint(82, 62+nMH), wxSize(60, 17), 0, wxDefaultValidator, wxT("WxRadioButton3"));
	m_pRadioGroupOriginal->InsertGroup(p);
	p = new wxRadioButton(this, ID_WXRADIOBUTTON3, wxT("买卖价"), wxPoint(148, 62+nMH), wxSize(55, 17), 0, wxDefaultValidator, wxT("WxRadioButton4"));
	m_pRadioGroupOriginal->InsertGroup(p);
	p = new wxRadioButton(this, ID_WXRADIOBUTTON4, wxT("指定价"), wxPoint(320, 62+nMH), wxSize(54, 17), 0, wxDefaultValidator, wxT("WxRadioButton5"));
	m_pRadioGroupOriginal->InsertGroup(p);
	p = new wxRadioButton(this, ID_WXRADIOBUTTON5, wxT("市价"), wxPoint(22, 160+nMH), wxSize(44, 17), wxRB_GROUP, wxDefaultValidator, wxT("WxRadioButton2"));
	m_pRadioGroupDest->InsertGroup(p);
	p = new wxRadioButton(this, ID_WXRADIOBUTTON6, wxT("最新价"), wxPoint(82, 160+nMH), wxSize(56, 17), 0, wxDefaultValidator, wxT("WxRadioButton8"));
	m_pRadioGroupDest->InsertGroup(p);
	p = new wxRadioButton(this, ID_WXRADIOBUTTON7, wxT("买卖价"), wxPoint(148, 160+nMH), wxSize(54, 17), 0, wxDefaultValidator, wxT("WxRadioButton9"));
	m_pRadioGroupDest->InsertGroup(p);
	p = new wxRadioButton(this, ID_WXRADIOBUTTON8, wxT("指定价"), wxPoint(320, 160+nMH), wxSize(54, 17), 0, wxDefaultValidator, wxT("WxRadioButton10"));
	m_pRadioGroupDest->InsertGroup(p);

	

// 	wxWindow* pButton = (FindWindowById(ID_WXBUTTONEDIT, this));
// 	if(pButton) pButton->Show(false);
// 	pButton = (FindWindowById(ID_WXBUTTON3, this));
// 	if(pButton) pButton->Show(false);
// 	pButton = (FindWindowById(ID_WXBUTTONCLOSE, this));
// 	if(pButton) pButton->Show(false);


	SetTitle(wxT("移仓"));
	SetIcon(wxNullIcon);
	SetSize(8,8,815,491+nMH);
	Center();

	////GUI Items Creation End
}

void CMovePositionDlg::LoadConfig()
{

	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return;
	MOVEPOSITIONCFG moveCfg = pMgr->GetMovePositionCfg();

	m_pRadioGroupOriginal->SetGroupValue(moveCfg.nSourcePriceLimit);
	m_pRadioGroupDest->SetGroupValue(moveCfg.nTargetPriceLimit);

	wxString strText;

	strText.Printf("%d", moveCfg.nSourcePriceOffset);
	if(m_pCNumericTextCtrl1) m_pCNumericTextCtrl1->SetValue(strText);
	
	strText.Printf("%d", moveCfg.nTargetPriceOffset);
	if(m_pCNumericTextCtrl2) m_pCNumericTextCtrl2->SetValue(strText);

	if(m_pComboBox2) m_pComboBox2->SetSelection(moveCfg.nOperateOrder);
	if(m_pComboBox3) m_pComboBox3->SetSelection(moveCfg.nSourceNotDealAction);
	if(m_pComboBox4) m_pComboBox4->SetSelection(moveCfg.nTargetNotDealAction);

	strText.Printf("%d", moveCfg.nOutTimeSec);
	if(m_pCNumericTextCtrl3) m_pCNumericTextCtrl3->SetValue(strText);

	strText.Printf("%d", moveCfg.nRetryTimes);
	if(m_pCNumericTextCtrl4) m_pCNumericTextCtrl4->SetValue(strText);

	strText.Printf("%d", moveCfg.nSourceNotDealPriceOffset);
	if(m_pCNumericTextCtrl5) m_pCNumericTextCtrl5->SetValue(strText);

	strText.Printf("%d", moveCfg.nTargetNotDealPriceOffset);
	if(m_pCNumericTextCtrl6) m_pCNumericTextCtrl6->SetValue(strText);

}

void CMovePositionDlg::SaveConfig()
{
	
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return;
	MOVEPOSITIONCFG moveCfg;

	wxRadioButton* pRadiobutton = NULL;

	moveCfg.nSourcePriceLimit = m_pRadioGroupOriginal->GetGroupValue();
	moveCfg.nTargetPriceLimit = m_pRadioGroupDest->GetGroupValue();
	
	wxString strText;

	if(m_pCNumericTextCtrl1) moveCfg.nSourcePriceOffset = atoi(m_pCNumericTextCtrl1->GetValue().c_str());
	if(m_pCNumericTextCtrl2) moveCfg.nTargetPriceOffset = atoi(m_pCNumericTextCtrl2->GetValue().c_str());
	if(m_pCNumericTextCtrl3) moveCfg.nOutTimeSec = atoi(m_pCNumericTextCtrl3->GetValue().c_str());
	if(m_pCNumericTextCtrl4) moveCfg.nRetryTimes = atoi(m_pCNumericTextCtrl4->GetValue().c_str());

	if(m_pComboBox2) moveCfg.nOperateOrder = m_pComboBox2->GetSelection();
	if(m_pComboBox3) moveCfg.nSourceNotDealAction = m_pComboBox3->GetSelection();
	if(m_pComboBox4) moveCfg.nTargetNotDealAction = m_pComboBox4->GetSelection();

	if(m_pCNumericTextCtrl5) moveCfg.nSourceNotDealPriceOffset = atoi(m_pCNumericTextCtrl5->GetValue().c_str());
	if(m_pCNumericTextCtrl6) moveCfg.nTargetNotDealPriceOffset = atoi(m_pCNumericTextCtrl6->GetValue().c_str());

	pMgr->SetMovePositionCfg(moveCfg);

}

void CMovePositionDlg::SetDefault()
{
	SetAble(true,true,false);


	wxWindow * pWin = NULL;
	for (int nID = ID_WXLISTCTRL1; nID <= ID_WXLISTCTRL2; ++nID)
	{
		pWin = (FindWindowById(nID, this));
		if (pWin)
		{
			(static_cast<wxListCtrl*>(pWin))->DeleteAllItems();
		}
	}

	wxStaticText * pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT15, this));
	if (pStatic) pStatic->SetLabel(wxT("原始合约__秒后，按最新价浮动__个价位改单"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT17, this));
	if (pStatic) pStatic->SetLabel(wxT("执行时间__秒，已重试__次，剩余重试机会__次"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT18, this));
	if (pStatic) pStatic->SetLabel(wxT("目标合约__秒后，按最新价浮动__个价位改单"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT20, this));
	if (pStatic) pStatic->SetLabel(wxT("执行时间__秒，已重试__次，剩余重试机会__次"));

}

bool CMovePositionDlg::InitGrid()
{

	m_RowDatas[0].clear();
	m_RowDatas[1].clear();

	wxListCtrl* pwxListCtrls[3];

	pwxListCtrls[0] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL3, this);
	pwxListCtrls[1] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL4, this);
	pwxListCtrls[2] = NULL;

	int k=0;
	while(pwxListCtrls[k] != NULL) {
		wxListCtrl* pwxListCtrl = pwxListCtrls[k];
		std::map<int, long>& fieldID2ColumnID = m_FieldID2ColumnIDs[k];
		std::map<int, LIST_COLUMN_CFG>& fieldAttr = m_FieldAttrs[k];

		pwxListCtrl->DeleteAllItems();

		int Mycount = pwxListCtrl->GetColumnCount();
		if( Mycount!=0) {
			for( int i=0; i<Mycount; i++)
				pwxListCtrl->DeleteColumn(0);
		}

		//m_iRowCnt=0;
		CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
		wxASSERT(pCfgMgr);
		LPLIST_TABLE_CFG pListCfg = pCfgMgr->GetListCfg(GID_ALL_ORDER);
		wxASSERT(pListCfg);
		
		////设置LIST界面属性
		//m_HeadBackgroundColor = wxColor(m_pListCfg->HeadBackgroundColor);
		//m_HeadColor = wxColor(m_pListCfg->HeadColor);
		//m_BackgroundColor = wxColor(m_pListCfg->BackgroundColor);
		//m_TextColor = wxColor(m_pListCfg->TextColor);
		//m_Font.SetNativeFontInfoUserDesc(m_pListCfg->Font); 
		//m_EvenLineBgColor=wxColor(m_pListCfg->EvenLineBackgroundColor);
		//m_OddLineBgColor= wxColor(m_pListCfg->OddLineBackgroundColor);

//		pwxListCtrl->SetHeaderBackgroundColour(m_HeadBackgroundColor);
//		pwxListCtrl->SetHeaderForegroundColour(m_HeadColor);
//		pwxListCtrl->SetBackgroundColour(m_BackgroundColor);
//		pwxListCtrl->SetForegroundColour(m_TextColor);
//		pwxListCtrl->SetFont(m_Font);
//		pwxListCtrl->SetOddRowColour(m_pListCfg->OddLineBackgroundColor);
//		pwxListCtrl->SetEvenRowColour(m_pListCfg->EvenLineBackgroundColor);
		wxListItem col;
		int nColCount = pCfgMgr->GetListColCount(GID_ALL_ORDER);
		int nColIndex =-1;

		fieldID2ColumnID.clear();
		fieldAttr.clear();
		for(int i = 0;  i < nColCount; i++)
		{
			LPLIST_COLUMN_CFG pColCfg = pCfgMgr->GetListColCfg(GID_ALL_ORDER, i);
			wxASSERT(pColCfg);
			if(pColCfg->id<0 || pColCfg->id>ALLORDER_MAX)
				continue;

//			//"合约" "报单编号"栏，左对齐
//			if( (pColCfg->id == ALLORDER_InstrumentID) || (pColCfg->id == ALLORDER_OrderLocalID) )
//				col.SetAlign(wxLISTEXT_FORMAT_LEFT);
//			else//其余的，右对齐
//				col.SetAlign(wxLISTEXT_FORMAT_RIGHT);

			if(pColCfg->Visibility != 0)
			{		
				nColIndex++;
				col.SetText(pCfgMgr->GetColCfgCaption(GID_ALL_ORDER,pColCfg->id));
				col.SetWidth(pColCfg->Width);
				col.SetTextColour(pColCfg->TextColor);
				fieldID2ColumnID[pColCfg->id] = nColIndex;
				fieldAttr[pColCfg->id] = *pColCfg;

				pwxListCtrl->InsertColumn(nColIndex, col); 
			}
		}
		
		k++;
	}

    return true;
}

BOOL CMovePositionDlg::Init(CMovePositionServiceThread* poMovePositionServiceThread)
{
	m_poMovePositionServiceThread = poMovePositionServiceThread;

	// 显示数据到界面上

	InitGrid();

	LoadConfig();
	SetDefault();

	wxListCtrl* pwxListCtrl = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL3, this);
	if(pwxListCtrl != NULL)
		pwxListCtrl->DeleteAllItems();
	pwxListCtrl = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL4, this);
	if(pwxListCtrl != NULL)
		pwxListCtrl->DeleteAllItems();


	wxString str;
	
	str.Printf("原始合约：%s，平仓%s%d手，按以下价格下单 ", m_tPositionData.InstrumentID, 
			m_tPositionData.PosiDirection == THOST_FTDC_PD_Long?"卖出":"买入", m_tPositionData.Position);
	(FindWindowById(ID_WXSTATICTEXT1, this))->SetLabel(str);

	str.Printf("开仓%s%d手，按以下价格下单 ", 
			m_tPositionData.PosiDirection == THOST_FTDC_PD_Long?"买入":"卖出", m_tPositionData.Position);
	(FindWindowById(ID_WXSTATICTEXT6, this))->SetLabel(str);

	str.Printf("原始合约%s平仓情况", m_tPositionData.InstrumentID);
	(FindWindowById(ID_WXSTATICTEXT16, this))->SetLabel(str);

	PlatformStru_InstrumentInfo insInfo;
	ZeroMemory(&insInfo, sizeof(insInfo));
	if (DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentInfo(m_tPositionData.InstrumentID,insInfo) == 0)
	{
		m_fPriceTick = insInfo.PriceTick;
		m_pCustomSpinCtrl1->SetStep(m_fPriceTick);
		m_pCustomSpinCtrl2->SetStep(m_fPriceTick);
	}

	PlatformStru_DepthMarketData quotData;
	ZeroMemory(&quotData, sizeof(quotData));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_tPositionData.InstrumentID,quotData) == 0)
	{
		str = "";
		str << quotData.LastPrice;
		(m_pCustomSpinCtrl1->GetTextCtrl())->SetLabel(str);
	}
	
	std::set<std::string> outData;
	outData.clear();
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentList(outData);
	std::set<std::string>::iterator iter = outData.begin();
	int i=0;
	m_pComboBox1->Clear();
	for (;iter != outData.end(); ++iter)
	{
		std::string & strItem = *iter;
		wxString  wxstr = wxString( strItem.c_str(), wxConvUTF8);

		PlatformStru_InstrumentInfo insInfoTemp;
		ZeroMemory(&insInfoTemp, sizeof(insInfoTemp));
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(strItem, insInfoTemp);
		if(strcmp(insInfo.ProductID, insInfoTemp.ProductID) == 0 &&
				strcmp(strItem.c_str(), m_tPositionData.InstrumentID) != 0)

		//if (_strnicmp(insInfo.ThostInstrument.ProductID, strItem.c_str(), strlen(insInfo.ThostInstrument.ProductID)) == 0 &&
		//	strcmp(strItem.c_str(), m_tPositionData.InstrumentID) != 0)
		{

			m_vecStaticData.push_back(wxstr);

			if (0 == i)
			{
				SetStaticTitle(wxstr);
				m_strInstrumentIDBackup=strItem;
				if(DEFAULT_SVR()) DEFAULT_SVR()->SetSubscribeStatus(m_strInstrumentIDBackup.c_str(),S_STATUS_POS);

				ZeroMemory(&quotData, sizeof(quotData));
				if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_strInstrumentIDBackup.c_str(),quotData) == 0)
				{
					str = "";
					str << quotData.LastPrice;
					(m_pCustomSpinCtrl2->GetTextCtrl())->SetLabel(str);
				}
			}

			m_pComboBox1->Insert(strItem,i);
			++i;
		}
	}

	if (m_pComboBox1->GetCount() > 0)
	{
		m_pComboBox1->SetSelection(0);
	}
	
	wxWindow* pButton = FindWindowById(ID_WXBUTTON2, this);
	if(pButton) pButton->Show(true);
	pButton = (FindWindowById(ID_WXBUTTON1, this));
	if(pButton) pButton->Show(true);
	pButton = (FindWindowById(ID_WXBUTTONEDIT, this));
	if(pButton) pButton->Show(false);
	pButton = (FindWindowById(ID_WXBUTTON3, this));
	if(pButton) pButton->Show(false);
	pButton = (FindWindowById(ID_WXBUTTONCLOSE, this));
	if(pButton) pButton->Show(false);

	SaveLastState();
	return FALSE;
}

void CMovePositionDlg::OnClose(wxCloseEvent& event)
{
	if (m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->Hide();
	}

	wxWindow* pButton1 = NULL;
	wxWindow* pButton2 = NULL;
	pButton1 = (FindWindowById(ID_WXBUTTONEDIT, this));
	pButton2 = (FindWindowById(ID_WXBUTTONCLOSE, this));
	if(pButton1 && pButton2) {
		if(pButton1->IsShown() && !pButton2->IsShown()) {
			wxMessageDialog dialog( NULL, wxT("移仓操作正在进行中，是否确认打断此操作？\n注：已进行的仓位操作和现有挂单均会维持现状"), _("中国国际期货网上交易终端"), 
				wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
			int nResult = dialog.ShowModal();
			if(nResult==wxID_YES)
			{
				//wxCommandEvent event;
				//OnCancel(event);
				DoCancel();
			}
			if(nResult==wxID_NO)
			{
				return;
			}
		}
	}

	event.Skip();
}

bool CMovePositionDlg::Show(bool show)
{
	if(!show) {
		SaveConfig();
	}
    return wxDialog::Show(show);
}


#define VK_TAB		0x09  //TAB key  
#define VK_RETURN	0x0D  //ENTER key  
#define VK_LEFT		0x25  //LEFT ARROW key  
#define VK_UP		0x26  //UP ARROW key  
#define VK_RIGHT	0x27  //RIGHT ARROW key  
#define VK_DOWN		0x28  //DOWN ARROW key  

void CMovePositionDlg::OnHookKeyPress(wxKeyEvent& evt)
{
	//wxSpinEvent evtSpin;
	//wxKeyEvent* pEvent=&evt;//(wxKeyEvent*)evt.GetClientData();
 //   if(!pEvent) return;
 //   wxWindow *win = FindFocus();
 //   if(win == NULL) {
	//	evt.Skip();
 //       return;
 //   }
	//pEvent->SetId(win->GetId());
 //   if (pEvent->GetKeyCode()==WXK_TAB || pEvent->GetKeyCode()==VK_TAB)
 //   {
 //       bool backward = pEvent->ShiftDown();
 //       int id,thisID=win->GetId(),count=m_TabOrderVector.size();
 //       for(id=0;id<count;id++)
 //       {
 //           if(m_TabOrderVector[id]==thisID)
 //           {
 //               if(backward)
 //                   id=id>0?id-1:count-1;
 //               else
 //                   id=id<count-1?id+1:0;
 //               FindWindowById(m_TabOrderVector[id],this)->SetFocus();
 //               return;
 //           }
 //       }
 //   }
 //   else if(pEvent->GetKeyCode()==WXK_UP || pEvent->GetKeyCode()==VK_UP)
 //   {
 //       int id,thisID=win->GetId(),count=m_TabOrderVector.size();
 //       for(id=0;id<count;id++)
 //       {
 //           if(m_TabOrderVector[id]==thisID)
 //           {
 //               if(id>0)
 //               {
 //                   FindWindowById(m_TabOrderVector[id-1],this)->SetFocus();
 //                  return;
 //               }
 //           }
 //       }
 //   }
 //   else if(pEvent->GetKeyCode()==WXK_DOWN || pEvent->GetKeyCode()==VK_DOWN)
 //   {
 //       int id,thisID=win->GetId(),count=m_TabOrderVector.size();
 //       for(id=0;id<count;id++)
 //       {
 //           if(m_TabOrderVector[id]==thisID)
 //           {
 //               if(id<count-1)
 //               {
 //                   FindWindowById(m_TabOrderVector[id+1],this)->SetFocus();
 //                   return;
 //               }
 //           }
 //       }
 //   }
	//else {
	//	for(int nSel = 0; nSel < m_nCmbOrderCount; nSel++) {
	//		if(m_poOrderConfirmPanels[nSel]!=NULL)
	//			m_poOrderConfirmPanels[nSel]->OnHookKeyPress(evt);
	//	}
	//}
}

//void CMovePositionDlg::OnCfgChanged(wxCommandEvent& evt)
//{
//}


//显示错误信息
void CMovePositionDlg::ShowTradeInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow)
{
    if(TRADEINFODLG(this)==NULL) return;
	TRADEINFODLG(this)->ShowTradeMsg(errormessage, -1, Title, -1, bIsShow);

//	LOG_DATA* pData = new LOG_DATA;
//	pData->strTitle = Title;
//	pData->strFormat = errormessage;
//	TRADEINFODLG(this)->WriteLog( pData );
}


void CMovePositionDlg::OnRcvTimer(wxCommandEvent& event)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

    std::string strSrc(EvtParamStr);
//    std::string strSrc = (event.GetString()).mb_str();

	// 例如"1,1,3, 7, 3, 2"，表示移仓单编号，原始合约还是目标合约，
	//执行时间，倒计时剩余时间，已重试次数，剩余重试次数

	std::vector<std::string> vecData;
	tokenize_str(strSrc, vecData);

	wxString str;

	//原始合约
	if (vecData.at(1) == "1")
	{
		wxWindow * pStatic = FindWindowById(ID_WXSTATICTEXT17, this);
		str.Printf("执行时间%s秒，已重试%s次，剩余重试机会%s次", vecData.at(2).c_str(), vecData.at(4).c_str(), vecData.at(5).c_str());
		if (pStatic)
		{
			pStatic->SetLabel(str);
		}

		wxStaticBox * pStaticBox = static_cast<wxStaticBox*>(FindWindowById(ID_WXSTATICBOX5, this));
		str.Printf("原始合约%s平仓情况", m_tMove.tSrcPos.strInstrumentID);
		if (pStaticBox)
		{
			pStaticBox->SetLabel(str);
		}

		long value;
		if (m_pCNumericTextCtrl1)
		{
			m_pCNumericTextCtrl1->GetValue().ToLong(&value);		
		}
		pStaticBox = static_cast<wxStaticBox*>(FindWindowById(ID_WXSTATICTEXT15, this));
		str.Printf("原始合约%s秒后，按最新价浮动%d个价位改单", vecData.at(3).c_str(), m_tMove.tSrcPos.nTimeOutPriceOffset);
		if (pStaticBox)
		{
			pStaticBox->SetLabel(str);
		}

		pStatic = FindWindowById(ID_WXSTATICTEXT16, this);
		str.Printf("原始合约%s平仓情况", m_tMove.tSrcPos.strInstrumentID);
		if (pStatic)
		{
			pStatic->SetLabel(str);
		}


	}
	//目标合约
	else if (vecData.at(1) == "2")
	{
		wxWindow * pStatic = FindWindowById(ID_WXSTATICTEXT20, this);
		str.Printf("执行时间%s秒，已重试%s次，剩余重试机会%s次", vecData.at(2).c_str(), vecData.at(4).c_str(), vecData.at(5).c_str());
		if (pStatic)
		{
			pStatic->SetLabel(str);
		}

		wxStaticBox * pStaticBox = static_cast<wxStaticBox*>(FindWindowById(ID_WXSTATICBOX6, this));
		str.Printf("目标合约%s开仓情况", m_tMove.tDestPos.strInstrumentID);
		if (pStaticBox)
		{
			pStaticBox->SetLabel(str);
		}

		long value;
		if (m_pCNumericTextCtrl1)
		{
			m_pCNumericTextCtrl1->GetValue().ToLong(&value);
		}
		pStaticBox = static_cast<wxStaticBox*>(FindWindowById(ID_WXSTATICTEXT18, this));
		str.Printf("目标合约%s秒后，按最新价浮动%d个价位改单", vecData.at(3).c_str(), m_tMove.tDestPos.nTimeOutPriceOffset);
		if (pStaticBox)
		{
			pStaticBox->SetLabel(str);
		}

		pStatic = FindWindowById(ID_WXSTATICTEXT19, this);
		str.Printf("目标合约%s开仓情况", m_tMove.tDestPos.strInstrumentID);
		if (pStatic)
		{
			pStatic->SetLabel(str);
		}

	}

}

void CMovePositionDlg::OnRcvTradeLog(wxCommandEvent& event)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

//    std::string strSrc = (event.GetString()).mb_str();
    std::string strSrc(EvtParamStr);

	// 例如"1,1, 1, 1, 25555, 20, 25550, 3, 17"，表示移仓单编号，
	//原始合约还是目标合约，委托单号，成交单号，委托价格，报单手数，成交价格，成交数量，剩余数量

	std::vector<std::string> vecData;
	tokenize_str(strSrc, vecData);

	if (vecData.size() > 9)
	{
		return;
	}

	
	//原始合约
	if ("1" == vecData.at(1))
	{
		wxListCtrl * pListctrl = static_cast<wxListCtrl*>(FindWindowById(ID_WXLISTCTRL1, this));
		wxString str;

		if (pListctrl)
		{
			int curRow = pListctrl->GetItemCount();
			wxString strText;
			pListctrl->InsertItem(curRow, strText);
			pListctrl->SetItem(curRow, 0, vecData.at(2));
			pListctrl->SetItem(curRow, 1, vecData.at(3));
			pListctrl->SetItem(curRow, 2, vecData.at(4));
			pListctrl->SetItem(curRow, 3, vecData.at(5));
			pListctrl->SetItem(curRow, 4, vecData.at(6));
			pListctrl->SetItem(curRow, 5, vecData.at(7));
			pListctrl->SetItem(curRow, 6, vecData.at(8));

			if ("0" != vecData.at(8))
			{
				pListctrl->SetItem(curRow, 7, wxT("部分成交"));
			}
			else
			{
				pListctrl->SetItem(curRow, 7, wxT("全部成交"));
			}
		}
	}
	//目标合约
	else if ("2" == vecData.at(1))
	{
		wxListCtrl * pListctrl = static_cast<wxListCtrl*>(FindWindowById(ID_WXLISTCTRL2, this));
		wxString str;

		if (pListctrl)
		{
			int curRow = pListctrl->GetItemCount();
			wxString strText;
			pListctrl->InsertItem(curRow, strText);
			pListctrl->SetItem(curRow, 0, vecData.at(2));
			pListctrl->SetItem(curRow, 1, vecData.at(3));
			pListctrl->SetItem(curRow, 2, vecData.at(4));
			pListctrl->SetItem(curRow, 3, vecData.at(5));
			pListctrl->SetItem(curRow, 4, vecData.at(6));
			pListctrl->SetItem(curRow, 5, vecData.at(7));
			pListctrl->SetItem(curRow, 6, vecData.at(8));

			if ("0" != vecData.at(8))
			{
				pListctrl->SetItem(curRow, 7, wxT("部分成交"));
			}
			else
			{
				pListctrl->SetItem(curRow, 7, wxT("全部成交"));
			}
		}
	}



}

void CMovePositionDlg::OnRcvAction(wxCommandEvent& event)
{
	wxWindow* pButton = NULL;
	switch(event.GetInt()) {
	case CMovePositionServiceThread::conActionPause: 
		break;
	case CMovePositionServiceThread::conActionMoving:				//1 开始移仓或仓位互换
		break;
	case CMovePositionServiceThread::conActionUserChanging:			//2 执行外围改单
		break;
	case CMovePositionServiceThread::conActionAutoChanging:			//3 执行超时改单
		break;
	case CMovePositionServiceThread::conActionChanged:				//4 改单完成
		break;
	case CMovePositionServiceThread::conActionCompleted:			//5 移仓完成
		break;
	case CMovePositionServiceThread::conActionCancel:				//6 用户撤销
		break;
	case CMovePositionServiceThread::conActionCancel_TimeOut:		//7 超时撤销
		m_bRunning = FALSE;
		if (m_pComboBox3->GetSelection() == 4 || m_pComboBox4->GetSelection() == 4)
		{
			//wxMessageBox("移仓操作未完成，已撤单。","提示");
			ShowTradeInfoDlg("提示","移仓操作未完成，已撤单。", TRUE);
		}
		else
		{
			//wxMessageBox("移仓操作已停止，未完成。","提示");
			ShowTradeInfoDlg("提示","移仓操作已停止，未完成。", TRUE);
		}
		pButton = FindWindowById(ID_WXBUTTON3, this);
		if(pButton) {
			pButton->Show(false);
		}
		pButton = FindWindowById(ID_WXBUTTON1, this);
		if(pButton) {
			pButton->Show(false);
		}
		pButton = FindWindowById(ID_WXBUTTONEDIT, this);
		if(pButton) {
			pButton->Show(false);
			//pButton->Enable(true);
		}
		pButton = FindWindowById(ID_WXBUTTONCLOSE, this);
		if(pButton) {
			pButton->Show(true);
			pButton->Enable(true);
		}
		break;
	case CMovePositionServiceThread::conActionCancel_Exciption:		//8 异常撤销
		break;
	case CMovePositionServiceThread::conActionMoveCompleted:		//9 移仓单完成
		m_bRunning = FALSE;
		pButton = FindWindowById(ID_WXBUTTON3, this);
		if(pButton) {
			pButton->Show(false);
		}
		pButton = FindWindowById(ID_WXBUTTON1, this);
		if(pButton) {
			pButton->Show(false);
		}
		pButton = FindWindowById(ID_WXBUTTONEDIT, this);
		if(pButton) {
			pButton->Show(false);
			//pButton->Enable(true);
		}
		pButton = FindWindowById(ID_WXBUTTONCLOSE, this);
		if(pButton) {
			pButton->Show(true);
			pButton->Enable(true);
		}

		//wxMessageBox("移仓单完成！","提示");
		ShowTradeInfoDlg("提示","移仓单完成！", TRUE);
		break;
	case CMovePositionServiceThread::conActionSwapCompleted:		//10仓位互换完成
		break;
	};
}

void CMovePositionDlg::OnStart(wxCommandEvent& event)
{

	if(m_tPositionData.TotalCloseOrderVolume>0) {
		wxString strText;
		strText.Printf("合约%s有未成交报单，请先关闭本窗口，手动撤单后再进行移仓操作。", m_tPositionData.InstrumentID);
		wxMessageDialog dialog( NULL, strText, _("提示"), wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}
	
	wxMessageDialog dialog( NULL, wxT("是否确认现在就进行移仓操作？"), _("提示"), wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	if(dialog.ShowModal()!=wxID_YES)
		return;

	m_bRunning = TRUE;

	SendRequest();
	SetAble(false);

	wxWindow* pButton = FindWindowById(ID_WXBUTTON2, this);
	if(pButton) 
		pButton->Show(false);

	pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) 
		pButton->Show(false);

	pButton = FindWindowById(ID_WXBUTTONEDIT, this);
	if(pButton) {
		pButton->Show(true);
		pButton->Enable(true);
	}
	pButton = (FindWindowById(ID_WXBUTTON3, this));
	if(pButton) 
		pButton->Show(true);

}

void CMovePositionDlg::OnSetDefault(wxCommandEvent& event)
{
	ReadLastState();
}

void CMovePositionDlg::OnEdit(wxCommandEvent& event)
{

	SetAble(true,false,false);
	wxWindow* pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) {
		pButton->Show(true);
	}

	m_pModifyOrderDlg->Clear();
	m_pModifyOrderDlg->SetOwner(this, TRUE);
	m_pModifyOrderDlg->Show();

	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tMove.nNum, TRUE);

}

void CMovePositionDlg::OnCancel(wxCommandEvent& event)
{
	wxMessageDialog dialog( NULL, wxT("移仓操作正在进行中，是否确认打断此操作？\n注：已进行的仓位操作和现有挂单均会维持现状"), _("中国国际期货网上交易终端"), 
		wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	int nResult = dialog.ShowModal();
	if(nResult==wxID_YES)
	{
		DoCancel();
	}
}

void CMovePositionDlg::DoCancel()
{

	SetAble(false);

	m_poMovePositionServiceThread->LockObject();

	m_poMovePositionServiceThread->MovePositionCancel(m_tMove.nNum);

	m_poMovePositionServiceThread->UnlockObject();

	wxWindow* pButton = FindWindowById(ID_WXBUTTON3, this);
	if(pButton) {
		pButton->Show(false);
	}
	pButton = FindWindowById(ID_WXBUTTONCLOSE, this);
	if(pButton) {
		pButton->Show(true);
		pButton->Enable(true);
	}

	if (m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->Hide();
	}

}

void CMovePositionDlg::OnBtnClose(wxCommandEvent& event)
{
	Close();
	//Destroy();
}

void CMovePositionDlg::SendRequest(bool bIsRedo)
{
	//return;

	CMovePositionServiceThread::MOVEPOSITION movepos;
	ZeroMemory(&movepos, sizeof(movepos));
	movepos.nState = CMovePositionServiceThread::conMoveActive;
	movepos.pWnd = this;

	if (m_pComboBox2) movepos.nActionMode = m_pComboBox2->GetCurrentSelection();

	long value;
	m_pCNumericTextCtrl3->GetValue().ToLong(&value);
	movepos.nTimeOutInterval = value;
	m_curOrderOption.nTimeout = value;

	m_pCNumericTextCtrl4->GetValue().ToLong(&value);
	movepos.nRetryNum = value;
	m_curOrderOption.nRetryTimes = value;

	CMovePositionServiceThread::POSITIONUNIT	tSrcPos;		// 原始仓位
	ZeroMemory(&tSrcPos, sizeof(tSrcPos));
	strcpy(tSrcPos.strInstrumentID, m_tPositionData.InstrumentID);
	if (m_tPositionData.PosiDirection == THOST_FTDC_PD_Long)
	{
		tSrcPos.bDirection = FALSE;
	}
	else
	{
		tSrcPos.bDirection = TRUE;
	}

	tSrcPos.fPriceTick = m_fPriceTick;
	if (m_tPositionData.HedgeFlag == THOST_FTDC_ECIDT_Speculation)
	{
		tSrcPos.bHedge = TRUE;
	}
	else
	{
		tSrcPos.bHedge = FALSE;
	}

	tSrcPos.nPriceType = m_pRadioGroupOriginal->GetGroupValue();
	m_curOrderOption.nSourcePriceType = tSrcPos.nPriceType;
	if (0 == tSrcPos.nPriceType)
	{
		PlatformStru_InstrumentInfo outData;
		memset(&outData,0,sizeof(outData));

		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(tSrcPos.strInstrumentID, outData);
//		if(strcmp(outData.ExchangeID, "SHFE")==0 && NeedSimulateMarketPrice()) 
		if(NEEDSIMULATEMARKET(outData.ExchangeID) && NeedSimulateMarketPrice()) 
			tSrcPos.bSimulate = TRUE;
	}

	tSrcPos.bIsClose = TRUE;

	//double val;
	//wxSpinButton * pSpinctrl = m_pCustomSpinCtrl1->GetSpinButton();
	//if (pSpinctrl)
	//{
	//	pSpinctrl->GetLabel().ToDouble(&val);
	//	tSrcPos.fPrice = val;
	//	m_curOrderOption.fSourceAssign = val;
	//}
	tSrcPos.fPrice = atof(m_pCustomSpinCtrl1->GetTextCtrl()->GetLabel());
	m_curOrderOption.fSourceAssign = tSrcPos.fPrice;

	tSrcPos.nVolume = m_tPositionData.Position;

	if (m_pCNumericTextCtrl1)
	{
		m_pCNumericTextCtrl1->GetValue().ToLong(&value);
		tSrcPos.nPriceOffset = value;
		m_curOrderOption.lSourceHuaDong = value;
	}

	if (m_pComboBox3)
	{
		tSrcPos.nTimeOutAction = m_pComboBox3->GetSelection();
		m_curOrderOption.nSourceHandle = m_pComboBox3->GetSelection();
	}

	if (m_pCNumericTextCtrl5)
	{
		m_pCNumericTextCtrl5->GetValue().ToLong(&value);
		tSrcPos.nTimeOutPriceOffset = value;
		m_curOrderOption.lSourceTimeoutHuaDong = value;
	}

	tSrcPos.nRemainVolume = m_tPositionData.Position-m_tPositionData.TodayPosition;
	tSrcPos.nRemainVolume_CloseToday = m_tPositionData.TodayPosition;

	//m_poMovePositionServiceThread->LockObject();

	if(tSrcPos.nRemainVolume>0)
		m_poMovePositionServiceThread->MakeReq(wxString(tSrcPos.strInstrumentID), tSrcPos.bDirection, 2, tSrcPos.fPrice, 
				tSrcPos.nRemainVolume, tSrcPos.bHedge, (tSrcPos.nPriceType? FALSE:TRUE), 
				(tSrcPos.nPriceType==0 && tSrcPos.bSimulate), tSrcPos.req);

	if(tSrcPos.nRemainVolume_CloseToday>0)
		m_poMovePositionServiceThread->MakeReq(wxString(tSrcPos.strInstrumentID), tSrcPos.bDirection, 1, tSrcPos.fPrice, 
				tSrcPos.nRemainVolume_CloseToday, tSrcPos.bHedge, (tSrcPos.nPriceType? FALSE:TRUE), 
				(tSrcPos.nPriceType==0 && tSrcPos.bSimulate), tSrcPos.req_CloseToday);

	movepos.tSrcPos = tSrcPos;

	//m_poMovePositionServiceThread->UnlockObject();

	CMovePositionServiceThread::POSITIONUNIT	tDestPos;		// 目标仓位
	ZeroMemory(&tDestPos, sizeof(tDestPos));
	std::string InstrumentID;
	if (m_pComboBox1)
	{
		InstrumentID = m_pComboBox1->GetLabel().mb_str();
	}

	char* pStr = const_cast<char*>(InstrumentID.c_str());
	strcpy(tDestPos.strInstrumentID, pStr);

	if (m_tPositionData.PosiDirection == THOST_FTDC_PD_Long)
	{
		tDestPos.bDirection = TRUE;
	}
	else
	{
		tDestPos.bDirection = FALSE;
	}

	tDestPos.fPriceTick = m_fPriceTick;
	if (m_tPositionData.HedgeFlag == THOST_FTDC_ECIDT_Speculation)
	{
		tDestPos.bHedge = TRUE;
	}
	else
	{
		tDestPos.bHedge = FALSE;
	}

	tDestPos.nPriceType = m_pRadioGroupDest->GetGroupValue();
	m_curOrderOption.nDestPriceType = tDestPos.nPriceType;
	if (0 == tDestPos.nPriceType)
	{
		PlatformStru_InstrumentInfo outData;
		memset(&outData,0,sizeof(outData));

		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(tDestPos.strInstrumentID, outData);
//		if(strcmp(outData.ExchangeID, "SHFE")==0 && NeedSimulateMarketPrice()) 
		if(NEEDSIMULATEMARKET(outData.ExchangeID) && NeedSimulateMarketPrice()) 
			tDestPos.bSimulate = TRUE;
	}

	tDestPos.bIsClose = FALSE;

	//pSpinctrl = m_pCustomSpinCtrl2->GetSpinButton();
	//if (pSpinctrl)
	//{
	//	pSpinctrl->GetLabel().ToDouble(&val);
	//	tDestPos.fPrice = val;
	//	m_curOrderOption.fDestAssign = val;
	//}
	tDestPos.fPrice = atof(m_pCustomSpinCtrl2->GetTextCtrl()->GetLabel());
	m_curOrderOption.fDestAssign = tDestPos.fPrice;

	tDestPos.nVolume = m_tPositionData.Position;

	if (m_pCNumericTextCtrl2)
	{
		m_pCNumericTextCtrl2->GetValue().ToLong(&value);
		tDestPos.nPriceOffset = value;
		m_curOrderOption.lDestHuaDong = value;
	}

	if (m_pComboBox4)
	{
		tDestPos.nTimeOutAction = m_pComboBox4->GetSelection();
		m_curOrderOption.nDestHandle = m_pComboBox4->GetSelection();
	}

	if (m_pCNumericTextCtrl6)
	{
		m_pCNumericTextCtrl6->GetValue().ToLong(&value);
		tDestPos.nTimeOutPriceOffset = value;
		m_curOrderOption.lDestTimeoutHuaDong = value;
	}

	tDestPos.nRemainVolume = tDestPos.nVolume;

	//	m_poMovePositionServiceThread->LockObject();

	m_poMovePositionServiceThread->MakeReq(wxString(tDestPos.strInstrumentID), tDestPos.bDirection, 0, tDestPos.fPrice, 
			tDestPos.nRemainVolume, tDestPos.bHedge, (tDestPos.nPriceType? FALSE:TRUE), 
			(tDestPos.nPriceType==0 && tDestPos.bSimulate), tDestPos.req);

	//	m_poMovePositionServiceThread->MakeReq(wxString(m_tPositionData.InstrumentID), tDestPos.nDirection, 0, tDestPos.fPrice, 
	//		m_tPositionData.TodayVolume, tDestPos.bHedge, (tDestPos.nPriceType? FALSE:TRUE), (tDestPos.nPriceType? FALSE:TRUE), tDestPos.req_CloseToday);

	movepos.tDestPos = tDestPos;

	BOOL bIsEqual = FALSE;

	m_poMovePositionServiceThread->LockObject();

	if(!bIsRedo) {
		int nNum = m_poMovePositionServiceThread->MovePositionAdd(movepos);
		m_poMovePositionServiceThread->MovePositionDoNow(nNum);
		m_tMove = movepos;
		m_tMove.nNum = nNum;
	}
	else {
		m_poMovePositionServiceThread->MovePositionRedo(m_tMove.nNum, movepos, bIsEqual);
	}

	m_poMovePositionServiceThread->UnlockObject();

}

void CMovePositionDlg::SetAble(bool bPart1, bool bPart2, bool bAll)
{

	wxWindow * p = NULL;

	int index3, index4;
	if (m_pComboBox3) index3 = m_pComboBox3->GetSelection();
	if (m_pComboBox4) index4 = m_pComboBox4->GetSelection();

	int Original = m_pRadioGroupOriginal->GetGroupValue();
	int dest = m_pRadioGroupDest->GetGroupValue();

	for (int nID = ID_WXEDIT1; nID < ID_WXCOMBOBOX1; ++nID)
	{
		p = FindWindowById(nID, this);
		if (p)
		{
			if (ID_WXEDIT1 == nID && (0 == Original || 3 == Original) && !bAll)
			{
				m_pCNumericTextCtrl1->Enable(false);
			}
			else if (ID_WXEDIT2 == nID && (0 == dest || 3 == dest) && !bAll)
			{
				m_pCNumericTextCtrl2->Enable(false);
			}
			else if (ID_WXEDIT1 == nID && (1 == Original || 2 == Original) && !bAll)
			{
				m_pCNumericTextCtrl1->Enable(true);
			}
			else if (ID_WXEDIT2 == nID && (1 == dest || 2 == dest) && !bAll)
			{
				m_pCNumericTextCtrl2->Enable(true);
			}
			else if (ID_WXCOMBOBOX3 == nID && (1 == index3 || 2 == index3) && !bAll)
			{
				m_pCNumericTextCtrl5->Enable(true);
				m_pComboBox3->Enable(true);
			}
			else if (ID_WXCOMBOBOX4 == nID && (1 == index4 || 2 == index4) && !bAll)
			{
				m_pCNumericTextCtrl6->Enable(true);
				m_pComboBox4->Enable(true);
			}
			else if (ID_WXCOMBOBOX3 == nID && (0 == index3 || 3 == index3) && !bAll)
			{
				m_pCNumericTextCtrl5->Enable(false);
				m_pComboBox3->Enable(true);
			}
			else if (ID_WXCOMBOBOX4 == nID && (0 == index4 || 3 == index4) && !bAll)
			{
				m_pCNumericTextCtrl6->Enable(false);
				m_pComboBox4->Enable(true);
			}
			else
				p->Enable(bPart1);
		}
	}

	for (int nID = ID_WXCOMBOBOX1; nID < ID_WXBUTTON1; ++nID)
	{
		p = FindWindowById(nID, this);
		if (p)
		{
			p->Enable(bPart2);
		}
	}

}

void CMovePositionDlg::SetStaticTitle(wxString & strID)
{
	wxString str;

	str.Printf("目标合约%s开仓情况", strID);
	(FindWindowById(ID_WXSTATICTEXT19, this))->SetLabel(str);

}

void CMovePositionDlg::OnComboboxSelectChange(wxCommandEvent& event)
{
	int nid = event.GetId();
	switch (nid)
	{
	case ID_WXCOMBOBOX1:
		if (m_pComboBox1 != NULL)
		{
			SetStaticTitle(m_pComboBox1->GetLabel());
			if(DEFAULT_SVR()) DEFAULT_SVR()->SetSubscribeStatus(m_strInstrumentIDBackup.c_str(),S_STATUS_POS,S_STATUS_DEL);
			if(DEFAULT_SVR()) DEFAULT_SVR()->SetSubscribeStatus(event.GetString().c_str(),S_STATUS_POS);
			m_strInstrumentIDBackup = event.GetString();

			PlatformStru_DepthMarketData quotData;
			ZeroMemory(&quotData, sizeof(quotData));
			if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_strInstrumentIDBackup.c_str(),quotData) == 0)
			{
				wxString str;
				str << quotData.LastPrice;
				(m_pCustomSpinCtrl2->GetTextCtrl())->SetLabel(str);
			}
		}
		break;
	case ID_WXCOMBOBOX3:
		if (m_pComboBox3 != NULL)
		{
			int index = m_pComboBox3->GetSelection();
			if (1 == index || 2 == index)
			{
				m_pCNumericTextCtrl5->Enable(true);
			}
			else
			{
				m_pCNumericTextCtrl5->Enable(false);
			}
			
		}
		break;
	case ID_WXCOMBOBOX4:
		if (m_pComboBox4 != NULL)
		{
			int index = m_pComboBox4->GetSelection();
			if (1 == index || 2 == index)
			{
				m_pCNumericTextCtrl6->Enable(true);
			}
			else
			{
				m_pCNumericTextCtrl6->Enable(false);
			}
		}
		break;
	}


}

void CMovePositionDlg::OnChildFocus(wxChildFocusEvent& event)
{
	if (m_pModifyOrderDlg->IsShown())
	{
		PrintChange();
	}
}

void CMovePositionDlg::PrintChange()
{

	long value;
	m_pCNumericTextCtrl3->GetValue().ToLong(&value);
	if (m_curOrderOption.nTimeout != value && m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->InsertModify("超时时间改变\n");
		m_curOrderOption.nTimeout = value;
	}

	m_pCNumericTextCtrl4->GetValue().ToLong(&value);
	if (m_curOrderOption.nRetryTimes != value && m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->InsertModify("重试次数改变\n");
		m_curOrderOption.nRetryTimes = value;
	}


	int nPriceType = m_pRadioGroupOriginal->GetGroupValue();

	if (nPriceType != m_curOrderOption.nSourcePriceType && m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->InsertModify("原始合约价格类型改变\n");
		m_curOrderOption.nSourcePriceType = nPriceType;
	}


	double val;
	wxSpinButton * pSpinctrl = m_pCustomSpinCtrl1->GetSpinButton();
	if (pSpinctrl)
	{
		val = pSpinctrl->GetValue();
		if (m_curOrderOption.fSourceAssign != val  && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("原始合约指定价格改变\n");
			m_curOrderOption.fSourceAssign = val;
		}
	}


	if (m_pCNumericTextCtrl1)
	{
		m_pCNumericTextCtrl1->GetValue().ToLong(&value);
		if (m_curOrderOption.lSourceHuaDong != value  && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("原始合约滑动价差改变\n");
			m_curOrderOption.lSourceHuaDong = value;
		}

	}


	if (m_pComboBox3 && m_curOrderOption.nSourceHandle != m_pComboBox3->GetSelection() && m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->InsertModify("原始合约未成交处理方式改变\n");
		m_curOrderOption.nSourceHandle = m_pComboBox3->GetSelection();
	}

	if (m_pCNumericTextCtrl5)
	{
		m_pCNumericTextCtrl5->GetValue().ToLong(&value);
		if (m_curOrderOption.lSourceTimeoutHuaDong != value && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("原始合约超时设置滑动价差改变\n");
			m_curOrderOption.lSourceTimeoutHuaDong = value;
		}
	}

	nPriceType = m_pRadioGroupDest->GetGroupValue();
	if (nPriceType != m_curOrderOption.nDestPriceType && m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->InsertModify("目标合约价格类型改变\n");
		m_curOrderOption.nDestPriceType = nPriceType;
	}

	pSpinctrl = m_pCustomSpinCtrl2->GetSpinButton();
	if (pSpinctrl)
	{
		val = pSpinctrl->GetValue();
		if (m_curOrderOption.fDestAssign != val  && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("目标合约指定价格改变\n");
			m_curOrderOption.fDestAssign = val;
		}
	}


	if (m_pCNumericTextCtrl2)
	{
		m_pCNumericTextCtrl2->GetValue().ToLong(&value);
		if (m_curOrderOption.lDestHuaDong != value  && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("目标合约滑动价差改变\n");
			m_curOrderOption.lDestHuaDong = value;
		}

	}

	if (m_pComboBox4 && m_curOrderOption.nDestHandle != m_pComboBox4->GetSelection() && m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->InsertModify("目标合约未成交处理方式改变\n");
		m_curOrderOption.nDestHandle = m_pComboBox4->GetSelection();
	}

	if (m_pCNumericTextCtrl6)
	{
		m_pCNumericTextCtrl6->GetValue().ToLong(&value);
		if (m_curOrderOption.lDestTimeoutHuaDong != value && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("目标合约超时设置滑动价差改变\n");
			m_curOrderOption.lDestTimeoutHuaDong = value;
		}
	}


}

void CMovePositionDlg::ReSendRequest()
{
	wxWindow* pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) 
		pButton->Show(false);

	pButton = FindWindowById(ID_WXBUTTONEDIT, this);
	if(pButton) {
		pButton->Show(true);
		pButton->Enable(true);
	}

	SendRequest(true);
	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tMove.nNum, FALSE);
	SetAble(false);
}

void CMovePositionDlg::RollBack()
{
	wxWindow* pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) 
		pButton->Show(false);

	pButton = FindWindowById(ID_WXBUTTONEDIT, this);
	if(pButton) {
		pButton->Show(true);
		pButton->Enable(true);
	}

	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tMove.nNum, FALSE);

	wxString strValue = wxString::Format(_T("%d"), m_curOrderOption.nTimeout);
	m_pCNumericTextCtrl3->SetLabel(strValue);

	strValue = wxString::Format(_T("%d"), m_curOrderOption.nRetryTimes);
	m_pCNumericTextCtrl4->SetLabel(strValue);

	m_pRadioGroupOriginal->SetGroupValue(m_curOrderOption.nSourcePriceType);

	wxSpinButton * pSpinctrl = m_pCustomSpinCtrl1->GetSpinButton();
	strValue = wxString::Format(_T("%d"), m_curOrderOption.fSourceAssign);
	if (pSpinctrl)
	{
		pSpinctrl->SetLabel(strValue);
	}


	strValue = wxString::Format(_T("%d"), m_curOrderOption.lSourceHuaDong);
	if (m_pCNumericTextCtrl1)
	{
		m_pCNumericTextCtrl1->SetLabel(strValue);
	}

	if (m_pComboBox3)
	{
		m_pComboBox3->SetSelection(m_curOrderOption.nSourceHandle);
	}
	if (0 == m_curOrderOption.nSourceHandle || 3 == m_curOrderOption.nSourceHandle)
	{
		m_pCNumericTextCtrl5->Enable(false);
	}
	else if (1 == m_curOrderOption.nSourceHandle || 2 == m_curOrderOption.nSourceHandle)
	{
		m_pCNumericTextCtrl5->Enable(true);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lSourceTimeoutHuaDong);
	if (m_pCNumericTextCtrl5)
	{
		m_pCNumericTextCtrl5->SetLabel(strValue);
	}

	m_pRadioGroupDest->SetGroupValue(m_curOrderOption.nDestPriceType);

	pSpinctrl = m_pCustomSpinCtrl2->GetSpinButton();
	strValue = wxString::Format(_T("%d"), m_curOrderOption.fDestAssign);
	if (pSpinctrl)
	{
		pSpinctrl->SetLabel(strValue);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lDestHuaDong);
	if (m_pCNumericTextCtrl2)
	{
		m_pCNumericTextCtrl2->SetLabel(strValue);
	}

	if (m_pComboBox4)
	{
		m_pComboBox4->SetSelection(m_curOrderOption.nDestHandle);
	}
	if (0 == m_curOrderOption.nDestHandle || 3 == m_curOrderOption.nDestHandle)
	{
		m_pCNumericTextCtrl6->Enable(false);
	}
	else if (1 == m_curOrderOption.nDestHandle || 2 == m_curOrderOption.nDestHandle)
	{
		m_pCNumericTextCtrl6->Enable(true);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lDestTimeoutHuaDong);
	if (m_pCNumericTextCtrl6)
	{
		m_pCNumericTextCtrl6->SetLabel(strValue);
	}

	SetAble(false);

}

void CMovePositionDlg::UpdateAllRecord()
{
	wxListCtrl* pwxListCtrls[3];

	pwxListCtrls[0] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL3, this);
	pwxListCtrls[1] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL4, this);
	pwxListCtrls[2] = NULL;

	int k=0;
	while(pwxListCtrls[k] != NULL) {
		wxListCtrl* pwxListCtrl = pwxListCtrls[k];
		std::map<int, long>& fieldID2ColumnID = m_FieldID2ColumnIDs[k];
		std::map<int, LIST_COLUMN_CFG>& fieldAttr = m_FieldAttrs[k];
		std::map<int, DataRtnOrder*>& mapRowData = m_RowDatas[k];

		pwxListCtrl->DeleteAllItems();

		//插入新的记录
		std::map<int, DataRtnOrder*>::iterator it_new=mapRowData.begin();
		while(it_new!=mapRowData.end()) {
			PlatformStru_OrderInfo tAllOrder;
			tAllOrder = (*it_new->second).OrderField;
			UpdateOrderList(tAllOrder, pwxListCtrl, fieldID2ColumnID);
			it_new++;
		}

		k++;
	}

}

int CMovePositionDlg::UpdateOrderList(const PlatformStru_OrderInfo& rawData, wxListCtrl* pwxListCtrl, std::map<int, long>& fieldID2ColumnID)
{

	int iRowNo=-1, iVolumeTotal=0, iColStatus=0,iColVolume=0;
    wxListItem tmp;
    wxString newValue;

    return LoadOneOrderList( pwxListCtrl->GetItemCount(), rawData, pwxListCtrl, fieldID2ColumnID);
    //return 0;
}

int CMovePositionDlg::LoadOneOrderList( int iRowNo, const PlatformStru_OrderInfo &rawData, wxListCtrl* pwxListCtrl, std::map<int, long>& fieldID2ColumnID)
{	
    int iFlag = -1, iCol = 0;
    wxString newText;//单元格的文本内容
    wxListItem item;	

    item.Clear();

    iCol = 0;

    CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
    wxASSERT(pCfgMgr);
    LPLIST_TABLE_CFG pColCfg = pCfgMgr->GetListCfg(GID_ALL_ORDER);
    wxASSERT(pColCfg);

    //增加新的一行，此时为空记录
    PlatformStru_OrderInfo *pRawData=new PlatformStru_OrderInfo();
    wxASSERT(pRawData);
    memcpy( pRawData, &rawData, sizeof(PlatformStru_OrderInfo) );
    pwxListCtrl->InsertItem( iRowNo, item);
    pwxListCtrl->SetItemData(iRowNo, (long)pRawData);

    item.SetId(iRowNo);//设置行号
    int LangluageId=SVR_LANGUAGE;
    std::map<int, long> ::iterator it;
    it = fieldID2ColumnID.find( ALLORDER_OrderLocalID );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;	
        newText=rawData.OrderSysID;//左边有空格
        newText.Trim(true).Trim(false);
        item.SetText(newText);
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_InstrumentID );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        newText=rawData.InstrumentID;
        item.SetText(newText);
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_Direction );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::Direction2String(rawData.Direction,LangluageId) );
        item.SetColumn( iCol );

        if ( rawData.Direction == THOST_FTDC_D_Buy )
            item.SetTextColour(wxColor(RGB(255,0,0)));//红色
        else if ( rawData.Direction == THOST_FTDC_D_Sell )
            item.SetTextColour(wxColor(RGB(0,128,0)));//绿色

        pwxListCtrl->SetItem(item);
        item.SetTextColour(wxColor(RGB(0,0,0)));//恢复item的颜色属性，该处理方式需要改进
    }

    it = fieldID2ColumnID.find( ALLORDER_CombOffsetFlag );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::OffsetFlag2String(rawData.CombOffsetFlag[0],LangluageId) );			
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_OrderStatus );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::OrderStatus2String(rawData.OrderStatus,LangluageId) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_LimitPrice );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;

		newText=GlobalFunc::ConvertToString(rawData.LimitPrice);

        item.SetText(newText);
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_VolumeTotalOriginal );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        newText.Printf("%d", rawData.VolumeTotalOriginal);
        item.SetText(newText);
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_VolumeTotal );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        newText.Printf("%d", rawData.VolumeTotal);
        item.SetText(newText);
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_VolumeTraded );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        newText.Printf("%d", rawData.VolumeTraded);
        item.SetText(newText);
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_StatusMsg );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;			
		item.SetText(rawData.StatusMsg);
        item.SetId(iRowNo);
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_InsertTime );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( wxString( rawData.InsertTime) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

  it = fieldID2ColumnID.find( ALLORDER_d_freezeMargin );
    if(it!=fieldID2ColumnID.end()) {
		iCol = it->second;
		if ( rawData.freezeMargin > 0.00001 || rawData.freezeMargin==0 )
		{					
			newText.Printf("%.2lf", rawData.freezeMargin ); 
			if ( newText== "\0" )		 newText= "0.00";
		}

		else 
		{
			newText.Printf( "-" );	
		}

		

		item.SetText(newText);
		item.SetColumn( iCol );
		pwxListCtrl->SetItem(item);	
    }


	it = fieldID2ColumnID.find( ALLORDER_d_troubleMoney );
	if(it!=fieldID2ColumnID.end()) 
	{
		iCol = it->second;
		if ( rawData.troubleMoney > 0.00001 ||rawData.troubleMoney == 0)
		{					
			newText.Printf("%.2lf", rawData.troubleMoney); 
			if ( newText== "\0" )		 newText= "0.00";
		}
		else
		{
			newText.Printf( "-" );				
		}

		item.SetText(newText);
		item.SetColumn( iCol );
		item.SetTextColour( wxColor(pColCfg->TextColor) );
		pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_CombHedgeFlag );
    if(it!=fieldID2ColumnID.end()) {
        iCol = it->second;	
        item.SetText( CFieldValueTool::HedgeFlag2String(rawData.CombHedgeFlag[0],LangluageId) );			
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_ExchangeID );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;	
        item.SetText( DEFAULT_SVR()?(DEFAULT_SVR()->ExchangeID2String(rawData.ExchangeID)).c_str():"" );			
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_BrokerOrderSeq );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        newText.Printf("%d", rawData.BrokerOrderSeq);
        item.SetText(newText);
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_OrderPriceType );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::OrderPriceType2String(rawData.OrderPriceType,LangluageId) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_TimeCondition );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::TimeCondition2String(rawData.TimeCondition,LangluageId) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_OrderType );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::OrderType2String(rawData.OrderType,LangluageId) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_UserForceClose );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::UserForceClose2String(rawData.UserForceClose,LangluageId) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_ForceCloseReason );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::ForceCloseReason2String(rawData.ForceCloseReason,LangluageId) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_OrderSubmitStatus );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( CFieldValueTool::OrderSubmitStatus2String(rawData.OrderSubmitStatus,LangluageId) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    it = fieldID2ColumnID.find( ALLORDER_UserProductInfo );
    if(it!=fieldID2ColumnID.end()) {
        iCol=it->second;
        item.SetText( wxString( rawData.UserProductInfo) );
        item.SetColumn( iCol );
        item.SetTextColour( wxColor(pColCfg->TextColor) );
        pwxListCtrl->SetItem(item);
    }

    return 0;
}

void CMovePositionDlg::OnRcvOrderLog(wxCommandEvent& evt)
{
    std::string EvtParamStr;
    char* EvtPtr=NULL;
    int EvtPtrLen;
    int UserDataInt;
    if(1)
    {
        bool brlt=false;
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CEventParam::GetEventParam(EvtParamID,NULL,NULL,NULL,&EvtPtrLen,0,NULL)&&
            EvtPtrLen==(int)sizeof(DataRtnOrder))
        {
            EvtPtr=(char*)malloc(EvtPtrLen);
            if(EvtPtr!=NULL&&
                CEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,EvtPtr,&EvtPtrLen,EvtPtrLen,&UserDataInt))
            {
                CEventParam::DeleteEventParam(EvtParamID);
                brlt=true;
            }
        }
        if(!brlt)
        {
            return;
        }
    }

    DataRtnOrder* RawData=(DataRtnOrder* )EvtPtr;
//    DataRtnOrder* RawData= (DataRtnOrder*)(evt.GetClientData());
    
//	if(!RawData) {
//		wxASSERT(RawData);
//		return;
//	}
	
//	int nNum = atoi(evt.GetString().c_str());
	int nNum = atoi(EvtParamStr.c_str());
	
	if(nNum != m_tMove.nNum) {
//		delete RawData;
//		RawData=NULL;
//		evt.SetClientData(NULL);
        if(EvtPtr!=NULL)
            free(EvtPtr);
		return;
	}
	if(UserDataInt==0) {
		std::map<int, DataRtnOrder*>& mapRowData = m_RowDatas[0];
		mapRowData[RawData->OrderField.RequestID] = RawData;
	}
	else if(UserDataInt==1) {
		std::map<int, DataRtnOrder*>& mapRowData = m_RowDatas[1];
		mapRowData[RawData->OrderField.RequestID] = RawData;
	}
    else
    {
        if(EvtPtr!=NULL)
            free(EvtPtr);
		return;
    }
	UpdateAllRecord();
//	delete RawData;
//	RawData=NULL;
//	evt.SetClientData(NULL);
//注意，不要释放EvtPtr,它已经被m_RowDatas用掉了
}

void CMovePositionDlg::OnSpinUpPress(wxSpinEvent& event)
{
	int nid = event.GetId();

	if (ID_WXSPINCTRL1 == nid)
	{
		m_pCustomSpinCtrl1->SpinUpPress(event);
	}

	else if (ID_WXSPINCTRL2 == nid)
	{
		m_pCustomSpinCtrl2->SpinUpPress(event);
	}
}

void CMovePositionDlg::OnSpinDownPress(wxSpinEvent& event)
{
	int nid = event.GetId();

	if (ID_WXSPINCTRL1 == nid)
	{
		m_pCustomSpinCtrl1->SpinDownPress(event);
	}

	else if (ID_WXSPINCTRL2 == nid)
	{
		m_pCustomSpinCtrl2->SpinDownPress(event);
	}
}

void CMovePositionDlg::OnRadioButton(wxCommandEvent& event)
{
	wxTextCtrl* pTextctrl = NULL;
	int nid = event.GetId();
	switch(event.GetId())
	{
	case ID_WXRADIOBUTTON1:
	case ID_WXRADIOBUTTON4:
		{

			if (m_pCNumericTextCtrl1) m_pCNumericTextCtrl1->Disable();
			break;
		}
	case ID_WXRADIOBUTTON5:
	case ID_WXRADIOBUTTON8:
		{
			if (m_pCNumericTextCtrl2) m_pCNumericTextCtrl2->Disable();
			break;
		}
	case ID_WXRADIOBUTTON2:
	case ID_WXRADIOBUTTON3:
		{

			if (m_pCNumericTextCtrl1) m_pCNumericTextCtrl1->Enable();
			break;
		}
	case ID_WXRADIOBUTTON6:
	case ID_WXRADIOBUTTON7:
		{
			if (m_pCNumericTextCtrl2) m_pCNumericTextCtrl2->Enable();
			break;
		}

	}

}

void CMovePositionDlg::SaveLastState()
{

	m_lastState.nSourcePriceType = m_pRadioGroupOriginal->GetGroupValue();

	if ( (1 == m_lastState.nSourcePriceType || 2 == m_lastState.nSourcePriceType) && m_pCNumericTextCtrl1)
	{
		m_lastState.SourceHuaDong = m_pCNumericTextCtrl1->GetValue();
	}

	CNumericTextCtrl * pTextctrl = m_pCustomSpinCtrl1->GetTextCtrl();
	if (pTextctrl)
	{
		m_lastState.SourceAssign = pTextctrl->GetLabel();
	}

	m_lastState.nDestPriceType = m_pRadioGroupDest->GetGroupValue();

	if ( (1 == m_lastState.nDestPriceType || 2 == m_lastState.nDestPriceType) && m_pCNumericTextCtrl2)
	{
		m_lastState.DestHuaDong = m_pCNumericTextCtrl2->GetValue();
	}

	pTextctrl = m_pCustomSpinCtrl2->GetTextCtrl();
	if (pTextctrl)
	{
		m_lastState.DestAssign = pTextctrl->GetLabel();
	}

	if (m_pComboBox1)
	{
		m_lastState.nDestContract = m_pComboBox1->GetSelection();
	}

	if (m_pComboBox2)
	{
		m_lastState.nOperOrder = m_pComboBox2->GetSelection();
	}

	if (m_pComboBox3)
	{
		m_lastState.nSourceHandle = m_pComboBox3->GetSelection();
	}

	if (m_pComboBox4)
	{
		m_lastState.nDestHandle = m_pComboBox4->GetSelection();
	}

	if (m_pCNumericTextCtrl3)
	{
		m_lastState.Timeout = m_pCNumericTextCtrl3->GetValue();
	}
	if (m_pCNumericTextCtrl4)
	{
		m_lastState.RetryTimes = m_pCNumericTextCtrl4->GetValue();
	}
	if (m_pCNumericTextCtrl5)
	{
		m_lastState.SourceTimeoutHuaDong = m_pCNumericTextCtrl5->GetValue();
	}
	if (m_pCNumericTextCtrl6)
	{
		m_lastState.DestTimeoutHuaDong = m_pCNumericTextCtrl6->GetValue();
	}
}

void CMovePositionDlg::ReadLastState()
{

	wxRadioButton * pRadiobutton = NULL;
	wxComboBox* pCombo = NULL;

	m_pRadioGroupOriginal->SetGroupValue(m_lastState.nSourcePriceType);

	switch (m_lastState.nSourcePriceType)
	{
	case 0:
		{
			if (m_pCNumericTextCtrl1)
			{
				m_pCNumericTextCtrl1->Enable(false);
			}
		}
		break;
	case 1:
		{
			if (m_pCNumericTextCtrl1)	
			{
				m_pCNumericTextCtrl1->Enable(true);
				m_pCNumericTextCtrl1->SetLabel(m_lastState.SourceHuaDong);
			}
		}
		break;
	case 2:
		{
			if (m_pCNumericTextCtrl1)
			{
				m_pCNumericTextCtrl1->Enable(true);
				m_pCNumericTextCtrl1->SetLabel(m_lastState.SourceHuaDong);
			}
		}
		break;
	case 3:
		{
			if (m_pCNumericTextCtrl1)
			{
				m_pCNumericTextCtrl1->Enable(false);
			}
		}
		break;
	}

	CNumericTextCtrl * pTextctrl = m_pCustomSpinCtrl1->GetTextCtrl();
	if (pTextctrl)
	{
		pTextctrl->SetLabel(m_lastState.SourceAssign);
	}

	m_pRadioGroupDest->SetGroupValue(m_lastState.nDestPriceType);
	switch (m_lastState.nDestPriceType)
	{
	case 0:
		{
			if (m_pCNumericTextCtrl2)
			{
				m_pCNumericTextCtrl2->Enable(false);
			}
		}
		break;
	case 1:
		{
			if (m_pCNumericTextCtrl2)
			{
				m_pCNumericTextCtrl2->Enable(true);
				m_pCNumericTextCtrl2->SetLabel(m_lastState.DestHuaDong);
			}
		}
		break;
	case 2:
		{
			if (m_pCNumericTextCtrl2)
			{
				m_pCNumericTextCtrl2->Enable(true);
				m_pCNumericTextCtrl2->SetLabel(m_lastState.DestHuaDong);
			}
		}
		break;
	case 3:
		{
			if (m_pCNumericTextCtrl2)
			{
				m_pCNumericTextCtrl2->Enable(false);
			}
		}
		break;
	}

	pTextctrl = m_pCustomSpinCtrl2->GetTextCtrl();
	if (pTextctrl)
	{
		pTextctrl->SetLabel(m_lastState.DestAssign);
	}

	if (m_pComboBox1)
	{
		m_pComboBox1->SetSelection(m_lastState.nDestContract);
		SetStaticTitle(m_pComboBox1->GetLabel());
	}

	if (m_pComboBox2)
	{
		m_pComboBox2->SetSelection(m_lastState.nOperOrder);
	}

	if (m_pComboBox3)
	{
		m_pComboBox3->SetSelection(m_lastState.nSourceHandle);
	}
	if (0 == m_lastState.nSourceHandle || 3 == m_lastState.nSourceHandle)
	{
		m_pCNumericTextCtrl5->Enable(false);
	}
	else if (1 == m_lastState.nSourceHandle || 2 == m_lastState.nSourceHandle)
	{
		m_pCNumericTextCtrl5->Enable(true);
	}

	if (m_pComboBox4)
	{
		m_pComboBox4->SetSelection(m_lastState.nDestHandle);
	}
	if (0 == m_lastState.nDestHandle || 3 == m_lastState.nDestHandle)
	{
		m_pCNumericTextCtrl6->Enable(false);
	}
	else if (1 == m_lastState.nDestHandle || 2 == m_lastState.nDestHandle)
	{
		m_pCNumericTextCtrl6->Enable(true);
	}

	if (m_pCNumericTextCtrl3)
	{
		m_pCNumericTextCtrl3->SetLabel(m_lastState.Timeout);
	}
	if (m_pCNumericTextCtrl4)
	{
		m_pCNumericTextCtrl4->SetLabel(m_lastState.RetryTimes);
	}
	if (m_pCNumericTextCtrl5)
	{
		m_pCNumericTextCtrl5->SetLabel(m_lastState.SourceTimeoutHuaDong);
	}
	if (m_pCNumericTextCtrl6)
	{
		m_pCNumericTextCtrl6->SetLabel(m_lastState.DestTimeoutHuaDong);
	}
}