#ifndef __EXTLISTCTRLH_G__
#define __EXTLISTCTRLH_G__


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "listctrl.h"
#endif

#include "wx/defs.h"
#include "wx/object.h"
#ifdef __WXMAC__
#include "wx/imaglist.h"
#else
#include "wx/generic/imaglist.h"
#endif
#include "..\\Module-Misc2\\globalDefines.h"
#include "wx/control.h"
#include "wx/timer.h"
#include "wx/dcclient.h"
#include "wx/scrolwin.h"
#include "wx/settings.h"
#include "extlistbase.h"
#include <vector>
#include <map>

#if wxUSE_DRAG_AND_DROP
class WXDLLEXPORT wxDropTarget;
#endif

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListItem;
class WXDLLEXPORT wxExtListEvent;

#if !defined(__WXMSW__) || defined(__WXUNIVERSAL__)
class WXDLLEXPORT wxExtListCtrl;
#define wxImageListType wxImageList
#else
#define wxImageListType wxImageList
#endif

//-----------------------------------------------------------------------------
// internal classes
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListHeaderData;
class WXDLLEXPORT wxExtListItemData;
class WXDLLEXPORT wxExtListLineData;

class WXDLLEXPORT wxExtListHeaderWindow;
class WXDLLEXPORT wxExtListMainWindow;

class WXDLLEXPORT wxExtListRenameTimer;
class WXDLLEXPORT wxExtListTextCtrl;

class wxTextCtrl;
//-----------------------------------------------------------------------------
// wxExtListCtrl
//-----------------------------------------------------------------------------
#define wxLIST_VIEW (wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_HRULES|wxLCEXT_SINGLE_SEL|wxLCEXT_TOGGLE_COLOUR)
class WXDLLEXPORT wxExtGenericListCtrl: public wxControl
{
public:
    void OnPaint(wxPaintEvent& evt);
public:
    wxExtGenericListCtrl();
    wxExtGenericListCtrl( wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = wxLCEXT_ICON,
                const wxValidator& validator = wxDefaultValidator,
		const wxString &name = wxEmptyString)
    {
        Create(parent, winid, pos, size, style, validator, name);
    }
    ~wxExtGenericListCtrl();
    void OnContextMenu(wxContextMenuEvent& event);
    void OnColumnRightClick(wxExtListEvent& event);
    //void ShowContextMenu(int style);
    bool Create( wxWindow *parent,
                 wxWindowID winid = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = wxLCEXT_ICON,
                 const wxValidator& validator = wxDefaultValidator,
		 const wxString &name = wxEmptyString);

    bool GetColumn( int col, wxExtListItem& item ) const;
    bool SetColumn( int col, wxExtListItem& item );
    bool SetColumn( int col,  const wxString& label, int imageId = -1 );
    int GetColumnWidth( int col ) const;
    bool SetColumnWidth( int col, int width);
    int GetColumnCtrlType( int col ) const;
    void SetColumnCtrlType( int col, int t);
    int GetCountPerPage() const; // not the same in wxGLC as in Windows, I think
    wxRect GetViewRect() const;
    void SetItemChecked(int id,bool check);
    bool GetItemChecked(int id);
    bool GetItem( wxExtListItem& info ) const;
    bool SetItem( wxExtListItem& info ) ;
    //同时更改多个项, 一般用于修改同一行的多个列
    bool SetItems( std::vector<wxExtListItem*>& Items, const bool bRefreshUI);

