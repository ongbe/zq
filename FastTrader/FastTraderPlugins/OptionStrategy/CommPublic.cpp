#include "stdafx.h"
#include "CommPublic.h"

#include "Geometry.h"
#include "PLProbability.h"

CDataInfo* CDataInfo::m_pInstance = NULL;

CDataInfo::CDataInfo()
{    
    m_bInit = false;
    m_dQuotLastPrice = 0.0;
    m_dVolatility = 0.0;
    m_dVolatilityTest = 0.0;
    m_nRelaInstCount = 5;
    m_vStrategyData.clear(); 

    m_PLProbability = NULL;

    m_bExchangeFlag = false;
    m_bProductFlag = false; 
    m_bInstrumentFlag = false;
    m_bCaculateFlag = false;
    m_LastPriceFlag = false;
    m_VolatilityFlag = false;
    m_bVolatilityTestFlag = false;

    m_time1 = m_time2 = NULL;
#ifdef _LOG 
    m_pLog = NULL;
    if(m_pLog==NULL)
    {
        //获取程序路径
        char PathFileName[512];
        memset(PathFileName,0,sizeof(PathFileName));

        int i,iSize = sizeof(PathFileName)-1-strlen(LogFileName_OptionStrategy);
        if(iSize<2) return;

        memset(PathFileName,0,iSize);
        GetModuleFileName(NULL,PathFileName,iSize-1);
        i=strlen(PathFileName);
        while(i>0&&PathFileName[i-1]!='\\'&&PathFileName[i-1]!=':') 
            PathFileName[--i]=0;
        if(PathFileName[i-1]!='\\')
            strcat((char*)PathFileName,"\\");

        strcat(PathFileName,LogFileName_OptionStrategy);    

        m_pLog = new CLogFile(PathFileName);
    }
#endif
}

CDataInfo::~CDataInfo()
{
    if(m_PLProbability)
    {
        delete m_PLProbability;
        m_PLProbability = NULL;
    }

    if(m_time1)
    {
        delete m_time1;
        m_time1 = NULL;
    }

    if(m_time2)
    {
        delete m_time2;
        m_time2 = NULL;
    }
#ifdef _LOG 
    if(m_pLog)
    {
        delete m_pLog;
        m_pLog = NULL;
    }
#endif
}

CDataInfo* CDataInfo::NewInstance()
{
    if( m_pInstance == NULL ) 
    {
        m_pInstance = new CDataInfo();
    }

    return m_pInstance;
}

void CDataInfo::DestroyInstance()
{
    if( m_pInstance != NULL) 
    {		
        delete m_pInstance;
        m_pInstance = NULL;
    }
}


//////////////////////////////////////////////////////////////////////////
void CDataInfo::SetBalancePoint(const vector<sBalancePt>& vBalancePt)
{
    m_vBalancePos = vBalancePt;
}
void CDataInfo::GetBalancePoint(vector<sBalancePt>& vBalancePt)
{
    vBalancePt = m_vBalancePos;
}

void CDataInfo::SetMouseIn(bool bMouseIn)
{
    m_bMouseIn = bMouseIn;
}
bool CDataInfo::GetMouseIn()
{
    return m_bMouseIn;
}

void CDataInfo::SetMousePt(CPoint sMousePt)
{
    m_ptMouse = sMousePt;
}
CPoint CDataInfo::GetMousePt()
{
    return m_ptMouse;
}
void CDataInfo::SetTradingDayFlag(bool bTradingDay)
{
    m_bTradingDayFlag = bTradingDay;
}
void CDataInfo::SetExchangeFlag(bool bArrived)
{
    m_bExchangeFlag = bArrived;
}
void CDataInfo::SetProductFlag(bool bArrived)
{
    m_bProductFlag = bArrived;
}
void CDataInfo::SetInstrumentFlag(bool bArrived)
{
    m_bInstrumentFlag = bArrived;
}
bool CDataInfo::GetTradingDayFlag()
{
    return m_bTradingDayFlag;
}
bool CDataInfo::GetExchangeFlag()
{
    return m_bExchangeFlag;
}
bool CDataInfo::GetProductFlag()
{
    return m_bProductFlag;
}
bool CDataInfo::GetInstrumentFlag()
{   
    return m_bInstrumentFlag;
}
void CDataInfo::SetVolatilityFlag(bool bArrived)
{
    m_VolatilityFlag = bArrived;
}
void CDataInfo::SetLastPriceFlag(bool bArrived)
{
    m_LastPriceFlag = bArrived;
}
bool CDataInfo::GetCaculateFlag()
{
    return m_bCaculateFlag;
}
void CDataInfo::SetVolatilityTestFlag(bool bTest)
{
    m_bVolatilityTestFlag = bTest;
    if(bTest)
        m_dVolatilityTest = m_dVolatility;
}
bool CDataInfo::GetVolatilityTestFlag()
{
    return m_bVolatilityTestFlag;
}
void    CDataInfo::SetInit(bool bInit)
{
    m_bInit = bInit;
}
bool    CDataInfo::GetInit()
{
    return m_bInit;
}
string  CDataInfo::GetAccount()
{
    return m_szAccount;
}
void    CDataInfo::SetVolatility(const double& dVolatility)
{
    m_dVolatility = ((int)(dVolatility*10000))/10000.0;        
}

