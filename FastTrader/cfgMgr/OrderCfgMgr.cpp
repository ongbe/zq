#include "StdAfx.h"

#include "Module-Misc2/globalDefines.h"
#include "Module-Misc2/GlobalFunc.h"
#include "cfgmgr/OrderCfgMgr.h"
#include "cfgMgr/ShortCutKeysMgr.h"

extern long ColorStr2Long(const char* pColTextColor);


OrderCfgMgr::OrderCfgMgr(void)
{
	memset(&m_OrderCfg, 0, sizeof(m_OrderCfg));

	m_OrderCfg.bBuy5Sell5.xmlset(false);							// 是否显示五档行情
	m_OrderCfg.bMouseInputInstrumentId.xmlset(true);			// 是否可以用鼠标输入合约代码
	m_OrderCfg.bInputAbbreviation.xmlset(true);					// 可以输入缩写和拼音
	m_OrderCfg.nContractListStyle.xmlset(0);					//合约弹出框风格
	m_OrderCfg.bOrderConfirm.xmlset(true);						//下单前确认
	m_OrderCfg.bAutoOpenClose.xmlset(false);						//固定使用自动开平
	m_OrderCfg.b13Change.xmlset(false);							//13切换
	m_OrderCfg.bReTrack.xmlset(true);							//继续跟盘
	m_OrderCfg.bSimulate.xmlset(false);							//是否模拟
	m_OrderCfg.nMKTPriceUse.xmlset(0);						// 0 不计算 1 最新价 2 买卖价
	m_OrderCfg.bMouseInputPrice.xmlset(true);					// 是否可以用鼠标输入价格
	m_OrderCfg.bKeyBoardFastPrice.xmlset(false);					// 键盘快速输入价格
	m_OrderCfg.bMouseInputVolume.xmlset(true);					// 是否可以用鼠标输入常用手数
	int theVolume[]={1,2,3,4,5,10,20,30,50};
    m_OrderCfg.Volume.xmlset(theVolume);
	//m_OrderCfg.Name[5];							//下单项目顺序

	//自动开平
	m_OrderCfg.ProcessPositionType.xmlset(1);				//如果有持仓处理方式 1全平 2超出不处理 3超出方向开仓

	//键盘快速下单
	m_OrderCfg.bOpenQuotKeyBoardOrder.xmlset(true);				//是否启用键盘下单
	m_OrderCfg.bSyncQuotPanel.xmlset(true);						//发出报单是否填充下单块
	m_OrderCfg.bReturnQuotTable.xmlset(true);					//下单后光标是否回报价表
	m_OrderCfg.KeyOrderDataNum = 0;
	//m_OrderCfg.KeyOrderData[100];					//键盘快速下单设置数据

	//单击报价 ,双击报价， 单击持仓，双击持仓
	//1发出报单(带确认)   2 发出报单(不带确认)  3 填单后光标不动
	//4 填单后光标到合约代码  5 填单后光标到价格 6  填单后光标开平
	//7 填单后光标到买卖   8 填单后光标到数量 9 填单后光标到下单按钮
	//10 ""
	m_OrderCfg.ClickQuotTableAction.xmlset(3);                                                       
	m_OrderCfg.DoubleClickQuotTableAction.xmlset(1);			//双击报价
	m_OrderCfg.MouseFastOrderDirection.xmlset(0);			//下单方向
	m_OrderCfg.bCtrlReverse.xmlset(false);						//是否允许Ctrl点击临时反转方向
	m_OrderCfg.bAlwaysOpen.xmlset(false);                        //是否鼠标点击报价开仓
	m_OrderCfg.ClickPositionAction.xmlset(3);				//单击持仓
	m_OrderCfg.DoubleClickPositionAction.xmlset(1);			//双击持仓
	m_OrderCfg.PositionVolume.xmlset(0);                     //调整仓位数量

	//高级
	m_OrderCfg.OrderMultiDataNum = 0;
	//m_OrderCfg.OrderMultiData[100];				//下单倍率数据

	//风险管理
	m_OrderCfg.VolumeLimit.xmlset(0);							//手数限制
	m_OrderCfg.DifferPrice.xmlset(0);							//偏离价位 
	m_OrderCfg.ActionAfterOrder.xmlset(1);						//1,下单后不清空下单板2,全部清空3,清空价格和手数4只清空手数  

	//提示及反馈
	m_OrderCfg.bOrderSuccessDlg.xmlset(true);					// 下单成功 是否 弹框
	m_OrderCfg.bOrderSuccessSound.xmlset(false);					// 下单成功 是否 发声
	m_OrderCfg.bOrderFailDlg.xmlset(true);						// 下单失败 是否 弹框
	m_OrderCfg.bOrderFailSound.xmlset(false);					// 下单失败 是否 发声
	m_OrderCfg.bReachUpperLimitDlg.xmlset(true);				// 条件单达到上限 是否 弹框
	m_OrderCfg.bReachUpperLimitSound.xmlset(true);				// 条件单达到上限 是否 发声
	m_OrderCfg.bConventSuccessDlg.xmlset(true);					// 条件单转换成功 是否 弹框
	m_OrderCfg.bConventSuccessSound.xmlset(true);				// 条件单转换成功 是否 发声
	m_OrderCfg.bConventFailDlg.xmlset(true);					// 条件单转换失败 是否 弹框
	m_OrderCfg.bConventFailSound.xmlset(true);					// 条件单转换失败 是否 发声
	m_OrderCfg.bTradeDlg.xmlset(true);							// 挂单成交 是否 弹框
	m_OrderCfg.bTradeSound.xmlset(true);						// 挂单成交 是否 发声
	m_OrderCfg.SellColor.xmlset(12910522);							//卖色
	m_OrderCfg.BuyColor.xmlset(16765183);							//买色

	//快捷键
	m_OrderCfg.cBuyHotKey.xmlset(1);							//买入快捷键
	m_OrderCfg.cSelHotKey.xmlset(3);							//卖出快捷键
	m_OrderCfg.cOpenOrderHotKey.xmlset(1);					//开仓快捷键
	m_OrderCfg.cCloseTodayVolumnHotKey.xmlset(2);			//平今快捷键
	m_OrderCfg.cCloseOrderHotKey.xmlset(3);					//平仓快捷键
	m_OrderCfg.szOpenOrderBtnHotKey.xmlset("F9");			//下单按钮快捷键
	m_OrderCfg.szOrderParkedBtnHotKey.xmlset("F10");			//预埋单按钮快捷键
	m_OrderCfg.nCursorPosAfterOrder.xmlset(0);				//下单后光标定位位置 0,默认 1,合约 2,买卖 3,开平 4,价格 5,手数
	m_OrderCfg.nAllNotHaveOppositePosition.xmlset(1);
	m_OrderCfg.nSomeHaveOppositePosition.xmlset(2);
	m_OrderCfg.nAllHaveOppositePosition.xmlset(2);
	m_OrderCfg.nMouseClosePositionStrategy.xmlset(2);
	m_OrderCfg.nCancelSameDirOpenOrderBeforeOrder.xmlset(0);
}

