// stdafx.cpp : source file that includes just the standard includes
// MonitorSvr.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
void split(const string& s, char c, vector<string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);
	if(j == -1 && s.length() != 0)
	{
		v.push_back(s);
		return;
	}
	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}

}
void Load(std::string& UDPSvrIP, unsigned short& UDPSvrPort, std::string& strTime)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath)-1 );

	string strPathFileName( szLocalPath );
	int nPos = strPathFileName.rfind( '\\' );
	if ( nPos == -1) return;
	strPathFileName = strPathFileName.substr( 0, nPos + 1 ) + string("MonitorSvrFront.xml");
	string strValue;	

	TiXmlDocument* pDocument=new TiXmlDocument(strPathFileName.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			const char* pRootElement = RootElement->Value();
			if(strcmp(pRootElement, "root") ==0)
			{
				TiXmlNode* pNode=RootElement->FirstChild("Server");
				if(pNode)
				{
					const char* pElement = pNode->Value();
					if(strcmp(pElement, "Server") ==0)
					{
						TiXmlElement* pChild=pNode->FirstChildElement("IP");
						if(pChild)
						{
							std::string str = pChild->GetText();
							UDPSvrIP = str;							
						}
						TiXmlElement* pChild2=pNode->FirstChildElement("Port");
						if(pChild)
						{
							std::string str = pChild2->GetText();
							UDPSvrPort = atoi(str.c_str());							
						}
					}
				}
				TiXmlNode* pNode2=RootElement->FirstChild("TimeSpan");
				if(pNode2)
				{
					TiXmlElement* pChild=pNode2->ToElement();
					if(pChild)
					{
						strTime= pChild->GetText();							
					}					
				}		
			}			
		}
	}
}
