// TechIndexConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionQuation.h"
#include "TechIndexConfigDlg.h"


// CTechIndexConfigDlg dialog

IMPLEMENT_DYNAMIC(CTechIndexConfigDlg, CDialog)

CTechIndexConfigDlg::CTechIndexConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTechIndexConfigDlg::IDD, pParent)
{

	LOGFONT lf;
	memset(&lf,0,sizeof(lf));
	lf.lfHeight = 13;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfWeight = FW_NORMAL;
	lf.lfQuality = PROOF_QUALITY;
	strcpy(lf.lfFaceName,_T("Tahoma"));
	m_hSysFontStatic = ::CreateFontIndirect(&lf);

	if(!m_FontStatic.m_hObject)
	{
		CFont font;
		LOGFONT lf;
		HFONT hSysFont; 
		hSysFont = m_hSysFontStatic;
		font.Attach( hSysFont );
		font.GetLogFont(&lf);
		font.Detach();
		m_FontStatic.CreateFontIndirect(&lf);
	}


	
}

CTechIndexConfigDlg::~CTechIndexConfigDlg()
{

}

void CTechIndexConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, WxStaticText1[0]);
	DDX_Control(pDX, IDC_STATIC2, WxStaticText1[1]);
	DDX_Control(pDX, IDC_STATIC3, WxStaticText1[2]);
	DDX_Control(pDX, IDC_STATIC4, WxStaticText1[3]);
	DDX_Control(pDX, IDC_STATIC5, WxStaticText1[4]);
	DDX_Control(pDX, IDC_STATIC6, WxStaticText1[5]);

	DDX_Control(pDX, IDC_STATIC12, WxStaticText11[0]);
	DDX_Control(pDX, IDC_STATIC22, WxStaticText11[1]);
	DDX_Control(pDX, IDC_STATIC32, WxStaticText11[2]);
	DDX_Control(pDX, IDC_STATIC42, WxStaticText11[3]);
	DDX_Control(pDX, IDC_STATIC52, WxStaticText11[4]);
	DDX_Control(pDX, IDC_STATIC62, WxStaticText11[5]);
	DDX_Control(pDX, IDC_SPIN1, WxSpinCtrl1[0]);
	DDX_Control(pDX, IDC_SPIN2, WxSpinCtrl1[1]);
	DDX_Control(pDX, IDC_SPIN3, WxSpinCtrl1[2]);
	DDX_Control(pDX, IDC_SPIN4, WxSpinCtrl1[3]);
	DDX_Control(pDX, IDC_SPIN5, WxSpinCtrl1[4]);
	DDX_Control(pDX, IDC_SPIN6, WxSpinCtrl1[5]);
	DDX_Control(pDX, IDC_EDIT1, m_Edit[0]);
	DDX_Control(pDX, IDC_EDIT2, m_Edit[1]);
	DDX_Control(pDX, IDC_EDIT3, m_Edit[2]);
	DDX_Control(pDX, IDC_EDIT4, m_Edit[3]);
	DDX_Control(pDX, IDC_EDIT5, m_Edit[4]);
	DDX_Control(pDX, IDC_EDIT6, m_Edit[5]);
}


BEGIN_MESSAGE_MAP(CTechIndexConfigDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTechIndexConfigDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTechIndexConfigDlg::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTechIndexConfigDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CTechIndexConfigDlg::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTechIndexConfigDlg::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTechIndexConfigDlg::OnDeltaposSpin6)
	ON_BN_CLICKED(IDC_BUTTON1, &CTechIndexConfigDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTechIndexConfigDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SPIN1, &CTechIndexConfigDlg::OnNMThemeChangedSpin1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SPIN1, &CTechIndexConfigDlg::OnNMReleasedcaptureSpin1)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SPIN1, &CTechIndexConfigDlg::OnNMOutofmemorySpin1)
END_MESSAGE_MAP()


