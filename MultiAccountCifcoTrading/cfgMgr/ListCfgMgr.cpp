#include "StdAfx.h"

#include "Module-Misc/globalDefines.h"
#include "CfgMgr.h"
#include "ListCfg.h"
#include "ListCfgMgr.h"

static int ColorMap[]={
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,1,2,3,4,5,6,7,    8,9,0,0,0,0,0,0,

	0,10,11,12,13,14,15,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,10,11,12,13,14,15,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,

	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0
};

long ColorStr2Long(const char* pColTextColor)
{
	if ( NULL == pColTextColor )
	{
		return -1;
	}

	long ColTextColor=0;
	if(pColTextColor[0]=='#')
	{
		ColTextColor = ( ColorMap[(pColTextColor[5])]<<20)+(ColorMap[(pColTextColor[6])]<<16)
			+(ColorMap[(pColTextColor[3])]<<12)+(ColorMap[(pColTextColor[4])]<<8)+
			(ColorMap[(pColTextColor[1])]<<4)+ColorMap[(pColTextColor[2])];
	}
	else
		ColTextColor=atol(pColTextColor);

	return ColTextColor;
}

ListCfgMgr::ListCfgMgr(void)
{
	m_ListCfgMap.clear();
}

ListCfgMgr::~ListCfgMgr(void)
{
	ClearCfg();
}

