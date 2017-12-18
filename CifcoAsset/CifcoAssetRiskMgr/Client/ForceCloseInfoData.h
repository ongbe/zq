#pragma once

#include <string>
#include <queue>
#include <vector>
#include <map>
#include "UserApiStruct.h"
#include "easymutex.h"

class CForceCloseInfoData
{
public:
    static CForceCloseInfoData* GetInstance();
    static void DestroyInstance();

private:
    CForceCloseInfoData(void);
    virtual ~CForceCloseInfoData(void);

public:
    
    // LockObject()与UnloadObject()这两个函数必须成对调用
    void LockObject();
    void UnlockObject();

    //通过合约代码和方向获得相关持仓信息
    bool GetPositionFieldByInstrumentIDandDirect(std::string strInstrumentID,std::string nDirect);
    SequencialPositionField curPositionField;
    //通过合约代码获得相关深度行情信息
    bool GetDepthMarketDataFieldByInstrumentID(std::string strInstrumentID);
    DepthMarketDataField curDepthMarketDataField;
    

    std::queue<SequencialPositionField>  queuePositionInfo;
    std::vector<SequencialPositionField> vecPositionInfo;
    std::queue<SequencialOrderField>     queueOrderInfo;//挂单信息
    std::vector<DepthMarketDataField>    vecDepthMarketDataInfo;//手动强平时用到，快捷用不到
    std::queue<RiskSyncAccountField>     queueFundInfo;
    
private:
    static CForceCloseInfoData* m_pInstance;
    Ceasymutex m_oMutex;

};

