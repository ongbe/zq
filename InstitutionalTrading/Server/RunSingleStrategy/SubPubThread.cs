using System;
using System.Runtime.InteropServices;
using ServiceStack.Redis;
using com.cifco.zqstp.Misc;

namespace RunSingleStrategy
{
    //定义订阅、发布线程类
    public class SubPubThread
    {

        [DllImport("user32.dll")]
        private static extern int PostThreadMessage(
                          int threadId, //线程标识
                          uint msg, //消息标识符
                          int wParam, //具体由消息决定
                          int lParam); //具体由消息决定
        

        private CommonTool Tool = new CommonTool();
        
        private int MainThreadId;
        private RedisClient Redis_SubPub;

        public SubPubThread(int _NewThreadId, RedisClient Redis_SubPub)
        {
            // TODO: Complete member initialization
            this.MainThreadId = _NewThreadId;
            this.Redis_SubPub = Redis_SubPub;
        } 


        public void TreadProc()
        {
            //订阅通道
            var channel_UserStatus = "cuof:" + RunSingleStrategy.StrategyInfo.TraderName;
            var channel_StrategyStatus = "csr:" + RunSingleStrategy.StrategyInfo.StrategyName;
            var channel_StrategyInstenceStatus = "csir:" + RunSingleStrategy.StrategyInfo.TraderName + ":" + RunSingleStrategy.StrategyInfo.StrategyName;
            var channel_StrategyTimerStatus = "st:" + RunSingleStrategy.StrategyInfo.StrategyName;
            var channel_Order = "co:" + RunSingleStrategy.StrategyInfo.TraderName;
            var channel_Trade = "ct:" + RunSingleStrategy.StrategyInfo.TraderName;
            var channel_OrderActionInput = "coi:" + RunSingleStrategy.StrategyInfo.TraderName;
            var channel_OrderAction = "coa:" + RunSingleStrategy.StrategyInfo.TraderName;


            CommonDef.subscribeChannels.Add(channel_UserStatus);
            CommonDef.subscribeChannels.Add(channel_StrategyStatus);
            CommonDef.subscribeChannels.Add(channel_StrategyInstenceStatus);
            CommonDef.subscribeChannels.Add(channel_StrategyTimerStatus);
            CommonDef.subscribeChannels.Add(channel_Order);
            CommonDef.subscribeChannels.Add(channel_Trade);
            CommonDef.subscribeChannels.Add(channel_OrderActionInput);
            CommonDef.subscribeChannels.Add(channel_OrderAction);            
            foreach(string isnt in RunSingleStrategy.StrategyInfo.Instruments.ToArray())
            {
                var channel_instrument = "cq:" + isnt;
                CommonDef.subscribeChannels.Add(channel_instrument);   
            }
            CommonDef.subscribeChannels.Add("cqt");

            string szLog;
            using (var subscription = Redis_SubPub.CreateSubscription())
            {
                subscription.OnSubscribe = channel =>
                {
                    //Tool.Log("Subscribed to '{0}'", channel);
                    szLog = "Subscribed to " + channel;
                    Log.Out(szLog);
                };
                subscription.OnUnSubscribe = channel =>
                {
                    //Tool.Log("UnSubscribed from '{0}'", channel);
                    szLog = "UnSubscribed from " + channel;
                    Log.Out(szLog);
                };
                subscription.OnMessage = (channel, msg) =>
                {
                    Tool.Log("Received '{0}' from channel '{1}'", msg, channel);
                    //Console.WriteLine("Received'{0}' from channel '{1}'", msg, channel);

                    //string szLog = "Received "+msg+" Data from channel " + channel;
                    //Log.Out(szLog);

                    //if (CommonDef.StrategyInstenceStatus)
                    //{
                        lock (EventList.obj)
                        {
                            EventList.comm_EventList.Add(msg);
                        }

                        PostThreadMessage(MainThreadId, CommonDef.WM_USER, 1, 1);
                    //}
                    //else
                    //{
                    //    XmlOpration XmlOpr = new XmlOpration(); 
                    //    string strStyle = XmlOpr.GetType(msg);
                    //    string strValue = null;
                    //    if (strStyle.Equals("SR"))
                    //    {
                    //        //策略启用停用
                    //        strValue = XmlOpr.GetVaule(msg, "run");
                    //        if (strValue.Equals("run")){}
                    //        else if (strValue.Equals("stop"))
                    //        {
                    //            //强制关闭进程             
                    //            CommonDef.KillProcess("RunSingleStrategy");

                    //            Log.Out("强制关闭进程2");
                    //        }
                    //    }
                    //    else if (strStyle.Equals("SIR"))
                    //    {
                    //        //策略实例启用停用
                    //        strValue = XmlOpr.GetVaule(msg, "run");
                    //        if (strValue.Equals("run"))
                    //        {
                    //            Log.Out("策略实例又重新启用");
                    //            CommonDef.StrategyInstenceStatus = true;
                    //        }                            
                    //    }
                    //}
                 };

                Log.Out("Start Listening On");
                subscription.SubscribeToChannels(CommonDef.subscribeChannels.ToArray()); //blocking
            }

            Log.Out("SubPubThread normal quit");   
        }
    }
}
