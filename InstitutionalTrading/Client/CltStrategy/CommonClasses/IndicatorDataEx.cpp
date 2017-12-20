
#include "stdafx.h"
#include "IndicatorDataEx.h"
#include "Tools.h"
#include "..\CommonClasses\ConvEnum.h"


using namespace System::Xml;


CIndicatorDataEx::CIndicatorDataEx() :
	CIndicatorData()
{
}

CIndicatorDataEx::~CIndicatorDataEx()
{
}

bool CIndicatorDataEx::MakeCode(const std::string& strTemplateFile, std::string& strRetCode)
{
    String^ strText = "";
    String^ strItem = "";
    String^ strTemp = "";
	char strBuff[128] = {0};
    try {
		StreamReader^ sr = gcnew StreamReader(gcnew String(strTemplateFile.c_str()));
        strText = sr->ReadToEnd();    //此方法等于一口气读完
        sr->Close();
    }
    catch (Exception^ e) {
        return false;
    }

    // 1.  替换名称和类型
	sprintf_s(strBuff, sizeof(strBuff)-1, "%u", m_eIndiType);
	strText = strText->Replace("<#IndicatorType#>", gcnew String(strBuff));
	strText = strText->Replace("<#IndicatorName#>", gcnew String(m_szName.c_str()));
    strText = strText->Replace("<#ClassName#>", gcnew String(m_szName.c_str()));

    // 2.  生成成员变量
    strItem = "";
	std::map<std::string, IndicatorPara>::iterator itParam;
	for(itParam = m_mapIndcatorPara.begin(); itParam != m_mapIndcatorPara.end(); itParam++) {
        strItem += "\r\n";
        strItem += "        /// <summary>\r\n";
        //strItem += "        /// ";
		//strTemp = param->strDescription.Replace("\r\n", "\r\n        /// ");
        //strItem += strTemp;
        strItem += "\r\n        /// </summary>\r\n";
		strTemp = "        " + gcnew String(IndiPara2String(itParam->second.eType)) + " " 
				+ gcnew String(itParam->second.szName) + " = " 
				+ gcnew String(itParam->second.szDefault) + ";\r\n";
        strItem += strTemp;
    }
    strText = strText->Replace("<#MembarParams#>", strItem);

    // 3.  生成带参数的构造函数
	strItem = gcnew String(m_szName.c_str());
    strItem += "(";
	for(itParam = m_mapIndcatorPara.begin(); itParam != m_mapIndcatorPara.end(); itParam++) {
		strTemp = gcnew String(IndiPara2String(itParam->second.eType)) 
				+ " " + gcnew String(itParam->second.szName) + ", ";
        strItem += strTemp;
    }
    strItem += ")";
    strItem = strItem->Replace(", )", ")");
    strText = strText->Replace("<#ClassConstract#>", strItem);

    // 4.  生成成员变量赋值代码
    strItem = "";
	for(itParam = m_mapIndcatorPara.begin(); itParam != m_mapIndcatorPara.end(); itParam++) {
		strTemp = "            this." + gcnew String(itParam->second.szName) 
				+ " = " + gcnew String(itParam->second.szName) + ";\r\n";
        strItem += strTemp;
    }
    strText = strText->Replace("<#MemberParamsAssign#>", strItem);

	strRetCode = Tools::String2string(strText);
    return true;
}

