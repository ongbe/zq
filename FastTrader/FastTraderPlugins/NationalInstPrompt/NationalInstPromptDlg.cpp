// NationalInstPromptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NationalInstPrompt.h"
#include "NationalInstPromptDlg.h"

#include <io.h> 
#include "Tools.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNationalInstPromptDlg* CNationalInstPromptDlg::m_pMainDlg = NULL;


void CNationalInstPromptDlg::EnterAlgorithm(const PlatformStru_DepthMarketData* pMarketData)
{
    int nCount = m_listCtrl.GetItemCount();
    for (int i=0;i<nCount;i++)
    {
        CParamData* pParamData = (CParamData*)m_listCtrl.GetItemData(i);

        string szInstNameUpper(pParamData->Param_BandInstrument);
        transform(szInstNameUpper.begin(), szInstNameUpper.end(), szInstNameUpper.begin(), (int (*)(int))toupper); 

        if( 0 == strcmp(pMarketData->InstrumentID,szInstNameUpper.c_str()))
        {
            bool bTrigle = pParamData->Calculate(pMarketData->LastPrice);
            if(bTrigle && pParamData->TurnOn)
            {
                pParamData->TurnOn = false;                
                pParamData->TurnOn?m_btnApplyCtrl.SetWindowText("停用"):m_btnApplyCtrl.SetWindowText("启用");
                m_listCtrl.SetItemText(i,7,pParamData->TurnOn?"启用":"停用");

                if (pParamData->Order_Alarm)
                    ::MessageBeep(MB_OK);

                if (pParamData->Order_AutoOrder)
                    SendInsertOrder(pParamData);
            }

            m_listCtrl.SetItemText(i,2,CTools::StringFromDouble(pMarketData->LastPrice).c_str());
            m_listCtrl.SetItemText(i,3,CTools::StringFromDouble(pParamData->Result_BandTargetPrice).c_str());
            m_listCtrl.SetItemText(i,4,CTools::StringFromDouble(pParamData->Param_BandPrice).c_str());
        }
    }
}

void CNationalInstPromptDlg::SendInsertOrder(CParamData* pData)
{
    string szInstNameUpper(pData->Order_Instrument);
    transform(szInstNameUpper.begin(), szInstNameUpper.end(), szInstNameUpper.begin(), (int (*)(int))toupper); 

    PlatformStru_InputOrder OrderInsert;
    memset(&OrderInsert,0,sizeof(OrderInsert));

    strncpy_s(OrderInsert.InvestorID,sizeof(OrderInsert.InvestorID),m_szAccount.c_str(),sizeof(OrderInsert.InvestorID)-1);
    strncpy_s(OrderInsert.InstrumentID,sizeof(OrderInsert.InstrumentID),szInstNameUpper.c_str(),sizeof(OrderInsert.InstrumentID)-1);
    OrderInsert.OrderPriceType='2';								        //市价 THOST_FTDC_OPT_AnyPrice='1' 限价 THOST_FTDC_OPT_LimitPrice='2';
    OrderInsert.LimitPrice = pData->Param_QuotPrice;
    OrderInsert.Direction=pData->Order_Direction==0?'0':'1';			//买卖方向，0-'0'=买；1-'1'=卖
    if(pData->Order_OffsetID==0) OrderInsert.CombOffsetFlag[0]='0';	    //开平标志，0-'0'=开仓
    else if(pData->Order_OffsetID==1) OrderInsert.CombOffsetFlag[0]='1';//开平标志，1-'1'=平仓
    else OrderInsert.CombOffsetFlag[0]='3';						        //开平标志，2-'3'=平今
    OrderInsert.CombHedgeFlag[0]='1';							        //投机套保，'1'=投机；'3'=套保
    OrderInsert.VolumeTotalOriginal=pData->Order_Count;		            //数量
    OrderInsert.TimeCondition='3';								        //有效期类型，市价单对应：立即完成，否则撤销='1'; 限价单对应：当日有效='3'
    OrderInsert.VolumeCondition='1';							        //成交量类型，'1'表示任何数量
    OrderInsert.MinVolume=1;
    OrderInsert.ContingentCondition='1';						        //立即触发
    OrderInsert.ForceCloseReason='0';							        //强平原因，'0'费强平
    OrderInsert.OrderClass='N';									        //新报单
    strncpy_s(OrderInsert.strAccount,sizeof(OrderInsert.strAccount),m_szAccount.c_str(),sizeof(OrderInsert.strAccount)-1);

    PlusinAPI* pAPI=PlusinAPI::GetInstance();
    if(pAPI) 
        pAPI->Send(CMDID_ORDER_INSERT_ORDER,&OrderInsert,sizeof(OrderInsert));
}

