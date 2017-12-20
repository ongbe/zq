#include "DataCenter.h"

CDataCenter::CDataCenter(void)
{
}

CDataCenter::~CDataCenter(void)
{
}
void CDataCenter::Init()
{
	m_lockStrategy.write_lock();
	std::vector<SStrategy> vecStratedy;
	CInterface_SvrDBOpr::getObj().DB_GetUserStrategys(vecStratedy);
	for(int i =0; i<(int)vecStratedy.size(); i++)
	{
		SStrategy& strategy = vecStratedy[i];
		m_mapStrategy[strategy.strategyName] = strategy;
	}
	m_lockStrategy.write_unlock();

	m_lockInstance.write_lock();
	std::vector<SStrategyInstance> vecInstance;
	CInterface_SvrDBOpr::getObj().DB_GetUserInstance(vecInstance);
	for(int i =0; i<(int)vecInstance.size(); i++)
	{
		SStrategyInstance& strategy = vecInstance[i];
		STraderInstance instance(strategy.strategyName, strategy.strTraderName);
		m_mapStrategyInstance[instance] = strategy;
	}
	m_lockInstance.write_unlock();

	m_lockStrategyIndexRelation.write_lock();
	std::vector<SStrategy2Index> vec;
	CInterface_SvrDBOpr::getObj().DB_GetStrategy2IndexRelation(vec);
	for(int i =0; i<(int)vec.size(); i++)
	{
		SStrategy2Index& relation = vec[i];

		m_mapStrategy2Index.insert(std::make_pair(relation.strategyName, relation.IndexName)); 	
		m_mapIndex2Strategy.insert(std::make_pair(relation.IndexName, relation.strategyName)); 	
	}
	m_lockStrategyIndexRelation.write_unlock();


	m_lockSIndex.write_lock();
	std::vector<SIndex> vecIndex;
	CInterface_SvrDBOpr::getObj().DB_GetUserIndex(vecIndex, "");
	for(int i =0; i<(int)vecIndex.size(); i++)
	{
		SIndex& relation = vecIndex[i];

		m_mapSIndex.insert(std::make_pair(relation.IndexName, relation)); 	
		
	}	
	m_lockSIndex.write_unlock();
}
bool CDataCenter::GetAllUserPriveledgeStrategy(std::string strTrader, vector<SStrategy>& vecStrategy)
{
	m_lockStrategy.write_lock();
	std::map<std::string, SStrategy>::iterator it = m_mapStrategy.begin();
	while(it != m_mapStrategy.end())
	{
		SStrategy index = it->second;	
		if(strTrader.length() == 0)
		{//如果交易员为空，则代表所有策略
			vecStrategy.push_back(index);
		}
		else
		{
			if(index.nPrivate == 2 || strcmp(index.strTraderName, strTrader.c_str())==0)
				vecStrategy.push_back(index);
		}
		
		it++;
	}
	m_lockStrategy.write_unlock();
	return true;
}
bool CDataCenter::GetStrategy(std::string strName, SStrategy& strategy)
{
	bool bFind =false;
	m_lockStrategy.write_lock();
	std::map<std::string, SStrategy>::iterator it = m_mapStrategy.find(strName);
	if(it != m_mapStrategy.end())
	{
		strategy = it->second;	
		bFind = true;
	}
	m_lockStrategy.write_unlock();
	return bFind;
}
bool CDataCenter::AddStrategy(SStrategy& strategy)//增加一个策略；已经有的则覆盖
{
	m_lockStrategy.write_lock();
	m_mapStrategy[strategy.strategyName] = strategy;
	m_lockStrategy.write_unlock();
	return true;
}
bool CDataCenter::DelStrategy(std::string strName)//删除一个策略；禁用状态下才可以删除策略。
{
	m_lockStrategy.write_lock();
	m_mapStrategy.erase(strName);
	m_lockStrategy.write_unlock();

	DelStrategy2IndexRelation(strName);

	return true;
}
bool CDataCenter::SetStragegyStatus(std::string strName, int bStart)// bstart 0:停用 1:启用	
{
	bool bChange =false;
	m_lockStrategy.write_lock();
	std::map<std::string, SStrategy>::iterator it = m_mapStrategy.find(strName);
	if(it != m_mapStrategy.end())
	{
		SStrategy& strategy = it->second;
		strategy.nUse = bStart;
		bChange = true;
	}
	m_lockStrategy.write_unlock();
	return bChange;
}
bool CDataCenter::IsStrategyBelongToTrader(std::string strName, std::string strTrader)//判断该策略是不是该交易员的
{
	bool bSame =false;
	m_lockStrategy.write_lock();
	std::map<std::string, SStrategy>::iterator it = m_mapStrategy.find(strName);
	if(it != m_mapStrategy.end())
	{
		SStrategy& strategy = it->second;
		if(strcmp(strategy.strTraderName, strTrader.c_str()) ==0)
			bSame = true;
	}
	m_lockStrategy.write_unlock();

	return bSame;
}
bool CDataCenter::GetAllInstance(std::map<STraderInstance, SStrategyInstance>& mapStrategyInstance)
{
	m_lockInstance.write_lock();
	mapStrategyInstance = m_mapStrategyInstance;	
	m_lockInstance.write_unlock();
	return true;
}
//策略方案
bool CDataCenter::GetInstance(std::string strName, std::string strTrader, SStrategyInstance& Instance)
{
	bool bFind =false;
	m_lockInstance.write_lock();
	STraderInstance instance(strName.c_str(), strTrader.c_str());
	std::map<STraderInstance, SStrategyInstance>::iterator it = m_mapStrategyInstance.find(instance);
	if(it != m_mapStrategyInstance.end())
	{
		Instance = it->second;	
		bFind = true;
	}
	m_lockInstance.write_unlock();
	return bFind;
}
bool CDataCenter::AddInstance(SStrategyInstance& strategy)//增加一个策略方案；已经有的则覆盖
{
	m_lockInstance.write_lock();
	STraderInstance instance(strategy.strategyName, strategy.strTraderName);
	m_mapStrategyInstance[instance] = strategy;
	m_lockInstance.write_unlock();
	return true;
}
bool CDataCenter::DelInstance(std::string strName, std::string strTrader)//删除一个策略方案；
{
	m_lockInstance.write_lock();
	if(strTrader == "")
	{
		std::map<STraderInstance, SStrategyInstance>::iterator it = m_mapStrategyInstance.begin();
		if(it != m_mapStrategyInstance.end())
		{
			const STraderInstance& traderInstance = it->first;
			if(traderInstance.strStrategy == strName)
			{
				it = m_mapStrategyInstance.erase(it);
			}
			else
				it++;
		}		
	}
	else
	{
		STraderInstance instance(strName.c_str(), strTrader.c_str());
		m_mapStrategyInstance.erase(instance);
	}	
	m_lockInstance.write_unlock();
	return true;
}
bool CDataCenter::SetInstanceStatus(std::string strName, std::string strTrader, bool bUse)// bUse 0:暂停 1:运行
{
	bool bChange =false;
	m_lockInstance.write_lock();
	if(strTrader == "")
	{
		std::map<STraderInstance, SStrategyInstance>::iterator it = m_mapStrategyInstance.begin();
		if(it != m_mapStrategyInstance.end())
		{
			const STraderInstance& traderInstance = it->first;
			if(traderInstance.strStrategy == strName)
			{
				SStrategyInstance& strategy = it->second;
				strategy.nStart = bUse;
			}
			it++;
		}		
	}
	else
	{
		STraderInstance instance(strName.c_str(), strTrader.c_str());
		std::map<STraderInstance, SStrategyInstance>::iterator it = m_mapStrategyInstance.find(instance);
		if(it != m_mapStrategyInstance.end())
		{
			SStrategyInstance& strategy = it->second;
			strategy.nStart = bUse;
			bChange = true;
		}
	}	
	m_lockInstance.write_unlock();
	return bChange;
}
bool CDataCenter::IsInstanceBelongToTrader(std::string strName, std::string strTrader)//判断该策略方案是不是该交易员的
{
	bool bSame =false;
	m_lockInstance.write_lock();
	STraderInstance instance(strName.c_str(), strTrader.c_str());
	std::map<STraderInstance, SStrategyInstance>::iterator it = m_mapStrategyInstance.find(instance);
	if(it != m_mapStrategyInstance.end())
	{
		SStrategyInstance& strategy = it->second;
		if(strcmp(strategy.strTraderName, strTrader.c_str()) ==0)
			bSame = true;
	}
	m_lockInstance.write_unlock();

	return bSame;
}
bool CDataCenter::GetAllUserPriveledgeIndex(std::string strName, vector<SIndex>& vecIndex)
{
	m_lockSIndex.write_lock();
	std::map<std::string, SIndex>::iterator it = m_mapSIndex.begin();
	while(it != m_mapSIndex.end())
	{
		SIndex index = it->second;	
		if(strName.length() == 0)
		{//如果交易员为空，则代表所有策略
			vecIndex.push_back(index);
		}
		else
		{
			if(index.nPrivate == 2 || strcmp(index.strTraderName, strName.c_str())==0)
				vecIndex.push_back(index);
		}
		
		it++;
	}
	m_lockSIndex.write_unlock();
	return true;
}
bool CDataCenter::GetIndex(std::string strName, SIndex& index)
{
	bool bFind =false;
	m_lockSIndex.write_lock();
	std::map<std::string, SIndex>::iterator it = m_mapSIndex.find(strName);
	if(it != m_mapSIndex.end())
	{
		index = it->second;	
		bFind = true;
	}
	m_lockSIndex.write_unlock();
	return bFind;
}
bool CDataCenter::AddIndex(SIndex& index)
{
	m_lockSIndex.write_lock();
	m_mapSIndex[index.IndexName] = index;
	m_lockSIndex.write_unlock();
	return true;
}
bool CDataCenter::DelIndex(std::string strName)
{
	m_lockSIndex.write_lock();
	m_mapSIndex.erase(strName);
	m_lockSIndex.write_unlock();
	return true;
}
bool CDataCenter::SetIndexStatus(std::string strName, int nPrivate)
{
	bool bChange =false;
	m_lockSIndex.write_lock();
	std::map<std::string, SIndex>::iterator it = m_mapSIndex.find(strName);
	if(it != m_mapSIndex.end())
	{
		SIndex& strategy = it->second;
		strategy.nPrivate = nPrivate;
		bChange = true;
	}
	m_lockSIndex.write_unlock();
	return bChange;
}
bool CDataCenter::IsIndexBelongToTrader(std::string strName, std::string strTrader)
{
	bool bSame =false;
	m_lockSIndex.write_lock();
	std::map<std::string, SIndex>::iterator it = m_mapSIndex.find(strName);
	if(it != m_mapSIndex.end())
	{
		SIndex& strategy = it->second;
		if(strcmp(strategy.strTraderName, strTrader.c_str()) ==0)
			bSame = true;
	}
	m_lockSIndex.write_unlock();

	return bSame;
}
bool CDataCenter::GetStrategyofIndex(std::string strName, std::vector<std::string>& vecStrategyName)
{
	m_lockStrategyIndexRelation.write_lock();

	int nCount = m_mapIndex2Strategy.count(strName);
	std::multimap<std::string, std::string>::iterator it = m_mapIndex2Strategy.find(strName);
	for(int i = 0; i<nCount; i++ )
	{
		std::string strStrategyName = it->second;
		vecStrategyName.push_back(strStrategyName);
	}
	m_lockStrategyIndexRelation.write_unlock();
	return 0;
}
bool CDataCenter::DelStrategy2IndexRelation(std::string strategyName)
{
	m_lockStrategyIndexRelation.write_lock();

	int nCount = m_mapStrategy2Index.count(strategyName);
	std::multimap<std::string, std::string>::iterator it = m_mapStrategy2Index.find(strategyName);
	for(int i =0; i<nCount; i++)
	{
		it = m_mapStrategy2Index.erase(it);
	}

	std::multimap<std::string, std::string>::iterator it2 = m_mapIndex2Strategy.begin();
	while(it2 != m_mapIndex2Strategy.end())
	{
		std::string strStrategy = it2->second;
		if(strStrategy == strategyName)
			it2 = m_mapIndex2Strategy.erase(it2);
		else
			it2 ++;
	}
	m_lockStrategyIndexRelation.write_unlock();
	return true;
}
bool CDataCenter::AddStrategy2IndexRelation(std::string strategyName, std::string strIndexName)
{
	m_lockStrategyIndexRelation.write_lock();

	m_mapStrategy2Index.insert(std::make_pair(strategyName, strIndexName)); 	
	m_mapIndex2Strategy.insert(std::make_pair(strIndexName, strategyName)); 	
	
	m_lockStrategyIndexRelation.write_unlock();
	return true;
}
bool CDataCenter::IsUploadAllInfoExist(SUploadAllInfo& uploadAllInfo)
{
	bool bHasTask = false;
	m_lockTrader2UploadAllInfo.write_lock();
	std::map<std::string, SUploadAllInfo>::iterator it = m_mapTrader2uploadAllInfo.find(uploadAllInfo.strTraderID);
	if(it != m_mapTrader2uploadAllInfo.end())
	{
		SUploadAllInfo& sUploadAllInfo = it->second;
		if(sUploadAllInfo.vecStrategy.size() != sUploadAllInfo.nStrategySuccess || sUploadAllInfo.vecIndex.size() != sUploadAllInfo.nIndexSuccess)
			bHasTask = true;
	}	
	m_lockTrader2UploadAllInfo.write_unlock();

	return bHasTask;
}
bool CDataCenter::AddUploadAllInfo(SUploadAllInfo& uploadAllInfo)
{
	m_lockTrader2UploadAllInfo.write_lock();	
	m_mapTrader2uploadAllInfo[uploadAllInfo.strTraderID] = uploadAllInfo;
	m_lockTrader2UploadAllInfo.write_unlock();

	for(int i = 0; i < (int)uploadAllInfo.vecStrategy.size(); i++)
	{
		std::string strStrategy = uploadAllInfo.vecStrategy[i];	
		bool bGet = false;
		SStrategy Strategy;
		m_lockStrategy.write_lock();
		std::map<std::string, SStrategy>::iterator it = m_mapStrategy.find(strStrategy);
		if(it != m_mapStrategy.end())
		{
			Strategy = it->second;

			
			bGet = true;			
		}
		m_lockStrategy.write_unlock();	
		
		if(bGet)
		{
			m_lockStrategyBack.write_lock();
			m_mapStrategyBack[strStrategy] = Strategy;
			m_lockStrategyBack.write_unlock();	
		}
		if(!bGet)
			continue;

		char szFilename[256];
		memset(szFilename, 0, sizeof(szFilename));
		sprintf(szFilename,"%s.dll",Strategy.strategyName);

		std::string strFileName   = szFilename;
		std::string strsrx = "strategy";
		std::string strDest = "Strategy_back";	
		bool bCopy = copyUploadFile(strsrx, strDest,  uploadAllInfo.strTraderID, strFileName, true);
		if(bCopy == false)
		{				
			return false;
		}

		//strFileName   = Strategy.strategyName +".zip";
		memset(szFilename, 0, sizeof(szFilename));
		sprintf(szFilename,"%s.zip",Strategy.strategyName);
		strFileName   = szFilename;
		strsrx = "strategy";
		strDest = "Strategy_back";
		bCopy = copyUploadFile(strsrx, strDest,  uploadAllInfo.strTraderID, strFileName, true);
		if(bCopy == false)
		{					
			return false;
		}

	}

	for(int i = 0; i < (int)uploadAllInfo.vecIndex.size(); i++)
	{
		std::string strIndex = uploadAllInfo.vecIndex[i];	
		bool bGet = false;
		SIndex index;

		m_lockSIndex.write_lock();
		std::map<std::string, SIndex>::iterator it = m_mapSIndex.find(strIndex);
		if(it != m_mapSIndex.end())
		{
			index = it->second;	
		
			bGet = true;		
		}
		m_lockSIndex.write_unlock();	
		if(bGet)
		{
			m_lockSIndexBack.write_lock();
			m_mapSIndexBack[strIndex] = index;
			m_lockSIndexBack.write_unlock();
		}
		
		if(!bGet)
			continue;

		char szFilename[256];
		memset(szFilename, 0, sizeof(szFilename));
		sprintf(szFilename,"%s.dll",index.IndexName);
		std::string strFileName   = szFilename;

		std::string strsrx = "strategy";
		std::string strDest = "Strategy_back";	
		bool bCopy = copyUploadFile(strsrx, strDest,  uploadAllInfo.strTraderID, strFileName, true);
		if(bCopy == false)
		{				
			return false;
		}
		memset(szFilename, 0, sizeof(szFilename));
		sprintf(szFilename,"%s.zip",index.IndexName);
		strFileName   = szFilename;
		strsrx = "strategy";
		strDest = "Strategy_back";
		bCopy = copyUploadFile(strsrx, strDest,  uploadAllInfo.strTraderID, strFileName, true);
		if(bCopy == false)
		{					
			return false;
		}
	}	
	return true;
}
bool CDataCenter::DelUploadAllInfo(SUploadAllInfo& uploadAllInfo)
{
	m_lockTrader2UploadAllInfo.write_lock();	
	m_mapTrader2uploadAllInfo.erase(uploadAllInfo.strTraderID);;
	m_lockTrader2UploadAllInfo.write_unlock();

	m_lockStrategyBack.write_lock();
	for(int i = 0; i < (int)uploadAllInfo.vecStrategy.size(); i++)
	{
		std::string strStrategy = uploadAllInfo.vecStrategy[i];	
		m_mapStrategyBack.erase(strStrategy);
	}	
	m_lockStrategyBack.write_unlock();	

	
	m_lockSIndexBack.write_lock();
	for(int i = 0; i < (int)uploadAllInfo.vecIndex.size(); i++)
	{
		std::string strIndex = uploadAllInfo.vecIndex[i];	
		m_mapSIndexBack.erase(strIndex);
	}
	m_lockSIndexBack.write_unlock();	
	return true;
}
bool CDataCenter::RollBackUploadInfo(SUploadAllInfo& uploadAllInfo)
{
	for(int i = 0; i < (int)uploadAllInfo.vecStrategy.size(); i++)
	{
		std::string strStrategy = uploadAllInfo.vecStrategy[i];	
		bool bGet = false;
		SStrategy Strategy;
		m_lockStrategyBack.write_lock();
		std::map<std::string, SStrategy>::iterator it = m_mapStrategyBack.find(strStrategy);
		if(it != m_mapStrategyBack.end())
		{
			Strategy = it->second;				
			bGet = true;			
		}
		m_lockStrategyBack.write_unlock();	

		if(bGet)
		{
			m_lockStrategy.write_lock();
			m_mapStrategy[strStrategy] = Strategy;
			m_lockStrategy.write_unlock();	
		}	
		//////////////////////////////////////////////////////////////////////////
		//拷贝文件
		int nError = 0;
		CInterface_SvrDBOpr::getObj().DB_AddStrategy(Strategy, nError);
		if(nError != 0)//
		{	
			
			return false;
		}
		AddStrategy(Strategy);
		
		if(!bGet)
			continue;

		char szFilename[256];
		memset(szFilename, 0, sizeof(szFilename));
		sprintf(szFilename,"%s.dll",Strategy.strategyName);

		std::string strFileName   = szFilename;
		std::string strsrx = "strategy";
		std::string strDest = "Strategy_back";	
		bool bCopy = copyUploadFile(strsrx, strDest,  uploadAllInfo.strTraderID, strFileName, false);
		if(bCopy == false)
		{				
			
			return false;
		}
		//////////////////////////////////////////////////////////////////////////
		//文件写回数据库
		bool bWrite = WriteFile2DB(strDest, Strategy.strategyName, 0, 0);
		if(!bWrite)
			return false;
		
		memset(szFilename, 0, sizeof(szFilename));
		sprintf(szFilename,"%s.zip",Strategy.strategyName);
		strsrx = "strategy";
		strDest = "Strategy_back";	
		bCopy = copyUploadFile(strsrx, strDest,  uploadAllInfo.strTraderID, strFileName, false);
		if(bCopy == false)
		{				
			
			return false;
		}
		bWrite = WriteFile2DB(strDest, Strategy.strategyName, 0, 1);
		if(!bWrite)
			return false;
	}

	for(int i = 0; i < (int)uploadAllInfo.vecIndex.size(); i++)
	{
		std::string strIndex = uploadAllInfo.vecIndex[i];	
		bool bGet = false;
		SIndex index;

		m_lockSIndexBack.write_lock();
		std::map<std::string, SIndex>::iterator it = m_mapSIndexBack.find(strIndex);
		if(it != m_mapSIndexBack.end())
		{
			index = it->second;					
			bGet = true;		
		}
		m_lockSIndexBack.write_unlock();	
		if(bGet)
		{
			m_lockSIndex.write_lock();
			m_mapSIndex[strIndex] = index;
			m_lockSIndex.write_unlock();
		}
		int nError = 0;
		CInterface_SvrDBOpr::getObj().DB_AddIndex(index, nError);
		if(nError != 0)//
		{			
			return false;
		}
		AddIndex(index);

		if(!bGet)
			continue;

		char szFilename[256];
		memset(szFilename, 0, sizeof(szFilename));
		sprintf(szFilename,"%s.dll",index.IndexName);
		std::string strFileName   = szFilename;
		std::string strsrx = "strategy";
		std::string strDest = "Strategy_back";	
		bool bCopy = copyUploadFile(strsrx, strDest,  uploadAllInfo.strTraderID, strFileName, false);
		if(bCopy == false)
		{					
			return false;
		}
		//////////////////////////////////////////////////////////////////////////
		//文件写回数据库
		bool bWrite = WriteFile2DB(strDest, index.IndexName, 1, 0);
		if(!bWrite)
			return false;

		memset(szFilename, 0, sizeof(szFilename));
		sprintf(szFilename,"%s.zip",index.IndexName);
		strsrx = "strategy";
		strDest = "Strategy_back";	
		bCopy = copyUploadFile(strsrx, strDest,  uploadAllInfo.strTraderID, strFileName, false);
		if(bCopy == false)
		{						
			return false;
		}
		bWrite = WriteFile2DB(strDest, index.IndexName, 1, 1);
		if(!bWrite)
			return false;
	}	
	return true;
}
bool CDataCenter::copyUploadFile(std::string& strsrx, std::string& strDest, std::string strDestSubPath, std::string filename, bool bCopyFromSrc)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
	std::string strSystemPath( szLocalPath );
	int nPos = strSystemPath.rfind( '\\' );
	if ( -1 != nPos )
	{
		strSystemPath = strSystemPath.substr( 0, nPos + 1 );
		if(!strsrx.empty())
		{
			if(std::string::npos==strsrx.rfind("\\"))
				strSystemPath+=strsrx+"\\";
			else
				strSystemPath+=strsrx;			
		}
		strsrx = strSystemPath + filename;
	}	

	strDest = strSystemPath + strDest;
	CreateDirectory(strDest.c_str(),NULL);

	strDest = strDest + "\\" + strDestSubPath;
	CreateDirectory(strDest.c_str(),NULL);
	
	strDest = strDest + "\\";
	strDest = strDest + filename;

	bool bSuccess = false;
	if(bCopyFromSrc)
		bSuccess = CopyFile(strsrx.c_str(), strDest.c_str(), false);
	else
		bSuccess = CopyFile(strDest.c_str(), strsrx.c_str(), false);
	return bSuccess;
}
bool CDataCenter::GetUploadAllInfo(std::string strTraderID, SUploadAllInfo& sUploadAllInfo)
{
	bool bGet = false;
	m_lockTrader2UploadAllInfo.write_lock();
	std::map<std::string, SUploadAllInfo>::iterator it = m_mapTrader2uploadAllInfo.find(strTraderID);
	if(it != m_mapTrader2uploadAllInfo.end())
	{
		sUploadAllInfo = it->second;
		bGet = true;
	}	
	m_lockTrader2UploadAllInfo.write_unlock();

	return bGet;
}
bool CDataCenter::WriteFile2DB(std::string strFilename, std::string strName, int nType, int nSubType)
{
	long fileSize = 0;
	FILE* pFile = fopen( strFilename.c_str(), "rb" );
	if(pFile == NULL)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	int lnSize = ftell(pFile);
	if(lnSize == 0)
	{
		fclose(pFile);
		return false;
	}

	char *lData = new char[lnSize];
	memset(lData,0,lnSize);

	fseek(pFile, 0, SEEK_SET);
	int lnCurrentPkg = 1;
	int lnCurrentReadSize = lnSize ;

	long lRead = fread(lData,lnCurrentReadSize,1,pFile);
	fclose(pFile);
	if(lRead <1 && lnSize>0)
	{		
		delete []lData;
		return false;
	}
	bool bIsStrategy = true;
	if(nType == 1)
		bIsStrategy = false;



	int nError = 0;
	if(bIsStrategy)
	{		
		int nDLL = UPLOAD_STRATEGY_DLL;
		if(nSubType == 1)
			nDLL = UPLOAD_STRATEGY_ZIP;

		CInterface_SvrDBOpr::getObj().DB_WriteStragegyFile(strName, nDLL, (char*)lData, lnCurrentReadSize, nError);
		if(nError != 0)
		{
			delete []lData;
			return false;
		}
	}
	else
	{
		int nDLL = UPLOAD_INDEX_DLL;
		if(nSubType == 1)
			nDLL = UPLOAD_INDEX_ZIP;		
		CInterface_SvrDBOpr::getObj().DB_WriteIndexFile(strName, nDLL, (char*)lData, lnCurrentReadSize, nError);
		if(nError != 0)
		{
			delete []lData;
			return false;
		}
	}
	delete []lData;
	return true;
}
bool CDataCenter::calcUploadAllInfo(std::string strTraderID, std::string strName, int nType)
{
	m_lockTrader2UploadAllInfo.write_lock();	
	std::map<std::string, SUploadAllInfo>::iterator it = m_mapTrader2uploadAllInfo.find(strTraderID);
	if(it != m_mapTrader2uploadAllInfo.end())
	{
		SUploadAllInfo& sUploadAllInfo = it->second;
		if(nType == 0)
		{
			for(int i =0; i<sUploadAllInfo.vecStrategy.size(); i++)
			{
				std::string strName2 = sUploadAllInfo.vecStrategy[i];			
				if(strcmp(strName2.c_str(), strName.c_str()) == 0)
					++sUploadAllInfo.nStrategySuccess;
			}
		}
		else if(nType ==1)
		{
			for(int i =0; i<sUploadAllInfo.vecIndex.size(); i++)
			{
				std::string strName2 = sUploadAllInfo.vecIndex[i];
				if(strcmp(strName2.c_str(), strName.c_str()) == 0)
					++sUploadAllInfo.nIndexSuccess;
			}
		}
	
	}	
	m_lockTrader2UploadAllInfo.write_unlock();

	return true;
}