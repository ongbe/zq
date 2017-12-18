// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ConfigMgr.h"
#include "inc\MyXML.h"

#ifdef _DEBUG
	#pragma comment(lib, "ToolsD.lib")
#else
	#pragma comment(lib, "Tools.lib")
#endif



#pragma unmanaged


//是否已经加载
bool CConfigManager::m_bLoaded=false;

//服务器地址
string CConfigManager::m_SvrIP("");
unsigned short CConfigManager::m_SvrPort=0;
//本地地址
string CConfigManager::m_LocalIP("");
unsigned short CConfigManager::m_LocalPort=0;

//内部变量
Ceasymutex CConfigManager::m_mutex;
unsigned int CConfigManager::m_UpdateCounter=0;
map<string, vector<EXCHANGERESTTIME>> CConfigManager::m_mapRestTime;

PLOTCTRLHEADCFG CConfigManager::m_plotCtrlHead = {0};
PLOTCTRLITEMCFG CConfigManager::m_plotCtrlItems[conMaxPlotCtrlItemNum] = {0};

void CConfigManager::Load()
{
//	char szLocalPath[256];
//	memset(szLocalPath, 0, 256);
//	GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath)-1 );
	
//	string strPathFileName( szLocalPath );
//	int nPos = strPathFileName.rfind( '\\' );
//	if ( nPos == -1) return;
//	strPathFileName = strPathFileName.substr( 0, nPos + 1 ) + string("cfg.xml");

	string strPathFileName( "cfg.xml" );

	m_mutex.lock();

	LoadExchangeTime();
	
	if(1)
	{
		//保证doc在unlock()之前析构掉
		CMyXML myxml(strPathFileName);
		if(myxml.m_bvalid)
		{
			if(!myxml.GetNodeContent(m_SvrIP,"root","Server","IP"))
				m_SvrIP="";
			int tmpport;
			if(!myxml.GetNodeContentInt(tmpport,"root","Server","Port"))
				tmpport=0;
			m_SvrPort=(unsigned short)tmpport;

			if(!myxml.GetNodeContent(m_LocalIP,"root","Local","IP"))
				m_LocalIP="";
			if(!myxml.GetNodeContentInt(tmpport,"root","Local","Port"))
				tmpport=0;
			m_LocalPort=(unsigned short)tmpport;

			m_UpdateCounter++;
			m_bLoaded=true;
		}
	}

	m_plotCtrlHead.maxLoseForDay = 9.9e180;				// 日内最大亏损额
	m_plotCtrlHead.forceCloseTime = 0x7FFFFFFF;			// 强平倒计时
	m_plotCtrlHead.cancelForceTime = 0x7FFFFFFF;		// 稳固时间
	m_plotCtrlHead.fcPriceUp = 0;						// 强平滑点
	m_plotCtrlHead.plotItemCount = 0;					// 附加的策略合约配置项

#if 1

	CFile file;
	int nCount = 0;
	CString strFile;
	CString strText;
	strFile = GetRealPath();
	strFile += CONFIGFILENAME;

	if(file.Open(strFile, CFile::modeRead|CFile::shareDenyNone)) {
		
		PLOTCTRLHEADCFG head;
		ZeroMemory(&head, sizeof(head));
		file.Read(&head, sizeof(head));
		m_plotCtrlHead = head;

		PLOTCTRLITEMCFG item;
		ZeroMemory(&item, sizeof(item));
		while(file.Read(&item, sizeof(item))==sizeof(item)) {
			strText = item.plotID;
			strText.MakeLower();
			strncpy(item.plotID, strText, sizeof(item.plotID)-1);
			m_plotCtrlItems[nCount] = item;
			nCount++;
			ZeroMemory(&item, sizeof(item));
		}

		file.Close();
		if(m_plotCtrlHead.plotItemCount != nCount)
			ZeroMemory(&m_plotCtrlHead, sizeof(m_plotCtrlHead));
	}


#else
	m_plotCtrlHead.maxLoseForDay = -100000.0;		// 日内最大亏损额
	m_plotCtrlHead.forceCloseTime = 180;			// 强平倒计时
	m_plotCtrlHead.cancelForceTime = 30;			// 稳固时间
	m_plotCtrlHead.fcPriceUp = 5;					// 强平滑点
	m_plotCtrlHead.plotItemCount = 0;				// 附加的策略合约配置项

	char csInstrumentID[64];
	ZeroMemory(csInstrumentID, sizeof(csInstrumentID));
	for(int i=0; i<12; i++) {
		int nIndex = m_plotCtrlHead.plotItemCount;
		sprintf(csInstrumentID, "IF13%02d", i+1);	
		strcpy(m_plotCtrlItems[nIndex].plotID, "jianciif");
		strcpy(m_plotCtrlItems[nIndex].inst, csInstrumentID);
		m_plotCtrlItems[nIndex].stopGainPoint = 20;
		m_plotCtrlItems[nIndex].stopLosePoint = 20;
		m_plotCtrlItems[nIndex].maxPosiVol = 10;
		m_plotCtrlItems[nIndex].singlePosiTime = 180;

		m_plotCtrlHead.plotItemCount++;
	}
	for(int i=0; i<12; i++) {
		int nIndex = m_plotCtrlHead.plotItemCount;
		sprintf(csInstrumentID, "IF13%02d", i+1);	
		strcpy(m_plotCtrlItems[nIndex].plotID, "plottest");
		strcpy(m_plotCtrlItems[nIndex].inst, csInstrumentID);
		m_plotCtrlItems[nIndex].stopGainPoint = 25;
		m_plotCtrlItems[nIndex].stopLosePoint = 25;
		m_plotCtrlItems[nIndex].maxPosiVol = 20;
		m_plotCtrlItems[nIndex].singlePosiTime = 120;

		m_plotCtrlHead.plotItemCount++;
	}

#endif

	m_mutex.unlock();
}

