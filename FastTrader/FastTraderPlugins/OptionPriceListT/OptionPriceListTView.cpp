// OptionPriceListTView.cpp : COptionPriceListTView 类的实现
//

#include "stdafx.h"
#include "OptionPriceListT.h"

#include "OptionPriceListTDoc.h"
#include "OptionPriceListTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COptionPriceListTView

IMPLEMENT_DYNCREATE(COptionPriceListTView, CView)

BEGIN_MESSAGE_MAP(COptionPriceListTView, CView)
END_MESSAGE_MAP()

// COptionPriceListTView 构造/析构

COptionPriceListTView::COptionPriceListTView()
{
	// TODO: 在此处添加构造代码

}

COptionPriceListTView::~COptionPriceListTView()
{
}

BOOL COptionPriceListTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COptionPriceListTView 绘制

void COptionPriceListTView::OnDraw(CDC* /*pDC*/)
{
	COptionPriceListTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// COptionPriceListTView 诊断

#ifdef _DEBUG
void COptionPriceListTView::AssertValid() const
{
	CView::AssertValid();
}

void COptionPriceListTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COptionPriceListTDoc* COptionPriceListTView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COptionPriceListTDoc)));
	return (COptionPriceListTDoc*)m_pDocument;
}
#endif //_DEBUG


// COptionPriceListTView 消息处理程序