BOOL CTechIndexConfigDlg::OnInitDialog() 
{	
	CDialog::OnInitDialog();

	for(INT I =0; I< 6; I++)
	{
		WxStaticText1[I].SetFont(&m_FontStatic);
		WxStaticText11[I].SetFont(&m_FontStatic);


		WxSpinCtrl1[I].SetDecimalPlaces (0);
		WxSpinCtrl1[I].SetTrimTrailingZeros (FALSE);
		WxSpinCtrl1[I].SetRangeAndDelta(0.1, 10.0, 1);
		WxSpinCtrl1[I].SetPos (5);
	}
	
	if(::IsWindow(m_hWnd))
	{	
		InitDialog(m_cInitConfig);
		CString strTitle;
		strTitle.Format(_T("指标参数调整: -- %s"), m_cInitConfig.strTechIndex);
		SetWindowText(strTitle);
	}

	return TRUE;
}
void CTechIndexConfigDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);	
	OnSpinValue(0, pNMUpDown->iPos);	
	*pResult = 0;
}

void CTechIndexConfigDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	

	OnSpinValue(1, pNMUpDown->iPos);	
	*pResult = 0;
}

void CTechIndexConfigDlg::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	OnSpinValue(2, pNMUpDown->iPos);	
	*pResult = 0;
}

void CTechIndexConfigDlg::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	OnSpinValue(3, pNMUpDown->iPos);	
	*pResult = 0;
}

void CTechIndexConfigDlg::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	OnSpinValue(4, pNMUpDown->iPos);	
	*pResult = 0;
}

void CTechIndexConfigDlg::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	OnSpinValue(5, pNMUpDown->iPos);	
	*pResult = 0;
}

