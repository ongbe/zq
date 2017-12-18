
#include "stdafx.h"
#include "CSwapPositionDlg.h"
#include "../inc/Module-Misc2/tools_util.h"
#include "CNumericTextCtrl.h"
#include "CRadioButtonGroup.h"
#include <wx/spinctrl.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include "../inc/Module-Misc2/SimpleWriteLog.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

extern void tokenize_str(const std::string & str, std::vector<std::string> &vecResult, const std::string & delims=", \t");

BEGIN_EVENT_TABLE(CSwapPositionDlg, wxDialog)
	EVT_CLOSE(OnClose)
//	EVT_CHAR_HOOK(OnHookKeyPress)
    EVT_COMMAND(wxID_ANY, wxEVT_MOVEPOSITION_TIMER, OnRcvTimer)
	EVT_COMMAND(wxID_ANY, wxEVT_MOVEPOSITION_ORDERLOG, OnRcvOrderLog)
	EVT_COMMAND(wxID_ANY, wxEVT_MOVEPOSITION_TRADELOG, OnRcvTradeLog)
	EVT_COMMAND(wxID_ANY, wxEVT_MOVEPOSITION_ACTION, OnRcvAction)
	EVT_BUTTON(ID_WXBUTTON2, OnStart)
	EVT_BUTTON(ID_WXBUTTONEDIT,OnEdit)
	EVT_BUTTON(ID_WXBUTTON3,OnCancel)
	EVT_BUTTON(ID_WXBUTTONCLOSE,OnBtnClose)
	EVT_BUTTON(ID_WXBUTTON1,OnSetDefault)
	EVT_COMBOBOX(ID_WXCOMBOBOX1, OnComboboxSelectChange)
	EVT_COMBOBOX(ID_WXCOMBOBOX2, OnComboboxSelectChange)
	EVT_COMBOBOX(ID_WXCOMBOBOX4, OnComboboxSelectChange)
	EVT_COMBOBOX(ID_WXCOMBOBOX5, OnComboboxSelectChange)
	EVT_COMBOBOX(ID_WXCOMBOBOX6, OnComboboxSelectChange)
	EVT_COMBOBOX(ID_WXCOMBOBOX7, OnComboboxSelectChange)
	EVT_CHILD_FOCUS(OnChildFocus)
	EVT_SPIN_UP(ID_WXSPINCTRL1, OnSpinUpPress)
	EVT_SPIN_DOWN(ID_WXSPINCTRL1, OnSpinDownPress)
	EVT_SPIN_UP(ID_WXSPINCTRL2, OnSpinUpPress)
	EVT_SPIN_DOWN(ID_WXSPINCTRL2, OnSpinDownPress)
	EVT_SPIN_UP(ID_WXSPINCTRL3, OnSpinUpPress)
	EVT_SPIN_DOWN(ID_WXSPINCTRL3, OnSpinDownPress)
	EVT_SPIN_UP(ID_WXSPINCTRL4, OnSpinUpPress)
	EVT_SPIN_DOWN(ID_WXSPINCTRL4, OnSpinDownPress)
	EVT_CHECKBOX(ID_WXCHECKBOX1, OnCheckClick)
	EVT_COMMAND_RANGE(ID_WXRADIOBUTTON1,ID_WXRADIOBUTTON16,wxEVT_COMMAND_RADIOBUTTON_SELECTED,OnRadioButton)
END_EVENT_TABLE()

CSwapPositionDlg::CSwapPositionDlg(wxWindow *parent)
	:wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(320,255)),
	m_pCustomSpinCtrl1(NULL),m_pCustomSpinCtrl2(NULL),m_pCustomSpinCtrl3(NULL),m_pCustomSpinCtrl4(NULL),
	m_pRadioGroupOriginal1(NULL),m_pRadioGroupOriginal2(NULL),m_pRadioGroupDest1(NULL),m_pRadioGroupDest2(NULL),
	m_pCNumericTextCtrl1(NULL),m_pCNumericTextCtrl2(NULL),m_pCNumericTextCtrl3(NULL),m_pCNumericTextCtrl4(NULL),
	m_pCNumericTextCtrl5(NULL),m_pCNumericTextCtrl6(NULL),m_pCNumericTextCtrl7(NULL),m_pCNumericTextCtrl8(NULL),
	m_pCNumericTextCtrl9(NULL),m_pCNumericTextCtrl10(NULL),m_pComboBox1(NULL),m_pComboBox2(NULL),m_pComboBox3(NULL),
	m_pComboBox4(NULL),m_pComboBox5(NULL),m_pComboBox6(NULL),m_pComboBox7(NULL)
{
	DIALOG_CREATE_LOG();
	m_pModifyOrderDlg = new CModifyOrderDlg(NULL);
	m_pModifyOrderDlg->Show(false);
	
	m_pRadioGroupOriginal1 = new CRadioButtonGroup;
	m_pRadioGroupOriginal2 = new CRadioButtonGroup;
	m_pRadioGroupDest1 = new CRadioButtonGroup;
	m_pRadioGroupDest2 = new CRadioButtonGroup;

	ZeroMemory(&m_tSwap, sizeof(CMovePositionServiceThread::SWAPPOSITION));
	ZeroMemory(&m_curOrderOption, sizeof(m_curOrderOption));

	m_poMovePositionServiceThread = NULL;
	
	AUTO_SETSIZE();
	CreateGUIControls();
	SetIcon(wxNullIcon);
	Center();
}

CSwapPositionDlg::~CSwapPositionDlg()
{
	if (m_pRadioGroupDest2)
	{
		delete m_pRadioGroupDest2;
		m_pRadioGroupDest2 = NULL;
	}
	if (m_pRadioGroupDest1)
	{
		delete m_pRadioGroupDest1;
		m_pRadioGroupDest1 = NULL;
	}
	if (m_pRadioGroupOriginal2)
	{
		delete m_pRadioGroupOriginal2;
		m_pRadioGroupOriginal2 = NULL;
	}
	if (m_pRadioGroupOriginal1)
	{
		delete m_pRadioGroupOriginal1;
		m_pRadioGroupOriginal1 = NULL;
	}
	if (m_pModifyOrderDlg)
		m_pModifyOrderDlg->Destroy();
}

// 是否用涨跌停价模拟市价，当交易所不支持市价时
BOOL CSwapPositionDlg::NeedSimulateMarketPrice()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return TRUE;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	return p->bSimulate;
}

