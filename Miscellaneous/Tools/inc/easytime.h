/*
 * easytime.h
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
#endif

#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <string.h>
#include <string>
using std::string;

#ifdef WIN32
#define int64_EasyTime __int64
#define snprintf_EasyTime sprintf_s
#else
#define int64_EasyTime int64_t
#define snprintf_EasyTime snprintf
#endif


#ifdef WIN32
#pragma managed(push,off)
#endif 


class CEasyTime_base
{
public:
	//get cur time string formated to "yyyymmdd"
	static char* getcurrentlocalyyyymmdd(char* prlt,int rltsize);
	static time_t mkgmtime(const struct tm *gmtm);
	//judge whether the year is laap year(run nian)
	static bool isLeapYear(short year);
	static short getDaysOfMonth(short year,short month);
	static void getSundayInfoOfMonth(short year,short month,
			short& rltDaysOfMonth,short& rltSundaysOfMonth,short& rltmdayOfFirstSunday,short& rltmdayOfLastSunday);
	static void getSundayInfoOfMonth2(short DaysOfMonth,short mday_firstday,short wday_firstday,
			short& rltSundaysOfMonth,short& rltmdayOfFirstSunday,short& rltmdayOfLastSunday);
	//convert utc to local tm
	static inline bool Convert_tm_from_utc(time_t utc,struct tm& itm)
	{
		struct tm* ptm=localtime(&utc);
		if(ptm) { memcpy((char*)&itm,(char*)ptm,sizeof(itm));  return true; }
		else { memset(&itm,0,sizeof(itm));  return false; }
	}
	static inline bool Convert_fileds_from_utc(time_t utc,short* prltyear=NULL,short* prltmonth=NULL,short* prltday=NULL,short* prlthour=NULL,short* prltminutes=NULL,short* prltsecs=NULL,short* prltweekday=NULL)
	{
		struct tm* ptm=localtime(&utc);
		if(!ptm) return false;
		if(prltyear) *prltyear=ptm->tm_year+1900;
		if(prltmonth) *prltmonth=ptm->tm_mon+1;
		if(prltday) *prltday=ptm->tm_mday;
		if(prlthour) *prlthour=ptm->tm_hour;
		if(prltminutes) *prltminutes=ptm->tm_min;
		if(prltsecs) *prltsecs=ptm->tm_sec;
		if(prltweekday) *prltweekday=ptm->tm_wday;
		return true; 
	}
	static inline time_t Convert_utc_from_datetime(int year,int month,int day,int hour,int minutes,int secs)
	{
		struct tm itm={0};
		itm.tm_year=year-1900;
		itm.tm_mon=month-1;
		itm.tm_mday=day;
		itm.tm_hour=hour;
		itm.tm_min=minutes;
		itm.tm_sec=secs;
		itm.tm_isdst=-1;
		return mktime(&itm);
	}
	static inline int getSecondsIntraday2(time_t utc)
	{
		struct tm* ptm=localtime(&utc);
		if(!ptm) return 0;
		return ptm->tm_hour*3600+ptm->tm_min*60+ptm->tm_sec;
	}
	//返回1970.1.1以来的月数
	static inline int getTotalMonthsFromUTC0(time_t utc)
	{
		struct tm* ptm=localtime(&utc);
		if(!ptm) return 0;
		return (ptm->tm_year+1900-1970)*12+ptm->tm_mon;
	}
	//将hh:mm字符串转换成秒数
	static inline int Convert_hh_mm_to_mins(const char* hhmm)
	{
		int hh=0,mm=0;
		sscanf_s(hhmm,"%d:%d",&hh,&mm);
		return hh*60+mm;
	}




public:
	virtual ~CEasyTime_base();

	virtual void clear(void)=0;
	virtual void settime(const struct timeb& r)=0;
	virtual void settime(time_t utc,unsigned short millitm)=0;
	virtual void settime(int year,int month,int day,int hour,int minutes,int secs,int millitm=0)=0;
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
	virtual int64_EasyTime gettimediff(const struct timeb& timeblater) const=0;
	virtual int64_EasyTime gettimediff(time_t utclater,unsigned short millitmlater) const=0;
	virtual int64_EasyTime gettimediff2curtime(void) const=0;
	virtual void convert2fileds(short* prltyear,short* prltmonth=NULL,short* prltday=NULL,short* prlthour=NULL,short* prltminutes=NULL,short* prltsecs=NULL,short* prltweekday=NULL) const;
	virtual int getyear(void) const;
	virtual int getmon(void) const;
	virtual int getday(void) const;
	virtual int getweekday(void) const;
	virtual int gethour(void) const;
	virtual int getmin(void) const;
	virtual int getsec(void) const;

protected:
	struct timeb m_timeb;
	struct tm m_tm;
};


//define timezone and dst(daylight saving time)
struct Stru_TimezoneDst
{
public:
	Stru_TimezoneDst();
	Stru_TimezoneDst(int Tz,short Usedst,
						short Startdst_month,short Startdst_sunday,short Startdst_hour,
						short Enddst_month,short Enddst_sunday,short Enddst_hour);
	Stru_TimezoneDst(const Stru_TimezoneDst& r);
    bool operator== (const Stru_TimezoneDst& r) const;
	void clear(void);
	//get utc of start/end dst in the year, it't standard UTC, without regard to DST
	bool getDst_IntraYear(short year,time_t& rltStartDst,time_t& rltEndDst) const;
	//get utc of start dst in the year
	bool getDst_IntraYear2(time_t refutc,time_t& rltStartDst,time_t& rltEndDst) const;
	//get time in timezone, without regard to DST
	struct tm* tzstdtm(time_t utc) const;
	//get struct tm of utc, in view of timezone and dst
	struct tm* tzdsttm(time_t utc) const;
	time_t mktzdsttime(const struct tm* ptzdsttm);
	bool isUseDst(void) const;
	bool isInDst(time_t refutc) const;
	bool isInDst2(time_t refutc,short year) const;

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
	CEasyTime();
	CEasyTime(const CEasyTime& r);
	CEasyTime(const struct timeb& r);
	CEasyTime(time_t utc,unsigned short millitm=0);
	//note: don't care dst in this construction
	CEasyTime(int year,int month,int day,int hour=0,int minutes=0,int secs=0);
	CEasyTime(const char* pyyyymmdd,const char* phhmmdd,unsigned short millitm=0);//yyyymmdd  hh:mm:ss.millitm
	virtual ~CEasyTime();

	CEasyTime& operator= (const CEasyTime& r);
    bool operator== (const CEasyTime& r) const;
    bool operator!= (const CEasyTime& r) const;

	virtual void clear(void);
	virtual void settime(const struct timeb& r);
	virtual void settime(time_t utc,unsigned short millitm);
	virtual void settime(int year,int month,int day,int hour,int minutes,int secs,int millitm=0);
	virtual void settime(const char* pyyyymmdd);//yyyymmdd 00:00:00.000
	virtual void settime(const char* pyyyymmdd,const char* phhmmdd,unsigned short millitm=0);//yyyymmdd  hh:mm:ss.millitm
	virtual void settime_yyyymmddhhmm(const char* pyyyymmddhhmm);
	virtual void setcurtime(void);

	virtual struct timeb gettimeb(void) const;
	virtual time_t getutc(void) const;
	virtual time_t getutc_timezero(void) const;
	virtual time_t getutc_timezero_firstdayofmonth(void) const;
	virtual time_t getutc_timezero_firstdayofnextmonth(void) const;
	virtual unsigned short getmillitm(void) const;
	virtual int getSecondsIntraday(void) const;
	virtual const struct tm& gettm(void) const;
	//get time string formated to "yyyy-mm-dd hh:mm:ss.mmm"
	virtual char* getstrtime(char* prlt,int rltsize) const;
	virtual string getstrtime(void) const;
	//get time string formated to "yyyy-mm-dd hh:mm:ss"
	virtual char* getstrtime2(char* prlt,int rltsize) const;
	virtual string getstrtime2(void) const;
	//get time string formated to "yyyymmdd"
	virtual char* getstryyyymmdd(char* prlt,int rltsize) const;
	virtual string getstryyyymmdd(void) const;
	//get time string formated to "yyyy-mm"
	virtual char* getstryyyymm(char* prlt,int rltsize) const;
	virtual string getstryyyymm(void) const;
	//get time string formated to "yyyy"
	virtual char* getstryyyy(char* prlt,int rltsize) const;
	virtual string getstryyyy(void) const;
	//get time string formated to "hh:mm:ss.mmm"
	virtual char* getstrshorttime(char* prlt,int rltsize) const;
	virtual string getstrshorttime(void) const;
	//get time string formated to "hh:mm:ss"
	virtual char* getstrshorttime2(char* prlt,int rltsize) const;
	virtual string getstrshorttime2(void) const;
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_EasyTime gettimediff(const CEasyTime& timelater) const;
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_EasyTime gettimediff(const struct timeb& timeblater) const;
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_EasyTime gettimediff(time_t utclater,unsigned short millitmlater) const;
	//get time diff between m_timeb to curtime, in ms. timeDiff=curtime-m_timeb
	virtual int64_EasyTime gettimediff2curtime(void) const;
    //get time diff in seconds formated to "-h-m-s"
	virtual string gettimediff2(const CEasyTime& timelater) const;

private:
	void calculate_tm_internal(void);
};

//define EasyTime for special timezone
class CEasyTime2: public CEasyTime_base
{
public:
	CEasyTime2(const Stru_TimezoneDst& tzdst);
	CEasyTime2(const CEasyTime2& r);
	CEasyTime2(time_t utc,unsigned short millitm,const Stru_TimezoneDst& tzdst);
	//note: don't care dst in this construction
	CEasyTime2(int year,int month,int day,int hour,int minutes,int secs,const Stru_TimezoneDst& tzdst);
	virtual ~CEasyTime2();

	CEasyTime2& operator= (const CEasyTime2& r);

	virtual void clear(void);

	virtual void settime(const struct timeb& r);
	virtual void settime(time_t utc,unsigned short millitm);
	virtual void settime(int year,int month,int day,int hour,int minutes,int secs,int millitm=0);
	virtual void setcurtime(void);

	const Stru_TimezoneDst& gettzdst(void) const;
	virtual struct timeb gettimeb(void) const;
	virtual time_t getutc(void) const;
	virtual time_t getutc_timezero(void) const;
	virtual unsigned short getmillitm(void) const;
	virtual int getSecondsIntraday(void) const;
	virtual const struct tm& gettm(void) const;

	//get time string formated to "yyyy-mm-dd hh:mm:ss.mmm"
	virtual char* getstrtime(char* prlt,int rltsize) const;
	//get time string formated to "yyyy-mm-dd hh:mm:ss"
	virtual char* getstrtime2(char* prlt,int rltsize) const;
	//get time string formated to "yyyymmdd"
	virtual char* getstryyyymmdd(char* prlt,int rltsize) const;
	//get time string formated to "yyyy"
	virtual char* getstryyyy(char* prlt,int rltsize) const;
	//get time string formated to "hh:mm:ss.mmm"
	virtual char* getstrshorttime(char* prlt,int rltsize) const;

	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_EasyTime gettimediff(const CEasyTime2& timelater) const;
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_EasyTime gettimediff(const struct timeb& timeblater) const;
	//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
	virtual int64_EasyTime gettimediff(time_t utclater,unsigned short millitmlater) const;
	//get time diff between m_timeb to curtime, in ms. timeDiff=curtime-m_timeb
	virtual int64_EasyTime gettimediff2curtime(void) const;

private:
	void calculate_tm_internal(void);

private:
	Stru_TimezoneDst m_tzdst;	//define timezone and dst setting(daylight saving time)
};


#ifdef WIN32
#pragma managed(pop)
#endif 


#undef int64_EasyTime
#undef snprintf_EasyTime


#endif /* EASYTIME_H_ */
