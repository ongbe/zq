#include "stdafx.h"
#include "SelfGroupFunc.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
#define DUMMY_TEXT   "__dummy_text__"
SelfGroupFunc::SelfGroupFunc(wxTreeCtrl* treectrl):
m_treeBase(treectrl),
m_root(NULL)
{
	if(treectrl)	
		m_root=m_treeBase->AddRoot(wxEmptyString);

}
void SelfGroupFunc::AddTreeData()
{
	if(DEFAULT_SVR()&&m_treeBase)
	{
		vector<string> exchangeIDset;
		DEFAULT_SVR()->GetExchangeIDs(exchangeIDset);
		for(int exch_index=0;exch_index<(int)exchangeIDset.size();++exch_index)
		{
			string exchangename=DEFAULT_SVR()?DEFAULT_SVR()->ExchangeID2String(exchangeIDset[exch_index]):"";
			wxTreeItemId exch_Id=m_treeBase->AppendItem(m_root,exchangename,-1,-1,new ExchangeItemData(exchangeIDset[exch_index]));
            m_treeBase->AppendItem(exch_Id,DUMMY_TEXT);

		}
	}
}
void SelfGroupFunc::GetSelections(vector<string>& contracts)
{
	contracts.clear();
	if(m_treeBase)
	{
		wxArrayTreeItemIds ids;
		m_treeBase->GetSelections(ids);
		std::string instrumentID;
		for (int index=0;index<(int)ids.size();++index)
		{
			if(3==IsTypeNode(m_treeBase->GetItemParent(ids[index])))
			{
				ContractItemData* pData=(ContractItemData*)m_treeBase->GetItemData(ids[index]);
				if(pData)
				{
					pData->GetData(instrumentID);
					contracts.push_back(instrumentID);
				}
			}
		}
	}
}
int SelfGroupFunc::IsTypeNode(wxTreeItemId Id)
{
	wxTreeItemId ActiveId=Id;
	if(!ActiveId)return 0;//无效节点
	ActiveId=m_treeBase->GetItemParent(ActiveId);
	if(!ActiveId)return 1;//根节点
	ActiveId=m_treeBase->GetItemParent(ActiveId);
	if(!ActiveId)return 2;//交易所节点
	ActiveId=m_treeBase->GetItemParent(ActiveId);
	if(!ActiveId)return 3;//品种节点
	return false;
}
wxString SelfGroupFunc::DisplayInstrument(wxString  strTypeName,wxString srcstring)
{
	//Pats合约合成代码
	//strInstrumentID = name + "-" + date + "@" + exchange;
	//for(int pos=strInstrumentID.length()-1;pos>=0;pos--) 
	//	if(strInstrumentID[pos]==' ')strInstrumentID[pos]='-';
	//处理外盘合约名，显示合约名到期时间
	if(srcstring.find('@')!=wxString::npos&&srcstring.find('-')!=wxString::npos)
	{
		wxString strShortName=srcstring.BeforeLast('@');
		strShortName=strShortName.Mid(strTypeName.Length()+1);
		for(int pos=strShortName.length()-1;pos>=0;pos--) 
			if(strShortName[pos]=='-')strShortName[pos]=' ';	
		return strShortName;

	}
	return srcstring;
}
void SelfGroupFunc::DoTreeItemExpanding(wxTreeEvent& event)
{
	if(m_treeBase)
	{
		wxTreeItemId Expand_Id=event.GetItem();
		wxString  strTypeName=m_treeBase->GetItemText(Expand_Id);
		if(2==IsTypeNode(Expand_Id))//交易所
		{
			wxTreeItemIdValue cookie;
			wxTreeItemId dummyID=m_treeBase->GetFirstChild(Expand_Id,cookie);
			if(m_treeBase->GetItemText(dummyID)==DUMMY_TEXT)
			{
				m_treeBase->DeleteChildren(Expand_Id);
				ExchangeItemData* pData=(ExchangeItemData*)m_treeBase->GetItemData(Expand_Id);
				std::map<std::string,std::set<std::string>> ProductIdMap;
				std::map<std::string,std::set<std::string>>::iterator ProductIdMap_iter;
				if(pData)
				{
					std::string strExch;
					pData->GetData(strExch);
					DEFAULT_SVR()->GetProductID_InstrumentIDsByExchangeID(ProductIdMap,strExch);
					for(ProductIdMap_iter=ProductIdMap.begin();ProductIdMap_iter!=ProductIdMap.end();++ProductIdMap_iter)
					{
						wxTreeItemId exch_Id=m_treeBase->AppendItem(Expand_Id,ProductIdMap_iter->first.c_str(),
							-1,-1,new ProductItemData(ProductIdMap_iter->second));
						m_treeBase->AppendItem(exch_Id,DUMMY_TEXT);
					}
				}
			}
		}
		if(3==IsTypeNode(Expand_Id))//产品名
		{
			wxTreeItemIdValue cookie;
			wxTreeItemId dummyID=m_treeBase->GetFirstChild(Expand_Id,cookie);
			if(m_treeBase->GetItemText(dummyID)==DUMMY_TEXT)
			{
				m_treeBase->DeleteChildren(Expand_Id);
				ProductItemData* pData=(ProductItemData*)m_treeBase->GetItemData(Expand_Id);
				std::set<std::string> datas;
				std::set<std::string>::iterator it;
				if(pData)
				{
					pData->GetData(datas);
					for(it=datas.begin();it!=datas.end();++it)
					{
						m_treeBase->AppendItem(Expand_Id,DisplayInstrument(strTypeName,*it),-1,-1,new ContractItemData(*it));
					}
				}
			}
		}
	}
	event.Skip();
}
string SelfGroupFunc::GetActivedTreeItem(wxTreeEvent& event)
{
	string strInstrument;
	if(m_treeBase)
	{
		wxTreeItemId Expand_Item=event.GetItem();
		if(3==IsTypeNode(m_treeBase->GetItemParent(Expand_Item)))
		{
			ContractItemData* pData=(ContractItemData*)m_treeBase->GetItemData(Expand_Item);
			if(pData)
			{
				pData->GetData(strInstrument);
			}
		}
	}
	return strInstrument;
}