bool ListCfgMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{
    if(root==NULL)
        return false;

	TiXmlElement *tListTableItem = root->FirstChildElement("ListTable");
	if(tListTableItem == NULL)
		return false;

	int tGroupIndex = 0;

	for(TiXmlElement *ListItem = tListTableItem->FirstChildElement("List"); 
		ListItem; 
		ListItem = ListItem->NextSiblingElement("List"))
	{
		//目前有两种列表配置，1报价表，2其它表
		const char *pId = ListItem->Attribute("id");
		if(!pId) continue;

		const char *pFont = ListItem->Attribute("Font");
		const char *pTextSize = ListItem->Attribute("TextSize");
		const char *pTextColor = ListItem->Attribute("TextColor");
		const char *pHeadColor = ListItem->Attribute("HeadColor");
		const char *pHeadBgColor = ListItem->Attribute("HeadBgColor");
		const char *pBgColor = ListItem->Attribute("BgColor");
		const char *pEvenLineBgColor = ListItem->Attribute("EvenLineBgColor");
		const char *pOddLineBgColor = ListItem->Attribute("OddLineBgColor");
		const char *pHeadVisibility = ListItem->Attribute("HeadVisibility");
		const char *pVisibility = ListItem->Attribute("Visibility");

		CListCfg *pListCfg = new CListCfg;
		int tListId = atoi(pId);
		pListCfg->SetListId(tListId);
		if ( pFont )
		{
			pListCfg->SetListFont(pFont);
		}
		else
		{
			pListCfg->SetListFont(LOADSTRING(DEFAULT_FONT));
		}
		pListCfg->SetListTextSize(pTextSize?atoi(pTextSize):80);
		pListCfg->SetListTextColor(pTextColor?ColorStr2Long(pTextColor):33554432);
		pListCfg->SetListHeadColor(pHeadColor?ColorStr2Long(pHeadColor):33554432);
		pListCfg->SetListHeadBackgroundColor(pHeadBgColor?ColorStr2Long(pHeadBgColor):46715092);
		pListCfg->SetListBackgroundColor(pBgColor?ColorStr2Long(pBgColor):50331647);
		pListCfg->SetListEvenLineBackgroundColor(pEvenLineBgColor?ColorStr2Long(pEvenLineBgColor):50331647);
		pListCfg->SetListOddLineBackgroundColor(pOddLineBgColor?ColorStr2Long(pOddLineBgColor):50331647);
		pListCfg->SetListHeadVisibility(pHeadVisibility?atoi(pHeadVisibility):1);
		pListCfg->SetListVisibility(pVisibility?atoi(pVisibility):1);

		for(TiXmlElement *ColumnItem = ListItem->FirstChildElement("column"); 
			ColumnItem; 
			ColumnItem = ColumnItem->NextSiblingElement("column"))
		{
			const char *pColId = ColumnItem->Attribute("id");
			if(!pColId)continue;
			const char *pWidth = ColumnItem->Attribute("Width");
			if(!pWidth) pWidth="100";
			const char *pColTextColor = ColumnItem->Attribute("TextColor");
			if(!pColTextColor) pColTextColor="33554432";
			const char *pColVisibility = ColumnItem->Attribute("Visibility");
			if(!pColVisibility) pColVisibility="1";

			pListCfg->AddColCfgInfo(atoi(pColId), atoi(pWidth),
				ColorStr2Long(pColTextColor),atoi(pColVisibility));
		}

		std::map<int, CListCfg*>::iterator it = m_ListCfgMap.find( tListId );
		if ( it != m_ListCfgMap.end() )
		{
			if ( !bFromUserCfg )
			{
				delete it->second;
				it->second = NULL;
				m_ListCfgMap[tListId] = pListCfg;
			}
			else
			{
				//比较有效列数目，列id是否一致，一致时，使用用户配置，否则，使用系统配置
				if( !pListCfg->CompareValidColList(m_ListCfgMap[tListId]))
				{
					delete pListCfg;
					pListCfg = NULL;
				}
				else
				{
					delete it->second;
					it->second = NULL;
					m_ListCfgMap[tListId] = pListCfg;
				}
			}
		}
		else
		{
			if ( !bFromUserCfg )
			{
				m_ListCfgMap.insert(std::make_pair(tListId, pListCfg));
			}
			else
			{
				delete pListCfg;
				pListCfg = NULL;
			}
		}
	}

	return true;

}
bool ListCfgMgr::SaveCfg(TiXmlElement *root)
{
	TiXmlElement *pListTableItem = root->FirstChildElement("ListTable");
	if(pListTableItem)
		root->RemoveChild(pListTableItem);

	pListTableItem = new TiXmlElement("ListTable");

	int ttest = (int)m_ListCfgMap.size();
	std::map<int, CListCfg*>::iterator iter = m_ListCfgMap.begin();

	for (iter; iter != m_ListCfgMap.end(); iter++)
	{
		CListCfg *pListCfg = (CListCfg *)iter->second;
		LPLIST_TABLE_CFG pListTableCfg = pListCfg->GetListCfg();

		TiXmlElement *ListItem = new TiXmlElement("List");
		ListItem->SetAttribute("id", pListTableCfg->Id);
		ListItem->SetAttribute("Font", pListTableCfg->szFont);
		ListItem->SetAttribute("TextSize", pListTableCfg->TextSize);
		ListItem->SetAttribute("TextColor", pListTableCfg->TextColor);
		ListItem->SetAttribute("HeadColor", pListTableCfg->HeadColor);
		ListItem->SetAttribute("HeadBgColor", pListTableCfg->HeadBackgroundColor);
		ListItem->SetAttribute("BgColor", pListTableCfg->BackgroundColor);
		ListItem->SetAttribute("EvenLineBgColor", pListTableCfg->EvenLineBackgroundColor);
		ListItem->SetAttribute("OddLineBgColor", pListTableCfg->OddLineBackgroundColor);
		ListItem->SetAttribute("HeadVisibility", pListTableCfg->HeadVisibility);
		ListItem->SetAttribute("Visibility", pListTableCfg->Visibility);

		pListTableItem->LinkEndChild(ListItem);
		int tListColCount = pListCfg->GetListColCount();

		for(int i = 0; i < tListColCount; i++)
		{
			TiXmlElement *ColumnItem = new TiXmlElement("column");
			LPLIST_COLUMN_CFG pListColCfg = pListCfg->GetListColCfg(i);

			ColumnItem->SetAttribute("id", pListColCfg->id);
			ColumnItem->SetAttribute("Width", pListColCfg->Width);
			ColumnItem->SetAttribute("TextColor", pListColCfg->TextColor);
			ColumnItem->SetAttribute("Visibility", pListColCfg->Visibility);

			ListItem->LinkEndChild(ColumnItem);
		}

	}

	if ( NULL != pListTableItem->FirstAttribute() || NULL != pListTableItem->FirstChild())
	{
		root->LinkEndChild(pListTableItem);
	}
	else
	{
		delete pListTableItem;
		pListTableItem = NULL;
	}

	return true;
}

LPLIST_TABLE_CFG ListCfgMgr::GetListCfg(int ModuleId)
{
	std::map<int, CListCfg*>::iterator iter =  m_ListCfgMap.find(ModuleId);
	if(iter != m_ListCfgMap.end())
	{
		return ((CListCfg *)(iter->second))->GetListCfg();
	}

	return NULL;
}

std::vector<LPLIST_COLUMN_CFG> *ListCfgMgr::GetListColCfg(int ModuleId)
{
	std::map<int, CListCfg*>::iterator iter =  m_ListCfgMap.find(ModuleId);
	if(iter != m_ListCfgMap.end())
	{
		return ((CListCfg *)(iter->second))->GetListColCfg();
	}

	return NULL;
}

int	ListCfgMgr::GetListColCount(int ModuleId)
{
	std::map<int, CListCfg*>::iterator iter =  m_ListCfgMap.find(ModuleId);
	if(iter != m_ListCfgMap.end())
	{
		return ((CListCfg *)(iter->second))->GetListColCount();
	}

	return 0;
}

