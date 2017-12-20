#pragma once
#include <list>
#include <string>
#include <map>
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"

using namespace std;

#define  NULL_VALUE "无"
class CStrategyData
{
public:
    CStrategyData();
	virtual ~CStrategyData();

	/*增加一条策略参数*/
	void AddStrategyPara(StrategyPara& data);
	/*删除一条策略参数*/
	void DeleteStrategyPara( const string& name);
	/*增加一条策略指标*/
	void AddStrategyIndicator(const string& Strategyname,StrategyIndicator& data);
    /*通过策略指标的名称一条策略指标*/
	bool GetStrategyIndicator(const string& szName,StrategyIndicator& data);
	/*删除一条策略指标*/
	void DeleteStrategyIndicator(const string& Strategyname,const string& Indicatorname);
	/*设置策略参数单选框下拉框时的选项*/
	void SetParaSubItem(const string&szTitle,list<StrategyPrarSubItem>& ListPrarSubItem);
	/*得到策略参数单选框下拉框时的选项*/
	void GetParaSubItem( map<string,list<StrategyPrarSubItem>>& PrarSubItem);
	void GetSubItem(const string name,list<StrategyPrarSubItem>& listSubItem);
	bool CheckStrategyParaName(const std::string& name);
	void SetParaType2NameMap(multimap<string,string>& data);
	void GetParaType2NameMap(multimap<string,string>& data);
	/*得到所有策略参数map*/
	void GetStrategyPara(std::map<std::string,StrategyPara>& data);
	std::map<std::string,StrategyPara>& GetStrategyParams();
	/*得到新建策略的所有指标配置*/
	void GetAllStrategyIndicator(std::map<std::string,StrategyIndicator>& lMapFiled);
	const string GetStrategyName();
	void SetStrategyName(const string name);
	const string GetStrategyNickName();
	void SetStrategyNickName(const string name);
	const string GetStrategyVersion();
	void SetStrategyVersion(const string Version);
	void  SetStrategyPrivate(const int data);
	int  GetStrategyPrivate();
	void GetIndicator(list<string>& data);

	bool IsOwner(const string& strCurrUserID);
	void SetOwner(const string& strName);
	string GetOwner();

	bool HasUpload();
	void ResetUpdoad(bool bHas);

	int GetServerUseStatus();
	void SetServerUseStatus(int nStatus);

	void SetDescribe(const string& strDescribe);
	string GetDescribe();

	int GetInstrumentCount();
	void SetInstrumentCount(int nInstrumentCount);


protected:

	string		m_strategyName;		//策略名	
	string		m_strategyNickName;	//策略中文名
	string		m_Version;			//策略版本号
	string		m_strDescribe;	   //策略描述
	int			m_nPrivate;			//1:私有；2共享
	string			m_strOwner;			//策略的拥有者
	bool			m_bHasUpload;		//是否已上传过
	int				m_nServerUseStatus;	//服务器指标禁用启用状态 1 启用，2 禁用
	int				m_nInstrumentCount;//策略的合约个数


	std::map<std::string,StrategyPara>                                   m_mapStrategyPara;//策略参数map
	multimap<string,string>                                              m_multimapstrategyParaType2Name;//策略参数类型和策略参数名称映射
	std::map<std::string,StrategyIndicator>                              m_mapStrategyIndicator;//策略的指标配置MAP
	map<string,string>                                                   m_mapIndicator;//策略指标名称到指标映射 一个策略用到了哪几个指标
	map<string,list<StrategyPrarSubItem>>                                m_mapPrarSubItem; //新建策略参数配置单选框下拉框时的选项



};
