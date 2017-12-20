using System;

using System.Xml;

using com.cifco.zqstp.Market;
using com.cifco.zqstp.Trader;


namespace RunSingleStrategy
{
    class XmlOpration
    {
        //参数解析
        /*
        <strategy>
            <strategyName>strategyName1</strategyName>
            <traderName>traderName1</traderName>
            <parameter>para1</parameter>
            <instrument>instrument1</instrument>
            <sessionID>111101</sessionID>
            <frontID>222201</frontID>
            <orderRef>1</orderRef>
            <StartTimer>1</StartTimer>            <TimerSpan>100</TimerSpan>        </strategy>
         * 
         * 注意 para1 格式必须是
        <params>
            <Param type="int" name="p1">1</Param>
            <Param type="double" name="p2">1.0</Param>
        </params>
        */
        public void AnalyzeXmlParam(string strXml, ref StrategyInfo strategyInfo)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);
                  
            RunSingleStrategy.StrategyInfo.StrategyName = doc.SelectSingleNode("strategy/strategyName").InnerText;
            RunSingleStrategy.StrategyInfo.TraderName = doc.SelectSingleNode("strategy/traderName").InnerText;
            RunSingleStrategy.StrategyInfo.Parameter = doc.SelectSingleNode("strategy/parameter").InnerXml;            
            RunSingleStrategy.StrategyInfo.SessionID = doc.SelectSingleNode("strategy/sessionID").InnerText;
            RunSingleStrategy.StrategyInfo.FrontID = doc.SelectSingleNode("strategy/frontID").InnerText;
            RunSingleStrategy.StrategyInfo.OrderRef = doc.SelectSingleNode("strategy/orderRef").InnerText;
            RunSingleStrategy.StrategyInfo.StartTimer = doc.SelectSingleNode("strategy/startTimer").InnerText;
            RunSingleStrategy.StrategyInfo.TimerSpan = doc.SelectSingleNode("strategy/timerSpan").InnerText;
            try
            {
                RunSingleStrategy.StrategyInfo.SaveHistory = 
                        Int32.Parse(doc.SelectSingleNode("strategy/savehistory").InnerText)!=0;
                RunSingleStrategy.StrategyInfo.HistoryPath = doc.SelectSingleNode("strategy/historypath").InnerText;
            }
            catch (Exception e) { }
            CommonDef.StrategyInstenceTimerStatus = (Int32.Parse(doc.SelectSingleNode("strategy/startTimer").InnerText) == 1) ? true : false;
            CommonDef.StrategyInstenceRunAfterOfflineStatus = (Int32.Parse(doc.SelectSingleNode("strategy/RunAfterOffline").InnerText) == 1) ? true : false;//0为空转，1为运行
            string szInsts = doc.SelectSingleNode("strategy/instrument").InnerText;

            if (szInsts[szInsts.Length - 1].Equals(','))
                szInsts = szInsts.Remove(szInsts.Length - 1);

