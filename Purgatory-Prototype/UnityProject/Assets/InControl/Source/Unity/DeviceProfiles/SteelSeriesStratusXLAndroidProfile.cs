using System;

namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class SteelSeriesStratusXLAndroidProfile : UnityInputDeviceProfile
	{
		// WARNING: The DPad doesn't work. I'm very surprised this actually works at all.
		// https://github.com/pbhogan/InControl/issues/203
		//
		public SteelSeriesStratusXLAndroidProfile()
		{
			Name = "SteelSeries Stratus XL";
			Meta = "SteelSeries Stratus XL on Android";

			SupportedPlatforms = new[] {
				"Android",
			};

			JoystickNames = new[] {
				"SteelSeries Stratus XL",
			};

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
					Source = Button13
				},
				new InputControlMapping {
					Handle = "Y",
					Target = InputControlType.Action4,
					Source = Button2
				},
				new InputControlMapping {
					Handle = "L1",
					Target = InputControlType.LeftBumper,
					Source = Button3
				},
				new InputControlMapping {
					Handle = "R1",
					Target = InputControlType.RightBumper,
					Source = Button14
				},
				new InputControlMapping {
					Handle = "L2",
					Target = InputControlType.LeftTrigger,
					Source = Button4
				},
				new InputControlMapping {
					Handle = "R2",
					Target = InputControlType.RightTrigger,
					Source = Button5
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
			};

			AnalogMappings[2].SourceRange = InputRange.ZeroToOne;
			AnalogMappings[3].SourceRange = InputRange.ZeroToMinusOne;
			AnalogMappings[6].SourceRange = InputRange.ZeroToOne;
			AnalogMappings[7].SourceRange = InputRange.ZeroToMinusOne;
		}
	}
	// @endcond
}

