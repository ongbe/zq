
#include "stdafx.h"
#include "StrategyDataEx.h"
#include "Tools.h"
#include "..\CommonClasses\ConvEnum.h"


using namespace System::Xml;


CStrategyDataEx::CStrategyDataEx() : 
	CStrategyData()
{
}

CStrategyDataEx::~CStrategyDataEx()
{
}

bool CStrategyDataEx::MakeCode(const std::string& strTemplateFile, std::string& strRetCode)
{
    String^ strText = "";
    String^ strItem = "";
    try {
		StreamReader^ sr = gcnew StreamReader(gcnew String(strTemplateFile.c_str()));
        strText = sr->ReadToEnd();    //此方法等于一口气读完
        sr->Close();
    }
    catch (Exception^ e) {
        return false;
    }

    // 1.  替换名称和类型
	strText = strText->Replace("<#StrategyNameEng#>", gcnew String(m_strategyName.c_str()));
	strText = strText->Replace("<#StrategyName#>", gcnew String(m_strategyNickName.c_str()));
    strText = strText->Replace("<#ClassName#>", gcnew String(m_strategyName.c_str()));

    // 2.  生成成员变量
    strItem = "";
	std::map<std::string,StrategyPara>::iterator itParam;
	for(itParam = m_mapStrategyPara.begin(); itParam != m_mapStrategyPara.end(); itParam++) {
        strItem += "        \r\n";
        strItem += "        /// <summary>\r\n";
        strItem += "        /// 参数描述: \r\n";
        //strItem += "        /// @title = \"" + param.strTitle + "\"\r\n";
        //strItem += "        /// @default = " + param.strDefault + "\r\n";
        //strItem += "        /// @readonly = " + (param.bReadOnly ? "true" : "false") + "\r\n";
        strItem += "        /// </summary>\r\n";

		String^ strType =  gcnew String(StrategyType2String(itParam->second.eType));
		if(strType=="DateTime") {
			strItem += "        public " 
					+ strType + " " + gcnew String(itParam->second.szName) 
					+ " = Convert.ToDateTime(\"" + gcnew String(itParam->second.szDefault) + "\");\r\n";
		}
		else if(strType=="String") {
			strItem += "        public " 
					+ strType + " " + gcnew String(itParam->second.szName) 
					+ " = \"" + gcnew String(itParam->second.szDefault) + "\";\r\n";
		}
		else {
			strItem += "        public " 
					+ strType + " " + gcnew String(itParam->second.szName) 
					+ " = " + gcnew String(itParam->second.szDefault) + ";\r\n";
		}
    }
	std::map<std::string,StrategyIndicator>::iterator itIndicator;
	for(itIndicator = m_mapStrategyIndicator.begin(); 
			itIndicator != m_mapStrategyIndicator.end(); itIndicator++) {
        strItem += "        \r\n";
        strItem += "        /// <summary>\r\n";
		strItem += "        /// 指标: " + gcnew String(itIndicator->second.szTitle) + "\r\n";
        strItem += "        /// </summary>\r\n";
        strItem += "        " 
				+ gcnew String(itIndicator->second.szName) 
				+ " " + gcnew String(itIndicator->second.szTitle) + ";\r\n";
    }
    strText = strText->Replace("<#MembarParams#>", strItem);

    // 3.  生成构造函数
	strItem = gcnew String(m_strategyName.c_str());
    strText = strText->Replace("<#ClassConstract#>", strItem);

    // 4.  生成参数设置函数
    strItem = "(";
	for(itParam = m_mapStrategyPara.begin(); itParam != m_mapStrategyPara.end(); itParam++) {
		strItem += gcnew String(StrategyType2String(itParam->second.eType)) 
				+ " " + gcnew String(itParam->second.szName) + ", ";
        // 不能设置缺省值，否则会出现Compiler Error CS0241
        //strItem += param.strType + " " + param.strName + " = " + param.strDefault + ", ";
    }
    strItem += ")";
    strItem = strItem->Replace(", )", ")");
    strText = strText->Replace("<#ResetParamsDeclare#>", strItem);

    // 5.  生成参数设置函数实现代码
    strItem = "";
	for(itParam = m_mapStrategyPara.begin(); itParam != m_mapStrategyPara.end(); itParam++) {
        strItem += "            newobj." 
				+ gcnew String(itParam->second.szName) + " = " 
				+ gcnew String(itParam->second.szName) + ";\r\n";
    }
    strText = strText->Replace("<#ResetParamsBody#>", strItem);

    // 6.  生成指标初始化代码
    strItem = "";
	for(itIndicator = m_mapStrategyIndicator.begin(); 
			itIndicator != m_mapStrategyIndicator.end(); itIndicator++) {
		strItem += "            " + gcnew String(itIndicator->second.szTitle) 
				+ " = new " + gcnew String(itIndicator->second.szName) 
				+ " (" + gcnew String(itIndicator->second.szPara) + ");\r\n";
		strItem += "            " + gcnew String(itIndicator->second.szTitle) 
				+ ".IndicatorKey = \"" + gcnew String(itIndicator->second.szName) 
				+ "." + gcnew String(itIndicator->second.szTitle) + "\";\r\n";
		if(itIndicator->second.nInstrument==0)
            strItem += "            " + "SaveIndicator (" 
				+ gcnew String(itIndicator->second.szTitle) + ", strInstruments);\r\n";
        else
            strItem += "            " + "SaveIndicator (" 
				+ gcnew String(itIndicator->second.szTitle) 
				+ ", strInstruments[" + (itIndicator->second.nInstrument-1) + "]);\r\n";
    }
    strText = strText->Replace("<#InitIndicator#>", strItem);

    strRetCode = Tools::String2string(strText);
    return true;
}

