
#include "stdafx.h"
#include "DataManage.h"
#include <string>


using namespace System::IO;
using namespace System::Xml;
using namespace System::Diagnostics;


CDataManage::CDataManage()
{
}

CDataManage::~CDataManage()
{
}

// 通过交易所ID获得相关的品种
bool CDataManage::GetProductID(const std::string& strExchangID, std::set<std::string>& setRetProductID)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;
	for(it = m_mapInstrument.begin(); it != m_mapInstrument.end(); it++) {
		if(strExchangID.compare(it->second.ExchangeID)==0) {
			setRetProductID.insert(it->second.ProductID);
		}
	}
	return true;
}

// 通过品种ID获得相关的合约信息
bool CDataManage::GetInstrument(const std::string& strProductID, 
		std::map<std::string, PlatformStru_InstrumentInfo>& mapRetInstrument)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;
	for(it = m_mapInstrument.begin(); it != m_mapInstrument.end(); it++) {
		if(strProductID.compare(it->second.ProductID)==0) {
			mapRetInstrument[it->first] = it->second;
		}
	}
	return true;
}

bool CDataManage::GetInstrumentInfo(const std::string& strInstrumentID, PlatformStru_InstrumentInfo& ret)
{
	std::map<std::string, PlatformStru_InstrumentInfo>::iterator it;
	bool bRet = false;
	if((it = m_mapInstrument.find(strInstrumentID)) != m_mapInstrument.end()) {
		ret = it->second;
		bRet = true;
	}
	return bRet;
}

bool CDataManage::GetMarginRate(const std::string& strInstrumentID, PlatformStru_InstrumentMarginRate& ret)
{
	std::map<std::string, PlatformStru_InstrumentMarginRate>::iterator it;
	bool bRet = false;
	if((it = m_mapMarginRate.find(strInstrumentID)) != m_mapMarginRate.end()) {
		ret = it->second;
		bRet = true;
	}
	return bRet;
}

bool CDataManage::GetCommissionRate(const std::string& strInstrumentID, PlatformStru_InstrumentCommissionRate& ret)
{
	std::map<std::string, PlatformStru_InstrumentCommissionRate>::iterator it;
	bool bRet = false;
	if((it = m_mapCommissionRate.find(strInstrumentID)) != m_mapCommissionRate.end()) {
		ret = it->second;
		bRet = true;
	}
	return bRet;
}

bool CDataManage::ImportQuot(const std::string& strFile)
{
	m_lstQuots.clear();
    try { 
        StreamReader^ sr = gcnew StreamReader(gcnew String(strFile.c_str()));
        String^ line = nullptr;
		line = sr->ReadLine();
        while(line = sr->ReadLine()) {
			PlatformStru_DepthMarketData quot;
			_ConvToQuot(Tools::String2string(line), quot);
			m_lstQuots.push_back(quot);
        }
        sr->Close();
		return true;
    }
    catch (Exception^ e) {
		return false;
    }
}

bool CDataManage::ImportInstrument(const std::string& strFile)
{
	m_mapInstrument.clear();
    try { 
        StreamReader^ sr = gcnew StreamReader(gcnew String(strFile.c_str()));
        String^ line = nullptr;
		line = sr->ReadLine();
        while(line = sr->ReadLine()) {
			PlatformStru_InstrumentInfo instrument;
			ConvToInstrument(Tools::String2string(line), instrument);
			m_mapInstrument[instrument.InstrumentID] = instrument;
        }
        sr->Close();
		return true;
    }
    catch (Exception^ e) {
		return false;
    }
}

