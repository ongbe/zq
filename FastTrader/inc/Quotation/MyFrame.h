#pragma once
#include "wx/frame.h"

#include "KLineView.h"
#include "KLine.h"
#include "KVolume.h"
#include "KScales.h"

#include "TLineView.h"
#include "TLine.h"
#include "TVolume.h"
#include "TScales.h"
#include "AutoLock.h"
#include "CritSec.h"
#include "TCPDataService.h"
#include "KSplitterWindow.h"
#include "QPanel.h"
#include "FData.h"
#include "../inc/PlusinCommon/PlusinAPI.h"

DECLARE_EVENT_TYPE(wxEVT_Data_InTime, 3700)
DECLARE_EVENT_TYPE(wxEVT_GetTime, 3701)
DECLARE_EVENT_TYPE(wxEVT_SetLineViewTime, 3702)
DECLARE_EVENT_TYPE(wxEVT_ReSend_Message, 3703)
DECLARE_EVENT_TYPE(wxEVT_MyFrame_AddMySelect, 3704)
DECLARE_EVENT_TYPE(wxEVT_MyFrame_MaxMize,	  3705)
class MyFrame: public wxPanel
{
public:
	MyFrame::MyFrame(wxWindow *parent,
		int x, int y, int width, int height,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

  virtual ~MyFrame();
  void Quit(wxCommandEvent& WXUNUSED(event) );
  void OnSize(wxSizeEvent& event);
  void OnDataInTime(PlatformStru_DepthMarketData* pData, CFData* pFData, bool bUpdate);//bupdate 主要用于区分是不是当前显示的期货行情，是则刷新，不是则不刷新


  
private:
	DECLARE_EVENT_TABLE()
	DECLARE_NO_COPY_CLASS(MyFrame)
public:
	CKLineView*		m_pKLineView;
	CTLineView*		m_pTLineView;
	CQPanel*		m_pQPanel;		
	KSplitterWindow* m_splitter;

public:
	
	
private:
	WXHWND   m_parent;
public:	
	void OnClose(wxCloseEvent& evt);
private:
	void OnFuncLDBClickKLine(wxCommandEvent& event);	
	void OnFuncLDBClickTLine(wxCommandEvent& event);

	void OnFuncKeyDownTLine(wxCommandEvent& event);
	void OnFuncKeyDownKLine(wxCommandEvent& event);
	void OnFuncKeyQPanel(wxCommandEvent& event);

	void OnKLineViewInfo(wxCommandEvent& event);
	

	



private:
	
	bool InitCfg(TiXmlElement *root);

	struct  SSpan
	{
		struct tm tmBegin;
		struct tm tmEnd;
	};
	map<string, vector<SSpan*>*> m_mapTransTime;//交易时间，现在已经用不上了，如果某天交易时间改到行情软件里面做，那么这个还是用的上的，先保留代码，以备将来可能的更改。


	

public:
	void        InitGUI(CFData *pData, PlatformStru_DepthMarketData *pQutoData, bool bRemain);
	void        InitGUIData(CFData *pFData, PlatformStru_DepthMarketData *pQutoData);
	bool		InitFromCFG(TiXmlElement *root);
	CFData*     GetFData();

	CFData*		m_pFData;
protected:
	int			   m_nTrace;	
	BOOL GetRegIntValue(HKEY hRoot,wxString strKeyName,wxString strValueName,int& nValue);


public:
	void OnFuncReturnKLine(wxCommandEvent& event);
	void OnFuncReturnTLine(wxCommandEvent& event);
	void OnFuncReturnBaoJiaBiao(wxCommandEvent& event);
	void OnFuncReturnBaoJiaBiao2(wxCommandEvent& event);


	void OnReturnKLine();
	void OnReturnTLine();

	int m_nRightSpanMiniWidth;
	void SetMiniWidth(unsigned long value);


	int					m_nWidthRightPanel;
	bool				m_bShowRightPanel;
	void				SizeWindow();

public:
	void SendMaxMize();
	void SetAddMySelect();
	int  m_nMaxMize;
	int  GetMaxSize();


	
};

