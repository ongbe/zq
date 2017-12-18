#include "easytradingtime2.h"
#include "easytime.h"

Ceasytradingtime2::Ceasytradingtime2(const vector<pair<int,int>>& tradingtime)
:	m_tzseconds(0)
	,m_basetradingtime(tradingtime)
	,m_tradingsecs_intraday(0)
	,m_tradingmins_intraday(0)
{
	calculate_tradingsecs();
}

Ceasytradingtime2::Ceasytradingtime2(const Ceasytradingtime2& r) 
:	m_tzseconds(0)
	,m_basetradingtime(r.m_basetradingtime)
	,m_tradingsecs_intraday(0)
	,m_tradingmins_intraday(0)
{ 
	calculate_tradingsecs();
}

Ceasytradingtime2::~Ceasytradingtime2(void)
{
}

Ceasytradingtime2& Ceasytradingtime2::operator= (const Ceasytradingtime2& r)
{
	new(this) Ceasytradingtime2(r);
	return *this;
}

//计算日内交易总秒数
void Ceasytradingtime2::calculate_tradingsecs(void)
{
	//计算时区修正值秒数
	time_t utc=time(NULL);
	struct tm localtm=*localtime(&utc);
	struct tm gmtm=*gmtime(&utc);
	m_tzseconds=localtm.tm_hour*3600+localtm.tm_min*60-(gmtm.tm_hour*3600+gmtm.tm_min*60);

	//计算1970.1.1的星期值。星期日为0。此后任意一天的星期值=(utcday+m_basetmweek)%7
	time_t baseutc=0;
	m_basetmweek=localtime(&baseutc)->tm_wday;

	//计算日内交易总秒数
	m_tradingsecs_intraday=0;
	for(int i=0;i<(int)m_basetradingtime.size();i++)
	{
		m_tradingsecs_intraday+=m_basetradingtime[i].second-m_basetradingtime[i].first;
	}
	m_tradingmins_intraday=m_tradingsecs_intraday/60;

	//计算当日的开盘收盘时间
	m_OpenTime_Today=(utc+m_tzseconds)/(24*3600)*(24*3600)+m_basetradingtime.begin()->first-m_tzseconds;
	m_CloseTime_Today=(utc+m_tzseconds)/(24*3600)*(24*3600)+m_basetradingtime.rbegin()->second-m_tzseconds;

}


//取utc时间在日内的交易分钟数。如交易时段为21:00-24:00;  0:00-2:30，则0:01的日内交易分钟数为181
bool Ceasytradingtime2::GetTradingMinutesIntraday(time_t utc,int& TradingMinutes) const
{
	TradingMinutes=0;
	int secs=((int)utc+m_tzseconds)%(24*3600);
	for(vector<pair<int,int>>::const_iterator it=m_basetradingtime.begin();it!=m_basetradingtime.end();it++)
	{
		if(secs<it->first) break;
		else if(secs>=it->second) TradingMinutes+=(it->second-it->first)/60;
		else
		{
			TradingMinutes+=(secs-it->first)/60;
			return true;
		}
	}
	return false;
}

//判断当前是否在交易时段内
bool Ceasytradingtime2::IsNowTradingTime(void)
{
	int secs=((int)time(NULL)+m_tzseconds)%(24*3600);
	for(vector<pair<int,int>>::const_iterator it=m_basetradingtime.begin();it!=m_basetradingtime.end();it++)
	{
		if(secs<it->first) break;
		else if(secs<it->second) return true;
	}
	return false;
}

