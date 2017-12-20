#pragma once
#include <map>
#include <string>
#include <set>
//交易员管理

struct TraderInfo 
{
	TraderInfo()
	{
		mnAllocNum = 0;
	}
	TraderInfo(std::string nName,std::string nPass)
	{
		msName = nName;
		msPass = nPass;
		mnAllocNum = 0;
	}
	std::string msName;
	std::string msPass;
	int   mnAllocNum; //被分配了几次
};

struct sTrader
{
	char msName[30];
	char msPass[30];
};

class CTraderManager
{
public:
	
	void Load();
	void SetTraderStatus(std::string nTraderName,bool bAdd);

	void FetchOneAviableTrader(TraderInfo & nTraderInfo);

	void GetProductSet(std::set<std::string> & nProductSet)
	{
		nProductSet = msetProduct;
	}
	void GetAllTraderInfo(std::map<std::string,TraderInfo> & nmapTraderInfo)
	{
		nmapTraderInfo = mMapTrader;
	}
	void GetTraderPassword(const std::string & nUserName,std::string & nPass)
	{
		std::map<std::string,TraderInfo>::iterator lIter = mMapTrader.find(nUserName);
		if(lIter != mMapTrader.end())
		{
			nPass = lIter->second.msPass;
		}

	}
public:
	std::map<std::string,TraderInfo> mMapTrader;
	int mnMaxNode;
	int mnMaxAlloc;
	std::set<std::string> msetProduct;
};