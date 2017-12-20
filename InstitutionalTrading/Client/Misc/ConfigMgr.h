#ifndef _CONFIGERMANAGER_H_
#define _CONFIGERMANAGER_H_

#include "easymutex.h"

#include <string>
#include <map>
#include <vector>
using namespace std;

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CConfigManager
{
public:
	static void Load();
	static bool IsLoaded(void) { return m_bLoaded; }
	static unsigned int GetUpdateCounter(void) { return m_UpdateCounter; }
	static bool GetSvrAddr(string& SvrIP,unsigned short& SvrPort);
	static bool GetLocalAddr(string& LocalIP,unsigned short& LocalPort);

    static bool GetQuotSvrAddr(string& QuotSvrIP,unsigned short& QuotSvrPort);

    static void SaveSelectedInst(map<string,vector<string>>& mapExchange2Inst);
    static bool GetAutoSelectedInst(vector<string>& vecExchange2Inst);
//     static void SaveSelectedColumn(map<string,vector<string>>& mapDataGridView2Column);
//     static bool GetAutoSelectedColumn(vector<string>& vecDataGridView2Column);
//     static void SaveInstrumentInfo(map<string,vector<string>>& mapInstrumentInfo);
//     static bool GetInstrumentInfo(vector<string>& vecInstrumentInfo);

    static string GetSvrAddr();
    static string GetQuotSvrAddr();

public:
	//是否已经加载
	static bool m_bLoaded;

	//服务器地址
	static string m_SvrIP;
	static unsigned short m_SvrPort;

    //行情服务器地址
    static string m_QuotSvrIP;
    static unsigned short m_QuotSvrPort;

    //自选合约
    static vector<string> m_vecExchange2Inst;
    //自选dataGridView列表
    //static vector<string> m_vecDataGridView2Column;
    //风控终端合约信息保留字段
    //static vector<string> CConfigManager::m_vecInstrumentInfo;

	//本地地址，可以不设置，或设置为零。在多网卡机器上需要设置
	static string m_LocalIP;
	static unsigned short m_LocalPort;

private:
	static Ceasymutex m_mutex;
	static unsigned int m_UpdateCounter;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


