#pragma once
//底层平台定义Type
typedef enum _tagPTYPE
{
	PTYPE_NULL,
	PTYPE_CTP,
	PTYPE_PATSAPI,
	PTYPE_CME,
}ePLATFORMTYPE;
//#define PTYPE_NULL	0
//#define PTYPE_CME	    1
//#define PTYPE_CTP		2
//#define PTYPE_PATSAPI	3


//定义默认平台TYPE为空
#define  PLATFORM_DEFAULT  PTYPE_NULL
//如果定义了_PTYPE_CME_使用CME平台
#ifdef  _PTYPE_CME_
	#ifdef PLATFORM_DEFAULT
	#undef PLATFORM_DEFAULT
	#endif
	#define PLATFORM_DEFAULT  PTYPE_CME
#endif
//如果定义了_PTYPE_CTP_使用综合交易平台平台
#ifdef  _PTYPE_CTP_
	#ifdef PLATFORM_DEFAULT
	#undef PLATFORM_DEFAULT
	#endif
	#define PLATFORM_DEFAULT  PTYPE_CTP
#endif
//如果定义了_PTYPE_PATSAPI_使用PATSAPI平台
#ifdef  _PTYPE_PATSAPI_
	#ifdef PLATFORM_DEFAULT
	#undef PLATFORM_DEFAULT
	#endif
	#define PLATFORM_DEFAULT  PTYPE_PATSAPI
#endif
