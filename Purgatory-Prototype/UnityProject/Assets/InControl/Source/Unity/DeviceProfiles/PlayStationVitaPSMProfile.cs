using System;


namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class PlayStationVitaPSMProfile : UnityInputDeviceProfile
	{
		public PlayStationVitaPSMProfile()
		{
			Name = "PlayStation Mobile";
			Meta = "PlayStation Mobile on Vita";

			SupportedPlatforms = new[] {
				"PSM UNITY FOR PSM",
				"PSM ON PS VITA",
				"PS VITA",
				"PSP2OS"
			};

			JoystickNames = new[] {
				"PS Vita"
			};

			ButtonMappings = new[] {
				new InputControlMapping {
					Handle = "Cross",
					Target = InputControlType.Action1,
					Source = Button0
				},
				new InputControlMapping {
					Handle = "Circle",
					Target = InputControlType.Action2,
					Source = Button1
				},
				new InputControlMapping {
					Handle = "Square",
					Target = InputControlType.Action3,
					Source = Button2
				},
				new InputControlMapping {
					Handle = "Triangle",
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
					Handle = "Select",
					Target = InputControlType.Select,
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
			};
		}
	}
	// @endcond
}
