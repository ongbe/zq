#line 1 "f:\\svn\\miscellaneous\\quotserver\\stdafx.cpp"




#line 1 "f:\\svn\\miscellaneous\\quotserver\\stdafx.h"





#pragma once

#line 1 "f:\\svn\\miscellaneous\\quotserver\\targetver.h"
#pragma once










#line 13 "f:\\svn\\miscellaneous\\quotserver\\targetver.h"

#line 9 "f:\\svn\\miscellaneous\\quotserver\\stdafx.h"

#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"















#pragma once
#line 18 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"




#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"














 





#line 22 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 23 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"














#line 38 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"



#line 42 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




















    

#line 65 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 67 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"














#pragma once































































































#line 112 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"




#line 117 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"




#line 122 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"
#line 123 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"





















#line 145 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"



#line 149 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"



































































































































































































































































































































































































































































































































































































































































                                                




                                                






































































































































#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"


#pragma once








#line 13 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"
#line 14 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"





typedef __w64 unsigned int   size_t;
#line 21 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

#line 23 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"
















#line 40 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"


namespace vc_attributes
{
#line 45 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

enum YesNoMaybe
{
	
	No = 0x0fff0001,
	Maybe = 0x0fff0010,
	Yes = 0x0fff0100
};

typedef enum YesNoMaybe YesNoMaybe;

enum AccessType
{
	NoAccess = 0,
	Read = 1,
	Write = 2,
	ReadWrite = 3
};

typedef enum AccessType AccessType;



[repeatable]
[source_annotation_attribute( Parameter )]
struct PreAttribute
{

	PreAttribute();
#line 75 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

	unsigned int Deref;
	YesNoMaybe Valid;
	YesNoMaybe Null;
	YesNoMaybe Tainted;
	AccessType Access;
	size_t ValidElementsConst;
	size_t ValidBytesConst;
	const wchar_t* ValidElements;
	const wchar_t* ValidBytes;
	const wchar_t* ValidElementsLength;
	const wchar_t* ValidBytesLength;
	size_t WritableElementsConst;
	size_t WritableBytesConst;
	const wchar_t* WritableElements;
	const wchar_t* WritableBytes;
	const wchar_t* WritableElementsLength;
	const wchar_t* WritableBytesLength;
	size_t ElementSizeConst;
	const wchar_t* ElementSize;
	YesNoMaybe NullTerminated;
	const wchar_t* Condition;
};

[repeatable]
[source_annotation_attribute( Parameter|ReturnValue )]
struct PostAttribute
{

	PostAttribute();
#line 106 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

	unsigned int Deref;
	YesNoMaybe Valid;
	YesNoMaybe Null;
	YesNoMaybe Tainted;
	AccessType Access;
	size_t ValidElementsConst;
	size_t ValidBytesConst;
	const wchar_t* ValidElements;
	const wchar_t* ValidBytes;
	const wchar_t* ValidElementsLength;
	const wchar_t* ValidBytesLength;
	size_t WritableElementsConst;
	size_t WritableBytesConst;
	const wchar_t* WritableElements;
	const wchar_t* WritableBytes;
	const wchar_t* WritableElementsLength;
	const wchar_t* WritableBytesLength;
	size_t ElementSizeConst;
	const wchar_t* ElementSize;
	YesNoMaybe NullTerminated;
	YesNoMaybe MustCheck;
	const wchar_t* Condition;
};

[source_annotation_attribute( Parameter )]
struct FormatStringAttribute
{

	FormatStringAttribute();
#line 137 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

	const wchar_t* Style;
	const wchar_t* UnformattedAlternative;
};

[repeatable]
[source_annotation_attribute( ReturnValue )]
struct InvalidCheckAttribute
{

	InvalidCheckAttribute();
#line 149 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

	long Value;
};

[source_annotation_attribute( Method )]
struct SuccessAttribute
{

	SuccessAttribute();
#line 159 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

	const wchar_t* Condition;
};

[repeatable]
[source_annotation_attribute( Parameter )]
struct PreBoundAttribute
{

	PreBoundAttribute();
#line 170 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"
	unsigned int Deref;
};

[repeatable]
[source_annotation_attribute( Parameter|ReturnValue )]
struct PostBoundAttribute
{

	PostBoundAttribute();
#line 180 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"
	unsigned int Deref;
};

[repeatable]
[source_annotation_attribute( Parameter )]
struct PreRangeAttribute
{

	PreRangeAttribute();
#line 190 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"
	unsigned int Deref;
	const char* MinVal;
	const char* MaxVal;
};

[repeatable]
[source_annotation_attribute( Parameter|ReturnValue )]
struct PostRangeAttribute
{

	PostRangeAttribute();
#line 202 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"
	unsigned int Deref;
	const char* MinVal;
	const char* MaxVal;
};

#line 208 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"


};  
#line 212 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"






















typedef ::vc_attributes::YesNoMaybe SA_YesNoMaybe;
const ::vc_attributes::YesNoMaybe SA_Yes = ::vc_attributes::Yes;
const ::vc_attributes::YesNoMaybe SA_No = ::vc_attributes::No;
const ::vc_attributes::YesNoMaybe SA_Maybe = ::vc_attributes::Maybe;

typedef ::vc_attributes::AccessType SA_AccessType;
const ::vc_attributes::AccessType SA_NoAccess = ::vc_attributes::NoAccess;
const ::vc_attributes::AccessType SA_Read = ::vc_attributes::Read;
const ::vc_attributes::AccessType SA_Write = ::vc_attributes::Write;
const ::vc_attributes::AccessType SA_ReadWrite = ::vc_attributes::ReadWrite;


typedef ::vc_attributes::PreAttribute          SA_Pre;
typedef ::vc_attributes::PostAttribute         SA_Post;
typedef ::vc_attributes::FormatStringAttribute SA_FormatString;
typedef ::vc_attributes::InvalidCheckAttribute SA_InvalidCheck; 
typedef ::vc_attributes::SuccessAttribute      SA_Success;
typedef ::vc_attributes::PreBoundAttribute     SA_PreBound;
typedef ::vc_attributes::PostBoundAttribute    SA_PostBound;
typedef ::vc_attributes::PreRangeAttribute     SA_PreRange;
typedef ::vc_attributes::PostRangeAttribute    SA_PostRange;
#line 256 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"















#line 272 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

#line 274 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"




















#line 295 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"


#line 298 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\codeanalysis\\sourceannotations.h"

#line 933 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"













































































































































































































































































































































#line 1267 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"



















































































































































































































#line 1479 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"
extern "C" {




#line 1485 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"




























































































































































































































#line 1706 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
#line 1740 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"



































































































































































































































    
    
#line 1970 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"






#line 1977 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"
#line 1978 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"


}
#line 1982 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\sal.h"



#line 69 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"























































































#pragma pack(push,8)
#line 158 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"













#pragma once
#line 16 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"






#line 23 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"









#pragma pack(push,8)
#line 34 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"


extern "C" {
#line 38 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"








#line 47 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"





typedef __w64 unsigned int   uintptr_t;
#line 54 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"

#line 56 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"





typedef char *  va_list;
#line 63 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"

#line 65 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"





#line 71 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"











#line 83 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"


#line 86 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"













#line 100 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"












































#line 145 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"


}
#line 149 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"


#pragma pack(pop)
#line 153 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"

#line 155 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\vadefs.h"
#line 160 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


extern "C" {
#line 164 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





#line 170 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 175 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 180 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"







#line 188 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 195 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 197 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 198 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 205 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 207 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 208 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"













#line 222 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 223 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





#line 229 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"







#line 237 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 239 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 241 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"







#line 249 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 250 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 255 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 257 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 258 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 263 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 265 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 266 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


 

#line 271 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
  
 #line 273 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 274 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"










#line 285 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 286 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 293 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 294 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

















#line 312 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 317 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 324 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





#line 330 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 337 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 338 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





#line 344 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 351 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 352 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 359 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 360 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 365 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


#line 368 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 370 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 371 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 372 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





#line 378 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"










#line 389 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 391 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 392 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 393 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"










#line 404 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 411 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 412 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"







#line 420 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 422 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 423 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"



 
  
 



#line 433 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 434 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


 
  
  
 



#line 444 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 445 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


 
  
   
  

#line 453 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
 



#line 458 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 459 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





#line 465 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 470 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 472 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 473 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"












typedef size_t rsize_t;

#line 488 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 489 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





typedef __w64 int            intptr_t;
#line 496 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 498 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"














typedef __w64 int            ptrdiff_t;
#line 514 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 516 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"







typedef unsigned short wint_t;
typedef unsigned short wctype_t;

#line 527 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





















__declspec(deprecated("This name was supported during some Whidbey pre-releases. Instead, use the standard name errno_t.")) typedef int errcode;


#line 552 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

typedef int errno_t;
#line 555 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


typedef __w64 long __time32_t;   

#line 560 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


typedef __int64 __time64_t;     

#line 565 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"





typedef __time64_t time_t;      
#line 572 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 574 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"







#line 582 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 583 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 588 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 590 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 591 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 596 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 598 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 599 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 606 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 607 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 612 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"



#line 616 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"











#line 628 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 633 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"




#line 638 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 640 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 641 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"







#line 649 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


































































































































































#line 812 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 813 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"









































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 1879 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"















































































































































#line 2023 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"
#line 2024 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct * pthreadlocinfo;
typedef struct threadmbcinfostruct * pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct
{
    pthreadlocinfo locinfo;
    pthreadmbcinfo mbcinfo;
} _locale_tstruct, *_locale_t;


typedef struct tagLC_ID {
        unsigned short wLanguage;
        unsigned short wCountry;
        unsigned short wCodePage;
} LC_ID, *LPLC_ID;

#line 2045 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


typedef struct threadlocaleinfostruct {
        int refcount;
        unsigned int lc_codepage;
        unsigned int lc_collate_cp;
        unsigned long lc_handle[6]; 
        LC_ID lc_id[6];
        struct {
            char *locale;
            wchar_t *wlocale;
            int *refcount;
            int *wrefcount;
        } lc_category[6];
        int lc_clike;
        int mb_cur_max;
        int * lconv_intl_refcount;
        int * lconv_num_refcount;
        int * lconv_mon_refcount;
        struct lconv * lconv;
        int * ctype1_refcount;
        unsigned short * ctype1;
        const unsigned short * pctype;
        const unsigned char * pclmap;
        const unsigned char * pcumap;
        struct __lc_time_data * lc_time_curr;
} threadlocinfo;

#line 2074 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


}
#line 2078 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"



