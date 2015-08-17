using System;
using System.Collections.Generic;
using UnityEngine;


namespace InControl
{
	public class TouchTrackControl : TouchControl
	{
		[Header( "Dimensions" )]

		[SerializeField]
		TouchUnitType areaUnitType = TouchUnitType.Percent;

		[SerializeField]
		Rect activeArea = new Rect( 25.0f, 25.0f, 50.0f, 50.0f );


		[Header( "Analog Target" )]

		public AnalogTarget target = AnalogTarget.LeftStick;
		public float scale = 1.0f;


		Rect worldActiveArea;
		Vector3 lastPosition;
		Vector3 thisPosition;
		Touch currentTouch;
		bool dirty;


		public override void CreateControl()
		{
			ConfigureControl();
		}


		public override void DestroyControl()
		{
			if (currentTouch != null)
			{
				TouchEnded( currentTouch );
				currentTouch = null;
			}
		}


		public override void ConfigureControl()
		{
			worldActiveArea = TouchManager.ConvertToWorld( activeArea, areaUnitType );
		}


		public override void DrawGizmos()
		{
			Utility.DrawRectGizmo( worldActiveArea, Color.yellow );
		}


		void Update()
		{
			if (dirty)
			{
				ConfigureControl();
				dirty = false;
			}
		}


		public override void SubmitControlState( ulong updateTick, float deltaTime )
		{
			var delta = thisPosition - lastPosition;
			SubmitRawAnalogValue( target, delta * scale, updateTick, deltaTime );
			lastPosition = thisPosition;
		}


		public override void CommitControlState( ulong updateTick, float deltaTime )
		{
			CommitAnalog( target );
		}


		public override void TouchBegan( Touch touch )
		{
			if (currentTouch != null)
			{
				return;
			}

			var beganPosition = TouchManager.ScreenToWorldPoint( touch.position );
			if (worldActiveArea.Contains( beganPosition ))
			{
				thisPosition = beganPosition;
				lastPosition = beganPosition;
				currentTouch = touch;
			}
		}


		public override void TouchMoved( Touch touch )
		{
			if (currentTouch != touch)
			{
				return;
			}

			thisPosition = TouchManager.ScreenToWorldPoint( touch.position );
		}


		public override void TouchEnded( Touch touch )
		{
			if (currentTouch != touch)
			{
				return;
			}

			thisPosition = Vector3.zero;
			lastPosition = Vector3.zero;
			currentTouch = null;
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

