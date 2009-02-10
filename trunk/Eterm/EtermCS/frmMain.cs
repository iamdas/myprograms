// =====================================================================
//
// EtermDEMO - A DEMO for Building and Testing eTerm SDK and Web eTerm
//
// by Xsharp
// Copyright Leey.Net, Oct 2007
// http://www.leey.net
//
// =====================================================================

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.IO;
using System.Text.RegularExpressions;


using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;
using System.Runtime.Remoting.Channels.Http;

using Leey.Net.eTerm;

using System.Xml;

using RemotableObjects;


namespace Eterm
{
    public partial class frmMain : Form, IMessageFilter, IObserver
    {

        delegate void NotifyDelegate(object sender, string text);
        private MyRemotableObject remotableObject = null;

        //private RichTextBox temprb = new RichTextBox();
        //private StringBuilder tempsb = null;

        private EtermConnection ec;

        public frmMain()
        {

            InitializeComponent();
            this.StartPosition = FormStartPosition.CenterScreen;

            //此处请设置为从代理商处取得的激活码
            Leey.Net.eTerm.LicenseManager.Instance.Key = "ceshiceshiceshi";


            ec = new EtermConnection();
            ec.OnReadPacket += new EtermConnection.CmdResultHandler(ec_OnReadPacket);
            ec.OnSendPacket += new EtermConnection.PacketHandler(ec_OnSendPacket);
            ec.OnLogin +=new Leey.Net.eTerm.Net.ObjectHandler(ec_OnLogin);
            ec.OnUnLogin+=new Leey.Net.eTerm.Net.ObjectHandler(ec_OnUnLogin);
            ec.OnEtermConnectionStateChanged+=new EtermConnection.EtermConnectionStateHandler(ec_OnEtermConnectionStateChanged);
            ec.OnGetChkCode += new EtermConnection.ChkCodeHandler(ec_OnGetChkCode);
            ec.OnError += new Leey.Net.eTerm.Net.ErrorHandler(ec_OnError);

            //ec.LocalIP = "10.108.16.20";

            //连续执行指令之间的间隔。 对于新申请的航信配置可适当设置此参数，避免指令执行过快出现 Transation in process 错误。
            ec.ExeInterval = 1000;

            //实现REMOTING
            HttpServerChannel channel = new HttpServerChannel(8081);
            ChannelServices.RegisterChannel(channel,false);
            RemotingConfiguration.RegisterWellKnownServiceType(typeof(MyRemotableObject), "HelloWorld.rem", WellKnownObjectMode.Singleton);
            RemotableObjects.Cache.Attach(this);
            
            //启动日志记录
            LogManager.Instance.LoadConfiguration();
            LogManager.Instance.Log.Info("应用程序启动");
        }


        //网页上发送来的指令，都会被传到Notify函数中， 如需对网页上执行的命令进行限制， 可在Notify函数里手工加入判断限制代码。
        #region IObserver Members

        public void Notify(object sender, string text)
        {
            if (InvokeRequired)
            {
                Invoke(new NotifyDelegate(Notify), sender, text);
                return;
            }

            richTextBox1.SelectionColor = Color.LimeGreen;
            richTextBox1.SelectionFont = new Font("宋体", 9);
            richTextBox1.AppendText("\r\n" + text);
            richTextBox1.AppendText("\r\n");


            if (ec.m_ConnectionState == StateClass.ConnectionState.Logined)
            {
                remotableObject = (MyRemotableObject)sender;

                if (text.ToUpper().Trim().StartsWith("RT"))
                {
                    //此构造函数可以提取团队中的姓名等特殊数据
                    RTWithNMAndPrice(text);
                }
                else if (text.ToUpper().Trim().StartsWith("AV"))
                {
                    GetFlights(text);
                }
            }
            else
            {
                remotableObject = (MyRemotableObject)sender;
                remotableObject.ResultString = "服务器ETERM未登陆 或 执行了RT以外的指令\r\rCommand:" + text;
                remotableObject.Set();
            }
        }

        #endregion


        #region 更加灵活的指令执行方式。GetFlights实现了自动翻页取得所有航班信息， RTWithNMAndPrice自己提取编码取得姓名信息并同时取得价格信息

