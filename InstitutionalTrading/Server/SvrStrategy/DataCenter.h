#pragma once
#include "stdafx.h"

struct STraderInstance
{
	STraderInstance()
	{
		memset(this,0,sizeof(*this));		
	};

	STraderInstance(const char* npsStrategy,const char* npsTraderID)
	{
		memset(this,0,sizeof(*this));

		if(npsStrategy != NULL && npsTraderID != NULL)
		{			
			//长度超出，崩溃
			strncpy_s(strTrader, sizeof(strTrader),npsTraderID, sizeof(strTrader)-1);
			strncpy_s(strStrategy, sizeof(strStrategy),npsStrategy, sizeof(strStrategy)-1);
		}
	};

	STraderInstance& operator= (const STraderInstance& other)
	{
		if (this == &other)
			return *this;

		memcpy((char*)this,(char*)&other,sizeof(*this));
		return *this;
	};

	bool operator< (const STraderInstance& r) const
	{
		int irlt;

		irlt=strncmp(strTrader,r.strTrader,sizeof(strTrader));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strStrategy,r.strStrategy,sizeof(strStrategy));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}

	bool operator== (const STraderInstance& r) const
	{
		int irlt;

		irlt=strncmp(strTrader,r.strTrader,sizeof(strTrader));
		if(irlt!=0) return false;

		irlt=strncmp(strStrategy,r.strStrategy,sizeof(strStrategy));
		if(irlt!=0) return false;		

		return true;
	}

	bool operator> (const STraderInstance& r) const
	{
		return !((*this) < r || (*this) == r);
	}		
	
	char	strTrader[25];	//交易员Id
	char	strStrategy[25];	//交易策略名
};
struct SUploadAllInfo
{
	std::string strID; //上传的唯一性ID
	std::string strTraderID;//交易员ID
	std::vector<std::string> vecStrategy;//策略名的数组
	int  nStrategySuccess;				 //策略上传成功数目
	std::vector<std::string> vecIndex;   //指标名的数组
	int  nIndexSuccess;				 //指标上传成功数目
	SUploadAllInfo()
	{
		nStrategySuccess = 0;
		nIndexSuccess	 = 0;
	}
};
class CDataCenter
{
public:
	CDataCenter(void);
	~CDataCenter(void);
private:
	std::map<std::string, SStrategy> m_mapStrategy;//根据策略名字查找相应详情
	CReadWriteLock	m_lockStrategy;

	std::map<STraderInstance, SStrategyInstance> m_mapStrategyInstance;//根据策略名字查找所有策略实例
	CReadWriteLock	m_lockInstance;

	std::map<std::string, SIndex> m_mapSIndex;//根据策略名字查找相应详情
	CReadWriteLock	m_lockSIndex;


	std::multimap<std::string, std::string> m_mapStrategy2Index;
	std::multimap<std::string, std::string> m_mapIndex2Strategy;
	CReadWriteLock	m_lockStrategyIndexRelation;
//////////////////////////////////////////////////////////////////////////
public:
	//备份的数据
	std::map<std::string, SStrategy> m_mapStrategyBack;
	CReadWriteLock	m_lockStrategyBack;	

	std::map<std::string, SIndex> m_mapSIndexBack;
	CReadWriteLock	m_lockSIndexBack;


	std::map<std::string, SUploadAllInfo>  m_mapTrader2uploadAllInfo;		//所有要上传的策略和指标;交易员对应的任务
	CReadWriteLock	m_lockTrader2UploadAllInfo;		

	//得到上传任务
	bool GetUploadAllInfo(std::string strTraderID, SUploadAllInfo& sUploadAllInfo);

	//判断是否存在上传任务
	bool IsUploadAllInfoExist(SUploadAllInfo& uploadAllInfo);

	//增加一个上传任务
	bool AddUploadAllInfo(SUploadAllInfo& uploadAllInfo);
	//删除一个上传策略或者目标
	bool DelUploadAllInfo(SUploadAllInfo& uploadAllInfo);

	//归位原有的上传任务信息
	bool RollBackUploadInfo(SUploadAllInfo& uploadAllInfo);

	bool copyUploadFile(std::string& strsrx, std::string& strDest, std::string strDestSubPath, std::string filename, bool bCopyFromSrc);

	//nType 策略还是指标0：策略 ，1：为指标
	//nSubType 0为dll，1为zip
	bool WriteFile2DB(std::string strFilename, std::string strName, int nType, int nSubType);

	//对上传任务进行计数 nType0为dll，1为zip
	bool calcUploadAllInfo(std::string strTraderID, std::string strName, int nType);


public:
	void Init();
	//策略	
	bool AddStrategy(SStrategy& strategy);//增加一个策略；已经有的则覆盖
	bool DelStrategy(std::string strName);//删除一个策略；禁用状态下才可以删除策略。
	bool SetStragegyStatus(std::string strName, int bStart);// bstart 0:停用 1:启用	
	bool IsStrategyBelongToTrader(std::string strName, std::string strTrader);//判断该策略是不是该交易员的
	bool GetStrategy(std::string strName, SStrategy& strategy);

	bool GetAllUserPriveledgeStrategy(std::string strTrader, vector<SStrategy>& vecStrategy);

	//策略方案
	bool AddInstance(SStrategyInstance& strategy);//增加一个策略方案；已经有的则覆盖
	bool DelInstance(std::string strName, std::string strTrader);//删除一个策略方案；
	bool SetInstanceStatus(std::string strName, std::string strTrader, bool bUse);// bUse 0:暂停 1:运行
	bool IsInstanceBelongToTrader(std::string strName, std::string strTrader);//判断该策略方案是不是该交易员的
	bool GetInstance(std::string strName, std::string strTrader, SStrategyInstance& Instance);
	bool GetAllInstance(std::map<STraderInstance, SStrategyInstance>& mapStrategyInstance);
	
	//指标
	bool AddIndex(SIndex& index);//增加一个指标；已经有的则覆盖
	bool DelIndex(std::string strName);//删除一个指标；
	bool SetIndexStatus(std::string strName, int nPrivate);// bstart 0:停用 1:启用	
	bool IsIndexBelongToTrader(std::string strName, std::string strTrader);//判断该指标是不是该交易员的
	bool GetIndex(std::string strName, SIndex& index);
	bool GetStrategyofIndex(std::string strName, std::vector<std::string>& vecStrategyName);//得到用到该指标的所有策略名

	bool GetAllUserPriveledgeIndex(std::string strName, vector<SIndex>& vecIndex);
	
	//策略指标的关系
	bool DelStrategy2IndexRelation(std::string strategyName);//删除策略与指标的关系

	bool AddStrategy2IndexRelation(std::string strategyName, std::string strIndexName);//增加策略名和指标的关系


	
	
};
