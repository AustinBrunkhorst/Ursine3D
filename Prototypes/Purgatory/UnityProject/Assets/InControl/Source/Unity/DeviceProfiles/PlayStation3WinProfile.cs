using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class PlayStation3WinProfile : UnityInputDeviceProfile
	{
		public PlayStation3WinProfile()
		{
			Name = "PlayStation 3 Controller";
			Meta = "PlayStation 3 Controller on Windows (via MotioninJoy Gamepad Tool)";

			SupportedPlatforms = new[] {
				"Windows"
			};

			JoystickNames = new[] {
				"MotioninJoy Virtual Game Controller"
			};

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "Cross",
					Target = InputControlType.Action1,
					Source = Button2
				},
				new InputControlMapping {
					Handle = "Circle",
					Target = InputControlType.Action2,
					Source = Button1
				},
				new InputControlMapping {
					Handle = "Square",
					Target = InputControlType.Action3,
					Source = Button3
				},
				new InputControlMapping {
					Handle = "Triangle",
					Target = InputControlType.Action4,
					Source = Button0
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
					Handle = "Left Trigger",
					Target = InputControlType.LeftTrigger,
					Source = Button6
				},
				new InputControlMapping {
					Handle = "Right Trigger",
					Target = InputControlType.RightTrigger,
					Source = Button7
				},
				new InputControlMapping {
					Handle = "Select",
					Target = InputControlType.Select,
					Source = Button8
				},
				new InputControlMapping {
					Handle = "Left Stick Button",
					Target = InputControlType.LeftStickButton,
					Source = Button10
				},
				new InputControlMapping {
					Handle = "Right Stick Button",
					Target = InputControlType.RightStickButton,
					Source = Button11
				},
				new InputControlMapping {
					Handle = "Start",
					Target = InputControlType.Start,
					Source = Button9
				},
				new InputControlMapping {
					Handle = "System",
					Target = InputControlType.System,
					Source = Button12
				}
			};

			AnalogMappings = new[] {
				LeftStickLeftMapping( Analog0 ),
				LeftStickRightMapping( Analog0 ),
				LeftStickUpMapping( Analog1 ),
				LeftStickDownMapping( Analog1 ),

				RightStickLeftMapping( Analog2 ),
				RightStickRightMapping( Analog2 ),
				RightStickUpMapping( Analog5 ),
				RightStickDownMapping( Analog5 ),

				DPadLeftMapping( Analog8 ),
				DPadRightMapping( Analog8 ),
				DPadUpMapping2( Analog9 ),
				DPadDownMapping2( Analog9 ),

//				new InputControlMapping {
//					Handle = "Tilt X",
//					Target = InputControlType.TiltX,
//					Source = Analog3
//				},
//				new InputControlMapping {
//					Handle = "Tilt Y",
//					Target = InputControlType.TiltY,
//					Source = Analog4
//				}
			};
		}
	}
	// @endcond
}

