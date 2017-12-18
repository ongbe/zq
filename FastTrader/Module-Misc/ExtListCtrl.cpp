/////////////////////////////////////////////////////////////////////////////
// Name:        generic/listctrl.cpp
// Purpose:     generic implementation of wxExtListCtrl
// Author:      Robert Roebling
//              Vadim Zeitlin (virtual list control support)
// Id:          $Id: listctrl.cc,v 1.5 2006/11/06 13:25:50 cvs Exp $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/*
   TODO

   1. we need to implement searching/sorting for virtual controls somehow
  ?2. when changing selection the lines are refreshed twice
 */

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------




#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "listctrl.h"
    #pragma implementation "listctrlbase.h"
#endif
#include "stdafx.h"
// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include "wx/log.h"
#include "tools_tools.h"
#include "Tools_Ansi.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif


#ifndef WX_PRECOMP
    #include "wx/app.h"

    #include "wx/dynarray.h"

    #include "wx/dcscreen.h"

    #include "wx/textctrl.h"
#endif

// under Win32 we always use the native version and also may use the generic
// one, however some things should be done only if we use only the generic
// version
#if defined(__WIN32__) && !defined(__WXUNIVERSAL__)
    #define HAVE_NATIVE_LISTCTRL
#endif

// if we have the native control, wx/listctrl.h declares it and not this one
#ifndef HAVE_NATIVE_LISTCTRL
#include "extgenericlistctrl.h"
#else // !HAVE_NATIVE_LISTCTRL
#include "extlistctrl.h"
//#include "extlistview.h"

    // if we have a native version, its implementation file does all this
    IMPLEMENT_DYNAMIC_CLASS(wxExtListItem, wxObject)
    //IMPLEMENT_DYNAMIC_CLASS(wxExtListView, wxExtListCtrl)
    IMPLEMENT_DYNAMIC_CLASS(wxExtListEvent, wxNotifyEvent)

    IMPLEMENT_DYNAMIC_CLASS(wxExtListCtrl_Obsolete, wxExtGenericListCtrl)
#endif // HAVE_NATIVE_LISTCTRL/!HAVE_NATIVE_LISTCTRL

#include "wx/selstore.h"
#include "wx/renderer.h"
#include "wx/math.h"
#include <list>

#ifdef __WXMAC__
    #include "wx/mac/private.h"
#endif

#pragma warning( disable : 4996 )


// NOTE: If using the wxExtListBox visual attributes works everywhere then this can
// be removed, as well as the #else case below.
#define _USE_VISATTR 0


// ----------------------------------------------------------------------------
// events
// ----------------------------------------------------------------------------

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_BEGIN_DRAG)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_BEGIN_RDRAG)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_BEGIN_LABEL_EDIT)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_END_LABEL_EDIT)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_DELETE_ITEM)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_DELETE_ALL_ITEMS)
#if WXWIN_COMPATIBILITY_2_4
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_GET_INFO)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_SET_INFO)
#endif
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_DESELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_KEY_DOWN)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_INSERT_ITEM)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_RIGHT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_BEGIN_DRAG)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_DRAGGING)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_END_DRAG)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_RIGHT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_LEFT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_MIDDLE_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_ACTIVATED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_FOCUSED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_CACHE_HINT)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_CONTEXT_MENU)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_CHECKBOX_CHANGED)
// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// // the height of the header window (FIXME: should depend on its font!)
// static const int HEADER_HEIGHT = 23;

static const int SCROLL_UNIT_X = 15;

// the spacing between the lines (in report mode)
static const int LINE_SPACING = 0;

// extra margins around the text label
static const int EXTRA_WIDTH = 4;
static const int EXTRA_HEIGHT = 4;

// margin between the window and the items
static const int EXTRA_BORDER_X = 2;
static const int EXTRA_BORDER_Y = 2;

// offset for the header window
static const int HEADER_OFFSET_X = 1;
static const int HEADER_OFFSET_Y = 1;

// margin between rows of icons in [small] icon view
static const int MARGIN_BETWEEN_ROWS = 6;

// when autosizing the columns, add some slack
static const int AUTOSIZE_COL_MARGIN = 10;

// default and minimal widths for the header columns
static const int WIDTH_COL_DEFAULT = 80;
static const int WIDTH_COL_MIN = 10;

// the space between the image and the text in the report mode
static const int IMAGE_MARGIN_IN_REPORT_MODE = 5;

const int COL_LEFT_MARGIN_WIDTH = 10;
const int COL_RIGHT_MARGIN_WIDTH = 10;

// ============================================================================
// private classes
// ============================================================================


//-----------------------------------------------------------------------------
//  wxColWidthInfo (internal)
//-----------------------------------------------------------------------------

struct wxColWidthInfo
{
    int     nMaxWidth;
    bool    bNeedsUpdate;   //  only set to true when an item whose
                            //  width == nMaxWidth is removed

    wxColWidthInfo(int w = 0, bool needsUpdate = false)
    {
        nMaxWidth = w;
        bNeedsUpdate = needsUpdate;
    }
};

WX_DEFINE_ARRAY_PTR(wxColWidthInfo *, ColWidthArray);

//-----------------------------------------------------------------------------
//  wxExtListItemData (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListItemData
{
public:
    wxExtListItemData(wxExtListMainWindow *owner);
    ~wxExtListItemData();

    void SetItem( const wxExtListItem &info );
    void SetImage( int image ) { m_image = image; }
    void SetData( wxUIntPtr data ) { m_data = data; }
	void SetColData( long data ) { m_colData = data; }
    //下面两个函数用于排序比较。 added by l. 20120426
    void SetiColData(int data)
        {   m_icolData = data;  if(!m_icolDataValid) m_icolDataValid=true; }
    void SetdColData(double data)
        {   m_dcolData = data;  if(!m_dcolDataValid) m_dcolDataValid=true; }
    void SetbtrynumericAsComparing(bool btrynumericAsComparing)
        {  m_btrynumericAsComparing = btrynumericAsComparing; }

    void SetPosition( int x, int y );
    void SetSize( int width, int height );

    bool HasText() const { return !m_text.empty(); }

    const wxString& GetText() const { return m_text; }
    const wxColor& GetColor(void) const { return m_color; }
    const wxFont& GetFont(void) const { return m_font; }

    void SetText(const wxString& text) { m_text = text; }
    void SetColor(const wxColor& color) { m_hascolor = true; m_color = color; }
    void SetFont(const wxFont& font) { m_hasFont = true; m_font = font; }

    // we can't use empty string for measuring the string width/height, so
    // always return something
    wxString GetTextForMeasuring() const
    {
        wxString s = GetText();
        if ( s.empty() )
            s = _T('H');

        return s;
    }

    bool IsHit( int x, int y ) const;

    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;

    int GetImage() const { return m_image; }
    bool HasImage() const { return GetImage() != -1; }

    void GetItem( wxExtListItem &info ) const;
    

    void SetAttr(wxExtListItemAttr *attr) { m_attr = attr; }
    wxExtListItemAttr *GetAttr() const { return m_attr; }
    
    bool HasColor(void) { return m_hascolor; }
    bool HasFont(void) { return m_hasFont; }

public:
    // the item image or -1
    int m_image;

    // user data associated with the item
    wxUIntPtr m_data;

	// column data for sort
	long m_colData;

    int             m_icolData; // 指定Item对应的Int值，用于排序比较。 added by l. 20120426
    double          m_dcolData; // 指定Item对应的double值，用于排序比较。 added by l. 20120426
    bool            m_icolDataValid;//m_icolData是否有效
    bool            m_dcolDataValid;//m_dcolData是否有效
    bool            m_btrynumericAsComparing;//比较时是否需要尝试数值的比较

    // the item coordinates are not used in report mode, instead this pointer
    // is NULL and the owner window is used to retrieve the item position and
    // size
    wxRect *m_rect;

    // the list ctrl we are in
    wxExtListMainWindow *m_owner;

    // custom attributes or NULL
    wxExtListItemAttr *m_attr;

protected:
    // common part of all ctors
    void Init();
    wxColor  m_color;
    bool     m_hascolor;
 
    wxString m_text;
    
    wxFont   m_font;
    bool     m_hasFont;
};

//-----------------------------------------------------------------------------
//  wxExtListHeaderData (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListHeaderData : public wxObject
{
public:
    wxExtListHeaderData();
    wxExtListHeaderData( const wxExtListItem &info );
    void SetItem( const wxExtListItem &item );
    void SetPosition( int x, int y );
    void SetWidth( int w );
    void SetFormat( int format );
    void SetHeight( int h );
    void SetCtrlType( int t);
    bool HasImage() const;

    bool HasText() const { return !m_text.empty(); }
    const wxString& GetText() const { return m_text; }
    void SetText(const wxString& text) { m_text = text; }

    void GetItem( wxExtListItem &item );

    bool IsHit( int x, int y ) const;
    int GetImage() const;
    int GetWidth() const;
    int GetFormat() const;
    int GetCtrlType() const;
protected:
    long      m_mask;
    int       m_image;
    wxString  m_text;
    int       m_format;
    int       m_width;
    int       m_xpos,
              m_ypos;
    int       m_height;
    int       m_ctrltype;
	long	  m_colData;
private:
    void Init();
};

//-----------------------------------------------------------------------------
//  wxExtListLineData (internal)
//-----------------------------------------------------------------------------

WX_DECLARE_LIST(wxExtListItemData, wxExtListItemDataList);
#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxExtListItemDataList);


class wxExtListLineData
{
public:
    // the list of subitems: only may have more than one item in report mode
    wxExtListItemDataList m_items;

    // this is not used in report view
    struct GeometryInfo
    {
        // total item rect
        wxRect m_rectAll;

        // label only
        wxRect m_rectLabel;

        // icon only
        wxRect m_rectIcon;

        // the part to be highlighted
        wxRect m_rectHighlight;

        // extend all our rects to be centered inside the one of given width
        void ExtendWidth(wxCoord w)
        {
            wxASSERT_MSG( m_rectAll.width <= w,
                            _T("width can only be increased") );

            m_rectAll.width = w;
            m_rectLabel.x = m_rectAll.x + (w - m_rectLabel.width)/2;
            m_rectIcon.x = m_rectAll.x + (w - m_rectIcon.width)/2;
            m_rectHighlight.x = m_rectAll.x + (w - m_rectHighlight.width)/2;
        }
    } *m_gi;

    // is this item selected? [NB: not used in virtual mode]
    bool m_highlighted;
    // back pointer to the list ctrl
    wxExtListMainWindow *m_owner;
    //checkbox selected?
    bool m_checkboxSelected;
public:
    wxExtListLineData(wxExtListMainWindow *owner);

    ~wxExtListLineData()
    {
        WX_CLEAR_LIST(wxExtListItemDataList, m_items);
        delete m_gi;
    }

    // are we in report mode?
    inline bool InReportView() const;

    // are we in virtual report mode?
    inline bool IsVirtual() const;

    // these 2 methods shouldn't be called for report view controls, in that
    // case we determine our position/size ourselves

    // calculate the size of the line
    void CalculateSize( wxDC *dc, int spacing );

    // remember the position this line appears at
    void SetPosition( int x, int y, int spacing );

    // wxExtListCtrl API

    void SetImage( int image ) { SetImage(0, image); }
    int GetImage() const { return GetImage(0); }
    bool HasImage() const { return GetImage() != -1; }
    bool HasText() const { return !GetText(0).empty(); }

    void SetItem( int index, const wxExtListItem &info );
    void GetItem( int index, wxExtListItem &info );

    wxString GetText(int index) const;
    void SetText( int index, const wxString s, int i );
    void SetColor( int index, const wxColor c );
    wxExtListItemAttr *GetAttr() const;
    void SetAttr(wxExtListItemAttr *attr);

    // return true if the highlighting really changed
    bool Highlight( bool on );

    void ReverseHighlight();

    bool IsHighlighted() const
    {
        wxASSERT_MSG( !IsVirtual(), _T("unexpected call to IsHighlighted") );

        return m_highlighted;
    }

    // draw the line on the given DC in icon/list mode
    void Draw( wxDC *dc );

    // the same in report mode
    void DrawInReportMode(wxDC *dc,
                           const wxRect& rect,
                           const wxRect& rectHL,
                           bool highlighted,
                           size_t lineID);

private:
    // set the line to contain num items (only can be > 1 in report mode)
    void InitItems( int num );

    // get the mode (i.e. style)  of the list control
    inline int GetMode() const;

    // prepare the DC for drawing with these item's attributes, return true if
    // we need to draw the items background to highlight it, false otherwise
    bool SetAttributes(wxDC *dc,
                       const wxExtListItemAttr *attr,
                       bool highlight,
                       size_t lineID=0);

    // draw the text on the DC with the correct justification; also add an
    // ellipsis if the text is too large to fit in the current width
    void DrawTextFormatted(wxDC *dc, const wxString &text, int col, int x, int y, int width);
	void DrawCheckBox(wxDC& dc,const wxRect& rect,bool bcheck);
    // these are only used by GetImage/SetImage above, we don't support images
    // with subitems at the public API level yet
    void SetImage( int index, int image );
    int GetImage( int index ) const;

public:

    int m_RowID;                //每行的行序号，在对行进行增加、删除、排序时维护
    int m_LineID;               //每行都有一个唯一的ID
    static int m_LineIDbase;    //每插入一行时此值加一

};

int wxExtListLineData::m_LineIDbase=0;

WX_DECLARE_EXPORTED_OBJARRAY(wxExtListLineData, wxExtListLineDataArray);
#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(wxExtListLineDataArray);

