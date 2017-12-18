#include "StdAfx.h"
#include "MainWindowCfgMgr.h"

MainWindowCfgMgr::MainWindowCfgMgr(void)
{
	memset(&m_MainWindowCfg, 0, sizeof(MAIN_WINDOW_CFG));

	m_MainWindowCfg.bIsTop.xmlset(false);					//是否置顶
	m_MainWindowCfg.bIsMinTray.xmlset(true);				//是否最小化到托盘
	m_MainWindowCfg.bAutoLock.xmlset(false);				//自动锁定时间（单位：秒）
	m_MainWindowCfg.nColorPlan.xmlset(1);				//配色方案
	m_MainWindowCfg.bFixPanel.xmlset(false);               //固定板块
	m_MainWindowCfg.bShowPanelShortKey.xmlset(false);      //板块标题显示快捷键  
	m_MainWindowCfg.nLogonVerify.xmlset(1);			//登录验证 0:不需要验证 1:数字字母组合验证 2:数字验证
	m_MainWindowCfg.bDelInstrumentWarning.xmlset(true);	//删除自选合约是否弹出提示框
	m_MainWindowCfg.TradeInfoDlgSize.xmlset(0);		//TradeInfoDlg的大小
	//m_MainWindowCfg.bModuleIsQry.xmlset(true);	        //1-各个模块使用成交回报触发查询的方式，0表示不使用
}

MainWindowCfgMgr::~MainWindowCfgMgr(void)
{
}

bool MainWindowCfgMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{
    if(root==NULL)
        return false;

    TiXmlElement *pMainWindowItem = root->FirstChildElement("MainWindow");
	if(pMainWindowItem == NULL)
		return false;

	const char *pAttr1 = pMainWindowItem->Attribute("Top");
	if(pAttr1) 
		m_MainWindowCfg.bIsTop.xmlset(atoi(pAttr1)>0?true:false, bFromUserCfg);

	const char *pAttr2 = pMainWindowItem->Attribute("MinTray");
	if(pAttr2)
		m_MainWindowCfg.bIsMinTray.xmlset(atoi(pAttr2)>0?true:false, bFromUserCfg);

	const char *pAttr3 = pMainWindowItem->Attribute("AutoLock");
	if(pAttr3)
		m_MainWindowCfg.bAutoLock.xmlset(atoi(pAttr3)>0?true:false, bFromUserCfg);

	const char *pAttr4 = pMainWindowItem->Attribute("ColorPlan");
	if(pAttr4)
		m_MainWindowCfg.nColorPlan.xmlset(atoi(pAttr4), bFromUserCfg);

    const char *pAttr5 = pMainWindowItem->Attribute("FixPanel");
	if(pAttr5)
		m_MainWindowCfg.bFixPanel.xmlset(atoi(pAttr5)>0?true:false, bFromUserCfg);

    const char *pAttr6 = pMainWindowItem->Attribute("ShowPanelShortKey");
	if(pAttr6)
		m_MainWindowCfg.bShowPanelShortKey.xmlset(atoi(pAttr6)>0?true:false, bFromUserCfg);

	const char *pAttr7 = pMainWindowItem->Attribute("LogonVerify");
	if(pAttr7)
		m_MainWindowCfg.nLogonVerify.xmlset(atoi(pAttr7), bFromUserCfg);

	const char *pAttr8 = pMainWindowItem->Attribute("DelInstrumentWarning");
	if(pAttr8)
		m_MainWindowCfg.bDelInstrumentWarning.xmlset(atoi(pAttr8)>0?true:false, bFromUserCfg);

	const char *pAttr9 = pMainWindowItem->Attribute("TradeInfoDlgSize");
	if(pAttr9)
		m_MainWindowCfg.TradeInfoDlgSize.xmlset(atoi(pAttr9), bFromUserCfg);
	//const char *pAttr10 = pMainWindowItem->Attribute("ModuleIsQry");
	//if(pAttr10)
	//	m_MainWindowCfg.bModuleIsQry.xmlset(atoi(pAttr10)>0?true:false, bFromUserCfg);

	return true;
}

bool MainWindowCfgMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement *pMainWindowItem = root->FirstChildElement("MainWindow");
	if(pMainWindowItem)
		root->RemoveChild(pMainWindowItem);

	pMainWindowItem = new TiXmlElement("MainWindow");
	if ( m_MainWindowCfg.bIsTop.needWriteToUserCfg())
	{
		pMainWindowItem->SetAttribute("Top", static_cast<int>(m_MainWindowCfg.bIsTop));
	}
	
	if ( m_MainWindowCfg.bIsMinTray.needWriteToUserCfg())
	{
		pMainWindowItem->SetAttribute("MinTray", static_cast<int>(m_MainWindowCfg.bIsMinTray));
	}
	
	if ( m_MainWindowCfg.bAutoLock.needWriteToUserCfg())
	{
		pMainWindowItem->SetAttribute("AutoLock", static_cast<int>(m_MainWindowCfg.bAutoLock));
	}
	
	if ( m_MainWindowCfg.nColorPlan.needWriteToUserCfg() )
	{
		pMainWindowItem->SetAttribute("ColorPlan", m_MainWindowCfg.nColorPlan);
	}
	
	if ( m_MainWindowCfg.bFixPanel.needWriteToUserCfg() )
	{
		pMainWindowItem->SetAttribute("FixPanel", static_cast<int>(m_MainWindowCfg.bFixPanel));
	}
	
	if ( m_MainWindowCfg.bShowPanelShortKey.needWriteToUserCfg())
	{
		pMainWindowItem->SetAttribute("ShowPanelShortKey", static_cast<int>(m_MainWindowCfg.bShowPanelShortKey));
	}
	
	if ( m_MainWindowCfg.nLogonVerify.needWriteToUserCfg())
	{
		pMainWindowItem->SetAttribute("LogonVerify", m_MainWindowCfg.nLogonVerify);
	}
	
	if ( m_MainWindowCfg.bDelInstrumentWarning.needWriteToUserCfg())
	{
		pMainWindowItem->SetAttribute("DelInstrumentWarning", static_cast<int>(m_MainWindowCfg.bDelInstrumentWarning));
	}
	
	if ( m_MainWindowCfg.TradeInfoDlgSize.needWriteToUserCfg())
	{
		pMainWindowItem->SetAttribute("TradeInfoDlgSize", static_cast<int>(m_MainWindowCfg.TradeInfoDlgSize));
	}
	
	//if ( m_MainWindowCfg.bModuleIsQry.needWriteToUserCfg() )
	//{
	//	pMainWindowItem->SetAttribute("ModuleIsQry", static_cast<int>(m_MainWindowCfg.bModuleIsQry));	
	//}
	
	if ( NULL != pMainWindowItem->FirstAttribute() || NULL != pMainWindowItem->FirstChild())
	{
		root->LinkEndChild(pMainWindowItem);
	}
	else
	{
		delete pMainWindowItem;
		pMainWindowItem = NULL;
	}

	return true;
}

void MainWindowCfgMgr::ClearCfg()
{
	memset(&m_MainWindowCfg, 0, sizeof(MAIN_WINDOW_CFG));
	m_MainWindowCfg.TradeInfoDlgSize=0;
}