#include "StdAfx.h"
#include "CConfirmItemChange.h"
#include "CfgMgr/CfgGlobal.h"

//////////////////////////////////////////////////////////////////////////////////////


CConfirmItemChange::CConfirmItemChange()
{
	m_nItemNum = conNothing;
}

CConfirmItemChange::~CConfirmItemChange()
{
}

void CConfirmItemChange::ChgToNotConfirm()
{
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) 
		return;
	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();
	if(pOrderCfg == NULL)
		return;

	switch(m_nItemNum) 
	{
	case conStdOrderConfirm:
		pOrderCfg->bOrderConfirm = false;
		break;
	case conAutoQuotClickConfirm:
		pOrderCfg->ClickQuotTableAction = 2;
		break;
	case conAutoQuotDBClickConfirm:
		pOrderCfg->DoubleClickQuotTableAction = 2;
		break;
	case conAutoPositionClickConfirm:
		pOrderCfg->ClickPositionAction = 2;
		break;
	case conAutoPositionDBClickConfirm:
		pOrderCfg->DoubleClickPositionAction = 2;
		break;
	case conAutoKeyPressConfirm:
		{
			for(int i=0; i<pOrderCfg->KeyOrderDataNum; i++) {
				if(m_strItemParam.compare(pOrderCfg->KeyOrderData[i].ShortKey)==0)
				{
					pOrderCfg->KeyOrderData[i].nAction = 2;
					break;
				}
			}

			//发送配置改变消息
			char szBuf[MAX_PATH];
			memset( szBuf, 0, MAX_PATH );
			itoa( static_cast<int>(MSG_ORDER_CHANGE), szBuf, 10 );
			wxString strMsg = szBuf;

			wxCommandEvent myEvent( wxEVT_CONFIG_CHANGED );
			DWORD EvtParamID;
			std::string EvtParamStr(strMsg.c_str());
			if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
			{
				myEvent.SetInt((int)EvtParamID);
				GETTOPWINDOW()->AddPendingEvent(myEvent);
			}
		}

		break;
	case conNothing:
		break;
	default:
		break;
	};
}
