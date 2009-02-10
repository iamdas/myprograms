using System;

namespace RemotableObjects
{

	public interface IObserver
	{
		void Notify(object sender,string text);
	}

}
