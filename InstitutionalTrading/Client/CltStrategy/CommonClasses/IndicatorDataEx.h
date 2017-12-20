#pragma once

#include "IndicatorData.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text;
using namespace System::IO;

class CIndicatorDataEx : public CIndicatorData
{
public:
    CIndicatorDataEx();
	virtual ~CIndicatorDataEx();

	void SetZipFile(std::string& strFile) {
		m_strZipFile = strFile;
	};
	std::string& GetZipFile() {
		return m_strZipFile;
	};
	void SetDLLFile(std::string& strFile) {
		m_strDLLFile = strFile;
	};
	std::string& GetDLLFile() {
		return m_strDLLFile;
	};

	bool MakeCode(const std::string& strTemplateFile, std::string& strRetCode);
	bool MakeXml(std::string& strRetXml);
	bool MakeCodeToFile(String^ strTemplateFile, String^ strPathFile);
	bool MakeXmlToFile(String^ strPathFile);

	bool ImportFromXml(std::string& strXml);
	bool ImportFromXmlFile(std::string& strFile);

protected:
	std::string		m_strZipFile;
	std::string		m_strDLLFile;

};
