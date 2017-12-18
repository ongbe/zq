#include "DB_OCCI.h"

#include "occi.h"
#include "FileOpr.h"

#ifdef _DEBUG
	#pragma comment(lib, "oraocci11d.lib") 
#else
	#pragma comment(lib, "oraocci11.lib") 
#endif
#pragma comment(lib, "comsuppw.lib") 

using namespace oracle::occi;

CDB_OCCI::CDB_OCCI(const string& username,const string& pw,const string& connstr)
:	m_pEnv(NULL)
	,m_pConn(NULL)
	,m_username(username)
	,m_pw(pw)
	,m_connstr(connstr)
	,m_TryInitTime(0)
	,m_ReleaseTime(0)
	,m_TryConnTime(0)
	,m_DisconnTime(0)
{
}

CDB_OCCI::~CDB_OCCI(void)
{
	Release();
}

void CDB_OCCI::Init(void)
{
	CeasymutexGuard guard(m_mutex);

	if(m_pEnv) 
		Release();

	m_TryInitTime.setcurtime();
	try
	{
		m_pEnv = Environment::createEnvironment(Environment::DEFAULT);

		CFileOpr::getObj().writelocallog("DB_OCCI","Init : createEnvironment(Environment::DEFAULT) success: m_pEnv:[%x]",m_pEnv);
	}
	catch (SQLException & oraex)
	{
		CFileOpr::getObj().writelocallog("DB_OCCI","Init : createEnvironment(Environment::DEFAULT) failure: errmsg:[%s]",oraex.what()==NULL?"bad ptr":oraex.what());
		m_pEnv=NULL;
	}
}

void CDB_OCCI::Release()
{
	CeasymutexGuard guard(m_mutex);

	if(m_pConn)
		Disconn_Internal();

	if(!m_pEnv)
		CFileOpr::getObj().writelocallog("DB_OCCI","Release : no action as m_pEnv==NULL");
	else
	{
		m_ReleaseTime.setcurtime();
		try
		{
			Environment::terminateEnvironment((Environment*)m_pEnv);
			CFileOpr::getObj().writelocallog("DB_OCCI","Release : terminateEnvironment(m_pEnv=%x) success",m_pEnv);
			m_pEnv = NULL;
		}
		catch (SQLException & oraex)
		{
			CFileOpr::getObj().writelocallog("DB_OCCI","Release : terminateEnvironment(m_pEnv=%x) failure: errmsg:[%s]",m_pEnv,oraex.what()==NULL?"bad ptr":oraex.what());
			m_pEnv = NULL;
		}
	}
}

void CDB_OCCI::Connect(void)
{
	CeasymutexGuard guard(m_mutex);
	Connect_Internal();
}
void CDB_OCCI::Disconn(void)
{
	CeasymutexGuard guard(m_mutex);
	Disconn_Internal();
}
void CDB_OCCI::Connect_Internal(void)
{
	if(!m_pEnv)
	{
		CFileOpr::getObj().writelocallog("DB_OCCI","Connect_Internal : no action as m_pEnv==NULL");
	}
	else
	{
		m_TryConnTime.setcurtime();
		try
		{
			CFileOpr::getObj().writelocallog("DB_OCCI","Connect_Internal : 开始连接occi：createConnection(username=%s, pw=%s, connstr=%s) success",m_username.c_str(),m_pw.c_str(),m_connstr.c_str());
			m_pConn = ((Environment*)m_pEnv)->createConnection(m_username, m_pw, m_connstr);
			CFileOpr::getObj().writelocallog("DB_OCCI","Connect_Internal : 完成连接occi：createConnection(username=%s, pw=%s, connstr=%s) success",m_username.c_str(),m_pw.c_str(),m_connstr.c_str());
		}
		catch (SQLException & oraex)
		{
			CFileOpr::getObj().writelocallog("DB_OCCI","Connect_Internal : 连接occi失败：createConnection(username=%s, pw=%s, connstr=%s) failure: errmsg:[%s]",m_username.c_str(),m_pw.c_str(),m_connstr.c_str(),oraex.what()==NULL?"bad ptr":oraex.what());
		}
	}
}
void CDB_OCCI::Disconn_Internal(void)
{
	if(!m_pEnv)
	{
		CFileOpr::getObj().writelocallog("DB_OCCI","Disconn_Interanl : no action as m_pEnv==NULL");
	}
	else
	{
		m_DisconnTime.setcurtime();
		try
		{
			((Environment*)m_pEnv)->terminateConnection((Connection*)m_pConn);
			CFileOpr::getObj().writelocallog("DB_OCCI","Disconn_Internal : terminateConnection() success: m_pEnv:[%x] m_pConn[%x]=NULL",m_pEnv,m_pConn);
			m_pConn=NULL;
		}
		catch (SQLException & oraex)
		{
			CFileOpr::getObj().writelocallog("DB_OCCI","Disconn : terminateConnection() failure: m_pEnv:[%x] m_pConn:[%x] errmsg:[%s]",m_pEnv,m_pConn,oraex.what()==NULL?"bad ptr":oraex.what());
			m_pConn=NULL;
		}
	}
}
//use [select count(*) from user_tables] to detect status of connection
void CDB_OCCI::CheckConn_Internal(void)
{
}

