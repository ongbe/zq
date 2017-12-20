// DBFRecordset.h : interface of the CDBFRecordset class
//
/////////////////////////////////////////////////////////////////////////////
/*
Авто?              Долгачев ??
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_DBFRECORDSET_H__INCLUDED_)
#define ALX_DBFRECORDSET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBFTableDef.h"
#include "ALXParser.h"

// режи?блокировки
enum LockModeEnum
{
	LockPessimistic	= 1,
	LockOptimistic	= 2
};
// ти?(алгоритм) блокировки
enum LockTypeEnum2
{
	NoLock		= 0,
	LockDBase	= 1,
	LockClipper	= 2,
	LockFoxpro	= 3
};
// режи?редактирован?
enum EditModeEnum2
{
	noMode,
	edit,
	addnew
};

// Функция конвертирует текс?OEM -> ANSI ил?ANSI -> OEM
void FAR alxOemToAnsiConvert(CString& strConvertText, TextConvertDirection enDirection);
// Функция конвертирует текс?ANSI -> OEM ил?OEM -> ANSI
void FAR alxAnsiToOemConvert(CString& strConvertText, TextConvertDirection enDirection);

// Функция - фильтр
BOOL FAR alxFilter(CDBFRecordset* pSet);

// заполнение массив?значен?ми поле?
void FAR alxFetchEmptyParserVariable(CDBFFields& DBFFields, CALXParser* pParser);
void FAR alxFetchParserVariable(CDBFRecordset* pSet, CALXParser* pParser);

////////////////////////////////////////////////////////////////////////
// CDBFRecordset - the result of a query or base table browse

class ALX_EXT_CLASS CDBFRecordset : public CObject
{
	DECLARE_DYNAMIC(CDBFRecordset)

friend BOOL FAR alxFilter(CDBFRecordset* pSet);

// Constructor
public:
	CDBFRecordset(CDBFTableDef* pDBFTable = NULL);

	void Open(LPCTSTR lpszFileName, UINT nOpenFlag = ALX_DBF_USE_SHARED);
	void Close();

// Attributes
public:

	BOOL IsOpen() const;	// открыт ли набо?записе?
	BOOL IsBOF() const;		// если те? являет? запись пере?первой
	BOOL IsEOF() const;		// если те? являет? запись посл?последне?
	BOOL IsDeleted() const;	// если была помечена на удаление

	CString GetName();			// возвращает имя файл?
	CString GetDBFilePath();	// возвращает полный путь ?DBF таблиц?

	CTime GetDateCreated();		// возвращает дату создан? таблиц?
	CTime GetDateLastUpdated();	// возвращает дату последне?модификаци?

	long GetRecordCount();		// ко?во записе??таблиц?

// Operations
public:
	// create/modify table operations
	BOOL CreateTable(LPCTSTR lpszFileName);	// создае?таблиц?
	void ModifyTable();						// модифицируем
	void UpdateTable();						// сохраняем изменения
	void CancelUpdateTable();				// отка?

	// create/modify field operations
	void AddField(FIELD_REC* pFieldRec);					// добавляем поле
	void InsertField(short nIndex, FIELD_REC* pFieldRec);	// вставляем поле
	void MoveField(short nIndex, short nNewIndex);			// перемещаем поле на позици?..
	void ModifyField(short nIndex, FIELD_REC* pFieldRec);	// изме?ем поле
	void DeleteField(short nIndex);							// удаляем поле

	// recordset operations
	void MoveNext();			// перехо?на следующу?запись
	void MovePrev();			// перехо?на преддыдущу?запись
	void MoveFirst();			// перехо?на первую запись
	void MoveLast();			// перехо?на последню?запись
	void Move(long lRecNumber);	// перехо?на запись по смещению

	// Поис?
	BOOL FindFirst(LPCTSTR lpszFilter);	// поис?первой записи
	BOOL FindNext(LPCTSTR lpszFilter);	// поис?следующе?записи
	BOOL FindNext();					// поис?следующе?записи
	BOOL FindLast(LPCTSTR lpszFilter);	// поис?последне?записи
	BOOL FindPrev(LPCTSTR lpszFilter);	// поис?предыдущей записи
	BOOL FindPrev();					// поис?предыдущей записи

	long GetAbsolutePosition();					// получаем абсолютную позици?те? записи
	void SetAbsolutePosition(long lOffsetRec);	// делаем те? запись?запись аб? позиция которо?задает? па? функци?

	// edit buffer operations
	virtual void AddNew();		// добавляем нову?запись
	virtual void Edit();		// изме?ем текущу?запись
	virtual void Update();		// сохраняем изменения
	virtual void CancelUpdate();// отка?
	virtual void Delete();		// помечаем запись на удаление
	virtual void Undelete();	// снимае?пометк?

	// locking
	BOOL LockRecord();		// блокируе?текущу?запись
	BOOL UnlockRecord();	// разблокируем текущу?запись

	// field operations
	virtual COleVariant GetFieldValue(LPCTSTR lpszName);
	virtual COleVariant GetFieldValue(int nIndex);

	virtual long GetFieldValue(LPCTSTR lpszName, COleVariant& varValue);
	virtual long GetFieldValue(int nIndex, COleVariant& varValue);

	virtual void SetFieldValue(LPCTSTR lpszName, const COleVariant& varValue);
	virtual void SetFieldValue(int nIndex, const COleVariant& varValue);

	// set text convert function
	void SetTextConvertFunction(void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection));

	short GetFieldCount();	// ко?во поле?

// Overridables
public:
	virtual CString GetDefaultDBName();

	// for recordset field exchange
//	virtual void DoFieldExchange(CDBFFieldExchange* pFX);

// Implementation
public:
	BOOL ChangeOpenFlag(UINT nNewFlag);
	virtual ~CDBFRecordset();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CDBFTableDef*	m_pTableDef;	// указател?на таблиц?
	CDBFFields		m_DBFFields;	// структур?поле?


protected:
	int m_nOpenType;		// способ открыт? таблиц?

	BOOL m_bBOF;			
	BOOL m_bEOF;

	long m_nCurTableRecN;	// индекс текуще?записи ?таблиц?

	UINT m_nLockMode;		// режи?блокировки
	UINT m_nLockType;		// ти?(алгоритм) блокировки

	UINT m_nEditMode;		// режи?редактирован?

	// указател?на функци?конвертаци?текста
	void (FAR * m_lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection);

	CALXParser* m_pFindParser;		// парсер выражени?поиска
	CALXParser* m_pFilterParser;	// парсер выражения фильтр?
	
	virtual void ThrowDBFException(long nError = NO_ALX_DBF_ERROR, LPCTSTR szDescription = NULL);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_DBFRECORDSET_H__INCLUDED_)
