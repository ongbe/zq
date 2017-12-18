#ifndef _RiskEvt2_H_
#define _RiskEvt2_H_

#include <string>
using namespace std;

class RiskEvt2
{
public:
	//订阅事件
	static void SubscribeEvt(unsigned int hWnd);
	//退订事件
	static void UnsubscribeEvt(unsigned int hWnd);

	//向服务器请求风险事件
	static void ReqRiskEvt(void);

	//处理服务器推送的风险事件消息
	static bool ProcessRiskEvtPkg(const char* pPkg,string*pstr,int fieldcnt);

};

#endif