void    CDataInfo::SetVolatilityTest(const double& dVolatilityTest)
{
    m_dVolatilityTest = ((int)(dVolatilityTest*10000))/10000.0;
}

double  CDataInfo::GetVolatility()
{
    return m_dVolatility;
}
void CDataInfo::GetProfitDlgParams(CTime& tExpireDate,string& strRestDays)
{     
    string sDate = GetExpireDate();
    int iYear,iMonth,iDay,WeekendDays,RemainDays;
    char buf[9];
    memset(buf,0,sizeof(buf));
    memcpy(buf,sDate.c_str(),sizeof(buf));
    sscanf(buf,"%04d%02d%02d",&iYear,&iMonth,&iDay);

    CTime time(iYear,iMonth,iDay,0,0,0);
    tExpireDate = time;

    SYSTEMTIME sys;
    GetLocalTime(&sys);  
 
    CEasyTime m_time1((int)sys.wYear,(int)sys.wMonth,(int)sys.wDay);

    memset(buf,0,sizeof(buf));
    memcpy(buf,m_InstrumentInfo.ExpireDate,sizeof(buf));
    sscanf(buf,"%04d%02d%02d",&iYear,&iMonth,&iDay);

    CEasyTime m_time2(iYear,iMonth,iDay);
    CEasyTimespan<time_t> timespan1=CEasyTimespan<time_t>::getWorkdayTimespan(m_time1.getutc(),m_time2.getutc(),WeekendDays);
    CEasyTimespan<time_t> timespan2= CEasyTimespan<time_t>:: split_TimespanToEveryDay (timespan1);
    RemainDays=timespan2. getTimespans().size();

    memset(buf,0,sizeof(buf));
    itoa(RemainDays,buf,10);
    strRestDays = (string)buf;
}
void    CDataInfo::SetRelaInstCount(const int& nCount)
{
    m_nRelaInstCount = nCount;
}
int     CDataInfo::GetRelaInstCount()
{
    return m_nRelaInstCount;
}

