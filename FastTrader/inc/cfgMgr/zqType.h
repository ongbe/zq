#pragma once

#define ITEM_INIT_NONE		0
#define ITEM_INIT_SYSTEM	1
#define ITEM_INIT_USER		2

#define DEF_ZQ_CLASS(type)\
struct zq_##type\
{\
public:\
	zq_##type()\
	{\
		m_val=0;\
		m_dirty=false;\
		m_fromuser=ITEM_INIT_NONE;\
	}\
	operator type()\
	{\
		return m_val;\
	}\
	type& operator=(const type& t)\
	{\
		if(m_val!=t)\
		{\
			m_dirty=true;\
			m_val=t;\
		}\
		return m_val;\
	}\
	type& operator=(type& t)\
	{\
		if(m_val!=t)\
		{\
			m_dirty=true;\
			m_val=t;\
		}\
		return m_val;\
	}\
	void xmlset(type& t,bool fromuser)\
	{\
		m_val=t;\
		m_fromuser=fromuser?ITEM_INIT_USER:ITEM_INIT_SYSTEM;\
	}\
	void xmlset(const type& t,bool fromuser=false)\
	{\
		m_val=t;\
		m_fromuser=fromuser?ITEM_INIT_USER:ITEM_INIT_SYSTEM;\
	}\
	bool isDirty(){return m_dirty;}\
	bool isFromUsercfg(){return m_fromuser==ITEM_INIT_USER;}\
	bool needWriteToUserCfg(){return isDirty() || isFromUsercfg();}\
private:\
	type m_val;\
	bool m_dirty;\
	int m_fromuser;\
}

#define DEF_ZQ_STRING(length)\
struct zq_char##length \
{\
public:\
	zq_char##length()\
	{\
		memset(m_val,0,length);\
		m_dirty=false;\
		m_fromuser=ITEM_INIT_NONE;\
	}\
	operator const char*()\
	{\
	return m_val;\
	}\
	bool copy(const char* pSrc)\
	{\
		bool b=(strcmp(m_val,pSrc)!=0);\
		if(b)\
		{\
			m_dirty=true;\
			strncpy_s(m_val,length,pSrc,strlen(pSrc));\
		}\
		return b;\
	}\
	bool copy(const char* pSrc,int count)\
	{\
		bool b=(strcmp(m_val,pSrc)!=0);\
		if(b)\
		{\
			m_dirty=true;\
			strncpy_s(m_val,length,pSrc,count);	\
		}\
		return b;\
	}\
	void xmlset(char* t,bool fromuser)\
	{\
		strncpy_s(m_val,length,t,strlen(t));\
		m_fromuser=fromuser?ITEM_INIT_USER:ITEM_INIT_SYSTEM;\
	}\
	void xmlset(const char* t,bool fromuser=false)\
	{\
		strncpy_s(m_val,length,t,strlen(t));\
		m_fromuser=fromuser?ITEM_INIT_USER:ITEM_INIT_SYSTEM;\
	}\
	bool isDirty(){return m_dirty;}\
	bool isFromUsercfg(){return m_fromuser==ITEM_INIT_USER;}\
	bool needWriteToUserCfg(){return isDirty() || isFromUsercfg();}\
private:\
	char m_val[length];\
	bool m_dirty;\
	int m_fromuser;\
}
#define DEF_ZQ_ARRAY(type,length)\
struct zq_##type##length \
{\
public:\
	zq_##type##length()\
	{\
	memset(m_val,0,length*sizeof(type));\
	m_dirty=false;\
	m_fromuser=ITEM_INIT_NONE;\
	}\
	const type& operator[](const unsigned __int64 index)\
	{\
	return m_val[index];\
	}\
	bool copy(const type* pSrc)\
	{\
		bool b=(memcmp(m_val,pSrc,length*sizeof(type))!=0);\
		if(b)\
		{\
		m_dirty=true;\
		memcpy(m_val,pSrc,length*sizeof(type));\
		}\
		return b;\
	}\
	bool copy(const type* pSrc,int count)\
	{\
		bool b=(memcmp(m_val,pSrc,length*sizeof(type))!=0);\
		if(b)\
		{\
		m_dirty=true;\
		memcpy(m_val,pSrc,count*sizeof(type));\
		}\
		return b;\
	}\
	void xmlset(type* t,bool fromuser)\
	{\
	memcpy(m_val,t,length*sizeof(type));\
	m_fromuser=fromuser?ITEM_INIT_USER:ITEM_INIT_SYSTEM;\
	}\
	void xmlset(const type* t,bool fromuser=false)\
	{\
	memcpy(m_val,t,length*sizeof(type));\
	m_fromuser=fromuser?ITEM_INIT_USER:ITEM_INIT_SYSTEM;\
	}\
	bool isDirty(){return m_dirty;}\
	bool isFromUsercfg(){return m_fromuser==ITEM_INIT_USER;}\
	bool needWriteToUserCfg(){return isDirty() || isFromUsercfg();}\
private:\
	type m_val[length];\
	bool m_dirty;\
	int m_fromuser;\
}
DEF_ZQ_CLASS(char);
DEF_ZQ_CLASS(int);
DEF_ZQ_CLASS(bool);
DEF_ZQ_CLASS(float);
DEF_ZQ_CLASS(double);
DEF_ZQ_CLASS(long);
DEF_ZQ_STRING(20);
DEF_ZQ_STRING(32);
DEF_ZQ_STRING(64);
DEF_ZQ_ARRAY(int,9);