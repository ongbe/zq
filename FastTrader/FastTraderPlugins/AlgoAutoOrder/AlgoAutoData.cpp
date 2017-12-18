#include "StdAfx.h"
#include "AlgoAutoData.h"
CAlgoAutoData::CAlgoAutoData(void)
{
    TradingTime = 0;
    TradingMinute=0;

    InstrumentDirection=0;
    InstrumentOffsetID=0;
    MarketDegreed=0;
    bActiceOverTime=false;
    bActiceTotalTime=false;
    bActiceOpt=false;
    OptimizeIndex=0;

    NewVolume=0;
    BidVolume=0;
    AskVolume=0;
    LatestPrice=0.0f;
    PriceTick=1.0f;
//     bMultiPrice=false;
//     bPatchOrder=false;

    vecOrderInfo.clear();
    vecOptInfo.clear();
    vecTradedVolumePerMinute.clear();
    vecTradedVolumePerTime.clear();
    vecOrderActiveInfo.clear();

    Volume=0;
    WaitVolume=0;
    AverageQuan=0.0f;
    TotalVolume=0;
    RetryCountSave=0;
    RetryTotalTimeSave=0;
    m_nOrderCount = 0;

    OrderCountValue=0;
    PatchOrderCountValue=0;
    bIdleTime=FALSE;
    bExecuteTime=FALSE;
}

CAlgoAutoData::CAlgoAutoData(const CAlgoAutoData& r)
{
    *this=r;
}

