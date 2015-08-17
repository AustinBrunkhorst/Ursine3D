using System;
using UnityEngine;


namespace InControl
{
	public class UnityButtonSource : InputControlSource
	{
		static string[,] buttonQueries;
		public int ButtonId;


		public UnityButtonSource()
		{
			SetupButtonQueries();
		}


		public UnityButtonSource( int buttonId )
		{
			ButtonId = buttonId;
			SetupButtonQueries();
		}


		public float GetValue( InputDevice inputDevice )
		{
			return GetState( inputDevice ) ? 1.0f : 0.0f;
		}


		public bool GetState( InputDevice inputDevice )
		{
			var joystickId = (inputDevice as UnityInputDevice).JoystickId;
			var buttonKey = GetButtonKey( joystickId, ButtonId );
			return Input.GetKey( buttonKey );
		}


		static void SetupButtonQueries()
		{
			if (buttonQueries == null)
			{			
				buttonQueries = new string[ UnityInputDevice.MaxDevices, UnityInputDevice.MaxButtons ];
				
				for (int joystickId = 1; joystickId <= UnityInputDevice.MaxDevices; joystickId++)
				{
					for (int buttonId = 0; buttonId < UnityInputDevice.MaxButtons; buttonId++)
					{
						buttonQueries[joystickId - 1, buttonId] = "joystick " + joystickId + " button " + buttonId;
					}
				}
			}
		}
		
		
		static string GetButtonKey( int joystickId, int buttonId )
		{
			return buttonQueries[joystickId - 1, buttonId];
		}
	}
}

