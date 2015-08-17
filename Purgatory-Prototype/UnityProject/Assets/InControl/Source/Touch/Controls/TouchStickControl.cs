using System.IO;
using UnityEngine;


namespace InControl
{
	public class TouchStickControl : TouchControl
	{
		[Header( "Position" )]

		[SerializeField]
		TouchControlAnchor anchor = TouchControlAnchor.BottomLeft;

		[SerializeField]
		TouchUnitType offsetUnitType = TouchUnitType.Percent;

		[SerializeField]
		Vector2 offset = new Vector2( 20.0f, 20.0f );

		[SerializeField]
		TouchUnitType areaUnitType = TouchUnitType.Percent;

		[SerializeField]
		Rect activeArea = new Rect( 0.0f, 0.0f, 50.0f, 100.0f );


		[Header( "Options" )]

		public AnalogTarget target = AnalogTarget.LeftStick;

		[Range( 0, 1 )] 
		public float lowerDeadZone = 0.1f;

		[Range( 0, 1 )] 
		public float upperDeadZone = 0.9f;

		public AnimationCurve inputCurve = AnimationCurve.Linear( 0.0f, 0.0f, 1.0f, 1.0f );

		public bool allowDragging = false;
		public bool snapToInitialTouch = true;
		public bool resetWhenDone = true;
		public float resetDuration = 0.1f;


		[Header( "Sprites" )]

		public TouchSprite ring = new TouchSprite( 20.0f );
		public TouchSprite knob = new TouchSprite( 10.0f );
		public float knobRange = 7.5f;


		Vector3 resetPosition;
		Vector3 beganPosition;
		Vector3 movedPosition;
		float ringResetSpeed;
		float knobResetSpeed;
		Rect worldActiveArea;
		float worldKnobRange;
		Vector3 value;
		Touch currentTouch;
		bool dirty;


		public override void CreateControl()
		{
			ring.Create( "Ring", transform, 1000 );
			knob.Create( "Knob", transform, 1001 );
		}


		public override void DestroyControl()
		{
			ring.Delete();
			knob.Delete();

			if (currentTouch != null)
			{
				TouchEnded( currentTouch );
				currentTouch = null;
			}
		}


		public override void ConfigureControl()
		{
			resetPosition = OffsetToWorldPosition( anchor, offset, offsetUnitType, true );
			transform.position = resetPosition;

			ring.Update( true );
			knob.Update( true );

			worldActiveArea = TouchManager.ConvertToWorld( activeArea, areaUnitType );
			worldKnobRange = TouchManager.ConvertToWorld( knobRange, knob.SizeUnitType );
		}


		public override void DrawGizmos()
		{
			ring.DrawGizmos( RingPosition, Color.yellow );
			knob.DrawGizmos( KnobPosition, Color.yellow );
			Utility.DrawCircleGizmo( RingPosition, worldKnobRange, Color.red );
			Utility.DrawRectGizmo( worldActiveArea, Color.green );
		}


		void Update()
		{
			if (dirty)
			{
				ConfigureControl();
				dirty = false;
			}
			else
			{
				ring.Update();
				knob.Update();
			}

			if (IsNotActive)
			{
				if (resetWhenDone && KnobPosition != resetPosition)
				{
					var ringKnobDelta = KnobPosition - RingPosition;
					RingPosition = Vector3.MoveTowards( RingPosition, resetPosition, ringResetSpeed * Time.deltaTime );
					KnobPosition = RingPosition + ringKnobDelta;
				}

				if (KnobPosition != RingPosition)
				{
					KnobPosition = Vector3.MoveTowards( KnobPosition, RingPosition, knobResetSpeed * Time.deltaTime );
				}
			}
		}


		public override void SubmitControlState( ulong updateTick, float deltaTime )
		{
			SubmitAnalogValue( target, value, lowerDeadZone, upperDeadZone, updateTick, deltaTime );
		}


		public override void CommitControlState( ulong updateTick, float deltaTime )
		{
			CommitAnalog( target );
		}


