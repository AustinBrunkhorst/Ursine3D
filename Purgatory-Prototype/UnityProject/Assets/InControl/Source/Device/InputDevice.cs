using System;
using UnityEngine;


namespace InControl
{
	public class InputDevice
	{
		public static readonly InputDevice Null = new InputDevice( "None" );

		internal int SortOrder = int.MaxValue;

		public string Name { get; protected set; }
		public string Meta { get; protected set; }

		public ulong LastChangeTick { get; protected set; }

		public InputControl[] Controls { get; protected set; }

		public OneAxisInputControl LeftStickX { get; protected set; }
		public OneAxisInputControl LeftStickY { get; protected set; }
		public TwoAxisInputControl LeftStick { get; protected set; }

		public OneAxisInputControl RightStickX { get; protected set; }
		public OneAxisInputControl RightStickY { get; protected set; }
		public TwoAxisInputControl RightStick { get; protected set; }

		public OneAxisInputControl DPadX { get; protected set; }
		public OneAxisInputControl DPadY { get; protected set; }
		public TwoAxisInputControl DPad { get; protected set; }

		public InputControl Command { get; protected set; }

		public bool IsAttached { get; internal set; }


		public InputDevice( string name )
		{
			Name = name;
			Meta = "";

			LastChangeTick = 0;

			const int numInputControlTypes = (int) InputControlType.Count + 1;
			Controls = new InputControl[numInputControlTypes];

			LeftStickX = new OneAxisInputControl();
			LeftStickY = new OneAxisInputControl();
			LeftStick = new TwoAxisInputControl();

			RightStickX = new OneAxisInputControl();
			RightStickY = new OneAxisInputControl();
			RightStick = new TwoAxisInputControl();

			DPadX = new OneAxisInputControl();
			DPadY = new OneAxisInputControl();
			DPad = new TwoAxisInputControl();

			Command = AddControl( InputControlType.Command, "Command" );
		}


		public bool HasControl( InputControlType inputControlType )
		{
			return Controls[(int) inputControlType] != null;
		}


		public InputControl GetControl( InputControlType inputControlType )
		{
			var control = Controls[(int) inputControlType];
			return control ?? InputControl.Null;
		}


		// Warning: this is super inefficient. Don't use it unless you have to, m'kay?
		public static InputControlType GetInputControlTypeByName( string inputControlName )
		{
			return (InputControlType) Enum.Parse( typeof(InputControlType), inputControlName );
		}


		// Warning: this is super inefficient. Don't use it unless you have to, m'kay?
		public InputControl GetControlByName( string inputControlName )
		{
			var inputControlType = GetInputControlTypeByName( inputControlName );
			return GetControl( inputControlType );
		}


		public InputControl AddControl( InputControlType inputControlType, string handle )
		{
			var inputControl = new InputControl( handle, inputControlType );
			Controls[(int) inputControlType] = inputControl;
			return inputControl;
		}


		public InputControl AddControl( InputControlType inputControlType, string handle, float lowerDeadZone, float upperDeadZone )
		{
			var inputControl = AddControl( inputControlType, handle );
			inputControl.LowerDeadZone = lowerDeadZone;
			inputControl.UpperDeadZone = upperDeadZone;
			return inputControl;
		}


		internal void UpdateWithState( InputControlType inputControlType, bool state, ulong updateTick, float deltaTime )
		{
			GetControl( inputControlType ).UpdateWithState( state, updateTick, deltaTime );
		}


		internal void UpdateWithValue( InputControlType inputControlType, float value, ulong updateTick, float deltaTime )
		{
			GetControl( inputControlType ).UpdateWithValue( value, updateTick, deltaTime );
		}


		internal void UpdateLeftStickWithValue( Vector2 value, ulong updateTick, float deltaTime )
		{
			LeftStickLeft.UpdateWithValue( Mathf.Max( 0.0f, -value.x ), updateTick, deltaTime );
			LeftStickRight.UpdateWithValue( Mathf.Max( 0.0f, value.x ), updateTick, deltaTime );

			if (InputManager.InvertYAxis)
			{
				LeftStickUp.UpdateWithValue( Mathf.Max( 0.0f, -value.y ), updateTick, deltaTime );
				LeftStickDown.UpdateWithValue( Mathf.Max( 0.0f, value.y ), updateTick, deltaTime );
			}
			else
			{
				LeftStickUp.UpdateWithValue( Mathf.Max( 0.0f, value.y ), updateTick, deltaTime );
				LeftStickDown.UpdateWithValue( Mathf.Max( 0.0f, -value.y ), updateTick, deltaTime );
			}
		}


		internal void CommitLeftStick()
		{
			LeftStickUp.Commit();
			LeftStickDown.Commit();
			LeftStickLeft.Commit();
			LeftStickRight.Commit();
		}


