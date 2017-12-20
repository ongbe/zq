// DBFCursorSet.cpp : implementation of the CDBFCursorSet class
//
/////////////////////////////////////////////////////////////////////////////
/*
Авто?              Долгачев ??
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBFCursorset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CDBFCursorset
IMPLEMENT_DYNAMIC(CDBFCursorset, CDBFRecordset)

/* Конструкто?*/
CDBFCursorset::CDBFCursorset(CDBFTableDef* pDBFTable)
{
	m_bDesc = FALSE;

	m_bShowDeletedRec = TRUE;

	m_nCurCursorRecN = -1;
	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	m_lpfnIsInclude = NULL;
}

/* Деструктор */
CDBFCursorset::~CDBFCursorset()
{
	if (IsOpen())
		Close();
}

/* Открытие таблиц?*/
void CDBFCursorset::Open(LPCTSTR lpszFileName, UINT nOpenFlag)
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

	// обновляем курсор
	Requery();
}

/* Закрывае?набо?записе?*/
void CDBFCursorset::Close()
{
	CDBFRecordset::Close();

	m_bDesc = FALSE;
	m_bShowDeletedRec = TRUE;

	m_nCurCursorRecN = -1;
	m_bBOF = TRUE;			
	m_bEOF = TRUE;

	// освобождае?ресурс?

	m_RecPtr.RemoveAll();
}

/* Функция возвращает TRUE если те? ячв?ет? перв? запись */
BOOL CDBFCursorset::IsBOF() const
{
	ASSERT_VALID(this);

	return m_bBOF;
}

/* Функция возвращает TRUE если те? ячв?ет? послед?я запись */
BOOL CDBFCursorset::IsEOF() const
{
	ASSERT_VALID(this);

	return m_bEOF;
}

/* Функция возвращает количество записе??наборе */
long CDBFCursorset::GetRecordCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_RecPtr.GetSize();
}

/* Функция возвращает абсолютную позици?записи ?наборе */
long CDBFCursorset::GetAbsolutePosition()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_nCurCursorRecN;
}

/* Функция устанавливае?абсолютную позици?записи ?наборе */
void CDBFCursorset::SetAbsolutePosition(long lPosition)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lPosition >= 0 && lPosition < GetRecordCount());

	DBF_LONG lCursorRecN = lPosition; 

	// Call Move.
	m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[lCursorRecN];
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;

}

/* Перехо?на следующу?запись */
void CDBFCursorset::MoveNext()
{
	ASSERT_VALID(this);
	ASSERT(!IsEOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN;
	lCursorRecN++;

	if(lCursorRecN < GetRecordCount())
	{
		// Call Move.
		m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
		m_nCurTableRecN = m_RecPtr[lCursorRecN];
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
void CDBFCursorset::MovePrev()
{
	ASSERT_VALID(this);
	ASSERT(!IsBOF());

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = m_nCurCursorRecN;
	lCursorRecN--;

	if(lCursorRecN < GetRecordCount())
	{
		// Call Move.
		m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
		m_nCurTableRecN = m_RecPtr[lCursorRecN];
		m_nCurCursorRecN = lCursorRecN;
	}
	else
	{
		m_bEOF = TRUE;
		m_bBOF = GetRecordCount() > 0; 
	}

}

/* Перехо?на первую запись ?наборе */
void CDBFCursorset::MoveFirst()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = 0; 

	// Call Move.
	m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[lCursorRecN];
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;
}

/* Перехо?на последню?запись ?наборе */
void CDBFCursorset::MoveLast()
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

	m_nEditMode = noMode;

	DBF_LONG lCursorRecN = 0; 

	lCursorRecN = GetRecordCount() - 1;

	// Call Move.
	m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[lCursorRecN];
	m_nCurCursorRecN = lCursorRecN;
	m_bBOF = m_bEOF = FALSE;
}

/* Перехо?на запись по смещению относительно текуще?записи */
void CDBFCursorset::Move(long lOffsetRec)
{
	ASSERT_VALID(this);
	ASSERT(GetRecordCount() > 0);

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
	m_pTableDef->ReadRecord(m_RecPtr[lCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[lCursorRecN];
	m_nCurCursorRecN = lCursorRecN;
}

/* Поис?первой записи */
BOOL CDBFCursorset::FindFirst(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = -1;
	return FindNext(lpszFilter);
}

/* Поис?следующе?записи */
BOOL CDBFCursorset::FindNext(LPCTSTR lpszFilter)
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
BOOL CDBFCursorset::FindNext()
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
			m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];
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
BOOL CDBFCursorset::FindLast(LPCTSTR lpszFilter)
{
	m_nCurCursorRecN = m_RecPtr.GetSize();
	return FindPrev(lpszFilter);
}

/* Поис?предыдущей записи */
BOOL CDBFCursorset::FindPrev(LPCTSTR lpszFilter)
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
BOOL CDBFCursorset::FindPrev()
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
			m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];
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
void CDBFCursorset::CancelUpdate()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);

	m_pTableDef->ReadRecord(m_RecPtr[m_nCurCursorRecN], m_DBFFields.m_pCurRec);
	m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];

	m_nEditMode = noMode;
}

