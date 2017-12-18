#include "StdAfx.h"
#include "UserLoginDlg.h"
#include "wx/file.h"
#include "wx/textfile.h"
#include <wx/settings.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include "../inc/Module-Misc2/GlobalConfigMgr.h"
#include "../inc/Module-Misc2/MutexLock.h"
#include "myxml.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
//int UserLoginDlg::m_sReady=TIMER_QRYSETTLEMENTINFO;
//int UserLoginDlg::m_Step=TIMER_QRYSETTLEMENTINFO;

bool UserLoginDlg::m_bInitQry=false;
int UserLoginDlg::m_InitQryStep=-1;

bool UserLoginDlg::m_bRspQrySettlementInfo=false;
bool UserLoginDlg::m_bRspQryInstrument=false;
bool UserLoginDlg::m_bRspQryTradingAccount=false;
bool UserLoginDlg::m_bRspQryTrade=false;
bool UserLoginDlg::m_bRspQryOrder=false;
bool UserLoginDlg::m_bRspQryInvestorPositionDetail=false;
bool UserLoginDlg::m_bRspQryInvestorPosition=false;
bool UserLoginDlg::m_bRspQryInvestorPositionCombDetail=false;

#define TOP_ELEMENT_EQU(text,tag)\
if(strcmp(pFieldElement->Value(),#text)==0&&pFieldElement->GetText())\
{\
	if(DEFAULT_SVR()) \
		DEFAULT_SVR()->GetServerParamCfg().tag=pFieldElement->GetText();\
}
#define TOP_ELEMENT_SAVE(text,tag)\
if(DEFAULT_SVR())\
{\
	TiXmlElement *pElement = new TiXmlElement(#text);\
	TiXmlText *pContent = new TiXmlText(DEFAULT_SVR()->GetServerParamCfg().tag.c_str());\
	pElement->LinkEndChild(pContent);\
	RootElement->LinkEndChild(pElement);\
}
MutexLock m_MutexLock;
void UserLoginDlg::OnTimer(wxTimerEvent& event)
{
//    SetProgressPosition(TIMER_FINISH,m_Step+1);   
    SetProgressPosition(TIMER_FINISH,m_InitQryStep+1);   
    ServiceStatus ss=DEFAULT_SVR()?DEFAULT_SVR()->GetCurrentStatus():SS_NOLOGIN;
	switch (ss)
	{
    case SS_NOLOGIN:
    case SS_LOGINFAILED:
		{
			DoLoginFail();
			break;
		}

	case SS_LOGINING:
		m_cnt++;
		if(m_cnt>=TIMER_OVERTIME*500/TIMER_MILLISECONDS)
		{
            m_ErrorMessage=LOADSTRING(USERLOGINDLG_LOGINTIMEOUT);
			DoLoginFail();
		}
		break;
	case SS_LOGINED:
        if(!DEFAULT_SVR()||!(DEFAULT_SVR()->IsDataCenterReady()))
            break;

        if(!m_bInitQry)
        {
            //启动初始化查询
            StartLoginQuery();
        }
        else
        {
            if(m_bRspQrySettlementInfo&&
                m_bRspQryInstrument&&
                m_bRspQryTradingAccount&&
                m_bRspQryTrade&&
                m_bRspQryOrder&&
                m_bRspQryInvestorPositionDetail&&
                m_bRspQryInvestorPosition&&
                m_bRspQryInvestorPositionCombDetail)
            {
                //全部初始化都已经完成
                EndLoginQuery();
            }
            else
            {
                int LastQryStep=m_InitQryStep;
                    
                //检查当前查询步骤
                if(!m_bRspQrySettlementInfo) m_InitQryStep=TIMER_QRYSETTLEMENTINFO;
                else if(!m_bRspQryInstrument) m_InitQryStep=TIMER_QRYINSTRUMENT; 
                else if(!m_bRspQryTradingAccount) m_InitQryStep=TIMER_QRYTRADINGACCOUNT;
                else if(!m_bRspQryTrade) m_InitQryStep=TIMER_QRYTRADE;
                else if(!m_bRspQryOrder) m_InitQryStep=TIMER_QRYORDER;
                else if(!m_bRspQryInvestorPositionDetail) m_InitQryStep=TIMER_QRYINVESTORPOSITIONDETAIL;
                else if(!m_bRspQryInvestorPosition) m_InitQryStep=TIMER_QRYINVESTORPOSITION;
                else if(!m_bRspQryInvestorPositionCombDetail) m_InitQryStep=TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL;
                else m_InitQryStep=TIMER_FINISH;

                //设置查询提示信息
                if(LastQryStep!=m_InitQryStep)
                {
                    if(m_InitQryStep==TIMER_QRYSETTLEMENTINFO)                      SetPromptText(LOADSTRING(USERLOGINDLG_QRYSTL));
                    else if(m_InitQryStep==TIMER_QRYINSTRUMENT)                     SetPromptText(LOADSTRING(USERLOGINDLG_QRYINST));
                    //else if(m_InitQryStep==TIMER_QRYDEPTHMARKETDATA)                SetPromptText(LOADSTRING(USERLOGINDLG_QRYDEPTHMARKETDATA));
                    else if(m_InitQryStep==TIMER_QRYTRADINGACCOUNT)                 SetPromptText(LOADSTRING(USERLOGINDLG_QRYFUND));
                    else if(m_InitQryStep==TIMER_QRYTRADE)                          SetPromptText(LOADSTRING(USERLOGINDLG_QRYTRADER));
                    else if(m_InitQryStep==TIMER_QRYORDER)                          SetPromptText(LOADSTRING(USERLOGINDLG_QRYORDER));
                    else if(m_InitQryStep==TIMER_QRYINVESTORPOSITIONDETAIL)         SetPromptText(LOADSTRING(USERLOGINDLG_QRYPOSDETA));
                    else if(m_InitQryStep==TIMER_QRYINVESTORPOSITION)               SetPromptText(LOADSTRING(USERLOGINDLG_QRYPOS));
                    else if(m_InitQryStep==TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL)  SetPromptText(LOADSTRING(USERLOGINDLG_QRYPOSCOMB));
                    else SetPromptText();
                }
            }
        }

        //DoLoginStep();
		break;
	default:
		break;
	}
}

void UserLoginDlg::PreLoginQuery(void)
{
    if(!DEFAULT_SVR()) return;


    //1. 初始化查询结果
    m_bRspQrySettlementInfo=false;
    m_bRspQryInstrument=false;
    m_bRspQryTradingAccount=false;
    m_bRspQryTrade=false;
    m_bRspQryOrder=false;
    m_bRspQryInvestorPositionDetail=false;
    m_bRspQryInvestorPosition=false;
    m_bRspQryInvestorPositionCombDetail=false;

    //2. 订阅查询结果消息
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQrySettlementInfo, GID_LOGIN_DLG, QrySettlementInfoCallBackFunc);
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryProduct, GID_LOGIN_DLG, QryProductListCallBackFunc);
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInstrument, GID_LOGIN_DLG, QryInstrumentListCallBackFunc);
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTradingAccount, GID_LOGIN_DLG, QryTradingAccountCallBackFunc);
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryTrade, GID_LOGIN_DLG, QryTradeCallBackFunc);
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryOrder, GID_LOGIN_DLG, QryOrderCallBackFunc);
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_LOGIN_DLG, QryInvestorPositionDetailCallBackFunc);
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInvestorPosition, GID_LOGIN_DLG, QryInvestorPositionCallBackFunc);
    DEFAULT_SVR()->SubscribeBusinessData(BID_RspQryInvestorPositionCombDetail, GID_LOGIN_DLG, QryInvestorPositionCombineDetailCallBackFunc);

 //   //4 尝试加载本地品种和合约信息。先品种后合约
	//if(LoadTodayProducts()&&LoadTodayInstruments()) 
 //   {
 //       m_bRspQryInstrument=true;

 //       // 添加生成容错合约列表
	//	map<string, string> mapConstractIDVariety;
	//	MakeConstractIDVarietyMap(mapConstractIDVariety);
	//	DEFAULT_SVR()->SetInstrumentVarietyMap(mapConstractIDVariety);
 //   }

 //   //4.1 尝试加载本地保存的当天的保证金率和手续费率
	//if(DEFAULT_SVR()->HaveFileCache())
	//{
	//	LoadCommissionRateAndMarginRate("cam.xml");
	//}

    //5. 通知底层，开始初始化查询，期间不要查费率
    DEFAULT_SVR()->NotifyInitQryStart();


}

