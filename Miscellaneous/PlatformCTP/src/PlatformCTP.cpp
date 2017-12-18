//#include "stdafx.h"
//
#include "PlatformCTP.h"
#include "tools_ansi.h"
#include "PostMsgByEventParam.h"

#include <map>
using std::make_pair;
using std::multimap;

#pragma unmanaged


//*************************************************************************
//  CPlatformCTP的实现
//*************************************************************************
CPlatformCTP::CPlatformCTP(const Stru_CTPConnectionParam& ConnParam,bool bPushCache,bool bUseComb,bool bCalcuCombQuotOnLegQuot)
:   m_ConnParam(ConnParam),
    m_Cache(bPushCache?40*1024*1024:1024*1024, 0),
    m_bPushCache(bPushCache),
    m_bUseComb(bUseComb),
    m_bCalcuCombQuotOnLegQuot(bCalcuCombQuotOnLegQuot),
    m_ConnStatus(CTPCONNSTATUS_UnInit),
	m_ConnStatusTime(0),
    m_RequestID(0)
{
}

char* CPlatformCTP::GetConnStatusName(eCTPCONNSTATUS sta)
{
	switch(sta)
	{
		case CTPCONNSTATUS_Disconnected   : return "Disconnected";
		case CTPCONNSTATUS_Connecting     : return "Connecting";
		case CTPCONNSTATUS_Logining       : return "Logining";
		case CTPCONNSTATUS_LoginFailure   : return "LoginFailure";
		case CTPCONNSTATUS_Connected      : return "Connected";
		case CTPCONNSTATUS_Disconnecting  : return "Disconnecting";
		case CTPCONNSTATUS_UnInit         : return "Uninit";
		default: return "Unknown";
	}
}

const char* CPlatformCTP::GetConnStatus2(void) const
{
	return GetConnStatusName(m_ConnStatus);
}

void CPlatformCTP::SetConnStatus(eCTPCONNSTATUS connsta,bool bQuot) 
{ 
	m_ConnStatus=connsta; 
	m_ConnStatusTime=time(NULL);
	char buf[256];
	memset(buf,0,sizeof(buf));
	_snprintf(buf,sizeof(buf)-1,"%s:%s",bQuot?"Quot":"Trade",GetConnStatus2());
	CPostMsgByEventParam::getObj().postMsgStr(bQuot?QuotConnMsg:TradeConnMsg,0,string(buf),NULL,0,(int)m_ConnStatus);
}


//生成连接字符串
//正常情况：tcp://...
//使用代理：socks5:// asp-sim2-front1.financial-trading-platform.com:26205/user:pass@127.0.0.1:10001
string CPlatformCTP::MakeConnectStr(const string& addr,const string& ProxyStr)
{
	//去掉左右空白
	std::string addrstr=addr;
	CTools_Ansi::mytrim(addrstr);
	//添加“tcp://”字符串
	if(addrstr.find("tcp://")!=0) addrstr.insert(0,"tcp://");
	//代理格式："socks5:// asp-sim2-front1.financial-trading-platform.com:26205/user:pass@127.0.0.1:10001"
	if(!ProxyStr.empty())
	{
		int pos=ProxyStr.find("://");
		if(pos!=string::npos)
		{
			addrstr.replace(0,3,ProxyStr.substr(0,pos));
			addrstr+=ProxyStr.substr(pos+2);		
		}
	}
	return addrstr;
}

void CPlatformCTP::SubscribePkg(ePkgCmd pkgID,int hWnd)
{
	CeasymutexGuard guard(m_mutex);
	if(!CTools_Ansi::IsKeyValueInMultimap<ePkgCmd,int>(m_subscribePkg,pkgID,hWnd))
		m_subscribePkg.insert(make_pair(pkgID,hWnd));
}
void CPlatformCTP::UnsubscribePkg(ePkgCmd pkgID,int hWnd)
{
	CeasymutexGuard guard(m_mutex);
	CTools_Ansi::EraseKeyValueInMultimap<ePkgCmd,int>(m_subscribePkg,pkgID,hWnd);
}
void CPlatformCTP::UnsubscribePkg(int hWnd)
{
	CeasymutexGuard guard(m_mutex);
	CTools_Ansi::EraseValueInMultimap<ePkgCmd,int>(m_subscribePkg,hWnd);
}
void CPlatformCTP::UnsubscribePkgAll()
{
	CeasymutexGuard guard(m_mutex);
	m_subscribePkg.clear();
}
//这个只能在派生类里使用，由使用者负责加锁
void CPlatformCTP::PostPkg_Internal(ePkgCmd pkgID,void* pdata,int datalen,void* pdata2,int datalen2,int userInt1,int userInt2)
{
	if(m_subscribePkg.empty()) return;
	multimap<ePkgCmd,int>::const_iterator it_lower=m_subscribePkg.lower_bound(pkgID);
	multimap<ePkgCmd,int>::const_iterator it_upper=m_subscribePkg.upper_bound(pkgID);
	while(it_lower!=it_upper)
	{
		CPostMsgByEventParam::getObj().postMsgStr2(it_lower->second,(int)DataArrivedMsg,(int)pkgID,string(""),pdata,datalen,pdata2,datalen2,userInt1,userInt2);
		it_lower++;
	}
}
