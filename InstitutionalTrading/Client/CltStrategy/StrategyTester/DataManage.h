#pragma once
#include <string>
#include <map>
#include <set>
#include <list>
#include "CommonStruct.h"
#include "Module-Misc2\KernelStruct.h"

using namespace std;

class CDataManage
{
protected:
	CDataManage(void);
	~CDataManage(void);

public:
	const std::map<std::string, PlatformStru_InstrumentInfo>& 
	GetInstrument() {
		return m_mapInstrument;
	};
	const std::list<PlatformStru_DepthMarketData>& 
	GetQuotList() {
		return m_lstQuots;
	}
	//const std::map<std::string, PlatformStru_DepthMarketData>& 
	//GetMarketDepth() {
	//	return m_mapMDQuot;
	//}
	// 通过交易所ID获得相关的品种
	bool GetProductID(const std::string& strExchangID, std::set<std::string>& setRetProductID);
	// 通过品种ID获得相关的合约信息
	bool GetInstrument(const std::string& strProductID, 
			std::map<std::string, PlatformStru_InstrumentInfo>& mapRetInstrument);
	bool GetExchangeID(const std::string& strInstrumentID, std::string& strExchangeID);
	bool GetProductID(const std::string& strInstrumentID, std::string& strProductID);

	bool GetInstrumentInfo(const std::string& strInstrumentID, PlatformStru_InstrumentInfo& ret);
	bool GetMarginRate(const std::string& strInstrumentID, PlatformStru_InstrumentMarginRate& ret);
	bool GetCommissionRate(const std::string& strInstrumentID, PlatformStru_InstrumentCommissionRate& ret);

public:
	bool ImportQuot(const std::string& strFile);
	bool ImportInstrument(const std::string& strFile);
	bool ImportRate(const std::string& strFile);

protected:
	//bool ConvToQuot(const std::string& strQuot, PlatformStru_DepthMarketData& ret);
	bool ConvToInstrument(const std::string& strInstrument, PlatformStru_InstrumentInfo& ret);

protected:

	std::list<PlatformStru_DepthMarketData>
							m_lstQuots;
	std::map<std::string, PlatformStru_InstrumentInfo>
							m_mapInstrument;
	//std::map<std::string, PlatformStru_DepthMarketData>
	//						m_mapMDQuot;
	std::map<std::string, PlatformStru_InstrumentMarginRate>
							m_mapMarginRate;
	std::map<std::string, PlatformStru_InstrumentCommissionRate>
							m_mapCommissionRate;

};