CAlgoAutoData& CAlgoAutoData::operator= (const CAlgoAutoData& r)
{       
    TradingTime=r.TradingTime;
    TradingMinute=r.TradingMinute;

    InstrumentName=r.InstrumentName;
    InstrumentDirection=r.InstrumentDirection;
    InstrumentOffsetID=r.InstrumentOffsetID;   
    LimitPrice=r.LimitPrice;
    StartTime=r.StartTime;
    EndTime=r.EndTime;
    MarketDegreed=r.MarketDegreed;
    RetryCount=r.RetryCount;
    bActiceOverTime=r.bActiceOverTime;
    bActiceTotalTime=r.bActiceTotalTime;
    bActiceOpt=r.bActiceOpt;
    OptimizeIndex=r.OptimizeIndex;

    NewVolume=r.NewVolume;
    BidVolume=r.BidVolume;
    AskVolume=r.AskVolume;
    PriceTick=r.PriceTick;

    vecOrderInfo=r.vecOrderInfo;
    vecOptInfo=r.vecOptInfo;
    vecTradedVolumePerMinute=r.vecTradedVolumePerMinute;
    vecTradedVolumePerTime=r.vecTradedVolumePerTime;
    vecOrderActiveInfo=r.vecOrderActiveInfo;

    Volume=r.Volume;
    AverageQuan=r.AverageQuan;
    TotalVolume=r.TotalVolume;
    RetryCountSave=r.RetryCountSave;
    RetryTotalTimeSave=r.RetryTotalTimeSave;

    return *this;
}
CAlgoAutoData::~CAlgoAutoData(void)
{
}
/*从INI文件加载保存的某个合约的数据，然后会写到窗口控件里*/
bool CAlgoAutoData::ReadIni(string& strInstrumentID)
{
    char Buf[512],PathFileName[512];

    //获取程序路径
    memset(Buf,0,sizeof(Buf));
    memset(PathFileName,0,sizeof(PathFileName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_AlgoAutoData));
    CString p;
    p.Format("%s_%s",strInstrumentID.c_str(),IniFileName_AlgoAutoData);
    strcat(PathFileName,p);
    CIniFile *pIniFile=new CIniFile(PathFileName);
    if(!pIniFile) return false;

    pIniFile->GetIniKeyStr("AlgoAutoData","InstrumentName","",Buf,sizeof(Buf));
    InstrumentName=Buf;
    InstrumentDirection =pIniFile->GetIniKeyVal("AlgoAutoData","InstrumentDirection",0);
    InstrumentOffsetID=pIniFile->GetIniKeyVal("AlgoAutoData","InstrumentOffsetID",0);
    pIniFile->GetIniKeyStr("AlgoAutoData","InstrumentCount","",Buf,sizeof(Buf));
    InstrumentCount=Buf;
    pIniFile->GetIniKeyStr("AlgoAutoData","LimitPrice","",Buf,sizeof(Buf));
    LimitPrice=Buf;
    MarketDegreed=pIniFile->GetIniKeyVal("AlgoAutoData","MarketDegreed",0);
    pIniFile->GetIniKeyStr("AlgoAutoData","OrderCountValue","",Buf,sizeof(Buf));
    OrderCount.value=Buf;
    OrderCount.type=pIniFile->GetIniKeyVal("AlgoAutoData","OrderCountType",0);
    pIniFile->GetIniKeyStr("AlgoAutoData","ResetCountValue","",Buf,sizeof(Buf));
//     ResetCount.value=Buf;
//     ResetCount.type=pIniFile->GetIniKeyVal("AlgoAutoData","ResetCountType",0);
    BasicPrice.type=pIniFile->GetIniKeyVal("AlgoAutoData","BasicPriceType",0);
    pIniFile->GetIniKeyStr("AlgoAutoData","IdleTimeValue","",Buf,sizeof(Buf));
    IdleTime.value=Buf;
    IdleTime.type=pIniFile->GetIniKeyVal("AlgoAutoData","IdleTimeType",0);
    pIniFile->GetIniKeyStr("AlgoAutoData","ExecuteTimeValue","",Buf,sizeof(Buf));
    ExecuteTime.value=Buf;
    ExecuteTime.type=pIniFile->GetIniKeyVal("AlgoAutoData","ExecuteTimeType",0);
    pIniFile->GetIniKeyStr("AlgoAutoData","RetryTotalTimeValue","",Buf,sizeof(Buf));
    RetryTotalTime.value=Buf;
    RetryTotalTime.type=pIniFile->GetIniKeyVal("AlgoAutoData","RetryTotalTimeType",0);

    pIniFile->GetIniKeyStr("AlgoAutoData","RetryCount","",Buf,sizeof(Buf));
    RetryCount=Buf;

    bActiceOverTime=pIniFile->GetIniKeyVal("AlgoAutoData","bActiceOverTime",0)==0?false:true;
    bActiceTotalTime=pIniFile->GetIniKeyVal("AlgoAutoData","bActiceTotalTime",0)==0?false:true;
    bActiceOpt=pIniFile->GetIniKeyVal("AlgoAutoData","bActiceOpt",0)==0?false:true;
    OptimizeIndex=pIniFile->GetIniKeyVal("AlgoAutoData","OptimizeIndex",0);
//     bMultiPrice=pIniFile->GetIniKeyVal("AlgoAutoData","bMultiPrice",0)==0?false:true;

    WDDATA_PriceSpread.index = pIniFile->GetIniKeyVal("AlgoAutoData","SlidePriceSpreadName",0);
    pIniFile->GetIniKeyStr("AlgoAutoData","SlidePriceSpreadData","",Buf,sizeof(Buf));
    AnalysisData(WDDATA_PriceSpread.data,Buf);

//     WDDATA_OrderCount.index = pIniFile->GetIniKeyVal("AlgoAutoData","RelateOrderCountName",0);        
//     pIniFile->GetIniKeyStr("AlgoAutoData","RelateOrderCountData","",Buf,sizeof(Buf));
//     AnalysisData(WDDATA_OrderCount.data,Buf);


    //opt
    vector<int>     vecIntIndex;
    vector<int>     vecIntBackTime;
    vector<int>     vecIntOptMarketDegreed;
    vector<int>     vecIntPriceChangeType;
    int             IntOptIndex;
    vector<int>		vecIntOptSlidePriceSpreadName; 	
    vector<string>  vecOptName;
    vector<string>  vecDoubleMultiple;
    vector<string>  vecRange;
    vector<string>  vecOption;
    vector<string>  vecHistory;
    vector<string>  vecDoublePriceChangeFromValue;
    vector<string>  vecDoublePriceChangeToValue;
    vector<string>  vecDoubleOptSlidePriceSpreadData;

    vecOptInfo.clear();

    int countSingle=0,countOpt = pIniFile->GetIniKeyVal("Opts","optCount",0);
    for (int m=0;m<countOpt;m++)
    {
        vecIntIndex.clear();
        vecIntBackTime.clear();
        vecIntOptMarketDegreed.clear();
        vecIntPriceChangeType.clear();
        vecOptName.clear();
        vecDoubleMultiple.clear();
        vecRange.clear();
        vecOption.clear();
        vecHistory.clear();
        vecDoublePriceChangeFromValue.clear();
        vecDoublePriceChangeToValue.clear();
        vecDoubleOptSlidePriceSpreadData.clear();

        OPTPARAM *optParam = new OPTPARAM;
        optParam->vecSingle.clear();
        optParam->vecSection.clear();

        char buf[265] = {0};
        sprintf(buf,"%s%d","Opt",m);

        countSingle = pIniFile->GetIniKeyVal(buf,"singleCount",0);

        vecIntIndex=pIniFile->GetIniKeyVal4(buf,"singleIndex");
        vecIntBackTime=pIniFile->GetIniKeyVal4(buf,"BackTime");
        vecIntOptMarketDegreed=pIniFile->GetIniKeyVal4(buf,"OptMarketDegreed");
        vecIntPriceChangeType=pIniFile->GetIniKeyVal4(buf,"PriceChangeType");

        IntOptIndex=pIniFile->GetIniKeyVal(buf,"OptIndex",0);

        vecIntOptSlidePriceSpreadName = pIniFile->GetIniKeyVal4(buf,"OptSlidePriceSpreadName");                

        pIniFile->GetIniKeyStr(buf,"optName","",Buf,sizeof(Buf));
        AnalysisData(vecOptName,Buf);

        pIniFile->GetIniKeyStr(buf,"Multiple","",Buf,sizeof(Buf));
        AnalysisData(vecDoubleMultiple,Buf);

        pIniFile->GetIniKeyStr(buf,"Range","",Buf,sizeof(Buf));
        AnalysisData(vecRange,Buf);

        pIniFile->GetIniKeyStr(buf,"Option","",Buf,sizeof(Buf));
        AnalysisData(vecOption,Buf);

        pIniFile->GetIniKeyStr(buf,"History","",Buf,sizeof(Buf));
        AnalysisData(vecHistory,Buf);

        pIniFile->GetIniKeyStr(buf,"PriceChangeFromValue","",Buf,sizeof(Buf));
        AnalysisData(vecDoublePriceChangeFromValue,Buf);

        pIniFile->GetIniKeyStr(buf,"PriceChangeToValue","",Buf,sizeof(Buf));
        AnalysisData(vecDoublePriceChangeToValue,Buf);

        pIniFile->GetIniKeyStr(buf,"OptSlidePriceSpreadData","",Buf,sizeof(Buf));
        AnalysisData(vecDoubleOptSlidePriceSpreadData,Buf);

        CString name;
        int nVer = 0;

        for (int n=0;n<countSingle;n++)
        {               
            SINGLEOPT *singleOpt = new SINGLEOPT;
            section *sec = new section;

            singleOpt->OptSlidePriceSpread.data.clear();

            singleOpt->Index = vecIntIndex.at(n);
            singleOpt->BackTime = vecIntBackTime.at(n);
            singleOpt->OptMarketDegreed = vecIntOptMarketDegreed.at(n);
            singleOpt->PriceChange.type = vecIntPriceChangeType.at(n);
            singleOpt->Multiple = vecDoubleMultiple.at(n);
            singleOpt->Range = vecRange.at(n);
            singleOpt->Option = vecOption.at(n);
            singleOpt->History = vecHistory.at(n);
            singleOpt->PriceChange.valueFrom = vecDoublePriceChangeFromValue.at(n).c_str();
            singleOpt->PriceChange.valueTo = vecDoublePriceChangeToValue.at(n).c_str();


            singleOpt->OptSlidePriceSpread.index = vecIntOptSlidePriceSpreadName.at(n);
            for (int p=0;p<5;p++)
                singleOpt->OptSlidePriceSpread.data.push_back(vecDoubleOptSlidePriceSpreadData.at(nVer+p));

            nVer += 5;
            optParam->vecSingle.push_back(singleOpt);

            sec->low = atof(singleOpt->PriceChange.valueFrom.c_str());
            sec->high = atof(singleOpt->PriceChange.valueTo.c_str());
            sec->index = n;
            optParam->vecSection.push_back(*sec);
       }

        optParam->Index = IntOptIndex;
        optParam->Name = vecOptName.at(0);

        vecOptInfo.push_back(optParam);
    }

    if(pIniFile)
    {
        delete pIniFile;
        pIniFile=NULL;
    }
    return true;
}