		public override void TouchBegan( Touch touch )
		{
			if (IsActive)
			{
				return;
			}

			beganPosition = TouchManager.ScreenToWorldPoint( touch.position );

			var insideActiveArea = worldActiveArea.Contains( beganPosition );
			var insideControl = ring.Contains( beganPosition );

			if (snapToInitialTouch && (insideActiveArea || insideControl))
			{
				RingPosition = beganPosition;
				KnobPosition = beganPosition;
				currentTouch = touch;
			}
			else
			if (insideControl)
			{
				KnobPosition = beganPosition;
				beganPosition = RingPosition;
				currentTouch = touch;
			}

			if (IsActive)
			{
				TouchMoved( touch );

				ring.State = true;
				knob.State = true;
			}
		}


		public override void TouchMoved( Touch touch )
		{
			if (currentTouch != touch)
			{
				return;
			}

			movedPosition = TouchManager.ScreenToWorldPoint( touch.position );

			var vector = movedPosition - beganPosition;
			var normal = vector.normalized;
			var length = vector.magnitude;

			if (allowDragging)
			{
				var excess = length - worldKnobRange;
				if (excess < 0.0f)
				{
					excess = 0.0f;
				}
				beganPosition = beganPosition + (excess * normal);
				RingPosition = beganPosition;
			}

			movedPosition = beganPosition + (Mathf.Clamp( length, 0.0f, worldKnobRange ) * normal);

			value = (movedPosition - beganPosition) / worldKnobRange;
			value.x = inputCurve.Evaluate( Utility.Abs( value.x ) ) * Mathf.Sign( value.x );
			value.y = inputCurve.Evaluate( Utility.Abs( value.y ) ) * Mathf.Sign( value.y );

			KnobPosition = movedPosition;
			RingPosition = beganPosition;
		}


		public override void TouchEnded( Touch touch )
		{
			if (currentTouch != touch)
			{
				return;
			}

			value = Vector3.zero;

			var ringResetDelta = (resetPosition - RingPosition).magnitude;
			ringResetSpeed = Utility.IsZero( resetDuration ) ? ringResetDelta : (ringResetDelta / resetDuration);

			var knobResetDelta = (RingPosition - KnobPosition).magnitude;
			knobResetSpeed = Utility.IsZero( resetDuration ) ? knobRange : (knobResetDelta / resetDuration);

			currentTouch = null;

			ring.State = false;
			knob.State = false;
		}


		public bool IsActive
		{
			get
			{
				return currentTouch != null;
			}
		}


		public bool IsNotActive
		{
			get
			{
				return currentTouch == null;
			}
		}


		public Vector3 RingPosition
		{
			get
			{
				return ring.Ready ? ring.Position : transform.position;
			}

			set
			{
				if (ring.Ready)
				{
					ring.Position = value;
				}
			}
		}


		public Vector3 KnobPosition
		{
			get
			{
				return knob.Ready ? knob.Position : transform.position;
			}

			set
			{
				if (knob.Ready)
				{
					knob.Position = value;
				}
			}
		}


		public TouchControlAnchor Anchor
		{ 
			get
			{
				return anchor;
			}

			set
			{
				if (anchor != value)
				{
					anchor = value;
					dirty = true;
				}
			}
		}


		public Vector2 Offset
		{ 
			get
			{
				return offset;
			}

			set
			{
				if (offset != value)
				{
					offset = value;
					dirty = true;
				}
			}
		}


		public TouchUnitType OffsetUnitType
		{ 
			get
			{
				return offsetUnitType;
			}

			set
			{
				if (offsetUnitType != value)
				{
					offsetUnitType = value;
					dirty = true;
				}
			}
		}


		public Rect ActiveArea
		{ 
			get
			{
				return activeArea;
			}

			set
			{
				if (activeArea != value)
				{
					activeArea = value;
					dirty = true;
				}
			}
		}


		public TouchUnitType AreaUnitType
		{ 
			get
			{
				return areaUnitType;
			}

			set
			{
				if (areaUnitType != value)
				{
					areaUnitType = value;
					dirty = true;
				}
			}
		}
	}
}