//-----------------------------------------------------------------------------
//  wxExtListHeaderWindow (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListHeaderWindow : public wxWindow
{
protected:
	wxExtListMainWindow  *m_owner;
	const wxCursor    *m_currentCursor;
	wxCursor          *m_resizeCursor;
	bool               m_isDragging;

    // column being resized or -1
    int m_column;

    // divider line position in logical (unscrolled) coords
    int m_currentX;

    // minimal position beyond which the divider line can't be dragged in
    // logical coords
    int m_minX;

public:
    wxExtListHeaderWindow();

    wxExtListHeaderWindow( wxWindow *win,
                        wxWindowID id,
                        wxExtListMainWindow *owner,
                        const wxPoint &pos = wxDefaultPosition,
                        const wxSize &size = wxDefaultSize,
                        long style = 0,
                        const wxString &name = wxT("wxExtListCtrlcolumntitles") );

    virtual ~wxExtListHeaderWindow();

    void DrawCurrent();
    void AdjustDC(wxDC& dc);

    void OnPaint( wxPaintEvent &event );
    void OnMouse( wxMouseEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    
    wxScrolledWindow *GetOwner(void) { return (wxScrolledWindow *) m_owner; }
    // needs refresh
    bool m_dirty;
    // background color
    wxColour m_backgroundcolor;

    bool m_bsort;
	int m_sort_column;
	bool m_sort_asc;
private:
    // common part of all ctors
    void Init();

    // generate and process the list event of the given type, return true if
    // it wasn't vetoed, i.e. if we should proceed
    bool SendListEvent(wxEventType type, wxPoint pos);

    DECLARE_DYNAMIC_CLASS(wxExtListHeaderWindow)
    DECLARE_EVENT_TABLE()
};

//-----------------------------------------------------------------------------
// wxExtListRenameTimer (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListRenameTimer: public wxTimer
{
private:
    wxExtListMainWindow *m_owner;

public:
    wxExtListRenameTimer( wxExtListMainWindow *owner );
    void Notify();
};

//-----------------------------------------------------------------------------
//  wxExtListTextCtrl (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListTextCtrl: public wxTextCtrl
{
public:
    wxExtListTextCtrl(wxExtListMainWindow *owner, size_t itemEdit,size_t columnEdit);

    void AcceptChangesAndFinish();
    void GetItemColumnID(size_t& item,size_t& column);
protected:
    void OnChar( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnKillFocus( wxFocusEvent &event );

    bool AcceptChanges();
    void Finish();

private:
    wxExtListMainWindow   *m_owner;
    wxString            m_startValue;
    size_t              m_itemEdited;
    size_t              m_columnEdited;
    bool                m_finished;
    bool                m_aboutToFinish;

    DECLARE_EVENT_TABLE()
};
//-----------------------------------------------------------------------------
//  wxExtListMainWindow (internal)
//-----------------------------------------------------------------------------

WX_DECLARE_LIST(wxExtListHeaderData, wxExtListHeaderDataList);
#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxExtListHeaderDataList);

class wxExtListMainWindow : public wxScrolledWindow
{
public:
    wxExtListMainWindow();
    wxExtListMainWindow( wxWindow *parent,
                      wxWindowID id,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = 0,
                      const wxString &name = _T("listctrlmainwindow") );

    virtual ~wxExtListMainWindow();

    wxWindow *GetMainWindowOfCompositeControl() { return GetParent(); }

    bool HasFlag(int flag) const { return m_parent->HasFlag(flag); }

    // return true if this is a virtual list control
    bool IsVirtual() const { return HasFlag(wxLCEXT_VIRTUAL); }

    // return true if the control is in report mode
    bool InReportView() const { return HasFlag(wxLCEXT_REPORT); }

    // return true if we are in single selection mode, false if multi sel
    bool IsSingleSel() const { return HasFlag(wxLCEXT_SINGLE_SEL); }

    // do we have a header window?
    bool HasHeader() const
        { return InReportView() && !HasFlag(wxLCEXT_NO_HEADER); }

    void HighlightAll( bool on );

    // all these functions only do something if the line is currently visible

    // change the line "selected" state, return true if it really changed
    bool HighlightLine( size_t line, bool highlight = true);

    // as HighlightLine() but do it for the range of lines: this is incredibly
    // more efficient for virtual list controls!
    //
    // NB: unlike HighlightLine() this one does refresh the lines on screen
    void HighlightLines( size_t lineFrom, size_t lineTo, bool on = true );

    // toggle the line state and refresh it
    void ReverseHighlight( size_t line )
        { HighlightLine(line, !IsHighlighted(line)); RefreshLine(line); }

    // return true if the line is highlighted
    bool IsHighlighted(size_t line) const;

    // refresh one or several lines at once
    void RefreshLine( size_t line );
    void RefreshLines( size_t lineFrom, size_t lineTo );

    // refresh all selected items
    void RefreshSelected();
	// resort all items
	void ReSortItems();

    // refresh all lines below the given one: the difference with
    // RefreshLines() is that the index here might not be a valid one (happens
    // when the last line is deleted)
    void RefreshAfter( size_t lineFrom );

    // the methods which are forwarded to wxExtListLineData itself in list/icon
    // modes but are here because the lines don't store their positions in the
    // report mode

    // get the bound rect for the entire line
    wxRect GetLineRect(size_t line) const;

    // get the bound rect of the label
    wxRect GetLineLabelRect(size_t line) const;

    // get the bound rect of the items icon (only may be called if we do have
    // an icon!)
    wxRect GetLineIconRect(size_t line) const;

    // get the rect to be highlighted when the item has focus
    wxRect GetLineHighlightRect(size_t line) const;

    // get the size of the total line rect
    wxSize GetLineSize(size_t line) const
        { return GetLineRect(line).GetSize(); }

    // return the hit code for the corresponding position (in this line)
    long HitTestLine(size_t line, int x, int y) const;

    // bring the selected item into view, scrolling to it if necessary
    void MoveToItem(size_t item);

    // bring the current item into view
    void MoveToFocus() { MoveToItem(m_current); }

    // start editing the label of the given item
    void EditLabel( long item ,long col);

    // suspend/resume redrawing the control
    void Freeze();
    void Thaw();

    void OnRenameTimer();
    bool OnRenameAccept(size_t itemEdit, size_t colEdit, const wxString& value);
    void OnRenameCancelled(size_t itemEdit);

    void OnMouse( wxMouseEvent &event );

    // called to switch the selection from the current item to newCurrent,
    void OnArrowChar( size_t newCurrent, const wxKeyEvent& event );

    void OnChar( wxKeyEvent &event );
    void OnKeyDown( wxKeyEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );
    void OnScroll(wxScrollWinEvent& event) ;

    void OnPaint( wxPaintEvent &event );

    void DrawImage( int index, wxDC *dc, int x, int y );
    void GetImageSize( int index, int &width, int &height ) const;
    int GetTextLength( const wxString &s ) const;

    void SetImageList( wxImageListType *imageList, int which );
    void SetItemSpacing( int spacing, bool isSmall = false );
    int GetItemSpacing( bool isSmall = false );

    void SetColumn( int col, wxExtListItem &item );
	bool SetColumn( int col, const wxString& label, int imageId = -1 );
    void SetColumnWidth( int col, int width );
    void GetColumn( int col, wxExtListItem &item ) const;
    int GetColumnWidth( int col ) const;
    int GetColumnCount() const { return m_columns.GetCount(); }
    int GetColumnCtrlType( int col ) const;
    void SetColumnCtrlType( int col, int t);
    // returns the sum of the heights of all columns
    int GetHeaderWidth() const;

    int GetCountPerPage() const;
    void SetOddRowColour(wxColour clr)
    {
        m_OddRowColour=clr;
    }
    wxColour GetOddRowColour()
    {
        return m_OddRowColour;
    }
    void SetEvenRowColour(wxColour clr)
    {
        m_EvenRowColour=clr;
    }
    wxColour GetEvenRowColour()
    {
        return m_EvenRowColour;
    }
    void SetItem( wxExtListItem &item );
    void SetItems( std::vector<wxExtListItem*>& items, const bool bRefreshUI);
    void GetItem( wxExtListItem &item ) const;
    void SetItemState( long item, long state, long stateMask );
    void SetItemStateAll( long state, long stateMask );
    int GetItemState( long item, long stateMask ) const;
    void GetItemRect( long index, wxRect &rect ) const;
    void GetSubItemRect( long index,int col, wxRect &rect ) const;
    wxRect GetViewRect() const;
    bool GetItemPosition( long item, wxPoint& pos ) const;
    int GetSelectedItemCount() const;
    void SetItemChecked(int id,bool check);
    bool GetItemChecked(int id);
    wxString GetItemText(long item) const
    {
        wxExtListItem info;
        info.m_mask = wxLISTEXT_MASK_TEXT;
        info.m_itemId = item;
        GetItem( info );
        return info.m_text;
    }

    void SetItemText(long item, const wxString& value)
    {
        wxExtListItem info;
        info.m_mask = wxLISTEXT_MASK_TEXT;
        info.m_itemId = item;
        info.m_text = value;
        SetItem( info );
    }

    // set the scrollbars and update the positions of the items
    void RecalculatePositions(bool noRefresh = false);

    // refresh the window and the header
    void RefreshAll();

    long GetNextItem( long item, int geometry, int state ) const;
    void DeleteItem( long index );
    void DeleteAllItems();
    void DeleteColumn( int col );
    void DeleteEverything();
    void EnsureVisible( long index );
    long FindItem( long start, const wxString& str, bool partial = false );
    long FindItem( long start, wxUIntPtr data);
    long FindItem( const wxPoint& pt );
    long HitTest( int x, int y, int &flags );
    void InsertItem( wxExtListItem &item );
    void InsertItem2( wxExtListItem &item, const bool bRefreshUI);
    void InsertColumn( long col, wxExtListItem &item );
    int GetItemWidthWithImage(wxExtListItem * item);
    void SortItems( wxExtListCtrlCompare fn,int column,  long data );
	void SetSortColData( long data );

    size_t GetItemCount() const;
    bool IsEmpty() const { return GetItemCount() == 0; }
    void SetItemCount(long count);

    // change the current (== focused) item, send a notification event
    void ChangeCurrent(size_t current);
    void ResetCurrent() { ChangeCurrent((size_t)-1); }
    bool HasCurrent() const { return m_current != (size_t)-1; }

    // send out a wxExtListEvent
    bool SendNotify( size_t line,
                     size_t column,
                     wxEventType command,
                     wxPoint point = wxDefaultPosition );

    // override base class virtual to reset m_lineHeight when the font changes
    virtual bool SetFont(const wxFont& font)
    {
        if ( !wxScrolledWindow::SetFont(font) )
            return false;

        m_lineHeight = 0;

        return true;
    }

    // these are for wxExtListLineData usage only

    // get the backpointer to the list ctrl
    wxExtGenericListCtrl *GetListCtrl() const
    {
        return wxStaticCast(GetParent(), wxExtGenericListCtrl);
    }

    // get the height of all lines (assuming they all do have the same height)
    wxCoord GetLineHeight() const;

    // get the y position of the given line (only for report view)
    wxCoord GetLineY(size_t line) const;

    // get the brush to use for the item highlighting
    wxBrush *GetHighlightBrush() const
    {
        return m_hasFocus ? m_highlightBrush : m_highlightUnfocusedBrush;
    }
    
    // get the editlabel
    wxTextCtrl * GetEditControl(void);
	void SetTextCtrlMaxLength(int len);
//protected:
    // the array of all line objects for a non virtual list control (for the
    // virtual list control we only ever use m_lines[0])
    wxExtListLineDataArray  m_lines;

    //放各行ItemData到行号的映射关系，在对行进行增、删、resort、SetItemData时更新。
    //用于FindItem2
    std::map<wxUIntPtr,int> m_mapItemData2RowID;
    //对列表行进行增、删、resort刷新全部map
    void RefreshmapItemData2RowID(void)
    {
        m_mapItemData2RowID.clear();
        int i,count=m_lines.size();
        for(i=0;i<count;i++)
        {
            if(m_lines[i].m_items.size()==0) continue;

            wxExtListItemDataList::compatibility_iterator node = m_lines[i].m_items.Item( 0 );
            if(!node) continue;

            wxExtListItemData *item = node->GetData();
            if(!item) continue;

            m_mapItemData2RowID[item->m_data]=i;
        }
    }
    //对列表进行InsertItem时更新map，执行完InsertItem后Update
    void UpdatemapItemData2RowID_AfterInsertItem(int InsertRowID)
    {
        std::map<wxUIntPtr,int>::iterator it;
        wxExtListItemDataList::compatibility_iterator node;
        wxExtListItemData *item;
        int i,count=m_lines.size();

        if(InsertRowID>=count)
            return;

        //先将m_mapItemData2RowID中InsertRowID+1以后的项的行号加一
        for(i=InsertRowID+1;i<count;i++)
        {
            if( m_lines[i].m_items.size()>0 &&
                (node = m_lines[i].m_items.Item( 0 ))!=NULL &&
                (item = node->GetData())!=NULL)
            {
                it=m_mapItemData2RowID.find(item->m_data);
                if(it!=m_mapItemData2RowID.end()&&it->second==i-1)
                    it->second=i;
            }
        }

        //设置插入行的对应关系
        if( m_lines[InsertRowID].m_items.size()>0 &&
            (node = m_lines[InsertRowID].m_items.Item( 0 ))!=NULL &&
            (item = node->GetData())!=NULL)
        {
            m_mapItemData2RowID[item->m_data]=InsertRowID;
        }
    }

    //对列表进行DeleteItem时更新map，执行DeleteItem前Update
    void UpdatemapItemData2RowID_BeforeDeleteItem(int DeleteRowID)
    {
        std::map<wxUIntPtr,int>::iterator it;
        wxExtListItemDataList::compatibility_iterator node;
        wxExtListItemData *item;
        int i,count=m_lines.size();

        if(DeleteRowID>=count)
            return;

        //删除欲删除行的对应关系
        if(m_lines[DeleteRowID].m_items.size()>0&&
            (node = m_lines[DeleteRowID].m_items.Item( 0 ))!=NULL&&
            (item = node->GetData())!=NULL)
        {
            it=m_mapItemData2RowID.find(item->m_data);
            if(it!=m_mapItemData2RowID.end()&&it->second==DeleteRowID)
                m_mapItemData2RowID.erase(item->m_data);
        }

        //将m_mapItemData2RowID中DeleteRowID+1以后的项的行号减一
        for(i=DeleteRowID+1;i<count;i++)
        {
            if( m_lines[i].m_items.size()>0 &&
                (node = m_lines[i].m_items.Item( 0 ))!=NULL &&
                (item = node->GetData())!=NULL)
            {
                it=m_mapItemData2RowID.find(item->m_data);
                if(it!=m_mapItemData2RowID.end()&&it->second==i)
                    it->second=i-1;
            }
        }
    }

    //对列表进行Resort后更新map
    void UpdatemapItemData2RowID_AfterResort()
    {
        std::map<wxUIntPtr,int>::iterator it;
        wxExtListItemDataList::compatibility_iterator node;
        wxExtListItemData *item;
        int i,count=m_lines.size();

        for(i=0;i<count;i++)
        {
            if( m_lines[i].m_items.size()>0 &&
                (node = m_lines[i].m_items.Item( 0 ))!=NULL &&
                (item = node->GetData())!=NULL)
            {
                it=m_mapItemData2RowID.find(item->m_data);
                if(it!=m_mapItemData2RowID.end()&&it->second!=i)
                    it->second=i;
            }
        }
    }

    //对列表进行SetItemData时更新map
    void UpdatemapItemData2RowID_SetItemData(int RowID,wxUIntPtr mdata)
    {
        m_mapItemData2RowID[mdata]=RowID;
    }
    //根据mdata查找行号，失败返回-1
    int FindItem2(wxUIntPtr mdata)
    {
        std::map<wxUIntPtr,int>::iterator it=m_mapItemData2RowID.find(mdata);
        if(it==m_mapItemData2RowID.end())
            return -1;
        else return it->second;
    }


    // the list of column objects
    wxExtListHeaderDataList m_columns;

    // currently focused item or -1
    size_t               m_current;
    // currently focused column or -1
    size_t               m_current_column;
    // the number of lines per page
    int                  m_linesPerPage;

    // this flag is set when something which should result in the window
    // redrawing happens (i.e. an item was added or deleted, or its appearance
    // changed) and OnPaint() doesn't redraw the window while it is set which
    // allows to minimize the number of repaintings when a lot of items are
    // being added. The real repainting occurs only after the next OnIdle()
    // call
    bool                 m_dirty;
//	bool				m_needReSort;
	long				m_bakSortColData;

    wxColour            *m_highlightColour;
    wxImageListType         *m_small_image_list;
    wxImageListType         *m_normal_image_list;
    int                  m_small_spacing;
    int                  m_normal_spacing;
    bool                 m_hasFocus;

    bool                 m_lastOnSame;
    wxTimer             *m_renameTimer;
    bool                 m_isCreated;
    int                  m_dragCount;
    wxPoint              m_dragStart;
    ColWidthArray        m_aColWidths;

    // for double click logic
    size_t m_lineLastClicked,
           m_lineBeforeLastClicked,
           m_lineSelectSingleOnUp;
	wxBitmap m_CheckBitmap,m_UncheckBitmap;
    wxExtListTextCtrl*     m_textctrl;
    int          m_textctrl_MaxLength;    
protected:
    // the total count of items in a virtual list control
    size_t m_countVirt;

    // the object maintaining the items selection state, only used in virtual
    // controls
    wxSelectionStore m_selStore;

    // common part of all ctors
    void Init();

    // get the line data for the given index
    wxExtListLineData *GetLine(size_t n) const
    {
        wxASSERT_MSG( n != (size_t)-1, _T("invalid line index") );

        if ( IsVirtual() )
        {
            wxConstCast(this, wxExtListMainWindow)->CacheLineData(n);

            n = 0;
        }

        return &m_lines[n];
    }

    // get a dummy line which can be used for geometry calculations and such:
    // you must use GetLine() if you want to really draw the line
    wxExtListLineData *GetDummyLine() const;

    // cache the line data of the n-th line in m_lines[0]
    void CacheLineData(size_t line);

    // get the range of visible lines
    void GetVisibleLinesRange(size_t *from, size_t *to);

    // force us to recalculate the range of visible lines
    void ResetVisibleLinesRange() { m_lineFrom = (size_t)-1; }

    // get the colour to be used for drawing the rules
    wxColour GetRuleColour() const
    {
        return wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT);
    }
    


private:
    // initialize the current item if needed
    void UpdateCurrent();

    // delete all items but don't refresh: called from dtor
    void DoDeleteAllItems();

    // the height of one line using the current font
    wxCoord m_lineHeight;

    // the total header width or 0 if not calculated yet
    wxCoord m_headerWidth;

    // the first and last lines being shown on screen right now (inclusive),
    // both may be -1 if they must be calculated so never access them directly:
    // use GetVisibleLinesRange() above instead
    size_t m_lineFrom,
           m_lineTo;

    // the brushes to use for item highlighting when we do/don't have focus
    wxBrush *m_highlightBrush,
            *m_highlightUnfocusedBrush;
    // if this is > 0, the control is frozen and doesn't redraw itself
    size_t m_freezeCount;
    wxColour m_OddRowColour;
    wxColour m_EvenRowColour;

	// Previous scrollbar position
    DECLARE_DYNAMIC_CLASS(wxExtListMainWindow)
    DECLARE_EVENT_TABLE()

    friend class wxExtGenericListCtrl;
};

// ============================================================================
// implementation
// ============================================================================

//-----------------------------------------------------------------------------
//  wxExtListItemData
//-----------------------------------------------------------------------------

wxExtListItemData::~wxExtListItemData()
{
    // in the virtual list control the attributes are managed by the main
    // program, so don't delete them
    if ( !m_owner->IsVirtual() )
    {
        delete m_attr;
    }

    delete m_rect;
}

void wxExtListItemData::Init()
{
    m_image = -1;
    m_data = 0;
	m_colData = -1;

    //下面四个变量用于排序比较。Added by l. 20120426
    m_icolData = 0; 
    m_dcolData = 0.0;
    m_icolDataValid = false;
    m_dcolDataValid = false;
    m_btrynumericAsComparing = false;

    m_color = wxColor(0,0,0);
    m_hascolor = false;
    m_hasFont = false;
    
    m_attr = NULL;
}

wxExtListItemData::wxExtListItemData(wxExtListMainWindow *owner)
{
    Init();

    m_owner = owner;

    if ( owner->InReportView() )
    {
        m_rect = NULL;
    }
    else
    {
        m_rect = new wxRect;
    }
}

void wxExtListItemData::SetItem( const wxExtListItem &info )
{
    if ( info.m_mask & wxLISTEXT_MASK_TEXT )
        SetText(info.m_text);
    
    if ( info.m_mask & wxLISTEXT_MASK_IMAGE )
        m_image = info.m_image;
    
    if ( info.m_mask & wxLISTEXT_MASK_DATA )
        m_data = info.m_data;
    
	m_colData = info.m_colData;

    //下面四个变量用于排序比较。Added by l. 20120426
    m_icolData = info.m_icolData; 
    m_dcolData = info.m_dcolData;
    m_icolDataValid = info.m_icolDataValid;
    m_dcolDataValid = info.m_dcolDataValid;
    m_btrynumericAsComparing = info.m_btrynumericAsComparing;

    if ( info.m_mask & wxLISTEXT_MASK_FONTCOLOR )
    {
	    SetColor(info.m_color);
    }
    
    if ( info.m_mask & wxLISTEXT_MASK_FONT )
    {
	    SetFont(info.m_font);
    }
    if ( info.HasAttributes() )
    {
        if ( m_attr )
            *m_attr = *info.GetAttributes();
        else
            m_attr = new wxExtListItemAttr(*info.GetAttributes());
    }

    if ( m_rect )
    {
        m_rect->x =
        m_rect->y =
        m_rect->height = 0;
        m_rect->width = info.m_width;
    }
}

void wxExtListItemData::SetPosition( int x, int y )
{
    wxCHECK_RET( m_rect, _T("unexpected SetPosition() call") );

    m_rect->x = x;
    m_rect->y = y;
}

void wxExtListItemData::SetSize( int width, int height )
{
    wxCHECK_RET( m_rect, _T("unexpected SetSize() call") );

    if ( width != -1 )
        m_rect->width = width;
    if ( height != -1 )
        m_rect->height = height;
}

bool wxExtListItemData::IsHit( int x, int y ) const
{
    wxCHECK_MSG( m_rect, false, _T("can't be called in this mode") );
#ifdef __WX_R2_6__
	return wxRect(GetX(), GetY(), GetWidth(), GetHeight()).Inside(x, y);
#else
	return wxRect(GetX(), GetY(), GetWidth(), GetHeight()).Contains(x, y);
#endif
}

int wxExtListItemData::GetX() const
{
    wxCHECK_MSG( m_rect, 0, _T("can't be called in this mode") );

    return m_rect->x;
}

int wxExtListItemData::GetY() const
{
    wxCHECK_MSG( m_rect, 0, _T("can't be called in this mode") );

    return m_rect->y;
}

int wxExtListItemData::GetWidth() const
{
    wxCHECK_MSG( m_rect, 0, _T("can't be called in this mode") );

    return m_rect->width;
}

int wxExtListItemData::GetHeight() const
{
    wxCHECK_MSG( m_rect, 0, _T("can't be called in this mode") );

    return m_rect->height;
}

void wxExtListItemData::GetItem( wxExtListItem &info ) const
{
    long mask = info.m_mask;
    if ( !mask )
    {
        // by default, get everything for backwards compatibility
        mask = -1;
    }

    if ( mask & wxLISTEXT_MASK_TEXT )
        info.m_text = m_text;
    if ( mask & wxLISTEXT_MASK_IMAGE )
        info.m_image = m_image;
    if ( mask & wxLISTEXT_MASK_DATA )
        info.m_data = m_data;

	info.m_colData = m_colData;

    //下面四个变量用于排序比较。Added by l. 20120426
    info.m_icolData = m_icolData; 
    info.m_dcolData = m_dcolData;
    info.m_icolDataValid = m_icolDataValid;
    info.m_dcolDataValid = m_dcolDataValid;
    info.m_btrynumericAsComparing = m_btrynumericAsComparing;

    if ( m_attr )
    {
        if ( m_attr->HasTextColour() )
            info.SetTextColour(m_attr->GetTextColour());
        if ( m_attr->HasBackgroundColour() )
            info.SetBackgroundColour(m_attr->GetBackgroundColour());
        if ( m_attr->HasFont() )
            info.SetFont(m_attr->GetFont());
    }
}

//-----------------------------------------------------------------------------
//  wxExtListHeaderData
//-----------------------------------------------------------------------------

void wxExtListHeaderData::Init()
{
    m_mask = 0;
    m_image = -1;
    m_format = 0;
    m_width = 0;
    m_xpos = 0;
    m_ypos = 0;
    m_height = 0;
	m_colData = -1;
    m_ctrltype=COLUMN_CTRL_TYPE_NORMAL;
}

wxExtListHeaderData::wxExtListHeaderData()
{
    Init();
}

wxExtListHeaderData::wxExtListHeaderData( const wxExtListItem &item )
{
    Init();

    SetItem( item );
}

void wxExtListHeaderData::SetItem( const wxExtListItem &item )
{
    m_mask = item.m_mask;
	m_colData = item.m_colData;

    if ( m_mask & wxLISTEXT_MASK_TEXT )
        m_text = item.m_text;

    if ( m_mask & wxLISTEXT_MASK_IMAGE )
        m_image = item.m_image;

    if ( m_mask & wxLISTEXT_MASK_FORMAT )
        m_format = item.m_format;

    if ( m_mask & wxLISTEXT_MASK_WIDTH )
        SetWidth(item.m_width);
    if ( m_mask & wxLISTEXT_MASK_CTRLTYPE)
        m_ctrltype=item.m_ctrltype;
}

void wxExtListHeaderData::SetPosition( int x, int y )
{
    m_xpos = x;
    m_ypos = y;
}

void wxExtListHeaderData::SetHeight( int h )
{
    m_height = h;
}

void wxExtListHeaderData::SetWidth( int w )
{
    m_width = w;
    if (m_width < 0)
        m_width = WIDTH_COL_DEFAULT;
    else if (m_width < WIDTH_COL_MIN)
        m_width = WIDTH_COL_MIN;
}

void wxExtListHeaderData::SetFormat( int format )
{
    m_format = format;
}
void wxExtListHeaderData::SetCtrlType( int t )
{
    m_ctrltype = t;
}
bool wxExtListHeaderData::HasImage() const
{
    return m_image != -1;
}

bool wxExtListHeaderData::IsHit( int x, int y ) const
{
    return ((x >= m_xpos) && (x <= m_xpos+m_width) && (y >= m_ypos) && (y <= m_ypos+m_height));
}

void wxExtListHeaderData::GetItem( wxExtListItem& item )
{
    item.m_mask = m_mask;
    item.m_text = m_text;
    item.m_image = m_image;
    item.m_format = m_format;
    item.m_width = m_width;
	item.m_colData = m_colData;
}

int wxExtListHeaderData::GetImage() const
{
    return m_image;
}

int wxExtListHeaderData::GetWidth() const
{
    return m_width;
}

int wxExtListHeaderData::GetFormat() const
{
    return m_format;
}
int wxExtListHeaderData::GetCtrlType() const
{
    return m_ctrltype;
}
//-----------------------------------------------------------------------------
//  wxExtListLineData
//-----------------------------------------------------------------------------

inline int wxExtListLineData::GetMode() const
{
    return m_owner->GetListCtrl()->GetWindowStyleFlag() & wxLCEXT_MASK_TYPE;
}

inline bool wxExtListLineData::InReportView() const
{
    return m_owner->HasFlag(wxLCEXT_REPORT);
}

inline bool wxExtListLineData::IsVirtual() const
{
    return m_owner->IsVirtual();
}

wxExtListLineData::wxExtListLineData( wxExtListMainWindow *owner )
{
    m_owner = owner;

    m_RowID=0;
    m_LineID=0;

    if ( InReportView() )
    {
        m_gi = NULL;
    }
    else // !report
    {
        m_gi = new GeometryInfo;
    }
    m_highlighted = false;
    m_checkboxSelected=false;
    InitItems( GetMode() == wxLCEXT_REPORT ? m_owner->GetColumnCount() : 1 );
}

void wxExtListLineData::CalculateSize( wxDC *dc, int spacing )
{
    wxExtListItemDataList::compatibility_iterator node = m_items.GetFirst();
    wxCHECK_RET( node, _T("no subitems at all??") );

    wxExtListItemData *item = node->GetData();

    wxString s;
    wxCoord lw, lh;

    switch ( GetMode() )
    {
        case wxLCEXT_ICON:
        case wxLCEXT_SMALL_ICON:
            m_gi->m_rectAll.width = spacing;

            s = item->GetText();

            if ( s.empty() )
            {
                lh =
                m_gi->m_rectLabel.width =
                m_gi->m_rectLabel.height = 0;
            }
            else // has label
            {
                dc->GetTextExtent( s, &lw, &lh );
                lw += EXTRA_WIDTH;
                lh += EXTRA_HEIGHT;

                m_gi->m_rectAll.height = spacing + lh;
                if (lw > spacing)
                    m_gi->m_rectAll.width = lw;

                m_gi->m_rectLabel.width = lw;
                m_gi->m_rectLabel.height = lh;
            }

            if (item->HasImage())
            {
                int w, h;
                m_owner->GetImageSize( item->GetImage(), w, h );
                m_gi->m_rectIcon.width = w + 8;
                m_gi->m_rectIcon.height = h + 8;

                if ( m_gi->m_rectIcon.width > m_gi->m_rectAll.width )
                    m_gi->m_rectAll.width = m_gi->m_rectIcon.width;
                if ( m_gi->m_rectIcon.height + lh > m_gi->m_rectAll.height - 4 )
                    m_gi->m_rectAll.height = m_gi->m_rectIcon.height + lh + 4;
            }

            if ( item->HasText() )
            {
                m_gi->m_rectHighlight.width = m_gi->m_rectLabel.width;
                m_gi->m_rectHighlight.height = m_gi->m_rectLabel.height;
            }
            else // no text, highlight the icon
            {
                m_gi->m_rectHighlight.width = m_gi->m_rectIcon.width;
                m_gi->m_rectHighlight.height = m_gi->m_rectIcon.height;
            }
            break;

        case wxLCEXT_LIST:
            s = item->GetTextForMeasuring();

            dc->GetTextExtent( s, &lw, &lh );
            lw += EXTRA_WIDTH;
            lh += EXTRA_HEIGHT;

            m_gi->m_rectLabel.width = lw;
            m_gi->m_rectLabel.height = lh;

            m_gi->m_rectAll.width = lw;
            m_gi->m_rectAll.height = lh;

            if (item->HasImage())
            {
                int w, h;
                m_owner->GetImageSize( item->GetImage(), w, h );
                m_gi->m_rectIcon.width = w;
                m_gi->m_rectIcon.height = h;

                m_gi->m_rectAll.width += 4 + w;
                if (h > m_gi->m_rectAll.height)
                    m_gi->m_rectAll.height = h;
            }

            m_gi->m_rectHighlight.width = m_gi->m_rectAll.width;
            m_gi->m_rectHighlight.height = m_gi->m_rectAll.height;
            break;

        case wxLCEXT_REPORT:
            wxFAIL_MSG( _T("unexpected call to SetSize") );
            break;

        default:
            wxFAIL_MSG( _T("unknown mode") );
    }
}

void wxExtListLineData::SetPosition( int x, int y, int spacing )
{
    wxExtListItemDataList::compatibility_iterator node = m_items.GetFirst();
    wxCHECK_RET( node, _T("no subitems at all??") );

    wxExtListItemData *item = node->GetData();

    switch ( GetMode() )
    {
        case wxLCEXT_ICON:
        case wxLCEXT_SMALL_ICON:
            m_gi->m_rectAll.x = x;
            m_gi->m_rectAll.y = y;

            if ( item->HasImage() )
            {
                m_gi->m_rectIcon.x = m_gi->m_rectAll.x + 4 +
                    (m_gi->m_rectAll.width - m_gi->m_rectIcon.width) / 2;
                m_gi->m_rectIcon.y = m_gi->m_rectAll.y + 4;
            }

            if ( item->HasText() )
            {
                if (m_gi->m_rectAll.width > spacing)
                    m_gi->m_rectLabel.x = m_gi->m_rectAll.x + 2;
                else
                    m_gi->m_rectLabel.x = m_gi->m_rectAll.x + 2 + (spacing/2) - (m_gi->m_rectLabel.width/2);
                m_gi->m_rectLabel.y = m_gi->m_rectAll.y + m_gi->m_rectAll.height + 2 - m_gi->m_rectLabel.height;
                m_gi->m_rectHighlight.x = m_gi->m_rectLabel.x - 2;
                m_gi->m_rectHighlight.y = m_gi->m_rectLabel.y - 2;
            }
            else // no text, highlight the icon
            {
                m_gi->m_rectHighlight.x = m_gi->m_rectIcon.x - 4;
                m_gi->m_rectHighlight.y = m_gi->m_rectIcon.y - 4;
            }
            break;

        case wxLCEXT_LIST:
            m_gi->m_rectAll.x = x;
            m_gi->m_rectAll.y = y;

            m_gi->m_rectHighlight.x = m_gi->m_rectAll.x;
            m_gi->m_rectHighlight.y = m_gi->m_rectAll.y;
            m_gi->m_rectLabel.y = m_gi->m_rectAll.y + 2;

            if (item->HasImage())
            {
                m_gi->m_rectIcon.x = m_gi->m_rectAll.x + 2;
                m_gi->m_rectIcon.y = m_gi->m_rectAll.y + 2;
                m_gi->m_rectLabel.x = m_gi->m_rectAll.x + 6 + m_gi->m_rectIcon.width;
            }
            else
            {
                m_gi->m_rectLabel.x = m_gi->m_rectAll.x + 2;
            }
            break;

        case wxLCEXT_REPORT:
            wxFAIL_MSG( _T("unexpected call to SetPosition") );
            break;

        default:
            wxFAIL_MSG( _T("unknown mode") );
    }
}

void wxExtListLineData::InitItems( int num )
{
    for (int i = 0; i < num; i++)
        m_items.Append( new wxExtListItemData(m_owner) );
}

void wxExtListLineData::SetItem( int index, const wxExtListItem &info )
{
    wxExtListItemDataList::compatibility_iterator node = m_items.Item( index );
    wxCHECK_RET( node, _T("invalid column index in SetItem") );
    wxExtListItemData *item = node->GetData();
    item->SetItem( info );
}

void wxExtListLineData::GetItem( int index, wxExtListItem &info )
{
    wxExtListItemDataList::compatibility_iterator node = m_items.Item( index );
    if (node)
    {
        wxExtListItemData *item = node->GetData();
        item->GetItem( info );
    }
}

wxString wxExtListLineData::GetText(int index) const
{
    wxString s;

    wxExtListItemDataList::compatibility_iterator node = m_items.Item( index );
    if (node)
    {
        wxExtListItemData *item = node->GetData();
        s = item->GetText();
    }

    return s;
}

void wxExtListLineData::SetText( int index, const wxString s, int i )
{
    wxExtListItemDataList::compatibility_iterator node = m_items.Item( index );
    if (node)
    {
        wxExtListItemData *item = node->GetData();
        item->SetText( s );
    }
}


void wxExtListLineData::SetColor( int index, const wxColor c )
{
	wxExtListItemDataList::compatibility_iterator node = m_items.Item( index );
	if (node)
	{
		wxExtListItemData *item = node->GetData();
		item->SetColor( c );
	}
}
void wxExtListLineData::SetImage( int index, int image )
{
    wxExtListItemDataList::compatibility_iterator node = m_items.Item( index );
    wxCHECK_RET( node, _T("invalid column index in SetImage()") );

    wxExtListItemData *item = node->GetData();
    item->SetImage(image);
}

int wxExtListLineData::GetImage( int index ) const
{
    wxExtListItemDataList::compatibility_iterator node = m_items.Item( index );
    wxCHECK_MSG( node, -1, _T("invalid column index in GetImage()") );

    wxExtListItemData *item = node->GetData();
    return item->GetImage();
}

wxExtListItemAttr *wxExtListLineData::GetAttr() const
{
    wxExtListItemDataList::compatibility_iterator node = m_items.GetFirst();
    wxCHECK_MSG( node, NULL, _T("invalid column index in GetAttr()") );

    wxExtListItemData *item = node->GetData();
    return item->GetAttr();
}

void wxExtListLineData::SetAttr(wxExtListItemAttr *attr)
{
    wxExtListItemDataList::compatibility_iterator node = m_items.GetFirst();
    wxCHECK_RET( node, _T("invalid column index in SetAttr()") );

    wxExtListItemData *item = node->GetData();
    item->SetAttr(attr);
}

bool wxExtListLineData::SetAttributes(wxDC *dc,
                                   const wxExtListItemAttr *attr,
                                   bool highlighted,
                                   size_t lineID)
{
    wxWindow *listctrl = m_owner->GetParent();

    // fg colour

    // don't use foreground colour for drawing highlighted items - this might
    // make them completely invisible (and there is no way to do bit
    // arithmetics on wxColour, unfortunately)
    wxColour colText;
    if ( highlighted )
    {
        colText = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
    }
    else
    {
        if ( attr && attr->HasTextColour() )
        {
            colText = attr->GetTextColour();
        }
        else
        {
            colText = listctrl->GetForegroundColour();
        }
    }

    dc->SetTextForeground(colText);

    // font
    wxFont font;
    if ( attr && attr->HasFont() )
    {
        font = attr->GetFont();
    }
    else
    {
        font = listctrl->GetFont();
    }

    dc->SetFont(font);

    // bg colour
    bool hasBgCol = attr && attr->HasBackgroundColour();
  /*  if ( highlighted || hasBgCol )
    {
        if ( highlighted )
        {
            dc->SetBrush( *m_owner->GetHighlightBrush() );
        }
        else
        {
            dc->SetBrush(wxBrush(attr->GetBackgroundColour(), wxSOLID));
        }

        dc->SetPen( *wxTRANSPARENT_PEN );

        return true;
    }*/
    if( highlighted )
    {
        dc->SetBrush( *m_owner->GetHighlightBrush() );
        dc->SetPen( *wxTRANSPARENT_PEN );
        return true;
    }
    else
    {
        if(hasBgCol)
        {
            dc->SetBrush(wxBrush(attr->GetBackgroundColour(), wxSOLID));
            dc->SetPen( *wxTRANSPARENT_PEN );  
            return true;
        }
        else
        {
            if(m_owner->HasFlag(wxLCEXT_TOGGLE_COLOUR))
            {  
                if(lineID%2==0)
                    dc->SetBrush(wxBrush(m_owner->GetOddRowColour(), wxSOLID));
                else
                    dc->SetBrush(wxBrush(m_owner->GetEvenRowColour(), wxSOLID));
                dc->SetPen( *wxTRANSPARENT_PEN );  
                return true;
            }
        }
    }

    return false;
}

void wxExtListLineData::Draw( wxDC *dc )
{
    wxExtListItemDataList::compatibility_iterator node = m_items.GetFirst();
    wxCHECK_RET( node, _T("no subitems at all??") );

    bool highlighted = IsHighlighted();

    wxExtListItemAttr *attr = GetAttr();

    if ( SetAttributes(dc, attr, highlighted) )
    {
        dc->DrawRectangle( m_gi->m_rectHighlight );
    }

    // just for debugging to better see where the items are
#if 0
    dc->SetPen(*wxRED_PEN);
    dc->SetBrush(*wxTRANSPARENT_BRUSH);
    dc->DrawRectangle( m_gi->m_rectAll );
    dc->SetPen(*wxGREEN_PEN);
    dc->DrawRectangle( m_gi->m_rectIcon );
#endif // 0

    wxExtListItemData *item = node->GetData();
    if (item->HasImage())
    {
        // centre the image inside our rectangle, this looks nicer when items
        // ae aligned in a row
        const wxRect& rectIcon = m_gi->m_rectIcon;

        m_owner->DrawImage(item->GetImage(), dc, rectIcon.x, rectIcon.y);
    }

    if (item->HasText())
    {
        const wxRect& rectLabel = m_gi->m_rectLabel;

        wxDCClipper clipper(*dc, rectLabel);
        dc->DrawText(item->GetText(), rectLabel.x, rectLabel.y);
    }
}
void wxExtListLineData::DrawCheckBox(wxDC& dc,const wxRect& rect,bool bcheck)
{
	int x=rect.x+(rect.width-13)/2;
	int y=rect.y+(rect.height-13)/2;
	if(bcheck)
	{
		if(m_owner->m_CheckBitmap.IsOk()) 
			dc.DrawBitmap(m_owner->m_CheckBitmap,x,y);
	}
	else
	{
		if(m_owner->m_UncheckBitmap.IsOk()) 
			dc.DrawBitmap(m_owner->m_UncheckBitmap,x,y);
	}
}
void wxExtListLineData::DrawInReportMode(wxDC *dc,
                                       const wxRect& rect,
                                       const wxRect& rectHL,
                                       bool highlighted,
                                       size_t lineID)
{
    // TODO: later we should support setting different attributes for
    //       different columns - to do it, just add "col" argument to
    //       GetAttr() and move these lines into the loop below
    wxExtListItemAttr *attr = GetAttr();

    if ( SetAttributes(dc, attr, highlighted,lineID) )
    {
       // wxBrush oldbr=dc->GetBrush();
        //wxBrush newbr(wxColour(0,255,255));
       // dc->SetBrush(newbr);
        dc->DrawRectangle( rectHL );
       // dc->SetBrush(oldbr);
    }

    wxCoord x = rect.x + HEADER_OFFSET_X,
            y = rect.y + (LINE_SPACING + EXTRA_HEIGHT) / 2;

    size_t col = 0;
    for ( wxExtListItemDataList::compatibility_iterator node = m_items.GetFirst();
          node;
          node = node->GetNext(), col++ )
    {
        wxExtListItemData *item = node->GetData();

        int width = m_owner->GetColumnWidth(col);
        int ctrltype = m_owner->GetColumnCtrlType(col);
        int xOld = x;
        x += width;

        if ( item->HasImage() )
        {
            int ix, iy;
            m_owner->DrawImage( item->GetImage(), dc, xOld, y );
            m_owner->GetImageSize( item->GetImage(), ix, iy );

            ix += IMAGE_MARGIN_IN_REPORT_MODE;

            xOld += ix;
            width -= ix;
        }
        wxString strCheckText=item->GetText();
        if(ctrltype==COLUMN_CTRL_TYPE_CHECKBOX)
        {

            //wxRendererNative::Get().DrawCheckBox(m_owner,*dc,wxRect(xOld,y,rect.height,rect.height),m_checkboxSelected?wxCONTROL_CHECKED:0);
			DrawCheckBox(*dc,wxRect(xOld,y,rect.height,rect.height),m_checkboxSelected);
            //dc->DrawRectangle(xOld, y, width - 8, rect.height);
            xOld += rect.height;
            width -= rect.height;

        }

        wxDCClipper clipper(*dc, xOld, y, width - 8, rect.height);

        if ( item->HasText() )
        {
		//DEBUG_INFO("wxExtListLineData::DrawInReportMode() item text %s",item->GetText().c_str());
		
		bool colored = item->HasColor();
		wxColor oldDC( dc->GetTextForeground() );
		wxFont oldFT( dc->GetFont() );
        wxColor oldDCBack( dc->GetTextBackground() );		
        bool font = item->HasFont();

        dc->SetTextForeground(item->GetColor());

		if(font)
			dc->SetFont(item->GetFont());
		
		DrawTextFormatted(dc, item->GetText(), col, xOld, y, width - 8);
		
		if( colored )
			dc->SetTextForeground(oldDC);
		if( font )
			dc->SetFont(oldFT);
        }
    }
}

void wxExtListLineData::DrawTextFormatted(wxDC *dc,
                                       const wxString &text,
                                       int col,
                                       int x,
                                       int y,
                                       int width)
{
    wxString drawntext, ellipsis;
    wxCoord w, h, base_w;
    wxExtListItem item;

    // determine if the string can fit inside the current width
    dc->GetTextExtent(text, &w, &h);
    if (w <= width)
    {
        // it can, draw it using the items alignment
        m_owner->GetColumn(col, item);
        switch ( item.GetAlign() )
        {
            default:
                wxFAIL_MSG( _T("unknown list item format") );
                // fall through

            case wxLISTEXT_FORMAT_LEFT:
                // nothing to do
                break;

            case wxLISTEXT_FORMAT_RIGHT:
                x += width - w;
                break;

            case wxLISTEXT_FORMAT_CENTER:
                x += (width - w) / 2;
                break;
        }

        dc->DrawText(text, x, y);
    }
    else // otherwise, truncate and add an ellipsis if possible
    {
        // determine the base width
        ellipsis = wxString(wxT("..."));
        dc->GetTextExtent(ellipsis, &base_w, &h);

        // continue until we have enough space or only one character left
        wxCoord w_c, h_c;
        size_t len = text.Length();
        drawntext =                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 text.Left(len);
        while (len > 1)
        {
            dc->GetTextExtent(drawntext.Last(), &w_c, &h_c);
            drawntext.RemoveLast();
            len--;
            w -= w_c;
            if (w + base_w <= width)
                break;
        }

        // if still not enough space, remove ellipsis characters
        while (ellipsis.Length() > 0 && w + base_w > width)
        {
            ellipsis = ellipsis.Left(ellipsis.Length() - 1);
            dc->GetTextExtent(ellipsis, &base_w, &h);
        }

        // now draw the text
        dc->DrawText(drawntext, x, y);
        dc->DrawText(ellipsis, x + w, y);
    }
}

bool wxExtListLineData::Highlight( bool on )
{
    wxCHECK_MSG( !IsVirtual(), false, _T("unexpected call to Highlight") );

    if ( on == m_highlighted )
        return false;

    m_highlighted = on;

    return true;
}

void wxExtListLineData::ReverseHighlight( void )
{
    Highlight(!IsHighlighted());
}

//-----------------------------------------------------------------------------
//  wxExtListHeaderWindow
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxExtListHeaderWindow,wxWindow)

BEGIN_EVENT_TABLE(wxExtListHeaderWindow,wxWindow)
    EVT_PAINT         (wxExtListHeaderWindow::OnPaint)
    EVT_MOUSE_EVENTS  (wxExtListHeaderWindow::OnMouse)
    EVT_SET_FOCUS     (wxExtListHeaderWindow::OnSetFocus)
END_EVENT_TABLE()

void wxExtListHeaderWindow::Init()
{
    m_currentCursor = (wxCursor *) NULL;
    m_isDragging = false;
    m_dirty = false;
    m_bsort=false;
    m_sort_column=-1;
    m_sort_asc=false;
    m_backgroundcolor=wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
}

wxExtListHeaderWindow::wxExtListHeaderWindow()
{
    Init();

    m_owner = (wxExtListMainWindow *) NULL;
    m_resizeCursor = (wxCursor *) NULL;
}

wxExtListHeaderWindow::wxExtListHeaderWindow( wxWindow *win,
                                        wxWindowID id,
                                        wxExtListMainWindow *owner,
                                        const wxPoint& pos,
                                        const wxSize& size,
                                        long style,
                                        const wxString &name )
                  : wxWindow( win, id, pos, size, style, name )
{
    Init();

    m_owner = owner;
    m_resizeCursor = new wxCursor( wxCURSOR_SIZEWE );

#if _USE_VISATTR
    wxVisualAttributes attr = wxPanel::GetClassDefaultAttributes();
    SetOwnForegroundColour( attr.colFg );
    SetOwnBackgroundColour( attr.colBg );
    if (!m_hasFont)
        SetOwnFont( attr.font );
#else
    //SetOwnForegroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    //SetOwnBackgroundColour( wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    SetOwnBackgroundColour(m_backgroundcolor);
    if (!m_hasFont)
        SetOwnFont( wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT ));
#endif
}

wxExtListHeaderWindow::~wxExtListHeaderWindow()
{
    delete m_resizeCursor;
}

