using System;
using System.Windows.Forms;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;

using System.Threading;



namespace RemotableObjects
{

	public class MyRemotableObject : MarshalByRefObject
	{
        ManualResetEvent alldone = new ManualResetEvent(false);
        private string resultstring = "";

		public MyRemotableObject()
		{
		
		}


        public string ResultString
        {
            set
            {
                resultstring = value;
            }
        }

        public void Set()
        {
            alldone.Set();
        }


		public string SetMessage(string message)
		{
            lock (this)
            {

                alldone.Reset();
                Cache.GetInstance().MessageString(this,message);
                alldone.WaitOne();

            }

            System.Threading.Thread.Sleep(200);
            return resultstring;
		}





	}
}