        private void EvaluatorTeamNM(CmdResult result)
        {
            if (Regex.IsMatch(ec.CurrentCmdPacket.CmdString.ToUpper().Trim(), "RT[0-9A-Z]{5}"))
            {
                if (Regex.IsMatch(result.Data, @"\sNM([0-9]{1,3})\s"))
                {
                    result.DiscardResult = true;
                    CmdPacket c= new CmdPacket("RTN", CmdCB, false, new CmdResultEvaluator(EvaluatorTeamNM), null);
                    c.EcChannel = ec;
                    ec.CurrentCmdPacket.SubCmdPacket = c;
                }
                else
                {
                    CmdPacket c= new CmdPacket("PAT:A", CmdCB, false, new CmdResultEvaluator(EvaluatorTeamNM),
                                                        null);
                    c.EcChannel = ec;
                    ec.CurrentCmdPacket.SubCmdPacket = c;
                }
            }
            else if (ec.CurrentCmdPacket.CmdString.ToUpper().Trim() == "RTN")
            {

                if (Regex.IsMatch(ec.TempResultBuffer + result.Data, @"NM([0-9]{1,3}).*?\1\."))
                {
                    CmdPacket c = new CmdPacket("PAT:A", CmdCB, false, new CmdResultEvaluator(EvaluatorTeamNM), null);
                    c.EcChannel = ec;
                    ec.CurrentCmdPacket.SubCmdPacket = c;                
                }
                else
                {
                    CmdPacket c = new CmdPacket("PN", CmdCB, true, new CmdResultEvaluator(EvaluatorTeamNM), null);
                    c.EcChannel = ec;
                    ec.CurrentCmdPacket.SubCmdPacket = c;                 
                }
            
            }
            else if (ec.CurrentCmdPacket.CmdString.ToUpper().Trim() == "PN")
            {
                if (Regex.IsMatch(ec.TempResultBuffer + result.Data, @"NM([0-9]{1,3}).*?\1\."))
                {
                    CmdPacket c = new CmdPacket("PAT:A", CmdCB, false, new CmdResultEvaluator(EvaluatorTeamNM), null);
                    c.EcChannel = ec;
                    ec.CurrentCmdPacket.SubCmdPacket = c;                  
                }
                else
                {
                    ec.CurrentCmdPacket.Repeat = true;
                }
            }
            else if (ec.CurrentCmdPacket.CmdString.ToUpper().Trim() == "PAT:A")
            {
                if (!result.Data.ToUpper().Contains("TOTAL:"))
                {
                    CmdPacket c = new CmdPacket("PAT:", CmdCB, false, new CmdResultEvaluator(EvaluatorTeamNM), null);
                    c.EcChannel = ec;
                    ec.CurrentCmdPacket.SubCmdPacket = c;                
                }
            }
        }

        private void EvaluatorAVH(CmdResult result)
        {

            AirFlights afs = new AirFlights();
            afs.Load(result.Data);

            result.Airflights = afs;

            string resultdate = result.GetAvhdate();
            string cmdate = ec.ParentCmdPacket.GetAvhdate();

            if (resultdate == cmdate)
            {
                if (afs.Count > 5)
                {
                    CmdPacket c = new CmdPacket("PN", CmdCB, false, new CmdResultEvaluator(EvaluatorAVH), null);
                    c.EcChannel = ec;
                    ec.CurrentCmdPacket.SubCmdPacket = c;                
                }
            }
            else
            {
                result.DiscardResult = true;
            }

        }

        public void RTWithNMAndPrice(string s)
        {

            CmdPacket c = new CmdPacket(s, CmdCB, new CmdResultEvaluator(EvaluatorTeamNM), null);
            c.EcChannel = ec;
            c.Send();

            //ec.SendCmd(c);

        }

        public void GetFlights(string s)
        {

            CmdPacket c = new CmdPacket(s, CmdCB, new CmdResultEvaluator(EvaluatorAVH), null);
            c.EcChannel = ec;
            c.Send();
            //ec.SendCmd(c);

        }

        #endregion


        #region 键盘消息过滤函数，主要处理小回车,F12的输入问题

