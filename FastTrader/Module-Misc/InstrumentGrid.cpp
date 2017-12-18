#include "stdafx.h"
#include "InstrumentGrid.h"
InstrumentGridTable::InstrumentGridTable(vector<SimpleInstrumentInfo>& instruments,vector<string>& headers):
m_Instruments(instruments)
,m_headers(headers)
{
}
wxString InstrumentGridTable::GetColLabelValue( int col )
{
	if(col>=0&&col<(int)m_headers.size())
		return m_headers[col].c_str();
	return "";
}
BEGIN_EVENT_TABLE(InstrumentGrid, wxGrid)
	EVT_KEY_DOWN( InstrumentGrid::OnKeyDown )
END_EVENT_TABLE()
InstrumentGrid::InstrumentGrid( wxWindow *parent,
			wxWindowID id,
			const wxPoint& pos ,
			const wxSize& size,
			long style,
			const wxString& name):
wxGrid(parent,id,pos,size,style,name)
{
	SetColLabelSize(23);
	EnableDragGridSize(false);
	SetRowLabelSize( 0 );
	EnableDragRowSize(false);
	EnableEditing(false);
	EnableGridLines(true);
	SetScrollLineY(GetDefaultRowSize());
	SetCellHighlightPenWidth(0);
	SetCellHighlightROPenWidth(0);
	SetDefaultColSize(75);
}
void InstrumentGrid::AddInstrumentGridTable(vector<SimpleInstrumentInfo>& instruments,vector<string>& headers)
{
	InstrumentGridTable* p=new InstrumentGridTable(instruments,headers);
    if(headers.size())
        SetDefaultColSize(150/headers.size());
	m_InstrumentTableArray.push_back(p);
}
bool InstrumentGrid::SetTable(int index)
{
	if(index>=0&&index<(int)m_InstrumentTableArray.size())
		return wxGrid::SetTable(m_InstrumentTableArray[index],false,wxGrid::wxGridSelectRows);
	return false;
}
InstrumentGrid::~InstrumentGrid()
{
	wxGrid::SetTable(NULL,false,wxGrid::wxGridSelectRows);
	for(size_t index=0;index<m_InstrumentTableArray.size();index++)
	{
		InstrumentGridTable* p=m_InstrumentTableArray[index];
		if(p) delete p;
	}
	m_InstrumentTableArray.clear();
}
bool InstrumentGrid::GetSelections(vector<int>& rows)
{
	if(GetTable()==NULL) return false;
	wxGridCellCoordsArray tls=GetSelectionBlockTopLeft();
	wxGridCellCoordsArray brs=GetSelectionBlockBottomRight();
	for(int i=0;i<(int)tls.size();i++)
	{
		for(int row=tls[i].GetRow();row<=brs[i].GetRow();++row)
		{
			rows.push_back(row);
		}
	}
	return true;
}
wxString InstrumentGrid::GetValue( int row, int col )
{
	if(GetTable()==NULL) return "";
	return GetTable()->GetValue(row,col);
}
int InstrumentGrid::GetNumberRows()
{
	if(GetTable()==NULL) return  0;
	return GetTable()->GetNumberRows();
}
void InstrumentGrid::OnKeyDown( wxKeyEvent& event )
{
	wxGrid::OnKeyDown(event);

	int nKeyCode = event.GetKeyCode();
	if ( nKeyCode == WXK_UP || nKeyCode == WXK_DOWN )
	{
		int nCursorRow = GetGridCursorRow();
		SelectBlock(nCursorRow,0,nCursorRow,GetNumberCols()-1);
		MakeCellVisible(nCursorRow,GetGridCursorCol());
	}
}
