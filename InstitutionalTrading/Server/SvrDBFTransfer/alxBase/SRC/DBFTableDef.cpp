// DBFTableDef.cpp : implementation of the CDBFTableDef class
//
/////////////////////////////////////////////////////////////////////////////
/*
Авто?              Долгачев ??
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBFTableDef.h"

#define MIN_DATE		(-657434L)  // about year 100
#define MAX_DATE		2958465L    // about year 9999

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// функция ме?ет расширение ?имен?файл?
CString alxChangeFileExt(CString& strFileName, CString strNewFileExt)
{
	CString strResult;
	int i = strFileName.ReverseFind('.');
	if(i >= 0)
		strResult = strFileName.Left(i);
	return strResult += '.'+strNewFileExt;
}

// функция создае??возвращает указател?на клас?мемо файл?
CMemoFile* alxNewMemoFile(UINT nMemoType)
{
	switch(nMemoType)
	{
	case MEMO_TYPE_FOXPRO:		// FoxPro
	case MEMO_TYPE_VFOXPRO:		// Visual FoxPro
	case MEMO_TYPE_FOX_DATABASE:// Visual FoxPro Database
		return new CFPTMemoFile;
	case MEMO_TYPE_SIX:			// HiPer-SIx
		return new CSMTMemoFile;
	case MEMO_TYPE_DBASEIV:		// dBASE (dBASEIV or higher)
		return new CDBTMemoFile;
	default:					// dBASE (lower dBASEIV)
		return new CMemoFile;
	}
}

// функция опреде?ет расширение для имен?мемо файл?
LPCTSTR alxMemoFileExt(UINT nMemoType)
{
	switch(nMemoType)
	{
	case MEMO_TYPE_FOXPRO:		// .FPT - FoxPro
	case MEMO_TYPE_VFOXPRO:		// .FPT - Visual FoxPro
		return _T("FPT");
	case MEMO_TYPE_FOX_DATABASE:// .DCT - Visual FoxPro Database
		return _T("DCT");
	case MEMO_TYPE_SIX:			// .SMT - HiPer-SIx
		return _T("SMT");
	default:					// .DBT - dBASE
		return _T("DBT");
	}
}

//////////////////////////////////////////////////////////////////////////
// CDBFTableDef
IMPLEMENT_DYNAMIC(CDBFTableDef, CObject)

/* Конструкто?*/
CDBFTableDef::CDBFTableDef()
{
	m_bOpen = FALSE;
	m_bNew = FALSE;
	m_nOpenFlag = ALX_DBF_USE_READ_ONLY | CFile::typeBinary;

	m_pMemoFile = NULL;

	m_pData = NULL;

	memset(&m_DBFHead, 0, sizeof(m_DBFHead));

	m_pOldDBFHead = NULL;
	m_pOldDBFFields = NULL;
	m_pSwapFld = NULL;
}

/* Деструктор */
CDBFTableDef::~CDBFTableDef()
{
	Close();

	// освобождае?выделенные ресурс?
	if(m_pOldDBFHead != NULL)
	{
		delete[] m_pOldDBFHead;
		m_pOldDBFHead = NULL;
	}
	if(m_pOldDBFFields != NULL)
	{
		if(m_pOldDBFFields->m_pFields != NULL)
			delete[] m_pOldDBFFields->m_pFields;
		delete m_pOldDBFFields;
		m_pOldDBFFields = NULL;
	}
	if(m_pSwapFld != NULL)
	{
		delete[] m_pSwapFld;
		m_pSwapFld = NULL;
	}
}

/* Открывае?таблиц?*/
void CDBFTableDef::Open(LPCTSTR lpszName, UINT nOpenFlag)
{
	ASSERT_VALID(this);
	ASSERT(lpszName != NULL);

	// открывае?ка?бинарный файл
	m_nOpenFlag = nOpenFlag | CFile::typeBinary;

	CString strMemoFileExt;
	// Re-open is invalid
	if (IsOpen())
	{
		ASSERT(FALSE);
		return;
	}

	if(m_DBFFields.m_pFields != NULL)
		delete[] m_DBFFields.m_pFields;


TRY
{

	// Открывае?файл
	DBF_CHECK_ERROR(
		m_DBFile.Open(lpszName, m_nOpenFlag), ALX_DBF_ERROR_DBFILE_OPEN);
	// Читаем заголово?
	DBF_CHECK_ERROR(
		m_DBFile.Read(&m_DBFHead, sizeof(m_DBFHead)) == sizeof(DBF_HEAD), ALX_DBF_ERROR_DBFHEADER_READ);
	// Проверяем заголово?
	DBF_CHECK_ERROR(
		m_DBFHead.rec_size > 1 && m_DBFHead.last_rec >= 0, ALX_DBF_ERROR_NOT_TABLE);
	// Проверяем размер файл?
	DBF_CHECK_ERROR(
		m_DBFile.GetLength() - (m_DBFHead.data_offset + m_DBFHead.rec_size * m_DBFHead.last_rec) <= 1, ALX_DBF_ERROR_NOT_TABLE);

	// Вычисляем количество поле?
	if(m_DBFHead.dbf_id == 0x30) // если FoxPro 3.0
	{
		m_DBFFields.m_nFieldCount = (m_DBFHead.data_offset - 296)/sizeof(FIELD_REC);

		// Должно быть, покрайне?мере, 1 поле
		DBF_CHECK_ERROR(
			m_DBFFields.m_nFieldCount > 0, ALX_DBF_ERROR_NOT_FIELDS);

		// Проверяем размер файл?
		DBF_CHECK_ERROR(
			m_DBFile.GetLength() >= (DWORD)(m_DBFHead.data_offset + (m_DBFHead.last_rec * m_DBFHead.rec_size)),
			ALX_DBF_ERROR_FILE_SIZE);

		m_DBFFields.m_pFields = new FIELD_REC[m_DBFFields.m_nFieldCount];

		// Читаем список поле?
		DBF_CHECK_ERROR(
			m_DBFile.Read((void*)m_DBFFields.m_pFields, (UINT)m_DBFFields.m_nFieldCount*sizeof(FIELD_REC)) == (UINT)m_DBFFields.m_nFieldCount*sizeof(FIELD_REC),
			ALX_DBF_ERROR_FIELDS_READ);
	}
	else
	{
		short nIndCurField = 0;
		short nFieldWidth;
		DBF_LONG lFldOffset = 1;
		BOOL bIsMoreFields = TRUE;
		short nFieldCount = (m_DBFHead.data_offset - sizeof(DBF_HEAD) - 1)/sizeof(FIELD_REC);
		FIELD_REC*	pFields = new FIELD_REC[nFieldCount];
		// Для каждог?по?
		while(bIsMoreFields && nIndCurField < nFieldCount)
		{
			// Считывае?мемо поле
			if(m_DBFile.Read(&pFields[nIndCurField],sizeof(FIELD_REC)) != sizeof(FIELD_REC))
			{
				ASSERT(FALSE);
				delete[] pFields;
				ALXThrowDBFException(ALX_DBF_ERROR_FIELDS_READ);
			}
			if(bIsMoreFields)
			{
				// Эт?последне?поле
				bIsMoreFields = (pFields[nIndCurField].field_name[0] != 0x0D);
				if(bIsMoreFields)
				{
					// Не?  смещение по? ?записи
					switch(pFields[nIndCurField].field_type)
					{
					case FLD_TYPE_NUMERIC:
					case FLD_TYPE_CURRENCY:
					case FLD_TYPE_FLOAT:
					case FLD_TYPE_DOUBLE:
					case FLD_TYPE_INTEGER:
						nFieldWidth = pFields[nIndCurField].len_info.num_size.len;
						break;
					default:
						nFieldWidth = pFields[nIndCurField].len_info.char_len;
					}

					pFields[nIndCurField].field_offset = lFldOffset;
					lFldOffset = lFldOffset + nFieldWidth;
					nIndCurField++;
				}
			}
		}

		m_DBFFields.m_nFieldCount = nIndCurField;
		m_DBFFields.m_pFields = new FIELD_REC[m_DBFFields.m_nFieldCount];
		memcpy(m_DBFFields.m_pFields, pFields, m_DBFFields.m_nFieldCount*sizeof(FIELD_REC));
		delete[]pFields;
	}

	UINT nMemoType = MEMO_TYPE_NOMEMO;

	// Имее?ли файл мемо по? ?
	switch(m_DBFHead.dbf_id)
	{
	case 0x30: // Visual FoxPro 3.0
		if(m_DBFHead.table_flag & TBL_FLAG_MEMO)
			if(m_DBFHead.table_flag & TBL_FLAG_DATABASE)
				nMemoType = MEMO_TYPE_FOX_DATABASE;
			else
				nMemoType = MEMO_TYPE_VFOXPRO;
		break;
	case 0xE5:// SMT файл
		nMemoType = MEMO_TYPE_SIX;
		break;
	case 0xF5:// FPT файл
		nMemoType = MEMO_TYPE_FOXPRO;
		break;
	case 0x8B://DBT файл dBASE IV
	case 0x7B://dBASE IV with memo 
	case 0xCB://DBT файл dBASE IV SQL
		nMemoType = MEMO_TYPE_DBASEIV;
		break;
	case 0x83:// DBT файл FoxBASE+/dBASE III PLUS
		nMemoType = MEMO_TYPE_DBASE;
		break;
	}

	// Если нужн? открывае?мемо файл.
	if(nMemoType)
	{
		m_pMemoFile = alxNewMemoFile(nMemoType);
		CString strMemoFileName = alxChangeFileExt(CString(lpszName), alxMemoFileExt(nMemoType));
		// Открывае?мемо файл
		m_pMemoFile->Open(strMemoFileName, nOpenFlag);
	}

	m_DBFFields.m_pCurRec = new DBF_CHAR[m_DBFHead.rec_size];
	memset(m_DBFFields.m_pCurRec, 0, m_DBFHead.rec_size);

	m_bOpen = TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();
	Close();
}
END_CATCH

}

