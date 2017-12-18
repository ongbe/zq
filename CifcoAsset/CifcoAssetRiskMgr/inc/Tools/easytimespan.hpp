/*
 * easytimespan.h
 *
 *  Define customize time span
 *
 *  Created on: Dec 1, 2012
 *      Author: root
 */

#ifndef EASYTIMESPAN_H_
#define EASYTIMESPAN_H_

#ifdef WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include <time.h>
#endif

#include <sys/types.h>
#include <sys/timeb.h>

#include <map>
using namespace std;

#include "easytime.hpp"


template <typename TimeType>
struct Stru_TimespanInfo
{
	Stru_TimespanInfo(TimeType istart=0,TimeType iend=0,int ioffset=0)
	{
		start=istart;
		end=iend;
		offset=ioffset;
	}
	TimeType start;
	TimeType end;
	int offset;
};


//TimeType can be time_t(standard utc time) or int(seconds intra day)
template <typename TimeType>
class CEasyTimespan
{
public:
	CEasyTimespan()
	{
	}
	CEasyTimespan(TimeType start,TimeType end)
	{
		m_mapTimespans[start]=Stru_TimespanInfo<TimeType>(start,end);
        normalize_internal();
	}
	CEasyTimespan(const map<TimeType,Stru_TimespanInfo<TimeType>>& timespans)
	{
		m_mapTimespans=timespans;
        normalize_internal();
	}
	CEasyTimespan(const CEasyTimespan& r)
	{
		*this=r;
	}
	virtual ~CEasyTimespan() {}

	CEasyTimespan& operator= (const CEasyTimespan& r)
	{
		m_mapTimespans=r.m_mapTimespans;
		m_mapOffset=r.m_mapOffset;
		return *this;
	}

	void clear(void)
	{
		m_mapTimespans.clear();
		m_mapOffset.clear();
	}

    //Whether the specified utc is in period
    bool isInTimespans(TimeType itime) const
    {
    	if((int)m_mapTimespans.size()==0) return false;
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it=m_mapTimespans.lower_bound(itime);
    	if(it!=m_mapTimespans.begin()&&(it==m_mapTimespans.end()||it->first>itime))
        	it--;
    	if(it!=m_mapTimespans.end()&&itime>=it->first&&itime<it->second.end) return true;
        return false;
    }
    //Whether the specified utc is in period
    bool isTimespanIncluding(TimeType start,TimeType end) const
    {
        if(start>=end) return false;
    	if((int)m_mapTimespans.size()==0) return false;

    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it=m_mapTimespans.lower_bound(start);
    	if(it!=m_mapTimespans.begin()&&(it==m_mapTimespans.end()||it->first>start))
        	it--;
    	if(it!=m_mapTimespans.end()&&start>=it->first&&end<=it->second.end) return true;
        return false;
    }

    const map<TimeType,Stru_TimespanInfo<TimeType>>& getTimespans(void) const
    {
        return m_mapTimespans;
    }
    const map<int,TimeType>& getOffsets(void) const
    {
    	return m_mapOffset;
    }

    Stru_TimespanInfo<TimeType> getFirstTimespan(void) const
	{
    	if(m_mapTimespans.size()==0) return Stru_TimespanInfo<TimeType>((TimeType)0,(TimeType)0);
    	return m_mapTimespans.begin()->second;
	}
    Stru_TimespanInfo<TimeType> getLastTimespan(void) const
	{
    	int isize=m_mapTimespans.size();
    	if(isize==0) return Stru_TimespanInfo<TimeType>((TimeType)0,(TimeType)0);
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it=m_mapTimespans.end();
   		it--;
    	return it->second;
	}
    TimeType getTimeByOffset(int offset) const
    {
    	typename map<int,TimeType>::const_iterator it=m_mapOffset.lower_bound(offset);
        if(it!=m_mapOffset.begin()&&(it==m_mapOffset.end()||it->first>offset))
            it--;
        if(it==m_mapOffset.end()||offset<it->first) return (TimeType)0;

        typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it2=m_mapTimespans.find(it->second);
        if(it2==m_mapTimespans.end()) return (TimeType)0;
        int deltaoff=offset-it->first;
        if(deltaoff>(int)(it2->second.end-it2->first)) return (TimeType)0;
        return it2->first+(TimeType)deltaoff;
    }
    int getOffsetByTime(TimeType itime) const
    {
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it=m_mapTimespans.lower_bound(itime);
        if(it!=m_mapTimespans.begin()&&(it==m_mapTimespans.end()||it->first>itime))
            it--;
        if(it==m_mapTimespans.end()||itime<it->first) return -1;

        TimeType deltatime=itime-it->first;
        if(deltatime>it->second.end-it->first) return -1;
        return it->second.offset+(int)deltatime;
    }

