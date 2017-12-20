#pragma once
#include "ThostFtdcMdApi.h"
#include "KernelStruct.h"
#include "CommonStruct.h"
#include "CommonPublicDef.h"
#include "DBFRecordset.h"
#include "easymutex.h"
#include <map>
#include <vector>
#include <queue>
using namespace std;

class CFileMonitor
{
public:
    //定义文件目录改变后的回调函数指针
    typedef  void ( *lpFunDealFile )(  tagACTION action, LPCTSTR lpszFileSrc, LPCTSTR lpszFileDst );
	static CThostFtdcMdSpi *m_pThostFtdcMdSpi;
	static void CopyDepthMarketData( const PlatformStru_DepthMarketData& data, CThostFtdcDepthMarketDataField& depthMarketData );


public:
    CFileMonitor();
    ~CFileMonitor();

    /*
    * 设置回调函数
    */
    void  SetDealFilePtr( lpFunDealFile pFunDeal );
    BOOL  StartMonitor( LPCTSTR lpszDir );
    void  EndMonitor();
    BOOL  IsMoniting();

    bool Analyse1( LPCTSTR lpszDir, vector<PlatformStru_DepthMarketData>& vShow2003, string& stTime);
    bool Analyse2( LPCTSTR lpszDir, vector<PlatformStru_DepthMarketData>& vSJSHQ, string& stTime );

private:
    string Parase(int nCol, const COleVariant& varValue, bool bShow2003=true);

public:
    volatile    BOOL    m_bContinue;

    HANDLE   m_hDir;    
    HANDLE   m_hThread;
    lpFunDealFile  m_pFunDeal;

    static DWORD WINAPI  MonitorProc ( LPVOID lParam );

public:
	bool m_bQuitSendQuot;
	HANDLE  m_hQuitSendQuotEvent;

	void Init();
	void Release();
	void GetQueueQuot(std::vector<PlatformStru_DepthMarketData>& vecData);

private:
    string              m_strTimeOffShow2003;
    string              m_strTimeOffSJSHQ;
    CDBFRecordset       m_SetShow2003;
    CDBFRecordset       m_SetSJSHQ;

	//
	Ceasymutex      m_mutex;
	HANDLE	m_hThreadSendQuot;
	DWORD	m_idThreadSendQuot; 
	queue<PlatformStru_DepthMarketData>      m_queueQuot; //保存解析后行情数据，发送给SPI

	//用于比较数据改变 
	map<string, PlatformStru_DepthMarketData> m_mapPreShow2003;
	map<string, PlatformStru_DepthMarketData> m_mapPreSJSHQ;
};