#ifdef __WXUNIVERSAL__
#include "wx/univ/renderer.h"
#include "wx/univ/theme.h"
#endif

// shift the DC origin to match the position of the main window horz
// scrollbar: this allows us to always use logical coords
void wxExtListHeaderWindow::AdjustDC(wxDC& dc)
{
    int xpix;
    m_owner->GetScrollPixelsPerUnit( &xpix, NULL );

    int x;
    m_owner->GetViewStart( &x, NULL );

    // account for the horz scrollbar offset
    dc.SetDeviceOrigin( -x * xpix, 0 );
}

void wxExtListHeaderWindow::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );

    PrepareDC( dc );
    AdjustDC( dc );

//     dc.BeginDrawing();

    dc.SetFont( GetFont() );

    // width and height of the entire header window
    int w, h;
    GetClientSize( &w, &h );
    m_owner->CalcUnscrolledPosition(w, 0, &w, NULL);

    dc.SetBackgroundMode(wxTRANSPARENT);

    dc.SetTextForeground(GetForegroundColour());
    int x = HEADER_OFFSET_X;

    int numColumns = m_owner->GetColumnCount();
    wxExtListItem item;
    wxRendererNative::Get().DrawHeaderButton
                                (
                                    this,
                                    dc,
                                    wxRect(0,0,w+1,h),
                                    m_parent->IsEnabled() ? 0
                                                          : (int)wxCONTROL_DISABLED
                                );

    dc.GradientFillLinear(wxRect(0,0,w,h-3),m_backgroundcolor,m_backgroundcolor);
#define GRADIENT_STEP 35
	unsigned char r=m_backgroundcolor.Red()>GRADIENT_STEP?m_backgroundcolor.Red()-GRADIENT_STEP:0;
	unsigned char g=m_backgroundcolor.Green()>GRADIENT_STEP?m_backgroundcolor.Green()-GRADIENT_STEP:0;
	unsigned char b=m_backgroundcolor.Blue()>GRADIENT_STEP?m_backgroundcolor.Blue()-GRADIENT_STEP:0;
    dc.GradientFillLinear(wxRect(0,h-3,w,h),m_backgroundcolor,wxColour(r,g,b));
    for ( int i = 0; i < numColumns && x < w; i++ )
    {
        m_owner->GetColumn( i, item );
        int wCol = item.m_width;


        // the width of the rect to draw: make it smaller to fit entirely
        // inside the column rect
#ifdef __WXMAC__
        int cw = wCol ;
        int ch = h ;
#else
        int cw = wCol ;
        int ch = h-2 ;
#endif
/*        wxRendererNative::Get().DrawHeaderButton
                                (
                                    this,
                                    dc,
                                    wxRect(x, HEADER_OFFSET_Y, cw, ch),
                                    m_parent->IsEnabled() ? 0
                                                          : (int)wxCONTROL_DISABLED
                                );*/
        wxPen oldpen=dc.GetPen();
        dc.SetPen(wxPen(GetForegroundColour(),1));
        dc.DrawLine(x+cw,HEADER_OFFSET_Y+2,x+cw,ch-4);
        dc.SetPen(oldpen);

        if(m_owner->HasFlag(wxLCEXT_MASK_SORT)&&m_bsort&&m_sort_column==i)
        {
            wxRendererNative::Get().DrawDropArrow
                (
                this,
                dc,
                m_sort_asc?wxRect(x+cw, ch, -12, -ch):wxRect(x+cw-12, HEADER_OFFSET_Y, 12, ch),
                m_parent->IsEnabled() ? 0
                : (int)wxCONTROL_DISABLED
                );
            cw-=12;
        }

        // see if we have enough space for the column label

        // for this we need the width of the text
        wxCoord wLabel;
        wxCoord hLabel;
        dc.GetTextExtent(item.GetText(), &wLabel, &hLabel);
        wLabel += 2*EXTRA_WIDTH;

        // and the width of the icon, if any
        static const int MARGIN_BETWEEN_TEXT_AND_ICON = 2;
        int ix = 0,     // init them just to suppress the compiler warnings
            iy = 0;
        const int image = item.m_image;
        wxImageListType *imageList;
        if ( image != -1 )
        {
            imageList = m_owner->m_small_image_list;
            if ( imageList )
            {
                imageList->GetSize(image, ix, iy);
                wLabel += ix + MARGIN_BETWEEN_TEXT_AND_ICON;
            }
        }
        else
        {
            imageList = NULL;
        }

        // ignore alignment if there is not enough space anyhow
        int xAligned;
        switch ( wLabel < cw ? item.GetAlign() : wxLISTEXT_FORMAT_LEFT )
        {
            default:
                wxFAIL_MSG( _T("unknown list item format") );
                // fall through

            case wxLISTEXT_FORMAT_LEFT:
                xAligned = x;
                break;

            case wxLISTEXT_FORMAT_RIGHT:
                xAligned = x + cw - wLabel;
                break;

            case wxLISTEXT_FORMAT_CENTER:
                xAligned = x + (cw - wLabel) / 2;
                break;
        }


        // if we have an image, draw it on the right of the label
        if ( imageList )
        {
            imageList->Draw
                       (
                        image,
                        dc,
                        xAligned + wLabel - ix - MARGIN_BETWEEN_TEXT_AND_ICON,
                        HEADER_OFFSET_Y + (h - 4 - iy)/2,
                        wxIMAGELIST_DRAW_TRANSPARENT
                       );

            cw -= ix + MARGIN_BETWEEN_TEXT_AND_ICON;
        }

        // draw the text clipping it so that it doesn't overwrite the column
        // boundary
        wxDCClipper clipper(dc, x, HEADER_OFFSET_Y, cw-4, h - 4 );

        dc.DrawText( item.GetText(),
                     xAligned + EXTRA_WIDTH, h / 2 - hLabel / 2 ); //HEADER_OFFSET_Y + EXTRA_HEIGHT );

        x += wCol;
    }

//     dc.EndDrawing();
}

void wxExtListHeaderWindow::DrawCurrent()
{
    int x1 = m_currentX;
    int y1 = 0;
    m_owner->ClientToScreen( &x1, &y1 );

    int x2 = m_currentX;
    int y2 = 0;
    m_owner->GetClientSize( NULL, &y2 );
    m_owner->ClientToScreen( &x2, &y2 );

    wxScreenDC dc;
    dc.SetLogicalFunction( wxINVERT );
    
    dc.SetPen( wxPen( *wxBLACK, 2, wxSOLID ) );
    
    dc.SetBrush( *wxTRANSPARENT_BRUSH );

    AdjustDC(dc);

    dc.DrawLine( x1, y1, x2, y2 );

    dc.SetLogicalFunction( wxCOPY );

    dc.SetPen( wxNullPen );
    dc.SetBrush( wxNullBrush );
}
bool StringIsDouble(wxString& strInput)
{
    wxString str=strInput.Trim(false).Trim(true);
    if(str.IsEmpty())return false;
    std::vector<int> sign,dot;
    for(size_t i=0;i<str.Len();++i)
    {
        if(str[i]>='0'&&str[i]<='9')
            continue;
        else if(str[i]=='-'||str[i]=='+')
            sign.push_back(i);
        else if(str[i]=='.')
            dot.push_back(i);
        else
            return false;
    }
    bool bSign=(sign.size()==0||(sign.size()==1&&sign[0]==0));
    bool bDot=dot.size()<=1;
    return bSign&&bDot;
}
int wxCALLBACK MyCompareFunction(const wxExtListItem& item1, const wxExtListItem& item2, long sortData)
{
    int ret=0;

    if(item1.m_icolDataValid&&item2.m_icolDataValid)
    {
        if(item1.m_icolData==item2.m_icolData) ret = 0;
        else ret = item1.m_icolData>item2.m_icolData?1:-1;
    }
    else if(item1.m_dcolDataValid&&item2.m_dcolDataValid)
    {
        if(item1.m_dcolData==item2.m_dcolData) ret = 0;
        else ret = item1.m_dcolData>item2.m_dcolData?1:-1;
    }
    else
    {
        const wxString& str1=item1.GetText();
        const wxString& str2=item2.GetText();

        if(item1.m_btrynumericAsComparing && item2.m_btrynumericAsComparing)
            ret=CTools_Ansi::mycompare_trynumeric(str1.c_str(),str2.c_str());
        else ret=CTools_Ansi::mycompare2(str1.c_str(),str2.c_str());
    }
    if(ret==0) 
    {
        if(item1.m_itemId>item2.m_itemId) ret=1;
        else if(item1.m_itemId<item2.m_itemId) ret=-1;
        else ret=1;
    }

    return sortData==1?ret:-ret;
}
void wxExtListHeaderWindow::OnMouse( wxMouseEvent &event )
{
    // we want to work with logical coords
    int x;
    m_owner->CalcUnscrolledPosition(event.GetX(), 0, &x, NULL);
    int y = event.GetY();

    if (m_isDragging)
    {
        SendListEvent(wxEVT_COMMAND_LIST_EXT_COL_DRAGGING, event.GetPosition());

        // we don't draw the line beyond our window, but we allow dragging it
        // there
        int w = 0;
        GetClientSize( &w, NULL );
        m_owner->CalcUnscrolledPosition(w, 0, &w, NULL);
        w -= 6;

        // erase the line if it was drawn
        if ( m_currentX < w )
            DrawCurrent();

        if (event.ButtonUp())
        {
            ReleaseMouse();
            m_isDragging = false;
            m_dirty = true;
            int delta=m_currentX - m_minX -m_owner->GetColumnWidth(m_column);
            m_owner->SetColumnWidth( m_column, m_currentX - m_minX );
            //resize edit's position and size
            if(m_owner->m_textctrl!=NULL)
            {  
                size_t itemid,columnid;
                m_owner->m_textctrl->GetItemColumnID(itemid,columnid);
                wxRect rect=m_owner->m_textctrl->GetRect();
                if(m_column<(int)columnid)
                {
                    m_owner->m_textctrl->SetPosition(wxPoint(rect.x+delta,rect.y));

                }
                else if(m_column==columnid)
                {
                    m_owner->m_textctrl->SetSize(m_currentX - m_minX,rect.height);

                }
            }
            SendListEvent(wxEVT_COMMAND_LIST_EXT_COL_END_DRAG, event.GetPosition());
        }
        else
        {
            if (x > m_minX + 7)
                m_currentX = x;
            else
                m_currentX = m_minX + 7;

            // draw in the new location
            if ( m_currentX < w )
                DrawCurrent();
        }
    }
    else // not dragging
    {
        m_minX = 0;
        bool hit_border = false;

        // end of the current column
        int xpos = 0;

        // find the column where this event occurred
        int col,
            countCol = m_owner->GetColumnCount();
        for (col = 0; col < countCol; col++)
        {
            xpos += m_owner->GetColumnWidth( col );
            m_column = col;

            if ( (abs(x-xpos) < 3) && (y < 22) )
            {
                // near the column border
                hit_border = true;
                break;
            }

            if ( x < xpos )
            {
                // inside the column
                break;
            }

            m_minX = xpos;
        }

        if ( col == countCol )
        {
            m_column = -1;
        }

        if (event.LeftDown() || event.RightUp())
        {
            if (hit_border && event.LeftDown())
            {
                if ( SendListEvent(wxEVT_COMMAND_LIST_EXT_COL_BEGIN_DRAG,
                                   event.GetPosition()) )
                {
                    m_isDragging = true;
                    m_currentX = x;
                    CaptureMouse();
                    DrawCurrent();
                }
                //else: column resizing was vetoed by the user code
            }
            else // click on a column
            {
		     // record the selected state of the columns
		        if (event.LeftDown())
		        {
			        for (int i=0; i < m_owner->GetColumnCount(); i++)
			        {
				        wxExtListItem colItem;
				        m_owner->GetColumn(i, colItem);
				        long state = colItem.GetState();
				        if (i == m_column)
                        {
                            if(m_column==m_sort_column)
                            {
                                if(!m_bsort)        //不排序改为降序排序
                                {       
                                    m_bsort=true;
                                    m_sort_asc=false;
                                }
                                else if(m_sort_asc==false)  //降序排序改为升序排序
                                    m_sort_asc=true;
                                else                        //升序排序改为不排序
                                    m_bsort=false;
                            }
                            else
                            {
                                m_bsort=true;
                                m_sort_column=m_column;
                                m_sort_asc=false;
				                m_owner->SetSortColData(colItem.GetColData());
                            }
					        colItem.SetState(state | wxLISTEXT_STATE_SELECTED);
                        }
				        else
					        colItem.SetState(state & ~wxLISTEXT_STATE_SELECTED);
				        m_owner->SetColumn(i, colItem);
			        }
                    if(m_owner->HasFlag(wxLCEXT_MASK_SORT)&&m_bsort)
                    {
                        this->SetCursor(*wxHOURGLASS_CURSOR);
                        m_owner->SortItems(MyCompareFunction,m_column,m_sort_asc);
                        this->SetCursor(*wxSTANDARD_CURSOR);
                    }
		        }
		    
                SendListEvent( event.LeftDown()
                                    ? wxEVT_COMMAND_LIST_EXT_COL_CLICK
                                    : wxEVT_COMMAND_LIST_EXT_COL_RIGHT_CLICK,
                                event.GetPosition());
            }
        }
        else if (event.Moving())
        {
            bool setCursor;
            if (hit_border)
            {
                setCursor = m_currentCursor == wxSTANDARD_CURSOR;
                m_currentCursor = m_resizeCursor;
            }
            else
            {
                setCursor = m_currentCursor != wxSTANDARD_CURSOR;
                m_currentCursor = wxSTANDARD_CURSOR;
            }

            if ( setCursor )
                SetCursor(*m_currentCursor);
        }
    }
}

void wxExtListHeaderWindow::OnSetFocus( wxFocusEvent &WXUNUSED(event) )
{
    m_owner->SetFocus();
    m_owner->Update();
}

bool wxExtListHeaderWindow::SendListEvent(wxEventType type, wxPoint pos)
{
    wxWindow *parent = GetParent();
    wxExtListEvent le( type, parent->GetId() );
    le.SetEventObject( parent );
    le.m_pointDrag = pos;

    // the position should be relative to the parent window, not
    // this one for compatibility with MSW and common sense: the
    // user code doesn't know anything at all about this header
    // window, so why should it get positions relative to it?
    le.m_pointDrag.y -= GetSize().y;

    le.m_col = m_column;
    le.m_sort_asc=m_sort_asc;
    return !parent->GetEventHandler()->ProcessEvent( le ) || le.IsAllowed();
}

//-----------------------------------------------------------------------------
// wxExtListRenameTimer (internal)
//-----------------------------------------------------------------------------

wxExtListRenameTimer::wxExtListRenameTimer( wxExtListMainWindow *owner )
{
    m_owner = owner;
}

void wxExtListRenameTimer::Notify()
{
    m_owner->OnRenameTimer();
}

//-----------------------------------------------------------------------------
// wxExtListTextCtrl (internal)
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxExtListTextCtrl,wxTextCtrl)
    EVT_CHAR           (wxExtListTextCtrl::OnChar)
    EVT_KEY_UP         (wxExtListTextCtrl::OnKeyUp)
    EVT_KILL_FOCUS     (wxExtListTextCtrl::OnKillFocus)
END_EVENT_TABLE()

wxExtListTextCtrl::wxExtListTextCtrl(wxExtListMainWindow *owner, size_t itemEdit,size_t columnEdit)
              : m_itemEdited(itemEdit),
                m_columnEdited(columnEdit)
{
    m_owner = owner;
    m_finished = false;
    m_aboutToFinish = false;

    wxExtListItem extitem;
    extitem.SetId(m_itemEdited);
    extitem.SetColumn(m_columnEdited);
    extitem.SetMask(wxLISTEXT_MASK_TEXT);
    owner->GetItem(extitem);
    m_startValue=extitem.GetText();

    wxRect rectLabel = owner->GetLineLabelRect(itemEdit);
    rectLabel.height=owner->GetLineHeight();
    int xpos = 0;
    int col,countCol = owner->GetColumnCount();
    for (col = 0; col < (int)m_columnEdited; col++)
    {
        xpos += m_owner->GetColumnWidth( col );
    }
    rectLabel.x = xpos;
    rectLabel.width=m_owner->GetColumnWidth( m_columnEdited );



    m_owner->CalcScrolledPosition(rectLabel.x, rectLabel.y,
                                  &rectLabel.x, &rectLabel.y);

  //  (void)Create(owner, wxID_ANY, m_startValue,
  //               wxPoint(rectLabel.x-4,rectLabel.y-4),
  //               wxSize(rectLabel.width+11,rectLabel.height+8));
    (void)Create(owner, wxID_ANY, m_startValue,
                 wxPoint(rectLabel.x,rectLabel.y),
                 wxSize(rectLabel.width,rectLabel.height));
}

void wxExtListTextCtrl::Finish()
{
    if ( !m_finished )
    {
        wxPendingDelete.Append(this);
        m_owner->m_textctrl = NULL;
        
        m_finished = true;

        m_owner->SetFocusIgnoringChildren();
    }
}

bool wxExtListTextCtrl::AcceptChanges()
{
    const wxString value = GetValue();

    if ( value == m_startValue )
    {
        // nothing changed, always accept
        return true;
    }

    if ( !m_owner->OnRenameAccept(m_itemEdited, m_columnEdited, value) )
    {
        // vetoed by the user
        return false;
    }

    // accepted, do rename the item
    wxExtListItem extitem;
    extitem.SetId(m_itemEdited);
    extitem.SetColumn(m_columnEdited);
    extitem.SetMask(wxLISTEXT_MASK_TEXT);
    extitem.SetText(value);
    m_owner->SetItem(extitem);

    return true;
}

void wxExtListTextCtrl::AcceptChangesAndFinish()
{
    m_aboutToFinish = true;
    // Notify the owner about the changes
    AcceptChanges();
    // Even if vetoed, close the control (consistent with MSW)
    Finish();
}
void wxExtListTextCtrl::GetItemColumnID(size_t& item,size_t& column)
{
    item=m_itemEdited;
    column=m_columnEdited;
}
void wxExtListTextCtrl::OnChar( wxKeyEvent &event )
{
    switch ( event.m_keyCode )
    {
        case WXK_RETURN:
            AcceptChangesAndFinish();
            break;

        case WXK_ESCAPE:
            Finish();
            m_owner->OnRenameCancelled( m_itemEdited );
            break;

        default:
            event.Skip();
    }
}

void wxExtListTextCtrl::OnKeyUp( wxKeyEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    // auto-grow the textctrl:
    wxSize parentSize = m_owner->GetSize();
    wxPoint myPos = GetPosition();
    wxSize mySize = GetSize();
    int sx, sy;
    GetTextExtent(GetValue() + _T("MM"), &sx, &sy);
    if (myPos.x + sx > parentSize.x)
        sx = parentSize.x - myPos.x;
    if (mySize.x > sx)
        sx = mySize.x;
    SetSize(sx, wxDefaultCoord);

    event.Skip();
}

void wxExtListTextCtrl::OnKillFocus( wxFocusEvent &event )
{
    if ( !m_finished && !m_aboutToFinish  )
    {
        // We must finish regardless of success, otherwise we'll get
        // focus problems:
        Finish();

        if ( !AcceptChanges() )
            m_owner->OnRenameCancelled( m_itemEdited );
    }

    // We must let the native text control handle focus, too, otherwise
    // it could have problems with the cursor (e.g., in wxGTK).
    event.Skip();
}
//-----------------------------------------------------------------------------
//  wxExtListMainWindow
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxExtListMainWindow,wxScrolledWindow)

BEGIN_EVENT_TABLE(wxExtListMainWindow,wxScrolledWindow)
  EVT_PAINT          (wxExtListMainWindow::OnPaint)
  EVT_MOUSE_EVENTS   (wxExtListMainWindow::OnMouse)
  EVT_CHAR           (wxExtListMainWindow::OnChar)
  EVT_KEY_DOWN       (wxExtListMainWindow::OnKeyDown)
  EVT_SET_FOCUS      (wxExtListMainWindow::OnSetFocus)
  EVT_KILL_FOCUS     (wxExtListMainWindow::OnKillFocus)
  EVT_SCROLLWIN      (wxExtListMainWindow::OnScroll)
END_EVENT_TABLE()

void wxExtListMainWindow::Init()
{
	m_bakSortColData = -1;
//	m_needReSort = false;
    m_dirty = true;
    m_countVirt = 0;
    m_lineFrom =
    m_lineTo = (size_t)-1;
    m_linesPerPage = 0;

    m_headerWidth =
    m_lineHeight = 0;

    m_small_image_list = (wxImageListType *) NULL;
    m_normal_image_list = (wxImageListType *) NULL;

    m_small_spacing = 30;
    m_normal_spacing = 40;

    m_hasFocus = false;
    m_dragCount = 0;
    m_isCreated = false;

    m_lastOnSame = false;
    m_renameTimer = new wxExtListRenameTimer( this );
    m_textctrl = NULL;
    
    m_current =(size_t)-1;
    m_current_column =(size_t)-1;
    m_lineLastClicked =(size_t)-1;
    m_lineSelectSingleOnUp =(size_t)-1;
    m_lineBeforeLastClicked = (size_t)-1;
    m_OddRowColour=wxColor(255,255,245);
    m_EvenRowColour=wxColor(240,240,240);
    m_freezeCount = 0;
	m_CheckBitmap.LoadFile("zqBMP_CHECKBOX");
	m_UncheckBitmap.LoadFile("zqBMP_UNCHECKBOX");

    m_mapItemData2RowID.clear();
}

wxExtListMainWindow::wxExtListMainWindow()
{
    Init();

    m_highlightBrush =
    m_highlightUnfocusedBrush = (wxBrush *) NULL;
}

wxExtListMainWindow::wxExtListMainWindow( wxWindow *parent,
                                    wxWindowID id,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style,
                                    const wxString &name )
                : wxScrolledWindow( parent, id, pos, size,
                                    style | wxHSCROLL | wxVSCROLL, name )
{
    Init();

    m_highlightBrush = new wxBrush
                           (
                            wxColor(189,189,255),
                            wxSOLID
                           );

    m_highlightUnfocusedBrush = new wxBrush
                                    (
                                       wxColor(189,189,255),
                                       wxSOLID
                                    );

    SetScrollbars( 0, 0, 0, 0, 0, 0 );

    wxVisualAttributes attr = wxExtGenericListCtrl::GetClassDefaultAttributes();
    SetOwnForegroundColour( attr.colFg );
    SetOwnBackgroundColour( attr.colBg );
    if (!m_hasFont)
        SetOwnFont( attr.font );
}

wxExtListMainWindow::~wxExtListMainWindow()
{
    DoDeleteAllItems();
    WX_CLEAR_LIST(wxExtListHeaderDataList, m_columns);
    WX_CLEAR_ARRAY(m_aColWidths);

    delete m_highlightBrush;
    delete m_highlightUnfocusedBrush;

    delete m_renameTimer;
}

void wxExtListMainWindow::CacheLineData(size_t line)
{
    wxExtGenericListCtrl *listctrl = GetListCtrl();

    wxExtListLineData *ld = GetDummyLine();

    size_t countCol = GetColumnCount();
    for ( size_t col = 0; col < countCol; col++ )
    {
        ld->SetText(col, listctrl->OnGetItemText(line, col), 1);
    }

    ld->SetImage(listctrl->OnGetItemImage(line));
    ld->SetAttr(listctrl->OnGetItemAttr(line));
}

wxExtListLineData *wxExtListMainWindow::GetDummyLine() const
{
    wxASSERT_MSG( !IsEmpty(), _T("invalid line index") );

    wxASSERT_MSG( IsVirtual(), _T("GetDummyLine() shouldn't be called") );

    wxExtListMainWindow *self = wxConstCast(this, wxExtListMainWindow);

    // we need to recreate the dummy line if the number of columns in the
    // control changed as it would have the incorrect number of fields
    // otherwise
    if ( !m_lines.IsEmpty() &&
            m_lines[0].m_items.GetCount() != (size_t)GetColumnCount() )
    {
        self->m_lines.Clear();
    }

    if ( m_lines.IsEmpty() )
    {
        wxExtListLineData *line = new wxExtListLineData(self);
        self->m_lines.Add(line);

        // don't waste extra memory -- there never going to be anything
        // else/more in this array
        self->m_lines.Shrink();
    }

    return &m_lines[0];
}

// ----------------------------------------------------------------------------
// line geometry (report mode only)
// ----------------------------------------------------------------------------

wxCoord wxExtListMainWindow::GetLineHeight() const
{
    // we cache the line height as calling GetTextExtent() is slow
    if ( !m_lineHeight )
    {
        wxExtListMainWindow *self = wxConstCast(this, wxExtListMainWindow);

        wxClientDC dc( self );
        dc.SetFont( GetFont() );

        wxCoord y;
        dc.GetTextExtent(_T("H"), NULL, &y);

        if ( m_small_image_list && m_small_image_list->GetImageCount() )
        {
            int iw = 0;
            int ih = 0;
            m_small_image_list->GetSize(0, iw, ih);
            y = wxMax(y, ih);
        }

        y += EXTRA_HEIGHT;
        self->m_lineHeight = y + LINE_SPACING;
    }

    return m_lineHeight;
}

wxCoord wxExtListMainWindow::GetLineY(size_t line) const
{
    wxASSERT_MSG( InReportView(), _T("only works in report mode") );

    return LINE_SPACING + line*GetLineHeight();
}

wxRect wxExtListMainWindow::GetLineRect(size_t line) const
{
    if ( !InReportView() )
        return GetLine(line)->m_gi->m_rectAll;

    wxRect rect;
    rect.x = HEADER_OFFSET_X;
    rect.y = GetLineY(line);
    rect.width = GetHeaderWidth();
    rect.height = GetLineHeight();

    return rect;
}

wxRect wxExtListMainWindow::GetLineLabelRect(size_t line) const
{
    if ( !InReportView() )
        return GetLine(line)->m_gi->m_rectLabel;

    wxRect rect;
    rect.x = HEADER_OFFSET_X;
    rect.y = GetLineY(line);
    rect.width = GetColumnWidth(0);
    rect.height = GetLineHeight();

    return rect;
}

wxRect wxExtListMainWindow::GetLineIconRect(size_t line) const
{
    if ( !InReportView() )
        return GetLine(line)->m_gi->m_rectIcon;

    wxExtListLineData *ld = GetLine(line);
    wxASSERT_MSG( ld->HasImage(), _T("should have an image") );

    wxRect rect;
    rect.x = HEADER_OFFSET_X;
    rect.y = GetLineY(line);
    GetImageSize(ld->GetImage(), rect.width, rect.height);

    return rect;
}

wxRect wxExtListMainWindow::GetLineHighlightRect(size_t line) const
{
    return InReportView() ? GetLineRect(line)
                          : GetLine(line)->m_gi->m_rectHighlight;
}

long wxExtListMainWindow::HitTestLine(size_t line, int x, int y) const
{
    wxASSERT_MSG( line < GetItemCount(), _T("invalid line in HitTestLine") );

    wxExtListLineData *ld = GetLine(line);

#ifdef __WX_R2_6__
	if ( ld->HasImage() && GetLineIconRect(line).Inside(x, y) )
#else
	if ( ld->HasImage() && GetLineIconRect(line).Contains(x, y) )
#endif
        return wxLISTEXT_HITTEST_ONITEMICON;

    // VS: Testing for "ld->HasText() || InReportView()" instead of
    //     "ld->HasText()" is needed to make empty lines in report view
    //     possible
    if ( ld->HasText() || InReportView() )
    {
        wxRect rect = InReportView() ? GetLineRect(line)
                                     : GetLineLabelRect(line);
#ifdef __WX_R2_6__
	if ( rect.Inside(x, y) )
#else
	if ( rect.Contains(x, y) )
#endif
            return wxLISTEXT_HITTEST_ONITEMLABEL;
    }

    return 0;
}

// ----------------------------------------------------------------------------
// highlight (selection) handling
// ----------------------------------------------------------------------------

bool wxExtListMainWindow::IsHighlighted(size_t line) const
{
    if ( IsVirtual() )
    {
        return m_selStore.IsSelected(line);
    }
    else // !virtual
    {
        wxExtListLineData *ld = GetLine(line);
        wxCHECK_MSG( ld, false, _T("invalid index in IsHighlighted") );
        return ld->IsHighlighted();
    }
}

void wxExtListMainWindow::HighlightLines( size_t lineFrom,
                                       size_t lineTo,
                                       bool highlight )
 {
    if ( IsVirtual() )
    {
        wxArrayInt linesChanged;
        if ( !m_selStore.SelectRange(lineFrom, lineTo, highlight,
                                     &linesChanged) )
        {
            // meny items changed state, refresh everything
            RefreshLines(lineFrom, lineTo);
        }
        else // only a few items changed state, refresh only them
        {
            size_t count = linesChanged.GetCount();
            for ( size_t n = 0; n < count; n++ )
            {
                RefreshLine(linesChanged[n]);
            }
        }
    }
    else // iterate over all items in non report view
    {
        for ( size_t line = lineFrom; line <= lineTo; line++ )
        {
            if ( HighlightLine(line, highlight) )
            {
                RefreshLine(line);
            }
        }
    }
}

bool wxExtListMainWindow::HighlightLine( size_t line, bool highlight )
{
    bool changed;

    if ( IsVirtual() )
    {
        changed = m_selStore.SelectItem(line, highlight);
    }
    else // !virtual
    {
        wxExtListLineData *ld = GetLine(line);
        wxCHECK_MSG( ld, false, _T("invalid index in HighlightLine") );

	changed = ld->Highlight(highlight);
    }
    
    bool dontNotify = HasFlag(wxLCEXT_STICKY_HIGHLIGHT) && HasFlag(wxLCEXT_STICKY_NOSELEVENT);
    
    if ( changed && ! dontNotify)
    {
	    SendNotify( line,m_current_column, highlight ? wxEVT_COMMAND_LIST_EXT_ITEM_SELECTED : wxEVT_COMMAND_LIST_EXT_ITEM_DESELECTED );
    }

    return changed;
}

void wxExtListMainWindow::RefreshLine( size_t line )
{
    if ( InReportView() )
    {
        size_t visibleFrom, visibleTo;
        GetVisibleLinesRange(&visibleFrom, &visibleTo);

        if ( line < visibleFrom || line > visibleTo )
            return;
    }

    wxRect rect = GetLineRect(line);

    CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );
    RefreshRect( rect );
}