OrderCfgMgr::~OrderCfgMgr(void)
{

}

bool OrderCfgMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{   
    if(root==NULL)
        return false;

    TiXmlElement *pOrder = root->FirstChildElement("Order");
	if(pOrder == NULL)
		return false;

	const char *pElementValue = NULL;
	TiXmlElement *pItem = pOrder->FirstChildElement("bMouseInputInstrumentId");
	if( pItem && pItem->GetText()) 
	{
		m_OrderCfg.bMouseInputInstrumentId.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bBuy5Sell5");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bBuy5Sell5.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bInputAbbreviation");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bInputAbbreviation.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("ContractListStyle");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.nContractListStyle.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bOrderConfirm");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bOrderConfirm.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bAutoOpenClose");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bAutoOpenClose.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("b13Change");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.b13Change.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bReTrack");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bReTrack.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bSimulate");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bSimulate.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("MKTPriceUse");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.nMKTPriceUse.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bMouseInputPrice");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bMouseInputPrice.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bKeyBoardFastPrice");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bKeyBoardFastPrice.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bMouseInputVolume");
	if(pItem && pItem->GetText())
	{
		m_OrderCfg.bMouseInputVolume.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	TiXmlElement *pVolumeItem = pOrder->FirstChildElement("Volume"); 

	if(pVolumeItem) 
	{
		int theVolume[9]={0};
		pElementValue = pVolumeItem->Attribute("v1");
		if(!pElementValue)
			theVolume[0]=0;
		else
			theVolume[0] = atoi(pElementValue);

		pElementValue = pVolumeItem->Attribute("v2");
		if(!pElementValue)
			theVolume[1]=0;
		else
			theVolume[1] = atoi(pElementValue);

		pElementValue = pVolumeItem->Attribute("v3");
		if(!pElementValue)
			theVolume[2] = 0;
		else
			theVolume[2] = atoi(pElementValue);

		pElementValue = pVolumeItem->Attribute("v4");
		if(!pElementValue)
			theVolume[3] =0;
		else
			theVolume[3] = atoi(pElementValue);

		pElementValue = pVolumeItem->Attribute("v5");
		if(!pElementValue)
			theVolume[4] =0;
		else
			theVolume[4] = atoi(pElementValue);

		pElementValue = pVolumeItem->Attribute("v6");
		if(!pElementValue)
			theVolume[5] =0;
		else
			theVolume[5] = atoi(pElementValue);

		pElementValue = pVolumeItem->Attribute("v7");
		if(!pElementValue)
			theVolume[6]=0;
		else
			theVolume[6] = atoi(pElementValue);

		pElementValue = pVolumeItem->Attribute("v8");
		if(!pElementValue)
			theVolume[7]=0;
		else
			theVolume[7] = atoi(pElementValue);

		pElementValue = pVolumeItem->Attribute("v9");
		if(!pElementValue)
			theVolume[8] =0;
		else
			theVolume[8] = atoi(pElementValue);
		m_OrderCfg.Volume.xmlset(theVolume);
	}

	TiXmlElement *pOrderItem = pOrder->FirstChildElement("OrderItem"); 
	if(!pOrderItem) 
	{
		pElementValue = pOrderItem->Attribute("item1");
		if(pElementValue)
			strncpy(m_OrderCfg.Name[0], pElementValue, sizeof(m_OrderCfg.Name[0])-1);

		pElementValue = pOrderItem->Attribute("item2");
		if(pElementValue)
			strncpy(m_OrderCfg.Name[1], pElementValue, sizeof(m_OrderCfg.Name[1])-1);

		pElementValue = pOrderItem->Attribute("item3");
		if(pElementValue)
			strncpy(m_OrderCfg.Name[2], pElementValue, sizeof(m_OrderCfg.Name[2])-1);

		pElementValue = pOrderItem->Attribute("item4");
		if(pElementValue)
			strncpy(m_OrderCfg.Name[3], pElementValue, sizeof(m_OrderCfg.Name[3])-1);

		pElementValue = pOrderItem->Attribute("item5");
		if(pElementValue)
			strncpy(m_OrderCfg.Name[4], pElementValue, sizeof(m_OrderCfg.Name[4])-1);
	}

	pItem = pOrder->FirstChildElement("KeyOrderDataNum");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.KeyOrderDataNum = atoi(pItem->GetText());
	}

	int index = 0;
	TiXmlElement* p = pOrder->FirstChildElement("KeyOrderData");
	if(p)
	{
		KeyOrderInfo info;
		index = 0;
		for(TiXmlElement *Item = p->FirstChildElement("item"); 
			Item; 
			Item = Item->NextSiblingElement("item"))
		{ 
			pElementValue = Item->Attribute("ShortKey");
			if( !pElementValue) continue;
			else
				strncpy(info.ShortKey,Item->Attribute("ShortKey"), sizeof(info.ShortKey)-1);

			pElementValue = Item->Attribute("BuySell");
			if( !pElementValue) continue;
			else
				info.nBuySell = atoi(Item->Attribute("BuySell"));

			pElementValue = Item->Attribute("Price");
			if( !pElementValue ) continue;
			else 
				info.nPrice = atoi(Item->Attribute("Price"));

			pElementValue = Item->Attribute("OverPrice");
			if( !pElementValue ) continue;
			else
				info.nOverPrice = atoi(Item->Attribute("OverPrice"));

			pElementValue = Item->Attribute("Action");
			if( !pElementValue ) continue;
			else
				info.nAction = atoi(Item->Attribute("Action"));

			pElementValue = Item->Attribute("ActionArea");
			if(! pElementValue) continue;
			else
				info.nActionArea = atoi(Item->Attribute("ActionArea"));
			memcpy(&m_OrderCfg.KeyOrderData[index++],&info,sizeof(info));
		}
	}

	pItem = pOrder->FirstChildElement("ProcessPositionType");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.ProcessPositionType.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bOpenQuotKeyBoardOrder");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bOpenQuotKeyBoardOrder.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bSyncQuotPanel");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bSyncQuotPanel.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bReturnQuotTable");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bReturnQuotTable.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("ClickQuotTableAction");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.ClickQuotTableAction.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("DoubleClickQuotTableAction");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.DoubleClickQuotTableAction.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("MouseFastOrderDirection");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.MouseFastOrderDirection.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bCtrlReverse");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bCtrlReverse.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bAlwaysOpen");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bAlwaysOpen.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("ClickPositionAction");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.ClickPositionAction.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("DoubleClickPositionAction");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.DoubleClickPositionAction.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("PositionVolume");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.PositionVolume.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("OrderMultiDataNum");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.OrderMultiDataNum = atoi(pItem->GetText());
	}

	p = pOrder->FirstChildElement("OrderMultiData");
	if(p)
	{
		index=0;
		char *pItem = NULL;
		OrderMultiply data;
		for(TiXmlElement *Item = p->FirstChildElement("item"); 
			Item; 
			Item = Item->NextSiblingElement("item"))
		{	
			pItem = (char*)Item->Attribute("ShortKey");
			if(!pItem)
				continue;
			else
				strncpy(data.ShortKey,Item->Attribute("ShortKey"), sizeof(data.ShortKey)-1);

			pItem = (char*)Item->Attribute("Volume");
			if( !pItem )
				continue;
			else
				data.Volume = atoi(Item->Attribute("Volume"));

			memcpy(&m_OrderCfg.OrderMultiData[index++],&data,sizeof(data));
		}
	}

	pItem = pOrder->FirstChildElement("VolumeLimit");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.VolumeLimit.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("DifferPrice");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.DifferPrice.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("ActionAfterOrder");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.ActionAfterOrder.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bOrderSuccessDlg");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bOrderSuccessDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bOrderSuccessSound");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bOrderSuccessSound.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bOrderFailDlg");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bOrderFailDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bOrderFailSound");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bOrderFailSound.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bReachUpperLimitDlg");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bReachUpperLimitDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bReachUpperLimitSound");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bReachUpperLimitSound.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bConventSuccessDlg");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bConventSuccessDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bConventSuccessSound");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bConventSuccessSound.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bConventFailDlg");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bConventFailDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bConventFailSound");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bConventFailSound.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bOrderSuccessDlg");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bOrderSuccessDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bTradeDlg");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bTradeDlg.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("bTradeSound");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.bTradeSound.xmlset(atoi(pItem->GetText())>0?true:false, bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("SellColor");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.SellColor.xmlset(ColorStr2Long(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("BuyColor");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.BuyColor.xmlset(ColorStr2Long(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("CursorPosAfterOrder");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.nCursorPosAfterOrder.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	//快捷键
	pItem = pOrder->FirstChildElement("BuyHotKey");
	if(pItem) 
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_OrderCfg.cBuyHotKey.xmlset(pElementValue[0], bFromUserCfg);
		}
	}

	pItem = pOrder->FirstChildElement("SellHotKey");
	if(pItem) 
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_OrderCfg.cSelHotKey.xmlset(pElementValue[0], bFromUserCfg);
		}
	}

	pItem = pOrder->FirstChildElement("OpenOrderHotKey");
	if(pItem) 
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_OrderCfg.cOpenOrderHotKey.xmlset(pElementValue[0], bFromUserCfg);
		}
	}

	pItem = pOrder->FirstChildElement("CloseTodayVolumnHotKey");
	if(pItem) 
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_OrderCfg.cCloseTodayVolumnHotKey.xmlset(pElementValue[0], bFromUserCfg);
		}
	}

	pItem = pOrder->FirstChildElement("CloseOrderHotKey");
	if(pItem) 
	{
		pElementValue = pItem->GetText();
		if(pElementValue)
		{
			m_OrderCfg.cCloseOrderHotKey.xmlset(pElementValue[0], bFromUserCfg);
		}
	}

	pItem = pOrder->FirstChildElement("OpenOrderBtnHotKey");
	if(pItem ) 
	{
		if ( pItem->GetText() )
		{
			m_OrderCfg.szOpenOrderBtnHotKey.xmlset(pItem->GetText(), bFromUserCfg);
		}
		else
		{
			m_OrderCfg.szOpenOrderBtnHotKey.xmlset("", bFromUserCfg);
		}
	}

	pItem = pOrder->FirstChildElement("OrderParkedBtnHotKey");
	if(pItem ) 
	{
		if ( pItem->GetText() )
		{
			m_OrderCfg.szOrderParkedBtnHotKey.xmlset(pItem->GetText(), bFromUserCfg);
		}
		else
		{
			m_OrderCfg.szOrderParkedBtnHotKey.xmlset("", bFromUserCfg);
		}
	}

	pItem = pOrder->FirstChildElement("AllNotHaveOppositePosition");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.nAllNotHaveOppositePosition.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("SomeHaveOppositePosition");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.nSomeHaveOppositePosition.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("AllHaveOppositePosition");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.nAllHaveOppositePosition.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("MouseClosePositionStrategy");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.nMouseClosePositionStrategy.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	pItem = pOrder->FirstChildElement("CancelSameDirOpenOrderBeforeOrder");
	if(pItem && pItem->GetText()) 
	{
		m_OrderCfg.nCancelSameDirOpenOrderBeforeOrder.xmlset(atoi(pItem->GetText()), bFromUserCfg);
	}

	return true;
}

