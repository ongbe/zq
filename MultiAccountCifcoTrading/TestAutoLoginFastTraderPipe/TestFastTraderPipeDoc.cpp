// TestFastTraderPipeDoc.cpp : implementation of the CTestFastTraderPipeDoc class
//

#include "stdafx.h"
#include "TestFastTraderPipe.h"

#include "TestFastTraderPipeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeDoc

IMPLEMENT_DYNCREATE(CTestFastTraderPipeDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestFastTraderPipeDoc, CDocument)
	//{{AFX_MSG_MAP(CTestFastTraderPipeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeDoc construction/destruction

CTestFastTraderPipeDoc::CTestFastTraderPipeDoc()
{
	// TODO: add one-time construction code here

}

CTestFastTraderPipeDoc::~CTestFastTraderPipeDoc()
{
}

BOOL CTestFastTraderPipeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeDoc serialization

void CTestFastTraderPipeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeDoc diagnostics

#ifdef _DEBUG
void CTestFastTraderPipeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestFastTraderPipeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeDoc commands
