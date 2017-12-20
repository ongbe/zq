#include "StdAfx.h"
#include "Module-Misc/GLobalFunc.h"
#include "CancelOrderCfgMgr.h"
#include "cfgMgr/ShortCutKeysMgr.h"

CancelOrderCfgMgr::CancelOrderCfgMgr(void)
{
	memset(&m_cancelOrderCfg, 0, sizeof(CANCEL_ORDER_CFG));

	m_cancelOrderCfg.bDoubleClickMouseCancel.xmlset(true);				//鼠标双击撤单
	m_cancelOrderCfg.bCancelConfirm.xmlset(true);							//撤单前确认
	m_cancelOrderCfg.bHideCancelButton.xmlset(false);			            //隐藏撤单按钮
	m_cancelOrderCfg.bHideCancelAllButton.xmlset(false);			        //隐藏全撤按钮

	m_cancelOrderCfg.bOpenModifyOrder.xmlset(true);				        //启用改单
	m_cancelOrderCfg.bModifyOrderConfirm.xmlset(true);			    	//改单前确认

	m_cancelOrderCfg.bCancelSuccessDlg.xmlset(true);						// 下单成功 是否 弹框
	m_cancelOrderCfg.bCancelSuccessSound.xmlset(false);					// 下单成功 是否 发声

	m_cancelOrderCfg.bCancelFailDlg.xmlset(true);							// 下单成功 是否 弹框
	m_cancelOrderCfg.bCancelFailSound.xmlset(false);						// 下单成功 是否 发声

	m_cancelOrderCfg.bOpenKeyBoardCancelOrder.xmlset(false);				//是否启用报价表键盘下单
	m_cancelOrderCfg.bReturnQuotTable.xmlset(false);						//下单后光标是否回报价表
	m_cancelOrderCfg.szCancelOrderBtnHotKey.xmlset("F8");				//撤单按钮快捷键
	m_cancelOrderCfg.szCancelAllOrderBtnHotKey.xmlset("F7");			//全部撤单按钮快捷键
}

CancelOrderCfgMgr::~CancelOrderCfgMgr(void)
{
}

void CancelOrderCfgMgr::SetCancelOrderCfg( const CANCEL_ORDER_CFG& cancelOrderCfg )
{
	m_cancelOrderCfg = cancelOrderCfg;
}

bool CancelOrderCfgMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{   
    if(root==NULL)
        return false;

    TiXmlElement *pCancelOrderItem = root->FirstChildElement("CancelOrder");
	if(pCancelOrderItem == NULL)
		return false;

	const char *pElementValue = NULL;
	TiXmlElement *pItem = pCancelOrderItem->FirstChildElement("bDoubleClickMouseCancel");
	if(pItem && pItem->GetText()) 
	{
		m_cancelOrderCfg.bDoubleClickMouseCancel.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bCancelConfirm");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bCancelConfirm.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bHideCancelButton");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bHideCancelButton.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bOpenModifyOrder");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bOpenModifyOrder.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bModifyOrderConfirm");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bModifyOrderConfirm.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bCancelSuccessDlg");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bCancelSuccessDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bCancelSuccessSound");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bCancelSuccessSound.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bCancelFailDlg");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bCancelFailDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bCancelFailSound");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bCancelFailSound.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bOpenKeyBoardCancelOrder");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bOpenKeyBoardCancelOrder.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("bReturnQuotTable");
	if(pItem && pItem->GetText())
	{
		m_cancelOrderCfg.bReturnQuotTable.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pCancelOrderItem->FirstChildElement("CancelOrderBtnHotKey");
	if(pItem )
	{
		if( pItem->GetText() )
		{
			m_cancelOrderCfg.szCancelOrderBtnHotKey.xmlset(pItem->GetText(), bFromUserCfg);
		}
		else
		{
			m_cancelOrderCfg.szCancelOrderBtnHotKey.xmlset("", bFromUserCfg);
		}
	}

	pItem = pCancelOrderItem->FirstChildElement("CancelAllOrderBtnHotKey");
	if(pItem )
	{
		if ( pItem->GetText())
		{
			m_cancelOrderCfg.szCancelAllOrderBtnHotKey.xmlset(pItem->GetText(), bFromUserCfg);
		}
		else
		{
			m_cancelOrderCfg.szCancelAllOrderBtnHotKey.xmlset("", bFromUserCfg);
		}
	}

	return true;
}

