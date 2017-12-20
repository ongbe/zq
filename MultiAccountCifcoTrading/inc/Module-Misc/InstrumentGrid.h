#include <wx/grid.h> 
#include <vector>
#include "../inc/ISvr/PlatformSvrMgr.h"
using namespace std;
class InstrumentGridTable : public wxGridTableBase
{
public:
	InstrumentGridTable(vector<InstrumentInfo>& instruments,vector<string>& headers);
    int GetNumberRows() { return m_Instruments.size(); }
	int GetNumberCols() { return m_headers.size(); }
	wxString GetColLabelValue( int col );
    wxString GetValue( int row, int col )
    {
		if(col==0)
			return m_Instruments[row].id.c_str();
		if(col==1)
			return m_Instruments[row].name.c_str();
        return "";
    }
	bool IsEmptyCell( int row, int col ){return false;}
    void SetValue( int , int , const wxString&  ) { /* ignore */ }

private:
	int m_rowcount;
	vector<string> m_headers;
	vector<InstrumentInfo> m_Instruments;
};
class InstrumentGrid: public wxGrid
{
public:
        InstrumentGrid( wxWindow *parent,
            wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxWANTS_CHARS|wxBORDER_SIMPLE,
            const wxString& name = wxGridNameStr );
		~InstrumentGrid();
		void AddInstrumentGridTable(vector<InstrumentInfo>& instruments,vector<string>& headers);
		bool SetTable(int index);
		bool GetSelections(vector<int>& rows);
		wxString GetValue( int row, int col );
		int GetNumberRows();
		void OnKeyDown( wxKeyEvent& event );
		DECLARE_EVENT_TABLE()
private:
	vector<InstrumentGridTable*> m_InstrumentTableArray;
};