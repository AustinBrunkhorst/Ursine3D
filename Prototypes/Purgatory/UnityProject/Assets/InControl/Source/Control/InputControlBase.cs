using System;
using UnityEngine;


namespace InControl
{
	public abstract class InputControlBase : IInputControl
	{
		public ulong UpdateTick { get; protected set; }

		float sensitivity = 1.0f;
		float lowerDeadZone = 0.0f;
		float upperDeadZone = 1.0f;
		float stateThreshold = 0.0f;

		public float FirstRepeatDelay = 0.8f;
		public float RepeatDelay = 0.1f;

		public bool Raw;

		ulong pendingTick;
		bool pendingCommit;

		float nextRepeatTime;
		float lastPressedTime;
		bool wasRepeated;

		InputControlState lastState;
		InputControlState nextState;
		InputControlState thisState;


		void PrepareForUpdate( ulong updateTick )
		{
			if (updateTick < pendingTick)
			{
				throw new InvalidOperationException( "Cannot be updated with an earlier tick." );
			}

			if (pendingCommit && updateTick != pendingTick)
			{
				throw new InvalidOperationException( "Cannot be updated for a new tick until pending tick is committed." );
			}

			if (updateTick > pendingTick)
			{
				lastState = thisState;
				nextState.Reset();
				pendingTick = updateTick;
				pendingCommit = true;
			}
		}


		public bool UpdateWithState( bool state, ulong updateTick, float deltaTime )
		{
			PrepareForUpdate( updateTick );

			nextState.Set( state || nextState.State );

			return state;
		}


		public bool UpdateWithValue( float value, ulong updateTick, float deltaTime )
		{
			PrepareForUpdate( updateTick );

			if (Utility.Abs( value ) > Utility.Abs( nextState.RawValue ))
			{
				nextState.RawValue = value;

				if (!Raw)
				{
					value = Utility.ApplyDeadZone( value, lowerDeadZone, upperDeadZone );
//					value = Utility.ApplySmoothing( value, lastState.Value, deltaTime, sensitivity );
				}

				nextState.Set( value, stateThreshold );

				return true;
			}

			return false;
		}


		internal void SetValue( float value, ulong updateTick )
		{
			if (updateTick > pendingTick)
			{
				lastState = thisState;
				nextState.Reset();
				pendingTick = updateTick;
				pendingCommit = true;
			}

			nextState.RawValue = value;
			nextState.Set( value, StateThreshold );
		}


		public void Commit()
		{
//			nextState.Set( Utility.ApplySmoothing( nextState.Value, lastState.Value, Time.deltaTime, sensitivity ), stateThreshold );

			thisState = nextState;

			var lastPressed = lastState.State;
			var thisPressed = thisState.State;

			wasRepeated = false;
			if (lastPressed && !thisPressed) // if was released...
			{
				nextRepeatTime = 0.0f;
			}
			else
			if (thisPressed) // if is pressed...
			{
				if (lastPressed != thisPressed) // if has changed...
				{
					nextRepeatTime = Time.realtimeSinceStartup + FirstRepeatDelay;
				}
				else
				if (Time.realtimeSinceStartup >= nextRepeatTime)
				{
					wasRepeated = true;
					nextRepeatTime = Time.realtimeSinceStartup + RepeatDelay;
				}
			}

			if (thisState != lastState)
			{
				UpdateTick = pendingTick;
			}

			pendingCommit = false;
		}


		public void CommitWithState( bool state, ulong updateTick, float deltaTime )
		{
			UpdateWithState( state, updateTick, deltaTime );
			Commit();
		}


		public void CommitWithValue( float value, ulong updateTick, float deltaTime )
		{
			UpdateWithValue( value, updateTick, deltaTime );
			Commit();
		}


		public bool State
		{
			get { return thisState.State; }
		}


		public bool LastState
		{
			get { return lastState.State; }
		}


		public float Value
		{
			get { return thisState.Value; }
		}


		public float LastValue
		{
			get { return lastState.Value; }
		}


		public float RawValue
		{
			get { return thisState.RawValue; }
		}


		internal float NextRawValue
		{
			get { return nextState.RawValue; }
		}


		public bool HasChanged
		{
			get { return thisState != lastState; }
		}


		public bool IsPressed
		{
			get { return thisState.State; }
		}


		public bool WasPressed
		{
			get { return thisState && !lastState; }
		}


		public bool WasReleased
		{
			get { return !thisState && lastState; }
		}


		public bool WasRepeated
		{
			get { return wasRepeated; }
		}


		public float Sensitivity
		{ 
			get { return sensitivity; }
			set
			{
				sensitivity = Mathf.Clamp01( value );
			}
		}


		public float LowerDeadZone
		{ 
			get { return lowerDeadZone; }
			set { lowerDeadZone = Mathf.Clamp01( value ); }
		}


		public float UpperDeadZone
		{ 
			get { return upperDeadZone; }
			set { upperDeadZone = Mathf.Clamp01( value ); }
		}


		public float StateThreshold
		{ 
			get { return stateThreshold; }
			set { stateThreshold = Mathf.Clamp01( value ); }
		}


		public static implicit operator bool( InputControlBase instance )
		{
			return instance.State;
		}


		public static implicit operator float( InputControlBase instance )
		{
			return instance.Value;
		}
	}
}