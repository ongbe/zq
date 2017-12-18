#include "StdAfx.h"
#include "FData.h"
#include "QuoteCommon_IO.h"
CFData::CFData(void)
{
	m_ExchangeName  = "";			//交易所名称
	m_strInstrument = "";			//合约名称

	m_bTLineShow	= false;
	m_enumPhrase    = PHRASE_DAY;	
	m_bDayShow		= false;
	m_bWeekShow     = false;
	m_bMonthShow    = false;
	m_bMin1Show		= false;
	m_bMin5Show		= false;
	m_bMin15Show	= false;
	m_bMin30Show	= false;
	m_bMin60Show	= false;
	m_bRecordIntime = true;
	InitializeCriticalSection (&m_csDepthMarketData);
	InitializeCriticalSection (&m_csTLine);

	InitializeCriticalSection (&m_csKLineDay);
	InitializeCriticalSection (&m_csKLineWeek);
	InitializeCriticalSection (&m_csKLineMonth);
	InitializeCriticalSection (&m_csKLineMin1);
	InitializeCriticalSection (&m_csKLineMin5);
	InitializeCriticalSection (&m_csKLineMin15);
	InitializeCriticalSection (&m_csKLineMin30);
	InitializeCriticalSection (&m_csKLineMin60);

	InitializeCriticalSection (&m_csTimeScales);
	
	
}

CFData::~CFData(void)
{
	ClearMemory_TechIndex();
	m_vecDepthMarketData.clear();
	m_vecTimeScales.clear();
	m_vecTLine.clear();
	m_vecKLineDay.clear();
	m_vecKLineWeek.clear();
	m_vecKLineMonth.clear();
	m_vecKLineMin1.clear();
	m_vecKLineMin5.clear();
	m_vecKLineMin15.clear();
	m_vecKLineMin30.clear();
	m_vecKLineMin60.clear();

	m_vecTMWidthSpan.clear();

}

