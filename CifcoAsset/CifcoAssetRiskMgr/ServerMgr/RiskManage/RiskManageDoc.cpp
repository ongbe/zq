// RiskManageDoc.cpp : CRiskManageDoc 类的实现
//

#include "stdafx.h"
#include "RiskManage.h"

#include "RiskManageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRiskManageDoc

IMPLEMENT_DYNCREATE(CRiskManageDoc, CDocument)

BEGIN_MESSAGE_MAP(CRiskManageDoc, CDocument)
END_MESSAGE_MAP()


// CRiskManageDoc 构造/析构

CRiskManageDoc::CRiskManageDoc()
{
	// TODO: 在此添加一次性构造代码

}

CRiskManageDoc::~CRiskManageDoc()
{
}

BOOL CRiskManageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CRiskManageDoc 序列化

void CRiskManageDoc::Serialize(CArchive& ar)
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


// CRiskManageDoc 诊断

#ifdef _DEBUG
void CRiskManageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRiskManageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRiskManageDoc 命令
