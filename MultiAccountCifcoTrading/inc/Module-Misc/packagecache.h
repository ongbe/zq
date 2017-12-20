//*******************************************************************************************
//	作用：	定义不定长包的缓存类
//	描述：	1. 此类用于将不定长数据块缓存
//			2. 数据结构为：Package = [ PackageHead + Data + Tail ]
//			3. Data占用的空间按4字节用零补整
//			4. 遵循先进先出的原则
//			5. 主要外部接口：
//				SaveDataTo()	：将一块数据存入缓存
//				GetDataFrom()	：从缓存中取出当前数据块
//				GetCurDataLen()	：取当前数据块的长度
//				IsEmpty()		：缓存是否为空
//			6. 杂项接口：
//				SetCacheParam()	：设置缓存参数，对于文件缓存，设置文件名和最大尺寸；对于内存缓存，设置最大尺寸
//			7. m_bEmptyID
//				很重要，因为缓存按环形缓冲区处理。需要通过m_bEmptyID判断是否为空
//			8. 非线程安全：
//				保存数据和读取数据的操作可以分别放在两个线程里。但不能多个线程同时读取或同时保存。
//			9. 保存两个指针：GetOff和SetOff，GetOff等于SetOff表示为空，GetOff可以追SetOff，但不能超过它，SetOff不能追上GetOff
//
//	history 
//	20100119	lzq		创建该文件
//
//*******************************************************************************************

#pragma once

#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4996)


//#include "toolpublic.h"

#include "MutexLock.h"

#define PREFIX_PACKAGECACHE		0xa973b71c					//Package开始标志
#define TAIL_PACKAGECACHE		0x5a7b9c2d					//Package结束标志




//下面定义Package包头结构
typedef struct
{
	DWORD	Prefix;						                    //前导标志，用于标志一个package的开始
	int		CmdID;						                    //Package的命令ID，可以是CMDID_xxxx
	int		PlatformID;					                    //交易平台ID，可以是PLATFORM_xxxx
	int		ExchangeID;					                    //交易所ID，可以是EXCHANGE_xxxx

	int		DataLen;					                    //数据长度，指内容的长度。注意真正内容的大小是按DataLen长度用零补整，使之为4字节边界。
	DWORD	dwCRC;						                    //内容的算术和，内容包括PackageHead和Data和Tail
	DWORD	dwSeq;						                    //同一类Package的序列号
}PackageHeadStru;



//	不定长package缓存类
class CPackageCache
{
public:	
	/**************************************************************************************************//**
	*   @brief				CPackageCache类的构造函数        
	*   @param[in] CacheSize	总字节数
	*   @param[in] Mode			运行模式:
	*   - 0 单线程读,单线程写
	*   - 1 单线程读,多线程写
	*   - 2 多线程读,单线程写
	*   - 3或其他 多线程读,多线程写
	*******************************************************************************************************/
	CPackageCache(int CacheSize, int Mode=1);	

	~CPackageCache();


																				//下面函数由保存线程调用
	bool			SaveDataTo(PackageHeadStru* pPackageHead, const void* pData,int* pOriSetOff=NULL,int* pNewSetOff=NULL);		//将一块数据存入缓存
	bool			SaveDataTo(int CmdID,int PlatformID,int ExchangeID,DWORD dwSeq,int DataLen, const void* pData,int* pOriSetOff=NULL,int* pNewSetOff=NULL);	//将一块数据存入缓存

																				//下面函数由读取线程调用				
    int             GetCurPackageDataLen(void);                                 //取当前包里数据长度，注意，是数据的长度，不是整个包的长度
	bool			PeekFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize,int* pGetOff=NULL);//从字符缓存中取出当前数据块，不删除。如果DataSize不够，返回失败.如果pData==NULL则只取包头
    bool            PeekFrom(int&CmdID,int&PlatformID,int& ExchangeID,DWORD& dwSeq,int& DataLen,void* pData,int DataSize,int* pGetOff=NULL);//从字符缓存中取出当前数据块，不删除。如果DataSize不够，返回失败.如果pData==NULL则只取包头
	bool			GetFrom(PackageHeadStru* pPackageHead,void* pData,int DataSize,int* pOriGetOff=NULL,int* pNewGetOff=NULL); //从缓存中取出当前数据块
	void			DeleteCurPackage(int* pOriGetOff=NULL,int* pNewGetOff=NULL);										//删除一个数据块
	bool			IsEmpty(void);												//本地缓存内容是否为空
	void			Empty(void);												//清空缓冲区，由读取线程调用
    void            GetPosition(int& GetOff,int& SetOff);

public:
	 
	/**************************************************************************************************//**
    *   @brief		将指定数据对齐，对齐部分填0，然后再添加Package的尾   
    *   @param[in]	pData 指定的数据 
	*   @param[in]	offset	数据偏移
	*   @param[in]	AlignedBytes 按指定字节数对齐，目前只支持4字节对齐
    *   @return		总共填补的字节数，包括TAIL_PACKAGECACHE标记的长度（sizeof(DWORD)）
    *******************************************************************************************************/
	static inline unsigned int FillPackageTail(void * pData, unsigned int offset, unsigned int AlignedBytes=4)
	{
		unsigned int dataAlignedLen = (offset+3)&(~3);
		unsigned int filledLen=dataAlignedLen-offset;
		if(filledLen>0)
			memset(((char*)pData)+offset, 0, filledLen);

		*(DWORD*)(((char*)pData)+dataAlignedLen) = TAIL_PACKAGECACHE;

		return (filledLen+sizeof(DWORD));
	};



private:
	char*			m_pCache;													//内存缓存时的指针
	int				m_CacheSize;												//Cache的尺寸，字节数，必须是4的整数倍
	int				m_GetOff;													//本地缓存里取数据的当前字节位置，相对于Cache头
	int				m_SetOff;													//本地缓存里存数据的当前字节位置，相对于Cache头

	void			SetCacheParam(int CacheSize);								//设置缓存的参数，包括最大尺寸
	int				ReadData(int Off,char* pData,int ReadLen) const;					//从指定位置读一块数据，考虑环形存储，iOff是相对位置
	int				WriteData(int Off, const char* pData,int WriteLen);				//向指定位置写一块数据，考虑环形存储，iOff是相对位置
	int				m_Mode;														//运行模式,参见构造函数的参数说明
	MutexLock       *m_pReadMutexLock;											//读互斥锁
	MutexLock       *m_pWriteMutexLock;											//写互斥锁

	//此函数以前是public属性
	void			IgnoreInvalidPackage(void);									//跳过无效的数据

																				//申明下面两个函数，但不实现它们，目的是禁止拷贝构造和赋值
	CPackageCache(const CPackageCache&);										//为了处理拷贝构造
	CPackageCache& operator= (const CPackageCache&);							//赋值符重载

};



