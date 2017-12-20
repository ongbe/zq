// TestFastTraderPipeDoc.h : interface of the CTestFastTraderPipeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTFASTTRADERPIPEDOC_H__966CE119_23B1_4D9E_89B7_F6238D43F618__INCLUDED_)
#define AFX_TESTFASTTRADERPIPEDOC_H__966CE119_23B1_4D9E_89B7_F6238D43F618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestFastTraderPipeDoc : public CDocument
{
protected: // create from serialization only
	CTestFastTraderPipeDoc();
	DECLARE_DYNCREATE(CTestFastTraderPipeDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestFastTraderPipeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestFastTraderPipeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestFastTraderPipeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTFASTTRADERPIPEDOC_H__966CE119_23B1_4D9E_89B7_F6238D43F618__INCLUDED_)