    //Calculate total seconds
    int getTimeDuration(void) const
    {
    	int isize=m_mapTimespans.size();
    	if(isize==0) return (TimeType)0;
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it=m_mapTimespans.end();
    	it--;
        return it->second.offset+(int)(it->second.end-it->second.start);
    }

    //Exclude the specified period2 from this object
    void exclude(const CEasyTimespan<TimeType>& Exclusions)
    {
        if((int)Exclusions.getTimespans().size()==0) return;
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it;
        for(it=Exclusions.getTimespans().begin();it!=Exclusions.getTimespans().end();it++)
        	exclude_internel(it->first,it->second.end);
        normalize_internal();
    }
    void exclude(const map<TimeType,Stru_TimespanInfo<TimeType>>& timespans)
    {
        if((int)timespans.size()==0) return;
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it;
        for(it=timespans.begin();it!=timespans.end();it++)
        	exclude_internel(it->first,it->second.end);
        normalize_internal();
    }
    //Exclude the specified period from this object
    void exclude(TimeType exclusion_start,TimeType exclusion_end)
    {
        if(exclusion_start>=exclusion_end) return;
    	exclude_internel(exclusion_start,exclusion_end);
        normalize_internal();
    }


    //insert the specified timespan to this object
    void insert(CEasyTimespan<TimeType>& ts,bool trycombine=true)
    {
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it;
    	for(it=ts.m_mapTimespans.begin();it!=ts.m_mapTimespans.end();it++)
    		insert_internel(it->first,it->second.end);
        normalize_internal(trycombine);
    }
    //insert the specified period to this object
    void insert(TimeType start,TimeType end,bool trycombine=true)
    {
    	insert_internel(start,end);
        normalize_internal(trycombine);
    }


    //calculate periods of weekend in time duration. and return weekend day
    //This function must be used if TimeType is time_t
    //If blocaltzdst==true, refer to local tzdst, otherwise refer to the special tzdst
	static CEasyTimespan<time_t> getWeekendTimespan(time_t startutc,time_t endutc,int& rltWeekendDays,
													bool blocaltzdst=true,const Stru_TimezoneDst& tzdst=Stru_TimezoneDst())
    {
        rltWeekendDays=0;

        CEasyTimespan<time_t> WeekendTimespan;
        short year,month,day,hour,min,sec,wday;
        time_t utc_Monday;

        if(blocaltzdst) CEasyTime(startutc,0).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
        else CEasyTime2(startutc,0,tzdst).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
        time_t utc_Timezero=startutc-(hour*3600+min*60+sec);

	    if(wday==6)
	    {
            //if startutc is Saturday
            utc_Monday=utc_Timezero+2*24*3600;
            WeekendTimespan.insert(startutc,(utc_Monday<=endutc?utc_Monday:endutc));
            rltWeekendDays += utc_Monday<=endutc ? 2 : (endutc-utc_Timezero)/(24*3600);
	    }
	    if(wday==0)
	    {
            //if startutc is Sunday
            utc_Monday=utc_Timezero+24*3600;
            WeekendTimespan.insert(startutc,(utc_Monday<=endutc?utc_Monday:endutc));
            rltWeekendDays += 1;
	    }
        else
        {
            //if startutc is Monday to Friday
            utc_Monday=utc_Timezero-(wday-1)*24*3600;
        }

	    while(1)
        {
            time_t utc_NextMonday=utc_Monday+7*24*3600;
		    time_t utc_NextSaturday=utc_Monday+5*24*3600;

		    if(utc_NextSaturday>=endutc)
			    break;
		    else if(utc_NextMonday>=endutc)
		    {
                WeekendTimespan.insert(utc_NextSaturday,endutc);
			    rltWeekendDays+=(endutc-utc_NextSaturday)/(24*3600);
			    break;
		    }
		    else
		    {
                WeekendTimespan.insert(utc_NextSaturday,utc_NextMonday);
			    rltWeekendDays+=2;
			    utc_Monday=utc_NextMonday;
		    }
        }
        WeekendTimespan.normalize_internal();
        return WeekendTimespan;
    }

