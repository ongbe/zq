using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;

namespace com.cifco.zqstp.Market
{
    public class MarketDetph
    {
        // 合约ID
        public string instrumentId = "";
        // 交易所ID
        public string exchangeId = "";
        // 交易日
        public string tradingday = "";
        // 更新时间
        public string updateTime = "";
        // 更新毫秒
        public int updateMS = 0;
        // 最新价
        public double latestPrice = 0.0;
        // 开盘价
        public double open = 0.0;
        // 最高价
        public double high = 0.0;
        // 最低价
        public double low = 0.0;
        // 现量
        public int latestVol = 0;
        // 持仓量
        public int openInt = 0;
        // 收盘价
        public double todayClosePrice = 0.0;
        // 成交金额
        public double turnover = 0.0;
        // 均价
        public double avgLatestPrice = 0.0;
        // 昨结算价
        public double preSettlementPrice = 0.0;
        // 昨收价
        public double yesterdayClosePrice = 0.0;
        // 昨持仓量
        public int yesterdayOpenInt = 0;
        // 结算价
        public double settlementPrice = 0.0;
        // 涨停价
        public double upperLimitPrice = 0.0;
        // 跌停价
        public double lowerLimitPrice = 0.0;
        // 买一价
        public double bidPrice1 = 0.0;
        // 卖一量
        public int bidVol1 = 0;
        // 卖一价
        public double askPrice1 = 0.0;
        // 卖一量
        public int askVol1 = 0;
        // 买二价
        public double bidPrice2 = 0.0;
        // 买二量
        public int bidVol2 = 0;
        // 卖二价
        public double askPrice2 = 0.0;
        // 卖二量
        public int askVol2 = 0;
        // 买三价
        public double bidPrice3 = 0.0;
        // 买三量
        public int bidVol3 = 0;
        // 卖三价
        public double askPrice3 = 0.0;
        // 卖三量
        public int askVol3 = 0;
        // 买四价
        public double bidPrice4 = 0.0;
        // 买四量
        public int bidVol4 = 0;
        // 卖四价
        public double askPrice4 = 0.0;
        // 卖四量
        public int askVol4 = 0;
        // 买五价
        public double bidPrice5 = 0.0;
        // 买五量
        public int bidVol5 = 0;
        // 卖五价
        public double askPrice5 = 0.0;
        // 卖五量
        public int askVol5 = 0;

        public bool CompareTo(MarketDetph o)
        {
            return instrumentId == o.instrumentId && exchangeId == o.exchangeId &&
                tradingday == o.tradingday && updateTime == o.updateTime && updateMS == o.updateMS;
        }

        public MarketDetph Clone()
        {
            MarketDetph newMD = new MarketDetph();

            // 合约ID
            newMD.instrumentId = instrumentId;
            // 交易所ID
            newMD.exchangeId = exchangeId;
            // 交易日
            newMD.tradingday = tradingday;
            // 更新时间
            newMD.updateTime = updateTime;
            // 更新毫秒
            newMD.updateMS = updateMS;
            // 最新价
            newMD.latestPrice = latestPrice;
            // 开盘价
            newMD.open = open;
            // 最高价
            newMD.high = high;
            // 最低价
            newMD.low = low;
            // 现量
            newMD.latestVol = latestVol;
            // 持仓量
            newMD.openInt = openInt;
            // 收盘价
            newMD.todayClosePrice = todayClosePrice;
            // 成交金额
            newMD.turnover = turnover;
            // 均价
            newMD.avgLatestPrice = avgLatestPrice;
            // 昨结算价
            newMD.preSettlementPrice = preSettlementPrice;
            // 昨收价
            newMD.yesterdayClosePrice = yesterdayClosePrice;
            // 昨持仓量
            newMD.yesterdayOpenInt = yesterdayOpenInt;
            // 结算价
            newMD.settlementPrice = settlementPrice;
            // 涨停价
            newMD.upperLimitPrice = upperLimitPrice;
            // 跌停价
            newMD.lowerLimitPrice = lowerLimitPrice;
            // 买一价
            newMD.bidPrice1 = bidPrice1;
            // 卖一量
            newMD.bidVol1 = bidVol1;
            // 卖一价
            newMD.askPrice1 = askPrice1;
            // 卖一量
            newMD.askVol1 = askVol1;
            // 买二价
            newMD.bidPrice2 = bidPrice2;
            // 买二量
            newMD.bidVol2 = bidVol2;
            // 卖二价
            newMD.askPrice2 = askPrice2;
            // 卖二量
            newMD.askVol2 = askVol2;
            // 买三价
            newMD.bidPrice3 = bidPrice3;
            // 买三量
            newMD.bidVol3 = bidVol3;
            // 卖三价
            newMD.askPrice3 = askPrice3;
            // 卖三量
            newMD.askVol3 = askVol3;
            // 买四价
            newMD.bidPrice4 = bidPrice4;
            // 买四量
            newMD.bidVol4 = bidVol4;
            // 卖四价
            newMD.askPrice4 = askPrice4;
            // 卖四量
            newMD.askVol4 = askVol4;
            // 买五价
            newMD.bidPrice5 = bidPrice5;
            // 买五量
            newMD.bidVol5 = bidVol5;
            // 卖五价
            newMD.askPrice5 = askPrice5;
            // 卖五量
            newMD.askVol5 = askVol5;

            return newMD;
        }
    }

    // Kline结构，可用于KLine图，分时图等的数据存储
    public class Bar
    {
        // Bar周期
        public int barCycle;
        // Bar周期单位
        public int barUnit;
        // 开始日期
        public int date;
        // 开始时间
        public int time;
        // 开盘价
        public double open;
        // 最高价
        public double high;
        // 最低价
        public double low;
        // 收盘价
        public double close;
        // 持仓量
        public int openInt;
        // 成交金额
        public double turnover;
        // 成交量
        public int vol;
        // 现量
        public int latestVol;
        // 价格变动，算法未知
        public double priceChange;
        // 价格变动率，算法未知
        public double priceChangeRate;
        // 状态，用法未知
        public int status;
        // 索引，用法未知
        public int index;


        public bool CompareTo(Bar o)
        {
            return barCycle == o.barCycle && barUnit == o.barUnit &&
                date == o.date && time == o.time;
        }

        public Bar Clone()
        {
            Bar newbar = new Bar();

            // Bar周期
            newbar.barCycle = barCycle;
            // Bar周期单位
            newbar.barUnit = barUnit;
            // 开始日期
            newbar.date = date;
            // 开始时间
            newbar.time = time;
            // 开盘价
            newbar.open = open;
            // 最高价
            newbar.high = high;
            // 最低价
            newbar.low = low;
            // 收盘价
            newbar.close = close;
            // 持仓量
            newbar.openInt = openInt;
            // 成交金额
            newbar.turnover = turnover;
            // 成交量
            newbar.vol = vol;
            // 现量
            newbar.latestVol = latestVol;
            // 价格变动，算法未知
            newbar.priceChange = priceChange;
            // 价格变动率，算法未知
            newbar.priceChangeRate = priceChangeRate;
            // 状态，用法未知
            newbar.status = status;
            // 索引，用法未知
            newbar.index = index;

            return newbar;
        }
    }

}
