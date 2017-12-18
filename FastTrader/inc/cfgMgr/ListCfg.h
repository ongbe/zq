
#pragma once

#include <string>
#include <vector>

#include "CfgGlobal.h"

class CListCfg
{
public:
	CListCfg(void);
	virtual ~CListCfg(void);

	LPLIST_TABLE_CFG GetListCfg(){return &m_ListCfg;}
	std::vector<LPLIST_COLUMN_CFG>* GetListColCfg(){return &m_ColVector;}
	int	GetListColCount();
	LPLIST_COLUMN_CFG GetListColCfg(int index);
	int	GetListVisibleColCount(){return m_VisibleColVector.size();}
	LPLIST_COLUMN_CFG GetListVisibleColCfg(int index);;

	void SetListId(int value){ m_ListCfg.Id = value;}
	void SetListFont( const char* font ){ strncpy(m_ListCfg.szFont, font, sizeof(m_ListCfg.szFont)-1);}
	void SetListFont( const std::string& font ){ strncpy(m_ListCfg.szFont, font.c_str(), sizeof(m_ListCfg.szFont)-1);}
	void SetListTextSize(int value){ m_ListCfg.TextSize = value;}
	void SetListTextColor(unsigned long value){ m_ListCfg.TextColor = value;}
	void SetListHeadColor(unsigned long value){ m_ListCfg.HeadColor = value;}
	void SetListHeadBackgroundColor(unsigned long value){ m_ListCfg.HeadBackgroundColor = value;}
	void SetListBackgroundColor(unsigned long value){ m_ListCfg.BackgroundColor = value;}
	void SetListEvenLineBackgroundColor(unsigned long value){ m_ListCfg.EvenLineBackgroundColor = value;}
	void SetListOddLineBackgroundColor(unsigned long value){ m_ListCfg.OddLineBackgroundColor = value;}
	void SetListHeadVisibility(int value){ m_ListCfg.HeadVisibility = value;}
	void SetListVisibility(int value){ m_ListCfg.Visibility = value;}

	void SetListCfg(int TextSize, unsigned long TextColor, unsigned long HeadColor, 
		unsigned long HeadBackgroundColor,unsigned long BackgroundColor, 
		unsigned long EvenLineBackgroundColor, unsigned long OddLineBackgroundColor,
		int HeadVisibility,int Visibility);

	void AddColCfgInfo(LPLIST_COLUMN_CFG pColCfg);

	void AddColCfgInfo( int id, int Width, 
		unsigned long TextColor, int Visibility);

	void ClearColumnCfg();

	LPLIST_COLUMN_CFG GetALLListColCfg(int index);
	int GetALLListColCount();

	//比较所有列的数目，列id是否一致，一致返回true，不一致返回false
	bool CompareValidColList(CListCfg* pListCfg );

private:
	std::set<int> GetValidListColId(); 


private:
	LIST_TABLE_CFG  m_ListCfg;
	std::vector<LPLIST_COLUMN_CFG> m_vAllCol;
	std::vector<LPLIST_COLUMN_CFG> m_ColVector;     //该平台有效的字段
	std::vector<LPLIST_COLUMN_CFG> m_VisibleColVector;     //按顺序排列
};