bool OrderCfgMgr::SaveCfg(TiXmlElement *root)
{	
	TiXmlElement *pOrderItem = root->FirstChildElement("Order");
	if(pOrderItem)
		root->RemoveChild(pOrderItem);

	pOrderItem = new TiXmlElement("Order");

	TiXmlElement *pChildItem = NULL;
	std::string strContent;
	TiXmlText *pContent = NULL;

	if ( m_OrderCfg.bMouseInputInstrumentId.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bMouseInputInstrumentId");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bMouseInputInstrumentId.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bBuy5Sell5.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bBuy5Sell5");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bBuy5Sell5.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bInputAbbreviation.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bInputAbbreviation");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bInputAbbreviation.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.nContractListStyle.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ContractListStyle");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.nContractListStyle.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bOrderConfirm.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bOrderConfirm");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bOrderConfirm.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bAutoOpenClose.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bAutoOpenClose");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bAutoOpenClose.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.b13Change.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("b13Change");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.b13Change.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bReTrack.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bReTrack");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bReTrack.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bSimulate.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bSimulate");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bSimulate.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.nMKTPriceUse.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("MKTPriceUse");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.nMKTPriceUse.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bMouseInputPrice.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bMouseInputPrice");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bMouseInputPrice.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bKeyBoardFastPrice.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bKeyBoardFastPrice");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bKeyBoardFastPrice.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bMouseInputVolume.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bMouseInputVolume");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bMouseInputVolume.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}
	if ( m_OrderCfg.Volume.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("Volume");
		pChildItem->SetAttribute("v1",m_OrderCfg.Volume[0]);
		pChildItem->SetAttribute("v2",m_OrderCfg.Volume[1]);
		pChildItem->SetAttribute("v3",m_OrderCfg.Volume[2]);
		pChildItem->SetAttribute("v4",m_OrderCfg.Volume[3]);
		pChildItem->SetAttribute("v5",m_OrderCfg.Volume[4]);
		pChildItem->SetAttribute("v6",m_OrderCfg.Volume[5]);
		pChildItem->SetAttribute("v7",m_OrderCfg.Volume[6]);
		pChildItem->SetAttribute("v8",m_OrderCfg.Volume[7]);
		pChildItem->SetAttribute("v9",m_OrderCfg.Volume[8]);
		pOrderItem->LinkEndChild(pChildItem);
	}

	pChildItem = new TiXmlElement("OrderItem");
	pChildItem->SetAttribute( "item1",m_OrderCfg.Name[0] );
	pChildItem->SetAttribute( "item2",m_OrderCfg.Name[1] );
	pChildItem->SetAttribute( "item3",m_OrderCfg.Name[2] );
	pChildItem->SetAttribute( "item4",m_OrderCfg.Name[3] );
	pChildItem->SetAttribute( "item5",m_OrderCfg.Name[4] );
	pOrderItem->LinkEndChild(pChildItem);

	//自动开平
	if ( m_OrderCfg.ProcessPositionType.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ProcessPositionType");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.ProcessPositionType.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bOpenQuotKeyBoardOrder.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bOpenQuotKeyBoardOrder");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bOpenQuotKeyBoardOrder.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bSyncQuotPanel.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bSyncQuotPanel");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bSyncQuotPanel.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bReturnQuotTable.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bReturnQuotTable");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bReturnQuotTable.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	pChildItem = new TiXmlElement("KeyOrderDataNum");
	pOrderItem->LinkEndChild(pChildItem);
	strContent = GlobalFunc::ConvertToString(m_OrderCfg.KeyOrderDataNum);
	pContent = new TiXmlText(strContent.c_str());  
	pChildItem->LinkEndChild(pContent);

	pChildItem = new TiXmlElement("KeyOrderData");
	pOrderItem->LinkEndChild(pChildItem);

	for( int i=0; i<m_OrderCfg.KeyOrderDataNum; ++i )
	{
		TiXmlElement* pElement =  new TiXmlElement("item");
		pElement->SetAttribute("ShortKey",m_OrderCfg.KeyOrderData[i].ShortKey);
		pElement->SetAttribute("BuySell",m_OrderCfg.KeyOrderData[i].nBuySell);
		pElement->SetAttribute("Price",m_OrderCfg.KeyOrderData[i].nPrice);
		pElement->SetAttribute("OverPrice",m_OrderCfg.KeyOrderData[i].nOverPrice);
		pElement->SetAttribute("Action",m_OrderCfg.KeyOrderData[i].nAction);
		pElement->SetAttribute("ActionArea",m_OrderCfg.KeyOrderData[i].nActionArea);
		pChildItem->LinkEndChild(pElement);
	}

	if ( m_OrderCfg.ClickQuotTableAction.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClickQuotTableAction");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.ClickQuotTableAction.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.DoubleClickQuotTableAction.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("DoubleClickQuotTableAction");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.DoubleClickQuotTableAction.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.MouseFastOrderDirection.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("MouseFastOrderDirection");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.MouseFastOrderDirection.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bCtrlReverse.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bCtrlReverse");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bCtrlReverse.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bAlwaysOpen.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bAlwaysOpen");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bAlwaysOpen.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.ClickPositionAction.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ClickPositionAction");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.ClickPositionAction.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.DoubleClickPositionAction.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("DoubleClickPositionAction");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.DoubleClickPositionAction.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.PositionVolume.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("PositionVolume");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.PositionVolume.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	pChildItem = new TiXmlElement("OrderMultiDataNum");
	pOrderItem->LinkEndChild(pChildItem);
	strContent = GlobalFunc::ConvertToString(m_OrderCfg.OrderMultiDataNum);
	pContent = new TiXmlText(strContent.c_str());  
	pChildItem->LinkEndChild(pContent);

	//高级
	pChildItem = new TiXmlElement("OrderMultiData");
	pOrderItem->LinkEndChild(pChildItem);

	for( int i=0; i<m_OrderCfg.OrderMultiDataNum; ++i )
	{
		TiXmlElement* pElement =  new TiXmlElement("item");
		pElement->SetAttribute("ShortKey",m_OrderCfg.OrderMultiData[i].ShortKey);
		pElement->SetAttribute("Volume",m_OrderCfg.OrderMultiData[i].Volume);
		pChildItem->LinkEndChild(pElement);
	}

	if ( m_OrderCfg.VolumeLimit.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("VolumeLimit");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.VolumeLimit.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.DifferPrice.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("DifferPrice");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.DifferPrice.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.ActionAfterOrder.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("ActionAfterOrder");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.ActionAfterOrder.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bOrderSuccessDlg.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bOrderSuccessDlg");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bOrderSuccessDlg.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bOrderSuccessSound.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bOrderSuccessSound");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bOrderSuccessSound.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bOrderFailDlg.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bOrderFailDlg");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bOrderFailDlg.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bOrderFailSound.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bOrderFailSound");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bOrderFailSound.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bReachUpperLimitDlg.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bReachUpperLimitDlg");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bReachUpperLimitDlg.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bReachUpperLimitSound.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bReachUpperLimitSound");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bReachUpperLimitSound.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bConventSuccessDlg.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bConventSuccessDlg");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bConventSuccessDlg.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bConventSuccessSound.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bConventSuccessSound");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bConventSuccessSound.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bConventFailDlg.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bConventFailDlg");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bConventFailDlg.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bConventFailSound.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bConventFailSound");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bConventFailSound.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bTradeDlg.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bTradeDlg");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bTradeDlg.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.bTradeSound.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("bTradeSound");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.bTradeSound.operator bool());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.SellColor.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("SellColor");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.SellColor.operator long());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.BuyColor.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("BuyColor");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.BuyColor.operator long());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.nCursorPosAfterOrder.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("CursorPosAfterOrder");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.nCursorPosAfterOrder.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	//快捷键
	if ( m_OrderCfg.cBuyHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("BuyHotKey");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = m_OrderCfg.cBuyHotKey;
		pContent = new TiXmlText( strContent.c_str() );  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.cSelHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("SellHotKey");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = m_OrderCfg.cSelHotKey;
		pContent = new TiXmlText( strContent.c_str() ); 
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.cOpenOrderHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenOrderHotKey");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = m_OrderCfg.cOpenOrderHotKey;
		pContent = new TiXmlText( strContent.c_str() ); 
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.cCloseTodayVolumnHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("CloseTodayVolumnHotKey");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = m_OrderCfg.cCloseTodayVolumnHotKey;
		pContent = new TiXmlText( strContent.c_str() );  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.cCloseOrderHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("CloseOrderHotKey");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = m_OrderCfg.cCloseOrderHotKey;
		pContent = new TiXmlText( strContent.c_str() );   
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.szOpenOrderBtnHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OpenOrderBtnHotKey");
		pOrderItem->LinkEndChild(pChildItem);
		pContent = new TiXmlText( m_OrderCfg.szOpenOrderBtnHotKey.operator const char*() );   
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.szOrderParkedBtnHotKey.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("OrderParkedBtnHotKey");
		pOrderItem->LinkEndChild(pChildItem);
		pContent = new TiXmlText(m_OrderCfg.szOrderParkedBtnHotKey.operator const char*() );   
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.nAllNotHaveOppositePosition.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("AllNotHaveOppositePosition");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.nAllNotHaveOppositePosition.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.nSomeHaveOppositePosition.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("SomeHaveOppositePosition");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.nSomeHaveOppositePosition.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.nAllHaveOppositePosition.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("AllHaveOppositePosition");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.nAllHaveOppositePosition.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.nMouseClosePositionStrategy.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("MouseClosePositionStrategy");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.nMouseClosePositionStrategy.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( m_OrderCfg.nCancelSameDirOpenOrderBeforeOrder.needWriteToUserCfg())
	{
		pChildItem = new TiXmlElement("CancelSameDirOpenOrderBeforeOrder");
		pOrderItem->LinkEndChild(pChildItem);
		strContent = GlobalFunc::ConvertToString(m_OrderCfg.nCancelSameDirOpenOrderBeforeOrder.operator int());
		pContent = new TiXmlText(strContent.c_str());  
		pChildItem->LinkEndChild(pContent);
	}

	if ( NULL != pOrderItem->FirstAttribute() || NULL != pOrderItem->FirstChild())
	{
		root->LinkEndChild(pOrderItem);
	}
	else
	{
		delete pOrderItem;
		pOrderItem = NULL;
	}

	return true;
}

