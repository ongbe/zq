// InternetGetFile.h: interface for the InternetGetFile namespace.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if !defined(INTERNETGETFILE_H)
#define INTERNETGETFILE_H

namespace Internet
{
    enum
    {
        INTERNET_SUCCESS = 0,
        INTERNET_ERROR_OPENURL,
        INTERNET_ERROR_FILEOPEN,
        INTERNET_ERROR_READFILE,
        INTERNET_ERROR_OPEN
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

    // 下载单个文件进度消息结构
    struct DOWNLOAD_PROGRESS_STRU
    {
        UINT iCurrentFileSize;				// 当前正在下载的文件的字节数
        UINT iCurrentFileFinishedSize;		// 当前文件已下载字节数
    };

    extern BOOL bInternetGetURLUserBreak;	// 中断下载标记，置为真则中断下载文件

    // Get URL form internet with inet API
    int InternetGetURL(const char* sURL, const char* sSaveFilename, const char* sHeader = NULL, HWND hProgressWindow = NULL);
};

#endif // !defined(INTERNETGETFILE_H)