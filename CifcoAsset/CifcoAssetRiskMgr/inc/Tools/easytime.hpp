/*
 * easytime.hpp
 *
 *  Define customize struct timeb
 *
 *  Created on: Dec 1, 2012
 *      Author: root
 */

#ifndef EASYTIME_H_
#define EASYTIME_H_

#ifdef WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include <time.h>
#endif

#include <sys/types.h>
#include <sys/timeb.h>

#ifdef WIN32
#define int64_ET __int64
#define snprintf_ET sprintf_s
#else
#define int64_ET int64_t
#define snprintf_ET snprintf
#endif



class CEasyTime_base
{
public:
	//get cur time string formated to "yyyymmdd"
	static char* getcurrentlocalyyyymmdd(char* prlt,int rltsize)
	{
		time_t curtime=time(NULL);
		struct tm tmptm;
		memcpy((char*)&tmptm,(char*)localtime(&curtime),sizeof(tmptm));
		snprintf_ET(prlt,rltsize-1,"%04d%02d%02d",
					tmptm.tm_year+1900,tmptm.tm_mon+1,tmptm.tm_mday);
		return prlt;
	}
	static time_t mkgmtime(const struct tm *gmtm)
	{
		time_t itime;
		struct tm tmptm=*gmtm;
#ifdef WIN32
		tmptm.tm_isdst=0;					//informs _mkgmtime() that DST isn't in effect
		itime=_mkgmtime(&tmptm);
#else
		tmptm.tm_isdst=-1;					//means that mktime() should attempt to determine whether DST is in effect at the specified time
		itime=mktime(&tmptm);				//converts a broken-down time structure, expressed as local time, to calendar time representation
		itime-=timezone;					//After mktime() returned, the golbal variable timezone was set to seconds West of UTC
		if(tmptm.tm_isdst>0)				//After mktime() returned, tm_isdst indicates whethre DST is in effect, 0/1
			itime-=3600;
#endif
		return itime;
	}
	//judge whether the year is laap year(run nian)
	static bool isLeapYear(short year)
	{
		return ( year%400==0 || (year%4==0&&year%100!=0) ) ? true : false;
	}
	static short getDaysOfMonth(short year,short month)
	{
		if(month==1||month==3||month==5||month==7||month==8||month==10||month==12) return 31;
		if(month==4||month==6||month==9||month==11) return 30;
		if(isLeapYear(year)) return 29;
		return 28;
	}
	static void getSundayInfoOfMonth(short year,short month,
										short& rltDaysOfMonth,short& rltSundaysOfMonth,short& rltmdayOfFirstSunday,short& rltmdayOfLastSunday)
	{
		rltDaysOfMonth=getDaysOfMonth(year,month);

		struct tm tmptm;
		memset(&tmptm,0,sizeof(tmptm));
		tmptm.tm_year=year-1900;
		tmptm.tm_mon=month-1;
		tmptm.tm_mday=1;
		tmptm.tm_isdst=-1;
		mktime(&tmptm);

		getSundayInfoOfMonth2(rltDaysOfMonth,tmptm.tm_mday,tmptm.tm_wday,
								rltSundaysOfMonth,rltmdayOfFirstSunday,rltmdayOfLastSunday);
	}
	static void getSundayInfoOfMonth2(short DaysOfMonth,short mday_firstday,short wday_firstday,
										short& rltSundaysOfMonth,short& rltmdayOfFirstSunday,short& rltmdayOfLastSunday)
	{
		rltmdayOfFirstSunday=mday_firstday+(7-wday_firstday)%7;
		rltSundaysOfMonth=(DaysOfMonth-rltmdayOfFirstSunday+1)/7+((DaysOfMonth-rltmdayOfFirstSunday+1)%7==0?0:1);
		rltmdayOfLastSunday=rltmdayOfFirstSunday+(rltSundaysOfMonth-1)*7;
	}


public:
	virtual ~CEasyTime_base()
	{
	}

	virtual void clear(void)=0;
	virtual void settime(const struct timeb& r)=0;
	virtual void settime(time_t utc,unsigned short millitm)=0;
	virtual void settime(int year,int month,int day,int hour,int minutes,int secs)=0;
	virtual void setcurtime(void)=0;

	virtual struct timeb gettimeb(void) const=0;
	virtual time_t getutc(void) const=0;
	virtual unsigned short getmillitm(void) const=0;
	virtual int getSecondsIntraday(void) const=0;
	virtual const struct tm& gettm(void) const=0;