//开始登录过程的查询
//1. 通知底层，开始初始化查询，期间不要查费率
//2. 订阅查询结果消息
//3. 将所有查询请求提交给底层QryQueue123
void UserLoginDlg::StartLoginQuery(void)
{
    if(!DEFAULT_SVR()) return;

    PreLoginQuery();

    m_bInitQry=true;

    //4. 提交查询请求

    //4.1 结算信息查询
    if(DEFAULT_SVR()->IsSettlementInfoConfirmed()) 
        m_bRspQrySettlementInfo=true;
    else
    {
		PlatformStru_QrySettlementInfo tmp;
		DEFAULT_SVR()->ReqQrySettlementInfo(tmp);
    }

    //4.2 品种及合约信息查询。先查品种后查合约
	if(!m_bRspQryInstrument) 
    {
        vector<string> ProductIDs;
        vector<string> InstrumentIDs;

        DEFAULT_SVR()->GetProductList(ProductIDs);
        DEFAULT_SVR()->GetInstrumentList(InstrumentIDs);

        if(ProductIDs.size()==0)
        {
            PlatformStru_QryProduct QryProduct;
            DEFAULT_SVR()->ReqQryProduct(QryProduct);
        }

        if(InstrumentIDs.size()==0)
        {
            PlatformStru_QryInstrument QryInstrument;
            DEFAULT_SVR()->ReqQryInstrument(QryInstrument);
        }
        else
        {
             m_bRspQryInstrument=true;

            // 添加生成容错合约列表
		    map<string, string> mapConstractIDVariety;
		    MakeConstractIDVarietyMap(mapConstractIDVariety);
		    DEFAULT_SVR()->SetInstrumentVarietyMap(mapConstractIDVariety);
       }
    }

 //   //4.3 加载本地保存的当天的保证金率和手续费率
	//if(DEFAULT_SVR()->HaveFileCache())
	//{
	//	LoadCommissionRateAndMarginRate("cam.xml");
	//}

    //4.4 资金信息查询
    {
	    PlatformStru_QryTradingAccount QryTradingAccount;
        DEFAULT_SVR()->ReqQryTradingAccount(QryTradingAccount);
    }

    //4.5 成交查询
    {
	    PlatformStru_QryTrade QryTrade;
        DEFAULT_SVR()->ReqQryTrade(QryTrade);
    }

    //4.6 报单查询
    {
        PlatformStru_QryOrder tmp;
        DEFAULT_SVR()->ReqQryOrder(tmp);
    }

    //4.7 持仓明细查询
    {
	    PlatformStru_QryInvestorPositionDetail QryInvestorPositionDetail;
        DEFAULT_SVR()->ReqQryInvestorPositionDetail(QryInvestorPositionDetail);
    }

    //4.8 持仓查询
    {
	    PlatformStru_QryInvestorPosition QryInvestorPosition;
        DEFAULT_SVR()->ReqQryInvestorPosition(QryInvestorPosition);
    }

    //4.9 组合持仓明细查询
//#ifndef _USE_COMBOINSTRUMENT
    if(!GlobalConfigManager::m_bUseComb)
        m_bRspQryInvestorPositionCombDetail=true;
//#else
    else
    {
        PlatformStru_QryInvestorPositionCombineDetail QryInvestorPositionCombineDetail;
        DEFAULT_SVR()->ReqQryInvestorPositionCombineDetail(QryInvestorPositionCombineDetail);
    }
//#endif

    ///如果设置了成交后重查，则最后再查一遍资金，保证资金不受持仓影响
    if(GlobalConfigManager::IsReq_OnRtnTrade())
    {
	    PlatformStru_QryTradingAccount QryTradingAccount;
        DEFAULT_SVR()->ReqQryTradingAccount(QryTradingAccount);
    }

    //4.10 银期查询
    {
		PlatformStru_QryContractBank  QryContractBank;
		DEFAULT_SVR()&&DEFAULT_SVR()->ReqQryContractBank(QryContractBank);
    }

    ////4.11 查询汇率
    //{
    //    PlatformStru_QryExchangeRate QryExchangeRate;
    //    DEFAULT_SVR()&&DEFAULT_SVR()->ReqQryExchangeRate(QryExchangeRate);
    //}

    //5. 设置当前查询步骤
    m_InitQryStep=-1;

    //6. 清空提示信息
    SetPromptText();
}

//结束初始化查询
void UserLoginDlg::EndLoginQuery(void)
{
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQrySettlementInfo, GID_LOGIN_DLG);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInstrument, GID_LOGIN_DLG);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTradingAccount, GID_LOGIN_DLG);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryTrade, GID_LOGIN_DLG);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInvestorPositionDetail, GID_LOGIN_DLG);
    if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInvestorPositionCombDetail, GID_LOGIN_DLG);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryInvestorPosition, GID_LOGIN_DLG);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspQryOrder, GID_LOGIN_DLG);

    m_timer.Stop();
	wxDialog::EndDialog(wxID_OK);

    if(DEFAULT_SVR()) 
    {
        DEFAULT_SVR()->NotifyInitQrySucc();      //通知底层，初始化查询结束，可以查费率了
        g_pPlatformMgr->NotifyInitQrySucc(DEFAULT_SVR());
    }
    m_bInitQry=false;
}


