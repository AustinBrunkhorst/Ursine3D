using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class ExecutionerXProfile : UnityInputDeviceProfile
	{
		public ExecutionerXProfile()
		{
			Name = "Executioner X Controller";
			Meta = "Executioner X Controller";

			SupportedPlatforms = new[] {
				"Windows",
				"OS X"
			};

			JoystickNames = new[] {
				"Zeroplus PS Vibration Feedback Converter",
				"Zeroplus PS Vibration Feedback Converter "
			};

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "3",
					Target = InputControlType.Action1,
					Source = Button2
				},
				new InputControlMapping {
					Handle = "2",
					Target = InputControlType.Action2,
					Source = Button1
				},
				new InputControlMapping {
					Handle = "4",
					Target = InputControlType.Action3,
					Source = Button3
				},
				new InputControlMapping {
					Handle = "1",
					Target = InputControlType.Action4,
					Source = Button0
				},
				new InputControlMapping {
					Handle = "Left Bumper",
					Target = InputControlType.LeftBumper,
					Source = Button6
				},
				new InputControlMapping {
					Handle = "Right Bumper",
					Target = InputControlType.RightBumper,
					Source = Button7
				},
				new InputControlMapping {
					Handle = "Start",
					Target = InputControlType.Start,
					Source = Button11
				},
				new InputControlMapping {
					Handle = "Options",
					Target = InputControlType.Options,
					Source = Button8
				},
				new InputControlMapping {
					Handle = "Left Trigger",
					Target = InputControlType.LeftTrigger,
					Source = Button4
				},
				new InputControlMapping {
					Handle = "Right Trigger",
					Target = InputControlType.RightTrigger,
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
			};
		}
	}
	// @endcond
}

