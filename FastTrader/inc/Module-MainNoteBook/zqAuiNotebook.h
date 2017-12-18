#pragma once
#include "stdafx.h"
#include <wx/splitter.h>
#define MAXGROUPITEM  19
#define MAX_CAPTION_LENGTH                 64
typedef bool (*CreatePaneFunc)(int id,wxWindow** win,wxString& caption);
typedef void (*ChangeStyleFunc)(int);
typedef struct _tagPageItem
{
    _tagPageItem()
	{
       window=NULL;
	   memset(caption,0,MAX_CAPTION_LENGTH);
	};
    bool operator== (const _tagPageItem& r) const
    {
		return this->window->GetId()==r.window->GetId();
    }
    wxWindow* window;
    char  caption[MAX_CAPTION_LENGTH];
}PageItem;
typedef std::vector<PageItem> PageArray;
class MySplitterWindow: public wxSplitterWindow
{
public:
	MySplitterWindow(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSP_3D| wxNO_BORDER);
	bool OnSashPositionChange(int WXUNUSED(newSashPosition));
    bool IsFixedAccountPanel();
public:
	bool m_fixed;
	float  m_fHoldingSashPosition;
};
class MyMiniFrame: public wxMiniFrame
{
public:
	MyMiniFrame(wxWindow *parent,
              wxWindowID id,
              const wxString& title,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style =wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION |
                                wxFRAME_NO_TASKBAR | wxFRAME_FLOAT_ON_PARENT | 
                                wxCLIP_CHILDREN|wxCLOSE_BOX)
  {
	  m_nFloatPanelIndex = 0;
	  m_nDeltaSashPos = 0;
	  m_parent=parent;
      Create(parent, id, title, pos, size, style);
  }
  void SetFloatPanePos(int isone,int sashpos, int nPageIndex );
  void OnClose(wxCloseEvent& event);
  void AdjustSashPos( int nMinWidth, bool bCheck );
  DECLARE_EVENT_TABLE()
private:
	int m_FloatPaneIsWindowOne;
	int m_FloatPaneSashPosition;
	int m_nFloatPanelIndex;
	int m_nDeltaSashPos;
	wxWindow* m_parent;

};
class zqPanel : public wxPanel
{
public:
	zqPanel();
    zqPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	wxString GetAppID(){return m_AppID;}
	void SetAppID(wxString id){m_AppID=id;}
	void OnSize(wxSizeEvent& event);
	DECLARE_EVENT_TABLE()
private:
	wxString m_AppID;
	DECLARE_DYNAMIC_CLASS(zqPanel)
};
class zqAuiNotebook : public wxPanel
{
public:
    /***********************************************************
    *   brief   :   标准构造函数
    ***********************************************************/
    zqAuiNotebook(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style=wxTAB_TRAVERSAL | wxNO_BORDER);
    virtual ~zqAuiNotebook(void);
protected:
	wxString  zqAddPage(wxWindow* page,wxString& caption);
	void DeletePageAndNotebook(wxAuiNotebook* pSrcNotebook);
	//拖动消息响应
	void OnDragDone(wxAuiNotebookEvent& event);
	//是否容许拖动
	void OnAllowNotebookDnD(wxAuiNotebookEvent& evt);
	//载入默认布局
	void LoadDefaultPerspective();
	//
	wxWindow* CreateSplitterPage(wxWindow* pParent,TiXmlElement* pItems);
	wxWindow* CreateSplitterFrame(wxWindow* pParent,TiXmlElement* pItems);
	bool LoadPerspective(const wxString& layout);
	bool CheckPerspective(const wxString& layout);
	bool CheckSplitterPage(TiXmlElement* pItems);
	bool CheckSplitterFrame(TiXmlElement* pItems);
	void ReadPerspective(const wxString& layout);
	//应用布局
	//void ApplyPerspective();
	//
	wxString GetSplitterDirection(wxSplitterWindow* pSplitter);
	void SaveSplitter(wxSplitterWindow* pSplitter,TiXmlElement *RootElement);
	//循环获取全部分割视图
	void MyEnumChildSplitter(MySplitterWindow* pSplitter,vector<MySplitterWindow*>* pAllWin);
	//获取全部分割视图。返回顶级分割
	MySplitterWindow* GetAllSplitterWindow(vector<MySplitterWindow*>* pAllWin=NULL);
	//移除布局
	void RemovePerspective();
	//页面依次添加到Tab中
    void TabAddPage(vector<wxWindow*>& parentvec);
	//保存布局到字符串
	wxString SavePerspective();
public:
    //
    bool LayoutStyleIsSunGard();
	//从文件载入布局
	void LoadPerspectiveFromFile(const wxString& filename=wxEmptyString);
	//保存布局到文件
	void SavePerspectiveToFile(const wxString& filename);
	//判断当前风格是否改变
	bool IsCurStyleChanged();
	void SetStyleChangeFlag( bool bChange );
public:
	bool DeleteBlankPage(int nPos, wxString AppID);
	bool DeleteQuotePage(wxString AppID);
	wxWindow* NewQuotePage(wxString caption,wxString AppID);
	wxWindow* NewBlankPage(int nPos,wxString caption,wxString AppID);
	void FixedPane(bool fixed);
	bool DoFloatPaneOpen(wxWindow* page,wxRect& rect=wxRect());
	bool IsFloatPaneOpen(wxWindow* page);
	bool DoFloatPaneClose(wxWindow* page);
	//金仕达设置界面打开，page显示在下面
	bool DoConfigPaneOpen(wxWindow* page);
	//恢复正常金仕达界面
	bool DoConfigPaneClose();
	//当前是否金仕达设置界面
	bool IsConfigPaneOpen();

public:
	size_t    GetPageCount(){return m_pageArray.size();}
	wxWindow* GetPageWindow(int index){return m_pageArray[index].window;}
	wxWindow* GetPageWindowFromId(int id);
	wxString  GetPageCaptionFromId(int id);
	int       GetPageIndex(wxWindow* page);
	wxString  GetPageCaption(int index){return m_pageArray[index].caption;}
	void      SetPageCaption(int index,wxString str);
public:
	void		SetSelection(int index);
	void		SetSelection(wxWindow* win);
	bool 		IsSelected(wxWindow* win);
	wxWindow*			GetSelectWindow();
	//是否是各个子模块窗口
	bool		IsModulePage(wxWindow* win);
	void        SetCallbackFunc(CreatePaneFunc func1,ChangeStyleFunc func2);
public:
	void OnSize(wxSizeEvent& evt);
	void OnIdle(wxIdleEvent& event);
	void EnumSplitter(wxSplitterWindow* pSplitter);
	//int CalcLayoutStyle(bool bfirst);
	int GetLayoutStyle(){return m_layout_style;}
    void SetLayoutCaption(wxString caption){m_layout_caption=caption;}
    wxString GetLayoutCaption(){return m_layout_caption;}
protected:
	int m_layout_style;	//布局类型
    wxString m_layout_caption;
	PageArray m_pageArray; 
	bool m_bNeedSetSashPosition;
	//当前是否金仕达设置界面
	bool m_IsConfigpaneOpen;
	//创建面板函数指针
	CreatePaneFunc m_CreatePaneFunc;
	ChangeStyleFunc m_ChangeStyleFunc;
	//面板id
	set<int>  m_PageIds;
	int m_CreateErrorId;
	DECLARE_EVENT_TABLE()
	//friend class MainFrame;
	//保存下浮面板打开前的分割状态
    vector<MySplitterWindow*> m_allBeforeSplitter;
private:
	bool m_bStyleChange;
};
