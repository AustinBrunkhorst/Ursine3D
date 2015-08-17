using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class GoogleNexusPlayerRemoteProfile : UnityInputDeviceProfile
	{
		public GoogleNexusPlayerRemoteProfile()
		{
			Name = "Google Nexus Player Remote";
			Meta = "Google Nexus Player Remote";

			SupportedPlatforms = new[] {
				"Android"
			};

			JoystickNames = new[] {
				"Google Nexus Remote"
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
				}
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
