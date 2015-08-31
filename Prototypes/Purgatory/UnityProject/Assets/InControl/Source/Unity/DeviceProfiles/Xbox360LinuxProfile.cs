using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class Xbox360LinuxProfile : UnityInputDeviceProfile
	{
		public Xbox360LinuxProfile()
		{
			Name = "XBox 360 Controller";
			Meta = "XBox 360 Controller on Linux";

			SupportedPlatforms = new[] {
				"Linux"
			};

			JoystickNames = new[] {
				"Microsoft X-Box 360 pad",
				"Generic X-Box pad",
				"Mega World Thrustmaster dual analog 3.2",
			};

			LastResortRegex = "360";

			MaxUnityVersion = new VersionInfo( 4, 9, 0, 0 );

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
					Source = Button9
				},
				new InputControlMapping {
					Handle = "Right Stick Button",
					Target = InputControlType.RightStickButton,
					Source = Button10
				},
				new InputControlMapping {
					Handle = "DPad Left",
					Target = InputControlType.DPadLeft,
					Source = Button11,
					Invert = true
				},
				new InputControlMapping {
					Handle = "DPad Right",
					Target = InputControlType.DPadRight,
					Source = Button12,
				},
				new InputControlMapping {
					Handle = "DPad Up",
					Target = InputControlType.DPadUp,
					Source = Button13,
					Invert = true
				},
				new InputControlMapping {
					Handle = "DPad Down",
					Target = InputControlType.DPadDown,
					Source = Button14,
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
				},
				new InputControlMapping {
					Handle = "Menu",
					Target = InputControlType.Menu,
					Source = Button8
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

				DPadLeftMapping( Analog6 ),
				DPadRightMapping( Analog6 ),
				DPadUpMapping( Analog7 ),
				DPadDownMapping( Analog7 ),

				new InputControlMapping {
					Handle = "Left Trigger",
					Target = InputControlType.LeftTrigger,
					Source = Analog2
				},
				new InputControlMapping {
					Handle = "Right Trigger",
					Target = InputControlType.RightTrigger,
					Source = Analog5
				}
			};
		}
	}
	// @endcond
}

