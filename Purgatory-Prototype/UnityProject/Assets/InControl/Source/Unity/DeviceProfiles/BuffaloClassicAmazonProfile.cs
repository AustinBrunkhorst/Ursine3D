using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class BuffaloClassicAmazonProfile : UnityInputDeviceProfile
	{
		// Right Bumper, Start and Select aren't supported.
		// Possibly they fall outside the number of buttons Unity supports?
		//
		public BuffaloClassicAmazonProfile()
		{
			Name = "Buffalo Class Gamepad";
			Meta = "Buffalo Class Gamepad on Amazon Fire TV";

			SupportedPlatforms = new[] {
				"Amazon AFT",
			};

			JoystickNames = new[] {
				"USB,2-axis 8-button gamepad  "
			};

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "A",
					Target = InputControlType.Action2,
					Source = Button15
				},
				new InputControlMapping {
					Handle = "B",
					Target = InputControlType.Action1,
					Source = Button16
				},
				new InputControlMapping {
					Handle = "X",
					Target = InputControlType.Action4,
					Source = Button17
				},
				new InputControlMapping {
					Handle = "Y",
					Target = InputControlType.Action3,
					Source = Button18
				},
				new InputControlMapping {
					Handle = "Left Bumper",
					Target = InputControlType.LeftBumper,
					Source = Button19
				},
//				new InputControlMapping {
//					Handle = "Right Bumper",
//					Target = InputControlType.RightBumper,
//					Source = new UnityButtonSource( 20 )
//				},
//				new InputControlMapping {
//					Handle = "Select",
//					Target = InputControlType.Select,
//					Source = Button21
//				},
//				new InputControlMapping {
//					Handle = "Start",
//					Target = InputControlType.Start,
//					Source = Button22
//				},
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

