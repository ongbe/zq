#include "stdafx.h"
#include "parseupdatexml.h"
#include "MyXML.h"
#include "Tools_Ansi.h"


CParseUpdateXml::CParseUpdateXml(const string& strUpdateXmlFileName)
:   m_bvalid(false)
{
    if(strUpdateXmlFileName.empty()) return;
    
    Parse(strUpdateXmlFileName);
}
CParseUpdateXml::~CParseUpdateXml()
{
}

void CParseUpdateXml::Parse(const string& strUpdateXmlFileName)
{
    CMyXML xml(strUpdateXmlFileName.c_str(),true,false);
    if(!xml.m_bvalid) return;

    if(!xml.GetNodeContent(m_version,"update","version")) return;
    if(!xml.GetNodeContent(m_version_min,"update","version_min")) return;
    if(!xml.GetNodeContent(m_version_svrpath,"update","version_svrpath")) return;
    if(!xml.GetNodeContent(m_note,"update","note")) return;
    
    vector<string> replacefiles;
    if(!xml.GetSubContents(replacefiles,"update","ReplaceFiles")) return;
    for(int i=0;i<(int)replacefiles.size();i++)
    {
        if(m_version_svrpath.empty())
            m_replacefiles[replacefiles[i]]=false;
        else 
            m_replacefiles[m_version_svrpath+string("\\")+replacefiles[i]]=false;
    }

    m_bvalid=true;
}

//比较两个版本串。如1.5.7.1或1,5,7,1。返回1/0,-1
int CParseUpdateXml::CompareVersion(const string& ver1,const string& ver2)
{
	int i1=0,i2=0,i3=0,i4=0;
	int j1=0,j2=0,j3=0,j4=0;
    CTools_Ansi::GetFourFieldInt(ver1.c_str(),ver1.size(),0,&i1,&i2,&i3,&i4,'.',',');
    CTools_Ansi::GetFourFieldInt(ver2.c_str(),ver2.size(),0,&j1,&j2,&j3,&j4,'.',',');

    if(i1>j1||i1==j1&&i2>j2||i1==j1&&i2==j2&&i3>j3||i1==j1&&i2==j2&&i3==j3&&i4>j4) return 1;
    if(i1==j1&&i2==j2&&i3==j3&&i4==j4) return 0;
    return -1;
}

//将版本串格式化为1.5.7.1
string CParseUpdateXml::FormatVersion(const string& ver)
{
	int i1=0,i2=0,i3=0,i4=0;
    CTools_Ansi::GetFourFieldInt(ver.c_str(),ver.size(),0,&i1,&i2,&i3,&i4,'.',',');
    char buf[256]={0};
    sprintf_s(buf,sizeof(buf)-1,"%d.%d.%d.%d",i1,i2,i3,i4);
    return string(buf);
}