void    CDataInfo::SetStrategyData(const vector <sStrategyData>& vData)
{
    m_vStrategyData = vData;
}
void    CDataInfo::GetStrategyData(vector <sStrategyData>& vData)
{
    vData = m_vStrategyData;
}
void    CDataInfo::SetChartInfo(CHART_TYPE eType, const CRect& rc)
{
    m_ChartType = eType;

    if(eProbaChart == eType)
        CopyRect(m_ProbaRect,rc);
    else if(eProfitChart == eType)
        CopyRect(m_ProfitRect,rc);
}
void    CDataInfo::Calculate()
{
    CeasymutexGuard guard(m_mutexLastQuot);

    m_bCaculateFlag = false;

    if(!m_bInit)
        return;

    SYSTEMTIME sys;
    GetLocalTime(&sys);  

    int iYear,iMonth,iDay,WeekendDays,RemainDays;
    if(m_time1)
    {
        delete m_time1;
        m_time1 = NULL;
    }
    m_time1 = new CEasyTime((int)sys.wYear,(int)sys.wMonth,(int)sys.wDay);    

    char buf[9];
    memset(buf,0,sizeof(buf));
    memcpy(buf,m_InstrumentInfo.ExpireDate,sizeof(buf));
    sscanf(buf,"%04d%02d%02d",&iYear,&iMonth,&iDay);

    if(m_time2)
    {
        delete m_time2;
        m_time2 = NULL;
    }
    m_time2 = new CEasyTime(iYear,iMonth,iDay);
    CEasyTimespan<time_t> timespan1=CEasyTimespan<time_t>::getWorkdayTimespan(m_time1->getutc(),m_time2->getutc(),WeekendDays);
    CEasyTimespan<time_t> timespan2= CEasyTimespan<time_t>:: split_TimespanToEveryDay (timespan1);
    RemainDays=timespan2. getTimespans().size();

    if(m_PLProbability)
    {
        delete m_PLProbability;
        m_PLProbability = NULL;
    }

    if(!m_LastPriceFlag || !m_VolatilityFlag)
    {
        if(!m_VolatilityFlag)
            m_strNote = _T("波动率没有查询到");
        if(!m_LastPriceFlag)
            m_strNote = _T("最新价没有查询到");
        return;
    }

    m_PLProbability = new CPLProbability(m_InstrumentDepthData.LastPrice, m_bVolatilityTestFlag?m_dVolatilityTest:m_dVolatility, RemainDays);
    m_PLProbability->GetRange(m_dValueMin, m_dValueMax); 
    
    SetXAxisTick();
    SetYAxisTick();

    if(eProfitChart == m_ChartType)
    {
        SetPos();
        m_bCaculateFlag = true;
    }    
}
void CDataInfo::CalculateTest()
{
    m_bCaculateFlag = false;

    SYSTEMTIME sys;
    GetLocalTime(&sys);  

    int iYear,iMonth,iDay,WeekendDays,RemainDays;
    if(m_time1)
    {
        delete m_time1;
        m_time1 = NULL;
    }
    m_time1 = new CEasyTime((int)sys.wYear,(int)sys.wMonth,(int)sys.wDay);    

    char buf[9];
    memset(buf,0,sizeof(buf));
    memcpy(buf,m_InstrumentInfo.ExpireDate,sizeof(buf));
    sscanf(buf,"%04d%02d%02d",&iYear,&iMonth,&iDay);

    if(m_time2)
    {
        delete m_time2;
        m_time2 = NULL;
    }
    m_time2 = new CEasyTime(iYear,iMonth,iDay);
    CEasyTimespan<time_t> timespan1=CEasyTimespan<time_t>::getWorkdayTimespan(m_time1->getutc(),m_time2->getutc(),WeekendDays);
    CEasyTimespan<time_t> timespan2= CEasyTimespan<time_t>:: split_TimespanToEveryDay (timespan1);
    RemainDays=timespan2. getTimespans().size();

    if(m_PLProbability)
    {
        delete m_PLProbability;
        m_PLProbability = NULL;
    }

    if(!m_LastPriceFlag || !m_VolatilityFlag)
    {
        if(!m_VolatilityFlag)
            m_strNote = _T("波动率没有查询到");
        if(!m_LastPriceFlag)
            m_strNote = _T("最新价没有查询到");
        return;
    }

    m_PLProbability = new CPLProbability(m_InstrumentDepthData.LastPrice, m_dVolatilityTest, RemainDays);
    m_PLProbability->GetRange(m_dValueMin, m_dValueMax); 

    SetXAxisTick();
    SetYAxisTick();

    if(eProfitChart == m_ChartType)
    {
        SetPos();
        m_bCaculateFlag = true;
    }    
}

void CDataInfo::SetXAxisTick()
{ 
    m_vecXAxisPos.clear();

    double  dXAxisPos;      //x物理坐标(实际像素)       
    double  dXAxisPosValue; //x逻辑坐标
    double  dXAxisPosFirst, dXAxisPosValueFirst, dXAxisPosSecond, dXAxisPosValueSecond;
    double  dRectLeft  = (eProbaChart==m_ChartType)?m_ProbaRect.left:m_ProfitRect.left;
    double  dRectRight = (eProbaChart==m_ChartType)?m_ProbaRect.right:m_ProfitRect.right;
    double  dRectWidth = (eProbaChart==m_ChartType)?m_ProbaRect.Width():m_ProfitRect.Width();

    double  dXAxisPerSpaceW     =   dRectWidth/(m_nRelaInstCount+2);
    m_dPriceSpace               =   (m_dValueMax-m_dValueMin)/(m_nRelaInstCount-1);
    m_dPriceMin                 =   m_dValueMin-m_dPriceSpace;
    double  dPriceMax           =   m_dValueMax+m_dPriceSpace;
    double  dPriceRelaPosSpace  =   (dPriceMax-m_dPriceMin)/dRectWidth;
    double  dXDrawPos           = (dRectWidth - dRectWidth*(m_nRelaInstCount+1)/(m_nRelaInstCount+2))/2;//起始位置坐标

    CString str;
    for (int i=0;i<(m_nRelaInstCount+2);i++)
    {
        dXAxisPos = dRectLeft + dXDrawPos + i*dXAxisPerSpaceW;
        m_vecXAxisPos.push_back(dXAxisPos);

        dXAxisPosValue = (m_dPriceMin+i*m_dPriceSpace);
        if(i==0)
        {   
            dXAxisPosFirst = dXAxisPos;
            dXAxisPosValueFirst = dXAxisPosValue;
        }else if(i==1)
        {
            dXAxisPosSecond = dXAxisPos;
            dXAxisPosValueSecond = dXAxisPosValue;
        }
    }

    //确定X轴逻辑坐标最值
    m_dXAxisPosValueMin =  ( dXAxisPosValueFirst*dXAxisPosSecond - dXAxisPosValueSecond*dXAxisPosFirst + (dXAxisPosValueSecond-dXAxisPosValueFirst)*dRectLeft )/(dXAxisPosSecond-dXAxisPosFirst);
    m_dXAxisPosValueMax =  ( dXAxisPosValueFirst*dXAxisPosSecond - dXAxisPosValueSecond*dXAxisPosFirst + (dXAxisPosValueSecond-dXAxisPosValueFirst)*dRectRight )/(dXAxisPosSecond-dXAxisPosFirst);
}