		internal void UpdateRightStickWithValue( Vector2 value, ulong updateTick, float deltaTime )
		{
			RightStickLeft.UpdateWithValue( Mathf.Max( 0.0f, -value.x ), updateTick, deltaTime );
			RightStickRight.UpdateWithValue( Mathf.Max( 0.0f, value.x ), updateTick, deltaTime );

			if (InputManager.InvertYAxis)
			{
				RightStickUp.UpdateWithValue( Mathf.Max( 0.0f, -value.y ), updateTick, deltaTime );
				RightStickDown.UpdateWithValue( Mathf.Max( 0.0f, value.y ), updateTick, deltaTime );
			}
			else
			{
				RightStickUp.UpdateWithValue( Mathf.Max( 0.0f, value.y ), updateTick, deltaTime );
				RightStickDown.UpdateWithValue( Mathf.Max( 0.0f, -value.y ), updateTick, deltaTime );
			}
		}


		internal void CommitRightStick()
		{
			RightStickUp.Commit();
			RightStickDown.Commit();
			RightStickLeft.Commit();
			RightStickRight.Commit();
		}


		public virtual void Update( ulong updateTick, float deltaTime )
		{
			// Implemented by subclasses.
		}


		bool AnyCommandControlIsPressed()
		{
			for (int i = (int) InputControlType.Back; i <= (int) InputControlType.Power; i++)
			{
				var control = Controls[i];
				if (control != null && control.IsPressed)
				{
					return true;
				}
			}

			return false;
		}


		internal void ProcessLeftStick( ulong updateTick, float deltaTime )
		{
			var lowerDeadZone = Utility.Max( LeftStickLeft.LowerDeadZone, LeftStickRight.LowerDeadZone, LeftStickUp.LowerDeadZone, LeftStickDown.LowerDeadZone );
			var upperDeadZone = Utility.Min( LeftStickLeft.UpperDeadZone, LeftStickRight.UpperDeadZone, LeftStickUp.UpperDeadZone, LeftStickDown.UpperDeadZone );

			var x = Utility.ValueFromSides( LeftStickLeft.NextRawValue, LeftStickRight.NextRawValue );
			var y = Utility.ValueFromSides( LeftStickDown.NextRawValue, LeftStickUp.NextRawValue, InputManager.InvertYAxis );
			var v = Utility.ApplyCircularDeadZone( x, y, lowerDeadZone, upperDeadZone );

			LeftStick.Raw = true;
			LeftStick.UpdateWithAxes( v.x, v.y, updateTick, deltaTime );

			LeftStickX.Raw = true;
			LeftStickX.CommitWithValue( v.x, updateTick, deltaTime );

			LeftStickY.Raw = true;
			LeftStickY.CommitWithValue( v.y, updateTick, deltaTime );

			LeftStickLeft.SetValue( LeftStick.Left.Value, updateTick );
			LeftStickRight.SetValue( LeftStick.Right.Value, updateTick );
			LeftStickUp.SetValue( LeftStick.Up.Value, updateTick );
			LeftStickDown.SetValue( LeftStick.Down.Value, updateTick );
		}


		internal void ProcessRightStick( ulong updateTick, float deltaTime )
		{
			var lowerDeadZone = Utility.Max( RightStickLeft.LowerDeadZone, RightStickRight.LowerDeadZone, RightStickUp.LowerDeadZone, RightStickDown.LowerDeadZone );
			var upperDeadZone = Utility.Min( RightStickLeft.UpperDeadZone, RightStickRight.UpperDeadZone, RightStickUp.UpperDeadZone, RightStickDown.UpperDeadZone );

			var x = Utility.ValueFromSides( RightStickLeft.NextRawValue, RightStickRight.NextRawValue );
			var y = Utility.ValueFromSides( RightStickDown.NextRawValue, RightStickUp.NextRawValue, InputManager.InvertYAxis );
			var v = Utility.ApplyCircularDeadZone( x, y, lowerDeadZone, upperDeadZone );

			RightStick.Raw = true;
			RightStick.UpdateWithAxes( v.x, v.y, updateTick, deltaTime );

			RightStickX.Raw = true;
			RightStickX.CommitWithValue( v.x, updateTick, deltaTime );

			RightStickY.Raw = true;
			RightStickY.CommitWithValue( v.y, updateTick, deltaTime );

			RightStickLeft.SetValue( RightStick.Left.Value, updateTick );
			RightStickRight.SetValue( RightStick.Right.Value, updateTick );
			RightStickUp.SetValue( RightStick.Up.Value, updateTick );
			RightStickDown.SetValue( RightStick.Down.Value, updateTick );
		}


