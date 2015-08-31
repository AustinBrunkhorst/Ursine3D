using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class Xbox360WinProfile : UnityInputDeviceProfile
	{
		public Xbox360WinProfile()
		{
			Name = "XBox 360 Controller";
			Meta = "XBox 360 Controller on Windows";

			SupportedPlatforms = new[] {
				"Windows"
			};

			JoystickNames = new[] {
				"AIRFLO             ",
				"AxisPad",
				"Controller (Afterglow Gamepad for Xbox 360)",
				"Controller (Batarang wired controller (XBOX))",
				"Controller (Gamepad for Xbox 360)",
				"Controller (GPX Gamepad)",
				"Controller (Infinity Controller 360)",
				"Controller (Mad Catz FPS Pro GamePad)",
				"Controller (MadCatz Call of Duty GamePad)",
				"Controller (MadCatz GamePad)",
				"Controller (MLG GamePad for Xbox 360)",
				"Controller (Razer Sabertooth Elite)",
				"Controller (Rock Candy Gamepad for Xbox 360)",
				"Controller (SL-6566)",
				"Controller (Xbox 360 For Windows)",
				"Controller (Xbox 360 Wireless Receiver for Windows)",
				"Controller (Xbox Airflo wired controller)",
				"Controller (XEOX Gamepad)",
				"Cyborg V.3 Rumble Pad",
				"Generic USB Joystick ",
				"MadCatz GamePad (Controller)",
				"Saitek P990 Dual Analog Pad",
				"SL-6566 (Controller)",
				"USB Gamepad ",
				"WingMan RumblePad",
				"XBOX 360 For Windows (Controller)",
				"XEOX Gamepad (Controller)",
				"XEQX Gamepad SL-6556-BK",
			};

			LastResortRegex = "360|xbox|catz";

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "A",
					Target = InputControlType.Action1,
					Source = Button0
				},
				new InputControlMapping {
					Handle = "B",
					Target = InputControlType.Action2,
					Source = Button1
				},
				new InputControlMapping {
					Handle = "X",
					Target = InputControlType.Action3,
					Source = Button2
				},
				new InputControlMapping {
					Handle = "Y",
					Target = InputControlType.Action4,
					Source = Button3
				},
				new InputControlMapping {
					Handle = "Left Bumper",
					Target = InputControlType.LeftBumper,
					Source = Button4
				},
				new InputControlMapping {
					Handle = "Right Bumper",
					Target = InputControlType.RightBumper,
					Source = Button5
				},
				new InputControlMapping {
					Handle = "Left Stick Button",
					Target = InputControlType.LeftStickButton,
					Source = Button8
				},
				new InputControlMapping {
					Handle = "Right Stick Button",
					Target = InputControlType.RightStickButton,
					Source = Button9
				},
				new InputControlMapping {
					Handle = "Back",
					Target = InputControlType.Back,
					Source = Button6
				},
				new InputControlMapping {
					Handle = "Start",
					Target = InputControlType.Start,
					Source = Button7
				}
			};

			AnalogMappings = new[] {
				LeftStickLeftMapping( Analog0 ),
				LeftStickRightMapping( Analog0 ),
				LeftStickUpMapping( Analog1 ),
				LeftStickDownMapping( Analog1 ),

				RightStickLeftMapping( Analog3 ),
				RightStickRightMapping( Analog3 ),
				RightStickUpMapping( Analog4 ),
				RightStickDownMapping( Analog4 ),

				DPadLeftMapping( Analog5 ),
				DPadRightMapping( Analog5 ),
				DPadUpMapping2( Analog6 ),
				DPadDownMapping2( Analog6 ),

				new InputControlMapping {
					Handle = "Left Trigger",
					Target = InputControlType.LeftTrigger,
					Source = Analog2,
					SourceRange = InputRange.ZeroToOne,
					TargetRange = InputRange.ZeroToOne,
				},
				new InputControlMapping {
					Handle = "Right Trigger",
					Target = InputControlType.RightTrigger,
					Source = Analog2,
					SourceRange = InputRange.ZeroToMinusOne,
					TargetRange = InputRange.ZeroToOne,
				},

				new InputControlMapping {
					Handle = "Left Trigger",
					Target = InputControlType.LeftTrigger,
					Source = Analog8
				},
				new InputControlMapping {
					Handle = "Right Trigger",
					Target = InputControlType.RightTrigger,
					Source = Analog9
				}
			};
		}
	}
	// @endcond
}

