#pragma once

#ifdef _DEBUG
#define OUTPUT_LOG(format,...)  \
{   \
	char temp[512] = "%s(%s:%d): "; \
	strcat(temp, format);   \
	strcat(temp, "\n");   \
	char lsOut[1024] = {0};   \
	sprintf_s(lsOut,temp, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);   \
	OutputDebugStringA(lsOut); \
}
#else
#define OUTPUT_LOG(format,...)  \
{   \
	char temp[512] = "%s(%s:%d): "; \
	strcat(temp, format);   \
	strcat(temp, "\n");   \
	char lsOut[1024] = {0};   \
	sprintf_s(lsOut,temp, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);   \
	OutputDebugStringA(lsOut); \
}
#endif
//

#define PUSH_LOG_ERROR(Level,bSend,bShow,format,...)  \
{   \
	char temp[512] = ""; \
	strcat(temp, format);   \
	strcat(temp, "\n");   \
	char lsOut[512] = {0};   \
	sprintf_s(lsOut,temp,  ##__VA_ARGS__);   \
	sErrorInfo lError(Level,lsOut);\
	CInterface_SvrNotifyAndAsk::getObj().PushErrorInfo(lError,bSend,bShow);\
}

#define SendRspError(_socket,_rsp,_seq,_error,_userdata1,_userdata2,_userdata3,_userdata4) \
	const char * lErrorString = FormatErrorCode(_error);\
	CInterface_SvrTcp::getObj().SendPkgData(_socket, _rsp, \
	(void*)lErrorString, \
	strlen(lErrorString)+1, _seq,0,_userdata1,_userdata2,_userdata3,_userdata4);	

#define SendRspErrorWithPrefixString(_prefix,_socket,_rsp,_seq,_error,_userdata1,_userdata2,_userdata3,_userdata4) \
	const char * lErrorString = FormatErrorCode(_error);\
	char lsBuf[1024] = {0};\
	strcpy(lsBuf,_prefix); \
	strcat(lsBuf,lErrorString); \
	CInterface_SvrTcp::getObj().SendPkgData(_socket, _rsp, \
	(void*)lsBuf, \
	strlen(lsBuf)+1, _seq,0,_userdata1,_userdata2,_userdata3,_userdata4);	

#define SendInputParamError(_socket,_rsp,_seq) \
	const char * lErrorString = FormatErrorCode(CF_ERROR_COMMON_INPUT_PARAM);\
    CInterface_SvrTcp::getObj().SendPkgData(_socket, _rsp, \
										(void*)lErrorString, \
										strlen(lErrorString)+1, _seq,0,CF_ERROR_COMMON_INPUT_PARAM);	

#define  BREAK_ON_NULL_AND_SEND_ERROR_RSP(_x,_socket,_rsp,_seq) \
	if((_x) == NULL) \
	{\
		SendInputParamError(_socket,_rsp,_seq);\
		break;\
	}

#define  BREAK_ON_DIFF_AND_SEND_ERROR_RSP(_x,_y,_socket,_rsp,_seq) \
	if((_x) != (_y)) \
	{\
		SendInputParamError(_socket,_rsp,_seq);\
		break;\
	}

#define  BREAK_ON_AND_SEND_ERROR_RSP(_x,_socket,_rsp,_seq) \
	if((_x) == false) \
	{\
		SendInputParamError(_socket,_rsp,_seq);\
		break;\
	}

#if 0
#define TRADE_LOG(fmt, ...)  ;
#else
#define TRADE_LOG(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","TradeData", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

/*
       sign      Year        Month        Day                   id   
32bit  |-1-|------6------|-----5-----|-----5-----|--------------15---------------|  
*/
#define  MAKE_DATEBASE_ID(_year,_month,_day,_id) \
    (int)((int)( (int)(_year) << 25 ) | (int)( (int)(_month) << 20 ) | (int)( (int)(_day) << 15 ) | (int)(_id)  )
