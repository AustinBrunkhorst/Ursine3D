using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using UnityEngine;


namespace InControl
{
	public sealed class AutoDiscover : Attribute
	{
	}


	public class UnityInputDeviceProfile : InputDeviceProfile
	{
		[SerializeField]
		protected string[] JoystickNames;

		[SerializeField]
		protected string[] JoystickRegex;

		[SerializeField]
		protected string LastResortRegex;


		public UnityInputDeviceProfile()
		{
			Sensitivity = 1.0f;
			LowerDeadZone = 0.2f;
			UpperDeadZone = 0.9f;
		}


		public override bool IsKnown
		{ 
			get
			{
				return true;
			}
		}


		public override bool IsJoystick
		{ 
			get
			{ 
				return (LastResortRegex != null) ||
				(JoystickNames != null && JoystickNames.Length > 0) ||
				(JoystickRegex != null && JoystickRegex.Length > 0);
			} 
		}


		public override bool HasJoystickName( string joystickName )
		{
			if (IsNotJoystick)
			{
				return false;
			}

			if (JoystickNames != null)
			{
				if (JoystickNames.Contains( joystickName, StringComparer.OrdinalIgnoreCase ))
				{
					return true;
				}
			}

			if (JoystickRegex != null)
			{
				for (int i = 0; i < JoystickRegex.Length; i++)
				{
					if (Regex.IsMatch( joystickName, JoystickRegex[i], RegexOptions.IgnoreCase ))
					{
						return true;
					}
				}
			}

			return false;
		}


		public override bool HasLastResortRegex( string joystickName )
		{
			if (IsNotJoystick)
			{
				return false;
			}

			if (LastResortRegex == null)
			{
				return false;
			}

			return Regex.IsMatch( joystickName, LastResortRegex, RegexOptions.IgnoreCase );
		}


		public override bool HasJoystickOrRegexName( string joystickName )
		{
			return HasJoystickName( joystickName ) || HasLastResortRegex( joystickName );
		}

		/*
		#region Serialization

		public string Save()
		{
			var data = JSON.Dump( this, EncodeOptions.PrettyPrint | EncodeOptions.NoTypeHints );

			// Somewhat silly, but removes all default values for brevity.
			data = Regex.Replace( data, @"\t""JoystickRegex"": null,?\n", "" );
			data = Regex.Replace( data, @"\t""LastResortRegex"": null,?\n", "" );
			data = Regex.Replace( data, @"\t\t\t""Invert"": false,?\n", "" );
			data = Regex.Replace( data, @"\t\t\t""Scale"": 1,?\n", "" );
			data = Regex.Replace( data, @"\t\t\t""Raw"": false,?\n", "" );
			data = Regex.Replace( data, @"\t\t\t""IgnoreInitialZeroValue"": false,?\n", "" );
			data = Regex.Replace( data, @"\t\t\t""Sensitivity"": 1,?\n", "" );
			data = Regex.Replace( data, @"\t\t\t""LowerDeadZone"": 0,?\n", "" );
			data = Regex.Replace( data, @"\t\t\t""UpperDeadZone"": 1,?\n", "" );
			data = Regex.Replace( data, @"\t""Sensitivity"": 1,?\n", "" );
			data = Regex.Replace( data, @"\t""LowerDeadZone"": 0.2,?\n", "" );
			data = Regex.Replace( data, @"\t""UpperDeadZone"": 0.9,?\n", "" );
			data = Regex.Replace( data, @"\t\t\t""(Source|Target)Range"": {\n\t\t\t\t""Value0"": -1,\n\t\t\t\t""Value1"": 1\n\t\t\t},?\n", "" );
			data = Regex.Replace( data, @"\t""MinUnityVersion"": {\n\t\t""Major"": 3,\n\t\t""Minor"": 0,\n\t\t""Patch"": 0,\n\t\t""Build"": 0\n\t},?\n", "" );
			data = Regex.Replace( data, @"\t""MaxUnityVersion"": {\n\t\t""Major"": 9,\n\t\t""Minor"": 0,\n\t\t""Patch"": 0,\n\t\t""Build"": 0\n\t},?\n", "" );

			return data;
		}


		public static UnityInputDeviceProfile Load( string data )
		{
			return JSON.Load( data ).Make<UnityInputDeviceProfile>();
		}


		public void SaveToFile( string filePath )
		{
			var data = Save();
			Utility.WriteToFile( filePath, data );
		}


		public static UnityInputDeviceProfile LoadFromFile( string filePath )
		{
			var data = Utility.ReadFromFile( filePath );
			return Load( data );
		}

		#endregion
		*/


		#region InputControlSource helpers

		protected static InputControlSource Button( int index )
		{
			return new UnityButtonSource( index );
		}

		protected static InputControlSource Analog( int index )
		{
			return new UnityAnalogSource( index );
		}