bool CDataManage::ImportRate(const std::string& strFile)
{
	String^ valtext = nullptr;
	String^ valdefault = nullptr;
    XmlDocument^ doc = gcnew XmlDocument();

	try {
		doc->Load(gcnew String(strFile.c_str()));

		XmlNode^ nodeRoot = doc->SelectSingleNode("Rate");		//获取indicator节点 
		XmlNodeList^ nodeList = nodeRoot->SelectSingleNode("CommissionRate")->ChildNodes;		//获取CommissionRate节点的所有子节点 

		m_mapCommissionRate.clear();
		for each(XmlNode^ xn in nodeList) {				//遍历所有子节点 
			XmlElement^ xe=(XmlElement^)xn;				//将子节点类型转换为XmlElement类型
			PlatformStru_InstrumentCommissionRate rate;
			valtext = xe->InnerText;
			strncpy(rate.InstrumentID, Tools::String2string(valtext).c_str(), sizeof(rate.InstrumentID)-1);
			valtext = xe->GetAttribute("ORBM");
			rate.OpenRatioByMoney = atof(Tools::String2string(valtext).c_str());
			valtext = xe->GetAttribute("ORBV");
			rate.OpenRatioByVolume = atof(Tools::String2string(valtext).c_str());
			valtext = xe->GetAttribute("CRBM");
			rate.CloseRatioByMoney = atof(Tools::String2string(valtext).c_str());
			valtext = xe->GetAttribute("CRBV");
			rate.CloseRatioByVolume = atof(Tools::String2string(valtext).c_str());
			valtext = xe->GetAttribute("CTRBM");
			rate.CloseTodayRatioByMoney = atof(Tools::String2string(valtext).c_str());
			valtext = xe->GetAttribute("CTRBV");
			rate.CloseTodayRatioByVolume = atof(Tools::String2string(valtext).c_str());
			m_mapCommissionRate[rate.InstrumentID] = rate;
		}

		nodeList = nodeRoot->SelectSingleNode("MarginRate")->ChildNodes;		//获取MarginRate节点的所有子节点 
		m_mapMarginRate.clear();
		for each(XmlNode^ xn in nodeList) {				//遍历所有子节点 
			XmlElement^ xe=(XmlElement^)xn;				//将子节点类型转换为XmlElement类型
			PlatformStru_InstrumentMarginRate rate;
			rate.HedgeFlag = THOST_FTDC_HF_Speculation;
			valtext = xe->InnerText;
			strncpy(rate.InstrumentID, Tools::String2string(valtext).c_str(), sizeof(rate.InstrumentID)-1);
			valtext = xe->GetAttribute("LMBM");
			rate.LongMarginRatioByMoney = atof(Tools::String2string(valtext).c_str());
			valtext = xe->GetAttribute("LMBV");
			rate.LongMarginRatioByVolume = atof(Tools::String2string(valtext).c_str());
			valtext = xe->GetAttribute("SMBM");
			rate.ShortMarginRatioByMoney = atof(Tools::String2string(valtext).c_str());
			valtext = xe->GetAttribute("SMBV");
			rate.ShortMarginRatioByVolume = atof(Tools::String2string(valtext).c_str());
			m_mapMarginRate[rate.InstrumentID] = rate;
		}

		return true;
	}
	catch(Exception^ e) {
		return false;
	}
}

bool CDataManage::ConvToInstrument(const std::string& strInstrument, PlatformStru_InstrumentInfo& ret)
{
	std::vector<std::string> vecItem;
    int startpos=0,endpos=0;
	std::string strTrueLine=strInstrument+",";
    while(true) {
        endpos=strTrueLine.find(',',startpos);
        if(endpos<0) 
            break;
		vecItem.push_back(strTrueLine.substr(startpos,endpos-startpos));
        startpos=endpos+1;   
    }
	if(vecItem.size()<25) 
		return false;

    int index=0;
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.InstrumentID,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.ExchangeID,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.InstrumentName,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.ExchangeInstID,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.ProductID,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.ProductClass=vecItem[index++][0];
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.DeliveryYear=atoi(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.DeliveryMonth=atoi(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.MaxMarketOrderVolume=atoi(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.MinMarketOrderVolume=atoi(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.MaxLimitOrderVolume=atoi(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.MinLimitOrderVolume=atoi(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.VolumeMultiple=atoi(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.PriceTick=atof(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.CreateDate,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.OpenDate,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.ExpireDate,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.StartDelivDate,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    strcpy(ret.EndDelivDate,vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.InstLifePhase=vecItem[index++][0];
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.IsTrading=atoi(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.PositionType=vecItem[index++][0];
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.PositionDateType=vecItem[index++][0];
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.LongMarginRatio=atof(vecItem[index++].c_str());
	Debug::Write(gcnew String(vecItem[index].c_str()) + ",\t");
    ret.ShortMarginRatio=atof(vecItem[index++].c_str());
	Debug::WriteLine("");
	return true;
}