int UserLoginDlg::QrySettlementInfoCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQrySettlementInfo || data.Size!=sizeof(DataRspQrySettlementInfo))
		return 0;	
	DataRspQrySettlementInfo& refData = *(DataRspQrySettlementInfo*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
        m_bRspQrySettlementInfo=true;
		//m_sReady=TIMER_QRYSETTLEMENTINFO+1;
  //      m_Step=m_sReady;
		return 0;
	}
	else if(refData.RspInfoField.ErrorID!=0)
	{
        if(m_pInst) m_pInst->DoLoginFail();
		//m_sReady=TIMER_ERROR;
		return -1;
	}

	if(refData.bIsLast)
	{
        m_bRspQrySettlementInfo=true;
		//m_sReady=TIMER_QRYSETTLEMENTINFO+1;
  //      m_Step=m_sReady;
	}

	return 0;
}
int UserLoginDlg::QryProductListCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryProduct || data.Size!=sizeof(DataRspQryProduct))
		return 0;	
	//DataRspQryProduct& refData = *(DataRspQryProduct*)(const_cast<AbstractBusinessData*>(&data));	
	//if(refData.bIsLast)
	//{
 //       if(m_pInst) m_pInst->SaveTodayProducts();
	//}
	return 0;
}
int UserLoginDlg::QryInstrumentListCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryInstrument || data.Size!=sizeof(DataRspQryInstrument))
		return 0;	
	DataRspQryInstrument& refData = *(DataRspQryInstrument*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录,几乎不可能发生
	{
        m_bRspQryInstrument=true;
		//m_sReady=TIMER_QRYINSTRUMENT+1;
  //      m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
        if(m_pInst) m_pInst->DoLoginFail();
		//m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
        m_bRspQryInstrument=true;
		{
		    // 添加生成容错合约列表
			map<string, string> mapConstractIDVariety;
			MakeConstractIDVarietyMap(mapConstractIDVariety);
			DEFAULT_SVR()->SetInstrumentVarietyMap(mapConstractIDVariety);
		}
		//m_sReady=TIMER_QRYINSTRUMENT+1;
  //      m_Step=m_sReady;
		//if(m_pInst) m_pInst->SaveTodayInstruments();
	}
	return 0;
}
int UserLoginDlg::QryTradingAccountCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryTradingAccount || data.Size!=sizeof(DataRspQryTradingAccount))
		return 0;	
	DataRspQryTradingAccount& refData = *(DataRspQryTradingAccount*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
        m_bRspQryTradingAccount=true;
		//m_sReady=TIMER_QRYTRADINGACCOUNT+1;
  //      m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
        if(m_pInst) m_pInst->DoLoginFail();
		//m_sReady=TIMER_ERROR;
		return -1;
	}

	if(refData.bIsLast)
	{
        m_bRspQryTradingAccount=true;
		//m_sReady=TIMER_QRYTRADINGACCOUNT+1;
  //      m_Step=m_sReady;
	}
	return 0;
}
int UserLoginDlg::QryTradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryTrade || data.Size!=sizeof(DataRspQryTrade))
		return 0;	
	DataRspQryTrade& refData = *(DataRspQryTrade*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
        m_bRspQryTrade=true;
		//m_sReady=TIMER_QRYTRADE+1;
  //      m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
        if(m_pInst) m_pInst->DoLoginFail();
		//m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
        m_bRspQryTrade=true;
		//m_sReady=TIMER_QRYTRADE+1;
  //      m_Step=m_sReady;
	}
	return 0;
}
int UserLoginDlg::QryInvestorPositionDetailCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryInvestorPositionDetail || data.Size!=sizeof(DataRspQryInvestorPositionDetail))
		return 0;	
	DataRspQryInvestorPositionDetail& refData = *(DataRspQryInvestorPositionDetail*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
        m_bRspQryInvestorPositionDetail=true;
		//m_sReady=TIMER_QRYINVESTORPOSITIONDETAIL+1;
  //      m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
        if(m_pInst) m_pInst->DoLoginFail();
		//m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
        m_bRspQryInvestorPositionDetail=true;
		//m_sReady=TIMER_QRYINVESTORPOSITIONDETAIL+1;
  //      m_Step=m_sReady;
	}
	return 0;
}
int UserLoginDlg::QryInvestorPositionCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryInvestorPosition || data.Size!=sizeof(DataRspQryInvestorPosition))
		return 0;	
	DataRspQryInvestorPosition& refData = *(DataRspQryInvestorPosition*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
        m_bRspQryInvestorPosition=true;
		//m_sReady=TIMER_QRYINVESTORPOSITION+1;
  //      m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
        if(m_pInst) m_pInst->DoLoginFail();
		//m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
        m_bRspQryInvestorPosition=true;
		//m_sReady=TIMER_QRYINVESTORPOSITION+1;
  //      m_Step=m_sReady;
	}
	return 0;
}

int UserLoginDlg::QryInvestorPositionCombineDetailCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryInvestorPositionCombDetail || data.Size!=sizeof(DataRspQryInvestorPositionCombDetail))
		return 0;	
	DataRspQryInvestorPositionCombDetail& refData = *(DataRspQryInvestorPositionCombDetail*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
        m_bRspQryInvestorPositionCombDetail=true;
		//m_sReady=TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL+1;
  //      m_Step=m_sReady;
		return 0;
	}
	if(refData.RspInfoField.ErrorID!=0)
	{
        if(m_pInst) m_pInst->DoLoginFail();
		//m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
        m_bRspQryInvestorPositionCombDetail=true;
		//m_sReady=TIMER_QRYINVESTORPOSITIONCOMBINEDETAIL+1;
  //      m_Step=m_sReady;
	}
	return 0;
}


int UserLoginDlg::QryOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	MUTEX_GURAD(m_MutexLock);
	if(data.BID!=BID_RspQryOrder || data.Size!=sizeof(DataRspQryOrder))
		return 0;	
	DataRspQryOrder& refData = *(DataRspQryOrder*)(const_cast<AbstractBusinessData*>(&data));	
	if(refData.RspInfoField.ErrorID==CustomErrorID)//查询无记录
	{
        m_bRspQryOrder=true;
		//m_sReady=TIMER_QRYORDER+1;
  //      m_Step=m_sReady;
		return 0;
	}
	else if(refData.RspInfoField.ErrorID!=0)
	{
        if(m_pInst) m_pInst->DoLoginFail();
		//m_sReady=TIMER_ERROR;
		return -1;
	}
	if(refData.bIsLast)
	{
        m_bRspQryOrder=true;
		//m_sReady=TIMER_QRYORDER+1;
  //      m_Step=m_sReady;
	}
	return 0;
}
bool UserLoginDlg::LoadSaveServerParam(bool bLoad)
{
    std::string strAppPath;
	bool bReturn=false;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+=SERVERPARAM_FILENAME;
    if(bLoad)
    {
        //初始化值
        TiXmlDocument myDocument(strAppPath.c_str()); 
        if(myDocument.LoadFile())
        {
            TiXmlElement* RootElement=myDocument.RootElement();
            if(RootElement==NULL) return false;
            int currentitem=0,languageid=0;
			RootElement->Attribute("current",&currentitem);
#ifdef _USE_MULTI_LANGUAGE
			RootElement->Attribute("language",&languageid);	
			if(languageid<LANGUAGE_MIN||languageid>LANGUAGE_MAX)
			{
				LCID lcid=GetSystemDefaultLCID();
				switch(lcid)
				{
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_TRADITIONAL):
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_HONGKONG):				
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_MACAU):
					languageid=LANGUAGE_TRADITION;
					break;
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED):
				case MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SINGAPORE):
					languageid=LANGUAGE_CHINESE;
					break;
				default:
					languageid=LANGUAGE_ENGLISH;
				}
			}
			CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
			if( pMgr != NULL) 
			{
				pMgr->SetLanguageId(languageid);
			}
#else
			languageid=LANGUAGE_CHINESE;
