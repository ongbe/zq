#pragma once

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p){delete[] (p);  (p)=NULL;} }
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&); \
	void operator=(const TypeName&)
#endif

#ifndef __CUSTOM_FILE__
	#define __CUSTOM_FILE__    (std::string(__FILE__).substr(std::string(__FILE__).rfind('\\')+1).c_str())
#endif

#define MAX_SQL_LENGTH			5000	//SQL语句最大长度
#define MAX_USABLE_SQL_LENGTH	4999
#define LOG_BUFFER_SIZE			256
#define LOG_BUFFER_USABLE_SIZE	255
#define INVALID_ACCID			0		//无效账户ID
#define STATISTICS_DATA_BEGIN   (-1)
#define STATISTICS_DATA_END		(-2)
#define SAFE_GET_DATACENTER()  if(CDataCenter::Get())CDataCenter::Get()
#define GET_DBACCESS()		CDataCenter::Get()->GetDBAccess()

#define ALL_RISKMSGQUEUE_INIT() \
	CRiskMsgQueue::Init();\
	CRiskMsgQueue_Quot::Init();\
	CRiskMsgQueue_Sync::Init();\
	CRisk2ClientMsgQueue::Init();\
	CClient2RiskMsgQueue::Init();\
	CForceCloseMsgQueue::Init();
#define ALL_RISKMSGQUEUE_RELEASE() \
	CRiskMsgQueue::Release();\
	CRiskMsgQueue_Quot::Release();\
	CRiskMsgQueue_Sync::Release();\
	CRisk2ClientMsgQueue::Release();\
	CClient2RiskMsgQueue::Release();\
	CForceCloseMsgQueue::Release();

#define DIRTYITEM(tag)\
bool m_Dirty_##tag;\
CReadWriteLock m_DMutex_##tag;\
bool Get##tag##Dirty()\
{\
	bool bRet;\
	m_DMutex_##tag.read_lock();\
	bRet=m_Dirty_##tag;\
	m_Dirty_##tag=false;\
	m_DMutex_##tag.read_unlock();\
	return bRet;\
}\
void Set##tag##Dirty()\
{\
	m_DMutex_##tag.read_lock();\
	m_Dirty_##tag=true;\
	m_DMutex_##tag.read_unlock();\
}

#define PROCESS_MAP_BEGIN(tag)\
	unsigned int p_cmdid=tag;\
	if(0){\
	}

#define PROCESS_ITEM(ctype,func) \
else if(p_cmdid >= ctype##_Min && p_cmdid <= ctype##_Max ){\
func(reqData);\
}

#define PROCESS_MAP_END()\
else{\
	OutputUnkownReqLog(reqData);\
}	

#define SAFE_CLEAR2(tag,m)\
do\
{\
	m.write_lock();\
	tag.clear();\
	m.write_unlock();\
}while(0)
#define SAFE_CLEAR(tag) SAFE_CLEAR2(m_##tag,m_Mutex_##tag)

#define WRITESIMPLECTPDATA(tag) WriteSimpleCtpData(m_##tag,m_Mutex_##tag,#tag##".xml")
#define WRITECOMPLEXCTPDATA(tag) WriteComplexCtpData(m_##tag,m_Mutex_##tag,#tag##".xml")
#define READSIMPLECTPDATA(tag) ReadSimpleCtpData(m_##tag,m_Mutex_##tag,#tag##".xml")
#define READCOMPLEXCTPDATA(tag) ReadComplexCtpData(m_##tag,m_Mutex_##tag,#tag##".xml")
#define XML_ATTRIBUTE(p,tag)  (p->Attribute(tag)?p->Attribute(tag):"")
#define XML_ELEMENTTEXT(p)    (p?(p->Value()?std::string(p->Value()):std::string("")):std::string(""))
#define safestrcpy(dest,destlen,src) strncpy_s(dest,destlen,src,(destlen)-1)


#define GETDB_INT(tag)	do{info.tag=m_pRes->getInt(++index);}while(0)
#define GETDB_DOUBLE(tag)	do{info.tag=m_pRes->getDouble(++index);}while(0)
#define GETDB_CHAR(tag) do{std::string strValue=m_pRes->getString(++index);info.tag=strValue.empty()?0:strValue[0];}while(0)
#define GETDB_STR(tag)	do{safestrcpy(info.tag,sizeof(info.tag),m_pRes->getString(++index).c_str());}while(0)
#define SETDBBUF_INT(tag)	do{m_pStmt->setDataBuffer(++index,tag,OCCIINT, sizeof(int), len_int);}while(0)
#define SETDBBUF_DOUBLE(tag)	do{m_pStmt->setDataBuffer(++index,tag,OCCIFLOAT, sizeof(double), len_double);}while(0)
#define SETDBBUF_CHAR(tag) do{m_pStmt->setDataBuffer(++index,tag,OCCI_SQLT_CHR, sizeof(char), len_char);}while(0)
#define SETDBBUF_STR(tag,type,lenarray)	do{m_pStmt->setDataBuffer(++index,tag,OCCI_SQLT_STR, sizeof(type), lenarray);}while(0)
#define NEW_ARRAY_PTR(type,ptrname)  type* ptrname=new type[nMaxInsertRow];CSafeArrayPtr<type> ap_##ptrname(ptrname) 
#define FILLDB_EQUVAL(tag) tag[nRecordNum] = field.tag
#define FILLDB_STR(tag,lenarray) \
do{\
	safestrcpy(tag[nRecordNum],sizeof(field.tag),field.tag);\
	lenarray[nRecordNum]=strlen(field.tag)+1;\
}while(0)