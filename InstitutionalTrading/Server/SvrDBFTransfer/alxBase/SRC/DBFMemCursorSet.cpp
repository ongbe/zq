// DBFMemCursorset.cpp : implementation of the CDBFMemCursorset class
//
/////////////////////////////////////////////////////////////////////////////
/*
Авто?              Долгачев ??
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBFMemCursorset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CDBFMemCursorset
IMPLEMENT_DYNAMIC(CDBFMemCursorset, CDBFRecordset )

/* Конструкто?*/
CDBFMemCursorset::CDBFMemCursorset(CDBFTableDef* pDBFTable)
{
	m_bDesc = FALSE;

	m_bShowDeletedRec = TRUE;

	m_pTableData = NULL;

	m_pEditRec = NULL;

	m_lLoadRecord = 0;

	m_lpfnIsInclude = NULL;

	m_nCurCursorRecN = -1;

	m_bBOF = TRUE;			
	m_bEOF = TRUE;
}

/* Деструктор */
CDBFMemCursorset::~CDBFMemCursorset()
{
	if (IsOpen())
		Close();
}

/* Открытие таблиц?*/
void CDBFMemCursorset::Open(LPCTSTR lpszFileName, UINT nOpenFlag)
{
	ASSERT_VALID(this);

	CString strFileName;

	// получаем имя файл?
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

	// заполняем структур?заголовк??поле?
	m_DBFFields = m_pTableDef->m_DBFFields;
	m_DBFHead = m_pTableDef->m_DBFHead;

	// распреде?ем па?ть для загрузки таблиц?
	UINT lFileSize = m_pTableDef->m_DBFile.GetLength();
	m_pTableData = new DBF_UCHAR[lFileSize];

	// Загружае?файл ?па?ть
	m_pTableDef->m_DBFile.Seek(0, CFile::begin);

#if _MSC_VER > 1200
	DBF_CHECK_ERROR(
		m_pTableDef->m_DBFile.Read(m_pTableData, lFileSize) == lFileSize, ALX_DBF_ERROR_DBFILE_MEMORYLOAD);
#else
	DBF_CHECK_ERROR(
		m_pTableDef->m_DBFile.ReadHuge(m_pTableData, lFileSize) == lFileSize, ALX_DBF_ERROR_DBFILE_MEMORYLOAD);
#endif // _MSC_VER > 1200

	// получаем количество загруженны?записе?
	m_lLoadRecord = m_pTableDef->GetRecordCount();

	// обновляем курсор
	Requery();
}

/* Закрывае?набо?записе?*/
void CDBFMemCursorset::Close()
{
	CDBFRecordset::Close();

	m_bDesc = FALSE;
	m_bShowDeletedRec = TRUE;

	m_nCurCursorRecN = -1;

	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	// освобождае?ресурс?

	if(m_pTableData != NULL) { delete[] m_pTableData; m_pTableData = NULL; }

	m_lLoadRecord = 0;

	if(m_pEditRec != NULL) { delete[] m_pEditRec; m_pEditRec = NULL; }


	m_RecPtr.RemoveAll();
	for(int i = 0; i < 	m_NewRecPtr.GetSize(); i++)
	{
		DBF_UCHAR* pNewRec = (DBF_UCHAR*)m_NewRecPtr[i];
		delete[] pNewRec;
	}
	m_NewRecPtr.RemoveAll();
}

/* Функция возвращает TRUE если те? ячв?ет? перв? запись */
BOOL CDBFMemCursorset::IsBOF() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_nCurCursorRecN == 0);
}

/* Функция возвращает TRUE если те? ячв?ет? послед?я запись */
BOOL CDBFMemCursorset::IsEOF() const
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_nCurCursorRecN >= m_RecPtr.GetSize() - 1);
}

/* Функция возвращает количество записе??наборе */
long CDBFMemCursorset::GetRecordCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_RecPtr.GetSize();
}

/* Функция возвращает абсолютную позици?записи ?наборе */
long CDBFMemCursorset::GetAbsolutePosition()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_nCurCursorRecN;
}

/* Функция устанавливае?абсолютную позици?записи ?наборе */
void CDBFMemCursorset::SetAbsolutePosition(long lPosition)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lPosition >= 0 && lPosition < GetRecordCount());

	DBF_LONG lCursorRecN = lPosition; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Перехо?на следующу?запись */