#line 2082 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 2084 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"



#line 2088 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 2090 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"



#line 2094 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 2096 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"






#line 2103 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"



#line 2107 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"


#pragma pack(pop)
#line 2111 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 2113 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"

#line 23 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"





#pragma pack(push,8)
#line 30 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


extern "C" {
#line 34 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

























struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;

#line 72 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"










#line 83 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"













#line 97 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"





















#line 119 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"







#line 127 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
#line 128 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"




 FILE * __cdecl __iob_func(void);
#line 134 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"










#line 145 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"



typedef __int64 fpos_t;




#line 154 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
#line 155 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


#line 158 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"






#line 165 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
























[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _filbuf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File );
  int __cdecl _flsbuf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);




[returnvalue:SA_Post(MustCheck=SA_Yes)]  FILE * __cdecl _fsopen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _ShFlag);
#line 197 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

 void __cdecl clearerr([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);

  errno_t __cdecl clearerr_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File );
#line 202 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl fclose([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl _fcloseall(void);




[returnvalue:SA_Post(MustCheck=SA_Yes)]  FILE * __cdecl _fdopen([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _FileHandle, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Mode);
#line 210 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl feof([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] FILE * _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl ferror([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] FILE * _File);
  int __cdecl fflush([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl fgetc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl _fgetchar(void);
  int __cdecl fgetpos([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File , [SA_Pre(Null=SA_No,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] fpos_t * _Pos);
  char * __cdecl fgets([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _MaxCount, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);




[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _fileno([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] FILE * _File);
#line 224 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"




#line 229 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  char * __cdecl _tempnam([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _DirName, [SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _FilePrefix);



#line 235 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

  int __cdecl _flushall(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "fopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  FILE * __cdecl fopen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Mode);

  errno_t __cdecl fopen_s([SA_Pre(Null=SA_No,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Pre(Deref=2,Valid=SA_No)] [SA_Post(Deref=1,Null=SA_Maybe)] [SA_Post(Deref=2,Valid=SA_Yes)] FILE ** _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Mode);
#line 241 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl fprintf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);

  int __cdecl fprintf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
#line 245 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl fputc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl _fputchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch);
  int __cdecl fputs([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  size_t __cdecl fread([SA_Pre(Null=SA_No,WritableBytes="\n@""_ElementSize*_Count")] [SA_Pre(Deref=1,Valid=SA_No)] void * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _ElementSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);

  size_t __cdecl fread_s([SA_Pre(Null=SA_No,WritableBytes="\n@""_ElementSize*_Count")] [SA_Pre(Deref=1,Valid=SA_No)] void * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _ElementSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
#line 252 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "freopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  FILE * __cdecl freopen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);

  errno_t __cdecl freopen_s([SA_Pre(Null=SA_No,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Pre(Deref=2,Valid=SA_No)] [SA_Post(Deref=1,Null=SA_Maybe)] [SA_Post(Deref=2,Valid=SA_Yes)] FILE ** _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _OldFile);
#line 256 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "fscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl fscanf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const char * _Format, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_fscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _fscanf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
#pragma warning(push)
#pragma warning(disable:6530)

  int __cdecl fscanf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char * _Format, ...);
#line 263 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl _fscanf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl fsetpos([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const fpos_t * _Pos);
  int __cdecl fseek([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] long _Offset, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Origin);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  long __cdecl ftell([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);

  int __cdecl _fseeki64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] __int64 _Offset, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Origin);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __int64 __cdecl _ftelli64([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);

  size_t __cdecl fwrite([SA_Pre(Null=SA_No,ValidElements="\n@""_Size*_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl getc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl getchar(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _getmaxstdio(void);

 char * __cdecl gets_s([SA_Pre(Null=SA_No,WritableElements="_Size")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _Size);
#line 278 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
extern "C++" { template <size_t _Size> inline char * __cdecl gets_s(char (&_Buffer)[_Size]) { return gets_s(_Buffer, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "gets_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl gets([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] [SA_Pre(Null=SA_No,WritableElementsConst=((size_t)-1))] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Buffer);
[returnvalue:SA_Post(MustCheck=SA_Yes)] int __cdecl _getw([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);


 void __cdecl perror([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _ErrMsg);
#line 285 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl _pclose([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  FILE * __cdecl _popen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Command, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Mode);
  int __cdecl printf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);

  int __cdecl printf_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
#line 291 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl putc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl putchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch);
  int __cdecl puts([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str);
  int __cdecl _putw([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Word, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);


[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl remove([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Filename);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl rename([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _OldFilename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _NewFilename);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _unlink([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Filename);

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_unlink" ". See online help for details."))  int __cdecl unlink([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Filename);
#line 303 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
#line 304 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
 void __cdecl rewind([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl _rmtmp(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "scanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl scanf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const char * _Format, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_scanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _scanf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);

  int __cdecl scanf_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char * _Format, ...);
#line 311 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl _scanf_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
__declspec(deprecated("This function or variable may be unsafe. Consider using " "setvbuf" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  void __cdecl setbuf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_Maybe,WritableElementsConst=512)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] [SA_Post(ValidElementsConst=0)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Buffer);
  int __cdecl _setmaxstdio([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Max);
  unsigned int __cdecl _set_output_format([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Format);
  unsigned int __cdecl _get_output_format(void);
  int __cdecl setvbuf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes,WritableBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
  int __cdecl _snprintf_s([SA_Pre(Null=SA_No,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
extern "C++" { __pragma(warning(push)); __pragma(warning(disable: 4793)); template <size_t _Size> inline int __cdecl _snprintf_s(char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...) { va_list _ArgList; ( _ArgList = (va_list)( &reinterpret_cast<const char &>(_Format) ) + ( (sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1) ) ); return _vsnprintf_s(_Dest, _Size, _MaxCount, _Format, _ArgList); } __pragma(warning(pop)); }

  int __cdecl sprintf_s([SA_Pre(Null=SA_No,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
#line 322 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
extern "C++" { __pragma(warning(push)); __pragma(warning(disable: 4793)); template <size_t _Size> inline int __cdecl sprintf_s(char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...) { va_list _ArgList; ( _ArgList = (va_list)( &reinterpret_cast<const char &>(_Format) ) + ( (sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1) ) ); return vsprintf_s(_Dest, _Size, _Format, _ArgList); } __pragma(warning(pop)); }
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _scprintf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "sscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl sscanf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const char * _Format, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_sscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _sscanf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);

  int __cdecl sscanf_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char * _Format, ...);
#line 329 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl _sscanf_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _snscanf([SA_Pre(Null=SA_No,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const char * _Format, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _snscanf_l([SA_Pre(Null=SA_No,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _snscanf_s([SA_Pre(Null=SA_No,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char * _Format, ...);
  int __cdecl _snscanf_s_l([SA_Pre(Null=SA_No,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "tmpfile_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  FILE * __cdecl tmpfile(void);

  errno_t __cdecl tmpfile_s([SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(Deref=1,Null=SA_No)] [SA_Post(Deref=2,Valid=SA_Yes)] FILE ** _File);
  errno_t __cdecl tmpnam_s([SA_Pre(Null=SA_No,WritableElements="_Size")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _Size);
#line 339 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl tmpnam_s([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Buf)[_Size]) { return tmpnam_s(_Buf, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "tmpnam_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl tmpnam([SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Buffer);
  int __cdecl ungetc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl vfprintf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);

  int __cdecl vfprintf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
#line 346 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl vprintf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);

  int __cdecl vprintf_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
#line 350 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "vsnprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl vsnprintf([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);

  int __cdecl vsnprintf_s([SA_Pre(Null=SA_No,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
#line 354 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl _vsnprintf_s([SA_Pre(Null=SA_No,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
extern "C++" { template <size_t _Size> inline int __cdecl _vsnprintf_s(char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _Args) { return _vsnprintf_s(_Dest, _Size, _MaxCount, _Format, _Args); } }
#pragma warning(push)
#pragma warning(disable:4793)
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_snprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _snprintf([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char *_Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...); __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _vsnprintf([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char *_Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _Args);
#pragma warning(pop)

 int __cdecl vsprintf_s([SA_Pre(Null=SA_No,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
#line 363 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
extern "C++" { template <size_t _Size> inline int __cdecl vsprintf_s(char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _Args) { return vsprintf_s(_Dest, _Size, _Format, _Args); } }
#pragma warning(push)
#pragma warning(disable:4793)
__declspec(deprecated("This function or variable may be unsafe. Consider using " "sprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl sprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...); __declspec(deprecated("This function or variable may be unsafe. Consider using " "vsprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl vsprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _Args);
#pragma warning(pop)
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _vscprintf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
  int __cdecl _snprintf_c([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
  int __cdecl _vsnprintf_c([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] char *_DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);

  int __cdecl _fprintf_p([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
  int __cdecl _printf_p([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
  int __cdecl _sprintf_p([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
  int __cdecl _vfprintf_p([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
  int __cdecl _vprintf_p([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
  int __cdecl _vsprintf_p([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _scprintf_p([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _vscprintf_p([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, va_list _ArgList);
 int __cdecl _set_printf_count_output([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Value);
 int __cdecl _get_printf_count_output();

  int __cdecl _printf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _printf_p_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _printf_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _vprintf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vprintf_p_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vprintf_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

  int __cdecl _fprintf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _fprintf_p_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _fprintf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _vfprintf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vfprintf_p_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vfprintf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_sprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _sprintf_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _sprintf_p_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _sprintf_s_l([SA_Pre(Null=SA_No,WritableBytes="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _vsprintf_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t, va_list _ArgList);
  int __cdecl _vsprintf_p_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale,  va_list _ArgList);
  int __cdecl _vsprintf_s_l([SA_Pre(Null=SA_No,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

  int __cdecl _scprintf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _scprintf_p_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _vscprintf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vscprintf_p_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _snprintf_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _snprintf_c_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _snprintf_s_l([SA_Pre(Null=SA_No,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _vsnprintf_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vsnprintf_c_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, const char *, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vsnprintf_s_l([SA_Pre(Null=SA_No,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const char* _Format,[SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);







#line 423 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"




[returnvalue:SA_Post(MustCheck=SA_Yes)]  FILE * __cdecl _wfsopen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _ShFlag);
#line 429 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

  wint_t __cdecl fgetwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  wint_t __cdecl _fgetwchar(void);
  wint_t __cdecl fputwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  wint_t __cdecl _fputwchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  wint_t __cdecl getwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  wint_t __cdecl getwchar(void);
  wint_t __cdecl putwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  wint_t __cdecl putwchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Ch);
  wint_t __cdecl ungetwc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wint_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);

  wchar_t * __cdecl fgetws([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl fputws([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  wchar_t * __cdecl _getws_s([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords);
extern "C++" { template <size_t _Size> inline wchar_t * __cdecl _getws_s(wchar_t (&_String)[_Size]) { return _getws_s(_String, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_getws_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _getws([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_String);
  int __cdecl _putws([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str);

  int __cdecl fwprintf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);

  int __cdecl fwprintf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
#line 451 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl wprintf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);

  int __cdecl wprintf_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
#line 455 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _scwprintf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
  int __cdecl vfwprintf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);

  int __cdecl vfwprintf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);
#line 460 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl vwprintf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);

  int __cdecl vwprintf_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);
#line 464 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


 int __cdecl swprintf_s([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
#line 468 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
extern "C++" { __pragma(warning(push)); __pragma(warning(disable: 4793)); template <size_t _Size> inline int __cdecl swprintf_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...) { va_list _ArgList; ( _ArgList = (va_list)( &reinterpret_cast<const char &>(_Format) ) + ( (sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1) ) ); return vswprintf_s(_Dest, _Size, _Format, _ArgList); } __pragma(warning(pop)); }

 int __cdecl vswprintf_s([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);
#line 472 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
extern "C++" { template <size_t _Size> inline int __cdecl vswprintf_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _Args) { return vswprintf_s(_Dest, _Size, _Format, _Args); } }

  int __cdecl _swprintf_c([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
  int __cdecl _vswprintf_c([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);

  int __cdecl _snwprintf_s([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
extern "C++" { __pragma(warning(push)); __pragma(warning(disable: 4793)); template <size_t _Size> inline int __cdecl _snwprintf_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...) { va_list _ArgList; ( _ArgList = (va_list)( &reinterpret_cast<const char &>(_Format) ) + ( (sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1) ) ); return _vsnwprintf_s(_Dest, _Size, _Count, _Format, _ArgList); } __pragma(warning(pop)); }
  int __cdecl _vsnwprintf_s([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);
extern "C++" { template <size_t _Size> inline int __cdecl _vsnwprintf_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _Args) { return _vsnwprintf_s(_Dest, _Size, _Count, _Format, _Args); } }
#pragma warning(push)
#pragma warning(disable:4793)
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _snwprintf([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t *_Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...); __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnwprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _vsnwprintf([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t *_Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _Args);
#pragma warning(pop)

  int __cdecl _fwprintf_p([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
  int __cdecl _wprintf_p([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
  int __cdecl _vfwprintf_p([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);
  int __cdecl _vwprintf_p([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);
  int __cdecl _swprintf_p([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
  int __cdecl _vswprintf_p([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _scwprintf_p([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _vscwprintf_p([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);

  int __cdecl _wprintf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _wprintf_p_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _wprintf_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _vwprintf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vwprintf_p_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vwprintf_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

  int __cdecl _fwprintf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _fwprintf_p_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _fwprintf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _vfwprintf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vfwprintf_p_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vfwprintf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

  int __cdecl _swprintf_c_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _swprintf_p_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _swprintf_s_l([SA_Pre(Null=SA_No,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _vswprintf_c_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vswprintf_p_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vswprintf_s_l([SA_Pre(Null=SA_No,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _scwprintf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _scwprintf_p_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _vscwprintf_p_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);

 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _snwprintf_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _snwprintf_s_l([SA_Pre(Null=SA_No,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vsnwprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _vsnwprintf_l([SA_Pre(Null=SA_No,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
  int __cdecl _vsnwprintf_s_l([SA_Pre(Null=SA_No,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);










#line 535 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


#pragma warning(push)
#pragma warning(disable:4141 4996 4793)
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_swprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS."))  int __cdecl _swprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...); __declspec(deprecated("This function or variable may be unsafe. Consider using " "vswprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS."))  int __cdecl _vswprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _Args);
__declspec(deprecated("This function or variable may be unsafe. Consider using " "__swprintf_l_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS."))  int __cdecl __swprintf_l( wchar_t *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, _locale_t _Plocinfo, ...); __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vswprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS."))  int __cdecl __vswprintf_l( wchar_t *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, _locale_t _Plocinfo, va_list _Args);
#pragma warning(pop)


#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\swprintf.inl"












#pragma once







#line 22 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\swprintf.inl"










#line 33 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\swprintf.inl"

#pragma warning( push )
#pragma warning( disable : 4793 4412 )
static __inline int swprintf(wchar_t * _String, size_t _Count, const wchar_t * _Format, ...)
{
    va_list _Arglist;
    int _Ret;
    ( _Arglist = (va_list)( &reinterpret_cast<const char &>(_Format) ) + ( (sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1) ) );
    _Ret = _vswprintf_c_l(_String, _Count, _Format, 0, _Arglist);
    ( _Arglist = (va_list)0 );
    return _Ret;
}
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4412 )
static __inline int __cdecl vswprintf(wchar_t * _String, size_t _Count, const wchar_t * _Format, va_list _Ap)
{
    return _vswprintf_c_l(_String, _Count, _Format, 0, _Ap);
}
#pragma warning( pop )


#line 57 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\swprintf.inl"

#pragma warning( push )
#pragma warning( disable : 4793 4412 )
static __inline int _swprintf_l(wchar_t * _String, size_t _Count, const wchar_t * _Format, _locale_t _Plocinfo, ...)
{
    va_list _Arglist;
    int _Ret;
    ( _Arglist = (va_list)( &reinterpret_cast<const char &>(_Plocinfo) ) + ( (sizeof(_Plocinfo) + sizeof(int) - 1) & ~(sizeof(int) - 1) ) );
    _Ret = _vswprintf_c_l(_String, _Count, _Format, _Plocinfo, _Arglist);
    ( _Arglist = (va_list)0 );
    return _Ret;
}
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4412 )
static __inline int __cdecl _vswprintf_l(wchar_t * _String, size_t _Count, const wchar_t * _Format, _locale_t _Plocinfo, va_list _Ap)
{
    return _vswprintf_c_l(_String, _Count, _Format, _Plocinfo, _Ap);
}
#pragma warning( pop )


#pragma warning( push )
#pragma warning( disable : 4996 )

#pragma warning( push )
#pragma warning( disable : 4793 4141 )
extern "C++" __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "swprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline int swprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _String, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, ...)
{
    va_list _Arglist;
    ( _Arglist = (va_list)( &reinterpret_cast<const char &>(_Format) ) + ( (sizeof(_Format) + sizeof(int) - 1) & ~(sizeof(int) - 1) ) );
    int _Ret = _vswprintf(_String, _Format, _Arglist);
    ( _Arglist = (va_list)0 );
    return _Ret;
}
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4141 )
extern "C++" __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "vswprintf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline int __cdecl vswprintf([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _String, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _Ap)
{
    return _vswprintf(_String, _Format, _Ap);
}
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4793 4141 )
extern "C++" __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_swprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline int _swprintf_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _String, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, _locale_t _Plocinfo, ...)
{
    va_list _Arglist;
    ( _Arglist = (va_list)( &reinterpret_cast<const char &>(_Plocinfo) ) + ( (sizeof(_Plocinfo) + sizeof(int) - 1) & ~(sizeof(int) - 1) ) );
    int _Ret = __vswprintf_l(_String, _Format, _Plocinfo, _Arglist);
    ( _Arglist = (va_list)0 );
    return _Ret;
}
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4141 )
extern "C++" __declspec(deprecated("swprintf has been changed to conform with the ISO C standard, adding an extra character count parameter. To use traditional Microsoft swprintf, set _CRT_NON_CONFORMING_SWPRINTFS.")) __declspec(deprecated("This function or variable may be unsafe. Consider using " "_vswprintf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline int __cdecl _vswprintf_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _String, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, _locale_t _Plocinfo, va_list _Ap)
{
    return __vswprintf_l(_String, _Format, _Plocinfo, _Ap);
}
#pragma warning( pop )

#pragma warning( pop )

#line 126 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\swprintf.inl"

#line 128 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\swprintf.inl"
#line 129 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\swprintf.inl"

#line 545 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
#line 546 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"













#line 560 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  wchar_t * __cdecl _wtempnam([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Directory, [SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _FilePrefix);



#line 566 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _vscwprintf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _vscwprintf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="printf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, va_list _ArgList);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "fwscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl fwscanf([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t * _Format, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_fwscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _fwscanf_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);

  int __cdecl fwscanf_s([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t * _Format, ...);
#line 574 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl _fwscanf_s_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "swscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl swscanf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t * _Format, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_swscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _swscanf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);

  int __cdecl swscanf_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t *_Src, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t * _Format, ...);
#line 580 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl _swscanf_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _snwscanf([SA_Pre(Null=SA_No,ValidElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t * _Format, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_snwscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _snwscanf_l([SA_Pre(Null=SA_No,ValidElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
  int __cdecl _snwscanf_s([SA_Pre(Null=SA_No,ValidElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t * _Format, ...);
  int __cdecl _snwscanf_s_l([SA_Pre(Null=SA_No,ValidElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "wscanf_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl wscanf([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t * _Format, ...);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wscanf_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  int __cdecl _wscanf_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);

  int __cdecl wscanf_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t * _Format, ...);
#line 590 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
  int __cdecl _wscanf_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_FormatString(Style="scanf_s")] const wchar_t * _Format, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale, ...);
#pragma warning(pop)

[returnvalue:SA_Post(MustCheck=SA_Yes)]  FILE * __cdecl _wfdopen([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _FileHandle , [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Mode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wfopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  FILE * __cdecl _wfopen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Mode);
  errno_t __cdecl _wfopen_s([SA_Pre(Null=SA_No,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Pre(Deref=2,Valid=SA_No)] [SA_Post(Deref=1,Null=SA_Maybe)] [SA_Post(Deref=2,Valid=SA_Yes)] FILE ** _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Mode);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wfreopen_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  FILE * __cdecl _wfreopen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _OldFile);
  errno_t __cdecl _wfreopen_s([SA_Pre(Null=SA_No,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Pre(Deref=2,Valid=SA_No)] [SA_Post(Deref=1,Null=SA_Maybe)] [SA_Post(Deref=2,Valid=SA_Yes)] FILE ** _File, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Filename, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Mode, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _OldFile);



 void __cdecl _wperror([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _ErrMsg);
#line 603 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
[returnvalue:SA_Post(MustCheck=SA_Yes)]  FILE * __cdecl _wpopen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t *_Command, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Mode);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wremove([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Filename);
  errno_t __cdecl _wtmpnam_s([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _wtmpnam_s([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_Buffer)[_Size]) { return _wtmpnam_s(_Buffer, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_wtmpnam_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _wtmpnam([SA_Pre(Null=SA_Maybe)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_Buffer);

  wint_t __cdecl _fgetwc_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  wint_t __cdecl _fputwc_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  wint_t __cdecl _ungetwc_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wint_t _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);






#line 619 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"
inline wint_t __cdecl getwchar()
        {return (fgetwc((&__iob_func()[0]))); }   
inline wint_t __cdecl putwchar(wchar_t _C)
        {return (fputwc(_C, (&__iob_func()[1]))); }       
#line 624 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"










#line 635 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


#line 638 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


#line 641 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"















#line 657 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"












 void __cdecl _lock_file([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
 void __cdecl _unlock_file([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);



#line 675 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


  int __cdecl _fclose_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl _fflush_nolock([SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  size_t __cdecl _fread_nolock([SA_Pre(Null=SA_No,WritableBytes="\n@""_ElementSize*_Count")] [SA_Pre(Deref=1,Valid=SA_No)] void * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _ElementSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  size_t __cdecl _fread_nolock_s([SA_Pre(Null=SA_No,WritableBytes="\n@""_ElementSize*_Count")] [SA_Pre(Deref=1,Valid=SA_No)] void * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _ElementSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl _fseek_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] long _Offset, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Origin);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  long __cdecl _ftell_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl _fseeki64_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] __int64 _Offset, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Origin);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __int64 __cdecl _ftelli64_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  size_t __cdecl _fwrite_nolock([SA_Pre(Null=SA_No,ValidBytes="\n@""_Size*_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _DstBuf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
  int __cdecl _ungetc_nolock([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);












#line 700 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"











#line 712 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_tempnam" ". See online help for details."))  char * __cdecl tempnam([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Directory, [SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _FilePrefix);



#line 718 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

 __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fcloseall" ". See online help for details."))  int __cdecl fcloseall(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fdopen" ". See online help for details."))  FILE * __cdecl fdopen([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _FileHandle, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Format);
 __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fgetchar" ". See online help for details."))  int __cdecl fgetchar(void);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fileno" ". See online help for details."))  int __cdecl fileno([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] FILE * _File);
 __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_flushall" ". See online help for details."))  int __cdecl flushall(void);
 __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_fputchar" ". See online help for details."))  int __cdecl fputchar([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_getw" ". See online help for details."))  int __cdecl getw([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
 __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_putw" ". See online help for details."))  int __cdecl putw([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] FILE * _File);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_rmtmp" ". See online help for details."))  int __cdecl rmtmp(void);

#line 730 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


}
#line 734 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"


#pragma pack(pop)
#line 738 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

#line 740 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\stdio.h"

#line 11 "f:\\svn\\miscellaneous\\quotserver\\stdafx.h"
#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"
















#pragma once
#line 19 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"

#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"














 


















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 21 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"









#pragma warning(disable:4514)       
 
#line 33 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"


















extern "C" {
#line 53 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"







#pragma deprecated("_ftcscat")
#pragma deprecated("_ftcschr")
#pragma deprecated("_ftcscpy")
#pragma deprecated("_ftcscspn")
#pragma deprecated("_ftcslen")
#pragma deprecated("_ftcsncat")
#pragma deprecated("_ftcsncpy")
#pragma deprecated("_ftcspbrk")
#pragma deprecated("_ftcsrchr")
#pragma deprecated("_ftcsspn")
#pragma deprecated("_ftcsstr")
#pragma deprecated("_ftcstok")
#pragma deprecated("_ftcsdup")
#pragma deprecated("_ftcsnset")
#pragma deprecated("_ftcsrev")
#pragma deprecated("_ftcsset")
#pragma deprecated("_ftcscmp")
#pragma deprecated("_ftcsicmp")
#pragma deprecated("_ftcsnccmp")
#pragma deprecated("_ftcsncmp")
#pragma deprecated("_ftcsncicmp")
#pragma deprecated("_ftcsnicmp")
#pragma deprecated("_ftcscoll")
#pragma deprecated("_ftcsicoll")
#pragma deprecated("_ftcsnccoll")
#pragma deprecated("_ftcsncoll")
#pragma deprecated("_ftcsncicoll")
#pragma deprecated("_ftcsnicoll")
#pragma deprecated("_ftcsclen")
#pragma deprecated("_ftcsnccat")
#pragma deprecated("_ftcsnccpy")
#pragma deprecated("_ftcsncset")
#pragma deprecated("_ftcsdec")
#pragma deprecated("_ftcsinc")
#pragma deprecated("_ftcsnbcnt")
#pragma deprecated("_ftcsnccnt")
#pragma deprecated("_ftcsnextc")
#pragma deprecated("_ftcsninc")
#pragma deprecated("_ftcsspnp")
#pragma deprecated("_ftcslwr")
#pragma deprecated("_ftcsupr")
#pragma deprecated("_ftclen")
#pragma deprecated("_ftccpy")
#pragma deprecated("_ftccmp")
#line 105 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"





































































































































































































































































































































































































































































































































































































































































































































































}   
#line 848 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"

#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"















#pragma once
#line 18 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"




#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"














 


















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 23 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"


extern "C" {
#line 27 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"




#line 32 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
















 void *  __cdecl _memccpy( [SA_Pre(Null=SA_Maybe,WritableBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] void * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const void * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  const void *  __cdecl memchr( [SA_Pre(Null=SA_Maybe,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf , [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _memicmp([SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf1, [SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _memicmp_l([SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf1, [SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
        [returnvalue:SA_Post(MustCheck=SA_Yes)] int     __cdecl memcmp([SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf1, [SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
         void *  __cdecl memcpy([SA_Pre(Null=SA_Maybe,WritableBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(ValidBytes="_Size")] [SA_Post(Deref=1,Valid=SA_Yes)] void * _Dst, [SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);

 errno_t  __cdecl memcpy_s([SA_Pre(Null=SA_Maybe,WritableBytes="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(ValidBytes="_MaxCount")] [SA_Post(Deref=1,Valid=SA_Yes)] void * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _DstSize, [SA_Pre(Null=SA_Maybe,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _MaxCount);
#line 57 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
        void *  __cdecl memset([SA_Pre(Null=SA_Maybe,WritableBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(ValidBytes="_Size")] [SA_Post(Deref=1,Valid=SA_Yes)] void * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);



__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_memccpy" ". See online help for details."))  void * __cdecl memccpy([SA_Pre(Null=SA_Maybe,WritableBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_No)] void * _Dst, [SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_memicmp" ". See online help for details."))  int __cdecl memicmp([SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf1, [SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Buf2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
#line 64 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

#line 66 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

  errno_t __cdecl _strset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_DstSize")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSize, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Value);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _strset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Value) { return _strset_s(_Dest, _Size, _Value); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_strset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl _strset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Value);

  errno_t __cdecl strcpy_s([SA_Pre(Null=SA_No,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src);
#line 73 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl strcpy_s([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source) { return strcpy_s(_Dest, _Size, _Source); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "strcpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl strcpy([SA_Pre(Null=SA_No,WritableElementsLength="_Source")] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source);

  errno_t __cdecl strcat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src);
#line 78 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl strcat_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source) { return strcat_s(_Dest, _Size, _Source); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "strcat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl strcat([SA_Pre(Null=SA_No,WritableElementsLength="_Source")] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source);
        [returnvalue:SA_Post(MustCheck=SA_Yes)] int     __cdecl strcmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2);
        [returnvalue:SA_Post(MustCheck=SA_Yes)] size_t  __cdecl strlen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t  __cdecl strnlen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);

[returnvalue:SA_Post(MustCheck=SA_Yes)] static __inline size_t  __cdecl strnlen_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount)
{
    return (_Str==0) ? 0 : strnlen(_Str, _MaxCount);
}
#line 89 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

  errno_t __cdecl memmove_s([SA_Pre(Null=SA_Maybe,WritableBytes="_DstSize")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(ValidBytes="_MaxCount")] [SA_Post(Deref=1,Valid=SA_Yes)] void * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _DstSize, [SA_Pre(Null=SA_Maybe,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _MaxCount);
#line 92 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"



#line 96 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
  void *  __cdecl memmove([SA_Pre(Null=SA_Maybe,WritableBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(ValidBytes="_Size")] [SA_Post(Deref=1,Valid=SA_Yes)] void * _Dst, [SA_Pre(Null=SA_Maybe,ValidBytes="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const void * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
#line 98 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"




#line 103 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  char *  __cdecl _strdup([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src);



#line 109 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  const char *  __cdecl strchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _stricmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _strcmpi([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _stricmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl strcoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const  char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _strcoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _stricoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _stricoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _strncoll  ([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _strncoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _strnicoll ([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _strnicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t  __cdecl strcspn([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)]  const char * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_strerror_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char *  __cdecl _strerror([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _ErrMsg);
  errno_t __cdecl _strerror_s([SA_Pre(Null=SA_No,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _ErrMsg);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _strerror_s(char (&_Buffer)[_Size], [SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _ErrorMessage) { return _strerror_s(_Buffer, _Size, _ErrorMessage); } }
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "strerror_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char *  __cdecl strerror([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int);

  errno_t __cdecl strerror_s([SA_Pre(Null=SA_No,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _ErrNum);
#line 130 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl strerror_s(char (&_Buffer)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _ErrorMessage) { return strerror_s(_Buffer, _Size, _ErrorMessage); } }
  errno_t __cdecl _strlwr_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _strlwr_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_String)[_Size]) { return _strlwr_s(_String, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_strlwr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl _strlwr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_String);
  errno_t __cdecl _strlwr_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _strlwr_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_String)[_Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _strlwr_s_l(_String, _Size, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_strlwr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl _strlwr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);

  errno_t __cdecl strncat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _MaxCount);
#line 140 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl strncat_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return strncat_s(_Dest, _Size, _Source, _Count); } }
#pragma warning(push)
#pragma warning(disable:6059)

__declspec(deprecated("This function or variable may be unsafe. Consider using " "strncat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl strncat([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
#pragma warning(pop)


#line 149 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl strncmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
#line 151 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _strnicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int     __cdecl _strnicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);

  errno_t __cdecl strncpy_s([SA_Pre(Null=SA_No,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _MaxCount);
#line 156 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl strncpy_s(char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return strncpy_s(_Dest, _Size, _Source, _Count); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "strncpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl strncpy([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
  errno_t __cdecl _strnset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _strnset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _strnset_s(_Dest, _Size, _Val, _Count); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_strnset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl _strnset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dest, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  const char *  __cdecl strpbrk([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  const char *  __cdecl strrchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch);
 char *  __cdecl _strrev([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t  __cdecl strspn([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  const char *  __cdecl strstr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _SubStr);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "strtok_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char *  __cdecl strtok([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Delim);

[returnvalue:SA_Post(MustCheck=SA_Yes)]  char *  __cdecl strtok_s([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Delim, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char ** _Context);
#line 170 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
  errno_t __cdecl _strupr_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _strupr_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_String)[_Size]) { return _strupr_s(_String, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_strupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl _strupr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_String);
  errno_t __cdecl _strupr_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size, _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _strupr_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_String)[_Size], _locale_t _Locale) { return _strupr_s_l(_String, _Size, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_strupr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  char * __cdecl _strupr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
  size_t  __cdecl strxfrm ([SA_Pre(Null=SA_Maybe,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char * _Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
  size_t  __cdecl _strxfrm_l([SA_Pre(Null=SA_Maybe,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char * _Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);


extern "C++" {


[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl strchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch)
	{ return (char*)strchr((const char*)_Str, _Ch); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl strpbrk([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Control)
	{ return (char*)strpbrk((const char*)_Str, _Control); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl strrchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Ch)
	{ return (char*)strrchr((const char*)_Str, _Ch); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl strstr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _SubStr)
	{ return (char*)strstr((const char*)_Str, _SubStr); }
#line 192 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"


[returnvalue:SA_Post(MustCheck=SA_Yes)] inline void * __cdecl memchr([SA_Pre(Null=SA_Maybe,ValidBytes="_N")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] void * _Pv, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _C, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _N)
	{ return (void*)memchr((const void*)_Pv, _C, _N); }
#line 197 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
}
#line 199 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"






#line 206 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strdup" ". See online help for details."))  char * __cdecl strdup([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Src);



#line 212 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"


[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strcmpi" ". See online help for details."))  int __cdecl strcmpi([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_stricmp" ". See online help for details."))  int __cdecl stricmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str2);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strlwr" ". See online help for details."))  char * __cdecl strlwr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strnicmp" ". See online help for details."))  int __cdecl strnicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strnset" ". See online help for details."))  char * __cdecl strnset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strrev" ". See online help for details."))  char * __cdecl strrev([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strset" ". See online help for details."))         char * __cdecl strset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Val);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_strupr" ". See online help for details."))  char * __cdecl strupr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Str);

#line 224 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"









#line 234 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  wchar_t * __cdecl _wcsdup([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str);



#line 240 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"


  errno_t __cdecl wcscat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src);
#line 244 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl wcscat_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Source) { return wcscat_s(_Dest, _Size, _Source); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "wcscat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl wcscat([SA_Pre(Null=SA_No,WritableElementsLength="_Source")] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Source);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  const wchar_t * __cdecl wcschr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str, wchar_t _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl wcscmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2);

  errno_t __cdecl wcscpy_s([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src);
#line 251 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl wcscpy_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Source) { return wcscpy_s(_Dest, _Size, _Source); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "wcscpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl wcscpy([SA_Pre(Null=SA_No,WritableElementsLength="_Source")] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Source);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl wcscspn([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl wcslen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl wcsnlen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);

[returnvalue:SA_Post(MustCheck=SA_Yes)] static __inline size_t __cdecl wcsnlen_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount)
{
    return (_Src == 0) ? 0 : wcsnlen(_Src, _MaxCount);
}
#line 262 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

  errno_t __cdecl wcsncat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _MaxCount);
#line 265 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl wcsncat_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return wcsncat_s(_Dest, _Size, _Source, _Count); } }
#pragma warning(push)
#pragma warning(disable:6059)
__declspec(deprecated("This function or variable may be unsafe. Consider using " "wcsncat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl wcsncat([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
#pragma warning(pop)
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl wcsncmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);

  errno_t __cdecl wcsncpy_s([SA_Pre(Null=SA_No,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] rsize_t _MaxCount);
#line 274 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
extern "C++" { template <size_t _Size> inline errno_t __cdecl wcsncpy_s(wchar_t (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return wcsncpy_s(_Dest, _Size, _Source, _Count); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "wcsncpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl wcsncpy([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  const wchar_t * __cdecl wcspbrk([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  const wchar_t * __cdecl wcsrchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl wcsspn([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  const wchar_t * __cdecl wcsstr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _SubStr);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "wcstok_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl wcstok([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Delim);

[returnvalue:SA_Post(MustCheck=SA_Yes)]  wchar_t * __cdecl wcstok_s([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Delim, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t ** _Context);
#line 284 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcserror_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _wcserror([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _ErrNum);
  errno_t __cdecl _wcserror_s([SA_Pre(Null=SA_Maybe,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Buf, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _ErrNum);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _wcserror_s(wchar_t (&_Buffer)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _Error) { return _wcserror_s(_Buffer, _Size, _Error); } }
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "__wcserror_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl __wcserror([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str);
  errno_t __cdecl __wcserror_s([SA_Pre(Null=SA_Maybe,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Buffer, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _ErrMsg);
extern "C++" { template <size_t _Size> inline errno_t __cdecl __wcserror_s(wchar_t (&_Buffer)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _ErrorMessage) { return __wcserror_s(_Buffer, _Size, _ErrorMessage); } }

[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsnicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsnicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
  errno_t __cdecl _wcsnset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _wcsnset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_Dst)[_Size], wchar_t _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount) { return _wcsnset_s(_Dst, _Size, _Val, _MaxCount); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsnset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _wcsnset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_Str, wchar_t _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
 wchar_t * __cdecl _wcsrev([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str);
  errno_t __cdecl _wcsset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Value);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _wcsset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_Str)[_Size], wchar_t _Val) { return _wcsset_s(_Str, _Size, _Val); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _wcsset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_Str, wchar_t _Val);

  errno_t __cdecl _wcslwr_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _wcslwr_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_String)[_Size]) { return _wcslwr_s(_String, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcslwr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _wcslwr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_String);
  errno_t __cdecl _wcslwr_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_SizeInWords")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInWords, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _wcslwr_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_String)[_Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _wcslwr_s_l(_String, _Size, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcslwr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _wcslwr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
  errno_t __cdecl _wcsupr_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _wcsupr_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_String)[_Size]) { return _wcsupr_s(_String, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _wcsupr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_String);
  errno_t __cdecl _wcsupr_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_Size")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Size, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _wcsupr_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] wchar_t (&_String)[_Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _wcsupr_s_l(_String, _Size, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_wcsupr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  wchar_t * __cdecl _wcsupr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t *_String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
  size_t __cdecl wcsxfrm([SA_Pre(Null=SA_Maybe,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t * _Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
  size_t __cdecl _wcsxfrm_l([SA_Pre(Null=SA_Maybe,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] wchar_t * _Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t *_Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl wcscoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcscoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t *_Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsncoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsncoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsnicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _wcsnicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);




extern "C++" {
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline wchar_t * __cdecl wcschr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] wchar_t *_Str, wchar_t _Ch)
        {return ((wchar_t *)wcschr((const wchar_t *)_Str, _Ch)); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline wchar_t * __cdecl wcspbrk([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] wchar_t *_Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t *_Control)
        {return ((wchar_t *)wcspbrk((const wchar_t *)_Str, _Control)); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline wchar_t * __cdecl wcsrchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] wchar_t *_Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Ch)
        {return ((wchar_t *)wcsrchr((const wchar_t *)_Str, _Ch)); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline wchar_t * __cdecl wcsstr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] wchar_t *_Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t *_SubStr)
        {return ((wchar_t *)wcsstr((const wchar_t *)_Str, _SubStr)); }
}
#line 340 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
#line 341 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"






#line 348 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsdup" ". See online help for details."))  wchar_t * __cdecl wcsdup([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str);



#line 354 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"





[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsicmp" ". See online help for details."))  int __cdecl wcsicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsnicmp" ". See online help for details."))  int __cdecl wcsnicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsnset" ". See online help for details."))  wchar_t * __cdecl wcsnset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableElements="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] wchar_t _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsrev" ". See online help for details."))  wchar_t * __cdecl wcsrev([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsset" ". See online help for details."))  wchar_t * __cdecl wcsset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str, wchar_t _Val);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcslwr" ". See online help for details."))  wchar_t * __cdecl wcslwr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str);
__declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsupr" ". See online help for details."))  wchar_t * __cdecl wcsupr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] wchar_t * _Str);
[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: " "_wcsicoll" ". See online help for details."))  int __cdecl wcsicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const wchar_t * _Str2);

#line 369 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"


#line 372 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"



}
#line 377 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"

#line 379 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\string.h"
#line 850 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"


extern "C" {
#line 854 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"















#line 870 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"
























































































































































































#line 1055 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"


























































#line 1114 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"





















#line 1136 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"







#line 1144 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"













#line 1158 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"




























#line 1187 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"








































}   
#line 1229 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"

#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"














#pragma once
#line 17 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"




#line 1 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\crtdefs.h"














 


















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 22 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"







#pragma pack(push,8)
#line 31 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"


extern "C" {
#line 35 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"
























#line 60 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsdup([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str);



#line 66 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"

[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbbtombc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbbtombc_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbbtype([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned char _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _CType);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbbtype_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned char _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] int _CType, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctombb([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctombb_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 int __cdecl _mbsbtype([SA_Pre(Null=SA_No,ValidBytes="_Pos")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Pos);
 int __cdecl _mbsbtype_l([SA_Pre(Null=SA_No,ValidBytes="_Pos")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Pos, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbscat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbscat_s(unsigned char (&_Dst)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _DstSizeInBytes) { return _mbscat_s(_Dst, _Size, _DstSizeInBytes); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbscat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbscat([SA_Pre(Null=SA_No,WritableElementsLength="_Source")] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source);
 errno_t __cdecl _mbscat_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbscat_s_l(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbscat_s_l(_Dest, _Size, _Source, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbscat_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbscat_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]   const unsigned char * __cdecl _mbschr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]   const unsigned char * __cdecl _mbschr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbscmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbscmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbscoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbscoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbscpy_s([SA_Pre(Null=SA_No,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbscpy_s(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source) { return _mbscpy_s(_Dest, _Size, _Source); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbscpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbscpy([SA_Pre(Null=SA_No,WritableElementsLength="_Source")] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source);
 errno_t __cdecl _mbscpy_s_l([SA_Pre(Null=SA_No,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbscpy_s(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbscpy_s(_Dest, _Size, _Source, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbscpy_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbscpy_l([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbscspn([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbscspn_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Control, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsdec([SA_Pre(Null=SA_No,ValidElements="\n@""_Pos-_Start +1")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Start, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Pos);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsdec_l([SA_Pre(Null=SA_No,ValidElements="\n@""_Pos-_Start+1")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Start, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Pos, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsinc([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Ptr);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsinc_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Ptr, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbslen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbslen_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbsnlen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbsnlen_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbslwr_s([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbslwr_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes,WritableBytesConst=_Size)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char (&_String)[_Size]) { return _mbslwr_s(_String, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbslwr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbslwr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String);
 errno_t __cdecl _mbslwr_s_l([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbslwr_s_l(unsigned char (&_String)[_Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbslwr_s_l(_String, _Size, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbslwr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbslwr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbsnbcat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsnbcat_s(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _mbsnbcat_s(_Dest, _Size, _Source, _Count); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsnbcat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsnbcat([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
 errno_t __cdecl _mbsnbcat_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsnbcat_s_l(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbsnbcat_s_l(_Dest, _Size, _Source, _Count, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsnbcat_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsnbcat_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnbcmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnbcmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnbcoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnbcoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbsnbcnt([SA_Pre(Null=SA_No,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbsnbcnt_l([SA_Pre(Null=SA_No,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbsnbcpy_s([SA_Pre(Null=SA_No,WritableElements="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsnbcpy_s(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _mbsnbcpy_s(_Dest, _Size, _Source, _Count); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsnbcpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsnbcpy([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
 errno_t __cdecl _mbsnbcpy_s_l([SA_Pre(Null=SA_No,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsnbcpy_s_l(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbsnbcpy_s_l(_Dest, _Size, _Source, _Count, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsnbcpy_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsnbcpy_l([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnbicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnbicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnbicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnbicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbsnbset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsnbset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount) { return _mbsnbset_s(_Dest, _Size, _Val, _MaxCount); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsnbset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsnbset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
 errno_t __cdecl _mbsnbset_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsnbset_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbsnbset_s_l(_Dest, _Size, _Val, _MaxCount, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsnbset_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsnbset_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbsncat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsncat_s(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _mbsncat_s(_Dest, _Size, _Source, _Count); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsncat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsncat([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
 errno_t __cdecl _mbsncat_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsncat_s_l(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbsncat_s_l(_Dest, _Size, _Source, _Count, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsncat_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsncat_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbsnccnt([SA_Pre(Null=SA_No,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbsnccnt_l([SA_Pre(Null=SA_No,ValidBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsncmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsncmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsncoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsncoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbsncpy_s([SA_Pre(Null=SA_No,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsncpy_s(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _mbsncpy_s(_Dest, _Size, _Source, _Count); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsncpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsncpy([SA_Pre(Null=SA_No)] [SA_Pre(Null=SA_No,ElementSizeConst=2,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
 errno_t __cdecl _mbsncpy_s_l([SA_Pre(Null=SA_No,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsncpy_s_l(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbsncpy_s_l(_Dest, _Size, _Source, _Count, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsncpy_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsncpy_l([SA_Pre(Null=SA_No,WritableElements="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] unsigned char *_Dest, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbsnextc ([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbsnextc_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _mbsnicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsninc([SA_Pre(Null=SA_No,ValidBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsninc_l([SA_Pre(Null=SA_No,ValidBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbsnset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsnset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount) { return _mbsnset_s(_Dest, _Size, _Val, _MaxCount); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsnset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsnset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount);
 errno_t __cdecl _mbsnset_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsnset_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbsnset_s_l(_Dest, _Size, _Val, _MaxCount, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsnset_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsnset_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_MaxCount")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _MaxCount, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]   const unsigned char * __cdecl _mbspbrk([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)]   const unsigned char * __cdecl _mbspbrk_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Control, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]   const unsigned char * __cdecl _mbsrchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]   const unsigned char * __cdecl _mbsrchr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 unsigned char * __cdecl _mbsrev([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Str);
 unsigned char * __cdecl _mbsrev_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Str, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbsset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val) { return _mbsset_s(_Dest, _Size, _Val); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val);
 errno_t __cdecl _mbsset_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsset_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbsset_s_l(_Dest, _Size, _Val, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsset_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsset_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Val, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbsspn([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Control);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbsspn_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Control, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsspnp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Str2);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbsspnp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Str1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Str2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]   const unsigned char * __cdecl _mbsstr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Substr);
[returnvalue:SA_Post(MustCheck=SA_Yes)]   const unsigned char * __cdecl _mbsstr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Substr, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbstok_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) unsigned char * __cdecl _mbstok([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Delim);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  __declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbstok_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) unsigned char * __cdecl _mbstok_l([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Delim, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbstok_s([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Delim, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char ** _Context);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned char * __cdecl _mbstok_s_l([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Delim, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char ** _Context, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbsupr_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsupr_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes,WritableBytesConst=_Size)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] unsigned char (&_String)[_Size]) { return _mbsupr_s(_String, _Size); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsupr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String);
 errno_t __cdecl _mbsupr_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_Str, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbsupr_s_l(unsigned char (&_String)[_Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbsupr_s_l(_String, _Size, _Locale); } }
__declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbsupr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details."))  unsigned char * __cdecl _mbsupr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char *_String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbclen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Str);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  size_t __cdecl _mbclen_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Str, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbccpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) void __cdecl _mbccpy([SA_Pre(Null=SA_No,WritableBytesConst=2)] [SA_Pre(Deref=1,Valid=SA_No)] unsigned char * _Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src);
 __declspec(deprecated("This function or variable may be unsafe. Consider using " "_mbccpy_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) void __cdecl _mbccpy_l([SA_Pre(Null=SA_No,WritableBytesConst=2)] [SA_Pre(Deref=1,Valid=SA_No)] unsigned char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Src, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
 errno_t __cdecl _mbccpy_s([SA_Pre(Null=SA_No,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] int * _PCopied, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbccpy_s(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] int * _PCopied, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source) { return _mbccpy_s(_Dest, _Size, _PCopied, _Source); } }
 errno_t __cdecl _mbccpy_s_l([SA_Pre(Null=SA_No,WritableBytes="_DstSizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] unsigned char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _DstSizeInBytes, [SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] int * _PCopied, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Src, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
extern "C++" { template <size_t _Size> inline errno_t __cdecl _mbccpy_s_l(unsigned char (&_Dest)[_Size], [SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] int * _PCopied, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Source, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _mbccpy_s_l(_Dest, _Size, _PCopied, _Source, _Locale); } }





extern "C++" {
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline unsigned char * __cdecl _mbschr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Char)
{
    return ((unsigned char *)_mbschr((const unsigned char *)_String, _Char)); 
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] inline unsigned char * __cdecl _mbschr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Char, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return ((unsigned char *)_mbschr_l((const unsigned char *)_String, _Char, _Locale)); 
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] inline unsigned char * __cdecl _mbspbrk([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] unsigned char *_String, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_CharSet)
{
    return ((unsigned char *)_mbspbrk((const unsigned char *)_String, _CharSet)); 
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] inline unsigned char * __cdecl _mbspbrk_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] unsigned char *_String, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_CharSet, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return ((unsigned char *)_mbspbrk_l((const unsigned char *)_String, _CharSet, _Locale)); 
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] inline unsigned char * __cdecl _mbsrchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Char)
{
    return ((unsigned char *)_mbsrchr((const unsigned char *)_String, _Char)); 
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] inline unsigned char * __cdecl _mbsrchr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] unsigned char *_String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Char, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return ((unsigned char *)_mbsrchr_l((const unsigned char *)_String, _Char, _Locale)); 
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] inline unsigned char * __cdecl _mbsstr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] unsigned char *_String, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Match)
{
    return ((unsigned char *)_mbsstr((const unsigned char *)_String, _Match)); 
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] inline unsigned char * __cdecl _mbsstr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] unsigned char *_String, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char *_Match, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return ((unsigned char *)_mbsstr_l((const unsigned char *)_String, _Match, _Locale)); 
}
}
#line 257 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"
#line 258 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"



[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcalnum([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcalnum_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcalpha([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcalpha_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcdigit([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcdigit_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcgraph([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcgraph_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbclegal([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbclegal_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbclower([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbclower_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcprint([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcprint_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcpunct([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcpunct_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcspace([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcspace_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcupper([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcupper_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);

[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctolower([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctolower_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctoupper([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctoupper_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);


#line 289 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"


[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbblead([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbblead_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbbtrail([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbbtrail_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbslead([SA_Pre(Null=SA_No,ValidElements="\n@""_Pos - _Str+1")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Pos);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbslead_l([SA_Pre(Null=SA_No,ValidElements="\n@""_Pos - _Str+1")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Pos, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbstrail([SA_Pre(Null=SA_No,ValidElements="\n@""_Pos - _Str+1")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Pos);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbstrail_l([SA_Pre(Null=SA_No,ValidElements="\n@""_Pos - _Str+1")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] const unsigned char * _Str, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const unsigned char * _Pos, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);

#line 301 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"



[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbchira([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbchira_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbckata([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbckata_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcsymbol([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcsymbol_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcl0([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcl0_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcl1([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcl1_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcl2([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  int __cdecl _ismbcl2_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbcjistojms([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbcjistojms_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbcjmstojis([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbcjmstojis_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctohira([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctohira_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctokata([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch);
[returnvalue:SA_Post(MustCheck=SA_Yes)]  unsigned int __cdecl _mbctokata_l([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Ch, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale);



}
#line 329 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"


#pragma pack(pop)
#line 333 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"

#line 335 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\mbstring.h"

#line 1231 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"


extern "C" {
#line 1235 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"



typedef char            _TCHAR;
typedef signed char     _TSCHAR;
typedef unsigned char   _TUCHAR;
typedef unsigned char   _TXCHAR;
typedef unsigned int    _TINT;

#line 1245 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"



typedef char            TCHAR;
#line 1250 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"

#line 1252 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"
















































































































































































































#line 1461 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"













__inline const char * _tcschr([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _c) {return (const char *)_mbschr((const unsigned char *)_s1,_c);}
__inline size_t _tcscspn([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2) {return _mbscspn((const unsigned char *)_s1,(const unsigned char *)_s2);}

 __inline errno_t _tcsncat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_Destination_size_chars")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Destination_size_chars, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char *_Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) 
{
    return _mbsnbcat_s((unsigned char *)_Destination, _Destination_size_chars, (const unsigned char *)_Source,_Count);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsncat_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _tcsncat_s(_Dest, _Size, _Source, _Count); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsncat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsncat([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsnbcat((unsigned char *)_Dst,(const unsigned char *)_Source,_Count);
#pragma warning(pop)
}



 __inline errno_t _tcsncat_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_Destination_size_chars")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Destination_size_chars, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char *_Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbsnbcat_s_l((unsigned char *)_Destination, _Destination_size_chars, (const unsigned char *)_Source,_Count, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsncat_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcsncat_s_l(_Dest, _Size, _Source, _Count, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsncat_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsncat_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsnbcat_l((unsigned char *)_Dst,(const unsigned char *)_Source,_Count, _Locale);
#pragma warning(pop)
}



 __inline errno_t _tcsncpy_s([SA_Pre(Null=SA_No,WritableBytes="_Destination_size_chars")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Destination_size_chars, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) 
{
    return _mbsnbcpy_s((unsigned char *)_Destination, _Destination_size_chars, (const unsigned char *)_Source,_Count);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsncpy_s([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _tcsncpy_s(_Dest, _Size, _Source, _Count); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsncpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsncpy(char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsnbcpy((unsigned char *)_Dst,(const unsigned char *)_Source,_Count);
#pragma warning(pop)
}



 __inline errno_t _tcsncpy_s_l([SA_Pre(Null=SA_No,WritableBytes="_Destination_size_chars")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Destination_size_chars, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbsnbcpy_s_l((unsigned char *)_Destination, _Destination_size_chars, (const unsigned char *)_Source,_Count, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsncpy_s_l([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcsncpy_s_l(_Dest, _Size, _Source, _Count, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsncpy_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsncpy_l([SA_Pre(Null=SA_No,WritableBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsnbcpy_l((unsigned char *)_Dst,(const unsigned char *)_Source,_Count, _Locale);
#pragma warning(pop)
}



[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline const char * _tcspbrk([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2) {return (const char *)_mbspbrk((const unsigned char *)_s1,(const unsigned char *)_s2);}
[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline const char * _tcsrchr([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _c) {return (const char *)_mbsrchr((const unsigned char *)_s1,_c);}
[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline size_t _tcsspn([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2) {return _mbsspn((const unsigned char *)_s1,(const unsigned char *)_s2);}
[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline const char * _tcsstr([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2) {return (const char *)_mbsstr((const unsigned char *)_s1,(const unsigned char *)_s2);}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcstok_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char *  _tcstok([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _String,[SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Delimiters) 
{
#pragma warning(push)
#pragma warning(disable:4996)
	return (char * )_mbstok((unsigned char *)_String,(const unsigned char *)_Delimiters);
#pragma warning(pop)
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline char *  _tcstok_s([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _String,[SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Delimiters, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char **_Current_position) 
{
	return (char * )_mbstok_s((unsigned char *)_String,(const unsigned char *)_Delimiters, (unsigned char **)_Current_position);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcstok_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char *  _tcstok_l([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _String,[SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Delimiters, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
#pragma warning(push)
#pragma warning(disable:4996)
	return (char * )_mbstok_l((unsigned char *)_String,(const unsigned char *)_Delimiters, _Locale);
#pragma warning(pop)
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline char *  _tcstok_s_l([SA_Pre(Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _String,[SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Delimiters, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_Maybe,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char **_Current_position, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
	return (char * )_mbstok_s_l((unsigned char *)_String,(const unsigned char *)_Delimiters, (unsigned char **)_Current_position, _Locale);
}

 __inline errno_t _tcsnset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value , [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
    return _mbsnbset_s((unsigned char *)_Dst, _SizeInBytes, _Value, _Count);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsnset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _tcsnset_s(_Dest, _Size, _Value, _Count); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsnset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsnset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsnbset((unsigned char *)_Dst, _Value, _Count);
#pragma warning(pop)
}



 __inline errno_t _tcsnset_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value , [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsnbset_s_l((unsigned char *)_Dst, _SizeInBytes, _Value, _Count, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsnset_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcsnset_s_l(_Dest, _Size, _Value, _Count, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsnset_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsnset_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsnbset_l((unsigned char *)_Dst, _Value, _Count, _Locale);
#pragma warning(pop)
}



__inline char * _tcsrev([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _s1) {return (char *)_mbsrev((unsigned char *)_s1);}

 __inline errno_t _tcsset_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value)
{
    return _mbsset_s((unsigned char *)_Dst, _SizeInBytes, _Value);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsset_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value) { return _tcsset_s(_Dest, _Size, _Value); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsset([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsset((unsigned char *)_Dst, _Value);
#pragma warning(pop)
}



 __inline errno_t _tcsset_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsset_s_l((unsigned char *)_Dst, _SizeInBytes, _Value, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsset_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcsset_s_l(_Dest, _Size, _Value, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsset_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsset_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsset_l((unsigned char *)_Dst, _Value, _Locale);
#pragma warning(pop)
}



[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcscmp([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2) {return _mbscmp((const unsigned char *)_s1,(const unsigned char *)_s2);}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2) 
{
    return _mbsicmp((const unsigned char *)_String1,(const unsigned char *)_String2);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbsicmp_l((const unsigned char *)_String1,(const unsigned char *)_String2, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsnccmp([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _n) {return _mbsncmp((const unsigned char *)_s1,(const unsigned char *)_s2,_n);}
__inline int _tcsncmp([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _n) {return _mbsnbcmp((const unsigned char *)_s1,(const unsigned char *)_s2,_n);}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsncicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Char_count)
{
    return _mbsnicmp((const unsigned char *)_String1,(const unsigned char *)_String2,_Char_count);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsncicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Char_count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsnicmp_l((const unsigned char *)_String1,(const unsigned char *)_String2,_Char_count, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsnicmp([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Char_count) 
{
    return _mbsnbicmp((const unsigned char *)_String1,(const unsigned char *)_String2,_Char_count);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsnicmp_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Char_count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbsnbicmp_l((const unsigned char *)_String1,(const unsigned char *)_String2,_Char_count, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcscoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2)
{
    return _mbscoll((const unsigned char *)_String1,(const unsigned char *)_String2);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcscoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbscoll_l((const unsigned char *)_String1,(const unsigned char *)_String2, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2)
{
    return _mbsicoll((const unsigned char *)_String1,(const unsigned char *)_String2);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsicoll_l((const unsigned char *)_String1,(const unsigned char *)_String2, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsnccoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
    return _mbsncoll((const unsigned char *)_String1,(const unsigned char *)_String2, _Count);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsnccoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsncoll_l((const unsigned char *)_String1,(const unsigned char *)_String2, _Count, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsncoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
    return _mbsnbcoll((const unsigned char *)_String1,(const unsigned char *)_String2, _Count);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsncoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsnbcoll_l((const unsigned char *)_String1,(const unsigned char *)_String2, _Count, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsncicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
    return _mbsnicoll((const unsigned char *)_String1,(const unsigned char *)_String2, _Count);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsncicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsnicoll_l((const unsigned char *)_String1,(const unsigned char *)_String2, _Count, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsnicoll([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
    return _mbsnbicoll((const unsigned char *)_String1,(const unsigned char *)_String2, _Count);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline int _tcsnicoll_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String1, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String2, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsnbicoll_l((const unsigned char *)_String1,(const unsigned char *)_String2, _Count, _Locale);
}


[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline size_t _tcsclen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String) 
{
    return _mbslen((const unsigned char *)_String);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline size_t _tcscnlen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Maximum) 
{
    return _mbsnlen((const unsigned char *)_String, _Maximum);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline size_t _tcsclen_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbslen_l((const unsigned char *)_String, _Locale);
}

[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline size_t _tcscnlen_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Maximum, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbsnlen_l((const unsigned char *)_String, _Maximum, _Locale);
}

 __inline errno_t _tcsnccat_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_Destination_size_chars")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Destination_size_chars, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) 
{
    return _mbsncat_s((unsigned char *)_Destination, _Destination_size_chars, (const unsigned char *)_Source, _Count);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsnccat_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _tcsnccat_s(_Dest, _Size, _Source, _Count); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsnccat_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsnccat([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsncat((unsigned char *)_Dst,(const unsigned char *)_Source, _Count);
#pragma warning(pop)
}



 __inline errno_t _tcsnccat_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_Destination_size_chars")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Destination_size_chars, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbsncat_s_l((unsigned char *)_Destination, _Destination_size_chars, (const unsigned char *)_Source, _Count, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsnccat_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcsnccat_s_l(_Dest, _Size, _Source, _Count, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsnccat_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsnccat_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsncat_l((unsigned char *)_Dst,(const unsigned char *)_Source, _Count, _Locale);
#pragma warning(pop)
}



 __inline errno_t _tcsnccpy_s([SA_Pre(Null=SA_No,WritableBytes="_Destination_size_chars")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Destination_size_chars, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) 
{
    return _mbsncpy_s((unsigned char *)_Destination, _Destination_size_chars, (const unsigned char *)_Source, _Count);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsnccpy_s([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _tcsnccpy_s(_Dest, _Size, _Source, _Count); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsnccpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsnccpy([SA_Pre(Null=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsncpy((unsigned char *)_Dst,(const unsigned char *)_Source, _Count);
#pragma warning(pop)
}



 __inline errno_t _tcsnccpy_s_l([SA_Pre(Null=SA_No,WritableBytes="_Destination_size_chars")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Destination_size_chars, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbsncpy_s_l((unsigned char *)_Destination, _Destination_size_chars, (const unsigned char *)_Source, _Count, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsnccpy_s_l([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcsnccpy_s_l(_Dest, _Size, _Source, _Count, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsnccpy_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsnccpy_l([SA_Pre(Null=SA_No,WritableBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Maybe)] char *_Dst, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsncpy_l((unsigned char *)_Dst,(const unsigned char *)_Source, _Count, _Locale);
#pragma warning(pop)
}



 __inline errno_t _tcsncset_s([SA_Pre(Null=SA_No,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
    return _mbsnset_s((unsigned char *)_Destination, _SizeInBytes, _Value, _Count);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsncset_s(char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) { return _tcsncset_s(_Dest, _Size, _Value, _Count); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsncset_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsncset([SA_Pre(Null=SA_No,WritableBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsnset((unsigned char *)_Dst, _Value, _Count);
#pragma warning(pop)
}



 __inline errno_t _tcsncset_s_l([SA_Pre(Null=SA_No,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
    return _mbsnset_s_l((unsigned char *)_Destination, _SizeInBytes, _Value, _Count, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsncset_s_l(char (&_Dest)[_Size], [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcsncset_s_l(_Dest, _Size, _Value, _Count, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsncset_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsncset_l([SA_Pre(Null=SA_No,WritableBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_Dst, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _Value, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsnset_l((unsigned char *)_Dst, _Value, _Count, _Locale);
#pragma warning(pop)
}





[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline char * _tcsdec([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2) {return (char *)_mbsdec((const unsigned char *)_s1,(const unsigned char *)_s2);}
[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline char * _tcsinc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1) {return (char *)_mbsinc((const unsigned char *)_s1);}
[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline size_t _tcsnbcnt([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _n) {return _mbsnbcnt((const unsigned char *)_s1,_n);}
[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline size_t _tcsnccnt([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _n) {return _mbsnccnt((const unsigned char *)_s1,_n);}
[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline char * _tcsninc([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _n) {return (char *)_mbsninc((const unsigned char *)_s1,_n);}
[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline char * _tcsspnp([SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s1,[SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] const char * _s2) {return (char *)_mbsspnp((const unsigned char *)_s1,(const unsigned char *)_s2);}

 __inline errno_t _tcslwr_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _String, size_t _SizeInBytes) 
{
    return _mbslwr_s((unsigned char *)_String, _SizeInBytes);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcslwr_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_String)[_Size]) { return _tcslwr_s(_String, _Size); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcslwr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcslwr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_String)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbslwr((unsigned char *)_String);
#pragma warning(pop)
}



 __inline errno_t _tcslwr_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbslwr_s_l((unsigned char *)_String, _SizeInBytes, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcslwr_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_String)[_Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcslwr_s_l(_String, _Size, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcslwr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcslwr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbslwr_l((unsigned char *)_String, _Locale);
#pragma warning(pop)
}



 __inline errno_t _tcsupr_s([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count) 
{
    return _mbsupr_s((unsigned char *)_String, _Count);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsupr_s([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_String)[_Size]) { return _tcsupr_s(_String, _Size); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsupr_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsupr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_String)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsupr((unsigned char *)_String);
#pragma warning(pop)
}



 __inline errno_t _tcsupr_s_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes,WritableBytes="_Count")] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _String, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _Count, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbsupr_s_l((unsigned char *)_String, _Count, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tcsupr_s_l([SA_Pre(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=2,Valid=SA_Yes)] [SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_String)[_Size], [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tcsupr_s_l(_String, _Size, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tcsupr_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline char * __cdecl _tcsupr_l([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char *_String, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale)
{
#pragma warning(push)
#pragma warning(disable:4996)
    return (char *)_mbsupr_l((unsigned char *)_String, _Locale);
#pragma warning(pop)
}



[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline size_t _tclen([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _s1) {return _mbclen((const unsigned char *)_s1);}

 __inline errno_t _tccpy_s([SA_Pre(Null=SA_No,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] int *_PCopied, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source) 
{
    return _mbccpy_s((unsigned char *)_Destination, _SizeInBytes, _PCopied, (const unsigned char *)_Source);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tccpy_s([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] int * _PCopied, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source) { return _tccpy_s(_Dest, _Size, _PCopied, _Source); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tccpy_s" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline void _tccpy([SA_Pre(Null=SA_No,WritableBytesConst=2)] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source) 
{
#pragma warning(push)
#pragma warning(disable:4996)
    _mbccpy((unsigned char *)_Destination, (const unsigned char *)_Source);
#pragma warning(pop)
}

 __inline errno_t _tccpy_s_l([SA_Pre(Null=SA_No,WritableBytes="_SizeInBytes")] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] size_t _SizeInBytes, [SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] int *_PCopied, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
    return _mbccpy_s_l((unsigned char *)_Destination, _SizeInBytes, _PCopied, (const unsigned char *)_Source, _Locale);
}

extern "C++" { template <size_t _Size> inline errno_t __cdecl _tccpy_s_l([SA_Post(Deref=1,Null=SA_No,NullTerminated=SA_Yes)] [SA_Post(Deref=2,Valid=SA_Yes)] char (&_Dest)[_Size], [SA_Pre(Null=SA_Maybe,WritableElementsConst=1)] [SA_Pre(Deref=1,Valid=SA_No)] int * _PCopied, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) { return _tccpy_s_l(_Dest, _Size, _PCopied, _Source, _Locale); } }

__declspec(deprecated("This function or variable may be unsafe. Consider using " "_tccpy_s_l" " instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.")) __inline void _tccpy_l([SA_Pre(Null=SA_No,WritableBytesConst=2)] [SA_Pre(Deref=1,Valid=SA_No)] [SA_Post(NullTerminated=SA_Yes)] [SA_Post(Deref=1,Valid=SA_Yes)] char * _Destination, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _Source, [SA_Pre(Null=SA_Maybe)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] _locale_t _Locale) 
{
#pragma warning(push)
#pragma warning(disable:4996)
    _mbccpy_l((unsigned char *)_Destination,( const unsigned char *)_Source, _Locale);
#pragma warning(pop)
}


[returnvalue:SA_Post(MustCheck=SA_Yes)] __inline unsigned int _tcsnextc([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char * _s1)
{
    unsigned int _n=0; 
    if (_ismbblead((unsigned int)*(unsigned char *)_s1)) 
    {
        


        if(_s1[1]!='\0')
        {
            _n=((unsigned int)*(unsigned char *)_s1)<<8; 
            _s1++;
        }
    } 
    _n+=(unsigned int)*(unsigned char *)_s1; 
    
    return(_n);
}

#line 1994 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"




extern "C++" {
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl _tcschr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] char *_S, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _C)
        {return ((char *)_tcschr((const char *)_S, _C)); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl _tcspbrk([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] char *_S, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char *_P)
        {return ((char *)_tcspbrk((const char *)_S, _P)); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl _tcsrchr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] char *_S, [SA_Pre(Null=SA_No)] [SA_Pre(Deref=1,Valid=SA_Yes,Access=SA_Read)] unsigned int _C)
        {return ((char *)_tcsrchr((const char *)_S, _C)); }
[returnvalue:SA_Post(MustCheck=SA_Yes)] inline char * __cdecl _tcsstr([SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] char *_S, [SA_Pre(Null=SA_No,NullTerminated=SA_Yes)] [SA_Pre(Deref=1,Valid=SA_Yes)] [SA_Pre(Deref=1,Access=SA_Read)] const char *_P)
        {return ((char *)_tcsstr((const char *)_S, _P)); }
}
#line 2009 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"
#line 2010 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"

#line 2012 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"

















































































































































































































































































































































































#line 2382 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"


#line 2385 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"










}   
#line 2397 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"

#line 2399 "d:\\program files\\microsoft visual studio 9.0\\vc\\include\\tchar.h"


#line 12 "f:\\svn\\miscellaneous\\quotserver\\stdafx.h"




#line 6 "f:\\svn\\miscellaneous\\quotserver\\stdafx.cpp"



