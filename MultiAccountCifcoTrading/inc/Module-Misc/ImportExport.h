#pragma once
#include "constants.h"
class CImportExport
{
public:
	CImportExport(int sysbaseid,int userbaseid);
	virtual ~CImportExport(void);
	//读取外部布局生成菜单项
    wxMenu* CreateLayoutMenu(wxMenu* layoutSubMenu=NULL);
	//配置文件的保存
    void DoConfigSave(wxString caption);
	//配置文件的载入,返回是否需要设置窗口大小为1024*768
    bool DoChangeStyle(int cmdid);
	//删除一个用户配置
	void DoDeleteConfig(wxString caption);
	//判断当前配置是否和系统配置文件一致
	//bool CfgXmlIsChanged();
public:
	//获取可执行程序路径
	wxString GetAppPath(){return m_strAppPath.c_str();}
    //获取当前配置文件全路径
	wxString GetCfgFileName(){return m_strAppPath+USERCFG_FILENAME;}
    //获取当前布局文件全路径
	wxString GetLayoutFileName(){return m_strAppPath+LAYOUT_FILENAME;}
    //获取系统布局路径
	wxString GetSystemLayoutPath(){return m_strAppPath+"layout\\";}
    //获取系统配置文件全路径
	wxString GetSystemCfgFileName(){return m_strAppPath+"syscfg.xml";}
    //获取系统布局文件全路径
	wxString GetSystemLayoutFileName(wxString caption){return GetSystemLayoutPath()+caption+".layout";}
    //获取用户布局路径
	wxString GetUserLayoutPath(){return m_strAppPath+"userlayout\\";}
    //获取用户配置文件全路径
	wxString GetUserCfgFileName(wxString caption){return GetUserLayoutPath()+caption+".xml";}
    //获取用户布局文件全路径
	wxString GetUserLayoutFileName(wxString caption){return GetUserLayoutPath()+caption+".layout";}
	//获得用户布局文件列表
	void GetLayoutArray(wxArrayString& arr,bool renew=true);
	//获取系统布局文件列表
	void LoadSystemLayoutArray(wxArrayString& arr);
private:
	//系统布局菜单项最小ID
	int m_SystemBaseId;
	//用户布局菜单项最小ID
	int m_UserBaseId;
	std::string m_strAppPath;
    //
    wxArrayString m_UserLayoutCaption;

};