        public bool PreFilterMessage(ref System.Windows.Forms.Message m )
        {


            if ((Keys)m.WParam.ToInt32() == Keys.Escape)
            {
                if (m.LParam.ToInt32() == 65537 )
                {
                    richTextBox1.SelectedText = ""+(char)9632;
                    return true;
                }
            }

            if (Control.ModifierKeys == Keys.Control && (Keys)m.WParam.ToInt32() == Keys.V && richTextBox1.Focused)
            {
                if (m.LParam.ToInt32() == 3080193)
                {
                    richTextBox1.AppendText(Clipboard.GetText());
                    return true;
                }
            }


            if ((Keys)m.WParam.ToInt32() == Keys.Enter)
            {
                if (m.LParam.ToInt32() == 18612225 && m.Msg == 256 && richTextBox1.Focused)
                {
                    DealwithCmd();
                }
                else
                {
                    return false;
                }
            }

            if ((Keys)m.WParam.ToInt32() == Keys.F12 && richTextBox1.Focused)
            {
                if (m.LParam.ToInt32() == 5767169)
                {
                    DealwithCmd();
                }
            }

            return false;

        }

        #endregion


        private void DealwithCmd()
        {
            string s = GetCommandLine().Trim();
            if (s != "")
            {
                if (s.ToUpper().StartsWith("PRINT"))
                {
                    string[] splits=Regex.Split(s,@"\s{1,}");
                    if (splits.Length != 3) { throw new Exception("PRINT格式不对"); }
                    Packet c = new PrintPacket(splits[1], splits[2], CmdCB);
                    c.EcChannel = ec;
                    c.Send();
                }
                else
                {
                    CmdPacket c = new CmdPacket(s, CmdCB);
                    c.EcChannel = ec;
                    c.Send();
                }
            }
            else
            {
                richTextBox1.Text += "\r\n" + "请检查命令格式！";
                richTextBox1.Text += "\r\n" + (char)9632;
            }
        }


        private void Open(object sender)
        {

            if (CurrentUser.type.Contains("eTerm"))
            {
                ec.eTerminalType = EtermConnection.TerminalType.eTerm;
            }
            else if (CurrentUser.type.Contains("iTerm"))
            {
                ec.eTerminalType = EtermConnection.TerminalType.iTerm;
            }

            //ec.SIText = "SI 8888 8888f 9";
            ec.Open(CurrentUser.server, CurrentUser.port, CurrentUser.ssl, CurrentUser.user, CurrentUser.pass);
        }

        private void CmdCB(object sender, CmdResult result)
        {
            if (InvokeRequired)
            {
                Invoke(new EtermConnection.CmdResultCB(CmdCB), sender, result);
                return;
            }


            richTextBox1.SelectionColor = Color.LimeGreen;
            richTextBox1.SelectionFont = new Font("宋体", 9);
            richTextBox1.AppendText("\r\n" + result.Data);
            this.richTextBox1.Select(this.richTextBox1.TextLength, 0);
            this.richTextBox1.ScrollToCaret();
            richTextBox1.AppendText("\r\n" + (char)9632);


            if (remotableObject != null)
            {
                remotableObject.ResultString = result.Data;
                remotableObject.Set();
            }

        }


        #region EtermConnection事件处理函数



        private void ec_OnLogin(object sender)
        {
            LogManager.Instance.Log.Info("登录航信服务器成功!");
        }

        private void ec_OnUnLogin(object sender)
        {
            LogManager.Instance.Log.Info("航信服务器连接断开!");
        }

        private void ec_OnReadPacket(object sender, CmdResult result)
        {
            //LogManager.Instance.Log.Info("接收数据: "+result.Data);
            //MessageBox.Show(result.Data);
        }

        private void ec_OnSendPacket(object sender, Packet cmdpacket)
        {
            //LogManager.Instance.Log.Info("发送数据: " + cmdpacket.CmdString);
        }


        private void ec_OnGetChkCode(object sender, Image i)
        {
            if (InvokeRequired)
            {
                Invoke(new EtermConnection.ChkCodeHandler(ec_OnGetChkCode), sender, i);
                return;
            }
            frmRegistor frmregisto = new frmRegistor(ec, i);
            frmregisto.StartPosition = FormStartPosition.CenterScreen;
            frmregisto.ShowDialog();
            frmregisto.Dispose();

        }



