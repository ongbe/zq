#include "StdAfx.h"
#include "PlatformStruct.h"
#include "BusinessData.hpp"


char* PlatformStru_OrderInfo::tostring(char*prlt,int rltsize) const
{
    if(bInvalid&&prlt&&rltsize>0) 
    {
        strncpy(prlt,"Invalid",rltsize);
        prlt[rltsize-1]=0;
        return prlt;
    }

    OrderKey tmpkey(*this);
    return tmpkey.tostring(prlt,rltsize);
}


char* PlatformStru_TradeInfo::tostring(char*prlt,int rltsize) const
{
    struct TradeKey tmpkey(*this);
    return tmpkey.tostring(prlt,rltsize);
}



char* PlatformStru_TradeTotalInfo::tostring(char*prlt,int rltsize) const
{
    TradeTotalKey tmpkey(*this);
    return tmpkey.tostring(prlt,rltsize);
}



char* PlatformStru_PositionDetail::tostring(char*prlt,int rltsize) const
{
    if(bInvalid&&prlt&&rltsize>0) 
    {
        strncpy(prlt,"Invalid",rltsize);
        prlt[rltsize-1]=0;
        return prlt;
    }
    PositionDetailKey tmpkey(*this);
    tmpkey.tostring(prlt,rltsize);
    int len=strlen(prlt);
    if(len+20<rltsize) sprintf(prlt+len,",Volume=%d",Volume);
    return prlt;
}


char* PlatformStru_Position::tostring(char*prlt,int rltsize) const
{
    if(bInvalid&&prlt&&rltsize>0) 
    {
        strncpy(prlt,"Invalid",rltsize);
        prlt[rltsize-1]=0;
        return prlt;
    }
    PositionKey tmpkey(*this);
    tmpkey.tostring(prlt,rltsize);
    int len=strlen(prlt);
    if(len+20<rltsize) sprintf(prlt+len,",Position=%d",Position);
    return prlt;
}


char* PlatformStru_PositionComb::tostring(char*prlt,int rltsize) const
{
    if(bInvalid&&prlt&&rltsize>0) 
    {
        strncpy(prlt,"Invalid",rltsize);
        prlt[rltsize-1]=0;
        return prlt;
    }
    PositionCombKey tmpkey(*this);
    tmpkey.tostring(prlt,rltsize);
    int len=strlen(prlt);
    if(len+20<rltsize) sprintf(prlt+len,",TotalAmt=%d",TotalAmt);
    return prlt;
}