#endif
            if(DEFAULT_SVR()) DEFAULT_SVR()->GetServerParamCfg().CurSvrGrpID=currentitem;
			if(DEFAULT_SVR()) DEFAULT_SVR()->GetServerParamCfg().LanguageId=languageid;
            TiXmlElement* pFieldElement=RootElement->FirstChildElement();
            while(pFieldElement)
            {
				TOP_ELEMENT_EQU(UniServer,UniserverAddr)
				TOP_ELEMENT_EQU(VersionStr,VersionStr)
				TOP_ELEMENT_EQU(ProxyType,ProxyType)
				TOP_ELEMENT_EQU(ProxyAddr,ProxyAddr)
				TOP_ELEMENT_EQU(ProxyPort,ProxyPort)
				TOP_ELEMENT_EQU(ProxyUser,ProxyUser)
				TOP_ELEMENT_EQU(ProxyPwd,ProxyPwd)
				if(strcmp(pFieldElement->Value(),"Server")==0)
                {
                    //一个服务器
                    SERVER_PARAM_CFG_ITEM serveritem;
                    serveritem.CTPSimulate=0;
                    TiXmlElement* pItemElement=pFieldElement->FirstChildElement();
                    while(pItemElement)
                    {
                        if(strcmp(pItemElement->Value(),"Name")==0&&pItemElement->GetText())
                            serveritem.ServerName=pItemElement->GetText();
                        if(strcmp(pItemElement->Value(),"CTPSimulate")==0&&pItemElement->GetText())
                            serveritem.CTPSimulate=atoi(pItemElement->GetText());                        
                        if(strcmp(pItemElement->Value(),"BrokerID")==0&&pItemElement->GetText())
                            serveritem.BrokerID=pItemElement->GetText();
                        if(strcmp(pItemElement->Value(),"InvestorID")==0&&pItemElement->GetText())
                            serveritem.InvestorID=pItemElement->GetText();
                        if(strcmp(pItemElement->Value(),"Trading")==0)
                        {
                            TiXmlElement* pTradingItemElement=pItemElement->FirstChildElement();
                            while(pTradingItemElement)
                            {
                                serveritem.TradeAddr.push_back(pTradingItemElement->GetText());
                                pTradingItemElement=pTradingItemElement->NextSiblingElement();
                            }
                        }
                        if(strcmp(pItemElement->Value(),"MarketData")==0)
                        {
                            TiXmlElement* pMarketDataItemElement=pItemElement->FirstChildElement();
                            while(pMarketDataItemElement)
                            {
                                serveritem.QuotAddr.push_back(pMarketDataItemElement->GetText());
                                pMarketDataItemElement=pMarketDataItemElement->NextSiblingElement();
                            }
                        }
                        pItemElement=pItemElement->NextSiblingElement();
						
                    }
                    if(DEFAULT_SVR()) DEFAULT_SVR()->GetServerParamCfg().vec.push_back(serveritem);
					bReturn=true;
                }

                pFieldElement=pFieldElement->NextSiblingElement();
            }
        }
    }
    else if(DEFAULT_SVR())
    {
        TiXmlDocument myDocument;
        TiXmlElement *RootElement = new TiXmlElement("ServerParam");
        if(DEFAULT_SVR())
        {
            RootElement->SetAttribute("current",DEFAULT_SVR()->GetServerParamCfg().CurSvrGrpID);
		    RootElement->SetAttribute("language",DEFAULT_SVR()->GetServerParamCfg().LanguageId);
        }
        myDocument.LinkEndChild(RootElement);
		TOP_ELEMENT_SAVE(UniServer,UniserverAddr)
		TOP_ELEMENT_SAVE(VersionStr,VersionStr)
		TOP_ELEMENT_SAVE(ProxyType,ProxyType)
		TOP_ELEMENT_SAVE(ProxyAddr,ProxyAddr)
		TOP_ELEMENT_SAVE(ProxyPort,ProxyPort)
		TOP_ELEMENT_SAVE(ProxyUser,ProxyUser)
		TOP_ELEMENT_SAVE(ProxyPwd,ProxyPwd)
        for(int index=0;index<(int)DEFAULT_SVR()->GetServerParamCfg().vec.size();index++)
        {
            SERVER_PARAM_CFG_ITEM& cfgitem=DEFAULT_SVR()->GetServerParamCfg().vec[index];

            TiXmlElement *pServerElement = new TiXmlElement("Server");
            RootElement->LinkEndChild(pServerElement);
            {
                TiXmlElement *pElement = new TiXmlElement("Name");
                TiXmlText *pContent = new TiXmlText(cfgitem.ServerName.c_str());
                pElement->LinkEndChild(pContent);
                pServerElement->LinkEndChild(pElement);
            }
            {
                TiXmlElement *pElement = new TiXmlElement("CTPSimulate");
                wxString str;str.Printf("%d",cfgitem.CTPSimulate);
                TiXmlText *pContent = new TiXmlText(str.c_str());
                pElement->LinkEndChild(pContent);
                pServerElement->LinkEndChild(pElement);
            }
            {
                TiXmlElement *pElement = new TiXmlElement("BrokerID");
                TiXmlText *pContent = new TiXmlText(cfgitem.BrokerID.c_str());
                pElement->LinkEndChild(pContent);
                pServerElement->LinkEndChild(pElement);
            }
            {
                TiXmlElement *pElement = new TiXmlElement("InvestorID");
                TiXmlText *pContent = new TiXmlText(cfgitem.InvestorID.c_str());
                pElement->LinkEndChild(pContent);
                pServerElement->LinkEndChild(pElement);
            }
            {
                TiXmlElement *pTradingElement = new TiXmlElement("Trading");
                pServerElement->LinkEndChild(pTradingElement);
                for(int item=0;item<(int)cfgitem.TradeAddr.size();item++)
                {
                    TiXmlElement *pUnitElement = new TiXmlElement("item");
                    TiXmlText *pContent = new TiXmlText(cfgitem.TradeAddr[item].c_str());
                    pUnitElement->LinkEndChild(pContent);
                    pTradingElement->LinkEndChild(pUnitElement);
                }
            }
            {
                TiXmlElement *pMarketDataElement = new TiXmlElement("MarketData");
                pServerElement->LinkEndChild(pMarketDataElement);
                for(int item=0;item<(int)cfgitem.QuotAddr.size();item++)
                {
                    TiXmlElement *pUnitElement = new TiXmlElement("item");
                    TiXmlText *pContent = new TiXmlText(cfgitem.QuotAddr[item].c_str());
                    pUnitElement->LinkEndChild(pContent);
                    pMarketDataElement->LinkEndChild(pUnitElement);
                }
            }
        }
        myDocument.SaveFile(strAppPath.c_str());
    }
    return bReturn;
}
void UserLoginDlg::SplitwxString(wxString& strLine,std::vector<std::string>& vec)
{
    int startpos=0,endpos=0;
    wxString strTrueLine=strLine;
    if(strLine.Right(1)!=",")
        strTrueLine+=",";
    while(true)
    {
        endpos=strTrueLine.find(',',startpos);
        if(endpos<0)
        {
            break;
        }
        vec.push_back(strTrueLine.Mid(startpos,endpos-startpos).c_str());
        startpos=endpos+1;   
    }

}
//bool UserLoginDlg::LoadCommissionRate(TiXmlElement* pRoot)
//{
//    TiXmlElement* pFieldElement=pRoot->FirstChildElement();
//    PlatformStru_InstrumentCommissionRate field;
//    memset(&field,0,sizeof(field));
//    //获取BrokerID和InvestorID
//    field.InvestorRange=THOST_FTDC_IR_Single;
//	string strInvestorID,strBrokerID;
//	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
//    strcpy(field.BrokerID,strBrokerID.c_str());
//    strcpy(field.InvestorID,strInvestorID.c_str());
//
//    wxString tagname,tagvalue;
//    while(pFieldElement)
//    {
//        if(pFieldElement->GetText())
//            strcpy(field.InstrumentID,pFieldElement->GetText());
//        TiXmlAttribute* attributeName=pFieldElement->FirstAttribute();
//        while(attributeName)
//        {
//            tagname=attributeName->Name()?attributeName->Name():wxEmptyString;
//            if(tagname=="ORBM")       field.OpenRatioByMoney=attributeName->DoubleValue();
//            else if(tagname=="ORBV")  field.OpenRatioByVolume=attributeName->DoubleValue();
//            else if(tagname=="CRBM")  field.CloseRatioByMoney=attributeName->DoubleValue();
//            else if(tagname=="CRBV")  field.CloseRatioByVolume=attributeName->DoubleValue();
//            else if(tagname=="CTRBM")  field.CloseTodayRatioByMoney=attributeName->DoubleValue();
//            else if(tagname=="CTRBV")  field.CloseTodayRatioByVolume=attributeName->DoubleValue();
//            attributeName=attributeName->Next();
//        }
//        if(DEFAULT_SVR()) DEFAULT_SVR()->SetCommissionRate(field.InstrumentID,field);
//        pFieldElement=pFieldElement->NextSiblingElement();
//    }
//    return true;
//}
//bool UserLoginDlg::LoadMarginRate(TiXmlElement* pRoot)
//{
//    TiXmlElement* pFieldElement=pRoot->FirstChildElement();
//    PlatformStru_InstrumentMarginRate field;
//    memset(&field,0,sizeof(field));
//    //获取BrokerID和InvestorID
//	string strInvestorID="",strBrokerID="";
//	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
//    field.InvestorRange=THOST_FTDC_IR_Single;
//    strcpy(field.BrokerID,strBrokerID.c_str());
//    strcpy(field.InvestorID,strInvestorID.c_str());
//    wxString tagname,tagvalue;
//    while(pFieldElement)
//    {
//        if(pFieldElement->GetText())
//            strcpy(field.InstrumentID,pFieldElement->GetText());
//        TiXmlAttribute* attributeName=pFieldElement->FirstAttribute();
//        while(attributeName)
//        {
//            tagname=attributeName->Name()?attributeName->Name():wxEmptyString;
//            if(tagname=="LMBM")       field.LongMarginRatioByMoney=attributeName->DoubleValue();
//            else if(tagname=="LMBV")  field.LongMarginRatioByVolume=attributeName->DoubleValue();
//            else if(tagname=="SMBM")  field.ShortMarginRatioByMoney=attributeName->DoubleValue();
//            else if(tagname=="SMBV")  field.ShortMarginRatioByVolume=attributeName->DoubleValue();
//            attributeName=attributeName->Next();
//        }
//        if(DEFAULT_SVR()) DEFAULT_SVR()->SetMarginRate(field.InstrumentID,field);
//        pFieldElement=pFieldElement->NextSiblingElement();
//    }
//    return true;
//}