/*保存某个合约设置信息到INI文件中，保存在程序运行目录*/
void CAlgoAutoData::SaveIni(void)
{
    char PathFileName[512];
    CString temp;

    //获取程序路径
    memset(PathFileName,0,sizeof(PathFileName));
    CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_AlgoAutoData));
    CString p;
    p.Format("%s_%s",InstrumentName.c_str(),IniFileName_AlgoAutoData);
    strcat(PathFileName,p);
    CIniFile *pIniFile=new CIniFile(PathFileName);
    if(!pIniFile) return;

    pIniFile->UpdateKeyStr("AlgoAutoData","InstrumentName",InstrumentName.c_str());
    pIniFile->UpdateKeyVal("AlgoAutoData","InstrumentDirection",InstrumentDirection);
    pIniFile->UpdateKeyVal("AlgoAutoData","InstrumentOffsetID",InstrumentOffsetID);
    pIniFile->UpdateKeyStr("AlgoAutoData","InstrumentCount",InstrumentCount.c_str());
    pIniFile->UpdateKeyStr("AlgoAutoData","LimitPrice",LimitPrice.c_str());
    pIniFile->UpdateKeyVal("AlgoAutoData","MarketDegreed",MarketDegreed);
    pIniFile->UpdateKeyStr("AlgoAutoData","OrderCountValue",OrderCount.value.c_str());
    pIniFile->UpdateKeyVal("AlgoAutoData","OrderCountType",OrderCount.type);
