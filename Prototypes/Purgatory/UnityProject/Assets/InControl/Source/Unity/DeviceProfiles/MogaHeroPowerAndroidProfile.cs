using System;

namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class MogaHeroPowerAndroidProfile : UnityInputDeviceProfile
	{
		public MogaHeroPowerAndroidProfile()
		{
			Name = "Moga Hero Power";
			Meta = "Moga Hero Power on Android";

			SupportedPlatforms = new[] {
				"Android"
			};

			JoystickNames = new[] {
				"Moga 2 HID"
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
					Source = Button2
				},
				new InputControlMapping {
					Handle = "Y",
					Target = InputControlType.Action4,
					Source = Button3
				},
				new InputControlMapping {
					Handle = "Start",
					Target = InputControlType.Start,
					Source = Button10
				},
				new InputControlMapping {
					Handle = "L1",
					Target = InputControlType.LeftBumper,
					Source = Button4
				},
				new InputControlMapping {
					Handle = "R1",
					Target = InputControlType.RightBumper,
					Source = Button5
				},
				new InputControlMapping {
					Handle = "Left Stick Button",
					Target = InputControlType.LeftStickButton,
					Source = Button8
				},
				new InputControlMapping {
					Handle = "Right Stick Button",
					Target = InputControlType.RightStickButton,
					Source = Button9
				},
			};

			AnalogMappings = new[] {
				LeftTriggerMapping( Analog12 ),
				RightTriggerMapping( Analog11 ),

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
				DPadDownMapping( Analog5 )
			};

			AnalogMappings[0].SourceRange = InputRange.ZeroToOne;
			AnalogMappings[1].SourceRange = InputRange.ZeroToOne;
		}
	}
	// @endcond
}
