// DBFTableDef.cpp : implementation of the CDBFTableDef class
//
/////////////////////////////////////////////////////////////////////////////
/*
Авто?              Долгачев ??
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBFRecordset.h"
#include "DBFException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* Функция конвертирует текс?OEM -> ANSI ил?ANSI -> OEM */
void FAR alxOemToAnsiConvert(CString& strConvertText, TextConvertDirection enDirection)
{
	if(enDirection == GetText)
		strConvertText.OemToAnsi();
	else
		strConvertText.AnsiToOem();
}

/* Функция конвертирует текс?ANSI -> OEM ил?OEM -> ANSI */
void FAR alxAnsiToOemConvert(CString& strConvertText, TextConvertDirection enDirection)
{
	if(enDirection == GetText)
		strConvertText.AnsiToOem();
	else
		strConvertText.OemToAnsi();
}

/* Функция - фильтр. */
BOOL FAR alxFilter(CDBFRecordset* pSet)
{
	// получаем значен? поле?используемых ?выражени?
	alxFetchParserVariable(pSet, pSet->m_pFilterParser);

	// вычисляем результа?
	VARIANT* pvarExprResult = pSet->m_pFilterParser->Execute();

	// если не логическое выражени?
	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return V_BOOL(pvarExprResult) != FALSE;
} 

// заполнение массив?значен?ми поле?
void FAR alxFetchEmptyParserVariable(CDBFFields& DBFFields, CALXParser* pParser)
{
	for(short i = 0; i < DBFFields.GetFieldCount(); i++)
	{
		COleVariant varFldValue;

		switch(DBFFields[i].m_chType)
		{
			case FLD_TYPE_CHARACTER:
			case FLD_TYPE_MEMO:
				varFldValue.ChangeType(VT_BSTR);
				pParser->AddVariable(DBFFields[i].m_strName, varFldValue, i);
				break;
			case FLD_TYPE_DOUBLE: // возможно FLD_TYPE_BINARY
				if(DBFFields[i].m_nLen != sizeof(DBF_DOUBLE))
					break;
			case FLD_TYPE_NUMERIC:
			case FLD_TYPE_FLOAT:
			case FLD_TYPE_CURRENCY:
			case FLD_TYPE_INTEGER:
				varFldValue.ChangeType(VT_R8);
				pParser->AddVariable(DBFFields[i].m_strName, varFldValue, i);
				break;
			case FLD_TYPE_DATE:
			case FLD_TYPE_DATETIME:
				varFldValue.ChangeType(VT_DATE);
				pParser->AddVariable(DBFFields[i].m_strName, varFldValue, i);
				break;
			case FLD_TYPE_LOGICAL:
				varFldValue.ChangeType(VT_BOOL);
				pParser->AddVariable(DBFFields[i].m_strName, varFldValue, i);
				break;
		}
	}
}

// заполнение массив?значен?ми поле?
void FAR alxFetchParserVariable(CDBFRecordset* pSet, CALXParser* pParser)
{
	LPCSTR szName = pParser->GetFirstVariableName();
	// перебираем по? используемые ?выражени?
	while(szName != NULL)
	{
		COleVariant varFldValue;
		// получаем индекс по?
		DWORD dwInd = pParser->GetVariableKey();
		// получаем значение по?
		pSet->GetFieldValue(dwInd, varFldValue);
		// передаем значен? поле?
		pParser->SetVariableValue(pSet->m_DBFFields[dwInd].m_strName, varFldValue);

		// перехади ?следующему полю
		szName = pParser->GetNextVariableName();
	}
}
//////////////////////////////////////////////////////////////////////////
// CDBFRecordset
IMPLEMENT_DYNAMIC(CDBFRecordset, CObject )

/* Конструкто?*/
CDBFRecordset::CDBFRecordset(CDBFTableDef* pDBFTable)
{
	m_pTableDef = NULL;

	m_lpfnTextConvert = NULL;

	m_pFindParser = NULL;
	m_pFilterParser = NULL;

	m_nCurTableRecN = -1;
	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	m_nLockMode = LockPessimistic;
	m_nLockType = NoLock;

	m_nEditMode = noMode;
}

/* Деструктор */
CDBFRecordset::~CDBFRecordset()
{
	if (IsOpen())
		Close();
}