void CDBFMemCursorset::MoveNext()
{
	ASSERT_VALID(this);
	ASSERT(!IsEOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN;
	
	lCursorRecN++;

	if(lCursorRecN < GetRecordCount())
	{
		// Call Move.
		m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
		m_nCurCursorRecN = lCursorRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bEOF = TRUE;
		m_bBOF = GetRecordCount() > 0; 
	}
}

/* Перехо?на предыдущую запись */
void CDBFMemCursorset::MovePrev()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN; 

	lCursorRecN--;

	if(lCursorRecN >= 0)
	{
		// Call Move.
		m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
		m_nCurCursorRecN = lCursorRecN;
		m_bBOF = m_bEOF = FALSE;
	}
	else
	{
		m_bBOF = TRUE;
		m_bEOF = GetRecordCount() > 0; 
	}
}

/* Перехо?на первую запись ?наборе */
void CDBFMemCursorset::MoveFirst()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = 0; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Перехо?на последню?запись ?наборе */
void CDBFMemCursorset::MoveLast()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = GetRecordCount() - 1; 

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Перехо?на запись по смещению относительно текуще?записи */
void CDBFMemCursorset::Move(long lOffsetRec)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN + lOffsetRec;
	
	// перехо??предыдущим запи??
	if(lOffsetRec < 0)
	{
		if(lCursorRecN < 0)
		{
			lCursorRecN = 0;
			m_bBOF = TRUE;
		}
		else
			m_bBOF = FALSE;

		m_bEOF = FALSE; 
	}
	// перехо??следующи?запи??
	else if(lOffsetRec > 0)
	{
		if(lCursorRecN >= GetRecordCount())
		{
			lCursorRecN = GetRecordCount() - 1;
			m_bEOF = TRUE;
		}
		else
			m_bEOF = FALSE;

		m_bBOF = FALSE; 
	}

	// Call Move.
	m_DBFFields.m_pCurRec = GetRecord(lCursorRecN);
	m_nCurCursorRecN = lCursorRecN;
}

/* Поис?первой записи */
BOOL CDBFMemCursorset::FindFirst(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = -1;
	return FindNext(lpszFilter);
}

/* Поис?следующе?записи */
BOOL CDBFMemCursorset::FindNext(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurCursorRecN++;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurCursorRecN--;

		return FindNext();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* Поис?следующе?записи */
BOOL CDBFMemCursorset::FindNext()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurCursorRecN++;

		if(m_nCurCursorRecN >= GetRecordCount())
			return FALSE;

		// перебираем записи
		for(; m_nCurCursorRecN < GetRecordCount(); m_nCurCursorRecN++)
		{
			m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
		
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

	if(m_nCurCursorRecN >= GetRecordCount())
	{
		m_nCurCursorRecN = GetRecordCount() - 1;
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
BOOL CDBFMemCursorset::FindLast(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = m_RecPtr.GetSize();
	return FindPrev(lpszFilter);
}

/* Поис?предыдущей записи */
BOOL CDBFMemCursorset::FindPrev(LPCTSTR lpszFilter)
{
	if(m_pFindParser != NULL)	delete m_pFindParser;
	m_pFindParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFindParser);

	try
	{
		m_nCurCursorRecN--;

		m_pFindParser->Parse(lpszFilter);
		m_pFindParser->DelUnusedVariables();

		m_nCurCursorRecN++;

		return FindPrev();
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
	return FALSE;
}

/* Поис?предыдущей записи */
BOOL CDBFMemCursorset::FindPrev()
{
	ASSERT(m_pFindParser != NULL);

	BOOL bResult = FALSE;
	VARIANT* pvarExprResult = NULL;

	try
	{
		m_nCurCursorRecN--;

		if(m_nCurCursorRecN < 0)
			return FALSE;

		// перебираем записи
		for(; m_nCurCursorRecN >= 0; m_nCurCursorRecN--)
		{
			m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
		
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

	if(m_nCurCursorRecN < 0)
	{
		m_nCurCursorRecN = 0;
		m_bBOF = TRUE;
	}
	else
		m_bBOF = FALSE;

	m_bEOF = FALSE;

	if(V_VT(pvarExprResult) != VT_BOOL)
		ALXThrowDBFException(ALX_DBF_ERROR_NOT_LOGIC_EXPR);

	return bResult;
}

/* Отка?от сохранен? изменени??записи */
void CDBFMemCursorset::CancelUpdate()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);
	ASSERT(m_pEditRec != NULL);

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	m_nEditMode = noMode;
}

/* Добавление ново?записи. Для сохранен? изменени?необходимо
   вызват?функци?- Update(), для отката - CancelUpdate()      */ 
void CDBFMemCursorset::AddNew()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);
	ASSERT(m_pEditRec == NULL);

	m_nEditMode = addnew;

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	// выде?ем па?ть для ново?записи
	m_pEditRec = new DBF_CHAR[m_DBFHead.rec_size];

	memset(m_pEditRec, 0, m_DBFHead.rec_size);

	COleVariant varEmpty;
	varEmpty.Clear();
	// присваивае?всем по??пустое значение
	for(short i = 0; i < m_DBFFields.GetFieldCount(); i++)
	{
		FIELD_REC* pFieldRec = m_DBFFields.GetFieldRec(i);
		alxSetFieldValue(m_pEditRec, pFieldRec, varEmpty, m_lpfnTextConvert);
	}

	m_pEditRec[0] = REC_FLAG_NORMAL;

	m_DBFFields.m_pCurRec = m_pEditRec;
}

/* Изменени?записи. Для сохранен? изменени?необходимо
   вызват?функци?- Update(), для отката - CancelUpdate() */ 
void CDBFMemCursorset::Edit()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);
	ASSERT(m_pEditRec == NULL);

	m_nEditMode = edit;

	if(m_pEditRec != NULL)
		delete[] m_pEditRec;

	// выде?ем па?ть для копи?записи
	m_pEditRec = new DBF_CHAR[m_DBFHead.rec_size];

	// копируем запись
	memcpy(m_pEditRec, m_DBFFields.m_pCurRec, m_DBFHead.rec_size);

	m_DBFFields.m_pCurRec = m_pEditRec;
}

