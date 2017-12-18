/*
 * easytime.cpp
 *
 *  Define customize struct timeb
 *
 *  Created on: Dec 1, 2012
 *      Author: root
 */


#include "easytime.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
#define int64_EasyTime __int64
#define snprintf_EasyTime sprintf_s
#else
#define int64_EasyTime int64_t
#define snprintf_EasyTime snprintf
#endif

#ifdef WIN32
#pragma unmanaged
#endif 


//get cur time string formated to "yyyymmdd"
char* CEasyTime_base::getcurrentlocalyyyymmdd(char* prlt,int rltsize)
{
	time_t curtime=time(NULL);
	struct tm tmptm;
	memcpy((char*)&tmptm,(char*)localtime(&curtime),sizeof(tmptm));
	snprintf_EasyTime(prlt,rltsize-1,"%04d%02d%02d",
				tmptm.tm_year+1900,tmptm.tm_mon+1,tmptm.tm_mday);
	return prlt;
}
time_t CEasyTime_base::mkgmtime(const struct tm *gmtm)
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
bool CEasyTime_base::isLeapYear(short year)
{
	return ( year%400==0 || (year%4==0&&year%100!=0) ) ? true : false;
}
short CEasyTime_base::getDaysOfMonth(short year,short month)
{
	if(month==1||month==3||month==5||month==7||month==8||month==10||month==12) return 31;
	if(month==4||month==6||month==9||month==11) return 30;
	if(isLeapYear(year)) return 29;
	return 28;
}
void CEasyTime_base::getSundayInfoOfMonth(short year,short month,
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
void CEasyTime_base::getSundayInfoOfMonth2(short DaysOfMonth,short mday_firstday,short wday_firstday,
		short& rltSundaysOfMonth,short& rltmdayOfFirstSunday,short& rltmdayOfLastSunday)
{
	rltmdayOfFirstSunday=mday_firstday+(7-wday_firstday)%7;
	rltSundaysOfMonth=(DaysOfMonth-rltmdayOfFirstSunday+1)/7+((DaysOfMonth-rltmdayOfFirstSunday+1)%7==0?0:1);
	rltmdayOfLastSunday=rltmdayOfFirstSunday+(rltSundaysOfMonth-1)*7;
}

void CEasyTime_base::convert2fileds(short* prltyear,short* prltmonth,short* prltday,short* prlthour,short* prltminutes,short* prltsecs,short* prltweekday) const
{
	if(prltyear) *prltyear=m_tm.tm_year+1900;
	if(prltmonth) *prltmonth=m_tm.tm_mon+1;
	if(prltday) *prltday=m_tm.tm_mday;
	if(prlthour) *prlthour=m_tm.tm_hour;
	if(prltminutes) *prltminutes=m_tm.tm_min;
	if(prltsecs) *prltsecs=m_tm.tm_sec;
	if(prltweekday) *prltweekday=m_tm.tm_wday;
}
int CEasyTime_base::getyear(void) const
{
	return (int)(m_tm.tm_year+1900);
}
int CEasyTime_base::getmon(void) const
{
	return (int)(m_tm.tm_mon+1);
}
int CEasyTime_base::getday(void) const
{
	return (int)(m_tm.tm_mday);
}
int CEasyTime_base::getweekday(void) const
{
	return (int)(m_tm.tm_wday);
}
int CEasyTime_base::gethour(void) const
{
	return (int)(m_tm.tm_hour);
}
int CEasyTime_base::getmin(void) const
{
	return (int)(m_tm.tm_min);
}
int CEasyTime_base::getsec(void) const
{
	return (int)(m_tm.tm_sec);
}


CEasyTime_base::~CEasyTime_base()
{
}



Stru_TimezoneDst::Stru_TimezoneDst()
{
	clear();
}
Stru_TimezoneDst::Stru_TimezoneDst(int Tz,short Usedst,
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
Stru_TimezoneDst::Stru_TimezoneDst(const Stru_TimezoneDst& r)
{
	*this=r;
}
bool Stru_TimezoneDst::operator== (const Stru_TimezoneDst& r) const
{
	if(tz!=r.tz||usedst!=r.usedst) return false;
	if(!usedst) return true;
	if(usedst&&(startdst_month!=r.startdst_month||startdst_sunday!=r.startdst_sunday||startdst_hour!=r.startdst_hour||
				enddst_month!=r.enddst_month||enddst_sunday!=r.enddst_sunday||enddst_hour!=r.enddst_hour))
		return false;
	else return true;
}
void Stru_TimezoneDst::clear(void)
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
bool Stru_TimezoneDst::getDst_IntraYear(short year,time_t& rltStartDst,time_t& rltEndDst) const
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
bool Stru_TimezoneDst::getDst_IntraYear2(time_t refutc,time_t& rltStartDst,time_t& rltEndDst) const
{
	struct tm *ptm=tzstdtm(refutc);
	return getDst_IntraYear(ptm->tm_year+1900,rltStartDst,rltEndDst);
}

//get time in timezone, without regard to DST
struct tm* Stru_TimezoneDst::tzstdtm(time_t utc) const
{
	utc-=tz;
	return gmtime(&utc);
}
//get struct tm of utc, in view of timezone and dst
struct tm* Stru_TimezoneDst::tzdsttm(time_t utc) const
{
	utc-=tz;
	if(isInDst(utc)) utc-=3600;
	return gmtime(&utc);
}
time_t Stru_TimezoneDst::mktzdsttime(const struct tm* ptzdsttm)
{
	time_t utc=CEasyTime_base::mkgmtime(ptzdsttm);
	utc-=tz;
	if(isInDst2(utc,ptzdsttm->tm_year+1900))
		utc-=3600;
	return utc;
}

bool Stru_TimezoneDst::isUseDst(void) const
{
	return usedst==0?false:true;
}
bool Stru_TimezoneDst::isInDst(time_t refutc) const
{
	time_t rltStartDst,rltEndDst;
	bool brlt=getDst_IntraYear2(refutc,rltStartDst,rltEndDst);
	if(!brlt) return false;
	return refutc>=rltStartDst&&refutc<rltEndDst?true:false;
}
bool Stru_TimezoneDst::isInDst2(time_t refutc,short year) const
{
	time_t rltStartDst,rltEndDst;
	bool brlt=getDst_IntraYear(year,rltStartDst,rltEndDst);
	if(!brlt) return false;
	return refutc>=rltStartDst&&refutc<rltEndDst?true:false;
}



CEasyTime::CEasyTime()
{
	ftime(&m_timeb);
	calculate_tm_internal();
}
CEasyTime::CEasyTime(const CEasyTime& r)
{
	*this=r;
}
CEasyTime::CEasyTime(const struct timeb& r)
{
	m_timeb=r;
	calculate_tm_internal();
}
CEasyTime::CEasyTime(time_t utc,unsigned short millitm)
{
	ftime(&m_timeb);
	m_timeb.time=utc;
	m_timeb.millitm=millitm;
	calculate_tm_internal();
}
//note: don't care dst in this construction
CEasyTime::CEasyTime(int year,int month,int day,int hour,int minutes,int secs)
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
CEasyTime::CEasyTime(const char* pyyyymmdd,const char* phhmmdd,unsigned short millitm)
{
	settime(pyyyymmdd,phhmmdd,millitm);
}


CEasyTime::~CEasyTime()
{
}

CEasyTime& CEasyTime::operator= (const CEasyTime& r)
{
	m_timeb=r.m_timeb;
	m_tm=r.m_tm;
	return *this;
}
bool CEasyTime::operator== (const CEasyTime& r) const
{
	return m_timeb.time==r.m_timeb.time&&m_timeb.millitm==r.m_timeb.millitm?true:false;
}
bool CEasyTime::operator!= (const CEasyTime& r) const
{
	return m_timeb.time!=r.m_timeb.time||m_timeb.millitm!=r.m_timeb.millitm?true:false;
}

void CEasyTime::clear(void)
{
	memset(&m_timeb,0,sizeof(m_timeb));
	calculate_tm_internal();
}

void CEasyTime::settime(const struct timeb& r)
{
	m_timeb=r;
	calculate_tm_internal();
}
void CEasyTime::settime(time_t utc,unsigned short millitm)
{
	m_timeb.time=utc;
	m_timeb.millitm=millitm;
	calculate_tm_internal();
}
void CEasyTime::settime(int year,int month,int day,int hour,int minutes,int secs,int millitm)
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
	m_timeb.millitm=millitm;
	calculate_tm_internal();
}
void CEasyTime::setcurtime(void)
{
	ftime(&m_timeb);
	calculate_tm_internal();
}
//yyyymmdd 00:00:00.000
void CEasyTime::settime(const char* pyyyymmdd)
{
	int iYear=0,iMon=0,iDay=0;
    sscanf(pyyyymmdd,"%04d%02d%02d",&iYear,&iMon,&iDay);

	struct tm itm;
	memset(&itm,0,sizeof(itm));
	itm.tm_year=iYear-1900;
	itm.tm_mon=iMon-1;
	itm.tm_mday=iDay;
	itm.tm_isdst=-1;

	memset(&m_timeb,0,sizeof(m_timeb));
	m_timeb.time=mktime(&itm);
	calculate_tm_internal();
}
//yyyymmdd  hh:mm:ss.millitm
void CEasyTime::settime(const char* pyyyymmdd,const char* phhmmdd,unsigned short millitm)
{
	short iYear=0,iMon=0,iDay=0;
	if(pyyyymmdd)
		sscanf(pyyyymmdd,"%04d%02d%02d",&iYear,&iMon,&iDay);
	else
		CEasyTime().convert2fileds(&iYear,&iMon,&iDay);

	int iHour=0,iMin=0,iSec=0;
	sscanf(phhmmdd,"%02d:%02d:%02d",&iHour,&iMin,&iSec);

	struct tm itm;
	memset(&itm,0,sizeof(itm));
	itm.tm_year=iYear-1900;
	itm.tm_mon=iMon-1;
	itm.tm_mday=iDay;
	itm.tm_hour=iHour;
	itm.tm_min=iMin;
	itm.tm_sec=iSec;
	itm.tm_isdst=-1;

	memset(&m_timeb,0,sizeof(m_timeb));
	m_timeb.time=mktime(&itm);
	m_timeb.millitm=millitm;
	calculate_tm_internal();
}
void CEasyTime::settime_yyyymmddhhmm(const char* pyyyymmddhhmm)
{
	short iYear=0,iMon=0,iDay=0,iHour=0,iMin=0;
	if(pyyyymmddhhmm)
		sscanf(pyyyymmddhhmm,"%04d%02d%02d%02d%02d",&iYear,&iMon,&iDay,&iHour,&iMin);
	else
		CEasyTime().convert2fileds(&iYear,&iMon,&iDay,&iHour,&iMin);

	settime((int)iYear,(int)iMon,(int)iDay,(int)iHour,(int)iMin,0,0);
}


struct timeb CEasyTime::gettimeb(void) const
{
	return m_timeb;
}
time_t CEasyTime::getutc(void) const
{
	return m_timeb.time;
}
time_t CEasyTime::getutc_timezero(void) const
{
	return m_timeb.time-m_tm.tm_hour*3600-m_tm.tm_min*60-m_tm.tm_sec;
}
time_t CEasyTime::getutc_timezero_firstdayofmonth(void) const
{
	return m_timeb.time-(m_tm.tm_mday-1)*24*3600-m_tm.tm_hour*3600-m_tm.tm_min*60-m_tm.tm_sec;
}
time_t CEasyTime::getutc_timezero_firstdayofnextmonth(void) const
{
	int days=getDaysOfMonth((short)m_tm.tm_year+1900,(short)m_tm.tm_mon+1);
	return m_timeb.time-(m_tm.tm_mday-1)*24*3600-m_tm.tm_hour*3600-m_tm.tm_min*60-m_tm.tm_sec+days*24*3600;
}
unsigned short CEasyTime::getmillitm(void) const
{
	return m_timeb.millitm;
}
int CEasyTime::getSecondsIntraday(void) const
{
	return m_tm.tm_hour*3600+m_tm.tm_min*60+m_tm.tm_sec;
}
const struct tm& CEasyTime::gettm(void) const
{
	return m_tm;
}

//get time string formated to "yyyy-mm-dd hh:mm:ss.mmm"
char* CEasyTime::getstrtime(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
				m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
	return prlt;
}
string CEasyTime::getstrtime(void) const
{
    char buf[64]={0};
    getstrtime(buf,sizeof(buf));
    return string(buf);
}
//get time string formated to "yyyy-mm-dd hh:mm:ss"
char* CEasyTime::getstrtime2(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d-%02d-%02d %02d:%02d:%02d",
				m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec);
	return prlt;
}
string CEasyTime::getstrtime2(void) const
{
    char buf[64]={0};
    getstrtime2(buf,sizeof(buf));
    return string(buf);
}
//get time string formated to "yyyymmdd"
char* CEasyTime::getstryyyymmdd(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d%02d%02d",
				m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday);
	return prlt;
}
string CEasyTime::getstryyyymmdd(void) const
{
    char buf[64]={0};
    getstryyyymmdd(buf,sizeof(buf));
    return string(buf);
}
//get time string formated to "yyyy-mm"
char* CEasyTime::getstryyyymm(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d-%02d",
				m_tm.tm_year+1900,m_tm.tm_mon+1);
	return prlt;
}
string CEasyTime::getstryyyymm(void) const
{
    char buf[64]={0};
    getstryyyymm(buf,sizeof(buf));
    return string(buf);
}

