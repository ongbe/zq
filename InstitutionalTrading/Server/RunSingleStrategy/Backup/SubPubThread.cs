using System;
using System.Runtime.InteropServices;
using ServiceStack.Redis;

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
            var channel_Order = "co:" + RunSingleStrategy.StrategyInfo.TraderName;
            var channel_Trade = "ct:" + RunSingleStrategy.StrategyInfo.TraderName;
            var channel_OrderActionInput = "coi:" + RunSingleStrategy.StrategyInfo.TraderName;
            var channel_OrderAction = "coa:" + RunSingleStrategy.StrategyInfo.TraderName;


            CommonDef.subscribeChannels.Add(channel_UserStatus);
            CommonDef.subscribeChannels.Add(channel_StrategyStatus);
            CommonDef.subscribeChannels.Add(channel_StrategyInstenceStatus);
            CommonDef.subscribeChannels.Add(channel_Order);
            CommonDef.subscribeChannels.Add(channel_Trade);
            CommonDef.subscribeChannels.Add(channel_OrderActionInput);
            CommonDef.subscribeChannels.Add(channel_OrderAction);            
            foreach(string isnt in RunSingleStrategy.StrategyInfo.Instruments.ToArray())
            {
                var channel_instrument = "cq:" + isnt;
                CommonDef.subscribeChannels.Add(channel_instrument);   
            }

            using (var subscription = Redis_SubPub.CreateSubscription())
            {
                subscription.OnSubscribe = channel =>
                {
                    Tool.Log("Subscribed to '{0}'", channel);
                };
                subscription.OnUnSubscribe = channel =>
                {
                    Tool.Log("UnSubscribed from '{0}'", channel);
                };
                subscription.OnMessage = (channel, msg) =>
                {
                    //Tool.Log("Received '{0}' from channel '{1}'", msg, channel);
                    Console.WriteLine("Received Data from channel '{0}'", channel);                    

                    lock (EventList.obj)
                    {
                        EventList.comm_EventList.Add(msg);
                    }

                    if (CommonDef.StrategyInstenceStatus)
                        PostThreadMessage(MainThreadId, CommonDef.WM_USER, 1, 1);
                 };

                Tool.Log("Start Listening On");
                subscription.SubscribeToChannels(CommonDef.subscribeChannels.ToArray()); //blocking
            }

            Tool.Log("SubPubThread normal quit");
        }
    }
}