int CNationalInstPromptDlg::PluginsAPICallBack(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId)
{
    switch(nCmdId)
    {
    case CMDID_CONSOLE_LOGON_RSP:
        {
            //登录返回
            if(nErrorCode==SUCCESS)
            {
                m_pMainDlg->m_bLogon=true;

                //查询账号
                PlusinAPI* pAPI=PlusinAPI::GetInstance();
                if(pAPI)
                    pAPI->Send(CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY,NULL,0);
            }
            else 
                m_pMainDlg->m_bLogon=false;

            break;
        }
    case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
        {
            //查询账号返回
            if(nErrorCode==SUCCESS&&nDataLen==(int)sizeof(CommonAccountIDType))
            {
                CommonAccountIDType& pAccount=*(CommonAccountIDType*)pData;
                if(pAccount[sizeof(CommonAccountIDType)-1]!=0)
                    pAccount[sizeof(CommonAccountIDType)-1]=0;
                m_pMainDlg->m_szAccount=string(pAccount); 
            }
            break;
        }        
    case CMDID_QUOTE_SUBSCRIBE_RSP:
        {
            //订阅行情的结果返回
            if(nErrorCode==SUCCESS&&nDataLen==sizeof(InstrumentIDType))
            {
                InstrumentIDType InstName;
                memcpy(InstName,pData,nDataLen);
                InstName[sizeof(InstName)-1]=0;
            }
            break;
        }
    case CMDID_QUOTE_SEND_QUOTE:
        {
            //接收到行情推送
            if(nErrorCode==SUCCESS&&nDataLen==sizeof(PlatformStru_DepthMarketData))
            {
                PlatformStru_DepthMarketData* pMarketData=(PlatformStru_DepthMarketData*)pData;      
                m_pMainDlg->EnterAlgorithm(pMarketData);

                if(m_pMainDlg->m_bChildOpened)
                {
                    string szInstNameUpper(CTools::GetStringFromCEdit(m_pMainDlg->m_pOptDlg->m_editBandInstrumentCtrl).c_str());
                    transform(szInstNameUpper.begin(), szInstNameUpper.end(), szInstNameUpper.begin(), (int (*)(int))toupper); 

                    if(0==strcmp(pMarketData->InstrumentID,szInstNameUpper.c_str()))
                        CTools::SetDoubleToCEdit(m_pMainDlg->m_pOptDlg->m_editQuotPriceCtrl,pMarketData->LastPrice);
                }
            }
            break;
        }    
    }
    return 0;
}

// CNationalInstPromptDlg dialog

CNationalInstPromptDlg::CNationalInstPromptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNationalInstPromptDlg::IDD, pParent)
    , m_bChildOpened(false)
    , m_pOptDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pMainDlg = this;
    m_mapIndex2Data.clear();;
}

CNationalInstPromptDlg::~CNationalInstPromptDlg()
{
    m_pMainDlg = NULL;

    //释放空间
    map<int,CParamData*>::iterator it = m_mapIndex2Data.begin();
    for (;it!=m_mapIndex2Data.end();it++)
    {
        CParamData* pData = it->second;
        if(pData)
        {
            delete pData;
            pData = NULL;
        }
    }

    m_mapIndex2Data.clear();
}

void CNationalInstPromptDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST, m_listCtrl);
    DDX_Control(pDX, IDC_BUTTON_APPLY, m_btnApplyCtrl);
}

BEGIN_MESSAGE_MAP(CNationalInstPromptDlg, CDialog)
	ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()   
    ON_MESSAGE(WM_USER_CLOSE,OnChildClose) 
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CNationalInstPromptDlg::OnBnClickedButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CNationalInstPromptDlg::OnBnClickedButtonModify)
    ON_BN_CLICKED(IDC_BUTTON_DELETE, &CNationalInstPromptDlg::OnBnClickedButtonDelete)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CNationalInstPromptDlg::OnBnClickedButtonApply)
    ON_WM_CLOSE()
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CNationalInstPromptDlg::OnLvnItemchangedList)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CNationalInstPromptDlg::OnDeltaposSpin1)
END_MESSAGE_MAP()


// CNationalInstPromptDlg message handlers

BOOL CNationalInstPromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);

    //插入列 
    m_listCtrl.InsertColumn( 0, "现券代码",     LVCFMT_CENTER,    90 );
    m_listCtrl.InsertColumn( 1, "合约",         LVCFMT_CENTER,    60 ); 
    m_listCtrl.InsertColumn( 2, "期货现价",     LVCFMT_CENTER,    90 ); 
    m_listCtrl.InsertColumn( 3, "目标价格",     LVCFMT_CENTER,    120 ); 
    m_listCtrl.InsertColumn( 4, "现券价格",     LVCFMT_CENTER,    90 ); 
    m_listCtrl.InsertColumn( 5, "报警",         LVCFMT_CENTER,    60 ); 
    m_listCtrl.InsertColumn( 6, "自动下单",     LVCFMT_CENTER,    90 ); 
    m_listCtrl.InsertColumn( 7, "状态",         LVCFMT_CENTER,    60 ); 


    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            pAPI->Init( "NationalInstPrompt",
                        "NationalInstPrompt",
                        &CNationalInstPromptDlg::PluginsAPICallBack,
                        g_hWnd_FT,
                        NEED_NO_POS_NO_BIND,
                        GetSafeHwnd());
            pAPI->Logon();
        }
    }

    //从ini加载ParamData
    if(ReadData())
    {
        SetDataToListCtrl();
        SubscribeInstrument();
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNationalInstPromptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNationalInstPromptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNationalInstPromptDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    int irlt=MessageBox("正在执行交易，如果退出，交易将停止。\n是否退出(并保存当前配置)？",NULL,MB_YESNO);
    if(irlt==IDNO)
        return;

    SaveData();

    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
            pAPI->Logout();

        PlusinAPI::DestroyInstance();
    }

    CDialog::OnClose();
}
void CNationalInstPromptDlg::OnBnClickedButtonAdd()
{
    // TODO: Add your control notification handler code here
    m_bChildOpened = true;
    CParamData* pParamData = new CParamData;

    COptSetupDlg dlg(pParamData);
    m_pOptDlg = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK

        int nCount = m_listCtrl.GetItemCount();
        m_listCtrl.InsertItem(nCount,pParamData->Param_BandCode.c_str());
        m_listCtrl.SetItemText(nCount,1,pParamData->Param_BandInstrument.c_str());
        m_listCtrl.SetItemText(nCount,2,CTools::StringFromDouble(pParamData->Param_QuotPrice).c_str());
        m_listCtrl.SetItemText(nCount,3,CTools::StringFromDouble(pParamData->Result_BandTargetPrice).c_str());
        m_listCtrl.SetItemText(nCount,4,CTools::StringFromDouble(pParamData->Param_BandPrice).c_str());
        m_listCtrl.SetItemText(nCount,5,pParamData->Order_Alarm?"启用":"停用");
        m_listCtrl.SetItemText(nCount,6,pParamData->Order_AutoOrder?"启用":"停用");
        m_listCtrl.SetItemText(nCount,7,pParamData->TurnOn?"启用":"停用");
        
        //绑定
        m_listCtrl.SetItemData(nCount,(DWORD_PTR)pParamData);

        //保存
        m_mapIndex2Data.insert(make_pair(nCount,pParamData));
    }
    else if (nResponse == IDCANCEL)
    {
        delete pParamData;
        pParamData = NULL;
    }
}