//get time string formated to "yyyy"
char* CEasyTime::getstryyyy(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d",m_tm.tm_year+1900);
	return prlt;
}
string CEasyTime::getstryyyy(void) const
{
    char buf[64]={0};
    getstryyyy(buf,sizeof(buf));
    return string(buf);
}
//get time string formated to "hh:mm:ss.mmm"
char* CEasyTime::getstrshorttime(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%02d:%02d:%02d.%03d",
				m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
	return prlt;
}
string CEasyTime::getstrshorttime(void) const
{
    char buf[64]={0};
    getstrshorttime(buf,sizeof(buf));
    return string(buf);
}
//get time string formated to "hh:mm:ss"
char* CEasyTime::getstrshorttime2(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%02d:%02d:%02d",
				m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
	return prlt;
}
string CEasyTime::getstrshorttime2(void) const
{
    char buf[64]={0};
    getstrshorttime2(buf,sizeof(buf));
    return string(buf);
}


//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
int64_EasyTime CEasyTime::gettimediff(const CEasyTime& timelater) const
{
	int64_EasyTime t1,t2;
	t1=(int64_EasyTime)m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)m_timeb.millitm;
	t2=(int64_EasyTime)timelater.m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)timelater.m_timeb.millitm;
	return t2-t1;
}
//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
int64_EasyTime CEasyTime::gettimediff(const struct timeb& timeblater) const
{
	int64_EasyTime t1,t2;
	t1=(int64_EasyTime)m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)m_timeb.millitm;
	t2=(int64_EasyTime)timeblater.time*(int64_EasyTime)1000+(int64_EasyTime)timeblater.millitm;
	return t2-t1;
}
//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
int64_EasyTime CEasyTime::gettimediff(time_t utclater,unsigned short millitmlater) const
{
	int64_EasyTime t1,t2;
	t1=(int64_EasyTime)m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)m_timeb.millitm;
	t2=(int64_EasyTime)utclater*(int64_EasyTime)1000+(int64_EasyTime)millitmlater;
	return t2-t1;
}
//get time diff between m_timeb to curtime, in ms. timeDiff=curtime-m_timeb
int64_EasyTime CEasyTime::gettimediff2curtime(void) const
{
	struct timeb curtime;
	ftime(&curtime);
	int64_EasyTime t1,t2;
	t1=(int64_EasyTime)m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)m_timeb.millitm;
	t2=(int64_EasyTime)curtime.time*(int64_EasyTime)1000+(int64_EasyTime)curtime.millitm;
	return t2-t1;
}
//get time diff in seconds formated to "-h-m-s"
string CEasyTime::gettimediff2(const CEasyTime& timelater) const
{
    time_t diff=timelater.m_timeb.time-m_timeb.time;
    char buf[64]={0};
    if(diff>=3600) snprintf_EasyTime(buf,sizeof(buf)-1,"%dh%dm%ds",diff/3600,(diff%3600)/60,diff%60);
    else if(diff>=60) snprintf_EasyTime(buf,sizeof(buf)-1,"%dm%ds",diff/60,diff%60);
    else snprintf_EasyTime(buf,sizeof(buf)-1,"%ds",diff);
    return string(buf);
}

