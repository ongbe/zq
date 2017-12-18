#include "StdAfx.h"
#include "zqAuiNotebook.h"
#include "cfgMgr/CfgGlobal.h"
#include <wx/file.h>
#include "gui-common/FrameBase.h"
#include <set>
#include <map>
#include <algorithm>
#include "../inc/PlusinCommon/CommonDef.h"
#include "Module-Misc2/GlobalConfigMgr.h"

using namespace std;

#define MAX_PAGE_INDEX 13
#define DEFAULT_STYLE STYLE_SunGard
#define PERSPECTIVE_ROOT   "splitterlayout"
#define SINGLE_PANE         "S"
#define SPLITWINDOW_MIN_WIDTH	42
const wxSize ORDERPANEL_MIN_SIZE(200, 220);

//标准版
static int pTabGroup[MAXGROUPITEM]=
{
    GID_QUOT_TABLE,0,
    GID_ALL_ORDER,GID_THRADE_RECORD,GID_OPEN_ORDER,
    GID_POSITIONALL,
    GID_ACCOUNT,GID_INSTRUMENT,GID_QUERYLOCAL_ORDER,0,
	GID_ORDER_INSERT_PANEL,0,0,0
};
//多账号版
static int pTabGroup_MultiAccount[MAXGROUPITEM]=
{
    GID_QUOT_TABLE,0,
    GID_ALL_ORDER,GID_THRADE_RECORD,GID_OPEN_ORDER,
    GID_POSITIONALL,
    GID_INSTRUMENT,GID_QUERYLOCAL_ORDER,GID_MULTI_POSITION,0,
	GID_ORDER_INSERT_PANEL,0,0,0
};
static bool g_bQuotTableNotTab=true;//程序内部默认配置中报价表是否显示tab页签
static bool g_bFixPane=false;

MySplitterWindow::MySplitterWindow(wxWindow *parent, wxWindowID id,
                                   const wxPoint& pos,
                                   const wxSize& size,
								   long style)
{
	m_fixed=(style&wxSP_NOSASH)>0;
	if(m_fixed)
	{
		style&=~wxSP_NOSASH;
	}
	Create(parent, id, pos, size, style);
	m_fHoldingSashPosition=0.5;
    m_sashSize=2;
	SetSashGravity(0.5);
	SetMinimumPaneSize(SPLITWINDOW_MIN_WIDTH);
}

bool MySplitterWindow::OnSashPositionChange(int WXUNUSED(newSashPosition))
{
	wxWindow* pParent = GetParent();
	while( pParent != NULL )
	{
		zqAuiNotebook* pNoteBook = wxDynamicCast(pParent, zqAuiNotebook);
		if ( pNoteBook != NULL )
		{
			pNoteBook->SetStyleChangeFlag(true);
			break;
		}

		pParent = pParent->GetParent();
	}
	return IsFixedAccountPanel();
}
bool MySplitterWindow::IsFixedAccountPanel()
{
    wxWindow* pWin1=GetWindow1();
    if(pWin1)
    {
        wxAuiNotebook* pBook=wxDynamicCast(pWin1,wxAuiNotebook);
        wxSplitterWindow* pSplitter1=wxDynamicCast(pWin1,wxSplitterWindow);
        if(pBook==NULL&&pSplitter1==NULL)//无标签页面
        {
            if(pWin1->GetId()==GID_ACCOUNT)//资金面板
            {
                return false;
            }
        }
    }
    return true;
}

BEGIN_EVENT_TABLE(MyMiniFrame, wxMiniFrame)
    EVT_CLOSE(MyMiniFrame::OnClose)
END_EVENT_TABLE()
void MyMiniFrame::SetFloatPanePos(int isone,int sashpos, int nPageIndex )
{
	m_FloatPaneIsWindowOne=isone;
	m_FloatPaneSashPosition=sashpos;
	m_nFloatPanelIndex = nPageIndex;
}
void MyMiniFrame::OnClose(wxCloseEvent& evt)
{
	Hide();
	wxSplitterWindow* pSplitter=wxDynamicCast(m_parent,wxSplitterWindow);
    wxWindowList& winlist=GetChildren();
    wxWindowList::compatibility_iterator node = winlist.GetFirst();
    wxWindow *pFloatPage=NULL;
	while (node)
    {
        pFloatPage = node->GetData();
        node = node->GetNext();
    }  
	if(pSplitter)
	{ 
        wxAuiNotebook* pNotebook=NULL;
        if(m_FloatPaneIsWindowOne<0)
        {
            pNotebook=(wxAuiNotebook*)pSplitter->GetWindow1();
            pNotebook=wxDynamicCast(pNotebook,wxAuiNotebook);
            pFloatPage->Reparent(pNotebook);
            pNotebook->InsertPage(m_nFloatPanelIndex,pFloatPage,GetLabel(),true);
        }
        else
        {
            if(pSplitter->IsSplit())
            {
                pNotebook=(wxAuiNotebook*)((m_FloatPaneIsWindowOne==1)?pSplitter->GetWindow1():pSplitter->GetWindow2());
                pNotebook=wxDynamicCast(pNotebook,wxAuiNotebook);
                pFloatPage->Reparent(pNotebook);
                pNotebook->InsertPage(m_nFloatPanelIndex,pFloatPage,GetLabel(),true);
				pSplitter->SetSashPosition(m_FloatPaneSashPosition);
            }
            else
            {
                pNotebook=new wxAuiNotebook(pSplitter,wxID_ANY,wxDefaultPosition,wxDefaultSize,TAB_STYLE);
                pFloatPage->Reparent(pNotebook);
                pNotebook->AddPage(pFloatPage,GetLabel(),true);
                if(pSplitter->GetSplitMode()==wxSPLIT_VERTICAL)
                {
                    if(m_FloatPaneIsWindowOne==1)
                        pSplitter->SplitVertically(pNotebook,pSplitter->GetWindow1());
                    else
                        pSplitter->SplitVertically(pSplitter->GetWindow1(),pNotebook);
                }
                else
                {
                    if(m_FloatPaneIsWindowOne==1)
                        pSplitter->SplitHorizontally(pNotebook,pSplitter->GetWindow1());
                    else
                        pSplitter->SplitHorizontally(pSplitter->GetWindow1(),pNotebook);
                }
                pSplitter->SetSashPosition(m_FloatPaneSashPosition);
				pSplitter->SetMinSize(ORDERPANEL_MIN_SIZE);
				pNotebook->SetMinSize(ORDERPANEL_MIN_SIZE);
            }
        }

	}

    wxAuiManagerEvent e(wxEVT_AUI_PANE_CLOSE);
    wxGetTopLevelParent(m_parent)->ProcessEvent(e);
	evt.Skip();

}