	virtual char* getstrtime(char* prlt,int rltsize) const=0;
	virtual char* getstrtime2(char* prlt,int rltsize) const=0;
	virtual char* getstryyyymmdd(char* prlt,int rltsize) const=0;
	virtual char* getstryyyy(char* prlt,int rltsize) const=0;
	virtual char* getstrshorttime(char* prlt,int rltsize) const=0;
	virtual int64_ET gettimediff(const struct timeb& timeblater) const=0;
	virtual int64_ET gettimediff(time_t utclater,unsigned short millitmlater) const=0;
	virtual int64_ET gettimediff2curtime(void) const=0;
	virtual void convert2fileds(short* prltyear,short* prltmonth=NULL,short* prltday=NULL,short* prlthour=NULL,short* prltminutes=NULL,short* prltsecs=NULL,short* prltweekday=NULL) const=0;

protected:
	struct timeb m_timeb;
	struct tm m_tm;
};


//define timezone and dst(daylight saving time)
struct Stru_TimezoneDst
{
public:
	Stru_TimezoneDst()
	{
		clear();
	}
	Stru_TimezoneDst(int Tz,short Usedst,
						short Startdst_month,short Startdst_sunday,short Startdst_hour,
						short Enddst_month,short Enddst_sunday,short Enddst_hour)
	{
	    tz=Tz;
	    usedst=Usedst;
	    startdst_month=Startdst_month;
	    startdst_sunday=Startdst_sunday;
	    startdst_hour=Startdst_hour;
	    enddst_month=Enddst_month;
	    enddst_sunday=Enddst_sunday;
	    enddst_hour=Enddst_hour;
	}
	Stru_TimezoneDst(const Stru_TimezoneDst& r)
	{
		*this=r;
	}
    bool operator== (const Stru_TimezoneDst& r) const
	{
    	if(tz!=r.tz||usedst!=r.usedst) return false;
    	if(!usedst) return true;
    	if(usedst&&(startdst_month!=r.startdst_month||startdst_sunday!=r.startdst_sunday||startdst_hour!=r.startdst_hour||
    				enddst_month!=r.enddst_month||enddst_sunday!=r.enddst_sunday||enddst_hour!=r.enddst_hour))
    		return false;
    	else return true;
	}
	void clear(void)
	{
#ifdef WIN32
		_tzset();
		tz=_timezone;
		usedst=_daylight!=0?1:0;
#else
		tzset();
		tz=timezone;
		usedst=daylight!=0?1:0;
#endif
	    startdst_month=0;
	    startdst_sunday=0;
	    startdst_hour=0;
	    enddst_month=0;
	    enddst_sunday=0;
	    enddst_hour=0;
	}

	//get utc of start/end dst in the year, it't standard UTC, without regard to DST
	bool getDst_IntraYear(short year,time_t& rltStartDst,time_t& rltEndDst) const
	{
		if(!usedst) return false;

		short DaysOfMonth,SundaysOfMonth,mday_firstSunday,mday_lastSunday;
		short dstday;

		CEasyTime_base::getSundayInfoOfMonth(year,startdst_month,
												DaysOfMonth,SundaysOfMonth,mday_firstSunday,mday_lastSunday);

		if(startdst_sunday>=1&&startdst_sunday<=5) dstday=mday_firstSunday+(startdst_sunday-1)*7;
		else if(startdst_sunday>=-5&&startdst_sunday<=-1) dstday=mday_lastSunday+(startdst_sunday+1)*7;
		else return false;

		struct tm tmptm;
		memset(&tmptm,0,sizeof(tmptm));
		tmptm.tm_year=year-1900;
		tmptm.tm_mon=startdst_month-1;
		tmptm.tm_mday=dstday;
		tmptm.tm_hour=startdst_hour;
		rltStartDst=CEasyTime_base::mkgmtime(&tmptm)-tz;	//don't wonder DST, we get standard UTC only


		CEasyTime_base::getSundayInfoOfMonth(year,enddst_month,
												DaysOfMonth,SundaysOfMonth,mday_firstSunday,mday_lastSunday);

		if(enddst_sunday>=1&&enddst_sunday<=5) dstday=mday_firstSunday+(enddst_sunday-1)*7;
		else if(enddst_sunday>=-5&&enddst_sunday<=-1) dstday=mday_lastSunday+(enddst_sunday+1)*7;
		else return false;

		memset(&tmptm,0,sizeof(tmptm));
		tmptm.tm_year=year-1900;
		tmptm.tm_mon=enddst_month-1;
		tmptm.tm_mday=dstday;
		tmptm.tm_hour=enddst_hour-1;		//for example, the end hour is 2:00, it means 1:00 of standard UTC(DST 2:00)
		if(tmptm.tm_hour<0)
		{
			tmptm.tm_mday--;
			tmptm.tm_hour=23;
		}
		rltEndDst=CEasyTime_base::mkgmtime(&tmptm)-tz;	//don't wonder DST, we get standard UTC only

		return true;
	}
	//get utc of start dst in the year
	bool getDst_IntraYear2(time_t refutc,time_t& rltStartDst,time_t& rltEndDst) const
	{
		struct tm *ptm=tzstdtm(refutc);
		return getDst_IntraYear(ptm->tm_year+1900,rltStartDst,rltEndDst);
	}

