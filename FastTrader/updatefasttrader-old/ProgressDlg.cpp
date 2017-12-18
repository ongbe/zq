#include "ProgressDlg.h"

extern CheckVersion g_check;
BEGIN_EVENT_TABLE(CProgressDlg, wxDialog)
    EVT_BUTTON(ID_WXBUTTON1, CProgressDlg::OnOK)
    EVT_BUTTON(ID_WXBUTTON3, CProgressDlg::OnCancel)
    EVT_COMMAND(wxID_ANY, wxEVT_UPDATE_PROGRESS,OnUpdatProgress)
    EVT_CLOSE( CProgressDlg::OnClose)
    EVT_PAINT(CProgressDlg::OnPaint)	
	EVT_TIMER(1234, OnTimer)
END_EVENT_TABLE()
void CProgressDlg::OnTimer(wxTimerEvent& event)
{
	if(m_nDownLoadCount != m_nDownLoadFinished)
	{
		m_bFail = true;
		wxString strInfo = wxT("升级失败！请自行下载安装程序，手动安装！");
		double dbPercent =0;
		g_check.ProcessMsg(strInfo, dbPercent);
		
	}
	m_timerStopUpdate.Stop();
}

CProgressDlg::CProgressDlg(wxWindow *parent)
: wxDialog(parent, wxID_ANY, wxString(_T("安装-升级程序")),wxDefaultPosition,wxSize(582,389)),
m_hThreadHandle(NULL),m_timerStopUpdate(this,1234)
{
	m_bFail = false;
	m_pFile = NULL;
	GetJPGHandle(m_BackBitmap);	
	SetIcon(wxIcon(_T("IDI_UPDATEFASTTRADER")));	
	WxButtonCancel = new wxButton(this, ID_WXBUTTON3, wxT("取消"), wxPoint(462, 328), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	WxButtonOK = new wxButton(this, ID_WXBUTTON1, wxT("确定"), wxPoint(339, 328), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton1"));
	WxButtonOK->Enable(false);
	WxStaticText2 = new wxStaticText(this, ID_PROGRESS_STATIC, wxT("0%"), wxPoint(196, 286), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxGauge1 = new wxGauge(this, ID_WXGAUGE1, 100, wxPoint(195, 266), wxSize(364, 17), wxGA_HORIZONTAL, wxDefaultValidator, wxT("WxGauge1"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("升级程序正在进行到以下步骤："), wxPoint(196, 9), wxDefaultSize, 0, wxT("WxStaticText1"));

	wxArrayString arrayStringFor_WxListBox1;
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(195, 32), wxSize(362, 227), arrayStringFor_WxListBox1, wxLB_SINGLE);

	m_nDownLoadCount				= 0;
	m_nDownLoadFinished		= 0;


}
CProgressDlg::~CProgressDlg()
{
//	if(m_pCheckVersion)
//	{
//		delete m_pCheckVersion;
//		m_pCheckVersion = NULL;
//	}
}
void CProgressDlg::OnOK(wxCommandEvent& event)
{
    if(m_strMainProc.IsEmpty())
        return;
    wxString strExe;
    strExe = m_strMainProc;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset(&si,0,sizeof(si));
    si.cb=sizeof(si);
    si.wShowWindow=SW_SHOW;
    si.dwFlags=STARTF_USESHOWWINDOW;
    CreateProcess(strExe,NULL,NULL,FALSE,NULL,NULL,NULL,NULL,&si,&pi);
    Close(true);
}
void CProgressDlg::OnCancel(wxCommandEvent& event)
{
    g_check.SetCancelUpdate(true);	

    Close(true);

}
DWORD WINAPI UpdateThread(LPVOID lpvParam)
{
	CProgressDlg *pDlg =(CProgressDlg *)lpvParam;
	if(pDlg == NULL)
		return 0;
/*
	if (g_check.NeedUpdate())
	{
		if(g_check.DownLoadAllFile())
		{
			if(!g_check.DoUpdate())
			{
				wxString strInfo = wxT("升级失败！请自行下载安装程序，手动安装！");
				double dbPercent =0;
				g_check.ProcessMsg(strInfo, dbPercent);
			}
		}
		else
		{
			wxString strInfo = wxT("升级失败！请自行下载安装程序，手动安装！");
			double dbPercent =0;
			g_check.ProcessMsg(strInfo, dbPercent);

		}		
	}*/
	if(!g_check.DoUpdate())
	{
		wxString strInfo = wxT("升级失败！请自行下载安装程序，手动安装！");
		double dbPercent =0;
		g_check.ProcessMsg(strInfo, dbPercent);
	}

	return 0;
}
bool CProgressDlg::CreateUpdateThread()
{
	m_hThreadHandle = CreateThread(NULL, 0, UpdateThread, this, 0, 0); 
	return true;
}
void CProgressDlg::OnUpdatProgress(wxCommandEvent& event)
{
	string str =  event.GetString();
	double *pDB = reinterpret_cast<double *>(event.GetClientData());

	wxString strListBox = str;
	int nItem = WxListBox1->GetCount();
	WxListBox1->Insert(strListBox, nItem);
	int nItem2 = WxListBox1->GetCount();	
	WxListBox1->Select(nItem2 -1);

	int nPercent = *pDB *100;
	WxGauge1->SetValue(nPercent);

	wxString strLable;
	strLable.Printf(wxT("%d%%"), nPercent);

	if(nPercent == 100)
	{
		wxString str= strLable;
		strLable.Printf(wxT("%s 升级完成！点击确定将启动中国国际期货交易终端。"), str);
		WxButtonOK->Enable(true);		
	}
	WxStaticText2->SetLabel(strLable);


}
void CProgressDlg::OnClose(wxCloseEvent& evt)
{
	g_check.SetCancelUpdate(true);	
	CloseHandle(m_hThreadHandle);
    Destroy();
}
void CProgressDlg::OnPaint(wxPaintEvent& event)
{
	if(m_BackBitmap.IsOk())
	{
		wxPaintDC dc(this);
		dc.DrawBitmap(m_BackBitmap,0,0);	
	}
	else
		event.Skip();
}
void CProgressDlg::GetJPGHandle(wxBitmap& bmp)
{
	HRSRC hRsrc = FindResource(NULL,"IDR_JPG1", "JPG");
	if (NULL == hRsrc)
		return ;
	DWORD lpSize=0;
	DWORD dwSize = SizeofResource(NULL, hRsrc); 
	if (0 == dwSize)
		return ;
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc); 
	if (NULL == hGlobal)
		return ;
	void* pBuffer = LockResource(hGlobal); 
	if (NULL == pBuffer)
		return ;
	
	lpSize=dwSize;
	if(pBuffer)
	{
		wxImage::AddHandler(new wxJPEGHandler); 
		wxImage::AddHandler(new wxBMPHandler);
		wxImage image;
		wxMemoryInputStream stream(pBuffer, lpSize);
		image.LoadFile(stream, wxBITMAP_TYPE_JPEG);
		bmp= wxBitmap(image);
	}


}

WXLRESULT CProgressDlg::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{
	
	if(message==RECV_DATA_MSG)
	{	
		//发送回传 UserData1 == 0 为成功
		//         userdata2 = 当前第几个包
		//         userdata3 = 总包数
		char *p = (char*)lParam;
		RecvData recvData;					
		memset(&recvData, 0, sizeof(recvData));
		memcpy(&recvData, p, sizeof(recvData));

		switch(recvData.head.cmdid) 
		{			
		case CMDID_SvrGetRsp:
			{
				if(recvData.head.userdata1 != 0)
				{//服务端读文件失败，则升级失败					
					UpdateFail();
					LOG_INFO("服务端读文件失败\n");
					return false;
				}

				if(m_bFail)//如果失败了，就不再继续升级了
					return wxDialog::MSWWindowProc(message,wParam,lParam); 

				m_timerStopUpdate.Stop();
				m_timerStopUpdate.Start(1000*120);//两分钟没有收到包则升级失败了
		
				if ( recvData.head.userdata1 == 0 )
				{					
					std::map<int, std::string > mapSeq2File;
					g_check.GetmapSeq2File(mapSeq2File);
					int nCount  = mapSeq2File.size();
					std::map<int, std::string >::iterator it = mapSeq2File.find(recvData.head.seq);
					if(it != mapSeq2File.end())
					{
						std::string strFile = it->second;	
					//	LOG_INFO("接收文件%s; seq:%d; subseq:%d",strFile.c_str(), recvData.head.seq, recvData.head.userdata2);

						std::string strPath;
						GetPath(strPath);
						wxString strUpdatePath=strPath+"Update";
						wxString ConfigFilename=strPath+"Update\\" + strFile;
						CreateDir(ConfigFilename.c_str()); 
						if(strcmp(strFile.c_str(), g_check.m_sPara.strMainFile.c_str()) == 0)
							ConfigFilename=strPath+"Update\\userVersion.xml";
						
						if( recvData.head.userdata2 == 1)
						{//如果是第一个包则删除原有的文件，以免追加有问题
							::DeleteFile(ConfigFilename.c_str());
							m_nDownLoadCount++;
							wxString strInfo;
							strInfo.Printf(wxT("下载:%s..."), strFile.c_str() );
							if(nCount ==1)
								ProcessMsg(strInfo, 0);
							else
								ProcessMsg(strInfo, 0.8*m_nDownLoadCount/nCount);	

							m_nOneFilePackCount = 1;
							m_pFile = fopen( ConfigFilename.c_str(), "ab+" );
						}
						else
							m_nOneFilePackCount ++;
					
					//	FILE* pFile = fopen( ConfigFilename.c_str(), "a+" );
					//	FILE* pFile = fopen( ConfigFilename.c_str(), "wb+" );
					//	FILE* pFile = fopen( ConfigFilename.c_str(), "a+b" );
					//	FILE* pFile = fopen( ConfigFilename.c_str(), "ab+" );
						if(m_pFile == NULL)
						{//写文件是失败，则升级失败
							UpdateFail();
							LOG_INFO("打开文件失败%s\n", ConfigFilename.c_str());
							return false;
						}
						fseek(m_pFile, 0, SEEK_END);
						//fseek(pFile, 0, SEEK_SET);
						long nWrite = fwrite(recvData.pData, recvData.head.len, 1, m_pFile);
						if(nWrite < 1  && recvData.head.len != 0)
						{//写文件是失败，则升级失败
							UpdateFail();
							LOG_INFO("写文件失败%s\n", ConfigFilename.c_str());
							return false;
						}
						//fclose(m_pFile);
						
						if(recvData.head.userdata2 == recvData.head.userdata3)
						{		
							fclose(m_pFile);
							if(m_nOneFilePackCount != recvData.head.userdata3)
							{//该文件有包丢失
								UpdateFail();
								LOG_INFO("文件收包失败%s\n", ConfigFilename.c_str());
								return wxDialog::MSWWindowProc(message,wParam,lParam); 
							}
							m_nDownLoadFinished++;
							
							wxString strInfo;
							strInfo.Printf(wxT("下载:%s完成"), strFile.c_str() );
							if(nCount ==1)
								ProcessMsg(strInfo, 0);
							else
								ProcessMsg(strInfo, 0.8*m_nDownLoadFinished/nCount);
							
							if(strcmp(strFile.c_str(), g_check.m_sPara.strMainFile.c_str()) == 0)
							{
								if(g_check.m_bMainFileOut)//只下载服务端升级配置文件
								{
									Close(true);
								}
								else
								{								
									if(g_check.NeedUpdate())
										g_check.DownLoadAllFile();									
								}
							}
							else
							{
								if(m_nDownLoadFinished == nCount)
								{//全部下载完成
									CreateUpdateThread();
								}
							}							
						}
					}	
					else
						LOG_INFO("error接收文件; seq:%d; subseq:%d", recvData.head.seq, recvData.head.subseq);
				}		

				break;
			}	
		default:break;
		}
	}
	else if(message == CONNECT_DATA_MSG)
	{
		g_check.QueryUpdateMainFile();
	}

	return wxDialog::MSWWindowProc(message,wParam,lParam); 
}
void CProgressDlg::UpdateFail()
{
	m_bFail = true;
	wxString strInfo = wxT("升级失败！请自行下载安装程序，手动安装！");
	double dbPercent =0;
	g_check.ProcessMsg(strInfo, dbPercent);
}
bool CProgressDlg::DirExist(const char *pszDirName) 
{ 
	WIN32_FIND_DATA fileinfo; 
	char _szDir[_MAX_PATH]; 
	strcpy(_szDir, pszDirName); 
	int nLen = strlen(_szDir); 
	if( (_szDir[nLen-1] == '\\') || (_szDir[nLen-1] == '/') ) 
	{ 
		_szDir[nLen-1] = '\0'; 
	} 
	HANDLE hFind = ::FindFirstFile(_szDir, &fileinfo); 

	if (hFind == INVALID_HANDLE_VALUE) 
	{ 
		return false; 
	} 
	if( fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ) 
	{ 
		::FindClose(hFind); 
		return true; 
	} 
	::FindClose(hFind); 
	return false; 
}
bool CProgressDlg::CreateDir(const char *pszDir) 
{ 
	char pszDirName[256];
	std::string strFullPath = pszDir;
	str_replace(strFullPath, "/", "\\");
	int nPos = strFullPath.rfind("\\");
	if(nPos != -1)
	{
		std::string str = strFullPath.substr(0, nPos);
		strcpy(pszDirName, str.c_str());
	}
	
	bool bRet = false; 
	char _szDir[_MAX_PATH]; 
	char _szTmp[_MAX_DIR]; 
	int nLen = 0; 
	int idx ; 
	if( (DirExist(pszDirName)) == true ) 
		return true; 
	strcpy(_szDir, pszDirName); 
	nLen = strlen(_szDir); 
	if( _szDir[nLen-1] == '\\' || _szDir[nLen-1] == '/' ) 
	{ 
		_szDir[nLen-1] = '\0'; 
	} 
	nLen = strlen(_szDir); 
	memset(_szTmp, 0, _MAX_DIR); 
	char _str[2]; 
	for(idx = 0; idx < nLen; idx++) 
	{ 
		if(_szDir[idx] != '\\' ) 
		{ 
			_str[0] = _szDir[idx]; 
			_str[1] = 0; 
			strcat(_szTmp, _str); 
		} 
		else 
		{ 
			bRet = !!::CreateDirectory(_szTmp, NULL); 
			if( bRet ) 
			{ 
				::SetFileAttributes(_szTmp, FILE_ATTRIBUTE_NORMAL); 
			} 
			_str[0] = _szDir[idx]; 
			_str[1] = 0; 
			strcat(_szTmp, _str); 
		} 
		if( idx == nLen-1 ) 
		{ 
			bRet = !!::CreateDirectory(_szTmp, NULL); //没有WARING 加两个感叹号
			if( bRet ) 
			{ 
				::SetFileAttributes(_szTmp, FILE_ATTRIBUTE_NORMAL); 
			} 
		} 
	} 
	if( DirExist(_szTmp) ) 
		return true; 
	return false; 
} 
void CProgressDlg::ProcessMsg(wxString str, double dbPrecnt)
{
	wxCommandEvent myEvent(wxEVT_UPDATE_PROGRESS);
	myEvent.SetString(str);
	myEvent.SetClientData(&dbPrecnt);
	ProcessEvent(myEvent);
}