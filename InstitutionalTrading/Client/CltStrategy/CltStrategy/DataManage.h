#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include "CommonStruct.h"
#include "..\CommonClasses\IndicatorDataEx.h"
#include "..\CommonClasses\StrategyDataEx.h"
using namespace std;

class CDataManage
{
protected:
	CDataManage(void);
	~CDataManage(void);

public:
	static CDataManage* NewInstance();
	static void DestroyInstance();
	static CDataManage* m_pInstance;

public:
	void Clear();
 //   /*新建指标中新增参数时检查参数名是否重复*/
	//bool CheckName(const std::string& name);
	bool CheckIndicatorName(const std::string& name);
	bool CheckStrategyName(const std::string& name);
	//static const char* IndiPara2String(IndicatorParaType eType);
	//static IndicatorParaType IndiPara2Enum( const string& eType);
	//*增加一条指标*/
     void AddIndcator(CIndicatorDataEx& data);
	void AddDownloadIndicator(CIndicatorDataEx& data);
	///*删除一条指标*/
     void DeleteIndcator( const string& name);
	 /*得到一条指标*/
	 bool GetIndicator( const string& name,CIndicatorDataEx& data);
	 bool GetDownloadIndicator( const string& name,CIndicatorDataEx& data);
	 bool IsIndicatorExist(const string& name);
	 bool IsInStrategyExist(const string& name);
	//static IndicatorType IndiType2Enum( const string& eType);
	//static IndicatorFile IndiFile2Enum( const string& eType);
	///*新建指标时清空指标参数MAP*/
	//void ClearIndiPara();
	/*得到一条指标的所有参数*/
	bool GetIndiPara(const string& IndiName,std::map<std::string, IndicatorPara> &  lMapFiled);
	//static DisplayType DisplayType2Enum(const string& eType);
	//static StrategyParaType StrategyType2Enum(const string& eType);
	//判断输入的字符串是否是INT
	bool IsInteger(string& str);
	//static StrategyDirection StrategyDirection2Enum( const string& eType);
	/*bool CheckStrategyParaName(const std::string& name);*/
	//static const char* StrategyType2String(StrategyParaType eType);
	//static const char* DirectionType2String(StrategyDirection eType);
	//static const char* DisplayType2String(DisplayType eType);
	/*得到所有指标*/
	void GetAllIndicator( std::map<std::string, CIndicatorDataEx> & lMapFiled);
	void GetAllIndicator_Download( std::map<std::string, CIndicatorDataEx> & lMapFiled, bool bOnlyMyself=false);
	/*通过指标名得到指标信息*/
	bool GetIndicatorInfobyName(const string& IndiName,CIndicatorDataEx& data);
	//static const char* Bool2String( bool bData);
	/*增加一条策略*/
	void AddStrategy(CStrategyDataEx& data);
	void AddDownloadStrategy(CStrategyDataEx& data);
	/*删除一条策略*/
	void DeleteStrategy( const string& name);
	/*得到一条策略*/
	bool GetStrategy( const string& name,CStrategyDataEx& data);
	bool GetDownloadStrategy( const string& name,CStrategyDataEx& data);
	/*得到所有的策略*/
	void GetAllStrategy(std::map<std::string, CStrategyDataEx> & lMapFiled);
	void GetAllStrategy_Download(std::map<std::string, CStrategyDataEx> & lMapFiled, bool bOnlyMyself=false);
	/*得到某个策略用到的指标*/
	void GetIndicator(const string& name,list<string>& data);
	void SetIndicatorDiaplay(list<string>& data,bool state);
	void indicatorDisplyMapClear();
	/*判断一条指标私有还是公有*/
	int IsPrivate_Indicator(const std::string& name);
	/*判断一条策略私有还是公有*/
	int IsPrivate_Strategy(const std::string& name);
	/*是否高亮某条指标*/
	bool IsRelativeIndi(const string& name);

	/*判断一条指标是否为指定用户所有*/
	bool IsOwner_Indicator(const std::string& name, const std::string strUserID);
	/*判断一条策略是否为指定用户所有*/
	bool IsOwner_Strategy(const std::string& name, const std::string strUserID);

protected:


	// 2014年4月3日，新定的功能封装方法
	//CIndicatorData		// 1.  封装现有的Indicator的接口和数据
	//CIndicatorDataEx	// 2.  导入和导出indicator的xml
	//					// 3.  生成对应的c#代码，从原GenerateCodes转移过来
	//CStrategyData		// 1.  封装现有的Strategy的接口和数据
	//CStrategyDataEx		// 2.  导入和导出Strategy的xml
	//					// 3.  生成对应的c#代码，从原GenerateCodes转移过来
	std::map<std::string, CIndicatorDataEx>		m_mapIndicatorInfo;// 保存所有的指标，能区分是私有还是共享
	std::map<std::string, CStrategyDataEx>		m_mapStrategyInfo;// 保存所有的策略，能区分是私有还是共享
	//// 1.  新增一个获得选中策略相关的指标列表函数。
	//// 2.  新增一个Check指标是否被策略使用的函数。
	//// 3.  获得一个指标对象
	//// 4.  获得一个策略对象
	std::map<string,int>      m_mapIndicatorDiaplay;//指标名，发布时指标被使用到的次数

	std::map<std::string, CIndicatorDataEx>		m_mapDownloadIndicators;
	std::map<std::string, CStrategyDataEx>		m_mapDownloadStrategies;

};