/* Сохранение измененной записи */
void CDBFMemCursorset::Update()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);
	ASSERT(m_pEditRec != NULL);

	switch(m_nEditMode)
	{
	case edit:
		{
			SetRecord(m_nCurCursorRecN, m_pEditRec);
			delete [] m_pEditRec;
			break;
		}
	case addnew:
		{
			m_DBFHead.last_rec++;
			m_NewRecPtr.Add(m_pEditRec);
			m_RecPtr.Add(m_pEditRec);
			m_nCurCursorRecN = max(0,GetRecordCount() - 1);
			SetRecord(m_nCurCursorRecN, m_pEditRec);
			m_bBOF = m_bEOF = FALSE;
			break;
		}
	}
	
	m_pEditRec = NULL;

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);

	m_nEditMode = noMode;
}

/* Запись помечает? на удаление */
void CDBFMemCursorset::Delete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_DELETED;

	SetRecord(m_nCurCursorRecN, m_DBFFields.m_pCurRec);
}

/* Функция возвращает TRUE если запись была помечена для удален? */
BOOL CDBFMemCursorset::IsDeleted()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return (m_DBFFields.m_pCurRec[0] == REC_FLAG_DELETED);
}

/* Снимае?пометк?на удаление */
void CDBFMemCursorset::Undelete()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFFields.m_pCurRec[0] = REC_FLAG_NORMAL;

	SetRecord(m_nCurCursorRecN, m_DBFFields.m_pCurRec);
}

/* Удаляем указател?на запись. Чтоб?действительн?запись была удален?
   необходимо вызват?функци?Save(). Запись буде?востановлена посл?Requery() */
void CDBFMemCursorset::Remove()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_DBFHead.last_rec--;
	m_RecPtr.RemoveAt(m_nCurCursorRecN);

	if(m_nCurCursorRecN >= GetRecordCount() && m_nCurCursorRecN > 0)
		m_nCurCursorRecN--;

	m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
}

/* Функция возвращает TRUE если набо?записе?бы?успешн?открыт */
BOOL CDBFMemCursorset::IsOpen() const
{
	ASSERT_VALID(this);
	return (m_pTableDef == NULL ? FALSE : m_pTableDef->IsOpen());
}