void MyMiniFrame::AdjustSashPos( int nMinWidth, bool bCheck )
{
	if ( !bCheck )
	{
		m_FloatPaneSashPosition -= m_nDeltaSashPos;
		m_nDeltaSashPos = 0;
	}
	else
	{
		wxSplitterWindow* pSplitter=wxDynamicCast(m_parent,wxSplitterWindow);
		if( NULL != pSplitter && pSplitter->GetSplitMode() == wxSPLIT_VERTICAL )
		{
			wxSize splitterSize = pSplitter->GetSize();
			if( m_FloatPaneIsWindowOne == 1 )
			{
				if ( m_FloatPaneSashPosition < nMinWidth )
				{
					m_nDeltaSashPos = nMinWidth - m_FloatPaneSashPosition;
					m_FloatPaneSashPosition = nMinWidth;
				}
			}
			else if ( m_FloatPaneIsWindowOne == 0)
			{
				if ( splitterSize.GetWidth() - m_FloatPaneSashPosition < nMinWidth)
				{
					m_nDeltaSashPos = splitterSize.GetWidth() - nMinWidth - m_FloatPaneSashPosition;
					m_FloatPaneSashPosition = splitterSize.GetWidth() - nMinWidth;
				}
			}
			else
			{

			}
		}
	}
}

IMPLEMENT_DYNAMIC_CLASS(zqPanel,wxPanel)

BEGIN_EVENT_TABLE(zqPanel, wxPanel)
	EVT_SIZE(zqPanel::OnSize)
END_EVENT_TABLE()

zqPanel::zqPanel():
wxPanel()
{
}
zqPanel::zqPanel(wxWindow *parent,
            wxWindowID winid ,
            const wxPoint& pos,
            const wxSize& size ,
            long style,
			const wxString& name):
wxPanel(parent,winid,pos,size,style,name)
{
}
void zqPanel::OnSize( wxSizeEvent& event )
{
	HWND childHWnd = GetWindow( GetHwndOf(this), GW_CHILD);
	if ( IsWindow( childHWnd ))
	{
		wxSize size = GetClientSize();
		::MoveWindow( childHWnd, 0, 0, size.GetWidth(), size.GetHeight(),TRUE);
	}
}
zqAuiNotebook::zqAuiNotebook(wxWindow* parent,
                             wxWindowID id ,
                             const wxPoint& pos ,
                             const wxSize& size ,
                             long style):
wxPanel(parent,id,pos,size,style)
, m_CreatePaneFunc(NULL)
{
	m_bNeedSetSashPosition=true;
	m_IsConfigpaneOpen=false;
	m_bStyleChange = false;
}

zqAuiNotebook::~zqAuiNotebook(void)
{

}
BEGIN_EVENT_TABLE(zqAuiNotebook,wxPanel)
	EVT_SIZE(zqAuiNotebook::OnSize)
	EVT_IDLE(zqAuiNotebook::OnIdle)
	EVT_AUINOTEBOOK_DRAG_DONE(wxID_ANY,zqAuiNotebook::OnDragDone)
    EVT_AUINOTEBOOK_ALLOW_DND(wxID_ANY, zqAuiNotebook::OnAllowNotebookDnD)
END_EVENT_TABLE()

 void zqAuiNotebook::LoadPerspectiveFromFile(const wxString& filename)
 {
	 m_IsConfigpaneOpen = false;
     bool loadsuccess=false;
     if(wxFile::Exists(filename))
     {
         wxFile file(filename);
         if(file.IsOpened())
         { 
             wxFileOffset nSize = file .Length();
             if ( nSize != wxInvalidOffset )
             { 
                 char* data = new char[nSize+1];
                 data[nSize]=0;
                 if ( file.Read(data,nSize) == nSize )
                 {
                     loadsuccess=LoadPerspective(data);
                 }  
                 delete[] data;
             }
             file .Close() ;
         }
     }
     if(!loadsuccess)
     {
         LoadDefaultPerspective();
     }
}
#define GET_LPSTR(p) (p?p:wxEmptyString)
wxWindow* zqAuiNotebook::CreateSplitterPage(wxWindow* pParent,TiXmlElement* pItems)
{
	wxString strData,strText=GET_LPSTR(pItems->GetText());
	wxWindow* win=NULL;
	wxString caption;	
    if(strText.find(',')==wxString::npos&&strText.EndsWith(SINGLE_PANE))//是无标签面板
    {
        strData=strText.SubString(0,strText.Length()-1-strlen(SINGLE_PANE));      
        if(m_CreatePaneFunc)
        {
            int nModuleID = atoi(strData);
            if(m_CreatePaneFunc(nModuleID,&win,caption))
            {
               win->Reparent(pParent);
               zqAddPage(win,caption);
            }
        }
    }
    else
    {
        wxAuiNotebook* pNotebook=new wxAuiNotebook(pParent,wxID_ANY,wxDefaultPosition,wxDefaultSize,TAB_STYLE);
        do
        {
            strData=strText.BeforeFirst(',');
            if(m_CreatePaneFunc)
            {

                int nModuleID = atoi(strData);                
                if(m_CreatePaneFunc(nModuleID,&win,caption))
                {
                    win->Reparent(pNotebook);
                    pNotebook->AddPage(win,zqAddPage(win,caption));
                    if ( nModuleID == GID_ORDER_INSERT_PANEL )
                    {
                        pNotebook->SetMinSize(ORDERPANEL_MIN_SIZE);
                        pParent->SetMinSize(ORDERPANEL_MIN_SIZE);
                    }
                }
                else
                    m_CreateErrorId=atoi(strData);
            }
            strText=strText.AfterFirst(',');
        }
        while(strText.Length()>0);
        win=pNotebook;
    }

    return win;
}

