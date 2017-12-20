#include "stdafx.h"

#include "Manage.h"
#include "shellapi.h"
CManage::CManage(void)
{
	
}

CManage::~CManage(void)
{
}
void CManage::Init()
{
	m_DataCenter.Init();
	StartAllInstance();
}
bool CManage::BeginSStrategyInstanceProcess(SStrategyInstance& instance)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si,0,sizeof(si));
	si.cb=sizeof(si);
	si.wShowWindow=SW_SHOW;
	si.dwFlags=STARTF_USESHOWWINDOW;

	std::string strXML;
	loadXMLString(instance, strXML);
	std::string strFileFolder = "Strategy";
	std::string strFileName   = "RunSingleStrategy.exe";
	GetFolderFileName(strFileName, strFileFolder);

	std::string strCmd = strFileName + " \"" + strXML + "\"";	
	
	bool bRet = CreateProcess(NULL,(LPSTR)strCmd.c_str(),NULL, NULL, FALSE, 0, NULL, (LPSTR)strFileFolder.c_str(), &si,&pi);
	STraderInstance key(instance.strategyName, instance.strTraderName);
	m_lockInstance2Handle.write_lock();
	m_mapInstance2Handle[key] = pi.hProcess;
	m_lockInstance2Handle.write_unlock();
	return bRet;
}
void CManage::StartAllInstance()
{
	std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
	m_DataCenter.GetAllInstance(mapStrategyInstance);
	std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
	//	if(instance.nStart == 1)
		{//运行
			BeginSStrategyInstanceProcess(instance);
		}
		it++;
	}
	
}
void CManage::StopAllInstance()
{
	std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
	m_DataCenter.GetAllInstance(mapStrategyInstance);
	std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
// 	//	if(instance.nStart == 1)
 		{//先停用
 			CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(instance.strTraderName, instance.strategyName, 1);			
 		}
		it++;
	}
	Sleep(2000);
	it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
		//	if(instance.nStart == 1)
		{//运行
			TerminatInstanceThread(instance.strategyName, instance.strTraderName);			
		}
		it++;
	}
}
void CManage::HandleAllInstanceTimer(bool bStop)
{
    std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
    m_DataCenter.GetAllInstance(mapStrategyInstance);
    std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
    while(it != mapStrategyInstance.end())
    {
        SStrategyInstance& instance = it->second;
        if(instance.bStartTimer)
        {           
            //不用停用（因为主从断开后就没有行情驱动去下单了），只是关闭启动的定时器
            CInterface_SvrTradeData::getObj().PushStrategyTimer(instance.strTraderName, instance.strategyName, 1);
        }
        it++;
    }
}
bool CManage::TerminatInstanceThread(std::string strName, std::string strUploader)
{
	m_lockInstance2Handle.write_lock();
	STraderInstance key(strName.c_str(), strUploader.c_str());
	std::map<STraderInstance, HANDLE>::iterator it =  m_mapInstance2Handle.find(key);
	if(it != m_mapInstance2Handle.end())
	{
		HANDLE hd = it->second;
		::TerminateProcess(hd, 4);
	}
	m_lockInstance2Handle.write_unlock();
	return true;
}
void CManage::string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst )
{	
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
	{
		strBig.replace( pos, srclen, strdst );
		pos += dstlen;
	}
} 
bool CManage::loadXMLString(SStrategyInstance& instance, std::string& strXML)
{

	TiXmlDocument *myDocument = new TiXmlDocument();
//	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","gb2312","no");  
//	myDocument->LinkEndChild(decl);  


	//创建一个根元素并连接。
	TiXmlElement *RootElement = new TiXmlElement("strategy");

	myDocument->LinkEndChild(RootElement);

	//创建一个Person元素并连接。
	TiXmlElement *strategyName = new TiXmlElement("strategyName");
	RootElement->LinkEndChild(strategyName);
	TiXmlText *strategyNameContent = new TiXmlText(instance.strategyName);
	strategyName->LinkEndChild(strategyNameContent);

	TiXmlElement *traderName = new TiXmlElement("traderName");
	RootElement->LinkEndChild(traderName);
	TiXmlText *traderNameContent = new TiXmlText(instance.strTraderName);
	traderName->LinkEndChild(traderNameContent);

	TiXmlElement *para = new TiXmlElement("parameter");
	RootElement->LinkEndChild(para);

	char szPara[2555];
	sprintf(szPara,"%s", instance.strPara);
//	sprintf(szPara,"%s","<para\>test</para>");
	TiXmlText *paraContent = new TiXmlText(szPara);	
	para->LinkEndChild(paraContent);
	
	
	TiXmlElement *instrument = new TiXmlElement("instrument");
	RootElement->LinkEndChild(instrument);
	TiXmlText *instrumentContent = new TiXmlText(instance.Instruments);
	instrument->LinkEndChild(instrumentContent);

	int nFrontID =0, nSessionID =0;
	CInterface_SvrLogin::getObj().GenerateUniqueFrontAndSessionID(nFrontID, nSessionID);
	char szFrontID[55];
	sprintf(szFrontID,"%d",nFrontID);
	char szSessionID[55];
	sprintf(szSessionID,"%d",nSessionID);

	TiXmlElement *sessionID = new TiXmlElement("sessionID");
	RootElement->LinkEndChild(sessionID);
	TiXmlText *sessionIDContent = new TiXmlText(szSessionID);
	sessionID->LinkEndChild(sessionIDContent);

	TiXmlElement *FrontID = new TiXmlElement("frontID");
	RootElement->LinkEndChild(FrontID);
	TiXmlText *szFrontIDContent = new TiXmlText(szFrontID);
	FrontID->LinkEndChild(szFrontIDContent);

	TiXmlElement *orderRefID = new TiXmlElement("orderRef");
	RootElement->LinkEndChild(orderRefID);
	TiXmlText *orderRefIDContent = new TiXmlText("1");
	orderRefID->LinkEndChild(orderRefIDContent);
//////////////////////////////////////////////////////////////////////////
	char szStartTimer[55];
	sprintf(szStartTimer,"%d",instance.bStartTimer);

	TiXmlElement *StartTimer = new TiXmlElement("startTimer");
	RootElement->LinkEndChild(StartTimer);
	TiXmlText *sessionStartTimer = new TiXmlText(szStartTimer);
	StartTimer->LinkEndChild(sessionStartTimer);

	char szTimerSpan[55];
	sprintf(szTimerSpan,"%d",instance.nTimerSpan);

	TiXmlElement *TimerSpan = new TiXmlElement("timerSpan");
	RootElement->LinkEndChild(TimerSpan);
	TiXmlText *sessionTimerSpan = new TiXmlText(szTimerSpan);
	TimerSpan->LinkEndChild(sessionTimerSpan);

	
	int nRunAfterOffline =0;
	char szRunAfterOffline[5];
	memset(&szRunAfterOffline,0,sizeof(szRunAfterOffline));
	if(instance.nStart == 1)
	{
		sprintf(szRunAfterOffline,"%d",1);
		if(instance.nRunAfterOffline  == 0)
		{//离线后不运行
			UserInfo lUserInfo;
			bool bGet = CInterface_SvrUserOrg::getObj().GetUserByName(instance.strTraderName,lUserInfo);			
			if(bGet)
			{
				sUserOnlineInfo lsInfo;
				CInterface_SvrLogin::getObj().GetOnlineUserStatus(lUserInfo.nUserID,lsInfo);
				if(lsInfo.mUserOnlineCount == 0)
				{
					sprintf(szRunAfterOffline,"%d",0);
				}
			}
		}		
	}
	else 
		sprintf(szRunAfterOffline,"%d",0);

	TiXmlElement *RunAfterOffline = new TiXmlElement("RunAfterOffline");//0为空转，1为运行；（RunAfterOffline不是交易员里先后暂停运行的意思，名字起错了）
	RootElement->LinkEndChild(RunAfterOffline);
	TiXmlText *ttszRunAfterOffline = new TiXmlText(szRunAfterOffline);
	RunAfterOffline->LinkEndChild(ttszRunAfterOffline);

//////////////////////////////////////////////////////////////////////////
	TiXmlPrinter printer;
	myDocument->Accept(&printer);
	strXML =	printer.CStr();
	string_replace(strXML, "&lt;", "<");
	string_replace(strXML, "&gt;", ">");
	string_replace(strXML, "&apos;", "'");
	string_replace(strXML, "&#x0D;&#x0A;", "");
	string_replace(strXML, "&#x0A;", "");//只有换行符就直接去掉
	string_replace(strXML, "&#x0D;", "");//c#代码不支持回车换行符
	//string_replace(strXML, ">\n;", ">");//c#代码不支持回车换行符
	//myDocument->SaveFile("c:\\1.xml");
	return true;
}
bool CManage::UploadBaseStrategy(SStrategy& strategy, int& nError)
{
	if(strategy.strTraderName =="")
	{//如果上传人为空，则不允许上传
		nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;
		return false;
	}
	SStrategy strategyExist;
	bool bGet = m_DataCenter.GetStrategy(strategy.strategyName, strategyExist);
	if(bGet) {
		if(strcmp(strategyExist.strTraderName, strategy.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
		if(strategyExist.nUse ==1)
		{//策略已经存在，而且处于启用状态
			nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;
			return false;
		}
	}
	//strategy.nUse = 2;//上传的时候先处于禁用，不然的话第二步，第三步上传没有完成的话，下次就没法重新再次上传这个策略了（因为策略处于启用状态）。
	CInterface_SvrDBOpr::getObj().DB_AddStrategy(strategy, nError);
	if(nError != 0)//
		return false;
	m_DataCenter.AddStrategy(strategy);

	std::vector<SStrategy2Index> vec;
	ParsePara(strategy.strPara, vec);
	for(int i =0; i<(int)vec.size(); i++)
	{
		SStrategy2Index& ssI = vec[i];
		bool bSuccess = AddStrategyIndexRelation(ssI.strategyName, ssI.IndexName, nError);	
		if(!bSuccess)
		{
			return false;
		}
		m_DataCenter.AddStrategy2IndexRelation(ssI.strategyName, ssI.IndexName);		
	}

	//log入库信息
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, strategy.strTraderName);
	strcpy(logData.logTargetorID, strategy.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Add;
	sprintf(logData.logContent,"上传策略(策略名:[%s]  交易员:[%s] ",strategy.strategyName, strategy.strTraderName);
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

	return true;
}
bool CManage::ParsePara(std::string strPara, std::vector<SStrategy2Index>& vec)
{
	TiXmlDocument *doc = new TiXmlDocument();    
	doc->Parse(strPara.c_str());   
	if (&doc == NULL)   
		return false;

	TiXmlElement* RootElement=doc->RootElement();
	if(RootElement)
	{
		TiXmlAttribute* attributeOfstrategy = RootElement->FirstAttribute();  //获得strategy的name属性
		std::string strstrategy = attributeOfstrategy->Value();


		const char* pRootElement = RootElement->Value();
		if(strcmp(pRootElement, "strategy") ==0)
		{
			TiXmlElement* pElement=RootElement->FirstChildElement("indicators");
			if(pElement)
			{
				TiXmlElement *pElementindicator = pElement->FirstChildElement("indicator");
				while(pElementindicator)
				{
					TiXmlAttribute* attributeOFindicator = pElementindicator->FirstAttribute();  //获得student的name属性
					std::string strindicator = attributeOFindicator->Value();
					SStrategy2Index sI;
					strcpy(sI.strategyName, strstrategy.c_str());
					strcpy(sI.IndexName, strindicator.c_str());
					vec.push_back(sI);
					pElementindicator = pElementindicator->NextSiblingElement();	
				}
			}

		}			
	}

	return true;
}
bool CManage::AddStrategyIndexRelation(std::string strName, std::string strIndex, int& nError)
{	
	CInterface_SvrDBOpr::getObj().DB_AddStrategy2IndexRelation(strName, strIndex, nError);
	if(nError != 0)//
		return false;
	m_DataCenter.AddStrategy2IndexRelation(strName, strIndex);

	return true;
}
bool CManage::UploadDllFile(std::string strName, std::string strUploader, void* pData, int nLength, int& nError)
{
	SStrategy strategyExist;
	if(m_DataCenter.GetStrategy(strName, strategyExist)) {
		if(strcmp(strUploader.c_str(), strategyExist.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
	}
	std::string strFileFolder = "Strategy";
	std::string strFileName   = strName + ".dll";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "wb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_STRATEGY_SAVEDLLFAIL;
		return false;// 服务端文件存储失败
	}
	fwrite(pData, nLength, 1, pFile);	
	fclose(pFile);
	CInterface_SvrDBOpr::getObj().DB_WriteStragegyFile(strName, UPLOAD_STRATEGY_DLL, (char*)pData, nLength, nError);
	if(nError != 0)
		return false;
	return true;
}
bool CManage::UploadZIPFile(std::string strName, std::string strUploader, void* pData, int nLength, int& nError)
{
	SStrategy strategyExist;
	if(m_DataCenter.GetStrategy(strName, strategyExist)) {
		if(strcmp(strUploader.c_str(), strategyExist.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
	}
	std::string strFileFolder = "Strategy";
	std::string strFileName   = strName + ".zip";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "wb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_STRATEGY_SAVEZIPFAIL;
		return false;// 服务端文件存储失败
	}
	fwrite(pData, nLength, 1, pFile);	
	fclose(pFile);
	CInterface_SvrDBOpr::getObj().DB_WriteStragegyFile(strName, UPLOAD_STRATEGY_ZIP, (char*)pData, nLength, nError);
	if(nError != 0)
		return false;

	m_DataCenter.calcUploadAllInfo(strUploader, strName, 0);
	return true;
}
bool CManage::DownloadAllBaseStategy(std::string strUploader, std::vector<SStrategy>& vecStrategy, int& nError)
{
	bool bGet = m_DataCenter.GetAllUserPriveledgeStrategy(strUploader, vecStrategy);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
		return false;		
	}

	return true;
}
bool CManage::DownloadBaseStrategy(std::string strName/*, std::string strUploader*/, SStrategy& strategy, int& nError)
{
	bool bGet = m_DataCenter.GetStrategy(strName, strategy);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
		return false;		
	}
/*	bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		if(strategy.nPrivate == 1)
		{//该策略不是属于该交易员，而且该策略为私有
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PRIVATE;
			return false;	
		}		
	}	*/
	return true;
}
bool CManage::DownloadDllFile(std::string strName, std::string strUploader, void*& pData, int& nLength, int& nError)
{
	SStrategy strategy;
	bool bGet = m_DataCenter.GetStrategy(strName, strategy);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
		return false;		
	}
	bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		if(strategy.nPrivate == 1)
		{//该策略不是属于该交易员，而且该策略为私有
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PRIVATE;
			return false;	
		}		
	}	
	
	std::string strFileFolder = "Strategy";
	std::string strFileName   = strName + ".dll";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "rb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_READDLLFAIL;
		return false;// 服务端文件存储失败
	}

	fseek(pFile, 0, SEEK_END);
	int lnSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	if(lnSize > 0)
	{
		pData = new char[lnSize+54];
		int n = fread((char*)pData+54,lnSize,1,pFile);
		if(n>0)
		 nLength = lnSize+54;
	}
	fclose(pFile);

	return lnSize>0;
}
bool CManage::DownloadZIPFile(std::string strName, std::string strUploader, void*& pData, int& nLength, int& nError)
{
	SStrategy strategy;
	bool bGet = m_DataCenter.GetStrategy(strName, strategy);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
		return false;		
	}
	bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		//if(strategy.nPrivate == 1)
		{//该策略不是属于该交易员，源程序不允许下载
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PUBLIC;
			return false;	
		}		
	}	

	std::string strFileFolder = "Strategy";
	std::string strFileName   = strName + ".zip";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "rb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_READZIPFAIL;
		return false;// 服务端文件存储失败
	}

	fseek(pFile, 0, SEEK_END);
	int lnSize = ftell(pFile);
	if(lnSize > 0)
	{
		pData = new char[lnSize+54];
		fseek(pFile, 0, SEEK_SET);
		int n = fread((char*)pData+54,lnSize,1,pFile);
		if(n>0)
			nLength = lnSize+54;
	}
	fclose(pFile);

	return lnSize>0;
}

