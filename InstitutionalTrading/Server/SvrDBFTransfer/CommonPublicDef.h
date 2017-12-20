#pragma once
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib") 

/*
* 文件目录改变的类型
*/
enum tagACTION
{
    eFILE_ACTION_ADDED=1,
    eFILE_ACTION_REMOVED,
    eFILE_ACTION_MODIFIED,
    eFILE_ACTION_RENAMED_OLD_NAME,
    eFILE_ACTION_RENAMED_NEW_NAME
};

enum eExchange
{
    eExchange_SHOW2003,//上交所
    eExchange_SJSHQ  //深交所
};

//struct sDBFStruct
//{
//    char sTradingDay[9];
//    char sCode[16];
//    char sName[32];
//    double dOpenPrice;
//    double dClosePrice;
//    double dHightPrice;
//    double dLowPrice;
//    double dLastPrice;
//
//    void init()
//    {
//        memset(sTradingDay,0,sizeof(sTradingDay));
//        memset(sCode,0,sizeof(sCode));
//        memset(sName,0,sizeof(sName));
//
//        dOpenPrice = 0.0f;
//        dClosePrice = 0.0f;
//        dHightPrice = 0.0f;
//        dLowPrice = 0.0f;
//        dLastPrice = 0.0f;
//    }
//};