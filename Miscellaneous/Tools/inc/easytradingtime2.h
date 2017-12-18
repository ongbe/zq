//*******************************************************************************************************
//			交易所交易时间参数
//			(1) 定义每个交易所的交易时段参数。通常由应用层从xml中读出
//			(2) 计算出每个交易所交易时段的分钟数。
//				比如上期交易时段为21:00-2:30 + 9:00-10:15 + 10:30-11:30 + 13:30-15:00
//				对应计算出1260-1440 + 0-150 + 540-615 + 630-690 + 810-900
//			(3) 提供函数，计算一个日内时间值在日内的交易分钟数
//				如9:01的日内交易分钟数为331。通过此值，可以计算出9:01分K线数据在数组中的位置
//			(4) 可通用于期货交易所和证券交易所
//*******************************************************************************************************

#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;
using std::make_pair;
using std::pair;

class Ceasytradingtime2
{
public:
	Ceasytradingtime2(const vector<pair<int,int>>& tradingtime);
	Ceasytradingtime2(const Ceasytradingtime2& r);
	virtual ~Ceasytradingtime2(void);
	Ceasytradingtime2& operator= (const Ceasytradingtime2& r);

public:

	//取utc时间的日内的交易分钟数。如交易时段为21:00-24:00;  0:00-2:30，则0:01的日内交易分钟数为181
	bool GetTradingMinutesIntraday(time_t utc,int& TradingMinutes) const;

	//判断当前是否在交易时段内
	bool IsNowTradingTime(void);

private:
	void calculate_tradingsecs(void);


public:
	//utc时区修正值。utc+m_tzseconds=1970.1.1以来的天数*24*3600+hour*3600+min*60+sec
	int m_tzseconds;

	//1970.1.1的星期值。星期日为0。此后任意一天的星期值=(utcday+m_basetmweek)%7。任意一天的周序号=(utcday+m_basetmweek)/7
	int m_basetmweek;

	//日内交易时段, 用日内秒数表示,每个时段不能跨日,不同时段可以跨日
	// 如21:00-24:00; 0:00-2:30; 9:00-10:15; 10:30-11:30; 13:30-15:00, 表示为：(75600,86400),(0,9000),(32400,36900),(37800,41400),(48600,54000)
	const vector<pair<int,int>> m_basetradingtime;

	//日内交易总秒数/总分钟数
	int m_tradingsecs_intraday;
	int m_tradingmins_intraday;

	//当日开盘和收盘的utc时间
	time_t m_OpenTime_Today;
	time_t m_CloseTime_Today;
};