//     pIniFile->UpdateKeyStr("AlgoAutoData","ResetCountValue",ResetCount.value.c_str());
//     pIniFile->UpdateKeyVal("AlgoAutoData","ResetCountType",ResetCount.type);
    pIniFile->UpdateKeyVal("AlgoAutoData","BasicPriceType",BasicPrice.type);
    pIniFile->UpdateKeyStr("AlgoAutoData","IdleTimeValue",IdleTime.value.c_str());
    pIniFile->UpdateKeyVal("AlgoAutoData","IdleTimeType",IdleTime.type);
    pIniFile->UpdateKeyStr("AlgoAutoData","ExecuteTimeValue",ExecuteTime.value.c_str());
    pIniFile->UpdateKeyVal("AlgoAutoData","ExecuteTimeType",ExecuteTime.type);
    pIniFile->UpdateKeyStr("AlgoAutoData","RetryTotalTimeValue",RetryTotalTime.value.c_str());
    pIniFile->UpdateKeyVal("AlgoAutoData","RetryTotalTimeType",RetryTotalTime.type);
    pIniFile->UpdateKeyStr("AlgoAutoData","RetryCount",RetryCount.c_str());
    pIniFile->UpdateKeyVal("AlgoAutoData","bActiceOverTime",(int)bActiceOverTime);
    pIniFile->UpdateKeyVal("AlgoAutoData","bActiceTotalTime",(int)bActiceTotalTime);
    pIniFile->UpdateKeyVal("AlgoAutoData","bActiceOpt",(int)bActiceOpt);
    pIniFile->UpdateKeyVal("AlgoAutoData","OptimizeIndex",OptimizeIndex);
//     pIniFile->UpdateKeyVal("AlgoAutoData","bMultiPrice",(int)bMultiPrice);

    //五档
    pIniFile->UpdateKeyVal("AlgoAutoData","SlidePriceSpreadName",WDDATA_PriceSpread.index);
    pIniFile->UpdateKeyVal5("AlgoAutoData","SlidePriceSpreadData",WDDATA_PriceSpread.data);
