#pragma once

#include <string>
#include <vector>
#define LOG_STRING_LEN 512;

enum LOG_PARAM_TYPE
{
	DEFAULT_TYPE,
	LOAD_STRING_TYPE,
	ORDER_STATUS_MSG_TYPE,
	PRICETYPE_MSG_TYPE,
	DIRECTION_TYPE,
	OFFSETFLAG_TYPE,
};

struct LOG_PARAM
{
	LOG_PARAM( std::string param, LOG_PARAM_TYPE type = DEFAULT_TYPE )
	{
		strParam = param;
		eParamType = type;
	}

	LOG_PARAM( char param, LOG_PARAM_TYPE type = DEFAULT_TYPE )
	{
		strParam = param;
		eParamType = type;
	}

	std::string strParam;
	LOG_PARAM_TYPE eParamType;
};

struct LOG_DATA
{
	std::string strTitle;
	std::string strFormat;
	std::vector<LOG_PARAM> vParam;

	LOG_DATA()
	{
		strTitle = "";
		strFormat = "";
		vParam.clear();
	}
};