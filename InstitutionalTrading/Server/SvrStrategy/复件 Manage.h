#pragma once
#include "DataCenter.h"
class CManage
{
public:
	CManage(void);
	~CManage(void);
	void Init();
	void StartAllInstance();
	bool loadXMLString(SStrategyInstance& instance, std::string& strXML);
	void StopAllInstance();//停止所有的实例，结算的时候需要
    void HandleAllInstanceTimer(bool bStop);//停止所有的实例的计时器（如果开的话），主从断开时候需要

	std::map<STraderInstance, HANDLE>  m_mapInstance2Handle;//策略方案和进程之间的关系
	CReadWriteLock	m_lockInstance2Handle;

	void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst );
private:
	CDataCenter m_DataCenter;
public:
	bool UploadStartAll(UploadStart& start, int& nError, std::string& strError);
	bool UploadEndAll(UploadEnd& end, int& nError);

public:
	bool UploadBaseStrategy(SStrategy& strategy, int& nError);
	bool ParsePara(std::string strPara, std::vector<SStrategy2Index>& vec);//解析上传参数，把xml里面策略和指标的关系
	bool UploadDllFile(std::string strName, std::string strUploader, void* pData, int nLength, int& nError);
	bool UploadZIPFile(std::string strName, std::string strUploader, void* pData, int nLength, int& nError);
	bool AddStrategyIndexRelation(std::string strName, std::string strIndex, int& nError);

	//下载所有策略基本信息
	bool DownloadAllBaseStategy(std::string strUploader, std::vector<SStrategy>& vecIndex, int& nError);
	bool DownloadBaseStrategy(std::string strName/*, std::string strUploader*/, SStrategy& strategy, int& nError);
	bool DownloadDllFile(std::string strName, std::string strUploader, void*& pData, int& nLength, int& nError);
	bool DownloadZIPFile(std::string strName, std::string strUploader, void*& pData, int& nLength, int& nError);

	
	//上传指标基本信息
	bool UploadBaseIndex(SIndex& Index, int& nError);
	//上传指标文件dll
	bool UploadIndexFile(std::string strName, std::string strUploader, int nType, void* pData, int nLength, int& nError);

	//下载指标基本信息
	bool DownloadBaseIndex(std::string strName/*, std::string strUploader*/, SIndex& index, int& nError);
	//下载指标文件dll
	bool DownloadIndexFile(std::string strName, std::string strUploader, int nType, void*& pData, int& nLength, int& nError);

	//下载所有指标基本信息
	bool DownloadAllBaseIndex(std::string strUploader, std::vector<SIndex>& vecIndex, int& nError);



	//启用停用策略
	bool UseStrategy(SUseStrategy& useStrategy, int& nError);
	//删除策略
	bool DelStrategy(SDelStrategy& sDel, int& nError);


	//下载所有策略方案信息
	bool DownloadAllInstance(std::string strUploader,std::vector<SStrategyInstance>& vecInstance, int& nError);

	//增加策略方案
	bool AddInstance(SStrategyInstance& strategy, int& nErrorCode);

	//修改策略方案
	bool ModifyInstance(SStrategyInstance& strategy, int& nErrorCode);

	//删除某交易员相关的策略方案
	bool DelInstance_ByTrader(SDelStrategy& sDel, int& nErrorCode);
	//启用停用策略方案
	bool UseInstance(SUseStrategy& useStrategy, int& nError);

	//交易员离线上线控制方案是否运行
	bool OnOffLineProcessInstance(std::string strTrader, bool bOnline);//bOnline true:上线； false 下线

	bool BeginSStrategyInstanceProcess(SStrategyInstance& instance);

	void SendMessage2Trader(std::string strUserName, MessageInfo& MsgInfo);

	//退出某交易员的某个策略方案
	bool TerminatInstanceThread(std::string strName, std::string strUploader);

	//指标或者策略有正在运行的
	bool HasStrategyRun(std::vector<std::string>& vecStrategy, std::vector<std::string>& vecIndex, std::string strTrader,int& nError, std::string& strError);
};