wxWindow* zqAuiNotebook::CreateSplitterFrame(wxWindow* pParent,TiXmlElement* pItems)
{
	wxString strTagName=GET_LPSTR(pItems->Value());
	if(strTagName=="splitter")
	{
		vector<wxWindow*> windowvector;
		MySplitterWindow* pSplitter=new MySplitterWindow(pParent);
		if(pParent==this)
		{
			pSplitter->SetSize(pParent->GetClientSize());
			pSplitter->Hide();
		}
		TiXmlElement* pChildItems=pItems->FirstChildElement();
		while(pChildItems)
		{
			strTagName=GET_LPSTR(pChildItems->Value());
			if(strTagName=="splitter")
			{
				windowvector.push_back(CreateSplitterFrame(pSplitter,pChildItems));
			}
			if(strTagName=="page")
			{
				windowvector.push_back(CreateSplitterPage(pSplitter,pChildItems));
			}
			pChildItems=pChildItems->NextSiblingElement();
		}
		const char *strDir = pItems->Attribute("dir");	
		const char *strPos = pItems->Attribute("position");	
		const char *strGrv = pItems->Attribute("gravity");
		if(strDir&&wxString(strDir)=="horizontal")
		{
            if(windowvector.size()==2)
			    pSplitter->SplitHorizontally(windowvector[0],windowvector[1]);
		}
		else if(strDir&&wxString(strDir)=="vertical")
		{
            if(windowvector.size()==2)
			    pSplitter->SplitVertically(windowvector[0],windowvector[1]);
		}
		else if(wxString(strDir)=="unsplit")
		{
            if(windowvector.size()>=1)
			    pSplitter->Initialize(windowvector[0]);

		}
		if(strPos) pSplitter->m_fHoldingSashPosition=atof(strPos);
        if(strGrv) pSplitter->SetSashGravity(atof(strGrv));
		windowvector.clear();
		return pSplitter;
	}
	if(strTagName=="page")
	{
		return CreateSplitterPage(pParent,pItems);
	}
	return NULL;
}
bool zqAuiNotebook::LoadPerspective(const wxString& layout) 
{
	if(CheckPerspective(layout))
	{
		ReadPerspective(layout);
		m_bStyleChange = false;
		return true;
	}
	return false;
}
bool zqAuiNotebook::CheckPerspective(const wxString& layout) 
{
    TiXmlDocument myDocument;
	if(myDocument.Parse(layout)==NULL) return false;//解析错误
    TiXmlElement* RootElement=myDocument.RootElement();  
	if(RootElement==NULL)return false;//没有根节点
	wxString strTagName=GET_LPSTR(RootElement->Value());
	if(strTagName!=PERSPECTIVE_ROOT) return false;//根节点不是 PERSPECTIVE_ROOT
	TiXmlElement* pItems=RootElement->FirstChildElement();
	if(pItems==NULL) return false;
	m_PageIds.clear();
	return CheckSplitterFrame(pItems);
}
bool zqAuiNotebook::CheckSplitterPage(TiXmlElement* pItems)
{
	wxString strData,strText=GET_LPSTR(pItems->GetText());
	int index=0,value=0;
	do
	{
		strData=strText.BeforeFirst(',');
		value=atoi(strData);
		if(value>0)
		{
			pair<set<int>::iterator,bool> ret=m_PageIds.insert(value);
			if(!ret.second)return false;//有重复的
			index++;
		}
		strText=strText.AfterFirst(',');
	}
	while(strText.Length()>0);
	if(index==0) return false;//没有一个
	return true;
}
bool zqAuiNotebook::CheckSplitterFrame(TiXmlElement* pItems)
{
	wxString strTagName=GET_LPSTR(pItems->Value());
	if(strTagName=="splitter")
	{
		const char *pDir = pItems->Attribute("dir");	
		if(pDir==NULL) return false;
		wxString strDir=pDir;
		int index=0,count=0;
		if(strDir=="horizontal"||strDir=="vertical")//两个子节点
			count=2;
		else if(strDir=="unsplit")//一个子节点
			count=1;
		else
			return false;
		TiXmlElement* pChildItems=pItems->FirstChildElement();
		while(pChildItems)
		{
			strTagName=GET_LPSTR(pChildItems->Value());
			if(strTagName=="splitter")
			{
				if(!CheckSplitterFrame(pChildItems)) return false;
			}
			else if(strTagName=="page")
			{
				if(!CheckSplitterPage(pChildItems)) return false;
			}
			else
				return false;
			index++;
			pChildItems=pChildItems->NextSiblingElement();
		}
		if(index!=count) return false;
	}
	else if(strTagName=="page")
	{
		if(!CheckSplitterPage(pItems)) return false;
	}
	else
		return false;
	return true;
}
void zqAuiNotebook::ReadPerspective(const wxString& layout) 
{
    RemovePerspective();
	//初始化变量
	m_CreateErrorId=0;
    //递归读取
	TiXmlDocument myDocument;
	myDocument.Parse(layout);
	TiXmlElement* RootElement=myDocument.RootElement();  
    const char *strCaption = RootElement->Attribute("caption");
    if(strCaption)  SetLayoutCaption(strCaption);
	// 获取风格
	const char* strStyle = RootElement->Attribute("style");
	if ( NULL != strStyle )
	{
		m_layout_style = static_cast<_style>(atoi(strStyle));
	}
	else
	{
		m_layout_style = STYLE_SunGard;
	}

	TiXmlElement* pItems=RootElement->FirstChildElement(); 
	CreateSplitterFrame(this,pItems);
	m_bNeedSetSashPosition=true;
}
void zqAuiNotebook::TabAddPage(vector<wxWindow*>& parentvec)
{
	int index=0,parentindex=0;
	for(index=0;index<MAXGROUPITEM;index++)
	{
        int stockindex = IS_MULTIACCOUNT_VERSION ? pTabGroup_MultiAccount[index] : pTabGroup[index];
		if(stockindex>0)
		{
			wxWindow* win=NULL;
			wxString caption;	
			if(m_CreatePaneFunc)
			{
				m_CreatePaneFunc(stockindex,&win,caption);
			}
			if(win)
			{
				win->Reparent(parentvec[parentindex]);
                wxAuiNotebook* pBook=wxDynamicCast(parentvec[parentindex],wxAuiNotebook);
				wxString StrCaption=zqAddPage(win,caption);
                if(pBook)  
                    pBook->AddPage(win,StrCaption);	
                else
                    parentvec[parentindex]=win;

			}

		}
		else
		{
			parentindex++;
		}
	}
}
void zqAuiNotebook::MyEnumChildSplitter(MySplitterWindow* pSplitter,vector<MySplitterWindow*>* pAllWin)
{
	MySplitterWindow* pSplitter1=wxDynamicCast(pSplitter->GetWindow1(),MySplitterWindow);
	MySplitterWindow* pSplitter2=wxDynamicCast(pSplitter->GetWindow2(),MySplitterWindow);
	if(pSplitter1)
	{
		pAllWin->push_back(pSplitter1);
		MyEnumChildSplitter(pSplitter1,pAllWin);
	}
	if(pSplitter2)
	{
		pAllWin->push_back(pSplitter2);
		MyEnumChildSplitter(pSplitter2,pAllWin);
	}	
}
MySplitterWindow* zqAuiNotebook::GetAllSplitterWindow(vector<MySplitterWindow*>* pAllWin)
{
	wxWindowList& winlist=GetChildren();
	wxWindowList::compatibility_iterator node = winlist.GetFirst();
	MySplitterWindow* pSplitter=NULL;
	int iTopCount=0;
	while(node)
	{
		if(wxDynamicCast(node->GetData(), MySplitterWindow))
		{
			pSplitter=(MySplitterWindow*)node->GetData();
			iTopCount++;
		}
		node = node->GetNext();
	}  
	wxASSERT(iTopCount<=1);
	if(pAllWin)
	{
		pAllWin->clear();
		pAllWin->push_back(pSplitter);
		MyEnumChildSplitter(pSplitter,pAllWin);
	}
	return pSplitter;

}
void zqAuiNotebook::RemovePerspective()
{
	for(size_t i=0;i<GetPageCount();i++)
	{
		wxAuiNotebook* pNotebook=wxDynamicCast(GetPageWindow(i)->GetParent(),wxAuiNotebook);
		if(pNotebook)
		{
			pNotebook->RemovePage(pNotebook->GetPageIndex(GetPageWindow(i)));
			pNotebook->RemoveEmptyTabFrames();
		}
		GetPageWindow(i)->Reparent(this);
	}
	MySplitterWindow* pFirstSplitter=GetAllSplitterWindow();
	wxDELETE(pFirstSplitter);
    SetLayoutCaption(wxEmptyString);
}
void zqAuiNotebook::LoadDefaultPerspective() 
{
    m_layout_style=STYLE_SunGard;//赋值默认
    RemovePerspective();
    vector<wxWindow*> parentvec;
    long TabStyle=TAB_STYLE;
    m_IsConfigpaneOpen=false;
    if(g_bFixPane)  TabStyle&=~wxAUI_NB_TAB_MOVE;

    MySplitterWindow* topSplitter=new MySplitterWindow(this);
    topSplitter->Hide();
    MySplitterWindow* bottomSplitter=new MySplitterWindow(topSplitter,wxID_ANY);
    if(g_bQuotTableNotTab)
        parentvec.push_back(topSplitter);
    else
        parentvec.push_back(new wxAuiNotebook(topSplitter,wxID_ANY,wxDefaultPosition,wxDefaultSize,TabStyle));
    parentvec.push_back(new wxAuiNotebook(bottomSplitter,wxID_ANY,wxDefaultPosition,wxDefaultSize,TabStyle));
    parentvec.push_back(new wxAuiNotebook(bottomSplitter,wxID_ANY,wxDefaultPosition,wxDefaultSize,TabStyle));
    TabAddPage(parentvec);
    topSplitter->SplitHorizontally(parentvec[0],bottomSplitter);
    bottomSplitter->SplitVertically(parentvec[1],parentvec[2]);

    parentvec.empty();
    topSplitter->SetSize(GetClientSize());
    m_bNeedSetSashPosition=true;
}
wxString zqAuiNotebook::GetSplitterDirection(wxSplitterWindow* pSplitter)
{
	if(pSplitter->IsSplit())
	{
		if(pSplitter->GetSplitMode()==wxSPLIT_HORIZONTAL)
			return "horizontal";
		else
			return "vertical";
	}
	return "unsplit";
}
void zqAuiNotebook::SaveSplitter(wxSplitterWindow* pSplitter,TiXmlElement *RootElement)
{
	wxWindow* win1=pSplitter->GetWindow1();
	wxWindow* win2=pSplitter->GetWindow2();
	wxString strDir=GetSplitterDirection(pSplitter);

	TiXmlElement *itemsElement = new TiXmlElement("splitter");
	RootElement->LinkEndChild(itemsElement);
	itemsElement->SetAttribute("dir",strDir);
	float fPos = 0.0;
	if ( pSplitter->GetSplitMode() == wxSPLIT_HORIZONTAL)
	{
		fPos = float(pSplitter->GetSashPosition())/pSplitter->GetClientSize().GetHeight();
	}
	else
	{
		fPos = float(pSplitter->GetSashPosition())/pSplitter->GetClientSize().GetWidth();
	}
	itemsElement->SetAttribute("position",GlobalFunc::ConvertToString(fPos).c_str());
	itemsElement->SetAttribute("gravity",GlobalFunc::ConvertToString(pSplitter->GetSashGravity()).c_str());
	{
		wxString strItem;
		wxSplitterWindow* pChildSplitter=wxDynamicCast(win1,wxSplitterWindow);
		wxAuiNotebook* pChildNotebook=wxDynamicCast(win1,wxAuiNotebook);
		if(pChildSplitter)
		{
			SaveSplitter(pChildSplitter,itemsElement);
		}
		if(pChildNotebook)
		{
			TiXmlElement *pageElement = new TiXmlElement("page");
			itemsElement->LinkEndChild(pageElement);
			int count=pChildNotebook->GetPageCount();
			for(int i=0;i<count;i++)
			{
				strItem+=wxString::Format("%d,",pChildNotebook->GetPage(i)->GetId());
			}
			if(strItem.Right(1)==",") strItem.RemoveLast(1);
			TiXmlText *pContent = new TiXmlText(strItem.c_str());
			pageElement->LinkEndChild(pContent);
		}
        if(pChildSplitter==NULL&&pChildNotebook==NULL)//是无标签面板
        {
			TiXmlElement *pageElement = new TiXmlElement("page");
			itemsElement->LinkEndChild(pageElement);
			strItem=wxString::Format("%d%s",win1->GetId(),SINGLE_PANE);
			TiXmlText *pContent = new TiXmlText(strItem.c_str());
			pageElement->LinkEndChild(pContent);
        }
	}
	if(strDir!="unsplit")
	{
		wxString strItem;
		wxSplitterWindow* pChildSplitter=wxDynamicCast(win2,wxSplitterWindow);
		wxAuiNotebook* pChildNotebook=wxDynamicCast(win2,wxAuiNotebook);
		if(pChildSplitter)
		{
			SaveSplitter(pChildSplitter,itemsElement);
		}
		if(pChildNotebook)
		{
			TiXmlElement *pageElement = new TiXmlElement("page");
			itemsElement->LinkEndChild(pageElement);
			int count=pChildNotebook->GetPageCount();
			for(int i=0;i<count;i++)
			{
				strItem+=wxString::Format("%d,",pChildNotebook->GetPage(i)->GetId());
			}
			if(strItem.Right(1)==",") strItem.RemoveLast(1);
			TiXmlText *pContent = new TiXmlText(strItem.c_str());
			pageElement->LinkEndChild(pContent);
		}
        if(pChildSplitter==NULL&&pChildNotebook==NULL)//是无标签面板
        {
			TiXmlElement *pageElement = new TiXmlElement("page");
			itemsElement->LinkEndChild(pageElement);
			strItem=wxString::Format("%d%s",win2->GetId(),SINGLE_PANE);
			TiXmlText *pContent = new TiXmlText(strItem.c_str());
			pageElement->LinkEndChild(pContent);
        }
	}

}
wxString zqAuiNotebook::SavePerspective() 
{
	TiXmlDocument myDocument;
    TiXmlElement *RootElement = new TiXmlElement(PERSPECTIVE_ROOT);
	RootElement->SetAttribute("style", m_layout_style);
    if(!GetLayoutCaption().IsEmpty())
        RootElement->SetAttribute("caption",GetLayoutCaption());
	myDocument.LinkEndChild(RootElement);

    MySplitterWindow* pTopSplitter=GetAllSplitterWindow();
	if(pTopSplitter)
	{
		SaveSplitter(pTopSplitter,RootElement);
	}
	TiXmlPrinter printer; 
	RootElement->Accept(&printer); 
	return wxString::Format("%s", printer.CStr()); 
}
void zqAuiNotebook::SavePerspectiveToFile(const wxString& filename)
{
	wxString str;
	str=SavePerspective();
	if(!str.IsEmpty()&&!filename.IsEmpty())
	{
		wxFile file;
		if(file.Open(filename,wxFile::write))
		{
			file.Write(str,str.Length());
			file.Close();
		}
	}

	m_bStyleChange = false;
}