bool CStrategyDataEx::MakeXml(std::string& strRetXml)
{
    String^ strItem = "";
    strItem += "<strategy" 
			+ " id = '" + gcnew String(m_strategyName.c_str()) 
			+ "' name = '" + gcnew String(m_strategyNickName.c_str())
			+ "' instrumentcount = '" + m_nInstrumentCount
			+ "' version = '" + gcnew String(m_Version.c_str()) + "'>\r\n";
    strItem += "    <description>\r\n";
	strItem += gcnew String(m_strDescribe.c_str());
    strItem += "\r\n    </description>\r\n";
    strItem += "    <params>\r\n";
	std::map<std::string,StrategyPara>::iterator itParam;
	for(itParam = m_mapStrategyPara.begin(); itParam != m_mapStrategyPara.end(); itParam++) {
        strItem += "        <param type='" 
				+ gcnew String(StrategyType2String(itParam->second.eType)) 
				+ "' name='" + gcnew String(itParam->second.szName)
				+ "' direction='" + gcnew String(DirectionType2String(itParam->second.eDirection))
				+ "' appearmode='" + gcnew String(DisplayType2String(itParam->second.eDisplayType))
				+ "' readonly='" + gcnew String(Bool2String(itParam->second.bReadOnly))
				+ "' title='" + gcnew String(itParam->second.szTitle)
				+ "' default='" + gcnew String(itParam->second.szDefault)
                + "'>\r\n";
		map<string,list<StrategyPrarSubItem>>::iterator itSub = m_mapPrarSubItem.find(itParam->first);
		if(itSub != m_mapPrarSubItem.end()) {
			list<StrategyPrarSubItem>::iterator itlist;
			for(itlist = itSub->second.begin(); itlist != itSub->second.end(); itlist++) {
				strItem += "            <subitem name='" 
						+ gcnew String((*itlist).szName)
						+ "' value='" + gcnew String((*itlist).szValue)
						+ "'></subitem>\r\n";
			}
		}
        strItem += "        </param>\r\n";
    }
    strItem += "    </params>\r\n";
    strItem += "    <indicators>\r\n";
	std::map<std::string,StrategyIndicator>::iterator itIndicator;
	for(itIndicator = m_mapStrategyIndicator.begin(); 
			itIndicator != m_mapStrategyIndicator.end(); itIndicator++) {
		char csInstrumentIndex[128] = {0};
		sprintf_s(csInstrumentIndex, sizeof(csInstrumentIndex)-1, "%u", itIndicator->second.nInstrument);
		strItem += "        <indicator type='" 
				+ gcnew String(itIndicator->second.szName) 
				+ "' name='" + gcnew String(itIndicator->second.szTitle)
				+ "' instrumentindex='" + gcnew String(csInstrumentIndex)
				+ "' barcycle='" + "-1"
				+ "' cycleunit='" + "-1"
				+ "' paramassign='" + gcnew String(itIndicator->second.szPara)
				+ "' outlog='" + gcnew String(Bool2String(itIndicator->second.bOutput))
				+ "'>" + "</indicator>\r\n";
    }
    strItem += "    </indicators>\r\n";
    strItem += "</strategy>\r\n";
    // 返回参数列表
    strRetXml = Tools::String2string(strItem);

    return true;
}

bool CStrategyDataEx::MakeCodeToFile(String^ strTemplateFile, String^ strPathFile)
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

bool CStrategyDataEx::MakeXmlToFile(String^ strPathFile)
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

