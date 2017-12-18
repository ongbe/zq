/*
 * easytradingtimespan.hpp
 *
 *  Define timespan for trading
 *  Include: bIncludeWeekend,Timespan_IntraDay,Timespan_Excludion
 *
 *  Created on: Dec 1, 2012
 *      Author: root
 */

#ifndef EASYTRADINGTIMESPAN_H_
#define EASYTRADINGTIMESPAN_H_

#ifdef WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#endif

#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <map>

#include "easytime.h"
#include "easytimespan.hpp"

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CEasyTradingTimespan
{
public:
	//Timespan_IntraDay: timespan intra day, special seconds intraday
	//Timespan_Exclusion: timespan that excluding from trading time. For example public holidays
	//tzdst: setting of timezone and dst(daylight saving time)
	CEasyTradingTimespan(bool bExcludeWeekend=true,
							const CEasyTimespan<int>& Timespan_IntraDay=CEasyTimespan<int>(),
							const CEasyTimespan<time_t>& Timespan_Exclusion=CEasyTimespan<time_t>(),
							bool blocaltzdst=true,
							const Stru_TimezoneDst& tzdst=Stru_TimezoneDst())
	{
		set(bExcludeWeekend,Timespan_IntraDay,Timespan_Exclusion,blocaltzdst,tzdst);
	}
	CEasyTradingTimespan(const CEasyTradingTimespan& r)
	{
		*this=r;
	}
	virtual ~CEasyTradingTimespan() {}

	CEasyTradingTimespan& operator= (const CEasyTradingTimespan& r)
	{
		m_blocaltzdst=r.m_blocaltzdst;
		m_tzdst=r.m_tzdst;
		m_bExcludeWeekend=r.m_bExcludeWeekend;
		m_Timespan_EachDay=r.m_Timespan_EachDay;
		m_Timespan_Exclusion=r.m_Timespan_Exclusion;
		m_Year=r.m_Year;
		m_Month=r.m_Month;
		m_Day=r.m_Day;
		m_UTC_Timezero=r.m_UTC_Timezero;
		m_Timespan_Today=r.m_Timespan_Today;
		return *this;
	}

	void clear(void)
	{
		m_blocaltzdst=false;
		m_tzdst.clear();
		m_bExcludeWeekend=true;
		m_Timespan_EachDay.clear();
		m_Timespan_Exclusion.clear();
		m_Year=1970;
		m_Month=1;
		m_Day=1;
		m_UTC_Timezero=0;
		m_Timespan_Today.clear();
	}

	void set(bool bExcludeWeekend=true,
				const CEasyTimespan<int>& Timespan_IntraDay=CEasyTimespan<int>(),
				const CEasyTimespan<time_t>& Timespan_Exclusion=CEasyTimespan<time_t>(),
				bool blocaltzdst=true,
				const Stru_TimezoneDst& tzdst=Stru_TimezoneDst())
	{
		m_blocaltzdst=blocaltzdst;
		m_tzdst=tzdst;

		m_bExcludeWeekend=bExcludeWeekend;
		std::map<int,Stru_TimespanInfo<int>>::const_iterator it;
		for(it=Timespan_IntraDay.getTimespans().begin();it!=Timespan_IntraDay.getTimespans().end();it++)
			m_Timespan_EachDay.insert(it->first%(24*3600),it->second.end%(24*3600));
		m_Timespan_EachDay.normalize();
		m_Timespan_Exclusion=Timespan_Exclusion;
		m_Timespan_Exclusion.normalize();

		if(m_blocaltzdst) CEasyTime(time(NULL)).convert2fileds(&m_Year,&m_Month,&m_Day);
		else CEasyTime2(time(NULL),0,m_tzdst).convert2fileds(&m_Year,&m_Month,&m_Day);
		m_UTC_Timezero=gettimezero(m_Year,m_Month,m_Day);
		setTodayTimespan(m_Year,m_Month,m_Day);
	}

	bool isTradingday(short year,short month,short day) const
	{
		short wday;
		time_t timezero=gettimezero(year,month,day);
		if(m_blocaltzdst) wday=CEasyTime(year,month,day).gettm().tm_wday;
		else wday=CEasyTime2(year,month,day,0,0,0,m_tzdst).gettm().tm_wday;

		if(m_bExcludeWeekend&&(wday==0||wday==6)) return false;
		if(m_Timespan_Exclusion.isTimespanIncluding(timezero,timezero+24*3600)) return false;
		return true;
	}
	bool isTradingday_today(void) const
	{
		return isTradingday(m_Year,m_Month,m_Day);
	}

	//get trading timespan in the special day
	CEasyTimespan<time_t> getUntradingTimespan_Oneday(short year,short month,short day)
	{
		CEasyTimespan<time_t> TradingTimespan;
		time_t timezero=gettimezero(year,month,day);

		if(!isTradingday(year,month,day)||(int)m_Timespan_EachDay.getTimespans().size()==0)
			return CEasyTimespan<time_t>(timezero,timezero+24*3600);

		int off=0;
		std::map<int,Stru_TimespanInfo<int>>::const_iterator it=m_Timespan_EachDay.getTimespans().begin();
		for(;it!=m_Timespan_EachDay.getTimespans().end();it++)
		{
			if(it->first>off)
				TradingTimespan.insert((time_t)off+timezero,(time_t)(it->first)+timezero);
            off=it->second.end;
		}
		if(off<24*3600)
			TradingTimespan.insert((time_t)off+timezero,(time_t)(24*3600)+timezero);

		return TradingTimespan;
	}

	//get trading timespan in the special day
	CEasyTimespan<time_t> getTradingTimespan_Oneday(short year,short month,short day,int startseconds=0,int endseconds=24*3600)
	{
		if(!isTradingday(year,month,day)) return CEasyTimespan<time_t>();

		CEasyTimespan<time_t> TradingTimespan;
		time_t timezero=gettimezero(year,month,day);

		std::map<int,Stru_TimespanInfo<int>>::const_iterator it;
		for(it=m_Timespan_EachDay.getTimespans().begin();it!=m_Timespan_EachDay.getTimespans().end();it++)
		{
			int start=it->first;
			if(start<startseconds) start=startseconds;
			int end=it->second.end;
			if(end>endseconds) end=endseconds;
			if(start<end)
				TradingTimespan.insert((time_t)start+timezero,(time_t)end+timezero);
		}
		TradingTimespan.exclude(m_Timespan_Exclusion);

		return TradingTimespan;
	}

	//get trading timespan in a period
	CEasyTimespan<time_t> getTradingTimespan_InPeriod(time_t start,time_t end,int& rltWeekendDays)
	{
		rltWeekendDays=0;
		CEasyTimespan<time_t> WorkdayTimespan;
		if(m_bExcludeWeekend) WorkdayTimespan=CEasyTimespan<time_t>::getWorkdayTimespan(start,end,rltWeekendDays,m_blocaltzdst,m_tzdst);
		else WorkdayTimespan=CEasyTimespan<time_t>(start,end);

		CEasyTimespan<time_t> WorkhourTimespan=exclude_NonWorkingTime_Eachday(WorkdayTimespan);

		WorkhourTimespan.exclude(m_Timespan_Exclusion);
		WorkhourTimespan.normalize(true);

		return WorkhourTimespan;
	}

    //exclude non working time from time span.
    //Timespan_IntraDay: timespan intra day, special seconds intraday, value is among 0-24*3600
    //TimeType must be time_t
    CEasyTimespan<time_t> exclude_NonWorkingTime_Eachday(CEasyTimespan<time_t>& oriTimespan)
    {
		CEasyTimespan<time_t> rltTimespan;
		CEasyTimespan<time_t> SplittedDayTimespan=CEasyTimespan<time_t>::split_TimespanToEveryDay(oriTimespan,m_blocaltzdst,m_tzdst);
		std::map<time_t,Stru_TimespanInfo<time_t>>::const_iterator it;
    	for(it=SplittedDayTimespan.getTimespans().begin();it!=SplittedDayTimespan.getTimespans().end();it++)
    	{
    		short year,month,day;
    		int SecondsIntraday;
    		if(m_blocaltzdst)
    		{
    			CEasyTime tt(it->first,0);
    			tt.convert2fileds(&year,&month,&day);
    			SecondsIntraday=tt.getSecondsIntraday();
    		}
    		else
    		{
    			CEasyTime2 tt(it->first,0,m_tzdst);
    			tt.convert2fileds(&year,&month,&day);
    			SecondsIntraday=tt.getSecondsIntraday();
    		}

    		CEasyTimespan<time_t> TradingTimespan_today=getTradingTimespan_Oneday(year,month,day,SecondsIntraday,SecondsIntraday+(int)(it->second.end-it->first));
    		rltTimespan.insert(TradingTimespan_today);
    	}

    	return rltTimespan;
    }

    const Stru_TimezoneDst& getTimezoneDst(void) const
    {
    	return m_tzdst;
    }
    bool isExcludeWeekend(void) const
    {
    	return m_bExcludeWeekend;
    }
    bool isLocalTzDST(void) const
    {
    	return m_blocaltzdst;
    }

    const int getSeconds_EachDay(void) const
    {
    	return m_Timespan_EachDay.getTimeDuration();
    }

    void setTodayTimespan(short year,short month,short day)
    {
    	m_Timespan_Today=getTradingTimespan_Oneday(year,month,day);
    }
    const CEasyTimespan<time_t> getTodayTimespan(void) const
	{
    	return m_Timespan_Today;
	}
	void convert2fileds(time_t utc,short* prltyear,short* prltmonth=NULL,short* prltday=NULL,short* prlthour=NULL,short* prltminutes=NULL,short* prltsecs=NULL,short* prltweekday=NULL) const
	{
		if(m_blocaltzdst)
			CEasyTime(utc).convert2fileds(prltyear,prltmonth,prltday,prlthour,prltminutes,prltsecs,prltweekday);
		else
			CEasyTime2(utc,0,m_tzdst).convert2fileds(prltyear,prltmonth,prltday,prlthour,prltminutes,prltsecs,prltweekday);
	}
	int getSecondsIntraday(time_t utc) const
	{
		if(m_blocaltzdst) return CEasyTime(utc).getSecondsIntraday();
		else return CEasyTime2(utc,0,m_tzdst).getSecondsIntraday();
	}
	time_t gettimezero(short year,short month,short day) const
	{
		if(m_blocaltzdst) return CEasyTime(year,month,day,0,0,0).gettimeb().time;
		else return CEasyTime2(year,month,day,0,0,0,m_tzdst).gettimeb().time;
	}
	time_t getUTC_TodayTimezero(void) const
	{
		return m_UTC_Timezero;
	}





private:
    bool					m_blocaltzdst;					//whether local tzdst is used
	Stru_TimezoneDst 		m_tzdst;						//setting of timezone and dst(daylight saving time)

	bool 					m_bExcludeWeekend;
	CEasyTimespan<int> 		m_Timespan_EachDay;				//timespan intra day, special seconds intraday
	CEasyTimespan<time_t> 	m_Timespan_Exclusion;			//timespan that excluding from trading time. For example public holidays

	short					m_Year;
	short					m_Month;
	short					m_Day;
	time_t					m_UTC_Timezero;
	CEasyTimespan<time_t>	m_Timespan_Today;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif /* EASYTRADINGTIMESPAN_H_ */