bool CancelOrderCfgMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement *pCancelOrderItem = root->FirstChildElement("CancelOrder");
	if(pCancelOrderItem)
		root->RemoveChild(pCancelOrderItem);
	
	pCancelOrderItem = new TiXmlElement("CancelOrder");

	TiXmlElement *pChildItem = NULL;
	std::string strContent;
	TiXmlText *pContent = NULL;
	if ( m_cancelOrderCfg.bDoubleClickMouseCancel.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bDoubleClickMouseCancel");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bDoubleClickMouseCancel.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bCancelConfirm.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bCancelConfirm");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bCancelConfirm.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bHideCancelButton.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bHideCancelButton");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bHideCancelButton.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bOpenModifyOrder.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bOpenModifyOrder");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bOpenModifyOrder.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bModifyOrderConfirm.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bModifyOrderConfirm");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bModifyOrderConfirm.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bCancelSuccessDlg.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bCancelSuccessDlg");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bCancelSuccessDlg.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bCancelSuccessSound.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bCancelSuccessSound");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bCancelSuccessSound.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bCancelFailDlg.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bCancelFailDlg");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bCancelFailDlg.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bCancelFailSound.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bCancelFailSound");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bCancelFailSound.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bOpenKeyBoardCancelOrder.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bOpenKeyBoardCancelOrder");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bOpenKeyBoardCancelOrder.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.bReturnQuotTable.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bReturnQuotTable");
		pCancelOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_cancelOrderCfg.bReturnQuotTable.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.szCancelOrderBtnHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("CancelOrderBtnHotKey");
		pCancelOrderItem->LinkEndChild(pChildItem);
		pContent = new TiXmlText(m_cancelOrderCfg.szCancelOrderBtnHotKey.operator const char*());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_cancelOrderCfg.szCancelAllOrderBtnHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("CancelAllOrderBtnHotKey");
		pCancelOrderItem->LinkEndChild(pChildItem);
		pContent = new TiXmlText(m_cancelOrderCfg.szCancelAllOrderBtnHotKey.operator const char*());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( NULL != pCancelOrderItem->FirstAttribute() || NULL != pCancelOrderItem->FirstChild())
	{
		root->LinkEndChild(pCancelOrderItem);
	}
	else
	{
		delete pCancelOrderItem;
		pCancelOrderItem = NULL;
	}
	
	return true;
}

void CancelOrderCfgMgr::ClearCfg()
{
	ZeroMemory(&m_cancelOrderCfg, sizeof(m_cancelOrderCfg));
}

void CancelOrderCfgMgr::CalcUsableShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr )
{
	if ( NULL == pShortCutKeyMgr)
	{
		return;
	}

	pShortCutKeyMgr->DelUsableShortCutKey(m_cancelOrderCfg.szCancelOrderBtnHotKey.operator const char*());
	pShortCutKeyMgr->DelUsableShortCutKey(m_cancelOrderCfg.szCancelAllOrderBtnHotKey.operator const char*());
}

void CancelOrderCfgMgr::ProcessDulplicateShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr, bool bModify )
{
	if ( NULL == pShortCutKeyMgr)
	{
		return;
	}

	if ( !bModify )
	{
		pShortCutKeyMgr->CalcDulplicateShortCutKey(m_cancelOrderCfg.szCancelOrderBtnHotKey.operator const char*());
		pShortCutKeyMgr->CalcDulplicateShortCutKey(m_cancelOrderCfg.szCancelAllOrderBtnHotKey.operator const char*());
	}
	else
	{
		if ( pShortCutKeyMgr->IsShortKeyDulplicate(m_cancelOrderCfg.szCancelOrderBtnHotKey.operator const char*()))
		{
			m_cancelOrderCfg.szCancelOrderBtnHotKey.xmlset("", true);
		}

		if ( pShortCutKeyMgr->IsShortKeyDulplicate(m_cancelOrderCfg.szCancelAllOrderBtnHotKey.operator const char*()))
		{
			m_cancelOrderCfg.szCancelAllOrderBtnHotKey.xmlset("", true);
		}
	}
}