//bool UserLoginDlg::LoadCommissionRateAndMarginRate(wxString strFileName)
//{
//    //保证只运行一次
//    static bool bfirst=true;
//    if(bfirst)
//    {
//        bfirst=false;
//    }
//    else 
//        return false;
//    std::string apppath=strFileName.c_str();
//    GlobalFunc::GetPath(apppath);
//    TiXmlDocument* pDocument=new TiXmlDocument(apppath.c_str());
//    TiXmlDocument& myDocument=*pDocument; 
//    bool retval=false;
//	if(myDocument.LoadFile())
//	{
//        TiXmlElement* RootElement=myDocument.RootElement();
//        if(RootElement)
//        {
//            wxString strDate,strRoot=RootElement->Value()?RootElement->Value():wxEmptyString;
//            if(strRoot.CompareTo("Rate")==0)
//            {
//                const char* dateval=RootElement->Attribute("date");
//                const char* idval=RootElement->Attribute("id");
//                string strTradeDay="";
//                if(DEFAULT_SVR()) strTradeDay=DEFAULT_SVR()->GetTradingDay();
//                if(!strTradeDay.empty()&&dateval&&idval)
//                {
//                    string strInvestorID="",strBrokerID="";
//                    if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
//                    if(dateval==strTradeDay&&strInvestorID==idval)//当天，读入文件
//                    {
//                        TiXmlElement* pCommissionRateElement=RootElement->FirstChildElement();
//                        if(pCommissionRateElement)
//                        {
//                            if(LoadCommissionRate(pCommissionRateElement)) 
//                            {
//                                TiXmlElement* pMarginRateElement=pCommissionRateElement->NextSiblingElement();
//                                if(pMarginRateElement)
//                                {
//                                    retval=LoadMarginRate(pMarginRateElement);
//                                }
//                            }
//
//                        }         
//                    }
//                }
//            }
//        }
//	}
//	wxDELETE(pDocument);
//    return retval;
//}

BOOL UserLoginDlg::MakeConstractIDVarietyMap(map<string, string>& mapConstractIDVariety)
{
	
	set<string> InstrumentIDArray;
	InstrumentIDArray.clear();
	if(!DEFAULT_SVR()||DEFAULT_SVR()->GetInstrumentList(InstrumentIDArray)<=0)
		return FALSE;

	int nRet = 0;
	string strInstrumentID;
	wxString strNewID;
	string strExchangeID;
	int nCurrTenYear=0;
	int nCurrMonth=0;
	PlatformStru_InstrumentInfo tInstrumentField;
	SYSTEMTIME sysTime;

	GetLocalTime(&sysTime);
	nCurrTenYear = (sysTime.wYear%100)/10;
	nCurrMonth = sysTime.wMonth;

	// 生成郑商所4位日期及全小写容差合约
	for(set<string>::iterator it = InstrumentIDArray.begin(); 
			it != InstrumentIDArray.end(); ++it) {
		strInstrumentID = *it;
		///获取指定合约信息
        memset(&tInstrumentField,0,sizeof(tInstrumentField));
		nRet = DEFAULT_SVR()->GetInstrumentInfo(strInstrumentID, tInstrumentField);
		if(nRet == 0) {
			strExchangeID = tInstrumentField.ExchangeID;
			if(strExchangeID.compare("CZCE")==0) {
				int nPos = 0;
				BOOL bIsFound = FALSE;
				while(nPos < (int)strlen(tInstrumentField.InstrumentID)) {
					if(tInstrumentField.InstrumentID[nPos]>='0' 
							&& tInstrumentField.InstrumentID[nPos]<='9') {
						bIsFound = TRUE;
						break;
					}
					nPos++;
				}
				if(bIsFound) {
					int nMonth = 0;
					char strName[16];
					char strYear[16];
					char strMonth[16];
					ZeroMemory(strName, sizeof(strName));
					ZeroMemory(strYear, sizeof(strYear));
					ZeroMemory(strMonth, sizeof(strMonth));
					strncpy(strName, tInstrumentField.InstrumentID, nPos);
					strncpy(strYear+1, tInstrumentField.InstrumentID+nPos, 1);
					strncpy(strMonth, tInstrumentField.InstrumentID+nPos+1, 2);
					nMonth = atoi(strMonth);
					if(nCurrMonth < nMonth) {
						strYear[0] = nCurrTenYear+'0';
					}
					else {
						if(strYear[1]=='9') {
							strYear[0] = nCurrTenYear+1+'0';
						}
						else {
							strYear[0] = nCurrTenYear+'0';
						}
					}
					strNewID = strName;
					strNewID += strYear;
					strNewID += strMonth;
					//strNewID.UpperCase();
					//mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
					strNewID.LowerCase();
					mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
				}
			}
		}
	}
	// 生成全小写容差合约
	for(set<string>::iterator it = InstrumentIDArray.begin(); 
			it != InstrumentIDArray.end(); ++it) {
		strInstrumentID = *it;
		strNewID = strInstrumentID;
		//strNewID.UpperCase();
		//mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
		strNewID.LowerCase();
		mapConstractIDVariety[strNewID.c_str()] = strInstrumentID;
	}

	return TRUE;
}

