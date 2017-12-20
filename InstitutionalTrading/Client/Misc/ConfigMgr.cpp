// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ConfigMgr.h"
#include "MyXML.h"



#pragma unmanaged


//是否已经加载
bool CConfigManager::m_bLoaded=false;

//服务器地址
string CConfigManager::m_SvrIP("");
unsigned short CConfigManager::m_SvrPort=0;
//行情服务器地址
string CConfigManager::m_QuotSvrIP("");
unsigned short CConfigManager::m_QuotSvrPort=0;
//本地地址
string CConfigManager::m_LocalIP("");
unsigned short CConfigManager::m_LocalPort=0;
//自选合约
vector<string> CConfigManager::m_vecExchange2Inst;
//dataGriadView显示列
// vector<string> CConfigManager::m_vecDataGridView2Column;
//风控终端合约信息保留字段
// vector<string> CConfigManager::m_vecInstrumentInfo;

//内部变量
Ceasymutex CConfigManager::m_mutex;
unsigned int CConfigManager::m_UpdateCounter=0;


void CConfigManager::Load()
{
//	char szLocalPath[256];
//	memset(szLocalPath, 0, 256);
//	GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath)-1 );
	
//	string strPathFileName( szLocalPath );
//	int nPos = strPathFileName.rfind( '\\' );
//	if ( nPos == -1) return;
//	strPathFileName = strPathFileName.substr( 0, nPos + 1 ) + string("cfg.xml");

    string strValue;
	string strPathFileName( "cfg.xml" );

	m_mutex.lock();

	if(1)
	{
		//保证doc在unlock()之前析构掉
		CMyXML myxml(strPathFileName.c_str());
		if(myxml.m_bvalid)
		{
			if(!myxml.GetNodeContent(m_SvrIP,"root","Server","IP"))
				m_SvrIP="";
			int tmpport;
			if(!myxml.GetNodeContentInt(tmpport,"root","Server","Port"))
				tmpport=0;
			m_SvrPort=(unsigned short)tmpport;

            if(!myxml.GetNodeContent(m_QuotSvrIP,"root","QuotServer","IP"))
                m_QuotSvrIP="";
            if(!myxml.GetNodeContentInt(tmpport,"root","QuotServer","Port"))
                tmpport=0;
            m_QuotSvrPort=(unsigned short)tmpport;

			if(!myxml.GetNodeContent(m_LocalIP,"root","Local","IP"))
				m_LocalIP="";
			if(!myxml.GetNodeContentInt(tmpport,"root","Local","Port"))
				tmpport=0;
			m_LocalPort=(unsigned short)tmpport;
            
            myxml.GetSubContents(m_vecExchange2Inst,"root","SelectedInstSection");
//             myxml.GetSubContents(m_vecDataGridView2Column,"root","SelectedColumnSection");
//             myxml.GetSubContents(m_vecInstrumentInfo,"root","InstrumentInfo");

			m_UpdateCounter++;
			m_bLoaded=true;
		}
	}

	m_mutex.unlock();
}
void CConfigManager::SaveSelectedInst(map<string,vector<string>>& mapExchange2Inst)
{
    if(mapExchange2Inst.empty())
        return;

    string strPathFileName( "cfg.xml" );

    m_mutex.lock();

    //保证doc在unlock()之前析构掉
    CMyXML myxml(strPathFileName.c_str());
    if(myxml.m_bvalid)
    {
        vector<string> vecValue;
        for (map<string,vector<string>>::iterator it=mapExchange2Inst.begin();it!=mapExchange2Inst.end();it++)
        {
            string szTemp = it->first;
            int s=szTemp.find_first_not_of(" \t");
            int e=szTemp.find_last_not_of(" \t");
            szTemp=szTemp.substr(s,e-s+1);

            char szBuffer[5120];
            memset(szBuffer,0,sizeof(szBuffer));

            char szExchange[5120];
            memset(szExchange,0,sizeof(szExchange));
            strcpy(szExchange,szTemp.c_str());

            char szInst[5120];
            memset(szInst,0,sizeof(szInst));
            int len;
            for (vector<string>::iterator it2=it->second.begin();it2!=it->second.end();it2++)
            {
                strcat(szInst,(*it2).c_str());
                len = strlen(szInst);
                szInst[len]=';';
            }

            len = strlen(szInst);
            szInst[len-1]='\0';            

            sprintf(szBuffer,"%s %s",szExchange,szInst);   
            
            string szItem(szBuffer);
            vecValue.push_back(szItem);  
            
            myxml.ModifyOrAppendSubNodes_save("key",vecValue,"root","SelectedInstSection");
        }

        m_UpdateCounter--;
        m_bLoaded=false;
    }


    m_mutex.unlock();
}