	//get time in timezone, without regard to DST
	struct tm* tzstdtm(time_t utc) const
	{
		utc-=tz;
		return gmtime(&utc);
	}
	//get struct tm of utc, in view of timezone and dst
	struct tm* tzdsttm(time_t utc) const
	{
		utc-=tz;
		if(isInDst(utc)) utc-=3600;
		return gmtime(&utc);
	}
	time_t mktzdsttime(const struct tm* ptzdsttm)
	{
		time_t utc=CEasyTime_base::mkgmtime(ptzdsttm);
		utc-=tz;
		if(isInDst2(utc,ptzdsttm->tm_year+1900))
			utc-=3600;
		return utc;
	}

	bool isUseDst(void) const
	{
		return usedst==0?false:true;
	}
	bool isInDst(time_t refutc) const
	{
		time_t rltStartDst,rltEndDst;
		bool brlt=getDst_IntraYear2(refutc,rltStartDst,rltEndDst);
		if(!brlt) return false;
		return refutc>=rltStartDst&&refutc<rltEndDst?true:false;
	}
	bool isInDst2(time_t refutc,short year) const
	{
		time_t rltStartDst,rltEndDst;
		bool brlt=getDst_IntraYear(year,rltStartDst,rltEndDst);
		if(!brlt) return false;
		return refutc>=rltStartDst&&refutc<rltEndDst?true:false;
	}


	int	tz;										//timezone of exchange. -28800(8h) for Chinese exchange
	short usedst;								//Whether dst is used in the exchange. 1:used; 0:unused

	short startdst_month;						//start month of dst. 1-12. for American 2012, it should be 3, means the third month, March
	short startdst_sunday;						//start sunday of dst. 1/2/3/4/5/-1/-2. for American 2012, it should be 2, means the second sunday
	short startdst_hour;						//start hour of dst. 0-23. refer to standard time. for American 2012, it should be 2.
	short enddst_month;							//end month of dst. 1-12. for American 2012, it should be 11, means the eleven month, November
	short enddst_sunday;						//end sunday of dst. 1/2/3/4/5/-1/-2. for American 2012, it should be 1, means the first sunday
	short enddst_hour;							//end hour of dst. 0-23. refer to standard time. for American 2012, it should be 2.
};


//define local EasyTime
class CEasyTime: public CEasyTime_base
{
public:
	CEasyTime()
	{
		ftime(&m_timeb);
		calculate_tm_internal();
	}
	CEasyTime(const CEasyTime& r)
	{
		*this=r;
	}
	CEasyTime(const struct timeb& r)
	{
		m_timeb=r;
		calculate_tm_internal();
	}
	CEasyTime(time_t utc,unsigned short millitm=0)
	{
		ftime(&m_timeb);
		m_timeb.time=utc;
		m_timeb.millitm=millitm;
		calculate_tm_internal();
	}
	//note: don't care dst in this construction
	CEasyTime(int year,int month,int day,int hour=0,int minutes=0,int secs=0)
	{
	    struct tm itm;

	    memset(&itm,0,sizeof(itm));
	    itm.tm_year=year-1900;
	    itm.tm_mon=month-1;
	    itm.tm_mday=day;
	    itm.tm_hour=hour;
	    itm.tm_min=minutes;
	    itm.tm_sec=secs;
	    itm.tm_isdst=-1;

	    ftime(&m_timeb);
	    m_timeb.time=mktime(&itm);
	    m_timeb.millitm=0;
		calculate_tm_internal();
	}
	virtual ~CEasyTime()
	{
	}