//bool UserLoginDlg::LoadTodayInstruments()
//{
//	if((!DEFAULT_SVR())||(!DEFAULT_SVR()->HaveFileCache()))
//		return false;
//
//    CMyXML myxml("Instruments.xml");
//
//    //判断交易日和服务器组是否正确
//    if(!myxml.m_bvalid) return false;
//    if(myxml.GetNodeContent2(string(""),"root","TradingDay")!=DEFAULT_SVR()->GetTradingDay()) return false;
//    if(myxml.GetNodeContentInt2(-1,"root","ServerGrpNo")!=DEFAULT_SVR()->GetServerParamCfg().CurSvrGrpID) return false;
//    
//    //分离出列头名称
//    vector<string> vecColName=CTools_Ansi::SplitString(myxml.GetNodeContent2(string(""),"root","Instrumentcolname").c_str(),',');
//
//    multimap<string,CMyXML>::const_iterator itInstruments=myxml.m_mapsubnode.find(string("Instruments"));
//    if(itInstruments==myxml.m_mapsubnode.end()) return false;
//    if(itInstruments->second.m_mapsubnode.size()==0) return false;
//
//    //合约数量
//    int InstrumentCnt=itInstruments->second.m_mapsubnode.size();
//
//    //字段数量
//    int ColCnt=vecColName.size();
//
//    //这里使用malloc的方法申请内存，并自行管理内存，而不用stl来管理。
//    //原因是在debug模式下，如果合约数量在1400条左右，用stl会耗时5-6秒
//
//    //合约信息数组，合约原始文本指针数组，每条合约每个字段的位置和长度数组
//    PlatformStru_InstrumentInfo* pInstrumentInfos=(PlatformStru_InstrumentInfo*)malloc(sizeof(PlatformStru_InstrumentInfo)*InstrumentCnt);
//    char** pLineContent=(char**)malloc(sizeof(char*)*InstrumentCnt);
//    int *psubposs=(int*)malloc(sizeof(int)*InstrumentCnt*ColCnt);
//    int *psublens=(int*)malloc(sizeof(int)*InstrumentCnt*ColCnt);
//    memset(pInstrumentInfos,0,sizeof(PlatformStru_InstrumentInfo)*InstrumentCnt);
//    memset(pLineContent,0,sizeof(char*)*InstrumentCnt);
//    memset(psubposs,0,sizeof(int)*InstrumentCnt*ColCnt);
//    memset(psublens,0,sizeof(int)*InstrumentCnt*ColCnt);
//
//    //分离出每一行合约的每个字段的位置和长度，放在subposs和sublens里
//    multimap<string,CMyXML>::const_iterator it;
//    int line=0;
//    for(it=itInstruments->second.m_mapsubnode.begin();it!=itInstruments->second.m_mapsubnode.end();it++,line++)
//    {
//        if(it->second.m_content.empty()) break;
//
//        pLineContent[line]=(char*)malloc(it->second.m_content.size()+1);
//        memcpy(pLineContent[line],it->second.m_content.c_str(),it->second.m_content.size());
//        pLineContent[line][it->second.m_content.size()]=0;
//
//        int rltcolcnt;
//        if(!CTools_Ansi::SplitString(psubposs+ColCnt*line,psublens+ColCnt*line,rltcolcnt,ColCnt,pLineContent[line],','))
//            break;
//        if(rltcolcnt!=ColCnt) break;
//    }
//    if(it!=itInstruments->second.m_mapsubnode.end())
//    {
//        //数据格式不符，返回失败
//        free(pInstrumentInfos);
//        for(int i=0;i<InstrumentCnt;i++) if(pLineContent[i]!=NULL) free(pLineContent[i]);
//        free(pLineContent);
//        free(psubposs);
//        free(psublens);
//        return false;
//    }
//
//
////宏定义，取出所有合约的某一个字段值，放到InstrumentInfos中
//#define min(x,y) (x<=y?x:y)
//#define CopyInstrumentItem_String(colname,col) \
//    {\
//        for(line=0;line<InstrumentCnt;line++)\
//            memcpy(pInstrumentInfos[line].##colname,pLineContent[line]+psubposs[ColCnt*line+col],min(sizeof(pInstrumentInfos[line].##colname)-1,psublens[ColCnt*line+col]));\
//    }
//#define CopyInstrumentItem_Integer(colname,col)\
//    {\
//        for(line=0;line<InstrumentCnt;line++)\
//            pInstrumentInfos[line].##colname=CTools_Ansi::GetIntegerFromString(pLineContent[line]+psubposs[ColCnt*line+col]);\
//    }
//#define CopyInstrumentItem_char(colname,col)\
//    {\
//        for(line=0;line<InstrumentCnt;line++)\
//            pInstrumentInfos[line].##colname=(char)CTools_Ansi::GetIntegerFromString(pLineContent[line]+psubposs[ColCnt*line+col]);\
//    }
//#define CopyInstrumentItem_Double(colname,col)\
//    {\
//        for(line=0;line<InstrumentCnt;line++)\
//        {\
//            if(psublens[ColCnt*line+col]==16)\
//            {\
//                pInstrumentInfos[line].##colname=CTools_Ansi::GetDoubleFromHexString(pLineContent[line]+psubposs[ColCnt*line+col],psublens[ColCnt*line+col]);\
//            }\
//        }\
//    }
//
//    //对每一列，从每条合约原始数据中取出列值，放到对应合约信息中去
//    for(int i=0;i<(int)vecColName.size();i++)
//    {
//        if(strcmp(vecColName[i].c_str(),"InstrumentID")==0) CopyInstrumentItem_String(InstrumentID,i)
//        else if(strcmp(vecColName[i].c_str(),"ExchangeID")==0) CopyInstrumentItem_String(ExchangeID,i)
//        else if(strcmp(vecColName[i].c_str(),"InstrumentName")==0) CopyInstrumentItem_String(InstrumentName,i)
//        else if(strcmp(vecColName[i].c_str(),"ExchangeInstID")==0) CopyInstrumentItem_String(ExchangeInstID,i)
//        else if(strcmp(vecColName[i].c_str(),"ProductID")==0) CopyInstrumentItem_String(ProductID,i)
//
//        else if(strcmp(vecColName[i].c_str(),"ProductClass")==0) CopyInstrumentItem_char(ProductClass,i)
//        else if(strcmp(vecColName[i].c_str(),"DeliveryYear")==0) CopyInstrumentItem_Integer(DeliveryYear,i)
//        else if(strcmp(vecColName[i].c_str(),"DeliveryMonth")==0) CopyInstrumentItem_Integer(DeliveryMonth,i)
//        else if(strcmp(vecColName[i].c_str(),"MaxMarketOrderVolume")==0) CopyInstrumentItem_Integer(MaxMarketOrderVolume,i)
//        else if(strcmp(vecColName[i].c_str(),"MinMarketOrderVolume")==0) CopyInstrumentItem_Integer(MinMarketOrderVolume,i)
//        else if(strcmp(vecColName[i].c_str(),"MaxLimitOrderVolume")==0) CopyInstrumentItem_Integer(MaxLimitOrderVolume,i)
//        else if(strcmp(vecColName[i].c_str(),"MinLimitOrderVolume")==0) CopyInstrumentItem_Integer(MinLimitOrderVolume,i)
//        else if(strcmp(vecColName[i].c_str(),"VolumeMultiple")==0) CopyInstrumentItem_Integer(VolumeMultiple,i)
//
//        else if(strcmp(vecColName[i].c_str(),"PriceTick")==0) CopyInstrumentItem_Double(PriceTick,i)
//
//        else if(strcmp(vecColName[i].c_str(),"CreateDate")==0) CopyInstrumentItem_String(CreateDate,i)
//        else if(strcmp(vecColName[i].c_str(),"OpenDate")==0) CopyInstrumentItem_String(OpenDate,i)
//        else if(strcmp(vecColName[i].c_str(),"ExpireDate")==0) CopyInstrumentItem_String(ExpireDate,i)
//        else if(strcmp(vecColName[i].c_str(),"StartDelivDate")==0) CopyInstrumentItem_String(StartDelivDate,i)
//        else if(strcmp(vecColName[i].c_str(),"EndDelivDate")==0) CopyInstrumentItem_String(EndDelivDate,i)
//
//        else if(strcmp(vecColName[i].c_str(),"InstLifePhase")==0) CopyInstrumentItem_char(InstLifePhase,i)
//        else if(strcmp(vecColName[i].c_str(),"IsTrading")==0) CopyInstrumentItem_Integer(IsTrading,i)
//        else if(strcmp(vecColName[i].c_str(),"PositionType")==0) CopyInstrumentItem_char(PositionType,i)
//        else if(strcmp(vecColName[i].c_str(),"PositionDateType")==0) CopyInstrumentItem_char(PositionDateType,i)
//        else if(strcmp(vecColName[i].c_str(),"LongMarginRatio")==0) CopyInstrumentItem_Double(LongMarginRatio,i)
//        else if(strcmp(vecColName[i].c_str(),"ShortMarginRatio")==0) CopyInstrumentItem_Double(ShortMarginRatio,i)
//        else if(strcmp(vecColName[i].c_str(),"MaxMarginSideAlgorithm")==0) CopyInstrumentItem_char(MaxMarginSideAlgorithm,i)
//
//        else if(strcmp(vecColName[i].c_str(),"UnderlyingInstrID")==0) CopyInstrumentItem_String(UnderlyingInstrID,i)
//        else if(strcmp(vecColName[i].c_str(),"StrikePrice")==0) CopyInstrumentItem_Double(StrikePrice,i)
//        else if(strcmp(vecColName[i].c_str(),"OptionsType")==0) CopyInstrumentItem_char(OptionsType,i)
//        else if(strcmp(vecColName[i].c_str(),"UnderlyingMultiple")==0) CopyInstrumentItem_Double(UnderlyingMultiple,i)
//        else if(strcmp(vecColName[i].c_str(),"CombinationType")==0) CopyInstrumentItem_char(CombinationType,i)
//
//        else if(strcmp(vecColName[i].c_str(),"CombMarginDiscountMode")==0) CopyInstrumentItem_Integer(CombMarginDiscountMode,i)
//        else if(strcmp(vecColName[i].c_str(),"UnwindPriorities")==0) CopyInstrumentItem_Integer(UnwindPriorities,i)
//        else if(strcmp(vecColName[i].c_str(),"PriceForMarginOfTodayPosition")==0) CopyInstrumentItem_Integer(PriceForMarginOfTodayPosition,i)
//        else if(strcmp(vecColName[i].c_str(),"CloseTodayInstructionSupport")==0) CopyInstrumentItem_Integer(CloseTodayInstructionSupport,i)
//        else if(strcmp(vecColName[i].c_str(),"CloseInstructionSupport")==0) CopyInstrumentItem_Integer(CloseInstructionSupport,i)
//    }
//
//#undef CopyInstrumentItem_Double
//#undef CopyInstrumentItem_char
//#undef CopyInstrumentItem_Integer
//#undef CopyInstrumentItem_String
//#undef min
//
//
//    //将合约信息设置到底层
//    for(int i=0;i<InstrumentCnt;i++)
//        DEFAULT_SVR()->SetInstrumentInfo(pInstrumentInfos[i].InstrumentID,pInstrumentInfos[i],i==InstrumentCnt-1);
//
//    free(pInstrumentInfos);
//    for(int i=0;i<InstrumentCnt;i++) if(pLineContent[i]!=NULL) free(pLineContent[i]);
//    free(pLineContent);
//    free(psubposs);
//    free(psublens);
//
//
//    return true;
//}


