using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class MogaProLinuxProfile : UnityInputDeviceProfile
	{
		public MogaProLinuxProfile()
		{
			Name = "MOGA Pro";
			Meta = "MOGA Pro on Linux";

			SupportedPlatforms = new[] {
				"Linux",
			};

			JoystickNames = new[] {
				"Moga Pro HID",
			};

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
					Source = Button7
				},
				new InputControlMapping {
					Handle = "Right Stick Button",
					Target = InputControlType.RightStickButton,
					Source = Button8
				},
				new InputControlMapping {
					Handle = "Start",
					Target = InputControlType.Start,
					Source = Button6
				}
			};

			AnalogMappings = new[] {
				LeftStickLeftMapping( Analog0 ),
				LeftStickRightMapping( Analog0 ),
				LeftStickUpMapping( Analog1 ),
				LeftStickDownMapping( Analog1 ),

				RightStickLeftMapping( Analog2 ),
				RightStickRightMapping( Analog2 ),
				RightStickUpMapping( Analog3 ),
				RightStickDownMapping( Analog3 ),

				DPadLeftMapping( Analog6 ),
				DPadRightMapping( Analog6 ),
				DPadUpMapping( Analog7 ),
				DPadDownMapping( Analog7 ),

				new InputControlMapping {
					Handle = "Left Trigger",
					Target = InputControlType.LeftTrigger,
					Source = Analog5,
				},
				new InputControlMapping {
					Handle = "Right Trigger",
					Target = InputControlType.RightTrigger,
					Source = Analog4
				}
			};
		}
	}
	// @endcond
}