    wxString GetSubItemText(int index, int col);
    long SetItem( long index, int col, const wxString& label, int imageId = -1 );
    int  GetItemState( long item, long stateMask ) const;
    bool SetItemState( long item, long state, long stateMask);
    bool SetItemImage( long item, int image, int selImage = -1 );
    void SetOddRowColour(wxColour clr);
    wxColour GetOddRowColour();
    void SetEvenRowColour(wxColour clr);
    wxColour GetEvenRowColour();
#if wxABI_VERSION >= 20603
    bool SetItemColumnImage( long item, long column, int image );
#endif
    wxString GetItemText( long item ) const;
    void SetItemText( long item, const wxString& str );
    wxUIntPtr GetItemData( long item ) const;
    bool SetItemData( long item, long data );
    bool GetItemRect( long item, wxRect& rect, int code = wxLISTEXT_RECT_BOUNDS ) const;
    bool GetItemPosition( long item, wxPoint& pos ) const;
    bool SetItemPosition( long item, const wxPoint& pos ); // not supported in wxGLC
    int GetItemCount() const;
    int GetColumnCount() const;
    void SetItemSpacing( int spacing, bool isSmall = false );
    wxSize GetItemSpacing() const;
    void SetItemTextColour( long item, const wxColour& col);
    wxColour GetItemTextColour( long item ) const;
    void SetItemBackgroundColour( long item, const wxColour &col);
    wxColour GetItemBackgroundColour( long item ) const;
    void SetHeaderBackgroundColour( const wxColour &col );
    wxColour GetHeaderBackgroundColour() const;
    void GetSubItemRect( long index,int col, wxRect &rect ) const;
#if wxABI_VERSION >= 20602
    void SetItemFont( long item, const wxFont &f);
    wxFont GetItemFont( long item ) const;
#endif
    int GetSelectedItemCount() const;
    bool IsSelectedItem(size_t line) const;
    wxColour GetTextColour() const;
    void SetTextColour(const wxColour& col);
    long GetTopItem() const;
    void SetTextCtrlMaxLength(int len);
    void SetSingleStyle( long style, bool add = true ) ;
    void SetWindowStyleFlag( long style );
    void RecreateWindow() {}
    long GetNextItem( long item, int geometry = wxLISTEXT_NEXT_ALL, int state = wxLISTEXT_STATE_DONTCARE ) const;
    wxImageListType *GetImageList( int which ) const;
    void SetImageList( wxImageListType *imageList, int which );
    void AssignImageList( wxImageListType *imageList, int which );
    bool Arrange( int flag = wxLISTEXT_ALIGN_DEFAULT ); // always wxLISTEXT_ALIGN_LEFT in wxGLC

    void ClearAll();
    bool DeleteItem( long item );
    bool DeleteAllItems();
    bool DeleteAllColumns();
    bool DeleteColumn( int col );

    void SetItemCount(long count);

    void EditLabel( long item  ,long col) { Edit(item ,col); }
    void Edit( long item  ,long col);

    bool EnsureVisible( long item );
    long FindItem( long start, const wxString& str, bool partial = false );
    long FindItem( long start, wxUIntPtr data );
    //使用m_mapItemData2RowID来快速查找
    long FindItem2( wxUIntPtr data );
    long FindItem( long start, const wxPoint& pt, int direction ); // not supported in wxGLC
    long HitTest( const wxPoint& point, int& flags);
    long InsertItem(wxExtListItem& info);
    long InsertItem2(wxExtListItem& info, const bool bRefreshUI);
    long InsertItem( long index, const wxString& label );
    long InsertItem2( long index, const wxString& label, const long mdata, const bool bRefreshUI );
    long InsertItem( long index, int imageIndex );
    long InsertItem( long index, const wxString& label, int imageIndex );
    long InsertColumn( long col, wxExtListItem& info );
    long InsertColumn( long col, const wxString& heading,
                       int format = wxLISTEXT_FORMAT_LEFT, int width = -1 );
    bool ScrollList( int dx, int dy );
    bool SortItems( wxExtListCtrlCompare fn,int column, long data );
    bool Update( long item );
    
    // updated
    wxScrolledWindow * GetScrolledWin(void);

    // are we in report mode?
    bool InReportView() const { return HasFlag(wxLCEXT_REPORT); }

    // are we in virtual report mode?
    bool IsVirtual() const { return HasFlag(wxLCEXT_VIRTUAL); }

    // do we have a header window?
    bool HasHeader() const
        { return InReportView() && !HasFlag(wxLCEXT_NO_HEADER); }

    // refresh items selectively (only useful for virtual list controls)
    void RefreshItem(long item);
    void RefreshItems(long itemFrom, long itemTo);

	// resort all items
	void ReSortItems();

    // obsolete, don't use
    wxDEPRECATED( int GetItemSpacing( bool isSmall ) const );