	CEasyTime& operator= (const CEasyTime& r)
	{
		m_timeb=r.m_timeb;
		m_tm=r.m_tm;
		return *this;
	}
    bool operator== (const CEasyTime& r) const
	{
    	return m_timeb.time==r.m_timeb.time&&m_timeb.millitm==r.m_timeb.millitm?true:false;
	}
    bool operator!= (const CEasyTime& r) const
	{
    	return m_timeb.time!=r.m_timeb.time||m_timeb.millitm!=r.m_timeb.millitm?true:false;
	}

	virtual void clear(void)
	{
		memset(&m_timeb,0,sizeof(m_timeb));
		calculate_tm_internal();
	}

	virtual void settime(const struct timeb& r)
	{
		m_timeb=r;
		calculate_tm_internal();
	}
	virtual void settime(time_t utc,unsigned short millitm)
	{
		m_timeb.time=utc;
		m_timeb.millitm=millitm;
		calculate_tm_internal();
	}
	virtual void settime(int year,int month,int day,int hour,int minutes,int secs)
	{
	    struct tm itm;
	    memset(&itm,0,sizeof(itm));
	    itm.tm_year=year-1900;
	    itm.tm_mon=month-1;
	    itm.tm_mday=day;
	    itm.tm_hour=hour;
	    itm.tm_min=minutes;
	    itm.tm_sec=secs;
	    itm.tm_isdst=-1;

	    memset(&m_timeb,0,sizeof(m_timeb));
		m_timeb.time=mktime(&itm);
		calculate_tm_internal();
	}
	virtual void setcurtime(void)
	{
		ftime(&m_timeb);
		calculate_tm_internal();
	}


	virtual struct timeb gettimeb(void) const
	{
		return m_timeb;
	}
	virtual time_t getutc(void) const
	{
		return m_timeb.time;
	}
	virtual time_t getutc_timezero(void) const
	{
		return m_timeb.time-m_tm.tm_hour*3600-m_tm.tm_min*60-m_tm.tm_sec;
	}
	virtual unsigned short getmillitm(void) const
	{
		return m_timeb.millitm;
	}
	virtual int getSecondsIntraday(void) const
	{
		return m_tm.tm_hour*3600+m_tm.tm_min*60+m_tm.tm_sec;
	}
	virtual const struct tm& gettm(void) const
	{
		return m_tm;
	}

	//get time string formated to "yyyy-mm-dd hh:mm:ss.mmm"
	virtual char* getstrtime(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
					m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
		return prlt;
	}
	//get time string formated to "yyyy-mm-dd hh:mm:ss"
	virtual char* getstrtime2(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%04d-%02d-%02d %02d:%02d:%02d",
					m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec);
		return prlt;
	}
	//get time string formated to "yyyymmdd"
	virtual char* getstryyyymmdd(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%04d%02d%02d",
					m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday);
		return prlt;
	}
	//get time string formated to "yyyy"
	virtual char* getstryyyy(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%04d",m_tm.tm_year+1900);
		return prlt;
	}
	//get time string formated to "hh:mm:ss.mmm"
	virtual char* getstrshorttime(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%02d:%02d:%02d.%03d",
					m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
		return prlt;
	}

	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_ET gettimediff(const CEasyTime& timelater) const
	{
		int64_ET t1,t2;
		t1=(int64_ET)m_timeb.time*(int64_ET)1000+(int64_ET)m_timeb.millitm;
		t2=(int64_ET)timelater.m_timeb.time*(int64_ET)1000+(int64_ET)timelater.m_timeb.millitm;
		return t2-t1;
	}
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_ET gettimediff(const struct timeb& timeblater) const
	{
		int64_ET t1,t2;
		t1=(int64_ET)m_timeb.time*(int64_ET)1000+(int64_ET)m_timeb.millitm;
		t2=(int64_ET)timeblater.time*(int64_ET)1000+(int64_ET)timeblater.millitm;
		return t2-t1;
	}
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_ET gettimediff(time_t utclater,unsigned short millitmlater) const
	{
		int64_ET t1,t2;
		t1=(int64_ET)m_timeb.time*(int64_ET)1000+(int64_ET)m_timeb.millitm;
		t2=(int64_ET)utclater*(int64_ET)1000+(int64_ET)millitmlater;
		return t2-t1;
	}
	//get time diff between m_timeb to curtime, in ms. timeDiff=curtime-m_timeb
	virtual int64_ET gettimediff2curtime(void) const
	{
		struct timeb curtime;
		ftime(&curtime);
		int64_ET t1,t2;
		t1=(int64_ET)m_timeb.time*(int64_ET)1000+(int64_ET)m_timeb.millitm;
		t2=(int64_ET)curtime.time*(int64_ET)1000+(int64_ET)curtime.millitm;
		return t2-t1;
	}
	virtual void convert2fileds(short* prltyear,short* prltmonth=NULL,short* prltday=NULL,short* prlthour=NULL,short* prltminutes=NULL,short* prltsecs=NULL,short* prltweekday=NULL) const
	{
	    if(prltyear) *prltyear=m_tm.tm_year+1900;
	    if(prltmonth) *prltmonth=m_tm.tm_mon+1;
	    if(prltday) *prltday=m_tm.tm_mday;
	    if(prlthour) *prlthour=m_tm.tm_hour;
	    if(prltminutes) *prltminutes=m_tm.tm_min;
	    if(prltsecs) *prltsecs=m_tm.tm_sec;
	    if(prltweekday) *prltweekday=m_tm.tm_wday;
	}

