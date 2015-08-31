using System;
using UnityEngine;


namespace InControl
{
	public class UnityMouseAxisSource : InputControlSource
	{
		public string MouseAxisQuery;


		public UnityMouseAxisSource()
		{
		}


		public UnityMouseAxisSource( string axis )
		{
			MouseAxisQuery = "mouse " + axis;
		}
		
		
		public float GetValue( InputDevice inputDevice )
		{
			return Input.GetAxisRaw( MouseAxisQuery );
		}
		
		
		public bool GetState( InputDevice inputDevice )
		{
			return Utility.IsNotZero( GetValue( inputDevice ) );
		}
	}
}