void CEasyTime::calculate_tm_internal(void)
{
	struct tm* ptm=localtime(&m_timeb.time);
	if(ptm) memcpy((char*)&m_tm,(char*)ptm,sizeof(m_tm));
	else memset(&m_tm,0,sizeof(m_tm));
}

CEasyTime2::CEasyTime2(const Stru_TimezoneDst& tzdst)
:m_tzdst(tzdst)
{
	memset(&m_timeb,0,sizeof(m_timeb));
	calculate_tm_internal();
}
CEasyTime2::CEasyTime2(const CEasyTime2& r)
{
	*this=r;
}
CEasyTime2::CEasyTime2(time_t utc,unsigned short millitm,const Stru_TimezoneDst& tzdst)
:m_tzdst(tzdst)
{
	memset(&m_timeb,0,sizeof(m_timeb));
	m_timeb.time=utc;
	m_timeb.millitm=millitm;
	calculate_tm_internal();
}
//note: don't care dst in this construction
CEasyTime2::CEasyTime2(int year,int month,int day,int hour,int minutes,int secs,const Stru_TimezoneDst& tzdst)
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
CEasyTime2::~CEasyTime2()
{
}

CEasyTime2& CEasyTime2::operator= (const CEasyTime2& r)
{
	m_tzdst=r.m_tzdst;
	m_timeb=r.m_timeb;
	m_tm=r.m_tm;
	return *this;
}

