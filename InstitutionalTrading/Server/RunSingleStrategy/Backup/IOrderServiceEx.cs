using System;
using System.Collections.Generic;
using RunSingleStrategy;
using ServiceStack.Redis;
using com.cifco.zqstp.Misc;

namespace com.cifco.zqstp.Trader
{
    public class IOrderServiceEx : IOrderService
    {
        public RedisClient Redis;
        private XmlOpration xmlOpr = new XmlOpration();
        private CommonTool Tool = new CommonTool();

        public IOrderServiceEx(RedisClient Redis_PushMsg)
        {
            // TODO: Complete member initialization
            this.Redis = Redis_PushMsg;
        }

        // 请求下单
        public override int ReqOrderInsert(InputOrder inputOrder)
        {
            lock (this)
            {
                string message = null;
                xmlOpr.Transfer(inputOrder, ref message);

                Log.Out(message);

                //Publish到redis下单通道            
                Redis.PublishMessage(CommonDef.OrderInputChannel, message);
            }
            return 0;
        }

        // 请求撤单
        public override int ReqOrderAction(InputAction inputAction)
        {
            lock (this)
            {
                string message = null;
                xmlOpr.Transfer(inputAction, ref message);

                //Publish到redis撤单通道            
                Redis.PublishMessage(CommonDef.OrderActionChannel, message);
            }
            return 0;
        }

        // 请求查询交易员资金帐号
        public override bool GetTradingAccount(QryTradingAccount qry, ref TradingAccount refTradingAccount)
        {
            bool result = false;
            lock (this)
            {
                string szHashId = "fund";
                string szKey = RunSingleStrategy.StrategyInfo.TraderName;
                byte[] bKey = Tool.GetBytes(szKey);

                byte[] bValue = Redis.HGet(szHashId, bKey);
                if (bValue != null)
                {
                    string szValue = Tool.GetString(bValue);
                    xmlOpr.Transfer(szValue, ref refTradingAccount);
                    result = true;
                }
            }
            return result;
        }

        // 请求查询交易员持仓
        public override bool GetPositions(QryPosition qry, ref List<PositionInfo> refPositions)
        {
            bool result = false;
            lock (this)
            {
                string szHashId = "p:" + RunSingleStrategy.StrategyInfo.TraderName;
                string szInst = qry.InstrumentID;
                string szValue = null;
                byte[] bKey = null;
                byte[] bValue = null;
                byte[][] bValueAll = null;                

                bValueAll = Redis.HKeys(szHashId);
                List<string> keyList = new List<string>();

                foreach (byte[] elem in bValueAll)
                {
                    szValue = Tool.GetString(elem);
                    keyList.Add(szValue);
                }

                if (qry.PosiDirection.Equals('-'))
                {
                    //双向 
                    if (szInst.Length == 0)
                    {
                        //全部合约
                        foreach (string key in keyList)
                        {
                            bKey = Tool.GetBytes(key);
                            bValue = Redis.HGet(szHashId, bKey);
                            if (bValue != null)
                            {
                                PositionInfo position = new PositionInfo();
                                szValue = Tool.GetString(bValue);
                                xmlOpr.Transfer(szValue, ref position);
                                refPositions.Add(position);
                                result = true;
                            }
                        }
                    }
                    else
                    {
                        //特定合约
                        foreach (string key in keyList)
                        {
                            if (key.Contains(szInst))
                            {
                                bKey = Tool.GetBytes(key);
                                bValue = Redis.HGet(szHashId, bKey);
                                if (bValue != null)
                                {
                                    PositionInfo position = new PositionInfo();
                                    szValue = Tool.GetString(bValue);
                                    xmlOpr.Transfer(szValue, ref position);
                                    refPositions.Add(position);
                                    result = true;
                                }
                            }
                        }
                    }
                }
                else
                {
                    //单向
                    string szDirect = "Direct" + Convert.ToInt32(qry.PosiDirection).ToString();
                    if (szInst.Length == 0)
                    {
                        //全部合约
                        foreach (string key in keyList)
                        {
                            if (key.Contains(szDirect))
                            {
                                bKey = Tool.GetBytes(key);
                                bValue = Redis.HGet(szHashId, bKey);
                                if (bValue != null)
                                {
                                    PositionInfo position = new PositionInfo();
                                    szValue = Tool.GetString(bValue);
                                    xmlOpr.Transfer(szValue, ref position);
                                    refPositions.Add(position);
                                    result = true;
                                }
                            }
                        }
                    }
                    else
                    {
                        //特定合约
                        foreach (string key in keyList)
                        {
                            if (key.Contains(szInst) && key.Contains(szDirect))
                            {
                                bKey = Tool.GetBytes(key);
                                bValue = Redis.HGet(szHashId, bKey);
                                if (bValue != null)
                                {
                                    PositionInfo position = new PositionInfo();
                                    szValue = Tool.GetString(bValue);
                                    xmlOpr.Transfer(szValue, ref position);
                                    refPositions.Add(position);
                                    result = true;
                                }
                            }
                        }
                    }
                }
            }
            return result;
        }

        // 请求查询报单
        public override bool GetOrders(QryOrder qry, ref List<OrderInfo> refOrders)
        {
            bool result = false;
            lock (this)
            {
                string szHashId = "o:" + RunSingleStrategy.StrategyInfo.TraderName;
                string szInst = qry.InstrumentID;

                List<string> keyList = new List<string>();

                byte[][] para = Redis.HKeys(szHashId);
                foreach (byte[] elem in para)
                {
                    string szValue = Tool.GetString(elem);
                    keyList.Add(szValue);
                }

                foreach (string key in keyList)
                {
                    //目前不用考虑特定合约的情况，只关心用户，每次返回该用户的所有报单
                    //if (key.Contains(szInst))
                    //{
                    OrderInfo orderInfo = new OrderInfo();
                    byte[] bValue = Redis.HGet(szHashId, Tool.GetBytes(key));
                    string szValue = Tool.GetString(bValue);
                    xmlOpr.Transfer(szValue, ref orderInfo);
                    refOrders.Add(orderInfo);
                    result = true;
                    //}
                }
            }
            return result;
        }

        // 请求查询成交。保留函数，未实现。
        public override bool GetTrades(QryTrade qry, ref List<TradeInfo> refTrades)
        {
            lock (this)
            {
                throw new NotImplementedException();
            }
        }
    }
}
