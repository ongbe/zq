#include "ProgressDlg.h"

BEGIN_EVENT_TABLE(CProgressDlg, wxDialog)
    EVT_BUTTON(ID_WXBUTTON1, CProgressDlg::OnOK)
    EVT_BUTTON(ID_WXBUTTON3, CProgressDlg::OnCancel)
    EVT_COMMAND(wxID_ANY, wxEVT_UPDATE_PROGRESS,OnUpdatProgress)
    EVT_CLOSE( CProgressDlg::OnClose)
    EVT_PAINT(CProgressDlg::OnPaint)
END_EVENT_TABLE()

CProgressDlg::CProgressDlg(wxWindow *parent)
: wxDialog(parent, wxID_ANY, wxString(_T("安装-升级程序")),wxDefaultPosition,wxSize(582,389)),
m_hThreadHandle(NULL)
{

	GetJPGHandle(m_BackBitmap);	
	SetIcon(wxIcon(_T("IDI_UPDATEFASTTRADER")));	
	WxButtonCancel = new wxButton(this, ID_WXBUTTON3, wxT("取消"), wxPoint(462, 328), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton3"));

	WxButtonOK = new wxButton(this, ID_WXBUTTON1, wxT("确定"), wxPoint(339, 328), wxSize(97, 25), 0, wxDefaultValidator, wxT("WxButton1"));
	WxButtonOK->Enable(false);
	WxStaticText2 = new wxStaticText(this, ID_PROGRESS_STATIC, wxT("0%"), wxPoint(196, 286), wxDefaultSize, 0, wxT("WxStaticText2"));

	WxGauge1 = new wxGauge(this, ID_WXGAUGE1, 100, wxPoint(195, 266), wxSize(364, 17), wxGA_HORIZONTAL, wxDefaultValidator, wxT("WxGauge1"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("升级程序正在进行到以下步骤："), wxPoint(196, 9), wxDefaultSize, 0, wxT("WxStaticText1"));

	//WxStaticBitmap1 = new wxStaticBitmap(this, ID_WXSTATICBITMAP1, wxNullBitmap, wxPoint(0, 0), wxSize(186, 325) );

	wxArrayString arrayStringFor_WxListBox1;
	WxListBox1 = new wxListBox(this, ID_WXLISTBOX1, wxPoint(195, 32), wxSize(362, 227), arrayStringFor_WxListBox1, wxLB_SINGLE);

	m_pCheckVersion = new CheckVersion("", "");
	m_pCheckVersion->SetMsgWindow(this);	
}
CProgressDlg::~CProgressDlg()
{
	if(m_pCheckVersion)
	{
		delete m_pCheckVersion;
		m_pCheckVersion = NULL;
	}
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
    if(m_pCheckVersion)
    {
        m_pCheckVersion->SetCancelUpdate(true);	
    }

    Close(true);

}
DWORD WINAPI UpdateThread(LPVOID lpvParam)
{
	CProgressDlg *pDlg =(CProgressDlg *)lpvParam;
	if(pDlg == NULL)
		return 0;

	if (pDlg->m_pCheckVersion->NeedUpdate())
	{
		if(pDlg->m_pCheckVersion->DownLoadAllFile())
		{
			if(!pDlg->m_pCheckVersion->DoUpdate())
			{
				wxString strInfo = wxT("升级失败！请自行下载安装程序，手动安装！");
				double dbPercent =0;
				pDlg->m_pCheckVersion->ProcessMsg(strInfo, dbPercent);
			}
		}
		else
		{
			wxString strInfo = wxT("升级失败！请自行下载安装程序，手动安装！");
			double dbPercent =0;
			pDlg->m_pCheckVersion->ProcessMsg(strInfo, dbPercent);

		}		
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
	if(m_pCheckVersion)
		m_pCheckVersion->SetCancelUpdate(true);	
//	WaitForSingleObject(m_hThreadHandle, INFINITE);
	CloseHandle(m_hThreadHandle);
	
	if(m_pCheckVersion)
	{		
		delete m_pCheckVersion;
		m_pCheckVersion = NULL;
	}	
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