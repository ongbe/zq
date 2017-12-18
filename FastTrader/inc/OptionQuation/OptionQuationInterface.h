#include <string>
#include <vector>


#pragma once
#ifdef QUATION_EXPORTS
	#define QUATION_API __declspec(dllexport)
	#define EXPIMP_TEMPLATE
#else
	#define QUATION_API __declspec(dllimport)
	#define EXPIMP_TEMPLATE extern
#endif

typedef HWND (*Func_CreateQuationWnd)(HWND hParent);
typedef void (*Func_SetTradingDay)(std::string strTradingDay);
typedef void (*Func_ShowInstrument)(std::string ShowInstrument);
//typedef void (*Func_SetTradingScales)(std::string strInstrument, std::string& strTime);
typedef void (*Func_SetTradingScales)(std::string strInstrument, const std::vector<std::pair<std::string,std::string>>& TradingTime);
typedef void (*Func_InitFinish)(std::string strInstrument);

bool QUATION_API CreateQuationWnd(HWND hParent);
void QUATION_API SetTradingDay(std::string strTradingDay);
void QUATION_API ShowInstrument(std::string strInstrument);
//void QUATION_API SetTradingScales(std::string strInstrument, std::string& strTime);
void QUATION_API SetTradingScales(std::string strInstrument, const std::vector<std::pair<std::string,std::string>>& TradingTime);

void QUATION_API InitFinish(std::string strInstrument);