bool CManage::UploadBaseIndex(SIndex& Index, int& nError)
{
	if(Index.strTraderName =="")
	{//如果上传人为空，则不允许上传
		nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;
		return false;
	}
	SIndex oldIndex;
	bool bGet = m_DataCenter.GetIndex(Index.IndexName, oldIndex);
	if(bGet)
	{	
		if(strcmp(oldIndex.strTraderName, Index.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
		std::vector<std::string> vecStrategyName;
		m_DataCenter.GetStrategyofIndex(Index.IndexName, vecStrategyName);		
		for(int i =0; i< (int)vecStrategyName.size(); i++)
		{
			std::string strStrategyName = vecStrategyName[i];
			SStrategy strategyExist;
			bool bGet = m_DataCenter.GetStrategy(strStrategyName, strategyExist);
			if(bGet)
			{
				if(strategyExist.nUse ==1)
				{//策略已经存在，而且处于启用状态
					nError = CF_ERROR_DATA_IndexIsUsing;
					return false;
				}
			}
		}				
	}
	CInterface_SvrDBOpr::getObj().DB_AddIndex(Index, nError);
	if(nError != 0)//
		return false;
	m_DataCenter.AddIndex(Index);

	//log入库信息
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, Index.strTraderName);
	strcpy(logData.logTargetorID, Index.IndexName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Modify;
	sprintf(logData.logContent,"上传指标(指标名:[%s]  交易员:[%s] ",Index.IndexName, Index.strTraderName);
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);
	return true;
}
bool CManage::UploadIndexFile(std::string strName, std::string strUploader, int nType, void* pData, int nLength, int& nError)
{
	SIndex existIndex;
	if(m_DataCenter.GetIndex(strName, existIndex)) {	
		if(strcmp(strUploader.c_str(), existIndex.strTraderName)!=0) {
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
			return false;
		}
	}
	std::string strFileFolder = "Strategy";
	std::string strFileName;
	if(nType == 2)
	   strFileName   = strName + ".dll";
	else if(nType == 3)
		strFileName   = strName + ".zip";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "wb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_INDEX_SAVEDLLFAIL;
		return false;// 服务端文件存储失败
	}
	fwrite(pData, nLength, 1, pFile);	
	fclose(pFile);
	CInterface_SvrDBOpr::getObj().DB_WriteIndexFile(strName, nType, (char*)pData, nLength, nError);
	if(nError != 0)
		return false;

	if(nType == 3)
		m_DataCenter.calcUploadAllInfo(strUploader, strName, 1);
	return true;

}
bool CManage::DownloadAllBaseIndex(std::string strUploader, std::vector<SIndex>& vecIndex, int& nError)
{
	return	m_DataCenter.GetAllUserPriveledgeIndex(strUploader, vecIndex);
}
bool CManage::DownloadBaseIndex(std::string strName/*, std::string strUploader*/, SIndex& index, int& nError)
{
	bool bGet = m_DataCenter.GetIndex(strName, index);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_INDEX_NOTEXIST;
		return false;		
	}/*
	bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		if(index.nPrivate == 1)
		{//该策略不是属于该交易员，而且该策略为私有
			nError = CF_ERROR_DATA_INDEX_NOTBELONGTOUPLOADER_PRIVATE;
			return false;	
		}		
	}	*/
	return true;
}
bool CManage::DownloadIndexFile(std::string strName, std::string strUploader, int nType, void*& pData, int& nLength, int& nError)
{
	SIndex Index;
	bool bGet = m_DataCenter.GetIndex(strName, Index);
	if(!bGet)
	{
		nError = CF_ERROR_DATA_INDEX_NOTEXIST;
		return false;		
	}
	bool bIsUpLoader = m_DataCenter.IsIndexBelongToTrader(strName, strUploader);
	if(!bIsUpLoader)
	{
		if(Index.nPrivate == 1)
		{//该策略不是属于该交易员，而且该策略为私有
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PRIVATE;
			return false;	
		}
		else if(nType == 3) {
			// 共享不能下载源程序
			nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_PUBLIC;
			return false;
		}
	}	

	std::string strFileFolder = "Strategy";
	std::string strFileName;
	if(nType == 2)
		strFileName   = strName + ".dll";
	else if(nType == 3)
		strFileName   = strName + ".zip";
	GetFolderFileName(strFileName, strFileFolder);
	FILE* pFile = fopen( strFileName.c_str(), "rb" );
	if(pFile == NULL)
	{
		nError = CF_ERROR_DATA_READDLLFAIL;
		return false;// 服务端文件存储失败
	}

	fseek(pFile, 0, SEEK_END);
	int lnSize = ftell(pFile);
	if(lnSize > 0)
	{
		pData = new char[lnSize + 54];//这个54个字节为包头的的部分，如果以后请求包改的话，这个也得改，这个设计的不好
		fseek(pFile, 0, SEEK_SET);
		int n = fread((char*)pData+54,lnSize,1,pFile);//这个54个字节为包头的的部分，如果以后请求包改的话，这个也得改，这个设计的不好
		if(n>0)
			nLength = lnSize+54;
	}
	fclose(pFile);

	return lnSize>0;

}
bool CManage::UseStrategy(SUseStrategy& useStrategy, int& nError)
{
	if(useStrategy.nUserType == 2)
	{//策略终端
		SStrategy strategy;
		bool bGet = m_DataCenter.GetStrategy(useStrategy.strategyName, strategy);
		if(!bGet)
		{
			nError = CF_ERROR_DATA_STRATEGY_NOTEXIST;
			return false;		
		}
		bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(useStrategy.strategyName, useStrategy.strTraderName);
		if(!bIsUpLoader)
		{
			if(strategy.nPrivate == 1)
			{//该策略不是属于该交易员，而且该策略为私有
				nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_USE;
				return false;	
			}		
		}	
	}

	CInterface_SvrDBOpr::getObj().DB_SetStragegyStatus(useStrategy.strategyName, useStrategy.nStatus, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;
	
	m_DataCenter.SetStragegyStatus(useStrategy.strategyName, useStrategy.nStatus);	

	if(useStrategy.nStatus == 1)
	{
		//启用策略
		std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
		m_DataCenter.GetAllInstance(mapStrategyInstance);
		std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
		while(it != mapStrategyInstance.end())
		{
			if(m_mapInstance2Handle.find(it->first) != m_mapInstance2Handle.end()) {
				it++;
				continue;
			}
			SStrategyInstance& instance = it->second;
			if(useStrategy.strategyName == useStrategy.strategyName  && instance.nStart == 1)
			{//运行
				BeginSStrategyInstanceProcess(instance);
			}
			it++;
		}
	}
	else {
		//禁止策略
		std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
		m_DataCenter.GetAllInstance(mapStrategyInstance);
		std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
		while(it != mapStrategyInstance.end())
		{
			CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(it->first.strTrader, it->first.strStrategy, 1);			
			it++;
		}
		Sleep(1000);
		it = mapStrategyInstance.begin();
		while(it != mapStrategyInstance.end())
		{			
			TerminatInstanceThread(it->first.strStrategy, it->first.strTrader);//杀死进程	
			m_mapInstance2Handle.erase(it->first);
			it++;
		}
	}
	if(useStrategy.nStatus == 1)
		CInterface_SvrTradeData::getObj().PushStrategyStopOrRun(useStrategy.strategyName, 0);
	else if(useStrategy.nStatus == 0)
		CInterface_SvrTradeData::getObj().PushStrategyStopOrRun(useStrategy.strategyName, 1);
		
	//log入库信息
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, useStrategy.strTraderName);
	strcpy(logData.logTargetorID, useStrategy.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Modify;
	sprintf(logData.logContent,"策略状态改变(策略名:[%s]  交易员:[%s]， 状态：[%d] ",useStrategy.strategyName, useStrategy.strTraderName, useStrategy.nStatus );
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);
	return true;
}
bool CManage::DelStrategy(SDelStrategy& sDel, int& nError)
{
	if(sDel.nUserType == 2)
	{//策略终端
		SStrategy strategyExist;
		bool bGet = m_DataCenter.GetStrategy(sDel.strategyName, strategyExist);
		if(bGet)
		{
			if(strategyExist.nUse ==1)
			{//策略已经存在，而且处于启用状态
				nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;
				return false;
			}
		}
		bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(sDel.strategyName, sDel.strTraderName);
		if(!bIsUpLoader)
		{
			if(strategyExist.nPrivate == 1)
			{//该策略不是属于该交易员，而且该策略为私有
				nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER_USE;
				return false;	
			}		
		}	
	}

	CInterface_SvrDBOpr::getObj().DB_DelStrategy(sDel.strategyName, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;
	
	CInterface_SvrDBOpr::getObj().DB_DelInstance(sDel.strategyName, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;
	
	CInterface_SvrDBOpr::getObj().DB_DelStrategy2IndexRelation(sDel.strategyName, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.DelStrategy(sDel.strategyName);
	m_DataCenter.DelInstance(sDel.strategyName, sDel.strTraderName);
	m_DataCenter.DelStrategy2IndexRelation(sDel.strategyName);

	CInterface_SvrTradeData::getObj().PushStrategyStopOrRun(sDel.strategyName, 1);//通知所有策略方案停止
	Sleep(500);
	TerminatInstanceThread(sDel.strategyName, sDel.strTraderName);	

	//log入库信息
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, sDel.strTraderName);
	strcpy(logData.logTargetorID, sDel.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Delete;
	sprintf(logData.logContent,"删除策略(策略名:[%s]  交易员:[%s]",sDel.strategyName, sDel.strTraderName );
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);
	
	return true;
}
bool CManage::AddInstance(SStrategyInstance& strategy, int& nErrorCode)
{
	CInterface_SvrDBOpr::getObj().DB_AddInstance(strategy, nErrorCode);
	if(nErrorCode != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.AddInstance(strategy);

	CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(strategy.strTraderName, strategy.strategyName, 1);
	Sleep(500);

	TerminatInstanceThread(strategy.strategyName, strategy.strTraderName);	
	BeginSStrategyInstanceProcess(strategy);

	//log入库信息
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, strategy.strTraderName);
	strcpy(logData.logTargetorID, strategy.strategyName);
	
	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	
	logData.logAction     = Oper_Add;
	sprintf(logData.logContent,"增加交易策略方案(策略名:[%s]  交易员:[%s]  合约：[%s]) ",strategy.strategyName, strategy.strTraderName, strategy.Instruments);
	logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"增加策略方案");
	sprintf(MsgInfo.szContent,"增加交易策略方案(策略名:[%s]  交易员:[%s]  合约：[%s]) ",strategy.strategyName, strategy.strTraderName, strategy.Instruments);
	SendMessage2Trader(strategy.strTraderName, MsgInfo);
	return true;
}
bool CManage::ModifyInstance(SStrategyInstance& strategy, int& nErrorCode)
{
	CInterface_SvrDBOpr::getObj().DB_AddInstance(strategy, nErrorCode);
	if(nErrorCode != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.AddInstance(strategy);

	CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(strategy.strTraderName, strategy.strategyName, 1);
	Sleep(500);

	TerminatInstanceThread(strategy.strategyName, strategy.strTraderName);		
	BeginSStrategyInstanceProcess(strategy);

	//log入库信息
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, strategy.strTraderName);
	strcpy(logData.logTargetorID, strategy.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Modify;
	sprintf(logData.logContent,"修改交易策略方案(策略名:[%s]  交易员:[%s]  合约：[%s]) ",strategy.strategyName, strategy.strTraderName, strategy.Instruments);
	logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"策略方案修改");
	sprintf(MsgInfo.szContent,"修改交易策略方案(策略名:[%s]  交易员:[%s]  合约：[%s]) ",strategy.strategyName, strategy.strTraderName, strategy.Instruments);
	SendMessage2Trader(strategy.strTraderName, MsgInfo);
	return true;
}
bool CManage::DelInstance_ByTrader(SDelStrategy& sDel, int& nErrorCode)
{
	//bool bIsUpLoader = m_DataCenter.IsStrategyBelongToTrader(sDel.strategyName, sDel.strTraderName);
	//if(!bIsUpLoader)
	//{		
	//	//该策略方案不是属于该交易员
	//	nErrorCode = CF_ERROR_DATA_INSTANCE_NOTBELONGTOUPLOADER;
	//	return false;			
	//}	
	CInterface_SvrDBOpr::getObj().DB_DelInstance_ByTrader(sDel.strategyName, sDel.strTraderName, nErrorCode);
	if(nErrorCode != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.DelInstance(sDel.strategyName, sDel.strTraderName);
	
	CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(sDel.strTraderName, sDel.strategyName, 1);//1的时候是空转
	Sleep(500);
	TerminatInstanceThread(sDel.strategyName, sDel.strTraderName);	
	//log入库信息
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, sDel.strTraderName);
	strcpy(logData.logTargetorID, sDel.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Delete;
	sprintf(logData.logContent,"删除交易策略方案(策略名:[%s]  交易员:[%s]  ",sDel.strategyName, sDel.strTraderName);
	logData.logResult = (CF_ERROR_SUCCESS == nErrorCode)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"删除策略方案");
	sprintf(MsgInfo.szContent,"删除交易策略方案(策略名:[%s]  交易员:[%s]  ",sDel.strategyName, sDel.strTraderName);
	SendMessage2Trader(sDel.strTraderName, MsgInfo);
	return true;
}
bool CManage::UseInstance(SUseStrategy& useStrategy, int& nError)
{
	if(useStrategy.nUserType == 2)
	{//策略终端
		SStrategyInstance instance;
		bool bGet = m_DataCenter.GetInstance(useStrategy.strategyName, useStrategy.strTraderName, instance);
		if(!bGet)
		{
			nError = CF_ERROR_DATA_INDEX_NOTEXIST;
			return false;		
		}
		bool bIsUpLoader = m_DataCenter.IsInstanceBelongToTrader(useStrategy.strategyName, useStrategy.strTraderName);
		if(!bIsUpLoader)
		{
			//该策略方案不是属于该交易员
			nError = CF_ERROR_DATA_INSTANCE_NOTBELONGTOUPLOADER;
			return false;	

		}	
	}
	int nStatus = 0;//SStrategyInstance用1,0表示启停
	if(useStrategy.nStatus ==1)
		nStatus = 1;
	CInterface_SvrDBOpr::getObj().DB_SetInstanceStatus(useStrategy.strategyName, useStrategy.strTraderName, nStatus, nError);
	if(nError != CF_ERROR_SUCCESS)
		return false;

	m_DataCenter.SetInstanceStatus(useStrategy.strategyName, useStrategy.strTraderName, nStatus);
	//启用策略
	if(useStrategy.nStatus == 1)
	{
		STraderInstance instance(useStrategy.strategyName, useStrategy.strTraderName);
		if(m_mapInstance2Handle.find(instance) == m_mapInstance2Handle.end())
		{
			std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
			m_DataCenter.GetAllInstance(mapStrategyInstance);
			std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.find(instance);
			if(it != mapStrategyInstance.end()) 
			{
				SStrategyInstance& instance = it->second;
				//运行
				BeginSStrategyInstanceProcess(instance);
			}
		}
	}
	if(useStrategy.nStatus == 0)//停用
		CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(useStrategy.strTraderName, 
			useStrategy.strategyName,  1);
	else if(useStrategy.nStatus == 1)//启用
		CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(useStrategy.strTraderName, 
			useStrategy.strategyName, 0);
	//log入库信息
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nLogErrorCode; 
	LogStrategy logData;
	memset(&logData,0,sizeof(LogStrategy));

	strcpy(logData.logOperatorID, useStrategy.strTraderName);
	strcpy(logData.logTargetorID, useStrategy.strategyName);

	strcpy(logData.logTable,"LOG_STRATEGY");
	sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	logData.logAction     = Oper_Modify;
	std::string str;
	str = useStrategy.nStatus ==1?"启用":"停用";
	sprintf(logData.logContent,"%s交易策略方案(策略名:[%s]  交易员:[%s] ) ",str.c_str(), useStrategy.strategyName, useStrategy.strTraderName);
	logData.logResult = (CF_ERROR_SUCCESS == nError)?true:false;

	CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);


	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"策略方案状态改变");
	sprintf(MsgInfo.szContent,"%s交易策略方案(策略名:[%s]  交易员:[%s] ) ",str.c_str(), useStrategy.strategyName, useStrategy.strTraderName);
	SendMessage2Trader(useStrategy.strTraderName, MsgInfo);
	return true;
}
bool CManage::OnOffLineProcessInstance(std::string strTrader, bool bOnline)
{
	std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
	m_DataCenter.GetAllInstance(mapStrategyInstance);
	std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
		if(instance.nRunAfterOffline == false )
		{//运行
			if(bOnline)
			{
				if(instance.nStart == 1)
				{//PushStrategyInstanceStopOrRun 这个函数1的是时候是空转，0的时候开始运行
					CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(strTrader, instance.strategyName, 0);
					Strategy_LOGINFO("交易员上线，PushStrategyInstanceStopOrRun  [运行]\n");
					//BeginSStrategyInstanceProcess(instance);
					//log入库信息
					SYSTEMTIME sysTime;
					::GetLocalTime(&sysTime);
					int nLogErrorCode; 
					LogStrategy logData;
					memset(&logData,0,sizeof(LogStrategy));

					strcpy(logData.logOperatorID, instance.strTraderName);
					strcpy(logData.logTargetorID, instance.strategyName);

					strcpy(logData.logTable,"LOG_STRATEGY");
					sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
						sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

					logData.logAction     = Oper_Modify;
					sprintf(logData.logContent,"交易员上线运行,策略方案(策略名:[%s]  交易员:[%s]  合约：[%s]) ",instance.strategyName, instance.strTraderName, instance.Instruments);
					logData.logResult = true;

					CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

					MessageInfo MsgInfo;
					memset(&MsgInfo,0,sizeof(MessageInfo));
					safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"策略方案运行");
					sprintf(MsgInfo.szContent,"交易员[%s]上线,策略方案:[%s] 开始运行;\n ", instance.strTraderName, instance.strategyName);
					SendMessage2Trader(instance.strTraderName, MsgInfo);
				}
			}
			else
			{
				if(strcmp(strTrader.c_str(),instance.strTraderName) ==0)
				{
					CInterface_SvrTradeData::getObj().PushStrategyInstanceStopOrRun(strTrader, instance.strategyName, 1);
					Strategy_LOGINFO("交易员离线，PushStrategyInstanceStopOrRun  [停止]\n");
					//log入库信息
					SYSTEMTIME sysTime;
					::GetLocalTime(&sysTime);
					int nLogErrorCode; 
					LogStrategy logData;
					memset(&logData,0,sizeof(LogStrategy));

					strcpy(logData.logOperatorID, instance.strTraderName);
					strcpy(logData.logTargetorID, instance.strategyName);

					strcpy(logData.logTable,"LOG_STRATEGY");
					sprintf(logData.logDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
						sysTime.wYear, sysTime.wMonth, sysTime.wDay,sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

					logData.logAction     = Oper_Modify;
					sprintf(logData.logContent,"交易员下线停止策略方案(策略名:[%s]  交易员:[%s]  合约：[%s]) ",instance.strategyName, instance.strTraderName, instance.Instruments);
					logData.logResult = true;

					CInterface_SvrDBOpr::getObj().DB_WriteLogToDB(logData, nLogErrorCode);

					MessageInfo MsgInfo;
					memset(&MsgInfo,0,sizeof(MessageInfo));
					safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"策略方案停止");
					sprintf(MsgInfo.szContent,"交易员[%s]下线,策略方案:[%s] 停止运行;\n ", instance.strTraderName, instance.strategyName);
					SendMessage2Trader(instance.strTraderName, MsgInfo);
				}
			}			
		}
		it++;
	}
	return true;
}
bool CManage::DownloadAllInstance(std::string strUploader, std::vector<SStrategyInstance>& vecInstance, int& nError)
{
	std::map<STraderInstance, SStrategyInstance> mapStrategyInstance;
	m_DataCenter.GetAllInstance(mapStrategyInstance);
	std::map<STraderInstance, SStrategyInstance>::iterator it = mapStrategyInstance.begin();
	while(it != mapStrategyInstance.end())
	{
		SStrategyInstance& instance = it->second;
		if(strUploader =="" || strUploader == instance.strTraderName)//得到所有策略方案 或者 某交易员所有的策略方案 
			vecInstance.push_back(instance);
		it++;
	}
	return true;
}
void CManage::SendMessage2Trader(std::string strUserName, MessageInfo& MsgInfo)
{
	UserInfo lUserInfo;
	bool bGet = CInterface_SvrUserOrg::getObj().GetUserByName(strUserName,lUserInfo);
	if(!bGet)
		return;
	set<int> setRealUsers;
	setRealUsers.insert(lUserInfo.nUserID);

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);		
	int nExpiredDate = sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;

	CInterface_SvrMsg::getObj().SendMsg(0, setRealUsers, conAutoMsg, nExpiredDate, 
		MsgInfo.szTitle, MsgInfo.szContent);

}