void wxExtListMainWindow::RefreshLines( size_t lineFrom, size_t lineTo )
{
    // we suppose that they are ordered by caller
    wxASSERT_MSG( lineFrom <= lineTo, _T("indices in disorder") );

    wxASSERT_MSG( lineTo < GetItemCount(), _T("invalid line range") );

    if ( InReportView() )
    {
        size_t visibleFrom, visibleTo;
        GetVisibleLinesRange(&visibleFrom, &visibleTo);

        if ( lineFrom < visibleFrom )
            lineFrom = visibleFrom;
        if ( lineTo > visibleTo )
            lineTo = visibleTo;

        wxRect rect;
        rect.x = 0;
        rect.y = GetLineY(lineFrom);
        rect.width = GetClientSize().x;
        rect.height = GetLineY(lineTo) - rect.y + GetLineHeight();

        CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );
        RefreshRect( rect );
    }
    else // !report
    {
        // TODO: this should be optimized...
        for ( size_t line = lineFrom; line <= lineTo; line++ )
        {
            RefreshLine(line);
        }
    }
}

void wxExtListMainWindow::RefreshAfter( size_t lineFrom )
{
    if ( InReportView() )
    {
        size_t visibleFrom, visibleTo;
        GetVisibleLinesRange(&visibleFrom, &visibleTo);

        if ( lineFrom < visibleFrom )
            lineFrom = visibleFrom;
        else if ( lineFrom > visibleTo )
            return;

        wxRect rect;
        rect.x = 0;
        rect.y = GetLineY(lineFrom);
        CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );

        wxSize size = GetClientSize();
        rect.width = size.x;
        // refresh till the bottom of the window
        rect.height = size.y - rect.y;

        RefreshRect( rect );
    }
    else // !report
    {
        // TODO: how to do it more efficiently?
        m_dirty = true;
    }
}

void wxExtListMainWindow::RefreshSelected()
{
    if ( IsEmpty() )
        return;

    size_t from, to;
    if ( InReportView() )
    {
        GetVisibleLinesRange(&from, &to);
    }
    else // !virtual
    {
        from = 0;
        to = GetItemCount() - 1;
    }

    if ( HasCurrent() && m_current >= from && m_current <= to )
    {
        RefreshLine(m_current);
    }

    for ( size_t line = from; line <= to; line++ )
    {
        // NB: the test works as expected even if m_current == -1
        if ( line != m_current && IsHighlighted(line) )
        {
            RefreshLine(line);
        }
    }
}

void wxExtListMainWindow::ReSortItems()
{
	wxExtListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;
	if ( headerWin &&
        HasFlag(wxLCEXT_MASK_SORT) && 
        headerWin->m_bsort &&
        headerWin->m_sort_column >=0 &&
        headerWin->m_sort_column < (int)GetItemCount())
	{
	    SortItems(MyCompareFunction, headerWin->m_sort_column, headerWin->m_sort_asc);
	}
}

void wxExtListMainWindow::Freeze()
{
    m_freezeCount++;
}

void wxExtListMainWindow::Thaw()
{
    wxCHECK_RET( m_freezeCount > 0, _T("thawing unfrozen list control?") );

    if ( !--m_freezeCount )
    {
        Refresh();
    }
}

void wxExtListMainWindow::OnPaint( wxPaintEvent &WXUNUSED(event) )
{

    // Note: a wxPaintDC must be constructed even if no drawing is
    // done (a Windows requirement).
    wxPaintDC dc( this );

    if ( IsEmpty() || m_freezeCount )
    {
        // nothing to draw or not the moment to draw it
        return;
    }

    if ( m_dirty )
    {
        // delay the repainting until we calculate all the items positions
        return;
    }

     PrepareDC( dc );

    int dev_x, dev_y;
    CalcScrolledPosition( 0, 0, &dev_x, &dev_y );

//     dc.BeginDrawing();

    dc.SetFont( GetFont() );

    if ( InReportView() )
    {
        int lineHeight = GetLineHeight();

        size_t visibleFrom, visibleTo;
	    GetVisibleLinesRange(&visibleFrom, &visibleTo);
	
	    // mrcs: draw additional items
	    if(visibleFrom > 0)
		    visibleFrom--;
    	
	    if(visibleTo < GetItemCount() - 1)
		    visibleTo++;
	
        wxRect rectLine;
        //wxCoord xOrig, yOrig;
        //CalcUnscrolledPosition(0, 0, &xOrig, &yOrig);
	    int xOrig = dc.LogicalToDeviceX( 0 );
	    int yOrig = dc.LogicalToDeviceY( 0 );

        // tell the caller cache to cache the data
        if ( IsVirtual() )
        {
            wxExtListEvent evCache(wxEVT_COMMAND_LIST_EXT_CACHE_HINT,
                                GetParent()->GetId());
            evCache.SetEventObject( GetParent() );
            evCache.m_oldItemIndex = visibleFrom;
            evCache.m_itemIndex = visibleTo;
            GetParent()->GetEventHandler()->ProcessEvent( evCache );
        }

        for ( size_t line = visibleFrom; line <= visibleTo; line++ )
        {
            rectLine = GetLineRect(line);

            if ( /*!IsExposed(rectLine.x - xOrig, rectLine.y - yOrig,
                            rectLine.width, rectLine.height)*/
		 
		     !IsExposed(rectLine.x + xOrig, rectLine.y + yOrig,
			        rectLine.width, rectLine.height)
		     )
            {
                // don't redraw unaffected lines to avoid flicker
                continue;
            }

	        GetLine(line)->DrawInReportMode(&dc,
                                                 rectLine,
                                                 GetLineHighlightRect(line),
					         IsHighlighted(line) && ! HasFlag(wxLCEXT_NO_HIGHLIGHT),
                             line
					       );
        }

        if ( HasFlag(wxLCEXT_HRULES) )
        {
            wxPen pen(GetRuleColour(), 1, wxSOLID);
            wxSize clientSize = GetClientSize();

            // Don't draw the first one
	        size_t i = visibleFrom;
	        if (i == 0) i = 1; // Don't draw the first one
            for ( ; i <= visibleTo; i++ )
            {
                dc.SetPen(pen);
                dc.SetBrush( *wxTRANSPARENT_BRUSH );
                dc.DrawLine(0 - dev_x, i*lineHeight,
                            clientSize.x - dev_x, i*lineHeight);
            }

            // Draw last horizontal rule
            if ( visibleTo == GetItemCount() - 1 )
            {
                dc.SetPen(pen);
                dc.SetBrush( *wxTRANSPARENT_BRUSH );
                dc.DrawLine(0 - dev_x, (m_lineTo+1)*lineHeight,
                            clientSize.x - dev_x , (m_lineTo+1)*lineHeight );
            }
        }

        // Draw vertical rules if required
        if ( HasFlag(wxLCEXT_VRULES) && !IsEmpty() )
        {
            wxPen pen(GetRuleColour(), 1, wxSOLID);

            wxRect firstItemRect;
            wxRect lastItemRect;
            GetItemRect(visibleFrom, firstItemRect);
            GetItemRect(visibleTo, lastItemRect);
            int x = firstItemRect.GetX();
            dc.SetPen(pen);
            dc.SetBrush(* wxTRANSPARENT_BRUSH);
            for (int col = 0; col < GetColumnCount(); col++)
            {
                int colWidth = GetColumnWidth(col);
                x += colWidth;
		        int x_pos = x - dev_x;
		        if (col < GetColumnCount()-1) x_pos -= 2;
        //                 dc.DrawLine(x - dev_x - 2, firstItemRect.GetY() - 1 - dev_y,
        //                             x - dev_x - 2, lastItemRect.GetBottom() + 1 - dev_y);
		        dc.DrawLine(x_pos, firstItemRect.GetY() - 1 - dev_y,
			            x_pos, lastItemRect.GetBottom() + 1 - dev_y);
            }
        }
    }
    else // !report
    {
        size_t count = GetItemCount();
        for ( size_t i = 0; i < count; i++ )
        {
            GetLine(i)->Draw( &dc );
        }
    }

#if 0//ndef __WXMAC__
    // Don't draw rect outline under Mac at all.
    if ( HasCurrent() )
    {
	    if ( m_hasFocus && ! HasFlag(wxLCEXT_NO_HIGHLIGHT))
	    {
		    dc.SetPen( *wxBLACK_PEN );
		    dc.SetBrush( *wxTRANSPARENT_BRUSH );
		    dc.DrawRectangle( GetLineHighlightRect(m_current) );
	    }
    }
#endif

//     dc.EndDrawing();
}

void wxExtListMainWindow::HighlightAll( bool on )
{
    if ( IsSingleSel() )
    {
        wxASSERT_MSG( !on, _T("can't do this in a single sel control") );

        // we just have one item to turn off
        if ( HasCurrent() && IsHighlighted(m_current) )
        {
            HighlightLine(m_current, false);
            RefreshLine(m_current);
        }
    }
    else // multi sel
    {
        HighlightLines(0, GetItemCount() - 1, on);
    }
}

bool wxExtListMainWindow::SendNotify( size_t line,
                                     size_t column,
                                   wxEventType command,
                                   wxPoint point )
{
	bool bRet = true;
	wxExtListEvent le( command, GetParent()->GetId() );
	le.SetEventObject( GetParent() );
	le.m_itemIndex = line;
    if(column>=0)
        le.m_col = column;
	// set only for events which have position
	if ( point != wxDefaultPosition )
		le.m_pointDrag = point;

	// don't try to get the line info for virtual list controls: the main
	// program has it anyhow and if we did it would result in accessing all
	// the lines, even those which are not visible now and this is precisely
	// what we're trying to avoid
	if ( !IsVirtual() && (command != wxEVT_COMMAND_LIST_EXT_DELETE_ITEM) )
	{
		if ( line != (size_t)-1 )
		{
			GetLine(line)->GetItem( 0, le.m_item );
		}
        //else: this happens for wxEVT_COMMAND_LIST_EXT_ITEM_FOCUSED event
	}
    	//else: there may be no more such item

	GetParent()->GetEventHandler()->ProcessEvent( le );
	
	bRet = le.IsAllowed();
	
	return bRet;
}

void wxExtListMainWindow::ChangeCurrent(size_t current)
{
    m_current = current;

    SendNotify(current,-1, wxEVT_COMMAND_LIST_EXT_ITEM_FOCUSED);
}

void wxExtListMainWindow::EditLabel( long item  ,long col)
{
	wxCHECK_RET( (item >= 0) && ((size_t)item < GetItemCount()),
		      wxT("wrong index in wxExtGenericListCtrl::EditLabel()") );
    wxCHECK_RET( (item >= 0) && (col < (long)GetColumnCount()),
		      wxT("wrong column in wxExtGenericListCtrl::EditLabel()") );
	size_t itemEdit = (size_t)item;

	wxExtListEvent le( wxEVT_COMMAND_LIST_EXT_BEGIN_LABEL_EDIT, GetParent()->GetId() );
	le.SetEventObject( GetParent() );
	le.m_itemIndex = item;
	wxExtListLineData *data = GetLine(itemEdit);
	wxCHECK_RET( data, _T("invalid index in EditLabel()") );
	data->GetItem( 0, le.m_item );

    
	m_textctrl = new wxExtListTextCtrl(this, itemEdit,col);
	if(m_textctrl_MaxLength>0)
		m_textctrl->SetMaxLength(m_textctrl_MaxLength);
    
    
	if ( GetParent()->GetEventHandler()->ProcessEvent( le ) && !le.IsAllowed() )
	{
        	// vetoed by user code
		return;
	}

    	// We have to call this here because the label in question might just have
    	// been added and no screen update taken place.
	if ( m_dirty )
		wxSafeYield();
	int scrpos1=GetScrollPos(wxHORIZONTAL);
	m_textctrl->SetFocus();
	int scrpos2=GetScrollPos(wxHORIZONTAL);
	if(scrpos2!=scrpos1&&HasHeader())
	{
		wxExtGenericListCtrl* lc = GetListCtrl();
		wxCHECK_RET( lc, _T("no listctrl window?") );
		lc->m_headerWin->Refresh();
		lc->m_headerWin->Update();
	}
}

wxTextCtrl * wxExtListMainWindow::GetEditControl(void)
{
	return m_textctrl;
}
void wxExtListMainWindow::SetTextCtrlMaxLength(int len)
{
	m_textctrl_MaxLength=len;
}
void wxExtListMainWindow::OnRenameTimer()
{
    wxCHECK_RET( HasCurrent(), wxT("unexpected rename timer") );

    EditLabel( m_current,m_current_column);
}

bool wxExtListMainWindow::OnRenameAccept(size_t itemEdit, size_t colEdit, const wxString& value)
{
    wxExtListEvent le( wxEVT_COMMAND_LIST_EXT_END_LABEL_EDIT, GetParent()->GetId() );
    le.SetEventObject( GetParent() );
    le.m_itemIndex = itemEdit;
	le.m_col = colEdit;

    wxExtListLineData *data = GetLine(itemEdit);
    wxCHECK_MSG( data, false, _T("invalid index in OnRenameAccept()") );

    data->GetItem( 0, le.m_item );
    le.m_item.m_text = value;
    return !GetParent()->GetEventHandler()->ProcessEvent( le ) ||
                le.IsAllowed();
}

void wxExtListMainWindow::OnRenameCancelled(size_t itemEdit)
{
    // let owner know that the edit was cancelled
    wxExtListEvent le( wxEVT_COMMAND_LIST_EXT_END_LABEL_EDIT, GetParent()->GetId() );

    le.SetEditCanceled(true);

    le.SetEventObject( GetParent() );
    le.m_itemIndex = itemEdit;

    wxExtListLineData *data = GetLine(itemEdit);
    wxCHECK_RET( data, _T("invalid index in OnRenameCancelled()") );

    data->GetItem( 0, le.m_item );

    GetEventHandler()->ProcessEvent( le );
}

void wxExtListMainWindow::OnMouse( wxMouseEvent &event )
{
#ifdef __WXMAC__
    // On wxMac we can't depend on the EVT_KILL_FOCUS event to properly
    // shutdown the edit control when the mouse is clicked elsewhere on the
    // listctrl because the order of events is different (or something like
    // that,) so explicitly end the edit if it is active.
    if ( event.LeftDown() && m_textctrl)
    {
        m_textctrl->AcceptChangesAndFinish();
    }
#endif
    if ( event.LeftDown() )
	    SetFocusIgnoringChildren();
    
    event.SetEventObject( GetParent() );
    if ( /*event.GetEventType() != wxEVT_MOTION && */GetParent()->GetEventHandler()->ProcessEvent( event) )
        return;

   
    if (event.GetEventType() == wxEVT_MOUSEWHEEL)
    {
        // let the base handle mouse wheel events.
	    
// 	size_t visibleFrom, visibleTo;
// 	GetVisibleLinesRange(&visibleFrom, &visibleTo);
// 	if(visibleFrom > 0)
// 		visibleFrom--;
// 	RefreshLines(visibleFrom, visibleTo);
	//Refresh();
        event.Skip();
        return;
    }


    if (!HasCurrent()||IsEmpty() )
    {
        if (event.RightDown())
        {
            wxContextMenuEvent evtCtx(wxEVT_COMMAND_LIST_EXT_CONTEXT_MENU);
            ProcessEvent(evtCtx);
        }
        return;
    }
    if (m_dirty)
        return;

    if ( !(event.Dragging() || event.ButtonDown() || event.LeftUp() ||
        event.ButtonDClick() || (HasFlag(wxLCEXT_STICKY_HIGHLIGHT) && event.Moving())  ) )
        return;

    int x = event.GetX();
    int y = event.GetY();
    CalcUnscrolledPosition( x, y, &x, &y );

    // where did we hit it (if we did)?
    long hitResult = 0;

    size_t count = GetItemCount(),
           current;
           
    if ( InReportView() )
    {
        // find the column where this event occurred
        int xpos = 0;
        int col,countCol = GetColumnCount();
        for (col = 0; col < countCol; col++)
        {
            xpos += GetColumnWidth( col );
            m_current_column = col;
            if ( x < xpos )
            {
                // inside the column
                break;
            }
            //m_minX = xpos;
        }

        if ( col == countCol )
        {
            m_current_column = -1;
        }
        current = y / GetLineHeight();
        if ( current < count )
            hitResult = HitTestLine(current, x, y);
    }
    else // !report
    {
        // TODO: optimize it too! this is less simple than for report view but
        //       enumerating all items is still not a way to do it!!
        for ( current = 0; current < count; current++ )
        {
            hitResult = HitTestLine(current, x, y);
            if ( hitResult )
                break;
        }
    }
    
    if(event.GetEventType() == wxEVT_MOTION)
    {
	    if ( HasFlag(wxLCEXT_STICKY_HIGHLIGHT)  && hitResult)
	    {
		    if (!IsHighlighted(current))
		    {
			    HighlightAll(false);
			    ChangeCurrent(current);
			    ReverseHighlight(m_current);
		    }
	    }
	    event.Skip();
	    return;
    }
    
    if (event.Dragging())
    {
        if (m_dragCount == 0)
        {
            // we have to report the raw, physical coords as we want to be
            // able to call HitTest(event.m_pointDrag) from the user code to
            // get the item being dragged
            m_dragStart = event.GetPosition();
        }

        m_dragCount++;

        if (m_dragCount != 3)
            return;

        int command = event.RightIsDown() ? wxEVT_COMMAND_LIST_EXT_BEGIN_RDRAG
                                          : wxEVT_COMMAND_LIST_EXT_BEGIN_DRAG;

        wxExtListEvent le( command, GetParent()->GetId() );
        le.SetEventObject( GetParent() );
        le.m_itemIndex = m_lineLastClicked;
        le.m_pointDrag = m_dragStart;
        GetParent()->GetEventHandler()->ProcessEvent( le );

        return;
    }
    else
    {
        m_dragCount = 0;
    }

    if ( !hitResult )
    {
        // outside of any item
        if (event.RightDown())
        {
            wxContextMenuEvent evtCtx(wxEVT_COMMAND_LIST_EXT_CONTEXT_MENU);
            ProcessEvent(evtCtx);
        }
        return;
    }

    bool forceClick = false;
    if (event.ButtonDClick())
    {
        m_renameTimer->Stop();
        m_lastOnSame = false;

        if ( current == m_lineLastClicked )
        {
            SendNotify( current,m_current_column, wxEVT_COMMAND_LIST_EXT_ITEM_ACTIVATED );

            return;
        }
        else
        {
            // The first click was on another item, so don't interpret this as
            // a double click, but as a simple click instead
            forceClick = true;
        }
    }

    if (event.LeftUp())
    {
        if(m_lineSelectSingleOnUp != (size_t) -1)
        {
            // select single line
            HighlightAll( false );
            ReverseHighlight(m_lineSelectSingleOnUp);
        }
        if(m_current_column>=0&&GetColumnCtrlType(m_current_column)==COLUMN_CTRL_TYPE_CHECKBOX)
        {
            GetLine(current)->m_checkboxSelected=!GetLine(current)->m_checkboxSelected;
            SendNotify( current,m_current_column, wxEVT_COMMAND_LIST_EXT_CHECKBOX_CHANGED );
            RefreshLine(current);
        }
        if (m_lastOnSame)
        {
            if ((current == m_current) &&
                (hitResult == wxLISTEXT_HITTEST_ONITEMLABEL) &&
                (m_current_column>=0&&GetColumnCtrlType(m_current_column)==COLUMN_CTRL_TYPE_EDIT)
                /*HasFlag(wxLCEXT_EDIT_LABELS) */)
            {
                m_renameTimer->Start( 100, true );
            }
        }
        m_lastOnSame = false;
        m_lineSelectSingleOnUp = (size_t) -1;
    }
    else
    {
        // This is necessary, because after a DnD operation in
        // from and to ourself, the up event is swallowed by the
        // DnD code. So on next non-up event (which means here and
        // now) m_lineSelectSingleOnUp should be reset.
        m_lineSelectSingleOnUp = (size_t) -1;
    }
    if (event.RightDown())
    {
	if(SendNotify( current,-1, wxEVT_COMMAND_LIST_EXT_ITEM_RIGHT_CLICK, event.GetPosition() ))
	{
		m_lineBeforeLastClicked = m_lineLastClicked;
		m_lineLastClicked = current;
	        // If the item is already selected, do not update the selection.
        	// Multi-selections should not be cleared if a selected item is clicked.
		
		if (!IsHighlighted(current))
		{
			HighlightAll(false);
			ChangeCurrent(current);
			ReverseHighlight(m_current);
		}
        wxContextMenuEvent evtCtx(wxEVT_CONTEXT_MENU, GetId(), event.GetPosition());
        ProcessEvent(evtCtx);
		// Allow generation of context menu event
		event.Skip();
	}	
    }
    else if (event.MiddleDown())
    {
        SendNotify( current,-1, wxEVT_COMMAND_LIST_EXT_ITEM_MIDDLE_CLICK );
    }
    else if ( event.LeftDown() || forceClick )
    {
        m_lineBeforeLastClicked = m_lineLastClicked;
        m_lineLastClicked = current;

        size_t oldCurrent = m_current;
        bool oldWasSelected = IsHighlighted(m_current);
        
       					
        bool cmdModifierDown = event.CmdDown();
        if ( IsSingleSel() || !(cmdModifierDown || event.ShiftDown()) )
        {
            if( IsSingleSel() || !IsHighlighted(current) )
            {
                HighlightAll( false );

                ChangeCurrent(current);

                ReverseHighlight(m_current);
            }
            else // multi sel & current is highlighted & no mod keys
            {
                m_lineSelectSingleOnUp = current;
                ChangeCurrent(current); // change focus
            }
        }
        else // multi sel & either ctrl or shift is down
        {
            if (cmdModifierDown)
            {
                ChangeCurrent(current);

                ReverseHighlight(m_current);
            }
            else if (event.ShiftDown())
            {
                ChangeCurrent(current);

                size_t lineFrom = oldCurrent,
                       lineTo = current;

                if ( lineTo < lineFrom )
                {
                    lineTo = lineFrom;
                    lineFrom = m_current;
                }

                HighlightLines(lineFrom, lineTo);
            }
            else // !ctrl, !shift
            {
                // test in the enclosing if should make it impossible
                wxFAIL_MSG( _T("how did we get here?") );
            }
        }

        if (m_current != oldCurrent)
        {
            RefreshLine( oldCurrent );
        }

        // forceClick is only set if the previous click was on another item
        m_lastOnSame = !forceClick && (m_current == oldCurrent) && oldWasSelected;
        
        if(  HasFlag(wxLCEXT_STICKY_HIGHLIGHT) && HasFlag(wxLCEXT_STICKY_NOSELEVENT) 
            	&& HasFlag(wxLCEXT_SEND_LEFTCLICK) )
            {	
            		SendNotify( current, -1,wxEVT_COMMAND_LIST_EXT_ITEM_LEFT_CLICK,
                    event.GetPosition() );
            }      
                    
        
    }
}

void wxExtListMainWindow::MoveToItem(size_t item)
{
    if ( item == (size_t)-1 )
        return;

    wxRect rect = GetLineRect(item);

    int client_w, client_h;
    GetClientSize( &client_w, &client_h );

    const int hLine = GetLineHeight();

    int view_x = SCROLL_UNIT_X*GetScrollPos( wxHORIZONTAL );
    int view_y = hLine*GetScrollPos( wxVERTICAL );

    if ( InReportView() )
    {
        // the next we need the range of lines shown it might be different, so
        // recalculate it
        ResetVisibleLinesRange();

        if (rect.y < view_y )
            Scroll( -1, rect.y/hLine );
        if (rect.y+rect.height+5 > view_y+client_h)
            Scroll( -1, (rect.y+rect.height-client_h+hLine)/hLine );
    }
    else // !report
    {
        if (rect.x-view_x < 5)
            Scroll( (rect.x-5)/SCROLL_UNIT_X, -1 );
        if (rect.x+rect.width-5 > view_x+client_w)
            Scroll( (rect.x+rect.width-client_w+SCROLL_UNIT_X)/SCROLL_UNIT_X, -1 );
    }
}

// ----------------------------------------------------------------------------
// keyboard handling
// ----------------------------------------------------------------------------

void wxExtListMainWindow::OnArrowChar(size_t newCurrent, const wxKeyEvent& event)
{
    wxCHECK_RET( newCurrent < (size_t)GetItemCount(),
                 _T("invalid item index in OnArrowChar()") );

    size_t oldCurrent = m_current;

    // in single selection we just ignore Shift as we can't select several
    // items anyhow
    if ( event.ShiftDown() && !IsSingleSel() )
    {
        ChangeCurrent(newCurrent);

        // refresh the old focus to remove it
        RefreshLine( oldCurrent );

        // select all the items between the old and the new one
        if ( oldCurrent > newCurrent )
        {
            newCurrent = oldCurrent;
            oldCurrent = m_current;
        }

        HighlightLines(oldCurrent, newCurrent);
    }
    else // !shift
    {
        // all previously selected items are unselected unless ctrl is held
        if ( !event.ControlDown() )
            HighlightAll(false);

        ChangeCurrent(newCurrent);

        // refresh the old focus to remove it
        RefreshLine( oldCurrent );

        if ( !event.ControlDown() )
        {
            HighlightLine( m_current, true );
        }
    }

    //RefreshLine( m_current );
    Refresh();
    MoveToFocus();
}

void wxExtListMainWindow::OnKeyDown( wxKeyEvent &event )
{
    wxWindow *parent = GetParent();

    /* we propagate the key event up */
    wxKeyEvent ke( wxEVT_KEY_DOWN );
    ke.m_shiftDown = event.m_shiftDown;
    ke.m_controlDown = event.m_controlDown;
    ke.m_altDown = event.m_altDown;
    ke.m_metaDown = event.m_metaDown;
    ke.m_keyCode = event.m_keyCode;
    ke.m_x = event.m_x;
    ke.m_y = event.m_y;
    ke.SetEventObject( parent );
    if (parent->GetEventHandler()->ProcessEvent( ke )) return;

    event.Skip();
}

void wxExtListMainWindow::OnChar( wxKeyEvent &event )
{
    wxWindow *parent = GetParent();

    /* we send a list_key event up */
    if ( HasCurrent() )
    {
        wxExtListEvent le( wxEVT_COMMAND_LIST_EXT_KEY_DOWN, GetParent()->GetId() );
        le.m_itemIndex = m_current;
        GetLine(m_current)->GetItem( 0, le.m_item );
        le.m_code = event.GetKeyCode();
        le.SetEventObject( parent );
        parent->GetEventHandler()->ProcessEvent( le );
    }

    /* we propagate the char event up */
    wxKeyEvent ke( wxEVT_CHAR );
    ke.m_shiftDown = event.m_shiftDown;
    ke.m_controlDown = event.m_controlDown;
    ke.m_altDown = event.m_altDown;
    ke.m_metaDown = event.m_metaDown;
    ke.m_keyCode = event.m_keyCode;
    ke.m_x = event.m_x;
    ke.m_y = event.m_y;
    ke.SetEventObject( parent );
    if (parent->GetEventHandler()->ProcessEvent( ke )) return;

    if (event.GetKeyCode() == WXK_TAB)
    {
        wxNavigationKeyEvent nevent;
        nevent.SetWindowChange( event.ControlDown() );
        nevent.SetDirection( !event.ShiftDown() );
        nevent.SetEventObject( GetParent()->GetParent() );
        nevent.SetCurrentFocus( m_parent );
        if (GetParent()->GetParent()->GetEventHandler()->ProcessEvent( nevent ))
            return;
    }

    /* no item -> nothing to do */
    if (!HasCurrent())
    {
        event.Skip();
        return;
    }

    switch (event.GetKeyCode())
    {
        case WXK_UP:
            if ( m_current > 0 )
                OnArrowChar( m_current - 1, event );
            break;

        case WXK_DOWN:
            if ( m_current < (size_t)GetItemCount() - 1 )
                OnArrowChar( m_current + 1, event );
            break;

        case WXK_END:
            if (!IsEmpty())
                OnArrowChar( GetItemCount() - 1, event );
            break;

        case WXK_HOME:
            if (!IsEmpty())
                OnArrowChar( 0, event );
            break;

        case WXK_PRIOR:
            {
                int steps = InReportView() ? m_linesPerPage - 1 : m_current % m_linesPerPage;

                int index = m_current - steps;
                if (index < 0)
                    index = 0;

                OnArrowChar( index, event );
            }
            break;

        case WXK_NEXT:
            {
                int steps = InReportView()
                               ? m_linesPerPage - 1
                               : m_linesPerPage - (m_current % m_linesPerPage) - 1;

                size_t index = m_current + steps;
                size_t count = GetItemCount();
                if ( index >= count )
                    index = count - 1;

                OnArrowChar( index, event );
            }
            break;

        case WXK_LEFT:
            if ( !InReportView() )
            {
                int index = m_current - m_linesPerPage;
                if (index < 0)
                    index = 0;

                OnArrowChar( index, event );
            }
            break;

        case WXK_RIGHT:
            if ( !InReportView() )
            {
                size_t index = m_current + m_linesPerPage;

                size_t count = GetItemCount();
                if ( index >= count )
                    index = count - 1;

                OnArrowChar( index, event );
            }
            break;

        //case WXK_SPACE:
        //    if ( IsSingleSel() )
        //    {
        //        SendNotify( m_current, m_current_column,wxEVT_COMMAND_LIST_EXT_ITEM_ACTIVATED );

        //        if ( IsHighlighted(m_current) )
        //        {
        //            // don't unselect the item in single selection mode
        //            break;
        //        }
        //        //else: select it in ReverseHighlight() below if unselected
        //    }

        //    ReverseHighlight(m_current);
        //    break;

        case WXK_RETURN:
        case WXK_EXECUTE:
            SendNotify( m_current,m_current_column, wxEVT_COMMAND_LIST_EXT_ITEM_ACTIVATED );
            break;

        default:
            event.Skip();
    }
}

// ----------------------------------------------------------------------------
// focus handling
// ----------------------------------------------------------------------------

void wxExtListMainWindow::OnSetFocus( wxFocusEvent &WXUNUSED(event) )
{
    if ( GetParent() )
    {
        wxFocusEvent event( wxEVT_SET_FOCUS, GetParent()->GetId() );
        event.SetEventObject( GetParent() );
        if ( GetParent()->GetEventHandler()->ProcessEvent( event) )
            return;
    }

    // wxGTK sends us EVT_SET_FOCUS events even if we had never got
    // EVT_KILL_FOCUS before which means that we finish by redrawing the items
    // which are already drawn correctly resulting in horrible flicker - avoid
    // it
    if ( !m_hasFocus )
    {
        m_hasFocus = true;

        RefreshSelected();
    }
}

void wxExtListMainWindow::OnKillFocus( wxFocusEvent &WXUNUSED(event) )
{
    if ( GetParent() )
    {
        wxFocusEvent event( wxEVT_KILL_FOCUS, GetParent()->GetId() );
        event.SetEventObject( GetParent() );
        if ( GetParent()->GetEventHandler()->ProcessEvent( event) )
            return;
    }
    m_hasFocus = false;
    RefreshSelected();
}

