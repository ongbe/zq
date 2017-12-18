#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "IniFile.h"
#include "Tools_Win32.h"
#include "Tools_AlgoTrading.h"

#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

//保存策略计算参数的ini文件名
#define IniFileName_AlgoAutoData "AlgoAutoData.ini"

typedef struct ORDERLATEST
{
    PlatformStru_OrderInfo    orderInfo; 
    time_t                    time;  

}ORDERLATEST,*pORDERLATEST;

typedef struct ORDERWAITCANCEL
{
    BOOL    bSucess; 
    int     nOrderRef;     

    ORDERWAITCANCEL()
    {
        bSucess = FALSE;
        nOrderRef = -1;
    }

}ORDERWAITCANCEL,*pORDERWAITCANCEL;

struct ORDERPARAM
{
    string value;
    string valueFrom;
    string valueTo;
    int   type;
};
//下单信息封装
struct ORDERINFO 
{
    string			name;	            //合约名称
    int				direction;			//方向，0:买；1:卖
    int				offsetID;		    //开平，0:开仓 1:平今 2:平仓
    double          price;	            //价格
    int             count;              //数量
    int             index;              //几档
};

//下单请求回应封装
struct ORDERRSP 
{
    bool            finish;             //是否成交
    string 		    orderRef;	        //合约名称
    double          price;	            //价格
    int             count;              //数量
    int             realCount;    

    ORDERLATEST     status;             //最新状态
    bool            cancle;             //是否要撤
    bool            bStatus;            //状态是否改变 
    bool            bRspArrive;         //下单是否回应

    ORDERRSP()
    {
        finish = false;
        cancle = false;
        bStatus = false;
        bRspArrive = false;
        price = 0.0f;
        count = 0;
    }

};

typedef struct 
{
    double low;
    double high;
    int index; 
}section;

typedef struct 
{
    int index;//0 点位 1 手数
    vector<string> data;
}WDDATA_PRICESPREAD;

typedef struct 
{
    int index;//0 百分比 1 手数
    vector<string> data;
}WDDATA_ORDERCOUNT;

//优化索引对应的参数
typedef struct SINGLEOPT
{
    int             Index;                          //索引
    int             BackTime;                       //回溯时间秒数
    ORDERPARAM      PriceChange;                    //价格变动范围
    string          Multiple;                       //加速倍数
    int             OptMarketDegreed;               //市场参与度            
    WDDATA_PRICESPREAD OptSlidePriceSpread;       //滑动价差

    string Range;                                  //Detil list 范围项
    string Option;                                 //Detil list 优化参数项
    string History;                                //Detil list 历史价格项

}SINGLEOPT,pSINGLEOPT;
//优化设置信息封装
typedef struct OPTPARAM
{
    int             Index;                           //优化索引
    string          Name;                            //名称
    vector<SINGLEOPT*> vecSingle;                    //保存所有的参数
    vector<section> vecSection;                      //保存所有的价格区间

    OPTPARAM()
    {
        Index = -1;
        vecSingle.clear();
        vecSection.clear();
    }

    ~OPTPARAM()
    {
        vector<SINGLEOPT*>::iterator iter = vecSingle.begin();
        for(;iter!=vecSingle.end();iter++)
        {
            SINGLEOPT* pSingle = (*iter);
            if (pSingle != NULL)
            {
                delete pSingle;
                pSingle = NULL;
            }
        }

        vecSingle.clear();
    }
}OPTPARAM,*POPTPARAM;


class CAlgoAutoData
{
public:
    CAlgoAutoData(void);
    CAlgoAutoData(const CAlgoAutoData& r);
    ~CAlgoAutoData(void);

    CAlgoAutoData& operator= (const CAlgoAutoData& r);

    bool ReadIni(string& strInstrumentID); 
    void SaveIni(void);
    BOOL AutoOrderCalculate();//下单计算


    int             Volume;                         //每次下单的成交量
    int             WaitVolume;                     //每次下单的挂单量
    double			AverageQuan;				    //平均加权
    int             TotalVolume;                    //总成交量(每次下单成交量之和)
    int             RetryCountSave;                 //记录重试次数
    int             RetryTotalTimeSave;             //记录重试总时长


    //柱状图相关
    int				TradingTime;			        //交易次数
    int				TradingMinute;					//交易时段范围包括的分钟数
    vector<int>		vecTradedVolumePerMinute;       //记录每分钟成交的量
    vector<int>		vecTradedVolumePerTime;         //记录每次下单成交的量

    //界面项
    string			InstrumentName;	                //合约名称
    int				InstrumentDirection;			//方向，0:买；1:卖
    int				InstrumentOffsetID;		    	//开平，0:开仓 1:平今 2：平仓
    string          InstrumentCount;	            //交易总数量
    string          LimitPrice;                     //限价               
    CTime           StartTime;                      //交易开始时间
    CTime           EndTime;                        //交易结束时间
    int             MarketDegreed;                  //市场参与度
    ORDERPARAM      OrderCount;                     //每次下单数量
//     ORDERPARAM      ResetCount;                     //每次重置数量
    ORDERPARAM      BasicPrice;                     //盯盘价格
    ORDERPARAM      IdleTime;                       //空闲时间
    ORDERPARAM      ExecuteTime;                    //执行时间
    ORDERPARAM      RetryTotalTime;                 //超时重试总时间
    string          RetryCount;                     //超时重试次数
    bool            bActiceOverTime;                //是否启动超时重试次数
    bool            bActiceTotalTime;               //是否启动超时重试总时长
    bool            bActiceOpt;                     //是否启动优化
    int             OptimizeIndex;                  //优化索引

//     bool            bMultiPrice;                    //是否启用多档价格
//     bool            bPatchOrder;                    //是否是补单
    WDDATA_PRICESPREAD  WDDATA_PriceSpread;         //滑动价差
//     WDDATA_ORDERCOUNT   WDDATA_OrderCount;          //对应下单量


    //行情项
    int             NewVolume;                      //现交易量
    int             BidVolume;                      //买量
    int             AskVolume;                      //卖量
    double          LatestPrice;                    //最新价
    double          PriceTick;                      //最小变动价位
   
    PlatformStru_Position m_tBuyPosition;             // 买持仓信息
    PlatformStru_Position m_tSellPosition;             // 卖持仓信息

    //计算结果
    int             OrderCountValue;                //每次下单量（对于百分比时）
    int             PatchOrderCountValue;           //本次实际下单量
//     int             ResetCountValue;             //每次重置单量（对于百分比时）
    BOOL            bIdleTime;
    BOOL            bExecuteTime;
    int             IdleTimeValue;                  //秒
    int             ExecuteTimeValue;               //秒
    int             RetryTotalTimeValue;            //秒
    // 记录下单档数
    int m_nOrderCount;

    vector<ORDERINFO>    vecOrderInfo;              //常规的下单
    vector<ORDERINFO>    vecOrderActiveInfo;        //优化条件的下单
    vector<OPTPARAM*>    vecOptInfo;                //保存的优化设置

    //请求相应
    vector<ORDERRSP>            vecOrderRSP;        //记录本次下单的报单

private:       
    void AnalysisData(vector<string>& vec, char *pBuf);//读取容器中的数据
    int  DfxRound(double dValue);//四舍五入
};
