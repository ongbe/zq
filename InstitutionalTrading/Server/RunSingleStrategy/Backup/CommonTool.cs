using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace RunSingleStrategy
{
    class CommonTool
    {
        public void Log(string fmt, params object[] args)
        {
            Debug.WriteLine("{0}", string.Format(fmt, args).Trim());
        }

        public string GetString(byte[] stringBytes)
        {
            return Encoding.UTF8.GetString(stringBytes);
        }

        public byte[] GetBytes(string stringValue)
        {
            return Encoding.UTF8.GetBytes(stringValue);
        }
    }
}
