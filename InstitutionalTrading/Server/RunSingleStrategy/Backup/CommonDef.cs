using System.Collections.Generic;

namespace RunSingleStrategy
{  
    public struct tagMSG
    {
        public int hwnd;
        public uint message;
        public int wParam;
        public long lParam;
        public uint time;
        public int pt;
    }

    public class CommonDef
    {     
        //Redis相关
        public static string IP = "10.11.112.140";
        public static int    Port = 6379;

        //publish Channel 
        public const string OrderInputChannel   = "OrderInput";
        public const string OrderActionChannel  = "OrderAction";

        //save subscribe Channel 
        public static List<string> subscribeChannels = new List<string>();

        //策略
        public const string FoctoryType = "com.cifco.zqstp.strategy.Strategy.Customer.Factory";
        public const string FoctoryMethod = "CreateObject";

        //策略实例启停用判断
        public static bool StrategyInstenceStatus = true;

        //MainForm
        public const string WindowName = "MainForm";

  
        //退出进程
        public const int WM_SYSCOMMAND = 0x0112;
        public const int SC_CLOSE = 0xF060;   
        

        

        //用户自定义消息
        public const int WM_USER = 5000;

        public const int WM_USER_TRADER_STATUS              = WM_USER + 1; //交易员是否离线
        public const int WM_USER_STRATEGY_STATUS            = WM_USER + 2; //策略启用停用
        public const int WM_USER_STRATEGY_INSTENCE_START    = WM_USER + 3; //策略实例开始
        public const int WM_USER_STRATEGY_INSTENCE_PAUSE    = WM_USER + 4; //策略实例暂停        
        public const int WM_USER_QUOT                       = WM_USER + 5; //行情
        public const int WM_USER_ORDER                      = WM_USER + 6; //报单
        public const int WM_USER_TRADE                      = WM_USER + 7; //成交
        public const int WM_USER_ORDER_ERROR                = WM_USER + 8; //下单错误返回
        public const int WM_USER_ORDER_ACTION               = WM_USER + 9; //撤单错误返回
    }

    public class StrategyInfo
    {
        public static StrategyInfo getInstence()
        {
            if (static_StrategyInfo == null)
                return new StrategyInfo();
            else
                return static_StrategyInfo;
        }

        public static StrategyInfo static_StrategyInfo = null;

        private static string strategyName;
        private static string traderName;
        private static string parameter;
        private static List<string> instruments = new List<string>(); 
        private static string sessionID;
        private static string frontID;
        private static string orderRef;
        private static string startTimer;
        private static string timerSpan;
        

        public static string StrategyName
        {
            get { return strategyName; }
            set { strategyName = value; }
        }
        public static string TraderName
        {
            get { return traderName; }
            set { traderName = value; }
        }
        public static string Parameter
        {
            get { return parameter; }
            set { parameter = value; }
        }
        public static List<string> Instruments
        {
            get { return instruments; }
            set { instruments = value; }
        }
        public static string SessionID
        {
            get { return sessionID; }
            set { sessionID = value; }
        }
        public static string FrontID
        {
            get { return frontID; }
            set { frontID = value; }
        }
        public static string OrderRef
        {
            get { return orderRef; }
            set { orderRef = value; }
        }
        public static string StartTimer
        {
            get { return startTimer; }
            set { startTimer = value; }
        }
        public static string TimerSpan
        {
            get { return timerSpan; }
            set { timerSpan = value; }
        }
    }
}

