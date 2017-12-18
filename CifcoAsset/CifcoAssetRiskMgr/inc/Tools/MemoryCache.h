// StreamCache.h: interface for the CMemoryCache class.
//
//	作用：	定义字符流缓存类
//	描述：	1. 此类用于将字符流缓存
//			2. 遵循先进先出的原则
//			3. 主要外部接口：
//				SaveDataTo()	：将一块数据存入缓存
//				GetDataFrom()	：从缓存中取出当前数据块
//				GetCurDataLen()	：取当前数据块的长度
//				IsEmpty()		：缓存是否为空
//			4. m_bEmptyID
//				很重要，因为缓存按环形缓冲区处理。需要通过m_bEmptyID判断是否为空
//			5. 线程安全：
//				保存数据和读取数据的操作线程安全，可以多个线程同时读取或同时保存。
//			6. 保存两个指针：GetOff和SetOff，GetOff等于SetOff表示为空，GetOff可以追SetOff，但不能超过它，SetOff不能追上GetOff
//			7. 注意，构造和析构对象时，是线程不安全的。父线程要注意保护 
//
//	history 
//	20100119	lzq		创建该文件
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STREAMCACHE_H__BD4088B5_55BB_414B_99FD_99A903F1A9E3__INCLUDED_)
#define AFX_STREAMCACHE_H__BD4088B5_55BB_414B_99FD_99A903F1A9E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4996)

#ifdef WIN32
#pragma managed(push,off)
#endif 



class CMemoryCache  
{
public:
	/**************************************************************************************************//**
	*   @brief				CMemoryCache类的构造函数        
	*   @param[in] CacheSize	缓冲区大小
	*   @param[in] Mode			运行模式:
	*   - 0 单线程读,单线程写
	*   - 3 多线程读,多线程写
	*******************************************************************************************************/
	CMemoryCache(int CacheSize,int Mode=3);
	virtual ~CMemoryCache();


	//将一块数据存入缓存，加锁
	bool			SaveDataTo(void *pData,int DataLen);

	//从缓存中取数据，加锁				
	int				GetLen(void);												//取当前缓冲区内容的长度
	int				GetPos_SubStr(const char* pSubStr);							//取子串的位置，子串以0结尾
	bool			PeekFrom(void *pData,int PeekLen);							//从本地缓存中取出指定长度的内容，不删除
	bool			GetFrom(void *pData,int GetLen);							//从缓存中取出指定长度的内容，并从缓存中删除
	void			DeleteData(int DeleteLen);									//删除指定长度的数据
	void			Empty(void);												//清空缓冲区，由读取线程调用

	//下面几个操作不加锁
	bool			IsEmpty(void);												//本地缓存内容是否为空
    void            GetPosition(int& GetOrd,int& SetOrd);
	int				GetCacheSize(void);											//取Cache的尺寸

private:
	char			*m_pData;													//内存缓存时的指针
	int				m_CacheSize;
	int				m_GetOff;													//本地缓存里取数据的当前Block的编号，相对于文件头，注意，不是字节编号
	int				m_SetOff;													//本地缓存里存数据的当前Block的编号，相对于文件头，注意，不是字节编号
	int				m_Mode;														//运行模式,参见构造函数的参数说明
	char			*m_pSem;													//读写互斥锁


	int				GetLen_Internal(void);										//取当前缓冲区内容的长度，内部调用
	int				ReadDate_Internal(int iOff,void*pData,int ReadLen);			//从指定位置读一块数据，考虑环形存储，iOff是相对位置
	int				WriteData_Internal(int iOff,void*pData,int WriteLen);		//向指定位置写一块数据，考虑环形存储，iOff是相对位置

																				//申明下面两个函数，但不实现它们，目的是禁止拷贝构造和赋值
	CMemoryCache(const CMemoryCache&);											//为了处理拷贝构造
	CMemoryCache& operator= (const CMemoryCache&);								//赋值符重载

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_STREAMCACHE_H__BD4088B5_55BB_414B_99FD_99A903F1A9E3__INCLUDED_)