void wxExtListMainWindow::DrawImage( int index, wxDC *dc, int x, int y )
{
    if ( HasFlag(wxLCEXT_ICON) && (m_normal_image_list))
    {
        m_normal_image_list->Draw( index, *dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
    }
    else if ( HasFlag(wxLCEXT_SMALL_ICON) && (m_small_image_list))
    {
        m_small_image_list->Draw( index, *dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
    }
    else if ( HasFlag(wxLCEXT_LIST) && (m_small_image_list))
    {
        m_small_image_list->Draw( index, *dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
    }
    else if ( InReportView() && (m_small_image_list))
    {
        m_small_image_list->Draw( index, *dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
    }
}

void wxExtListMainWindow::GetImageSize( int index, int &width, int &height ) const
{
    if ( HasFlag(wxLCEXT_ICON) && m_normal_image_list )
    {
        m_normal_image_list->GetSize( index, width, height );
    }
    else if ( HasFlag(wxLCEXT_SMALL_ICON) && m_small_image_list )
    {
        m_small_image_list->GetSize( index, width, height );
    }
    else if ( HasFlag(wxLCEXT_LIST) && m_small_image_list )
    {
        m_small_image_list->GetSize( index, width, height );
    }
    else if ( InReportView() && m_small_image_list )
    {
        m_small_image_list->GetSize( index, width, height );
    }
    else
    {
        width =
        height = 0;
    }
}

int wxExtListMainWindow::GetTextLength( const wxString &s ) const
{
    wxClientDC dc( wxConstCast(this, wxExtListMainWindow) );
    dc.SetFont( GetFont() );

    wxCoord lw;
    dc.GetTextExtent( s, &lw, NULL );

    return lw + AUTOSIZE_COL_MARGIN;
}

void wxExtListMainWindow::SetImageList( wxImageListType *imageList, int which )
{
    m_dirty = true;

    // calc the spacing from the icon size
    int width = 0,
        height = 0;
    if ((imageList) && (imageList->GetImageCount()) )
    {
        imageList->GetSize(0, width, height);
    }

    if (which == wxIMAGE_LIST_NORMAL)
    {
        m_normal_image_list = imageList;
        m_normal_spacing = width + 8;
    }

    if (which == wxIMAGE_LIST_SMALL)
    {
        m_small_image_list = imageList;
        m_small_spacing = width + 14;
        m_lineHeight = 0;  // ensure that the line height will be recalc'd
    }
}

void wxExtListMainWindow::SetItemSpacing( int spacing, bool isSmall )
{
    m_dirty = true;
    if (isSmall)
    {
        m_small_spacing = spacing;
    }
    else
    {
        m_normal_spacing = spacing;
    }
}

int wxExtListMainWindow::GetItemSpacing( bool isSmall )
{
    return isSmall ? m_small_spacing : m_normal_spacing;
}

// ----------------------------------------------------------------------------
// columns
// ----------------------------------------------------------------------------

void wxExtListMainWindow::SetColumn( int col, wxExtListItem &item )
{
    wxExtListHeaderDataList::compatibility_iterator node = m_columns.Item( col );

    wxCHECK_RET( node, _T("invalid column index in SetColumn") );

    if ( item.m_width == wxLISTEXT_AUTOSIZE_USEHEADER )
        item.m_width = GetTextLength( item.m_text );

    wxExtListHeaderData *column = node->GetData();
    column->SetItem( item );

    wxExtListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;
    if ( headerWin )
        headerWin->m_dirty = true;

    m_dirty = true;

    // invalidate it as it has to be recalculated
    m_headerWidth = 0;
}
bool wxExtListMainWindow::SetColumn( int col, const wxString& label, int imageId)
{
    wxExtListItem info;
    info.m_text = label;
    info.m_mask = wxLISTEXT_MASK_TEXT;
    info.m_col = col;
    if ( imageId > -1 )
    {
        info.m_image = imageId;
        info.m_mask |= wxLISTEXT_MASK_IMAGE;
    };
    SetColumn(col,info);
    return true;
}
void wxExtListMainWindow::SetColumnWidth( int col, int width )
{
    wxCHECK_RET( col >= 0 && col < GetColumnCount(),
                 _T("invalid column index") );

    wxCHECK_RET( InReportView(),
                 _T("SetColumnWidth() can only be called in report mode.") );

    m_dirty = true;
    wxExtListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;
    if ( headerWin )
        headerWin->m_dirty = true;

    wxExtListHeaderDataList::compatibility_iterator node = m_columns.Item( col );
    wxCHECK_RET( node, _T("no column?") );

    wxExtListHeaderData *column = node->GetData();

    size_t count = GetItemCount();

    if (width == wxLISTEXT_AUTOSIZE_USEHEADER)
    {
        width = GetTextLength(column->GetText());
    }
    else if ( width == wxLISTEXT_AUTOSIZE )
    {
        if ( IsVirtual() )
        {
            // TODO: determine the max width somehow...
            width = WIDTH_COL_DEFAULT;
        }
        else // !virtual
        {
            wxClientDC dc(this);
            dc.SetFont( GetFont() );

            int max = AUTOSIZE_COL_MARGIN;

            //  if the cached column width isn't valid then recalculate it
            if (m_aColWidths.Item(col)->bNeedsUpdate)
            {
                for (size_t i = 0; i < count; i++)
                {
                    wxExtListLineData *line = GetLine(i);
                    wxExtListItemDataList::compatibility_iterator n = line->m_items.Item( col );

                    wxCHECK_RET( n, _T("no subitem?") );

                    wxExtListItemData *itemData = n->GetData();
                    wxExtListItem      item;

                    itemData->GetItem(item);
                    int itemWidth = GetItemWidthWithImage(&item);
                    if (itemWidth > max)
                        max = itemWidth;
                }

                m_aColWidths.Item(col)->bNeedsUpdate = false;
                m_aColWidths.Item(col)->nMaxWidth = max;
            }

            max = m_aColWidths.Item(col)->nMaxWidth;
            width = max + AUTOSIZE_COL_MARGIN;
        }
    }

    column->SetWidth( width );

    // invalidate it as it has to be recalculated
    m_headerWidth = 0;
}

int wxExtListMainWindow::GetHeaderWidth() const
{
    if ( !m_headerWidth )
    {
        wxExtListMainWindow *self = wxConstCast(this, wxExtListMainWindow);

        size_t count = GetColumnCount();
        for ( size_t col = 0; col < count; col++ )
        {
            self->m_headerWidth += GetColumnWidth(col);
        }
    }

    return m_headerWidth;
}

void wxExtListMainWindow::GetColumn( int col, wxExtListItem &item ) const
{
    wxExtListHeaderDataList::compatibility_iterator node = m_columns.Item( col );
    wxCHECK_RET( node, _T("invalid column index in GetColumn") );

    wxExtListHeaderData *column = node->GetData();
    column->GetItem( item );
}

int wxExtListMainWindow::GetColumnWidth( int col ) const
{
    wxExtListHeaderDataList::compatibility_iterator node = m_columns.Item( col );
    wxCHECK_MSG( node, 0, _T("invalid column index") );

    wxExtListHeaderData *column = node->GetData();
    return column->GetWidth();
}
void wxExtListMainWindow::SetColumnCtrlType( int col,int t) 
{
    wxExtListHeaderDataList::compatibility_iterator node = m_columns.Item( col );
    wxCHECK_MSG( node, , _T("invalid column index") );

    wxExtListHeaderData *column = node->GetData();
    return column->SetCtrlType(t);
}
int wxExtListMainWindow::GetColumnCtrlType( int col ) const
{
    wxExtListHeaderDataList::compatibility_iterator node = m_columns.Item( col );
    wxCHECK_MSG( node, 0, _T("invalid column index") );

    wxExtListHeaderData *column = node->GetData();
    return column->GetCtrlType();
}
// ----------------------------------------------------------------------------
// item state
// ----------------------------------------------------------------------------

void wxExtListMainWindow::SetItem( wxExtListItem &item )
{
    long id = item.m_itemId;
    wxCHECK_RET( id >= 0 && (size_t)id < GetItemCount(),
                 _T("invalid item index in SetItem") );

    if ( !IsVirtual() )
    {
        wxExtListLineData *line = GetLine((size_t)id);
        line->SetItem( item.m_col, item );

        //对指定行进行SetItemData时更新mapItemData2RowID
        if(item.m_col==0&&(item.m_mask & wxLISTEXT_MASK_DATA)==wxLISTEXT_MASK_DATA)
        {
            UpdatemapItemData2RowID_SetItemData((int)id,item.m_data);
        }


		// Set resort flag
		wxExtListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;

        // Set item state if user wants
        if ( item.m_mask & wxLISTEXT_MASK_STATE )
            SetItemState( item.m_itemId, item.m_state, item.m_state );

        if (InReportView())
        {
            //  update the Max Width Cache if needed
            int width = GetItemWidthWithImage(&item);

            if (width > m_aColWidths.Item(item.m_col)->nMaxWidth)
                m_aColWidths.Item(item.m_col)->nMaxWidth = width;
        }
    }

	// update the item on screen
    if(IsShown()&&m_freezeCount==0)
	{
        wxRect rectClient=GetClientRect();
		wxRect rectItem;
		GetSubItemRect(id,item.m_col, rectItem);
        static bool bgo=true;
        if(bgo&&rectClient.Intersects(rectItem))
		    RefreshRect(rectItem,!HasFlag(wxLCEXT_NO_REFRESH_BACKGROUND));
	}

}

void wxExtListMainWindow::SetItems( std::vector<wxExtListItem*>& items, const bool bRefreshUI)
{
    int ItemCount=GetItemCount();
    int i,isize=items.size();
    long id;
    wxRect UpdateRect;
    bool bUpdateRectEmpty=true;
    wxRect rectClient=GetClientRect();

    for(i=0;i<isize;i++)
    {
        wxExtListItem& item=*(items[i]);

        id = item.m_itemId;

        if( id < 0 || id >= ItemCount)
            continue;

        if ( !IsVirtual() )
        {
            wxExtListLineData *line = GetLine((size_t)id);
            line->SetItem( item.m_col, item );

            //对指定行进行SetItemData时更新mapItemData2RowID
            if(item.m_col==0&&(item.m_mask & wxLISTEXT_MASK_DATA)==wxLISTEXT_MASK_DATA)
            {
                UpdatemapItemData2RowID_SetItemData((int)id,item.m_data);
            }


            // Set item state if user wants
            if ( item.m_mask & wxLISTEXT_MASK_STATE )
                SetItemState( item.m_itemId, item.m_state, item.m_state );
        }

	    // update the item on screen
        static bool bgo=true;
        if(bgo&&IsShown()&&m_freezeCount==0)
	    {
		    wxRect rectItem;
		    GetSubItemRect(id,item.m_col, rectItem);
            if(rectClient.Intersects(rectItem))
            {
                if(bUpdateRectEmpty)
                {
                    UpdateRect=rectItem;
                    bUpdateRectEmpty=false;
                }
                else
                {
                    UpdateRect=UpdateRect.Union(rectItem);
                }
            }
	    }
    }

    if(bRefreshUI&&!bUpdateRectEmpty&&rectClient.Intersects(UpdateRect))
    {
        UpdateRect=UpdateRect.Intersect(rectClient);
        RefreshRect(UpdateRect,!HasFlag(wxLCEXT_NO_REFRESH_BACKGROUND));
    }
}

void wxExtListMainWindow::SetItemChecked(int id,bool check)
{
    wxCHECK_RET( id >= 0 && (size_t)id < GetItemCount(),
                 _T("invalid item index in SetItem") );
    wxExtListLineData *line = GetLine((size_t)id);
    line->m_checkboxSelected=check;
}
bool wxExtListMainWindow::GetItemChecked(int id)
{
    wxCHECK_MSG( id >= 0 && (size_t)id < GetItemCount(),
                 false,_T("invalid item index in SetItem") );
    wxExtListLineData *line = GetLine((size_t)id);
    return line->m_checkboxSelected;
}
void wxExtListMainWindow::SetItemStateAll(long state, long stateMask)
{
    if ( IsEmpty() )
        return;

    // first deal with selection
    if ( stateMask & wxLISTEXT_STATE_SELECTED )
    {
        // set/clear select state
        if ( IsVirtual() )
        {
            // optimized version for virtual listctrl.
            m_selStore.SelectRange(0, GetItemCount() - 1, state == wxLISTEXT_STATE_SELECTED);
            Refresh();
        }
        else if ( state & wxLISTEXT_STATE_SELECTED )
        {
            const long count = GetItemCount();
            for( long i = 0; i <  count; i++ )
            {
                SetItemState( i, wxLISTEXT_STATE_SELECTED, wxLISTEXT_STATE_SELECTED );
            }

        }
        else
        {
            // clear for non virtual (somewhat optimized by using GetNextItem())
            long i = -1;
            while ( (i = GetNextItem(i, wxLISTEXT_NEXT_ALL, wxLISTEXT_STATE_SELECTED)) != -1 )
            {
                SetItemState( i, 0, wxLISTEXT_STATE_SELECTED );
            }
        }
    }

    if ( HasCurrent() && (state == 0) && (stateMask & wxLISTEXT_STATE_FOCUSED) )
    {
        // unfocus all: only one item can be focussed, so clearing focus for
        // all items is simply clearing focus of the focussed item.
        SetItemState(m_current, state, stateMask);
    }
    //(setting focus to all items makes no sense, so it is not handled here.)
}

void wxExtListMainWindow::SetItemState( long litem, long state, long stateMask )
{
    if ( litem == -1 )
    {
        SetItemStateAll(state, stateMask);
        return;
    }

    wxCHECK_RET( litem >= 0 && (size_t)litem < GetItemCount(),
                 _T("invalid list ctrl item index in SetItem") );

    size_t oldCurrent = m_current;
    size_t item = (size_t)litem;    // safe because of the check above

    // do we need to change the focus?
    if ( stateMask & wxLISTEXT_STATE_FOCUSED )
    {
        if ( state & wxLISTEXT_STATE_FOCUSED )
        {
            // don't do anything if this item is already focused
            if ( item != m_current )
            {
                ChangeCurrent(item);

                if ( oldCurrent != (size_t)-1 )
                {
                    if ( IsSingleSel() )
                    {
                        HighlightLine(oldCurrent, false);
                    }

                    RefreshLine(oldCurrent);
                }

                RefreshLine( m_current );
            }
        }
        else // unfocus
        {
            // don't do anything if this item is not focused
            if ( item == m_current )
            {
                ResetCurrent();

                if ( IsSingleSel() )
                {
                    // we must unselect the old current item as well or we
                    // might end up with more than one selected item in a
                    // single selection control
                    HighlightLine(oldCurrent, false);
                }

                RefreshLine( oldCurrent );
            }
        }
    }

    // do we need to change the selection state?
    if ( stateMask & wxLISTEXT_STATE_SELECTED )
    {
        bool on = (state & wxLISTEXT_STATE_SELECTED) != 0;

        if ( IsSingleSel() )
        {
            if ( on )
            {
                // selecting the item also makes it the focused one in the
                // single sel mode
                if ( m_current != item )
                {
                    ChangeCurrent(item);

                    if ( oldCurrent != (size_t)-1 )
                    {
                        HighlightLine( oldCurrent, false );
                        RefreshLine( oldCurrent );
                    }
                }
            }
            else // off
            {
                // only the current item may be selected anyhow
                if ( item != m_current )
                    return;
            }
        }

        if ( HighlightLine(item, on) )
        {
            RefreshLine(item);
        }
    }
}

int wxExtListMainWindow::GetItemState( long item, long stateMask ) const
{
    wxCHECK_MSG( item >= 0 && (size_t)item < GetItemCount(), 0,
                 _T("invalid list ctrl item index in GetItemState()") );

    int ret = wxLISTEXT_STATE_DONTCARE;

    if ( stateMask & wxLISTEXT_STATE_FOCUSED )
    {
        if ( (size_t)item == m_current )
            ret |= wxLISTEXT_STATE_FOCUSED;
    }

    if ( stateMask & wxLISTEXT_STATE_SELECTED )
    {
        if ( IsHighlighted(item) )
            ret |= wxLISTEXT_STATE_SELECTED;
    }

    return ret;
}

void wxExtListMainWindow::GetItem( wxExtListItem &item ) const
{
    wxCHECK_RET( item.m_itemId >= 0 && (size_t)item.m_itemId < GetItemCount(),
                 _T("invalid item index in GetItem") );

    wxExtListLineData *line = GetLine((size_t)item.m_itemId);
    line->GetItem( item.m_col, item );

    // Get item state if user wants it
    if ( item.m_mask & wxLISTEXT_MASK_STATE )
        item.m_state = GetItemState( item.m_itemId, wxLISTEXT_STATE_SELECTED |
                                                 wxLISTEXT_STATE_FOCUSED );
}

// ----------------------------------------------------------------------------
// item count
// ----------------------------------------------------------------------------

size_t wxExtListMainWindow::GetItemCount() const
{
    return IsVirtual() ? m_countVirt : m_lines.GetCount();
}

void wxExtListMainWindow::SetItemCount(long count)
{
    m_selStore.SetItemCount(count);
    m_countVirt = count;

    ResetVisibleLinesRange();

    // scrollbars must be reset
    m_dirty = true;
}

int wxExtListMainWindow::GetSelectedItemCount() const
{
    // deal with the quick case first
    if ( IsSingleSel() )
    {
        return HasCurrent() ? IsHighlighted(m_current) : false;
    }

    // virtual controls remmebers all its selections itself
    if ( IsVirtual() )
        return m_selStore.GetSelectedCount();

    // TODO: we probably should maintain the number of items selected even for
    //       non virtual controls as enumerating all lines is really slow...
    size_t countSel = 0;
    size_t count = GetItemCount();
    for ( size_t line = 0; line < count; line++ )
    {
        if ( GetLine(line)->IsHighlighted() )
            countSel++;
    }

    return countSel;
}

// ----------------------------------------------------------------------------
// item position/size
// ----------------------------------------------------------------------------

wxRect wxExtListMainWindow::GetViewRect() const
{
    wxASSERT_MSG( !HasFlag(wxLCEXT_REPORT | wxLCEXT_LIST),
                    _T("wxExtListCtrl::GetViewRect() only works in icon mode") );

    // we need to find the longest/tallest label
    wxCoord xMax = 0,
            yMax = 0;
    const int count = GetItemCount();
    if ( count )
    {
        for ( int i = 0; i < count; i++ )
        {
            wxRect r;
            GetItemRect(i, r);

            wxCoord x = r.GetRight(),
                    y = r.GetBottom();

            if ( x > xMax )
                xMax = x;
            if ( y > yMax )
                yMax = y;
        }
    }

    // some fudge needed to make it look prettier
    xMax += 2*EXTRA_BORDER_X;
    yMax += 2*EXTRA_BORDER_Y;

    // account for the scrollbars if necessary
    const wxSize sizeAll = GetClientSize();
    if ( xMax > sizeAll.x )
        yMax += wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y);
    if ( yMax > sizeAll.y )
        xMax += wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);

    return wxRect(0, 0, xMax, yMax);
}

void wxExtListMainWindow::GetItemRect( long index, wxRect &rect ) const
{
    wxCHECK_RET( index >= 0 && (size_t)index < GetItemCount(),
                 _T("invalid index in GetItemRect") );

    // ensure that we're laid out, otherwise we could return nonsense
    if ( m_dirty )
    {
        wxConstCast(this, wxExtListMainWindow)->
            RecalculatePositions(true /* no refresh */);
    }

    rect = GetLineRect((size_t)index);

    CalcScrolledPosition(rect.x, rect.y, &rect.x, &rect.y);
}
void wxExtListMainWindow::GetSubItemRect( long index,int col, wxRect &rect ) const
{
    wxCHECK_RET( index >= 0 && (size_t)index < GetItemCount(),
                 _T("invalid index in GetItemRect") );
    wxCHECK_RET( col >= 0 && col < GetColumnCount(),
                 _T("invalid column in GetItemRect") );
    // ensure that we're laid out, otherwise we could return nonsense
    if ( m_dirty )
    {
        wxConstCast(this, wxExtListMainWindow)->
            RecalculatePositions(true /* no refresh */);
    }

    rect = GetLineRect((size_t)index);
    for(int cc=0;cc<col;++cc)
    {
       rect.x+=GetColumnWidth(cc);
    }
    rect.width=GetColumnWidth(col);
    CalcScrolledPosition(rect.x, rect.y, &rect.x, &rect.y);
}
bool wxExtListMainWindow::GetItemPosition(long item, wxPoint& pos) const
{
    wxRect rect;
    GetItemRect(item, rect);

    pos.x = rect.x;
    pos.y = rect.y;

    return true;
}

// ----------------------------------------------------------------------------
// geometry calculation
// ----------------------------------------------------------------------------

void wxExtListMainWindow::RecalculatePositions(bool noRefresh)
{
    wxClientDC dc( this );
    dc.SetFont( GetFont() );

    const size_t count = GetItemCount();

    int iconSpacing;
    if ( HasFlag(wxLCEXT_ICON) )
        iconSpacing = m_normal_spacing;
    else if ( HasFlag(wxLCEXT_SMALL_ICON) )
        iconSpacing = m_small_spacing;
    else
        iconSpacing = 0;

    // Note that we do not call GetClientSize() here but
    // GetSize() and subtract the border size for sunken
    // borders manually. This is technically incorrect,
    // but we need to know the client area's size WITHOUT
    // scrollbars here. Since we don't know if there are
    // any scrollbars, we use GetSize() instead. Another
    // solution would be to call SetScrollbars() here to
    // remove the scrollbars and call GetClientSize() then,
    // but this might result in flicker and - worse - will
    // reset the scrollbars to 0 which is not good at all
    // if you resize a dialog/window, but don't want to
    // reset the window scrolling. RR.
    // Furthermore, we actually do NOT subtract the border
    // width as 2 pixels is just the extra space which we
    // need around the actual content in the window. Other-
    // wise the text would e.g. touch the upper border. RR.
    int clientWidth,
        clientHeight;
    GetSize( &clientWidth, &clientHeight );

    const int lineHeight = GetLineHeight();

    if ( InReportView() )
    {
        // all lines have the same height and we scroll one line per step
        int entireHeight = count*lineHeight + LINE_SPACING;

        m_linesPerPage = clientHeight / lineHeight;

        ResetVisibleLinesRange();

        SetScrollbars( SCROLL_UNIT_X, lineHeight,
                       GetHeaderWidth() / SCROLL_UNIT_X,
                       (entireHeight + lineHeight - 1) / lineHeight,
                       GetScrollPos(wxHORIZONTAL),
                       GetScrollPos(wxVERTICAL),
                       true );
    }
    else // !report
    {
        // we have 3 different layout strategies: either layout all items
        // horizontally/vertically (wxLCEXT_ALIGN_XXX styles explicitly given) or
        // to arrange them in top to bottom, left to right (don't ask me why
        // not the other way round...) order
        if ( HasFlag(wxLCEXT_ALIGN_LEFT | wxLCEXT_ALIGN_TOP) )
        {
            int x = EXTRA_BORDER_X;
            int y = EXTRA_BORDER_Y;

            wxCoord widthMax = 0;

            size_t i;
            for ( i = 0; i < count; i++ )
            {
                wxExtListLineData *line = GetLine(i);
                line->CalculateSize( &dc, iconSpacing );
                line->SetPosition( x, y, iconSpacing );

                wxSize sizeLine = GetLineSize(i);

                if ( HasFlag(wxLCEXT_ALIGN_TOP) )
                {
                    if ( sizeLine.x > widthMax )
                        widthMax = sizeLine.x;

                    y += sizeLine.y;
                }
                else // wxLCEXT_ALIGN_LEFT
                {
                    x += sizeLine.x + MARGIN_BETWEEN_ROWS;
                }
            }

            if ( HasFlag(wxLCEXT_ALIGN_TOP) )
            {
                // traverse the items again and tweak their sizes so that they are
                // all the same in a row
                for ( i = 0; i < count; i++ )
                {
                    wxExtListLineData *line = GetLine(i);
                    line->m_gi->ExtendWidth(widthMax);
                }
            }


            SetScrollbars
            (
                SCROLL_UNIT_X,
                lineHeight,
                (x + SCROLL_UNIT_X) / SCROLL_UNIT_X,
                (y + lineHeight) / lineHeight,
                GetScrollPos( wxHORIZONTAL ),
                GetScrollPos( wxVERTICAL ),
                true
            );
        }
        else // "flowed" arrangement, the most complicated case
        {
            // at first we try without any scrollbars, if the items don't fit into
            // the window, we recalculate after subtracting the space taken by the
            // scrollbar

            int entireWidth = 0;

            for (int tries = 0; tries < 2; tries++)
            {
                entireWidth = 2*EXTRA_BORDER_X;

                if (tries == 1)
                {
                    // Now we have decided that the items do not fit into the
                    // client area, so we need a scrollbar
                    entireWidth += SCROLL_UNIT_X;
                }

                int x = EXTRA_BORDER_X;
                int y = EXTRA_BORDER_Y;
                int maxWidthInThisRow = 0;

                m_linesPerPage = 0;
                int currentlyVisibleLines = 0;

                for (size_t i = 0; i < count; i++)
                {
                    currentlyVisibleLines++;
                    wxExtListLineData *line = GetLine(i);
                    line->CalculateSize( &dc, iconSpacing );
                    line->SetPosition( x, y, iconSpacing );

                    wxSize sizeLine = GetLineSize(i);

                    if ( maxWidthInThisRow < sizeLine.x )
                        maxWidthInThisRow = sizeLine.x;

                    y += sizeLine.y;
                    if (currentlyVisibleLines > m_linesPerPage)
                        m_linesPerPage = currentlyVisibleLines;

                    if ( y + sizeLine.y >= clientHeight )
                    {
                        currentlyVisibleLines = 0;
                        y = EXTRA_BORDER_Y;
                        maxWidthInThisRow += MARGIN_BETWEEN_ROWS;
                        x += maxWidthInThisRow;
                        entireWidth += maxWidthInThisRow;
                        maxWidthInThisRow = 0;
                    }

                    // We have reached the last item.
                    if ( i == count - 1 )
                        entireWidth += maxWidthInThisRow;

                    if ( (tries == 0) &&
                            (entireWidth + SCROLL_UNIT_X > clientWidth) )
                    {
                        clientHeight -= wxSystemSettings::
                                            GetMetric(wxSYS_HSCROLL_Y);
                        m_linesPerPage = 0;
                        break;
                    }

                    if ( i == count - 1 )
                        tries = 1;  // Everything fits, no second try required.
                }
            }

            SetScrollbars
            (
                SCROLL_UNIT_X,
                lineHeight,
                (entireWidth + SCROLL_UNIT_X) / SCROLL_UNIT_X,
                0,
                GetScrollPos( wxHORIZONTAL ),
                0,
                true
            );
        }
    }

    if ( !noRefresh )
    {
        // FIXME: why should we call it from here?
        UpdateCurrent();

        RefreshAll();
    }
}

void wxExtListMainWindow::RefreshAll()
{
    m_dirty = false;
    Refresh();

    wxExtListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;
    if ( headerWin && headerWin->m_dirty )
    {
        headerWin->m_dirty = false;
        headerWin->Refresh();
    }
}

void wxExtListMainWindow::UpdateCurrent()
{
    if ( !HasCurrent() && !IsEmpty() )
    {
        ChangeCurrent(0);
    }
}

long wxExtListMainWindow::GetNextItem( long item,
                                    int WXUNUSED(geometry),
                                    int state ) const
{
    long ret = item,
         max = GetItemCount();
    wxCHECK_MSG( (ret == -1) || (ret < max), -1,
                 _T("invalid listctrl index in GetNextItem()") );

    // notice that we start with the next item (or the first one if item == -1)
    // and this is intentional to allow writing a simple loop to iterate over
    // all selected items
    ret++;
    if ( ret == max )
    {
        // this is not an error because the index was ok initially, just no
        // such item
        return -1;
    }

    if ( !state )
    {
        // any will do
        return (size_t)ret;
    }

    size_t count = GetItemCount();
    for ( size_t line = (size_t)ret; line < count; line++ )
    {
        if ( (state & wxLISTEXT_STATE_FOCUSED) && (line == m_current) )
            return line;

        if ( (state & wxLISTEXT_STATE_SELECTED) && IsHighlighted(line) )
            return line;
    }

    return -1;
}

// ----------------------------------------------------------------------------
// deleting stuff
// ----------------------------------------------------------------------------

void wxExtListMainWindow::DeleteItem( long lindex )
{
    size_t count = GetItemCount();

    wxCHECK_RET( (lindex >= 0) && ((size_t)lindex < count),
                 _T("invalid item index in DeleteItem") );

    size_t index = (size_t)lindex;

    // we don't need to adjust the index for the previous items
    if ( HasCurrent() && m_current >= index )
    {
        // if the current item is being deleted, we want the next one to
        // become selected - unless there is no next one - so don't adjust
        // m_current in this case
        if ( m_current != index || m_current == count - 1 )
        {
            m_current--;
        }
    }

    if ( InReportView() )
    {
    //  mark the Column Max Width cache as dirty if the items in the line
    //  we're deleting contain the Max Column Width
        wxExtListLineData * const line = GetLine(index);
        wxExtListItemDataList::compatibility_iterator n;
        wxExtListItemData *itemData;
        wxExtListItem      item;
        int             itemWidth;

        for (size_t i = 0; i < m_columns.GetCount(); i++)
        {
            n = line->m_items.Item( i );
            itemData = n->GetData();
            itemData->GetItem(item);

            itemWidth = GetItemWidthWithImage(&item);

            if (itemWidth >= m_aColWidths.Item(i)->nMaxWidth)
                m_aColWidths.Item(i)->bNeedsUpdate = true;
        }

        ResetVisibleLinesRange();
    }

    if ( IsVirtual() )
    {
        m_countVirt--;

        m_selStore.OnItemDelete(index);
    }
    else
    {
        UpdatemapItemData2RowID_BeforeDeleteItem(index);

        m_lines.RemoveAt( index );

        for(size_t i=index;i<m_lines.Count();i++)
            m_lines[i].m_RowID=i;
    }

    // we need to refresh the (vert) scrollbar as the number of items changed
    m_dirty = true;

    SendNotify( index,-1, wxEVT_COMMAND_LIST_EXT_DELETE_ITEM );

    RefreshAfter(index);
}

void wxExtListMainWindow::DeleteColumn( int col )
{
    wxExtListHeaderDataList::compatibility_iterator node = m_columns.Item( col );

    wxCHECK_RET( node, wxT("invalid column index in DeleteColumn()") );

    m_dirty = true;
    delete node->GetData();
    m_columns.Erase( node );

    if ( !IsVirtual() )
    {
        // update all the items
        for ( size_t i = 0; i < m_lines.GetCount(); i++ )
        {
            wxExtListLineData * const line = GetLine(i);
            wxExtListItemDataList::compatibility_iterator n = line->m_items.Item( col );
            delete n->GetData();
            line->m_items.Erase(n);
        }
    }

    if ( InReportView() )   //  we only cache max widths when in CTPReport View
    {
        delete m_aColWidths.Item(col);
        m_aColWidths.RemoveAt(col);
    }

    // invalidate it as it has to be recalculated
    m_headerWidth = 0;

	wxExtListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;
	if ( headerWin)
	{
		if ( headerWin->m_sort_column == col )
		{
            if(headerWin->m_bsort)
                headerWin->m_bsort = false;
			headerWin->m_sort_column = -1;
		}
		else if ( headerWin->m_sort_column > col )
		{
			headerWin->m_sort_column--;
		}
	}
}

void wxExtListMainWindow::DoDeleteAllItems()
{
    if ( IsEmpty() )
    {
        // nothing to do - in particular, don't send the event
        return;
    }

    ResetCurrent();

    // to make the deletion of all items faster, we don't send the
    // notifications for each item deletion in this case but only one event
    // for all of them: this is compatible with wxMSW and documented in
    // DeleteAllItems() description

    wxExtListEvent event( wxEVT_COMMAND_LIST_EXT_DELETE_ALL_ITEMS, GetParent()->GetId() );
    event.SetEventObject( GetParent() );
    GetParent()->GetEventHandler()->ProcessEvent( event );

    if ( IsVirtual() )
    {
        m_countVirt = 0;

        m_selStore.Clear();
    }

    if ( InReportView() )
    {
        ResetVisibleLinesRange();
        for (size_t i = 0; i < m_aColWidths.GetCount(); i++)
        {
            m_aColWidths.Item(i)->bNeedsUpdate = true;
        }
    }

    m_lines.Clear();
}

void wxExtListMainWindow::DeleteAllItems()
{
    DoDeleteAllItems();

    RecalculatePositions();
}

void wxExtListMainWindow::DeleteEverything()
{
    WX_CLEAR_LIST(wxExtListHeaderDataList, m_columns);
    WX_CLEAR_ARRAY(m_aColWidths);

    DeleteAllItems();
}

// ----------------------------------------------------------------------------
// scanning for an item
// ----------------------------------------------------------------------------

void wxExtListMainWindow::EnsureVisible( long index )
{
    wxCHECK_RET( index >= 0 && (size_t)index < GetItemCount(),
                 _T("invalid index in EnsureVisible") );

    // We have to call this here because the label in question might just have
    // been added and its position is not known yet
    if ( m_dirty )
    {
        RecalculatePositions(true /* no refresh */);
    }

    MoveToItem((size_t)index);
}

long wxExtListMainWindow::FindItem(long start, const wxString& str, bool WXUNUSED(partial) )
{
    long pos = start;
    wxString tmp = str;
    if (pos < 0)
        pos = 0;

    size_t count = GetItemCount();
    for ( size_t i = (size_t)pos; i < count; i++ )
    {
        wxExtListLineData *line = GetLine(i);
        if ( line->GetText(0) == tmp )
            return i;
    }

    return wxNOT_FOUND;
}

long wxExtListMainWindow::FindItem(long start, wxUIntPtr data)
{
    long pos = start;
    if (pos < 0)
        pos = 0;

    size_t count = GetItemCount();
    for (size_t i = (size_t)pos; i < count; i++)
    {
        wxExtListLineData *line = GetLine(i);
        wxExtListItem item;
        line->GetItem( 0, item );
        if (item.m_data == data)
            return i;
    }

    return wxNOT_FOUND;
}

long wxExtListMainWindow::FindItem( const wxPoint& pt )
{
    size_t topItem;
    GetVisibleLinesRange(&topItem, NULL);

    wxPoint p;
    GetItemPosition( GetItemCount()-1, p );
    if( p.y == 0 )
        return topItem;
    long id = (long) floor( pt.y*double(GetItemCount()-topItem-1)/p.y+topItem );
    if( id >= 0 && id < (long)GetItemCount() )
        return id;

    return wxNOT_FOUND;
}

long wxExtListMainWindow::HitTest( int x, int y, int &flags )
{
    CalcUnscrolledPosition( x, y, &x, &y );

    size_t count = GetItemCount();

    if ( InReportView() )
    {
        size_t current = y / GetLineHeight();
        if ( current < count )
        {
            flags = HitTestLine(current, x, y);
            if ( flags )
                return current;
        }
    }
    else // !report
    {
        // TODO: optimize it too! this is less simple than for report view but
        //       enumerating all items is still not a way to do it!!
        for ( size_t current = 0; current < count; current++ )
        {
            flags = HitTestLine(current, x, y);
            if ( flags )
                return current;
        }
    }

    return wxNOT_FOUND;
}

// ----------------------------------------------------------------------------
// adding stuff
// ----------------------------------------------------------------------------

void wxExtListMainWindow::InsertItem( wxExtListItem &item )
{
    wxASSERT_MSG( !IsVirtual(), _T("can't be used with virtual control") );

    int count = GetItemCount();
    wxCHECK_RET( item.m_itemId >= 0, _T("invalid item index") );

    if (item.m_itemId > count)
        item.m_itemId = count;

    size_t id = item.m_itemId;

    m_dirty = true;

    if ( InReportView() )
    {
        ResetVisibleLinesRange();

        // calculate the width of the item and adjust the max column width
        wxColWidthInfo *pWidthInfo = m_aColWidths.Item(item.GetColumn());
        int width = GetItemWidthWithImage(&item);
        item.SetWidth(width);
        if (width > pWidthInfo->nMaxWidth)
            pWidthInfo->nMaxWidth = width;
    }

    wxExtListLineData *line = new wxExtListLineData(this);

    line->SetItem( item.m_col, item );

    //每行都有一个唯一的ID，插入一行时此值加一
    line->m_LineID=wxExtListLineData::m_LineIDbase++;
    line->m_RowID=id;

    m_lines.Insert( line, id );

    UpdatemapItemData2RowID_AfterInsertItem(id);

    m_dirty = true;

    // If an item is selected at or below the point of insertion, we need to
    // increment the member variables because the current row's index has gone
    // up by one
    if ( HasCurrent() && m_current >= id )
    {
        m_current++;
    }

    SendNotify(id,-1, wxEVT_COMMAND_LIST_EXT_INSERT_ITEM);

    RefreshLines(id, GetItemCount() - 1);
}

void wxExtListMainWindow::InsertItem2( wxExtListItem &item, const bool bRefreshUI )
{
    wxASSERT_MSG( !IsVirtual(), _T("can't be used with virtual control") );

    int count = GetItemCount();
    wxCHECK_RET( item.m_itemId >= 0, _T("invalid item index") );

    if (item.m_itemId > count)
        item.m_itemId = count;

    size_t id = item.m_itemId;

    m_dirty = true;

    if ( InReportView() )
    {
        ResetVisibleLinesRange();

        // calculate the width of the item and adjust the max column width
        wxColWidthInfo *pWidthInfo = m_aColWidths.Item(item.GetColumn());
        int width = GetItemWidthWithImage(&item);
        item.SetWidth(width);
        if (width > pWidthInfo->nMaxWidth)
            pWidthInfo->nMaxWidth = width;
    }

    wxExtListLineData *line = new wxExtListLineData(this);


    line->SetItem( item.m_col, item );

    //每行都有一个唯一的ID，插入一行时此值加一
    line->m_LineID=wxExtListLineData::m_LineIDbase++;
    line->m_RowID=id;

    m_lines.Insert( line, id );

    UpdatemapItemData2RowID_AfterInsertItem(id);

    m_dirty = true;

    // If an item is selected at or below the point of insertion, we need to
    // increment the member variables because the current row's index has gone
    // up by one
    if ( HasCurrent() && m_current >= id )
    {
        m_current++;
    }

    SendNotify(id,-1, wxEVT_COMMAND_LIST_EXT_INSERT_ITEM);

    if(bRefreshUI)
    {
        RefreshLines(id, GetItemCount() - 1);
    }
}

void wxExtListMainWindow::InsertColumn( long col, wxExtListItem &item )
{
    m_dirty = true;
    if ( InReportView() )
    {
        if (item.m_width == wxLISTEXT_AUTOSIZE_USEHEADER)
            item.m_width = GetTextLength( item.m_text );

        wxExtListHeaderData *column = new wxExtListHeaderData( item );
        wxColWidthInfo *colWidthInfo = new wxColWidthInfo();

        bool insert = (col >= 0) && ((size_t)col < m_columns.GetCount());
        if ( insert )
        {
            wxExtListHeaderDataList::compatibility_iterator
                node = m_columns.Item( col );
            m_columns.Insert( node, column );
            m_aColWidths.Insert( colWidthInfo, col );
        }
        else
        {
            m_columns.Append( column );
            m_aColWidths.Add( colWidthInfo );
        }

        if ( !IsVirtual() )
        {
            // update all the items
            for ( size_t i = 0; i < m_lines.GetCount(); i++ )
            {
                wxExtListLineData * const line = GetLine(i);
                wxExtListItemData * const data = new wxExtListItemData(this);
                if ( insert )
                    line->m_items.Insert(col, data);
                else
                    line->m_items.Append(data);
            }
        }

        // invalidate it as it has to be recalculated
        m_headerWidth = 0;

		wxExtListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;
		if ( headerWin )
		{
			if ( headerWin->m_sort_column >= col )
			{
				headerWin->m_sort_column++;
			}
		}
		
    }
}

int wxExtListMainWindow::GetItemWidthWithImage(wxExtListItem * item)
{
    int width = 0;
    wxClientDC dc(this);

    dc.SetFont( GetFont() );

    if (item->GetImage() != -1)
    {
        int ix, iy;
        GetImageSize( item->GetImage(), ix, iy );
        width += ix + 5;
    }

    if (!item->GetText().empty())
    {
        wxCoord w;
        dc.GetTextExtent( item->GetText(), &w, NULL );
        width += w;
    }

    return width;
}

// ----------------------------------------------------------------------------
// sorting
// 注意，先按指定列排序，如果值相同，按行的LineID排序，如果还相同，返回1，表示保持原有顺序
// ----------------------------------------------------------------------------

wxExtListCtrlCompare ext_list_ctrl_compare_func_2;
int               ext_list_ctrl_compare_column;  
long              ext_list_ctrl_compare_data;

int LINKAGEMODE list_ctrl_compare_func_1( wxExtListLineData **arg1, wxExtListLineData **arg2 )
{
    wxExtListLineData *line1 = *arg1;
    wxExtListLineData *line2 = *arg2;
    wxExtListItem item1;
    wxExtListItem item2;
    if(ext_list_ctrl_compare_column>=0)
    {
        line1->GetItem( ext_list_ctrl_compare_column, item1 );
        item1.m_itemId=line1->m_LineID;             //用于指定列值相等时的排序
        line2->GetItem( ext_list_ctrl_compare_column, item2 );
        item2.m_itemId=line2->m_LineID;             //用户指定列值相等时的排序
        //     wxUIntPtr data2 = item2.m_data;

        return ext_list_ctrl_compare_func_2( item1, item2, ext_list_ctrl_compare_data );
    }
    return 0;

}

void wxExtListMainWindow::SortItems( wxExtListCtrlCompare fn, int column,long data )
{
    ext_list_ctrl_compare_func_2 = fn;
    ext_list_ctrl_compare_data = data;
    ext_list_ctrl_compare_column=column;
    
    wxArrayInt LineIDArray;
    for(size_t i=0;i<m_lines.GetCount();++i)
        LineIDArray.push_back(m_lines[i].m_LineID);

    m_lines.Sort( list_ctrl_compare_func_1 );

    size_t i;
    for(i=0;i<m_lines.GetCount();++i)
        m_lines[i].m_RowID=i;
    
    UpdatemapItemData2RowID_AfterResort();

    for(size_t i=0;i<m_lines.GetCount();++i)
    {
        if(LineIDArray[i]!=m_lines[i].m_LineID)
        {
            m_dirty = true;
            break;
        }
    }
}

void wxExtListMainWindow::SetSortColData( long data )
{
	m_bakSortColData = data;
}

// ----------------------------------------------------------------------------
// scrolling
// ----------------------------------------------------------------------------

void wxExtListMainWindow::OnScroll(wxScrollWinEvent& event)
{

    // update our idea of which lines are shown when we redraw the window the
    // next time
    ResetVisibleLinesRange();

    // FIXME
#if ( defined(__WXGTK__) || defined(__WXMAC__) ) && !defined(__WXUNIVERSAL__)
    wxScrolledWindow::OnScroll(event);
#else
    HandleOnScroll( event );
#endif

    if ( event.GetOrientation() == wxHORIZONTAL && HasHeader() )
    {
        wxExtGenericListCtrl* lc = GetListCtrl();
        wxCHECK_RET( lc, _T("no listctrl window?") );

        lc->m_headerWin->Refresh();
        lc->m_headerWin->Update();
    }

    ::UpdateWindow((HWND)m_hWnd);


}

int wxExtListMainWindow::GetCountPerPage() const
{
    if ( !m_linesPerPage )
    {
        wxConstCast(this, wxExtListMainWindow)->
            m_linesPerPage = GetClientSize().y / GetLineHeight();
    }

    return m_linesPerPage;
}

void wxExtListMainWindow::GetVisibleLinesRange(size_t *from, size_t *to)
{
    wxASSERT_MSG( InReportView(), _T("this is for report mode only") );

    if ( m_lineFrom == (size_t)-1 )
    {
        size_t count = GetItemCount();
        if ( count )
        {
            m_lineFrom = GetScrollPos(wxVERTICAL);

            // this may happen if SetScrollbars() hadn't been called yet
            if ( m_lineFrom >= count )
                m_lineFrom = count - 1;

            // we redraw one extra line but this is needed to make the redrawing
            // logic work when there is a fractional number of lines on screen
            m_lineTo = m_lineFrom + m_linesPerPage;
            if ( m_lineTo >= count )
                m_lineTo = count - 1;
        }
        else // empty control
        {
            m_lineFrom = 0;
            m_lineTo = (size_t)-1;
        }
    }

    wxASSERT_MSG( IsEmpty() ||
                  (m_lineFrom <= m_lineTo && m_lineTo < GetItemCount()),
                  _T("GetVisibleLinesRange() returns incorrect result") );

    if ( from )
        *from = m_lineFrom;
    if ( to )
        *to = m_lineTo;
}

// -------------------------------------------------------------------------------------
// wxExtGenericListCtrl
// -------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(wxExtGenericListCtrl, wxControl)

BEGIN_EVENT_TABLE(wxExtGenericListCtrl,wxControl)
EVT_SIZE(wxExtGenericListCtrl::OnSize)
EVT_CONTEXT_MENU(wxExtGenericListCtrl::OnContextMenu)
EVT_LIST_EXT_COL_RIGHT_CLICK(wxID_ANY, wxExtGenericListCtrl::OnColumnRightClick)
    EVT_PAINT(wxExtGenericListCtrl::OnPaint)
END_EVENT_TABLE()

void wxExtGenericListCtrl::OnPaint(wxPaintEvent& evt)
{
    HWND hChild;
    if(m_mainWin!=NULL&&
        (hChild=(HWND)m_mainWin->m_hWnd)!=NULL&&
        ::IsWindow(hChild)) 
    {
        //如果更新区域包含了m_mainWind的非客户区，则重画其非客户区，确保该窗口被更新。
        //否则可能出现该窗口非客户区得不到更新的情况，尤其是滚动条
        int irlt;
        HRGN UpdateRgn=::CreateRectRgn(0,0,0,0);
        irlt=::GetUpdateRgn((HWND)m_hWnd,UpdateRgn,false);
        if(irlt!=ERROR&&irlt!=NULLREGION)
        {
            RECT ChildRect;
            ::GetWindowRect(hChild,&ChildRect);
            POINT Pt;
            Pt.x=ChildRect.left;
            Pt.y=ChildRect.top;
            ::ScreenToClient((HWND)m_hWnd,&Pt);

            HRGN ChildWinRgn=::CreateRectRgn(Pt.x,Pt.y,ChildRect.right-ChildRect.left+Pt.x,ChildRect.bottom-ChildRect.top+Pt.y);

            RECT ChildClientRect;
            ::GetClientRect(hChild,&ChildClientRect);
            Pt.x=0;
            Pt.y=0;
            ::ClientToScreen(hChild,&Pt);
            ::ScreenToClient((HWND)m_hWnd,&Pt);
            HRGN ChildClientRgn=::CreateRectRgn(Pt.x,Pt.y,ChildClientRect.right-ChildClientRect.left+Pt.x,ChildClientRect.bottom-ChildClientRect.top+Pt.y);

            HRGN NCRgn=::CreateRectRgn(0,0,0,0);
            irlt=::CombineRgn(NCRgn,ChildWinRgn,ChildClientRgn,RGN_DIFF);

            if(irlt!=ERROR&&irlt!=NULLREGION)
            {
                HRGN DestRgn=::CreateRectRgn(0,0,0,0);
                irlt=::CombineRgn(DestRgn,NCRgn,UpdateRgn,RGN_AND);
                if(irlt!=ERROR&&irlt!=NULLREGION)
                {
                    RECT Rect;
                    ::GetRgnBox(DestRgn,&Rect);
                    ::SendMessage(hChild,WM_NCPAINT,0,0);
                }
                ::DeleteObject(DestRgn);
            }

            ::DeleteObject(NCRgn);
            ::DeleteObject(ChildClientRgn);
            ::DeleteObject(ChildWinRgn);
        }
        ::DeleteObject(UpdateRgn);
    }


    evt.Skip();    
};

wxExtGenericListCtrl::wxExtGenericListCtrl()
{
    m_imageListNormal = (wxImageListType *) NULL;
    m_imageListSmall = (wxImageListType *) NULL;
    m_imageListState = (wxImageListType *) NULL;

    m_ownsImageListNormal =
    m_ownsImageListSmall =
    m_ownsImageListState = false;
    m_bShowContextMenu=-1;
    m_mainWin = (wxExtListMainWindow*) NULL;
    m_headerWin = (wxExtListHeaderWindow*) NULL;
    m_headerHeight = 0;
}

wxExtGenericListCtrl::~wxExtGenericListCtrl()
{
    if (m_ownsImageListNormal)
        delete m_imageListNormal;
    if (m_ownsImageListSmall)
        delete m_imageListSmall;
    if (m_ownsImageListState)
        delete m_imageListState;
}
void wxExtGenericListCtrl::OnContextMenu(wxContextMenuEvent& event)
{
	wxContextMenuEvent evt(wxEVT_COMMAND_LIST_EXT_CONTEXT_MENU);
	evt.SetId(1);
	ProcessEvent(evt);
}
void wxExtGenericListCtrl::OnColumnRightClick(wxExtListEvent& event)
{ 
	wxContextMenuEvent evt(wxEVT_COMMAND_LIST_EXT_CONTEXT_MENU);
	evt.SetId(0);
	ProcessEvent(evt);
    event.Skip();
}
void wxExtGenericListCtrl::CalculateAndSetHeaderHeight()
{
    if ( m_headerWin )
    {
#ifdef __WXMAC__
        SInt32 h ;
        GetThemeMetric( kThemeMetricListHeaderHeight, &h );
#else
        // we use 'g' to get the descent, too
        int w, h, d;
        m_headerWin->GetTextExtent(wxT("Hg"), &w, &h, &d);
        h += d + 2 * HEADER_OFFSET_Y + EXTRA_HEIGHT;
#endif
        // only update if changed
        if ( h != m_headerHeight )
        {
            m_headerHeight = h;

            if ( HasHeader() )
                ResizeReportView(true);
            else    //why is this needed if it doesn't have a header?
                m_headerWin->SetSize(m_headerWin->GetSize().x, m_headerHeight);
        }
    }
}

void wxExtGenericListCtrl::CreateHeaderWindow()
{
    m_headerWin = new wxExtListHeaderWindow
                      (
                        this, wxID_ANY, m_mainWin,
                        wxPoint(0,0),
                        wxSize(GetClientSize().x, m_headerHeight),
                        wxTAB_TRAVERSAL
                      );
    CalculateAndSetHeaderHeight();
}

bool wxExtGenericListCtrl::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint &pos,
                        const wxSize &size,
                        long style,
                        const wxValidator &validator,
                        const wxString &name)
{
    m_imageListNormal =
    m_imageListSmall =
    m_imageListState = (wxImageListType *) NULL;
    m_ownsImageListNormal =
    m_ownsImageListSmall =
    m_ownsImageListState = false;

    m_mainWin = (wxExtListMainWindow*) NULL;
    m_headerWin = (wxExtListHeaderWindow*) NULL;

    m_headerHeight = 0;

    if ( !(style & wxLCEXT_MASK_TYPE) )
    {
        style = style | wxLCEXT_LIST;
    }

    //这里，不要边框，否则可能出现边框得不到更新的情况(wxWedgits的刷新问题)
    if((style & wxBORDER_MASK) ==0) 
        style |= wxBORDER_NONE;
    if ( !wxControl::Create( parent, id, pos, size, style, validator, name ) )
        return false;

    // don't create the inner window with the border
    style &= ~wxBORDER_MASK;

    m_mainWin = new wxExtListMainWindow( this, wxID_ANY, wxPoint(0,0), size, style );

#ifdef  __WXMAC_CARBON__
    // Human Interface Guidelines ask us for a special font in this case
    if ( GetWindowVariant() == wxWINDOW_VARIANT_NORMAL )
    {
        wxFont font ;
        font.MacCreateThemeFont( kThemeViewsFont ) ;
        SetFont( font ) ;
    }
#endif
    if ( InReportView() )
    {
        CreateHeaderWindow();
#ifdef  __WXMAC_CARBON__
        if (m_headerWin)
        {
            wxFont font ;
            font.MacCreateThemeFont( kThemeSmallSystemFont ) ;
            m_headerWin->SetFont( font );
            CalculateAndSetHeaderHeight();
        }
#endif
        if ( HasFlag(wxLCEXT_NO_HEADER) )
        {
            // VZ: why do we create it at all then?
            m_headerWin->Show( false );
        }
    }
#ifdef __WX_R2_6__
SetBestSize(size);
#else
SetInitialSize(size);
#endif

    return true;
}

void wxExtGenericListCtrl::SetSingleStyle( long style, bool add )
{
    wxASSERT_MSG( !(style & wxLCEXT_VIRTUAL),
                  _T("wxLCEXT_VIRTUAL can't be [un]set") );

    long flag = GetWindowStyle();

    if (add)
    {
        if (style & wxLCEXT_MASK_TYPE)
            flag &= ~(wxLCEXT_MASK_TYPE | wxLCEXT_VIRTUAL);
        if (style & wxLCEXT_MASK_ALIGN)
            flag &= ~wxLCEXT_MASK_ALIGN;
        if (style & wxLCEXT_MASK_SORT)
            flag &= ~wxLCEXT_MASK_SORT;
    }

    if (add)
    {
        flag |= style;
    }
    else
    {
        flag &= ~style;
    }

    SetWindowStyleFlag( flag );
}
void wxExtGenericListCtrl::SetTextCtrlMaxLength(int len)
{
    if (m_mainWin)
    {
		m_mainWin->SetTextCtrlMaxLength(len);
	}
}
void wxExtGenericListCtrl::SetWindowStyleFlag( long flag )
{
    if (m_mainWin)
    {
        m_mainWin->DeleteEverything();

        // has the header visibility changed?
        bool hasHeader = HasHeader();
        bool willHaveHeader = (flag & wxLCEXT_REPORT) && !(flag & wxLCEXT_NO_HEADER);

        if ( hasHeader != willHaveHeader )
        {
            // toggle it
            if ( hasHeader )
            {
                if ( m_headerWin )
                {
                    // don't delete, just hide, as we can reuse it later
                    m_headerWin->Show(false);
                }
                //else: nothing to do
            }
            else // must show header
            {
                if (!m_headerWin)
                {
                    CreateHeaderWindow();
                }
                else // already have it, just show
                {
                    m_headerWin->Show( true );
                }
            }

            ResizeReportView(willHaveHeader);
        }
    }

    wxWindow::SetWindowStyleFlag( flag );
}

bool wxExtGenericListCtrl::GetColumn(int col, wxExtListItem &item) const
{
    m_mainWin->GetColumn( col, item );
    return true;
}

bool wxExtGenericListCtrl::SetColumn( int col, wxExtListItem& item )
{
    m_mainWin->SetColumn( col, item );
    return true;
}
bool wxExtGenericListCtrl::SetColumn( int col, const wxString& label, int imageId)
{
    m_mainWin->SetColumn( col, label, imageId);
    return true;
}
int wxExtGenericListCtrl::GetColumnWidth( int col ) const
{
    return m_mainWin->GetColumnWidth( col );
}

bool wxExtGenericListCtrl::SetColumnWidth( int col, int width )
{
    m_mainWin->SetColumnWidth( col, width );
    return true;
}

int wxExtGenericListCtrl::GetCountPerPage() const
{
  return m_mainWin->GetCountPerPage();  // different from Windows ?
}
wxString wxExtGenericListCtrl::GetSubItemText(int index, int col)
{
    wxExtListItem item;
    wxString strText;
    item.SetMask(wxLISTEXT_MASK_TEXT);
    item.SetId( index );
    item.SetColumn( col );
    if(GetItem(item))
    {
        strText=item.GetText();
    }
    return strText;
}
bool wxExtGenericListCtrl::GetItem( wxExtListItem &info ) const
{
    m_mainWin->GetItem( info );
    return true;
}
void wxExtGenericListCtrl::SetItemChecked(int id,bool check)
{
    m_mainWin->SetItemChecked(id,check);
}
bool wxExtGenericListCtrl::GetItemChecked(int id)
{
    return m_mainWin->GetItemChecked(id);
}
bool wxExtGenericListCtrl::SetItem( wxExtListItem &info )
{
	m_mainWin->SetItem( info );
	return true;
}

bool wxExtGenericListCtrl::SetItems( std::vector<wxExtListItem*>& infos, const bool bRefreshUI)
{
	m_mainWin->SetItems( infos, bRefreshUI );
	return true;
}

long wxExtGenericListCtrl::SetItem( long index, int col, const wxString& label, int imageId )
{
    wxExtListItem info;
    info.m_text = label;
    info.m_mask = wxLISTEXT_MASK_TEXT;
    info.m_itemId = index;
    info.m_col = col;
    if ( imageId > -1 )
    {
        info.m_image = imageId;
        info.m_mask |= wxLISTEXT_MASK_IMAGE;
    };
    m_mainWin->SetItem(info);
    return true;
}

int wxExtGenericListCtrl::GetItemState( long item, long stateMask ) const
{
    return m_mainWin->GetItemState( item, stateMask );
}

bool wxExtGenericListCtrl::SetItemState( long item, long state, long stateMask )
{
    m_mainWin->SetItemState( item, state, stateMask );
    return true;
}
int wxExtGenericListCtrl::GetColumnCtrlType( int col ) const
{
    return m_mainWin->GetColumnCtrlType( col);
}
void wxExtGenericListCtrl::SetColumnCtrlType( int col, int t)
{
    m_mainWin->SetColumnCtrlType(col,t);
}
void wxExtGenericListCtrl::SetOddRowColour(wxColour clr)
{
    m_mainWin->SetOddRowColour(clr);
}
wxColour wxExtGenericListCtrl::GetOddRowColour()
{
    return m_mainWin->GetOddRowColour();
}
void wxExtGenericListCtrl::SetEvenRowColour(wxColour clr)
{
    m_mainWin->SetEvenRowColour(clr);
}
wxColour wxExtGenericListCtrl::GetEvenRowColour()
{
    return m_mainWin->GetEvenRowColour();
}
bool
wxExtGenericListCtrl::SetItemImage( long item, int image, int WXUNUSED(selImage) )
{
    return SetItemColumnImage(item, 0, image);
}

#if wxABI_VERSION >= 20603
bool
wxExtGenericListCtrl::SetItemColumnImage( long item, long column, int image )
{
    wxExtListItem info;
    info.m_image = image;
    info.m_mask = wxLISTEXT_MASK_IMAGE;
    info.m_itemId = item;
    info.m_col = column;
    m_mainWin->SetItem( info );
    return true;
}
#endif

wxString wxExtGenericListCtrl::GetItemText( long item ) const
{
    return m_mainWin->GetItemText(item);
}

void wxExtGenericListCtrl::SetItemText( long item, const wxString& str )
{
    m_mainWin->SetItemText(item, str);
}

wxUIntPtr wxExtGenericListCtrl::GetItemData( long item ) const
{
    wxExtListItem info;
    info.m_mask = wxLISTEXT_MASK_DATA;
    info.m_itemId = item;
    m_mainWin->GetItem( info );
    return info.m_data;
}

bool wxExtGenericListCtrl::SetItemData( long item, long data )
{
    wxExtListItem info;
    info.m_mask = wxLISTEXT_MASK_DATA;
    info.m_itemId = item;
    info.m_data = data;
    m_mainWin->SetItem( info );
    return true;
}

wxRect wxExtGenericListCtrl::GetViewRect() const
{
    return m_mainWin->GetViewRect();
}

bool wxExtGenericListCtrl::GetItemRect( long item, wxRect &rect,  int WXUNUSED(code) ) const
{
    m_mainWin->GetItemRect( item, rect );
    if ( m_mainWin->HasHeader() )
        rect.y += m_headerHeight + 1;
    return true;
}

bool wxExtGenericListCtrl::GetItemPosition( long item, wxPoint& pos ) const
{
    m_mainWin->GetItemPosition( item, pos );
    return true;
}

bool wxExtGenericListCtrl::SetItemPosition( long WXUNUSED(item), const wxPoint& WXUNUSED(pos) )
{
    return 0;
}

int wxExtGenericListCtrl::GetItemCount() const
{
    return m_mainWin->GetItemCount();
}

int wxExtGenericListCtrl::GetColumnCount() const
{
    return m_mainWin->GetColumnCount();
}

void wxExtGenericListCtrl::SetItemSpacing( int spacing, bool isSmall )
{
    m_mainWin->SetItemSpacing( spacing, isSmall );
}

wxSize wxExtGenericListCtrl::GetItemSpacing() const
{
    const int spacing = m_mainWin->GetItemSpacing(HasFlag(wxLCEXT_SMALL_ICON));

    return wxSize(spacing, spacing);
}

int wxExtGenericListCtrl::GetItemSpacing( bool isSmall ) const
{
    return m_mainWin->GetItemSpacing( isSmall );
}

void wxExtGenericListCtrl::SetItemTextColour( long item, const wxColour &col )
{
    wxExtListItem info;
    info.m_itemId = item;
    info.SetTextColour( col );
    m_mainWin->SetItem( info );
}

wxColour wxExtGenericListCtrl::GetItemTextColour( long item ) const
{
    wxExtListItem info;
    info.m_itemId = item;
    m_mainWin->GetItem( info );
    return info.GetTextColour();
}

void wxExtGenericListCtrl::SetItemBackgroundColour( long item, const wxColour &col )
{
    wxExtListItem info;
    info.m_itemId = item;
    info.SetBackgroundColour( col );
    m_mainWin->SetItem( info );
}

wxColour wxExtGenericListCtrl::GetItemBackgroundColour( long item ) const
{
    wxExtListItem info;
    info.m_itemId = item;
    m_mainWin->GetItem( info );
    return info.GetBackgroundColour();
}
void wxExtGenericListCtrl::SetHeaderBackgroundColour( const wxColour &col )
{
   m_headerWin->m_backgroundcolor=col;
}

wxColour wxExtGenericListCtrl::GetHeaderBackgroundColour() const
{
   return    m_headerWin->m_backgroundcolor;
}
void wxExtGenericListCtrl::GetSubItemRect( long index,int col, wxRect &rect ) const
{
    m_mainWin->GetSubItemRect(index,col,rect);
}
void wxExtGenericListCtrl::SetItemFont( long item, const wxFont &f )
{
    wxExtListItem info;
    info.m_itemId = item;
    info.SetFont( f );
    m_mainWin->SetItem( info );
}

wxFont wxExtGenericListCtrl::GetItemFont( long item ) const
{
    wxExtListItem info;
    info.m_itemId = item;
    m_mainWin->GetItem( info );
    return info.GetFont();
}

int wxExtGenericListCtrl::GetSelectedItemCount() const
{
    return m_mainWin->GetSelectedItemCount();
}
bool wxExtGenericListCtrl::IsSelectedItem(size_t line) const
{
    return m_mainWin->IsHighlighted(line);
}
wxColour wxExtGenericListCtrl::GetTextColour() const
{
    return GetForegroundColour();
}

void wxExtGenericListCtrl::SetTextColour(const wxColour& col)
{
    SetForegroundColour(col);
}

long wxExtGenericListCtrl::GetTopItem() const
{
    size_t top;
    m_mainWin->GetVisibleLinesRange(&top, NULL);
    return (long)top;
}

long wxExtGenericListCtrl::GetNextItem( long item, int geom, int state ) const
{
    return m_mainWin->GetNextItem( item, geom, state );
}

wxImageListType *wxExtGenericListCtrl::GetImageList(int which) const
{
    if (which == wxIMAGE_LIST_NORMAL)
    {
        return m_imageListNormal;
    }
    else if (which == wxIMAGE_LIST_SMALL)
    {
        return m_imageListSmall;
    }
    else if (which == wxIMAGE_LIST_STATE)
    {
        return m_imageListState;
    }
    return (wxImageListType *) NULL;
}

void wxExtGenericListCtrl::SetImageList( wxImageListType *imageList, int which )
{
    if ( which == wxIMAGE_LIST_NORMAL )
    {
        if (m_ownsImageListNormal) delete m_imageListNormal;
        m_imageListNormal = imageList;
        m_ownsImageListNormal = false;
    }
    else if ( which == wxIMAGE_LIST_SMALL )
    {
        if (m_ownsImageListSmall) delete m_imageListSmall;
        m_imageListSmall = imageList;
        m_ownsImageListSmall = false;
    }
    else if ( which == wxIMAGE_LIST_STATE )
    {
        if (m_ownsImageListState) delete m_imageListState;
        m_imageListState = imageList;
        m_ownsImageListState = false;
    }

    m_mainWin->SetImageList( imageList, which );
}

void wxExtGenericListCtrl::AssignImageList(wxImageListType *imageList, int which)
{
    SetImageList(imageList, which);
    if ( which == wxIMAGE_LIST_NORMAL )
        m_ownsImageListNormal = true;
    else if ( which == wxIMAGE_LIST_SMALL )
        m_ownsImageListSmall = true;
    else if ( which == wxIMAGE_LIST_STATE )
        m_ownsImageListState = true;
}

bool wxExtGenericListCtrl::Arrange( int WXUNUSED(flag) )
{
    return 0;
}

bool wxExtGenericListCtrl::DeleteItem( long item )
{
    m_mainWin->DeleteItem( item );
    return true;
}

bool wxExtGenericListCtrl::DeleteAllItems()
{
    m_mainWin->DeleteAllItems();
    return true;
}

bool wxExtGenericListCtrl::DeleteAllColumns()
{
    size_t count = m_mainWin->m_columns.GetCount();
    for ( size_t n = 0; n < count; n++ )
        DeleteColumn(0);

	wxExtListHeaderWindow *headerWin = m_mainWin->GetListCtrl()->m_headerWin;
	if ( headerWin )
	{
        headerWin->m_bsort = false;
		headerWin->m_sort_column = -1;
	}

    return true;
}

void wxExtGenericListCtrl::ClearAll()
{
    m_mainWin->DeleteEverything();
}

bool wxExtGenericListCtrl::DeleteColumn( int col )
{
    m_mainWin->DeleteColumn( col );

    // if we don't have the header any longer, we need to relayout the window
    if ( !GetColumnCount() )
    {
        ResizeReportView(false /* no header */);
    }

    return true;
}

void wxExtGenericListCtrl::Edit( long item  ,long col)
{
    m_mainWin->EditLabel( item ,col);
}

bool wxExtGenericListCtrl::EnsureVisible( long item )
{
    m_mainWin->EnsureVisible( item );
    return true;
}

long wxExtGenericListCtrl::FindItem( long start, const wxString& str,  bool partial )
{
    return m_mainWin->FindItem( start, str, partial );
}

long wxExtGenericListCtrl::FindItem( long start, wxUIntPtr data )
{
    return m_mainWin->FindItem( start, data );
}

//使用m_mapItemData2RowID来快速查找
long wxExtGenericListCtrl::FindItem2( wxUIntPtr data )
{
    return m_mainWin->FindItem2(data );
}

long wxExtGenericListCtrl::FindItem( long WXUNUSED(start), const wxPoint& pt,
                           int WXUNUSED(direction))
{
    return m_mainWin->FindItem( pt );
}

long wxExtGenericListCtrl::HitTest( const wxPoint &point, int &flags )
{
    return m_mainWin->HitTest( (int)point.x, (int)point.y, flags );
}

long wxExtGenericListCtrl::InsertItem( wxExtListItem& info )
{
    m_mainWin->InsertItem( info );
    return info.m_itemId;
}

long wxExtGenericListCtrl::InsertItem2( wxExtListItem& info, const bool bRefreshUI )
{
    m_mainWin->InsertItem2( info, bRefreshUI );
    return info.m_itemId;
}

long wxExtGenericListCtrl::InsertItem( long index, const wxString &label )
{
    wxExtListItem info;
    info.m_text = label;
    info.m_mask = wxLISTEXT_MASK_TEXT;
    info.m_itemId = index;
    return InsertItem( info );
}

long wxExtGenericListCtrl::InsertItem2( long index, const wxString &label, const long mdata, const bool bRefreshUI  )
{
    wxExtListItem info;
    info.m_text = label;
    info.m_data = mdata;
    info.m_mask = wxLISTEXT_MASK_TEXT | wxLISTEXT_MASK_DATA;
    info.m_itemId = index;
    return InsertItem2( info, bRefreshUI );
}

long wxExtGenericListCtrl::InsertItem( long index, int imageIndex )
{
    wxExtListItem info;
    info.m_mask = wxLISTEXT_MASK_IMAGE;
    info.m_image = imageIndex;
    info.m_itemId = index;
    return InsertItem( info );
}

long wxExtGenericListCtrl::InsertItem( long index, const wxString &label, int imageIndex )
{
    wxExtListItem info;
    info.m_text = label;
    info.m_image = imageIndex;
    info.m_mask = wxLISTEXT_MASK_TEXT | wxLISTEXT_MASK_IMAGE;
    info.m_itemId = index;
    return InsertItem( info );
}

long wxExtGenericListCtrl::InsertColumn( long col, wxExtListItem &item )
{
    wxCHECK_MSG( m_headerWin, -1, _T("can't add column in non report mode") );

    m_mainWin->InsertColumn( col, item );

    // if we hadn't had header before and have it now we need to relayout the
    // window
    if ( GetColumnCount() == 1 && m_mainWin->HasHeader() )
    {
        ResizeReportView(true /* have header */);
    }

    m_headerWin->Refresh();

    return 0;
}

long wxExtGenericListCtrl::InsertColumn( long col, const wxString &heading,
                               int format, int width )
{
    wxExtListItem item;
    item.m_mask = wxLISTEXT_MASK_TEXT | wxLISTEXT_MASK_FORMAT;
    item.m_text = heading;
    if (width >= -2)
    {
        item.m_mask |= wxLISTEXT_MASK_WIDTH;
        item.m_width = width;
    }
    item.m_format = format;

    return InsertColumn( col, item );
}

bool wxExtGenericListCtrl::ScrollList( int WXUNUSED(dx), int WXUNUSED(dy) )
{
    return 0;
}

// Sort items.
// fn is a function which takes 3 long arguments: item1, item2, data.
// item1 is the long data associated with a first item (NOT the index).
// item2 is the long data associated with a second item (NOT the index).
// data is the same value as passed to SortItems.
// The return value is a negative number if the first item should precede the second
// item, a positive number of the second item should precede the first,
// or zero if the two items are equivalent.
// data is arbitrary data to be passed to the sort function.

bool wxExtGenericListCtrl::SortItems( wxExtListCtrlCompare fn,int column,  long data )
{
    m_mainWin->SortItems( fn, column, data );
    return true;
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

void wxExtGenericListCtrl::OnSize(wxSizeEvent& WXUNUSED(event))
{
    if ( !m_mainWin )
        return;

    ResizeReportView(m_mainWin->HasHeader());

    m_mainWin->RecalculatePositions();
}

void wxExtGenericListCtrl::ResizeReportView(bool showHeader)
{
    int cw, ch;
    GetClientSize( &cw, &ch );

    if ( showHeader )
    {
        m_headerWin->SetSize( 0, 0, cw, m_headerHeight );
        if(ch > m_headerHeight)
            m_mainWin->SetSize( 0, m_headerHeight + 1, 
                                   cw, ch - m_headerHeight - 1 );
        else
            m_mainWin->SetSize( 0, m_headerHeight + 1, 
                                   cw, 0);
    }
    else // no header window
    {
        m_mainWin->SetSize( 0, 0, cw, ch );
    }
}

void wxExtGenericListCtrl::OnInternalIdle()
{
    wxWindow::OnInternalIdle();

    // do it only if needed
    if ( !m_mainWin->m_dirty )
        return;

    m_mainWin->RecalculatePositions();
}

// ----------------------------------------------------------------------------
// font/colours
// ----------------------------------------------------------------------------

bool wxExtGenericListCtrl::SetBackgroundColour( const wxColour &colour )
{
    if (m_mainWin)
    {
        m_mainWin->SetBackgroundColour( colour );
        m_mainWin->m_dirty = true;
    }

    return true;
}

bool wxExtGenericListCtrl::SetForegroundColour( const wxColour &colour )
{
    if ( !wxWindow::SetForegroundColour( colour ) )
        return false;

    if (m_mainWin)
    {
        m_mainWin->SetForegroundColour( colour );
        m_mainWin->m_dirty = true;
    }

    /*if (m_headerWin)
    {
        m_headerWin->SetForegroundColour( colour );
    }*/

    return true;
}
bool wxExtGenericListCtrl::SetHeaderForegroundColour( const wxColour &colour )
{
    if (m_headerWin)
    {
        m_headerWin->SetForegroundColour( colour );
    }
    return true;
}
bool wxExtGenericListCtrl::SetFont( const wxFont &font )
{
    if ( !wxWindow::SetFont( font ) )
        return false;

    if (m_mainWin)
    {
        m_mainWin->SetFont( font );
        m_mainWin->m_dirty = true;
    }

    if (m_headerWin)
    {
        m_headerWin->SetFont( font );
        CalculateAndSetHeaderHeight();
    }

    Refresh();

    return true;
}



#if _USE_VISATTR
#include "wx/listbox.h"
#endif

//static
wxVisualAttributes
wxExtGenericListCtrl::GetClassDefaultAttributes(wxWindowVariant variant)
{
#if _USE_VISATTR
    // Use the same color scheme as wxExtListBox
    return wxExtListBox::GetClassDefaultAttributes(variant);
#else
    wxUnusedVar(variant);
    wxVisualAttributes attr;
    attr.colFg = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    attr.colBg = wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX);
    attr.font  = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    return attr;
#endif
}

wxScrolledWindow * wxExtGenericListCtrl::GetScrolledWin(void)
{
	return m_headerWin->GetOwner();
}

// ----------------------------------------------------------------------------
// methods forwarded to m_mainWin
// ----------------------------------------------------------------------------

#if wxUSE_DRAG_AND_DROP

void wxExtGenericListCtrl::SetDropTarget( wxDropTarget *dropTarget )
{
    m_mainWin->SetDropTarget( dropTarget );
}

wxDropTarget *wxExtGenericListCtrl::GetDropTarget() const
{
    return m_mainWin->GetDropTarget();
}

#endif // wxUSE_DRAG_AND_DROP

bool wxExtGenericListCtrl::SetCursor( const wxCursor &cursor )
{
    return m_mainWin ? m_mainWin->wxWindow::SetCursor(cursor) : false;
}

wxColour wxExtGenericListCtrl::GetBackgroundColour() const
{
    return m_mainWin ? m_mainWin->GetBackgroundColour() : wxColour();
}

wxColour wxExtGenericListCtrl::GetForegroundColour() const
{
    return m_mainWin ? m_mainWin->GetForegroundColour() : wxColour();
}
wxColour wxExtGenericListCtrl::GetHeaderForegroundColour() const
{
    return m_headerWin ? m_headerWin->GetForegroundColour() : wxColour();
}
bool wxExtGenericListCtrl::DoPopupMenu( wxMenu *menu, int x, int y )
{
#if wxUSE_MENUS
    return m_mainWin->PopupMenu( menu, x, y );
#else
    return false;
#endif // wxUSE_MENUS
}

#if wxABI_VERSION >= 20603
void wxExtGenericListCtrl::DoClientToScreen( int *x, int *y ) const
{
    return m_mainWin->DoClientToScreen(x, y);
}

void wxExtGenericListCtrl::DoScreenToClient( int *x, int *y ) const
{
    return m_mainWin->DoScreenToClient(x, y);
}
#endif // 2.6.3

void wxExtGenericListCtrl::SetFocus()
{
    /* The test in window.cpp fails as we are a composite
       window, so it checks against "this", but not m_mainWin. */
    if ( DoFindFocus() != this )
        m_mainWin->SetFocus();
}

wxSize wxExtGenericListCtrl::DoGetBestSize() const
{
    // Something is better than nothing...
    // 100x80 is what the MSW version will get from the default
    // wxControl::DoGetBestSize
    return wxSize(100,80);
}

// ----------------------------------------------------------------------------
// virtual list control support
// ----------------------------------------------------------------------------

wxString wxExtGenericListCtrl::OnGetItemText(long WXUNUSED(item), long WXUNUSED(col)) const
{
    // this is a pure virtual function, in fact - which is not really pure
    // because the controls which are not virtual don't need to implement it
    wxFAIL_MSG( _T("wxExtGenericListCtrl::OnGetItemText not supposed to be called") );

    return wxEmptyString;
}

int wxExtGenericListCtrl::OnGetItemImage(long WXUNUSED(item)) const
{
    wxCHECK_MSG(!GetImageList(wxIMAGE_LIST_SMALL),
                -1,
                wxT("List control has an image list, OnGetItemImage should be overridden."));
    return -1;
}

wxExtListItemAttr *
wxExtGenericListCtrl::OnGetItemAttr(long WXUNUSED_UNLESS_DEBUG(item)) const
{
    wxASSERT_MSG( item >= 0 && item < GetItemCount(),
                  _T("invalid item index in OnGetItemAttr()") );

    // no attributes by default
    return NULL;
}

void wxExtGenericListCtrl::SetItemCount(long count)
{
    wxASSERT_MSG( IsVirtual(), _T("this is for virtual controls only") );

    m_mainWin->SetItemCount(count);
}

void wxExtGenericListCtrl::RefreshItem(long item)
{
    m_mainWin->RefreshLine(item);
}

void wxExtGenericListCtrl::RefreshItems(long itemFrom, long itemTo)
{
    m_mainWin->RefreshLines(itemFrom, itemTo);
}

void wxExtGenericListCtrl::ReSortItems()
{
	m_mainWin->ReSortItems();
}

/*
 * Generic wxExtListCtrl is more or less a container for two other
 * windows which drawings are done upon. These are namely
 * 'm_headerWin' and 'm_mainWin'.
 * Here we override 'virtual wxWindow::Refresh()' to mimic the
 * behaviour wxExtListCtrl has under wxMSW.
 */
void wxExtGenericListCtrl::Refresh(bool eraseBackground, const wxRect *rect)
{
    if (!rect)
    {
        // The easy case, no rectangle specified.
        if (m_headerWin)
            m_headerWin->Refresh(eraseBackground);

        if (m_mainWin)
            m_mainWin->Refresh(eraseBackground);
    }
    else
    {
        // Refresh the header window
        if (m_headerWin)
        {
            wxRect rectHeader = m_headerWin->GetRect();
            rectHeader.Intersect(*rect);
            if (rectHeader.GetWidth() && rectHeader.GetHeight())
            {
                int x, y;
                m_headerWin->GetPosition(&x, &y);
                rectHeader.Offset(-x, -y);
                m_headerWin->Refresh(eraseBackground, &rectHeader);
            }

        }

        // Refresh the main window
        if (m_mainWin)
        {
            wxRect rectMain = m_mainWin->GetRect();
            rectMain.Intersect(*rect);
            if (rectMain.GetWidth() && rectMain.GetHeight())
            {
                int x, y;
                m_mainWin->GetPosition(&x, &y);
                rectMain.Offset(-x, -y);
                m_mainWin->Refresh(eraseBackground, &rectMain);
            }
        }
    }
}

void wxExtGenericListCtrl::Freeze()
{
    m_mainWin->Freeze();
}

void wxExtGenericListCtrl::Thaw()
{
    m_mainWin->Thaw();
}

wxTextCtrl * wxExtGenericListCtrl::GetEditControl(void)
{
	wxTextCtrl * retval = 0L;
	
	if(m_mainWin)
		retval = m_mainWin->GetEditControl();
	
	return retval;
}

//获取排序的列序号，如果没有排序，返回-1
int wxExtGenericListCtrl::GetSortCol(void)
{
    if(HasFlag(wxLCEXT_MASK_SORT)&&
        m_headerWin&&
        m_headerWin->m_bsort&&
        m_headerWin->m_sort_column>=0&&
        m_headerWin->m_sort_column<GetColumnCount())
    {
        return m_headerWin->m_sort_column;
    }
    return -1;
}


//停止排序，将排序列改为-1
void wxExtGenericListCtrl::DisableSortCol(void)
{
    if(HasFlag(wxLCEXT_MASK_SORT)&&
        m_headerWin)
    {
        m_headerWin->m_bsort = false;

        ::InvalidateRect((HWND)(m_headerWin->GetHWND()),NULL,FALSE);
        ::SendMessage((HWND)(m_headerWin->GetHWND()),WM_PAINT,0,0);
    }
}

void wxExtGenericListCtrl::ChangeRow(long row, bool isUp)
{
	if(row<0||row>=GetItemCount())
		return;
	int other=isUp?(row>0?row-1:row):(row<GetItemCount()?row+1:row);
	if(other!=row)
	{
		//获取
		wxArrayString other_value,row_value;
		for(int col=0;col<GetColumnCount();col++)
		{
			other_value.Add(GetSubItemText(other,col));
			row_value.Add(GetSubItemText(row,col));
		}
		bool other_checked=GetItemChecked(other);
		bool row_checked=GetItemChecked(row);
		int other_state=GetItemState(other,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED);
		int row_state=GetItemState(row,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED);
		long other_data = GetItemData( other );
		long row_data = GetItemData( row );
		//设置
		for(int col=0;col<GetColumnCount();col++)
		{
			SetItem(row,col,other_value[col]);
			SetItem(other,col,row_value[col]);
		}
		SetItemChecked(row,other_checked);
		SetItemChecked(other,row_checked);
		SetItemData( row, other_data );
		SetItemData( other, row_data );
        SetItemState(row,other_state,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED);
        SetItemState(other,row_state,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED);
		EnsureVisible(other);

	}
}

wxString wxExtGenericListCtrl::GetColumnLongestText( long nCol )
{
	wxString strMaxText;
	int nItemCount = GetItemCount();
	for ( int i = 0; i < nItemCount; i++ )
	{
		wxString text = GetSubItemText(i, nCol);
		if ( text.Length() > strMaxText.Length() )
		{
			strMaxText = text;
		}
	}

	wxExtListItem item;
	GetColumn(nCol, item);
	wxString strHeader = item.GetText();
	if ( strHeader.Length() > strMaxText.Length())
	{
		strMaxText = strHeader;
	}

	return strMaxText;
}

void wxExtGenericListCtrl::AutoAdjustColumnWidth()
{
	int nColumnCount = GetColumnCount();
	for ( int i = 0; i < nColumnCount; i++ )
	{
		wxString strMaxText = GetColumnLongestText(i);
		int nHeiht = 0;
		int nTextWidth = 0;
		GetTextExtent(strMaxText, &nTextWidth, &nHeiht);
		nTextWidth = nTextWidth + COL_LEFT_MARGIN_WIDTH + COL_RIGHT_MARGIN_WIDTH;
		SetColumnWidth(i, nTextWidth);
	}
}


void wxExtGenericListCtrl::RefreshHeader()
{
    if(m_headerWin)
    {
		m_headerWin->Refresh();
		m_headerWin->Update();
    }
}

//////////////////////////////////////////////////////////////////////////
//wxGenericGrid
#include "../inc/Module-Misc2/tools_util.h"
IMPLEMENT_DYNAMIC_CLASS(wxGenericGrid,wxGrid)
#ifdef _USE_GRID_LIST
IMPLEMENT_DYNAMIC_CLASS(wxExtListCtrl, wxGenericGrid)
#else
IMPLEMENT_DYNAMIC_CLASS(wxExtListCtrl, wxExtListCtrl_Obsolete)
#endif
BEGIN_EVENT_TABLE(wxGenericGrid,wxGrid)
EVT_PAINT(wxGenericGrid::OnPaint)
EVT_KEY_DOWN(wxGenericGrid::OnKeyDown)
EVT_GRID_CELL_LEFT_CLICK(wxGenericGrid::OnGridCellLeftClick)
EVT_GRID_CELL_RIGHT_CLICK(wxGenericGrid::OnGridCellRightClick)
EVT_GRID_CELL_LEFT_DCLICK(wxGenericGrid::OnGridCellLeftDClick)
EVT_GRID_LABEL_RIGHT_CLICK(wxGenericGrid::OnGridCellRightClick)
EVT_GRID_LABEL_LEFT_CLICK(wxGenericGrid::OnGridLableLeftClick)
EVT_GRID_CMD_CELL_CHANGE(wxID_ANY, wxGenericGrid::OnGridCellChange)
END_EVENT_TABLE()

wxGenericGrid::wxGenericGrid( wxWindow *parent,
		 wxWindowID winid ,
		 const wxPoint& pos ,
		 const wxSize& size ,
		 long style ,
		 const wxValidator& validator ,
		 const wxString &name ):
			wxGrid(parent, winid, pos, size, wxWANTS_CHARS|(style&wxBORDER_MASK), name)
{

	CreateGrid(0,0);
	SetSelectionMode(wxGridSelectRows);
	SetRowLabelSize( 0 );
	EnableDragGridSize(false);
	EnableDragRowSize(false);
	//EnableEditing(false);

	if (style & wxLCEXT_NO_HEADER)
	{
		SetColLabelSize(0);
	}
	else
	{
		SetColLabelSize(23);
	}

	if ( style & wxLCEXT_VRULES || style & wxLCEXT_HRULES )
	{
		EnableGridLines(true);
	}
	else
	{
		EnableGridLines(false);
	}

	SetCellHighlightPenWidth(0);
	SetCellHighlightROPenWidth(0);

// 	SetLabelBackgroundColour(wxColor(pListCfg->HeadBackgroundColor));
// 	SetLabelTextColour(wxColor(pListCfg->HeadColor));
// 	SetDefaultCellBackgroundColour(wxColor(pListCfg->BackgroundColor));
// 	SetCellTextColour(pListCfg->TextColor);
// 	int lh=GetLineHeight(cellfont);
// 	SetLabelSize(wxHORIZONTAL,lh);
// 	SetDefaultRowSize(lh,true);

	wxFont cellfont(9, wxDEFAULT, wxNORMAL, wxNORMAL, false, wxT("宋体"));
 	SetDefaultCellFont(cellfont);
 	SetLabelFont(cellfont);

	m_bIsAscending = true;
	m_nSortCol = -1;

}

int  wxGenericGrid::GetColumnWidth( int col ) 
{
	if (col < GetColumnCount() && col >= 0)
	{
		return GetColSize(col);
	}

	return 100;
}

bool wxGenericGrid::SetColumnWidth( int col, int width)
{
	if (col < GetColumnCount() && col >= 0)
	{
		SetColSize(col,width);
		return true;
	}

	return false;
}

bool wxGenericGrid::GetItemChecked(int id)
{
	std::map<int,bool>::iterator iter = m_mapCheck.find(id);
	if (iter != m_mapCheck.end())
	{
		return iter->second;
	}

	return false;
}

void wxGenericGrid::SetItemChecked(int id,bool check)
{
	std::map<int,bool>::iterator iter = m_mapCheck.find(id);
	if (iter != m_mapCheck.end())
	{
		m_mapCheck[id] = check;
	}
	else
	{
		m_mapCheck.insert( std::make_pair(id,check) );
	}
}

int wxGenericGrid::GetItemCount() 
{
	return GetRows();
}

int wxGenericGrid::GetColumnCount() 
{
	return GetNumberCols();
	
}

int wxGenericGrid::GetSelectedItemCount() const
{
// 	wxArrayInt array = GetSelectedRows();
// 	return array.size();
	return 1;
}

// wxColour wxGenericGrid::GetItemBackgroundColour( long rowIndex )
// {
// 	if (GetColumnCount() > 0 && rowIndex < GetItemCount() && rowIndex >= 0)
// 	{
// 		return GetCellBackgroundColour(rowIndex, 0);
// 	}
// 
// 	return wxColour(255,255,255);//白色
// }

void wxGenericGrid::SetItemBackgroundColour(long rowIndex, const wxColour &colColor)
{
	if (rowIndex < 0 || rowIndex >= GetItemCount())
	{
		return;
	}

	for (int col=0; col < GetColumnCount(); ++col)
	{
		SetCellBackgroundColour(col, rowIndex, colColor);
	}
}
// void wxGenericGrid::SetItemForegroundColour(long rowIndex, const wxColour &colColor)
// {
// 	if (rowIndex < 0 || rowIndex >= GetItemCount())
// 	{
// 		return;
// 	}
// 
// 	for (int col=0; col < GetColumnCount(); ++col)
// 	{
// 		SetCellTextColour(col, rowIndex, colColor);
// 	}
// }
// wxColour wxGenericGrid::GetItemForegroundColour(long rowIndex)
// {
// 	if (GetColumnCount() > 0 && rowIndex < GetItemCount())
// 	{
// 		return GetCellTextColour(rowIndex,0);
// 	}
// 
// 	return wxColour(255,255,255);
// }
void wxGenericGrid::SetOddRowColour(wxColour clr)
{
	for (int row = 0; row < GetItemCount(); ++row)
	{

		for (int col = 0; col < GetColumnCount(); ++col)
		{
			if(row%2==0)
				SetCellBackgroundColour(clr,row,col);	
		}
	}

}

// wxColour wxGenericGrid::GetOddRowColour()
// {
// 	return m_OddRowColour;
// }

void wxGenericGrid::SetEvenRowColour(wxColour clr)
{
	for (int row = 0; row < GetItemCount(); ++row)
	{

		for (int col = 0; col < GetColumnCount(); ++col)
		{
			if(row%2!=0)
				SetCellBackgroundColour(clr,row,col);	
		}
	}
}

// wxColour wxGenericGrid::GetEvenRowColour()
// {
// 	return m_EvenRowColour;
// }

bool wxGenericGrid::SetBackgroundColour( const wxColour &colour )
{
	for (int row = 0; row < GetItemCount(); ++row)
	{

		for (int col = 0; col < GetColumnCount(); ++col)
		{
			SetCellBackgroundColour(row,col,colour);	
		}
	}

	return true;
}

bool wxGenericGrid::SetForegroundColour( const wxColour &colour )
{

	for (int row = 0; row < GetItemCount(); ++row)
	{
		for (int col = 0; col < GetColumnCount(); ++col)
		{
			SetCellTextColour(colour,row,col);
		}
	}
	return true;
}

void wxGenericGrid::SetHeaderBackgroundColour( const wxColour &colColor )
{
	SetLabelBackgroundColour(colColor);
}

bool wxGenericGrid::SetHeaderForegroundColour( const wxColour &colour )
{
	SetLabelTextColour(colour);
	return true;
}

bool wxGenericGrid::SetFont( const wxFont &font )
{
	for (int row = 0; row < GetItemCount(); ++row)
	{
		for (int col = 0; col < GetColumnCount(); ++col)
		{
			SetCellFont(row,col,font);
		}
	}
	
	return true;
}

// const wxFont& wxGenericGrid::GetFont(void) const
// {
// 	return m_font;
// }

void wxGenericGrid::ClearData()
{
//	m_textctrl_MaxLength = -1;
}

void wxGenericGrid::ClearAll()
{
	ClearData();
	DeleteAllColumns();
	DeleteAllItems();
}

bool wxGenericGrid::DeleteAllItems()
{

	if (GetItemCount() == 0)
	{
		return false;
	}

	bool ret = false;

	int row = GetItemCount()-1;
	while(row >= 0 && row < GetItemCount())
	{
		ret = DeleteRows(row, 1);
		row--;
	}

	m_mapCheck.clear();
	m_mapItemData.clear();

	return ret;
}

bool wxGenericGrid::DeleteAllColumns()
{
	if (GetColumnCount() == 0)
	{
		return false;
	}

	bool ret = false;

	int col = GetColumnCount()-1;
	while(col >= 0 && col < GetColumnCount())
	{
		ret = DeleteCols(col, 1);
		col--;
	}

	m_mapColumnCtrlType.clear();

	return ret;
}

bool wxGenericGrid::DeleteColumn( int col )
{

	if (col < 0 || col >= GetColumnCount())
	{
		return false;
	}

	//记住每一列的名称
	std::vector<wxString> vecTitleName;
	for (int colomn=0; colomn < GetColumnCount(); ++colomn)
	{
		wxString & tmp = GetColLabelValue(colomn);

		if (colomn != col)
			vecTitleName.push_back(tmp);
	}


 	if (!m_mapColumnCtrlType.empty())
 	{

		std::map<long,long> mapData;
		mapData.insert(m_mapColumnCtrlType.begin(),m_mapColumnCtrlType.find(col));

		for (size_t curcol = col+1; curcol < m_mapColumnCtrlType.size(); ++curcol)
		{
			mapData.insert( std::make_pair(curcol-1, GetColumnCtrlType(curcol)) );
		}

		m_mapColumnCtrlType.clear();
		m_mapColumnCtrlType = mapData;

	}

	bool ret = DeleteCols(col, 1);

	//重置列名称
	col = 0;
	for (std::vector<wxString>::iterator it = vecTitleName.begin();
		it != vecTitleName.end();
		++it,++col)
	{
		SetColLabelValue(col, *it);
	}

	return ret;
}

bool wxGenericGrid::DeleteItem( long item )
{
	if (item < 0 || item >= GetItemCount())
	{
		return false;
	}

	bool ret = DeleteRows(item, 1);

	if (!m_mapItemData.empty())
	{

		std::map<long,long> mapData;
		mapData.insert(m_mapItemData.begin(),m_mapItemData.find(item));
		for (size_t currow = item+1; currow < m_mapItemData.size(); ++currow)
		{
			mapData.insert( std::make_pair(currow-1, GetItemData(currow)) );
		}

		m_mapItemData.clear();
		m_mapItemData = mapData;

	}

	//带checkbox的grid
	if (!m_mapCheck.empty())
	{

		std::map<int,bool> mapData;
		mapData.insert(m_mapCheck.begin(),m_mapCheck.find(item));
		for (size_t currow = item+1; currow < m_mapCheck.size(); ++currow)
		{
			mapData.insert( std::make_pair(currow-1, GetItemChecked(currow)) );
		}
		m_mapCheck.clear();
		m_mapCheck = mapData;

	}


	return ret;
}

bool wxGenericGrid::IsSelectedItem(int line) 
{
	if (GetColumnCount() > 0 && line < GetItemCount() && line >= 0)
	{
		return IsInSelection(line, 0);
	}

	return false;
}

void wxGenericGrid::Refresh(bool eraseBackground, const wxRect *rect)
{
	if (!rect)
	{
		wxGrid::Refresh(eraseBackground);
	}
	else
	{

		wxRect rectMain = GetRect();
		rectMain.Intersect(*rect);
		if (rectMain.GetWidth() && rectMain.GetHeight())
		{
			int x, y;
			GetPosition(&x, &y);
			rectMain.Offset(-x, -y);
			wxGrid::Refresh(eraseBackground, &rectMain);
		}

	}
}

void wxGenericGrid::SetWindowStyleFlag( long style )
{
	wxGrid::SetWindowStyle(style);
}

wxString wxGenericGrid::GetSubItemText(int index, int col)
{
	return GetCellValue(index, col);
}

bool wxGenericGrid::IsWithCheckbox()
{
	if (!m_mapColumnCtrlType.empty())
	{
		std::map<long,long>::iterator it = m_mapColumnCtrlType.begin();
		for (; it != m_mapColumnCtrlType.end(); ++it)
		{
			if (it->second == COLUMN_CTRL_TYPE_CHECKBOX)
			{
				return true;
			}
		}
	}
	
	return false;
}

long wxGenericGrid::InsertItem( long index, const wxString& label )
{

	int row=GetItemCount();
	if(index>=row)
	{
		AppendRows();

		if (IsWithCheckbox())
		{
			m_mapCheck.insert( std::make_pair(index,false) );
		}
	}
	else
	{
		InsertRows(index);
		row=index;

		if (!m_mapItemData.empty())
		{
			std::map<long,long> mapData;
			mapData.insert(m_mapItemData.begin(),m_mapItemData.find(row));
			mapData.insert( std::make_pair(row, -1) );
			for (size_t currow = row; currow < m_mapItemData.size(); ++currow)
			{
				mapData.insert( std::make_pair(currow+1, GetItemData(currow)) );
			}

			m_mapItemData.clear();
			m_mapItemData = mapData;
		}

		if (!m_mapCheck.empty())
		{
			std::map<int,bool> mapData;
			mapData.insert(m_mapCheck.begin(),m_mapCheck.find(row));
			mapData.insert( std::make_pair(row, false) );
			for (size_t currow = row; currow < m_mapCheck.size(); ++currow)
			{
				mapData.insert( std::make_pair(currow+1, GetItemChecked(currow)) );
			}

			m_mapCheck.clear();
			m_mapCheck = mapData;
		}

	}

	SetCellValue(row,0,label);

	for (int col=0; col < GetColumnCount(); ++col)
	{
		int coltype = GetColumnCtrlType(col);
		if (COLUMN_CTRL_TYPE_EDIT == coltype)
		{
			SetReadOnly(row,col,false);
		}
		else
		{
			SetReadOnly(row,col,true);
		}
	}

	return row;

}

long wxGenericGrid::InsertItem(wxExtListItem& info)
{
	return InsertItem(info.m_itemId, info.m_text);
}

long wxGenericGrid::InsertItem( long index, const wxString& label, int imageIndex )
{
	return InsertItem(index,label);
}

long wxGenericGrid::InsertItem(long index, wxExtListItem& item)
{
	return InsertItem(index, item.m_text);
}

long wxGenericGrid::InsertColumn( long col, wxExtListItem& info )
{
	return InsertColumn(col, info.m_text, info.m_format, info.GetWidth());
}

long wxGenericGrid::InsertColumn( long col, const wxString& heading, int format, int width )
{
	int max=GetColumnCount();

	if(col>=max)
	{
		AppendCols();
		col = max;
		SetColumnCtrlType(col,COLUMN_CTRL_TYPE_NORMAL);
	}
	else
	{
		InsertCols(col);

		//如果是插入列，需要修改插入点后列的属性
		std::map<long,long> mapData;
		mapData.insert(m_mapColumnCtrlType.begin(),m_mapColumnCtrlType.find(col));
		mapData.insert( std::make_pair(col, COLUMN_CTRL_TYPE_NORMAL) );

		for (long curcol = col; curcol < max; ++curcol)
		{
			mapData.insert( std::make_pair(curcol+1, GetColumnCtrlType(curcol)) );
		}

		m_mapColumnCtrlType.clear();
		m_mapColumnCtrlType = mapData;

	}

	SetColLabelValue(col,heading);
	if (0 != width)
	{
		SetColumnWidth(col,width);	
	}
	else
	{
		SetColumnWidth(col, 100 );
	}

	
	if (format == 0)
	{
		SetColLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTRE);
	}
	else if (format == 1)
	{
		SetColLabelAlignment(wxALIGN_RIGHT,wxALIGN_CENTRE);
	}
	else if (format == 2)
	{
		SetColLabelAlignment(wxALIGN_CENTRE,wxALIGN_CENTRE);
	}

	return col;
}