void    CDataInfo::GetXAxisInfo(vector<double>& vXAxisPos, double& dPriceMin, double& dSpaceValue,double& dXPosValueMin,double& dXPosValueMax)
{
    vXAxisPos   = m_vecXAxisPos;
    dPriceMin   = m_dPriceMin;
    dSpaceValue = m_dPriceSpace;
    dXPosValueMin = m_dXAxisPosValueMin;
    dXPosValueMax = m_dXAxisPosValueMax;
}
void    CDataInfo::SetYAxisTick()
{
//     if(eProbaChart == m_ChartType)
//     {
        m_dYAxisPosValueMin1 = 0.0;
        m_dYAxisPosValueMax1 = -99999999.0;

        double dYposValue;
        for (double dPrice = m_dXAxisPosValueMin; dPrice < m_dXAxisPosValueMax; dPrice += 0.01)
        {
            dYposValue = m_PLProbability->CalcuProbabilityDensity(dPrice);
            if(dYposValue < 0.00001)
                dYposValue = 0.0;

            if(m_dYAxisPosValueMax1 < dYposValue)
                m_dYAxisPosValueMax1 = dYposValue;
        }
//     }
//     else if(eProfitChart == m_ChartType)
//     {       
        m_dYAxisPosValueMin2 = m_dYAxisPosValueMax2 = 0.0;

        sStrategyData sData;
        eOrderStyle eType = eNone;
        double      dYAxisPosValueMin2 = 0.0, dYAxisPosValueMax2 = 0.0;


        sort(m_vStrategyData.begin(),m_vStrategyData.end(),CompareOrderByStrikePriceASC);
        for(vector<sStrategyData>::iterator it=m_vStrategyData.begin();it!=m_vStrategyData.end();it++)
        {
            sData = *it;
            if(sData.eStyle == BuyBullish) //买入看涨
            { 
                //确定Y轴逻辑坐标最值
                dYAxisPosValueMin2 = -sData.dOrderPrice*sData.nOrderCount*sData.dUnderlyingMultiple;
                dYAxisPosValueMax2 = (m_dXAxisPosValueMax-sData.dStrikePrice-sData.dOrderPrice*sData.nOrderCount)*sData.dUnderlyingMultiple;
            }
            else if(sData.eStyle == SellBullish)//卖出看涨
            {
                //确定Y轴逻辑坐标最值
                dYAxisPosValueMin2 = -(m_dXAxisPosValueMax-sData.dStrikePrice-sData.dOrderPrice*sData.nOrderCount)*sData.dUnderlyingMultiple;
                dYAxisPosValueMax2 = sData.dOrderPrice*sData.nOrderCount*sData.dUnderlyingMultiple;
            }
            if(sData.eStyle == BuyBearish) //买入看跌
            {
                //确定Y轴逻辑坐标最值
                dYAxisPosValueMin2 = -sData.dOrderPrice*sData.nOrderCount*sData.dUnderlyingMultiple;
                dYAxisPosValueMax2 = (sData.dStrikePrice-m_dXAxisPosValueMin-sData.dOrderPrice*sData.nOrderCount)*sData.dUnderlyingMultiple;
            }
            else if(sData.eStyle == SellBearish)//卖出看跌
            {
                //确定Y轴逻辑坐标最值
                dYAxisPosValueMin2 = (m_dXAxisPosValueMin-sData.dStrikePrice+sData.dOrderPrice*sData.nOrderCount)*sData.dUnderlyingMultiple;
                dYAxisPosValueMax2 = sData.dOrderPrice*sData.nOrderCount*sData.dUnderlyingMultiple;
            }

            if(eType == eNone)
            {
                m_dYAxisPosValueMin2 += dYAxisPosValueMin2;
                m_dYAxisPosValueMax2 += dYAxisPosValueMax2;
            }
            else if(sData.eStyle == eType)
            {
                m_dYAxisPosValueMin2 += dYAxisPosValueMin2;
                m_dYAxisPosValueMax2 += dYAxisPosValueMax2;
            }
            else
            {      
                m_dYAxisPosValueMin2 += dYAxisPosValueMax2;
                m_dYAxisPosValueMax2 += dYAxisPosValueMin2;
            }

            eType = sData.eStyle;
        }
//     }
}
void    CDataInfo::SetPos()
{
    m_vPoint.clear();
    GetStrategyPos(m_vPoint);
    sort(m_vPoint.begin(),m_vPoint.end(),ComparePointByXASC);
    MergeStrategyPos(m_vPoint);

    //求平衡点
    vector<_LINESEG> _vLineSeg;//平衡点所在所有线段
    m_bBalance = GetBalancePos(m_vPoint, _vLineSeg);

    sort(m_vPoint.begin(),m_vPoint.end(),ComparePointByXASC);
}
void CDataInfo::GetStrategyPos(vector<_POINT>& _vPoint)
{
    sStrategyData sData;
    sTurnPt ptTurn;
    sBalancePt ptBalance;
    sPoint pt;
    vector<sPoint> vPos; 
    double  dYBalancePosValue = 0.0, dXBalancePosValue; //平衡点逻辑坐标
    double  dXMaxTurnPosValue,  dYMaxTurnPosValue;      //最大损益转折点逻辑坐标


    for(vector<sStrategyData>::iterator it=m_vStrategyData.begin();it!=m_vStrategyData.end();it++)
    {
        sData = *it;

        if(sData.eStyle == BuyBullish) //买入看涨
        { 
            dXMaxTurnPosValue = sData.dStrikePrice;
            dYMaxTurnPosValue = -sData.dOrderPrice*sData.nOrderCount*sData.dUnderlyingMultiple;
            dXBalancePosValue = sData.dStrikePrice + sData.dOrderPrice;
        }
        else if(sData.eStyle == SellBullish)//卖出看涨
        {
            dXMaxTurnPosValue = sData.dStrikePrice;
            dYMaxTurnPosValue = sData.dOrderPrice*sData.nOrderCount*sData.dUnderlyingMultiple;
            dXBalancePosValue = sData.dStrikePrice + sData.dOrderPrice;
        }
        if(sData.eStyle == BuyBearish) //买入看跌
        {
            dXMaxTurnPosValue = sData.dStrikePrice;
            dYMaxTurnPosValue = -sData.dOrderPrice*sData.nOrderCount*sData.dUnderlyingMultiple;
            dXBalancePosValue = sData.dStrikePrice - sData.dOrderPrice;
        }
        else if(sData.eStyle == SellBearish)//卖出看跌
        {
            dXMaxTurnPosValue = sData.dStrikePrice;
            dYMaxTurnPosValue = sData.dOrderPrice*sData.nOrderCount*sData.dUnderlyingMultiple;
            dXBalancePosValue = sData.dStrikePrice - sData.dOrderPrice;
        }

        ptTurn.x = dXMaxTurnPosValue;
        ptTurn.y = dYMaxTurnPosValue;

        ptBalance.x = dXBalancePosValue;
        ptBalance.y = dYBalancePosValue;

        pt.ptTurn = ptTurn;
        pt.ptBalance = ptBalance;
        pt.eStyle = sData.eStyle;
        vPos.push_back(pt);
    }

    //合并
    eOrderStyle eStyle;
    vector<_POINT> _vWinPots, _vLostPots;
    vector<_POINT>::iterator result;
    _POINT _PT,_PTLeft,_PTRight,_PTTurn,_PTBalance;
    _LINE  _Line,_LineLeft(1,0,-m_dXAxisPosValueMin),_LineRight(1,0,-m_dXAxisPosValueMax),_LineXY(1,-1,0);
    for(vector<sPoint>::iterator it=vPos.begin();it!=vPos.end();it++)
    {
        pt = *it;  

        _PTTurn = pt.ptTurn;
        _PTBalance = pt.ptBalance;

        _Line = makeline(_PTTurn,_PTBalance);

        if(pt.ptTurn.x - pt.ptBalance.x < 0.000001) //看涨 
        {
            eStyle = eBullish;
            lineintersect(_Line,_LineRight,_PT);
            _PTLeft.x = m_dXAxisPosValueMin;
            _PTLeft.y = _PTTurn.y;
        }
        else if(pt.ptTurn.x > pt.ptBalance.x) //看跌
        {
            eStyle = eBearish;
            lineintersect(_Line,_LineLeft,_PT);
            _PTRight.x = m_dXAxisPosValueMax;
            _PTRight.y = _PTTurn.y;
        }

        if(eStyle == eBullish)
        {
            result = find( _vPoint.begin( ), _vPoint.end( ), _PTLeft);
            if(result == _vPoint.end())
                _vPoint.push_back(_PTLeft);
            else
                result->y *= 2;

            result = find( _vPoint.begin( ), _vPoint.end( ), _PTTurn);
            if(result == _vPoint.end())
                _vPoint.push_back(_PTTurn);
            else
                result->y *= 2;

            result = find( _vPoint.begin( ), _vPoint.end( ), _PTBalance);
            if(result == _vPoint.end())
                _vPoint.push_back(_PTBalance);
            else
                result->y *= 2;   

            result = find( _vPoint.begin( ), _vPoint.end( ), _PT);
            if(result == _vPoint.end())
                _vPoint.push_back(_PT);
            else
                result->y *= 2;
        }
        else
        {
            result = find( _vPoint.begin( ), _vPoint.end( ), _PT);
            if(result == _vPoint.end())
                _vPoint.push_back(_PT);
            else
                result->y *= 2;

            result = find( _vPoint.begin( ), _vPoint.end( ), _PTBalance);
            if(result == _vPoint.end())
                _vPoint.push_back(_PTBalance);
            else
                result->y *= 2;  

            result = find( _vPoint.begin( ), _vPoint.end( ), _PTTurn);
            if(result == _vPoint.end())
                _vPoint.push_back(_PTTurn);
            else
                result->y *= 2;

            result = find( _vPoint.begin( ), _vPoint.end( ), _PTRight);
            if(result == _vPoint.end())
                _vPoint.push_back(_PTRight);
            else
                result->y *= 2;
        }
    }
}
void CDataInfo::MergeStrategyPos(vector<_POINT>& _vPoint)
{
    _POINT _PT;
    vector<_POINT> _vPos(_vPoint);

    _vPoint.clear();
    for (vector<_POINT>::iterator it = _vPos.begin(); it != _vPos.end();it++)
    {
        _PT = *it;

        //合并
        _PT.y = GetYPosValue(_PT.x);
        _vPoint.push_back(_PT);
    }

    //去重  
    vector<_POINT>::iterator end_unique = unique(_vPoint.begin(),_vPoint.end());  
    //删除重复  
    _vPoint.erase(end_unique,_vPoint.end());  

    //重新确定Y轴逻辑最值
    double dYValueMin = 99999999.0, dYValueMax = -999999999.0;
    for(vector<_POINT>::iterator it = _vPoint.begin(); it!=_vPoint.end(); it++)
    {
        if(dYValueMin > it->y)
            dYValueMin = it->y;
        if(dYValueMax < it->y)
            dYValueMax = it->y;
    }

    m_dYAxisPosValueMin2 = dYValueMin;
    m_dYAxisPosValueMax2 = dYValueMax;
}
double CDataInfo::GetYPosValue(const double& dXPosValue)
{
    sStrategyData dataFirst,dataSecond;
    double dResult = 0.0;

    vector<sStrategyData>::iterator itFirst = m_vStrategyData.begin();
    vector<sStrategyData>::iterator itSecond;
    for(;itFirst!=m_vStrategyData.end(); itFirst++)
    {     
        itSecond = itFirst+1;

        dataFirst = *itFirst;
        if(itSecond!=m_vStrategyData.end())
            dataSecond = *itSecond;
        else
            dataSecond = dataFirst;

        if(dXPosValue <= dataFirst.dStrikePrice)
        {
            if(dataFirst.eStyle == BuyBullish)
                dResult +=  (-dataFirst.dOrderPrice*dataFirst.nOrderCount) * dataFirst.dUnderlyingMultiple;
            else if(dataFirst.eStyle == SellBullish)
                dResult +=  (dataFirst.dOrderPrice*dataFirst.nOrderCount) * dataFirst.dUnderlyingMultiple;
            else if(dataFirst.eStyle == BuyBearish)
                dResult +=  (-dXPosValue + dataFirst.dStrikePrice - dataFirst.dOrderPrice * dataFirst.nOrderCount)*dataFirst.dUnderlyingMultiple;
            else if(dataFirst.eStyle == SellBearish)
                dResult +=  (dXPosValue - dataFirst.dStrikePrice + dataFirst.dOrderPrice * dataFirst.nOrderCount)*dataFirst.dUnderlyingMultiple;
        }
        else if(dXPosValue > dataFirst.dStrikePrice)
        {
            if(dataFirst.eStyle == BuyBullish)
                dResult += (dXPosValue - dataFirst.dStrikePrice - dataFirst.dOrderPrice * dataFirst.nOrderCount)*dataFirst.dUnderlyingMultiple;
            else if(dataFirst.eStyle == SellBullish)
                dResult += -(dXPosValue - dataFirst.dStrikePrice - dataFirst.dOrderPrice * dataFirst.nOrderCount)*dataFirst.dUnderlyingMultiple;
            else if(dataFirst.eStyle == BuyBearish)
                dResult +=  (-dataFirst.dOrderPrice*dataFirst.nOrderCount) * dataFirst.dUnderlyingMultiple;
            else if(dataFirst.eStyle == SellBearish)
                dResult +=  (dataFirst.dOrderPrice*dataFirst.nOrderCount) * dataFirst.dUnderlyingMultiple;
        }            
    }

    return dResult;
}
void CDataInfo::GetYPosValue(CHART_TYPE  eType, double& dYPosValueMin, double& dYPosValueMax)
{
    if(eProbaChart == eType)
    {
        dYPosValueMin = m_dYAxisPosValueMin1;
        dYPosValueMax = m_dYAxisPosValueMax1;
    }
    else if(eProfitChart == eType)
    {
        dYPosValueMin = m_dYAxisPosValueMin2;
        dYPosValueMax = m_dYAxisPosValueMax2;
    }
}
bool CDataInfo::GetBalancePos(vector<_POINT>& _vPoints, vector<_LINESEG>& _vLineSeg)
{
    _POINT _PT, _PTFirst, _PTSecond; ;//交点
    _LINE _Line, _LineBalance(0,1,0);  
    vector<_POINT> vPos(_vPoints);
    vector<sBalancePt> vBalancePos;

    vector<_POINT>::iterator itFirst = vPos.begin();
    vector<_POINT>::iterator it, itSecond;
    for (;itFirst!=vPos.end();itFirst++)
    {
        it = itFirst;
        itSecond = itFirst+1;

        _PTFirst = *itFirst;
        if(itSecond!=vPos.end())
            _PTSecond = *itSecond; 

        _LINESEG _LineSeg(_PTFirst,_PTSecond);
        _Line = makeline(_PTFirst,_PTSecond);

        if(slope(_Line)==0 && fabs(_PTFirst.y)==0)//平行线
        {
            //将平衡点插入
            it = find(_vPoints.begin(),_vPoints.end(),_PTSecond);
            if(it==_vPoints.end())
                _vPoints.push_back(_PTSecond);

            vBalancePos.push_back(_PTSecond);

            _LINESEG LineSeg(_PTFirst, _PTSecond);
            _vLineSeg.push_back(LineSeg); 
        }
        else if(lineintersect(_LineBalance, _Line, _PT))
        {
            if(online(_LineSeg,_PT))
            {
                //将平衡点插入
                it = find(_vPoints.begin(),_vPoints.end(),_PT);
                if(it==_vPoints.end())
                    _vPoints.push_back(_PT);

                vBalancePos.push_back(_PT);

                _LINESEG LineSeg(_PTFirst, _PTSecond);
                _vLineSeg.push_back(LineSeg); 
            }    
        }
    }

    //去重  
    vector<_POINT>::iterator end_unique = unique(vBalancePos.begin(),vBalancePos.end());  
    //删除重复  
    vBalancePos.erase(end_unique,vBalancePos.end());  

    CDataInfo::NewInstance()->SetBalancePoint(vBalancePos);

    return _vLineSeg.empty()?false:true; 
}