    virtual wxVisualAttributes GetDefaultAttributes() const
    {
        return GetClassDefaultAttributes(GetWindowVariant());
    }

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);

    // implementation only from now on
    // -------------------------------

    void OnInternalIdle( );
    void OnSize( wxSizeEvent &event );

    // We have to hand down a few functions
    virtual void Refresh(bool eraseBackground = true,
                         const wxRect *rect = NULL);

    virtual void Freeze();
    virtual void Thaw();

    virtual bool SetBackgroundColour( const wxColour &colour );

    virtual bool SetForegroundColour( const wxColour &colour ); 
    virtual bool SetHeaderForegroundColour( const wxColour &colour );

    virtual wxColour GetForegroundColour() const;
    virtual wxColour GetHeaderForegroundColour() const;
   
    virtual wxColour GetBackgroundColour() const;


    virtual bool SetFont( const wxFont &font );
    virtual bool SetCursor( const wxCursor &cursor );
	virtual void ChangeRow(long row, bool isUp);
#if wxUSE_DRAG_AND_DROP
    virtual void SetDropTarget( wxDropTarget *dropTarget );
    virtual wxDropTarget *GetDropTarget() const;
#endif

    virtual bool DoPopupMenu( wxMenu *menu, int x, int y );

    virtual bool ShouldInheritColours() const { return false; }
    virtual void SetFocus();

    virtual wxSize DoGetBestSize() const;
    
    wxTextCtrl * GetEditControl(void);

    //获取排序的列序号，如果没有排序，返回-1
    int GetSortCol(void);
    //停止排序，将排序列改为-1
    void DisableSortCol(void);

	void AutoAdjustColumnWidth();

    //刷新表头
    void RefreshHeader(void);
    // implementation
    // --------------

    wxImageListType         *m_imageListNormal;
    wxImageListType         *m_imageListSmall;
    wxImageListType         *m_imageListState;  // what's that ?
    bool                 m_ownsImageListNormal,
                         m_ownsImageListSmall,
                         m_ownsImageListState;
    int                 m_bShowContextMenu;
    wxExtListHeaderWindow  *m_headerWin;
    wxExtListMainWindow    *m_mainWin;
    wxCoord              m_headerHeight;

protected:
    // return the text for the given column of the given item
    virtual wxString OnGetItemText(long item, long column) const;

    // return the icon for the given item
    virtual int OnGetItemImage(long item) const;

    // return the attribute for the item (may return NULL if none)
    virtual wxExtListItemAttr *OnGetItemAttr(long item) const;

    // it calls our OnGetXXX() functions
    friend class WXDLLEXPORT wxExtListMainWindow;

#if wxABI_VERSION >= 20603
    // take into account the coordinates difference between the container
    // window and the list control window itself here
    virtual void DoClientToScreen( int *x, int *y ) const;
    virtual void DoScreenToClient( int *x, int *y ) const;
#endif // 2.6.3

public:
    // Virtual function hiding supression
    virtual void Update() { wxWindow::Update(); }

private:
    // create the header window
    void CreateHeaderWindow();

    // calculate and set height of the header
    void CalculateAndSetHeaderHeight();

    // reposition the header and the main window in the report view depending
    // on whether it should be shown or not
    void ResizeReportView(bool showHeader);

	wxString GetColumnLongestText(long nCol );

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxExtGenericListCtrl)
};

//#if !defined(__WXMSW__) || defined(__WXUNIVERSAL__)
/*
 * wxExtListCtrl has to be a real class or we have problems with
 * the run-time information.
 */