void CFData::SetScales(vector<struct tm>& vecScales)
{
	if(vecScales.size() < 2)
		return;

	EnterCriticalSection(&m_csTimeScales);
	m_vecTimeScales.clear();
	m_vecTimeScales = vecScales;
	m_nOneDayScalesCount = m_vecTimeScales.size();

	m_vecTMWidthSpan.clear();
	m_TMWidthCount = 0;
	for(int i = 0; i< (int)m_vecTimeScales.size(); i++)
	{
		struct tm tm1 = m_vecTimeScales[i];
		struct tm tm2 = m_vecTimeScales[i+1];
		DWORD dw = abs(difftime(mktime(&tm1), mktime(&tm2)));
		m_vecTMWidthSpan.push_back(dw);
		m_TMWidthCount = m_TMWidthCount + dw;
		i++;
	}
	LeaveCriticalSection(&m_csTimeScales);
}
void CFData::SetLastClosePrice(double dbPrice)
{
	m_PreSettlementPrice = dbPrice;
}
void CFData::SetDataInTime(PlatformStru_DepthMarketData* pData)
{
	if(pData == NULL)
		return;	
	//用这些局部变量，主要考虑关键区之外的改变
	bool bTLineShow;
	bool bDayShow;
	bool bWeekShow;
	bool bMonthShow;
	bool bMin1Show;
	bool bMin5Show;
	bool bMin15Show;
	bool bMin30Show;
	bool bMin60Show;

	EnterCriticalSection(&m_csDepthMarketData);
	if(m_bRecordIntime)
		m_vecDepthMarketData.push_back(*pData);
	bTLineShow  = m_bTLineShow;
	bDayShow	= m_bDayShow;
	bWeekShow	= m_bWeekShow;
	bMonthShow	= m_bMonthShow;
	bMin1Show	= m_bMin1Show;
	bMin5Show	= m_bMin5Show;
	bMin15Show	= m_bMin15Show;
	bMin30Show	= m_bMin30Show;
	bMin60Show	= m_bMin60Show;
	LeaveCriticalSection(&m_csDepthMarketData);

	CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
	CString strTime		= pData->UpdateTime;
	CString strFutureID = pData->InstrumentID;
	if(isInvalidValue(pData->LastPrice)) 		
	{		
		return;//无效值
	}	

	if(isInvalidValue(pData->AskPrice1))
		pData->AskPrice1 = 0;
	if(isInvalidValue(pData->BidPrice1))
		pData->BidPrice1 = 0;
	if(isInvalidValue(pData->Volume))
		pData->Volume = 0;
	if(isInvalidValue(pData->Turnover))
		pData->Turnover = 0;
	if(isInvalidValue(pData->OpenInterest))
		pData->OpenInterest = 0;
	if(strDate.GetLength() != 8)
	{		
		return;
	}
	if(!isInvalidValue(pData->HighestPrice))
	{//如果最高价无效，则以这单为第一笔数据，兼容组合单
		m_fMaxValue = pData->HighestPrice;	
		m_fMinValue = pData->LowestPrice;
	}	
	else
	{
		double dbMaxSpan  = fabs(pData->LastPrice - pData->PreSettlementPrice);
		double dbMaxValue = pData->PreSettlementPrice + dbMaxSpan;
		double dbMinValue = pData->PreSettlementPrice - dbMaxSpan;
		if(dbMaxValue > m_fMaxValue)
			m_fMaxValue = dbMaxValue;
		if(dbMinValue < m_fMinValue)
			m_fMinValue = dbMinValue;

	}
	double dcClose = pData->PreSettlementPrice;
	if(dcClose <0.00001)
		dcClose = pData->OpenPrice;//如果没有昨结算，则用今日开盘价
	SetLastClosePrice(dcClose);
	if(bTLineShow)
		SetInTimeData_TLine(pData);	
	SetInTimeData_KLine(pData,	 bDayShow,
								 bWeekShow,
								 bMonthShow,
								 bMin1Show,
								 bMin5Show,
								 bMin15Show,
								 bMin30Show,
								 bMin60Show);		
}
BOOL CFData::SetInTimeData_KLine(PlatformStru_DepthMarketData* pData, 
														 bool bDayShow,
														 bool bWeekShow,
														 bool bMonthShow,
														 bool bMin1Show,
														 bool bMin5Show,
														 bool bMin15Show,
														 bool bMin30Show,
														 bool bMin60Show)
{	
	EnterCriticalSection(&m_csTimeScales);
	if(m_vecTimeScales.size() == 0)
	{	
		LeaveCriticalSection(&m_csTimeScales);
		return false;
	}
	LeaveCriticalSection(&m_csTimeScales);
	

	CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
	CString strTime		= pData->UpdateTime;
	CString strFutureID = pData->InstrumentID;

	SKLine pKLine;
	pKLine.dwTime.tm_year	= atoi(strDate.Left(4))-1900;
	pKLine.dwTime.tm_mon	= atoi(strDate.Mid(4,2))-1;
	pKLine.dwTime.tm_mday	= atoi(strDate.Right(2));
	pKLine.dwTime.tm_hour	= atoi(strTime.Left(2));
	pKLine.dwTime.tm_min	= atoi(strTime.Mid(3, 2));
	pKLine.dwTime.tm_sec	= atoi(strTime.Mid(6, 2));;
	pKLine.dwTime.tm_wday	= 0;
	pKLine.dwTime.tm_yday	= 0;
	pKLine.dwHoldVolume    = pData->OpenInterest;
	pKLine.dwVolume        = pData->Volume;
	pKLine.fClosePrice     = pData->LastPrice;
	pKLine.fHighPrice      = pData->HighestPrice;
	pKLine.fLowPrice		= pData->LowestPrice;
	pKLine.fOpenPrice      = pData->OpenPrice;
	pKLine.fSumTradeVolume	= pData->Turnover;	
	SetInTimeData_DiffSpan(pKLine, PHRASE_1MIN,  bMin1Show);
	SetInTimeData_DiffSpan(pKLine, PHRASE_5MIN,  bMin5Show);
	SetInTimeData_DiffSpan(pKLine, PHRASE_15MIN, bMin15Show);
	SetInTimeData_DiffSpan(pKLine, PHRASE_30MIN, bMin30Show);
	SetInTimeData_DiffSpan(pKLine, PHRASE_60MIN, bMin60Show);
	SetInTimeData_DiffSpan(pKLine, PHRASE_DAY,	 bDayShow);
	SetInTimeData_DiffSpan(pKLine, PHRASE_WEEK,  bWeekShow);
	SetInTimeData_DiffSpan(pKLine, PHRASE_MONTH, bMonthShow);

	m_sKLineLast = pKLine;
	return true;
}
BOOL CFData::SetInTimeData_DiffSpan(SKLine& pKLine, EnumPhrase enumPhrase, bool bShow)
{		
	SKLine pTemp = pKLine;
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			if(bShow)
			{				
				SetInTimeData_MINS(m_vecKLineMin1, pTemp, 60);//5	
			}			
		}
		break;
	case PHRASE_5MIN:
		{
			if(bShow)
			{				
				SetInTimeData_MINS(m_vecKLineMin5, pTemp, 300);//5	
			}	
		}
		break;
	case PHRASE_15MIN:
		{
			if(bShow)
			{				
				SetInTimeData_MINS(m_vecKLineMin15, pTemp, 900);//5	
			}	
		}
		break;
	case PHRASE_30MIN:
		{
			if(bShow)
			{				
				SetInTimeData_MINS(m_vecKLineMin30, pTemp, 1800);//5	
			}	
		}
		break;
	case PHRASE_60MIN:
		{
			if(bShow)
			{				
				SetInTimeData_MINS(m_vecKLineMin60, pTemp, 3600);//5	
			}	
		}
		break;
	case PHRASE_DAY:
		{
			if(bShow)
			{				
				//////////////////////////////////////////////////////////////////////////1149
				//日，周，月线不需要时,分,秒;统一处理				
				pTemp.dwTime.tm_hour = 0;
				pTemp.dwTime.tm_min  = 0;
				pTemp.dwTime.tm_sec  = 0;
				SetInTimeData_Day(m_vecKLineDay, pTemp);	
			}	
		}
		break;
	case PHRASE_WEEK:
		{
			if(bShow)
			{				
				//////////////////////////////////////////////////////////////////////////1149
				//日，周，月线不需要时,分,秒;统一处理				
				pTemp.dwTime.tm_hour = 0;
				pTemp.dwTime.tm_min  = 0;
				pTemp.dwTime.tm_sec  = 0;
				SetInTimeData_Week(m_vecKLineWeek, pTemp);	
			}	
		}
		break;
	case PHRASE_MONTH:
		{
			if(bShow)
			{				
				//////////////////////////////////////////////////////////////////////////1149
				//日，周，月线不需要时,分,秒;统一处理				
				pTemp.dwTime.tm_hour = 0;
				pTemp.dwTime.tm_min  = 0;
				pTemp.dwTime.tm_sec  = 0;
				SetInTimeData_Month(m_vecKLineMonth, pTemp);	
			}	
		}
		break;
	}

	return true;


}
BOOL CFData::SetInTimeData_Day(vector<SKLine>& vecKLinePara, SKLine& pKLine)
{
	if(isInvalidValue(pKLine.fHighPrice)
		||isInvalidValue(pKLine.fOpenPrice)
		||isInvalidValue(pKLine.fLowPrice))
		return true;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);		
	}
	else
	{
		SKLine& pLast = vecKLinePara[0];		
		if(pLast.dwTime.tm_year == pKLine.dwTime.tm_year
			&& pLast.dwTime.tm_mon == pKLine.dwTime.tm_mon 
			&& pLast.dwTime.tm_mday == pKLine.dwTime.tm_mday)
		{			
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwHoldVolume = pKLine.dwHoldVolume;
			pLast.dJieSuan     = pKLine.dJieSuan;
			pLast.dwVolume     = pKLine.dwVolume;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;
			pLast.fOpenPrice	= pLast.fOpenPrice;
			pLast.fSumTradeVolume	= pLast.fSumTradeVolume;
			bSameMin = true;
		}
		if(!bSameMin)
		{			
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);				
		}
	}
	return bSameMin;
}
bool CFData::IsOneWeek(struct tm tm1, struct tm tm2)
{
	COleDateTime date1(tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
	COleDateTime date2(tm2.tm_year+1900, tm2.tm_mon+1, tm2.tm_mday, tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
	if(date1.GetYear() != date2.GetYear())
		return false;
	int nDay1 = date1.GetDayOfYear();
	int nDay2 = date2.GetDayOfYear();

	if(nDay1/7 +1 != nDay2/7+1)
		return false;

	return true;
}

bool CFData::IsOneMonth(struct tm tm1, struct tm tm2)
{
	if((tm1.tm_year == tm2.tm_year) && (tm1.tm_mon == tm2.tm_mon))
		return true;
	return false;
}
BOOL CFData::SetInTimeData_Week(vector<SKLine>& vecKLinePara, SKLine& pKLine)
{	
	if(isInvalidValue(pKLine.fHighPrice)
		||isInvalidValue(pKLine.fOpenPrice)
		||isInvalidValue(pKLine.fLowPrice))
		return true;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		vecKLinePara.push_back(pKLine);		
	}
	else
	{
		SKLine& pLast = vecKLinePara[0];	
		if(IsOneWeek(pLast.dwTime, pKLine.dwTime))
		{			
			pLast.dwTime = pKLine.dwTime;
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwHoldVolume = pKLine.dwHoldVolume;
			pLast.dJieSuan     = pKLine.dJieSuan;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;

			if(m_sKLineLast.dwTime.tm_year != 0)
			{
				pLast.dwVolume     += pKLine.dwVolume - m_sKLineLast.dwVolume;				
				pLast.fSumTradeVolume	+= pKLine.fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
			}
			bSameMin = true;			
		}
		if(!bSameMin)
		{
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);				
		}
	}
	return bSameMin;
}
BOOL CFData::SetInTimeData_Month(vector<SKLine>& vecKLinePara, SKLine& pKLine)
{
	if(isInvalidValue(pKLine.fHighPrice)
		||isInvalidValue(pKLine.fOpenPrice)
		||isInvalidValue(pKLine.fLowPrice))
		return true;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{		
		vecKLinePara.push_back(pKLine);		
	}
	else
	{
		SKLine& pLast = vecKLinePara[0];	
		if(IsOneMonth(pLast.dwTime, pKLine.dwTime))
		{
			pLast.dwTime = pKLine.dwTime;
			pLast.fClosePrice  = pKLine.fClosePrice;
			pLast.dwHoldVolume = pKLine.dwHoldVolume;
			pLast.dJieSuan     = pKLine.dJieSuan;
			if(pLast.fHighPrice < pKLine.fClosePrice  )
				pLast.fHighPrice	= pKLine.fClosePrice;
			if(pLast.fLowPrice > pKLine.fClosePrice)
				pLast.fLowPrice	= pKLine.fClosePrice;
			if(m_sKLineLast.dwTime.tm_year != 0)
			{
				pLast.dwVolume     += pKLine.dwVolume - m_sKLineLast.dwVolume;				
				pLast.fSumTradeVolume	+= pKLine.fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
			}
			bSameMin = true;
		}
		if(!bSameMin)
		{
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
		}
	}

	return bSameMin;
}
void CFData::TestVector(vector<SKLine> vecKLinePara)
{
#ifdef _DEBUG
	if(vecKLinePara.size() == 0)
		return ;
	SKLine& pLast = vecKLinePara[0];
	if(pLast.dwVolume < 0)
	{
		AfxMessageBox("有异常数据产生！",MB_OK);		
	}
#endif
}
BOOL CFData::SetInTimeData_MINS(vector<SKLine>& vecKLinePara, SKLine& pKLine, int nTimeSpan)
{
	vector<struct tm> vecTimeScales;
	EnterCriticalSection(&m_csTimeScales);
	vecTimeScales = m_vecTimeScales;
	LeaveCriticalSection(&m_csTimeScales);

	TestVector(vecKLinePara);
	if(vecTimeScales.size() == 0)
		return false;
	SKLine ptemp = pKLine;

	bool bSameMin = false;
	if(vecKLinePara.size() == 0)
	{
		
		struct tm tm1 = vecTimeScales[0];
		struct tm tm2 = pKLine.dwTime;
		if(difftime(mktime(&tm1), mktime(&tm2)) != 0)
		{
			pKLine.dwVolume		= 0;
			pKLine.fSumTradeVolume   = 0;
			pKLine.fOpenPrice      = pKLine.fClosePrice;
			pKLine.fHighPrice		= pKLine.fClosePrice;
			pKLine.fLowPrice       = pKLine.fClosePrice;
		}

		struct tm TM = GetTimeInSpan(pKLine.dwTime, nTimeSpan);//60分钟，3600秒
		pKLine.dwTime = TM;
		pKLine.dwTime.tm_sec   = 0;

		vecKLinePara.push_back(pKLine);
		TestVector(vecKLinePara);
	}
	else
	{

		SKLine& pLast = vecKLinePara[0];
		struct tm TM = GetTimeInSpan(pKLine.dwTime, nTimeSpan);//15:15:09 就可以转换为15:15
		if(pLast.dwTime.tm_year == TM.tm_year
			&& pLast.dwTime.tm_mon == TM.tm_mon 
			&& pLast.dwTime.tm_mday == TM.tm_mday)
		{//
			struct tm tm1 = pLast.dwTime;
			struct tm tm2 = TM;
			int dwTimeSpan = difftime(mktime(&tm1), mktime(&tm2));
			if(dwTimeSpan==0)
			{//同一分钟	
				bSameMin = true;
				pLast.fClosePrice  = pKLine.fClosePrice;
				pLast.dwHoldVolume = pKLine.dwHoldVolume;
				pLast.dJieSuan     = pKLine.dJieSuan;
				if(pLast.fHighPrice < pKLine.fClosePrice  )
					pLast.fHighPrice	= pKLine.fClosePrice;
				if(pLast.fLowPrice > pKLine.fClosePrice)
					pLast.fLowPrice	= pKLine.fClosePrice;

				if(m_sKLineLast.dwTime.tm_year != 0)
				{
					pLast.dwVolume     += pKLine.dwVolume - m_sKLineLast.dwVolume;				
					pLast.fSumTradeVolume	+= pKLine.fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
				}
				TestVector(vecKLinePara);
				return bSameMin;

			}
			else if(dwTimeSpan >0)
			{		
				SKLine& pLastFirst = vecKLinePara[vecKLinePara.size() - 1];
				if(difftime(mktime(&(pLastFirst.dwTime)), mktime(&TM)) > 0)
				{//比第一个数据还要早，主要是正对模拟行情处理
					if(m_sTLineLast.dwTime.tm_year != 0)
					{
						pKLine.dwVolume  = pKLine.dwVolume - m_sKLineLast.dwVolume;
						pKLine.fSumTradeVolume = pKLine.fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
					}
					else
					{
						pKLine.dwVolume		= 0;
						pKLine.fSumTradeVolume   = 0;
					}
					vecKLinePara.push_back(pKLine);					
				}
				else 
				{//实时行情加到目前数组的中间去
					vector<SKLine>::iterator itLast = vecKLinePara.end();
					for(vector<SKLine>::iterator it = vecKLinePara.begin(); it!= vecKLinePara.end(); it++)
					{
						SKLine& pLastTemp = *it;
						if(pLastTemp.dwTime.tm_year == TM.tm_year
							&& pLastTemp.dwTime.tm_mon == TM.tm_mon 
							&& pLastTemp.dwTime.tm_mday == TM.tm_mday
							&& pLastTemp.dwTime.tm_hour == TM.tm_hour
							&& pLastTemp.dwTime.tm_min == TM.tm_min)
						{//如果是按时间先加进去的则有可能出现这种情况
							bSameMin = true;

							pLastTemp.fClosePrice = pKLine.fClosePrice;
							if(pLastTemp.fHighPrice < pKLine.fClosePrice  )
								pLastTemp.fHighPrice	= pKLine.fClosePrice;
							if(pLastTemp.fLowPrice > pKLine.fClosePrice)
								pLastTemp.fLowPrice	= pKLine.fClosePrice;
							if(m_sKLineLast.dwTime.tm_year != 0)
							{
								pLastTemp.dwVolume  += pKLine.dwVolume - m_sKLineLast.dwVolume;
								pLastTemp.fSumTradeVolume += pKLine.fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
							}						
							break;
						}
						if(difftime(mktime(&pLastTemp.dwTime), mktime(&TM)) < 0)
						{		
							pKLine.dwTime = TM;
							pKLine.dwTime.tm_sec   = 0;
							if(m_sKLineLast.dwTime.tm_year != 0)
							{
								pKLine.dwVolume  = pKLine.dwVolume - m_sKLineLast.dwVolume;
								pKLine.fSumTradeVolume = pKLine.fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
							}
							//////////////////////////////////////////////////////////////////////////
							//对于插入数组的类型，最低最高价都应该新的计，以开盘价为准
							pKLine.fOpenPrice      = pKLine.fClosePrice;
							pKLine.fHighPrice		= pKLine.fClosePrice;
							pKLine.fLowPrice       = pKLine.fClosePrice;
							//////////////////////////////////////////////////////////////////////////
							vecKLinePara.insert(it, pKLine);
							break;
						}
						itLast = it;
					}	
				}
				TestVector(vecKLinePara);
				return bSameMin;
			}
		}
		if(!bSameMin)
		{
			if(m_sKLineLast.dwTime.tm_year != 0)
			{//
				pKLine.dwVolume     = pKLine.dwVolume - m_sKLineLast.dwVolume;				

				pKLine.fSumTradeVolume	= pKLine.fSumTradeVolume - m_sKLineLast.fSumTradeVolume;
				/* 廖说开盘价不用上一时间周期的收盘价修改之前;以后如果换成上一分钟的收盘价则应该把注解的打开，把后面的注掉就可以了
				pKLine.fOpenPrice      = m_sKLineLast.fClosePrice;
				pKLine.fHighPrice		= m_sKLineLast.fClosePrice;
				pKLine.fLowPrice       = m_sKLineLast.fClosePrice;
				*/
				pKLine.fOpenPrice      = pKLine.fClosePrice;
				pKLine.fHighPrice		= pKLine.fClosePrice;
				pKLine.fLowPrice       = pKLine.fClosePrice;
			}
			else
			{//刚开盘的第一笔
				struct tm tm1 = vecTimeScales[0];
				struct tm tm2 = pKLine.dwTime;
				if(difftime(mktime(&tm1), mktime(&tm2)) != 0)
				{
					pKLine.dwVolume		= 0;
					pKLine.fSumTradeVolume   = 0;
					pKLine.fOpenPrice      = pKLine.fClosePrice;
					pKLine.fHighPrice		= pKLine.fClosePrice;
					pKLine.fLowPrice       = pKLine.fClosePrice;					
				}
			}				

			pKLine.dwTime = TM;
			pKLine.dwTime.tm_sec   = 0;
			vecKLinePara.insert(vecKLinePara.begin(), pKLine);	
		}		
	}
	TestVector(vecKLinePara);
	return bSameMin;
}
BOOL CFData::SetInTimeData_TLine(PlatformStru_DepthMarketData* pData)
{
	EnterCriticalSection(&m_csTimeScales);
	if(m_vecTimeScales.size() == 0)
	{
		LeaveCriticalSection(&m_csTimeScales);
		return false;
	}
	LeaveCriticalSection(&m_csTimeScales);
	
	
	CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
	CString strTime		= pData->UpdateTime;
	CString strFutureID = pData->InstrumentID;

	STLine pLine;
	pLine.dwTime.tm_year	= atoi(strDate.Left(4))-1900;
	pLine.dwTime.tm_mon	= atoi(strDate.Mid(4,2))-1;
	pLine.dwTime.tm_mday	= atoi(strDate.Right(2));

	pLine.dwTime.tm_hour	= atoi(strTime.Left(2));
	pLine.dwTime.tm_min	= atoi(strTime.Mid(3, 2));
	pLine.dwTime.tm_sec	= atoi(strTime.Mid(6, 2));;
	pLine.dwTime.tm_wday	= 0;
	pLine.dwTime.tm_yday	= 0;
	pLine.fLastPrice = pData->LastPrice;
	pLine.dwVolume	  = pData->Volume;	
	pLine.dwVolumeMoney	  = pData->Turnover;
	pLine.dwHoldVolume	      = pData->OpenInterest;

	double dcClose = pData->PreSettlementPrice;
	if(dcClose <0.00001)
		dcClose = pData->OpenPrice;//如果没有昨结算，则用今日开盘价
	pLine.dwTime.tm_wday	= 0;
	pLine.dwTime.tm_yday	= 0;

	BOOL bNotDelMemory = SetInTimeData_DiffSpan_TLine(pLine);
	if(!bNotDelMemory)
	{	
		
	}
	return TRUE;
}
BOOL CFData::SetInTimeData_DiffSpan_TLine(STLine& pTLine)
{
	vector<struct tm> vecTimeScales;
	EnterCriticalSection(&m_csTimeScales);
	vecTimeScales = m_vecTimeScales;
	LeaveCriticalSection(&m_csTimeScales);

	bool bSameMin = false;
	STLine sTemp = pTLine;
	struct tm TM = GetTimeInSpan_TLine(pTLine.dwTime, 60);//60分钟，3600秒
	pTLine.dwTime  = TM;//这样处理就需要先过滤重复行情
	pTLine.dwTime.tm_sec   = 0;

	if(m_vecTLine.size() == 0)
	{
		bSameMin = true;		
		struct tm tm1 = vecTimeScales[0];
		struct tm tm2 = pTLine.dwTime;
		if(difftime(mktime(&tm1), mktime(&tm2)) !=0)
		{
			pTLine.dwVolume		= 0;
			pTLine.dwVolumeMoney   = 0;
		}
		pTLine.dwTime  = TM;	
		m_vecTLine.push_back(pTLine);	
	}
	else
	{
		STLine& pLast = m_vecTLine[0];

		if(pLast.dwTime.tm_year == TM.tm_year
			&& pLast.dwTime.tm_mon == TM.tm_mon 
			&& pLast.dwTime.tm_mday == TM.tm_mday
			&& pLast.dwTime.tm_hour == TM.tm_hour)
		{			
			/*	if(pLast.dwTime.tm_min == pTLine.dwTime.tm_min && pLast.dwTime.tm_sec == pTLine.dwTime.tm_sec)
			{
			bSameMin = false;//重复行情
			m_sTLineLast = sTemp;
			return bSameMin;
			}*/
			if(pLast.dwTime.tm_min == TM.tm_min )
			{
				bSameMin = false;
				pLast.dwTime     = pTLine.dwTime;
				pLast.fLastPrice = pTLine.fLastPrice;
				if(m_sTLineLast.dwTime.tm_year != 0)
				{
					pLast.dwVolume  += pTLine.dwVolume - m_sTLineLast.dwVolume;
					pLast.dwVolumeMoney += pTLine.dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
				}			
			}
		}	
		int nTimeSpanTemp = difftime(mktime(&pLast.dwTime), mktime(&pTLine.dwTime));//刚加的，要测试一下
		if(nTimeSpanTemp >0)
		{
			STLine& pLastFirst = m_vecTLine[m_vecTLine.size() - 1];
			if(difftime(mktime(&(pLastFirst.dwTime)), mktime(&TM)) > 0)
			{//比第一个数据还要早，主要是正对模拟行情处理
				bSameMin = true;
				if(m_sTLineLast.dwTime.tm_year != 0)
				{
					pTLine.dwVolume  = pTLine.dwVolume - m_sTLineLast.dwVolume;
					pTLine.dwVolumeMoney = pTLine.dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
				}
				else
				{
					pTLine.dwVolume		= 0;
					pTLine.dwVolumeMoney   = 0;
				}
				m_vecTLine.push_back(pTLine);					
			}
			else
			{			
				vector<STLine>::iterator itLast = m_vecTLine.end();
				for(vector<STLine>::iterator it = m_vecTLine.begin(); it!= m_vecTLine.end(); it++)
				{
					STLine& pLastTemp = *it;
					if(pLastTemp.dwTime.tm_year == TM.tm_year
						&& pLastTemp.dwTime.tm_mon == TM.tm_mon 
						&& pLastTemp.dwTime.tm_mday == TM.tm_mday
						&& pLastTemp.dwTime.tm_hour == TM.tm_hour
						&& pLastTemp.dwTime.tm_min == TM.tm_min)
					{//如果是按时间先加进去的则有可能出现这种情况
						bSameMin = false;

						pLastTemp.fLastPrice = pTLine.fLastPrice;
						if(m_sTLineLast.dwTime.tm_year != 0)
						{
							pLastTemp.dwVolume  += pTLine.dwVolume - m_sTLineLast.dwVolume;
							pLastTemp.dwVolumeMoney += pTLine.dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
						}							
						break;
					}
					else if(difftime(mktime(&pLastTemp.dwTime), mktime(&TM)) < 0)
					{//处于中间的某个数据，兼容模拟行情	
						bSameMin = true;
						//	if(itLast == m_vecTLine.end())
						//		m_vecTLine.insert(m_vecTLine.begin(), pTLine);	
						//	else
						if(m_sTLineLast.dwTime.tm_year != 0)
						{
							pTLine.dwVolume  = pTLine.dwVolume - m_sTLineLast.dwVolume;
							pTLine.dwVolumeMoney = pTLine.dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
						}
						else
						{
							pTLine.dwVolume		= 0;
							pTLine.dwVolumeMoney   = 0;
						}
						m_vecTLine.insert(it, pTLine);						
						break;
					}				
					itLast = it;
				}	
			}			
		}
		else if(!bSameMin && nTimeSpanTemp < 0)
		{
			if(m_sTLineLast.dwTime.tm_year != 0)
			{
				pTLine.dwVolume  = pTLine.dwVolume - m_sTLineLast.dwVolume;
				pTLine.dwVolumeMoney = pTLine.dwVolumeMoney - m_sTLineLast.dwVolumeMoney;
			}
			else
			{
				pTLine.dwVolume		= 0;
				pTLine.dwVolumeMoney   = 0;
			}
			m_vecTLine.insert(m_vecTLine.begin(), pTLine);				
			bSameMin = true;			
		}
	}

	m_sTLineLast = sTemp;
	return bSameMin;
}
struct tm CFData::GetTimeInSpan_TLine(struct tm tmPara, int nSpan)
{
	vector<struct tm> vecTimeScales;
	EnterCriticalSection(&m_csTimeScales);
	vecTimeScales = m_vecTimeScales;
	LeaveCriticalSection(&m_csTimeScales);

