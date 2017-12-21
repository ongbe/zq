#include "StdAfx.h"
#include "SortListCtrl.h"

#include <cstdlib>
#include <cmath>

CSortListCtrl::CSortListCtrl(void)
: m_bAsc(false)
, m_nSortedCol(0)
{

}

CSortListCtrl::~CSortListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CSortListCtrl::OnLvnColumnclick)
END_MESSAGE_MAP()

int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortListCtrl* pList = (CSortListCtrl*)lParamSort;
	int nItem1,nItem2;

	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = lParam1;
	nItem1 = pList->FindItem(&FindInfo, -1);
	FindInfo.lParam = lParam2;
	nItem2 = pList->FindItem(&FindInfo, -1);

	if((nItem1 == -1) || (nItem2 == -1)) 
	{ 
		return 0; 
	}

	CString str1, str2;
	str1 = pList->GetItemText(nItem1, pList->m_nSortedCol);
	str2 = pList->GetItemText(nItem2, pList->m_nSortedCol);

	HDITEM headerItem;
	headerItem.mask = HDI_LPARAM;
	CHeaderCtrl* pHeaderCtrl = pList->GetHeaderCtrl( );
	pHeaderCtrl->GetItem(pList->m_nSortedCol, &headerItem);
	UINT nType = (UINT)(headerItem.lParam);

	int iCompRes = 0;
	switch(nType)
	{
	case CSortListCtrl::DATA_TYPE::INT_TYPE:
		{
			int i1,i2;
			std::string wstr;
			std::string str;
			wstr = str1.GetBuffer(str1.GetLength());
			str = wstr;
			i1 = atoi(str.c_str());
			wstr = str2.GetBuffer(str2.GetLength());
			str = wstr;
			i2 = atoi(str.c_str());

			if( i1 > i2) 
				iCompRes = 1; 
			else if( i1 == i2 ) 
				iCompRes = 0; 
			else 
				iCompRes = -1;
		}
		break;
	case CSortListCtrl::DATA_TYPE::DOUBLE_TYPE:
		{
			double i1,i2;
			std::string wstr;
			std::string str;
			wstr = str1.GetBuffer(str1.GetLength());
			str = wstr;
			i1 = atof(str.c_str());
			wstr = str2.GetBuffer(str2.GetLength());
			str = wstr;
			i2 = atof(str.c_str());

			if((i1 - i2) > 0.001) 
				iCompRes = 1; 
			else if((i2 - i1) > 0.001) 
				iCompRes = -1; 
			else
				iCompRes = 0;
		}
		break;
	case CSortListCtrl::DATA_TYPE::STRING_TYPE:
	default:
		{
			if(str1.MakeLower() > str2.MakeLower()) 
				iCompRes = 1; 
			else if(str1.MakeLower() < str2.MakeLower()) 
				iCompRes = -1; 
			else if(str1.MakeLower() == str2.MakeLower())
				iCompRes = 0;
			else
				iCompRes = 0;
		}
		break;
	}

	if(pList->m_bAsc)
		return iCompRes;
	else
		return iCompRes * -1;

	return 0;
}

void CSortListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	if(pNMLV->iSubItem == m_nSortedCol)
	{
		m_bAsc = !m_bAsc;
	}
	else
	{
		m_bAsc = TRUE;
		m_nSortedCol = pNMLV->iSubItem;
	}

	//非常重要，不能遗漏
	//********************************************************
	int num = this->GetItemCount();  
    while(num--)
		this->SetItemData(num, num);  
	//********************************************************

	SortItems(ListCompare, (DWORD)this); 

	UpdateData(FALSE);

	*pResult = 0;
}