//void UserLoginDlg::SaveTodayInstruments()
//{
//    set<string> InstrumentIDs;
//    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetInstrumentList(InstrumentIDs)>0)
//    {
//        CMyXML myxml(string("root"),string(""));
//
//        //存交易日
//        myxml.m_mapsubnode.insert(make_pair("TradingDay",CMyXML(string("TradingDay"),DEFAULT_SVR()->GetTradingDay())));
//
//        //存服务器组编号
//        char buf[256]={0};
//        itoa(DEFAULT_SVR()->GetServerParamCfg().CurSvrGrpID,buf,10);
//        myxml.m_mapsubnode.insert(make_pair("ServerGrpNo",CMyXML(string("ServerGrpNo"),string(buf))));
//
//        //合约组
//        CMyXML instrumentsxml("Instruments",string(""));
//
//
//        const int bufsize=65536;
//
//        char *pColName=(char*)malloc(bufsize);
//        memset(pColName,0,bufsize);
//        int ColNameLen=0,namelen;
//
//        char *pContent=(char*)malloc(bufsize);
//        int ContentLen,contlen;
//
//        bool bFirstLine=true;
//
//
//#define JoinContent(colname,content) \
//            contlen=strlen(content);\
//            if(ContentLen+contlen+1>=bufsize) break;\
//            \
//            memcpy(pContent+ContentLen,content,contlen);\
//            ContentLen+=contlen;\
//            pContent[ContentLen++]=',';\
//            pContent[ContentLen]=0;\
//            \
//            if(bFirstLine)\
//            {\
//                namelen=strlen(colname);\
//                if(ColNameLen+namelen+1>=bufsize) break;\
//                \
//                memcpy(pColName+ColNameLen,colname,namelen);\
//                ColNameLen+=namelen;\
//                pColName[ColNameLen++]=',';\
//                pColName[ColNameLen]=0;\
//            }
//#define JoinContent_String(colname) JoinContent(#colname,InstrumentInfo.##colname)        
//#define JoinContent_Integer(colname) { string strcontent=CTools_Ansi::GetStringFromInteger((int)InstrumentInfo.##colname);  const char* pcontent=strcontent.c_str();  JoinContent(#colname,pcontent) }
//#define JoinContent_Double(colname) { string strcontent=CTools_Ansi::GetHexStringFromDouble((double)InstrumentInfo.##colname);  const char* pcontent=strcontent.c_str();  JoinContent(#colname,pcontent) }
//
//
//        string strcontent;
//        //存合约内容
//        for(set<string>::const_iterator it=InstrumentIDs.begin();it!=InstrumentIDs.end();it++,bFirstLine=false)
//        {
//            PlatformStru_InstrumentInfo InstrumentInfo;
//            if(DEFAULT_SVR()->GetInstrumentInfo(it->c_str(), InstrumentInfo)!=0) continue;
//
//            memset(pContent,0,bufsize);
//            ContentLen=0;
//
//            JoinContent_String(InstrumentID)
//            JoinContent_String(ExchangeID)
//            JoinContent_String(InstrumentName)
//            JoinContent_String(ExchangeInstID)
//            JoinContent_String(ProductID)
//            
//            JoinContent_Integer(ProductClass)
//            JoinContent_Integer(DeliveryYear)
//            JoinContent_Integer(DeliveryMonth)
//            JoinContent_Integer(MaxMarketOrderVolume)
//            JoinContent_Integer(MinMarketOrderVolume)
//            JoinContent_Integer(MaxLimitOrderVolume)
//            JoinContent_Integer(MinLimitOrderVolume)
//            JoinContent_Integer(VolumeMultiple)
//
//            JoinContent_Double(PriceTick)
//
//            JoinContent_String(CreateDate)
//            JoinContent_String(OpenDate)
//            JoinContent_String(ExpireDate)
//            JoinContent_String(StartDelivDate)
//            JoinContent_String(EndDelivDate)
//
//            JoinContent_Integer(InstLifePhase)
//            JoinContent_Integer(IsTrading)
//            JoinContent_Integer(PositionType)
//            JoinContent_Integer(PositionDateType)
//            JoinContent_Double(LongMarginRatio)
//            JoinContent_Double(ShortMarginRatio)
//            JoinContent_Integer(MaxMarginSideAlgorithm)
//
//            JoinContent_String(UnderlyingInstrID)
//            JoinContent_Double(StrikePrice)
//            JoinContent_Integer(OptionsType)
//            JoinContent_Double(UnderlyingMultiple)
//            JoinContent_Integer(CombinationType)
//
//            JoinContent_Integer(CombMarginDiscountMode)
//            JoinContent_Integer(UnwindPriorities)
//            JoinContent_Integer(PriceForMarginOfTodayPosition)
//            JoinContent_Integer(CloseTodayInstructionSupport)
//            JoinContent_Integer(CloseInstructionSupport)
//
//            if(ContentLen>0) pContent[--ContentLen]=0;
//            CMyXML subxml("Instrument",string(pContent));
//            instrumentsxml.m_mapsubnode.insert(make_pair("Instrument",subxml));
//
//        }
//
//#undef JoinContent_Double
//#undef JoinContent_Integer
//#undef JoinContent_String
//#undef JoinContent
//
//        if(ColNameLen>0) pColName[--ColNameLen]=0;
//        CMyXML instrumentnamexml("Instrumentcolname",string(pColName));
//        myxml.m_mapsubnode.insert(make_pair("Instrumentcolname",instrumentnamexml));
//
//        myxml.m_mapsubnode.insert(make_pair("Instruments",instrumentsxml));
//
//        free(pContent);
//        free(pColName);
//
//        myxml.SaveXML("Instruments.xml");
//    }
//
//}