/**
* @功能描述: 执行查询SQL语句，并返回结果集
* @参数列表: pSql :查询SQL语句
* @参数列表: vec :结果集
* @参数列表: nErrorCode :错误码
* @返 回 值: true,执行成功 false,执行失败
**/
bool CDB_OCCI::QueryData( const char* pSql, std::vector<std::vector<_variant_t>>& rtnvec, int& ErrCode, string& ErrMsg )
{
	CeasymutexGuard guard(m_mutex);

	ErrCode=0;
	ErrMsg="";
	bool brlt=false;

	if(!CheckParam_Init_Internal(pSql, ErrCode, ErrMsg )) 
		return false;

	Statement* pStmt=NULL;
	ResultSet* pRes=NULL;
	try
    {
        pStmt = ((Connection*)m_pConn)->createStatement();
		if(!pStmt)
		{ 
			ErrCode=-1;  
			ErrMsg="createStatement失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","QueryData : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			return false; 
		}

		pRes = pStmt->executeQuery( pSql );
		if(!pRes)
		{ 
			ErrCode=-1;  
			ErrMsg="executeQuery失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","QueryData : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			((Connection*)m_pConn)->terminateStatement(pStmt);
			return false; 
		}

        vector<MetaData> vMetaData = pRes->getColumnListMetaData();
        while ( pRes->next())
        {
            vector<_variant_t> vColumn;
            for(size_t i = 0; i < vMetaData.size(); i++)
            {
                _variant_t var;
                MetaData data = vMetaData[i];
                int nType = data.getInt(MetaData::ATTR_DATA_TYPE);
                switch(nType)
                {
					case OCCIIBDOUBLE:
                    {
                        var.vt = VT_R8;
                        var.dblVal = pRes->getBDouble(i+1).value;
                        break;
                    }
					case OCCI_SQLT_AFC: //ansi char
                    {
                        string strValue = pRes->getString(i+1);
                        var.vt = VT_I1;
                        var.cVal = strValue[0];
                        break;
                    }
					case OCCI_SQLT_CHR: //char string
                    {						
                        string strValue = pRes->getString(i+1);
                        var.SetString(strValue.c_str());
						break;
                    }
					case OCCI_SQLT_NUM:
                    {
                        int nScale = data.getInt(MetaData::ATTR_SCALE);
                        if ( nScale == 0 )
                        {
                            //INT
                            var.vt = VT_INT;
                            var.intVal = pRes->getInt(i+1);
							var.uintVal=(unsigned int)pRes->getUInt(i+1);
                        }
                        else if ( nScale == -127)
                        {
                            //DOUBLE 
                            var.vt = VT_R8;
                            var.dblVal = pRes->getDouble(i+1);
                        }
                        else
                        {
                            //DOUBLE 
                            var.vt = VT_R8;
                            var.dblVal = pRes->getDouble(i+1);
                        }
                        break;
                    }
					case OCCI_SQLT_DAT:		//日期
                    {
                        Date dd = pRes->getDate(i+1);
                        int nYear;
                        unsigned int nMonth, nDay, nHour, nMinute, nSecond;
                        dd.getDate(nYear, nMonth, nDay, nHour, nMinute, nSecond);
						char szDate[256]={0};
                        sprintf(szDate,"%4d-%02d-%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMinute, nSecond);
                        var.SetString(szDate);	
						break;
                    }
					default:				//其它情况都按字符串返回
					{
                        string strValue = pRes->getString(i+1);
                        var.SetString(strValue.c_str());
						break;
					}
                }

                vColumn.push_back(var);
            }

            rtnvec.push_back(vColumn);
        }
		brlt=true;
    }
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
		CFileOpr::getObj().writelocallog("DB_OCCI","QueryData : action failure: pSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] pRes=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pSql,m_pEnv,m_pConn,pStmt,pRes,ErrCode,ErrMsg.c_str());
    }

	if(!ReleaseStmt_Rset_Internal(pStmt,pRes))
		return false;
    return brlt;
}

/**
* @功能描述: 执行更新、删除SQL语句，返回影响的记录条数
* @参数列表: pSql :SQL语句
* @参数列表: AffectedCount :受影响的记录条数
* @参数列表: ErrCode/ErrMsg :错误码/错误信息
* @返 回 值: true,执行成功 false,执行失败
**/
bool  CDB_OCCI::ExcuteUpdate( const char* pSql, int& AffectedCount, int& ErrCode, string& ErrMsg )
{
	CeasymutexGuard guard(m_mutex);

	AffectedCount=0;
	ErrCode=0;
	ErrMsg="";
	bool brlt=false;

	if(!CheckParam_Init_Internal(pSql, ErrCode, ErrMsg )) 
		return false;

	Statement* pStmt=NULL;
	try
    {
        pStmt = ((Connection*)m_pConn)->createStatement();
		if(!pStmt)
		{ 
			ErrCode=-1;  
			ErrMsg="createStatement失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","ExcuteUpdate : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			return false; 
		}

        AffectedCount = pStmt->executeUpdate( pSql );
        ((Connection*)m_pConn)->commit();
		brlt=true;
    }
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
		CFileOpr::getObj().writelocallog("DB_OCCI","ExcuteUpdate : action failure: pSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pSql,m_pEnv,m_pConn,pStmt,ErrCode,ErrMsg.c_str());

		Rollback_Internal();
    }

	if(!ReleaseStmt_Rset_Internal(pStmt,NULL))
		return false;
    return brlt;
}

/**
* @功能描述: 执行任意SQL语句
* @参数列表: pSql :SQL语句
* @参数列表: ErrCode/ErrMsg :错误码/错误信息
* @返 回 值: true,执行成功 false,执行失败
**/
bool  CDB_OCCI::Excute( const char* pSql, int& ErrCode, string& ErrMsg )
{
	CeasymutexGuard guard(m_mutex);

	ErrCode=0;
	ErrMsg="";
	bool brlt=false;

	if(!CheckParam_Init_Internal(pSql, ErrCode, ErrMsg )) 
		return false;

	Statement* pStmt=NULL;
	try
    {
        pStmt = ((Connection*)m_pConn)->createStatement();
		if(!pStmt)
		{ 
			ErrCode=-1;  
			ErrMsg="createStatement失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","Excute : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			return false; 
		}

        ((Connection*)m_pConn)->commit();
		brlt=true;
    }
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
		CFileOpr::getObj().writelocallog("DB_OCCI","Excute : action failure: pSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pSql,m_pEnv,m_pConn,pStmt,ErrCode,ErrMsg.c_str());

		Rollback_Internal();
    }

	if(!ReleaseStmt_Rset_Internal(pStmt,NULL))
		return false;
    return brlt;
}

/**
* @功能描述: 执行查询SQL语句，返回查询结果的首记录的第一个字段的整型值。一般查询select count(*) from...
* @参数列表: pSql :SQL语句
* @参数列表: FirstRtnValue: 查询结果的首记录的第一个字段的整型值
* @参数列表: ErrCode/ErrMsg :错误码/错误信息
* @返 回 值: true,执行成功 false,执行失败
**/
bool  CDB_OCCI::ExcuteSelectCount( const char* pSql, int& FirstRtnValue, int& ErrCode, string& ErrMsg )
{
	CeasymutexGuard guard(m_mutex);

	FirstRtnValue=0;
	ErrCode=0;
	ErrMsg="";
	bool brlt=false;

	if(!CheckParam_Init_Internal(pSql, ErrCode, ErrMsg )) 
		return false;

	Statement* pStmt=NULL;
	ResultSet* pRes=NULL;

	try
    {
        pStmt = ((Connection*)m_pConn)->createStatement();
		if(!pStmt)
		{ 
			ErrCode=-1;  
			ErrMsg="createStatement失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","ExcuteSelectCount : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			return false; 
		}

		pRes = pStmt->executeQuery( pSql );
		if(!pRes)
		{ 
			ErrCode=-1;  
			ErrMsg="executeQuery失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","ExcuteSelectCount : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			((Connection*)m_pConn)->terminateStatement(pStmt);
			return false; 
		}

        if ( pRes->next())
        {
            FirstRtnValue = pRes->getInt(1);
			brlt=true;
        }
    }
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
        brlt=false;
		CFileOpr::getObj().writelocallog("DB_OCCI","ExcuteSelectCount : action failure: pSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] pRes=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pSql,m_pEnv,m_pConn,pStmt,pRes,ErrCode,ErrMsg.c_str());
    }

	if(!ReleaseStmt_Rset_Internal(pStmt,pRes))
		return false;
    return brlt;
}

/**
* @功能描述: 插入一条数据，并返回插入记录的ID
* @参数列表: pSql :插入数据SQL语句
* @参数列表: pGetIDSql :查询可用sequnce id的SQL语句
* @参数列表: rtnID : 返回插入记录的ID
* @参数列表: ErrCode/ErrMsg :错误码/错误信息
* @返 回 值: true,执行成功 false,执行失败
**/
bool  CDB_OCCI::InsertAndReturnID( const char* pSql, const char* pGetIDSql, int& rtnID, int& ErrCode, string& ErrMsg )
{
	CeasymutexGuard guard(m_mutex);

	rtnID=0;
	ErrCode=0;
	ErrMsg="";
	bool brlt=false;

	if(!CheckParam_Init_Internal(pSql, ErrCode, ErrMsg )) 
		return false;

	if(!pGetIDSql)
	{
		ErrCode=-1;  
		ErrMsg="参数错误";  
		CFileOpr::getObj().writelocallog("DB_OCCI","InsertAndReturnID : action failure: pGetIDSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pGetIDSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
		return false; 
	}

	Statement* pStmt=NULL;
	ResultSet* pRes=NULL;

	try
    {
        pStmt = ((Connection*)m_pConn)->createStatement();
		if(!pStmt)
		{ 
			ErrCode=-1;  
			ErrMsg="createStatement失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","InsertAndReturnID : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			return false; 
		}

        pStmt->executeUpdate( pSql );
        ((Connection*)m_pConn)->commit();
	}
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
		CFileOpr::getObj().writelocallog("DB_OCCI","InsertAndReturnID : executeUpdate action failure: pSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pSql,m_pEnv,m_pConn,pStmt,ErrCode,ErrMsg.c_str());

		Rollback_Internal();
		if(!ReleaseStmt_Rset_Internal(pStmt,NULL))
			return false;
		return brlt;
    }

	try
	{
		pRes = pStmt->executeQuery( pGetIDSql );
		if(!pRes)
		{ 
			ErrCode=-1;  
			ErrMsg="InsertAndReturnID中执行executeQuery(pGetIDSql)失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","InsertAndReturnID : executeQuery action failure: pGetIDSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pGetIDSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			((Connection*)m_pConn)->terminateStatement(pStmt);
			return false; 
		}

        if ( pRes->next())
        {
            rtnID = pRes->getInt(1);
			brlt=true;
        }
    }
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
        brlt=false;
		CFileOpr::getObj().writelocallog("DB_OCCI","InsertAndReturnID : executeQuery action failure: pGetIDSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] pRes=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pGetIDSql,m_pEnv,m_pConn,pStmt,pRes,ErrCode,ErrMsg.c_str());
    }

	if(!ReleaseStmt_Rset_Internal(pStmt,pRes))
		return false;
    return brlt;
}

/**
* @功能描述: 执行批量读取数据
* @参数列表: bQuery : 是否是读取。true表示批量读取，执行rset->getNumArrayRows；false表示批量更新，执行pSmt->executeArrayUpdate
* @参数列表: pSql : 读的SQL语句
* @参数列表: fieldnum : 字段/参数的数量
* @参数列表: buffers : 数据指针数组，应该是数组void* [fieldnum]
* @参数列表: types : 字段类型数组。应该是数组FieldType_DB_OCCI[fieldnum]
* @参数列表: sizes : 字段缓冲区尺寸数组，应该是int[fieldnum]
* @参数列表: lens :  字段数据长度数组，应该是unsigned short*[fieldnum]。int类型应该是sizeof(int); 字符串类型应该是strlen()。此值可以是NULL
* @参数列表: cbfunc : 读写过程中的回调函数。开始读写前和每次读写后都会执行此回调函数
* @参数列表: UserData: 用户定义字，回调时带回。用于调用者区分上下文
* @参数列表: ErrCode/ErrMsg :错误码/错误信息
* @返 回 值: true,执行成功 false,执行失败
**/
bool CDB_OCCI::BatchQuery( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, BatchDBRWProcessFunc_OCCI cbfunc,unsigned int UserData,int& ErrCode, string& ErrMsg)
{
	CeasymutexGuard guard(m_mutex);

	ErrCode=0;
	ErrMsg="";
	bool brlt=false;

	if(!CheckParam_Init_Internal(pSql, ErrCode, ErrMsg )) 
		return false;

	if(fieldnum==0||!buffers||!types||!sizes||!cbfunc)
	{
		ErrCode=-1;  
		ErrMsg="参数错误";  
		CFileOpr::getObj().writelocallog("DB_OCCI","BatchQuery : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x] fieldnum=[%d] buffers=[%x] types=[%x] sizes=[%x] cbfunc=[%x]",
			pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn,fieldnum,buffers,types,sizes,cbfunc);
		return false; 
	}

	Statement* pStmt=NULL;
	ResultSet* pRes=NULL;
	try
    {
        pStmt = ((Connection*)m_pConn)->createStatement(pSql);
		if(!pStmt)
		{ 
			ErrCode=-1;  
			ErrMsg="createStatement失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","BatchQuery : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			return false; 
		}

		pRes = pStmt->executeQuery();
		if(!pRes)
		{ 
			ErrCode=-1;  
			ErrMsg="executeQuery失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","BatchQuery : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			((Connection*)m_pConn)->terminateStatement(pStmt);
			return false; 
		}

		for(int i=0;i<fieldnum;i++)
			pRes->setDataBuffer(i+1,buffers[i],(oracle::occi::Type)(int)types[i],types[i]==eOCCI_SQLT_STR?sizes[i]-1:sizes[i],lens[i]);

		int BatchNo=0;
		int ThisTimeRWCount=0;
		int NextTimeRWCount=0;
		ResultSet::Status status=ResultSet::DATA_AVAILABLE;
		while(cbfunc(BatchNo,ThisTimeRWCount,NextTimeRWCount,UserData)&&NextTimeRWCount>0)
		{
			BatchNo++;
			for(int i=0;i<fieldnum;i++)
				memset((char*)buffers[i],0,sizes[i]);
			status=pRes->next(NextTimeRWCount);
			if(status==ResultSet::DATA_AVAILABLE) ThisTimeRWCount=(int)pRes->getNumArrayRows();
			else break;
		}

		brlt=true;
    }
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
		CFileOpr::getObj().writelocallog("DB_OCCI","BatchQuery : action failure: pSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] pRes=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pSql,m_pEnv,m_pConn,pStmt,pRes,ErrCode,ErrMsg.c_str());
    }

	if(!ReleaseStmt_Rset_Internal(pStmt,pRes))
		return false;
    return brlt;
}

/**
* @功能描述: 执行批量更新数据
* @参数列表: pSql : 写的SQL语句。应该类似于：insert into dbname values(:v1,:v2,:v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10)
* @参数列表: fieldnum : 参数的数量
* @参数列表: buffers : 数据指针数组，应该是数组void* [fieldnum]
* @参数列表: types : 字段类型数组。应该是数组FieldType_DB_OCCI[fieldnum]
* @参数列表: sizes : 字段缓冲区尺寸数组，应该是int[fieldnum]
* @参数列表: lens :  字段数据长度数组，应该是(unsigned short*)[fieldnum]。int类型应该是sizeof(int); 字符串类型应该是strlen()
* @参数列表: cbfunc : 读写过程中的回调函数。开始读写前和每次读写后都会执行此回调函数
* @参数列表: UserData: 用户定义字，回调时带回。用于调用者区分上下文
* @参数列表: ErrCode/ErrMsg :错误码/错误信息
* @返 回 值: true,执行成功 false,执行失败
**/
bool CDB_OCCI::BatchUpdate( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, BatchDBRWProcessFunc_OCCI cbfunc,unsigned int UserData,int& ErrCode, string& ErrMsg)
{
	CeasymutexGuard guard(m_mutex);

	ErrCode=0;
	ErrMsg="";
	bool brlt=false;

	if(!CheckParam_Init_Internal(pSql, ErrCode, ErrMsg )) 
		return false;

	if(fieldnum==0||!buffers||!types||!sizes||!lens||!cbfunc)
	{
		ErrCode=-1;  
		ErrMsg="参数错误";  
		CFileOpr::getObj().writelocallog("DB_OCCI","BatchUpdate : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x] fieldnum=[%d] buffers=[%x] types=[%x] sizes=[%x] lens[%x] cbfunc=[%x]",
			pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn,fieldnum,buffers,types,sizes,lens,cbfunc);
		return false; 
	}

	Statement* pStmt=NULL;
	ResultSet* pRes=NULL;
	try
    {
        pStmt = ((Connection*)m_pConn)->createStatement(pSql);
		if(!pStmt)
		{ 
			ErrCode=-1;  
			ErrMsg="createStatement失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","BatchUpdate : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			return false; 
		}

		int BatchNo=0;
		int ThisTimeRWCount=0;
		int NextTimeRWCount=0;
		Statement::Status status=Statement::PREPARED;
		while(cbfunc(BatchNo,ThisTimeRWCount,NextTimeRWCount,UserData)&&NextTimeRWCount>0)
		{
			BatchNo++;
			for(int i=0;i<fieldnum;i++)
				pStmt->setDataBuffer(i+1,buffers[i],(oracle::occi::Type)(int)types[i],sizes[i],lens[i]);
			status=pStmt->executeArrayUpdate(NextTimeRWCount);

			ThisTimeRWCount=NextTimeRWCount;
		}

		brlt=true;
    }
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
		CFileOpr::getObj().writelocallog("DB_OCCI","BatchUpdate : action failure: pSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] pRes=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pSql,m_pEnv,m_pConn,pStmt,pRes,ErrCode,ErrMsg.c_str());
    }

	if(!ReleaseStmt_Rset_Internal(pStmt,pRes))
		return false;
    return brlt;
}
/**
* @功能描述: 执行批量更新数据
* @参数列表: UpdateCount :  需要更新的记录数量
**/
bool CDB_OCCI::BatchUpdate( const char* pSql, int fieldnum,void**buffers, FieldType_DB_OCCI* types,int* sizes, unsigned short** lens, int UpdateCount,int& ErrCode, string& ErrMsg)
{
	CeasymutexGuard guard(m_mutex);

	ErrCode=0;
	ErrMsg="";
	bool brlt=false;

	if(!CheckParam_Init_Internal(pSql, ErrCode, ErrMsg )) 
		return false;

	if(fieldnum==0||!buffers||!types||!sizes||!lens||!UpdateCount)
	{
		ErrCode=-1;  
		ErrMsg="参数错误";  
		CFileOpr::getObj().writelocallog("DB_OCCI","BatchUpdate : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x] fieldnum=[%d] buffers=[%x] types=[%x] sizes=[%x] lens[%x] UpdateCount=[%d]",
			pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn,fieldnum,buffers,types,sizes,lens,UpdateCount);
		return false; 
	}

	Statement* pStmt=NULL;
	ResultSet* pRes=NULL;
	try
    {
        pStmt = ((Connection*)m_pConn)->createStatement(pSql);
		if(!pStmt)
		{ 
			ErrCode=-1;  
			ErrMsg="createStatement失败";  
			CFileOpr::getObj().writelocallog("DB_OCCI","BatchUpdate : action failure: pSql=[%s] ErrCode=[%d] ErrMsg=[%s] m_pEnv=[%x] m_pConn=[%x]",pSql,ErrCode,ErrMsg.c_str(),m_pEnv,m_pConn);
			return false; 
		}

		int BatchNo=0;
		int ThisTimeRWCount=0;
		int NextTimeRWCount=0;
		Statement::Status status=Statement::PREPARED;
		for(int i=0;i<fieldnum;i++)
			pStmt->setDataBuffer(i+1,buffers[i],(oracle::occi::Type)(int)types[i],sizes[i],lens[i]);
		status=pStmt->executeArrayUpdate(UpdateCount);

		brlt=true;
    }
	catch(SQLException & oraex)
	{
		ErrCode=oraex.getErrorCode();  
		ErrMsg = oraex.what()==NULL?"bad ptr":oraex.what();  
		CFileOpr::getObj().writelocallog("DB_OCCI","BatchUpdate : action failure: pSql=[%s] m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] pRes=[%x] ErrCode=[%d] ErrMsg=[%s]",
			pSql,m_pEnv,m_pConn,pStmt,pRes,ErrCode,ErrMsg.c_str());
    }

	if(!ReleaseStmt_Rset_Internal(pStmt,pRes))
		return false;
    return brlt;
}

bool CDB_OCCI::CheckParam_Init_Internal(const char* pSql,int& ErrCode, string& ErrMsg )
{
	if(!pSql)
	{
		ErrCode=-1;  
		ErrMsg="参数错误";  
		return false; 
	}

	if(!m_pEnv) 
	{
		Init();
		if(!m_pEnv)
		{
			ErrCode=-1;  
			ErrMsg="occi初始化失败";  
			return false; 
		}
	}
	if(!m_pConn) 
	{
		Connect();
		if(!m_pConn)
		{
			ErrCode=-1;  
			ErrMsg="occi连接失败";  
			return false; 
		}
	}
	return true;
}

bool CDB_OCCI::ReleaseStmt_Rset_Internal(void* pStmt,void* pRes)
{
	try
	{
		if(pStmt&&pRes) ((Statement*)pStmt)->closeResultSet((ResultSet*)pRes);
		if(m_pConn&&pStmt) ((Connection*)m_pConn)->terminateStatement((Statement*)pStmt);
		return true;
	}
	catch(SQLException & oraex)
	{
		CFileOpr::getObj().writelocallog("DB_OCCI","ReleaseStmt_Rset_Internal : action failure: m_pEnv=[%x] m_pConn=[%x] pStmt=[%x] pRes=[%x] ErrCode=[%d] ErrMsg=[%s]",
			m_pEnv,m_pConn,pStmt,pRes,oraex.getErrorCode(),oraex.what()==NULL?"bad ptr":oraex.what());
		return false;
	}
}

void CDB_OCCI::Rollback_Internal()
{
	try
	{
		if(m_pConn) ((Connection*)m_pConn)->rollback();
	}
	catch(SQLException & oraex)
	{
		CFileOpr::getObj().writelocallog("DB_OCCI","Rollback_Internal : action failure: m_pEnv=[%x] m_pConn=[%x] ErrCode=[%d] ErrMsg=[%s]",
			m_pEnv,m_pConn,oraex.getErrorCode(),oraex.what()==NULL?"bad ptr":oraex.what());
	}
}
