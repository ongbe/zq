/*----------------------------------------------------------------------
Copyright (C)2001 MJSoft. All Rights Reserved.
          This source may be used freely as long as it is not sold for
					profit and this copyright information is not altered or removed.
					Visit the web-site at www.mjsoft.co.uk
					e-mail comments to info@mjsoft.co.uk
File:     SortListCtrl.cpp
Purpose:  Provides a sortable list control, it will sort text, numbers
          and dates, ascending or descending, and will even draw the
					arrows just like windows explorer!
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "SortListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSortListCtrl::CSortListCtrl()
: m_iSortColumn( -1 )
, m_bSortAscending( TRUE )
{
}

CSortListCtrl::~CSortListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
	//ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CSortListCtrl::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;

	// if it's a second click on the same column then reverse the sort order,
	// otherwise sort the new column in ascending order.
	//Sort( iColumn, iColumn == m_iSortColumn ? !m_bSortAscending : TRUE );

	*pResult = 0;
}


void CSortListCtrl::Sort( int iColumn )
{
	m_bSortAscending = (iColumn == m_iSortColumn ? !m_bSortAscending : TRUE);
	m_iSortColumn = iColumn;

	// show the appropriate arrow in the header control.
	CHeaderCtrl* pHdrCtrl = GetHeaderCtrl(); 

	HD_ITEM curItem; 

	//erase sortmark image from all of columns 
	for( int i = 0; i < pHdrCtrl->GetItemCount(); i++ ) 
	{ 
		curItem.mask = HDI_IMAGE|HDI_FORMAT; 
		pHdrCtrl->GetItem(i, &curItem); 
		curItem.fmt &= ~HDF_IMAGE; 
		pHdrCtrl->SetItem(i, &curItem); 
	} 

	//draw sortmark image on the colmun just selected by user 
	if( iColumn != -1 ) 
	{
		curItem.iImage = !m_bSortAscending; 
		curItem.fmt |= HDF_IMAGE|HDF_BITMAP_ON_RIGHT; 
		pHdrCtrl->SetItem(iColumn, &curItem); 
	} 

	//m_ctlHeader.SetSortArrow( m_iSortColumn, m_bSortAscending );

	//VERIFY( SortItems( CompareFunction, reinterpret_cast<DWORD>( this ) ) );
}

void CSortListCtrl::GetSortParam( int& nColumn, BOOL& bAscending )
{
	nColumn = m_iSortColumn;
	bAscending = m_bSortAscending;
}