wxUIntPtr wxGenericGrid::GetItemData( long item ) 
{
	std::map<long,long>::iterator iter = m_mapItemData.find(item);
	if (iter != m_mapItemData.end())
	{
		return (wxUIntPtr)(iter->second);
	}

	return 0;
}
bool wxGenericGrid::SetItemData( long item, long data )
{
	if (item < 0 || item >= GetItemCount())
	{
		return false;
	}

	std::map<long,long>::iterator iter = m_mapItemData.find(item);
	if (iter != m_mapItemData.end())
	{
		iter->second = data;
	}
	else
	{
		m_mapItemData.insert( std::make_pair(item,data) );
	}

	return true;
}

bool wxGenericGrid::GetColumn( int col, wxExtListItem& item ) 
{
	if (col < 0 || col >= GetColumnCount())
	{
		return false;
	}

	item.m_text = GetColLabelValue(col);
	return true;
}
bool wxGenericGrid::SetColumn( int col, wxExtListItem& item )
{
	if (col < 0 || col >= GetColumnCount())
	{
		return false;
	}

	SetColLabelValue(col,item.m_text);
	return 0;
}

bool wxGenericGrid::GetItem( wxExtListItem& info ) 
{
	if (info.m_col < 0 || info.m_col >= GetColumnCount() || info.m_itemId < 0 || info.m_itemId >= GetItemCount())
	{
		return false;
	}

	info.m_text = GetCellValue(info.m_itemId,info.m_col);
	info.m_data = GetItemData(info.m_itemId);
	info.m_color = GetCellTextColour(info.m_itemId,info.m_col);
	return true;
}
bool wxGenericGrid::SetItem( wxExtListItem& item ) 
{
	if (item.m_col < 0 || item.m_col >= GetColumnCount() || item.m_itemId < 0 || item.m_itemId >= GetItemCount())
	{
		return false;
	}



// 	WXLOG_INFO("row=%d,col=%d",item.m_itemId,item.m_col);
// 	WXLOG_INFO("value=%s",item.m_text.mb_str());
	SetCellValue(item.m_text,item.m_itemId,item.m_col);
	SetCellTextColour(item.m_color, item.m_itemId,item.m_col);
	return true;
}
long wxGenericGrid::SetItem( long index, int col, const wxString& label, int imageId)
{
	if (col < 0 || col >= GetColumnCount() || index < 0 || index >= GetItemCount())
	{
		return false;
	}


// 	WXLOG_INFO("row=%d,col=%d",index,col);
// 	WXLOG_INFO("value=%s",label.mb_str());
	SetCellValue(label,index,col);
	return 0;
}