/* Создае?нову?таблиц?*/
BOOL CDBFRecordset::CreateTable(LPCTSTR lpszFileName)
{
	if(IsOpen())
		Close();

	CString strFileName;

	if(lpszFileName == NULL)
		strFileName = GetDefaultDBName();
	else
		strFileName = lpszFileName;

	ASSERT(strFileName.GetLength() > 4);

	m_pTableDef = new CDBFTableDef;

	// таблиц??режиме создан?
	if(m_pTableDef->CreateTable(strFileName))
		return FALSE;

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;

	return TRUE;
}

/* Модификация таблиц?*/
void CDBFRecordset::ModifyTable()
{
	// таблиц??режиме модификаци?
	m_pTableDef->ModifyTable();

	return;
}

/* Добавляем поле ?таблиц?*/
void CDBFRecordset::AddField(FIELD_REC *pFieldRec)
{
	m_pTableDef->AddField(pFieldRec);

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Вставляем поле ?таблиц?*/
void CDBFRecordset::InsertField(short nIndex, FIELD_REC *pFieldRec)
{
	m_pTableDef->InsertField(nIndex, pFieldRec);

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Удаляем поле из таблиц?*/
void CDBFRecordset::DeleteField(short nIndex)
{
	m_pTableDef->DeleteField(nIndex);

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Изме?ем поле таблиц?*/
void CDBFRecordset::ModifyField(short nIndex, FIELD_REC *pFieldRec)
{
	m_pTableDef->ModifyField(nIndex, pFieldRec);

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Перемещаем поле таблиц?на нову?позици?*/
void CDBFRecordset::MoveField(short nIndex, short nNewIndex)
{
	m_pTableDef->MoveField(nIndex, nNewIndex);

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Отка?от изменени?сделанны??структур?таблиц?*/
void CDBFRecordset::CancelUpdateTable()
{
	m_pTableDef->CancelUpdateTable();

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;
}

/* Сохраняем изменения ?таблиц?*/
void CDBFRecordset::UpdateTable()
{
	m_pTableDef->UpdateTable();

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;

	if(m_pTableDef->GetRecordCount() > 0)
		MoveFirst();
	else
		m_nCurTableRecN = 0;
}

/* Открытие набора записе?*/
void CDBFRecordset::Open(LPCTSTR lpszFileName, UINT nOpenFlag)
{
	ASSERT_VALID(this);

	CString strFileName;

	if(lpszFileName == NULL)
		strFileName = GetDefaultDBName();
	else
		strFileName = lpszFileName;

	ASSERT(strFileName.GetLength() > 4);

	// Re-Opening is invalid.
	if (IsOpen())
	{
		ASSERT(FALSE);
		return;
	}

	m_pTableDef = new CDBFTableDef;

	// открывае?таблиц?
	m_pTableDef->Open(strFileName, nOpenFlag);

	if(!IsOpen())
		return;

	// получаем структур?поле?
	m_DBFFields = m_pTableDef->m_DBFFields;

	if(m_pTableDef->GetRecordCount() > 0)
		MoveFirst();
	else
		m_nCurTableRecN = 0;
}

/* Закрывае?набо?записе?*/
void CDBFRecordset::Close()
{
	ASSERT_VALID(this);

	if (IsOpen())
		m_pTableDef->Close();

	if(m_pFilterParser != NULL)
		delete m_pFilterParser;
	if(m_pFindParser != NULL)
		delete m_pFindParser;

	m_nEditMode = noMode;

	m_nCurTableRecN = -1;
	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	if(m_pTableDef != NULL)
	{
		delete m_pTableDef;
		m_pTableDef = NULL;
		m_DBFFields.m_pFields = NULL;
		m_DBFFields.m_pCurRec = NULL;
	}
}

CString CDBFRecordset::GetDefaultDBName()
{
	ASSERT_VALID(this);

	// Override and add UNC path to .DBF file
	return _T("");
}

/* Функция возвращаеи TRUE если индекс те? записи
   выше?за границ?таблиц?*/
BOOL CDBFRecordset::IsBOF() const
{
	ASSERT_VALID(this);

	return m_bBOF;
}

/* Функция возвращаеи TRUE если индекс те? записи
   выше?за границ?таблиц?*/
BOOL CDBFRecordset::IsEOF() const
{
	ASSERT_VALID(this);

	return m_bEOF;
}

/* Функция возвращаеи TRUE если те? помечена для удален? */
BOOL CDBFRecordset::IsDeleted() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	return (m_DBFFields.m_pCurRec[0] == REC_FLAG_DELETED);
}

/* Функция возвращает имя таблиц?*/
CString CDBFRecordset::GetName()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetName();
}

/* Функция возвращает полный путь ?DBF таблиц?*/
CString CDBFRecordset::GetDBFilePath()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetDBFilePath();
}

/* Функция возвращает дату создан? таблиц?*/
CTime CDBFRecordset::GetDateCreated()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetDateCreated();
}

/* Функция возвращает дату изменения таблиц?*/
CTime CDBFRecordset::GetDateLastUpdated()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetDateLastUpdated();
}

