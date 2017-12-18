#pragma once

//解析update.xml
//文件内容示例如下：
//<update>
//   <version>1,5,0,7</version>
//   <version_min>1,3,2,84</version_min>
//   <version_svrpath>update_std_01050007</version_svrpath>
//   <ReplaceFiles>
//		<FileName>FastTrader.exe</FileName>
//   	<FileName>layout\1.layout</FileName>
//   </ReplaceFiles>  
//   <note>更新说明</note>
//</update>  

#include "stdafx.h"

#include <string>
#include <map>
using std::string;
using std::map;




class CParseUpdateXml
{
public:
    CParseUpdateXml(const string& strUpdateXmlFileName);
    ~CParseUpdateXml();


    void Parse(const string& strUpdateXmlFileName);

    //下面是update.xml的内容
    string          m_version;                  //服务器版本号，1.5.0.7
    string          m_version_min;              //客户端可升级的最低版本号。低于此版本号的只能重新安装
    string          m_version_svrpath;          //升级文件在服务器上的相对路径
    map<string,bool>m_replacefiles;             //需要更新替换的文件集合。first是filename, second表示是否已经下载
    string          m_note;                     //更新说明
    bool            m_bvalid;                   //是否有效

    //比较两个版本串。如1.5.7.1或1,5,7,1。返回1/0,-1
    static int CompareVersion(const string& ver1,const string& ver2);
    //将版本串格式化为1.5.7.1
    static string FormatVersion(const string& ver);

    //下面两个方法禁用
	CParseUpdateXml(const CParseUpdateXml&);
	CParseUpdateXml& operator= (const CParseUpdateXml&);
};
