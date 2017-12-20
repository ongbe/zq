using System.Collections.Generic;
using System.Diagnostics;
using System;

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

        //策略是否空转判断
        public static bool StrategyStatus = true;

        //策略实例在交易员离线后的启停用判断
        public static bool StrategyInstenceRunAfterOfflineStatus = false;

        //策略实例计时器启停用判断
        public static bool StrategyInstenceTimerStatus = false;

        //MainForm
        public const string WindowName = "MainForm";

  
        //退出进程
        public const int WM_SYSCOMMAND = 0x0112;
        public const int SC_CLOSE = 0xF060;

        //无效值定义
        public const double dInvalidValue = 1.7976931348623158e+308;
        public const uint iInvalidValue = 0xfeeefeee;
        

        //用户自定义消息
        public const int WM_USER = 5000;        


        public static void KillProcess(string processName)
        {
            try
            {
                foreach (Process thisproc in Process.GetProcessesByName(processName))
                {
                    //立即杀死进程        
                    thisproc.Kill();
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        } 
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

        private static bool bSaveHistory = false;
        private static string strHistoryPath = "";
        

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
        public static bool SaveHistory
        {
            get { return bSaveHistory; }
            set { bSaveHistory = value; }
        }
        public static string HistoryPath
        {
            get { return strHistoryPath; }
            set { strHistoryPath = value; }
        }
    }
}