void CNationalInstPromptDlg::OnBnClickedButtonModify()
{
    // TODO: Add your control notification handler code here
    int nSelectedIndex = GetListSelectedIndex();
    if(nSelectedIndex<0)
        return;

    m_bChildOpened = true;
    CParamData* pParamData = (CParamData*)m_listCtrl.GetItemData(nSelectedIndex);
    if(pParamData)
    {
        COptSetupDlg dlg(pParamData);
        m_pOptDlg = &dlg;
        INT_PTR nResponse = dlg.DoModal();
        if (nResponse == IDOK)
        {
            m_listCtrl.SetItemText(nSelectedIndex,0,pParamData->Param_BandCode.c_str());
            m_listCtrl.SetItemText(nSelectedIndex,1,pParamData->Param_BandInstrument.c_str());
            m_listCtrl.SetItemText(nSelectedIndex,2,CTools::StringFromDouble(pParamData->Param_QuotPrice).c_str());
            m_listCtrl.SetItemText(nSelectedIndex,3,CTools::StringFromDouble(pParamData->Result_BandTargetPrice).c_str());
            m_listCtrl.SetItemText(nSelectedIndex,4,CTools::StringFromDouble(pParamData->Param_BandPrice).c_str());
            m_listCtrl.SetItemText(nSelectedIndex,5,pParamData->Order_Alarm?"启用":"停用");
            m_listCtrl.SetItemText(nSelectedIndex,6,pParamData->Order_AutoOrder?"启用":"停用");
            m_listCtrl.SetItemText(nSelectedIndex,7,pParamData->TurnOn?"启用":"停用");
        }
    }
}

void CNationalInstPromptDlg::OnBnClickedButtonDelete()
{
    // TODO: Add your control notification handler code here
    int nSelectedIndex = GetListSelectedIndex();
    if(nSelectedIndex<0)
        return;

    if(IDCANCEL == ::MessageBox(NULL, "是否确认删除?", "警告", MB_OKCANCEL|MB_ICONWARNING|MB_SYSTEMMODAL))
        return;


    map<int,CParamData*>::iterator it = m_mapIndex2Data.find(nSelectedIndex);
    if(it!=m_mapIndex2Data.end())
    {
        CParamData* pData = it->second;
        if(pData)
        {
            delete pData;
            pData = NULL;
        }

        m_mapIndex2Data.erase(it);
    }

    m_listCtrl.DeleteItem(nSelectedIndex);

    //重新绑定
    map<int,CParamData*>::iterator it_id = m_mapIndex2Data.begin();
    for (int index=0;it_id!=m_mapIndex2Data.end();index++,it_id++)
    {
        int& key = const_cast<int&>(it_id->first);
        key = index;

        m_listCtrl.SetItemData(index,(DWORD_PTR)(it_id->second));
    }
}

void CNationalInstPromptDlg::OnBnClickedButtonApply()
{
    // TODO: Add your control notification handler code here
    int nSelectedIndex = GetListSelectedIndex();
    if(nSelectedIndex<0)
        return;

    CParamData* pParamData = (CParamData*)m_listCtrl.GetItemData(nSelectedIndex);

    pParamData->TurnOn = !pParamData->TurnOn;    
    pParamData->TurnOn?m_btnApplyCtrl.SetWindowText("停用"):m_btnApplyCtrl.SetWindowText("启用");
    m_listCtrl.SetItemText(nSelectedIndex,7,pParamData->TurnOn?"启用":"停用");
}