bool CDataInfo::GetPos(vector<_POINT>& vPos)
{
    vPos = m_vPoint;
    return m_bBalance;
}
double CDataInfo::GetProbaYPosValue(double dXPosValue)
{
    return m_PLProbability->CalcuProbabilityDensity(dXPosValue);
}
double CDataInfo::GetArea(double dXPosValue)
{
    return m_PLProbability->CalcuDistribution(dXPosValue);
}
void CDataInfo::SetInstrumentInfo(const PlatformStru_InstrumentInfo& info)
{
    m_InstrumentInfo = info;
}
double CDataInfo::GetInstPriceTick()
{
    return m_InstrumentInfo.PriceTick;
}
double CDataInfo::GetInstPriceTick(const string& sInstID)
{
    PlatformStru_InstrumentInfo sInfo;
    vector<PlatformStru_InstrumentInfo>::iterator it = m_vecRelaInstrumentInfoReq.begin();
    for (;it!=m_vecRelaInstrumentInfoReq.end();it++)
    {
        sInfo = *it;
        if(0==strncmp(sInfo.InstrumentID,sInstID.c_str(),sizeof(sInfo.InstrumentID)-1))
            return sInfo.PriceTick;
    }

    return 0.0;
}
void CDataInfo::SetInstrumentDepthData(const PlatformStru_DepthMarketData& depthData)
{
    m_InstrumentDepthData = depthData;
}
double CDataInfo::GetInstUpperPrice()
{
    return m_InstrumentDepthData.UpperLimitPrice;
}
double CDataInfo::GetInstLowerPrice()
{
    return m_InstrumentDepthData.LowerLimitPrice;
}
double CDataInfo::GetInstUpperPrice(const string& sInstID)
{
    CeasymutexGuard guard(m_mutexLastQuot);
    map<string,PlatformStru_DepthMarketData>::iterator it = m_mapLastQuot.find(sInstID);
    if(it!=m_mapLastQuot.end())
        return it->second.UpperLimitPrice;

    return 0.0;
}
double CDataInfo::GetInstLowerPrice(const string& sInstID)
{
    CeasymutexGuard guard(m_mutexLastQuot);
    map<string,PlatformStru_DepthMarketData>::iterator it = m_mapLastQuot.find(sInstID);
    if(it!=m_mapLastQuot.end())
        return it->second.LowerLimitPrice;

    return 0.0;
}
double  CDataInfo::GetLastPrice()
{
    return m_InstrumentDepthData.LastPrice;
}
double CDataInfo::GetInstVolumeMultiple()
{
    return m_InstrumentInfo.VolumeMultiple;
}   
double CDataInfo::GetInstVolumeMultiple(const string& sInstID)
{
    //标的物相关期权的合约乘数都相同，所有取一个就可以
    PlatformStru_InstrumentInfo sInfo;
    vector<PlatformStru_InstrumentInfo>::iterator it = m_vecRelaInstrumentInfoReq.begin();
    for (;it!=m_vecRelaInstrumentInfoReq.end();it++)
    {
        sInfo = *it;
        //if(0==strncmp(sInfo.InstrumentID,sInstID.c_str(),sizeof(sInfo.InstrumentID)-1))
            return sInfo.VolumeMultiple;
    }

    return 0.0;
}
string  CDataInfo::GetExpireDate()
{
    return m_InstrumentInfo.ExpireDate;
}


