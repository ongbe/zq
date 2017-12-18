///-----------------------------------------------------------------
///
/// @file      TechIndexConfigDlg.cpp
/// @author    Administrator
/// Created:   2012-3-16 9:11:45
/// @section   DESCRIPTION
///            TechIndexConfigDlg class implementation
///
///------------------------------------------------------------------
#include "StdAfx.h"
#include "../../../inc/Quotation/TechIndexConfigDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// TechIndexConfigDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
//IMPLEMENT_CLASS(TechIndexConfigDlg, wxDialog)
DEFINE_EVENT_TYPE(wxEVT_CONFIG_CHANGE)

BEGIN_EVENT_TABLE(TechIndexConfigDlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	EVT_TEXT      (ID_WXSPINCTRL6,                 OnSpinUp6)
	EVT_TEXT      (ID_WXSPINCTRL5,                 OnSpinUp5)
	EVT_TEXT      (ID_WXSPINCTRL4,                 OnSpinUp4)
	EVT_TEXT      (ID_WXSPINCTRL3,                 OnSpinUp3)
	EVT_TEXT      (ID_WXSPINCTRL2,                 OnSpinUp2)
	EVT_TEXT      (ID_WXSPINCTRL1,                 OnSpinUp1)
	EVT_BUTTON(ID_WXBUTTON1,   OnRestore)
	EVT_BUTTON(ID_WXBUTTON2,   OnCloseEvent)

	
	EVT_CLOSE(TechIndexConfigDlg::OnClose)
END_EVENT_TABLE()
////Event Table End

TechIndexConfigDlg::TechIndexConfigDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	m_pMsgWnd = NULL;
	CreateGUIControls();
	m_bDelConfig = false;
	m_bWriteConfig = false;
}

TechIndexConfigDlg::~TechIndexConfigDlg()
{
} 

void TechIndexConfigDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxFont wxfont2(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	
	WxStaticText1[5] = new wxStaticText(this, ID_WXSTATICTEXT61, wxT("WxStaticText61"), wxPoint(10, 143), wxDefaultSize, 0, wxT("WxStaticText6"));
	WxStaticText1[4] = new wxStaticText(this, ID_WXSTATICTEXT51, wxT("WxStaticText51"), wxPoint(10, 118), wxDefaultSize, 0, wxT("WxStaticText5"));
	WxStaticText1[3] = new wxStaticText(this, ID_WXSTATICTEXT41, wxT("WxStaticText41"), wxPoint(10, 86), wxDefaultSize, 0, wxT("WxStaticText4"));
	WxStaticText1[2] = new wxStaticText(this, ID_WXSTATICTEXT31, wxT("WxStaticText31"), wxPoint(10, 57), wxDefaultSize, 0, wxT("WxStaticText3"));
	WxStaticText1[1] = new wxStaticText(this, ID_WXSTATICTEXT21, wxT("WxStaticText21"), wxPoint(10, 28), wxDefaultSize, 0, wxT("WxStaticText2"));
	WxStaticText1[0] = new wxStaticText(this, ID_WXSTATICTEXT11, wxT("WxStaticText11"), wxPoint(10, 5), wxDefaultSize, 0, wxT("WxStaticText1"));
	
	WxStaticText1[5]->SetFont(wxfont2);
	WxStaticText1[4]->SetFont(wxfont2);
	WxStaticText1[3]->SetFont(wxfont2);
	WxStaticText1[2]->SetFont(wxfont2);
	WxStaticText1[1]->SetFont(wxfont2);
	WxStaticText1[0]->SetFont(wxfont2);
	
	
	WxSpinCtrl1[5] = new wxSpinCtrl(this, ID_WXSPINCTRL6, wxT("0"), wxPoint(10, 123), wxSize(60, 21), wxSP_ARROW_KEYS, 0, 100, 0);
	WxSpinCtrl1[4] = new wxSpinCtrl(this, ID_WXSPINCTRL5, wxT("0"), wxPoint(10, 99), wxSize(60, 21), wxSP_ARROW_KEYS, 0, 100, 0);
	WxSpinCtrl1[3] = new wxSpinCtrl(this, ID_WXSPINCTRL4, wxT("0"), wxPoint(10, 75), wxSize(60, 21), wxSP_ARROW_KEYS, 0, 100, 0);
	WxSpinCtrl1[2] = new wxSpinCtrl(this, ID_WXSPINCTRL3, wxT("0"), wxPoint(10, 51), wxSize(60, 21), wxSP_ARROW_KEYS, 0, 100, 0);
	WxSpinCtrl1[1] = new wxSpinCtrl(this, ID_WXSPINCTRL2, wxT("0"), wxPoint(10, 27), wxSize(60, 21), wxSP_ARROW_KEYS, 0, 100, 0);
	WxSpinCtrl1[0] = new wxSpinCtrl(this, ID_WXSPINCTRL1, wxT("0"), wxPoint(10, 3), wxSize(60, 21), wxSP_ARROW_KEYS, 0, 100, 0);

	WxStaticText11[5] = new wxStaticText(this, ID_WXSTATICTEXT6, wxT("WxStaticText6"), wxPoint(100, 143), wxDefaultSize, 0, wxT("WxStaticText6"));
	WxStaticText11[4] = new wxStaticText(this, ID_WXSTATICTEXT5, wxT("WxStaticText5"), wxPoint(100, 118), wxDefaultSize, 0, wxT("WxStaticText5"));
	WxStaticText11[3] = new wxStaticText(this, ID_WXSTATICTEXT4, wxT("WxStaticText4"), wxPoint(100, 86), wxDefaultSize, 0, wxT("WxStaticText4"));
	WxStaticText11[2] = new wxStaticText(this, ID_WXSTATICTEXT3, wxT("WxStaticText3"), wxPoint(100, 57), wxDefaultSize, 0, wxT("WxStaticText3"));
	WxStaticText11[1] = new wxStaticText(this, ID_WXSTATICTEXT2, wxT("WxStaticText2"), wxPoint(100, 28), wxDefaultSize, 0, wxT("WxStaticText2"));
	WxStaticText11[0] = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("WxStaticText1"), wxPoint(100, 5), wxDefaultSize, 0, wxT("WxStaticText1"));
	
	WxStaticText11[5]->SetFont(wxfont2);
	WxStaticText11[4]->SetFont(wxfont2);
	WxStaticText11[3]->SetFont(wxfont2);
	WxStaticText11[2]->SetFont(wxfont2);
	WxStaticText11[1]->SetFont(wxfont2);
	WxStaticText11[0]->SetFont(wxfont2);	

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("恢复缺省参数"), wxPoint(88,172), wxSize(105, 25), 0, wxDefaultValidator, wxT("WxButton1"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("关闭"), wxPoint(200, 172), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton1"));
//////////////////////////////////////////////////////////////////////////
/*	vector<int> nConfig;
	nConfig.push_back(50);
	nConfig.push_back(10);
	nConfig.push_back(20);
	nConfig.push_back(30);
	nConfig.push_back(60);
	nConfig.push_back(120);
	
	SetTechIndex(MA, nConfig);//这部分代码留到外边处理
	InitDialog(m_cInitConfig);*/
//////////////////////////////////////////////////////////////////////////////
	
	wxString strTitle;
	strTitle.Printf(wxT("指标参数调整: -- %s"), m_cInitConfig.strTechIndex);
	SetTitle(strTitle);
	SetIcon(wxNullIcon);
	SetSize(8,8,304,240);
	Center();
}
void TechIndexConfigDlg::SetTechIndex(wxString strTechIndex, EnumPhrase enumPhrase, vector<int>& vecValue)
{		
	m_cInitConfig.strTechIndex	= strTechIndex;
	m_enumPhrase = enumPhrase;
	if(strTechIndex == MA)
	{
		for(int i =0; i<6; i++)
		{
			m_cInitConfig.bConfigShow[i]	= true;		
			m_cInitConfig.bRightShow[i]	= true;
			m_cInitConfig.strRight[i]		= "日均线;";

			if((int)vecValue.size() > i)
				m_cInitConfig.nConfig[i]		= vecValue[i];
		}
	}
	else if(strTechIndex == VOL)
	{
		for(int i =0; i<3; i++)
		{
			m_cInitConfig.bConfigShow[i]	= true;		
			m_cInitConfig.bRightShow[i]	= true;
			m_cInitConfig.strRight[i]		= "日均量;";

			if((int)vecValue.size() > i)
				m_cInitConfig.nConfig[i]		= vecValue[i];
		}	
	}
	else if(strTechIndex == KDJ)
	{
		for(int i =0; i<3; i++)
		{
			m_cInitConfig.bLeftShow[i]		= true;
			m_cInitConfig.strLeft[i]		= "天数:";
			m_cInitConfig.bConfigShow[i]	= true;		
			m_cInitConfig.bRightShow[i]		= true;
			m_cInitConfig.strRight[i]		= "天;";

			if((int)vecValue.size() > i)
				m_cInitConfig.nConfig[i]		= vecValue[i];
		}
		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();
		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(100);
		m_cInitConfig.vecConfigMax.push_back(40);
		m_cInitConfig.vecConfigMax.push_back(40);
		
	}
	else if(strTechIndex == MACD)
	{
		for(int i =0; i<3; i++)
		{
			m_cInitConfig.bLeftShow[i]		= true;			
			m_cInitConfig.bConfigShow[i]	= true;		
			m_cInitConfig.bRightShow[i]		= true;
			if((int)vecValue.size() > i)
				m_cInitConfig.nConfig[i]		= vecValue[i];
		}

		m_cInitConfig.strLeft[0]		= "DIFF:收盘价的:";
		m_cInitConfig.strRight[0]		= "天（短期）平滑移动";

		m_cInitConfig.strLeft[1]		= "平均值减去";
		m_cInitConfig.strRight[1]		= "天（长期）平滑移动平均值";

		m_cInitConfig.strLeft[2]		= "DEA:DIFF的";
		m_cInitConfig.strRight[2]		= "天平滑移动平均值";
		
		m_cInitConfig.bLeftShow[3]		= true;	
		m_cInitConfig.strLeft[3]		= "MACD:2倍的（DIFF-DEA）";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();
		
		m_cInitConfig.vecConfigMin.push_back(20);
		m_cInitConfig.vecConfigMax.push_back(100);

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(40);		

		m_cInitConfig.vecConfigMin.push_back(2);		
		m_cInitConfig.vecConfigMax.push_back(60);

	}
	else if(strTechIndex == BOLL)
	{
		m_cInitConfig.bLeftShow[0]		= false;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		
		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];
		

		m_cInitConfig.strRight[0]		= "日标准差;";
		m_cInitConfig.strLeft[1]		= "宽度:";
		m_cInitConfig.strRight[1]		= ";";

		m_cInitConfig.strLeft[2]		= "DEA:DIFF的";
	
		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(0);
		m_cInitConfig.vecConfigMax.push_back(10);	

	}
	else if(strTechIndex == UOS)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "天数:";
		m_cInitConfig.strRight[0]		= "日;";

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];
		m_cInitConfig.strLeft[1]		= "天数:";
		m_cInitConfig.strRight[1]		= "日;";

		m_cInitConfig.bLeftShow[2]		= true;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[2];
		m_cInitConfig.strLeft[2]		= "天数:";
		m_cInitConfig.strRight[2]		= "日;";
		
		m_cInitConfig.bLeftShow[3]		= false;			
		m_cInitConfig.bConfigShow[3]	= true;		
		m_cInitConfig.bRightShow[3]		= true;
		m_cInitConfig.nConfig[3]		= vecValue[3];
		m_cInitConfig.strRight[3]		= "日移动平均;";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(30);

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(60);	

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(60);	

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(120);	

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(30);	

	}
	else if(strTechIndex == BIAS)
	{
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strRight[0]		= "日乖离率;";

		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];		
		m_cInitConfig.strRight[1]		= "日乖离率;";

		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[2];
		m_cInitConfig.strRight[2]		= "日乖离率;";		

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(300);	

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(300);			
	}
	else if(strTechIndex == DMI)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "天数:";	

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;			
		m_cInitConfig.nConfig[1]		= vecValue[1];
		m_cInitConfig.strLeft[1]		= "天数:";		
	}
	else if(strTechIndex == PSY)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.strLeft[0]		= "请设置计算参数:";	

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;			
		m_cInitConfig.nConfig[1]		= vecValue[0];
		m_cInitConfig.strLeft[1]		= "          N:";

		m_cInitConfig.bRightShow[1]		= true;		
		m_cInitConfig.strRight[1]		= "(1 - 100);";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(1);  // config[0]的设置起始没有意义，但是兼容后面设置的逻辑，所以必须把2之前的都给赋值
		m_cInitConfig.vecConfigMax.push_back(100);//
		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);

	}
	else if(strTechIndex == ROC)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "ROC:100*(收盘价/";
		m_cInitConfig.strRight[0]		= "日前收盘价-1）;";

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];
		m_cInitConfig.strLeft[1]		= "ROC的:";
		m_cInitConfig.strRight[1]		= "日移动平均价;";
		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(1);  // config[0]的设置起始没有意义，但是兼容后面设置的逻辑，所以必须把2之前的都给赋值
		m_cInitConfig.vecConfigMax.push_back(100);//
		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(50);
	}
	else if(strTechIndex == BBI)
	{
		m_cInitConfig.bLeftShow[0]		= false;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.strRight[0]		= "日平均价加";
		m_cInitConfig.nConfig[0]		= vecValue[0];
	
		m_cInitConfig.bLeftShow[1]		= false;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];
		m_cInitConfig.strRight[1]		= "日平均价加";

		m_cInitConfig.bLeftShow[2]		= false;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[2];
		m_cInitConfig.strRight[2]		= "日平均价加";


		m_cInitConfig.bLeftShow[3]		= false;			
		m_cInitConfig.bConfigShow[3]	= true;		
		m_cInitConfig.bRightShow[3]		= true;
		m_cInitConfig.nConfig[3]		= vecValue[3];		
		m_cInitConfig.strRight[3]		= "日平均价加除以四";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(1);  // config[0]的设置起始没有意义，但是兼容后面设置的逻辑，所以必须把2之前的都给赋值
		m_cInitConfig.vecConfigMax.push_back(36);//
		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(16);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);
		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);
	}
	else if(strTechIndex == EXPMA)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "MA1:收盘价的";
		m_cInitConfig.strRight[0]		= "日指数平滑移动平均值";

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];
		m_cInitConfig.strLeft[1]		= "MA2:收盘价的";
		m_cInitConfig.strRight[1]		= "日指数平滑移动平均值";

		m_cInitConfig.bLeftShow[2]		= true;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[2];
		m_cInitConfig.strLeft[2]		= "MA3:收盘价的";
		m_cInitConfig.strRight[2]		= "日指数平滑移动平均值";

		m_cInitConfig.bLeftShow[3]		= true;			
		m_cInitConfig.bConfigShow[3]	= true;		
		m_cInitConfig.bRightShow[3]		= true;
		m_cInitConfig.nConfig[3]		= vecValue[3];
		m_cInitConfig.strLeft[3]		= "MA4:收盘价的";
		m_cInitConfig.strRight[3]		= "日指数平滑移动平均值";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(300);

	}
	else if(strTechIndex == DMA)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "DDD:";
		m_cInitConfig.strRight[0]		= "日均价 减去";

		m_cInitConfig.bLeftShow[1]		= false;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];		
		m_cInitConfig.strRight[1]		= "日均价";

		m_cInitConfig.bLeftShow[2]		= true;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[2];
		m_cInitConfig.strLeft[2]		= "AMA:DDD的";
		m_cInitConfig.strRight[2]		= "日均值";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(10);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(300);
	}
	else if(strTechIndex == SAR)
	{
		m_cInitConfig.bLeftShow[0]		= false;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];		
		m_cInitConfig.strRight[0]		= "日抛物转向，";

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];	
		m_cInitConfig.strLeft[1]		= "步长为";
		m_cInitConfig.strRight[1]		= "%";

		m_cInitConfig.bLeftShow[2]		= true;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[2];
		m_cInitConfig.strLeft[2]		= "AMA:DDD的";
		m_cInitConfig.strRight[2]		= "日均值";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(100);
	}
	else if(strTechIndex == TRIX)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "TR：收盘价的(";
		m_cInitConfig.strRight[0]		= ")日三重指数平滑平均数；";

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= false;		
		m_cInitConfig.bRightShow[1]		= false;
		m_cInitConfig.strLeft[1]		= "TRIX：100乘以(今日TR 减去 昨日TR)除以昨日TR;";

		m_cInitConfig.bLeftShow[2]		= true;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[1];
		m_cInitConfig.strLeft[2]		= "TRMA：TRIX的(";
		m_cInitConfig.strRight[2]		= ")日移动平均值。";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(3);
		m_cInitConfig.vecConfigMax.push_back(100);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);		
	}
	else if(strTechIndex == MTM)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "MTM=今日收盘价-";
		m_cInitConfig.strRight[0]		= "日前收盘价;";

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];
		m_cInitConfig.strLeft[1]		= "MTM的";
		m_cInitConfig.strRight[1]		= "日移动平均;";

		
		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);		
	}
	else if(strTechIndex == CRTECH)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= false;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "天数：";		

		m_cInitConfig.bLeftShow[1]		= false;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];		
		m_cInitConfig.strRight[1]		= "日CR;";

		m_cInitConfig.bLeftShow[2]		= false;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[2];		
		m_cInitConfig.strRight[2]		= "日CR;";

		m_cInitConfig.bLeftShow[3]		= false;			
		m_cInitConfig.bConfigShow[3]	= true;		
		m_cInitConfig.bRightShow[3]		= true;
		m_cInitConfig.nConfig[3]		= vecValue[3];		
		m_cInitConfig.strRight[3]		= "日CR;";


		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);	

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);	

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);		
	}
	else if(strTechIndex == VR)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strLeft[0]		= "天数:";		
		m_cInitConfig.strRight[0]		= "天;";

		m_cInitConfig.bLeftShow[1]		= false;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];		
		m_cInitConfig.strRight[1]		= "日移动均线;";
	
		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(5);
		m_cInitConfig.vecConfigMax.push_back(300);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(100);			
	}
	else if(strTechIndex == EMV)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= false;		
		m_cInitConfig.bRightShow[0]		= false;		
		m_cInitConfig.strLeft[0]		= "请设置计算参数：";		
		

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[0];		
		m_cInitConfig.strLeft[1]		= "              P1：";
		m_cInitConfig.strRight[1]		= "（1 — 999）";

		m_cInitConfig.bLeftShow[2]		= true;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[1];		
		m_cInitConfig.strLeft[2]		= "              P2：";
		m_cInitConfig.strRight[2]		= "（1 — 999）";


		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(900);

		m_cInitConfig.vecConfigMin.push_back(1);
		m_cInitConfig.vecConfigMax.push_back(900);			
	}
	else if(strTechIndex == CCI)
	{
		m_cInitConfig.bLeftShow[0]		= true;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;		
		m_cInitConfig.nConfig[0]		= vecValue[0];		
		m_cInitConfig.strLeft[0]		= "天数：";		
		m_cInitConfig.strRight[0]		= "天；";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();
		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(100);
	}
	else if(strTechIndex == RSI)
	{
		m_cInitConfig.bLeftShow[0]		= false;			
		m_cInitConfig.bConfigShow[0]	= true;		
		m_cInitConfig.bRightShow[0]		= true;
		m_cInitConfig.nConfig[0]		= vecValue[0];
		m_cInitConfig.strRight[0]		= "日RSI;";

		m_cInitConfig.bLeftShow[1]		= false;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[1];		
		m_cInitConfig.strRight[1]		= "日RSI;";

		m_cInitConfig.bLeftShow[2]		= false;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[2];
		m_cInitConfig.strRight[2]		= "日RSI;";

		m_cInitConfig.vecConfigMin.clear();
		m_cInitConfig.vecConfigMax.clear();

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(100);

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(100);	

		m_cInitConfig.vecConfigMin.push_back(2);
		m_cInitConfig.vecConfigMax.push_back(100);	
	}
	InitDialog(m_cInitConfig);
	wxString strTitle;
	strTitle.Printf(wxT("指标参数调整: -- %s"), m_cInitConfig.strTechIndex);
	SetTitle(strTitle);
}
void TechIndexConfigDlg::OnClose(wxCloseEvent& /*event*/)
{
	if(m_bDelConfig)
	{
		vector<int> vecCofig;
		WriteConfigFrmFile(m_cInitConfig.strTechIndex.c_str(), m_enumPhrase, vecCofig, true);
	}
	else if(m_bWriteConfig)
	{
		vector<int> vecCofig;
		for(int i=0; i<MAX_CONFIG_ITEMS; i++)
		{
			if(m_cInitConfig.bConfigShow[i])
				vecCofig.push_back(m_cInitConfig.nConfig[i]);		
		}
		WriteConfigFrmFile(m_cInitConfig.strTechIndex.c_str(), m_enumPhrase, vecCofig);
	}
	Destroy();
}
void TechIndexConfigDlg::OnRestore( wxCommandEvent &event )
{
	m_bInit = false;//setvalue时候不要触发消息
	if(m_cInitConfig.strTechIndex == MA)
	{
		m_cInitConfig.nConfig[0]		= 5;
		m_cInitConfig.nConfig[1]		= 10;
		m_cInitConfig.nConfig[2]		= 20;
		m_cInitConfig.nConfig[3]		= 30;
		m_cInitConfig.nConfig[4]		= 60;
		m_cInitConfig.nConfig[5]		= 120;

		for(int i =0; i< 6;i++)
			WxSpinCtrl1[i]->SetValue(m_cInitConfig.nConfig[i]);	

		for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
			m_cInitConfig.nConfig[i]		= WxSpinCtrl1[i]->GetValue();	
		
	}
	else if(m_cInitConfig.strTechIndex == VOL)
	{
		m_cInitConfig.nConfig[0]		= 5;
		m_cInitConfig.nConfig[1]		= 10;
		m_cInitConfig.nConfig[2]		= 20;

		for(int i =0; i< 6;i++)
			WxSpinCtrl1[i]->SetValue(m_cInitConfig.nConfig[i]);	

		for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
			m_cInitConfig.nConfig[i]		= WxSpinCtrl1[i]->GetValue();	

	}
	else if(m_cInitConfig.strTechIndex == KDJ)
	{
		m_cInitConfig.nConfig[0]		= 9;
		m_cInitConfig.nConfig[1]		= 3;
		m_cInitConfig.nConfig[2]		= 3;

		for(int i =0; i< 6;i++)
			WxSpinCtrl1[i]->SetValue(m_cInitConfig.nConfig[i]);	

		for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
			m_cInitConfig.nConfig[i]		= WxSpinCtrl1[i]->GetValue();	

	}
	else if(m_cInitConfig.strTechIndex == MACD)
	{
		m_cInitConfig.nConfig[0]		= 26;		
		m_cInitConfig.nConfig[1]		= 12;			
		m_cInitConfig.nConfig[2]		= 9;

		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]);	

	}
	else if(m_cInitConfig.strTechIndex == BOLL)
	{
		m_cInitConfig.nConfig[0]		= 26;		
		m_cInitConfig.nConfig[1]		= 2;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 
	}
	else if(m_cInitConfig.strTechIndex == UOS)
	{
		m_cInitConfig.nConfig[0]		= 7;		
		m_cInitConfig.nConfig[1]		= 14;
		m_cInitConfig.nConfig[2]		= 28;		
		m_cInitConfig.nConfig[3]		= 6;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]);	
		WxSpinCtrl1[3]->SetValue(m_cInitConfig.nConfig[3]); 
	}
	else if(m_cInitConfig.strTechIndex == BIAS)
	{
		m_cInitConfig.nConfig[0]		= 6;		
		m_cInitConfig.nConfig[1]		= 12;
		m_cInitConfig.nConfig[2]		= 24;		
		
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]);
	}
	else if(m_cInitConfig.strTechIndex == DMI)
	{
		m_cInitConfig.nConfig[0]		= 14;		
		m_cInitConfig.nConfig[1]		= 6;		

		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]);		
	}
	else if(m_cInitConfig.strTechIndex == PSY)
	{
		m_cInitConfig.nConfig[0]		= 12;			
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[0]);		
	}
	else if(m_cInitConfig.strTechIndex == ROC)
	{
		m_cInitConfig.nConfig[0]		= 12;		
		m_cInitConfig.nConfig[1]		= 6;	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[0]);		
	}
	else if(m_cInitConfig.strTechIndex == BBI)
	{
		m_cInitConfig.nConfig[0]		= 3;		
		m_cInitConfig.nConfig[1]		= 6;	
		m_cInitConfig.nConfig[2]		= 12;		
		m_cInitConfig.nConfig[3]		= 24;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);		
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]);	
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]);	
		WxSpinCtrl1[3]->SetValue(m_cInitConfig.nConfig[3]);	
	}
	else if(m_cInitConfig.strTechIndex == EXPMA)
	{
		m_cInitConfig.nConfig[0]		= 5;		
		m_cInitConfig.nConfig[1]		= 10;	
		m_cInitConfig.nConfig[2]		= 20;		
		m_cInitConfig.nConfig[3]		= 60;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);		
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]);	
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]);	
		WxSpinCtrl1[3]->SetValue(m_cInitConfig.nConfig[3]);	
	}
	else if(m_cInitConfig.strTechIndex == DMA)
	{
		m_cInitConfig.nConfig[0]		= 10;		
		m_cInitConfig.nConfig[1]		= 50;
		m_cInitConfig.nConfig[2]		= 10;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]); 
	}
	else if(m_cInitConfig.strTechIndex == SAR)
	{
		m_cInitConfig.nConfig[0]		= 10;		
		m_cInitConfig.nConfig[1]		= 2;
		m_cInitConfig.nConfig[2]		= 20;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]); 
	}
	else if(m_cInitConfig.strTechIndex == TRIX)
	{
		m_cInitConfig.nConfig[0]		= 12;		
		m_cInitConfig.nConfig[1]		= 20;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 		
	}
	else if(m_cInitConfig.strTechIndex == MTM)
	{
		m_cInitConfig.nConfig[0]		= 6;		
		m_cInitConfig.nConfig[1]		= 6;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 		
	}
	else if(m_cInitConfig.strTechIndex == CRTECH)
	{
		m_cInitConfig.nConfig[0]		= 26;		
		m_cInitConfig.nConfig[1]		= 5;
		m_cInitConfig.nConfig[2]		= 10;		
		m_cInitConfig.nConfig[3]		= 20;
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 	
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]);	
		WxSpinCtrl1[3]->SetValue(m_cInitConfig.nConfig[3]); 
	}
	else if(m_cInitConfig.strTechIndex == VR)
	{
		m_cInitConfig.nConfig[0]		= 26;		
		m_cInitConfig.nConfig[1]		= 6;	
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 		
	}
	else if(m_cInitConfig.strTechIndex == EMV)
	{
		m_cInitConfig.nConfig[0]		= 14;		
		m_cInitConfig.nConfig[1]		= 9;	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[1]); 		
	}
	else if(m_cInitConfig.strTechIndex == CCI)
	{
		m_cInitConfig.nConfig[0]		= 14;				
		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);			
	}
	else if(m_cInitConfig.strTechIndex == RSI)
	{
		m_cInitConfig.nConfig[0]		= 6;		
		m_cInitConfig.nConfig[1]		= 12;
		m_cInitConfig.nConfig[2]		= 24;		

		WxSpinCtrl1[0]->SetValue(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1]->SetValue(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2]->SetValue(m_cInitConfig.nConfig[2]);
	}
	for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
	{
		if(m_cInitConfig.bConfigShow[i])
			m_vecConfig.push_back(m_cInitConfig.nConfig[i]);
	}
	wxCommandEvent myEvent(wxEVT_CONFIG_CHANGE);
	myEvent.SetClientData(&m_cInitConfig);
	if(m_pMsgWnd)
		m_pMsgWnd->AddPendingEvent(myEvent);
	m_bInit =true;	
	m_bDelConfig = true;
	event.Skip();
}
void TechIndexConfigDlg::InitDialog( CInitConfig&  cInitConfig)
{
	m_bInit = false;//setvalue时候不要触发消息
	m_cInitConfig = cInitConfig;

	wxFont wxfont2(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	wxClientDC dc(this);	
	dc.SetFont(wxfont2);

	int nTop = 10;	
	for(int i =0; i< 6; i++)
	{
		int nSpinHeight =0;
		wxCoord w = 0,h =0;
		int nLeft =10;
		if(!cInitConfig.bLeftShow[i])
		{
			WxStaticText1[i]->Show(false);
		}
		else
		{
			WxStaticText1[i]->SetLabel(cInitConfig.strLeft[i]);		
			dc.GetTextExtent(cInitConfig.strLeft[i], &w, &h);	
			WxStaticText1[i]->SetPosition(wxPoint(nLeft, nTop));
			WxStaticText1[i]->SetSize(wxSize(w, h));
			nLeft = nLeft+w+5;
		}

		if(!cInitConfig.bConfigShow[i])
		{
			WxSpinCtrl1[i]->Show(false);
			wxSize size = WxSpinCtrl1[i]->GetSize();
			nSpinHeight = size.y;
			WxSpinCtrl1[i]->SetPosition(wxPoint(nLeft, nTop));
			nLeft = nLeft + size.x+ 3;
		}
		else
		{
			if((int)cInitConfig.vecConfigMin.size() >i && (int)cInitConfig.vecConfigMax.size())
				WxSpinCtrl1[i]->SetRange(cInitConfig.vecConfigMin[i], cInitConfig.vecConfigMax[i]);
			else
				WxSpinCtrl1[i]->SetRange(0, 999);
			WxSpinCtrl1[i]->SetValue(cInitConfig.nConfig[i]);		

			wxSize size = WxSpinCtrl1[i]->GetSize();
			nSpinHeight = size.y;
			WxSpinCtrl1[i]->SetPosition(wxPoint(nLeft, nTop));
			nLeft = nLeft + size.x+ 3;
		}

		if(!cInitConfig.bRightShow[i])
		{
			WxStaticText11[i]->Show(false);
		}
		else
		{
			WxStaticText11[i]->SetLabel(cInitConfig.strRight[i]);
			dc.GetTextExtent(cInitConfig.strRight[i], &w, &h);	
			WxStaticText11[i]->SetPosition(wxPoint(nLeft, nTop));
			WxStaticText11[i]->SetSize(wxSize(w, h));
			nLeft = nLeft+w+5;
		}

		if(nSpinHeight)
			nTop += nSpinHeight+5;//spin显示的情况
		else if(h)
			nTop+= h+5;//文字显示，spin隐藏
	}

	 if(m_cInitConfig.strTechIndex == MACD)
	 {	
		wxPoint pt0 = WxSpinCtrl1[0]->GetPosition();
		wxPoint pt1 = WxSpinCtrl1[1]->GetPosition();
		
		WxSpinCtrl1[0]->SetPosition(pt1);
		WxSpinCtrl1[1]->SetPosition(pt0);
	 }	
	m_bInit =true;
}
void TechIndexConfigDlg::OnCloseEvent( wxCommandEvent &event )
{
	
	Close(true);
//	event.Skip();
}
void TechIndexConfigDlg::OnSpinValue(int nID, int nValue)
{	
	if(!m_bInit)
		return;
	if(nID >= MAX_CONFIG_ITEMS || nID <0)
		return;

	
	for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
		m_cInitConfig.nConfig[i]		= WxSpinCtrl1[i]->GetValue();	
	m_cInitConfig.nConfig[nID] = nValue;//event.GetInt();


	m_bWriteConfig = true;
	m_bDelConfig   = false;

	wxCommandEvent myEvent(wxEVT_CONFIG_CHANGE);
	myEvent.SetClientData(&m_cInitConfig);
	if(m_pMsgWnd)
		m_pMsgWnd->AddPendingEvent(myEvent);
}
void TechIndexConfigDlg::OnSpinUp6( wxCommandEvent& event )
{
	OnSpinValue(5, event.GetInt());	
}
void TechIndexConfigDlg::OnSpinUp5( wxCommandEvent& event )
{
	OnSpinValue(4, event.GetInt());		
}
void TechIndexConfigDlg::OnSpinUp4( wxCommandEvent& event )
{
	OnSpinValue(3, event.GetInt());
}
void TechIndexConfigDlg::OnSpinUp3( wxCommandEvent& event )
{
	OnSpinValue(2, event.GetInt());
}
void TechIndexConfigDlg::OnSpinUp2( wxCommandEvent& event )
{
	OnSpinValue(1, event.GetInt());
}
void TechIndexConfigDlg::OnSpinUp1( wxCommandEvent& event )
{
	OnSpinValue(0, event.GetInt());
}

bool TechIndexConfigDlg::WriteConfigFrmFile(string strTechIndexName, EnumPhrase enumPhrase, vector<int>& vecCofig, bool bDel)
{
	wxString strPath;
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	strPath = filename.substr(0, splitpos+1);

	wxString strDir;
	strDir.Printf(_T("%shqData\\hqCfg.xml"), strPath);


	TiXmlDocument* pXmlDocment = new TiXmlDocument( strDir.c_str());
	if( NULL == pXmlDocment)
	{
		return false;
	}
	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() )
	{
		return false;
	}
	TiXmlElement *root = pXmlDocment->RootElement();
	if ( NULL == root )
	{
		return false;
	}
	if( std::string(root->Value()) != "hq")
	{
		return false;
	}
	TiXmlElement *tList = root->FirstChildElement("techindex");
	if(tList == NULL)
	{
		tList = new TiXmlElement("techindex");
		root->LinkEndChild(tList);			
	}
	
	wxString strTechTitle;
	for(int nConfig =0; nConfig<(int)vecCofig.size(); nConfig++)
	{
		wxString strTemp;
		if(nConfig == 0)
			strTemp.Printf(wxT("%d"), vecCofig[nConfig]);
		else
			strTemp.Printf(wxT(", %d"), vecCofig[nConfig]);

		strTechTitle.Append(strTemp);
	}
	
	TiXmlNode *ListItem = tList->FirstChild(strTechIndexName);
	if(ListItem != NULL)
	{
		string strOrg = GetTechNamePhrase(strTechIndexName,  enumPhrase);
		TiXmlNode *ListItem2 = ListItem->FirstChild(strOrg);
		if(bDel)
		{				
			ListItem->RemoveChild(ListItem2);
			pXmlDocment->SaveFile(strDir.c_str());
			return true;
		}
		if(ListItem2 == NULL)
		{
			TiXmlElement *InstrumentItem2 = new TiXmlElement(strOrg.c_str());
			ListItem->LinkEndChild(InstrumentItem2);
		}
		TiXmlNode *ListItemChild = ListItem->FirstChild(strOrg.c_str());
		if(ListItemChild)
		{
			TiXmlNode *mLast = ListItemChild->FirstChild();
			if(mLast == NULL)
			{
				TiXmlText newText(strTechTitle.c_str());	
				ListItemChild->InsertEndChild(newText);
			}
			else
			{
				mLast->SetValue(strTechTitle.c_str());
			}
		}
		else
		{
			TiXmlElement *InstrumentItem2 = new TiXmlElement(strOrg.c_str());
			ListItem->LinkEndChild(InstrumentItem2);

			TiXmlText newText(strTechTitle.c_str());	
			InstrumentItem2->InsertEndChild(newText);
		}
	}
	else
	{
		TiXmlElement *InstrumentItem = new TiXmlElement(strTechIndexName.c_str());
		tList->LinkEndChild(InstrumentItem);

		string strOrg = GetTechNamePhrase(strTechIndexName,  enumPhrase);
		TiXmlElement *InstrumentItem2 = new TiXmlElement(strOrg.c_str());
		InstrumentItem->LinkEndChild(InstrumentItem2);

		TiXmlText newText(strTechTitle.c_str());	
		InstrumentItem2->InsertEndChild(newText);
				
	}
	pXmlDocment->SaveFile(strDir.c_str());
	return true;
}