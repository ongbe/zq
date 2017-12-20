#if !defined(UPDATE_FASTTRADER_H)
#define UPDATE_FASTTRADER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma once

#include "stdafx.h" 
//#include "checkversion.h"

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread thread
// 本地保存的升级配置文件名
#define UPDATE_CONFIG_FILENAME "UpdateConfig.ini"

// 升级配置文件中的区段名
#define SECTION_UPDATE "UPDATE"
#define SECTION_COMMON "COMMON"

// 升级下载文件状态消息结构
struct DOWNLOAD_INFO_STRU
{
	UINT iFileCount;					// 合计文件数
	UINT iFileSize;						// 合计字节数
};

// 下载单个文件进度消息结构
struct DOWNLOAD_PROGRESS_STRU
{
	UINT iCurrentFileSize;				// 当前正在下载的文件的字节数
	UINT iCurrentFileFinishedSize;		// 当前文件已下载字节数
};
// 上报到界面的升级进度消息
enum
{
	NOTIFY_DOWNLOAD_INFO = 1001,		// 通知要下载的文件状况
	NOTIFY_DOWNLOADED_INFO,				// 通知已下载的文件状况
	NOTIFY_DOWNLOAD_PROGRESS,			// 通知下载单个文件进度
	NOTIFY_DOWNLOADING_FILENAME,		// 通知正在下载的文件名
	NOTIFY_DOWNLOAD_FILE_FAIL,			// 通知下载文件失败
	NOTIFY_VERIFY_FILE_FAIL,			// 通知校验文件失败
	NOTIFY_UPDATE_FILE_FAIL,			// 通知更新文件失败
    NOTIFY_FINISH_UPDATE,				// 通知升级完毕消息
};



/////////////////////////////////////////////////////////////////////////////
// CAutoUpdateApp:
// See AutoUpdate.cpp for the implementation of this class
//

class CUpdateFasttraderApp : public wxApp
{
public:
	virtual bool OnInit();
 //   CUpdateFasttraderApp();
	//~CUpdateFasttraderApp();
  //  wxString m_pszExeName;

	//BOOL DoUpdate();					// 执行升级

	//DECLARE_MESSAGE_MAP()

private:

private:
 //   void CUpdateFasttraderApp::GetPath(std::string &filePath);
 //   size_t GetFileSize(const char* sFilename);
  //  char* GetFilename( char* sFilePath);
};
DECLARE_APP(CUpdateFasttraderApp)
#endif // 