LPLIST_COLUMN_CFG ListCfgMgr::GetListColCfg(int ModuleId, int index)
{
	std::map<int, CListCfg*>::iterator iter =  m_ListCfgMap.find(ModuleId);
	if(iter != m_ListCfgMap.end())
	{
		return ((CListCfg *)(iter->second))->GetListColCfg(index);
	}

	return NULL;
}

int	ListCfgMgr::GetListVisibleColCount(int ModuleId)
{
	std::map<int, CListCfg*>::iterator iter =  m_ListCfgMap.find(ModuleId);
	if(iter != m_ListCfgMap.end())
	{
		return ((CListCfg *)(iter->second))->GetListVisibleColCount();
	}

	return 0;
}

LPLIST_COLUMN_CFG ListCfgMgr::GetListVisibleColCfg(int ModuleId, int index)
{
	std::map<int, CListCfg*>::iterator iter =  m_ListCfgMap.find(ModuleId);
	if(iter != m_ListCfgMap.end())
	{
		return ((CListCfg *)(iter->second))->GetListVisibleColCfg(index);
	}

	return NULL;
}

void ListCfgMgr::ClearCfg()
{
	std::map<int, CListCfg*> ::iterator iter = m_ListCfgMap.begin();
	for(iter; iter != m_ListCfgMap.end(); iter++)
	{
		CListCfg* p = iter->second;
		SAFE_DELETE(p)
	}

	m_ListCfgMap.clear();
}

void ListCfgMgr::SetListColCfg(int ModuleId, LPLIST_TABLE_CFG pList,
			std::map<std::string ,LPLIST_COLUMN_CFG> NewColCfgMap,int ColCount)
{
	std::map<int, CListCfg*>::iterator iter =  m_ListCfgMap.find(ModuleId);
	if(iter != m_ListCfgMap.end())
	{
		m_ListCfgMap.erase(iter);	
	}

	CListCfg *pListCfg =  new CListCfg;
	pListCfg->SetListId(pList->Id);
	pListCfg->SetListFont( pList->szFont );
	pListCfg->SetListTextSize(pList->TextSize);
	pListCfg->SetListTextColor(pList->TextColor);
	pListCfg->SetListHeadColor(pList->HeadColor);
	pListCfg->SetListHeadBackgroundColor(pList->HeadBackgroundColor);
	pListCfg->SetListBackgroundColor(pList->BackgroundColor);
	pListCfg->SetListEvenLineBackgroundColor(pList->EvenLineBackgroundColor);
	pListCfg->SetListOddLineBackgroundColor(pList->OddLineBackgroundColor);
	pListCfg->SetListHeadVisibility(pList->HeadVisibility);
	pListCfg->SetListVisibility(pList->Visibility);
	for(std::map<std::string ,LPLIST_COLUMN_CFG>::iterator it=NewColCfgMap.begin ();it!=NewColCfgMap.end();it++)
	{
		pListCfg->AddColCfgInfo(it->second->id, it->second->Width,
			it->second->TextColor, it->second->Visibility);
	}

	m_ListCfgMap.insert(std::make_pair(ModuleId, pListCfg));
}

void ListCfgMgr::SetListColCfg(int ModuleId, LPLIST_TABLE_CFG pList,std::vector<LPLIST_COLUMN_CFG> NewColCfgVec)
{
	std::map<int, CListCfg*>::iterator iter =  m_ListCfgMap.find(ModuleId);
	if(iter != m_ListCfgMap.end())
	{
		m_ListCfgMap.erase(iter);	
	}

	CListCfg *pListCfg =  new CListCfg;
	pListCfg->SetListId(pList->Id);
	pListCfg->SetListFont( pList->szFont );
	pListCfg->SetListTextSize(pList->TextSize);
	pListCfg->SetListTextColor(pList->TextColor);
	pListCfg->SetListHeadColor(pList->HeadColor);
	pListCfg->SetListHeadBackgroundColor(pList->HeadBackgroundColor);
	pListCfg->SetListBackgroundColor(pList->BackgroundColor);
	pListCfg->SetListEvenLineBackgroundColor(pList->EvenLineBackgroundColor);
	pListCfg->SetListOddLineBackgroundColor(pList->OddLineBackgroundColor);
	pListCfg->SetListHeadVisibility(pList->HeadVisibility);
	pListCfg->SetListVisibility(pList->Visibility);

	for(std::vector<LPLIST_COLUMN_CFG>::iterator it=NewColCfgVec.begin ();it!=NewColCfgVec.end();it++)
	{
		pListCfg->AddColCfgInfo((*it)->id, (*it)->Width,
			(*it)->TextColor, (*it)->Visibility);
	}

	m_ListCfgMap.insert(std::make_pair(ModuleId, pListCfg));
}

std::map<int, CListCfg*>*  ListCfgMgr::GetListCfgMap()
{
	return &m_ListCfgMap;
}
