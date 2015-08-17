using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class AndroidTVRemoteProfile : UnityInputDeviceProfile
	{
		public AndroidTVRemoteProfile()
		{
			Name = "Android TV Remote";
			Meta = "Android TV Remote on Android TV";

			SupportedPlatforms = new[] {
				"Android"
			};

			JoystickNames = new[] {
				"",
				"touch-input",
				"navigation-input"
			};

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "A",
					Target = InputControlType.Action1,
					Source = Button0
				},
				new InputControlMapping {
					Handle = "Back",
					Target = InputControlType.Back,
					Source = EscapeKey
				},
			};

			AnalogMappings = new[] {
				DPadLeftMapping( Analog4 ),
				DPadRightMapping( Analog4 ),
				DPadUpMapping( Analog5 ),
				DPadDownMapping( Analog5 ),
			};
		}
	}
	// @endcond
}