/* Ко?во записе??таблиц?*/
long CDBFRecordset::GetRecordCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_pTableDef->GetRecordCount();
}

/* Ко?во поле??таблиц?*/
short CDBFRecordset::GetFieldCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_DBFFields.GetFieldCount();
}

/* Функция возвращает абсолютную позици?те? записи */
long CDBFRecordset::GetAbsolutePosition()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_nCurTableRecN;
}

/* Функция делает те? запись абсолютн? позиция которо?задает? па? lPosition */
void CDBFRecordset::SetAbsolutePosition(long lPosition)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lPosition >= 0 && lPosition < GetRecordCount());

	DBF_LONG lTableRecN = lPosition; 

	// Call Move.
	m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
	m_nCurTableRecN = lTableRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Перехо??следующе?записи */
void CDBFRecordset::MoveNext()
{
	ASSERT_VALID(this);
	ASSERT(!IsEOF());

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = m_nCurTableRecN;
	
	lTableRecN++;

	if(lTableRecN < GetRecordCount())
	{
		// Call Move.
		m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
		m_nCurTableRecN = lTableRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bEOF = TRUE;
		m_bBOF = GetRecordCount() > 0; 
	}
	
}

/* Перехо??предыдущей записи */
void CDBFRecordset::MovePrev()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = m_nCurTableRecN; 

	lTableRecN--;

	if(lTableRecN >= 0)
	{
		// Call Move.
		m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
		m_nCurTableRecN = lTableRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bBOF = TRUE;
		m_bEOF = GetRecordCount() > 0; 
	}
}

/* Перехо??первой записи */
void CDBFRecordset::MoveFirst()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = 0; 

	// Call Move.
	m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
	m_nCurTableRecN = lTableRecN;
	m_bBOF = m_bEOF = FALSE;
	
}

/* Перехо??последне?записи */
void CDBFRecordset::MoveLast()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = GetRecordCount() - 1; 

	// Call Move.
	m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
	m_nCurTableRecN = lTableRecN;
	m_bBOF = m_bEOF = FALSE;
}

/* Перехо?на запись по смещению относительно текуще?записи */
void CDBFRecordset::Move(long lOffsetRec)
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lTableRecN = m_nCurTableRecN + lOffsetRec; 

	// перехо??предыдущим запи??
	if(lOffsetRec < 0)
	{
		if(lTableRecN < 0)
		{
			lTableRecN = 0;
			m_bBOF = TRUE;
		}
		else
			m_bBOF = FALSE;

		m_bEOF = FALSE; 
	}
	// перехо??следующи?запи??
	else if(lOffsetRec > 0)
	{
		if(lTableRecN >= GetRecordCount())
		{
			lTableRecN = GetRecordCount() - 1;
			m_bEOF = TRUE;
		}
		else
			m_bEOF = FALSE;

		m_bBOF = FALSE; 
	}

	// Call Move.
	m_pTableDef->ReadRecord(lTableRecN, m_DBFFields.m_pCurRec);
	m_nCurTableRecN = lTableRecN;
}

/* Поис?первой записи */
BOOL CDBFRecordset::FindFirst(LPCTSTR lpszFilter)
{
	m_nCurTableRecN = -1;
	return FindNext(lpszFilter);
}

/* Поис?следующе?записи */
BOOL CDBFRecordset::FindNext(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurTableRecN++;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurTableRecN--;

		return FindNext();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* Поис?следующе?записи */
BOOL CDBFRecordset::FindNext()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurTableRecN++;

		if(m_nCurTableRecN >= GetRecordCount())
			return FALSE;

		// перебираем записи
		for(; m_nCurTableRecN < GetRecordCount(); m_nCurTableRecN++)
		{
			m_pTableDef->ReadRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
		
			alxFetchParserVariable(this, m_pFindParser);

			// вычисляем результа?
			pvarExprResult = m_pFindParser->Execute();

			// если не логическое выражени?
			if(V_VT(pvarExprResult) != VT_BOOL)
				break;

			// если нашл?
			if(V_BOOL(pvarExprResult) != FALSE)
			{
				bResult = TRUE;
				break;
			}
		}
	
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}

	if(m_nCurTableRecN >= GetRecordCount())
	{
		m_nCurTableRecN = GetRecordCount() - 1;
		m_bEOF = TRUE;
	}
	else
		m_bEOF = FALSE;

	m_bBOF = FALSE;

	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return bResult;
}
/* Поис?последне?записи */
BOOL CDBFRecordset::FindLast(LPCTSTR lpszFilter)
{
	m_nCurTableRecN = GetRecordCount();
	return FindPrev(lpszFilter);
}

