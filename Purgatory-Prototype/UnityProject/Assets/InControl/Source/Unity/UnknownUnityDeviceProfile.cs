using System;


namespace InControl
{
	public sealed class UnknownUnityDeviceProfile : UnityInputDeviceProfile
	{
		public UnknownUnityDeviceProfile( string joystickName )
		{
			Name = "Unknown Controller";
			Meta = "\"" + joystickName + "\"";

			Sensitivity = 1.0f;
			LowerDeadZone = 0.2f;
			UpperDeadZone = 0.9f;

			SupportedPlatforms = null;
			JoystickNames = new[] { joystickName };

			AnalogMappings = new InputControlMapping[ UnityInputDevice.MaxAnalogs + 4 ];

			// I've yet to encounter a device that doesn't have this mapping.
			AnalogMappings[0] = LeftStickLeftMapping( Analog0 );
			AnalogMappings[1] = LeftStickRightMapping( Analog0 );
			AnalogMappings[2] = LeftStickUpMapping( Analog1 );
			AnalogMappings[3] = LeftStickDownMapping( Analog1 );

			for (int i = 0; i < UnityInputDevice.MaxAnalogs; i++)
			{
				AnalogMappings[i + 4] = new InputControlMapping {
					Handle = "Analog " + i,
					Source = Analog( i ),
					Target = InputControlType.Analog0 + i
				};
			}

			ButtonMappings = new InputControlMapping[ UnityInputDevice.MaxButtons ];
			for (int i = 0; i < UnityInputDevice.MaxButtons; i++)
			{
				ButtonMappings[i] = new InputControlMapping {
					Handle = "Button " + i,
					Source = Button( i ),
					Target = InputControlType.Button0 + i
				};
			}
		}


		public sealed override bool IsKnown
		{
			get
			{ 
				return false; 
			}
		}
	}
}