bool CStrategyDataEx::ImportFromXml(std::string& strXml)
{
	String^ valtype = nullptr;
	String^ valtext = nullptr;
	String^ valdefault = nullptr;
    XmlDocument^ doc = gcnew XmlDocument();
    doc->LoadXml(gcnew String(strXml.c_str()));

    XmlNode^ nodeRoot = doc->SelectSingleNode("strategy");		//获取indicator节点 
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("id");
	m_strategyName = Tools::String2string(valtext).c_str();
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("name");
	m_strategyNickName = Tools::String2string(valtext).c_str();
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("instrumentcount");
	m_nInstrumentCount = atoi(Tools::String2string(valtext).c_str());
    valtext = ((XmlElement^)nodeRoot)->GetAttribute("version");
	m_Version = Tools::String2string(valtext).c_str();

    XmlNode^ nodeDescription = nodeRoot->SelectSingleNode("description");		//获取indicator节点 
	valtext = ((XmlElement^)nodeDescription)->InnerText;
	m_strDescribe = Tools::String2string(valtext).c_str();

	std::string strParamName;
    XmlNodeList^ nodeList = nodeRoot->SelectSingleNode("params")->ChildNodes;		//获取Params节点的所有子节点 

	m_mapStrategyPara.clear();
	m_mapPrarSubItem.clear();
    for each(XmlNode^ xn in nodeList) {				//遍历所有子节点 
        XmlElement^ xe=(XmlElement^)xn;				//将子节点类型转换为XmlElement类型
		StrategyPara param = {0};
        valtype = xe->GetAttribute("type");
		param.eType = StrategyType2Enum(Tools::String2string(valtype));
        valtext = xe->GetAttribute("name");
		strParamName = Tools::String2string(valtext).c_str();
		strncpy(param.szName, strParamName.c_str(), sizeof(param.szName)-1); 
        valtext = xe->GetAttribute("direction");
		param.eDirection = StrategyDirection2Enum(Tools::String2string(valtext));
        valtext = xe->GetAttribute("appearmode");
		param.eDisplayType = DisplayType2Enum(Tools::String2string(valtext));
        valtext = xe->GetAttribute("readonly");
		param.bReadOnly = valtext=="true";
        valtext = xe->GetAttribute("title");
		strncpy(param.szTitle, Tools::String2string(valtext).c_str(), sizeof(param.szTitle)-1);
        valdefault = xe->GetAttribute("default");
		strncpy(param.szDefault, Tools::String2string(valdefault).c_str(), sizeof(param.szDefault)-1);

		//m_mapStrategyPara[strParamName] = param;
		m_mapStrategyPara[param.szTitle] = param;

		XmlNodeList^ nodeSubItemList = xn->ChildNodes;		//获取Params节点的所有子节点 
		std::list<StrategyPrarSubItem> listSub;
		for each(XmlNode^ xnsub in nodeSubItemList) {				//遍历所有子节点 
			XmlElement^ xesub=(XmlElement^)xnsub;				//将子节点类型转换为XmlElement类型 
			StrategyPrarSubItem sub = {0};
			valtext = xesub->GetAttribute("name");
			strncpy(sub.szName, Tools::String2string(valtext).c_str(), sizeof(sub.szName)-1);
			valdefault = xesub->GetAttribute("value");
			strncpy(sub.szValue, Tools::String2string(valdefault).c_str(), sizeof(sub.szValue)-1);

			listSub.push_back(sub);
		}
		m_mapPrarSubItem[strParamName] = listSub;
    }

    nodeList = nodeRoot->SelectSingleNode("indicators")->ChildNodes;		//获取indicators节点的所有子节点 
	m_mapStrategyIndicator.clear();
    for each(XmlNode^ xn in nodeList) {				//遍历所有子节点 
        XmlElement^ xe=(XmlElement^)xn;				//将子节点类型转换为XmlElement类型 
		StrategyIndicator indicator = {0};
        valtype = xe->GetAttribute("type");
		strncpy(indicator.szName, Tools::String2string(valtype).c_str(), sizeof(indicator.szName)-1);
        valtext = xe->GetAttribute("name");
 		strncpy(indicator.szTitle, Tools::String2string(valtext).c_str(), sizeof(indicator.szTitle)-1);
        valtext = xe->GetAttribute("instrumentindex");
		indicator.nInstrument = atoi(Tools::String2string(valtext).c_str());
        valtext = xe->GetAttribute("barcycle");
        valtext = xe->GetAttribute("cycleunit");
        valtext = xe->GetAttribute("paramassign");
		strncpy(indicator.szPara, Tools::String2string(valtext).c_str(), sizeof(indicator.szPara)-1);
        valtext = xe->GetAttribute("outlog");
		indicator.bOutput = valtype=="true";

		m_mapIndicator[m_strategyName] = indicator.szName;
		m_mapStrategyIndicator[indicator.szName] = indicator;
    }

	return true;
}

bool CStrategyDataEx::ImportFromXmlFile(std::string& strFile)
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

