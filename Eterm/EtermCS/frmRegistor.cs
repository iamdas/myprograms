using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using Leey.Net.eTerm;


namespace Eterm
{
    public partial class frmRegistor : Form
    {
        EtermConnection  ec;
        Image i;

        public frmRegistor(EtermConnection ec, Image i)
        {
            InitializeComponent();

            this.ec = ec;
            this.i = i;
            
            this.pictureBox1.Image = i;

        }

        private void frmRegistor_Load(object sender, EventArgs e)
        {

        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            ec.ChkCode = textBox1.Text.Trim();
            this.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ec.RefreshChkCode = true;
            this.Close();
        }
    }
}