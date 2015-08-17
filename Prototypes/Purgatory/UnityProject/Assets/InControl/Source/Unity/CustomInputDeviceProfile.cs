using System;
using UnityEngine;


namespace InControl
{
	public class CustomInputDeviceProfile : InputDeviceProfile
	{
		public CustomInputDeviceProfile()
		{
			Name = "Custom Device Profile";
			Meta = "Custom Device Profile";

			SupportedPlatforms = new[] {
				"Windows",
				"Mac",
				"Linux"
			};

			Sensitivity = 1.0f;
			LowerDeadZone = 0.0f;
			UpperDeadZone = 1.0f;
		}


		public sealed override bool IsKnown
		{ 
			get
			{
				return true;
			}
		}


		public sealed override bool IsJoystick
		{ 
			get
			{ 
				return false; 
			} 
		}


		public sealed override bool HasJoystickName( string joystickName )
		{
			return false;
		}


		public sealed override bool HasLastResortRegex( string joystickName )
		{
			return false;
		}


		public sealed override bool HasJoystickOrRegexName( string joystickName )
		{
			return false;
		}


		#region InputControlSource Helpers

		protected static InputControlSource KeyCodeButton( params KeyCode[] keyCodeList )
		{
			return new UnityKeyCodeSource( keyCodeList );
		}

		protected static InputControlSource KeyCodeComboButton( params KeyCode[] keyCodeList )
		{
			return new UnityKeyCodeComboSource( keyCodeList );
		}

		#endregion
	}
}

