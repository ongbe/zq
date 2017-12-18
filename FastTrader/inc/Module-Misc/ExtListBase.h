///////////////////////////////////////////////////////////////////////////////
// Name:        wx/listctrl.h
// Purpose:     wxExtListCtrl class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     04.12.99
// RCS-ID:      $Id: listbase.h,v 1.2 2006/09/23 13:12:36 cvs Exp $
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_EXTLISTBASE_H_BASE_
#define _WX_EXTLISTBASE_H_BASE_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//    #pragma interface "listctrlbase.h"
#endif

#include "wx/defs.h"


#include "wx/colour.h"
#include "wx/font.h"
#include "wx/gdicmn.h"

#include "wx/event.h"

#include "wx/settings.h"

// ----------------------------------------------------------------------------
// types
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// wxExtListCtrl constants
// ----------------------------------------------------------------------------

// style flags
#define wxLCEXT_VRULES          0x0001
#define wxLCEXT_HRULES          0x0002

#define wxLCEXT_ICON            0x0004
#define wxLCEXT_SMALL_ICON      0x0008
#define wxLCEXT_LIST            0x0010
#define wxLCEXT_REPORT          0x0020

#define wxLCEXT_ALIGN_TOP       0x0040
#define wxLCEXT_ALIGN_LEFT      0x0080
#define wxLCEXT_AUTOARRANGE     0x0100
#define wxLCEXT_VIRTUAL         0x0200
#define wxLCEXT_EDIT_LABELS     0x0400
#define wxLCEXT_NO_HEADER       0x0800
#define wxLCEXT_NO_SORT_HEADER  0x1000
#define wxLCEXT_SINGLE_SEL      0x2000
#define wxLCEXT_SORT_ASCENDING  0x4000
#define wxLCEXT_SORT_DESCENDING 0x8000
#define wxLCEXT_NO_HIGHLIGHT    0x10000
#define wxLCEXT_STICKY_HIGHLIGHT    0x20000
#define wxLCEXT_STICKY_NOSELEVENT    0x40000
#define wxLCEXT_SEND_LEFTCLICK    0x80000
#define wxLCEXT_TOGGLE_COLOUR     0x100000
#define wxLCEXT_NO_REFRESH_BACKGROUND   0x200000
#define wxLCEXT_MASK_TYPE       (wxLCEXT_ICON | wxLCEXT_SMALL_ICON | wxLCEXT_LIST | wxLCEXT_REPORT)
#define wxLCEXT_MASK_ALIGN      (wxLCEXT_ALIGN_TOP | wxLCEXT_ALIGN_LEFT)
#define wxLCEXT_MASK_SORT       (wxLCEXT_SORT_ASCENDING | wxLCEXT_SORT_DESCENDING)

// for compatibility only
#define wxLCEXT_USER_TEXT       wxLCEXT_VIRTUAL

// Omitted because
//  (a) too much detail
//  (b) not enough style flags
//  (c) not implemented anyhow in the generic version
//
// #define wxLCEXT_NO_SCROLL
// #define wxLCEXT_NO_LABEL_WRAP
// #define wxLCEXT_OWNERDRAW_FIXED
// #define wxLCEXT_SHOW_SEL_ALWAYS

