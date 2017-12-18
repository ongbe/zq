#ifndef _CQueryTransfer_H_
#define _CQueryTransfer_H_

#include <string>
using namespace std;

class CQueryTransfer
{
public:
	//订阅事件
	static void SubscribeEvt(unsigned int hWnd);
	//退订事件
	static void UnsubscribeEvt(unsigned int hWnd);

	//向服务器请求投资者出入金
	static void ReqQueryTransfer(std::string strAccountID);



};

#endif