// ----------------------------------------------------------------------------
// wxExtListView: a class which provides a better API for list control
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListCtrl_Obsolete : public wxExtGenericListCtrl
{
    DECLARE_DYNAMIC_CLASS(wxExtListCtrl_Obsolete)
public:
    wxExtListCtrl_Obsolete() { }
    wxExtListCtrl_Obsolete( wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxLCEXT_REPORT,
                const wxValidator& validator = wxDefaultValidator,
		const wxString &name = wxEmptyString)
    : wxExtGenericListCtrl(parent, winid, pos, size, style, validator, name)
    {
    }

    // focus/selection stuff
    // ---------------------

    // [de]select an item
    void Select(long n, bool on = true)
    {
        SetItemState(n, on ? wxLISTEXT_STATE_SELECTED : 0, wxLISTEXT_STATE_SELECTED);
    }

    // focus and show the given item
    void Focus(long index)
    {
        SetItemState(index, wxLISTEXT_STATE_FOCUSED, wxLISTEXT_STATE_FOCUSED);
        EnsureVisible(index);
    }

    // get the currently focused item or -1 if none
    long GetFocusedItem() const
    {
        return GetNextItem(-1, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_FOCUSED);
    }

    // get first and subsequent selected items, return -1 when no more
    long GetNextSelected(long item) const
        { return GetNextItem(item, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED); }
    long GetFirstSelected() const
        { return GetNextSelected(-1); }

    // return true if the item is selected
    bool IsSelected(long index)
        { return GetItemState(index, wxLISTEXT_STATE_SELECTED) != 0; }

    // columns
    // -------

    void SetColumnImage(int col, int image)
    {
        wxExtListItem item;
        item.SetMask(wxLISTEXT_MASK_IMAGE);
        item.SetImage(image);
        SetColumn(col, item);
    }

    void ClearColumnImage(int col) { SetColumnImage(col, -1); }

};

// ----------------------------------------------------------------------------
// wxGenericGrid
// ----------------------------------------------------------------------------
#include "wx/grid.h"
#include <map>
class wxGenericGrid : public wxGrid
{
	
public:
    wxGenericGrid():m_bIsAscending(true),m_nSortCol(-1){ }
	wxGenericGrid( wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxWANTS_CHARS,
		const wxValidator& validator = wxDefaultValidator,
		const wxString &name = wxEmptyString);
public:

	long GetNextSelected(long item) 
	{ return GetNextItem(item, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED); }

	long GetFirstSelected() 
	{ return GetNextSelected(-1); }

	void Select(long n, bool on = true)
	{
		SetItemState(n, on ? wxLISTEXT_STATE_SELECTED : 0, wxLISTEXT_STATE_SELECTED);
	}

	void Focus(long index)
	{
		SetItemState(index, wxLISTEXT_STATE_FOCUSED, wxLISTEXT_STATE_FOCUSED);
		//EnsureVisible(index);
	}

	bool GetColumn( int col, wxExtListItem& item ) ;
	bool SetColumn( int col, wxExtListItem& item );

	int  GetColumnWidth( int col ) ;
	bool SetColumnWidth( int col, int width);

	bool GetItemChecked(int id);
	void SetItemChecked(int id,bool check);

	bool GetItem( wxExtListItem& info ) ;
	long GetNextItem( long item, int geometry = wxLISTEXT_NEXT_ALL, int state = wxLISTEXT_STATE_DONTCARE ) ;
	bool SetItem( wxExtListItem& item ) ;
	long SetItem( long index, int col, const wxString& label, int imageId = -1 );

	bool SetItemState( long item, long state, long stateMask);

	int GetColumnCtrlType( int col );
	void SetColumnCtrlType( int col, int t);

	wxUIntPtr GetItemData( long item ) ;
	bool SetItemData( long item, long data );

	int GetItemCount() ;
	int GetColumnCount() ;
	int GetSelectedItemCount() const;

// 	wxColour GetItemBackgroundColour(long rowIndex);
// 	wxColour GetItemForegroundColour(long rowIndex);
	void SetItemBackgroundColour(long rowIndex, const wxColour &colColor);
//	void SetItemForegroundColour(long rowIndex, const wxColour &colColor);
	void SetHeaderBackgroundColour( const wxColour &colColor );
	bool SetHeaderForegroundColour( const wxColour &colour );
	void SetOddRowColour(wxColour clr);
	void SetEvenRowColour(wxColour clr);
// 	wxColour GetOddRowColour();
// 	wxColour GetEvenRowColour();
	virtual bool SetFont( const wxFont &font );
// 	const wxFont& GetFont(void) const;

	void ClearAll();
	bool DeleteItem( long item );
	bool DeleteAllItems();
	bool DeleteColumn( int col ); 
	bool DeleteAllColumns();

