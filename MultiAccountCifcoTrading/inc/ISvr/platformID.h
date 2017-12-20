#pragma once
typedef enum _tagPTYPE
{
	PTYPE_NULL,
	PTYPE_CTP,
	PTYPE_PATSAPI,
	PTYPE_CME,
}PTYPE;
//底层平台定义ID
//#define PID_NULL	0
//#define PID_CME	    1
//#define PTYPE_CTP		2
//#define PTYPE_PATSAPI	3
//
//
////定义默认平台ID为空
//#define  PLATFORM_DEFAULT  PID_NULL
////如果定义了_PID_CME_使用CME平台
//#ifdef  _PID_CME_
//	#ifdef PLATFORM_DEFAULT
//	#undef PLATFORM_DEFAULT
//	#endif
//	#define PLATFORM_DEFAULT  PID_CME
//#endif
////如果定义了_PID_CTP_使用综合交易平台平台
//#ifdef  _PID_CTP_
//	#ifdef PLATFORM_DEFAULT
//	#undef PLATFORM_DEFAULT
//	#endif
//	#define PLATFORM_DEFAULT  PTYPE_CTP
//#endif
////如果定义了_PTYPE_PATSAPI_使用PATSAPI平台
//#ifdef  _PTYPE_PATSAPI_
//	#ifdef PLATFORM_DEFAULT
//	#undef PLATFORM_DEFAULT
//	#endif
//	#define PLATFORM_DEFAULT  PTYPE_PATSAPI
//#endif
