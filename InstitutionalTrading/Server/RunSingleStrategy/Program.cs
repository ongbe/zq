using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Threading;
using com.cifco.zqstp.Market;
using com.cifco.zqstp.Misc;
using com.cifco.zqstp.strategy.Strategy;
using com.cifco.zqstp.Trader;
using ServiceStack.Redis;
using System.Text;
using System.Windows.Forms;



namespace RunSingleStrategy
{
    class Program
    {
        [DllImport("kernel32.dll")]
        private static extern int GetCurrentThreadId();

        [DllImport("user32.dll")]
        private static extern int GetMessage( ref tagMSG lpMsg, //指向MSG结构的指针，该结构从线程的消息队列里接收消息信息;
                                              int hwnd, //取得其消息的窗口的句柄。这是一个有特殊含义的值（NULL）。GetMessage为任何属于调用线程的窗口检索消息;　　　　　　　　　　　　　　　　　　　　　　　　　　　　 　　　　　　　　　　　　　　　　 int wMsgFilterMin, //指定被检索的最小消息值的整数
                                              int wMsgFilterMin, //指定被检索的最小消息值的整数
                                              int wMsgFilterMax); //指定被检索的最大消息值的整数  
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);

        private static CStrategy Strategy;
        private static RedisClient Redis_Publish;
        private static RedisClient Redis_Subscribe;
        private static CommonTool Tool = new CommonTool();
        private static XmlOpration XmlOpr = new XmlOpration();        
        
 
        //心跳定时器
        private static System.Timers.Timer Timer_HeartBeat;
        //策略定时器
        private static System.Timers.Timer Timer_Strategy;

        //定时器处理
        private static void timer_Tick(object sender, EventArgs e)
        {
            string HeatBeatChannel = RunSingleStrategy.StrategyInfo.TraderName + ":" + RunSingleStrategy.StrategyInfo.StrategyName;
            string HeatBeatMessage = "<HB UserName=\"" + RunSingleStrategy.StrategyInfo.TraderName + "\" StrategyName=\"" + RunSingleStrategy.StrategyInfo.StrategyName + "\" />";

            //Publish到redis下单通道 
            Tool.Log("Send heartbeat '{0}' to channel '{1}'", HeatBeatMessage, HeatBeatChannel);
            Redis_Publish.PublishMessage(HeatBeatChannel, HeatBeatMessage);
        }
        private static void timer_Strategy_Tick(object sender, EventArgs e)
        {
            //当有计时器并且不空转条件下
            if (CommonDef.StrategyInstenceTimerStatus && CommonDef.StrategyStatus && CommonDef.StrategyInstenceRunAfterOfflineStatus)
            {
                Strategy.OnTimer();
                Console.WriteLine("定时器触发\n");
            }
        }
        
        private static string IniReadString(string inipath, string Section, string Key)
        { 
            StringBuilder temp = new StringBuilder(500); 
            int i = GetPrivateProfileString(Section, Key, "", temp, 500, inipath); 
            return temp.ToString();
        }

        

        /* 参数格式
        const string arg =
                                "<strategy><strategyName>MATrend</strategyName>" +
                                      "<traderName>0003</traderName>" +
                                      "<parameter>" +
                                          "<strategy id=\'MATrend\' name=\'均线穿越\' version=\'1.0\'>" +
                                          "<description>这是关于MATrend策略的描述</description>" +
                                          "<params>" +
                                              "<param type=\'int\' name=\'p1\' direction=\'INOUT\' appearmode=\'文本框\' readonly=\'False\' title=\'策略参数一\' default=\'1\'></param>" +
                                              "<param type=\'double\' name=\'p2\' direction=\'INOUT\' appearmode=\'文本框\' readonly=\'False\' title=\'策略参数二\' default=\'1.5\'></param>" +
                                          "</params>" +
                                          "</strategy>" +
                                      "</parameter>" +
                                      "<instrument>IF1404</instrument>" +
                                      "<sessionID>10000</sessionID>" +
                                      "<frontID>20000</frontID>" +
                                      "<orderRef>1</orderRef>" +
                                      "<startTimer>1</startTimer>" +
                                      "<timerSpan>100</timerSpan>" +
                                  "</strategy>";
*/
        