// wxString wxGenericGrid::GetColLabelValue( int col )
// {
// 	if (col < 0 || (size_t)col >= m_vecColTitle.size())
// 	{
// 		return "";
// 	}
// 
// 	return m_vecColTitle.at(col);
// }

long wxGenericGrid::GetNextItem( long item, int geometry, int state ) 
{

	if ( -1 == item&&GetRows()>0)
	{
		return GetGridCursorRow();
	}

// 	long ret = item;
// 	long max = GetItemCount();
// 
// 	ret++;
// 	if (ret < 0 || ret >= max)
// 	{
// 		return 0;
// 	}
// 
// 	if ( ret == max )
// 	{
// 		return -1;
// 	}
// 
// 	if ( !state )
// 	{
// 		return (size_t)ret;
// 	}
// 
// 	size_t i;
// 	size_t count = GetItemCount();
// 	size_t curRow = GetGridCursorRow();
// 	wxArrayInt SelectArray = GetSelectedRows();
// 	for ( size_t line = (size_t)ret; line < count; line++ )
// 	{
// 		if ( (state & wxLISTEXT_STATE_FOCUSED) && (line == curRow) )
// 			return line;
// 
// 		for (i = 0; i < SelectArray.size(); ++i)
// 		{
// 			if ( (state & wxLISTEXT_STATE_SELECTED) && line == SelectArray[i] )
// 				return line;
// 		}
// 
// 	}

	return -1;
}

