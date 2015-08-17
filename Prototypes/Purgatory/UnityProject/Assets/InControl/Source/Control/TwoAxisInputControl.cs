using System;
using UnityEngine;


namespace InControl
{
	public class TwoAxisInputControl : IInputControl
	{
		public float X { get; protected set; }
		public float Y { get; protected set; }

		public OneAxisInputControl Left { get; protected set; }
		public OneAxisInputControl Right { get; protected set; }
		public OneAxisInputControl Up { get; protected set; }
		public OneAxisInputControl Down { get; protected set; }

		public ulong UpdateTick { get; protected set; }

		public float LowerDeadZone = 0.0f;
		public float UpperDeadZone = 1.0f;

		public bool Raw;

		bool thisState;
		bool lastState;
		Vector2 thisValue;
		Vector2 lastValue;


		public TwoAxisInputControl()
		{
			Left = new OneAxisInputControl();
			Right = new OneAxisInputControl();
			Up = new OneAxisInputControl();
			Down = new OneAxisInputControl();
		}


		// TODO: Is there a better way to handle this? Maybe calculate deltaTime internally.
		public void Filter( TwoAxisInputControl twoAxisInputControl, float deltaTime )
		{
			UpdateWithAxes( twoAxisInputControl.X, twoAxisInputControl.Y, InputManager.CurrentTick, deltaTime );
		}


		internal void UpdateWithAxes( float x, float y, ulong updateTick, float deltaTime )
		{
			lastState = thisState;
			lastValue = thisValue;

			if (Raw)
			{
				thisValue = new Vector2( x, y );
			}
			else
			{
				thisValue = Utility.ApplyCircularDeadZone( x, y, LowerDeadZone, UpperDeadZone );
			}

			X = thisValue.x;
			Y = thisValue.y;

			Left.CommitWithValue( Mathf.Max( 0.0f, -X ), updateTick, deltaTime );
			Right.CommitWithValue( Mathf.Max( 0.0f, X ), updateTick, deltaTime );

			if (InputManager.InvertYAxis)
			{
				Up.CommitWithValue( Mathf.Max( 0.0f, -Y ), updateTick, deltaTime );
				Down.CommitWithValue( Mathf.Max( 0.0f, Y ), updateTick, deltaTime );
			}
			else
			{
				Up.CommitWithValue( Mathf.Max( 0.0f, Y ), updateTick, deltaTime );
				Down.CommitWithValue( Mathf.Max( 0.0f, -Y ), updateTick, deltaTime );
			}

			thisState = Up.State || Down.State || Left.State || Right.State;

			if (thisValue != lastValue)
			{
				UpdateTick = updateTick;
				HasChanged = true;
			}
			else
			{
				HasChanged = false;
			}
		}


		//		internal void UpdateWithAxes( OneAxisInputControl horizontalAxis, OneAxisInputControl verticalAxis, ulong updateTick, float deltaTime )
		//		{
		//			LowerDeadZone = Mathf.Max( horizontalAxis.LowerDeadZone, verticalAxis.LowerDeadZone );
		//			UpperDeadZone = Mathf.Min( horizontalAxis.UpperDeadZone, verticalAxis.UpperDeadZone );
		//			Raw = horizontalAxis.Raw || verticalAxis.Raw;
		//			UpdateWithAxes( horizontalAxis.RawValue, verticalAxis.RawValue, updateTick, deltaTime );
		//		}


		float stateThreshold = 0.0f;
		public float StateThreshold
		{
			set
			{
				stateThreshold = value;
				Left.StateThreshold = value;
				Right.StateThreshold = value;
				Up.StateThreshold = value;
				Down.StateThreshold = value;
			}

			get
			{
				return stateThreshold;
			}
		}


		public bool State
		{
			get { return thisState; }
		}


		public bool LastState
		{
			get { return lastState; }
		}


		public Vector2 Value
		{ 
			get { return thisValue; }
		}


		public Vector2 LastValue
		{ 
			get { return lastValue; }
		}


		public Vector2 Vector
		{ 
			get { return thisValue; }
		}


		public bool HasChanged
		{
			get;
			protected set;
		}


		public bool IsPressed
		{
			get { return thisState; }
		}


		public bool WasPressed
		{
			get { return thisState && !lastState; }
		}


		public bool WasReleased
		{
			get { return !thisState && lastState; }
		}


		public float Angle
		{
			get
			{
				return Utility.VectorToAngle( thisValue );
			}
		}


		public static implicit operator bool( TwoAxisInputControl instance )
		{
			return instance.thisState;
		}


		public static implicit operator Vector2( TwoAxisInputControl instance )
		{
			return instance.thisValue;
		}


		public static implicit operator Vector3( TwoAxisInputControl instance )
		{
			return instance.thisValue;
		}
	}
}

