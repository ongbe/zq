#pragma once

#include <string>
#include <cliext/map>
#include <cliext/vector>
using namespace System;

#pragma warning(disable : 4677)

public ref class Tools
{
public:
	static std::string String2string(String^ mstr)
	{
		const char* pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(mstr)).ToPointer();
		std::string rltstr(pchars);
		Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));
		return rltstr;
	}
	static String^ string2String(const char* pchars)
	{
		String^ tmp= gcnew String(pchars);
		return tmp;
	}

    static bool TransferSLT2cli(cliext::vector<String^> clivecSrc, std::vector<std::string>& vecDest)
    {
        if(!clivecSrc.empty())
        {
            for (cliext::vector<String^>::iterator it=clivecSrc.begin();it!=clivecSrc.end();it++)
            {
                String^ szInst = *it;
                vecDest.push_back(Tools::String2string(szInst));
            }

            return true;
        }

        return false;
    }
    static bool TransferSLT2cli(std::map<std::string,std::vector<std::string>> mapSrc,cliext::map<String^,cliext::vector<String^>^>% climapDest)
    {
        if(!mapSrc.empty())
        {
            for (std::map<std::string,std::vector<std::string>>::iterator it=mapSrc.begin();it!=mapSrc.end();it++)
            {
                String^ szExchange = Tools::string2String(it->first.c_str());
                cliext::vector<String^>^ clivecInstrument = gcnew cliext::vector<String^>();
                for (std::vector<std::string>::iterator it2=it->second.begin();it2!=it->second.end();it2++)
                    clivecInstrument->push_back(Tools::string2String((*it2).c_str()));

                climapDest.insert(cliext::make_pair(szExchange, clivecInstrument));
            }

            return true;
        }

        return false;
    }
    static bool TransferSLT2cli(cliext::map<String^,cliext::vector<String^>^> mapSrc,std::map<std::string,std::vector<std::string>>& climapDest)
    {
        if(!mapSrc.empty())
        {
            for (cliext::map<String^,cliext::vector<String^>^>::iterator it=mapSrc.begin();it!=mapSrc.end();it++)
            {
                std::string szExchange = String2string(it->first);
                std::vector<std::string> clivecInstrument;
                for (cliext::vector<String^>::iterator it2=it->second->begin();it2!=it->second->end();it2++)
                    clivecInstrument.push_back(Tools::String2string(*it2));

                climapDest.insert(std::make_pair(szExchange, clivecInstrument));
            }

            return true;
        }

        return false;
    }
};
