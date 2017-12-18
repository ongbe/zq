// VariablePackage.h: interface for the CVariablePackage class.
//
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
//			6. m_bEmptyID
//				很重要，因为缓存按环形缓冲区处理。需要通过m_bEmptyID判断是否为空
//			7. 线程安全：
//				保存数据和读取数据的操作线程安全，可以多个线程同时读取或同时保存。
//			8. 保存两个指针：GetOff和SetOff，GetOff等于SetOff表示为空，GetOff可以追SetOff，但不能超过它，SetOff不能追上GetOff
//			9. 注意，构造和析构对象时，是线程不安全的。父线程要注意保护 
//
//	history 
//	20100119	lzq		创建该文件
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIABLEPACKAGE_H__C9312ED9_E58A_4C11_8758_3EDE51E2D61A__INCLUDED_)
#define AFX_VARIABLEPACKAGE_H__C9312ED9_E58A_4C11_8758_3EDE51E2D61A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable : 4996)

#include <string.h>

#ifdef WIN32
#pragma managed(push,off)
#endif 


#define PREFIX_VARIABLEPACKAGE		0xa973b72c					//Package开始标志
#define TAIL_VARIABLEPACKAGE		0x5a7b9c3d					//Package结束标志




class CVariablePackage  
{
public:
	//下面定义VariablePackage包头结构
	struct HeadStru
	{
		HeadStru()
		{
			clear();
		};
		void clear(void)
		{
			memset(this,0,sizeof(*this));
		};

		unsigned int	Prefix;										//前导标志，用于标志一个package的开始
		unsigned int	CmdID;                                      //命令字
		int				DataLen1;					                //数据块1长度，指内容的长度。注意真正内容的大小是按DataLen长度用零补整，使之为4字节边界。
		int				DataLen2;					                //数据块2长度，指内容的长度。注意真正内容的大小是按DataLen长度用零补整，使之为4字节边界。
		unsigned int	dwCRC;						                //内容的算术和，内容包括PackageHead和Data和Tail
		unsigned int	dwSeq;						                //同一类Package的序列号

		unsigned int	UserData1;
		unsigned int	UserData2;
		unsigned int	UserData3;
		unsigned int	UserData4;
	};

public:	
	/**************************************************************************************************//**
	*   @brief				CVariablePackage        
	*   @param[in] CacheSize	总字节数
	*   @param[in] Mode			运行模式:
	*   - 0 单线程读,单线程写，不加锁
	*   - 3 多线程读,多线程写，加锁
	*******************************************************************************************************/
	CVariablePackage(int CacheSize, int Mode=3);	
	virtual ~CVariablePackage();

	//将一块数据存入缓存，加锁
	bool    SaveDataTo(HeadStru& PackageHead, const void* pData1, const void*pData2=NULL);		
	bool	SaveDataTo2(unsigned int CmdID,const void* pData1,int DataLen1,const void* pData2=NULL,int DataLen2=0,unsigned int UserData1=0,unsigned int UserData2=0,unsigned int UserData3=0,unsigned int UserData4=0);

    //取当前包里数据长度，注意，是数据的长度，不是整个包的长度
    int     GetCurPackageDataLen(void);                                 
    //从字符缓存中取出当前数据块，不删除。如果DataSize不够，返回失败.如果pData==NULL则只取包头
	bool	PeekFrom(HeadStru& PackageHead,void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0);
    //从字符缓存中取出当前数据块，不删除。如果DataSize不够，返回失败.如果pData==NULL则只取包头
    HeadStru PeekFrom(void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0);
    //从缓存中取出当前数据块，然后删除
	bool	GetFrom(HeadStru& PackageHead,void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0); 
    //从字符缓存中取出当前数据块，然后删除。如果DataSize不够，返回失败.如果pData==NULL则只取包头
    HeadStru GetFrom(void* pData1,int DataSize1,void* pData2=NULL,int DataSize2=0);
    //删除一个数据块
	void	DeleteCurPackage(void);										
    //清空缓冲区，由读取线程调用
	void	Empty(void);												
	int		GetCacheSize(void);

	//下面两个操作不加锁
	bool	IsEmpty(void);
    void    GetPosition(int& GetOff,int& SetOff);

public:
	 


private:
	char*			m_pCache;													//内存缓存时的指针
	int				m_CacheSize;												//Cache的尺寸，字节数，必须是4的整数倍
	int				m_GetOff;													//本地缓存里取数据的当前字节位置，相对于Cache头
	int				m_SetOff;													//本地缓存里存数据的当前字节位置，相对于Cache头
	int				m_Mode;														//运行模式,参见构造函数的参数说明
	char			*m_pSem;													//读写互斥锁
	unsigned int	m_Seq;

	int				ReadData(int Off,char* pData,int ReadLen) const;			//从指定位置读一块数据，考虑环形存储，iOff是相对位置
	int				WriteData(int Off, const char* pData,int WriteLen);			//向指定位置写一块数据，考虑环形存储，iOff是相对位置
	void			IgnoreInvalidPackage(void);									//跳过无效的数据

																				//申明下面两个函数，但不实现它们，目的是禁止拷贝构造和赋值
	CVariablePackage(const CVariablePackage&);									//为了处理拷贝构造
	CVariablePackage& operator= (const CVariablePackage&);						//赋值符重载

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_VARIABLEPACKAGE_H__C9312ED9_E58A_4C11_8758_3EDE51E2D61A__INCLUDED_)