	if(vecTimeScales.size() == 0)
	{
		struct tm tm1;
		tm1.tm_year = 0;
		return tm1;
	}
	time_t tmMax =   mktime(&vecTimeScales[vecTimeScales.size()-1]);
	time_t tReturn;
	//用这个值判断成功否
	time_t tm11 =   mktime(&tmPara);
	int nIndex = 0;	
	for(int i= 0; i< (int)vecTimeScales.size(); i++)
	{
		if(difftime(mktime(&vecTimeScales[i]), mktime(&tmPara)) >0) 
		{
			nIndex = i;
			break;
		}
		if(i == vecTimeScales.size() -1)
			return vecTimeScales[i];//15:00:07 算成 15:00:00
	}
	if(nIndex == 0)
	{		

		nIndex = nIndex +1;
		//	return vecTimeScales[0];	
	}
	else if( nIndex%2==0)
	{
		time_t t1 = mktime( &vecTimeScales[nIndex-1]);
		t1 -= 1;//
		struct tm tmReturn = *localtime(&t1);
		return GetTimeInSpan(tmReturn, nSpan);
		//return vecTimeScales[nIndex-1];//11:30:10 算成11:30分
	}
	DWORD	dwCount = 0;
	for(int i = 0; i<nIndex-1; i++)
	{		
		time_t t1 = mktime( &vecTimeScales[i] );
		time_t t2 = mktime( &vecTimeScales[i+1]);
		DWORD dw = abs(difftime(t1, t2));
		dwCount += dw;
		i++;
	}
	if(nIndex != 0)
	{
		DWORD dwn = dwCount%nSpan;
		time_t t1 = mktime( &vecTimeScales[nIndex -1]);
		time_t t3 = t1+dwn;
		while(t3 <= tm11)
		{
			t3 += nSpan;
		}
		if(nIndex + 2 < (int)vecTimeScales.size())
		{
			time_t tNext = mktime( &vecTimeScales[nIndex]);
			if(t3 > tNext)
			{
				t3 = t3- tNext + mktime(&vecTimeScales[nIndex+1]);//11:29:00在30分钟是应该是13:15（9:15开盘）
			}			
		}
		tReturn = t3;	
	}
	if(tReturn > tmMax)
		tReturn = tmMax;

