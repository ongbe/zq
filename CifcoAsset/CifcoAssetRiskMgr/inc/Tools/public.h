//******************************************************************************************************
//	一些通用的定义
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//******************************************************************************************************

#pragma once

#ifndef PUBLIC_INCLUDE
#define PUBLIC_INCLUDE


typedef		unsigned long		DWORD;
typedef		unsigned short		WORD;
typedef		int					BOOL;
typedef		unsigned int		UINT;
typedef		unsigned char       BYTE;
//typedef		__int64				LONGLONG;
typedef		void*				LPVOID;
typedef		void*				PVOID;
typedef		unsigned char		UCHAR;
typedef		UCHAR*				PUCHAR;
typedef		void *				HANDLE;




#define		MAKEWORD_PUBLIC(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define		MAKELONG_PUBLIC(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define		LOWORD_PUBLIC(l)           ((WORD)(l))
#define		HIWORD_PUBLIC(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define		LOBYTE_PUBLIC(w)           ((BYTE)(w))
#define		HIBYTE_PUBLIC(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#ifndef		TRUE
#define		TRUE				1
#endif

#ifndef		FALSE
#define		FALSE				0
#endif

#ifndef		NULL
#ifdef		__cplusplus
#define		NULL		0
#else
#define		NULL		((void *)0)
#endif
#endif

																				//计算时间差
#ifndef		DELTAMS
#define		DELTAMS(Secs1,MS1,Secs2,MS2)	((int)(((int)Secs2-(int)Secs1)*1000+(int)MS2-(int)MS1))
#endif

																				//取结构中元素的地址偏移量
#ifndef		OffsetInStruct
#define		OffsetInStruct(Stru,Element)	((int32)&(((Stru*)0)->Element))
#endif

																				//取结构(类)中元素的sizeof
#ifndef		SizeofElement
#define		SizeofElement(Stru,Element)		sizeof( ( ((Stru*)0) -> Element ) )
#endif

																				//取数组的元素个数
#ifndef		ArrayElements
#define		ArrayElements(i)				(sizeof(i)/sizeof((i)[0]))
#endif

																				//取结构(类)中元素的数组个数 
#ifndef		ArrayElementsInStru
#define		ArrayElementsInStru(Stru,Element) ( sizeof( (((Stru*)0)->Element) ) / sizeof( (((Stru*)0)->Element)[0] ) )
#endif


#ifdef		HOSTNAMELEN			
#undef		HOSTNAMELEN
#endif
#define		HOSTNAMELEN			32														//主机名称最大长度

#ifdef		USERNAMELEN
#undef		USERNAMELEN
#endif
#define		USERNAMELEN			32														//用户名称的最大长度


#endif