void CDataInfo::SetLastQuotInfo(const PlatformStru_DepthMarketData& data)
{
    CeasymutexGuard guard(m_mutexLastQuot);

    map<string,PlatformStru_DepthMarketData>::iterator it = m_mapLastQuot.find(string(data.InstrumentID));
    m_mapLastQuot[string(data.InstrumentID)] = data;
}

void CDataInfo::GetLastQuotInfo(map<string,PlatformStru_DepthMarketData>& mapData)
{
    mapData = m_mapLastQuot;
} 
double  CDataInfo::GetLimitPrice(const string& sInstID, bool bDirect)
{
    CeasymutexGuard guard(m_mutexLastQuot);
    map<string,PlatformStru_DepthMarketData>::iterator it = m_mapLastQuot.find(sInstID);
    if(it!=m_mapLastQuot.end())
    {
        if(bDirect)//买
            return it->second.BidPrice1;
        else 
            return it->second.AskPrice1;
    }

    return 0.0;
}
void CDataInfo::GetRelaInstrumentInfo(vector<PlatformStru_InstrumentInfo>& vData)
{
    vData = m_vecRelaInstrumentInfoReq;
}
void CDataInfo::GetExchangeID2ProductID(map<string,set<string>>& mapData)
{
    mapData = m_mapExchangeID2ProductID;
}
void CDataInfo::GetProductID2InstID(map<string,set<string>>& mapData)
{
    mapData = m_mapProductID2InstID;
}
string CDataInfo::GetProductByInstID(const string& szCurrInstrumentID)
{
    set<string> setInstIDs;
    map<string,set<string>>::iterator it = m_mapProductID2InstID.begin();
    for (;it!=m_mapProductID2InstID.end();it++)
    {
        setInstIDs = it->second;
        set<string>::iterator it_set = setInstIDs.find(szCurrInstrumentID);
        if(it_set!=setInstIDs.end())
            return it->first;
    }

    return " ";
}
string CDataInfo::GetExchangeByProductID(const string& sProductName)
{
    set<string> setProductIDs;
    map<string,set<string>>::iterator it = m_mapExchangeID2ProductID.begin();
    for (;it!=m_mapExchangeID2ProductID.end();it++)
    {
        setProductIDs = it->second;
        set<string>::iterator it_set = setProductIDs.find(sProductName);
        if(it_set!=setProductIDs.end())
            return it->first;
    }

    return " ";
}
CString CDataInfo::GetNoteText()
{
    return m_strNote;
}
void CDataInfo::SetNoteText(CString strNote)
{
    m_strNote = strNote;
}