/* Функция выбрасывае?исключение */
void CDBFMemCursorset::ThrowDBFException(long nALXDBFError, LPCTSTR szDescription)
{
	ASSERT_VALID(this);

	ALXThrowDBFException(nALXDBFError, szDescription);
}

#ifdef _DEBUG
void CDBFMemCursorset::AssertValid() const
{
	CDBFRecordset::AssertValid();
}

void CDBFMemCursorset::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CDBFRecordset::Dump(dc);

	dc << "\n";
}
#endif //_DEBUG

/* Функция возвращает указател?на запись */
DBF_CHAR* CDBFMemCursorset::GetRecord(long lRecNumber)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
	return (DBF_CHAR*)m_RecPtr[lRecNumber];
}

/* Функция возвращает указател?на запись */
void CDBFMemCursorset::GetRecord(long lRecNumber, DBF_CHAR* lpBuffer)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
}

/* Функция копирует данные из указанного буфера ?буфе?записи
   Использует? для занисения изменени??буфе?DBF таблиц?   */
void CDBFMemCursorset::SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer)
{
	ASSERT(lRecNumber < m_RecPtr.GetSize());
	memcpy(m_RecPtr[lRecNumber],lpBuffer,m_DBFHead.rec_size);
}

/* Сортировка по указанному полю */
void CDBFMemCursorset::SortByField(LPCTSTR lpszName, BOOL bDesc /* = FALSE */)
{
    short nIndex;
	for(short nIndex = 0;
		strcmp(m_DBFFields.m_pFields[nIndex].field_name, lpszName) != 0 &&
		nIndex <= m_DBFFields.m_nFieldCount;
		nIndex++)
	
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	SortByField(nIndex, bDesc);
}

/* Сортировка по указанному полю */
void CDBFMemCursorset::SortByField(int nIndex, BOOL bDesc /* = FALSE */)
{
	m_nSortedField = nIndex;

	m_bDesc = bDesc;

	if(GetRecordCount() > 1)
	{
		FIELD_REC* pFieldRec = m_DBFFields.GetFieldRec(nIndex);

		// по двоичным по??не сортируе?
		switch(pFieldRec->field_type)
		{
		case FLD_TYPE_DOUBLE: // возможно FLD_TYPE_BINARY
			// если FLD_TYPE_DOUBLE
			if(pFieldRec->len_info.num_size.len == sizeof(DBF_DOUBLE))
				break;
		case FLD_TYPE_GENERAL:
		case FLD_TYPE_PICTURE:
			return;
		}

		qsort( 0, m_RecPtr.GetSize() - 1 );

		m_DBFFields.m_pCurRec = GetRecord(m_nCurCursorRecN);
	}
}

/* Функция сравнения двух записе?(индекс по? по которому
   осуществ?ет? сравнени?задает? m_nSortedField)       */
int CDBFMemCursorset::compare(const void *elem1, const void *elem2)
{
	// if elem1 < elem2 return TRUE else return FALSE
	COleVariant var1, var2;
	BOOL bResult;

	// если по убыванию
	if(m_bDesc)
	{
		// получаем значен? поле?(причем ме?ем их местам?
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem1;
		var2 = GetFieldValue(m_nSortedField);
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem2;
		var1 = GetFieldValue(m_nSortedField);
	}
	else
	{
		// получаем значен? поле?
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem1;
		var1 = GetFieldValue(m_nSortedField);
		m_DBFFields.m_pCurRec = (DBF_CHAR*)elem2;
		var2 = GetFieldValue(m_nSortedField);
	}
	
	// сравниваем значен? поле?
	if(var1 == var2)
		bResult = FALSE;
	else
	{
		if(var1.vt == VT_EMPTY && var2.vt != VT_EMPTY)
			bResult = TRUE;
		else if (var2.vt == VT_EMPTY && var1.vt != VT_EMPTY)
			bResult = FALSE;
		else
		{
			switch(var1.vt)
			{
			case VT_BSTR:
				{
					CString str1 = var1.bstrVal;
					CString str2 = var2.bstrVal;
					bResult = (str1 < str2);
					break;
				}
			case VT_R8:
				{
					bResult = (var1.dblVal < var2.dblVal);
					break;
				}
			case VT_I4:
				{
					bResult = (var1.lVal < var2.lVal);
					break;
				}
			case VT_CY:
				{
					COleCurrency cur1, cur2;
					cur1.m_cur = var1.cyVal;
					cur2.m_cur = var2.cyVal;
					bResult = (cur1 < cur2);
					break;
				}
			case VT_DATE:
				{
					bResult = (var1.date < var2.date);
					break;
				}
			case VT_BOOL:
				{
					bResult = (var1.boolVal < var2.boolVal);
					break;
				}
			}
		}
	}

	return bResult;
}