            string[] arrInst = szInsts.Split(',');
            foreach (string szInst in arrInst)
                RunSingleStrategy.StrategyInfo.Instruments.Add(szInst);

        }

        //得到节点的名称（仅限一个节点）
        public string GetType(string strXml)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);

            XmlNode node = doc.FirstChild;
            string szType = node.Name;
            
            return szType;
        }

        //得到节点的属性（仅限一个节点）
        public string GetVaule(string strXml, string szKey)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);

            XmlNode node = doc.FirstChild;
            string szValue = node.Attributes[szKey].Value;

            return szValue;
        }

        //将下单InputOrder结构转换成xml字符串
        public void Transfer(InputOrder inputOrder, ref string strXml)
        {
            int OrderRef = Int32.Parse(RunSingleStrategy.StrategyInfo.OrderRef);
            
            //生成xml字符串流：
            XmlDocument xmldoc = new XmlDocument();
            XmlElement xmlelem;

            xmlelem = xmldoc.CreateElement("", "OrderInput", "");
            xmlelem.SetAttribute("DealerID", RunSingleStrategy.StrategyInfo.TraderName);
            xmlelem.SetAttribute("BrokerID", "8090");
            xmlelem.SetAttribute("InstrumentID", inputOrder.InstrumentID);
            xmlelem.SetAttribute("Direction", inputOrder.Direction.ToString());
            xmlelem.SetAttribute("OpenClose", inputOrder.OpenClose.ToString());
            xmlelem.SetAttribute("Hedge", inputOrder.Hedge.ToString());
            xmlelem.SetAttribute("Price", inputOrder.Price.ToString());
            xmlelem.SetAttribute("IsMarket", inputOrder.IsMarket.ToString());
            xmlelem.SetAttribute("Qty", inputOrder.Qty.ToString());
            xmlelem.SetAttribute("SessionID", RunSingleStrategy.StrategyInfo.SessionID.ToString());
            xmlelem.SetAttribute("FrontID", RunSingleStrategy.StrategyInfo.FrontID.ToString());
            xmlelem.SetAttribute("OrderRef", (OrderRef++).ToString());
            RunSingleStrategy.StrategyInfo.OrderRef = OrderRef.ToString();


            xmldoc.AppendChild(xmlelem);

            strXml = xmldoc.InnerXml;            
        }

        //将撤单InputAction结构转换成xml字符串
        public void Transfer(InputAction inputAction, ref string strXml)
        {
            //生成xml字符串流：
            XmlDocument xmldoc = new XmlDocument();
            XmlElement xmlelem;

            xmlelem = xmldoc.CreateElement("", "OrderAction", "");
            xmlelem.SetAttribute("DealerID", inputAction.DealerID);
            xmlelem.SetAttribute("BrokerID", inputAction.BrokerID);
            xmlelem.SetAttribute("OrderRef", inputAction.OrderRef);
            xmlelem.SetAttribute("Session", inputAction.Session.ToString());
            xmlelem.SetAttribute("FrontID", inputAction.FrontID.ToString());
            xmlelem.SetAttribute("OrderSysID", inputAction.OrderSysID);
            xmlelem.SetAttribute("ExchangeID", inputAction.ExchangeID);
            xmlelem.SetAttribute("InstrumentID", inputAction.InstrumentID);

            xmldoc.AppendChild(xmlelem);

            strXml = xmldoc.InnerXml;    
        }

        //将xml字符串转换成行情MarketDetph结构
        public void Transfer(string strXml, ref MarketDetph md)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);

            //取得节点名为Q的XmlNode集合  
            XmlNodeList xxList = doc.GetElementsByTagName("Q");
            foreach (XmlNode xxNode in xxList)
            {
                XmlNodeList childList = xxNode.ChildNodes;
                //取得InstrumentID下的子节点集合     
                foreach (XmlNode xxxxNode in xxList)
                {
                    // 合约ID
                    md.instrumentId = xxxxNode.Attributes["InstrumentID"].Value;
                    // 交易所ID
                    md.exchangeId = xxxxNode.Attributes["ExchangeID"].Value;
                    // 交易日
                    md.tradingday = xxxxNode.Attributes["TradingDay"].Value;
                    // 更新时间
                    md.updateTime = xxxxNode.Attributes["UpdateTime"].Value;
                    // 更新毫秒
                    md.updateMS = Int32.Parse(xxxxNode.Attributes["UpdateMillisec"].Value);
                    // 最新价
                    string szTemp = xxxxNode.Attributes["LastPrice"].Value;
                    if (szTemp.Length == 0)
                        md.latestPrice = CommonDef.dInvalidValue;
                    else
                        md.latestPrice = double.Parse(xxxxNode.Attributes["LastPrice"].Value);                    
                    // 开盘价
                    szTemp = xxxxNode.Attributes["OpenPrice"].Value;
                    if (szTemp.Length == 0)
                        md.open = CommonDef.dInvalidValue;
                    else
                        md.open = double.Parse(xxxxNode.Attributes["OpenPrice"].Value);  
                    // 最高价
                    szTemp = xxxxNode.Attributes["HighestPrice"].Value;
                    if (szTemp.Length == 0)
                        md.high = CommonDef.dInvalidValue;
                    else
                        md.high = double.Parse(xxxxNode.Attributes["HighestPrice"].Value);                    
                    // 最低价
                    szTemp = xxxxNode.Attributes["LowestPrice"].Value;
                    if (szTemp.Length == 0)
                        md.low = CommonDef.dInvalidValue;
                    else
                        md.low = double.Parse(xxxxNode.Attributes["LowestPrice"].Value);
                    // 现量
                    md.latestVol = Int32.Parse(xxxxNode.Attributes["Volume"].Value);
                    // 持仓量
                    szTemp = xxxxNode.Attributes["LowestPrice"].Value;
                    if (szTemp.Length == 0)
                        md.low = CommonDef.iInvalidValue;
                    else
                        md.openInt = (int)double.Parse(xxxxNode.Attributes["OpenInterest"].Value); 
                    // 收盘价
                    szTemp = xxxxNode.Attributes["ClosePrice"].Value;
                    if (szTemp.Length == 0)
                        md.todayClosePrice = CommonDef.dInvalidValue;
                    else
                        md.todayClosePrice = double.Parse(xxxxNode.Attributes["ClosePrice"].Value);
                    // 成交金额
                    szTemp = xxxxNode.Attributes["Turnover"].Value;
                    if (szTemp.Length == 0)
                        md.turnover = CommonDef.dInvalidValue;
                    else
                        md.turnover = double.Parse(xxxxNode.Attributes["Turnover"].Value);
                    // 均价
                    szTemp = xxxxNode.Attributes["AveragePrice"].Value;
                    if (szTemp.Length == 0)
                        md.avgLatestPrice = CommonDef.dInvalidValue;
                    else
                        md.avgLatestPrice = double.Parse(xxxxNode.Attributes["AveragePrice"].Value);
                    // 昨结算价
                    szTemp = xxxxNode.Attributes["PreSettlementPrice"].Value;
                    if (szTemp.Length == 0)
                        md.preSettlementPrice = CommonDef.dInvalidValue;
                    else
                        md.preSettlementPrice = double.Parse(xxxxNode.Attributes["PreSettlementPrice"].Value);
                    // 昨收价
                    szTemp = xxxxNode.Attributes["PreClosePrice"].Value;
                    if (szTemp.Length == 0)
                        md.yesterdayClosePrice = CommonDef.dInvalidValue;
                    else
                        md.yesterdayClosePrice = double.Parse(xxxxNode.Attributes["PreClosePrice"].Value);
                    // 昨持仓量
                    md.yesterdayOpenInt = (int)double.Parse(xxxxNode.Attributes["PreOpenInterest"].Value); 
                    // 结算价
                    szTemp = xxxxNode.Attributes["SettlementPrice"].Value;
                    if (szTemp.Length == 0)
                        md.settlementPrice = CommonDef.dInvalidValue;
                    else
                        md.settlementPrice = double.Parse(xxxxNode.Attributes["SettlementPrice"].Value);
                    // 涨停价
                    szTemp = xxxxNode.Attributes["UpperLimitPrice"].Value;
                    if (szTemp.Length == 0)
                        md.upperLimitPrice = CommonDef.dInvalidValue;
                    else
                        md.upperLimitPrice = double.Parse(xxxxNode.Attributes["UpperLimitPrice"].Value);
                    // 跌停价
                    szTemp = xxxxNode.Attributes["LowerLimitPrice"].Value;
                    if (szTemp.Length == 0)
                        md.lowerLimitPrice = CommonDef.dInvalidValue;
                    else
                        md.lowerLimitPrice = double.Parse(xxxxNode.Attributes["LowerLimitPrice"].Value);
                    // 买一价
                    szTemp = xxxxNode.Attributes["BidPrice1"].Value;
                    if (szTemp.Length == 0)
                        md.bidPrice1 = CommonDef.dInvalidValue;
                    else
                        md.bidPrice1 = double.Parse(xxxxNode.Attributes["BidPrice1"].Value);
                    // 卖一量
                    md.bidVol1 = Int32.Parse(xxxxNode.Attributes["BidVolume1"].Value);
                    // 卖一价
                    szTemp = xxxxNode.Attributes["AskPrice1"].Value;
                    if (szTemp.Length == 0)
                        md.askPrice1 = CommonDef.dInvalidValue;
                    else
                        md.askPrice1 = double.Parse(xxxxNode.Attributes["AskPrice1"].Value);
                    // 卖一量
                    md.askVol1 = Int32.Parse(xxxxNode.Attributes["AskVolume1"].Value);
                    // 买二价
                    //md.bidPrice2 = double.Parse(xxxxNode.Attributes["BidPrice2"].Value);
                    // 买二量
                    //md.bidVol2 = Int32.Parse(xxxxNode.Attributes["BidVolume2"].Value);
                    // 卖二价
                    //md.askPrice2 = double.Parse(xxxxNode.Attributes["AskPrice2"].Value);
                    // 卖二量
                    //md.askVol2 = Int32.Parse(xxxxNode.Attributes["AskVolume2"].Value);
                    // 买三价
                    //md.bidPrice3 = double.Parse(xxxxNode.Attributes["BidPrice3"].Value);
                    // 买三量
                    //md.bidVol3 = Int32.Parse(xxxxNode.Attributes["BidVolume3"].Value);
                    // 卖三价
                    //md.askPrice3 = double.Parse(xxxxNode.Attributes["AskPrice3"].Value);
                    // 卖三量
                    //md.askVol3 = Int32.Parse(xxxxNode.Attributes["AskVolume3"].Value);
                    // 买四价
                    //md.bidPrice4 = double.Parse(xxxxNode.Attributes["BidPrice4"].Value);
                    // 买四量
                    //md.bidVol4 = Int32.Parse(xxxxNode.Attributes["BidVolume4"].Value);
                    // 卖四价
                    //md.askPrice4 = double.Parse(xxxxNode.Attributes["AskPrice4"].Value);
                    // 卖四量
                    //md.askVol4 = Int32.Parse(xxxxNode.Attributes["AskVolume4"].Value);
                    // 买五价
                    //md.bidPrice5 = double.Parse(xxxxNode.Attributes["BidPrice5"].Value);
                    // 买五量
                    //md.bidVol5 = Int32.Parse(xxxxNode.Attributes["BidVolume5"].Value);
                    // 卖五价
                    //md.askPrice5 = double.Parse(xxxxNode.Attributes["AskPrice5"].Value);
                    // 卖五量
                    //md.askVol5 = Int32.Parse(xxxxNode.Attributes["AskVolume5"].Value);
                }
            }
        }

        //将xml字符串转换成报单(兼容下单错误返回)OrderInfo结构
        public void Transfer(string strXml, ref OrderInfo order)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);

            //取得节点名为Q的XmlNode集合  
            XmlNodeList xxList = doc.GetElementsByTagName("O");
            foreach (XmlNode xxNode in xxList)
            {
                XmlNodeList childList = xxNode.ChildNodes;
                //取得InstrumentID下的子节点集合     
                foreach (XmlNode xxxxNode in xxList)
                {
                    ///投资者代码
                    order.InvestorID = xxxxNode.Attributes["InvestorID"].Value;
                    ///合约代码
                    order.InstrumentID = xxxxNode.Attributes["InstrumentID"].Value;
                    ///报单引用
                    order.OrderRef = xxxxNode.Attributes["OrderRef"].Value;
                    ///报单价格条件
                    order.OrderPriceType = char.Parse(xxxxNode.Attributes["OrderPriceType"].Value);
                    ///买卖方向
                    order.Direction = char.Parse(xxxxNode.Attributes["Direction"].Value);
                    ///组合开平标志
                    order.CombOffsetFlag = xxxxNode.Attributes["CombOffsetFlag"].Value;
                    ///组合投机套保标志
                    order.CombHedgeFlag = xxxxNode.Attributes["CombHedgeFlag"].Value;
                    ///价格
                    order.LimitPrice = double.Parse(xxxxNode.Attributes["LimitPrice"].Value);
                    ///数量
                    order.VolumeTotalOriginal = Int32.Parse(xxxxNode.Attributes["VolumeTotalOriginal"].Value);
                    ///成交量类型
                    order.VolumeCondition = char.Parse(xxxxNode.Attributes["VolumeCondition"].Value);
                    ///最小成交量
                    order.MinVolume = Int32.Parse(xxxxNode.Attributes["MinVolume"].Value);
                    ///请求编号
                    order.RequestID = Int32.Parse(xxxxNode.Attributes["RequestID"].Value);
                    ///本地报单编号
                    order.OrderLocalID = xxxxNode.Attributes["OrderLocalID"].Value;
                    ///交易所代码
                    order.ExchangeID = xxxxNode.Attributes["ExchangeID"].Value;
                    ///报单提交状态
                    order.OrderSubmitStatus = char.Parse(xxxxNode.Attributes["OrderSubmitStatus"].Value);
                    ///报单提示序号
                    order.NotifySequence = Int32.Parse(xxxxNode.Attributes["NotifySequence"].Value);
                    ///交易日
                    order.TradingDay = xxxxNode.Attributes["TradingDay"].Value;
                    ///报单编号
                    order.OrderSysID = xxxxNode.Attributes["OrderSysID"].Value;
                    ///报单状态
                    order.OrderStatus = char.Parse(xxxxNode.Attributes["OrderStatus"].Value);
                    ///今成交数量
                    order.VolumeTraded = Int32.Parse(xxxxNode.Attributes["VolumeTraded"].Value);
                    ///剩余数量
                    order.VolumeTotal = Int32.Parse(xxxxNode.Attributes["VolumeTotal"].Value);
                    ///报单日期
                    order.InsertDate = xxxxNode.Attributes["InsertDate"].Value;
                    ///委托时间
                    order.InsertTime = xxxxNode.Attributes["InsertTime"].Value;
                    ///最后修改时间
                    order.UpdateTime = xxxxNode.Attributes["UpdateTime"].Value;
                    ///撤销时间
                    order.CancelTime = xxxxNode.Attributes["CancelTime"].Value;
                    ///序号
                    order.SequenceNo = Int32.Parse(xxxxNode.Attributes["SequenceNo"].Value);
                    ///前置编号
                    order.FrontID = Int32.Parse(xxxxNode.Attributes["FrontID"].Value);
                    ///会话编号
                    order.SessionID = Int32.Parse(xxxxNode.Attributes["SessionID"].Value);
                    ///状态信息
                    order.StatusMsg = xxxxNode.Attributes["StatusMsg"].Value;
                    ///经纪公司报单编号
                    order.BrokerOrderSeq = Int32.Parse(xxxxNode.Attributes["BrokerOrderSeq"].Value);
                    ///相关报单
                    order.RelativeOrderSysID = xxxxNode.Attributes["RelativeOrderSysID"].Value;
                }
            }
        }

        //将xml字符串转换成成交TradeInfo结构
        public void Transfer(string strXml, ref TradeInfo trade)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);

            //取得节点名为Q的XmlNode集合  
            XmlNodeList xxList = doc.GetElementsByTagName("T");
            foreach (XmlNode xxNode in xxList)
            {
                XmlNodeList childList = xxNode.ChildNodes;
                //取得InstrumentID下的子节点集合     
                foreach (XmlNode xxxxNode in xxList)
                {
                    ///经纪公司代码
                    trade.BrokerID = xxxxNode.Attributes["BrokerID"].Value;
                    ///投资者代码
                    trade.InvestorID = xxxxNode.Attributes["InvestorID"].Value;
                    ///合约代码
                    trade.InstrumentID = xxxxNode.Attributes["InstrumentID"].Value;
                    ///报单引用
                    trade.OrderRef = xxxxNode.Attributes["OrderRef"].Value;
                    ///交易所代码
                    trade.ExchangeID = xxxxNode.Attributes["ExchangeID"].Value;
                    ///成交编号
                    trade.TradeID = xxxxNode.Attributes["TradeID"].Value;
                    ///买卖方向
                    trade.Direction = char.Parse(xxxxNode.Attributes["Direction"].Value);
                    ///报单编号
                    trade.OrderSysID = xxxxNode.Attributes["OrderSysID"].Value;
                    ///开平标志
                    trade.OffsetFlag = char.Parse(xxxxNode.Attributes["OffsetFlag"].Value);
                    ///投机套保标志
                    trade.HedgeFlag = char.Parse(xxxxNode.Attributes["HedgeFlag"].Value);
                    ///价格
                    trade.Price = double.Parse(xxxxNode.Attributes["Price"].Value);
                    ///数量
                    trade.Volume = Int32.Parse(xxxxNode.Attributes["Volume"].Value);
                    ///成交时期
                    trade.TradeDate = xxxxNode.Attributes["TradeDate"].Value;
                    ///成交时间
                    trade.TradeTime = xxxxNode.Attributes["TradeTime"].Value;
                    ///成交类型
                    trade.TradeType = char.Parse(xxxxNode.Attributes["TradeType"].Value);
                    ///本地报单编号
                    trade.OrderLocalID = xxxxNode.Attributes["OrderLocalID"].Value;
                    ///序号
                    trade.SequenceNo = Int32.Parse(xxxxNode.Attributes["SequenceNo"].Value);
                    ///交易日
                    trade.TradingDay = xxxxNode.Attributes["TradingDay"].Value;
                    ///经纪公司报单编号
                    trade.BrokerOrderSeq = Int32.Parse(xxxxNode.Attributes["BrokerOrderSeq"].Value);
                }
            }
        }

        //将xml字符串转换成撤单错误返回ActionInput结构
        public void Transfer(string strXml, ref ActionInput actionInput)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);

            //取得节点名为QA的XmlNode集合  
            XmlNodeList xxList = doc.GetElementsByTagName("OA");
            foreach (XmlNode xxNode in xxList)
            {
                XmlNodeList childList = xxNode.ChildNodes;
                //取得InstrumentID下的子节点集合     
                foreach (XmlNode xxxxNode in xxList)
                {
                    ///经纪公司代码
                    actionInput.BrokerID = xxxxNode.Attributes["BrokerID"].Value;
                    ///投资者代码
                    actionInput.InvestorID = xxxxNode.Attributes["InvestorID"].Value;
                    ///报单操作引用
                    actionInput.OrderActionRef = Int32.Parse(xxxxNode.Attributes["OrderActionRef"].Value);
                    ///报单引用
                    actionInput.OrderRef = xxxxNode.Attributes["OrderRef"].Value;
                    ///请求编号
                    actionInput.RequestID = Int32.Parse(xxxxNode.Attributes["RequestID"].Value);
                    ///前置编号
                    actionInput.FrontID = Int32.Parse(xxxxNode.Attributes["FrontID"].Value);
                    ///会话编号
                    actionInput.SessionID = Int32.Parse(xxxxNode.Attributes["SessionID"].Value);
                    ///交易所代码
                    actionInput.ExchangeID = xxxxNode.Attributes["ExchangeID"].Value;
                    ///报单编号
                    actionInput.OrderSysID = xxxxNode.Attributes["OrderSysID"].Value;
                    ///操作标志
                    actionInput.ActionFlag = char.Parse(xxxxNode.Attributes["ActionFlag"].Value);
                    ///价格
                    actionInput.LimitPrice = double.Parse(xxxxNode.Attributes["LimitPrice"].Value);
                    ///数量变化
                    actionInput.VolumeChange = Int32.Parse(xxxxNode.Attributes["VolumeChange"].Value);
                    ///用户代码
                    actionInput.UserID = xxxxNode.Attributes["UserID"].Value;
                    ///合约代码
                    actionInput.InstrumentID = xxxxNode.Attributes["InstrumentID"].Value;
                }
            }
        }

        //将xml字符串转换成资金TradingAccount结构
        public void Transfer(string strXml, ref TradingAccount tradingAccount)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);

            //取得节点名为Fund的XmlNode集合  
            XmlNodeList xxList = doc.GetElementsByTagName("Fund");
            foreach (XmlNode xxNode in xxList)
            {
                XmlNodeList childList = xxNode.ChildNodes;
                //取得InstrumentID下的子节点集合     
                foreach (XmlNode xxxxNode in xxList)
                {
                    ///经纪公司代码
                    tradingAccount.BrokerID = xxxxNode.Attributes["BrokerID"].Value;
                    ///投资者帐号
                    tradingAccount.AccountID = xxxxNode.Attributes["AccountID"].Value;
                    ///入金金额
                    tradingAccount.Deposit = double.Parse(xxxxNode.Attributes["Deposit"].Value);
                    ///出金金额
                    tradingAccount.Withdraw = double.Parse(xxxxNode.Attributes["Withdraw"].Value);
                    ///冻结的保证金
                    tradingAccount.FrozenMargin = double.Parse(xxxxNode.Attributes["FrozenMargin"].Value);
                    ///冻结的资金
                    tradingAccount.FrozenCash = double.Parse(xxxxNode.Attributes["FrozenCash"].Value);
                    ///冻结的手续费
                    tradingAccount.FrozenCommission = double.Parse(xxxxNode.Attributes["FrozenCommission"].Value);
                    ///当前保证金总额
                    tradingAccount.CurrMargin = double.Parse(xxxxNode.Attributes["CurrMargin"].Value);
                    ///资金差额
                    tradingAccount.CashIn = double.Parse(xxxxNode.Attributes["CashIn"].Value);
                    ///手续费
                    tradingAccount.Commission = double.Parse(xxxxNode.Attributes["Commission"].Value);
                    ///平仓盈亏
                    tradingAccount.CloseProfit = double.Parse(xxxxNode.Attributes["CloseProfit"].Value);
                    ///持仓盈亏
                    tradingAccount.PositionProfit = double.Parse(xxxxNode.Attributes["PositionProfit"].Value);
                    ///期货结算准备金
                    tradingAccount.Balance = double.Parse(xxxxNode.Attributes["Balance"].Value);
                    ///可用资金
                    tradingAccount.Available = double.Parse(xxxxNode.Attributes["Available"].Value);
                    ///可取资金
                    tradingAccount.WithdrawQuota = double.Parse(xxxxNode.Attributes["WithdrawQuota"].Value);
                    ///基本准备金
                    tradingAccount.Reserve = double.Parse(xxxxNode.Attributes["Reserve"].Value);
                    ///交易日
                    tradingAccount.TradingDay = xxxxNode.Attributes["TradingDay"].Value;
                    ///信用额度
                    tradingAccount.Credit = double.Parse(xxxxNode.Attributes["Credit"].Value);
                    ///质押金额
                    tradingAccount.Mortgage = double.Parse(xxxxNode.Attributes["Mortgage"].Value);
                    ///交易所保证金
                    tradingAccount.ExchangeMargin = double.Parse(xxxxNode.Attributes["ExchangeMargin"].Value);
                    ///投资者交割保证金
                    tradingAccount.DeliveryMargin = double.Parse(xxxxNode.Attributes["DeliveryMargin"].Value);
                    ///交易所交割保证金
                    tradingAccount.ExchangeDeliveryMargin = double.Parse(xxxxNode.Attributes["ExchangeDeliveryMargin"].Value);
                    ///静态权益
                    tradingAccount.StaticProfit = double.Parse(xxxxNode.Attributes["StaticProfit"].Value);
                    ///动态权益
                    tradingAccount.DynamicProfit = double.Parse(xxxxNode.Attributes["DynamicProfit"].Value);
                    ///风险度
                    tradingAccount.RiskDegree = double.Parse(xxxxNode.Attributes["RiskDegree"].Value);
                }
            }
        }

        //将xml字符串转换成持仓PositionInfo结构
        public void Transfer(string strXml, ref PositionInfo position)
        {
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(strXml);            

            //取得节点名为Fund的XmlNode集合  
            XmlNodeList xxList = doc.GetElementsByTagName("P");
            foreach (XmlNode xxNode in xxList)
            {
                XmlNodeList childList = xxNode.ChildNodes;
                //取得InstrumentID下的子节点集合     
                foreach (XmlNode xxxxNode in xxList)
                {
                    ///合约代码
                    position.InstrumentID = xxxxNode.Attributes["InstrumentID"].Value;
                    ///经纪公司代码
                    position.BrokerID = xxxxNode.Attributes["BrokerID"].Value;
                    ///投资者代码
                    position.InvestorID = xxxxNode.Attributes["InvestorID"].Value;
                    ///持仓多空方向
                    position.PosiDirection = char.Parse(xxxxNode.Attributes["PosiDirection"].Value);
                    ///投机套保标志
                    position.HedgeFlag = char.Parse(xxxxNode.Attributes["HedgeFlag"].Value);
                    ///交易日
                    position.TradingDay = xxxxNode.Attributes["TradingDay"].Value;

                    ///当前净持仓
                    position.Position = Int32.Parse(xxxxNode.Attributes["Position"].Value);
                    ///今日持仓(指今日净持仓)
                    position.TodayPosition = Int32.Parse(xxxxNode.Attributes["TodayPosition"].Value);
                    ///上日持仓(指历史持仓总和，包括今日平掉的部分)
                    position.YdPosition = Int32.Parse(xxxxNode.Attributes["YdPosition"].Value);

                    ///开仓量(指今日开仓量)
                    position.OpenVolume = Int32.Parse(xxxxNode.Attributes["OpenVolume"].Value);
                    ///平仓量(指今日平仓量)
                    position.CloseVolume = Int32.Parse(xxxxNode.Attributes["CloseVolume"].Value);
                    ///开仓金额(指今开仓金额)
                    position.OpenAmount = double.Parse(xxxxNode.Attributes["OpenAmount"].Value);
                    ///平仓金额(指今平仓金额)
                    position.CloseAmount = double.Parse(xxxxNode.Attributes["CloseAmount"].Value);
                    ///持仓成本(当前净持仓的持仓成本)
                    position.PositionCost = double.Parse(xxxxNode.Attributes["PositionCost"].Value);
                    ///开仓成本(未平持仓明细的开仓金额的总和(只计算未平部分))
                    position.OpenCost = double.Parse(xxxxNode.Attributes["OpenCost"].Value);

                    ///多头冻结(多头持仓: 开买冻结；空头持仓：平买冻结)
                    position.LongFrozen = Int32.Parse(xxxxNode.Attributes["LongFrozen"].Value);
                    ///空头冻结(多头持仓: 平卖冻结；空头持仓：开卖冻结)
                    position.ShortFrozen = Int32.Parse(xxxxNode.Attributes["ShortFrozen"].Value);
                    ///多头冻结金额
                    position.LongFrozenAmount = double.Parse(xxxxNode.Attributes["LongFrozenAmount"].Value);
                    ///空头冻结金额
                    position.ShortFrozenAmount = double.Parse(xxxxNode.Attributes["ShortFrozenAmount"].Value);
                    ///冻结的保证金
                    position.FrozenMargin = double.Parse(xxxxNode.Attributes["FrozenMargin"].Value);
                    ///冻结的手续费
                    position.FrozenCommission = double.Parse(xxxxNode.Attributes["FrozenCommission"].Value);
                    ///冻结的资金
                    position.FrozenCash = double.Parse(xxxxNode.Attributes["FrozenCash"].Value);

                    ///手续费
                    position.Commission = double.Parse(xxxxNode.Attributes["Commission"].Value);
                    ///占用的保证金
                    position.UseMargin = double.Parse(xxxxNode.Attributes["UseMargin"].Value);
                    try
                    {
                    ///保证金率
                    position.MarginRateByMoney = double.Parse(xxxxNode.Attributes["MarginRateByMoney"].Value);
                    ///保证金率(按手数)
                    position.MarginRateByVolume = double.Parse(xxxxNode.Attributes["MarginRateByVolume"].Value);
                     ///持仓盈亏(盯市)
                    position.PositionProfit = double.Parse(xxxxNode.Attributes["PositionProfit"].Value);
                    ///平仓盈亏(盯市)
                    position.CloseProfit = double.Parse(xxxxNode.Attributes["CloseProfit"].Value);

                   ///交易所保证金
                    position.ExchangeMargin = double.Parse(xxxxNode.Attributes["ExchangeMargin"].Value);

                    ///逐日盯市平仓盈亏
                    position.CloseProfitByDate = double.Parse(xxxxNode.Attributes["CloseProfitByDate"].Value);
                    ///逐笔对冲平仓盈亏
                    position.CloseProfitByTrade = double.Parse(xxxxNode.Attributes["CloseProfitByTrade"].Value);
                    }
                    catch (Exception e) { }

                    ///上次结算价
                    position.PreSettlementPrice = double.Parse(xxxxNode.Attributes["PreSettlementPrice"].Value);
                    ///本次结算价
                    position.SettlementPrice = double.Parse(xxxxNode.Attributes["SettlementPrice"].Value);

                    ///组合成交形成的持仓
                    position.CombPosition = Int32.Parse(xxxxNode.Attributes["CombPosition"].Value);
                    ///组合多头冻结
                    position.CombLongFrozen = Int32.Parse(xxxxNode.Attributes["CombLongFrozen"].Value);
                    ///组合空头冻结
                    position.CombShortFrozen = Int32.Parse(xxxxNode.Attributes["CombShortFrozen"].Value);

                    //交易所代码
                    position.ExchangeID = xxxxNode.Attributes["ExchangeID"].Value;
                    //浮动盈亏，所有持仓明细浮盈之和，就是逐笔对冲的持仓盈亏
                    position.PositionProfitByTrade = double.Parse(xxxxNode.Attributes["PositionProfitByTrade"].Value);
                    //盯市总盈亏，盯市平仓盈亏和持仓盈亏之和
                    position.TotalPositionProfitByDate = double.Parse(xxxxNode.Attributes["TotalPositionProfitByDate"].Value);

                    //可平量Position-TotalCloseOrderVolume
                    position.CanCloseVolume = Int32.Parse(xxxxNode.Attributes["CanCloseVolume"].Value);
                    //可平今量TodayPosition-CloseTodayOrderVolume
                    position.CanCloseTodayVolume = Int32.Parse(xxxxNode.Attributes["CanCloseTodayVolume"].Value);
                    //可平昨量YdPosition-CloseYesterdayOrderVolume
                    position.CanCloseydVolume = Int32.Parse(xxxxNode.Attributes["CanCloseydVolume"].Value);

                    //持仓均价(持仓成本/持仓量)
                    position.AveragePositionPrice = double.Parse(xxxxNode.Attributes["AveragePositionPrice"].Value);
                    //开仓均价((今日开仓成本+历史开仓成本)/总开仓量)
                    position.AverageOpenPrice = double.Parse(xxxxNode.Attributes["AverageOpenPrice"].Value);

                    //今平历史仓量
                    position.TodayCloseYdVolume = Int32.Parse(xxxxNode.Attributes["TodayCloseYdVolume"].Value);
                    //上日剩余仓位，从持仓明细计算
                    position.YdPositionRemain = Int32.Parse(xxxxNode.Attributes["YdPositionRemain"].Value);
                }
            }
        }
    }
}
