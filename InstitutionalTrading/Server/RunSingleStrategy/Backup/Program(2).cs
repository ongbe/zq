using System;
using System.Diagnostics;
using System.Xml;

namespace RunSingleStrategyTest
{
    class Program
    {
        public static bool StartProcess(string filename, string arg)
        {
            try
            {
                Process myprocess = new Process();
                ProcessStartInfo startInfo = new ProcessStartInfo(filename, arg);

                myprocess.StartInfo = startInfo;
                myprocess.StartInfo.UseShellExecute = true;
                myprocess.StartInfo.WorkingDirectory = filename.Replace(@"\RunSingleStrategy.exe", "");//设置要打开的进程的工作目录

                myprocess.Start();
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("启动应用程序时出错！原因：" + ex.Message);
            }
            return false;
        }



        static void Main(string[] args)
        {
            int sessionID = 10000;
            int frontID   = 20000;

            for (int i = 0; i < 1; i++)
            {
                sessionID++;
                frontID++;

                string arg =
                            "\"<strategy>" +
                            "<strategyName>MATrend</strategyName>" +
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
                            "<sessionID>"+sessionID.ToString()+"</sessionID>" +
                            "<frontID>"+frontID.ToString()+"</frontID>" +
                            "<orderRef>1</orderRef>" +
                            "<StartTimer>1</StartTimer>" +
                            "<TimerSpan>100</TimerSpan>" +
                            "</strategy>\"";
 
                StartProcess(@"F:\svn\InstitutionalTrading\Server\RunSingleStrategy\bin\Debug\RunSingleStrategy.exe", arg);
            }
        }
    }
}