		protected static InputControlSource Button0 = Button( 0 );
		protected static InputControlSource Button1 = Button( 1 );
		protected static InputControlSource Button2 = Button( 2 );
		protected static InputControlSource Button3 = Button( 3 );
		protected static InputControlSource Button4 = Button( 4 );
		protected static InputControlSource Button5 = Button( 5 );
		protected static InputControlSource Button6 = Button( 6 );
		protected static InputControlSource Button7 = Button( 7 );
		protected static InputControlSource Button8 = Button( 8 );
		protected static InputControlSource Button9 = Button( 9 );
		protected static InputControlSource Button10 = Button( 10 );
		protected static InputControlSource Button11 = Button( 11 );
		protected static InputControlSource Button12 = Button( 12 );
		protected static InputControlSource Button13 = Button( 13 );
		protected static InputControlSource Button14 = Button( 14 );
		protected static InputControlSource Button15 = Button( 15 );
		protected static InputControlSource Button16 = Button( 16 );
		protected static InputControlSource Button17 = Button( 17 );
		protected static InputControlSource Button18 = Button( 18 );
		protected static InputControlSource Button19 = Button( 19 );

		protected static InputControlSource Analog0 = Analog( 0 );
		protected static InputControlSource Analog1 = Analog( 1 );
		protected static InputControlSource Analog2 = Analog( 2 );
		protected static InputControlSource Analog3 = Analog( 3 );
		protected static InputControlSource Analog4 = Analog( 4 );
		protected static InputControlSource Analog5 = Analog( 5 );
		protected static InputControlSource Analog6 = Analog( 6 );
		protected static InputControlSource Analog7 = Analog( 7 );
		protected static InputControlSource Analog8 = Analog( 8 );
		protected static InputControlSource Analog9 = Analog( 9 );
		protected static InputControlSource Analog10 = Analog( 10 );
		protected static InputControlSource Analog11 = Analog( 11 );
		protected static InputControlSource Analog12 = Analog( 12 );
		protected static InputControlSource Analog13 = Analog( 13 );
		protected static InputControlSource Analog14 = Analog( 14 );
		protected static InputControlSource Analog15 = Analog( 15 );
		protected static InputControlSource Analog16 = Analog( 16 );
		protected static InputControlSource Analog17 = Analog( 17 );
		protected static InputControlSource Analog18 = Analog( 18 );
		protected static InputControlSource Analog19 = Analog( 19 );

		protected static InputControlSource MenuKey = new UnityKeyCodeSource( KeyCode.Menu );
		protected static InputControlSource EscapeKey = new UnityKeyCodeSource( KeyCode.Escape );

		#endregion

		#region InputDeviceMapping helpers

		protected static InputControlMapping LeftStickLeftMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Left Stick Left",
				Target = InputControlType.LeftStickLeft,
				Source = analog,
				SourceRange = InputRange.ZeroToMinusOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping LeftStickRightMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Left Stick Right",
				Target = InputControlType.LeftStickRight,
				Source = analog,
				SourceRange = InputRange.ZeroToOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping LeftStickUpMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Left Stick Up",
				Target = InputControlType.LeftStickUp,
				Source = analog,
				SourceRange = InputRange.ZeroToMinusOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping LeftStickDownMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Left Stick Down",
				Target = InputControlType.LeftStickDown,
				Source = analog,
				SourceRange = InputRange.ZeroToOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping RightStickLeftMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Right Stick Left",
				Target = InputControlType.RightStickLeft,
				Source = analog,
				SourceRange = InputRange.ZeroToMinusOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping RightStickRightMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Right Stick Right",
				Target = InputControlType.RightStickRight,
				Source = analog,
				SourceRange = InputRange.ZeroToOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping RightStickUpMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Right Stick Up",
				Target = InputControlType.RightStickUp,
				Source = analog,
				SourceRange = InputRange.ZeroToMinusOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping RightStickDownMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Right Stick Down",
				Target = InputControlType.RightStickDown,
				Source = analog,
				SourceRange = InputRange.ZeroToOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping LeftTriggerMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Left Trigger",
				Target = InputControlType.LeftTrigger,
				Source = analog,
				SourceRange = InputRange.MinusOneToOne,
				TargetRange = InputRange.ZeroToOne,
				IgnoreInitialZeroValue = true
			};
		}

		protected static InputControlMapping RightTriggerMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "Right Trigger",
				Target = InputControlType.RightTrigger,
				Source = analog,
				SourceRange = InputRange.MinusOneToOne,
				TargetRange = InputRange.ZeroToOne,
				IgnoreInitialZeroValue = true
			};
		}

		protected static InputControlMapping DPadLeftMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "DPad Left",
				Target = InputControlType.DPadLeft,
				Source = analog,
				SourceRange = InputRange.ZeroToMinusOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping DPadRightMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "DPad Right",
				Target = InputControlType.DPadRight,
				Source = analog,
				SourceRange = InputRange.ZeroToOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping DPadUpMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "DPad Up",
				Target = InputControlType.DPadUp,
				Source = analog,
				SourceRange = InputRange.ZeroToMinusOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping DPadDownMapping( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "DPad Down",
				Target = InputControlType.DPadDown,
				Source = analog,
				SourceRange = InputRange.ZeroToOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		protected static InputControlMapping DPadUpMapping2( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "DPad Up",
				Target = InputControlType.DPadUp,
				Source = analog,
				SourceRange = InputRange.ZeroToOne,
				TargetRange = InputRange.ZeroToOne
			};
		}
		
		protected static InputControlMapping DPadDownMapping2( InputControlSource analog )
		{
			return new InputControlMapping {
				Handle = "DPad Down",
				Target = InputControlType.DPadDown,
				Source = analog,
				SourceRange = InputRange.ZeroToMinusOne,
				TargetRange = InputRange.ZeroToOne
			};
		}

		#endregion
	}
}

