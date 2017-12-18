#include "StdAfx.h"
#include "ImportExport.h"
#include "..\\inc\\Module-Misc2\\GlobalFunc.h"
#include "wx/dir.h"
#include <wx/file.h> 
#include "wx/filename.h"
#include "constants.h"
#include "..\\inc\\Module-Misc2\\globalDefines.h"
#include "cfgMgr/CfgMgr.h"
CImportExport::CImportExport(int sysbaseid,int userbaseid):
m_SystemBaseId(sysbaseid),
m_UserBaseId(userbaseid)
{
	GlobalFunc::GetPath(m_strAppPath);
}

CImportExport::~CImportExport(void)
{
}
wxMenu* CImportExport::CreateLayoutMenu(wxMenu* layoutSubMenu)
{
	if(layoutSubMenu==NULL)
		layoutSubMenu = new wxMenu;       
	while(layoutSubMenu->GetMenuItemCount())
			layoutSubMenu->Remove(layoutSubMenu->FindItemByPosition(0));    
	wxArrayString sys_layouts,user_layouts;
	size_t index=0;
	LoadSystemLayoutArray(sys_layouts);
	GetLayoutArray(user_layouts);
	for(index=0;index<sys_layouts.GetCount();index++)
	{
		layoutSubMenu->AppendCheckItem(m_SystemBaseId+index,sys_layouts[index]);
	}

    if(user_layouts.GetCount()>0)
	    layoutSubMenu->AppendSeparator();
	for(index=0;index<user_layouts.GetCount();index++)
	{
		layoutSubMenu->AppendCheckItem(m_UserBaseId+index,user_layouts[index]);
	}
	return layoutSubMenu;
}
void CImportExport::DoConfigSave(wxString caption)
{
	wxString layoutpath=GetUserLayoutPath();
	if(!wxDirExists(layoutpath)) wxMkdir(layoutpath);

	wxCopyFile(GetLayoutFileName(),GetUserLayoutFileName(caption));

}
void CImportExport::DoDeleteConfig(wxString caption)
{
	wxRemoveFile(GetUserCfgFileName(caption));
	wxRemoveFile(GetUserLayoutFileName(caption));
}
bool CImportExport::DoChangeStyle(int cmdid)
{
	wxArrayString layouts;
	wxString src_layout;
	bool bSizeTo1024=false;
	if(cmdid>=m_SystemBaseId&&cmdid<m_SystemBaseId+50)
	{
		src_layout=wxString::Format("%d",cmdid-m_SystemBaseId+1);
		src_layout=GetSystemLayoutFileName(src_layout);
		if(cmdid-m_SystemBaseId==1&&GetSystemMetrics(SM_CXFULLSCREEN)>=1024)
			bSizeTo1024=true;
	}
	else if(cmdid>=m_UserBaseId&&cmdid<m_UserBaseId+50)
	{
		int index=cmdid-m_UserBaseId;
		GetLayoutArray(layouts);
		if(index<0||index>=(int)layouts.GetCount()) return bSizeTo1024;
		src_layout=GetUserLayoutFileName(layouts[index]);
	}
	else
		return bSizeTo1024;
	wxCopyFile(src_layout,GetLayoutFileName());
	return bSizeTo1024;
}
//bool CImportExport::CfgXmlIsChanged()
//{
//	bool bret=true;
//
//	return bret;
//}
//读取外部布局文件生成菜单项列表
void CImportExport::GetLayoutArray(wxArrayString& arr,bool renew)
{
    if(renew)
    {
        wxString layoutpath=GetUserLayoutPath();
        if(wxDirExists(layoutpath))
        {
            wxDir dir(layoutpath);
            if(dir.IsOpened())
            {
                wxString filename;
                wxString filespec = wxT("*.layout");
                arr.clear();
                bool cont = dir.GetFirst(&filename, filespec , wxDIR_FILES ) ;
                while(cont)
                {
                    if(filename.Length()>strlen(".layout"))
                    {
                        wxFileName fname(layoutpath+filename);
                        if(arr.GetCount()>50) break;
                        arr.Add(fname.GetName());
                    }
                    cont = dir.GetNext(&filename);
                }
            }
        }
        m_UserLayoutCaption=arr;
    }
    else
        arr=m_UserLayoutCaption;
}
void CImportExport::LoadSystemLayoutArray(wxArrayString& arr)
{
	arr.Empty();
	arr.Add(LOADSTRING(LAYOUT_STANDARD));
	arr.Add(LOADSTRING(LAYOUT_CLASSICS));
	arr.Add(LOADSTRING(LAYOUT_SIMPLE));
	arr.Add(LOADSTRING(LAYOUT_QUICK));
}
