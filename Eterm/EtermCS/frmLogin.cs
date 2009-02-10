using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;


namespace Eterm
{
    public partial class frmLogin : Form
    {

        private EtermCS.Properties.Settings MySettings;


        public frmLogin()
        {
            MySettings = new EtermCS.Properties.Settings();

            InitializeComponent();

            checkBox1.Checked = (bool)MySettings["remember"];

            if (checkBox1.Checked)
            {
                username.Text = MySettings["username"].ToString();
                password.Text = MySettings["password"].ToString();
                server.Text = MySettings["server"].ToString();
                port.Text =MySettings["port"].ToString();
                ssl.Checked = (bool)MySettings["ssl"];
                type.Text=MySettings["type"].ToString();
            }

        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {

            if (username.Text.Trim() == "" && password.Text.Trim() == "" && server.Text.Trim() == "" && port.Text.Trim() == "" && type.Text.Trim()=="")
            {
                MessageBox.Show("登录信息不完整！");
                return;
            }

            MySettings["username"] = username.Text.Trim();
            MySettings["password"] = password.Text.Trim();
            MySettings["remember"] = checkBox1.Checked;
            MySettings["server"] = server.Text.Trim();
            MySettings["port"] = Convert.ToInt32(port.Text.Trim());
            MySettings["ssl"] = ssl.Checked;
            MySettings["type"] = type.Text;
            MySettings.Save();
            CurrentUser.user = username.Text.Trim();
            CurrentUser.pass = password.Text.Trim();
            CurrentUser.server = server.Text.Trim();
            CurrentUser.port = Convert.ToInt16( port.Text.Trim());
            CurrentUser.ssl = ssl.Checked;
            CurrentUser.type = type.Text;

            DialogResult = DialogResult.OK;

        }
    }
}