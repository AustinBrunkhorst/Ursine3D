using System;

namespace InControl
{
	// @cond nodoc
	[AutoDiscover]
	public class IpegaPG9023AndroidProfile : UnityInputDeviceProfile
	{
		// https://github.com/pbhogan/InControl/issues/202
		//
		public IpegaPG9023AndroidProfile()
		{
			Name = "IPEGA PG-9023";
			Meta = "IPEGA PG-9023 on Android";

			SupportedPlatforms = new[] {
				"Android"
			};

			JoystickNames = new[] {
				"ipega Extending Game Controller"
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
					Handle = "Select",
					Target = InputControlType.Select,
					Source = Button11
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
					Handle = "L2",
					Target = InputControlType.LeftTrigger,
					Source = Button6
				},
				new InputControlMapping {
					Handle = "R1",
					Target = InputControlType.RightBumper,
					Source = Button5
				},
				new InputControlMapping {
					Handle = "R2",
					Target = InputControlType.RightTrigger,
					Source = Button7
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
				DPadDownMapping( Analog5 )
			};
		}
	}
	// @endcond
}