void CTechIndexConfigDlg::OnBnClickedButton1()
{
	m_bInit = false;//SetPos时候不要触发消息
	if(m_cInitConfig.strTechIndex == MA)
	{
		m_cInitConfig.nConfig[0]		= 5;
		m_cInitConfig.nConfig[1]		= 10;
		m_cInitConfig.nConfig[2]		= 20;
		m_cInitConfig.nConfig[3]		= 30;
		m_cInitConfig.nConfig[4]		= 60;
		m_cInitConfig.nConfig[5]		= 120;

		for(int i =0; i< 6;i++)
			WxSpinCtrl1[i].SetPos(m_cInitConfig.nConfig[i]);	

		for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
			m_cInitConfig.nConfig[i]		= WxSpinCtrl1[i].GetPos();	

	}
	else if(m_cInitConfig.strTechIndex == VOL)
	{
		m_cInitConfig.nConfig[0]		= 5;
		m_cInitConfig.nConfig[1]		= 10;
		m_cInitConfig.nConfig[2]		= 20;

		for(int i =0; i< 6;i++)
			WxSpinCtrl1[i].SetPos(m_cInitConfig.nConfig[i]);	

		for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
			m_cInitConfig.nConfig[i]		= WxSpinCtrl1[i].GetPos();	

	}
	else if(m_cInitConfig.strTechIndex == KDJ)
	{
		m_cInitConfig.nConfig[0]		= 9;
		m_cInitConfig.nConfig[1]		= 3;
		m_cInitConfig.nConfig[2]		= 3;

		for(int i =0; i< 6;i++)
			WxSpinCtrl1[i].SetPos(m_cInitConfig.nConfig[i]);	

		for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
			m_cInitConfig.nConfig[i]		= WxSpinCtrl1[i].GetPos();	

	}
	else if(m_cInitConfig.strTechIndex == MACD)
	{
		m_cInitConfig.nConfig[0]		= 26;		
		m_cInitConfig.nConfig[1]		= 12;			
		m_cInitConfig.nConfig[2]		= 9;

		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]);	

	}
	else if(m_cInitConfig.strTechIndex == BOLL)
	{
		m_cInitConfig.nConfig[0]		= 26;		
		m_cInitConfig.nConfig[1]		= 2;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 
	}
	else if(m_cInitConfig.strTechIndex == UOS)
	{
		m_cInitConfig.nConfig[0]		= 7;		
		m_cInitConfig.nConfig[1]		= 14;
		m_cInitConfig.nConfig[2]		= 28;		
		m_cInitConfig.nConfig[3]		= 6;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]);	
		WxSpinCtrl1[3].SetPos(m_cInitConfig.nConfig[3]); 
	}
	else if(m_cInitConfig.strTechIndex == BIAS)
	{
		m_cInitConfig.nConfig[0]		= 6;		
		m_cInitConfig.nConfig[1]		= 12;
		m_cInitConfig.nConfig[2]		= 24;		

		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]);
	}
	else if(m_cInitConfig.strTechIndex == DMI)
	{
		m_cInitConfig.nConfig[0]		= 14;		
		m_cInitConfig.nConfig[1]		= 6;		

		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]);		
	}
	else if(m_cInitConfig.strTechIndex == PSY)
	{
		m_cInitConfig.nConfig[0]		= 12;			
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[0]);		
	}
	else if(m_cInitConfig.strTechIndex == ROC)
	{
		m_cInitConfig.nConfig[0]		= 12;		
		m_cInitConfig.nConfig[1]		= 6;	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[0]);		
	}
	else if(m_cInitConfig.strTechIndex == BBI)
	{
		m_cInitConfig.nConfig[0]		= 3;		
		m_cInitConfig.nConfig[1]		= 6;	
		m_cInitConfig.nConfig[2]		= 12;		
		m_cInitConfig.nConfig[3]		= 24;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);		
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]);	
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]);	
		WxSpinCtrl1[3].SetPos(m_cInitConfig.nConfig[3]);	
	}
	else if(m_cInitConfig.strTechIndex == EXPMA)
	{
		m_cInitConfig.nConfig[0]		= 5;		
		m_cInitConfig.nConfig[1]		= 10;	
		m_cInitConfig.nConfig[2]		= 20;		
		m_cInitConfig.nConfig[3]		= 60;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);		
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]);	
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]);	
		WxSpinCtrl1[3].SetPos(m_cInitConfig.nConfig[3]);	
	}
	else if(m_cInitConfig.strTechIndex == DMA)
	{
		m_cInitConfig.nConfig[0]		= 10;		
		m_cInitConfig.nConfig[1]		= 50;
		m_cInitConfig.nConfig[2]		= 10;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]); 
	}
	else if(m_cInitConfig.strTechIndex == SAR)
	{
		m_cInitConfig.nConfig[0]		= 10;		
		m_cInitConfig.nConfig[1]		= 2;
		m_cInitConfig.nConfig[2]		= 20;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]); 
	}
	else if(m_cInitConfig.strTechIndex == TRIX)
	{
		m_cInitConfig.nConfig[0]		= 12;		
		m_cInitConfig.nConfig[1]		= 20;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 		
	}
	else if(m_cInitConfig.strTechIndex == MTM)
	{
		m_cInitConfig.nConfig[0]		= 6;		
		m_cInitConfig.nConfig[1]		= 6;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 		
	}
	else if(m_cInitConfig.strTechIndex == CRTECH)
	{
		m_cInitConfig.nConfig[0]		= 26;		
		m_cInitConfig.nConfig[1]		= 5;
		m_cInitConfig.nConfig[2]		= 10;		
		m_cInitConfig.nConfig[3]		= 20;
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 	
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]);	
		WxSpinCtrl1[3].SetPos(m_cInitConfig.nConfig[3]); 
	}
	else if(m_cInitConfig.strTechIndex == VR)
	{
		m_cInitConfig.nConfig[0]		= 26;		
		m_cInitConfig.nConfig[1]		= 6;	
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 		
	}
	else if(m_cInitConfig.strTechIndex == EMV)
	{
		m_cInitConfig.nConfig[0]		= 14;		
		m_cInitConfig.nConfig[1]		= 9;	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[1]); 		
	}
	else if(m_cInitConfig.strTechIndex == CCI)
	{
		m_cInitConfig.nConfig[0]		= 14;				
		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);			
	}
	else if(m_cInitConfig.strTechIndex == RSI)
	{
		m_cInitConfig.nConfig[0]		= 6;		
		m_cInitConfig.nConfig[1]		= 12;
		m_cInitConfig.nConfig[2]		= 24;		

		WxSpinCtrl1[0].SetPos(m_cInitConfig.nConfig[0]);	
		WxSpinCtrl1[1].SetPos(m_cInitConfig.nConfig[1]); 
		WxSpinCtrl1[2].SetPos(m_cInitConfig.nConfig[2]);
	}
	for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
	{
		if(m_cInitConfig.bConfigShow[i])
			m_vecConfig.push_back(m_cInitConfig.nConfig[i]);
	}

	if(m_pMsgWnd)
		m_pMsgWnd->SendMessage(MESSAGE_CONFIG_CHANGE,(WPARAM)&m_cInitConfig,0);
	m_bInit =true;	
	m_bDelConfig = true;

}