	long InsertItem(wxExtListItem& info);
	long InsertItem(long index, wxExtListItem& item);
	long InsertItem( long index, const wxString& label );
	long InsertItem( long index, const wxString& label, int imageIndex );
	long InsertColumn( long col, wxExtListItem& info );
	long InsertColumn( long col, const wxString& heading, int format = wxLISTEXT_FORMAT_LEFT, int width = -1 );

	bool IsSelectedItem(int line) ;
	wxString GetSubItemText(int index, int col);

	void SetWindowStyleFlag( long style );
	virtual bool SetBackgroundColour( const wxColour &colour );
	virtual bool SetForegroundColour( const wxColour &colour ); 
	virtual void Refresh(bool eraseBackground = true, const wxRect *rect = NULL);
	//virtual wxString GetColLabelValue( int col );

	bool IsSelected(long index)
	{ return GetItemState(index, wxLISTEXT_STATE_SELECTED) != 0; }

	wxString GetItemText(long item) ;
	void SetTextCtrlMaxLength(int len);
	long FindItem(long start, wxUIntPtr data);

public:
	void ChangeRow(long row, bool isUp);
	void GetItemRect( long index, wxRect &rect );
	void DrawColLabel( wxDC& dc, int col );

protected:
	void OnPaint( wxPaintEvent &event );
	void OnKeyDown(wxKeyEvent& event);
	void OnGridCellLeftClick(wxGridEvent& event);
	void OnGridCellRightClick(wxGridEvent& event);
	void OnGridCellLeftDClick(wxGridEvent& event);
	void OnGridLableLeftClick(wxGridEvent& event);
	void OnGridCellChange(wxGridEvent& event);

private:
	long GetItemState( long item, long stateMask ) ;

	bool IsVirtual() const { return HasFlag(wxLCEXT_VIRTUAL); }

	bool HasFlag(int flag) const { return wxGrid::HasFlag(flag); }
	
	bool InReportView() const { return HasFlag(wxLCEXT_REPORT); }

	wxExtListLineData * GetLine(size_t n) const;
	void CacheLineData(size_t line);
	void ClearData();
	bool SendNotify( size_t line,
		size_t column,
		wxEventType command,
		wxPoint point = wxDefaultPosition);

	bool HighlightLine( size_t line, bool highlight );
	bool SortCol(long col, bool isAscending);
	bool IsWithCheckbox();

private:

	//行的相关数据
	std::map<long,long> m_mapItemData;			//每行的相关数据
	std::map<long,long> m_mapItemState;			//每行的状态，如焦点行、选中行等
	std::map<int,bool>  m_mapCheck;				//带checkbox的Grid每行的check状态

	//列的相关数据
	std::map<long,long> m_mapColumnCtrlType;	//每列的属性

	//是否升序
	bool m_bIsAscending;

	//排序列号
	int m_nSortCol;

	//int m_textctrl_MaxLength;

	DECLARE_DYNAMIC_CLASS(wxGenericGrid)
	DECLARE_EVENT_TABLE()
};
#ifdef _USE_GRID_LIST
class  wxExtListCtrl: public wxGenericGrid
{
public:
    wxExtListCtrl() { }
    wxExtListCtrl( wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxLCEXT_REPORT,
                const wxValidator& validator = wxDefaultValidator,
		const wxString &name = wxEmptyString)
    : wxGenericGrid(parent, winid, pos, size, style, validator, name)
    {
    }
    DECLARE_DYNAMIC_CLASS(wxExtListCtrl)
};
#else
class  wxExtListCtrl: public wxExtListCtrl_Obsolete
{
public:
    wxExtListCtrl() { }
    wxExtListCtrl( wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxLCEXT_REPORT,
                const wxValidator& validator = wxDefaultValidator,
		const wxString &name = wxEmptyString)
    : wxExtListCtrl_Obsolete(parent, winid, pos, size, style, validator, name)
    {
    }
    DECLARE_DYNAMIC_CLASS(wxExtListCtrl)
};
#endif
//#endif // !__WXMSW__ || __WXUNIVERSAL__

#endif // __LISTCTRLH_G__
