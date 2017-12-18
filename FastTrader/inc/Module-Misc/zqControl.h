#pragma once
#include <wx/menu.h>
#include <wx/wxprec.h>
BOOL zqPopupMenu(wxMenu *menu, int x, int y,wxWindow* owner);
class zqStaticText : public wxStaticText
{
  public:
    enum ETransparentStaticTextFlags
      {
        // not really transparent :)
        IsOpaque = 0x0001,

        DefaultFlags = 0
      };

    zqStaticText();
    zqStaticText(wxWindow * ParentWindow,
                            wxWindowID ControlIdent,
                            const wxString& LabelString,
                            const wxPoint& Position = wxDefaultPosition,
                            const wxSize& Size = wxDefaultSize,
                            long Style = 0,
                            const wxString& Name = "TransparentStaticTextControl")
    {
        Create(ParentWindow, ControlIdent, LabelString, Position, Size, Style, Name);
    }

    bool Create(wxWindow * ParentWindow,
                wxWindowID ControlIdent,
                const wxString& LabelString,
                const wxPoint& Position = wxDefaultPosition,
                const wxSize& Size = wxDefaultSize,
                long Style = 0,
                const wxString& Name = "TransparentStaticTextControl");

    void OnEraseBackground (wxEraseEvent & EraseEvent);
    void OnPaint (wxPaintEvent & PaintEvent);
	void OnLeftUp(wxMouseEvent& event);

	void SetCheck(wxWindow * p)
	{
		m_pCheckBox = p;
	}

  private:
    DECLARE_DYNAMIC_CLASS (zqStaticText)
    DECLARE_EVENT_TABLE()

private:
	wxWindow * m_pCheckBox;
	bool  m_AlignRight;
};
//class zqCheckBox: public wxEvtHandler
//{
//public:
//    zqCheckBox();
//    zqCheckBox(wxWindow *parent,
//               wxWindowID id,
//               const wxString& label,
//               const wxPoint& pos = wxDefaultPosition,
//               const wxSize& size = wxDefaultSize,
//               long style = 0,
//               const wxValidator& validator = wxDefaultValidator,
//               const wxString& name = wxCheckBoxNameStr)
//    {
//        Create(parent, id, label, pos, size, style, validator, name);
//    }
//    bool Create(wxWindow *parent,
//               wxWindowID id,
//               const wxString& label,
//               const wxPoint& pos = wxDefaultPosition,
//               const wxSize& size = wxDefaultSize,
//               long style = 0,
//               const wxValidator& validator = wxDefaultValidator,
//               const wxString& name = wxCheckBoxNameStr);
//   ~zqCheckBox(){}
//    virtual void SetValue(bool value);
//    virtual bool GetValue() const;
//    void OnStaticLeftUp(wxMouseEvent& event);
//  private:
//    wxCheckBox* m_pCheckbox;
//    zqStaticText* m_pStaticText;
//};

class ToggleStaticText : public wxStaticText
{
public:
    ToggleStaticText():m_normal(true) { }

    ToggleStaticText(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label1,
                 const wxString& label2,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxBORDER_STATIC,
                 const wxString& name = wxStaticTextNameStr)
    {
        Create(parent, id, label1,label2, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                 const wxString& label1,
                 const wxString& label2,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxBORDER_STATIC,
                const wxString& name = wxStaticTextNameStr)
    {
        m_label1=label1;
        m_label2=label2;
        m_normal=true;
        return wxStaticText::Create(parent, id, label1, pos, size, style, name);
    }
    void OnLeftUp(wxMouseEvent& event);
    void SetNormal(bool status);
    bool IsNormal();
	void GetLabel2(wxString& label1,wxString& label2);
	void SetLabel2(const wxString& label1,const wxString& label2);
private:
    bool m_normal;
    wxString m_label1;
    wxString m_label2;
    DECLARE_DYNAMIC_CLASS (ToggleStaticText)
    DECLARE_EVENT_TABLE()
};
class zqRadioButton : public wxControl
{
public:
    zqRadioButton();
    zqRadioButton(wxWindow *parent,
                  wxWindowID id,
                  const wxString& label,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxRadioButtonNameStr);
    void OnLeftUp(wxMouseEvent& evt);
   void OnEraseBackground (wxEraseEvent & EraseEvent);
   void OnPaint (wxPaintEvent & PaintEvent);
   void SetValue(bool val);
   bool GetValue()  const;
   void SetLabel(wxString val);
   wxString Getlabel()  const;
private:
    bool m_checked;
    wxString m_label;
    DECLARE_DYNAMIC_CLASS (zqRadioButton)
    DECLARE_EVENT_TABLE()
};
class zqNCButton 
{
public:
    zqNCButton(int id,wxString text,bool checked,int offset,bool IsCheckbox=true);
	~zqNCButton();
    int GetRightOffset(){return m_right_offset;}
	void SetRightOffset(int offset){m_right_offset=offset;}
	int GetWidth(){return m_width;}
	wxString GetText();
	void SetText(wxString text){m_strText=text;}
    wxRect& GetRect(){return m_rect;}
    bool GetChecked(){return m_checked;}
    void SetRect(wxRect& rt){m_rect=rt;}
    void SetCheck(bool bCheck){m_checked=bCheck;}
	int  GetId(){return m_id;}
	void SetId(int id){m_id=id;}
	bool  IsCheckbox(){return m_IsCheckbox;}
	void SetCheckbox(bool id){m_IsCheckbox=id;}
	wxBitmap*  GetBitmap(){return m_checked?m_bitmap_press:m_bitmap_normal;}
	void SetBitmap(wxBitmap* bitmap1,wxBitmap* bitmap2);
private:
    int m_right_offset;
	int m_width;
    wxRect m_rect;
    wxString m_strText;
    bool m_checked;
	int  m_id;
	bool m_IsCheckbox;
	wxBitmap* m_bitmap_normal;
	wxBitmap* m_bitmap_press;
};
class zqMenu : public wxMenu
{
public:
    zqMenu(const wxString& title, long style = 0);
    zqMenu(long style = 0); 
    wxMenuItem* Append(int itemid,
                       const wxString& text = wxEmptyString,
                       const wxString& help = wxEmptyString,
                       wxItemKind kind = wxITEM_NORMAL);
};