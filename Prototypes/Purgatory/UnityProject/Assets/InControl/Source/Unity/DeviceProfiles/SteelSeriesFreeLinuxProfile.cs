using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class SteelSeriesFreeLinuxProfile : UnityInputDeviceProfile
	{
		public SteelSeriesFreeLinuxProfile()
		{
			Name = "SteelSeries Free";
			Meta = "SteelSeries Free on Linux";

			SupportedPlatforms = new[] {
				"Linux",
			};

			JoystickNames = new[] {
				"Zeemote: SteelSeries FREE"
			};

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "4",
					Target = InputControlType.Action1,
					Source = Button0
				},
				new InputControlMapping {
					Handle = "3",
					Target = InputControlType.Action2,
					Source = Button1
				},
				new InputControlMapping {
					Handle = "1",
					Target = InputControlType.Action3,
					Source = Button3
				},
				new InputControlMapping {
					Handle = "2",
					Target = InputControlType.Action4,
					Source = Button4
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
					Handle = "Back",
					Target = InputControlType.Select,
					Source = Button12
				},
				new InputControlMapping {
					Handle = "Start",
					Target = InputControlType.Start,
					Source = Button11
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

				DPadLeftMapping( Analog4 ),
				DPadRightMapping( Analog4 ),
				DPadUpMapping( Analog5 ),
				DPadDownMapping( Analog5 ),
			};
		}
	}
	// @endcond
}

