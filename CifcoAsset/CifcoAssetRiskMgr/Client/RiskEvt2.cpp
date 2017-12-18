#include "StdAfx.h"
#include "RiskEvt2.h"


#include <windows.h>
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"

//订阅事件
void RiskEvt2::SubscribeEvt(unsigned int hWnd)
{
//	CTcpLayer::SubscribePkg(Cmd_RM_SubscribeRiskEvent_Req,(int)hWnd);
//	CTcpLayer::SubscribePkg(Cmd_RM_SubscribeRiskEvtHandling_Req,(int)hWnd);
	
}
//退订事件
void RiskEvt2::UnsubscribeEvt(unsigned int hWnd)
{
	CTcpLayer::UnsubscribeAllPkg((int)hWnd);
}


//向服务器请求风险事件
void RiskEvt2::ReqRiskEvt(void)
{
	char buf[sizeof(Stru_UniPkgHead)+4];
	Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
	PkgHead.Set(0,Cmd_RM_SubscribeRiskEvent_Rsp,0,0);
	*(unsigned int*)(buf+sizeof(PkgHead))=TailID_UniPkg;

	CTcpLayer::Senddata(buf,sizeof(buf));
}

//处理服务器推送的风险事件消息
bool RiskEvt2::ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt)
{
	for(int i=0;i<fieldcnt;i++)
	{
		char buf[256];
		itoa(i,buf,10);
		pstr[i].assign(buf);
	}
	return true;
}