	struct tm tmReturn;
	tmReturn.tm_year = 0;
	tmReturn = *localtime(&tReturn);

	return tmReturn;
}
struct tm CFData::GetTimeInSpan(struct tm tmPara, int nSpan)
{
	vector<struct tm> vecTimeScales;
	EnterCriticalSection(&m_csTimeScales);
	vecTimeScales = m_vecTimeScales;
	LeaveCriticalSection(&m_csTimeScales);

	time_t tmMax =   mktime(&vecTimeScales[vecTimeScales.size()-1]);
	time_t tReturn;

	//用这个值判断成功否e
	time_t tm11 =   mktime(&tmPara);
	int nIndex = 0;	
	for(int i= 0; i< (int)vecTimeScales.size(); i++)
	{
		if(difftime(mktime(&vecTimeScales[i]), mktime(&tmPara)) >0) 
		{
			nIndex = i;
			break;
		}

		if(i == vecTimeScales.size() -1)
			return vecTimeScales[i];//15:00:07 算成 15:00:00
	}

	if(nIndex == 0)
	{		
		time_t t1 = mktime( &vecTimeScales[0]);
		t1 += nSpan;
		struct tm tmReturn = *localtime(&t1);
		return tmReturn; 
		//	return vecTimeScales[0];	
	}
	else if( nIndex%2==0)
	{
		time_t t1 = mktime( &vecTimeScales[nIndex-1]);
		t1 -= 1;//
		struct tm tmReturn = *localtime(&t1);
		return GetTimeInSpan(tmReturn, nSpan);
		//	return vecTimeScales[nIndex-1];//11:30:10 算成11:30分
	}
	DWORD	dwCount = 0;
	for(int i = 0; i<nIndex-1; i++)
	{		
		time_t t1 = mktime( &vecTimeScales[i] );
		time_t t2 = mktime( &vecTimeScales[i+1]);
		DWORD dw = abs(difftime(t1, t2));
		dwCount += dw;
		i++;
	}
	if(nIndex != 0)
	{
		DWORD dwn = dwCount%nSpan;
		time_t t1 = mktime( &vecTimeScales[nIndex -1]);
		time_t t3 = t1-dwn;
		while(t3 <= tm11)
		{
			t3 += nSpan;
		}
		if(nIndex + 2 < (int)vecTimeScales.size())
		{
			time_t tNext = mktime( &vecTimeScales[nIndex]);
			if(t3 > tNext)
			{
				t3 = t3- tNext + mktime(&vecTimeScales[nIndex+1]);//11:29:00在30分钟是应该是13:15（9:15开盘）
			}			
		}
		tReturn = t3;	
	}
	if(tReturn > tmMax)
		tReturn = tmMax;

	struct tm tmReturn;
	tmReturn.tm_year = 0;
	tmReturn = *localtime(&tReturn);

	return tmReturn;
}
struct tm CFData::GetTimeInSpan_History(struct tm tmPara, int nSpan)
{
	vector<struct tm> vecTimeScales;
	EnterCriticalSection(&m_csTimeScales);
	vecTimeScales = m_vecTimeScales;
	LeaveCriticalSection(&m_csTimeScales);
	for(int p =0; p<(int)vecTimeScales.size(); p++)
	{
		struct tm& tm2= vecTimeScales[p];
		tm2.tm_year = tmPara.tm_year;
		tm2.tm_mon  = tmPara.tm_mon;
		tm2.tm_mday = tmPara.tm_mday;
	}

	time_t tmMax =   mktime(&vecTimeScales[vecTimeScales.size()-1]);
	time_t tReturn;

	//用这个值判断成功否
	time_t tm11 =   mktime(&tmPara);
	int nIndex = 0;	
	for(int i= 0; i< (int)vecTimeScales.size(); i++)
	{
		if(difftime(mktime(&vecTimeScales[i]), mktime(&tmPara)) >0) 
		{
			nIndex = i;
			break;
		}

		if(i == vecTimeScales.size() -1)
			return vecTimeScales[i];//15:00:07 算成 15:00:00
	}

	if(nIndex == 0)
	{		
		time_t t1 = mktime( &vecTimeScales[0]);
		t1 += nSpan;
		struct tm tmReturn = *localtime(&t1);
		return tmReturn; 
		//	return vecTimeScales[0];	
	}
	else if( nIndex%2==0)
	{
		time_t t1 = mktime( &vecTimeScales[nIndex-1]);
		t1 -= 1;//
		struct tm tmReturn = *localtime(&t1);
		return GetTimeInSpan(tmReturn, nSpan);
		//	return vecTimeScales[nIndex-1];//11:30:10 算成11:30分
	}
	DWORD	dwCount = 0;
	for(int i = 0; i<nIndex-1; i++)
	{		
		time_t t1 = mktime( &vecTimeScales[i] );
		time_t t2 = mktime( &vecTimeScales[i+1]);
		DWORD dw = abs(difftime(t1, t2));
		dwCount += dw;
		i++;
	}
	if(nIndex != 0)
	{
		DWORD dwn = dwCount%nSpan;
		time_t t1 = mktime( &vecTimeScales[nIndex -1]);
		time_t t3 = t1-dwn;
		while(t3 <= tm11)
		{
			t3 += nSpan;
		}
		if(nIndex + 2 < (int)vecTimeScales.size())
		{
			time_t tNext = mktime( &vecTimeScales[nIndex]);
			if(t3 > tNext)
			{
				t3 = t3- tNext + mktime(&vecTimeScales[nIndex+1]);//11:29:00在30分钟是应该是13:15（9:15开盘）
			}			
		}
		tReturn = t3;	
	}
	if(tReturn > tmMax)
		tReturn = tmMax;

	struct tm tmReturn;
	tmReturn.tm_year = 0;
	tmReturn = *localtime(&tReturn);

	return tmReturn;
}