/* Добавление ново?записи. Для сохранен? изменени?необходимо
   вызват?функци?- Update(), для отката - CancelUpdate()      */ 
void CDBFCursorset::AddNew()
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

/* Изменени?записи. Для сохранен? изменени?необходимо
   вызват?функци?- Update(), для отката - CancelUpdate() */ 
void CDBFCursorset::Edit()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_nEditMode = edit;
}

/* Сохранение измененной записи */
void CDBFCursorset::Update()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode != noMode);

	switch(m_nEditMode)
	{
	case edit:
		{
			m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
			break;
		}
	case addnew:
		{
			m_pTableDef->ReadHeader();
			m_pTableDef->m_DBFHead.last_rec++;
			m_nCurTableRecN = max(0,m_pTableDef->m_DBFHead.last_rec - 1);
			m_pTableDef->WriteRecord(m_nCurTableRecN, m_DBFFields.m_pCurRec);
			m_pTableDef->WriteHeader();
			m_RecPtr.Add(m_nCurTableRecN);
			m_nCurCursorRecN = GetRecordCount() -1;
			break;
		}
	}

	m_nEditMode = noMode;
}

/* Удаляем указател?на запись. Чтоб?действительн?запись была удален?
   необходимо вызват?функци?Save(). Запись буде?востановлена посл?Requery() */
void CDBFCursorset::Remove()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	ASSERT(m_nEditMode == noMode);

	m_RecPtr.RemoveAt(m_nCurCursorRecN);

	if(m_nCurCursorRecN >= m_RecPtr.GetSize() && m_nCurCursorRecN > 0)
		m_nCurCursorRecN--;

	if(GetRecordCount() > 0)
	{
		m_pTableDef->ReadRecord(m_RecPtr[m_nCurCursorRecN], m_DBFFields.m_pCurRec);
		m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];
	}
}

/* Функция возвращает TRUE если набо?записе?бы?успешн?открыт */
BOOL CDBFCursorset::IsOpen() const
{
	ASSERT_VALID(this);
	return (m_pTableDef == NULL ? FALSE : m_pTableDef->IsOpen());
}

/* Функция выбрасывае?исключение */
void CDBFCursorset::ThrowDBFException(long nALXDBFError, LPCTSTR szDescription)
{
	ASSERT_VALID(this);

	ALXThrowDBFException(nALXDBFError, szDescription);
}

#ifdef _DEBUG
void CDBFCursorset::AssertValid() const
{
	CDBFRecordset::AssertValid();
}

void CDBFCursorset::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CDBFRecordset::Dump(dc);

	dc << "\n";
}
#endif //_DEBUG