// Mask flags to tell app/GUI what fields of wxExtListItem are valid
#define wxLISTEXT_MASK_STATE           0x0001
#define wxLISTEXT_MASK_TEXT            0x0002
#define wxLISTEXT_MASK_IMAGE           0x0004
#define wxLISTEXT_MASK_DATA            0x0008
#define wxLISTEXT_SET_ITEM             0x0010
#define wxLISTEXT_MASK_WIDTH           0x0020
#define wxLISTEXT_MASK_FORMAT          0x0040
#define wxLISTEXT_MASK_FONTCOLOR       0x0080
#define wxLISTEXT_MASK_FONT            0x0100
#define wxLISTEXT_MASK_CTRLTYPE        0x0200
// State flags for indicating the state of an item
#define wxLISTEXT_STATE_DONTCARE       0x0000
#define wxLISTEXT_STATE_DROPHILITED    0x0001      // MSW only
#define wxLISTEXT_STATE_FOCUSED        0x0002
#define wxLISTEXT_STATE_SELECTED       0x0004
#define wxLISTEXT_STATE_CUT            0x0008      // MSW only
#define wxLISTEXT_STATE_DISABLED       0x0010      // OS2 only
#define wxLISTEXT_STATE_FILTERED       0x0020      // OS2 only
#define wxLISTEXT_STATE_INUSE          0x0040      // OS2 only
#define wxLISTEXT_STATE_PICKED         0x0080      // OS2 only
#define wxLISTEXT_STATE_SOURCE         0x0100      // OS2 only

// Hit test flags, used in HitTest
#define wxLISTEXT_HITTEST_ABOVE            0x0001  // Above the client area.
#define wxLISTEXT_HITTEST_BELOW            0x0002  // Below the client area.
#define wxLISTEXT_HITTEST_NOWHERE          0x0004  // In the client area but below the last item.
#define wxLISTEXT_HITTEST_ONITEMICON       0x0020  // On the bitmap associated with an item.
#define wxLISTEXT_HITTEST_ONITEMLABEL      0x0080  // On the label (string) associated with an item.
#define wxLISTEXT_HITTEST_ONITEMRIGHT      0x0100  // In the area to the right of an item.
#define wxLISTEXT_HITTEST_ONITEMSTATEICON  0x0200  // On the state icon for a tree view item that is in a user-defined state.
#define wxLISTEXT_HITTEST_TOLEFT           0x0400  // To the left of the client area.
#define wxLISTEXT_HITTEST_TORIGHT          0x0800  // To the right of the client area.

#define wxLISTEXT_HITTEST_ONITEM (wxLISTEXT_HITTEST_ONITEMICON | wxLISTEXT_HITTEST_ONITEMLABEL | wxLISTEXT_HITTEST_ONITEMSTATEICON)

// Flags for GetNextItem (MSW only except wxLISTEXT_NEXT_ALL)
enum
{
    wxLISTEXT_NEXT_ABOVE,          // Searches for an item above the specified item
    wxLISTEXT_NEXT_ALL,            // Searches for subsequent item by index
    wxLISTEXT_NEXT_BELOW,          // Searches for an item below the specified item
    wxLISTEXT_NEXT_LEFT,           // Searches for an item to the left of the specified item
    wxLISTEXT_NEXT_RIGHT           // Searches for an item to the right of the specified item
};

// Alignment flags for Arrange (MSW only except wxLISTEXT_ALIGN_LEFT)
enum
{
    wxLISTEXT_ALIGN_DEFAULT,
    wxLISTEXT_ALIGN_LEFT,
    wxLISTEXT_ALIGN_TOP,
    wxLISTEXT_ALIGN_SNAP_TO_GRID
};

// Column format (MSW only except wxLISTEXT_FORMAT_LEFT)
enum wxExtListColumnFormat
{
    wxLISTEXT_FORMAT_LEFT,
    wxLISTEXT_FORMAT_RIGHT,
    wxLISTEXT_FORMAT_CENTRE,
    wxLISTEXT_FORMAT_CENTER = wxLISTEXT_FORMAT_CENTRE
};

// Autosize values for SetColumnWidth
enum
{
    wxLISTEXT_AUTOSIZE = -1,
    wxLISTEXT_AUTOSIZE_USEHEADER = -2      // partly supported by generic version
};

// Flag values for GetItemRect
enum
{
    wxLISTEXT_RECT_BOUNDS,
    wxLISTEXT_RECT_ICON,
    wxLISTEXT_RECT_LABEL
};