/* Поис?предыдущей записи */
BOOL CDBFRecordset::FindPrev(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurTableRecN--;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurTableRecN++;

		return FindPrev();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* Поис?предыдущей записи */
BOOL CDBFRecordset::FindPrev()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurTableRecN--;

		if(m_nCurTableRecN < 0)
			return FALSE;

		// перебираем записи
		for(; m_nCurTableRecN >= 0; m_nCurTableRecN--)
		{
			m_pTableDef->ReadRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
		
			alxFetchParserVariable(this, m_pFindParser);

			// вычисляем результа?
			pvarExprResult = m_pFindParser->Execute();

			// если не логическое выражени?
			if(V_VT(pvarExprResult) != VT_BOOL)
				break;

			// если нашл?
			if(V_BOOL(pvarExprResult) != FALSE)
			{
				bResult = TRUE;
				break;
			}
		}
	
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}

	if(m_nCurTableRecN < 0)
	{
		m_nCurTableRecN = 0;
		m_bBOF = TRUE;
	}
	else
		m_bBOF = FALSE;

	m_bEOF = FALSE;

	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return bResult;
}

/* Отка?от сохранен? изменени?сделанны??записи */
void CDBFRecordset::CancelUpdate()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);

	m_pTableDef->ReadRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);

	m_nEditMode = noMode;
}

/* Блокировка текуще?записи */
BOOL CDBFRecordset::LockRecord()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());
	ASSERT(!IsEOF());
	ASSERT(m_nEditMode == noMode);

	if(!(m_nOpenType & ALX_DBF_USE_EXCLUSIVE))
	{
		switch(m_nLockType)
		{
		case LockDBase:
			{
				// 0xEFFFFFFFL - rec number
				QWORD offset = 0xEFFFFFFFL - m_nCurTableRecN;
				QWORD range = 1;
				return::LockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		case LockClipper:
			{
				// 0x40000000L + rec number
				QWORD offset = 1000000000 + (m_nCurTableRecN + 1);
				QWORD range = 1;
				return::LockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		case LockFoxpro:
			{
				// 0x40000000L + rec offset
				QWORD offset = 0x40000000L +/* m_pTableDef->m_DBFHead.data_offset +*/ m_pTableDef->m_DBFHead.rec_size * m_nCurTableRecN;
				QWORD range = 1;
				return::LockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		}
	}
	return TRUE;
}

/* Сняти?блокировки ?текуще?записи */
BOOL CDBFRecordset::UnlockRecord()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());
	ASSERT(!IsEOF());
	ASSERT(m_nEditMode == noMode);

	if(!(m_nOpenType & ALX_DBF_USE_EXCLUSIVE))
	{
		switch(m_nLockType)
		{
		case LockDBase:
			{
				// 0xEFFFFFFFL - rec number
				QWORD offset = 0xEFFFFFFFL - m_nCurTableRecN;
				QWORD range = 1;
				return::UnlockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		case LockClipper:
			{
				// 0x40000000L + rec number
				QWORD offset = 1000000000 + (m_nCurTableRecN + 1);
				QWORD range = 1;
				return::UnlockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		case LockFoxpro:
			{
				// 0x40000000L + rec offset
				QWORD offset = 0x40000000L +/* m_pTableDef->m_DBFHead.data_offset +*/ m_pTableDef->m_DBFHead.rec_size * m_nCurTableRecN;
				QWORD range = 1;
				return::UnlockFile((HANDLE)m_pTableDef->m_DBFile.m_hFile, LODWORD(offset), HIDWORD(offset), LODWORD(range), HIDWORD(range));
			}
		}
	}
	return TRUE;
}

/* Добавление ново?записи */
void CDBFRecordset::AddNew()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = addnew;

	memset(m_DBFFields.m_pCurRec, 0, m_pTableDef->m_DBFHead.rec_size);

	COleVariant varEmpty;
	varEmpty.Clear();
	// присваивае?всем по??пустое значение
	for(short i = 0; i < m_DBFFields.GetFieldCount(); i++)
	{
		FIELD_REC* pFieldRec = m_DBFFields.GetFieldRec(i);
		alxSetFieldValue(m_DBFFields.m_pCurRec, pFieldRec, varEmpty, m_lpfnTextConvert);
	}

	m_DBFFields.m_pCurRec[0] = REC_FLAG_NORMAL;
}

/* Изменени?текуще?записи */
void CDBFRecordset::Edit()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = edit;
}

