#include "stdafx.h"
#include "windows.h"
#include "TraderManager.h"
#include "Tools_Win32.h"


void CTraderManager::Load()
{
	//获取程序路径
	char lsMainPath[MAX_PATH] = {0};
	CTools_Win32::GetMainPath(lsMainPath,sizeof(lsMainPath)-1);



	char lsFilePath[512];
	sprintf(lsFilePath,"%sTestAdmin.ini",lsMainPath);
	
	mnMaxNode = GetPrivateProfileIntA("Client","MAXNODE",10,lsFilePath);
	mnMaxAlloc = GetPrivateProfileIntA("Client","MAXAlloc",6,lsFilePath);

	char lsUserKey[50] = "Trader";
	char strBuf[102400] = {0};
	GetPrivateProfileStringA(lsUserKey,NULL,"",strBuf,sizeof(strBuf),lsFilePath);

	int lnLastIndex = 0;
	for(int i = 0; i < 102400; i++)
	{
		if(strBuf[i] == 0)
		{
			std::string lsTrader = (char*)(strBuf + lnLastIndex);
			if(!lsTrader.empty())
			{
				//获取密码
				char lsPass[100] = {0};
				GetPrivateProfileStringA(lsUserKey,lsTrader.c_str(),"",lsPass,sizeof(lsPass),lsFilePath);	
				std::string lPass = lsPass;
				TraderInfo lsTraderInfo(lsTrader,lPass);
				mMapTrader[lsTrader] = lsTraderInfo;
			}
			if(strBuf[i+1] == 0)
				break;
			else
				lnLastIndex = i+1;
		}	

	}
	char lsProductKey[50] = "Product";
	char strProductBuf[102400] = {0};
	GetPrivateProfileStringA(lsProductKey,NULL,"",strProductBuf,sizeof(strProductBuf),lsFilePath);
	
	int lnLastProductIndex = 0;
	for(int i = 0; i < 102400; i++)
	{
		if(strProductBuf[i] == 0)
		{
			std::string lsKey = (char*)(strProductBuf + lnLastProductIndex);
			if(!lsKey.empty())
			{
				//获取密码
				char lsProduct[100] = {0};
				GetPrivateProfileStringA(lsProductKey,lsKey.c_str(),"",lsProduct,sizeof(lsProduct),lsFilePath);	
				std::string sPro = lsProduct;
			    msetProduct.insert(sPro);
			}
			if(strProductBuf[i+1] == 0)
				break;
			else
				lnLastProductIndex = i+1;
		}	

	}

}

void CTraderManager::SetTraderStatus(std::string nTraderName,bool bAdd)
{	
	std::map<std::string,TraderInfo>::iterator it2lower= mMapTrader.find(nTraderName);
	if(it2lower!=mMapTrader.end())
	{
		if(bAdd)
			it2lower->second.mnAllocNum++;
		else
			it2lower->second.mnAllocNum--;

	}

   
}

void CTraderManager::FetchOneAviableTrader(TraderInfo & nTraderInfo)
{
	
	//找分配最小的
	
	int lnMin = 100000;
	std::map<std::string,TraderInfo>::iterator lIter = mMapTrader.begin();
	for(; lIter != mMapTrader.end();lIter++)
	{
		if(lIter->second.mnAllocNum < mnMaxAlloc &&  lIter->second.mnAllocNum < lnMin)
		{
			lnMin = lIter->second.mnAllocNum;
			nTraderInfo = lIter->second;
		}
	}
	
}