// Flag values for FindItem (MSW only)
enum
{
    wxLISTEXT_FIND_UP,
    wxLISTEXT_FIND_DOWN,
    wxLISTEXT_FIND_LEFT,
    wxLISTEXT_FIND_RIGHT
};
enum
{
    COLUMN_CTRL_TYPE_NORMAL=0,
    COLUMN_CTRL_TYPE_EDIT,
    COLUMN_CTRL_TYPE_CHECKBOX,
    COLUMN_CTRL_TYPE_COMBOBOX
};
// ----------------------------------------------------------------------------
// wxExtListItemAttr: a structure containing the visual attributes of an item
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListItemAttr
{
public:
    // ctors
    wxExtListItemAttr() { }
    wxExtListItemAttr(const wxColour& colText,
                   const wxColour& colBack,
                   const wxFont& font)
        : m_colText(colText), m_colBack(colBack), m_font(font) { }

    // setters
    void SetTextColour(const wxColour& colText) { m_colText = colText; }
    void SetBackgroundColour(const wxColour& colBack) { m_colBack = colBack; }
    void SetFont(const wxFont& font) { m_font = font; }

    // accessors
    bool HasTextColour() const { return m_colText.Ok(); }
    bool HasBackgroundColour() const { return m_colBack.Ok(); }
    bool HasFont() const { return m_font.Ok(); }

    const wxColour& GetTextColour() const { return m_colText; }
    const wxColour& GetBackgroundColour() const { return m_colBack; }
    const wxFont& GetFont() const { return m_font; }

private:
    wxColour m_colText,
    m_colBack;
    wxFont   m_font;
};