void CTechIndexConfigDlg::OnBnClickedButton2()
{
	if(m_bDelConfig)
	{
		vector<int> vecCofig;
		WriteConfigFrmFile(m_cInitConfig.strTechIndex.GetBuffer(m_cInitConfig.strTechIndex.GetLength()), m_enumPhrase, vecCofig, true);
	}
	else if(m_bWriteConfig)
	{
		vector<int> vecCofig;
		for(int i=0; i<MAX_CONFIG_ITEMS; i++)
		{
			if(m_cInitConfig.bConfigShow[i])
				vecCofig.push_back(m_cInitConfig.nConfig[i]);		
		}
		WriteConfigFrmFile(m_cInitConfig.strTechIndex.GetBuffer(m_cInitConfig.strTechIndex.GetLength()), m_enumPhrase, vecCofig);
	}
	OnOK();

}
void CTechIndexConfigDlg::SetTechIndex(CString strTechIndex, int nIndex, EnumPhrase enumPhrase, vector<int>& vecValue)
{		
	m_cInitConfig.strTechIndex	= strTechIndex;
	m_cInitConfig.nIndex		= nIndex;
	m_enumPhrase = enumPhrase;
	if(strTechIndex == MA)
	{
		for(int i =0; i<6; i++)
		{
			m_cInitConfig.bConfigShow[i]	= true;		
			m_cInitConfig.bRightShow[i]		= true;
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
			m_cInitConfig.bRightShow[i]		= true;
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
		m_cInitConfig.strLeft[0]		= "TR:收盘价的(";
		m_cInitConfig.strRight[0]		= ")日三重指数平滑平均数;";

		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= false;		
		m_cInitConfig.bRightShow[1]		= false;
		m_cInitConfig.strLeft[1]		= "TRIX:100乘以(今日TR 减去 昨日TR)除以昨日TR;";

		m_cInitConfig.bLeftShow[2]		= true;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[1];
		m_cInitConfig.strLeft[2]		= "TRMA:TRIX的(";
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
		m_cInitConfig.strLeft[0]		= "天数:";		

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
		m_cInitConfig.strLeft[0]		= "请设置计算参数:";		


		m_cInitConfig.bLeftShow[1]		= true;			
		m_cInitConfig.bConfigShow[1]	= true;		
		m_cInitConfig.bRightShow[1]		= true;
		m_cInitConfig.nConfig[1]		= vecValue[0];		
		m_cInitConfig.strLeft[1]		= "              P1:";
		m_cInitConfig.strRight[1]		= "（1 — 999）";

		m_cInitConfig.bLeftShow[2]		= true;			
		m_cInitConfig.bConfigShow[2]	= true;		
		m_cInitConfig.bRightShow[2]		= true;
		m_cInitConfig.nConfig[2]		= vecValue[1];		
		m_cInitConfig.strLeft[2]		= "              P2:";
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
		m_cInitConfig.strLeft[0]		= "天数:";		
		m_cInitConfig.strRight[0]		= "天;";

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
	if(::IsWindow(m_hWnd))
	{	
		InitDialog(m_cInitConfig);
		CString strTitle;
		strTitle.Format(_T("指标参数调整: -- %s"), m_cInitConfig.strTechIndex);
		SetWindowText(strTitle);
	}
}
void CTechIndexConfigDlg::InitDialog( CInitConfig&  cInitConfig)
{
	m_bInit = false;//SetPos时候不要触发消息
	m_cInitConfig = cInitConfig;
	if(m_pMsgWnd == NULL)
		return;

	CDC* pDC = GetDC();	
	CFont *pOldFont = pDC->SelectObject(&m_FontStatic);

	int nTop = 10;	
	for(int i =0; i< 6; i++)
	{
		int nSpinHeight =0;
		CSize sz;
		int nLeft =10;
		if(!cInitConfig.bLeftShow[i])
		{
			WxStaticText1[i].ShowWindow(false);
		}
		else
		{
			WxStaticText1[i].SetWindowText(cInitConfig.strLeft[i]);		
			sz = pDC->GetTextExtent(cInitConfig.strLeft[i]);	
			WxStaticText1[i].MoveWindow(CRect(nLeft, nTop, nLeft + sz.cx, nTop + sz.cy));
		//	WxStaticText1[i].SetPosition(wxPoint(nLeft, nTop));
		//	WxStaticText1[i].SetSize(wxSize(w, h));
			nLeft = nLeft+sz.cx+5;
		}

		if(!cInitConfig.bConfigShow[i])
		{
			WxSpinCtrl1[i].ShowWindow(false);
			m_Edit[i].ShowWindow(false);
		
			CRect rtEdit;
			m_Edit[i].GetWindowRect(&rtEdit);

			CRect rtWindow;
			WxSpinCtrl1[i].GetWindowRect(&rtWindow);
			sz.cy = rtWindow.Height();
			sz.cx = rtWindow.Width()+ rtEdit.Width();
			nSpinHeight = sz.cy;
			//WxSpinCtrl1[i].SetPosition(wxPoint(nLeft, nTop));
			m_Edit[i].MoveWindow(CRect(nLeft, nTop, nLeft + rtEdit.Width(), nTop + sz.cy));

			WxSpinCtrl1[i].MoveWindow(CRect(nLeft + rtEdit.Width()+1 , nTop, nLeft + rtEdit.Width()+ rtWindow.Width()+1, nTop + sz.cy));
			nLeft = nLeft + sz.cx+ 3;
		}
		else
		{
			if((int)cInitConfig.vecConfigMin.size() >i && (int)cInitConfig.vecConfigMax.size())
				WxSpinCtrl1[i].SetRangeAndDelta(cInitConfig.vecConfigMin[i], cInitConfig.vecConfigMax[i], 1);
			else
				WxSpinCtrl1[i].SetRangeAndDelta(0, 999, 1);
			WxSpinCtrl1[i].SetPos(cInitConfig.nConfig[i]);		

			CRect rtEdit;
			m_Edit[i].GetWindowRect(&rtEdit);

			CRect rtWindow;
			WxSpinCtrl1[i].GetWindowRect(&rtWindow);
			sz.cy = rtWindow.Height();
			sz.cx = rtWindow.Width()+ rtEdit.Width();
			nSpinHeight = sz.cy;
			//WxSpinCtrl1[i].SetPosition(wxPoint(nLeft, nTop));
			m_Edit[i].MoveWindow(CRect(nLeft, nTop, nLeft + rtEdit.Width(), nTop + sz.cy));
			WxSpinCtrl1[i].MoveWindow(CRect(nLeft + rtEdit.Width()+1  , nTop, nLeft + rtEdit.Width()+ rtWindow.Width()+1, nTop + sz.cy));
		//	WxSpinCtrl1[i].MoveWindow(CRect(nLeft, nTop, nLeft + sz.cx, nTop + sz.cy));
			
			nLeft = nLeft + sz.cx+ 3;
		}

		if(!cInitConfig.bRightShow[i])
		{
			WxStaticText11[i].ShowWindow(false);
		}
		else
		{
			WxStaticText11[i].SetWindowText(cInitConfig.strRight[i]);
			sz = pDC->GetTextExtent(cInitConfig.strRight[i]);	
		//	dc.GetTextExtent(cInitConfig.strRight[i], &w, &h);	
			WxStaticText11[i].MoveWindow(CRect(nLeft, nTop, nLeft + sz.cx, nTop + sz.cy));
			//WxStaticText11[i].SetPosition(wxPoint(nLeft, nTop));
			//WxStaticText11[i].SetSize(wxSize(w, h));
			nLeft = nLeft+sz.cx + 5;
		}

		if(nSpinHeight)
			nTop += nSpinHeight+5;//spin显示的情况
		else if(sz.cy)
			nTop+= sz.cy+5;//文字显示，spin隐藏
	}

	if(m_cInitConfig.strTechIndex == MACD)
	{	
	//	CPoint pt0 = WxSpinCtrl1[0].GetPosition();
	//	CPoint pt1 = WxSpinCtrl1[1].GetPosition();

	//	WxSpinCtrl1[0].SetPosition(pt1);
	//	WxSpinCtrl1[1].SetPosition(pt0);
	}	

	pDC->SelectObject(pOldFont);

	ReleaseDC(pDC);
	m_bInit =true;
}
void CTechIndexConfigDlg::OnSpinValue(int nID, int nValue)
{	
	if(!m_bInit)
		return;
	if(nID >= MAX_CONFIG_ITEMS || nID <0)
		return;


	for(int i =0; i< MAX_CONFIG_ITEMS; i++)	
		m_cInitConfig.nConfig[i]		= WxSpinCtrl1[i].GetPos()+0.00001;	
	///m_cInitConfig.nConfig[nID] = nValue;


	m_bWriteConfig = true;
	m_bDelConfig   = false;

	if(m_pMsgWnd)
		m_pMsgWnd->SendMessage(MESSAGE_CONFIG_CHANGE,(WPARAM)&m_cInitConfig,0);
}
bool CTechIndexConfigDlg::WriteConfigFrmFile(string strTechIndexName, EnumPhrase enumPhrase, vector<int>& vecCofig, bool bDel)
{
	CString strPath;
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	strPath.Format(_T("%s"), filename.substr(0, splitpos+1).c_str()) ;

	CString strDir;
	strDir.Format(_T("%shqData\\hqCfg.xml"), strPath);


	TiXmlDocument* pXmlDocment = new TiXmlDocument( strDir.GetBuffer(strDir.GetLength()));
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

	CString strTechTitle;
	for(int nConfig =0; nConfig<(int)vecCofig.size(); nConfig++)
	{
		CString strTemp;
		if(nConfig == 0)
			strTemp.Format(_T("%d"), vecCofig[nConfig]);
		else
			strTemp.Format(_T(", %d"), vecCofig[nConfig]);

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
			pXmlDocment->SaveFile(strDir.GetBuffer(strDir.GetLength()));
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
				TiXmlText newText(strTechTitle.GetBuffer(strTechTitle.GetLength()));	
				ListItemChild->InsertEndChild(newText);
			}
			else
			{
				mLast->SetValue(strTechTitle.GetBuffer(strTechTitle.GetLength()));
			}
		}
		else
		{
			TiXmlElement *InstrumentItem2 = new TiXmlElement(strOrg.c_str());
			ListItem->LinkEndChild(InstrumentItem2);

			TiXmlText newText(strTechTitle.GetBuffer(strTechTitle.GetLength()));	
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

		TiXmlText newText(strTechTitle.GetBuffer(strTechTitle.GetLength()));	
		InstrumentItem2->InsertEndChild(newText);

	}
	pXmlDocment->SaveFile(strDir.GetBuffer(strDir.GetLength()));
	return true;
}
void CTechIndexConfigDlg::OnNMThemeChangedSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows XP or greater.
	// The symbol _WIN32_WINNT must be >= 0x0501.
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTechIndexConfigDlg::OnNMReleasedcaptureSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTechIndexConfigDlg::OnNMOutofmemorySpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
