using System;
using System.Collections.Generic;

namespace RunSingleStrategy
{
    class EventList
    {
        public static Object obj = new Object();
        public static List<string> comm_EventList = new List<string>();

        public static void GetItem(ref List<string> refEventList)
        {
            lock (EventList.obj)
            {
                refEventList.Clear();
                foreach (string item in comm_EventList)
                {
                    refEventList.Add(item);                    
                }

                comm_EventList.Clear();
            }
        }
    }
}
