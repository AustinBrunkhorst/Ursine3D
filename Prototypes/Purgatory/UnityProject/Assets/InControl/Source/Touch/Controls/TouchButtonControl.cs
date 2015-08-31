using System;
using UnityEngine;


namespace InControl
{
	public class TouchButtonControl : TouchControl
	{
		[Header( "Position" )]

		[SerializeField]
		TouchControlAnchor anchor = TouchControlAnchor.BottomRight;

		[SerializeField]
		TouchUnitType offsetUnitType = TouchUnitType.Percent;

		[SerializeField]
		Vector2 offset = new Vector2( -10.0f, 10.0f );

		[SerializeField]
		bool lockAspectRatio = true;


		[Header( "Options" )]

		public ButtonTarget target = ButtonTarget.Action1;
		public bool allowSlideToggle = true;
		public bool toggleOnLeave = false;


		[Header( "Sprites" )]

		public TouchSprite button = new TouchSprite( 15.0f );


		bool buttonState;
		Touch currentTouch;
		bool dirty;


		public override void CreateControl()
		{
			button.Create( "Button", transform, 1000 );
		}


		public override void DestroyControl()
		{
			button.Delete();

			if (currentTouch != null)
			{
				TouchEnded( currentTouch );
				currentTouch = null;
			}
		}


		public override void ConfigureControl()
		{
			transform.position = OffsetToWorldPosition( anchor, offset, offsetUnitType, lockAspectRatio );
			button.Update( true );
		}


		public override void DrawGizmos()
		{
			button.DrawGizmos( ButtonPosition, Color.yellow );
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
				button.Update();
			}
		}


		public override void SubmitControlState( ulong updateTick, float deltaTime )
		{
			if (currentTouch == null && allowSlideToggle)
			{
				ButtonState = false;
				var touchCount = TouchManager.TouchCount;
				for (int i = 0; i < touchCount; i++)
				{
					ButtonState = ButtonState || button.Contains( TouchManager.GetTouch( i ) );
				}
			}

			SubmitButtonState( target, ButtonState, updateTick, deltaTime );
		}


		public override void CommitControlState( ulong updateTick, float deltaTime )
		{
			CommitButton( target );
		}


		public override void TouchBegan( Touch touch )
		{
			if (currentTouch != null)
			{
				return;
			}

			if (button.Contains( touch ))
			{
				ButtonState = true;
				currentTouch = touch;
			}
		}


		public override void TouchMoved( Touch touch )
		{
			if (currentTouch != touch)
			{
				return;
			}

			if (toggleOnLeave && !button.Contains( touch ))
			{
				ButtonState = false;
				currentTouch = null;
			}
		}


		public override void TouchEnded( Touch touch )
		{
			if (currentTouch != touch)
			{
				return;
			}

			ButtonState = false;
			currentTouch = null;
		}


		bool ButtonState
		{
			get
			{
				return buttonState;
			}

			set
			{
				if (buttonState != value)
				{
					buttonState = value;
					button.State = value;
				}
			}
		}


		public Vector3 ButtonPosition
		{
			get
			{
				return button.Ready ? button.Position : transform.position;
			}

			set
			{
				if (button.Ready)
				{
					button.Position = value;
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
	}
}

