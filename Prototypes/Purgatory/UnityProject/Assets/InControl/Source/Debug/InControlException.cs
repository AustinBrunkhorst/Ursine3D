using System;
using System.Runtime.Serialization;


namespace InControl
{
	[Serializable]
	public class InControlException : Exception
	{
		public InControlException()
		{
		}


		public InControlException( string message )
			: base( message )
		{
		}


		public InControlException( string message, Exception inner )
			: base( message, inner )
		{
		}
	}
}