private:
	void calculate_tm_internal(void)
	{
		struct tm* ptm=localtime(&m_timeb.time);
		if(ptm) memcpy((char*)&m_tm,(char*)ptm,sizeof(m_tm));
		else memset(&m_tm,0,sizeof(m_tm));
	}

};

//define EasyTime for special timezone
class CEasyTime2: public CEasyTime_base
{
public:
	CEasyTime2(const Stru_TimezoneDst& tzdst)
	:m_tzdst(tzdst)
	{
		memset(&m_timeb,0,sizeof(m_timeb));
		calculate_tm_internal();
	}
	CEasyTime2(const CEasyTime2& r)
	{
		*this=r;
	}
	CEasyTime2(time_t utc,unsigned short millitm,const Stru_TimezoneDst& tzdst)
	:m_tzdst(tzdst)
	{
		memset(&m_timeb,0,sizeof(m_timeb));
		m_timeb.time=utc;
		m_timeb.millitm=millitm;
		calculate_tm_internal();
	}
	//note: don't care dst in this construction
	CEasyTime2(int year,int month,int day,int hour,int minutes,int secs,const Stru_TimezoneDst& tzdst)
	:m_tzdst(tzdst)
	{
	    struct tm itm;
	    memset(&itm,0,sizeof(itm));
	    itm.tm_year=year-1900;
	    itm.tm_mon=month-1;
	    itm.tm_mday=day;
	    itm.tm_hour=hour;
	    itm.tm_min=minutes;
	    itm.tm_sec=secs;

	    memset(&m_timeb,0,sizeof(m_timeb));
		m_timeb.time=m_tzdst.mktzdsttime(&itm);
		calculate_tm_internal();
	}
	virtual ~CEasyTime2()
	{
	}

	CEasyTime2& operator= (const CEasyTime2& r)
	{
		m_tzdst=r.m_tzdst;
		m_timeb=r.m_timeb;
		m_tm=r.m_tm;
		return *this;
	}

	virtual void clear(void)
	{
		memset(&m_timeb,0,sizeof(m_timeb));
		calculate_tm_internal();
	}

	virtual void settime(const struct timeb& r)
	{
		m_timeb=r;
		calculate_tm_internal();
	}
	virtual void settime(time_t utc,unsigned short millitm)
	{
		memset(&m_timeb,0,sizeof(m_timeb));
		m_timeb.time=utc;
		m_timeb.millitm=millitm;
		calculate_tm_internal();
	}
	virtual void settime(int year,int month,int day,int hour,int minutes,int secs)
	{
	    struct tm itm;
	    memset(&itm,0,sizeof(itm));
	    itm.tm_year=year-1900;
	    itm.tm_mon=month-1;
	    itm.tm_mday=day;
	    itm.tm_hour=hour;
	    itm.tm_min=minutes;
	    itm.tm_sec=secs;

	    memset(&m_timeb,0,sizeof(m_timeb));
		m_timeb.time=m_tzdst.mktzdsttime(&itm);
		calculate_tm_internal();
	}
	virtual void setcurtime(void)
	{
		ftime(&m_timeb);
		calculate_tm_internal();
	}


	const Stru_TimezoneDst& gettzdst(void) const
	{
		return m_tzdst;
	}