/* Закрывае?таблиц?*/
void CDBFTableDef::Close()
{
	if(m_bOpen || m_bNew)
	{
		m_DBFile.Close();
		if(m_pMemoFile != NULL)
		{
			if(m_pMemoFile->IsOpen())
				m_pMemoFile->Close();

			switch(m_pMemoFile->GetMemoFileType())
			{
			case MEMO_TYPE_FOXPRO:
			case MEMO_TYPE_VFOXPRO:
			case MEMO_TYPE_FOX_DATABASE:
				delete (CFPTMemoFile*) m_pMemoFile;
				break;
			case MEMO_TYPE_DBASEIV:
				delete (CDBTMemoFile*) m_pMemoFile;
				break;
			case MEMO_TYPE_SIX:
				delete (CSMTMemoFile*) m_pMemoFile;
				break;
			case MEMO_TYPE_DBASE:
				delete (CMemoFile*) m_pMemoFile;
				break;
			default:
				{
					ASSERT(FALSE);
					delete m_pMemoFile;
				}
			}

			m_pMemoFile = NULL;
		}

	}

	m_bOpen = FALSE;
	m_bNew = FALSE;


	if(m_DBFFields.m_pFields != NULL)
	{
		if(m_DBFFields.m_pCurRec != NULL)
			delete[] m_DBFFields.m_pCurRec;

		delete[] m_DBFFields.m_pFields;
	}

	m_DBFFields.m_pFields = NULL;

	if(m_pData != NULL)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}

/* Функция возвращает имя таблиц?*/
CString CDBFTableDef::GetName()
{
	return m_DBFile.GetFileName();
}

/* Функция возвращает полный путь ?DBF файл?*/
CString CDBFTableDef::GetDBFilePath()
{
	return m_DBFile.GetFilePath();
}

/* Функция возвращает полный путь ?Memo файл?*/
CString CDBFTableDef::GetMemoFilePath()
{
	if(m_pMemoFile != NULL)
		return m_pMemoFile->GetFilePath();
	else
		return CString("");
}

/* Функция возвращает имя Мемо файл?*/
CString CDBFTableDef::GetMemoName()
{
	if(m_pMemoFile != NULL)
		return m_pMemoFile->GetFileName();
	else
		return CString("");
}

/* Дата создан? таблиц?*/
CTime CDBFTableDef::GetDateCreated()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	CFileStatus fStatus;
	m_DBFile.GetStatus(fStatus);

	return fStatus.m_ctime;
}

/* Дата последне?модификаци?таблиц?*/
CTime CDBFTableDef::GetDateLastUpdated()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return CTime(ALX_DBF_CENTURY + m_DBFHead.last_update[0],m_DBFHead.last_update[1],m_DBFHead.last_update[2]);
}

/* Ко?во записе??таблиц?*/
long CDBFTableDef::GetRecordCount()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	return m_DBFHead.last_rec;
}

/* Ко?во поле??таблиц?*/
short CDBFTableDef::GetFieldCount()
{
	ASSERT_VALID(this);

	return m_DBFFields.m_nFieldCount;
}

/* Читаем запись */
void CDBFTableDef::ReadRecord(long lRecNumber, void* lpBuffer)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lpBuffer != NULL);
	ASSERT(lRecNumber < GetRecordCount());

TRY
{
	m_DBFile.Seek(m_DBFHead.data_offset + m_DBFHead.rec_size * lRecNumber, CFile::begin);
	// Читаем запись
	int nRead = m_DBFile.Read(lpBuffer, m_DBFHead.rec_size);

	if(nRead != m_DBFHead.rec_size)
	{
		long lOffset = m_DBFHead.data_offset + m_DBFHead.rec_size * lRecNumber;
		if(lOffset < sizeof(DBF_HEAD) || lOffset + m_DBFHead.rec_size > (long)m_DBFile.GetLength())
			ALXThrowDBFException(ALX_DBF_ERROR_RECORD_OUTOFRANGE);
		else
			ALXThrowDBFException(ALX_DBF_ERROR_RECORD_READ);
	}
}
CATCH(CFileException, e)
{
	memset(&lpBuffer,'\0', m_DBFHead.rec_size);

	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_RECORD_READ);

} END_CATCH

}

/* Записываем запись */
void CDBFTableDef::WriteRecord(long lRecNumber, void* lpBuffer)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());
	ASSERT(lpBuffer != NULL);
	ASSERT(lRecNumber < GetRecordCount());

TRY
{
	m_DBFile.Seek(m_DBFHead.data_offset + m_DBFHead.rec_size * lRecNumber, CFile::begin);
	// Записываем запись
	m_DBFile.Write(lpBuffer, m_DBFHead.rec_size);

}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_RECORD_WRITE);

} END_CATCH

}

/* Читаем заголово?таблиц?*/
BOOL CDBFTableDef::ReadHeader()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

TRY
{
	m_DBFile.Seek(0, CFile::begin);
	// Читаем заголово?
	if(m_DBFile.Read(&m_DBFHead, sizeof(m_DBFHead)) == sizeof(DBF_HEAD))
		return TRUE;
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_DBFHEADER_WRITE);

} END_CATCH

	return FALSE;
}

/* Записываем заголово?таблиц?*/
void CDBFTableDef::WriteHeader()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

TRY
{
	m_DBFile.Seek(0, CFile::begin);
	// Записываем заголово?
	m_DBFile.Write(&m_DBFHead, sizeof(m_DBFHead));
}
CATCH(CFileException, e)
{
	e->ReportError();
	e->Delete();

	ALXThrowDBFException(ALX_DBF_ERROR_DBFHEADER_WRITE);

} END_CATCH

}

/* Читаем мемо поле */
void CDBFTableDef::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	if(m_pMemoFile != NULL)
		m_pMemoFile->ReadMemoRecord(lOffsetRec, nLen, strMemo);
}

/* Читаем мемо поле */
void CDBFTableDef::ReadMemoRecord(DBF_ULONG lOffsetRec, DBF_LONG& nLen, unsigned char** pMemoData)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	if(m_pMemoFile != NULL)
		m_pMemoFile->ReadMemoRecord(lOffsetRec, nLen, pMemoData);
}

/* Записываем мемо поле */
void CDBFTableDef::WriteMemoRecord(DBF_ULONG& lOffsetRec, DBF_LONG& nLen, CString& strMemo)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	if(m_pMemoFile != NULL)
		m_pMemoFile->WriteMemoRecord(lOffsetRec, nLen, strMemo);
}

