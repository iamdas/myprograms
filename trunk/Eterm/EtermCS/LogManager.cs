// =====================================================================
//
// LogManager
//
// by leey
// Copyright Leey.Net, Mar 2008
// http://www.leey.net
//
// =====================================================================

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Leey.Net.eTerm
{
    public class LogManager
    {

        private static LogManager singleobject = null;
        public static LogManager Instance
        {
            get
            {
                if (singleobject == null)
                {
                    singleobject = new LogManager();
                }
                return singleobject;
            }
        }


        log4net.ILog m_Log;
        public log4net.ILog Log
        {
            get { return m_Log; }
        }


        private string logPath;

        public void LoadConfiguration()
        {

            logPath = Path.Combine(Directory.GetCurrentDirectory(), "logs");

            m_Log = log4net.LogManager.GetLogger(this.GetType().FullName);
            string path = Directory.GetCurrentDirectory();
            string filename = Path.Combine(path, "log4net.config");
            FileInfo file = new FileInfo(filename);

            log4net.Config.XmlConfigurator.Configure(file);

            file = null;

        }

    }
}