// void CConfigManager::SaveSelectedColumn(map<string,vector<string>>& mapDataGridView2Column)
// {
//     if(mapDataGridView2Column.empty())
//         return;
// 
//     string strPathFileName( "cfg.xml" );
// 
//     m_mutex.lock();
// 
//     //保证doc在unlock()之前析构掉
//     CMyXML myxml(strPathFileName.c_str());
//     if(myxml.m_bvalid)
//     {
//         for (map<string,vector<string>>::iterator it=mapDataGridView2Column.begin();it!=mapDataGridView2Column.end();it++)
//         {
//             string szTemp = it->first;
//             int s=szTemp.find_first_not_of(" \t");
//             int e=szTemp.find_last_not_of(" \t");
//             szTemp=szTemp.substr(s,e-s+1);
// 
//             char szBuffer[512];
//             memset(szBuffer,0,sizeof(szBuffer));
// 
//             char szDataGridView[64];
//             memset(szDataGridView,0,sizeof(szDataGridView));
//             strcpy(szDataGridView,szTemp.c_str());
// 
//             char szColumn[128];
//             memset(szColumn,0,sizeof(szColumn));
// 
//             int len;
//             for (vector<string>::iterator it2=it->second.begin();it2!=it->second.end();it2++)
//             {
//                 strcat(szColumn,(*it2).c_str());
//                 len = strlen(szColumn);
//                 szColumn[len]=';';
//             }
// 
//             len = strlen(szColumn);
//             szColumn[len-1]='\0';            
// 
//             sprintf(szBuffer,"%s %s",szDataGridView,szColumn);   
// 
//             string szItem(szBuffer);
// 
//             CMyXML xmlNode(szDataGridView,szItem);
//             myxml.ModifyOrAppendNode_save(xmlNode,"root","SelectedColumnSection");
//         }
// 
//         m_UpdateCounter--;
//         m_bLoaded=false;
//     }
// 
// 
//     m_mutex.unlock();
// }
// 
// void CConfigManager::SaveInstrumentInfo(map<string,vector<string>>& mapInstrumentInfo)
// {
//     if(mapInstrumentInfo.empty())
//         return;
// 
//     string strPathFileName( "cfg.xml" );
// 
//     m_mutex.lock();
// 
//     //保证doc在unlock()之前析构掉
//     CMyXML myxml(strPathFileName.c_str());
//     if(myxml.m_bvalid)
//     {
//         for (map<string,vector<string>>::iterator it=mapInstrumentInfo.begin();it!=mapInstrumentInfo.end();it++)
//         {
//             string szTemp = it->first;
//             int s=szTemp.find_first_not_of(" \t");
//             int e=szTemp.find_last_not_of(" \t");
//             szTemp=szTemp.substr(s,e-s+1);
// 
//             char szBuffer[512];
//             memset(szBuffer,0,sizeof(szBuffer));
// 
//             char szInst[64];
//             memset(szInst,0,sizeof(szInst));
//             strcpy(szInst,szTemp.c_str());
// 
//             char szValue[128];
//             memset(szValue,0,sizeof(szValue));
// 
//             int len;
//             for (vector<string>::iterator it2=it->second.begin();it2!=it->second.end();it2++)
//             {
//                 strcat(szValue,(*it2).c_str());
//                 len = strlen(szValue);
//                 szValue[len]=';';
//             }
// 
//             len = strlen(szValue);
//             szValue[len-1]='\0';            
// 
//             sprintf(szBuffer,"%s %s",szInst,szValue);   
// 
//             string szItem(szBuffer);
// 
//             CMyXML xmlNode(szInst,szItem);
//             myxml.ModifyOrAppendNode_save(xmlNode,"root","InstrumentInfo");
//         }
// 
//         m_UpdateCounter--;
//         m_bLoaded=false;
//     }
// 
// 
//     m_mutex.unlock();
// }
// 
bool CConfigManager::GetSvrAddr(string& SvrIP,unsigned short& SvrPort)
{
	if(!m_bLoaded) { SvrIP=string("");  SvrPort=0;  return false; }
	m_mutex.lock();
	SvrIP=m_SvrIP;
	SvrPort=m_SvrPort;
	m_mutex.unlock();
	return true;
}