/* Функция возвращает значение по?*/
long CDBFTableDef::GetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, COleVariant& varValue,
						void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection))
{
	// получаем значение из таблиц?
	long lLen = alxGetFieldValue(pRec, pFieldRec, varValue, lpfnTextConvert);
	// если нужн?получаем значение из мемо файл?
	switch(pFieldRec->field_type)
	{
	case FLD_TYPE_MEMO:
		{
			if(varValue.vt == VT_UI4)
				if(varValue.ulVal > 0) // если есть запись ?мемо файл?
				{
					CString strValue;
					ReadMemoRecord(varValue.ulVal, lLen, strValue);

					if(lpfnTextConvert != NULL && !(pFieldRec->field_flag & FLD_FLAG_BINARY))
						(*lpfnTextConvert)(strValue, GetText);

					varValue.Clear();
					varValue = strValue;
				}
				else
				{
					lLen = 0;
					varValue.Clear();
					varValue.vt = VT_EMPTY;
				}
			else if(varValue.vt == VT_DECIMAL) // SIX
				if(varValue.decVal.Lo32 > 0) // если есть запись ?мемо файл?
				{
					CString strValue;
					lLen = varValue.decVal.Hi32;
					ReadMemoRecord(varValue.ulVal, lLen, strValue);
					if(lpfnTextConvert != NULL)
						(*lpfnTextConvert)(strValue, GetText);
					varValue.Clear();
					varValue = strValue;
				}
				else
				{
					lLen = 0;
					varValue.Clear();
					varValue.vt = VT_EMPTY;
				}
			break;
		}
	case FLD_TYPE_DOUBLE: // возможно FLD_TYPE_BINARY
		{
			// если FLD_TYPE_DOUBLE
			if(pFieldRec->len_info.num_size.len == sizeof(DBF_DOUBLE))
				break;
		}
	case FLD_TYPE_GENERAL:
	case FLD_TYPE_PICTURE:
		{
			if(m_pData != NULL)	{ delete[] m_pData;	m_pData = NULL;	}
			if(varValue.ulVal > 0) // если есть запись ?мемо файл?
			{
				ReadMemoRecord(varValue.ulVal, lLen, &m_pData);
				varValue.vt = VT_BYREF|VT_UI1; 
				varValue.pbVal = m_pData;
			}
			else
			{
				lLen = 0;
				varValue.Clear();
				varValue.vt = VT_EMPTY;
			}
			break;
		}
	}
	return lLen;
}

/* Функция сохраняет значение по? */
void CDBFTableDef::SetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, const COleVariant& varValue,
						void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection))
{
	// если нужн?записываем ?мемо файл
	switch(pFieldRec->field_type)
	{
	case FLD_TYPE_MEMO:
		{
			COleVariant varDBFValue;
			if(varValue.vt == VT_BSTR)
			{
				CString strValue = varValue.bstrVal;

				DBF_CHAR*  pCurRec = new DBF_CHAR[pFieldRec->field_offset + pFieldRec->len_info.char_len];

				// получаем значение из таблиц?
				alxGetFieldValue(pRec, pFieldRec, varDBFValue, NULL);

				if(varDBFValue.vt == VT_UI4)
				{
					if(lpfnTextConvert != NULL && !(pFieldRec->field_flag & FLD_FLAG_BINARY))
						(*lpfnTextConvert)(strValue, SetText);

					DBF_LONG lLen = strValue.GetLength();
					WriteMemoRecord(varDBFValue.ulVal, lLen, strValue);
				}
				else if(varDBFValue.vt == VT_DECIMAL) // SIX
				{
					if(lpfnTextConvert != NULL)
						(*lpfnTextConvert)(strValue, SetText);

					DBF_LONG lLen = varDBFValue.decVal.Hi32 > 0 ? varDBFValue.decVal.Hi32 : strValue.GetLength();
					WriteMemoRecord(varDBFValue.ulVal, lLen, strValue);
					varDBFValue.decVal.Hi32 = lLen;
				}

				delete[] pCurRec;

				alxSetFieldValue(pRec, pFieldRec, varDBFValue, NULL);
			}
			break;
		}
	case FLD_TYPE_DOUBLE: // возможно FLD_TYPE_BINARY
		{
			// если FLD_TYPE_DOUBLE
			if(pFieldRec->len_info.num_size.len == sizeof(DBF_DOUBLE))
			{
				alxSetFieldValue(pRec, pFieldRec, varValue, NULL);
				break;
			}
		}
	case FLD_TYPE_GENERAL:
	case FLD_TYPE_PICTURE:
		{
			break;
		}
	default:
		alxSetFieldValue(pRec, pFieldRec, varValue, lpfnTextConvert);
	}
}

#ifdef _DEBUG
void CDBFTableDef::AssertValid() const
{
	CObject::AssertValid();
}

void CDBFTableDef::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CObject::Dump(dc);

	dc << "m_bOpen = " << m_bOpen;
	dc << "\nm_bNew = " << m_bNew;

	dc << "\n";
}
#endif //_DEBUG

BOOL CDBFTableDef::IsOpen() const
{
	ASSERT_VALID(this);
	return m_bOpen || m_bNew;
}

/* Создае?нову?таблиц?*/
BOOL CDBFTableDef::CreateTable(LPCTSTR lpszName)
{
	ASSERT_VALID(this);
	ASSERT(lpszName != NULL);
	ASSERT(!IsOpen());

	m_nOpenFlag = CFile::shareExclusive | CFile::modeReadWrite | CFile::typeBinary;

	if(m_DBFFields.m_pFields != NULL)
		delete[] m_DBFFields.m_pFields;
	m_DBFFields.m_pFields = NULL;

TRY
{
	// Создае?файл
	DBF_CHECK_ERROR(
		m_DBFile.Open(lpszName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive), ALX_DBF_ERROR_DBFILE_CREATE);

	COleDateTime curDate = COleDateTime::GetCurrentTime();
	memset(&m_DBFHead, 0, sizeof(DBF_HEAD));
	m_DBFHead.dbf_id = 0x03; // dBASE III // dBase IV
	m_DBFHead.last_update[0] = curDate.GetYear() - ALX_DBF_CENTURY;
	m_DBFHead.last_update[1] = curDate.GetMonth();
	m_DBFHead.last_update[2] = curDate.GetDay();
	m_DBFHead.last_rec = 0;
	m_DBFHead.data_offset = sizeof(DBF_HEAD) + 1;
	m_DBFHead.rec_size = 0;

	m_DBFFields.m_nFieldCount = 0;
	m_nEditMode = create;

	return TRUE;
}
CATCH(CFileException, e)
{

	e->ReportError();
	e->Delete();
	Close();
}
END_CATCH

	return FALSE;
}

/* Модификация таблиц?*/
void CDBFTableDef::ModifyTable()
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	// сохраняем текущи?заголово?таблиц?
	m_pOldDBFHead = new DBF_HEAD;
	memcpy(m_pOldDBFHead, &m_DBFHead, sizeof(DBF_HEAD));

	delete[] m_DBFFields.m_pCurRec;
	m_DBFFields.m_pCurRec = NULL;

	// сохраняем текущу?структур?поле?во временно?массив?
	m_pOldDBFFields = new CDBFFields;
	m_pOldDBFFields->m_pFields = new FIELD_REC[m_DBFFields.m_nFieldCount];
	memcpy(m_pOldDBFFields->m_pFields, m_DBFFields.m_pFields, sizeof(FIELD_REC) * m_DBFFields.m_nFieldCount);
	m_pOldDBFFields->m_nFieldCount = m_DBFFields.m_nFieldCount;
	m_pOldDBFFields->m_pCurRec = m_DBFFields.m_pCurRec;

	// выде?ем па?ть для масива соответствий старых ?новы?поле?
	m_pSwapFld = new DBF_INT[m_DBFFields.m_nFieldCount];
	for(short i = 0; i < m_DBFFields.m_nFieldCount; i++)
		m_pSwapFld[i] = i; // индекс нового по? соот? индекс?старог?

	COleDateTime curDate = COleDateTime::GetCurrentTime();
	m_DBFHead.last_update[0] = curDate.GetYear() - ALX_DBF_CENTURY;
	m_DBFHead.last_update[1] = curDate.GetMonth();
	m_DBFHead.last_update[2] = curDate.GetDay();

	m_nEditMode = modify;

	return;
}