/* Функция копирует данные из указанного буфера ?буфе?записи
   Использует? для занисения изменени??буфе?DBF таблиц?   */
void CDBFCursorset::SetRecord(long lRecNumber, const DBF_CHAR* lpBuffer)
{
/*	ASSERT(lRecNumber < m_RecPtr.GetSize());
	memcpy(m_RecPtr[lRecNumber],lpBuffer,m_DBFHead.rec_size);
*/
}

/* Сортировка по указанному полю */
void CDBFCursorset::SortByField(LPCTSTR lpszName, BOOL bDesc /* = FALSE */)
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
void CDBFCursorset::SortByField(int nIndex, BOOL bDesc /* = FALSE */)
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

		qsort( 0, GetRecordCount() - 1 );

		m_pTableDef->ReadRecord(m_RecPtr[m_nCurCursorRecN], m_DBFFields.m_pCurRec);
		m_nCurTableRecN = m_RecPtr[m_nCurCursorRecN];
	}

}

/* Функция сравнения двух записе?(индекс по? по которому
   осуществ?ет? сравнени?задает? m_nSortedField)       */
int CDBFCursorset::compare(const long elem1, const long elem2)
{
	// if elem1 < elem2 return TRUE else return FALSE
	COleVariant var1, var2;
	BOOL bResult;

	// если по убыванию
	if(m_bDesc)
	{
		// получаем значен? поле?(причем ме?ем их местам?
		m_pTableDef->ReadRecord(elem1, m_DBFFields.m_pCurRec);
		var2 = GetFieldValue(m_nSortedField);
		m_pTableDef->ReadRecord(elem2, m_DBFFields.m_pCurRec);
		var1 = GetFieldValue(m_nSortedField);
	}
	else
	{
		// получаем значен? поле?
		m_pTableDef->ReadRecord(elem1, m_DBFFields.m_pCurRec);
		var1 = GetFieldValue(m_nSortedField);
		m_pTableDef->ReadRecord(elem2, m_DBFFields.m_pCurRec);
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
void CDBFCursorset::qsort(int from, int to)
{
    int i, j;
	long x, tmp;

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
void CDBFCursorset::SetDelete(BOOL bHideDeleteRec /* = TRUE */)
{
	m_bShowDeletedRec = !bHideDeleteRec;
}

/* Опреде?ем способ вывода поле?(отображают?
   ил?не?помеченные на удаление записи)        */
BOOL CDBFCursorset::GetDelete()
{
	return !m_bShowDeletedRec;
}

/* Обновляем курсор */
void CDBFCursorset::Requery()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	m_nEditMode = noMode;

	m_RecPtr.SetSize(m_pTableDef->GetRecordCount());

	long nRec = 0;
	// если отображать помеченные на удаление записи
	if(m_bShowDeletedRec)
	{
		// перебираем вс?записи
		for(long i = 0; i < m_RecPtr.GetSize(); i++)
		{
			m_RecPtr[nRec] =  i;
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
		for(long i = 0; i < m_RecPtr.GetSize(); i++)
		{
			m_pTableDef->ReadRecord(m_RecPtr[nRec], m_DBFFields.m_pCurRec);
			if(REC_FLAG_DELETED != m_DBFFields.m_pCurRec[0])
			{
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
	
	if(GetRecordCount() > 0)
		MoveFirst();
	else
	{
		m_nCurTableRecN = 0;
		m_nCurCursorRecN = 0;
	}
}

/* Передаем указател?на функци?фильтр? Чтоб?убрать фильтр нужн?передать NULL */
void CDBFCursorset::SetFilter(BOOL (FAR * lpfnIsInclude) (CDBFRecordset* pSet) )
{
	m_lpfnIsInclude = lpfnIsInclude;
}

void CDBFCursorset::SetFilter(LPCTSTR lpszFilter)
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