// ----------------------------------------------------------------------------
// wxExtListItem: the item or column info, used to exchange data with wxExtListCtrl
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListItem : public wxObject
{
public:
    wxExtListItem() { Init(); m_attr = NULL; }
    wxExtListItem(const wxExtListItem& item)
        : wxObject(),
          m_mask(item.m_mask),
          m_itemId(item.m_itemId),
          m_col(item.m_col),
          m_state(item.m_state),
          m_stateMask(item.m_stateMask),
          m_text(item.m_text),
          m_image(item.m_image),
          m_data(item.m_data),
		  m_colData(item.m_colData),
          m_icolData(item.m_icolData),
          m_dcolData(item.m_dcolData),
          m_icolDataValid(item.m_icolDataValid),
          m_dcolDataValid(item.m_dcolDataValid),
          m_btrynumericAsComparing(item.m_btrynumericAsComparing),
          m_format(item.m_format),
          m_width(item.m_width),
	  m_color(item.m_color),
	  m_font(item.m_font),
          m_attr(NULL)
    {
        // copy list item attributes
        if( item.HasAttributes() )
            m_attr = new wxExtListItemAttr(*item.GetAttributes());
    }
    virtual ~wxExtListItem() { delete m_attr; }

    // resetting
    void Clear() { Init(); m_text.clear(); ClearAttributes(); }
    void ClearAttributes() { if ( m_attr ) { delete m_attr; m_attr = NULL; } }

    //保持Item的UserData不变(清除UserData相关的Mask位)
    void HoldItemData(void)
    {
        m_mask &= (~wxLISTEXT_MASK_DATA);
    }

    // setters
    void SetMask(long mask)
        { m_mask = mask; }
    void SetId(long id)
        { m_itemId = id; }
    void SetColumn(int col)
        { m_col = col; }
    void SetState(long state)
        { m_mask |= wxLISTEXT_MASK_STATE; m_state = state; m_stateMask |= state; }
    void SetStateMask(long stateMask)
        { m_stateMask = stateMask; }
    void SetText(const wxString& text)
        { m_mask |= wxLISTEXT_MASK_TEXT; m_text = text; }
    void SetImage(int image)
        { m_mask |= wxLISTEXT_MASK_IMAGE; m_image = image; }
    void SetData(long data)
        { m_mask |= wxLISTEXT_MASK_DATA; m_data = data; }
    void SetData(void *data)
        { m_mask |= wxLISTEXT_MASK_DATA; m_data = wxPtrToUInt(data); }
	void SetColData(long data)
		{ m_colData = data; }
    //下面两个函数用于排序比较。 added by l. 20120426
    void SetiColData(int data)
        {   m_icolData = data;  if(!m_icolDataValid) m_icolDataValid=true; }
    void SetdColData(double data)
        {   m_dcolData = data;  if(!m_dcolDataValid) m_dcolDataValid=true; }
    void SetbtrynumericAsComparing(bool btrynumericAsComparing)
        {   m_btrynumericAsComparing=btrynumericAsComparing; }

    void SetWidth(int width)
        { m_mask |= wxLISTEXT_MASK_WIDTH; m_width = width; }
    void SetAlign(wxExtListColumnFormat align)
        { m_mask |= wxLISTEXT_MASK_FORMAT; m_format = align; }
    void SetCtrlType(int t)
        { m_mask |= wxLISTEXT_MASK_CTRLTYPE; m_ctrltype = t; }
 /*   void SetTextColour(const wxColour& colText)
        { Attributes().SetTextColour(colText); }
    void SetBackgroundColour(const wxColour& colBack)
        { Attributes().SetBackgroundColour(colBack); }
    void SetFont(const wxFont& font)
        { Attributes().SetFont(font); }*/
   void SetTextColour(const wxColour& colText)
        {m_mask |= wxLISTEXT_MASK_FONTCOLOR; m_color = colText;  }
    void SetFont(const wxFont& font)
        {m_mask |= wxLISTEXT_MASK_FONT; m_font = font;  }
    void SetBackgroundColour(const wxColour& colBack)
        { Attributes().SetBackgroundColour(colBack); }
    // accessors
    long GetMask() const { return m_mask; }
    long GetId() const { return m_itemId; }
    int GetColumn() const { return m_col; }
    long GetState() const { return m_state & m_stateMask; }
    const wxString& GetText() const { return m_text; }
    int GetImage() const { return m_image; }
    wxUIntPtr GetData() const { return m_data; }
	long GetColData() const	{ return m_colData; }
    //下面两个函数用于排序比较。Added by l. 20120426
    bool GetiColData(int& rltdata) const	
    { if(m_icolDataValid) rltdata=m_icolData;  return m_icolDataValid?true:false; }
    bool GetdColData(double& rltdata) const	
    { if(m_dcolDataValid) rltdata=m_dcolData;  return m_dcolDataValid?true:false; }
    bool GetbtrynumericAsComparing(void) const
    { return m_btrynumericAsComparing; }

    int GetWidth() const { return m_width; }
    wxExtListColumnFormat GetAlign() const { return (wxExtListColumnFormat)m_format; }

    wxExtListItemAttr *GetAttributes() const { return m_attr; }
    bool HasAttributes() const { return m_attr != NULL; }
    int GetCtrlType() const { return m_ctrltype; }
   /* wxColour GetTextColour() const
        { return HasAttributes() ? m_attr->GetTextColour() : wxNullColour; }
    wxColour GetBackgroundColour() const
        { return HasAttributes() ? m_attr->GetBackgroundColour()
                                 : wxNullColour; }
    wxFont GetFont() const
        { return HasAttributes() ? m_attr->GetFont() : wxNullFont; }*/
    wxColour GetTextColour() const
        { return m_color; }
    wxFont GetFont() const
        { return m_font; }
    wxColour GetBackgroundColour() const
        { return HasAttributes() ? m_attr->GetBackgroundColour()
                                 : wxNullColour; }
    // this conversion is necessary to make old code using GetItem() to
    // compile
    operator long() const { return m_itemId; }

    // these members are public for compatibility

    long            m_mask;     // Indicates what fields are valid
    long            m_itemId;   // The zero-based item position
    int             m_col;      // Zero-based column, if in report mode
    long            m_state;    // The state of the item
    long            m_stateMask;// Which flags of m_state are valid (uses same flags)
    wxString        m_text;     // The label/header text
    int             m_image;    // The zero-based index into an image list
    wxUIntPtr       m_data;     // App-defined data
	long			m_colData;	// Column data, used for sort

    int             m_icolData; // 指定Item对应的Int值，用于排序比较。 added by l. 20120426
    double          m_dcolData; // 指定Item对应的double值，用于排序比较。 added by l. 20120426
    bool            m_icolDataValid;//m_icolData是否有效
    bool            m_dcolDataValid;//m_dcolData是否有效
    bool            m_btrynumericAsComparing;//比较时是否尝试转换为数值进行比较

    // For columns only
    int             m_format;   // left, right, centre
    int             m_width;    // width of column
    int             m_ctrltype;//child control type :edit,checkbox,combobox
    wxColor	    m_color;
    wxFont	    m_font;
#ifdef __WXPM__
    int             m_miniImage; // handle to the mini image for OS/2
#endif

protected:
    // creates m_attr if we don't have it yet
    wxExtListItemAttr& Attributes()
    {
        if ( !m_attr )
            m_attr = new wxExtListItemAttr;

        return *m_attr;
    }

    void Init()
    {
        m_mask = 0;
        m_itemId = 0;
        m_col = 0;
        m_state = 0;
        m_stateMask = 0;
        m_image = 0;
        m_data = 0;
		m_colData = -1;

        //下面四个变量用于排序比较。Added by l. 20120426
        m_icolData = 0; 
        m_dcolData = 0.0;
        m_icolDataValid = false;
        m_dcolDataValid = false;
        m_btrynumericAsComparing = false;

        m_format = wxLISTEXT_FORMAT_CENTRE;
        m_width = 0;
	
	m_color = wxColor(0, 0, 0);
	m_font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT );
    }

    wxExtListItemAttr *m_attr;     // optional pointer to the items style