	//calculate periods of worktime in time duration. and return workday of week. worktime is from Monday to Friday
    //This function must be used if TimeType is time_t
	static CEasyTimespan<time_t> getWorkdayTimespan(time_t startutc,time_t endutc,int& rltWeekendDays,
														bool blocaltzdst=true,const Stru_TimezoneDst& tzdst=Stru_TimezoneDst())
    {
        rltWeekendDays=0;

        CEasyTimespan<time_t> WorkdayTimespan;
        short year,month,day,hour,min,sec,wday;
        time_t utc_Monday;

        if(blocaltzdst) CEasyTime(startutc,0).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
        else CEasyTime2(startutc,0,tzdst).convert2fileds(&year,&month,&day,&hour,&min,&sec,&wday);
        time_t utc_Timezero=startutc-(hour*3600+min*60+sec);

	    if(wday==6)
	    {
            //if startutc is Saturday
            utc_Monday=utc_Timezero+2*24*3600;
            rltWeekendDays += utc_Monday<=endutc ? 2 : (int)(endutc-utc_Timezero)/(24*3600);
	    }
	    if(wday==0)
	    {
            //if startutc is Sunday
            utc_Monday=utc_Timezero+24*3600;
            rltWeekendDays += 1;
	    }
        else
        {
            //if startutc is Monday to Friday
            utc_Monday=utc_Timezero-(wday-1)*24*3600;
        }

	    while(1)
        {
            time_t utc_NextMonday=utc_Monday+7*24*3600;
		    time_t utc_NextSaturday=utc_Monday+5*24*3600;

		    if(utc_NextSaturday>=endutc)
            {
		    	WorkdayTimespan.insert((utc_Monday>=startutc?utc_Monday:startutc),endutc);
			    break;
            }
		    else if(utc_NextMonday>=endutc)
		    {
		    	WorkdayTimespan.insert((utc_Monday>=startutc?utc_Monday:startutc),utc_NextSaturday);
			    rltWeekendDays+=(int)(endutc-utc_NextSaturday)/(24*3600);
			    break;
		    }
		    else
		    {
		    	WorkdayTimespan.insert((utc_Monday>=startutc?utc_Monday:startutc),utc_NextSaturday);
			    rltWeekendDays+=2;
			    utc_Monday=utc_NextMonday;
		    }
        }
	    WorkdayTimespan.normalize_internal();
        return WorkdayTimespan;
    }

	//split timespan to every year
    //If blocaltzdst==true, refer to local tzdst, otherwise refer to the special tzdst
	static CEasyTimespan<time_t> split_TimespanToEveryYear(CEasyTimespan<time_t>& oriTimespan,
															bool blocaltzdst=true,const Stru_TimezoneDst& tzdst=Stru_TimezoneDst())
	{
		CEasyTimespan<time_t> rltTimespan;
		typename map<time_t,Stru_TimespanInfo<time_t>>::const_iterator it;
    	for(it=oriTimespan.m_mapTimespans.begin();it!=oriTimespan.m_mapTimespans.end();it++)
    	{
    		short year;
    		if(blocaltzdst) CEasyTime(it->first).convert2fileds(&year);
    		else CEasyTime2(it->first,0,tzdst).convert2fileds(&year);

    		time_t inserttime=it->first;
    		while(inserttime<it->second.end)
    		{
        		time_t endtime;
        		if(blocaltzdst) endtime=CEasyTime(year+1,1,1,0,0,0).gettimeb().time;
        		else endtime=CEasyTime2(year+1,1,1,0,0,0,tzdst).gettimeb().time;
        		if(endtime>it->second.end) endtime=it->second.end;

        		rltTimespan.insert(inserttime,endtime,false);
        		inserttime=endtime;
    		}
    	}
    	return rltTimespan;
	}

	//split timespan to every day
    //If blocaltzdst==true, refer to local tzdst, otherwise refer to the special tzdst
	static CEasyTimespan<time_t> split_TimespanToEveryDay(CEasyTimespan<time_t>& oriTimespan,
															bool blocaltzdst=true,const Stru_TimezoneDst& tzdst=Stru_TimezoneDst())
	{
		CEasyTimespan<time_t> rltTimespan;
		typename map<TimeType,Stru_TimespanInfo<TimeType>>::const_iterator it;
    	for(it=oriTimespan.m_mapTimespans.begin();it!=oriTimespan.m_mapTimespans.end();it++)
    	{
    		time_t inserttime=it->first;

    		int firstsecs;
    		if(blocaltzdst) firstsecs=CEasyTime(it->first).getSecondsIntraday();
    		else firstsecs=CEasyTime2(it->first,0,tzdst).getSecondsIntraday();
    		int insertsecs=24*3600-firstsecs;

    		while(inserttime<it->second.end)
    		{
    			time_t endtime=inserttime+insertsecs;
        		if(endtime>it->second.end) endtime=it->second.end;
    			rltTimespan.insert(inserttime,endtime,false);
    			inserttime=endtime;
    			insertsecs=24*3600;
    		}
    	}
    	return rltTimespan;
	}