//     pIniFile->UpdateKeyVal("AlgoAutoData","RelateOrderCountName",WDDATA_OrderCount.index);
//     pIniFile->UpdateKeyVal5("AlgoAutoData","RelateOrderCountData",WDDATA_OrderCount.data);

    //优化参数
    int nIndex = 0;
    if (vecOptInfo.size() != 0)
    {
        int optCount = vecOptInfo.size();
        pIniFile->UpdateKeyVal("Opts","optCount",optCount);

        vector<OPTPARAM*>::iterator iterOpt = vecOptInfo.begin();
        vector<SINGLEOPT*>::iterator iterSingle;

        vector<int>     vecIntIndex;
        vector<int>     vecIntBackTime;
        vector<int>     vecIntOptMarketDegreed;
        vector<int>     vecIntPriceChangeType;
        vector<int>		vecIntOptSlidePriceSpreadName; 	
        vector<string>  vecDoubleMultiple;
        vector<string>  vecRange;
        vector<string>  vecOption;
        vector<string>  vecHistory;
        vector<string>  vecDoublePriceChangeFromValue;
        vector<string>  vecDoublePriceChangeToValue;
        vector<string>  vecDoubleOptSlidePriceSpreadData;

        int singleCount;
        string strTemp;

        for (;iterOpt!=vecOptInfo.end();iterOpt++,nIndex++)
        {
            vecIntIndex.clear();
            vecIntBackTime.clear();
            vecIntOptMarketDegreed.clear();
            vecIntPriceChangeType.clear();
            vecDoubleMultiple.clear();
            vecRange.clear();
            vecOption.clear();
            vecHistory.clear();
            vecDoublePriceChangeFromValue.clear();
            vecDoublePriceChangeToValue.clear();
            vecDoubleOptSlidePriceSpreadData.clear();

            for (iterSingle = (*iterOpt)->vecSingle.begin();iterSingle != (*iterOpt)->vecSingle.end();iterSingle++)
            {
                vecIntIndex.push_back((*iterSingle)->Index);
                vecIntBackTime.push_back((*iterSingle)->BackTime);
                vecIntOptMarketDegreed.push_back((*iterSingle)->OptMarketDegreed);
                vecIntPriceChangeType.push_back((*iterSingle)->PriceChange.type); 
                vecDoubleMultiple.push_back((*iterSingle)->Multiple);
                vecRange.push_back((*iterSingle)->Range);
                vecOption.push_back((*iterSingle)->Option);
                vecHistory.push_back((*iterSingle)->History);
                vecDoublePriceChangeFromValue.push_back((*iterSingle)->PriceChange.valueFrom);
                vecDoublePriceChangeToValue.push_back((*iterSingle)->PriceChange.valueTo);

                vecIntOptSlidePriceSpreadName.push_back((*iterSingle)->OptSlidePriceSpread.index);
                for (int i=0;i<5;i++)
                   vecDoubleOptSlidePriceSpreadData.push_back((*iterSingle)->OptSlidePriceSpread.data[i]);
            } 

            char buf[265] = {0};
            sprintf(buf,"%s%d","Opt",nIndex);
            singleCount = (*iterOpt)->vecSingle.size();
            pIniFile->UpdateKeyVal(buf,"optIndex",nIndex);
            pIniFile->UpdateKeyStr(buf,"optName",((*iterOpt)->Name).c_str());
            pIniFile->UpdateKeyVal(buf,"singleCount",singleCount);
            pIniFile->UpdateKeyVal4(buf,"singleIndex",vecIntIndex);
            pIniFile->UpdateKeyVal4(buf,"BackTime",vecIntBackTime);
            pIniFile->UpdateKeyVal5(buf,"Multiple",vecDoubleMultiple);
            pIniFile->UpdateKeyVal5(buf,"Range",vecRange);
            pIniFile->UpdateKeyVal5(buf,"Option",vecOption);
            pIniFile->UpdateKeyVal5(buf,"History",vecHistory);
            pIniFile->UpdateKeyVal4(buf,"OptMarketDegreed",vecIntOptMarketDegreed);
            pIniFile->UpdateKeyVal4(buf,"PriceChangeType",vecIntPriceChangeType);                    
            pIniFile->UpdateKeyVal5(buf,"PriceChangeFromValue",vecDoublePriceChangeFromValue);
            pIniFile->UpdateKeyVal5(buf,"PriceChangeToValue",vecDoublePriceChangeToValue);
            pIniFile->UpdateKeyVal4(buf,"OptSlidePriceSpreadName",vecIntOptSlidePriceSpreadName);
            pIniFile->UpdateKeyVal5(buf,"OptSlidePriceSpreadData",vecDoubleOptSlidePriceSpreadData);
        }
    }else
    {
        pIniFile->UpdateKeyVal("Opts","optCount",0);
    }

    if(pIniFile)
    {
        delete pIniFile;
        pIniFile=NULL;
    }

}
/*从INI文件读取五档行情数据字符串，然后保存到vector中*/
void CAlgoAutoData::AnalysisData(vector<string>& vec, char *pBuf)
{
    if(pBuf == NULL) return;
    vec.clear();
    char *point[1024], *p = pBuf;
    int i, j;

    for (i = 0; i < 1024; i++)
    {   
        point[i] = p;
        if ((p = strchr(p, 59)) == NULL) break;
        else *p++ = '\0';
    }   

    for (j = 0; j <= i; j++)
        vec.push_back(point[j]);
}
/*四舍五入算法*/
int CAlgoAutoData::DfxRound(double dValue){
    int j;
    j=(int)(dValue+0.5f);
    return j;
}