/* Алгоритм сортировки поле?*/
void CDBFMemCursorset::qsort(int from, int to)
{
    int i, j;
	void *x, *tmp;

//	int from	= 0;					/* левы?начальны?индекс */
//	int to		= m_RecPtr.GetSize();	/* правый конечный индекс */

	if( from >= to ) return;
	/* числ?элементо?<= 1 */

	i = from; j = to;
	x = m_RecPtr[ (i+j) / 2 ];     /* значение из середины */

	do
	{
		/* сужени?вправо */
		while( compare(m_RecPtr[i], x) ) i++ ;	// m_RecPtr[i] < x

		/* сужени?влев?*/
		while( compare(x, m_RecPtr[j]) ) j--;	// x < m_RecPtr[j] 

		if( i <= j )
		{   /* обме?ть */
			tmp = m_RecPtr[i]; m_RecPtr[i] = m_RecPtr[j] ; m_RecPtr[j] = tmp;
			i++;  j--;
		}
	} while( i <= j );

            /* Теперь об?част?сошлис??одно?точк?
             * Длин?лево?част?= j - from + 1
             *       правой      = to - i   + 1
             * Вс?числ??лево?част?меньше всех чисе??правой.
             * Теперь надо просто отсортироват?каждую част??отдельност?
             * Сначал?сортируе?боле?короткую (для экономии па?ти
             * ?стек?). Рекурс?:
             */
            if( (j - from)  <  (to - i) ){
                    qsort( from, j );
                    qsort( i, to   );
            } else {
                    qsort( i,   to );
                    qsort( from, j );
            }
}

/* Устанавливае?способ вывода поле?(отображать
   ил?не?помеченные на удаление записи)        */
void CDBFMemCursorset::SetDelete(BOOL bHideDeleteRec /* = TRUE */)
{
	m_bShowDeletedRec = !bHideDeleteRec;
}

/* Опреде?ем способ вывода поле?(отображают?
   ил?не?помеченные на удаление записи)        */
BOOL CDBFMemCursorset::GetDelete()
{
	return !m_bShowDeletedRec;
}

/* Обновляем курсор */
void CDBFMemCursorset::Requery()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = noMode;

	m_RecPtr.SetSize(m_lLoadRecord + m_NewRecPtr.GetSize());

	long nRec = 0;
	// если отображать помеченные на удаление записи
	if(m_bShowDeletedRec)
	{
		// перебираем вс?загруженны?записи
		for(long i = 0; i < m_lLoadRecord; i++)
		{
			m_RecPtr[nRec] =  m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i;
			// если указан?функция фильтр?
			if(m_lpfnIsInclude != NULL)
			{
				// если запись удовлетворяет услови?фильтр?
				if((*m_lpfnIsInclude)(this))
					nRec++;
			}
			else
				nRec++;
		}
		// перебираем добавленны?посл?загрузки записи
		for(long j = 0; j < m_NewRecPtr.GetSize(); j++)
		{
			m_RecPtr[nRec] = m_NewRecPtr[j];
			// если указан?функция фильтр?
			if(m_lpfnIsInclude != NULL) 
			{
				// если запись удовлетворяет услови?фильтр?
				if((*m_lpfnIsInclude)(this))
					nRec++;
			}
			else
				nRec++;
		}
	}
	else
	{
		// перебираем вс?загруженны?записи
		for(long i = 0; i < m_lLoadRecord; i++)
		{
			if(REC_FLAG_DELETED != *(DBF_CHAR*)(m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i))
			{
				m_RecPtr[nRec] =  m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i;
				// если указан?функция фильтр?
				if(m_lpfnIsInclude != NULL) 
				{
					// если запись удовлетворяет услови?фильтр?
					if((*m_lpfnIsInclude)(this))
						nRec++;
				}
				else
					nRec++;
			}
		}
		// перебираем добавленны?посл?загрузки записи
        long i;
		for(i = 0; i < m_NewRecPtr.GetSize(); i++)
		{
			if(REC_FLAG_DELETED != *(DBF_CHAR*)(m_NewRecPtr[i]))
			{
				m_RecPtr[nRec] = m_NewRecPtr[i];
				// если указан?функция фильтр?
				if(m_lpfnIsInclude != NULL) 
				{
					// если запись удовлетворяет услови?фильтр?
					if((*m_lpfnIsInclude)(this))
						nRec++;
				}
				else
					nRec++;
			}
		}
	}

	m_RecPtr.SetSize(nRec);
	
	m_DBFHead.last_rec = m_RecPtr.GetSize();

	if(GetRecordCount() > 0)
		MoveFirst();
	else
		m_nCurCursorRecN = 0;
}