private:
    // VZ: this is strange, we have a copy ctor but not operator=(), why?
    wxExtListItem& operator=(const wxExtListItem& item);

    DECLARE_DYNAMIC_CLASS(wxExtListItem)
};


// type of compare function for wxExtListCtrl sort operation
typedef int (wxCALLBACK *wxExtListCtrlCompare)(const wxExtListItem& item1, const wxExtListItem& item2, long sortData);

// ----------------------------------------------------------------------------
// wxExtListEvent - the event class for the wxExtListCtrl notifications
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxExtListEvent : public wxNotifyEvent
{
public:
    wxExtListEvent(wxEventType commandType = wxEVT_NULL, int winid = 0)
        : wxNotifyEvent(commandType, winid)
        , m_code(0)
        , m_oldItemIndex(0)
        , m_itemIndex(0)
        , m_col(0)
        , m_pointDrag()
        , m_item()
        ,m_sort_asc(false)
        , m_editCancelled(false)
        { }

    wxExtListEvent(const wxExtListEvent& event)
        : wxNotifyEvent(event)
        , m_code(event.m_code)
        , m_oldItemIndex(event.m_oldItemIndex)
        , m_itemIndex(event.m_itemIndex)
        , m_col(event.m_col)
        , m_pointDrag(event.m_pointDrag)
        , m_item(event.m_item)
        ,m_sort_asc(event.m_sort_asc)
        , m_editCancelled(event.m_editCancelled)
        { }

    int GetKeyCode() const { return m_code; }
    long GetIndex() const { return m_itemIndex; }
    int GetColumn() const { return m_col; }
    wxPoint GetPoint() const { return m_pointDrag; }
    const wxString& GetLabel() const { return m_item.m_text; }
    const wxString& GetText() const { return m_item.m_text; }
    int GetImage() const { return m_item.m_image; }
    long GetData() const { return wx_static_cast(long, m_item.m_data); }
	long GetColData() const { return m_item.m_colData; }
    long GetMask() const { return m_item.m_mask; }
    const wxExtListItem& GetItem() const { return m_item; }

    // for wxEVT_COMMAND_LIST_EXT_CACHE_HINT only
    long GetCacheFrom() const { return m_oldItemIndex; }
    long GetCacheTo() const { return m_itemIndex; }
    bool GetSortAsc() const { return m_sort_asc; }
    // was label editing canceled? (for wxEVT_COMMAND_LIST_EXT_END_LABEL_EDIT only)
    bool IsEditCancelled() const { return m_editCancelled; }
    void SetEditCanceled(bool editCancelled) { m_editCancelled = editCancelled; }

#if WXWIN_COMPATIBILITY_2_2
    // these methods don't do anything at all
    long GetOldIndex() const { return 0; }
    long GetOldItem() const { return 0; }

    // this one is superseded by GetKeyCode()
    int GetCode() const { return GetKeyCode(); }
#endif // WXWIN_COMPATIBILITY_2_2

    virtual wxEvent *Clone() const { return new wxExtListEvent(*this); }

//protected: -- not for backwards compatibility
    int           m_code;
    long          m_oldItemIndex; // only for wxEVT_COMMAND_LIST_EXT_CACHE_HINT
    long          m_itemIndex;
    int           m_col;
    bool          m_sort_asc;
    wxPoint       m_pointDrag;

    wxExtListItem    m_item;

protected:
    bool          m_editCancelled;

private:
    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxExtListEvent)
};

