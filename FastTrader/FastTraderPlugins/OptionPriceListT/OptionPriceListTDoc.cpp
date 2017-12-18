// OptionPriceListTDoc.cpp : COptionPriceListTDoc 类的实现
//

#include "stdafx.h"
#include "OptionPriceListT.h"

#include "OptionPriceListTDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COptionPriceListTDoc

IMPLEMENT_DYNCREATE(COptionPriceListTDoc, CDocument)

BEGIN_MESSAGE_MAP(COptionPriceListTDoc, CDocument)
END_MESSAGE_MAP()


// COptionPriceListTDoc 构造/析构

COptionPriceListTDoc::COptionPriceListTDoc()
{
	// TODO: 在此添加一次性构造代码

}

COptionPriceListTDoc::~COptionPriceListTDoc()
{
}

BOOL COptionPriceListTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// COptionPriceListTDoc 序列化

void COptionPriceListTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// COptionPriceListTDoc 诊断

#ifdef _DEBUG
void COptionPriceListTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COptionPriceListTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COptionPriceListTDoc 命令