		internal void ProcessDPad( ulong updateTick, float deltaTime )
		{
			var lowerDeadZone = Utility.Max( DPadLeft.LowerDeadZone, DPadRight.LowerDeadZone, DPadUp.LowerDeadZone, DPadDown.LowerDeadZone );
			var upperDeadZone = Utility.Min( DPadLeft.UpperDeadZone, DPadRight.UpperDeadZone, DPadUp.UpperDeadZone, DPadDown.UpperDeadZone );

			var x = Utility.ValueFromSides( DPadLeft.NextRawValue, DPadRight.NextRawValue );
			var y = Utility.ValueFromSides( DPadDown.NextRawValue, DPadUp.NextRawValue, InputManager.InvertYAxis );
			var v = Utility.ApplyCircularDeadZone( x, y, lowerDeadZone, upperDeadZone );

			DPad.Raw = true;
			DPad.UpdateWithAxes( v.x, v.y, updateTick, deltaTime );

			DPadX.Raw = true;
			DPadX.CommitWithValue( v.x, updateTick, deltaTime );

			DPadY.Raw = true;
			DPadY.CommitWithValue( v.y, updateTick, deltaTime );

			DPadLeft.SetValue( DPad.Left.Value, updateTick );
			DPadRight.SetValue( DPad.Right.Value, updateTick );
			DPadUp.SetValue( DPad.Up.Value, updateTick );
			DPadDown.SetValue( DPad.Down.Value, updateTick );
		}


		public void Commit( ulong updateTick, float deltaTime )
		{
			// We need to do some processing to ensure all the various objects
			// holding directional values are calculated optimally with circular 
			// deadzones and then set properly everywhere.
			ProcessLeftStick( updateTick, deltaTime );
			ProcessRightStick( updateTick, deltaTime );
			ProcessDPad( updateTick, deltaTime );

			// Next, commit all control values.
			int controlCount = Controls.Length;
			for (int i = 0; i < controlCount; i++)
			{
				var control = Controls[i];
				if (control != null)
				{
					control.Commit();

					if (control.HasChanged)
					{
						LastChangeTick = updateTick;
					}
				}
			}

			// Calculate the "Command" control for known controllers and commit it.
			if (IsKnown)
			{
				Command.CommitWithState( AnyCommandControlIsPressed(), updateTick, deltaTime );
			}
		}


		public bool LastChangedAfter( InputDevice otherDevice )
		{
			return LastChangeTick > otherDevice.LastChangeTick;
		}


		internal void RequestActivation()
		{
			LastChangeTick = InputManager.CurrentTick;
		}


		public virtual void Vibrate( float leftMotor, float rightMotor )
		{
		}


		public void Vibrate( float intensity )
		{
			Vibrate( intensity, intensity );
		}


		public virtual bool IsSupportedOnThisPlatform
		{
			get { return true; }
		}


		public virtual bool IsKnown
		{
			get { return true; }
		}


		public bool IsUnknown
		{
			get { return !IsKnown; }
		}


		public bool MenuWasPressed
		{
			get
			{
				return GetControl( InputControlType.Command ).WasPressed;
			}
		}


		public InputControl AnyButton
		{
			get
			{
				int controlCount = Controls.GetLength( 0 );
				for (int i = 0; i < controlCount; i++)
				{
					var control = Controls[i];
					if (control != null && control.IsButton && control.IsPressed)
					{
						return control;
					}
				}

				return InputControl.Null;
			}
		}

		public InputControl LeftStickUp { get { return GetControl( InputControlType.LeftStickUp ); } }
		public InputControl LeftStickDown { get { return GetControl( InputControlType.LeftStickDown ); } }
		public InputControl LeftStickLeft { get { return GetControl( InputControlType.LeftStickLeft ); } }
		public InputControl LeftStickRight { get { return GetControl( InputControlType.LeftStickRight ); } }

		public InputControl RightStickUp { get { return GetControl( InputControlType.RightStickUp ); } }
		public InputControl RightStickDown { get { return GetControl( InputControlType.RightStickDown ); } }
		public InputControl RightStickLeft { get { return GetControl( InputControlType.RightStickLeft ); } }
		public InputControl RightStickRight { get { return GetControl( InputControlType.RightStickRight ); } }

		public InputControl DPadUp { get { return GetControl( InputControlType.DPadUp ); } }
		public InputControl DPadDown { get { return GetControl( InputControlType.DPadDown ); } }
		public InputControl DPadLeft { get { return GetControl( InputControlType.DPadLeft ); } }
		public InputControl DPadRight { get { return GetControl( InputControlType.DPadRight ); } }

		public InputControl Action1 { get { return GetControl( InputControlType.Action1 ); } }
		public InputControl Action2 { get { return GetControl( InputControlType.Action2 ); } }
		public InputControl Action3 { get { return GetControl( InputControlType.Action3 ); } }
		public InputControl Action4 { get { return GetControl( InputControlType.Action4 ); } }

		public InputControl LeftTrigger { get { return GetControl( InputControlType.LeftTrigger ); } }
		public InputControl RightTrigger { get { return GetControl( InputControlType.RightTrigger ); } }

		public InputControl LeftBumper { get { return GetControl( InputControlType.LeftBumper ); } }
		public InputControl RightBumper { get { return GetControl( InputControlType.RightBumper ); } }

		public InputControl LeftStickButton { get { return GetControl( InputControlType.LeftStickButton ); } }
		public InputControl RightStickButton { get { return GetControl( InputControlType.RightStickButton ); } }


		public TwoAxisInputControl Direction
		{
			get
			{
				return DPad.UpdateTick > LeftStick.UpdateTick ? DPad : LeftStick;
			}
		}


		public static implicit operator bool( InputDevice device )
		{
			return device != null;
		}
	}
}