/* Добавляем поле ?таблиц?*/
void CDBFTableDef::AddField(FIELD_REC *pFieldRec)
{
	ASSERT(m_nEditMode != noMode);

	if(m_DBFFields.m_nFieldCount > 0)
	{
		// временная област?хранен? поле?
		FIELD_REC*	pFields = new FIELD_REC[m_DBFFields.m_nFieldCount];
		// копируем список поле?
		memcpy(pFields, m_DBFFields.m_pFields, sizeof(FIELD_REC)*m_DBFFields.m_nFieldCount);
		// освобождае?па?ть
		delete[] m_DBFFields.m_pFields;
		// увеличивае?област?
		m_DBFFields.m_pFields = new FIELD_REC[m_DBFFields.m_nFieldCount+1];
		// копируем старые по?
		memcpy(m_DBFFields.m_pFields, pFields, sizeof(FIELD_REC)*m_DBFFields.m_nFieldCount);
		// копируем ново?поле
		memcpy(&m_DBFFields.m_pFields[m_DBFFields.m_nFieldCount], pFieldRec, sizeof(FIELD_REC));
		// получаем ново?количество поле?
		m_DBFFields.m_nFieldCount++;
		// освобождае?временну?па?ть
		delete[] pFields;
		// если модификация таблиц?
		if(m_nEditMode == modify) // изме?ем массив соответствий поле?
		{	// выде?ем временну?па?ть
			DBF_INT* pTmpSwapFld = new DBF_INT[m_DBFFields.m_nFieldCount-1];
			// сохраняем массив
			memcpy(pTmpSwapFld, m_pSwapFld, sizeof(DBF_INT)*(m_DBFFields.m_nFieldCount-1));
			delete[] m_pSwapFld;
			m_pSwapFld = new DBF_INT[m_DBFFields.m_nFieldCount];
			memcpy(m_pSwapFld, pTmpSwapFld, sizeof(DBF_INT)*(m_DBFFields.m_nFieldCount-1));
			// присваевае?значение новому элементу
			m_pSwapFld[m_DBFFields.m_nFieldCount-1] = -1; // не?по? ?старой структур?соот? новому полю
			delete[] pTmpSwapFld;
		}
	}
	else
	{
		// выде?ем па?ть
		m_DBFFields.m_pFields = new FIELD_REC[1];
		// копируем ново?поле
		memcpy(m_DBFFields.m_pFields, pFieldRec, sizeof(FIELD_REC));
		// получаем ново?количество поле?
		m_DBFFields.m_nFieldCount = 1;
		// если модификация таблиц?
		if(m_nEditMode == modify)
		{	// изме?ем массив соответствий поле?
			m_pSwapFld = new DBF_INT[1];
			m_pSwapFld[0] = -1;
		}
	}
}

/* Вставляем поле ?таблиц?*/
void CDBFTableDef::InsertField(short nIndex, FIELD_REC *pFieldRec)
{
	ASSERT(m_nEditMode != noMode);
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	if(m_DBFFields.m_nFieldCount > 0)
	{
		// временная област?хранен? поле?
		FIELD_REC*	pFields = new FIELD_REC[m_DBFFields.m_nFieldCount];
		// копируем список поле?
		memcpy(pFields, m_DBFFields.m_pFields, sizeof(FIELD_REC)*m_DBFFields.m_nFieldCount);
		// освобождае?па?ть
		delete[] m_DBFFields.m_pFields;
		// увеличивае?област?
		m_DBFFields.m_pFields = new FIELD_REC[m_DBFFields.m_nFieldCount+1];
		// копируем старые по?
		memcpy(m_DBFFields.m_pFields, pFields, sizeof(FIELD_REC)*nIndex);
		memcpy(&m_DBFFields.m_pFields[nIndex+1], &pFields[nIndex], sizeof(FIELD_REC)*(m_DBFFields.m_nFieldCount-nIndex));
		// копируем ново?поле
		memcpy(&m_DBFFields.m_pFields[nIndex], pFieldRec, sizeof(FIELD_REC));
		// получаем ново?количество поле?
		m_DBFFields.m_nFieldCount++;
		// освобождае?временну?па?ть
		delete[] pFields;
		// если модификация таблиц?
		if(m_nEditMode == modify) // изме?ем массив соответствий поле?
		{	// выде?ем временну?па?ть	
			DBF_INT* pTmpSwapFld = new DBF_INT[m_DBFFields.m_nFieldCount-1];
			// сохраняем массив
			memcpy(pTmpSwapFld, m_pSwapFld, sizeof(DBF_INT)*(m_DBFFields.m_nFieldCount-1));
			// освобождае?па?ть
			delete[] m_pSwapFld;
			// выде?ем па?ть нового размер?
			m_pSwapFld = new DBF_INT[m_DBFFields.m_nFieldCount];
			// востаннавливае?сохраненны?значен?
			memcpy(m_pSwapFld, pTmpSwapFld, sizeof(DBF_INT)*nIndex);
			memcpy(&m_pSwapFld[nIndex+1], &pTmpSwapFld[nIndex], sizeof(DBF_INT)*(m_DBFFields.m_nFieldCount-1-nIndex));
			m_pSwapFld[nIndex] = -1;
			delete[] pTmpSwapFld;
		}
	}
	else
	{
		// выде?ем па?ть
		m_DBFFields.m_pFields = new FIELD_REC[1];
		// копируем ново?поле
		memcpy(m_DBFFields.m_pFields, pFieldRec, sizeof(FIELD_REC));
		// получаем ново?количество поле?
		m_DBFFields.m_nFieldCount = 1;
		// если модификация таблиц?
		if(m_nEditMode == modify)
		{	// изме?ем массив соответствий поле?
			m_pSwapFld = new DBF_INT[1];
			m_pSwapFld[0] = -1;
		}
	}
}

/* Удаляем поле из таблиц?*/
void CDBFTableDef::DeleteField(short nIndex)
{
	ASSERT(m_nEditMode != noMode);
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	if(m_DBFFields.m_nFieldCount > 1)
	{
		// временная област?хранен? поле?
		FIELD_REC*	pFields = new FIELD_REC[m_DBFFields.m_nFieldCount];
		// копируем список поле?
		memcpy(pFields, m_DBFFields.m_pFields, sizeof(FIELD_REC)*m_DBFFields.m_nFieldCount);
		// освобождае?па?ть
		delete[] m_DBFFields.m_pFields;
		// уменшаем област?
		m_DBFFields.m_pFields = new FIELD_REC[m_DBFFields.m_nFieldCount-1];
		// копируем старые по?
		memcpy(m_DBFFields.m_pFields, pFields, sizeof(FIELD_REC)*nIndex);
		memcpy(&m_DBFFields.m_pFields[nIndex], &pFields[nIndex+1], sizeof(FIELD_REC)*(m_DBFFields.m_nFieldCount-nIndex-1));
		// получаем ново?количество поле?
		m_DBFFields.m_nFieldCount--;
		// освобождае?временну?па?ть
		delete[] pFields;
		// если модификация таблиц?
		if(m_nEditMode == modify) // изме?ем массив соответствий поле?
		{	// выде?ем временну?па?ть	
			DBF_INT* pTmpSwapFld = new DBF_INT[m_DBFFields.m_nFieldCount+1];
			// сохраняем массив
			memcpy(pTmpSwapFld, m_pSwapFld, sizeof(DBF_INT)*(m_DBFFields.m_nFieldCount+1));
			// освобождае?па?ть
			delete[] m_pSwapFld;
			// выде?ем па?ть нового размер?
			m_pSwapFld = new DBF_INT[m_DBFFields.m_nFieldCount];
			// востаннавливае?сохраненны?значен?
			memcpy(m_pSwapFld, pTmpSwapFld, sizeof(DBF_INT)*nIndex);
			memcpy(&m_pSwapFld[nIndex], &pTmpSwapFld[nIndex+1], sizeof(DBF_INT)*(m_DBFFields.m_nFieldCount+1-nIndex-1));
			delete[] pTmpSwapFld;
		}
	}
	else
	{
		// освобождае?па?ть
		delete[] m_DBFFields.m_pFields;
		m_DBFFields.m_pFields = NULL;
		// получаем ново?количество поле?
		m_DBFFields.m_nFieldCount = 0;
		// если модификация таблиц?
		if(m_nEditMode == modify)
		{	// изме?ем массив соответствий поле?
			delete[] m_pSwapFld;
			m_pSwapFld = NULL;
		}
	}
}