//bool UserLoginDlg::LoadTodayProducts()
//{
//	if((!DEFAULT_SVR())||(!DEFAULT_SVR()->HaveFileCache()))
//		return false;
//
//    CMyXML myxml("Products.xml");
//    if(!myxml.m_bvalid) return false;
//    if(myxml.GetNodeContent2(string(""),"root","TradingDay")!=DEFAULT_SVR()->GetTradingDay()) return false;
//    if(myxml.GetNodeContentInt2(-1,"root","ServerGrpNo")!=DEFAULT_SVR()->GetServerParamCfg().CurSvrGrpID) return false;
//    
//    multimap<string,CMyXML>::const_iterator itlower=myxml.m_mapsubnode.lower_bound(string("Product"));
//    multimap<string,CMyXML>::const_iterator itupper=myxml.m_mapsubnode.upper_bound(string("Product"));
//    for(;itlower!=itupper;itlower++)
//    {
//        if(!itlower->second.m_bvalid) continue;
//
//        PlatformStru_ProductInfo ProductInfo;
//        string strValue;
//
//        strncpy(ProductInfo.ProductID,itlower->second.m_content.c_str(),sizeof(ProductInfo.ProductID)-1);
//        for(multimap<string,string>::const_iterator it=itlower->second.m_mapattr.begin();it!=itlower->second.m_mapattr.end();it++)
//        {
//            if(it->first==string("ProductName"))                strncpy(ProductInfo.ProductName,it->second.c_str(),sizeof(ProductInfo.ProductName)-1);
//            else if(it->first==string("ExchangeID"))            strncpy(ProductInfo.ExchangeID,it->second.c_str(),sizeof(ProductInfo.ExchangeID)-1);
//            else if(it->first==string("ProductClass"))          ProductInfo.ProductClass=(char)CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("VolumeMultiple"))        ProductInfo.VolumeMultiple=CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("PriceTick"))             ProductInfo.PriceTick=CTools_Ansi::GetDoubleFromHexString(it->second);
//            else if(it->first==string("MaxMarketOrderVolume"))  ProductInfo.MaxMarketOrderVolume=CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("MinMarketOrderVolume"))  ProductInfo.MinMarketOrderVolume=CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("MaxLimitOrderVolume"))   ProductInfo.MaxLimitOrderVolume=CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("MinLimitOrderVolume"))   ProductInfo.MinLimitOrderVolume=CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("PositionType"))          ProductInfo.PositionType=(char)CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("PositionDateType"))      ProductInfo.PositionDateType=(char)CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("CloseDealType"))         ProductInfo.CloseDealType=(char)CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("TradeCurrencyID"))       strncpy(ProductInfo.TradeCurrencyID,it->second.c_str(),sizeof(ProductInfo.TradeCurrencyID)-1);
//            else if(it->first==string("MortgageFundUseRange"))  ProductInfo.MortgageFundUseRange=(char)CTools_Ansi::GetIntegerFromString(it->second);
//            else if(it->first==string("ExchangeProductID"))     strncpy(ProductInfo.ExchangeProductID,it->second.c_str(),sizeof(ProductInfo.ExchangeProductID)-1);
//            else if(it->first==string("UnderlyingMultiple"))    ProductInfo.UnderlyingMultiple=CTools_Ansi::GetDoubleFromHexString(it->second);
//        }
//        DEFAULT_SVR()->SetProductInfo(ProductInfo.ProductID,ProductInfo);
//    }
//    return true;
//}

//void UserLoginDlg::SaveTodayProducts()
//{
//    vector<string> ProductIDs;
//    if(DEFAULT_SVR()&&DEFAULT_SVR()->GetProductList(ProductIDs)>0)
//    {
//        CMyXML myxml(string("root"),string(""));
//
//        //存交易日
//        myxml.m_mapsubnode.insert(make_pair("TradingDay",CMyXML(string("TradingDay"),DEFAULT_SVR()->GetTradingDay())));
//
//        //存服务器组编号
//        char buf[256]={0};
//        itoa(DEFAULT_SVR()->GetServerParamCfg().CurSvrGrpID,buf,10);
//        myxml.m_mapsubnode.insert(make_pair("ServerGrpNo",CMyXML(string("ServerGrpNo"),string(buf))));
//
//        //存品种内容
//        for(vector<string>::const_iterator it=ProductIDs.begin();it!=ProductIDs.end();it++)
//        {
//            PlatformStru_ProductInfo ProductInfo;
//            if(!DEFAULT_SVR()->GetProductInfo2(it->c_str(), ProductInfo)) continue;
//
//            CMyXML subxml("Product",*it);
//            subxml.m_mapattr.insert(make_pair("ProductName",ProductInfo.ProductName));
//            subxml.m_mapattr.insert(make_pair("ExchangeID",ProductInfo.ExchangeID));
//            subxml.m_mapattr.insert(make_pair("ProductClass",CTools_Ansi::GetStringFromInteger((int)ProductInfo.ProductClass)));
//            subxml.m_mapattr.insert(make_pair("VolumeMultiple",CTools_Ansi::GetStringFromInteger((int)ProductInfo.VolumeMultiple)));
//            subxml.m_mapattr.insert(make_pair("PriceTick",CTools_Ansi::GetHexStringFromDouble(ProductInfo.PriceTick)));
//            subxml.m_mapattr.insert(make_pair("MaxMarketOrderVolume",CTools_Ansi::GetStringFromInteger((int)ProductInfo.MaxMarketOrderVolume)));
//            subxml.m_mapattr.insert(make_pair("MinMarketOrderVolume",CTools_Ansi::GetStringFromInteger((int)ProductInfo.MinMarketOrderVolume)));
//            subxml.m_mapattr.insert(make_pair("MaxLimitOrderVolume",CTools_Ansi::GetStringFromInteger((int)ProductInfo.MaxLimitOrderVolume)));
//            subxml.m_mapattr.insert(make_pair("MinLimitOrderVolume",CTools_Ansi::GetStringFromInteger((int)ProductInfo.MinLimitOrderVolume)));
//            subxml.m_mapattr.insert(make_pair("PositionType",CTools_Ansi::GetStringFromInteger((int)ProductInfo.PositionType)));
//            subxml.m_mapattr.insert(make_pair("PositionDateType",CTools_Ansi::GetStringFromInteger((int)ProductInfo.PositionDateType)));
//            subxml.m_mapattr.insert(make_pair("CloseDealType",CTools_Ansi::GetStringFromInteger((int)ProductInfo.CloseDealType)));
//            subxml.m_mapattr.insert(make_pair("TradeCurrencyID",ProductInfo.TradeCurrencyID));
//            subxml.m_mapattr.insert(make_pair("MortgageFundUseRange",CTools_Ansi::GetStringFromInteger((int)ProductInfo.MortgageFundUseRange)));
//            subxml.m_mapattr.insert(make_pair("ExchangeProductID",ProductInfo.ExchangeProductID));
//            subxml.m_mapattr.insert(make_pair("UnderlyingMultiple",CTools_Ansi::GetHexStringFromDouble(ProductInfo.UnderlyingMultiple)));
//
//            myxml.m_mapsubnode.insert(make_pair("Product",subxml));
//        }
//
//        myxml.SaveXML("Products.xml");
//    }
//
//}