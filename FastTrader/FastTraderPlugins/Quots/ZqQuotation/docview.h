/////////////////////////////////////////////////////////////////////////////
// Name:        docview.h
// Purpose:     Document/view demo
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: docview.h 35650 2005-09-23 12:56:45Z MR $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __DOCVIEWSAMPLEH__
#define __DOCVIEWSAMPLEH__

#include "wx/mdi.h"
#include "wx/docview.h"
#include "wx/docmdi.h"
#include "../../../inc/PlusinCommon/PlusinAPI.h"
#include "../../../inc/Quotation/F10Panel.h"
class wxDocManager;

// Define a new application
class MyApp: public wxApp
{
  public:
    MyApp(void);
    bool OnInit(void);
    int OnExit(void);

    wxMDIChildFrame *CreateChildFrame(wxDocument *doc, wxView *view, bool isCanvas);
	
	wxMDIChildFrame *GetSubFrame();
  protected:
    wxDocManager* m_docManager;
public:
	wxMDIChildFrame *m_subframe;
};

DECLARE_APP(MyApp)

// Define a new frame
//class MyCanvas;
class CHQFrame: public wxDocMDIParentFrame
{
  DECLARE_CLASS(CHQFrame)
 public:
  wxMenu *editMenu;
  
  CHQFrame(wxDocManager *manager, wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size,
    long type, WXHWND hWnd);

  void OnAbout(wxCommandEvent& event);
  MyFrame *CreateKFrame(wxView *view, wxMDIChildFrame *parent);

  void OnClose(wxCloseEvent& evt);
  void OnSize(wxSizeEvent& event);
  void OnTimer1(wxTimerEvent& event);
private:
	 MyFrame *m_myFrame;
	 
public:
	wxTimer			m_timerTrans;
	int				m_nTime;//timer的时间限制
	DWORD			m_dwBeginTickCount;
	struct tm		m_tmLastTrans;//最后交易时间
	void			SetLastTransTime(struct tm tmLastTrans);
	bool			m_bChangedTime;//时间更改标志
	string			m_strTradingDay; //交易日期
	bool			m_bInitTimerTrans;//第一次执行，用于初始化操作
private:
	WXHWND   m_parent;
	void OnReSendMessage(wxCommandEvent& event);
public:
	PlusinAPI*		m_pApi;
	int OnRecvApiMsg_Process( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId  );
	static int OnRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId);

	void ProcessConsoleMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen , int nRequestId);
	void ProcessExchangeMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen , int nRequestId);
	void ProcessInstrumentMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );
	void ProcessQuoteMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );
	void ProcessAccountMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );

	void ClearFData();
	map<string,  CFData *>  m_mapID2FData;
	map<DWORD,  string>     m_mapNum2ID;     //返回包的顺序，对应哪只期货的id
	int						m_nQueryNum;     //请求的序号

	CFData*		GetFDataByQueryID(int nQueryNum);
	int			GetNextRequestId();


	string		m_strSelFData;//哪只是显示的

protected:
	vector<string> m_vecFData;    //报价表中的所有合约
	int			   m_nFDataIndex; //在报价表的序号
	void		   OnPrior();     //报价表的前一个合约行情
	void		   OnNext();      //报价表的下一个合约行情

	void		   DelFData(int nCount);//超过多少只合约后，删除内存中多余的

	void SetTodayScales(string strTimes, vector<struct tm>& vecTimeScales);

public:
	void SendMaxMize();
	void SetAddMySelect();
	int  m_nMaxMize;
	int  GetMaxSize();


public:
	void OnFuncLDBClickKLine(wxCommandEvent& event);
	void OnFuncKeyDownTLine(wxCommandEvent& event);
	void OnFuncKeyDownKLine(wxCommandEvent& event);
	void OnFuncKeyQPanel(wxCommandEvent& event);
	void OnF10HeadCtrlKeyDown(wxCommandEvent& event);
	void OnReturnBaoJiaBiao(wxCommandEvent& event);
	
	void OnMyFrameAddMySelect(wxCommandEvent& event);
	void OnMyFrameMaxMize(wxCommandEvent& event);

	void OnF10();


	CF10Panel	m_F10Panel;
	bool InitFromCFG();
DECLARE_EVENT_TABLE()
};

extern CHQFrame *GetMainFrame(void);

#define DOCVIEW_CUT     1
#define DOCVIEW_ABOUT   wxID_ABOUT

extern bool singleWindowMode;


#endif
