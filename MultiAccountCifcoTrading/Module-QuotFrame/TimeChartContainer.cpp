#include "StdAfx.h"
#include "TimeChartContainer.h"
#include "../inc/gui/MainFrame.h"
#include "../inc/Plusin/Plusin.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
extern CPlusin g_Plusin;


CTimeChartContainer::CTimeChartContainer(wxWindow *parent,
               wxWindowID id,
               const wxPoint& pos,
               const wxSize& size,
               long style,
			   const wxString& name)
			   :zqPanel(parent, id, pos, size, style, name)
{

}
CTimeChartContainer::~CTimeChartContainer(void)
{
}
BEGIN_EVENT_TABLE(CTimeChartContainer, zqPanel)
EVT_LEFT_DCLICK(CTimeChartContainer::OnLeftDClick)
EVT_COMMAND(wxID_ANY,wxEVT_PLUGIN_HIDE,OnPluginHide)
END_EVENT_TABLE()
void CTimeChartContainer::OnLeftDClick(wxMouseEvent& event)
{
	wxCommandEvent parentevt(wxEVT_QUOT_VIEW_CHANGE);
	GetParent()->ProcessEvent(parentevt);
}
void CTimeChartContainer::OnPluginHide(wxCommandEvent& event)
{
	wxCommandEvent parentevt(wxEVT_QUOT_VIEW_CHANGE);
	GetParent()->ProcessEvent(parentevt);
}
void CTimeChartContainer::SendInstrumentMsg(wxPanel* panel)
{
	if(DEFAULT_SVR()&&DEFAULT_SVR()->SupportPlugins())
	{
		string strInstrumentID;
		((MainFrame*)GETTOPWINDOW())->GetCurInstrumentID_QuotPanel(strInstrumentID);

		HWND childHWnd = GetWindow( GetHwndOf(this), GW_CHILD);
		if ( IsWindow( childHWnd ))
		{
			//发送当前合约组合约列表
			CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
			if ( NULL == pCfgMgr )
			{
				return;
			}

			std::vector<std::string>* pInstrumentList = pCfgMgr->GetCurrentGroupMember();
			if ( NULL == pInstrumentList || pInstrumentList->size() == 0)
			{
				return;
			}

			char* pBuffer = new char[sizeof(InstrumentIDType)*pInstrumentList->size()];
			memset(pBuffer, 0, sizeof(InstrumentIDType)*pInstrumentList->size());
			for ( size_t i = 0; i < pInstrumentList->size(); i++ )
			{
				memcpy(pBuffer + i*sizeof(InstrumentIDType), 
					pInstrumentList->at(i).c_str(), pInstrumentList->at(i).length());
			}

			COPYDATASTRUCT data;
			ZeroMemory(&data, sizeof(COPYDATASTRUCT));
			data.dwData = MAKELONG(CMDID_QUOTE_CURRENT_GROUP_INST_LIST, SUCCESS);
			data.cbData = sizeof(InstrumentIDType)*pInstrumentList->size();
			data.lpData = pBuffer;
			g_Plusin.Send(data, childHWnd);
			delete[] pBuffer;
			pBuffer = NULL;

			//发送当前选中合约
			InstrumentIDType szInstrumentID;
			memset(szInstrumentID, 0, sizeof(szInstrumentID));
			strcpy(szInstrumentID, strInstrumentID.c_str());

			ZeroMemory( &data, sizeof(COPYDATASTRUCT));
			data.dwData = MAKELONG(CMDID_QUOTE_CURRENT_SEL, SUCCESS);
			data.cbData = sizeof(szInstrumentID);
			data.lpData = szInstrumentID;
			g_Plusin.Send(data, childHWnd);
		}
	}
}