/* Сохранение измененной записи */
void CDBFRecordset::Update()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);

	switch(m_nEditMode)
	{
	case edit:
		{
			m_pTableDef->ReadHeader();
			m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
//			m_pTableDef->WriteHeader();
			break;
		}
	case addnew:
		{
			m_pTableDef->ReadHeader();
			m_pTableDef->m_DBFHead.last_rec++;
			m_nCurTableRecN = max(0,GetRecordCount() - 1);
			m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
			m_pTableDef->WriteHeader();
			m_bBOF = m_bEOF = FALSE;
			break;
		}
	}

	m_nEditMode = noMode;
}

/* Запись помечает? для удален? */
void CDBFRecordset::Delete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_DELETED;

	m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
}

/* Снимается пометк?на удаление */
void CDBFRecordset::Undelete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_NORMAL;

	m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
}

/* Получаем значение по? */
COleVariant CDBFRecordset::GetFieldValue(LPCTSTR lpszName)
{
	COleVariant varValue;

	GetFieldValue(lpszName, varValue);

	return varValue;
}

/* Получаем значение по? */
COleVariant CDBFRecordset::GetFieldValue(int nIndex)
{
	COleVariant varValue;

	GetFieldValue(nIndex, varValue);

	return varValue;
}

/* Получаем значение по? */
long CDBFRecordset::GetFieldValue(LPCTSTR lpszName, COleVariant& varValue)
{
    short nIndex;
	for(short nIndex = 0;
		strcmp(m_DBFFields.m_pFields[nIndex].field_name, lpszName) != 0 &&
		nIndex <= m_DBFFields.m_nFieldCount;
		nIndex++)
		{};
	
	return GetFieldValue(nIndex, varValue);
}

/* Получаем значение по? */
long CDBFRecordset::GetFieldValue(int nIndex, COleVariant& varValue)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	return m_pTableDef->GetFieldValue(m_DBFFields.m_pCurRec, m_DBFFields.GetFieldRec(nIndex), varValue, m_lpfnTextConvert);
}

/* Сохраняем значение по? */
void CDBFRecordset::SetFieldValue(LPCTSTR lpszName,	const COleVariant& varValue)
{
    short nIndex;
	for(short nIndex = 0;
		strcmp(m_DBFFields.m_pFields[nIndex].field_name, lpszName) != 0 &&
		nIndex <= m_DBFFields.m_nFieldCount;
		nIndex++)
		{};

	SetFieldValue(nIndex, varValue);
}

/* Сохраняем значение по? */
void CDBFRecordset::SetFieldValue(int nIndex, const COleVariant& varValue)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	m_pTableDef->SetFieldValue(m_DBFFields.m_pCurRec, m_DBFFields.GetFieldRec(nIndex), varValue, m_lpfnTextConvert);
}

/* Функция опреде?ет указател?на функци?конвертаци?текста */
void CDBFRecordset::SetTextConvertFunction(void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection))
{
	m_lpfnTextConvert = lpfnTextConvert;
}

/* Функция возвращает TRUE если набо?бы?успешн?открыт */
BOOL CDBFRecordset::IsOpen() const
{
	ASSERT_VALID(this);
	return (m_pTableDef == NULL ? FALSE : m_pTableDef->IsOpen());
}

/* Функция выбрасывае?исключение */
void CDBFRecordset::ThrowDBFException(long nALXDBFError, LPCTSTR szDescription)
{
	ASSERT_VALID(this);

	ALXThrowDBFException(nALXDBFError, szDescription);
}

#ifdef _DEBUG
void CDBFRecordset::AssertValid() const
{
	CObject::AssertValid();
}

void CDBFRecordset::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CObject::Dump(dc);

	dc << "\nm_nOpenType = " << m_nOpenType;
	dc << "\nm_nEditMode = " << m_nEditMode;
	dc << "\nm_nCurTableRecN = " << m_nCurTableRecN;
	dc << "\nm_bBOF = " << m_bBOF;			
	dc << "\nm_bEOF = " << m_bEOF;			

	dc << "\n";
}
#endif //_DEBUG




BOOL CDBFRecordset::ChangeOpenFlag(UINT nNewFlag)
{
	ASSERT(m_pTableDef != NULL);

	return m_pTableDef->ChangeOpenFlag(nNewFlag);
}