bool CManage::UploadStartAll(UploadStart& start, int& nError, std::string& strError)
{
	TiXmlDocument *doc = new TiXmlDocument();    
	doc->Parse(start.uploadstring);   
	if (&doc == NULL)   
		return false;	

	SUploadAllInfo uploadAllInfo;
	TiXmlHandle docHandle(doc);   
	TiXmlNode * uploadid = docHandle.FirstChild("uploadid").ToElement();   
	TiXmlElement * elemElem = uploadid->ToElement();   
	const char* pData  =  elemElem->GetText();
	std::string strID="";
	if(pData)
		strID = pData;
	uploadAllInfo.strID = strID;
	
	TiXmlNode *traderid = docHandle.FirstChild("traderid").ToElement();   
	TiXmlElement * elemElem1 = traderid->ToElement();   
	const char* charelemElem1  =  elemElem1->GetText();
	std::string strtraderID ="";
	if(charelemElem1)
		strtraderID = charelemElem1;

	uploadAllInfo.strTraderID = strtraderID;

	TiXmlNode * stratregyname = docHandle.FirstChild("stratregyname").ToElement();   
	TiXmlElement * elemElem2 = stratregyname->ToElement();   
	
	const char* charelemElem2  =  elemElem2->GetText();
	std::string strstratregyname = "";
	if(charelemElem2)
		strstratregyname = charelemElem2;

	uploadAllInfo.vecStrategy = CTools_Ansi::SplitString(strstratregyname.c_str(), ';');

	TiXmlNode * indexname = docHandle.FirstChild("indexname").ToElement();   
	TiXmlElement * elemElem3 = indexname->ToElement();   
	const char* charelemElem3  =  elemElem3->GetText();
	std::string strindexname = "";
	if(charelemElem3)
		strindexname = charelemElem3;
	uploadAllInfo.vecIndex = CTools_Ansi::SplitString(strindexname.c_str(), ';');

	bool bUse = HasStrategyRun(uploadAllInfo.vecStrategy, uploadAllInfo.vecIndex, strtraderID, nError, strError);
	if(!bUse)//指标和策略正在使用
		return false;

	bool bBack = m_DataCenter.AddUploadAllInfo(uploadAllInfo);	
	if(!bBack)
	{		
		nError = CF_ERROR_DATA_UPLOAD_BACKFAIL;
		return false;
	}	
	return true;
}
bool CManage::HasStrategyRun(std::vector<std::string>& vecStrategy, std::vector<std::string>& vecIndex, std::string strTrader,int& nError, std::string& strError)
{
	for(int i =0; i<(int)vecStrategy.size(); i++)
	{
		std::string str = vecStrategy[i];
		SStrategy strategyExist;
		bool bGet = m_DataCenter.GetStrategy(str, strategyExist);
		if(bGet) 
		{
			if(strcmp(strategyExist.strTraderName, strTrader.c_str())!=0)
			{
				nError = CF_ERROR_DATA_STRATEGY_NOTBELONGTOUPLOADER;
				return false;
			}
			if(strategyExist.nUse ==1)
			{//策略已经存在，而且处于启用状态
				nError = CF_ERROR_DATA_STRATEGY_EXISTANDUSE;

				const char * lErrorString = FormatErrorCode(nError);					
				char Buf[1024];
				memset(Buf,0,sizeof(Buf));
				sprintf(Buf, lErrorString, str.c_str());
				strError = Buf;
				return false;
			}
		}
	}
	for(int i =0; i<(int)vecIndex.size(); i++)
	{
		std::string str = vecIndex[i];
		std::vector<std::string> vecStrategyName;
		m_DataCenter.GetStrategyofIndex(str, vecStrategyName);		
		for(int i =0; i< (int)vecStrategyName.size(); i++)
		{
			std::string strStrategyName = vecStrategyName[i];
			SStrategy strategyExist;
			bool bGet = m_DataCenter.GetStrategy(strStrategyName, strategyExist);
			if(bGet)
			{
				if(strategyExist.nUse ==1)
				{//策略已经存在，而且处于启用状态
					nError = CF_ERROR_DATA_IndexIsUsing;
					const char * lErrorString = FormatErrorCode(nError);					
					char Buf[1024];
					memset(Buf,0,sizeof(Buf));
					sprintf(Buf, lErrorString, str.c_str(), strStrategyName.c_str());
					strError = Buf;
					return false;
				}
			}
		}				
	}
	return true;
}
bool CManage::UploadEndAll(UploadEnd& end, int& nError)
{
	SUploadAllInfo uploadAllInfo;
	m_DataCenter.GetUploadAllInfo(end.strTraderName, uploadAllInfo);

	if(m_DataCenter.IsUploadAllInfoExist(uploadAllInfo))
	{
		bool bBack = m_DataCenter.RollBackUploadInfo(uploadAllInfo);
		if(!bBack)
		{	
			Strategy_LOGINFO("上传失败，恢复到原来的失败\n");
			nError = CF_ERROR_DATA_UPLOAD_RollBACKFAIL;
			return false;
		}	
		else
		{
			Strategy_LOGINFO("上传失败，恢复到原来的成功\n");
			nError = CF_ERROR_DATA_UPLOAD_FAIL;
			return false;			
		}
	}
	else
		m_DataCenter.DelUploadAllInfo(uploadAllInfo);	
	return true;
}
