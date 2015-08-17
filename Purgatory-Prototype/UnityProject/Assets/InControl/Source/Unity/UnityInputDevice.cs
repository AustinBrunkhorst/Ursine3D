using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace InControl
{
	public class UnityInputDevice : InputDevice
	{
		public const int MaxDevices = 10;
		public const int MaxButtons = 20;
		public const int MaxAnalogs = 20;

		internal int JoystickId { get; private set; }
		public InputDeviceProfile Profile { get; protected set; }


		public UnityInputDevice( InputDeviceProfile profile, int joystickId )
			: base( profile.Name )
		{
			Initialize( profile, joystickId );
		}


		public UnityInputDevice( InputDeviceProfile profile )
			: base( profile.Name )
		{
			Initialize( profile, 0 );
		}


		void Initialize( InputDeviceProfile profile, int joystickId )
		{
			Profile = profile;
			Meta = Profile.Meta;

			var analogMappingCount = Profile.AnalogCount;
			for (int i = 0; i < analogMappingCount; i++)
			{
				var analogMapping = Profile.AnalogMappings[i];
				var analogControl = AddControl( analogMapping.Target, analogMapping.Handle );

				analogControl.Sensitivity = Mathf.Min( Profile.Sensitivity, analogMapping.Sensitivity );
				analogControl.LowerDeadZone = Mathf.Max( Profile.LowerDeadZone, analogMapping.LowerDeadZone );
				analogControl.UpperDeadZone = Mathf.Min( Profile.UpperDeadZone, analogMapping.UpperDeadZone );
				analogControl.Raw = analogMapping.Raw;
			}

			var buttonMappingCount = Profile.ButtonCount;
			for (int i = 0; i < buttonMappingCount; i++)
			{
				var buttonMapping = Profile.ButtonMappings[i];
				AddControl( buttonMapping.Target, buttonMapping.Handle );
			}

			JoystickId = joystickId;
			if (joystickId != 0)
			{
				SortOrder = 100 + joystickId;
			}
		}


		public override void Update( ulong updateTick, float deltaTime )
		{
			if (Profile == null)
			{
				return;
			}

			var analogMappingCount = Profile.AnalogCount;
			for (int i = 0; i < analogMappingCount; i++)
			{
				var analogMapping = Profile.AnalogMappings[i];
				var analogValue = analogMapping.Source.GetValue( this );
				var targetControl = GetControl( analogMapping.Target );

				if (!(analogMapping.IgnoreInitialZeroValue && targetControl.IsOnZeroTick && Utility.IsZero( analogValue )))
				{
					var mappedValue = analogMapping.MapValue( analogValue );
					targetControl.UpdateWithValue( mappedValue, updateTick, deltaTime );
				}
			}

			var buttonMappingCount = Profile.ButtonCount;
			for (int i = 0; i < buttonMappingCount; i++)
			{
				var buttonMapping = Profile.ButtonMappings[i];
				var buttonState = buttonMapping.Source.GetState( this );

				UpdateWithState( buttonMapping.Target, buttonState, updateTick, deltaTime );
			}
		}


		public override bool IsSupportedOnThisPlatform
		{
			get
			{ 
				return Profile != null && Profile.IsSupportedOnThisPlatform; 
			}
		}


		public override bool IsKnown
		{
			get
			{ 
				return Profile != null && Profile.IsKnown; 
			}
		}
	}
}