bool CConfigManager::GetQuotSvrAddr(string& QuotSvrIP,unsigned short& QuotSvrPort)
{
    if(!m_bLoaded) { QuotSvrIP=string("");  QuotSvrPort=0;  return false; }
    m_mutex.lock();
    QuotSvrIP=m_QuotSvrIP;
    QuotSvrPort=m_QuotSvrPort;
    m_mutex.unlock();
    return true;
}

bool CConfigManager::GetLocalAddr(string& LocalIP,unsigned short& LocalPort)
{
	if(!m_bLoaded) { LocalIP=string("");  LocalPort=0;  return false; }
	m_mutex.lock();
	LocalIP=m_LocalIP;
	LocalPort=m_LocalPort;
	m_mutex.unlock();
	return true;
}

bool CConfigManager::GetAutoSelectedInst(vector<string>& vecExchange2Inst)
{
    if(!m_bLoaded)
        return false;

    m_mutex.lock();
    vecExchange2Inst = m_vecExchange2Inst;
    m_mutex.unlock();
    return true;
}

// bool CConfigManager::GetAutoSelectedColumn(vector<string>& vecDataGridView2Column)
// {
//     if(!m_bLoaded)
//         return false;
// 
//     m_mutex.lock();
//     vecDataGridView2Column = m_vecDataGridView2Column;
//     m_mutex.unlock();
//     return true;
// }
// bool CConfigManager::GetInstrumentInfo(vector<string>& vecInstrumentInfo)
// {
//     if(!m_bLoaded)
//         return false;
// 
//     m_mutex.lock();
//     vecInstrumentInfo = m_vecInstrumentInfo;
//     m_mutex.unlock();
//     return true;
// }

string CConfigManager::GetSvrAddr()
{
    //为保证取得的地址是当前最新的，每次要重新读取
    string strValue;
    string strPathFileName( "cfg.xml" );

    m_mutex.lock();

    CMyXML myxml(strPathFileName.c_str());
    if(myxml.m_bvalid)
    {
        if(!myxml.GetNodeContent(m_SvrIP,"root","Server","IP"))
            m_SvrIP="";
        int tmpport;
        if(!myxml.GetNodeContentInt(tmpport,"root","Server","Port"))
            tmpport=0;
        m_SvrPort=(unsigned short)tmpport;
    }

    char szbuf[128];
    memset(szbuf,0,sizeof(szbuf));
	sprintf(szbuf,"%s:%d",m_SvrIP.c_str(),m_SvrPort);

    m_mutex.unlock();
    return (string)szbuf;    
}
string CConfigManager::GetQuotSvrAddr()
{
    //为保证取得的地址是当前最新的，每次要重新读取
    string strValue;
    string strPathFileName( "cfg.xml" );

    m_mutex.lock();

    CMyXML myxml(strPathFileName.c_str());
    if(myxml.m_bvalid)
    {
        int tmpport;
        if(!myxml.GetNodeContent(m_QuotSvrIP,"root","QuotServer","IP"))
            m_QuotSvrIP="";
        if(!myxml.GetNodeContentInt(tmpport,"root","QuotServer","Port"))
            tmpport=0;
        m_QuotSvrPort=(unsigned short)tmpport;
    }

    char szbuf[128];
    memset(szbuf,0,sizeof(szbuf));
    sprintf(szbuf,"%s:%d",m_QuotSvrIP.c_str(),m_QuotSvrPort);

    m_mutex.unlock();
    return (string)szbuf;    
}