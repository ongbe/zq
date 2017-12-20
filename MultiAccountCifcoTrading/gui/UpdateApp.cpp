#include "StdAfx.h"
#include "UpdateApp.h"
#include "MyProcess.h"
#include "VersionDefine.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#define UPDATE_FILENAME     "updatefasttrader.exe"
#define UPDATE_FILENAME_SRC "Update\\updatefasttrader.exe"
#define UPADTE_XML          "Update\\userVersion.xml"
long UpdateApp::m_UpdaterPid=0;
bool UpdateApp::DoUpdate()
{
    //代替升级程序本身
    ReplaceUpdateApp();
	std::string strXmlName =UPADTE_XML;
	GlobalFunc::GetPath( strXmlName );
    if(wxFile::Exists(strXmlName.c_str()))//文件存在
    {
        bool bNeedUpdate=CheckVersion(strXmlName);  
        wxRemoveFile(strXmlName.c_str()); //删除文件
        if(bNeedUpdate)
        {//需要升级
            StartUpdateApp(false);
            return true;//应用程序退出
        }
        else
        {//不需要升级
            //启动升级程序负责下载生成 UPADTE_XML
            StartUpdateApp(true);
        }
    }
    else
    {
        //启动升级程序负责下载生成 UPADTE_XML
        StartUpdateApp(true);
    }
    return false;
}
//检查  UPADTE_XML 判断是否需要升级
bool UpdateApp::CheckVersion(const wxString&  strXmlName)
{
    TiXmlDocument myDocument(strXmlName.c_str());
    wxString PromptText,remoteVersion,strVersionInfo; 
    if(myDocument.LoadFile())
    {
		TiXmlElement* root=myDocument.RootElement();
		if(root) 
        {
		    wxString strRoot=root->Value()?root->Value():wxEmptyString;
		    if(strRoot=="update")
		    {
                TiXmlElement* pVersion = root->FirstChildElement("version");
                if(pVersion)
                    remoteVersion = pVersion->GetText()?pVersion->GetText():wxEmptyString;
                TiXmlElement* pVersionInfo = root->FirstChildElement("versionInfo");
                if(pVersionInfo)
                    strVersionInfo = pVersionInfo->GetText()?pVersionInfo->GetText():wxEmptyString;
                if(CheckNeedUpdate(remoteVersion,VER_STRING))
                {
					PromptText=LOADSTRING(UPDATEAPP_NEW_VER_PMT);
                    PromptText+=remoteVersion;
					PromptText+=LOADSTRING(UPDATEAPP_LCL_VER_PMT);
                    PromptText+=VER_STRING;
                    PromptText+=LOADSTRING(UPDATEAPP_REPLACE_PMT);
                    PromptText+=strVersionInfo;
                    if(wxMessageBox(PromptText,LOADSTRING(USERLOGINDLG_INFORMATION),wxYES_NO)==wxYES)
                    {
                        return true;
                    }    
                }
            }
        }
    }
    return false;
}
bool UpdateApp::CheckNeedUpdate(const wxString& RemoteVer,const wxString& LocalVer)
{
    vector<string> r,l;
    vector<int> dr,dl;
    if(SplitString(RemoteVer.c_str(),r)==4&&SplitString(LocalVer.c_str(),l))
    {
        for(int i=0;i<4;++i)
        {
            dr.push_back(atoi(r[i].c_str()));
            dl.push_back(atoi(l[i].c_str()));
        }
        if(dr[0]>dl[0]) 
            return true;
        else if(dr[0]<dl[0])
            return false;
        else
        {
            if(dr[1]>dl[1]) 
                return true;
            else if(dr[1]<dl[1])
                return false;
            else
            {
                if(dr[2]>dl[2]) 
                    return true;
                else if(dr[2]<dl[2])
                    return false;
                else
                {
                    if(dr[3]>dl[3]) 
                        return true;
                    else if(dr[3]<dl[3])
                        return false;
                    else
                    {
                        return false;
                    }
                }
            }
        }


    }
    return false;
}
void UpdateApp::StartUpdateApp(bool bOnlyDownload)
{
    string strName =UPDATE_FILENAME;
	GlobalFunc::GetPath(strName);
    char exepath[_MAX_PATH];
	GetModuleFileName(NULL,exepath,_MAX_PATH);
	
    if(!wxFile::Exists(strName.c_str()))
        return ;
    wxString cmd;
    if(bOnlyDownload)
        cmd.Printf("\"%s\"",strName.c_str());
    else
        cmd.Printf("\"%s\" \"%s\"",strName.c_str(),exepath);
    cProcess *process = new cProcess(NULL,m_UpdaterPid);
    process->Execute(cmd, wxEXEC_ASYNC,process); 

}
void UpdateApp::ReplaceUpdateApp()
{
	std::string strSrcName =UPDATE_FILENAME_SRC,strName =UPDATE_FILENAME;
	GlobalFunc::GetPath(strSrcName);
	GlobalFunc::GetPath(strName);
    if(wxFile::Exists(strSrcName.c_str()))//文件存在
    {
        if(wxCopyFile(strSrcName.c_str(),strName.c_str()))
        {
            wxRemoveFile(strSrcName.c_str()); 
        }
    }
}
void UpdateApp::KillUpdateApp()
{
	if(m_UpdaterPid)
		::wxKill(m_UpdaterPid,wxSIGKILL);
}