/*
每次根据实时行情数据，计算出每次的下单总数量，重置数量，拆分后的五档价格/数量数据。经过数据检验无误后将要下单的最终数据保存在vecOrderInfo中。
策略算法要求：
a)	每次下单总量，既不能超过现量乘以市场参与度的值。如果超过参与度的计算值，则使用参与度下单。如果超过品种的最大下单量。则根据最大下单量继续分拆下单，保证总数不变。
b)	交易总量即将达到时，下单量不能超过剩余数量。
c)	每次下单价格、不能超过限价。如果超过则使用限价进行下单。
*/
BOOL CAlgoAutoData::AutoOrderCalculate()
{ 
    printf("进入AutoOrderCalculate算法\n");

    //计算每次下单总量    
    int nTotal = 0;
    BOOL bBuy = FALSE;
    vecOrderInfo.clear();

    if(0 == OrderCount.type)//百分比
    {
        switch(InstrumentDirection)
        {
        case 0://买入
            bBuy = TRUE;
            OrderCountValue = (int)((atof(OrderCount.value.c_str()) * AskVolume)/100.0f);//买量
            break;
        case 1://卖出
            bBuy = FALSE;
            OrderCountValue = (int)((atof(OrderCount.value.c_str()) * BidVolume)/100.0f);//买量
        }
    }else if(1 == OrderCount.type)//手数
    {
        OrderCountValue = atoi(OrderCount.value.c_str());

        switch(InstrumentDirection)
        {
        case 0://买入
            bBuy = TRUE;
            break;
        case 1://卖出
            bBuy = FALSE;
        }
    }

    //每次下单总量，不能超过现量乘以市场参与度的值。
    if (MarketDegreed != 0 && OrderCountValue > (int)(NewVolume * (MarketDegreed/100.0f)))
    {
        OrderCountValue = (int)(NewVolume * (MarketDegreed/100.0f));            
    }

    if(OrderCountValue <= 0)
        return FALSE;

//     if(bPatchOrder)
//         PatchOrderCountValue = OrderCountValue - WaitVolume;

    char p[32]={0};
//     switch(ResetCount.type)//每次重置数量 为0时，忽略不考虑
//     { 
//     case 0:
//         itoa((int)((atof(ResetCount.value.c_str())*OrderCountValue)/100.0f),p,10);
//         break;
//     case 1:
//         itoa(atoi(ResetCount.value.c_str()),p,10);
//         break;
//     }
// 
//     ResetCount.value = p;
//     ResetCountValue = atoi(p);

    int nRestCount = 0;//总剩余数量
    switch(InstrumentOffsetID)
    {
    case 0://开仓    
        nRestCount = atoi(InstrumentCount.c_str()) - TotalVolume;
        break;
    case 1://平今            
        if(bBuy)
            nRestCount = ((atoi(InstrumentCount.c_str()) - TotalVolume)>=m_tSellPosition.TodayPosition)?m_tSellPosition.TodayPosition:(atoi(InstrumentCount.c_str()) - TotalVolume);
        else
            nRestCount = ((atoi(InstrumentCount.c_str()) - TotalVolume)>=m_tBuyPosition.TodayPosition)?m_tBuyPosition.TodayPosition:(atoi(InstrumentCount.c_str()) - TotalVolume);
        break;
    case 2://平仓
        if(bBuy)
            nRestCount = ((atoi(InstrumentCount.c_str()) - TotalVolume)>=m_tSellPosition.YdPositionRemain)?m_tSellPosition.YdPositionRemain:(atoi(InstrumentCount.c_str()) - TotalVolume);
        else
            nRestCount = ((atoi(InstrumentCount.c_str()) - TotalVolume)>=m_tBuyPosition.YdPositionRemain)?m_tBuyPosition.YdPositionRemain:(atoi(InstrumentCount.c_str()) - TotalVolume);
        break;        
    }

    ORDERINFO orderInfo;
    orderInfo.name = InstrumentName;
    orderInfo.direction = (InstrumentDirection == 0)?1:0;
    if (InstrumentOffsetID == 0) orderInfo.offsetID = 1;
    else if(InstrumentOffsetID == 1) orderInfo.offsetID = 2;
    else orderInfo.offsetID = 3;

    //计算多档下单量
    vector<string> vecPriceSpread = WDDATA_PriceSpread.data;
//     vector<string> vecOrderCount = WDDATA_OrderCount.data;

    int nVoulum = 0;//重新统计本次下单数量
    int nCountSum = 0;
    double nSum = 0.0f;
    int nCount = 0;

    int nComboPriceIndex = -1;
    int nComboCountIndex = -1;

    /*
    for (int i=0;i<(int)vecOrderCount.size();i++)
    {  
        //初始化
        orderInfo.count = 0;
        orderInfo.price = 0;
        orderInfo.index = i;

        if (WDDATA_OrderCount.index == 0 && WDDATA_PriceSpread.index == 0 && nRestCount > 0)//百分比 百分比
        {
            nCount = DfxRound(OrderCountValue*((atof(vecOrderCount[i].c_str()) + nSum)/100.0f)) - nCountSum;
            orderInfo.count = (nRestCount < nCount)?nRestCount:nCount;
            orderInfo.price = atof(BasicPrice.value.c_str())*(1+atof(vecPriceSpread[i].c_str())/100.0f);

            nCountSum += orderInfo.count;
            nSum += atof(vecOrderCount[i].c_str());
            nRestCount -= orderInfo.count;
            nVoulum += orderInfo.count;

        }else if(WDDATA_OrderCount.index == 0 && WDDATA_PriceSpread.index == 1 && nRestCount > 0)//百分比 点位 
        {
            //int nRealOrderCount = (bPatchOrder)?PatchOrderCountValue:OrderCountValue;
            //int nRealOrderCount = OrderCountValue;
            nCount = DfxRound(OrderCountValue*((atof(vecOrderCount[i].c_str()) + nSum)/100.0f)) - nCountSum;
            orderInfo.count = (nRestCount < nCount)?nRestCount:nCount;
            orderInfo.price = atof(BasicPrice.value.c_str())+(atof(vecPriceSpread[i].c_str()))*PriceTick;

            nCountSum += orderInfo.count;
            nSum += atof(vecOrderCount[i].c_str());
            nRestCount -= orderInfo.count;
            nVoulum += orderInfo.count;

        }
        else if (WDDATA_OrderCount.index == 1 && WDDATA_PriceSpread.index == 0)//手数 百分比
        {
            orderInfo.count = ((int)atof(vecOrderCount[i].c_str()) <= nRestCount)?(int)atof(vecOrderCount[i].c_str()):nRestCount;
            orderInfo.price = atof(BasicPrice.value.c_str())*(1+atof(vecPriceSpread[i].c_str())/100.0f);
            nRestCount -= orderInfo.count;
            nVoulum += orderInfo.count;

        }
        else if (WDDATA_OrderCount.index == 1 && WDDATA_PriceSpread.index == 1)//手数 点位
        {
            orderInfo.count = ((int)atof(vecOrderCount[i].c_str()) <= nRestCount)?(int)atof(vecOrderCount[i].c_str()):nRestCount;
            orderInfo.price = atof(BasicPrice.value.c_str())+atof(vecPriceSpread[i].c_str())*PriceTick;
            nRestCount -= orderInfo.count;
            nVoulum += orderInfo.count;
        }*/

        if(nRestCount > 0)
        {
            orderInfo.count = (OrderCountValue <= nRestCount)?OrderCountValue:nRestCount;
            orderInfo.price = atof(BasicPrice.value.c_str())+atof(vecPriceSpread[0].c_str())*PriceTick;
            nRestCount -= orderInfo.count;
            nVoulum += orderInfo.count;
        }


        //限价处理
        //CheckSumPrice(orderInfo.price,PriceTick,0);
        if(0 == InstrumentDirection)//买入
        {            
            if (orderInfo.price > atof(LimitPrice.c_str()))
                orderInfo.price = atof(LimitPrice.c_str());
        }
        else if (1 == InstrumentDirection)
        {
            if (orderInfo.price < atof(LimitPrice.c_str()))
                orderInfo.price = atof(LimitPrice.c_str());
        }

        if (orderInfo.count != 0)
             m_nOrderCount++;

        vecOrderInfo.push_back(orderInfo);
//     }

    itoa(nVoulum,p,10);
    OrderCount.value = p;

    return TRUE;
}