void CEasyTime2::clear(void)
{
	memset(&m_timeb,0,sizeof(m_timeb));
	calculate_tm_internal();
}

void CEasyTime2::settime(const struct timeb& r)
{
	m_timeb=r;
	calculate_tm_internal();
}
void CEasyTime2::settime(time_t utc,unsigned short millitm)
{
	memset(&m_timeb,0,sizeof(m_timeb));
	m_timeb.time=utc;
	m_timeb.millitm=millitm;
	calculate_tm_internal();
}
void CEasyTime2::settime(int year,int month,int day,int hour,int minutes,int secs,int millitm)
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
	m_timeb.millitm=millitm;
	calculate_tm_internal();
}
void CEasyTime2::setcurtime(void)
{
	ftime(&m_timeb);
	calculate_tm_internal();
}


const Stru_TimezoneDst& CEasyTime2::gettzdst(void) const
{
	return m_tzdst;
}

struct timeb CEasyTime2::gettimeb(void) const
{
	return m_timeb;
}
time_t CEasyTime2::getutc(void) const
{
	return m_timeb.time;
}
time_t CEasyTime2::getutc_timezero(void) const
{
	return m_timeb.time-m_tm.tm_hour*3600-m_tm.tm_min*60-m_tm.tm_sec;
}
unsigned short CEasyTime2::getmillitm(void) const
{
	return m_timeb.millitm;
}
int CEasyTime2::getSecondsIntraday(void) const
{
	return m_tm.tm_hour*3600+m_tm.tm_min*60+m_tm.tm_sec;
}
const struct tm& CEasyTime2::gettm(void) const
{
	return m_tm;
}