void OrderCfgMgr::ClearCfg()
{
	ZeroMemory(&m_OrderCfg, sizeof(m_OrderCfg));
}

void OrderCfgMgr::CalcUsableShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	for ( int i = 0; i < m_OrderCfg.KeyOrderDataNum; i++)
	{
		pShortCutKeyMgr->DelUsableShortCutKey(m_OrderCfg.KeyOrderData[i].ShortKey);
	}

	for ( int i = 0; i < m_OrderCfg.OrderMultiDataNum; i++ )
	{
		pShortCutKeyMgr->DelUsableShortCutKey(m_OrderCfg.OrderMultiData[i].ShortKey);
	}

	pShortCutKeyMgr->DelUsableShortCutKey(m_OrderCfg.szOpenOrderBtnHotKey.operator const char*());
	pShortCutKeyMgr->DelUsableShortCutKey(m_OrderCfg.szOrderParkedBtnHotKey.operator const char*());
}

void OrderCfgMgr::ProcessDulplicateShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr, bool bModify )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	for ( int i = 0; i < m_OrderCfg.KeyOrderDataNum; i++)
	{
		if ( !bModify )
		{
			pShortCutKeyMgr->CalcDulplicateShortCutKey(m_OrderCfg.KeyOrderData[i].ShortKey);
		}
		else
		{
			if ( pShortCutKeyMgr->IsShortKeyDulplicate(m_OrderCfg.KeyOrderData[i].ShortKey) )
			{
				strcpy(m_OrderCfg.KeyOrderData[i].ShortKey, "");
			}
		}
	}

	for ( int i = 0; i < m_OrderCfg.OrderMultiDataNum; i++ )
	{
		if ( !bModify )
		{
			pShortCutKeyMgr->CalcDulplicateShortCutKey(m_OrderCfg.OrderMultiData[i].ShortKey);
		}
		else
		{
			if ( pShortCutKeyMgr->IsShortKeyDulplicate(m_OrderCfg.OrderMultiData[i].ShortKey) )
			{
				strcpy(m_OrderCfg.OrderMultiData[i].ShortKey, "");
			}
		}
	}

	if ( !bModify )
	{
		pShortCutKeyMgr->CalcDulplicateShortCutKey(m_OrderCfg.szOpenOrderBtnHotKey.operator const char*());
		pShortCutKeyMgr->CalcDulplicateShortCutKey(m_OrderCfg.szOrderParkedBtnHotKey.operator const char*());
	}
	else
	{
		if ( pShortCutKeyMgr->IsShortKeyDulplicate(m_OrderCfg.szOpenOrderBtnHotKey.operator const char*()))
		{
			m_OrderCfg.szOpenOrderBtnHotKey.xmlset("", true);
		}

		if ( pShortCutKeyMgr->IsShortKeyDulplicate(m_OrderCfg.szOrderParkedBtnHotKey.operator const char*()))
		{
			m_OrderCfg.szOrderParkedBtnHotKey.xmlset("", true);
		}
	}
}