bool zqAuiNotebook::IsCurStyleChanged()
{
	return m_bStyleChange;
}

void zqAuiNotebook::SetStyleChangeFlag( bool bChange )
{
	m_bStyleChange = bChange;
}

void  zqAuiNotebook::SetPageCaption(int index,wxString str)
{
	strncpy(m_pageArray[index].caption,str.c_str(),MAX_CAPTION_LENGTH-1);
	wxAuiNotebook* pNotebook=wxDynamicCast(GetPageWindow(index)->GetParent(),wxAuiNotebook);
	if(pNotebook)
	{
		int id=pNotebook->GetPageIndex(GetPageWindow(index));
		if(id>=0)
		{
			pNotebook->SetPageText(id,str);
		}
	}
}

void zqAuiNotebook::FixedPane(bool fixed)
{
	g_bFixPane=fixed;
	unsigned int oldflags;
	for(int i=0;i<(int)GetPageCount();i++)
	{
		wxAuiNotebook* pNotebook=wxDynamicCast(GetPageWindow(i)->GetParent(),wxAuiNotebook);
		if(pNotebook)
		{
			oldflags=pNotebook->GetWindowStyleFlag();
			if(g_bFixPane)
				pNotebook->SetWindowStyleFlag(oldflags&(~wxAUI_NB_TAB_MOVE));
			else
				pNotebook->SetWindowStyleFlag(oldflags|wxAUI_NB_TAB_MOVE);
		}
	}
}
wxString zqAuiNotebook::zqAddPage(wxWindow* page,wxString& caption)
{
	PageItem item;
	item.window=page;
	strncpy(item.caption,caption.c_str(),MAX_CAPTION_LENGTH-1);
	PageArray::iterator it=find(m_pageArray.begin(),m_pageArray.end(),item);
	if(it==m_pageArray.end())
	{
		m_pageArray.push_back(item);
		return 	caption;	
	}
	return it->caption;
}
void zqAuiNotebook::SetSelection(int index)
{
	if(index>=0&&index<(int)GetPageCount())
	{
		wxAuiNotebook* pParent=wxDynamicCast(GetPageWindow(index)->GetParent(),wxAuiNotebook);
		if(pParent)
		{
			int id=pParent->GetPageIndex(GetPageWindow(index));
			pParent->SetSelection(id);
		}
	}
}
void zqAuiNotebook::SetSelection(wxWindow* win)
{
	wxAuiNotebook* pParent=wxDynamicCast(win->GetParent(),wxAuiNotebook);
	if(pParent)
	{
		int id=pParent->GetPageIndex(win);
		pParent->SetSelection(id);
	}
}
bool zqAuiNotebook::IsSelected(wxWindow* win)
{
	wxWindow* pPage=FindFocus();
    while(pPage)
	{
		if(pPage==win)
		{
			return true;
		}
		pPage=pPage->GetParent();
	}
	return false;
}