//读取ini文件的配置信息,设置到控件中去
bool CNationalInstPromptDlg::ReadData()
{
    char Node[128];
    char Buf[512];
    char PathFileName[512];

    //获取程序路径
    memset(PathFileName,0,sizeof(PathFileName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_NationalInstPrompt));
    strcat(PathFileName,IniFileName_NationalInstPrompt);

    /*  判断文件是否存在
        mode 模式，共5种模式：                               
        0-检查文件是否存在         
        1-检查文件是否可运行        
        2-检查文件是否可写访问    
        4-检查文件是否可读访问     
        6-检查文件是否可读/写访问 
    */

    if(-1==_access(PathFileName,0))
        return false;

    CIniFile *pIniFile=new CIniFile(PathFileName);

    int nY,nM,nD;

    int nCount=pIniFile->GetIniKeyVal("COUNT","Count",1);
    for (int i=0;i<nCount;i++)
    {
        int j=i+1;
        memset(Node,0,sizeof(Node));
        sprintf(Node,"%s_%d",Node_NationalInstPrompt,j);

        CParamData* pData = new CParamData;
        //策略输入项
        pIniFile->GetIniKeyStr(Node,"Param_BandCode","",Buf,sizeof(Buf));
        pData->Param_BandCode=Buf;
        pIniFile->GetIniKeyStr(Node,"Param_BandInstrument","",Buf,sizeof(Buf));
        pData->Param_BandInstrument=Buf;
        pData->Param_QuotPrice=pIniFile->GetIniKeyVal2(Node,"Param_QuotPrice",0.0f);
        pData->Param_IRR=pIniFile->GetIniKeyVal2(Node,"Param_IRR",0.0f);
        pData->Param_InterestRates=pIniFile->GetIniKeyVal2(Node,"Param_InterestRates",0.0f);
        pData->Param_BandPrice=pIniFile->GetIniKeyVal2(Node,"Param_BandPrice",0.0f);
        pData->Param_TransferFactor=pIniFile->GetIniKeyVal2(Node,"Param_TransferFactor",0.0f);
        pData->Param_YearInterestCount=pIniFile->GetIniKeyVal(Node,"Param_YearInterestCount",1);
        pIniFile->GetIniKeyStr(Node,"Param_PreDelivDate","",Buf,sizeof(Buf));
        pData->Param_PreDelivDateText=Buf;
        sscanf(Buf, "%d-%d-%d",&nY,&nM,&nD);
        pData->Param_PreDelivDate = CTime(nY,nM,nD,0,0,0);
        pIniFile->GetIniKeyStr(Node,"Param_NextDelivDate","",Buf,sizeof(Buf));
        pData->Param_NextDelivDateText=Buf;
        sscanf(Buf, "%d-%d-%d",&nY,&nM,&nD);
        pData->Param_NextDelivDate = CTime(nY,nM,nD,0,0,0);
        pIniFile->GetIniKeyStr(Node,"Param_DelivDate","",Buf,sizeof(Buf));
        pData->Param_DelivDateText=Buf;
        sscanf(Buf, "%d-%d-%d",&nY,&nM,&nD);
        pData->Param_DelivDate = CTime(nY,nM,nD,0,0,0);

        //触发输入项
        pData->Order_Alarm=pIniFile->GetIniKeyVal(Node,"Order_Alarm",0)==0?false:true;
        pData->Order_AutoOrder=pIniFile->GetIniKeyVal(Node,"Order_AutoOrder",0)==0?false:true;
        pIniFile->GetIniKeyStr(Node,"Order_Instrument","",Buf,sizeof(Buf));
        pData->Order_Instrument=Buf;
        pData->Order_Direction=pIniFile->GetIniKeyVal(Node,"Order_Direction",1);
        pData->Order_OffsetID=pIniFile->GetIniKeyVal(Node,"Order_OffsetID",0);
        pData->Order_Count=pIniFile->GetIniKeyVal(Node,"Order_Count",0);
        pData->Order_Price=pIniFile->GetIniKeyVal(Node,"Order_Price",0);

        m_mapIndex2Data.insert(make_pair(i,pData));
    }

    if(pIniFile)
    {
        delete pIniFile;
        pIniFile=NULL;
    }

    return true;
}

