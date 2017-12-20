#pragma once

#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include <string>
#include <map>

class CIndicatorData
{
public:
    CIndicatorData();
	virtual ~CIndicatorData();

public:
	static CIndicatorData* NewInstance();
	static void DestroyInstance();
	static CIndicatorData* m_pInstance;

	/*新建指标中新增参数时检查参数名是否重复*/
	bool CheckName(const std::string& name);

	void AddIndcatorPara(IndicatorPara& data);
	void DeleteIndcatorPara( const std::string& name);
	
	/*新建指标时清空指标参数MAP*/
	void ClearIndiPara();
	/*得到一条指标的所有参数*/
	void GetIndiPara(std::map<std::string, IndicatorPara> &  lMapFiled);
	const std::string GetIndicatorName();
	void SetIndicatorName(const std::string name);
	std::string GetIndicatorDescribe();
	void SetIndicatorDescribe(const std::string data);
	void SetIndicatorType( IndicatorType data);
	IndicatorType GetIndicatorType();
	void SetIndicatorFile(const std::string data);
	std::string GetIndicatorFile();
	void SetIndicatorPrivate(const int data);
	int GetIndicatorPrivate();

	bool IsOwner(const std::string& strCurrUserID);
	void SetOwner(const std::string& strName);
	std::string GetOwner();

	bool HasUpload();
	void ResetUpdoad(bool bHas);

	int GetServerUseStatus();
	void SetServerUseStatus(int nStatus);
    
protected:

	std::string				m_szName;			//指标名称
	IndicatorType			m_eIndiType;		// 指标类型
	std::string				m_szIndiFile;		//指标文件夹
	std::string				m_szDescribe;		//指标描述
	int						m_nPrivate;			//1:私有；2共享
	std::string				m_strOwner;			//指标的拥有者
	bool					m_bHasUpload;		//是否已上传过
	int						m_nServerUseStatus;	//服务器指标禁用启用状态

	std::map<std::string, IndicatorPara>	m_mapIndcatorPara;//指标参数配置使用的参数MAP,<参数名，参数机构>

};