void CSwapPositionDlg::CreateGUIControls()
{
    int nMH = 24;

    wxString strText;
	strText.Printf("%s", LOADSTRING(MARKETTYPE_CONFIRM_MESSAGE));
	wxStaticText* pStaticText0 = new wxStaticText(this, ID_MARKETTYPE_CONFIRMMSG_STATIC, 
			strText, wxPoint(5, 5));

	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxNotebook * WxNotebook1 = new wxNotebook(this, ID_WXNOTEBOOK1, wxPoint(508, 37+nMH), wxSize(446, 408), wxNB_DEFAULT);
	wxNotebook * WxNotebook2 = new wxNotebook(this, ID_WXNOTEBOOK2, wxPoint(8, 37+nMH), wxSize(490, 410), wxNB_DEFAULT);

	wxPanel * WxNoteBookPage1 = new wxPanel(WxNotebook2, ID_WXNOTEBOOKPAGE1, wxPoint(4, 23), wxSize(482, 383));
	WxNotebook2->AddPage(WxNoteBookPage1, wxT("价格设置"));
	wxPanel * WxNoteBookPage2 = new wxPanel(WxNotebook2, ID_WXNOTEBOOKPAGE2, wxPoint(4, 23), wxSize(482, 383));
	WxNotebook2->AddPage(WxNoteBookPage2, wxT("超时设置"));
	wxPanel * WxNoteBookPage3 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE3, wxPoint(4, 23), wxSize(438, 381));
	WxNotebook1->AddPage(WxNoteBookPage3, wxT("仓位1执行情况"));
	wxPanel * WxNoteBookPage4 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE4, wxPoint(4, 23), wxSize(438, 381));
	WxNotebook1->AddPage(WxNoteBookPage4, wxT("仓位2执行情况"));
	wxNotebook * WxNotebook3 = new wxNotebook(WxNoteBookPage3, ID_WXNOTEBOOK3, wxPoint(13, 70), wxSize(414, 120), wxNB_DEFAULT);
	wxNotebook * WxNotebook4 = new wxNotebook(WxNoteBookPage3, ID_WXNOTEBOOK4, wxPoint(13, 254), wxSize(414, 120), wxNB_DEFAULT);
	wxPanel * WxNoteBookPage5 = new wxPanel(WxNotebook3, ID_WXNOTEBOOKPAGE5, wxPoint(4, 23), wxSize(409, 51));
	wxNotebook * WxNotebook5 = new wxNotebook(WxNoteBookPage4, ID_WXNOTEBOOK5, wxPoint(13, 70), wxSize(414, 120), wxNB_DEFAULT);
	wxNotebook * WxNotebook6 = new wxNotebook(WxNoteBookPage4, ID_WXNOTEBOOK6, wxPoint(13, 254), wxSize(414, 120), wxNB_DEFAULT);
	WxNotebook3->AddPage(WxNoteBookPage5, wxT("成交情况"));
	wxPanel * WxNoteBookPage6 = new wxPanel(WxNotebook3, ID_WXNOTEBOOKPAGE6, wxPoint(4, 23), wxSize(409, 51));
	WxNotebook3->AddPage(WxNoteBookPage6, wxT("委托状态"));
	wxPanel * WxNoteBookPage7 = new wxPanel(WxNotebook4, ID_WXNOTEBOOKPAGE7, wxPoint(4, 23), wxSize(402, 57));
	WxNotebook4->AddPage(WxNoteBookPage7, wxT("成交情况"));
	wxPanel * WxNoteBookPage8 = new wxPanel(WxNotebook4, ID_WXNOTEBOOKPAGE8, wxPoint(4, 23), wxSize(402, 57));
	WxNotebook4->AddPage(WxNoteBookPage8, wxT("委托状态"));
	wxPanel * WxNoteBookPage9 = new wxPanel(WxNotebook5, ID_WXNOTEBOOKPAGE9, wxPoint(4, 23), wxSize(403, 39));
	WxNotebook5->AddPage(WxNoteBookPage9, wxT("成交情况"));
	wxPanel * WxNoteBookPage10 = new wxPanel(WxNotebook5, ID_WXNOTEBOOKPAGE10, wxPoint(4, 23), wxSize(403, 39));
	WxNotebook5->AddPage(WxNoteBookPage10, wxT("委托状态"));
	wxPanel * WxNoteBookPage11 = new wxPanel(WxNotebook6, ID_WXNOTEBOOKPAGE11, wxPoint(4, 23), wxSize(393, 53));
	WxNotebook6->AddPage(WxNoteBookPage11, wxT("成交情况"));
	wxPanel * WxNoteBookPage12 = new wxPanel(WxNotebook6, ID_WXNOTEBOOKPAGE12, wxPoint(4, 23), wxSize(393, 53));
	WxNotebook6->AddPage(WxNoteBookPage12, wxT("委托状态"));

	wxArrayString arrayStringFor_WxComboBox1;
	m_pComboBox1 = new wxComboBox(this, ID_WXCOMBOBOX1, wxT(""), wxPoint(48, 8+nMH), wxSize(103, 20), arrayStringFor_WxComboBox1, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox1"));

	wxArrayString arrayStringFor_WxComboBox2;
	m_pComboBox2 = new wxComboBox(this, ID_WXCOMBOBOX2, wxT(""), wxPoint(243, 8+nMH), wxSize(113, 20), arrayStringFor_WxComboBox2, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox2"));

	wxArrayString arrayStringFor_WxComboBox3;
	arrayStringFor_WxComboBox3.Add(wxT("先平后开"));
	arrayStringFor_WxComboBox3.Add(wxT("同时平开"));
	m_pComboBox3 = new wxComboBox(this, ID_WXCOMBOBOX3, wxT("先平后开"), wxPoint(750, 8+nMH), wxSize(123, 20), arrayStringFor_WxComboBox3, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox3"));
	m_pComboBox3->SetSelection(0);

	wxArrayString arrayStringFor_WxComboBox4;
	arrayStringFor_WxComboBox4.Add(wxT("(无动作)"));
	arrayStringFor_WxComboBox4.Add(wxT("按最新价浮动改单"));
	arrayStringFor_WxComboBox4.Add(wxT("按买卖价浮动改单"));
	arrayStringFor_WxComboBox4.Add(wxT("按市价改单"));
	arrayStringFor_WxComboBox4.Add(wxT("撤单并撤销"));
	m_pComboBox4 = new wxComboBox(WxNoteBookPage2, ID_WXCOMBOBOX4, wxT(""), wxPoint(153, 72), wxSize(145, 20), arrayStringFor_WxComboBox4, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox4"));
	m_pComboBox4->SetSelection(0);

	wxArrayString arrayStringFor_WxComboBox5;
	arrayStringFor_WxComboBox5.Add(wxT("(无动作)"));
	arrayStringFor_WxComboBox5.Add(wxT("按最新价浮动改单"));
	arrayStringFor_WxComboBox5.Add(wxT("按买卖价浮动改单"));
	arrayStringFor_WxComboBox5.Add(wxT("按市价改单"));
	arrayStringFor_WxComboBox5.Add(wxT("撤单并撤销"));
	m_pComboBox5 = new wxComboBox(WxNoteBookPage2, ID_WXCOMBOBOX5, wxT(""), wxPoint(153, 101), wxSize(145, 20), arrayStringFor_WxComboBox5, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox5"));
	m_pComboBox5->SetSelection(0);

	wxArrayString arrayStringFor_WxComboBox6;
	arrayStringFor_WxComboBox6.Add(wxT("(无动作)"));
	arrayStringFor_WxComboBox6.Add(wxT("按最新价浮动改单"));
	arrayStringFor_WxComboBox6.Add(wxT("按买卖价浮动改单"));
	arrayStringFor_WxComboBox6.Add(wxT("按市价改单"));
	arrayStringFor_WxComboBox6.Add(wxT("撤单并撤销"));
	m_pComboBox6 = new wxComboBox(WxNoteBookPage2, ID_WXCOMBOBOX6, wxT(""), wxPoint(153, 164), wxSize(145, 20), arrayStringFor_WxComboBox6, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox6"));
	m_pComboBox6->SetSelection(0);

	wxArrayString arrayStringFor_WxComboBox7;
	arrayStringFor_WxComboBox7.Add(wxT("(无动作)"));
	arrayStringFor_WxComboBox7.Add(wxT("按最新价浮动改单"));
	arrayStringFor_WxComboBox7.Add(wxT("按买卖价浮动改单"));
	arrayStringFor_WxComboBox7.Add(wxT("按市价改单"));
	arrayStringFor_WxComboBox7.Add(wxT("撤单并撤销"));
	m_pComboBox7 = new wxComboBox(WxNoteBookPage2, ID_WXCOMBOBOX7, wxT(""), wxPoint(153, 195), wxSize(145, 20), arrayStringFor_WxComboBox7, wxTE_READONLY, wxDefaultValidator, wxT("WxComboBox7"));
	m_pComboBox7->SetSelection(0);

	new wxButton(this, ID_WXBUTTON1, wxT("重置"), wxPoint(318, 453+nMH), wxSize(78, 24), 0, wxDefaultValidator, wxT("WxButton1"));
	new wxButton(this, ID_WXBUTTON2, wxT("开始"), wxPoint(406, 452+nMH), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton2"));
	new wxButton(this, ID_WXBUTTONEDIT, wxT("编辑"), wxPoint(406, 452+nMH), wxSize(70, 24), 0, wxDefaultValidator, wxT("WxButton2"));
	new wxButton(this, ID_WXBUTTON3, wxT("取消"), wxPoint(857, 453+nMH), wxSize(61, 26), 0, wxDefaultValidator, wxT("WxButton3"));
	new wxButton(this, ID_WXBUTTONCLOSE, wxT("关闭"), wxPoint(857, 453+nMH), wxSize(61, 26), 0, wxDefaultValidator, wxT("WxButton3"));

	m_pCNumericTextCtrl1 = new CNumericTextCtrl(WxNoteBookPage1, ID_WXEDIT1, wxT("0"), wxPoint(293, 65), wxSize(38, 21), 0, wxDefaultValidator, wxT("WxEdit1"));
	m_pCNumericTextCtrl2 = new CNumericTextCtrl(WxNoteBookPage1, ID_WXEDIT2, wxT("0"), wxPoint(293, 138), wxSize(38, 21), 0, wxDefaultValidator, wxT("WxEdit2"));
	m_pCNumericTextCtrl3 = new CNumericTextCtrl(WxNoteBookPage1, ID_WXEDIT3, wxT("0"), wxPoint(293, 248), wxSize(38, 21), 0, wxDefaultValidator, wxT("WxEdit3"));
	m_pCNumericTextCtrl4 = new CNumericTextCtrl(WxNoteBookPage1, ID_WXEDIT4, wxT("0"), wxPoint(293, 325), wxSize(38, 21), 0, wxDefaultValidator, wxT("WxEdit4"));
	m_pCNumericTextCtrl5 = new CNumericTextCtrl(WxNoteBookPage2, ID_WXEDIT5, wxT("0"), wxPoint(79, 15), wxSize(46, 21), 0, wxDefaultValidator, wxT("WxEdit5"));
	m_pCNumericTextCtrl6 = new CNumericTextCtrl(WxNoteBookPage2, ID_WXEDIT6, wxT("0"), wxPoint(271, 15), wxSize(46, 21), 0, wxDefaultValidator, wxT("WxEdit6"));
	m_pCNumericTextCtrl7 = new CNumericTextCtrl(WxNoteBookPage2, ID_WXEDIT7, wxT("0"), wxPoint(378, 72), wxSize(73, 21), 0, wxDefaultValidator, wxT("WxEdit7"));
	m_pCNumericTextCtrl8 = new CNumericTextCtrl(WxNoteBookPage2, ID_WXEDIT8, wxT("0"), wxPoint(378, 101), wxSize(73, 21), 0, wxDefaultValidator, wxT("WxEdit8"));
	m_pCNumericTextCtrl9 = new CNumericTextCtrl(WxNoteBookPage2, ID_WXEDIT9, wxT("0"), wxPoint(378, 164), wxSize(73, 21), 0, wxDefaultValidator, wxT("WxEdit9"));
	m_pCNumericTextCtrl10 = new CNumericTextCtrl(WxNoteBookPage2, ID_WXEDIT10, wxT("0"), wxPoint(378, 195), wxSize(73, 21), 0, wxDefaultValidator, wxT("WxEdit10"));

	m_pCNumericTextCtrl1->SetFilter(g_slide);
	m_pCNumericTextCtrl2->SetFilter(g_slide);
	m_pCNumericTextCtrl3->SetFilter(g_slide);
	m_pCNumericTextCtrl4->SetFilter(g_slide);
	m_pCNumericTextCtrl7->SetFilter(g_slide);
	m_pCNumericTextCtrl8->SetFilter(g_slide);
	m_pCNumericTextCtrl9->SetFilter(g_slide);
	m_pCNumericTextCtrl10->SetFilter(g_slide);

	new wxStaticText(this, ID_WXSTATICTEXT1, wxT("仓位1:"), wxPoint(7, 11+nMH), wxDefaultSize, 0, wxT("WxStaticText1"));
	new wxStaticText(this, ID_WXSTATICTEXT2, wxT("仓位2:"), wxPoint(197, 11+nMH), wxDefaultSize, 0, wxT("WxStaticText2"));
	new wxStaticText(this, ID_WXSTATICTEXT3, wxT("进行仓位互换"), wxPoint(370, 11+nMH), wxDefaultSize, 0, wxT("WxStaticText3"));
	new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT4, wxT("仓位1: ___ 平仓 __ __手 按以下价格"), wxPoint(17, 26), wxDefaultSize, 0, wxT("WxStaticText4"));
	new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT5, wxT("滑动价差"), wxPoint(236, 67), wxDefaultSize, 0, wxT("WxStaticText5"));
	new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT6, wxT("仓位1: ___ 开仓 __ __手 按以下价格"), wxPoint(17, 100), wxDefaultSize, 0, wxT("WxStaticText6"));
	new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT7, wxT("滑动价差"), wxPoint(236, 142), wxDefaultSize, 0, wxT("WxStaticText7"));
	new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT8, wxT("仓位2: ___ 平仓 __ __手 按以下价格"), wxPoint(17, 209), wxDefaultSize, 0, wxT("WxStaticText8"));
	new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT9, wxT("滑动价差"), wxPoint(237, 252), wxDefaultSize, 0, wxT("WxStaticText9"));
	new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT10, wxT("仓位2: ___ 开仓 __ __手 按以下价格"), wxPoint(17, 285), wxDefaultSize, 0, wxT("WxStaticText10"));
	new wxStaticText(WxNoteBookPage1, ID_WXSTATICTEXT11, wxT("滑动价差"), wxPoint(236, 329), wxDefaultSize, 0, wxT("WxStaticText11"));
	new wxStaticText(this, ID_WXSTATICTEXT12, wxT("操作顺序"), wxPoint(690, 12+nMH), wxDefaultSize, 0, wxT("WxStaticText12"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT13, wxT("超时设置"), wxPoint(22, 17), wxDefaultSize, 0, wxT("WxStaticText13"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT14, wxT("秒"), wxPoint(135, 17), wxDefaultSize, 0, wxT("WxStaticText14"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT15, wxT("重试次数"), wxPoint(215, 17), wxDefaultSize, 0, wxT("WxStaticText15"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT16, wxT("次"), wxPoint(327, 17), wxDefaultSize, 0, wxT("WxStaticText16"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT17, wxT("合约1平仓未成交处理"), wxPoint(30, 76), wxDefaultSize, 0, wxT("WxStaticText17"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT18, wxT("滑动价差"), wxPoint(318, 76), wxDefaultSize, 0, wxT("WxStaticText18"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT19, wxT("合约1开仓未成交处理"), wxPoint(30, 105), wxDefaultSize, 0, wxT("WxStaticText19"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT20, wxT("滑动价差"), wxPoint(318, 105), wxDefaultSize, 0, wxT("WxStaticText20"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT21, wxT("合约2平仓未成交处理"), wxPoint(30, 170), wxDefaultSize, 0, wxT("WxStaticText21"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT22, wxT("滑动价差"), wxPoint(318, 170), wxDefaultSize, 0, wxT("WxStaticText22"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT23, wxT("合约2开仓未成交处理"), wxPoint(30, 199), wxDefaultSize, 0, wxT("WxStaticText23"));
	new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT24, wxT("滑动价差"), wxPoint(318, 199), wxDefaultSize, 0, wxT("WxStaticText24"));
	new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT25, wxT("执行时间_秒，已重试_次，剩余_次重试机会"), wxPoint(13, 10), wxDefaultSize, 0, wxT("WxStaticText25"));
	new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT26, wxT("原始合约平仓_秒后，按最新价浮动_个价位改单"), wxPoint(13, 30), wxDefaultSize, 0, wxT("WxStaticText26"));
	new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT27, wxT("原始合约___开仓情况"), wxPoint(13, 50), wxDefaultSize, 0, wxT("WxStaticText27"));
	new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT28, wxT("执行时间_秒，已重试_次，剩余_次重试机会"), wxPoint(13, 194), wxDefaultSize, 0, wxT("WxStaticText28"));
	new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT29, wxT("目标合约平仓_秒后，按最新价浮动_个价位改单"), wxPoint(13, 214), wxDefaultSize, 0, wxT("WxStaticText29"));
	new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT30, wxT("目标合约___开仓情况"), wxPoint(13, 234), wxDefaultSize, 0, wxT("WxStaticText30"));
	new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT31, wxT("执行时间_秒，已重试_次，剩余_次重试机会"), wxPoint(13, 10), wxDefaultSize, 0, wxT("WxStaticText31"));
	new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT32, wxT("原始合约平仓_秒后，按最新价浮动_个价位改单"), wxPoint(13, 30), wxDefaultSize, 0, wxT("WxStaticText32"));
	new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT33, wxT("原始合约___开仓情况"), wxPoint(13, 50), wxDefaultSize, 0, wxT("WxStaticText33"));
	new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT34, wxT("执行时间_秒，已重试_次，剩余_次重试机会"), wxPoint(13, 194), wxDefaultSize, 0, wxT("WxStaticText34"));
	new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT35, wxT("目标合约平仓_秒后，按最新价浮动_个价位改单"), wxPoint(13, 214), wxDefaultSize, 0, wxT("WxStaticText35"));
	new wxStaticText(this, ID_WXSTATICTEXT36, wxT(""), wxPoint(585, 676), wxDefaultSize, 0, wxT("WxStaticText36"));
	new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT37, wxT("目标合约___开仓情况"), wxPoint(13, 234), wxDefaultSize, 0, wxT("WxStaticText37"));

	m_pCustomSpinCtrl1 = new CCustomSpinCtrl(WxNoteBookPage1, ID_WXEDIT11, ID_WXSPINCTRL1, wxT("0"), wxPoint(396, 65), wxSize(43, 21), wxSP_ARROW_KEYS, 0, 999999, 0);
	m_pCustomSpinCtrl2 = new CCustomSpinCtrl(WxNoteBookPage1, ID_WXEDIT12, ID_WXSPINCTRL2, wxT("0"), wxPoint(396, 138), wxSize(43, 21), wxSP_ARROW_KEYS, 0, 999999, 0);
	m_pCustomSpinCtrl3 = new CCustomSpinCtrl(WxNoteBookPage1, ID_WXEDIT13, ID_WXSPINCTRL3, wxT("0"), wxPoint(396, 248), wxSize(43, 21), wxSP_ARROW_KEYS, 0, 999999, 0);
	m_pCustomSpinCtrl4 = new CCustomSpinCtrl(WxNoteBookPage1, ID_WXEDIT14, ID_WXSPINCTRL4, wxT("0"), wxPoint(396, 325), wxSize(43, 21), wxSP_ARROW_KEYS, 0, 999999, 0);

	m_pCustomSpinCtrl1->SetFilter(g_assign);
	m_pCustomSpinCtrl2->SetFilter(g_assign);
	m_pCustomSpinCtrl3->SetFilter(g_assign);
	m_pCustomSpinCtrl4->SetFilter(g_assign);

	wxRadioButton * p = NULL;
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON1, wxT("市价"), wxPoint(20, 66), wxSize(49, 17), wxRB_GROUP, wxDefaultValidator, wxT("WxRadioButton1"));
	m_pRadioGroupOriginal1->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON2, wxT("最新价"), wxPoint(93, 66), wxSize(60, 17), 0, wxDefaultValidator, wxT("WxRadioButton3"));
	m_pRadioGroupOriginal1->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON3, wxT("买卖价"), wxPoint(166, 66), wxSize(60, 17), 0, wxDefaultValidator, wxT("WxRadioButton4"));
	m_pRadioGroupOriginal1->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON4, wxT("指定价"), wxPoint(340, 66), wxSize(56, 17), 0, wxDefaultValidator, wxT("WxRadioButton5"));
	m_pRadioGroupOriginal1->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON5, wxT("市价"), wxPoint(20, 141), wxSize(52, 17), wxRB_GROUP, wxDefaultValidator, wxT("WxRadioButton2"));
	m_pRadioGroupDest1->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON6, wxT("最新价"), wxPoint(93, 141), wxSize(68, 17), 0, wxDefaultValidator, wxT("WxRadioButton8"));
	m_pRadioGroupDest1->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON7, wxT("买卖价"), wxPoint(166, 141), wxSize(56, 17), 0, wxDefaultValidator, wxT("WxRadioButton9"));
	m_pRadioGroupDest1->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON8, wxT("指定价"), wxPoint(340, 141), wxSize(56, 17), 0, wxDefaultValidator, wxT("WxRadioButton10"));
	m_pRadioGroupDest1->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON9, wxT("市价"), wxPoint(21, 252), wxSize(46, 16), wxRB_GROUP, wxDefaultValidator, wxT("WxRadioButton6"));
	m_pRadioGroupOriginal2->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON10, wxT("最新价"), wxPoint(94, 251), wxSize(55, 17), 0, wxDefaultValidator, wxT("WxRadioButton13"));
	m_pRadioGroupOriginal2->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON11, wxT("买卖价"), wxPoint(167, 251), wxSize(55, 17), 0, wxDefaultValidator, wxT("WxRadioButton14"));
	m_pRadioGroupOriginal2->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON12, wxT("指定价"), wxPoint(340, 251), wxSize(56, 17), 0, wxDefaultValidator, wxT("WxRadioButton15"));
	m_pRadioGroupOriginal2->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON13, wxT("市价"), wxPoint(20, 328), wxSize(45, 17), wxRB_GROUP, wxDefaultValidator, wxT("WxRadioButton7"));
	m_pRadioGroupDest2->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON14, wxT("最新价"), wxPoint(93, 328), wxSize(57, 17), 0, wxDefaultValidator, wxT("WxRadioButton18"));
	m_pRadioGroupDest2->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON15, wxT("买卖价"), wxPoint(166, 328), wxSize(57, 17), 0, wxDefaultValidator, wxT("WxRadioButton19"));
	m_pRadioGroupDest2->InsertGroup(p);
	p = new wxRadioButton(WxNoteBookPage1, ID_WXRADIOBUTTON16, wxT("指定价"), wxPoint(340, 328), wxSize(56, 17), 0, wxDefaultValidator, wxT("WxRadioButton20"));
	m_pRadioGroupDest2->InsertGroup(p);

	new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX1, wxT("仓位1开平"), wxPoint(6, 5), wxSize(469, 173));
	new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX2, wxT("限价价格设置"), wxPoint(11, 47), wxSize(452, 46));
	new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX3, wxT("限价价格设置"), wxPoint(11, 119), wxSize(451, 48));
	new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX4, wxT("仓位2开平"), wxPoint(6, 186), wxSize(470, 183));
	new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX5, wxT("限价价格设置"), wxPoint(11, 230), wxSize(453, 48));
	new wxStaticBox(WxNoteBookPage1, ID_WXSTATICBOX6, wxT("限价价格设置"), wxPoint(11, 307), wxSize(453, 47));
	new wxStaticBox(WxNoteBookPage2, ID_WXSTATICBOX7, wxT("仓位1超时设置"), wxPoint(16, 48), wxSize(447, 82));
	new wxStaticBox(WxNoteBookPage2, ID_WXSTATICBOX8, wxT("仓位2超时设置"), wxPoint(16, 146), wxSize(449, 83));

	wxListCtrl * WxListCtrl1 = new wxListCtrl(WxNoteBookPage5, ID_WXLISTCTRL1, wxPoint(8, 7), wxSize(390, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl1"));
	wxListCtrl * WxListCtrl5 = new wxListCtrl(WxNoteBookPage6, ID_WXLISTCTRL5, wxPoint(8, 7), wxSize(390, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl5"));
	wxListCtrl * WxListCtrl2 = new wxListCtrl(WxNoteBookPage7, ID_WXLISTCTRL2, wxPoint(8, 7), wxSize(390, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl2"));
	wxListCtrl * WxListCtrl6 = new wxListCtrl(WxNoteBookPage8, ID_WXLISTCTRL6, wxPoint(8, 7), wxSize(390, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl6"));

 	WxListCtrl1->InsertColumn(0,wxT("状态"),wxLIST_FORMAT_LEFT,50 );
 	WxListCtrl1->InsertColumn(0,wxT("剩余数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("成交数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("成交价"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("报单数"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("委托价格"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("成交编号"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl1->InsertColumn(0,wxT("报单编号"),wxLIST_FORMAT_LEFT,67 );

 	WxListCtrl2->InsertColumn(0,wxT("状态"),wxLIST_FORMAT_LEFT,50 );
 	WxListCtrl2->InsertColumn(0,wxT("剩余数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("成交数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("成交价"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("报单数"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("委托价格"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("成交编号"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl2->InsertColumn(0,wxT("报单编号"),wxLIST_FORMAT_LEFT,67 );

	wxListCtrl * WxListCtrl3 = new wxListCtrl(WxNoteBookPage9, ID_WXLISTCTRL3, wxPoint(8, 7), wxSize(390, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl3"));
	wxListCtrl * WxListCtrl7 = new wxListCtrl(WxNoteBookPage10, ID_WXLISTCTRL7, wxPoint(8, 7), wxSize(390, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl7"));
	wxListCtrl * WxListCtrl4 = new wxListCtrl(WxNoteBookPage11, ID_WXLISTCTRL4, wxPoint(8, 7), wxSize(390, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl4"));
	wxListCtrl * WxListCtrl8 = new wxListCtrl(WxNoteBookPage12, ID_WXLISTCTRL8, wxPoint(8, 7), wxSize(390, 80), wxLC_REPORT, wxDefaultValidator, wxT("WxListCtrl8"));

 	WxListCtrl3->InsertColumn(0,wxT("状态"),wxLIST_FORMAT_LEFT,50 );
 	WxListCtrl3->InsertColumn(0,wxT("剩余数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl3->InsertColumn(0,wxT("成交数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl3->InsertColumn(0,wxT("成交价"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl3->InsertColumn(0,wxT("报单数"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl3->InsertColumn(0,wxT("委托价格"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl3->InsertColumn(0,wxT("成交编号"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl3->InsertColumn(0,wxT("报单编号"),wxLIST_FORMAT_LEFT,67 );

 	WxListCtrl4->InsertColumn(0,wxT("状态"),wxLIST_FORMAT_LEFT,50 );
 	WxListCtrl4->InsertColumn(0,wxT("剩余数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl4->InsertColumn(0,wxT("成交数量"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl4->InsertColumn(0,wxT("成交价"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl4->InsertColumn(0,wxT("报单数"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl4->InsertColumn(0,wxT("委托价格"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl4->InsertColumn(0,wxT("成交编号"),wxLIST_FORMAT_LEFT,67 );
 	WxListCtrl4->InsertColumn(0,wxT("报单编号"),wxLIST_FORMAT_LEFT,67 );

	new wxCheckBox(this, ID_WXCHECKBOX1, wxT("只能在相同数量不同方向间互换"), wxPoint(471, 10+nMH), wxSize(189, 16), 0, wxDefaultValidator, wxT("WxCheckBox1"));

// 	wxWindow* pButton = (FindWindowById(ID_WXBUTTONEDIT, this));
// 	if(pButton) pButton->Show(false);
// 	pButton = (FindWindowById(ID_WXBUTTON3, this));
// 	if(pButton) pButton->Show(false);
// 	pButton = (FindWindowById(ID_WXBUTTONCLOSE, this));
// 	if(pButton) pButton->Show(false);


	SetTitle(wxT("仓位互换"));
	SetIcon(wxNullIcon);
	SetSize(3,6,971,520+nMH);
	Center();

	////GUI Items Creation End
}

void CSwapPositionDlg::LoadConfig()
{

	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return;
	REPLACEPOSITION swapCfg = pMgr->GetReplacePositionCfg();

	m_pRadioGroupOriginal1->SetGroupValue(swapCfg.nClosePositionPriceLimit1);
	m_pRadioGroupDest1->SetGroupValue(swapCfg.nOpenPositionPriceLimit1);
	m_pRadioGroupOriginal2->SetGroupValue(swapCfg.nClosePositionPriceLimit2);
	m_pRadioGroupDest2->SetGroupValue(swapCfg.nOpenPositionPriceLimit2);

	wxString strText;

	strText.Printf("%d", swapCfg.nClosePositionPriceOffset1);
	if(m_pCNumericTextCtrl1) m_pCNumericTextCtrl1->SetValue(strText);
	strText.Printf("%d", swapCfg.nOpenPositionPriceOffset1);
	if(m_pCNumericTextCtrl2) m_pCNumericTextCtrl2->SetValue(strText);
	strText.Printf("%d", swapCfg.nClosePositionPriceOffset2);
	if(m_pCNumericTextCtrl3) m_pCNumericTextCtrl3->SetValue(strText);
	strText.Printf("%d", swapCfg.nOpenPositionPriceOffset2);
	if(m_pCNumericTextCtrl4) m_pCNumericTextCtrl4->SetValue(strText);
	strText.Printf("%d", swapCfg.nOutTimeSec);
	if(m_pCNumericTextCtrl5) m_pCNumericTextCtrl5->SetValue(strText);
	strText.Printf("%d", swapCfg.nRetryTimes);
	if(m_pCNumericTextCtrl6) m_pCNumericTextCtrl6->SetValue(strText);
	strText.Printf("%d", swapCfg.nOpenPositionNotDealPriceOffset1);
	if(m_pCNumericTextCtrl7) m_pCNumericTextCtrl7->SetValue(strText);
	strText.Printf("%d", swapCfg.nClosePositionNotDealPriceOffset1);
	if(m_pCNumericTextCtrl8) m_pCNumericTextCtrl8->SetValue(strText);
	strText.Printf("%d", swapCfg.nOpenPositionNotDealPriceOffset2);
	if(m_pCNumericTextCtrl9) m_pCNumericTextCtrl9->SetValue(strText);
	strText.Printf("%d", swapCfg.nClosePositionNotDealPriceOffset2);
	if(m_pCNumericTextCtrl10) m_pCNumericTextCtrl10->SetValue(strText);

	if(m_pComboBox3) m_pComboBox3->SetSelection(swapCfg.nOperateOrder);
	if(m_pComboBox4) m_pComboBox4->SetSelection(swapCfg.nOpenPositionNotDealAction1);
	if(m_pComboBox5) m_pComboBox5->SetSelection(swapCfg.nClosePositionNotDealAction1);
	if(m_pComboBox6) m_pComboBox6->SetSelection(swapCfg.nOpenPositionNotDealAction2);
	if(m_pComboBox7) m_pComboBox7->SetSelection(swapCfg.nClosePositionNotDealAction2);


}

void CSwapPositionDlg::SaveConfig()
{
	
	// 这里从xml中读是否要确认的配置
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return;
	REPLACEPOSITION swapCfg;

	wxRadioButton* pRadiobutton = NULL;

	swapCfg.nClosePositionPriceLimit1 = m_pRadioGroupOriginal1->GetGroupValue();
	swapCfg.nOpenPositionPriceLimit1 = m_pRadioGroupDest1->GetGroupValue();
	swapCfg.nClosePositionPriceLimit2 = m_pRadioGroupOriginal2->GetGroupValue();
	swapCfg.nOpenPositionPriceLimit2 = m_pRadioGroupDest2->GetGroupValue();

	wxString strText;

	if(m_pCNumericTextCtrl1) swapCfg.nClosePositionPriceOffset1 = atoi(m_pCNumericTextCtrl1->GetValue().c_str());
	if(m_pCNumericTextCtrl2) swapCfg.nOpenPositionPriceOffset1 = atoi(m_pCNumericTextCtrl2->GetValue().c_str());
	if(m_pCNumericTextCtrl3) swapCfg.nClosePositionPriceOffset2 = atoi(m_pCNumericTextCtrl3->GetValue().c_str());
	if(m_pCNumericTextCtrl4) swapCfg.nOpenPositionPriceOffset2 = atoi(m_pCNumericTextCtrl4->GetValue().c_str());
	if(m_pCNumericTextCtrl5) swapCfg.nOutTimeSec = atoi(m_pCNumericTextCtrl5->GetValue().c_str());
	if(m_pCNumericTextCtrl6) swapCfg.nRetryTimes = atoi(m_pCNumericTextCtrl6->GetValue().c_str());
	if(m_pCNumericTextCtrl7) swapCfg.nOpenPositionNotDealPriceOffset1 = atoi(m_pCNumericTextCtrl7->GetValue().c_str());
	if(m_pCNumericTextCtrl8) swapCfg.nClosePositionNotDealPriceOffset1 = atoi(m_pCNumericTextCtrl8->GetValue().c_str());
	if(m_pCNumericTextCtrl9) swapCfg.nOpenPositionNotDealPriceOffset2 = atoi(m_pCNumericTextCtrl9->GetValue().c_str());
	if(m_pCNumericTextCtrl10) swapCfg.nClosePositionNotDealPriceOffset2 = atoi(m_pCNumericTextCtrl10->GetValue().c_str());

	if(m_pComboBox3) swapCfg.nOperateOrder = m_pComboBox3->GetSelection();
	if(m_pComboBox4) swapCfg.nOpenPositionNotDealAction1 = m_pComboBox4->GetSelection();
	if(m_pComboBox5) swapCfg.nClosePositionNotDealAction1 = m_pComboBox5->GetSelection();
	if(m_pComboBox6) swapCfg.nOpenPositionNotDealAction2 = m_pComboBox6->GetSelection();
	if(m_pComboBox7) swapCfg.nClosePositionNotDealAction2 = m_pComboBox7->GetSelection();

	pMgr->SetReplacePositionCfg(swapCfg);

}

bool CSwapPositionDlg::InitGrid()
{

	m_RowDatas[0].clear();
	m_RowDatas[1].clear();
	m_RowDatas[2].clear();
	m_RowDatas[3].clear();

	wxListCtrl* pwxListCtrls[5];

	pwxListCtrls[0] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL5, this);
	pwxListCtrls[1] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL6, this);
	pwxListCtrls[2] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL7, this);
	pwxListCtrls[3] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL8, this);
	pwxListCtrls[4] = NULL;

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

BOOL CSwapPositionDlg::Init(CMovePositionServiceThread* poMovePositionServiceThread, vector<PlatformStru_Position> & vecPositionInfo)
{
	m_poMovePositionServiceThread = poMovePositionServiceThread;
	m_vecPositionStruct = vecPositionInfo;

	// 显示数据到界面上

	InitGrid();

	LoadConfig();
	SetDefault();

	InitAllCabin();

	wxWindow* pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) pButton->Show(true);
	pButton = FindWindowById(ID_WXBUTTON2, this);
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

void CSwapPositionDlg::OnClose(wxCloseEvent& event)
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
			wxMessageDialog dialog( NULL, wxT("仓位互换操作正在进行中，是否确认打断此操作？\n注：已进行的仓位操作和现有挂单均会维持现状"), _("中国国际期货网上交易终端"), 
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

bool CSwapPositionDlg::Show(bool show)
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

void CSwapPositionDlg::OnHookKeyPress(wxKeyEvent& evt)
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

//显示错误信息
void CSwapPositionDlg::ShowTradeInfoDlg(const string& Title, const string& errormessage, BOOL bIsShow)
{
    if(TRADEINFODLG(this)==NULL) return;
	TRADEINFODLG(this)->ShowTradeMsg(errormessage, -1, Title, -1, bIsShow);

//	LOG_DATA* pData = new LOG_DATA;
//	pData->strTitle = Title;
//	pData->strFormat = errormessage;
//	TRADEINFODLG(this)->WriteLog( pData );
}

//void CSwapPositionDlg::OnCfgChanged(wxCommandEvent& evt)
//{
//}

void CSwapPositionDlg::OnStart(wxCommandEvent& event)
{

	if (-1 == m_pComboBox1->GetSelection() || -1 == m_pComboBox2->GetSelection())
	{
		wxMessageBox("仓位1仓位2不能为空！","提示");
		return;
	}

	wxString strText;
	if(m_tPositionData1.TotalCloseOrderVolume>0) {
		wxString strTemp;
		strTemp.Printf("合约%s有未成交报单，请先关闭本窗口，手动撤单后再进行仓位互换操作。\n", m_tPositionData1.InstrumentID);
		strText+=strTemp;
	}
	if(m_tPositionData2.TotalCloseOrderVolume>0) {
		wxString strTemp;
		strTemp.Printf("合约%s有未成交报单，请先关闭本窗口，手动撤单后再进行仓位互换操作。\n", m_tPositionData2.InstrumentID);
		strText+=strTemp;
	}
	if(strText.Length()>0) {
		wxMessageDialog dialog( NULL, strText, _("提示"), wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return;
	}

	wxCheckBox * pCheckBox = static_cast<wxCheckBox *>(FindWindowById(ID_WXCHECKBOX1, this));
	if (pCheckBox)
	{
		if (pCheckBox->IsChecked())
		{

			if (m_pComboBox1 == NULL || m_pComboBox2 == NULL)
			{
				return;
			}

			map<wxString, STATIC_TITLE_DATA>::iterator iter1 = m_mapStaticData.find(m_pComboBox1->GetLabel());
			map<wxString, STATIC_TITLE_DATA>::iterator iter2 = m_mapStaticData.find(m_pComboBox2->GetLabel());
			if(iter1 != m_mapStaticData.end() && iter2 != m_mapStaticData.end()) {
				if ((iter1->second).Direct==(iter2->second).Direct || (iter1->second).nVol != (iter2->second).nVol)
				{
					wxMessageBox("只能在相同数量不同方向间互换，请重新选择！","提示");
					return;
				}
			}
		}
	}

	wxMessageDialog dialog( NULL, wxT("是否确认现在就进行仓位互换操作？"), _("提示"), wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	if(dialog.ShowModal()!=wxID_YES)
		return;

	SendRequest();
	SetAble(false);

	wxWindow* pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) 
		pButton->Show(false);
	pButton = FindWindowById(ID_WXBUTTON2, this);
	if(pButton) 
		pButton->Show(false);
	pButton = (FindWindowById(ID_WXBUTTON3, this));
	if(pButton) 
		pButton->Show(true);
	pButton = FindWindowById(ID_WXBUTTONEDIT, this);
	if(pButton) {
		pButton->Show(true);
		pButton->Enable(true);
	}

}

void CSwapPositionDlg::OnSetDefault(wxCommandEvent& event)
{
	ReadLastState();
}

void CSwapPositionDlg::OnEdit(wxCommandEvent& event)
{

	SetAble(true,false,false);

	wxWindow* pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) 
		pButton->Show(true);
	pButton = FindWindowById(ID_WXBUTTONEDIT, this);
	if(pButton) {
		pButton->Enable(false);
		pButton->Show(false);
	}

	m_pModifyOrderDlg->Clear();
	m_pModifyOrderDlg->SetOwner(this, FALSE);
	m_pModifyOrderDlg->Show();

	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tSwap.nMovePos1Num, TRUE);
	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tSwap.nMovePos2Num, TRUE);

}

void CSwapPositionDlg::OnCancel(wxCommandEvent& event)
{
	wxMessageDialog dialog( NULL, wxT("仓位互换操作正在进行中，是否确认打断此操作？\n注：已进行的仓位操作和现有挂单均会维持现状"), _("中国国际期货网上交易终端"), 
		wxNO_DEFAULT|wxYES_NO|wxICON_WARNING);
	int nResult = dialog.ShowModal();
	if(nResult==wxID_YES)
	{
		DoCancel();
	}
}

void CSwapPositionDlg::DoCancel()
{
	SetAble(false);
//	wxWindow* pButton = FindWindowById(ID_WXBUTTONEDIT, this);
//	if(pButton) {
//		pButton->Enable(true);
//	}
	m_poMovePositionServiceThread->LockObject();

	m_poMovePositionServiceThread->SwapPositionCancel(m_tSwap.nNum);

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

void CSwapPositionDlg::OnBtnClose(wxCommandEvent& event)
{
	Close();
	//Destroy();
}

bool CSwapPositionDlg::SendRequest(bool bIsRedo)
{
	
	CMovePositionServiceThread::SWAPPOSITION tSwap;
	CMovePositionServiceThread::MOVEPOSITION tMove1;
	CMovePositionServiceThread::MOVEPOSITION tMove2;

	ZeroMemory(&tSwap, sizeof(tSwap));
	ZeroMemory(&tMove1, sizeof(tMove1));
	ZeroMemory(&tMove2, sizeof(tMove2));

	PlatformStru_InstrumentInfo insInfo;
	ZeroMemory(&insInfo, sizeof(insInfo));
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(m_tPositionData1.InstrumentID,insInfo);
	m_fPriceTick1 = insInfo.PriceTick;
	m_pCustomSpinCtrl1->SetStep(m_fPriceTick1);
	m_pCustomSpinCtrl2->SetStep(m_fPriceTick1);

	ZeroMemory(&insInfo, sizeof(insInfo));
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(m_tPositionData2.InstrumentID,insInfo);
	m_fPriceTick2 = insInfo.PriceTick;
	m_pCustomSpinCtrl3->SetStep(m_fPriceTick2);
	m_pCustomSpinCtrl4->SetStep(m_fPriceTick2);


	//组装第一个

	{
		tMove1.nState = CMovePositionServiceThread::conMoveActive;
		tMove1.pWnd = this;

		//操作顺序
		if (m_pComboBox3) tMove1.nActionMode = m_pComboBox3->GetCurrentSelection();

		//超时设置
		long value;

		m_pCNumericTextCtrl5->GetValue().ToLong(&value);
		tMove1.nTimeOutInterval = value;
		m_curOrderOption.nTimeout = value;

		m_pCNumericTextCtrl6->GetValue().ToLong(&value);
		tMove1.nRetryNum = value;
		m_curOrderOption.nRetryTimes = value;

		//原始仓位
		CMovePositionServiceThread::POSITIONUNIT	tSrcPos;		// 原始仓位
		ZeroMemory(&tSrcPos, sizeof(tSrcPos));
		strcpy(tSrcPos.strInstrumentID, m_tPositionData1.InstrumentID);
		if (m_tPositionData1.PosiDirection == THOST_FTDC_PD_Long)
		{
			tSrcPos.bDirection = FALSE;
		}
		else
		{
			tSrcPos.bDirection = TRUE;
		}

		tSrcPos.fPriceTick = m_fPriceTick1;
        tSrcPos.nHedgeStatus=m_tPositionData1.HedgeFlag-THOST_FTDC_HF_Speculation;

		tSrcPos.nPriceType = m_pRadioGroupOriginal1->GetGroupValue();
		m_curOrderOption.nSourcePriceType1 = tSrcPos.nPriceType;

		if (0 == tSrcPos.nPriceType)
		{

			PlatformStru_InstrumentInfo outData;
			memset(&outData,0,sizeof(outData));

			if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(tSrcPos.strInstrumentID, outData);
//			if(strcmp(outData.ExchangeID, "SHFE")==0 && NeedSimulateMarketPrice()) 
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
		//	m_curOrderOption.fSourceAssign1 = val;
		//}
		tSrcPos.fPrice = atof(m_pCustomSpinCtrl1->GetTextCtrl()->GetLabel());
		m_curOrderOption.fSourceAssign1 = tSrcPos.fPrice;

		tSrcPos.nVolume = m_tPositionData1.Position;

		if (m_pCNumericTextCtrl1)
		{
			m_pCNumericTextCtrl1->GetValue().ToLong(&value);
			tSrcPos.nPriceOffset = value;
			m_curOrderOption.lSourceHuaDong1 = value;
		}

		if (m_pComboBox4)
		{
			tSrcPos.nTimeOutAction = m_pComboBox4->GetSelection();
			m_curOrderOption.nSourceHandle1 = m_pComboBox4->GetSelection();
		}

		if (m_pCNumericTextCtrl7)
		{
			m_pCNumericTextCtrl7->GetValue().ToLong(&value);
			tSrcPos.nTimeOutPriceOffset = value;
			m_curOrderOption.lSourceTimeoutHuaDong1 = value;
		}

		tSrcPos.nRemainVolume = m_tPositionData1.Position-m_tPositionData1.TodayPosition;
		tSrcPos.nRemainVolume_CloseToday = m_tPositionData1.TodayPosition;

		//m_poMovePositionServiceThread->LockObject();

		if(tSrcPos.nRemainVolume>0)
			m_poMovePositionServiceThread->MakeReq(wxString(tSrcPos.strInstrumentID), tSrcPos.bDirection, 2, tSrcPos.fPrice, 
					tSrcPos.nRemainVolume, tSrcPos.nHedgeStatus, (tSrcPos.nPriceType? FALSE:TRUE), 
					(tSrcPos.nPriceType==0 && NeedSimulateMarketPrice() ? TRUE:FALSE), tSrcPos.req);

		if(tSrcPos.nRemainVolume_CloseToday>0)
			m_poMovePositionServiceThread->MakeReq(wxString(tSrcPos.strInstrumentID), tSrcPos.bDirection, 1, tSrcPos.fPrice, 
					tSrcPos.nRemainVolume_CloseToday, tSrcPos.nHedgeStatus, (tSrcPos.nPriceType? FALSE:TRUE), 
					(tSrcPos.nPriceType==0 && NeedSimulateMarketPrice() ? TRUE:FALSE), tSrcPos.req_CloseToday);

		tMove1.tSrcPos = tSrcPos;

		//目标仓位

		CMovePositionServiceThread::POSITIONUNIT	tDestPos;		// 目标仓位
		ZeroMemory(&tDestPos, sizeof(tDestPos));
		strcpy(tDestPos.strInstrumentID, m_tPositionData1.InstrumentID);

		if (m_tPositionData2.PosiDirection == THOST_FTDC_PD_Long)
		{
			tDestPos.bDirection = TRUE;
		}
		else
		{
			tDestPos.bDirection = FALSE;
		}

		tDestPos.fPriceTick = m_fPriceTick1;
        tDestPos.nHedgeStatus=m_tPositionData1.HedgeFlag-THOST_FTDC_HF_Speculation;

		tDestPos.nPriceType = m_pRadioGroupDest1->GetGroupValue();
		m_curOrderOption.nDestPriceType1 = tDestPos.nPriceType;
		if (0 == tDestPos.nPriceType)
		{
			PlatformStru_InstrumentInfo outData;
			memset(&outData,0,sizeof(outData));

			if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(tSrcPos.strInstrumentID, outData);
//			if(strcmp(outData.ExchangeID, "SHFE")==0 && NeedSimulateMarketPrice()) 
			if(NEEDSIMULATEMARKET(outData.ExchangeID) && NeedSimulateMarketPrice()) 
				tDestPos.bSimulate = TRUE;
		}

		tDestPos.bIsClose = FALSE;

		//pSpinctrl = m_pCustomSpinCtrl2->GetSpinButton();
		//if (pSpinctrl)
		//{
		//	pSpinctrl->GetLabel().ToDouble(&val);
		//	tDestPos.fPrice = val;
		//	m_curOrderOption.fDestAssign1 = val;
		//}
		tDestPos.fPrice = atof(m_pCustomSpinCtrl2->GetTextCtrl()->GetLabel());
		m_curOrderOption.fDestAssign1 = tDestPos.fPrice;

		tDestPos.nVolume = m_tPositionData2.Position;

		if (m_pCNumericTextCtrl2)
		{
			m_pCNumericTextCtrl2->GetValue().ToLong(&value);
			tDestPos.nPriceOffset = value;
			m_curOrderOption.lDestHuaDong1 = value;
		}

		if (m_pComboBox5)
		{
			tDestPos.nTimeOutAction = m_pComboBox5->GetSelection();
			m_curOrderOption.nDestHandle1 = m_pComboBox5->GetSelection();
		}

		if (m_pCNumericTextCtrl8)
		{
			m_pCNumericTextCtrl8->GetValue().ToLong(&value);
			tDestPos.nTimeOutPriceOffset = value;
			m_curOrderOption.lDestTimeoutHuaDong1 = value;
		}

		tDestPos.nRemainVolume = tDestPos.nVolume;

		m_poMovePositionServiceThread->MakeReq(wxString(tDestPos.strInstrumentID), tDestPos.bDirection, 0, tDestPos.fPrice, 
				tDestPos.nRemainVolume, tDestPos.nHedgeStatus, (tDestPos.nPriceType? FALSE:TRUE), 
				(tDestPos.nPriceType==0 && NeedSimulateMarketPrice() ? TRUE:FALSE), tDestPos.req);

		tMove1.tDestPos = tDestPos;
	}


	//组装第二个
	{

		tMove2.nState = CMovePositionServiceThread::conMoveActive;
		tMove2.pWnd = this;

		//操作顺序
		if (m_pComboBox3) tMove2.nActionMode = m_pComboBox3->GetCurrentSelection();

		//超时设置
		long value;
		m_pCNumericTextCtrl5->GetValue().ToLong(&value);
		tMove2.nTimeOutInterval = value;
		m_pCNumericTextCtrl6->GetValue().ToLong(&value);
		tMove2.nRetryNum = value;

		//原始仓位
		CMovePositionServiceThread::POSITIONUNIT	tSrcPos;		// 原始仓位
		ZeroMemory(&tSrcPos, sizeof(tSrcPos));
		strcpy(tSrcPos.strInstrumentID, m_tPositionData2.InstrumentID);
		if (m_tPositionData2.PosiDirection == THOST_FTDC_PD_Long)
		{
			tSrcPos.bDirection = FALSE;
		}
		else
		{
			tSrcPos.bDirection = TRUE;
		}

		tSrcPos.fPriceTick = m_fPriceTick2;
        tSrcPos.nHedgeStatus=m_tPositionData2.HedgeFlag-THOST_FTDC_HF_Speculation;

		tSrcPos.nPriceType = m_pRadioGroupOriginal2->GetGroupValue();
		m_curOrderOption.nSourcePriceType2 = tSrcPos.nPriceType;
		if (0 == tSrcPos.nPriceType)
		{
			PlatformStru_InstrumentInfo outData;
			memset(&outData,0,sizeof(outData));

			if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(tSrcPos.strInstrumentID, outData);
//			if(strcmp(outData.ExchangeID, "SHFE")==0 && NeedSimulateMarketPrice()) 
			if(NEEDSIMULATEMARKET(outData.ExchangeID) && NeedSimulateMarketPrice()) 
				tSrcPos.bSimulate = TRUE;
		}

		tSrcPos.bIsClose = TRUE;

		//double val;
		//wxSpinButton * pSpinctrl = m_pCustomSpinCtrl3->GetSpinButton();
		//if (pSpinctrl)
		//{
		//	pSpinctrl->GetLabel().ToDouble(&val);
		//	tSrcPos.fPrice = val;
		//	m_curOrderOption.fSourceAssign2 = val;
		//}
		tSrcPos.fPrice = atof(m_pCustomSpinCtrl3->GetTextCtrl()->GetLabel());
		m_curOrderOption.fSourceAssign2 = tSrcPos.fPrice;

		tSrcPos.nVolume = m_tPositionData2.Position;

		if (m_pCNumericTextCtrl3)
		{
			m_pCNumericTextCtrl3->GetValue().ToLong(&value);
			tSrcPos.nPriceOffset = value;
			m_curOrderOption.lSourceHuaDong2 = value;
		}

		if (m_pComboBox6)
		{
			tSrcPos.nTimeOutAction = m_pComboBox6->GetSelection();
			m_curOrderOption.nSourceHandle2 = m_pComboBox6->GetSelection();
		}

		if (m_pCNumericTextCtrl9)
		{
			m_pCNumericTextCtrl9->GetValue().ToLong(&value);
			tSrcPos.nTimeOutPriceOffset = value;
			m_curOrderOption.lSourceTimeoutHuaDong2 = value;
		}

		tSrcPos.nRemainVolume = m_tPositionData2.Position-m_tPositionData2.TodayPosition;
		tSrcPos.nRemainVolume_CloseToday = m_tPositionData2.TodayPosition;

		//m_poMovePositionServiceThread->LockObject();

		if(tSrcPos.nRemainVolume>0)
			m_poMovePositionServiceThread->MakeReq(wxString(tSrcPos.strInstrumentID), tSrcPos.bDirection, 2, tSrcPos.fPrice, 
					tSrcPos.nRemainVolume, tSrcPos.nHedgeStatus, (tSrcPos.nPriceType? FALSE:TRUE), 
					(tSrcPos.nPriceType==0 && NeedSimulateMarketPrice() ? TRUE:FALSE), tSrcPos.req);

		if(tSrcPos.nRemainVolume_CloseToday>0)
			m_poMovePositionServiceThread->MakeReq(wxString(tSrcPos.strInstrumentID), tSrcPos.bDirection, 1, tSrcPos.fPrice, 
					tSrcPos.nRemainVolume_CloseToday, tSrcPos.nHedgeStatus, (tSrcPos.nPriceType? FALSE:TRUE), 
					(tSrcPos.nPriceType==0 && NeedSimulateMarketPrice() ? TRUE:FALSE), tSrcPos.req_CloseToday);

		tMove2.tSrcPos = tSrcPos;

		//目标仓位

		CMovePositionServiceThread::POSITIONUNIT	tDestPos;
		ZeroMemory(&tDestPos, sizeof(tDestPos));
		strcpy(tDestPos.strInstrumentID, m_tPositionData2.InstrumentID);

		if (m_tPositionData1.PosiDirection == THOST_FTDC_PD_Long)
		{
			tDestPos.bDirection = TRUE;
		}
		else
		{
			tDestPos.bDirection = FALSE;
		}

		tDestPos.fPriceTick = m_fPriceTick2;
        tDestPos.nHedgeStatus=m_tPositionData2.HedgeFlag-THOST_FTDC_HF_Speculation;

		tDestPos.nPriceType = m_pRadioGroupDest2->GetGroupValue();
		m_curOrderOption.nDestPriceType2 = tDestPos.nPriceType;
		if (0 == tDestPos.nPriceType)
		{
			PlatformStru_InstrumentInfo outData;
			memset(&outData,0,sizeof(outData));

			if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(tSrcPos.strInstrumentID, outData);
//			if(strcmp(outData.ExchangeID, "SHFE")==0 && NeedSimulateMarketPrice()) 
			if(NEEDSIMULATEMARKET(outData.ExchangeID) && NeedSimulateMarketPrice()) 
				tDestPos.bSimulate = TRUE;
		}

		tDestPos.bIsClose = FALSE;

		//pSpinctrl = m_pCustomSpinCtrl4->GetSpinButton();
		//if (pSpinctrl)
		//{
		//	pSpinctrl->GetLabel().ToDouble(&val);
		//	tDestPos.fPrice = val;
		//	m_curOrderOption.fDestAssign2 = val;
		//}
		tDestPos.fPrice = atof(m_pCustomSpinCtrl4->GetTextCtrl()->GetLabel());
		m_curOrderOption.fDestAssign2 = tDestPos.fPrice;

		tDestPos.nVolume = m_tPositionData1.Position;

		if (m_pCNumericTextCtrl4)
		{
			m_pCNumericTextCtrl4->GetValue().ToLong(&value);
			tDestPos.nPriceOffset = value;
			m_curOrderOption.lDestHuaDong2 = value;
		}

		if (m_pComboBox7)
		{
			tDestPos.nTimeOutAction = m_pComboBox7->GetSelection();
			m_curOrderOption.nDestHandle2 = m_pComboBox7->GetSelection();
		}

		if (m_pCNumericTextCtrl10)
		{
			m_pCNumericTextCtrl10->GetValue().ToLong(&value);
			tDestPos.nTimeOutPriceOffset = value;
			m_curOrderOption.lDestTimeoutHuaDong2 = value;
		}

		tDestPos.nRemainVolume = tDestPos.nVolume;

		m_poMovePositionServiceThread->MakeReq(wxString(tDestPos.strInstrumentID), tDestPos.bDirection, 0, tDestPos.fPrice, 
				tDestPos.nRemainVolume, tDestPos.nHedgeStatus, (tDestPos.nPriceType? FALSE:TRUE), 
				(tDestPos.nPriceType==0 && NeedSimulateMarketPrice() ? TRUE:FALSE), tDestPos.req);

		tMove2.tDestPos = tDestPos;

	}

	//执行

	BOOL bIsEqual = FALSE;

	m_poMovePositionServiceThread->LockObject();

	if(!bIsRedo) {
		int nNum = m_poMovePositionServiceThread->SwapPositionAdd(tSwap, tMove1, tMove2);
		m_poMovePositionServiceThread->SwapPositionDoNow(nNum);
		m_tMovePosition1 = tMove1;
		m_tMovePosition2 = tMove2;
		m_tSwap = tSwap;
		m_tSwap.nNum = nNum;
	}
	else {
		m_poMovePositionServiceThread->SwapPositionRedo(m_tSwap.nNum, tMove1, tMove2, bIsEqual);
	}

	m_poMovePositionServiceThread->UnlockObject();

	return true;

}

void CSwapPositionDlg::OnRcvTimer(wxCommandEvent& event)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

    std::string strSrc(EvtParamStr);

//	std::string strSrc = (event.GetString()).mb_str();

	// 例如"1,1,3, 7, 3, 2"，表示移仓单编号，原始合约还是目标合约，
	//执行时间，倒计时剩余时间，已重试次数，剩余重试次数

	std::vector<std::string> vecData;
	tokenize_str(strSrc, vecData);

	int nNumber = atoi(vecData.at(0).c_str());

	wxString str;

	if (m_tMovePosition1.nNum == nNumber)
	{

		//原始合约
		if (vecData.at(1) == "1")
		{
			wxWindow * pStatic = FindWindowById(ID_WXSTATICTEXT25, this);
			str.Printf("执行时间%s秒，已重试%s次，剩余重试机会%s次", vecData.at(2).c_str(), vecData.at(4).c_str(), vecData.at(5).c_str());
			if (pStatic)
			{
				pStatic->SetLabel(str);
			}

			pStatic = FindWindowById(ID_WXSTATICTEXT26, this);
			str.Printf("原始合约%s秒后，按最新价浮动%d个价位改单", vecData.at(3).c_str(), m_tMovePosition1.tSrcPos.nTimeOutPriceOffset);
			if (pStatic)
			{
				pStatic->SetLabel(str);
			}

		}
		//目标合约
		else if (vecData.at(1) == "2")
		{
			wxWindow * pStatic = FindWindowById(ID_WXSTATICTEXT28, this);
			str.Printf("执行时间%s秒，已重试%s次，剩余重试机会%s次", vecData.at(2).c_str(), vecData.at(4).c_str(), vecData.at(5).c_str());
			if (pStatic)
			{
				pStatic->SetLabel(str);
			}

			pStatic = FindWindowById(ID_WXSTATICTEXT29, this);
			str.Printf("目标合约%s秒后，按最新价浮动%d个价位改单", vecData.at(3).c_str(), m_tMovePosition1.tSrcPos.nTimeOutPriceOffset);
			if (pStatic)
			{
				pStatic->SetLabel(str);
			}

		}

	}
	if (m_tMovePosition2.nNum == nNumber)
	{

		//原始合约
		if (vecData.at(1) == "1")
		{
			wxWindow * pStatic = FindWindowById(ID_WXSTATICTEXT31, this);
			str.Printf("执行时间%s秒，已重试%s次，剩余重试机会%s次", vecData.at(2).c_str(), vecData.at(4).c_str(), vecData.at(5).c_str());
			if (pStatic)
			{
				pStatic->SetLabel(str);
			}

			pStatic = FindWindowById(ID_WXSTATICTEXT32, this);
			str.Printf("原始合约%s秒后，按最新价浮动%d个价位改单", vecData.at(3).c_str(), m_tMovePosition2.tSrcPos.nTimeOutPriceOffset);
			if (pStatic)
			{
				pStatic->SetLabel(str);
			}

		}
		//目标合约
		else if (vecData.at(1) == "2")
		{
			wxWindow * pStatic = FindWindowById(ID_WXSTATICTEXT34, this);
			str.Printf("执行时间%s秒，已重试%s次，剩余重试机会%s次", vecData.at(2).c_str(), vecData.at(4).c_str(), vecData.at(5).c_str());
			if (pStatic)
			{
				pStatic->SetLabel(str);
			}

			pStatic = FindWindowById(ID_WXSTATICTEXT35, this);
			str.Printf("目标合约%s秒后，按最新价浮动%d个价位改单", vecData.at(3).c_str(), m_tMovePosition2.tSrcPos.nTimeOutPriceOffset);
			if (pStatic)
			{
				pStatic->SetLabel(str);
			}

		}

	}


}

void CSwapPositionDlg::OnRcvTradeLog(wxCommandEvent& event)
{
    std::string EvtParamStr;
    if(1)
    {
        DWORD EvtParamID=(DWORD)event.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,NULL,NULL,0,NULL))
        {
            CFTEventParam::DeleteEventParam(EvtParamID);
        }
        else
        {
            return;
        }
    }

	std::string strSrc(EvtParamStr);
//	std::string strSrc = (event.GetString()).mb_str();

	// 例如"1,1, 1, 1, 25555, 20, 25550, 3, 17"，表示移仓单编号，
	//原始合约还是目标合约，委托单号，成交单号，委托价格，报单手数，成交价格，成交数量，剩余数量

	std::vector<std::string> vecData;
	tokenize_str(strSrc, vecData);

	if (vecData.size() > 9)
	{
		return;
	}

	int nNumber = atoi(vecData.at(0).c_str());
	if (m_tSwap.nMovePos1Num == nNumber)
	{

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
	else if(m_tSwap.nMovePos2Num == nNumber)
	{
		//原始合约
		if ("1" == vecData.at(1))
		{
			wxListCtrl * pListctrl = static_cast<wxListCtrl*>(FindWindowById(ID_WXLISTCTRL3, this));
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
			wxListCtrl * pListctrl = static_cast<wxListCtrl*>(FindWindowById(ID_WXLISTCTRL4, this));
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

}

void CSwapPositionDlg::OnRcvAction(wxCommandEvent& event)
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

		if (m_pComboBox4->GetSelection() == 4 || m_pComboBox5->GetSelection() == 4
			|| m_pComboBox6->GetSelection() == 4 || m_pComboBox7->GetSelection() == 4)
		{
			//wxMessageBox("换仓操作未完成，已撤单。","提示");
			ShowTradeInfoDlg("提示","换仓操作未完成，已撤单。", TRUE);
		}
		else
		{
			//wxMessageBox("换仓操作已停止，未完成。","提示");
			ShowTradeInfoDlg("提示","换仓操作已停止，未完成。", TRUE);
		}

		pButton = FindWindowById(ID_WXBUTTON3, this);
		if(pButton) {
			pButton->Show(false);
		}
		pButton = FindWindowById(ID_WXBUTTON1, this);
		if(pButton) {
			pButton->Show(false);
			//pButton->Enable(true);
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
		break;
	case CMovePositionServiceThread::conActionSwapCompleted:		//10仓位互换完成
		pButton = FindWindowById(ID_WXBUTTON3, this);
		if(pButton) {
			pButton->Show(false);
		}
		pButton = FindWindowById(ID_WXBUTTON1, this);
		if(pButton) {
			pButton->Show(false);
			//pButton->Enable(true);
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

		//wxMessageBox("仓位互换完成！","提示");
		ShowTradeInfoDlg("提示","仓位互换完成！", TRUE);
		break;
	};
}

void CSwapPositionDlg::SetAble(bool bPart1, bool bPart2, bool bAll)
{

	wxWindow * p = NULL;

	int index4, index5, index6, index7;
	if (m_pComboBox4) index4 = m_pComboBox4->GetSelection();
	if (m_pComboBox5) index5 = m_pComboBox5->GetSelection();
	if (m_pComboBox6) index6 = m_pComboBox6->GetSelection();
	if (m_pComboBox7) index7 = m_pComboBox7->GetSelection();

	int Original1 = m_pRadioGroupOriginal1->GetGroupValue();
	int dest1 = m_pRadioGroupDest1->GetGroupValue();
	int Original2 = m_pRadioGroupOriginal2->GetGroupValue();
	int dest2 = m_pRadioGroupDest2->GetGroupValue();

	for (int nID = ID_WXEDIT1; nID < ID_WXCOMBOBOX1; ++nID)
	{
		p = FindWindowById(nID, this);
		if (p)
		{
			if (ID_WXEDIT1 == nID && (0 == Original1 || 3 == Original1) && !bAll)
			{
				m_pCNumericTextCtrl1->Enable(false);
			}
			else if (ID_WXEDIT2 == nID && (0 == dest1 || 3 == dest1) && !bAll)
			{
				m_pCNumericTextCtrl2->Enable(false);
			}
			else if (ID_WXEDIT3 == nID && (0 == Original2 || 3 == Original2) && !bAll)
			{
				m_pCNumericTextCtrl3->Enable(false);
			}
			else if (ID_WXEDIT4 == nID && (0 == dest2 || 3 == dest2) && !bAll)
			{
				m_pCNumericTextCtrl4->Enable(false);
			}
			else if (ID_WXEDIT1 == nID && (1 == Original1 || 2 == Original1) && !bAll)
			{
				m_pCNumericTextCtrl1->Enable(true);
			}
			else if (ID_WXEDIT2 == nID && (1 == dest1 || 2 == dest1) && !bAll)
			{
				m_pCNumericTextCtrl2->Enable(true);
			}
			else if (ID_WXEDIT3 == nID && (1 == Original2 || 2 == Original2) && !bAll)
			{
				m_pCNumericTextCtrl3->Enable(true);
			}
			else if (ID_WXEDIT4 == nID && (1 == dest2 || 2 == dest2) && !bAll)
			{
				m_pCNumericTextCtrl4->Enable(true);
			}
			else if (ID_WXCOMBOBOX4 == nID && (1 == index4 || 2 == index4) && !bAll)
			{
				m_pCNumericTextCtrl7->Enable(true);
				m_pComboBox4->Enable(true);
			}
			else if (ID_WXCOMBOBOX5 == nID && (1 == index5 || 2 == index5) && !bAll)
			{
				m_pCNumericTextCtrl8->Enable(true);
				m_pComboBox5->Enable(true);
			}
			else if (ID_WXCOMBOBOX6 == nID && (1 == index6 || 2 == index6) && !bAll)
			{
				m_pCNumericTextCtrl9->Enable(true);
				m_pComboBox6->Enable(true);
			}
			else if (ID_WXCOMBOBOX7 == nID && (1 == index7 || 2 == index7) && !bAll)
			{
				m_pCNumericTextCtrl10->Enable(true);
				m_pComboBox7->Enable(true);
			}
			else if (ID_WXCOMBOBOX4 == nID && (0 == index4 || 3 == index4) && !bAll)
			{
				m_pCNumericTextCtrl7->Enable(false);
				m_pComboBox4->Enable(true);
			}
			else if (ID_WXCOMBOBOX5 == nID && (0 == index5 || 3 == index5) && !bAll)
			{
				m_pCNumericTextCtrl8->Enable(false);
				m_pComboBox5->Enable(true);
			}
			else if (ID_WXCOMBOBOX6 == nID && (0 == index6 || 3 == index6) && !bAll)
			{
				m_pCNumericTextCtrl9->Enable(false);
				m_pComboBox6->Enable(true);
			}
			else if (ID_WXCOMBOBOX7 == nID && (0 == index7 || 3 == index7) && !bAll)
			{
				m_pCNumericTextCtrl10->Enable(false);
				m_pComboBox7->Enable(true);
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

void CSwapPositionDlg::OnComboboxSelectChange(wxCommandEvent& event)
{
	int nID = event.GetId();


	map<wxString, STATIC_TITLE_DATA>::iterator iter1 = m_mapStaticData.find(m_pComboBox1->GetLabel());
	map<wxString, STATIC_TITLE_DATA>::iterator iter2 = m_mapStaticData.find(m_pComboBox2->GetLabel());
	switch (nID)
	{
	case ID_WXCOMBOBOX1:
		{
			if (iter1 != m_mapStaticData.end())
			{
				UpdateAllCabin(m_pComboBox1->GetLabel(), m_pComboBox2->GetLabel());
			}
		}
		break;
	case ID_WXCOMBOBOX2:
		{
			if (iter2 != m_mapStaticData.end())
			{

				SetStaticTitle(&(iter1->second),&(iter2->second));
				m_tPositionData2 = iter2->second.PositionStruct;

				PlatformStru_DepthMarketData quotData;
				ZeroMemory(&quotData, sizeof(quotData));
				if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_tPositionData2.InstrumentID,quotData) == 0)
				{
					wxString str;
					str << quotData.LastPrice;
					(m_pCustomSpinCtrl3->GetTextCtrl())->SetLabel(str);
					(m_pCustomSpinCtrl4->GetTextCtrl())->SetLabel(str);

					PlatformStru_InstrumentInfo insInfo;
					ZeroMemory(&insInfo, sizeof(insInfo));
					if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(m_tPositionData2.InstrumentID,insInfo);
					m_fPriceTick2 = insInfo.PriceTick;
					m_pCustomSpinCtrl3->SetStep(m_fPriceTick2);
					m_pCustomSpinCtrl4->SetStep(m_fPriceTick2);
				}

			}
		}
		break;
	case ID_WXCOMBOBOX4:
		if (m_pComboBox4 != NULL)
		{
			int index = m_pComboBox4->GetSelection();
			if (1 == index || 2 == index)
			{
				m_pCNumericTextCtrl7->Enable(true);
			}
			else
			{
				m_pCNumericTextCtrl7->Enable(false);
			}
		}
		break;
	case ID_WXCOMBOBOX5:
		if (m_pComboBox5 != NULL)
		{
			int index = m_pComboBox5->GetSelection();
			if (1 == index || 2 == index)
			{
				m_pCNumericTextCtrl8->Enable(true);
			}
			else
			{
				m_pCNumericTextCtrl8->Enable(false);
			}
		}
		break;
	case ID_WXCOMBOBOX6:
		if (m_pComboBox6 != NULL)
		{
			int index = m_pComboBox6->GetSelection();
			if (1 == index || 2 == index)
			{
				m_pCNumericTextCtrl9->Enable(true);
			}
			else
			{
				m_pCNumericTextCtrl9->Enable(false);
			}
		}
		break;
	case ID_WXCOMBOBOX7:
		if (m_pComboBox7 != NULL)
		{
			int index = m_pComboBox7->GetSelection();
			if (1 == index || 2 == index)
			{
				m_pCNumericTextCtrl10->Enable(true);
			}
			else
			{
				m_pCNumericTextCtrl10->Enable(false);
			}
		}
		break;
	}
}

void CSwapPositionDlg::SetStaticTitle(STATIC_TITLE_DATA * pData1, STATIC_TITLE_DATA * pData2)
{
	wxString str;

	//char buff1[5], buff2[5];
	//ZeroMemory(buff1,sizeof(buff1));
	//ZeroMemory(buff2,sizeof(buff2));
	//if (pData1)
	//{
	//	if (LOADSTRING(OLS_BUY)==pData1->Direct)
	//	{
	//		strcpy(buff1, LOADSTRING(OLS_SELL));
	//	}
	//	else
	//	{
	//		strcpy(buff1, LOADSTRING(OLS_BUY));
	//	}
	//}
	//if (pData2)
	//{
	//	if (LOADSTRING(OLS_BUY)==pData1->Direct)
	//	{
	//		strcpy(buff2, LOADSTRING(OLS_SELL));
	//	}
	//	else
	//	{
	//		strcpy(buff2, LOADSTRING(OLS_BUY));
	//	}
	//}

	str.Printf("仓位1: __平仓__,__手，按以下价格");
	(FindWindowById(ID_WXSTATICTEXT4, this))->SetLabel(str);

	str.Printf("仓位1: __开仓__,__手，按以下价格");
	(FindWindowById(ID_WXSTATICTEXT6, this))->SetLabel(str);

	str.Printf("仓位2: __平仓__,__手，按以下价格");
	(FindWindowById(ID_WXSTATICTEXT8, this))->SetLabel(str);

	str.Printf("仓位2: __开仓__,__手，按以下价格");
	(FindWindowById(ID_WXSTATICTEXT10, this))->SetLabel(str);

	str.Printf("合约1: __平仓情况");
	(FindWindowById(ID_WXSTATICTEXT27, this))->SetLabel(str);

	str.Printf("合约1: __开仓情况");
	(FindWindowById(ID_WXSTATICTEXT30, this))->SetLabel(str);

	str.Printf("合约2: __平仓情况");
	(FindWindowById(ID_WXSTATICTEXT33, this))->SetLabel(str);

	str.Printf("合约2: __开仓情况");
	(FindWindowById(ID_WXSTATICTEXT37, this))->SetLabel(str);

	if (pData1)
	{
		str.Printf("仓位1: %s平仓%s%d手，按以下价格", pData1->ID.c_str(), pData1->Direct==wxString("买入")?"卖出":"买入", pData1->nVol);
		(FindWindowById(ID_WXSTATICTEXT4, this))->SetLabel(str);

		str.Printf("仓位2: %s开仓%s%d手，按以下价格", pData2?pData2->ID.c_str():"__", pData1->Direct.c_str(), pData1->nVol);
		(FindWindowById(ID_WXSTATICTEXT10, this))->SetLabel(str);


		str.Printf("合约1: %s平仓情况", pData1->ID.c_str());
		(FindWindowById(ID_WXSTATICTEXT27, this))->SetLabel(str);

		str.Printf("合约1: %s开仓情况", pData1->ID.c_str());
		(FindWindowById(ID_WXSTATICTEXT30, this))->SetLabel(str);
	}
	if (pData2)
	{
		str.Printf("仓位1: %s开仓%s%d手，按以下价格", pData1?pData1->ID.c_str():"__", pData2->Direct.c_str(), pData2->nVol);
		(FindWindowById(ID_WXSTATICTEXT6, this))->SetLabel(str);

		str.Printf("仓位2: %s平仓%s%d手，按以下价格", pData2->ID.c_str(), pData2->Direct==wxString("买入")?"卖出":"买入", pData2->nVol);
		(FindWindowById(ID_WXSTATICTEXT8, this))->SetLabel(str);

		str.Printf("合约2: %s平仓情况", pData2->ID.c_str());
		(FindWindowById(ID_WXSTATICTEXT33, this))->SetLabel(str);

		str.Printf("合约2: %s开仓情况", pData2->ID.c_str());
		(FindWindowById(ID_WXSTATICTEXT37, this))->SetLabel(str);

	}

}


void CSwapPositionDlg::SetDefault()
{
	SetAble(true,true,false);

	wxWindow * pWin = NULL;
	for (int nID = ID_WXLISTCTRL1; nID <= ID_WXLISTCTRL4; ++nID)
	{
		pWin = (FindWindowById(nID, this));
		if (pWin)
		{
			(static_cast<wxListCtrl*>(pWin))->DeleteAllItems();
		}
	}

	wxStaticText * pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT25, this));
	if (pStatic) pStatic->SetLabel(wxT("执行时间_秒，已重试_次，剩余_次重试机会"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT26, this));
	if (pStatic) pStatic->SetLabel(wxT("原始合约平仓_秒后，按最新价浮动_个价位改单"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT28, this));
	if (pStatic) pStatic->SetLabel(wxT("执行时间_秒，已重试_次，剩余_次重试机会"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT29, this));
	if (pStatic) pStatic->SetLabel(wxT("目标合约平仓_秒后，按最新价浮动_个价位改单"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT31, this));
	if (pStatic) pStatic->SetLabel(wxT("执行时间_秒，已重试_次，剩余_次重试机会"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT32, this));
	if (pStatic) pStatic->SetLabel(wxT("原始合约平仓_秒后，按最新价浮动_个价位改单"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT34, this));
	if (pStatic) pStatic->SetLabel(wxT("执行时间_秒，已重试_次，剩余_次重试机会"));

	pStatic = static_cast<wxStaticText*>(FindWindowById(ID_WXSTATICTEXT35, this));
	if (pStatic) pStatic->SetLabel(wxT("目标合约平仓_秒后，按最新价浮动_个价位改单"));

}



void CSwapPositionDlg::OnChildFocus(wxChildFocusEvent& event)
{
	if (m_pModifyOrderDlg->IsShown())
	{
		PrintChange();
	}

}


void CSwapPositionDlg::PrintChange()
{

	long value;
	m_pCNumericTextCtrl5->GetValue().ToLong(&value);
	if (m_curOrderOption.nTimeout != value && m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->InsertModify("超时时间改变\n");
		m_curOrderOption.nTimeout = value;
	}

	m_pCNumericTextCtrl6->GetValue().ToLong(&value);
	if (m_curOrderOption.nRetryTimes != value && m_pModifyOrderDlg)
	{
		m_pModifyOrderDlg->InsertModify("重试次数改变\n");
		m_curOrderOption.nRetryTimes = value;
	}

	{
		int nPriceType = m_pRadioGroupOriginal1->GetGroupValue();
		if (nPriceType != m_curOrderOption.nSourcePriceType1 && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("仓位1原始合约价格类型改变\n");
			m_curOrderOption.nSourcePriceType1 = nPriceType;
		}


		double val;
		wxSpinButton * pSpinctrl = m_pCustomSpinCtrl1->GetSpinButton();
		if (pSpinctrl)
		{
			val = pSpinctrl->GetValue();
			if (m_curOrderOption.fSourceAssign1 != val  && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位1原始合约指定价格改变\n");
				m_curOrderOption.fSourceAssign1 = val;
			}
		}


		if (m_pCNumericTextCtrl1)
		{
			m_pCNumericTextCtrl1->GetValue().ToLong(&value);
			if (m_curOrderOption.lSourceHuaDong1 != value  && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位1原始合约滑动价差改变\n");
				m_curOrderOption.lSourceHuaDong1 = value;
			}
		}

		if (m_pComboBox4 && m_curOrderOption.nSourceHandle1 != m_pComboBox4->GetSelection() && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("仓位1原始合约未成交处理方式改变\n");
			m_curOrderOption.nSourceHandle1 = m_pComboBox4->GetSelection();
		}

		if (m_pCNumericTextCtrl7)
		{
			m_pCNumericTextCtrl7->GetValue().ToLong(&value);
			if (m_curOrderOption.lSourceTimeoutHuaDong1 != value && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位1原始合约超时设置滑动价差改变\n");
				m_curOrderOption.lSourceTimeoutHuaDong1 = value;
			}
		}

		nPriceType = m_pRadioGroupDest1->GetGroupValue();
		if (nPriceType != m_curOrderOption.nDestPriceType1 && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("仓位1目标合约价格类型改变\n");
			m_curOrderOption.nDestPriceType1 = nPriceType;
		}

		pSpinctrl = m_pCustomSpinCtrl2->GetSpinButton();
		if (pSpinctrl)
		{
			val = pSpinctrl->GetValue();
			if (m_curOrderOption.fDestAssign1 != val  && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位1目标合约指定价格改变\n");
				m_curOrderOption.fDestAssign1 = val;
			}
		}

		if (m_pCNumericTextCtrl2)
		{
			m_pCNumericTextCtrl2->GetValue().ToLong(&value);
			if (m_curOrderOption.lDestHuaDong1 != value  && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位1目标合约滑动价差改变\n");
				m_curOrderOption.lDestHuaDong1 = value;
			}
		}

		if (m_pComboBox5 && m_curOrderOption.nDestHandle1 != m_pComboBox5->GetSelection() && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("仓位1目标合约未成交处理方式改变\n");
			m_curOrderOption.nDestHandle1 = m_pComboBox5->GetSelection();
		}

		if (m_pCNumericTextCtrl8)
		{
			m_pCNumericTextCtrl8->GetValue().ToLong(&value);
			if (m_curOrderOption.lDestTimeoutHuaDong1 != value && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位1目标合约超时设置滑动价差改变\n");
				m_curOrderOption.lDestTimeoutHuaDong1 = value;
			}
		}
	}

//////////////////////////////////////////////////////////////////////////

	{
		int nPriceType = m_pRadioGroupOriginal2->GetGroupValue();
		if (nPriceType != m_curOrderOption.nSourcePriceType2 && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("仓位2原始合约价格类型改变\n");
			m_curOrderOption.nSourcePriceType2 = nPriceType;
		}


		double val;
		wxSpinButton * pSpinctrl = m_pCustomSpinCtrl3->GetSpinButton();
		if (pSpinctrl)
		{
			val = pSpinctrl->GetValue();
			if (m_curOrderOption.fSourceAssign2 != val  && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位2原始合约指定价格改变\n");
				m_curOrderOption.fSourceAssign2 = val;
			}
		}


		if (m_pCNumericTextCtrl3)
		{
			m_pCNumericTextCtrl3->GetValue().ToLong(&value);
			if (m_curOrderOption.lSourceHuaDong2 != value  && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位2原始合约滑动价差改变\n");
				m_curOrderOption.lSourceHuaDong2 = value;
			}
		}

		if (m_pComboBox6 && m_curOrderOption.nSourceHandle2 != m_pComboBox6->GetSelection() && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("仓位2原始合约未成交处理方式改变\n");
			m_curOrderOption.nSourceHandle2 = m_pComboBox6->GetSelection();
		}

		if (m_pCNumericTextCtrl9)
		{
			m_pCNumericTextCtrl9->GetValue().ToLong(&value);
			if (m_curOrderOption.lSourceTimeoutHuaDong2 != value && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位2原始合约超时设置滑动价差改变\n");
				m_curOrderOption.lSourceTimeoutHuaDong2 = value;
			}
		}

		nPriceType = m_pRadioGroupDest2->GetGroupValue();
		if (nPriceType != m_curOrderOption.nDestPriceType2 && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("仓位2目标合约价格类型改变\n");
			m_curOrderOption.nDestPriceType2 = nPriceType;
		}

		pSpinctrl = m_pCustomSpinCtrl4->GetSpinButton();
		if (pSpinctrl)
		{
			val = pSpinctrl->GetValue();
			if (m_curOrderOption.fDestAssign2 != val  && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位2目标合约指定价格改变\n");
				m_curOrderOption.fDestAssign2 = val;
			}
		}


		if (m_pCNumericTextCtrl4)
		{
			m_pCNumericTextCtrl4->GetValue().ToLong(&value);
			if (m_curOrderOption.lDestHuaDong2 != value  && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位2目标合约滑动价差改变\n");
				m_curOrderOption.lDestHuaDong2 = value;
			}
		}

		if (m_pComboBox7 && m_curOrderOption.nDestHandle2 != m_pComboBox7->GetSelection() && m_pModifyOrderDlg)
		{
			m_pModifyOrderDlg->InsertModify("仓位2目标合约未成交处理方式改变\n");
			m_curOrderOption.nDestHandle2 = m_pComboBox7->GetSelection();
		}

		if (m_pCNumericTextCtrl10)
		{
			m_pCNumericTextCtrl10->GetValue().ToLong(&value);
			if (m_curOrderOption.lDestTimeoutHuaDong2 != value && m_pModifyOrderDlg)
			{
				m_pModifyOrderDlg->InsertModify("仓位2目标合约超时设置滑动价差改变\n");
				m_curOrderOption.lDestTimeoutHuaDong2 = value;
			}
		}
	}


}

void CSwapPositionDlg::ReSendRequest()
{
	wxWindow* pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) 
		pButton->Show(false);
	pButton = FindWindowById(ID_WXBUTTONEDIT, this);
	if(pButton) 
		pButton->Show(false);

	SendRequest(true);
	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tSwap.nMovePos1Num, FALSE);
	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tSwap.nMovePos2Num, FALSE);
	SetAble(false);
}

void CSwapPositionDlg::RollBack()
{
	wxWindow* pButton = FindWindowById(ID_WXBUTTON1, this);
	if(pButton) 
		pButton->Show(false);
	pButton = FindWindowById(ID_WXBUTTONEDIT, this);
	if(pButton) 
		pButton->Show(false);

	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tSwap.nMovePos1Num, FALSE);
	m_poMovePositionServiceThread->MovePositionPauseTimer(m_tSwap.nMovePos2Num, FALSE);

	wxString strValue = wxString::Format(_T("%d"), m_curOrderOption.nTimeout);
	m_pCNumericTextCtrl5->SetLabel(strValue);

	strValue = wxString::Format(_T("%d"), m_curOrderOption.nRetryTimes);
	m_pCNumericTextCtrl6->SetLabel(strValue);

	m_pRadioGroupOriginal1->SetGroupValue(m_curOrderOption.nSourcePriceType1);

	wxSpinButton * pSpinctrl = m_pCustomSpinCtrl1->GetSpinButton();
	strValue = wxString::Format(_T("%d"), m_curOrderOption.fSourceAssign1);
	if (pSpinctrl)
	{
		pSpinctrl->SetLabel(strValue);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lSourceHuaDong1);
	if (m_pCNumericTextCtrl1)
	{
		m_pCNumericTextCtrl1->SetLabel(strValue);
	}

	if (m_pComboBox4)
	{
		m_pComboBox4->SetSelection(m_curOrderOption.nSourceHandle1);
	}
	if (0 == m_curOrderOption.nSourceHandle1 || 3 == m_curOrderOption.nSourceHandle1)
	{
		m_pCNumericTextCtrl7->Enable(false);
	}
	else if (1 == m_curOrderOption.nSourceHandle1 || 2 == m_curOrderOption.nSourceHandle1)
	{
		m_pCNumericTextCtrl7->Enable(true);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lSourceTimeoutHuaDong1);
	if (m_pCNumericTextCtrl7)
	{
		m_pCNumericTextCtrl7->SetLabel(strValue);
	}

	m_pRadioGroupDest1->SetGroupValue(m_curOrderOption.nDestPriceType1);

	pSpinctrl = m_pCustomSpinCtrl2->GetSpinButton();
	strValue = wxString::Format(_T("%d"), m_curOrderOption.fDestAssign1);
	if (pSpinctrl)
	{
		pSpinctrl->SetLabel(strValue);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lDestHuaDong1);
	if (m_pCNumericTextCtrl2)
	{
		m_pCNumericTextCtrl2->SetLabel(strValue);
	}

	if (m_pComboBox5)
	{
		m_pComboBox5->SetSelection(m_curOrderOption.nDestHandle1);
	}
	if (0 == m_curOrderOption.nDestHandle1 || 3 == m_curOrderOption.nDestHandle1)
	{
		m_pCNumericTextCtrl8->Enable(false);
	}
	else if (1 == m_curOrderOption.nDestHandle1 || 2 == m_curOrderOption.nDestHandle1)
	{
		m_pCNumericTextCtrl8->Enable(true);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lDestTimeoutHuaDong1);
	if (m_pCNumericTextCtrl8)
	{
		m_pCNumericTextCtrl8->SetLabel(strValue);
	}


//////////////////////////////////////////////////////////////////////////

	m_pRadioGroupOriginal2->SetGroupValue(m_curOrderOption.nSourcePriceType1);

	pSpinctrl = m_pCustomSpinCtrl3->GetSpinButton();
	strValue = wxString::Format(_T("%d"), m_curOrderOption.fSourceAssign1);
	if (pSpinctrl)
	{
		pSpinctrl->SetLabel(strValue);
	}


	strValue = wxString::Format(_T("%d"), m_curOrderOption.lSourceHuaDong1);
	if (m_pCNumericTextCtrl3)
	{
		m_pCNumericTextCtrl3->SetLabel(strValue);
	}

	if (m_pComboBox6)
	{
		m_pComboBox6->SetSelection(m_curOrderOption.nSourceHandle1);
	}
	if (0 == m_curOrderOption.nSourceHandle1 || 3 == m_curOrderOption.nSourceHandle1)
	{
		m_pCNumericTextCtrl9->Enable(false);
	}
	else if (1 == m_curOrderOption.nSourceHandle1 || 2 == m_curOrderOption.nSourceHandle1)
	{
		m_pCNumericTextCtrl9->Enable(true);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lSourceTimeoutHuaDong1);
	if (m_pCNumericTextCtrl9)
	{
		m_pCNumericTextCtrl9->SetLabel(strValue);
	}

	m_pRadioGroupDest2->SetGroupValue(m_curOrderOption.nDestPriceType1);

	pSpinctrl = m_pCustomSpinCtrl4->GetSpinButton();
	strValue = wxString::Format(_T("%d"), m_curOrderOption.fDestAssign1);
	if (pSpinctrl)
	{
		pSpinctrl->SetLabel(strValue);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lDestHuaDong1);
	if (m_pCNumericTextCtrl4)
	{
		m_pCNumericTextCtrl4->SetLabel(strValue);
	}

	if (m_pComboBox7)
	{
		m_pComboBox7->SetSelection(m_curOrderOption.nDestHandle1);
	}
	if (0 == m_curOrderOption.nDestHandle1 || 3 == m_curOrderOption.nDestHandle1)
	{
		m_pCNumericTextCtrl10->Enable(false);
	}
	else if (1 == m_curOrderOption.nDestHandle1 || 2 == m_curOrderOption.nDestHandle1)
	{
		m_pCNumericTextCtrl10->Enable(true);
	}

	strValue = wxString::Format(_T("%d"), m_curOrderOption.lDestTimeoutHuaDong1);
	if (m_pCNumericTextCtrl10)
	{
		m_pCNumericTextCtrl10->SetLabel(strValue);
	}

	SetAble(false);
}


void CSwapPositionDlg::UpdateAllRecord()
{
	wxListCtrl* pwxListCtrls[5];

	pwxListCtrls[0] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL5, this);
	pwxListCtrls[1] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL6, this);
	pwxListCtrls[2] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL7, this);
	pwxListCtrls[3] = (wxListCtrl*)FindWindowById(ID_WXLISTCTRL8, this);
	pwxListCtrls[4] = NULL;

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

int CSwapPositionDlg::UpdateOrderList(const PlatformStru_OrderInfo& rawData, wxListCtrl* pwxListCtrl, std::map<int, long>& fieldID2ColumnID)
{

	int iRowNo=-1, iVolumeTotal=0, iColStatus=0,iColVolume=0;
    wxListItem tmp;
    wxString newValue;

    return LoadOneOrderList( pwxListCtrl->GetItemCount(), rawData, pwxListCtrl, fieldID2ColumnID);
    //return 0;
}

int CSwapPositionDlg::LoadOneOrderList( int iRowNo, const PlatformStru_OrderInfo &rawData, wxListCtrl* pwxListCtrl, std::map<int, long>& fieldID2ColumnID)
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

void CSwapPositionDlg::OnRcvOrderLog(wxCommandEvent& evt)
{
    std::string EvtParamStr;
    char* EvtPtr=NULL;
    int EvtPtrLen;
    int UserDataInt;
    bool brlt=false;
    if(1)
    {
        DWORD EvtParamID=(DWORD)evt.GetInt();
        if(CFTEventParam::GetEventParam(EvtParamID,NULL,NULL,NULL,&EvtPtrLen,0,NULL)&&
            EvtPtrLen==(int)sizeof(DataRtnOrder))
        {
            EvtPtr=(char*)malloc(EvtPtrLen);
            if(EvtPtr!=NULL&&
                CFTEventParam::GetEventParam(EvtParamID,NULL,&EvtParamStr,EvtPtr,&EvtPtrLen,EvtPtrLen,&UserDataInt))
            {
                CFTEventParam::DeleteEventParam(EvtParamID);
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

	int nNum = atoi(EvtParamStr.c_str());
	
	if(!(nNum == m_tSwap.nMovePos1Num || nNum == m_tSwap.nMovePos2Num)) {
//		delete RawData;
//		RawData=NULL;
//		evt.SetClientData(NULL);
        if(EvtPtr!=NULL)
            free(EvtPtr);
		return;
	}
    brlt=false;
	if(nNum == m_tSwap.nMovePos1Num) {
		if(UserDataInt==0) {
			std::map<int, DataRtnOrder*>& mapRowData = m_RowDatas[0];
			mapRowData[RawData->OrderField.RequestID] = RawData;
            brlt=true;
		}
		else if(UserDataInt==1) {
			std::map<int, DataRtnOrder*>& mapRowData = m_RowDatas[1];
			mapRowData[RawData->OrderField.RequestID] = RawData;
            brlt=true;
		}
	}
	else if(nNum == m_tSwap.nMovePos2Num) {
		if(UserDataInt==0) {
			std::map<int, DataRtnOrder*>& mapRowData = m_RowDatas[2];
			mapRowData[RawData->OrderField.RequestID] = RawData;
            brlt=true;
		}
		else if(UserDataInt==1) {
			std::map<int, DataRtnOrder*>& mapRowData = m_RowDatas[3];
			mapRowData[RawData->OrderField.RequestID] = RawData;
            brlt=true;
		}
	}
	if(brlt) 
    {
        UpdateAllRecord();
        //注意，不要释放EvtPtr,它已经被m_RowDatas用掉了
    }
    else
    {
        if(EvtPtr!=NULL)
            free(EvtPtr);
    }
//	delete RawData;
//	RawData=NULL;
//	evt.SetClientData(NULL);
}

void CSwapPositionDlg::OnSpinUpPress(wxSpinEvent& event)
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

	else if (ID_WXSPINCTRL3 == nid)
	{
		m_pCustomSpinCtrl3->SpinUpPress(event);
	}

	else if (ID_WXSPINCTRL4 == nid)
	{
		m_pCustomSpinCtrl4->SpinUpPress(event);
	}
}

void CSwapPositionDlg::OnSpinDownPress(wxSpinEvent& event)
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

	else if (ID_WXSPINCTRL3 == nid)
	{
		m_pCustomSpinCtrl3->SpinDownPress(event);
	}

	else if (ID_WXSPINCTRL4 == nid)
	{
		m_pCustomSpinCtrl4->SpinDownPress(event);
	}
}

void CSwapPositionDlg::OnCheckClick(wxCommandEvent& event)
{

	if (m_pComboBox1 && m_pComboBox2)
	{
		UpdateAllCabin(m_pComboBox1->GetLabel(), m_pComboBox2->GetLabel());
	}

}

void CSwapPositionDlg::InitAllCabin()
{

	if (m_pComboBox1 && m_pComboBox2)
	{
		m_pComboBox1->Clear();
		m_pComboBox2->Clear();

		wxString strSourceDirection;
		if (m_tPositionData1.PosiDirection == THOST_FTDC_PD_Long)
		{
			strSourceDirection = LOADSTRING(OLS_BUY);
		}
		else
		{
			strSourceDirection = LOADSTRING(OLS_SELL);
		}
	
		wxString strItem;
		wxString strCur1;
		int i=0, j=0;
		int startIndex = 0;
		m_mapStaticData.clear();
		wxCheckBox * pCheckBox = static_cast<wxCheckBox *>(FindWindowById(ID_WXCHECKBOX1, this));
		bool bIsCheck = false;
		if(pCheckBox)
			bIsCheck = pCheckBox->IsChecked();
        int vi,vcount=m_vecPositionStruct.size();
        for(vi=0;vi<vcount;vi++)
		{
			PlatformStru_Position & item = m_vecPositionStruct[vi];

			STATIC_TITLE_DATA tmpData;

			tmpData.ID = item.InstrumentID;
			if (item.PosiDirection == THOST_FTDC_PD_Long)
			{
				tmpData.Direct = LOADSTRING(OLS_BUY);
			}
			else
			{
				tmpData.Direct = LOADSTRING(OLS_SELL);
			}
			tmpData.nVol = item.Position;
			tmpData.PositionStruct = item;

			strItem.Printf("%s,%s",item.InstrumentID,tmpData.Direct.c_str());

			m_mapStaticData.insert( make_pair(strItem, tmpData) );

			m_pComboBox1->Insert(strItem,i);

			if (0 == j)
			{
//				m_tPositionData1 = item;
				m_tPositionData2 = item;
			}

			if (strcmp(item.InstrumentID, m_tPositionData1.InstrumentID) == 0 && tmpData.Direct==strSourceDirection.c_str())
			{
				startIndex = i;
				strCur1 = strItem;
			}
			if (!bIsCheck)
			{
				if(!(strcmp(item.InstrumentID, m_tPositionData1.InstrumentID) == 0 && tmpData.Direct==strSourceDirection.c_str()))
				{
					m_pComboBox2->Insert(strItem,j);
					++j;
				}
			}
 			else
 			{
				if (tmpData.Direct!=strSourceDirection.c_str() && tmpData.nVol == m_tPositionData1.Position)
				{
					m_pComboBox2->Insert(strItem,j);
					++j;
				}
 			}


			++i;

		}

		m_pComboBox1->SetSelection(startIndex);
		m_pComboBox2->SetSelection(0);
/*
		bool isNull = true;
		std::map<wxString, STATIC_TITLE_DATA>::iterator iter1,iter2;
		std::map<wxString, STATIC_TITLE_DATA>::iterator iterStatic = m_mapStaticData.begin();
		for (; iterStatic != m_mapStaticData.end(); ++iterStatic)
		{
			if (strCur1 != iterStatic->first)
			{
				iter2 = iterStatic;
				isNull = false;
				break;
			}
		}

		iter1 = m_mapStaticData.find(strCur1);
		if (iter1 != m_mapStaticData.end() && !isNull)
		{
			SetStaticTitle(&(iter1->second),&(iter2->second));
			//m_tPositionData1 = iter1->second.PositionStruct;
			m_tPositionData2 = iter2->second.PositionStruct;

		}
		else if (iter1 != m_mapStaticData.end() && isNull)
		{
			SetStaticTitle(&(iter1->second), NULL);
			//m_tPositionData1 = iter1->second.PositionStruct;

		}

*/
		wxString str1 = m_pComboBox1->GetString(startIndex);
		wxString str2 = m_pComboBox2->GetString(0);

		bool isNull = true;
		std::map<wxString, STATIC_TITLE_DATA>::iterator iter1,iter2;
		iter1 = m_mapStaticData.find(str1.c_str());
		iter2 = m_mapStaticData.find(str2.c_str());

		if (iter1 != m_mapStaticData.end() && iter2 != m_mapStaticData.end())
		{
			SetStaticTitle(&(iter1->second),&(iter2->second));
			//m_tPositionData1 = iter1->second.PositionStruct;
			m_tPositionData2 = iter2->second.PositionStruct;

		}
		else if (iter1 != m_mapStaticData.end())
		{
			SetStaticTitle(&(iter1->second), NULL);
			//m_tPositionData1 = iter1->second.PositionStruct;
		}
		else {
			SetStaticTitle(NULL, &(iter2->second));
			m_tPositionData2 = iter1->second.PositionStruct;
		}


		PlatformStru_DepthMarketData quotData;
		ZeroMemory(&quotData, sizeof(quotData));
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_tPositionData1.InstrumentID,quotData) == 0)
		{
			wxString str;
			str << quotData.LastPrice;
			(m_pCustomSpinCtrl1->GetTextCtrl())->SetLabel(str);
			(m_pCustomSpinCtrl2->GetTextCtrl())->SetLabel(str);

			PlatformStru_InstrumentInfo insInfo;
			ZeroMemory(&insInfo, sizeof(insInfo));
			if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(m_tPositionData1.InstrumentID,insInfo);
			m_fPriceTick1 = insInfo.PriceTick;
			m_pCustomSpinCtrl1->SetStep(m_fPriceTick1);
			m_pCustomSpinCtrl2->SetStep(m_fPriceTick1);
		}
		ZeroMemory(&quotData, sizeof(quotData));
		if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_tPositionData2.InstrumentID,quotData) == 0)
		{
			wxString str;
			str << quotData.LastPrice;
			(m_pCustomSpinCtrl3->GetTextCtrl())->SetLabel(str);
			(m_pCustomSpinCtrl4->GetTextCtrl())->SetLabel(str);

			PlatformStru_InstrumentInfo insInfo;
			ZeroMemory(&insInfo, sizeof(insInfo));
			if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(m_tPositionData2.InstrumentID,insInfo);
			m_fPriceTick2 = insInfo.PriceTick;
			m_pCustomSpinCtrl3->SetStep(m_fPriceTick2);
			m_pCustomSpinCtrl4->SetStep(m_fPriceTick2);
		}

	}


}

void CSwapPositionDlg::UpdateAllCabin(wxString & cabinlable1, wxString & cabinlable2)
{

	wxCheckBox * pCheckBox2 = static_cast<wxCheckBox *>(FindWindowById(ID_WXCHECKBOX1, this));
	bool bIsCheck = false;
	if(pCheckBox2!=NULL)
		bIsCheck = pCheckBox2->IsChecked();

	if (m_pComboBox1==NULL || m_pComboBox2==NULL)
		return;
	
	m_pComboBox2->Clear();
	int i = 0;
	int eq = 0;
	std::map<wxString, STATIC_TITLE_DATA>::iterator iterStatic = m_mapStaticData.begin();
	std::map<wxString, STATIC_TITLE_DATA>::iterator iter1 = m_mapStaticData.find(cabinlable1);
	std::map<wxString, STATIC_TITLE_DATA>::iterator iter2;
	if (iter1 != m_mapStaticData.end())
	{
		for (; iterStatic != m_mapStaticData.end(); ++iterStatic)
		{

			if (iterStatic->first != iter1->first)
			{

				if (!bIsCheck)
				{
					m_pComboBox2->Insert(iterStatic->first,i);
					++i;
				}	

				else if (bIsCheck && 
					(iter1->second).Direct != (iterStatic->second).Direct && 
					iter1->second.nVol == iterStatic->second.nVol)
				{
					m_pComboBox2->Insert(iterStatic->first,i);

					if (cabinlable2 == iterStatic->first)
					{
						eq = i;
					}

					++i;
				}

				
			}

		}
	}


	m_tPositionData1 = iter1->second.PositionStruct;

	m_pComboBox2->SetSelection(eq);
	iter2 = m_mapStaticData.find(m_pComboBox2->GetLabel());
	if (iter2 != m_mapStaticData.end())
	{
		SetStaticTitle(&(iter1->second),&(iter2->second));
		//if (cabinlable1 == cabinlable2)
		{
			m_tPositionData2 = iter2->second.PositionStruct;
		}
	}
	else
	{
		SetStaticTitle(&(iter1->second),NULL);
	}

	PlatformStru_DepthMarketData quotData;
	ZeroMemory(&quotData, sizeof(quotData));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_tPositionData1.InstrumentID,quotData) == 0)
	{
		wxString str;
		str << quotData.LastPrice;
		(m_pCustomSpinCtrl1->GetTextCtrl())->SetLabel(str);
		(m_pCustomSpinCtrl2->GetTextCtrl())->SetLabel(str);

		PlatformStru_InstrumentInfo insInfo;
		ZeroMemory(&insInfo, sizeof(insInfo));
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(m_tPositionData1.InstrumentID,insInfo);
		m_fPriceTick1 = insInfo.PriceTick;
		m_pCustomSpinCtrl1->SetStep(m_fPriceTick1);
		m_pCustomSpinCtrl2->SetStep(m_fPriceTick1);
	}
	ZeroMemory(&quotData, sizeof(quotData));
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetQuotInfo(m_tPositionData2.InstrumentID,quotData) == 0)
	{
		wxString str;
		str << quotData.LastPrice;
		(m_pCustomSpinCtrl3->GetTextCtrl())->SetLabel(str);
		(m_pCustomSpinCtrl4->GetTextCtrl())->SetLabel(str);

		PlatformStru_InstrumentInfo insInfo;
		ZeroMemory(&insInfo, sizeof(insInfo));
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(m_tPositionData2.InstrumentID,insInfo);
		m_fPriceTick2 = insInfo.PriceTick;
		m_pCustomSpinCtrl3->SetStep(m_fPriceTick2);
		m_pCustomSpinCtrl4->SetStep(m_fPriceTick2);

	}
}

void CSwapPositionDlg::OnRadioButton(wxCommandEvent& event)
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
	case ID_WXRADIOBUTTON9:
	case ID_WXRADIOBUTTON12:
		{
			if (m_pCNumericTextCtrl3) m_pCNumericTextCtrl3->Disable();
			break;
		}
	case ID_WXRADIOBUTTON13:
	case ID_WXRADIOBUTTON16:
		{
			if (m_pCNumericTextCtrl4) m_pCNumericTextCtrl4->Disable();
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
	case ID_WXRADIOBUTTON10:
	case ID_WXRADIOBUTTON11:
		{
			if (m_pCNumericTextCtrl3) m_pCNumericTextCtrl3->Enable();
			break;
		}
	case ID_WXRADIOBUTTON14:
	case ID_WXRADIOBUTTON15:
		{
			if (m_pCNumericTextCtrl4) m_pCNumericTextCtrl4->Enable();
			break;
		}

	}

}

void CSwapPositionDlg::SaveLastState()
{

	wxCheckBox * pCheckBox = NULL;
	pCheckBox = static_cast<wxCheckBox*>(FindWindowById(ID_WXCHECKBOX1, this));
	if (pCheckBox)
	{
		m_lastState.bCheck = pCheckBox->IsChecked();
	}

	if (m_pCNumericTextCtrl5)
	{
		m_lastState.Timeout = m_pCNumericTextCtrl5->GetValue();
	}
	if (m_pCNumericTextCtrl6)
	{
		m_lastState.RetryTimes = m_pCNumericTextCtrl6->GetValue();
	}
	if (m_pCNumericTextCtrl7)
	{
		m_lastState.SourceTimeoutHuaDong1 = m_pCNumericTextCtrl7->GetValue();
	}
	if (m_pCNumericTextCtrl8)
	{
		m_lastState.DestTimeoutHuaDong1 = m_pCNumericTextCtrl8->GetValue();
	}
	if (m_pCNumericTextCtrl9)
	{
		m_lastState.SourceTimeoutHuaDong2 = m_pCNumericTextCtrl9->GetValue();
	}
	if (m_pCNumericTextCtrl10)
	{
		m_lastState.DestTimeoutHuaDong2 = m_pCNumericTextCtrl10->GetValue();
	}

	m_lastState.nSourcePriceType1 = m_pRadioGroupOriginal1->GetGroupValue();
	if ( (1 == m_lastState.nSourcePriceType1 || 2 == m_lastState.nSourcePriceType1) && m_pCNumericTextCtrl1)
	{
		m_lastState.SourceHuaDong1 = m_pCNumericTextCtrl1->GetValue();
	}

	CNumericTextCtrl * pTextctrl = m_pCustomSpinCtrl1->GetTextCtrl();
	if (pTextctrl)
	{
		m_lastState.SourceAssign1 = pTextctrl->GetLabel();
	}

	m_lastState.nDestPriceType1 = m_pRadioGroupDest1->GetGroupValue();
	if ( (1 == m_lastState.nDestPriceType1 || 2 == m_lastState.nDestPriceType1) && m_pCNumericTextCtrl2)
	{
		m_lastState.DestHuaDong1 = m_pCNumericTextCtrl2->GetValue();
	}

	pTextctrl = m_pCustomSpinCtrl2->GetTextCtrl();
	if (pTextctrl)
	{
		m_lastState.DestAssign1 = pTextctrl->GetLabel();
	}

	m_lastState.nSourcePriceType2 = m_pRadioGroupOriginal2->GetGroupValue();
	if ( (1 == m_lastState.nSourcePriceType2 || 2 == m_lastState.nSourcePriceType2) && m_pCNumericTextCtrl3)
	{
		m_lastState.SourceHuaDong2 = m_pCNumericTextCtrl3->GetValue();
	}

	pTextctrl = m_pCustomSpinCtrl3->GetTextCtrl();
	if (pTextctrl)
	{
		m_lastState.SourceAssign2 = pTextctrl->GetLabel();
	}

	m_lastState.nDestPriceType2 = m_pRadioGroupDest2->GetGroupValue();
	if ( (1 == m_lastState.nDestPriceType2 || 2 == m_lastState.nDestPriceType2) && m_pCNumericTextCtrl4)
	{
		m_lastState.DestHuaDong2 = m_pCNumericTextCtrl4->GetValue();
	}

	pTextctrl = m_pCustomSpinCtrl4->GetTextCtrl();
	if (pTextctrl)
	{
		m_lastState.DestAssign2 = pTextctrl->GetLabel();
	}

	if (m_pComboBox1)
	{
		m_lastState.nCabin1 = m_pComboBox1->GetSelection();
		m_lastState.Cabin1 = m_pComboBox1->GetLabel();
	}
	if (m_pComboBox2)
	{
		m_lastState.nCabin2 = m_pComboBox2->GetSelection();
		m_lastState.Cabin2 = m_pComboBox2->GetLabel();
	}
	if (m_pComboBox3)
	{
		m_lastState.nOperOrder = m_pComboBox3->GetSelection();
	}
	if (m_pComboBox4)
	{
		m_lastState.nSourceHandle1 = m_pComboBox4->GetSelection();
	}
	if (m_pComboBox5)
	{
		m_lastState.nDestHandle1 = m_pComboBox5->GetSelection();
	}
	if (m_pComboBox6)
	{
		m_lastState.nSourceHandle2 = m_pComboBox6->GetSelection();
	}
	if (m_pComboBox7)
	{
		m_lastState.nDestHandle2 = m_pComboBox7->GetSelection();
	}

}

void CSwapPositionDlg::ReadLastState()
{

	wxRadioButton * pRadiobutton = NULL;
	wxTextCtrl * pTextctrl = NULL;
	wxComboBox* pCombo = NULL;

	m_pRadioGroupOriginal1->SetGroupValue(m_lastState.nSourcePriceType1);
	switch (m_lastState.nSourcePriceType1)
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
				m_pCNumericTextCtrl1->SetLabel(m_lastState.SourceHuaDong1);
			}
		}
		break;
	case 2:
		{
			if (m_pCNumericTextCtrl1)
			{
				m_pCNumericTextCtrl1->Enable(true);
				m_pCNumericTextCtrl1->SetLabel(m_lastState.SourceHuaDong1);
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

	m_pRadioGroupDest1->SetGroupValue(m_lastState.nDestPriceType1);
	switch (m_lastState.nDestPriceType1)
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
				m_pCNumericTextCtrl2->SetLabel(m_lastState.DestHuaDong1);
			}
		}
		break;
	case 2:
		{
			if (m_pCNumericTextCtrl2)
			{
				m_pCNumericTextCtrl2->Enable(true);
				m_pCNumericTextCtrl2->SetLabel(m_lastState.DestHuaDong1);
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

	m_pRadioGroupOriginal2->SetGroupValue(m_lastState.nSourcePriceType2);
	switch (m_lastState.nSourcePriceType2)
	{
	case 0:
		{
			if (m_pCNumericTextCtrl3)
			{
				m_pCNumericTextCtrl3->Enable(false);
			}
		}
		break;
	case 1:
		{
			if (m_pCNumericTextCtrl3)	
			{
				m_pCNumericTextCtrl3->Enable(true);
				m_pCNumericTextCtrl3->SetLabel(m_lastState.SourceHuaDong2);
			}
		}
		break;
	case 2:
		{
			if (m_pCNumericTextCtrl3)
			{
				m_pCNumericTextCtrl3->Enable(true);
				m_pCNumericTextCtrl3->SetLabel(m_lastState.SourceHuaDong2);
			}
		}
		break;
	case 3:
		{
			if (m_pCNumericTextCtrl3)
			{
				m_pCNumericTextCtrl3->Enable(false);
			}
		}
		break;
	}

	m_pRadioGroupDest2->SetGroupValue(m_lastState.nDestPriceType2);
	switch (m_lastState.nDestPriceType2)
	{
	case 0:
		{
			if (m_pCNumericTextCtrl4)
			{
				m_pCNumericTextCtrl4->Enable(false);
			}
		}
		break;
	case 1:
		{
			if (m_pCNumericTextCtrl4)
			{
				m_pCNumericTextCtrl4->Enable(true);
				m_pCNumericTextCtrl4->SetLabel(m_lastState.DestHuaDong2);
			}
		}
		break;
	case 2:
		{
			if (m_pCNumericTextCtrl4)
			{
				m_pCNumericTextCtrl4->Enable(true);
				m_pCNumericTextCtrl4->SetLabel(m_lastState.DestHuaDong2);
			}
		}
		break;
	case 3:
		{
			if (m_pCNumericTextCtrl4)
			{
				m_pCNumericTextCtrl4->Enable(false);
			}
		}
		break;
	}

	if (m_pCNumericTextCtrl5)
	{
		m_pCNumericTextCtrl5->SetLabel(m_lastState.Timeout);
	}
	if (m_pCNumericTextCtrl6)
	{
		m_pCNumericTextCtrl6->SetLabel(m_lastState.RetryTimes);
	}
	if (m_pCNumericTextCtrl7)
	{
		m_pCNumericTextCtrl7->SetLabel(m_lastState.SourceTimeoutHuaDong1);
	}
	if (m_pCNumericTextCtrl8)
	{
		m_pCNumericTextCtrl8->SetLabel(m_lastState.DestTimeoutHuaDong1);
	}
	if (m_pCNumericTextCtrl9)
	{
		m_pCNumericTextCtrl9->SetLabel(m_lastState.SourceTimeoutHuaDong2);
	}
	if (m_pCNumericTextCtrl10)
	{
		m_pCNumericTextCtrl10->SetLabel(m_lastState.DestTimeoutHuaDong2);
	}

	pTextctrl = m_pCustomSpinCtrl1->GetTextCtrl();
	if (pTextctrl)
	{
		pTextctrl->SetLabel(m_lastState.SourceAssign1);
	}
	pTextctrl = m_pCustomSpinCtrl2->GetTextCtrl();
	if (pTextctrl)
	{
		pTextctrl->SetLabel(m_lastState.DestAssign1);
	}
	pTextctrl = m_pCustomSpinCtrl3->GetTextCtrl();
	if (pTextctrl)
	{
		pTextctrl->SetLabel(m_lastState.SourceAssign2);
	}
	pTextctrl = m_pCustomSpinCtrl4->GetTextCtrl();
	if (pTextctrl)
	{
		pTextctrl->SetLabel(m_lastState.DestAssign2);
	}

	if (m_pComboBox1 && m_pComboBox2)
	{
		UpdateAllCabin(m_pComboBox1->GetLabel(), m_pComboBox2->GetLabel());
		m_pComboBox1->SetSelection(m_lastState.nCabin1);
		m_pComboBox2->SetSelection(m_lastState.nCabin2);
	}

	if (m_pComboBox3)
	{
		m_pComboBox3->SetSelection(m_lastState.nOperOrder);
	}
	if (m_pComboBox4)
	{
		m_pComboBox4->SetSelection(m_lastState.nSourceHandle1);
	}
	if (0 == m_lastState.nSourceHandle1 || 3 == m_lastState.nSourceHandle1)
	{
		m_pCNumericTextCtrl7->Enable(false);
	}
	else if (1 == m_lastState.nSourceHandle1 || 2 == m_lastState.nSourceHandle1)
	{
		m_pCNumericTextCtrl7->Enable(true);
	}

	if (m_pComboBox5)
	{
		m_pComboBox5->SetSelection(m_lastState.nDestHandle1);
	}
	if (0 == m_lastState.nDestHandle1 || 3 == m_lastState.nDestHandle1)
	{
		m_pCNumericTextCtrl8->Enable(false);
	}
	else if (1 == m_lastState.nDestHandle1 || 2 == m_lastState.nDestHandle1)
	{
		m_pCNumericTextCtrl8->Enable(true);
	}

	if (m_pComboBox6)
	{
		m_pComboBox6->SetSelection(m_lastState.nSourceHandle2);
	}
	if (0 == m_lastState.nSourceHandle2 || 3 == m_lastState.nSourceHandle2)
	{
		m_pCNumericTextCtrl9->Enable(false);
	}
	else if (1 == m_lastState.nSourceHandle2 || 2 == m_lastState.nSourceHandle2)
	{
		m_pCNumericTextCtrl9->Enable(true);
	}

	if (m_pComboBox7)
	{
		m_pComboBox7->SetSelection(m_lastState.nDestHandle2);
	}
	if (0 == m_lastState.nDestHandle2 || 3 == m_lastState.nDestHandle2)
	{
		m_pCNumericTextCtrl10->Enable(false);
	}
	else if (1 == m_lastState.nDestHandle2 || 2 == m_lastState.nDestHandle2)
	{
		m_pCNumericTextCtrl10->Enable(true);
	}


	wxCheckBox * pCheckBox = NULL;
	pCheckBox = static_cast<wxCheckBox*>(FindWindowById(ID_WXCHECKBOX1, this));
	if (pCheckBox)
	{
		pCheckBox->SetValue(m_lastState.bCheck);
	}
}