bool CConfigManager::GetSvrAddr(string& SvrIP,unsigned short& SvrPort)
{
	if(!m_bLoaded) { SvrIP=string("");  SvrPort=0;  return false; }
	m_mutex.lock();
	SvrIP=m_SvrIP;
	SvrPort=m_SvrPort;
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

void CConfigManager::LoadExchangeTime()
{
	CString strPath;
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	strPath = filename.substr(0, splitpos+1).c_str();

	CString strCfgFile;
	strCfgFile.Format("%sCfg.xml", strPath);

	TiXmlDocument* pXmlDocment = new TiXmlDocument(strCfgFile);
	if(pXmlDocment == NULL) 
		return;

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() ) 
		return;

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( root == NULL ) 
		return;

	if( std::string(root->Value()) != "root")
		return;

	//各交易所交易时间配置文件

	TiXmlElement *tList = root->FirstChildElement("resttime");
	if(tList == NULL)
		return;

	int nStartTime = 0, nEndTime = 0, nSubNum = 0;
	int nHour = 0, nMinite = 0;
	EXCHANGERESTTIME subTime;

	for(TiXmlElement *GroupItem = tList->FirstChildElement("group");
			GroupItem; 
			GroupItem = GroupItem->NextSiblingElement("group")) {

		const char *pGroupid = GroupItem->Attribute("id");
		if(!pGroupid) 
			continue;

		string strID = pGroupid;
		const char *pGrouptime = GroupItem->Attribute("time");
		if(!pGrouptime)
			continue;

		vector<EXCHANGERESTTIME> vecRestTime;
		string str = pGrouptime;
		vector<string> vecTime;
		split(str, ' ', vecTime);
		for(int i = 0; i < (int)vecTime.size(); i++) {
			string strTime = vecTime[i];
			vector<string> vecOne;
			split(strTime, '-', vecOne);
			if(vecOne.size() != 2)
				break;

			nHour = 0, nMinite = 0;
			sscanf(vecOne[0].c_str(), "%d:%d", &nHour, &nMinite);
			nStartTime = nHour*3600 + nMinite*60;
			nHour = 0, nMinite = 0;
			sscanf(vecOne[1].c_str(), "%d:%d", &nHour, &nMinite);
			nEndTime = nHour*3600 + nMinite*60;

			nSubNum = nEndTime - nStartTime;

			if(nEndTime<=nStartTime)
				break;

			ZeroMemory(&subTime, sizeof(subTime));
			subTime.nStart = nStartTime;
			subTime.nEnd = nEndTime;
			subTime.nSubNum = nSubNum;

			vecRestTime.push_back(subTime);
		}
		m_mapRestTime.insert(std::make_pair(strID,  vecRestTime));			
	}
	//delete pXmlDocment;
}
