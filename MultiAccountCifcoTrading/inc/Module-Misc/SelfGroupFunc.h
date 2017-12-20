#pragma once
#include <wx/treectrl.h>
#include <set>
#include <string>
#include <vector>
#include <map>
using namespace std;
class ExchangeItemData:public wxTreeItemData
{
public:	
	ExchangeItemData(const std::string& datas){SetData(datas);}
	void SetData(const std::string& datas){m_Exchange=datas;}
	void GetData(std::string& datas){datas=m_Exchange;}
private:
	std::string m_Exchange;
};
class ProductItemData:public wxTreeItemData
{
public:	
	ProductItemData(const std::set<std::string>& datas){SetData(datas);}
	void SetData(const std::set<std::string>& datas){m_Instruments=datas;}
	void GetData(std::set<std::string>& datas){datas=m_Instruments;}
private:
	std::set<std::string> m_Instruments;
};

class ContractItemData:public wxTreeItemData
{
public:	
	ContractItemData(const std::string& datas){SetData(datas);}
	void SetData(const std::string& datas){m_Instrument=datas;}
	void GetData(std::string& datas){datas=m_Instrument;}
private:
	std::string m_Instrument;
};
class SelfGroupFunc
{
public:
    SelfGroupFunc(wxTreeCtrl* treectrl);
	void DoTreeItemExpanding(wxTreeEvent& event);
	string GetActivedTreeItem(wxTreeEvent& event);
	int IsTypeNode(wxTreeItemId Id);
	wxString DisplayInstrument(wxString  strTypeName,wxString srcstring);
	void AddTreeData();
	void GetSelections(vector<string>& contracts);
private:
	wxTreeCtrl* m_treeBase;
	wxTreeItemId m_root;
};