/* Изме?ем поле таблиц?*/
void CDBFTableDef::ModifyField(short nIndex, FIELD_REC *pFieldRec)
{
	ASSERT(m_nEditMode != noMode);
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);

	m_DBFFields.m_pFields[nIndex] = *pFieldRec;
}

/* Перемещаем поле таблиц?на нову?позици?*/
void CDBFTableDef::MoveField(short nIndex, short nNewIndex)
{
	ASSERT(m_nEditMode != noMode);
	ASSERT(nIndex < m_DBFFields.m_nFieldCount);
	ASSERT(nNewIndex < m_DBFFields.m_nFieldCount);

	while(nIndex > nNewIndex)
	{
		FIELD_REC FieldRec = m_DBFFields.m_pFields[nIndex];
		m_DBFFields.m_pFields[nIndex] = m_DBFFields.m_pFields[nIndex - 1];
		m_DBFFields.m_pFields[nIndex - 1] = FieldRec;

		if(m_nEditMode == modify)
		{
			DBF_INT iSwapFld = m_pSwapFld[nIndex];
			m_pSwapFld[nIndex] = m_pSwapFld[nIndex - 1];
			m_pSwapFld[nIndex - 1] = iSwapFld;
		}

		nIndex--;
	}
	while(nIndex < nNewIndex)
	{
		FIELD_REC FieldRec = m_DBFFields.m_pFields[nIndex];
		m_DBFFields.m_pFields[nIndex] = m_DBFFields.m_pFields[nIndex + 1];
		m_DBFFields.m_pFields[nIndex + 1] = FieldRec;

		if(m_nEditMode == modify)
		{
			DBF_INT iSwapFld = m_pSwapFld[nIndex];
			m_pSwapFld[nIndex] = m_pSwapFld[nIndex + 1];
			m_pSwapFld[nIndex + 1] = iSwapFld;
		}
		nIndex++;
	}

}

/* Отка?от изменени?сделанны??структур?таблиц?*/
void CDBFTableDef::CancelUpdateTable()
{
	ASSERT(m_nEditMode != noMode);

	if(m_nEditMode == modify)
	{
		memcpy(&m_DBFHead, m_pOldDBFHead, sizeof(DBF_HEAD));

		memcpy(m_DBFFields.m_pFields, m_pOldDBFFields->m_pFields, sizeof(FIELD_REC) * m_pOldDBFFields->m_nFieldCount);
		m_DBFFields.m_nFieldCount = m_pOldDBFFields->m_nFieldCount;
		m_DBFFields.m_pCurRec = m_pOldDBFFields->m_pCurRec;
	}
	else
	{
		if(m_DBFile.GetLength() == 0)
		{
			CString strFilePath = m_DBFile.GetFilePath();
			m_DBFile.Close();
			::DeleteFile(strFilePath);
		}
		else
			m_DBFile.Close();
	}

	if(m_pOldDBFHead != NULL)
	{
		delete[] m_pOldDBFHead;
		m_pOldDBFHead = NULL;
	}
	if(m_pOldDBFFields != NULL)
	{
		if(m_pOldDBFFields->m_pFields != NULL)
			delete[] m_pOldDBFFields->m_pFields;
		delete m_pOldDBFFields;
		m_pOldDBFFields = NULL;
	}
	if(m_pSwapFld != NULL)
	{
		delete[] m_pSwapFld;
		m_pSwapFld = NULL;
	}

	m_DBFFields.m_pCurRec = new DBF_CHAR[m_DBFHead.rec_size];
	memset(m_DBFFields.m_pCurRec, 0, m_DBFHead.rec_size);
}