// ----------------------------------------------------------------------------
// wxExtListCtrl event macros
// ----------------------------------------------------------------------------

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_BEGIN_DRAG, 700)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_BEGIN_RDRAG, 701)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_BEGIN_LABEL_EDIT, 702)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_END_LABEL_EDIT, 703)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_DELETE_ITEM, 704)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_DELETE_ALL_ITEMS, 705)
#if WXWIN_COMPATIBILITY_2_4
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_GET_INFO, 706)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_SET_INFO, 707)
#endif
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_SELECTED, 708)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_DESELECTED, 709)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_KEY_DOWN, 710)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_INSERT_ITEM, 711)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_CLICK, 712)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_RIGHT_CLICK, 713)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_MIDDLE_CLICK, 714)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_ACTIVATED, 715)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_CACHE_HINT, 716)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_RIGHT_CLICK, 717)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_BEGIN_DRAG, 718)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_DRAGGING, 719)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_COL_END_DRAG, 720)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_FOCUSED, 721)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_ITEM_LEFT_CLICK, 722)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_CONTEXT_MENU, 723)
    DECLARE_EVENT_TYPE(wxEVT_COMMAND_LIST_EXT_CHECKBOX_CHANGED,724)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*wxExtListEventFunction)(wxExtListEvent&);

#define wxExtListEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxExtListEventFunction, &func)