wxWindow* zqAuiNotebook::GetSelectWindow()
{
	for ( unsigned int i = 0; i < GetPageCount(); i++ )
	{
		if ( IsSelected( GetPageWindow(i)))
		{
			return GetPageWindow(i);
		}
	}

	return NULL;
}

void zqAuiNotebook::OnSize(wxSizeEvent& evt)
{
    static bool bLastMax=(::GetWindowLong((HWND)wxGetTopLevelParent(this)->GetHWND(),GWL_STYLE)&WS_MAXIMIZE)>0;
    bool bMax=(::GetWindowLong((HWND)wxGetTopLevelParent(this)->GetHWND(),GWL_STYLE)&WS_MAXIMIZE)>0;
    MySplitterWindow* pTopSplitter=GetAllSplitterWindow();
    if(pTopSplitter)
    {
        int x=evt.GetSize().x,y=evt.GetSize().y;
        if(x==0||y==0) return;
        pTopSplitter->SetSize(evt.GetSize());
        if(bLastMax!=bMax)
        {
            m_bNeedSetSashPosition=true;
            OnIdle(wxIdleEvent());
            bLastMax=bMax;
        }

    }
}
void zqAuiNotebook::OnIdle(wxIdleEvent& event)
{
	if(m_bNeedSetSashPosition)
	{
		m_bNeedSetSashPosition=false;
		vector<MySplitterWindow*> AllSplitter;
		MySplitterWindow* pTop=GetAllSplitterWindow(&AllSplitter);
		if(pTop)
		{
			for (size_t i=0;i<AllSplitter.size();++i)
			{
				if(!AllSplitter[i]->m_fixed)
				{
					if(AllSplitter[i]->GetSplitMode()==wxSPLIT_HORIZONTAL)
					{
						int nPos = AllSplitter[i]->m_fHoldingSashPosition*AllSplitter[i]->GetClientSize().GetHeight();
						AllSplitter[i]->SetSashPosition(nPos);
					}
					else
					{
						int nPos = AllSplitter[i]->m_fHoldingSashPosition*AllSplitter[i]->GetClientSize().GetWidth();
						AllSplitter[i]->SetSashPosition(nPos);
					}
				}
			}
			pTop->Show();
		}
	}
}
int  zqAuiNotebook::GetPageIndex(wxWindow* page)
{
	for(int i=0;i<(int)GetPageCount();i++)
	{
		if(GetPageWindow(i)==page)
		{
			return i;
		}
	}
	return -1;
}
wxWindow* zqAuiNotebook::GetPageWindowFromId(int id)
{
	for(int i=0;i<(int)GetPageCount();i++)
	{
		wxWindow* win=GetPageWindow(i);
		if(win->GetId()==id)
		{
			return win;
		}
	}
	return NULL;
}
wxString zqAuiNotebook::GetPageCaptionFromId(int id)
{
	for(int i=0;i<(int)GetPageCount();i++)
	{
		if(GetPageWindow(i)->GetId()==id)
		{
			return GetPageCaption(i);
		}
	}
	return wxEmptyString;
}
bool zqAuiNotebook::IsFloatPaneOpen(wxWindow* page)
{
	wxWindow* pParent=wxGetTopLevelParent(page);
	return pParent->IsKindOf(CLASSINFO(wxMiniFrame));
}
bool zqAuiNotebook::DoFloatPaneOpen(wxWindow* page,wxRect& rect)
{
	bool bret=true;
	wxWindow* child=page;
	wxWindow* pParent=child->GetParent();
	wxAuiNotebook* pNotebook=wxDynamicCast(pParent,wxAuiNotebook);
	int pageindex=0;
	if(pNotebook)
	{
		child=pNotebook;
		pParent=pNotebook->GetParent();
		if(wxDynamicCast(pParent,wxMiniFrame))
			return false;
		pageindex=pNotebook->GetPageIndex(page);
		if(pageindex>=0)
		{
			pNotebook->RemovePage(pageindex);
			pNotebook->RemoveEmptyTabFrames();
		}
	}
	wxSplitterWindow* pSplitter=wxDynamicCast(pParent,wxSplitterWindow);
	if(pSplitter)
	{
		wxString strCaption=GetPageCaption(GetPageIndex(page));
		wxPoint pt=rect.GetLeftTop();
		if(pt==wxPoint()) pt=wxDefaultPosition;
		wxSize  sz=rect.GetSize();
		if(sz==wxSize()) sz=wxDefaultSize;
		MyMiniFrame* pNewParent=new MyMiniFrame(pSplitter,wxID_ANY,strCaption,pt,sz);	
        int FloatPaneIsWindowOne=-1;
        if(pSplitter->GetWindow2())
            FloatPaneIsWindowOne=(pSplitter->GetWindow1()==child)?1:0;

		int FloatPaneSashPosition=pSplitter->GetSashPosition();
		if(pNotebook->GetPageCount()==0)
		{
			bret=pSplitter->Unsplit(child);         
			pNewParent->SetFloatPanePos(FloatPaneIsWindowOne,FloatPaneSashPosition, 0);
		}
		else
		{
			pNotebook->SetSelection(0);
			pNewParent->SetFloatPanePos(FloatPaneIsWindowOne,FloatPaneSashPosition, pageindex);
		}
		if(bret)
		{
			page->Reparent(pNewParent);
			pNewParent->Show();
			page->Show();
		}
	}
	return bret;

}
bool zqAuiNotebook::DoFloatPaneClose(wxWindow* page)
{
	wxWindow* child=page;
	wxWindow* pParent=child->GetParent();
	wxAuiNotebook* pNotebook=wxDynamicCast(pParent,wxAuiNotebook);
	int pageindex=0;
	if(pNotebook)
	{
		child=pNotebook;
		pParent=pNotebook->GetParent();
	}
	wxMiniFrame* pMiniFrame=wxDynamicCast(pParent,wxMiniFrame);
	wxCloseEvent evt(wxEVT_CLOSE_WINDOW);
	if(pMiniFrame) pMiniFrame->ProcessEvent(evt);
	return pMiniFrame!=NULL;
}
bool zqAuiNotebook::DoConfigPaneOpen(wxWindow* page)
{
	if(GetLayoutStyle()!=STYLE_SunGard)
		return false;
	MySplitterWindow* pTop=GetAllSplitterWindow(&m_allBeforeSplitter);
	if(pTop==NULL||m_allBeforeSplitter.size()<2) 
		return false;
	if(page==pTop->GetWindow1())
		return false;
	wxWindow* win2=pTop->GetWindow2();
	bool bSul=false;
	if(m_allBeforeSplitter[1]==win2)
	{
		//pTop->SetMinimumPaneSize(300);
		m_allBeforeSplitter[1]->Hide();//第一次由通常布局变成设置布局界面
	}
	else
		delete win2;//当前就是设置布局界面，更换页面
	page->Reparent(pTop); 
	bSul=pTop->ReplaceWindow(win2,page);
    if(bSul) m_IsConfigpaneOpen=true;
	return bSul;

}
bool zqAuiNotebook::IsConfigPaneOpen()
{
	return m_IsConfigpaneOpen;
}
bool zqAuiNotebook::DoConfigPaneClose()
{
	if(GetLayoutStyle()!=STYLE_SunGard)
		return false;
	MySplitterWindow* pTop=GetAllSplitterWindow();
	if(pTop==NULL||m_allBeforeSplitter.size()<2) 
		return false;
	wxWindow* win2=pTop->GetWindow2();
	if(win2==m_allBeforeSplitter[1]) return false;
	delete win2;
	m_allBeforeSplitter[1]->Show();
	bool bSul=pTop->ReplaceWindow(win2,m_allBeforeSplitter[1]);
	pTop->SetMinimumPaneSize(SPLITWINDOW_MIN_WIDTH);
	if(bSul) m_IsConfigpaneOpen=false;
	return bSul;
}
void zqAuiNotebook::DeletePageAndNotebook(wxAuiNotebook* pSrcNotebook)
{
	wxSplitterWindow* pParent=wxDynamicCast(pSrcNotebook->GetParent(),wxSplitterWindow);
	if(pParent&&pParent->GetParent())
	{
		wxSplitterWindow* pGrand=wxDynamicCast(pParent->GetParent(),wxSplitterWindow);
		wxWindow* win1=pParent->GetWindow1();
		wxWindow* win2=pParent->GetWindow2();
		if(pGrand)//分割视图
		{
			if(win1==pSrcNotebook)
			{
				win2->Reparent(pGrand);
				pGrand->ReplaceWindow(pParent,win2);
				delete pParent;
			}
			if(win2==pSrcNotebook)
			{
				win1->Reparent(pGrand);
				pGrand->ReplaceWindow(pParent,win1);
				delete pParent;
			}
		}
		else
		{
			pParent->Unsplit(pSrcNotebook);
			delete pSrcNotebook;
		}
	}
}
void zqAuiNotebook::OnDragDone(wxAuiNotebookEvent& event)
{

	WXLOG_INFO("ID=%d,Object=0x%08X,Sel=%d,OldSel=%d",
		event.GetId(),
		(DWORD)event.GetEventObject(),
		event.GetSelection(),
		event.GetOldSelection());
	wxAuiNotebook* pSrcNotebook=wxDynamicCast(event.GetEventObject(),wxAuiNotebook);
	if ( NULL == pSrcNotebook )
	{
		return;
	}

	wxWindow* pDragWindow = GetSelectWindow();
	if ( NULL == pDragWindow )
	{
		return;
	}

	m_bStyleChange = true;
	int oldsel=event.GetOldSelection();
	if(oldsel<0||oldsel>=(int)pSrcNotebook->GetPageCount()) oldsel=0;
	pSrcNotebook->SetSelection(oldsel);
	//如果拖动的是下单板，则清除原来窗口的最小尺寸限制，并设置当前所在窗口的最小尺寸
	if ( GID_ORDER_INSERT_PANEL == pDragWindow->GetId())
	{
		pSrcNotebook->SetMinSize(wxSize(0, 0));
		MySplitterWindow* pSplit = wxDynamicCast(pSrcNotebook->GetParent(), MySplitterWindow);
		if( NULL != pSplit)
		{
			pSplit->SetMinSize(wxSize(0, 0));
			pSplit->SetMinimumPaneSize(SPLITWINDOW_MIN_WIDTH);
		}
	}
	if(pSrcNotebook->GetPageCount()==0)
	{
		DeletePageAndNotebook(pSrcNotebook);
	}

	//如果拖动的是下单板，则设置所在窗口的最小尺寸限制
	if ( GID_ORDER_INSERT_PANEL == pDragWindow->GetId())
	{
		wxAuiNotebook* pTargetNotbook = wxDynamicCast(pDragWindow->GetParent(), wxAuiNotebook);
		if ( NULL != pTargetNotbook)
		{
			pTargetNotbook->SetMinSize(ORDERPANEL_MIN_SIZE);

			MySplitterWindow* pSplit = wxDynamicCast(pTargetNotbook->GetParent(), MySplitterWindow);
			if ( NULL != pSplit)
			{
				pSplit->SetMinSize(ORDERPANEL_MIN_SIZE);
			}
		}
	}

		//wxSplitterWindow* pParent=wxDynamicCast(pSrcNotebook->GetParent(),wxSplitterWindow);
		//if(pParent&&pParent->GetParent())
		//{
		//	
			//wxSplitterWindow* pGrand=wxDynamicCast(pParent->GetParent(),wxSplitterWindow);
			//wxWindow* win1=pParent->GetWindow1();
			//wxWindow* win2=pParent->GetWindow2();
			//if(pGrand)//分割视图
			//{
			//	if(win1==pSrcNotebook)
			//	{
			//		win2->Reparent(pGrand);
			//		pGrand->ReplaceWindow(pParent,win2);
			//		delete pParent;
			//	}
			//	if(win2==pSrcNotebook)
			//	{
			//		win1->Reparent(pGrand);
			//		pGrand->ReplaceWindow(pParent,win1);
			//		delete pParent;
			//	}
			//}
			//else
			//{
			//	pParent->Unsplit(pSrcNotebook);
			//	delete pSrcNotebook;
			//}
	//}

	//}
	//if(!m_IsConfigpaneOpen)
	//{
	//	//改变风格
	//	int newstyle=CalcLayoutStyle(false);
	//	if(newstyle!=m_layout_style)
	//	{
	//		m_layout_style=newstyle;
	//		m_ChangeStyleFunc();
	//	}
	//}

}
void zqAuiNotebook::EnumSplitter(wxSplitterWindow* pSplitter)
{
	MySplitterWindow* pSplitter1=wxDynamicCast(pSplitter->GetWindow1(),MySplitterWindow);
	if(pSplitter1)
	{
		EnumSplitter(pSplitter1);
	}
	MySplitterWindow* pSplitter2=wxDynamicCast(pSplitter->GetWindow2(),MySplitterWindow);
	if(pSplitter2)
	{
		EnumSplitter(pSplitter2);
	}
}
bool zqAuiNotebook::LayoutStyleIsSunGard()
{
	MySplitterWindow* pTop=GetAllSplitterWindow();
    if(pTop&&pTop->GetSplitMode()==wxSPLIT_HORIZONTAL)
    {
        wxSplitterWindow* pParent1=wxDynamicCast(pTop->GetWindow1(),wxSplitterWindow);
        wxSplitterWindow* pParent2=wxDynamicCast(pTop->GetWindow2(),wxSplitterWindow);
        if(pParent1==0&&pParent2&&pParent2->GetSplitMode()==wxSPLIT_VERTICAL)
        {
            wxSplitterWindow* pChild1=wxDynamicCast(pParent2->GetWindow1(),wxSplitterWindow);
            wxSplitterWindow* pChild2=wxDynamicCast(pParent2->GetWindow2(),wxSplitterWindow);
            if(pChild1==0&&pChild2==0)
                return true;
        }
    }
    
	return false;
}
void zqAuiNotebook::OnAllowNotebookDnD(wxAuiNotebookEvent& evt)
{
	if(!g_bFixPane) 
      evt.Allow();
}
bool zqAuiNotebook::IsModulePage(wxWindow* win)
{
	if(win)
	{
		for(size_t i=0;i<GetPageCount();i++)
		{
			if(GetPageWindow(i)==win)
				return true;
		}
	}
	return false;
}
void zqAuiNotebook::SetCallbackFunc(CreatePaneFunc func1,ChangeStyleFunc func2)
{
	m_CreatePaneFunc=func1;
	m_ChangeStyleFunc=func2;
}
wxWindow* zqAuiNotebook::NewQuotePage(wxString caption,wxString AppID)
{
    return ((FrameBase*)GETTOPWINDOW())->Plugin_New_Del_Panel_QuotFrame(1);
}
wxWindow* zqAuiNotebook::NewBlankPage(int nPos,wxString caption,wxString AppID)
{
	MySplitterWindow* pTop=GetAllSplitterWindow();
	if(pTop==NULL) return NULL;
	if(nPos==POS_QUOTE)
		return NewQuotePage(caption,AppID);
	//非行情模块
	zqPanel* win=new zqPanel(this,wxID_ANY);
	win->SetAppID(AppID);
	caption=zqAddPage(win,caption);
	wxAuiNotebook* pBook=NULL;
	if(pTop->IsSplit())
	{
		if(nPos==POS_ANY)
		{
			wxSplitterWindow* pSplitter=pTop;
			do
			{
				pBook=wxDynamicCast(pSplitter->GetWindow1(),wxAuiNotebook);
				pSplitter=(wxSplitterWindow *)pSplitter->GetWindow1();
			}while(pBook==NULL);
			win->Reparent(pBook);
			pBook->AddPage(win,caption);
		}
		//else if(pos==POS_QUOTE)
		//{
		//	wxWindow* pParent=FindWindowById(GID_QUOT_TABLE,this)->GetParent();
		//	pBook=wxDynamicCast(pParent,wxAuiNotebook);	
		//	if(pBook)
		//	{
		//		win->Reparent(pBook);
		//		pBook->InsertPage(pBook->GetPageIndex(FindWindowById(GID_QUOT_TABLE,this))+1,win,caption);
		//	}
		//}
		else if(nPos>=POS_LEFT&&nPos<=POS_BOTTOM)
		{
			MySplitterWindow* pSplitter=new MySplitterWindow(this,wxID_ANY,wxDefaultPosition,GetClientSize());
			pTop->Reparent(pSplitter);
			pBook=new wxAuiNotebook(pSplitter,wxID_ANY,wxDefaultPosition,wxDefaultSize,TAB_STYLE);
			win->Reparent(pBook);
			pBook->AddPage(win,caption);
			switch(nPos)
			{
			case POS_LEFT:
				pSplitter->SplitVertically(pBook,pTop,100);
				break;
			case POS_RIGHT:
				pSplitter->SplitVertically(pTop,pBook,-100);
				break;
			case POS_TOP:
				pSplitter->SplitHorizontally(pBook,pTop,100);
				break;
			case POS_BOTTOM:
				pSplitter->SplitHorizontally(pTop,pBook,-100);
				break;
			default:
				break;
			}
		}
		else
		{
			wxASSERT(0);
		}
	}
	else
	{
		wxWindow* srcbook=pTop->GetWindow1();
		if(nPos==POS_ANY/*||pos==POS_QUOTE*/)
		{
			pBook=wxDynamicCast(srcbook,wxAuiNotebook);	
			if(pBook)
			{
				win->Reparent(pBook);
				/*if(pos==POS_NORMAL)*/
					pBook->AddPage(win,caption);
				/*else
					pBook->InsertPage(pBook->GetPageIndex(FindWindowById(GID_QUOT_TABLE,this))+1,win,caption);*/
			}
		}
		else if(nPos>=POS_LEFT&&nPos<=POS_BOTTOM)
		{
			pBook=new wxAuiNotebook(pTop,wxID_ANY,wxDefaultPosition,wxDefaultSize,TAB_STYLE);
			win->Reparent(pBook);
			pBook->AddPage(win,caption);
			switch(nPos)
			{
			case POS_LEFT:
				pTop->SplitVertically(pBook,srcbook);
				break; 
			case POS_RIGHT:
				pTop->SplitVertically(srcbook,pBook);
				break;
			case POS_TOP:
				pTop->SplitHorizontally(pBook,srcbook);
				break;
			case POS_BOTTOM:
				pTop->SplitHorizontally(srcbook,pBook);
				break;
			}
		}
		else
		{
			wxASSERT(0);
		}
	}

	return win;
}
bool zqAuiNotebook::DeleteQuotePage(wxString AppID)
{
    ((FrameBase*)GETTOPWINDOW())->Plugin_New_Del_Panel_QuotFrame(2);
    return true;
}
bool zqAuiNotebook::DeleteBlankPage(int nPos, wxString AppID)
{
	if(nPos==POS_QUOTE)
		return DeleteQuotePage(AppID);
	zqPanel* p=NULL;
	wxAuiNotebook* notebook=NULL;
	for(size_t i=0;i<GetPageCount();i++)
	{
		p=wxDynamicCast(GetPageWindow(i),zqPanel);
		if(p&&p->GetAppID()==AppID)//找到面板
		{
			notebook=wxDynamicCast(p->GetParent(),wxAuiNotebook);
			if(notebook)
			{
				if(notebook->GetPageCount()>1)
					notebook->DeletePage(notebook->GetPageIndex(p));
				else
				{
					DeletePageAndNotebook(notebook);
				}
				//删除列表中项
				PageArray::iterator it=m_pageArray.begin();
				while(it!=m_pageArray.end())
				{
					if(it->window==p)
					{
						m_pageArray.erase(it);
						break;
					}
					++it;
				}
			}

		}
	}
	if(p==NULL) return false;

	return true;
}