// Запись изменени??файл */
BOOL CDBFMemCursorset::Save()
{
	CFile* pDBFile = &m_pTableDef->m_DBFile;
/*	if (!SaveFile.Open(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareDenyNone))
	{
//		ReportSaveLoadException(lpszPathName, &fe,
//			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}
*/
	DBF_CHAR* pRec;
	pDBFile->Seek(m_DBFHead.data_offset, CFile::begin);
	// сохраняем записи из таблиц?
    long i,j;
	for(long i = 0; i < m_lLoadRecord; i++)
	{
		pRec = (DBF_CHAR*)(m_pTableData + m_DBFHead.data_offset + m_DBFHead.rec_size * i);
		pDBFile->Write(pRec, m_DBFHead.rec_size);
	}
	// добавляем новы?записи
	for(long j = 0; j < m_NewRecPtr.GetSize(); j++)
	{
		pRec = (DBF_CHAR*) m_NewRecPtr[j];
		pDBFile->Write(pRec, m_DBFHead.rec_size);
	}

	char eof = REC_EOF;
	// записываем призна?конц?DBF файл?
	pDBFile->Write(&eof, 1);

	// Изме?ем структур?заголовк?
	COleDateTime curDate = COleDateTime::GetCurrentTime();
	if(m_DBFHead.dbf_id == 0x30) // Visual FoxPro
		m_DBFHead.last_update[0] = atoi(curDate.Format("%y"));
	else
		m_DBFHead.last_update[0] = curDate.GetYear() - ALX_DBF_CENTURY;

	m_DBFHead.last_update[1] = curDate.GetMonth();
	m_DBFHead.last_update[2] = curDate.GetDay();
	m_DBFHead.last_rec = i + j;

	pDBFile->Seek(0, CFile::begin);
	// записываем заголово?
	pDBFile->Write(&m_DBFHead, sizeof(DBF_HEAD));
	// записываем список поле?
	for(i = 0; i < m_DBFFields.GetFieldCount(); i++)
		pDBFile->Write(m_DBFFields.GetFieldRec(i), sizeof(FIELD_REC));
	char end = 0x0D;
	pDBFile->Write(&end, 1);
	// измением размер файл?
	pDBFile->SetLength(m_DBFHead.data_offset + m_DBFHead.last_rec * m_DBFHead.rec_size + 1);

	m_pTableDef->m_DBFHead.last_rec = m_DBFHead.last_rec;

	return TRUE;
}

/* Передаем указател?на функци?фильтр? Чтоб?убрать фильтр нужн?передать NULL */
void CDBFMemCursorset::SetFilter(BOOL (FAR * lpfnIsInclude) (CDBFRecordset* pSet) )
{
	m_lpfnIsInclude = lpfnIsInclude;
}

void CDBFMemCursorset::SetFilter(LPCTSTR lpszFilter)
{
	if(lpszFilter == NULL)
	{
		m_lpfnIsInclude = NULL;
		return;
	}

	if(m_pFilterParser != NULL)	delete m_pFilterParser;

	m_pFilterParser = new CALXParser;

	alxFetchEmptyParserVariable(m_DBFFields, m_pFilterParser);

	try
	{
		m_pFilterParser->Parse(lpszFilter);
		m_pFilterParser->DelUnusedVariables();

		m_lpfnIsInclude = alxFilter;
	}
	catch(CALXParserException* e)
	{
		ALXThrowDBFException(e);
	}
}