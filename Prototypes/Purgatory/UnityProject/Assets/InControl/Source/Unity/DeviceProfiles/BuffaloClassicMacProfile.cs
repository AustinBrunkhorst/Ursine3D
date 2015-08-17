using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class BuffaloClassicMacProfile : UnityInputDeviceProfile
	{
		public BuffaloClassicMacProfile()
		{
			Name = "Buffalo Class Gamepad";
			Meta = "Buffalo Class Gamepad on Mac";

			SupportedPlatforms = new[] {
				"OS X",
			};

			JoystickNames = new[] {
				" USB,2-axis 8-button gamepad"
			};

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "A",
					Target = InputControlType.Action2,
					Source = Button0
				},
				new InputControlMapping {
					Handle = "B",
					Target = InputControlType.Action1,
					Source = Button1
				},
				new InputControlMapping {
					Handle = "X",
					Target = InputControlType.Action4,
					Source = Button2
				},
				new InputControlMapping {
					Handle = "Y",
					Target = InputControlType.Action3,
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
					Handle = "Select",
					Target = InputControlType.Select,
					Source = Button6
				},
				new InputControlMapping {
					Handle = "Start",
					Target = InputControlType.Start,
					Source = Button7
				},
			};

			AnalogMappings = new[] {
				DPadLeftMapping( Analog0 ),
				DPadRightMapping( Analog0 ),
				DPadUpMapping( Analog1 ),
				DPadDownMapping( Analog1 ),
			};
		}
	}
	// @endcond
}