/* Сохраняем изменения ?таблиц?*/
void CDBFTableDef::UpdateTable()
{
	ASSERT_VALID(this);
	ASSERT(m_nEditMode != noMode);
	ASSERT(m_DBFFields.m_nFieldCount > 0);

	BOOL bMemoExist = FALSE;
	UINT nMemoType = MEMO_TYPE_NOMEMO;

	DBF_LONG field_offset = 1;
	// перебираем вс?по?
	for(short j = 0; j < m_DBFFields.GetFieldCount(); j++)
	{
		m_DBFFields.m_pFields[j].field_offset = field_offset;
		// вычисляем смещение до следующего по?
		switch(m_DBFFields.m_pFields[j].field_type)
		{
		case FLD_TYPE_NUMERIC:
		case FLD_TYPE_FLOAT:
			field_offset = field_offset + m_DBFFields.m_pFields[j].len_info.num_size.len;
			break;
		case FLD_TYPE_CURRENCY:
		case FLD_TYPE_DATETIME:
		case FLD_TYPE_INTEGER:
				nMemoType = MEMO_TYPE_VFOXPRO;
				m_DBFFields.m_pFields[j].field_flag |= FLD_FLAG_BINARY;
				field_offset = field_offset + m_DBFFields.m_pFields[j].len_info.num_size.len;
				break;
		case FLD_TYPE_DOUBLE: // or FLD_TYPE_BINARY
			if(m_DBFFields.m_pFields[j].len_info.num_size.len != sizeof(double))
			{
				nMemoType = MEMO_TYPE_DBASEIV;
				field_offset = field_offset + m_DBFFields.m_pFields[j].len_info.char_len;
			}
			else
			{
				nMemoType = MEMO_TYPE_VFOXPRO;
				m_DBFFields.m_pFields[j].field_flag |= FLD_FLAG_BINARY;
				field_offset = field_offset + m_DBFFields.m_pFields[j].len_info.num_size.len;
			}
			break;
		case FLD_TYPE_MEMO:
			bMemoExist = TRUE;
			field_offset = field_offset + m_DBFFields.m_pFields[j].len_info.char_len;
			break;
		case FLD_TYPE_NULLFLAGS:
			m_DBFFields.m_pFields[j].field_flag |= FLD_FLAG_SYSTEM;
		case FLD_TYPE_GENERAL:
			bMemoExist = TRUE;
			nMemoType = MEMO_TYPE_VFOXPRO;
			m_DBFFields.m_pFields[j].field_flag |= FLD_FLAG_BINARY;
			field_offset = field_offset + m_DBFFields.m_pFields[j].len_info.char_len;
			break;
		case FLD_TYPE_PICTURE:
			bMemoExist = TRUE;
			if(nMemoType != MEMO_TYPE_VFOXPRO) nMemoType = MEMO_TYPE_FOXPRO;
			field_offset = field_offset + m_DBFFields.m_pFields[j].len_info.char_len;
			break;
		default:
			field_offset = field_offset + m_DBFFields.m_pFields[j].len_info.char_len;
		}
	}

	// опреде?ем формат таблиц?- id ?ти?мемо по? (если есть)
	if(nMemoType == MEMO_TYPE_VFOXPRO || m_DBFHead.dbf_id == 0x30)
	{
		m_DBFHead.dbf_id = 0x30;

		if(bMemoExist)
			m_DBFHead.table_flag |= TBL_FLAG_MEMO;

		if(m_DBFHead.table_flag & TBL_FLAG_DATABASE)
			nMemoType = MEMO_TYPE_FOX_DATABASE;
		else
			nMemoType = MEMO_TYPE_VFOXPRO;

	}
	else if(bMemoExist)
	{
		switch(m_DBFHead.dbf_id)
		{
		case 0x03: m_DBFHead.dbf_id = 0x83;	nMemoType = MEMO_TYPE_DBASE;	break;
		case 0x43: m_DBFHead.dbf_id = 0x8B; nMemoType = MEMO_TYPE_DBASEIV;	break;
		case 0x63: m_DBFHead.dbf_id = 0xCB; nMemoType = MEMO_TYPE_DBASEIV;	break;
		case 0x8B: case 0x7B: case 0xCB:	nMemoType = MEMO_TYPE_DBASEIV;	break;
		case 0xE5:							nMemoType = MEMO_TYPE_SIX;		break;
		case 0xF5:							nMemoType = MEMO_TYPE_FOXPRO;	break;
		default:
			nMemoType = MEMO_TYPE_DBASE;
		}
	}
	else
	{
		switch(m_DBFHead.dbf_id)
		{
		case 0x83: m_DBFHead.dbf_id = 0x03; break;
		case 0x8B: m_DBFHead.dbf_id = 0x43; break;
		case 0xCB: m_DBFHead.dbf_id = 0x63; break;
		}
	}

	// вычисляем смещение до данных
	m_DBFHead.data_offset = sizeof(DBF_HEAD) + sizeof(FIELD_REC) * m_DBFFields.GetFieldCount() + 1;
	if(m_DBFHead.dbf_id == 0x30)
		m_DBFHead.data_offset += 263;

	// запоминаем размер по?
	m_DBFHead.rec_size = (DBF_UINT)field_offset;

	CFile *pFileNew, *pFileOld;
	CMemoFile *pMemoFileNew = NULL, *pMemoFileOld = NULL;

	// если модификация
	if(m_nEditMode == modify)
	{
		// создае?временны?файл

		pFileNew = new CFile;
		pFileOld = &m_DBFile;

		// Получаем имя временного файл?
		CString strTmpPath, strTmpMemoPath;
		strTmpPath = alxChangeFileExt(m_DBFile.GetFilePath(), _T("_tmp"));
		strTmpMemoPath = alxChangeFileExt(m_DBFile.GetFilePath(), _T("_tmp_"));

		CFileFind mFileFind; // удаляем старый временны?файл, если тако?есть
		if(mFileFind.FindFile(strTmpPath))
			CFile::Remove(strTmpPath);
		if(mFileFind.FindFile(strTmpMemoPath))
			CFile::Remove(strTmpMemoPath);

		// Создае?временны?файл
		DBF_CHECK_ERROR(
			pFileNew->Open(strTmpPath, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive), ALX_DBF_ERROR_DBFILE_CREATE);

		// если есть мемо по?
		if(bMemoExist)
		{
			pMemoFileNew = alxNewMemoFile(nMemoType);
			// создае?мемо файл
			m_pMemoFile->Create(strTmpMemoPath);
		}

		pMemoFileOld = m_pMemoFile;
	}
	// если создае?новы?файл
	else
	{
		pFileNew = &m_DBFile;

		// если есть мемо по?
		if(bMemoExist)
		{
			m_pMemoFile = alxNewMemoFile(nMemoType);
			CString strMemoFileName = alxChangeFileExt(pFileNew->GetFilePath(), alxMemoFileExt(nMemoType));
			// создае?мемо файл
			m_pMemoFile->Create(strMemoFileName);
		}
		pMemoFileNew = m_pMemoFile;
	}


	// записываем заголово?таблиц?
	pFileNew->Write(&m_DBFHead, sizeof(m_DBFHead));
	// записываем список поле?таблиц?
    int j;
	for(j = 0; j < m_DBFFields.GetFieldCount(); j++)
		pFileNew->Write(&m_DBFFields.m_pFields[j], sizeof(FIELD_REC));
	// размер пространства межд?заголовком ?данным?
	int nEmptySpace = m_DBFHead.data_offset - (sizeof(m_DBFHead) + sizeof(FIELD_REC) * m_DBFFields.GetFieldCount());
	if(nEmptySpace > 0)
	{
		DBF_CHAR* fill = new DBF_CHAR[nEmptySpace];
		memset(fill,'\0',nEmptySpace); // заполняем ну?ми
		fill[0] = 0x0D; // призна?завершен?
		pFileNew->Write(fill, nEmptySpace); // записываем
		delete[] fill;
	}

	// если модификация
	if(m_nEditMode == modify)
	{
		// переноси?данные ?нову?структур?

		DBF_CHAR* pOldRec = new DBF_CHAR[m_pOldDBFHead->rec_size];
		DBF_CHAR* pNewRec = new DBF_CHAR[m_DBFHead.rec_size];
		pFileOld->Seek(m_pOldDBFHead->data_offset, CFile::begin);
		pFileNew->Seek(m_DBFHead.data_offset, CFile::begin);
		// экспор?записе?
		for(long i = 0; i < m_pOldDBFHead->last_rec; i++)
		{
			pFileOld->Read(pOldRec, m_pOldDBFHead->rec_size);
			pNewRec[0] = pOldRec[0];
			// перебираем по?
			for(int j = 0; j < m_DBFFields.GetFieldCount(); j++)
			{
				COleVariant varValue;
				if(m_pSwapFld[j] >= 0)
					GetFieldValue(pOldRec, m_pOldDBFFields->GetFieldRec(m_pSwapFld[j]), varValue, NULL);
				SetFieldValue(pNewRec, m_DBFFields.GetFieldRec(j), varValue, NULL);
			}
			pFileNew->Write(pNewRec, m_DBFHead.rec_size);
		}
		delete[] pOldRec;
		delete[] pNewRec;

		delete[] m_pOldDBFHead;
		m_pOldDBFHead = NULL;
		delete[] m_pOldDBFFields->m_pFields;
		delete m_pOldDBFFields;
		m_pOldDBFFields = NULL;
		delete[] m_pSwapFld;
		m_pSwapFld = NULL;
  }

	// записываем призна?конц?
	char eof = REC_EOF;
	pFileNew->Write(&eof, 1);

	// измением размер файл?
	pFileNew->SetLength(m_DBFHead.data_offset + m_DBFHead.last_rec * m_DBFHead.rec_size + 1);

	// если модификация
	if(m_nEditMode == modify)
	{
		// переименовывае?созданны?файл ?те?, ?те? ?.bak

		CString strNewPath = pFileNew->GetFilePath(); 
		CString strOldPath = pFileOld->GetFilePath(); 
		CString strBakPath = alxChangeFileExt(pFileOld->GetFilePath(), _T("bak"));

		CString strNewMemoPath;
		CString strOldMemoPath;
		CString strBakMemoPath = alxChangeFileExt(pFileOld->GetFilePath(), _T("tbk"));

		if(pMemoFileNew != NULL)
		{
			strNewMemoPath = pMemoFileNew->GetFilePath();
			pMemoFileNew->Close();
		}
		if(pMemoFileOld != NULL)
		{
			strOldMemoPath = pMemoFileOld->GetFilePath();
			pMemoFileOld->Close();
			delete pMemoFileOld;
		}

		pFileOld->Close();
		pFileNew->Close();

		delete pFileNew;
		
		CFileFind mFileFind;
		if(mFileFind.FindFile(strBakPath))
			CFile::Remove(strBakPath);
		if(mFileFind.FindFile(strBakMemoPath))
			CFile::Remove(strBakMemoPath);
		CFile::Rename(strOldPath, strBakPath);
		if(!strOldMemoPath.IsEmpty())
			CFile::Rename(strOldMemoPath, strBakMemoPath);
		CFile::Rename(strNewPath, strOldPath);
		if(!strNewMemoPath.IsEmpty())
			CFile::Rename(strNewMemoPath, strOldMemoPath);

		// Открывае?файл
		DBF_CHECK_ERROR(
			m_DBFile.Open(strOldPath, m_nOpenFlag), ALX_DBF_ERROR_DBFILE_OPEN);

		// Если нужн? открывае?мемо файл.
		if(nMemoType)
		{
			m_pMemoFile = pMemoFileNew;
			CString strMemoFileName = alxChangeFileExt(strOldPath, alxMemoFileExt(nMemoType));
			// Открывае?мемо файл
			m_pMemoFile->Open(strMemoFileName, m_nOpenFlag);
		}

		m_bOpen = TRUE;
	}
	else
		m_bNew = TRUE;

	m_DBFFields.m_pCurRec = new DBF_CHAR[m_DBFHead.rec_size];
	memset(m_DBFFields.m_pCurRec, 0, m_DBFHead.rec_size);

	m_nEditMode = noMode;
}

