#pragma once
#include "wx/grid.h"
#include "cfgMgr/CfgMgr.h"
class MainGrid : public wxGrid
{
public:
	MainGrid(){}
	MainGrid( wxWindow *parent,
            wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxWANTS_CHARS);
	~MainGrid(void);
	bool IsRowVisible( int row,bool wholeCellVisible = true );
	void RefreshRow(int row);
    bool SetTable( wxGridTableBase *table, bool takeOwnership = false,
                   wxGrid::wxGridSelectionModes selmode =
                   wxGrid::wxGridSelectCells);

	bool GetSelections(vector<int>& vRows);
	void AutoAdjustColumnWidth();
    void SelectCurrentRow();
	void OnMainContextMenu( wxMouseEvent& event );
	void OnKeyDown( wxKeyEvent& event );
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(MainGrid)

private:
	wxString GetColumnLongestText(long nCol );
};
class QuotGridTable : public wxGridTableBase
{
public:
	QuotGridTable(int id);
	int GetNumberRows(){return m_rowcount;}
    int GetNumberCols();
    wxString GetValue( int row, int col );
	wxGridCellAttr *GetAttr( int row, int col,wxGridCellAttr::wxAttrKind  kind );
	wxString GetColLabelValue( int col );
    void SetValue( int , int , const wxString&  ) { /* ignore */ }
    bool IsEmptyCell( int , int  ) { return false; }
	void LoadNewQuotData(PlatformStru_DepthMarketData& rawQuot,bool bRefresh=true);
	int GetIdFromColumn(int col);
	string GetInstrumentIDFromRow(int row);
	char GetDirectionFromColumn(int Col);
	int GetRowFromInstrumentID(string InstrumentID);
public:
	void SetNumberRows(int count){m_rowcount=count;}
	void SetTextData(map<int,vector<string>>* pData){m_pTextData=pData;}
	void DeleteRows( vector<int> vRows );
protected:
	void SetPriceChangeColor(wxGridCellAttr * pCellAttr,int row,int col);
private:
	CfgMgr *m_pCfgMgr;
	int      m_id;
	int      m_rowcount;
	map<int,vector<string>>* m_pTextData; 
};