bool CIndicatorDataEx::MakeXml(std::string& strRetXml)
{
    String^ strItem = "";
	char csBuff[128] = {0};
	sprintf_s(csBuff, sizeof(csBuff)-1, "%u", m_eIndiType);
    strItem += "<indicator" + " type = '" 
			+ gcnew String(csBuff)
			+ "' name = '" + gcnew String(m_szName.c_str())
			+ "' folder = '" + gcnew String(m_szIndiFile.c_str()) + "'>\r\n";
    strItem += "    <description>\r\n";
	strItem += gcnew String(m_szDescribe.c_str());
    strItem += "\r\n    </description>\r\n";
    strItem += "    <params>\r\n";
	std::map<std::string, IndicatorPara>::iterator itParam;
	for(itParam = m_mapIndcatorPara.begin(); itParam != m_mapIndcatorPara.end(); itParam++) {
        strItem += "        <param type='" 
				+ gcnew String(IndiPara2String(itParam->second.eType)) 
				+ "' name='" + gcnew String(itParam->second.szName)
				+ "' default='" + gcnew String(itParam->second.szDefault) 
				+ "'>" + "</param>\r\n";
    }
    strItem += "    </params>\r\n";
    strItem += "</indicator>\r\n";
    // 返回参数列表
    strRetXml = Tools::String2string(strItem);
    
    return true;
}

bool CIndicatorDataEx::MakeCodeToFile(String^ strTemplateFile, String^ strPathFile)
{
	std::string strCode("");
    bool bRet = false;
    if (MakeCode(Tools::String2string(strTemplateFile), strCode)) {
        try {
            StreamWriter^ sw = gcnew StreamWriter(strPathFile);
			sw->Write(gcnew String(strCode.c_str()));
            sw->Close();
            bRet = true;
        }
        catch (Exception^ e) {
        }
    }
    return bRet;
}

bool CIndicatorDataEx::MakeXmlToFile(String^ strPathFile)
{
	std::string strXml("");
    bool bRet = false;
    if (MakeXml(strXml)) {
        try { 
            StreamWriter^ sw = gcnew StreamWriter(strPathFile);
			sw->Write(gcnew String(strXml.c_str()));
            sw->Close();
            bRet = true;
        }
        catch (Exception^ e) {
        }
    }
    return bRet;
}

bool CIndicatorDataEx::ImportFromXml(std::string& strXml)
{
	String^ valtype = nullptr;
	String^ valtext = nullptr;
	String^ valdefault = nullptr;
    XmlDocument^ doc = gcnew XmlDocument();
    doc->LoadXml(gcnew String(strXml.c_str()));

    XmlNode^ nodeRoot = doc->SelectSingleNode("indicator");		//获取indicator节点 
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("type");
	int nType = atoi(Tools::String2string(valtext).c_str());
	m_eIndiType = (IndicatorType)nType;
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("name");
	m_szName = Tools::String2string(valtext);
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("folder");
	m_szIndiFile = Tools::String2string(valtext);

    XmlNode^ nodeDescription = nodeRoot->SelectSingleNode("description");		//获取indicator节点 
	valtext = ((XmlElement^)nodeDescription)->InnerText;
	m_szDescribe = Tools::String2string(valtext);

	m_mapIndcatorPara.clear();
    XmlNodeList^ nodeList = nodeRoot->SelectSingleNode("params")->ChildNodes;		//获取Params节点的所有子节点 
    for each(XmlNode^ xn in nodeList) {				//遍历所有子节点 
        XmlElement^ xe=(XmlElement^)xn;				//将子节点类型转换为XmlElement类型 
		IndicatorPara param = {0};
        valtype = xe->GetAttribute("type");
		param.eType = IndiPara2Enum(Tools::String2string(valtype));
        valtext = xe->GetAttribute("name");
		strncpy(param.szName, Tools::String2string(valtext).c_str(), sizeof(param.szName)-1);
        valdefault = xe->GetAttribute("default");
		strncpy(param.szDefault, Tools::String2string(valtext).c_str(), sizeof(param.szDefault)-1);
		AddIndcatorPara(param);
    }
	return true;
}

bool CIndicatorDataEx::ImportFromXmlFile(std::string& strFile)
{
	try {
		StreamReader^ sr = gcnew StreamReader(
				gcnew String(strFile.c_str()));
		String^ xml = nullptr;
		xml = sr->ReadToEnd();
		sr->Close();

		return ImportFromXml(Tools::String2string(xml));
	}
	catch (Exception^ e) {
	}
	return false;
}

