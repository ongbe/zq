// MAManageClientDoc.cpp : CMAManageClientDoc 类的实现
//

#include "stdafx.h"
#include "MAManageClient.h"

#include "MAManageClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMAManageClientDoc

IMPLEMENT_DYNCREATE(CMAManageClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CMAManageClientDoc, CDocument)
END_MESSAGE_MAP()


// CMAManageClientDoc 构造/析构

CMAManageClientDoc::CMAManageClientDoc()
{
	// TODO: 在此添加一次性构造代码

}

CMAManageClientDoc::~CMAManageClientDoc()
{
}

BOOL CMAManageClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMAManageClientDoc 序列化

void CMAManageClientDoc::Serialize(CArchive& ar)
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


// CMAManageClientDoc 诊断

#ifdef _DEBUG
void CMAManageClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMAManageClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMAManageClientDoc 命令
