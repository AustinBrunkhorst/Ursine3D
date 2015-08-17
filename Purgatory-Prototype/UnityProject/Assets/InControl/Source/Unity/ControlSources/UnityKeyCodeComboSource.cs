using System;
using UnityEngine;


namespace InControl
{
	public class UnityKeyCodeComboSource : InputControlSource
	{
		public KeyCode[] KeyCodeList;


		public UnityKeyCodeComboSource()
		{
		}


		public UnityKeyCodeComboSource( params KeyCode[] keyCodeList )
		{
			KeyCodeList = keyCodeList;
		}


		public float GetValue( InputDevice inputDevice )
		{
			return GetState( inputDevice ) ? 1.0f : 0.0f;
		}


		public bool GetState( InputDevice inputDevice )
		{
			for (int i = 0; i < KeyCodeList.Length; i++)
			{
				if (!Input.GetKey( KeyCodeList[i] ))
				{
					return false;
				}
			}
			return true;
		}
	}
}

