#include <windows.h>
#include <TlHelp32.h>
#include "IsProcessExist.h"
#include "Tools_Ansi.h"

CIsProcessExist::CIsProcessExist(const string& ProcessFileName)
:   m_bExist(false)
{
	HANDLE          hProcessSnap   =   NULL;   
	PROCESSENTRY32  pe32   =   {0};  

    hProcessSnap  =  CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,   0);   
	if(hProcessSnap  ==  (HANDLE)-1)   
		return;   

	pe32.dwSize   =   sizeof(PROCESSENTRY32);   

    //取出纯文件名
    char filename[256]={0};
    if(!CTools_Ansi::GetPureFileName(ProcessFileName.c_str(),filename,sizeof(filename)-1))
        return;

	// 列举所有进程名称   
	if(Process32First(hProcessSnap,   &pe32))   
	{   
		do   
		{   
			if(string(pe32.szExeFile) == string(filename))
			{
                m_bExist=true;
                break;
			}
		}   
		while(Process32Next(hProcessSnap,&pe32)); 
	}   
	CloseHandle(hProcessSnap);
}
