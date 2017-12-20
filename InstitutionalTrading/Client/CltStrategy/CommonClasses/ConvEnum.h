#pragma once
#include <string>
#include "CommonStruct.h"
#include <stdlib.h>
using namespace std;

const char* IndiPara2String(IndicatorParaType eType);
IndicatorParaType IndiPara2Enum(const string& eType);
IndicatorType IndiType2Enum(const string& eType);
IndicatorFile IndiFile2Enum(const string& eType);
DisplayType DisplayType2Enum(const string& eType);
StrategyParaType StrategyType2Enum(const string& eType);
StrategyDirection StrategyDirection2Enum(const string& eType);
const char* StrategyType2String(StrategyParaType eType);
const char* DirectionType2String(StrategyDirection eType);
const char* DisplayType2String(DisplayType eType);
const char* Bool2String(bool bData);
const string ExchangeName2ID( const string& name);
const string ExchangeID2String(const string& ExchangeID);
std::string Int2String(int field);
std::string Double2String(double field);
std::string long2String(long field);