        static void Main(string[] args)
        {
            //MessageBox.Show("RunSingleStrategy.exe");
            /*参数判断*/
           if (args.Length <= 0)
           {
               Console.WriteLine("没有传入参数");
               Console.ReadLine();
               return;
           }
           if (args.Length > 0)
           {
               Console.WriteLine("传入的参数为：\n{0}", args[0]);                
           }

            string hostName = System.Reflection.Assembly.GetExecutingAssembly().Location;
            hostName = hostName.Substring(0, hostName.LastIndexOf('\\')) + "\\config.ini";
            CommonDef.IP = IniReadString(hostName, "IP", "Redis_IP");
            CommonDef.Port = Convert.ToInt32(IniReadString(hostName, "Port", "Redis_Port"));
            
            StrategyInfo strategyInfo = StrategyInfo.getInstence();            
            XmlOpr.AnalyzeXmlParam(args[0], ref strategyInfo);                       

            Redis_Publish = new RedisClient(CommonDef.IP, CommonDef.Port);
            Redis_Subscribe = new RedisClient(CommonDef.IP, CommonDef.Port);

            string szDllName = RunSingleStrategy.StrategyInfo.StrategyName + ".dll";
            //动态加载策略程序集DLL     
            Assembly ass = Assembly.LoadFrom(szDllName);
            Type type = ass.GetType(CommonDef.FoctoryType);
            //创建工厂实例 
            Object oFactory = System.Activator.CreateInstance(type);

            DateTime dt = DateTime.Now;
            string logPath = "";

            if (RunSingleStrategy.StrategyInfo.SaveHistory && RunSingleStrategy.StrategyInfo.HistoryPath.Length > 0)
                logPath = RunSingleStrategy.StrategyInfo.HistoryPath + "log.txt";
            else
                logPath = hostName.Substring(0, hostName.LastIndexOf('\\')) + "\\"
                         + RunSingleStrategy.StrategyInfo.StrategyName
                         + "_" + dt.ToShortDateString() + "_log.txt";
            Log.PathFile = logPath;
            Log.Save = true;
            StoreIndicators.PathFile = RunSingleStrategy.StrategyInfo.HistoryPath+"indicatorsdata.csv";
            StoreIndicators.Save = RunSingleStrategy.StrategyInfo.SaveHistory;

            //获取函数
            MethodInfo mi = type.GetMethod(CommonDef.FoctoryMethod);
            //创建策略实例
            Strategy = (CStrategy)mi.Invoke(oFactory, ParseParams.ParseStrategyParams(StrategyInfo.Parameter));
            //定义接口对象
            IOrderServiceEx orderService = new IOrderServiceEx(Redis_Publish);
            //用接口实例初始化策略
            Strategy.ResetOrderService(orderService);
            Strategy.ResetTimer(Int32.Parse(RunSingleStrategy.StrategyInfo.StartTimer) == 1, 
                    Int32.Parse(RunSingleStrategy.StrategyInfo.TimerSpan));
            Strategy.Initialize(RunSingleStrategy.StrategyInfo.Instruments.ToArray());
            Strategy.InitIndicators(RunSingleStrategy.StrategyInfo.Instruments.ToArray());


            //创建定时器
            Timer_HeartBeat = new System.Timers.Timer();
            Timer_HeartBeat.Elapsed += new System.Timers.ElapsedEventHandler(timer_Tick);
            Timer_HeartBeat.Interval = 5000;
            Timer_HeartBeat.AutoReset = true;   //设置是执行一次（false）还是一直执行(true)；   
            Timer_HeartBeat.Enabled = true;     //是否执行System.Timers.Timer.Elapsed事件； 

            //创建策略定时器
            if (Int32.Parse(RunSingleStrategy.StrategyInfo.StartTimer)==1)
            {
                Timer_Strategy = new System.Timers.Timer();
                Timer_Strategy.Elapsed += new System.Timers.ElapsedEventHandler(timer_Strategy_Tick);
                Timer_Strategy.Interval = Int32.Parse(RunSingleStrategy.StrategyInfo.TimerSpan) * 1000;
                Timer_Strategy.AutoReset = true;
                Timer_Strategy.Enabled = true;

                Log.Out("创建策略定时器");
                Console.WriteLine("创建策略定时器 \n"); 
            }

            //创建线程类
            int _NewThreadId = GetCurrentThreadId();
            SubPubThread spThread = new SubPubThread(_NewThreadId, Redis_Subscribe);
            Thread thread = new Thread(new ThreadStart(spThread.TreadProc));
            thread.Name = "PubSub";
            thread.Start();


            //消息处理
            tagMSG msg = new tagMSG();
            while (GetMessage(ref msg, 0, 0, 0) > 0)
            {
                if (msg.message == CommonDef.WM_USER)
                {
                    List<string> refEventList = new List<string>();
                    EventList.GetItem(ref refEventList);
                    if (refEventList.Count == 0)
                        continue;

                    foreach (string strItem in refEventList)
                    {
                        string strStyle = XmlOpr.GetType(strItem);
                        string strValue = null;


                        if (!CommonDef.StrategyStatus || !CommonDef.StrategyInstenceRunAfterOfflineStatus)
                        {
                            if (strStyle.Equals("SIR"))
                            {
                                //策略实例启用停用
                                strValue = XmlOpr.GetVaule(strItem, "run");
                                if (strValue.Equals("run"))
                                {
                                    Log.Out("策略实例又启用");
                                    Console.WriteLine("策略实例又启用 \n");

                                    CommonDef.StrategyStatus = true;
                                    CommonDef.StrategyInstenceRunAfterOfflineStatus = true;
                                }
                            }
                            
                            continue;
                        }


                        //此业务逻辑由策略服务器处理
                        //if (strStyle.Equals("UR"))
                        //{
                        //    //交易员是否离线
                        //    strValue = XmlOpr.GetVaule(strItem, "online");
                        //    if (strValue.Equals("on")) { Log.Out("交易员在线"); }
                        //    else if (strValue.Equals("off")) 
                        //    { 
                        //        Log.Out("交易员离线");
                        //        Console.WriteLine("交易员离线 \n"); 

                        //        CommonDef.StrategyInstenceStatus = false;  
                        //    }
                        //}
                        //else 
                        if (strStyle.Equals("SR"))
                        {
                            //策略启用停用
                            strValue = XmlOpr.GetVaule(strItem, "run");
                            if (strValue.Equals("run")) { Log.Out("策略启用"); }
                            else if (strValue.Equals("stop"))
                            {
                                Log.Out("策略停用");

                                //退出子线程
                                Redis_Subscribe.UnSubscribe(CommonDef.subscribeChannels.ToArray());

                                //客户端主动关闭
                                Redis_Publish.Shutdown();
                                Redis_Subscribe.Shutdown();

                                //强制关闭进程 (服务端处理)           
                                //CommonDef.KillProcess("RunSingleStrategy");

                                Log.Out("退出");
                                return;
                            }
                        }
                        else if (strStyle.Equals("SIR"))
                        {
                            //策略实例启用停用
                            strValue = XmlOpr.GetVaule(strItem, "run");
                            if (strValue.Equals("run")) 
                            {
                                Log.Out("策略实例启用");
                                Console.WriteLine("策略实例启用 \n");

                                CommonDef.StrategyStatus = true;
                            }
                            else if (strValue.Equals("stop"))
                            {
                                Log.Out("策略实例停用");
                                Console.WriteLine("策略实例停用 \n");

                                CommonDef.StrategyStatus = false;                             
                            }
                        }
                        else if (strStyle.Equals("ST"))
                        {
                            //策略定时器启用停用
                            strValue = XmlOpr.GetVaule(strItem, "run");
                            if (strValue.Equals("on"))
                            {
                                Log.Out("策略定时器启用");
                                CommonDef.StrategyInstenceTimerStatus = true;
                            }
                            else if (strValue.Equals("off"))
                            {
                                Log.Out("策略定时器停用");
                                CommonDef.StrategyInstenceTimerStatus = false;
                            }
                        }
                        else if (strStyle.Equals("Q"))
                        {
                            //行情                                
                            MarketDetph md = new MarketDetph();
                            XmlOpr.Transfer(strItem, ref md);
                            Strategy.OnMarketDepth(md);
                        }
                        else if (strStyle.Equals("O"))
                        {
                            Log.Out("报单:"+ strItem);
                            Console.WriteLine("报单 \n");

                            //报单
                            OrderInfo order = new OrderInfo();
                            XmlOpr.Transfer(strItem, ref order);
                            Strategy.OnOrder(order);
                        }
                        else if (strStyle.Equals("T"))
                        {
                            Log.Out("成交:" + strItem);
                            Console.WriteLine("成交 \n");

                            //成交
                            TradeInfo trade = new TradeInfo();
                            XmlOpr.Transfer(strItem, ref trade);
                            Strategy.OnTrade(trade);
                        }
                        else if (strStyle.Equals("OI"))
                        {
                            Log.Out("下单错误返回:" + strItem);
                            Console.WriteLine("下单错误返回 \n");

                            //下单错误返回
                            OrderInfo orderError = new OrderInfo();
                            XmlOpr.Transfer(strItem, ref orderError);
                            Strategy.OnOrderError(orderError);
                        }
                        else if (strStyle.Equals("OA"))
                        {
                            Log.Out("撤单错误返回:" + strItem);
                            Console.WriteLine("撤单错误返回 \n");

                            //撤单错误返回
                            ActionInput actionInput = new ActionInput();
                            XmlOpr.Transfer(strItem, ref actionInput);
                            Strategy.OnOrderAction(actionInput);
                        }
                        else if (strStyle.Equals("QT"))
                        {
                            Log.Out("收到用行情时间计算的Timer(QT)");
                            Console.WriteLine("收到用行情时间计算的Timer(QT) \n");             

                            Strategy.OnTimer();
                        }
                    }
                }
            }            
        }
    }
}