/* Функция возвращает значение по? из буфера записи */
long alxGetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, COleVariant& varValue,
						void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection))
{
	// Clear out variant
	varValue.Clear();
	varValue.vt = VT_EMPTY;

	long lLen;
	long nOffset = pFieldRec->field_offset;
	// ?зависимост?от типа по?
	switch(pFieldRec->field_type)
	{
	case FLD_TYPE_CHARACTER:
		{
			CString strValue;
			lLen = pFieldRec->len_info.char_len;
			// Выде?ем па?ть для по?
			char* buffer = strValue.GetBuffer(lLen + 1);
			// Копируем содержимое по? ?буфе?
			memcpy(buffer, &pRec[nOffset], lLen);
			buffer[lLen] = '\0';
			strValue.ReleaseBuffer(lLen + 1);
			if(!(pFieldRec->field_flag & FLD_FLAG_BINARY))
			{
				strValue.TrimRight();
				if(lpfnTextConvert != NULL)
					(*lpfnTextConvert)(strValue, GetText);
			}
			varValue = strValue;
			break;
		}
	case FLD_TYPE_NUMERIC:
	case FLD_TYPE_FLOAT:
		{
			lLen = pFieldRec->len_info.num_size.len;
			// Выде?ем па?ть для по?
			char* buffer = new char[ lLen + 1 ];
			// Копируем содержимое по? ?буфе?
			memcpy(buffer, &pRec[nOffset], lLen);
			buffer[lLen] = '\0';
			varValue.dblVal = atof(buffer);
			varValue.vt = VT_R8;
			delete[] buffer;
			break;
		}
	case FLD_TYPE_DATE:
		{
			unsigned int nYear;
			unsigned char bMonth, bDay;
			lLen = pFieldRec->len_info.char_len;
			// Выде?ем па?ть для по?
			char* buffer = new char[ 5 ];

			memcpy(buffer, &pRec[nOffset], 4);
			buffer[4] = '\0';
			nYear = atoi(buffer);
			memcpy(buffer, &pRec[nOffset+4], 2);
			buffer[2] = '\0';
			bMonth = atoi(buffer);
			memcpy(buffer, &pRec[nOffset+6], 2);
			buffer[2] = '\0';
			bDay = atoi(buffer);

			delete[] buffer;

			COleDateTime dteValue(nYear,bMonth,bDay,0,0,0);
			if(dteValue.GetStatus() == COleDateTime::valid)
				varValue = dteValue;
			break;
		}
	case FLD_TYPE_LOGICAL:
		{
			DBF_UCHAR chValue;
			lLen = pFieldRec->len_info.char_len;
			memcpy(&chValue, &pRec[nOffset], sizeof(DBF_UCHAR));
			if(chValue == 'T' || chValue == 't' ||chValue == 'Y' || chValue == 'y')
				varValue.boolVal = TRUE;
			else
				varValue.boolVal = FALSE;
			varValue.vt = VT_BOOL;
			break;
		}
	case FLD_TYPE_CURRENCY:
		{
			lLen = pFieldRec->len_info.num_size.len;
			memcpy(&varValue.cyVal, &pRec[nOffset], sizeof(varValue.cyVal));
			varValue.vt = VT_CY;
			break;
		}
	case FLD_TYPE_DATETIME:
		{
			COleDateTime dteValue(1,1,1,0,0,0);
			lLen = pFieldRec->len_info.char_len;
			long lDay,lHour, lMin, lSec;
			memcpy(&lDay, &pRec[nOffset], sizeof(lDay));
			memcpy(&lSec, &pRec[nOffset+4], sizeof(lSec));
			if(lDay >= 0x1A4452)
			{
				lDay = lDay - 0x1A4452;
				lSec = (lSec + 1) / 1000;
				lHour = lSec / 60 / 60;
				lSec = lSec - lHour * 60 * 60;
				lMin = lSec / 60;
				lSec = lSec - lMin * 60;

				COleDateTimeSpan dteSpan(lDay,lHour,lMin,lSec);
				dteValue = dteValue + dteSpan;
				varValue = dteValue;
			}
			break;
		}
	case FLD_TYPE_INTEGER:
		{
			lLen = pFieldRec->len_info.num_size.len;
			memcpy(&varValue.lVal, &pRec[nOffset], sizeof(varValue.lVal));
			varValue.vt = VT_I4;
			break;
		}
	case FLD_TYPE_DOUBLE: // возможно FLD_TYPE_BINARY
		{
			lLen = pFieldRec->len_info.num_size.len;
			// если FLD_TYPE_DOUBLE
			if(lLen == sizeof(DBF_DOUBLE))
			{
				memcpy(&varValue.dblVal, &pRec[nOffset], sizeof(varValue.dblVal));
				varValue.vt = VT_R8;
				break;
			}
		}
	case FLD_TYPE_MEMO:
		{
			lLen = pFieldRec->len_info.num_size.len;
			// если адре?записа??систем?SIX (текстово?мемо поле 0x0800)
			if(lLen == 10 && pRec[nOffset] > 0x00 && pRec[nOffset] <= 0x08 && pRec[nOffset+1] == 0x00)
			{
				memcpy(&varValue.decVal.Hi32, &pRec[nOffset+2], sizeof(varValue.decVal.Hi32));
				memcpy(&varValue.decVal.Lo32, &pRec[nOffset+6], sizeof(varValue.decVal.Lo32));
				varValue.vt = VT_DECIMAL;
				break;
			}
		}
	case FLD_TYPE_GENERAL:
	case FLD_TYPE_PICTURE:
		{
			lLen = pFieldRec->len_info.num_size.len;
			if(lLen == sizeof(DBF_ULONG))
				memcpy(&varValue.ulVal, &pRec[nOffset], sizeof(varValue.ulVal));
			else if(lLen == 10)
			{
				char* buffer = new char[11];
				memcpy(buffer, &pRec[nOffset], lLen);
				buffer[lLen] = '\0';
				varValue.ulVal = atol(buffer);
				delete[] buffer;
			}
			varValue.vt = VT_UI4;
			break;
		}
	case FLD_TYPE_NULLFLAGS:
		{
			lLen = pFieldRec->len_info.num_size.len;
			varValue.ulVal = 0;
			memcpy(&varValue.ulVal, &pRec[nOffset], lLen);
			varValue.vt = VT_UI4;
			break;
		}
	default:
		{
			CString strValue;
			lLen = pFieldRec->len_info.num_size.len;
			// Выде?ем па?ть для по?
			char* buffer = strValue.GetBuffer(lLen + 1);
			// Копируем содержимое по? ?буфе?
			memcpy(buffer, &pRec[nOffset], lLen);
			buffer[lLen] = '\0';
			strValue.ReleaseBuffer(lLen + 1);

			varValue = strValue;
		}
	}

	return lLen;
}