bool CFData::GetFileSavePath(CString &filepath)
{
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	std::string str = filename.substr(0, splitpos+1);

	filepath = str.c_str();

	return true;
}
bool CFData::GetSaveFileName(string  ContractDate, EnumPhrase enumPhrase, CString &filepath)
{
	string strPhrase;
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		strPhrase = "min1";
		break;
	case PHRASE_5MIN:
		strPhrase = "min5";
		break;
	case PHRASE_15MIN:
		strPhrase = "min15";
		break;
	case PHRASE_30MIN:
		strPhrase = "min30";
		break;
	case PHRASE_60MIN:
		strPhrase = "min60";
		break;
	case PHRASE_DAY:
		strPhrase = "day";
		break;
	case PHRASE_WEEK:
		strPhrase = "week";
		break;
	case PHRASE_MONTH:
		strPhrase = "month";
		break;

	}
	filepath.Format(_T("%s_%s"), ContractDate.c_str(), strPhrase.c_str());
	return true;
}
bool CFData::SaveFile()
{		
	CString strPath;
	GetFileSavePath(strPath);	
	CString strDir;
	strDir.Format(_T("%shqData\\"), strPath);
	CreateDirectory(strDir, NULL);
	CString strSubDir;
//	if(m_Simulate == 1)
//		strSubDir.Format(_T("%shqData_simi\\"), strDir);
//	else
		strSubDir.Format(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);
	strPath.Format(_T("%s\\"), strSubDir);
	CreateDirectory(strPath.GetBuffer (strPath.GetLength()), NULL);

	CString filepath;
	CString strFileName;

	if(m_vecKLineDay.size()!= 0 && m_bDayShow)//只有在读取本地数据以后，才会保存相应的周期数据，要不然本地数据会被覆盖掉
	{
		CString strPathTemp = strPath;
		strPathTemp.Append(_T("Day\\"));
		CreateDirectory(strPathTemp.GetBuffer(strPathTemp.GetLength()), NULL);
		GetSaveFileName(m_strInstrument, PHRASE_DAY,strFileName);
		filepath.Format(_T("%s\\%s"), strPathTemp, strFileName);
		SaveOneFile(filepath, m_vecKLineDay);
	}

	if(m_vecKLineWeek.size()!= 0 && m_bWeekShow)
	{
		CString strPathTemp = strPath;
		strPathTemp.Append(_T("Week\\"));
		CreateDirectory(strPathTemp.GetBuffer(strPathTemp.GetLength()), NULL);
		GetSaveFileName(m_strInstrument, PHRASE_WEEK,strFileName);
		filepath.Format(_T("%s\\%s"), strPathTemp, strFileName);
		SaveOneFile(filepath, m_vecKLineWeek);
	}

	if(m_vecKLineMonth.size()!= 0 && m_bMonthShow)
	{
		CString strPathTemp = strPath;
		strPathTemp.Append(_T("Month\\"));
		CreateDirectory(strPathTemp.GetBuffer(strPathTemp.GetLength()), NULL);
		GetSaveFileName(m_strInstrument, PHRASE_MONTH,strFileName);
		filepath.Format(_T("%s\\%s"), strPathTemp, strFileName);
		SaveOneFile(filepath, m_vecKLineMonth);
	}

	if(m_vecKLineMin1.size()!= 0 && m_bMin1Show)
	{
		CString strPathTemp = strPath;
		strPathTemp.Append(_T("Min1\\"));
		CreateDirectory(strPathTemp.GetBuffer(strPathTemp.GetLength()), NULL);
		GetSaveFileName(m_strInstrument, PHRASE_1MIN,strFileName);
		filepath.Format(_T("%s\\%s"), strPathTemp, strFileName);
		SaveOneFile(filepath, m_vecKLineMin1);
	}	

	if(m_vecKLineMin5.size()!= 0 && m_bMin5Show)
	{
		CString strPathTemp = strPath;
		strPathTemp.Append(_T("Min5\\"));
		CreateDirectory(strPathTemp.GetBuffer(strPathTemp.GetLength()), NULL);
		GetSaveFileName(m_strInstrument, PHRASE_5MIN,strFileName);
		filepath.Format(_T("%s\\%s"), strPathTemp, strFileName);
		SaveOneFile(filepath, m_vecKLineMin5);
	}	
	//////////////////////////////////////////////////////////////////////////
	//以后只保留1分钟，5分钟基础数据，15,30,60分钟不再保存，目前因为没有5分到其他的转换函数，暂时先简单化处理
	if(m_vecKLineMin15.size()!= 0 && m_bMin15Show)
	{
		CString strPathTemp = strPath;
		strPathTemp.Append(_T("Min15\\"));
		CreateDirectory(strPathTemp.GetBuffer(strPathTemp.GetLength()), NULL);
		GetSaveFileName(m_strInstrument, PHRASE_15MIN,strFileName);
		filepath.Format(_T("%s\\%s"), strPathTemp, strFileName);
		SaveOneFile(filepath, m_vecKLineMin15);
	}	

	if(m_vecKLineMin30.size()!= 0 && m_bMin30Show)
	{
		CString strPathTemp = strPath;
		strPathTemp.Append(_T("Min30\\"));
		CreateDirectory(strPathTemp.GetBuffer(strPathTemp.GetLength()), NULL);
		GetSaveFileName(m_strInstrument, PHRASE_30MIN,strFileName);
		filepath.Format(_T("%s\\%s"), strPathTemp, strFileName);
		SaveOneFile(filepath, m_vecKLineMin30);
	}	

	if(m_vecKLineMin60.size()!= 0 && m_bMin60Show)
	{
		CString strPathTemp = strPath;
		strPathTemp.Append(_T("Min60\\"));
		CreateDirectory(strPathTemp.GetBuffer(strPathTemp.GetLength()), NULL);
		GetSaveFileName(m_strInstrument, PHRASE_60MIN,strFileName);
		filepath.Format(_T("%s\\%s"), strPathTemp, strFileName);
		SaveOneFile(filepath, m_vecKLineMin60);
	}	
	return true;
}
bool CFData::SaveOneFile(CString filepath, vector<SKLine >& vecKLine)
{
	vector<SKLine_IO> m_vecKLine_IO;
	for(int i=0; i< (int)vecKLine.size(); i++)
	{
		SKLine p = vecKLine[i];
		SKLine_IO temp;
		temp.dwTime = mktime(& p.dwTime);
		temp.fOpenPrice = p.fOpenPrice;
		temp.fClosePrice= p.fClosePrice;
		temp.fHighPrice   = p.fHighPrice;
		temp.fLowPrice    = p.fLowPrice;
		temp.dwHoldVolume = p.dwHoldVolume;
		temp.dwVolume     = p.dwVolume;	
		temp.fSumTradeVolume = p.fSumTradeVolume;
		temp.dJieSuan		 = p.dJieSuan;
		m_vecKLine_IO.push_back(temp);
	}
	FILE* fp = fopen( filepath, "wb" );
	if(fp  == NULL )
		return false;
	int nCount = m_vecKLine_IO.size();
	if(nCount != 0)
	{
		fwrite(&nCount, sizeof(int), 1, fp);
		fwrite( &m_vecKLine_IO[0], sizeof(SKLine_IO) , m_vecKLine_IO.size() , fp );
	}
	fclose(fp);
	return true;
}
bool CFData::GetSelMinsValue(EnumPhrase enumPhrase)
{
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			return m_vecKLineMin1.size() == 0;
		}
		break;
	case PHRASE_5MIN:
		{
			return  m_vecKLineMin5.size() == 0;
		}
		break;
	case PHRASE_15MIN:
		{
			return  m_vecKLineMin15.size() == 0;
		}
		break;
	case PHRASE_30MIN:
		{
			return   m_vecKLineMin30.size() == 0;
		}
		break;
	case PHRASE_60MIN:
		{
			return m_vecKLineMin60.size() == 0;
		}
		break;
	case PHRASE_DAY:
		{
			return m_vecKLineDay.size() == 0;
		}
		break;
	case PHRASE_WEEK:
		{
			return m_vecKLineWeek.size() == 0;
		}
		break;
	case PHRASE_MONTH:
		{
			return m_vecKLineMonth.size() == 0;
		}
		break;
	}
	return false;
}
bool CFData::ReadFile(EnumPhrase enumPhrase)
{
	if(!GetSelMinsValue(enumPhrase))
		return true;//判断是不是已经读取过，读取过则不再重复读取
	
	CString strPath;
	GetFileSavePath(strPath);	
	CString strDir;
	strDir.Format(_T("%shqData\\"), strPath);
	CString strSubDir;
//	if(m_Simulate == 1)
//		strSubDir.Format(_T("%shqData_simi\\"), strDir);
//	else
		strSubDir.Format(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);
	strPath.Format(_T("%s\\"), strSubDir);
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			strPath.Append(_T("Min1\\"));
		}
		break;
	case PHRASE_5MIN:
		{
			strPath.Append(_T("Min5\\"));
		}
		break;
	case PHRASE_15MIN:
		{
			strPath.Append(_T("Min15\\"));
		}
		break;
	case PHRASE_30MIN:
		{
			strPath.Append(_T("Min30\\"));
		}
		break;
	case PHRASE_60MIN:
		{
			strPath.Append(_T("Min60\\"));
		}
		break;
	case PHRASE_DAY:
		{
			strPath.Append(_T("Day\\"));
		}
		break;
	case PHRASE_WEEK:
		{
			strPath.Append(_T("Week\\"));
		}
		break;
	case PHRASE_MONTH:
		{
			strPath.Append(_T("Month\\"));
		}
		break;
	}
	CString filepath;
	CString strFileName2;
	GetSaveFileName(m_strInstrument, enumPhrase,strFileName2);
	filepath.Format(_T("%s%s"), strPath, strFileName2);

	FILE* fp = fopen( filepath, "rb" );
	if(fp  == NULL )
		return false;
	int nCount;
	fread( &nCount, sizeof(int) , 1 , fp );	
	if(nCount <= 0)
	{
		fclose(fp);
		return true;
	}
	vector<SKLine_IO> vecKLine_IO(nCount);
	fread( &vecKLine_IO[0], sizeof(SKLine_IO) , nCount , fp );
	fclose(fp);
	vector<SKLine > vecKLineCommon;
	for(int i=0; i < (int)vecKLine_IO.size(); i++)
	{
		SKLine_IO kIO = vecKLine_IO[i];
		SKLine pKLine;
		time_t t = time_t(kIO.dwTime);
		pKLine.dwTime = *localtime(&t);
		pKLine.fOpenPrice = (double)kIO.fOpenPrice;
		pKLine.fHighPrice = (double)kIO.fHighPrice;
		pKLine.fLowPrice  = (double)kIO.fLowPrice;
		pKLine.fClosePrice = (double)kIO.fClosePrice;
		pKLine.dwVolume    = kIO.dwVolume;
		pKLine.fSumTradeVolume = (double)kIO.fSumTradeVolume;
		pKLine.dwHoldVolume    = kIO.dwHoldVolume;
		pKLine.dJieSuan        = (double)kIO.dJieSuan;
		vecKLineCommon.push_back(pKLine);
	}
	vecKLine_IO.clear();

	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			m_vecKLineMin1 = vecKLineCommon;
		}
		break;
	case PHRASE_5MIN:
		{
			 m_vecKLineMin5 = vecKLineCommon;
		}
		break;
	case PHRASE_15MIN:
		{
			m_vecKLineMin15 = vecKLineCommon;
		}
		break;
	case PHRASE_30MIN:
		{
			  m_vecKLineMin30 = vecKLineCommon;
		}
		break;
	case PHRASE_60MIN:
		{
			m_vecKLineMin60 = vecKLineCommon;
		}
		break;
	case PHRASE_DAY:
		{
			m_vecKLineDay = vecKLineCommon;
		}
		break;
	case PHRASE_WEEK:
		{
			m_vecKLineWeek = vecKLineCommon;
		}
		break;
	case PHRASE_MONTH:
		{
			m_vecKLineMonth = vecKLineCommon;
		}
		break;
	}	
	return true;
}
bool CFData::GetFileSavePath_TLine(CString &filepath)
{
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	std::string strfilepath = filename.substr(0, splitpos+1);
	filepath = strfilepath.c_str();
	return true;
}
bool CFData::ReadFile_TLine()
{	
	if(m_vecTimeScales.size() ==0)
		return false;

	struct tm tmDay;
	CString TradingDay;
	TradingDay = m_strTradingDay.c_str();
	tmDay.tm_year	= atoi(TradingDay.Left(4))-1900;//year since 1900		
	tmDay.tm_mon	= atoi(TradingDay.Mid(4,2))-1;//[0, 11]
	tmDay.tm_mday	= atoi(TradingDay.Right(2));

	CString strPath;
	GetFileSavePath_TLine(strPath);	
	CString strDir;
	strDir.Format(_T("%shqData\\"), strPath);
	CString strSubDir;
//	if(m_Simulate == 1)
//		strSubDir.Format(_T("%shqData_simi\\"), strDir);
//	else
		strSubDir.Format(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);

	strPath.Format(_T("%s%s\\"), strSubDir, m_ExchangeName.c_str());
	strPath.Append(_T("\\"));

	CString filepath;
	CString strFileName;
	CString strFileName2;
	strFileName2.Format(_T("%s_%s"), m_ExchangeName.c_str(), m_strInstrument.c_str());
	strFileName.Format(_T("%s\\%s%s"), strPath, strFileName2, m_strTradingDay.c_str());

	FILE* fp = fopen( strFileName, "rb" );
	if(fp  == NULL )
		return false;
	int nCount;
	fread( &nCount, sizeof(int) , 1 , fp );
	if(nCount <= 0)
	{
		fclose(fp);
		return true;
	}
	vector<STLine_IO> vecTLine_IO(nCount);
	fread( &vecTLine_IO[0], sizeof(STLine_IO) , nCount , fp );
	fclose(fp);

	for(int i=0; i < (int)vecTLine_IO.size(); i++)
	{
		STLine_IO kIO = vecTLine_IO[i];
		STLine pTLine;
		time_t t = time_t(kIO.dwTime);
		pTLine.dwTime = *localtime(&t);
		pTLine.dwHoldVolume = kIO.dwHoldVolume;
		pTLine.dwVolume     = kIO.dwVolume;
		pTLine.dwVolumeMoney = (double)kIO.dwVolumeMoney;
		pTLine.fLastPrice    = (double)kIO.fLastPrice;
		m_vecTLine.push_back(pTLine);
	}
	vecTLine_IO.clear();
	
	return true;
}
bool CFData::SaveFile_TLine()
{		
	if(m_vecTLine.size() ==0)
		return false;

	CString strPath;
	GetFileSavePath_TLine(strPath);	
	CString strDir;
	strDir.Format(_T("%shqData\\"), strPath);
	CreateDirectory(strDir, NULL);
	CString strSubDir;
	//if(m_Simulate == 1)
	//	strSubDir.Format(_T("%shqData_simi\\"), strDir);
	//else
		strSubDir.Format(_T("%shqData_Trans\\"), strDir);	
	CreateDirectory(strSubDir, NULL);

	strPath.Format(_T("%s\\"), strSubDir);
	CreateDirectory(strPath.GetBuffer(strPath.GetLength()), NULL);
	strPath.Append(_T("TL\\"));
	CreateDirectory(strPath.GetBuffer(strPath.GetLength()), NULL);

	CString filepath;
	CString strFileName;
	CString strFileName2;
	strFileName2.Format(_T("%s"),  m_strInstrument.c_str());
	strFileName.Format(_T("%s\\%s%s"), strPath, strFileName2, m_strTradingDay.c_str());

	SaveOneFile_TLine(strFileName, m_vecTLine);


	return true;
}
bool CFData::SaveOneFile_TLine(CString filepath, vector<STLine>& vecTLine)
{
	if(m_vecTLine.size() ==0)
		return false;

	vector<STLine_IO> vecTLine_IO;
	for(int i=0; i< (int)vecTLine.size(); i++)
	{
		STLine p = (vecTLine[i]);
		STLine_IO temp;
		temp.dwTime = mktime(& p.dwTime);
		temp.dwVolumeMoney = p.dwVolumeMoney;
		temp.dwHoldVolume = p.dwHoldVolume;
		temp.dwVolume     = p.dwVolume;	
		temp.fLastPrice   = p.fLastPrice ;
		vecTLine_IO.push_back(temp);
	}
	FILE* fp = fopen( filepath, "wb" );
	if(fp  == NULL )
		return false;
	int nCount = vecTLine_IO.size();
	if(nCount != 0)
	{
		fwrite(&nCount, sizeof(int), 1, fp);
		fwrite( &vecTLine_IO[0], sizeof(STLine_IO) , vecTLine_IO.size(), fp );
	}
	fclose(fp);
	return true;
}
bool CFData::GetVecTLine(vector<STLine>& vec)
{
	EnterCriticalSection(&m_csTLine);
	vec= m_vecTLine;
	LeaveCriticalSection(&m_csTLine);
	return true;
}
bool CFData::IsKLineQueryFinish(EnumPhrase enumPhrase)
{
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			return m_bMin1Show;
		}
		break;
	case PHRASE_5MIN:
		{
			return m_bMin5Show;
		}
		break;
	case PHRASE_15MIN:
		{
			return m_bMin15Show;
		}
		break;
	case PHRASE_30MIN:
		{
			return m_bMin30Show;
		}
		break;
	case PHRASE_60MIN:
		{
			return m_bMin60Show;
		}
		break;
	case PHRASE_DAY:
		{
			return m_bDayShow;
		}
		break;
	case PHRASE_WEEK:
		{
			return m_bWeekShow;
		}
		break;
	case PHRASE_MONTH:
		{
			return m_bMonthShow;
		}
		break;
	}

	return true;
}
bool CFData::GetVecKLine(EnumPhrase enumPhrase, vector<SKLine>& vec)
{	
	switch(enumPhrase)
	{
	case PHRASE_1MIN:
		{
			return GetVecKLineMin1(vec);
		}
		break;
	case PHRASE_5MIN:
		{
			return GetVecKLineMin5(vec);
		}
		break;
	case PHRASE_15MIN:
		{
			return GetVecKLineMin15(vec);
		}
		break;
	case PHRASE_30MIN:
		{
			return GetVecKLineMin30(vec);
		}
		break;
	case PHRASE_60MIN:
		{
			return GetVecKLineMin60(vec);
		}
		break;
	case PHRASE_DAY:
		{
			return GetVecKLineDay(vec);
		}
		break;
	case PHRASE_WEEK:
		{
			return GetVecKLineWeek(vec);
		}
		break;
	case PHRASE_MONTH:
		{
			return GetVecKLineMonth(vec);
		}
		break;
	}

	return false;
}
bool CFData::GetVecKLineDay(vector<SKLine>& vec)
{
	EnterCriticalSection(&m_csKLineDay);
	vec= m_vecKLineDay;
	LeaveCriticalSection(&m_csKLineDay);
	return true;
}
bool CFData::GetVecKLineWeek(vector<SKLine>& vec)
{
	EnterCriticalSection(&m_csKLineWeek);
	vec= m_vecKLineWeek;
	LeaveCriticalSection(&m_csKLineWeek);
	return true;
}
bool CFData::GetVecKLineMonth(vector<SKLine>& vec)
{
	EnterCriticalSection(&m_csKLineMonth);
	vec= m_vecKLineMonth;
	LeaveCriticalSection(&m_csKLineMonth);
	return true;
}
bool CFData::GetVecKLineMin1(vector<SKLine>& vec)
{
	EnterCriticalSection(&m_csKLineMin1);
	vec= m_vecKLineMin1;
	LeaveCriticalSection(&m_csKLineMin1);
	return true;
}
bool CFData::GetVecKLineMin5(vector<SKLine>& vec)
{
	EnterCriticalSection(&m_csKLineMin5);
	vec= m_vecKLineMin5;
	LeaveCriticalSection(&m_csKLineMin5);
	return true;
}
bool CFData::GetVecKLineMin15(vector<SKLine>& vec)
{
	EnterCriticalSection(&m_csKLineMin15);
	vec= m_vecKLineMin15;
	LeaveCriticalSection(&m_csKLineMin15);
	return true;
}
bool CFData::GetVecKLineMin30(vector<SKLine>& vec)
{
	EnterCriticalSection(&m_csKLineMin30);
	vec= m_vecKLineMin30;
	LeaveCriticalSection(&m_csKLineMin30);
	return true;
}
bool CFData::GetVecKLineMin60(vector<SKLine>& vec)
{
	EnterCriticalSection(&m_csKLineMin60);
	vec= m_vecKLineMin60;
	LeaveCriticalSection(&m_csKLineMin60);
	return true;
}
bool CFData::AppendHistoryTLineData(std::vector<TLineBase>& vecTimeLineData)
{
	if(vecTimeLineData.size() == 0)
		return true;

	std::vector<STLine> vecTLine;
	for(int i = vecTimeLineData.size()-1; i>=0; i--)
	{
		TLineBase& data = vecTimeLineData[i];
		time_t	dwTime = data.dwTime;
		STLine sTLine;
		sTLine.dwTime = *localtime(&dwTime);
		sTLine.fLastPrice = data.fLastPrice;
		sTLine.dwVolume = data.dwVolume;
		sTLine.dwHoldVolume = data.dwOpenInterest;
		sTLine.dwVolumeMoney = data.fTurnover;
		vecTLine.push_back(sTLine);
	}
	
	vector<STLine> vec;
	EnterCriticalSection(&m_csTLine);
	vec= m_vecTLine;
	LeaveCriticalSection(&m_csTLine);

	if(vecTLine.size() && vec.size())
	{//去掉历史数据中最后一个数据，因为这个数据已经向历史服务器请求了
		vector<STLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecTLine.insert(vecTLine.end(), vec.begin(), vec.end());

	
	EnterCriticalSection(&m_csTLine);
	m_vecTLine= vecTLine;
	m_bTLineShow = true;
	LeaveCriticalSection(&m_csTLine);
	return true;
}
bool CFData::AppendRealTimeTLineData()
{
	EnterCriticalSection(&m_csDepthMarketData);
	STLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecTLine.size()>0)
		tLine= m_vecTLine[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		STLine pLine;
		pLine.dwTime.tm_year	= atoi(strDate.Left(4))-1900;
		pLine.dwTime.tm_mon	= atoi(strDate.Mid(4,2))-1;
		pLine.dwTime.tm_mday	= atoi(strDate.Right(2));

		pLine.dwTime.tm_hour	= atoi(strTime.Left(2));
		pLine.dwTime.tm_min	= atoi(strTime.Mid(3, 2));
		pLine.dwTime.tm_sec	= atoi(strTime.Mid(6, 2));;
		pLine.dwTime.tm_wday	= 0;
		pLine.dwTime.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;
		struct tm tm2 = pLine.dwTime;
		if(difftime(mktime(&tm1), mktime(&tm2)) >0)
			continue;//已经在历史数据里面有了		
		SetInTimeData_TLine(&data);
	}	
	m_bTLineShow = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;
}
bool CFData::AppendHistoryKLineData_Min1(std::vector<KLineBase>& vecData)
{
	if(vecData.size() ==0)
		return true;

	std::vector<SKLine> vecKLine;
	for(int i = vecData.size()-1; i>=0; i--)
	{
		KLineBase& data = vecData[i];
		time_t	dwTime = data.dwTime;

	
	/*	std::string strDay = data.szDate;
		std::string strTime = data.szTime;
		if(strDay.length()<8 || strTime.length() < 8)
			continue;*/
		SKLine sKLine;
		/*sKLine.dwTime.tm_year = atoi(strDay.substr(0, 4).c_str()) - 1900;
		sKLine.dwTime.tm_mon  = atoi(strDay.substr(4, 2).c_str()) - 1;
		sKLine.dwTime.tm_mday = atoi(strDay.substr(6, 2).c_str());
		sKLine.dwTime.tm_hour = atoi(strTime.substr(0, 2).c_str());
		sKLine.dwTime.tm_min  = atoi(strTime.substr(3, 2).c_str());
		sKLine.dwTime.tm_sec  = atoi(strTime.substr(6, 2).c_str());
		sKLine.dwTime= GetTimeInSpan_History(sKLine.dwTime, 60);*/
		sKLine.dwTime = *localtime(&dwTime);
		sKLine.dwTime= GetTimeInSpan_History(sKLine.dwTime, 60);
		sKLine.fOpenPrice = data.fOpenPrice;
		sKLine.fHighPrice = data.fHighPrice;
		sKLine.fLowPrice  = data.fLowPrice;
		sKLine.fClosePrice = data.fClosePrice;
		//sKLine.dJieSuan    = data.SettlementPrice;
		sKLine.dwHoldVolume =data.dwOpenInterest;
		sKLine.dwVolume     = data.dwVolume;
		sKLine.fSumTradeVolume = data.fTurnover;

		vecKLine.push_back(sKLine);
	}

	vector<SKLine> vec;
	EnterCriticalSection(&m_csKLineMin1);
	vec= m_vecKLineMin1;
	LeaveCriticalSection(&m_csKLineMin1);

	if(vecKLine.size() && vec.size())
	{
		vector<SKLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecKLine.insert(vecKLine.end(), vec.begin(), vec.end());


	EnterCriticalSection(&m_csTLine);
	m_vecKLineMin1= vecKLine;
	LeaveCriticalSection(&m_csTLine);
	

	return true;
}
bool CFData::AppendRealTimeKLineData_Min1()
{
	EnterCriticalSection(&m_csDepthMarketData);
	SKLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecKLineMin1.size()>0)
		tLine= m_vecKLineMin1[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		struct tm tm2;
		tm2.tm_year	= atoi(strDate.Left(4))-1900;
		tm2.tm_mon	= atoi(strDate.Mid(4,2))-1;
		tm2.tm_mday	= atoi(strDate.Right(2));

		tm2.tm_hour	= atoi(strTime.Left(2));
		tm2.tm_min	= atoi(strTime.Mid(3, 2));
		tm2.tm_sec	= atoi(strTime.Mid(6, 2));;
		tm2.tm_wday	= 0;
		tm2.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;	
		if(difftime(mktime(&tm1), mktime(&tm2)) >0)
			continue;//已经在历史数据里面有了

	
		SetInTimeData_KLine(&data,	 false,
			false,
			false,
			true,
			false,
			false,
			false,
			false);		
	}	
	m_bMin1Show = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;	
}
bool CFData::AppendHistoryKLineData_Min5(std::vector<KLineBase>& vecData)
{
	if(vecData.size() ==0)
		return true;

	std::vector<SKLine> vecKLine;
	for(int i = vecData.size()-1; i>=0; i--)
	{
		KLineBase& data = vecData[i];
		time_t	dwTime = data.dwTime;

		SKLine sKLine;	
		sKLine.dwTime = *localtime(&dwTime);
		sKLine.dwTime= GetTimeInSpan_History(sKLine.dwTime, 300);

		sKLine.fOpenPrice = data.fOpenPrice;
		sKLine.fHighPrice = data.fHighPrice;
		sKLine.fLowPrice  = data.fLowPrice;
		sKLine.fClosePrice = data.fClosePrice;
		sKLine.dwHoldVolume =data.dwOpenInterest;
		sKLine.dwVolume     = data.dwVolume;
		sKLine.fSumTradeVolume = data.fTurnover;

		vecKLine.push_back(sKLine);
	}

	vector<SKLine> vec;
	EnterCriticalSection(&m_csKLineMin5);
	vec= m_vecKLineMin5;
	LeaveCriticalSection(&m_csKLineMin5);

	if(vecKLine.size() && vec.size())
	{
		vector<SKLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecKLine.insert(vecKLine.end(), vec.begin(), vec.end());


	EnterCriticalSection(&m_csKLineMin5);
	m_vecKLineMin5= vecKLine;
	LeaveCriticalSection(&m_csKLineMin5);


	return true;
}
bool CFData::AppendRealTimeKLineData_Min5()
{
	EnterCriticalSection(&m_csDepthMarketData);
	SKLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecKLineMin5.size()>0)
		tLine= m_vecKLineMin5[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		struct tm tm2;
		tm2.tm_year	= atoi(strDate.Left(4))-1900;
		tm2.tm_mon	= atoi(strDate.Mid(4,2))-1;
		tm2.tm_mday	= atoi(strDate.Right(2));

		tm2.tm_hour	= atoi(strTime.Left(2));
		tm2.tm_min	= atoi(strTime.Mid(3, 2));
		tm2.tm_sec	= atoi(strTime.Mid(6, 2));;
		tm2.tm_wday	= 0;
		tm2.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;	
		if(difftime(mktime(&tm1), mktime(&tm2)) >0)
			continue;//已经在历史数据里面有了


		SetInTimeData_KLine(&data,	 false,
			false,
			false,
			false,
			true,			
			false,
			false,
			false);		
	}	
	m_bMin5Show = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;	
}