    void normalize(bool trycombine=true)
    {
    	normalize_internal(trycombine);
    }

private:
    void normalize_internal(bool trycombine=true)
    {
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::iterator it;
        for(it=m_mapTimespans.begin();it!=m_mapTimespans.end();)
        {
            if(it->first>=it->second.end) it=m_mapTimespans.erase(it);
            else it++;
        }
        if(trycombine)
        {
            for(it=m_mapTimespans.begin();it!=m_mapTimespans.end();)
            {
        	    typename map<TimeType,Stru_TimespanInfo<TimeType>>::iterator it2=it;
                it2++;
                if(it2==m_mapTimespans.end()) break;
                if(it->second.end>=it2->first)
                {
                    it->second.end=it2->first;
                    m_mapTimespans.erase(it2);
                }
                else it++;
            }
        }
        int total=0;
        m_mapOffset.clear();
        for(it=m_mapTimespans.begin();it!=m_mapTimespans.end();it++)
        {
        	it->second.offset=total;
        	m_mapOffset[total]=it->first;
        	total+=(int)(it->second.end-it->first);
        }
    }
    //Exclude the specified period from this object
    void exclude_internel(TimeType exclusion_start,TimeType exclusion_end)
    {
        if(exclusion_start>=exclusion_end) return;
        typename map<TimeType,Stru_TimespanInfo<TimeType>>::iterator it;
        for(it=m_mapTimespans.begin();it!=m_mapTimespans.end();)
        {
            if(exclusion_end<=it->first)
            {
                break;
            }
            else if(exclusion_start<=it->first && exclusion_end>=it->second.end)
            {
                it=m_mapTimespans.erase(it);
            }
            else if(exclusion_start<=it->first && exclusion_end>it->first && exclusion_end<it->second.end)
            {
                TimeType oriend=it->second.end;
                m_mapTimespans.erase(it);
                m_mapTimespans[exclusion_end]=Stru_TimespanInfo<TimeType>(exclusion_end,oriend);
                break;
            }
            else if(exclusion_start>it->first && exclusion_end<it->second.end)
            {
                TimeType oriend=it->second.end;
                it->second.end=exclusion_start;
                m_mapTimespans[exclusion_end]=Stru_TimespanInfo<TimeType>(exclusion_end,oriend);
                break;
            }
            else if(exclusion_start>it->first && exclusion_start<it->second.end && exclusion_end>=it->second.end)
            {
                it->second.end=exclusion_start;
                it++;
            }
            else it++;
        }
    }
    //insert the specified period to this object
    void insert_internel(TimeType start,TimeType end)
    {
    	typename map<TimeType,Stru_TimespanInfo<TimeType>>::iterator it;
        for(it=m_mapTimespans.begin();it!=m_mapTimespans.end();)
        {
            if(start>=end)
            	break;
            else if(end<=it->first)
            {
            	m_mapTimespans[start]=Stru_TimespanInfo<TimeType>(start,end);
            	start=end;
                break;
            }
            else if(start<=it->first && end>=it->second.end)
            {
                TimeType oriend=it->second.end;
                m_mapTimespans.erase(it);
                m_mapTimespans[start]=Stru_TimespanInfo<TimeType>(start,oriend);
                it=m_mapTimespans.find(start);
                start=oriend;
            }
            else if(start<=it->first && end>it->first && end<it->second.end)
            {
                TimeType oriend=it->second.end;
                m_mapTimespans.erase(it);
                m_mapTimespans[start]=Stru_TimespanInfo<TimeType>(start,oriend);
                start=oriend;
                break;
            }
            else if(start>=it->first && end<=it->second.end)
            {
            	start=it->second.end;
                break;
            }
            else if(start>=it->first && start<it->second.end && end>it->second.end)
            {
                start=it->second.end;
                it++;
            }
            else it++;
        }
        if(start<end)
        	m_mapTimespans[start]=Stru_TimespanInfo<TimeType>(start,end);
    }

private:

    map<TimeType,Stru_TimespanInfo<TimeType>> m_mapTimespans;
    map<int,TimeType> m_mapOffset;
};




#endif /* EASYTIMESPAN_H_ */