	virtual struct timeb gettimeb(void) const
	{
		return m_timeb;
	}
	virtual time_t getutc(void) const
	{
		return m_timeb.time;
	}
	virtual time_t getutc_timezero(void) const
	{
		return m_timeb.time-m_tm.tm_hour*3600-m_tm.tm_min*60-m_tm.tm_sec;
	}
	virtual unsigned short getmillitm(void) const
	{
		return m_timeb.millitm;
	}
	virtual int getSecondsIntraday(void) const
	{
		return m_tm.tm_hour*3600+m_tm.tm_min*60+m_tm.tm_sec;
	}
	virtual const struct tm& gettm(void) const
	{
		return m_tm;
	}

	//get time string formated to "yyyy-mm-dd hh:mm:ss.mmm"
	virtual char* getstrtime(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
					m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
		return prlt;
	}
	//get time string formated to "yyyy-mm-dd hh:mm:ss"
	virtual char* getstrtime2(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%04d-%02d-%02d %02d:%02d:%02d",
					m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec);
		return prlt;
	}
	//get time string formated to "yyyymmdd"
	virtual char* getstryyyymmdd(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%04d%02d%02d",
					m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday);
		return prlt;
	}
	//get time string formated to "yyyy"
	virtual char* getstryyyy(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%04d",m_tm.tm_year+1900);
		return prlt;
	}
	//get time string formated to "hh:mm:ss.mmm"
	virtual char* getstrshorttime(char* prlt,int rltsize) const
	{
		snprintf_ET(prlt,rltsize-1,"%02d:%02d:%02d.%03d",
					m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
		return prlt;
	}

	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_ET gettimediff(const CEasyTime2& timelater) const
	{
		int64_ET t1,t2;
		t1=(int64_ET)m_timeb.time*(int64_ET)1000+(int64_ET)m_timeb.millitm;
		t2=(int64_ET)timelater.m_timeb.time*(int64_ET)1000+(int64_ET)timelater.m_timeb.millitm;
		return t2-t1;
	}
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_ET gettimediff(const struct timeb& timeblater) const
	{
		int64_ET t1,t2;
		t1=(int64_ET)m_timeb.time*(int64_ET)1000+(int64_ET)m_timeb.millitm;
		t2=(int64_ET)timeblater.time*(int64_ET)1000+(int64_ET)timeblater.millitm;
		return t2-t1;
	}
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_ET gettimediff(time_t utclater,unsigned short millitmlater) const
	{
		int64_ET t1,t2;
		t1=(int64_ET)m_timeb.time*(int64_ET)1000+(int64_ET)m_timeb.millitm;
		t2=(int64_ET)utclater*(int64_ET)1000+(int64_ET)millitmlater;
		return t2-t1;
	}
	//get time diff between m_timeb to curtime, in ms. timeDiff=curtime-m_timeb
	virtual int64_ET gettimediff2curtime(void) const
	{
		struct timeb curtime;
		ftime(&curtime);
		int64_ET t1,t2;
		t1=(int64_ET)m_timeb.time*(int64_ET)1000+(int64_ET)m_timeb.millitm;
		t2=(int64_ET)curtime.time*(int64_ET)1000+(int64_ET)curtime.millitm;
		return t2-t1;
	}
	//convert utc to fields
	virtual void convert2fileds(short* prltyear,short* prltmonth=NULL,short* prltday=NULL,short* prlthour=NULL,short* prltminutes=NULL,short* prltsecs=NULL,short* prltweekday=NULL) const
	{
	    if(prltyear) *prltyear=m_tm.tm_year+1900;
	    if(prltmonth) *prltmonth=m_tm.tm_mon+1;
	    if(prltday) *prltday=m_tm.tm_mday;
	    if(prlthour) *prlthour=m_tm.tm_hour;
	    if(prltminutes) *prltminutes=m_tm.tm_min;
	    if(prltsecs) *prltsecs=m_tm.tm_sec;
	    if(prltweekday) *prltweekday=m_tm.tm_wday;
	}

private:
	void calculate_tm_internal(void)
	{
		struct tm* ptm=m_tzdst.tzdsttm(m_timeb.time);
		if(ptm) memcpy((char*)&m_tm,(char*)ptm,sizeof(m_tm));
		else memset(&m_tm,0,sizeof(m_tm));
	}

private:
	Stru_TimezoneDst m_tzdst;	//define timezone and dst setting(daylight saving time)
};

#undef int64_ET
#undef snprintf_ET

#endif /* EASYTIME_H_ */