        private void ec_OnError(object sender, Exception ex)
        {
            if (InvokeRequired)
            {
                Invoke(new Leey.Net.eTerm.Net.ErrorHandler(ec_OnError), sender, ex);
                return;
            }
            EndProgressBar();
            //MessageBox.Show(ex.Message, "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
            toolStripLogin.Enabled = true;
            toolStripLogout.Enabled = false;
            toolStripStatusLabel1.Text = "已断开:" + ex.Message;
            ec.Close();

            LogManager.Instance.Log.Error("错误: "+ ex.Message);

        }




        private void ec_OnEtermConnectionStateChanged(object sender, StateClass.ConnectionState state)
        {

            if (InvokeRequired)
            {
                Invoke(new EtermConnection.EtermConnectionStateHandler(ec_OnEtermConnectionStateChanged), sender, state);
                return;
            }

            switch (state)
            {
                case StateClass.ConnectionState.Connecting:
                    toolStripLogin.Enabled = false;
                    toolStripLogout.Enabled = true;
                    toolStripStatusLabel1.Text = "正在连接";
                    StartProgressBar();
                    break;
                case StateClass.ConnectionState.Connected:
                    break;
                case StateClass.ConnectionState.Logining:
                    break;
                case StateClass.ConnectionState.Logined:
                    toolStripStatusLabel1.Text = "已连接";
                    EndProgressBar();
                    break;
                case StateClass.ConnectionState.DisConnected:
                    toolStripLogin.Enabled =true;
                    toolStripLogout.Enabled = false;
                    toolStripStatusLabel1.Text = "已断开";
                    EndProgressBar();
                    break;

            }

        }

        #endregion

        #region Form中事件处理函数

        private void Form1_Load(object sender, EventArgs e)
        {

            Application.AddMessageFilter(this);
           
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.WindowsShutDown)
            {
                LogManager.Instance.Log.Info("操作系统正在关闭应用程序以准备关机");
            }
            else if (e.CloseReason == CloseReason.UserClosing)
            { 
                LogManager.Instance.Log.Info("用户关闭应用程序");            
            }
            else if (e.CloseReason == CloseReason.TaskManagerClosing)
            {
                LogManager.Instance.Log.Info("任务管理器关闭应用程序");
            }
            else
            {
                LogManager.Instance.Log.Info("其他未知原因关闭应用程序");
            }
           
        }
       

        private void toolStripLogin_Click(object sender, EventArgs e)
        {
            frmLogin frmlogin = new frmLogin();
            frmlogin.StartPosition = FormStartPosition.CenterParent;

            if (frmlogin.ShowDialog() == DialogResult.OK)
            {
                ThreadPool.QueueUserWorkItem(new WaitCallback(Open));
            }
        }


        private void toolStripLogout_Click(object sender, EventArgs e)
        {
            ec.CloseAll();
        }


        #endregion

        #region LoginProgressBar

        private void StartProgressBar()
        {
            toolStripProgressBar1.Visible = true;
            timer1.Start();
        }

        private void EndProgressBar()
        {
            timer1.Stop();
            toolStripProgressBar1.Visible = false;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (toolStripProgressBar1.Value >= 100)
            {
                toolStripProgressBar1.Value = 0;
            }
            toolStripProgressBar1.Value += 10;
        }

        #endregion

        #region EtermCommandLine
        private string GetCommandLine()
        { 
        
            String  st= richTextBox1.Text.Substring(0, richTextBox1.SelectionStart).ToUpper();
            if (st.LastIndexOf("■") >= 0 )
            {
                st = st.Substring(st.LastIndexOf("■"), st.Length - st.LastIndexOf("■")).Trim();
            }
            else
            {
                return "";
            }


            if (st.Length <= 1 )
            {
                return "";
            }


            String command = st.Remove(0, 1).Replace("\n", "\r").Trim();


            if (command == null)
            {
                return "";
            }
            else
            {
                return command;
            }

        }
        #endregion

        private void toolStripSet_Click(object sender, EventArgs e)
        {

        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            
            RichTextBox rch = new RichTextBox();
            rch.LoadFile("bbb.rtf");

            AirFlights afs = new AirFlights();


            afs.Load(rch.Text);
            

            foreach (AirFlight air in afs)
            {
                richTextBox1.AppendText(air.AirCode + " " + air.AirLine + " " + air.AirType + " " + air.BerthsType + "\r\n");
            }

        }
    }



    public class CurrentUser
    {
        public static string user;
        public static string pass;
        public static string server;
        public static int    port;
        public static bool   ssl;
        public static string type;
    }



}