bool wxGenericGrid::HighlightLine( size_t line, bool highlight )
{
// 	bool changed;
// 
// 	if ( IsVirtual() )
// 	{
// 		changed = m_selStore.SelectItem(line, highlight);
// 	}
// 	else // !virtual
// 	{
// 		wxExtListLineData *ld = GetLine(line);
// 		wxCHECK_MSG( ld, false, _T("invalid index in HighlightLine") );
// 
// 		changed = ld->Highlight(highlight);
// 	}
// 
// 	bool dontNotify = HasFlag(wxLCEXT_STICKY_HIGHLIGHT) && HasFlag(wxLCEXT_STICKY_NOSELEVENT);
// 
// 	if ( changed && ! dontNotify)
// 	{
		bool changed = SendNotify( line,0, highlight ? wxEVT_COMMAND_LIST_EXT_ITEM_SELECTED : wxEVT_COMMAND_LIST_EXT_ITEM_DESELECTED );
// 	}

	return changed;
}

bool wxGenericGrid::SendNotify( size_t line,
									 size_t column,
									 wxEventType command,
									 wxPoint point )
{
	bool bRet = true;

	wxExtListEvent le( command, GetId() );
	le.SetEventObject( this );
	le.m_itemIndex = line;
	le.m_col = column;
	le.m_pointDrag = GetPosition();
	GetItem( le.m_item );

	ProcessEvent( le );
	bRet = le.IsAllowed();

	return bRet;
}

bool wxGenericGrid::SetItemState( long item, long state, long stateMask)
{

	if (item < 0 || item >= GetItemCount())
	{
		return false;
	}

	m_mapItemState.clear();

// 	std::map<long,long>::iterator iter = m_mapItemState.find(item);
// 	if (iter != m_mapItemState.end())
// 	{
// 		iter->second = state;
// 	}
// 	else
// 	{
		m_mapItemState.insert( std::make_pair(item,state) );
// 	}

	if ( stateMask & wxLISTEXT_STATE_FOCUSED )
	{
		if ( state & wxLISTEXT_STATE_FOCUSED )
		{
			SelectRow(item);
			//HighlightLine(item, false);
		}
	}

	if ( stateMask & wxLISTEXT_STATE_SELECTED )
	{
		if ( state & wxLISTEXT_STATE_SELECTED )
		{
			SelectRow(item);
			//HighlightLine(item, false);
		}
	}


	return true;
}

long wxGenericGrid::GetItemState( long item, long stateMask ) 
{
	if (item < 0 || item >= GetItemCount())
	{
		return 0;
	}

	long ret = false;
	std::map<long,long>::iterator iter = m_mapItemState.find(item);
	if (iter != m_mapItemState.end())
	{
		long & state = iter->second;

		if ( stateMask & wxLISTEXT_STATE_FOCUSED )
		{
			ret = state & wxLISTEXT_STATE_FOCUSED;
		}

		if ( stateMask & wxLISTEXT_STATE_SELECTED )
		{
			ret = state & wxLISTEXT_STATE_SELECTED;
		}
	}

	return ret;
}

int wxGenericGrid::GetColumnCtrlType( int col ) 
{
	std::map<long,long>::iterator iter = m_mapColumnCtrlType.find(col);
	if (iter != m_mapColumnCtrlType.end())
	{
		return iter->second;
	}

	return -1;
}
void wxGenericGrid::SetColumnCtrlType( int col, int t)
{
	if (t == COLUMN_CTRL_TYPE_CHECKBOX)
	{
		std::map<int,bool>::iterator it;
		for (int row=0; row < GetColumnCount(); ++row)
		{
			it = m_mapCheck.find(row);
			if (it == m_mapCheck.end())
			{
				m_mapCheck.insert( std::make_pair(row, false) );
			}
		}
	}

	std::map<long,long>::iterator iter = m_mapColumnCtrlType.find(col);
	if (iter != m_mapColumnCtrlType.end())
	{
		iter->second = t;

		//更新已有行的功能，按照新的列的属性
		for (int row=0; row < GetColumnCount(); ++row)
		{
			if (COLUMN_CTRL_TYPE_EDIT == t)
			{
				SetReadOnly(row,col,false);
			}
			else
			{
				SetReadOnly(row,col,true);
			}
		}
	}
	else
	{
		m_mapColumnCtrlType.insert( std::make_pair(col,t) );
	}

	//EnableEditing(true);
}

wxString wxGenericGrid::GetItemText(long item) 
{
	wxExtListItem info;
// 	info.m_mask = wxLISTEXT_MASK_TEXT;
	info.m_itemId = item;
	GetItem( info );
	return info.m_text;
}

void wxGenericGrid::SetTextCtrlMaxLength(int len)
{
//	m_textctrl_MaxLength=len;
}

long wxGenericGrid::FindItem(long start, wxUIntPtr data)
{
	long itemid = start;
	if (itemid < 0 || itemid >= GetItemCount())
	{
		itemid = 0;
	}

	int i=0;
	std::map<long,long>::iterator iter = m_mapItemData.begin();
	for (; iter != m_mapItemData.end(); ++iter)
	{
		if (iter->second == data)
		{
			return i;
		}

		++i;
	}

	return wxNOT_FOUND;
}

void wxGenericGrid::OnPaint( wxPaintEvent &event )
{

	int ctrltype = -1;
	for (int col=0; col < GetColumnCount(); ++col)
	{
		ctrltype = GetColumnCtrlType(col);
		if(ctrltype==COLUMN_CTRL_TYPE_CHECKBOX)
		{
			SetColFormatBool(col);
			for (int row = 0; row < GetItemCount(); ++row)
			{

				wxGridCellBoolRenderer * pLastRenderer = static_cast<wxGridCellBoolRenderer *>(GetCellRenderer(row,col));
				if (pLastRenderer == NULL)
				{
					wxGridCellBoolRenderer * pr = new wxGridCellBoolRenderer;
					SetCellRenderer(row, col, pr);
				}
				else
				{
					//SetCellRenderer(row, col, pLastRenderer);
				}

				wxGridCellBoolEditor * pLastEditor = static_cast<wxGridCellBoolEditor *>(GetCellEditor(row,col));
				if (pLastEditor == NULL)
				{
					wxGridCellBoolEditor * pe = new wxGridCellBoolEditor;
					SetCellEditor(row, col, pe);
					pe->UseStringValues();
				}
				else
				{
					//SetCellEditor(row, col, pLastEditor);
					pLastEditor->UseStringValues();
				}


				if (GetItemChecked(row))
				{
					SetCellValue("1",row,col);
 				}
				else
				{
					SetCellValue("0",row,col);
				}
				
// 				wxRect rect = CellToRect(wxGridCellCoords(row,col));
// 				wxRendererNative::Get().DrawCheckBox(this,dc,rect,GetItemChecked(row)?wxCONTROL_CHECKED:0);


				if(row%2!=0)
				{
					SetCellBackgroundColour(wxColor(255,255,245),row,col);
					SetCellBackgroundColour(wxColor(255,255,245),row,col-1);
				}
				else
				{
					SetCellBackgroundColour(wxColor(240,240,240),row,col);
					SetCellBackgroundColour(wxColor(240,240,240),row,col-1);
				}

			}
			
		}
	}

//  	if (m_nSortCol != -1)
//  	{
// 		wxPaintDC dc(this);
// // 		PrepareDC( dc );
// // 		AdjustDC( dc );
// // 		wxRect rc = CellToRect(0, m_nSortCol);
// // 		wxPen oldpen=dc.GetPen();
// // 		dc.SetPen(wxPen(wxColour(0,0,0),1));
// // 		dc.DrawLine(rc.GetLeftTop(), rc.GetBottomRight());
// 		wxRendererNative::Get().DrawHeaderButton(this, dc, wxRect(pt.x,pt.y,pt.x+5,pt.y+5),0,wxHDR_SORT_ICON_UP);
// // 		dc.SetPen(oldpen);
//  	}

 	event.Skip();
}
//////////////////////////////////////////////////////////////////////////
//#include <tools/util.h>
void wxGenericGrid::OnGridLableLeftClick(wxGridEvent& event)
{

	if (m_nSortCol != -1 && m_nSortCol != event.GetCol())
	{
// 		wxString lable1 = GetColLabelValue(m_nSortCol);
// 		wxString lable = lable1.Left(lable1.Find("("));
// 		SetColLabelValue(m_nSortCol, lable);
		m_bIsAscending = true;
	}

	m_nSortCol = event.GetCol();
	SortCol(event.GetCol(), m_bIsAscending);
	m_bIsAscending = !m_bIsAscending;

	//刷新排序箭头
	wxRect rc = GetClientRect();
	const wxRect r = rc;
	Refresh(false, &r);

}

enum SORTTYPE
{
	STRING_TYPE=0,
	NUMBERIC_TYPE
};

struct TABLE_BEFORE_ORDER 
{
	wxString cellText;
	wxColour cellClr;
};

struct SORT_DATA 
{
	long row;
	std::string data;
	double dData;
	int sorttype;

	SORT_DATA()
	{
		row = -1;
		data = "";
		dData = 0.0;
		sorttype = -1;
	}

	bool operator < (const SORT_DATA &m)const
	{
		if (NUMBERIC_TYPE == sorttype)
		{
			return this->dData < m.dData;
		}

		if (STRING_TYPE == sorttype)
		{
			return stricmp(this->data.c_str(), m.data.c_str()) < 0; 
		}

		return false;
	}

	bool operator > (const SORT_DATA &m)const
	{
		if (NUMBERIC_TYPE == sorttype)
		{
			return this->dData > m.dData;
		}

		if (STRING_TYPE == sorttype)
		{
			return stricmp(this->data.c_str(), m.data.c_str()) > 0; 
		}

		return false;
	} 
};

std::string& trim(std::string &s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

bool isChineseChar(const char* szText)
{

	int i = 0, nLen = strlen(szText); 
	for(; i < nLen; i++) 
	{ 
		if( szText[i] >= 0 && szText[i] <= 127 ) //不是全角字符？ 
			continue;
		else
			return true;
	}

	return false;
}

bool isNumberic(wxString & str)
{
	std::string s = std::string(str.mb_str());
	trim(s);
	if (isChineseChar(s.c_str()))
		return false;

	for ( size_t i = 0; i < s.size();  ++i )
	{
		if ('-' == s[i])
		{
			continue;
		}

		if ( 0 == isdigit( s[i] ) )
		{
			return false;
		}
	}

	return true;
}

bool wxGenericGrid::SortCol(long col, bool isAscending)
{
	if (col < 0 || col >= GetColumnCount() || GetItemCount() < 1)
	{
		return false;
	}

	std::list<SORT_DATA> ls;
	std::vector< std::vector<TABLE_BEFORE_ORDER> > table;
	std::vector<TABLE_BEFORE_ORDER> rowdata;
	wxString str;

	//仅判断排序列中的第一个数据，来决定这个类是哪类数据，可能不准确
	unsigned int sorttype = isNumberic(GetCellValue(0, col));

	//保存排序前的必要数据
	for (int row=0; row < GetItemCount(); ++row)
	{
		for (int column=0; column < GetColumnCount(); ++column)
		{
			TABLE_BEFORE_ORDER beforeOrder;
			beforeOrder.cellText = GetCellValue(row, column);
			beforeOrder.cellClr = GetCellTextColour(row, column);
			str = beforeOrder.cellText;

			rowdata.push_back(beforeOrder);
			if (column == col)
			{
				SORT_DATA tmp;
				tmp.row = row;
				tmp.sorttype = sorttype;

				if (NUMBERIC_TYPE == sorttype)
					str.ToDouble(&(tmp.dData));
				else
					tmp.data = std::string(str.mb_str());

				ls.push_back(tmp);
			}

		}

		table.push_back(rowdata);
		rowdata.clear();
	}

	//进行排序
	if (isAscending)
		ls.sort();
	else
	{
        std::greater<SORT_DATA> pt;
		ls.sort(pt);
	}

	//按照排序后顺序重新填写数据
	std::list<SORT_DATA>::iterator iter = ls.begin();
	long dest=0;
	std::map<long,long> mapData;
	std::map<long,long>::iterator iterdata;
	long i=0;
	for (; iter != ls.end(); ++iter)
	{
		SORT_DATA & item = *iter;

		for (int column=0; column < GetColumnCount(); ++column)
		{
			TABLE_BEFORE_ORDER & it = table[item.row][column];
			SetCellValue(it.cellText, dest, column);
			SetCellTextColour(dest, column, it.cellClr);
		}
		
		++dest;

		//更新数据项，保证排序后数据刷新的位置正确
		iterdata = m_mapItemData.find(item.row);
		if (iterdata != m_mapItemData.end())
		{
			mapData.insert( std::make_pair(i,iterdata->second) );
			++i;
		}
	}

	m_mapItemData.clear();
	m_mapItemData = mapData;

// 	wxString lable1 = GetColLabelValue(col);
// 	wxString lable = lable1.Left(lable1.Find("("));
// 	if (isAscending)
// 	{
// 		lable.Append("(↑)");
// 	}
// 	else
// 	{
// 		lable.Append("(↓)");
// 	}
// 	SetColLabelValue(col, lable);

	return true;
}

// bool wxGenericGrid::ChangeRowData(long src, long dest)
// {
// 	if (src < 0 || src >= GetItemCount() || dest < 0 || dest >= GetItemCount())
// 	{
// 		return false;
// 	}
// 
// 	wxString str;
// 	for (int col=0; col < GetColumnCount(); ++col)
// 	{
// 		str = GetCellValue(dest, col);
// 		SetCellValue(GetCellValue(src, col), dest, col);
// 		SetCellValue(str, src, col);
// 	}
// 
// 	return true;
// }
//////////////////////////////////////////////////////////////////////////
void wxGenericGrid::OnGridCellRightClick(wxGridEvent& event)
{ 
	int row = event.GetRow();
	SelectBlock(row,0,row,GetNumberCols()-1);
	MakeCellVisible(row,event.GetCol());
	SetCurrentCell(event.GetRow(),event.GetCol());
	SetItemState( row,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);

	wxContextMenuEvent evt(wxEVT_COMMAND_LIST_EXT_CONTEXT_MENU);
	evt.SetId(event.GetEventType()==wxEVT_GRID_CELL_RIGHT_CLICK?1:0);
	ProcessEvent(evt);
	event.Skip();
}

void wxGenericGrid::OnGridCellLeftClick(wxGridEvent& event)
{

	int ctrltype = -1;
	int col = event.GetCol();
	ctrltype = GetColumnCtrlType(col);
	int row = event.GetRow();

	if (event.ControlDown())
	{
	}
	else
	{
		SelectRow(row);
		HighlightLine(row, true);
	}

	if (ctrltype==COLUMN_CTRL_TYPE_CHECKBOX)
	{
		bool b = GetItemChecked(row);
		SetItemChecked(row,!b);
		SetColFormatBool(col);
		wxGrid::Refresh();
	}
	else
	{
		event.Skip();
	}

}

void wxGenericGrid::OnGridCellLeftDClick(wxGridEvent& event)
{
	wxExtListEvent le( wxEVT_COMMAND_LIST_EXT_ITEM_ACTIVATED, GetId() );
	le.SetEventObject( this );
	le.m_itemIndex = event.GetRow();

	le.m_col = event.GetCol();
	le.m_pointDrag = GetPosition();
	GetItem( le.m_item );

	ProcessEvent( le );
	le.IsAllowed();
}

void wxGenericGrid::ChangeRow(long row, bool isUp)
{
	if (row < 0)
	{
		return;
	}

	if (isUp)
	{
		if (row >= GetItemCount()/* || row-1 >= GetItemCount()*/)
		{
			return;
		}

		wxExtListItem item;
		item.SetId(row);
		item.SetColumn(0);
		GetItem( item );

		wxString str = GetCellValue(row,0);
		SetCellValue(GetCellValue(row-1,0),row,0);
		SetCellValue(str, row-1,0);

		SetItemState( row-1,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);

		if (!m_mapCheck.empty())
		{
			std::map<int,bool>::iterator iter = m_mapCheck.find(row);
			std::map<int,bool>::iterator foreiter = m_mapCheck.find(row-1);
			if (foreiter == m_mapCheck.end() || iter == m_mapCheck.end())
			{
				return;
			}

			bool b = iter->second;
			iter->second = foreiter->second;
			foreiter->second = b;
		}

		if (!m_mapItemData.empty())
		{
			std::map<long,long> mapData;
			mapData.insert(m_mapItemData.begin(),m_mapItemData.find(row-1));
			mapData.insert( std::make_pair(row-1, GetItemData(row)) );
			mapData.insert( std::make_pair(row, GetItemData(row-1)) );
			for (size_t currow = row+1; currow < m_mapItemData.size(); ++currow)
			{
				mapData.insert( std::make_pair(currow, GetItemData(currow)) );
			}

			m_mapItemData.clear();
			m_mapItemData = mapData;
		}

	}
	else
	{
		if (/*row >= GetItemCount() || */row+1 >= GetItemCount())
		{
			return;
		}

		wxExtListItem item;
		item.SetId(row);
		item.SetColumn(0);
		GetItem( item );

		wxString str = GetCellValue(row,0);
		SetCellValue(GetCellValue(row+1,0),row,0);
		SetCellValue(str, row+1,0);

		SetItemState( row+1,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);

		if (!m_mapCheck.empty())
		{
			std::map<int,bool>::iterator iter = m_mapCheck.find(row);
			std::map<int,bool>::iterator nextiter = m_mapCheck.find(row+1);
			if (nextiter == m_mapCheck.end() || iter == m_mapCheck.end())
			{
				return;
			}

			bool b = iter->second;
			iter->second = nextiter->second;
			nextiter->second = b;
		}

		if (!m_mapItemData.empty())
		{
			std::map<long,long> mapData;
			mapData.insert(m_mapItemData.begin(),m_mapItemData.find(row));
			mapData.insert( std::make_pair(row, GetItemData(row+1)) );
			mapData.insert( std::make_pair(row+1, GetItemData(row)) );
			for (size_t currow = row+2; currow < m_mapItemData.size(); ++currow)
			{
				mapData.insert( std::make_pair(currow, GetItemData(currow)) );
			}

			m_mapItemData.clear();
			m_mapItemData = mapData;
		}
	}

	wxGrid::Refresh();
}

void wxGenericGrid::OnKeyDown( wxKeyEvent& event )
{	

	if (event.ShiftDown())
	{
	}
	else if (event.ControlDown())
	{
	}
	else
	{
		wxGrid::OnKeyDown(event);
		SelectBlock(GetGridCursorRow(),0,GetGridCursorRow(),GetNumberCols()-1);
		MakeCellVisible(GetGridCursorRow(),GetGridCursorCol());
	}

}

//实际行的Rect，不考虑是否可见
void wxGenericGrid::GetItemRect( long index, wxRect &rect )
{
	if (index < 0 || index >= GetItemCount())
	{
		return;
	}

	wxRect rc0=CellToRect(index,0);
	wxRect rcMax=CellToRect(index,GetColumnCount()-1);
	rect.x=rc0.x;
	rect.y=rc0.y;
	rect.width=rcMax.x+rcMax.width-rc0.x;
	rect.height=rc0.height;

}

void wxGenericGrid::DrawColLabel( wxDC& dc, int col )
{
	wxGrid::DrawColLabel(dc, col);

	if (m_nSortCol != -1)
	{
		int colLeft = GetColLeft(m_nSortCol);

		wxRect rect;
		rect.SetX( colLeft + 0 );
		rect.SetY( 0 );
		rect.SetWidth( GetColWidth(m_nSortCol) - 0 );
		rect.SetHeight( GetColLabelSize() - 1 );

		wxHeaderButtonParams headButtonParams;
		headButtonParams.m_labelText = GetColLabelValue(m_nSortCol);

		if (m_bIsAscending)
		{
			wxRendererNative::Get().DrawHeaderButton(this, dc, rect, wxCONTROL_CURRENT, wxHDR_SORT_ICON_UP, &headButtonParams);
		}
		else
		{
			wxRendererNative::Get().DrawHeaderButton(this, dc, rect, wxCONTROL_CURRENT, wxHDR_SORT_ICON_DOWN, &headButtonParams);
		}

	}

}

void wxGenericGrid::OnGridCellChange(wxGridEvent& event)
{
	wxExtListEvent le( wxEVT_COMMAND_LIST_EXT_END_LABEL_EDIT, GetId() );
	le.SetEventObject( this );
	le.m_itemIndex = event.GetRow();
	le.m_col = event.GetCol();
	le.m_item.m_text = GetCellValue(event.GetRow(),event.GetCol());
	ProcessEvent( le );
	le.IsAllowed();
}