//get time string formated to "yyyy-mm-dd hh:mm:ss.mmm"
char* CEasyTime2::getstrtime(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
				m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
	return prlt;
}
//get time string formated to "yyyy-mm-dd hh:mm:ss"
char* CEasyTime2::getstrtime2(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d-%02d-%02d %02d:%02d:%02d",
				m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec);
	return prlt;
}
//get time string formated to "yyyymmdd"
char* CEasyTime2::getstryyyymmdd(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d%02d%02d",
				m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday);
	return prlt;
}
//get time string formated to "yyyy"
char* CEasyTime2::getstryyyy(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%04d",m_tm.tm_year+1900);
	return prlt;
}
//get time string formated to "hh:mm:ss.mmm"
char* CEasyTime2::getstrshorttime(char* prlt,int rltsize) const
{
	snprintf_EasyTime(prlt,rltsize-1,"%02d:%02d:%02d.%03d",
				m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec,m_timeb.millitm);
	return prlt;
}

//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
int64_EasyTime CEasyTime2::gettimediff(const CEasyTime2& timelater) const
{
	int64_EasyTime t1,t2;
	t1=(int64_EasyTime)m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)m_timeb.millitm;
	t2=(int64_EasyTime)timelater.m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)timelater.m_timeb.millitm;
	return t2-t1;
}
//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
int64_EasyTime CEasyTime2::gettimediff(const struct timeb& timeblater) const
{
	int64_EasyTime t1,t2;
	t1=(int64_EasyTime)m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)m_timeb.millitm;
	t2=(int64_EasyTime)timeblater.time*(int64_EasyTime)1000+(int64_EasyTime)timeblater.millitm;
	return t2-t1;
}
//get time diff between m_timeb to timeblater, in ms. timeDiff=timeblater-m_timeb
int64_EasyTime CEasyTime2::gettimediff(time_t utclater,unsigned short millitmlater) const
{
	int64_EasyTime t1,t2;
	t1=(int64_EasyTime)m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)m_timeb.millitm;
	t2=(int64_EasyTime)utclater*(int64_EasyTime)1000+(int64_EasyTime)millitmlater;
	return t2-t1;
}
//get time diff between m_timeb to curtime, in ms. timeDiff=curtime-m_timeb
int64_EasyTime CEasyTime2::gettimediff2curtime(void) const
{
	struct timeb curtime;
	ftime(&curtime);
	int64_EasyTime t1,t2;
	t1=(int64_EasyTime)m_timeb.time*(int64_EasyTime)1000+(int64_EasyTime)m_timeb.millitm;
	t2=(int64_EasyTime)curtime.time*(int64_EasyTime)1000+(int64_EasyTime)curtime.millitm;
	return t2-t1;
}

void CEasyTime2::calculate_tm_internal(void)
{
	struct tm* ptm=m_tzdst.tzdsttm(m_timeb.time);
	if(ptm) memcpy((char*)&m_tm,(char*)ptm,sizeof(m_tm));
	else memset(&m_tm,0,sizeof(m_tm));
}




#undef int64_EasyTime
#undef snprintf_EasyTime
