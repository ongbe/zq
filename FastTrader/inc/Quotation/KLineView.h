#pragma once

#include "wx\msw\dialog.h"
#include "KLine.h"
#include "KVolume.h"
#include "KScales.h"
#include "KTechIndex.h"
#include "KSplitterWindow.h"

#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
#include "../inc/Quotation/FData.h"
DECLARE_EVENT_TYPE(wxEVT_RETURN_TLINE, 4001)
DECLARE_EVENT_TYPE(wxEVT_RETURN_BAOJIABIAO, 4002)
DECLARE_EVENT_TYPE(wxEVT_KLINEVIW_INFO, 4003)

class MyFrame;
class CKLineView :public wxPanel
{
public:
	CKLineView(MyFrame* myFrame);

	wxWindow* CreateDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER|wxCLIP_CHILDREN,
		const wxString& name = wxPanelNameStr);


	~CKLineView(void);

	DECLARE_DYNAMIC_CLASS (CKLineView)
	 
	DECLARE_EVENT_TABLE()

	void OnPaint (wxPaintEvent & PaintEvent);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event)
	{
		event.Skip();
	}
protected:
	CKLine	 *m_pKLineCtrl;
	CKVolume *m_pKVolumeCtrl;
	CKTechIndex  *m_pKTechIndex;
	CKScales *m_pKScalesCtrl;


	long     m_lKLineType;//所有的图形形态
	
	KSplitterWindow* m_splitter;
	KSplitterWindow* m_splitterTop;
	KSplitterWindow* m_splitterBottom;
	

	MyFrame* m_myFrame;
public:
	

public:
	virtual void CfgShow(long lKLineType);											//显示哪些k线，量能，技术指标，时间轴
	void ShowTip(int nType, BOOL bShow = FALSE);
	wxPanel *GetControl(KTYPE kType);				//得到内部某个控件的地址
	void SetPhrase(EnumPhrase enumPhrase);			//实现时间周期的动态切换
	EnumPhrase GetPhrase();
	void SetMarkType(EnumMark enumMark);
	BOOL SetInTimeData();
	bool InitCfg(TiXmlElement *root);
	void SetLastTransTime(struct tm tmTime);
	
	void SetFData(CFData *pData);
	CFData* GetFData();

	
public:
	void ClearMemory_Mark();

public:
	void OnFuncKeyDown(wxCommandEvent& event);
	void OnFuncLButtonDown(wxCommandEvent& event);	
	void OnFuncLDBClick(wxCommandEvent& event);	
	void OnFuncRButtonUp(wxCommandEvent& event);
	void OnFuncMouseMoveHook(wxCommandEvent& event);
	void OnFuncMouseLeave(wxCommandEvent& event);
	void OnFuncMouseEnter(wxCommandEvent& event);
	void OnMouseLeave(wxMouseEvent &event);
	void OnMouseEnter(wxMouseEvent &event);
	
	void PhraseMin1(wxCommandEvent& event);
	void PhraseMin5(wxCommandEvent& event);
	void PhraseMin15(wxCommandEvent& event);
	void PhraseMin30(wxCommandEvent& event);
	void PhraseMin60(wxCommandEvent& event);

	void PhraseDay(wxCommandEvent& event);
	void PhraseWeek(wxCommandEvent& event);
	void PhraseMonth(wxCommandEvent& event);

	void CheckPhrase(wxMenu *&menuPopUp);
	void CrossShow(wxCommandEvent& event);
	
	
	void MarkTriangleUp(wxCommandEvent& event);
	void MarkTriangleDown(wxCommandEvent& event);
	void DelMark(wxCommandEvent& event);

	
	void OnMarksChange(wxCommandEvent& event);
	void OnTechIndexAdjustPara(wxCommandEvent& WXUNUSED(event));
	void OnTechIndexUseExpress(wxCommandEvent& WXUNUSED(event));

	void OnMA(wxCommandEvent& WXUNUSED(event));
	void OnKDJ(wxCommandEvent& WXUNUSED(event));
	void OnMACD(wxCommandEvent& WXUNUSED(event));
	void OnBOLL(wxCommandEvent& WXUNUSED(event));
	void OnUOS(wxCommandEvent& WXUNUSED(event));
	void OnBIAS(wxCommandEvent& WXUNUSED(event));
	void OnDMI(wxCommandEvent& WXUNUSED(event));
	void OnPSY(wxCommandEvent& WXUNUSED(event));
	void OnROC(wxCommandEvent& WXUNUSED(event));
	void OnVOL(wxCommandEvent& WXUNUSED(event));
	void OnBBI(wxCommandEvent& WXUNUSED(event));
	void OnEXPMA(wxCommandEvent& WXUNUSED(event));
	void OnDMA(wxCommandEvent& WXUNUSED(event));
	void OnSAR(wxCommandEvent& WXUNUSED(event));
	void OnTRIX(wxCommandEvent& WXUNUSED(event));
	void OnMTM(wxCommandEvent& WXUNUSED(event));
	void OnCR(wxCommandEvent& WXUNUSED(event));
	void OnVR(wxCommandEvent& WXUNUSED(event));
	void OnOBV(wxCommandEvent& WXUNUSED(event));
	void OnASI(wxCommandEvent& WXUNUSED(event));
	void OnEMV(wxCommandEvent& WXUNUSED(event));
	void OnCCI(wxCommandEvent& WXUNUSED(event));
	void OnRSI(wxCommandEvent& WXUNUSED(event));
	void OnCDP(wxCommandEvent& WXUNUSED(event));

	void OnKTLineAdd(wxCommandEvent& event);
	void OnKTLineMaxSize(wxCommandEvent& event);

	void OnReturnTLine(wxCommandEvent& event);
	void OnReturnBaoJiaBiao(wxCommandEvent& event);

	void OnViewOne(wxCommandEvent& WXUNUSED(event));
	void OnViewTwo(wxCommandEvent& WXUNUSED(event));
	void OnViewThree(wxCommandEvent& WXUNUSED(event));
	void OnViewInfo(wxCommandEvent& WXUNUSED(event));
	
private:
	CFData*				m_pFData;
	EnumPhrase			m_enumPhrase;   //时间周期
	string				m_strTechName_ByMessage;
	CKCtrl*				m_pRightUpWnd;
	bool				m_bInit;//新合约第一次周期切换
	
	int					m_nViewCount;//视图总数
	
	void				OnView1();
	void				OnView2();
	void				OnView3();

	wxString			GetTechIndex(KTYPE KType);

	bool				m_bShowRightPanel;
public:
	void				SetShowRightPanel(bool bShowRightPanel){	m_bShowRightPanel = bShowRightPanel;};



};