//保存配置信息到ini文件
void CNationalInstPromptDlg::SaveData(void)
{
    int nCount = m_mapIndex2Data.size();
    if(0==nCount)
        return;

    char Node[40];
    char PathFileName[512];

    //获取程序路径
    memset(PathFileName,0,sizeof(PathFileName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_NationalInstPrompt));
    strcat(PathFileName,IniFileName_NationalInstPrompt);
    CIniFile *pIniFile=new CIniFile(PathFileName);
    
    pIniFile->UpdateKeyVal("COUNT","Count",nCount);

    for (int i=0;i<nCount;i++)
    {
        int j=i+1;
        memset(Node,0,sizeof(Node));
        sprintf(Node,"%s_%d",Node_NationalInstPrompt,j);

        CParamData* pData = m_mapIndex2Data[i];

        //策略输入项             
        pIniFile->UpdateKeyStr(Node,"Param_BandCode",pData->Param_BandCode.c_str());
        pIniFile->UpdateKeyStr(Node,"Param_BandInstrument",pData->Param_BandInstrument.c_str());
        pIniFile->UpdateKeyVal2(Node,"Param_QuotPrice",pData->Param_QuotPrice);
        pIniFile->UpdateKeyVal2(Node,"Param_IRR",pData->Param_IRR);
        pIniFile->UpdateKeyVal2(Node,"Param_InterestRates",pData->Param_InterestRates);
        pIniFile->UpdateKeyVal2(Node,"Param_BandPrice",pData->Param_BandPrice);
        pIniFile->UpdateKeyVal2(Node,"Param_TransferFactor",pData->Param_TransferFactor);
        pIniFile->UpdateKeyVal(Node,"Param_YearInterestCount",pData->Param_YearInterestCount);
        pIniFile->UpdateKeyStr(Node,"Param_PreDelivDate",pData->Param_PreDelivDate.Format("%Y-%m-%d"));
        pIniFile->UpdateKeyStr(Node,"Param_NextDelivDate",pData->Param_NextDelivDate.Format("%Y-%m-%d"));
        pIniFile->UpdateKeyStr(Node,"Param_DelivDate",pData->Param_DelivDate.Format("%Y-%m-%d"));


        //触发输入项
        pIniFile->UpdateKeyVal(Node,"Order_Alarm",(int)pData->Order_Alarm);
        pIniFile->UpdateKeyVal(Node,"Order_AutoOrder",(int)pData->Order_AutoOrder);  
        pIniFile->UpdateKeyStr(Node,"Order_Instrument",pData->Order_Instrument.c_str());
        pIniFile->UpdateKeyVal(Node,"Order_Direction",(int)pData->Order_Direction);
        pIniFile->UpdateKeyVal(Node,"Order_OffsetID",(int)pData->Order_OffsetID);
        pIniFile->UpdateKeyVal(Node,"Order_Count",pData->Order_Count);
        pIniFile->UpdateKeyVal2(Node,"Order_Price",pData->Order_Price);
    }

    if(pIniFile)
    {
        delete pIniFile;
        pIniFile=NULL;
    }
}