bool CFData::AppendHistoryKLineData_Min15(std::vector<KLineBase>& vecData)
{
	if(vecData.size() ==0)
		return true;
	std::vector<SKLine> vecKLine;
	for(int i = vecData.size()-1; i>=0; i--)
	{
		KLineBase& data = vecData[i];
		time_t	dwTime = data.dwTime;

		SKLine sKLine;	
		sKLine.dwTime = *localtime(&dwTime);
		sKLine.dwTime= GetTimeInSpan_History(sKLine.dwTime, 900);
		
		sKLine.fOpenPrice = data.fOpenPrice;
		sKLine.fHighPrice = data.fHighPrice;
		sKLine.fLowPrice  = data.fLowPrice;
		sKLine.fClosePrice = data.fClosePrice;
		sKLine.dwHoldVolume =data.dwOpenInterest;
		sKLine.dwVolume     = data.dwVolume;
		sKLine.fSumTradeVolume = data.fTurnover;

		vecKLine.push_back(sKLine);
	}

	vector<SKLine> vec;
	EnterCriticalSection(&m_csKLineMin15);
	vec= m_vecKLineMin15;
	LeaveCriticalSection(&m_csKLineMin15);

	if(vecKLine.size() && vec.size())
	{
		vector<SKLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecKLine.insert(vecKLine.end(), vec.begin(), vec.end());


	EnterCriticalSection(&m_csKLineMin15);
	m_vecKLineMin15= vecKLine;
	LeaveCriticalSection(&m_csKLineMin15);

	return true;
}
bool CFData::AppendRealTimeKLineData_Min15()
{
	EnterCriticalSection(&m_csDepthMarketData);
	SKLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecKLineMin15.size()>0)
		tLine= m_vecKLineMin15[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		struct tm tm2;
		tm2.tm_year	= atoi(strDate.Left(4))-1900;
		tm2.tm_mon	= atoi(strDate.Mid(4,2))-1;
		tm2.tm_mday	= atoi(strDate.Right(2));

		tm2.tm_hour	= atoi(strTime.Left(2));
		tm2.tm_min	= atoi(strTime.Mid(3, 2));
		tm2.tm_sec	= atoi(strTime.Mid(6, 2));;
		tm2.tm_wday	= 0;
		tm2.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;	
		if(difftime(mktime(&tm1), mktime(&tm2)) >0)
			continue;//已经在历史数据里面有了


		SetInTimeData_KLine(&data,	 false,
			false,
			false,
			false,
			false,
			true,				
			false,
			false);		
	}	
	m_bMin15Show = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;	
}
bool CFData::AppendHistoryKLineData_Min30(std::vector<KLineBase>& vecData)
{
	if(vecData.size() ==0)
		return true;
	std::vector<SKLine> vecKLine;
	for(int i = vecData.size()-1; i>=0; i--)
	{
		KLineBase& data = vecData[i];
		time_t	dwTime = data.dwTime;	
		SKLine sKLine;	
		sKLine.dwTime = *localtime(&dwTime);
		sKLine.dwTime= GetTimeInSpan_History(sKLine.dwTime, 1800);
		sKLine.fOpenPrice = data.fOpenPrice;
		sKLine.fHighPrice = data.fHighPrice;
		sKLine.fLowPrice  = data.fLowPrice;
		sKLine.fClosePrice = data.fClosePrice;
		sKLine.dwHoldVolume =data.dwOpenInterest;
		sKLine.dwVolume     = data.dwVolume;
		sKLine.fSumTradeVolume = data.fTurnover;

		vecKLine.push_back(sKLine);
	}

	vector<SKLine> vec;
	EnterCriticalSection(&m_csKLineMin30);
	vec= m_vecKLineMin30;
	LeaveCriticalSection(&m_csKLineMin30);

	if(vecKLine.size() && vec.size())
	{
		vector<SKLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecKLine.insert(vecKLine.end(), vec.begin(), vec.end());


	EnterCriticalSection(&m_csKLineMin30);
	m_vecKLineMin30= vecKLine;
	LeaveCriticalSection(&m_csKLineMin30);

	return true;
}
bool CFData::AppendRealTimeKLineData_Min30()
{
	EnterCriticalSection(&m_csDepthMarketData);
	SKLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecKLineMin30.size()>0)
		tLine= m_vecKLineMin30[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		struct tm tm2;
		tm2.tm_year	= atoi(strDate.Left(4))-1900;
		tm2.tm_mon	= atoi(strDate.Mid(4,2))-1;
		tm2.tm_mday	= atoi(strDate.Right(2));

		tm2.tm_hour	= atoi(strTime.Left(2));
		tm2.tm_min	= atoi(strTime.Mid(3, 2));
		tm2.tm_sec	= atoi(strTime.Mid(6, 2));;
		tm2.tm_wday	= 0;
		tm2.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;	
		if(difftime(mktime(&tm1), mktime(&tm2)) >0)
			continue;//已经在历史数据里面有了


		SetInTimeData_KLine(&data,	 false,
			false,
			false,
			false,
			false,
			false,
			true,			
			false);		
	}	
	m_bMin30Show = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;	
}
bool CFData::AppendHistoryKLineData_Min60(std::vector<KLineBase>& vecData)
{
	if(vecData.size() ==0)
		return true;
	std::vector<SKLine> vecKLine;
	for(int i = vecData.size()-1; i>=0; i--)
	{
		KLineBase& data = vecData[i];
		time_t	dwTime = data.dwTime;	
		SKLine sKLine;	
		sKLine.dwTime = *localtime(&dwTime);
		sKLine.dwTime= GetTimeInSpan_History(sKLine.dwTime, 3600);
		sKLine.fOpenPrice = data.fOpenPrice;
		sKLine.fHighPrice = data.fHighPrice;
		sKLine.fLowPrice  = data.fLowPrice;
		sKLine.fClosePrice = data.fClosePrice;
		sKLine.dwHoldVolume =data.dwOpenInterest;
		sKLine.dwVolume     = data.dwVolume;
		sKLine.fSumTradeVolume = data.fTurnover;

		vecKLine.push_back(sKLine);
	}

	vector<SKLine> vec;
	EnterCriticalSection(&m_csKLineMin60);
	vec= m_vecKLineMin60;
	LeaveCriticalSection(&m_csKLineMin60);

	if(vecKLine.size() && vec.size())
	{
		vector<SKLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecKLine.insert(vecKLine.end(), vec.begin(), vec.end());


	EnterCriticalSection(&m_csKLineMin60);
	m_vecKLineMin60= vecKLine;
	LeaveCriticalSection(&m_csKLineMin60);

	return true;
}
bool CFData::AppendRealTimeKLineData_Min60()
{
	EnterCriticalSection(&m_csDepthMarketData);
	SKLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecKLineMin60.size()>0)
		tLine= m_vecKLineMin60[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		struct tm tm2;
		tm2.tm_year	= atoi(strDate.Left(4))-1900;
		tm2.tm_mon	= atoi(strDate.Mid(4,2))-1;
		tm2.tm_mday	= atoi(strDate.Right(2));

		tm2.tm_hour	= atoi(strTime.Left(2));
		tm2.tm_min	= atoi(strTime.Mid(3, 2));
		tm2.tm_sec	= atoi(strTime.Mid(6, 2));;
		tm2.tm_wday	= 0;
		tm2.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;	
		if(difftime(mktime(&tm1), mktime(&tm2)) >0)
			continue;//已经在历史数据里面有了


		SetInTimeData_KLine(&data,	 false,
			false,
			false,
			false,
			false,
			false,
			false,
			true			
			);		
	}	
	m_bMin60Show = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;	
}
bool CFData::AppendHistoryKLineData_Day(std::vector<KLineBase>& vecData)
{
	if(vecData.size() ==0)
		return true;
	std::vector<SKLine> vecKLine;
	for(int i = vecData.size()-1; i>=0; i--)
	{
		KLineBase& data = vecData[i];
		time_t	dwTime = data.dwTime;	
		SKLine sKLine;
		sKLine.dwTime = *localtime(&dwTime);
		sKLine.dwTime.tm_hour = 0;
		sKLine.dwTime.tm_min  = 0;
		sKLine.dwTime.tm_sec  = 0;
		
		sKLine.fOpenPrice = data.fOpenPrice;
		sKLine.fHighPrice = data.fHighPrice;
		sKLine.fLowPrice  = data.fLowPrice;
		sKLine.fClosePrice = data.fClosePrice;
		sKLine.dwHoldVolume =data.dwOpenInterest;
		sKLine.dwVolume     = data.dwVolume;
		sKLine.fSumTradeVolume = data.fTurnover;

		vecKLine.push_back(sKLine);
	}

	vector<SKLine> vec;
	EnterCriticalSection(&m_csKLineDay);
	vec= m_vecKLineDay;
	LeaveCriticalSection(&m_csKLineDay);

	if(vecKLine.size() && vec.size())
	{
		vector<SKLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecKLine.insert(vecKLine.end(), vec.begin(), vec.end());


	EnterCriticalSection(&m_csKLineDay);
	m_vecKLineDay = vecKLine;
	LeaveCriticalSection(&m_csKLineDay);

	return true;
}
bool CFData::AppendRealTimeKLineData_Day()
{
	EnterCriticalSection(&m_csDepthMarketData);
	SKLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecKLineDay.size()>0)
		tLine= m_vecKLineDay[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		struct tm tm2;
		tm2.tm_year	= atoi(strDate.Left(4))-1900;
		tm2.tm_mon	= atoi(strDate.Mid(4,2))-1;
		tm2.tm_mday	= atoi(strDate.Right(2));

		tm2.tm_hour	= atoi(strTime.Left(2));
		tm2.tm_min	= atoi(strTime.Mid(3, 2));
		tm2.tm_sec	= atoi(strTime.Mid(6, 2));;
		tm2.tm_wday	= 0;
		tm2.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;	
		if(difftime(mktime(&tm1), mktime(&tm2)) >0)
			continue;//已经在历史数据里面有了


		SetInTimeData_KLine(&data,	 true,
			false,
			false,
			false,
			false,
			false,
			false,
			false			
			);		
	}	
	m_bDayShow = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;	
}
bool CFData::AppendHistoryKLineData_Week(std::vector<KLineBase>& vecData)
{
	if(vecData.size() ==0)
		return true;
	std::vector<SKLine> vecKLine;
	for(int i = vecData.size()-1; i>=0; i--)
	{
		KLineBase& data = vecData[i];
		time_t	dwTime = data.dwTime;	
		SKLine sKLine;
		sKLine.dwTime = *localtime(&dwTime);
		sKLine.dwTime.tm_hour = 0;
		sKLine.dwTime.tm_min  = 0;
		sKLine.dwTime.tm_sec  = 0;

		sKLine.fOpenPrice = data.fOpenPrice;
		sKLine.fHighPrice = data.fHighPrice;
		sKLine.fLowPrice  = data.fLowPrice;
		sKLine.fClosePrice = data.fClosePrice;
		sKLine.dwHoldVolume =data.dwOpenInterest;
		sKLine.dwVolume     = data.dwVolume;
		sKLine.fSumTradeVolume = data.fTurnover;

		vecKLine.push_back(sKLine);
	}

	vector<SKLine> vec;
	EnterCriticalSection(&m_csKLineWeek);
	vec= m_vecKLineWeek;
	LeaveCriticalSection(&m_csKLineWeek);

	if(vecKLine.size() && vec.size())
	{
		vector<SKLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecKLine.insert(vecKLine.end(), vec.begin(), vec.end());


	EnterCriticalSection(&m_csKLineWeek);
	m_vecKLineWeek = vecKLine;
	LeaveCriticalSection(&m_csKLineWeek);

	return true;
}
bool CFData::AppendRealTimeKLineData_Week()
{
	EnterCriticalSection(&m_csDepthMarketData);
	SKLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecKLineWeek.size()>0)
		tLine= m_vecKLineWeek[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		struct tm tm2;
		tm2.tm_year	= atoi(strDate.Left(4))-1900;
		tm2.tm_mon	= atoi(strDate.Mid(4,2))-1;
		tm2.tm_mday	= atoi(strDate.Right(2));

		tm2.tm_hour	= atoi(strTime.Left(2));
		tm2.tm_min	= atoi(strTime.Mid(3, 2));
		tm2.tm_sec	= atoi(strTime.Mid(6, 2));;
		tm2.tm_wday	= 0;
		tm2.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;	
		if(difftime(mktime(&tm1), mktime(&tm2)) > 0)
			continue;//已经在历史数据里面有了


		SetInTimeData_KLine(&data,	 false,
			true,
			false,
			false,
			false,
			false,
			false,
			false			
			);		
	}	
	m_bWeekShow = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;	
}
bool CFData::AppendHistoryKLineData_Month(std::vector<KLineBase>& vecData)
{
	if(vecData.size() ==0)
		return true;
	std::vector<SKLine> vecKLine;
	for(int i = vecData.size()-1; i>=0; i--)
	{
		KLineBase& data = vecData[i];
		time_t	dwTime = data.dwTime;	
		SKLine sKLine;
		sKLine.dwTime = *localtime(&dwTime);
		sKLine.dwTime.tm_hour = 0;
		sKLine.dwTime.tm_min  = 0;
		sKLine.dwTime.tm_sec  = 0;

		sKLine.fOpenPrice = data.fOpenPrice;
		sKLine.fHighPrice = data.fHighPrice;
		sKLine.fLowPrice  = data.fLowPrice;
		sKLine.fClosePrice = data.fClosePrice;
		sKLine.dwHoldVolume =data.dwOpenInterest;
		sKLine.dwVolume     = data.dwVolume;
		sKLine.fSumTradeVolume = data.fTurnover;

		vecKLine.push_back(sKLine);
	}

	vector<SKLine> vec;
	EnterCriticalSection(&m_csKLineMonth);
	vec= m_vecKLineMonth;
	LeaveCriticalSection(&m_csKLineMonth);

	if(vecKLine.size() && vec.size())
	{
		vector<SKLine>::iterator it = vec.begin();
		vec.erase(it);
	}
	if(vec.size())
		vecKLine.insert(vecKLine.end(), vec.begin(), vec.end());


	EnterCriticalSection(&m_csKLineMonth);
	m_vecKLineMonth = vecKLine;
	LeaveCriticalSection(&m_csKLineMonth);

	return true;
}
bool CFData::AppendRealTimeKLineData_Month()
{
	EnterCriticalSection(&m_csDepthMarketData);
	SKLine tLine;
	tLine.dwTime.tm_year	= 0;
	tLine.dwTime.tm_mon		= 0;
	tLine.dwTime.tm_mday	= 0;
	tLine.dwTime.tm_hour	= 0;
	tLine.dwTime.tm_min		=0;
	tLine.dwTime.tm_sec		= 0;
	tLine.dwTime.tm_wday	= 0;
	tLine.dwTime.tm_yday	= 0;
	if(m_vecKLineMonth.size()>0)
		tLine= m_vecKLineMonth[0];
	for(int i=0; i<(int)m_vecDepthMarketData.size(); i++)
	{
		PlatformStru_DepthMarketData& data = m_vecDepthMarketData[i];
		CString strDate = m_strTradingDay.c_str();//如果行情里面没有日期，照样不能解决跨日期的问题
		CString strTime		= data.UpdateTime;
		CString strFutureID = data.InstrumentID;

		struct tm tm2;
		tm2.tm_year	= atoi(strDate.Left(4))-1900;
		tm2.tm_mon	= atoi(strDate.Mid(4,2))-1;
		tm2.tm_mday	= atoi(strDate.Right(2));

		tm2.tm_hour	= atoi(strTime.Left(2));
		tm2.tm_min	= atoi(strTime.Mid(3, 2));
		tm2.tm_sec	= atoi(strTime.Mid(6, 2));;
		tm2.tm_wday	= 0;
		tm2.tm_yday	= 0;
		struct tm tm1 = tLine.dwTime;	
		if(difftime(mktime(&tm1), mktime(&tm2)) >0)
			continue;//已经在历史数据里面有了


		SetInTimeData_KLine(&data,	 false,
			false,
			true,
			false,
			false,
			false,
			false,
			false			
			);		
	}	
	m_bMonthShow = true;
	LeaveCriticalSection(&m_csDepthMarketData);
	return true;	
}