#ifndef wx__DECLARE_EXTLISTEVT
#define wx__DECLARE_EXTLISTEVT(evt, id, fn) \
    wx__DECLARE_EVT1(wxEVT_COMMAND_LIST_EXT_ ## evt, id, wxExtListEventHandler(fn))
#endif

#define EVT_LIST_EXT_BEGIN_DRAG(id, fn) wx__DECLARE_EXTLISTEVT(BEGIN_DRAG, id, fn)
#define EVT_LIST_EXT_BEGIN_RDRAG(id, fn) wx__DECLARE_EXTLISTEVT(BEGIN_RDRAG, id, fn)
#define EVT_LIST_EXT_BEGIN_LABEL_EDIT(id, fn) wx__DECLARE_EXTLISTEVT(BEGIN_LABEL_EDIT, id, fn)
#define EVT_LIST_EXT_END_LABEL_EDIT(id, fn) wx__DECLARE_EXTLISTEVT(END_LABEL_EDIT, id, fn)
#define EVT_LIST_EXT_DELETE_ITEM(id, fn) wx__DECLARE_EXTLISTEVT(DELETE_ITEM, id, fn)
#define EVT_LIST_EXT_DELETE_ALL_ITEMS(id, fn) wx__DECLARE_EXTLISTEVT(DELETE_ALL_ITEMS, id, fn)
#define EVT_LIST_EXT_KEY_DOWN(id, fn) wx__DECLARE_EXTLISTEVT(KEY_DOWN, id, fn)
#define EVT_LIST_EXT_INSERT_ITEM(id, fn) wx__DECLARE_EXTLISTEVT(INSERT_ITEM, id, fn)

#define EVT_LIST_EXT_COL_CLICK(id, fn) wx__DECLARE_EXTLISTEVT(COL_CLICK, id, fn)
#define EVT_LIST_EXT_COL_RIGHT_CLICK(id, fn) wx__DECLARE_EXTLISTEVT(COL_RIGHT_CLICK, id, fn)
#define EVT_LIST_EXT_COL_BEGIN_DRAG(id, fn) wx__DECLARE_EXTLISTEVT(COL_BEGIN_DRAG, id, fn)
#define EVT_LIST_EXT_COL_DRAGGING(id, fn) wx__DECLARE_EXTLISTEVT(COL_DRAGGING, id, fn)
#define EVT_LIST_EXT_COL_END_DRAG(id, fn) wx__DECLARE_EXTLISTEVT(COL_END_DRAG, id, fn)

#define EVT_LIST_EXT_ITEM_SELECTED(id, fn) wx__DECLARE_EXTLISTEVT(ITEM_SELECTED, id, fn)
#define EVT_LIST_EXT_ITEM_DESELECTED(id, fn) wx__DECLARE_EXTLISTEVT(ITEM_DESELECTED, id, fn)
#define EVT_LIST_EXT_ITEM_RIGHT_CLICK(id, fn) wx__DECLARE_EXTLISTEVT(ITEM_RIGHT_CLICK, id, fn)
#define EVT_LIST_EXT_ITEM_LEFT_CLICK(id, fn) wx__DECLARE_EXTLISTEVT(ITEM_LEFT_CLICK, id, fn)
#define EVT_LIST_EXT_ITEM_MIDDLE_CLICK(id, fn) wx__DECLARE_EXTLISTEVT(ITEM_MIDDLE_CLICK, id, fn)
#define EVT_LIST_EXT_ITEM_ACTIVATED(id, fn) wx__DECLARE_EXTLISTEVT(ITEM_ACTIVATED, id, fn)
#define EVT_LIST_EXT_ITEM_FOCUSED(id, fn) wx__DECLARE_EXTLISTEVT(ITEM_FOCUSED, id, fn)

#define EVT_LIST_EXT_CACHE_HINT(id, fn) wx__DECLARE_EXTLISTEVT(CACHE_HINT, id, fn)
#define EVT_LIST_EXT_CHECKBOX_CHANGED(id,fn) wx__DECLARE_EXTLISTEVT(CHECKBOX_CHANGED, id, fn)
#define EVT_LIST_EXT_CONTEXT_MENU(fn) wx__DECLARE_EVT0(wxEVT_COMMAND_LIST_EXT_CONTEXT_MENU, wxContextMenuEventHandler(fn))


#if WXWIN_COMPATIBILITY_2_4
#define EVT_LIST_EXT_GET_INFO(id, fn) wx__DECLARE_EXTLISTEVT(GET_INFO, id, fn)
#define EVT_LIST_EXT_SET_INFO(id, fn) wx__DECLARE_EXTLISTEVT(SET_INFO, id, fn)
#endif


#endif
    // _WX_LISTCTRL_H_BASE_