/* Функция записывает значение по? ?буфе?записи */
void alxSetFieldValue(DBF_CHAR* pRec, FIELD_REC* pFieldRec, const COleVariant& var,
						void (FAR * lpfnTextConvert) (CString& strConvertText, TextConvertDirection enDirection))
{
	long lLen;
	long nOffset = pFieldRec->field_offset;
	COleVariant varValue = var;
	// ?зависимост?от типа по?
	switch(pFieldRec->field_type)
	{
	case FLD_TYPE_CHARACTER:
		{
			lLen = pFieldRec->len_info.char_len;
			memset(&pRec[nOffset],' ',lLen);
			if(varValue.vt != VT_NULL)
				varValue.ChangeType(VT_BSTR);
			// ?зависимост?от типа значен?
			if(varValue.vt == VT_BSTR)
			{
				CString strValue = varValue.bstrVal;
				if(lpfnTextConvert != NULL && !(pFieldRec->field_flag & FLD_FLAG_BINARY))
					(*lpfnTextConvert)(strValue, SetText);
				strncpy(&pRec[nOffset], strValue, min(strValue.GetLength(),lLen));
			}
			break;
		}
	case FLD_TYPE_NUMERIC:
	case FLD_TYPE_FLOAT:
		{
			CString strValue,strFormat;
			lLen = pFieldRec->len_info.num_size.len;
			DBF_CHAR bDec = pFieldRec->len_info.num_size.dec;
			memset(&pRec[nOffset],' ',lLen);
			if(varValue.vt != VT_NULL)
				varValue.ChangeType(VT_R8);
			// ?зависимост?от типа значен?
			if(varValue.vt == VT_R8)
			{
				CString strFormat;
				strFormat.Format("%s.%uf","%",bDec);
				strValue.Format(strFormat,varValue.dblVal);
			}

			if(strValue.GetLength() > lLen)
			{
				if(strValue.GetAt(lLen) == '.')
					strValue = ' ' + strValue;
				strValue = strValue.Left(lLen);
			}

			strncpy(&pRec[nOffset + (lLen - strValue.GetLength())], strValue, strValue.GetLength());

			break;
		}
	case FLD_TYPE_DATE:
		{
			lLen = pFieldRec->len_info.char_len;
			COleDateTime dte;
			if(varValue.vt != VT_DATE)
				varValue.ChangeType(VT_BSTR);
			// ?зависимост?от типа значен?
			switch(varValue.vt)
			{
			case VT_BSTR:
				{
					CString strValue = varValue.bstrVal;
					if(!dte.ParseDateTime(strValue))
					{
						memset(&pRec[nOffset],' ',lLen);
						break;
					}
				}
			case VT_DATE:
				{
					memset(&pRec[nOffset],'0',lLen);
					dte.m_dt = varValue.date;
					CString strValue;
					strValue.Format("%d",dte.GetYear());
					strncpy(&pRec[nOffset + 4 - strValue.GetLength()], strValue, strValue.GetLength());
					strValue.Format("%d",dte.GetMonth());
					strncpy(&pRec[nOffset + 6 - strValue.GetLength()], strValue, strValue.GetLength());
					strValue.Format("%d",dte.GetDay());
					strncpy(&pRec[nOffset + 8 - strValue.GetLength()], strValue, strValue.GetLength());
					break;
				}
			default:
				memset(&pRec[nOffset],' ',lLen);
			}
			break;
		}
	case FLD_TYPE_DATETIME:
		{
			lLen = pFieldRec->len_info.char_len;
			COleDateTime dte;
			if(varValue.vt != VT_DATE)
				varValue.ChangeType(VT_BSTR);
			// ?зависимост?от типа значен?
			switch(varValue.vt)
			{
			case VT_BSTR:
				{
					CString strValue = varValue.bstrVal;
					if(!dte.ParseDateTime(strValue))
					{
						memset(&pRec[nOffset],' ',lLen);
						break;
					}
				 }
			case VT_DATE:
				{
					dte.m_dt = varValue.date;
					long lDay = ((long)dte.m_dt) + 0x1A4452 + 693959 - 366;
					memcpy(&pRec[nOffset], &lDay, lLen/2);
					long lSec = (dte.GetHour()* 60 * 60 + dte.GetMinute() * 60 + dte.GetSecond()) * 1000;
					memcpy(&pRec[nOffset + lLen/2], &lSec, lLen/2);
					break;
				}
			default:
				memset(&pRec[nOffset],0,lLen);
			}
			break;
		}
	case FLD_TYPE_LOGICAL:
		{
			lLen = pFieldRec->len_info.char_len;
			if(varValue.vt != VT_NULL)
				varValue.ChangeType(VT_BOOL);
			// ?зависимост?от типа значен?
			switch(varValue.vt)
			{
			case VT_BOOL:
				{
					CString strValue = varValue.boolVal == TRUE ? "T" : "F";
					strncpy(&pRec[nOffset], strValue, lLen);
					break;
				}
			default:
				memset(&pRec[nOffset],' ',lLen);
			}
			break;
		}
	case FLD_TYPE_CURRENCY:
		{
			lLen = pFieldRec->len_info.num_size.len;
			if(varValue.vt != VT_NULL)
				varValue.ChangeType(VT_CY);
			// ?зависимост?от типа значен?
			switch(varValue.vt)
			{
			case VT_CY:
				memcpy(&pRec[nOffset], &varValue.cyVal, lLen);
				break;
			default:
				memset(&pRec[nOffset],0,lLen);
			}
			break;
		}
	case FLD_TYPE_INTEGER:
		{
			// ?зависимост?от типа значен?
			lLen = pFieldRec->len_info.num_size.len;
			if(varValue.vt != VT_NULL)
				varValue.ChangeType(VT_I4);
			switch(varValue.vt)
			{
			case VT_I4:
				memcpy(&pRec[nOffset], &varValue.lVal, lLen);
				break;
			default:
				memset(&pRec[nOffset],0,lLen);
			}
			break;
		}
	case FLD_TYPE_DOUBLE: // возможно FLD_TYPE_BINARY
		{
			lLen = pFieldRec->len_info.num_size.len;
			// если FLD_TYPE_DOUBLE
			if(lLen == sizeof(DBF_DOUBLE))
			{
				if(varValue.vt != VT_NULL)
					varValue.ChangeType(VT_R8);
				// ?зависимост?от типа значен?
				switch(varValue.vt)
				{
				case VT_R8:
					memcpy(&pRec[nOffset], &varValue.dblVal, lLen);
					break;
				default:
					memset(&pRec[nOffset],0,lLen);
				}
			}
			break;
		}
	case FLD_TYPE_NULLFLAGS:
		{
			lLen = pFieldRec->len_info.num_size.len;
			// ?зависимост?от типа значен?
			switch(varValue.vt)
			{
			case VT_UI4:
				memcpy(&pRec[nOffset], &varValue.ulVal, lLen);
				break;
			default:
				memset(&pRec[nOffset],0,lLen);
			}
			break;
		}
	case FLD_TYPE_MEMO:
		{
			lLen = pFieldRec->len_info.num_size.len;
			if(varValue.vt == VT_UI4)
			{
				if(lLen == sizeof(DBF_ULONG))
					memcpy(&pRec[nOffset], &varValue.ulVal, lLen);
				else if(lLen == 10)
				{
					// заполняем пробелам?
					memset(&pRec[nOffset],' ',lLen);
					if(varValue.ulVal > 0) // есть запись
					{
						CString strOffsetRec;
						strOffsetRec.Format("%u",varValue.ulVal);
						memcpy(&pRec[nOffset + (lLen - strOffsetRec.GetLength())], (LPCSTR)strOffsetRec, lLen);
					}

				}
			}
			else if (varValue.vt == VT_DECIMAL && lLen == 10) // SIX
			{
				if(varValue.decVal.Lo32 > 0) // есть запись
					pRec[nOffset] = 0x08;
				else
				{
					pRec[nOffset] = 0x00;
					varValue.decVal.Hi32 = varValue.decVal.Lo32 = 0;
				}
				pRec[nOffset+1] = 0x00;
				memcpy(&pRec[nOffset+2],&varValue.decVal.Hi32, sizeof(varValue.decVal.Hi32));
				memcpy(&pRec[nOffset+6],&varValue.decVal.Lo32, sizeof(varValue.decVal.Lo32));
			}
			break;
		}
	}
}


BOOL CDBFTableDef::ChangeOpenFlag(UINT nNewFlag)
{
	ASSERT_VALID(this);
	ASSERT(IsOpen());

	// открывае?ка?бинарный файл
	nNewFlag = nNewFlag | CFile::typeBinary;

	if(m_nOpenFlag == nNewFlag)
		return TRUE;

	if(m_pMemoFile != NULL && m_pMemoFile->IsOpen())
	{
		CString strMemoFileName = m_pMemoFile->GetFilePath();
		m_pMemoFile->CFile::Close();
		// Открывае?файл
		if(!m_pMemoFile->CFile::Open(strMemoFileName,nNewFlag))
		{
			// Открывае?файл
			DBF_CHECK_ERROR(
				m_pMemoFile->CFile::Open(strMemoFileName, m_nOpenFlag), ALX_DBF_ERROR_MEMO_OPEN);
			return FALSE;
		}
	}

	CFile* pDBFile = (CFile*) &m_DBFile;
	CString strDBFileName = pDBFile->GetFilePath();
	pDBFile->Close();
	// Открывае?файл
	if(!pDBFile->Open(strDBFileName,nNewFlag))
	{
		// Открывае?файл
		DBF_CHECK_ERROR(
			pDBFile->Open(strDBFileName, m_nOpenFlag), ALX_DBF_ERROR_DBFILE_OPEN);
		return FALSE;
	}

	m_nOpenFlag = nNewFlag;
	return TRUE;
}