void CNationalInstPromptDlg::SetDataToListCtrl()
{
    for(int index=0;index<(int)m_mapIndex2Data.size();index++)
    {
        CParamData* pData = m_mapIndex2Data[index];

        m_listCtrl.InsertItem(index,pData->Param_BandCode.c_str());
        m_listCtrl.SetItemText(index,1,pData->Param_BandInstrument.c_str());
        m_listCtrl.SetItemText(index,2,CTools::StringFromDouble(pData->Param_QuotPrice).c_str());
        m_listCtrl.SetItemText(index,3,CTools::StringFromDouble(pData->Result_BandTargetPrice).c_str());
        m_listCtrl.SetItemText(index,4,CTools::StringFromDouble(pData->Param_BandPrice).c_str());
        m_listCtrl.SetItemText(index,5,pData->Order_Alarm?"启用":"停用");
        m_listCtrl.SetItemText(index,6,pData->Order_AutoOrder?"启用":"停用");
        m_listCtrl.SetItemText(index,7,pData->TurnOn?"启用":"停用");

        //绑定
        m_listCtrl.SetItemData(index,(DWORD_PTR)pData);
    }
}

void CNationalInstPromptDlg::SubscribeInstrument()
{
    if(g_hWnd_FT!=NULL)
    {
        PlusinAPI* pAPI=PlusinAPI::GetInstance();
        if(pAPI)
        {
            for(int index=0;index<(int)m_mapIndex2Data.size();index++)
            {
                CParamData* pData = m_mapIndex2Data[index];

                string szInstNameUpper(pData->Param_BandInstrument);
                transform(szInstNameUpper.begin(), szInstNameUpper.end(), szInstNameUpper.begin(), (int (*)(int))toupper); 

                InstrumentIDType InstName;
                memset(InstName,0,sizeof(InstName));
                strncpy(InstName,szInstNameUpper.c_str(),sizeof(InstName)-1);


                pAPI->Send(CMDID_QUOTE_SUBSCRIBE,InstName,sizeof(InstName));
            }
        }
    }
}

// void CNationalInstPromptDlg::StartThread()
// {
//     //创建线程 每个线程对应一个item    
//     map<int,CParamData*>::iterator it = m_mapIndex2Data.begin();
//     for (;it!=m_mapIndex2Data.end();it++)
//     {
//         CParamData* pData = it->second;
// 
//         stru_data data;
//         data.pDlg = this;
//         data.pData = pData;
// 
// 
//         //创建线程   
//         DWORD code;
//         HANDLE hThread = NULL;
//         if(!GetExitCodeThread(hThread,&code)||(code!=STILL_ACTIVE)) 
//         {
//             hThread = CreateThread(NULL,0,ThreadFunc,&data,0,NULL);   
//             m_vecThreadHandle.push_back(hThread);
//         }
//     }
// }

void CNationalInstPromptDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here    
    if(pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
    {
        int nSelectedIndex = pNMLV->iItem;    
        
        CParamData* pParamData = (CParamData*)m_listCtrl.GetItemData(nSelectedIndex);
        pParamData->TurnOn?m_btnApplyCtrl.SetWindowText("停用"):m_btnApplyCtrl.SetWindowText("启用");
    }

    *pResult = 0;
}

int CNationalInstPromptDlg::GetListSelectedIndex()
{
    POSITION pos = m_listCtrl.GetFirstSelectedItemPosition(); 
    return m_listCtrl.GetNextSelectedItem(pos);
}

LRESULT CNationalInstPromptDlg::OnChildClose(WPARAM wParam, LPARAM lParam)
{
    m_bChildOpened = false;
    return 0;
}

void CNationalInstPromptDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int temp = 0;
    if(pNMUpDown->iDelta == 1)        // 如果此值为1 , 说明点击了Spin的往下箭头
        temp -= 1;
    else if(pNMUpDown->iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往上箭头
        temp += 1;

    int nSelectedIndex = GetListSelectedIndex();
    if(nSelectedIndex<0)
        return;

    CParamData* pParamData = (CParamData*)m_listCtrl.GetItemData(nSelectedIndex);
    pParamData->SetBandPrice(temp);

    *pResult = 0;
}
