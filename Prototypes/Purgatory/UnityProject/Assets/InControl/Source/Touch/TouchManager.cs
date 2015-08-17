using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using UnityEngine;
using System.Collections;


namespace InControl
{
	[ExecuteInEditMode]
	public class TouchManager : SingletonMonoBehavior<TouchManager>
	{
		const int MaxTouches = 16;

		public enum GizmoShowOption
		{
			Never,
			WhenSelected,
			UnlessPlaying,
			Always
		}

		[Space( 10 )]

		public Camera touchCamera;
		public GizmoShowOption controlsShowGizmos = GizmoShowOption.Always;

		[HideInInspector]
		public bool enableControlsOnTouch = false;

		[SerializeField, HideInInspector]
		bool _controlsEnabled = true;

		// Defaults to UI layer.
		[HideInInspector]
		public int controlsLayer = 5;

		public static event Action OnSetup;

		InputDevice device;
		Vector3 viewSize;
		Vector2 screenSize;
		Vector2 halfScreenSize;
		float percentToWorld;
		float halfPercentToWorld;
		float pixelToWorld;
		float halfPixelToWorld;
		TouchControl[] touchControls;
		Touch[] cachedTouches;
		List<Touch> activeTouches;
		ReadOnlyCollection<Touch> readOnlyActiveTouches;
		Vector2 lastMousePosition;
		bool isReady;

		#pragma warning disable 414
		Touch mouseTouch;
		#pragma warning restore 414


		protected TouchManager()
		{
		}


		void OnEnable()
		{
			if (!SetupSingleton())
			{
				return;
			}

			touchControls = GetComponentsInChildren<TouchControl>( true );

			if (Application.isPlaying)
			{
				InputManager.OnSetup += Setup;
				InputManager.OnUpdateDevices += UpdateDevice;
				InputManager.OnCommitDevices += CommitDevice;
			}
		}


		void OnDisable()
		{
			if (Application.isPlaying)
			{
				InputManager.OnSetup -= Setup;
				InputManager.OnUpdateDevices -= UpdateDevice;
				InputManager.OnCommitDevices -= CommitDevice;
			}

			Reset();
		}


		void Setup()
		{
			UpdateScreenSize( new Vector2( Screen.width, Screen.height ) );

			CreateDevice();
			CreateTouches();

			if (OnSetup != null)
			{
				OnSetup.Invoke();
				OnSetup = null;
			}
		}


		void Reset()
		{
			device = null;
			mouseTouch = null;
			cachedTouches = null;
			activeTouches = null;
			readOnlyActiveTouches = null;
			touchControls = null;
			OnSetup = null;
		}


		void Start()
		{
			// This little hack is necessary because right after Unity starts up,
			// cameras don't seem to have a correct projection matrix until after
			// their first update or around that time. So we basically need to
			// wait until the end of the first frame before everything is quite ready.
			StartCoroutine( Ready() );
		}


		IEnumerator Ready()
		{
			yield return new WaitForEndOfFrame();
			isReady = true;
			UpdateScreenSize( new Vector2( Screen.width, Screen.height ) );
			yield return null;
		}


		void Update()
		{
			if (!isReady)
			{
				return;
			}

			var currentScreenSize = new Vector2( Screen.width, Screen.height );
			if (screenSize != currentScreenSize)
			{
				UpdateScreenSize( currentScreenSize );
			}

			if (OnSetup != null)
			{
				OnSetup.Invoke();
				OnSetup = null;
			}
		}


		void CreateDevice()
		{
			device = new InputDevice( "TouchDevice" );

			device.AddControl( InputControlType.LeftStickLeft, "LeftStickLeft" );
			device.AddControl( InputControlType.LeftStickRight, "LeftStickRight" );
			device.AddControl( InputControlType.LeftStickUp, "LeftStickUp" );
			device.AddControl( InputControlType.LeftStickDown, "LeftStickDown" );

			device.AddControl( InputControlType.RightStickLeft, "RightStickLeft" );
			device.AddControl( InputControlType.RightStickRight, "RightStickRight" );
			device.AddControl( InputControlType.RightStickUp, "RightStickUp" );
			device.AddControl( InputControlType.RightStickDown, "RightStickDown" );

			device.AddControl( InputControlType.LeftTrigger, "LeftTrigger" );
			device.AddControl( InputControlType.RightTrigger, "RightTrigger" );

			device.AddControl( InputControlType.DPadUp, "DPadUp" );
			device.AddControl( InputControlType.DPadDown, "DPadDown" );
			device.AddControl( InputControlType.DPadLeft, "DPadLeft" );
			device.AddControl( InputControlType.DPadRight, "DPadRight" );

			device.AddControl( InputControlType.Action1, "Action1" );
			device.AddControl( InputControlType.Action2, "Action2" );
			device.AddControl( InputControlType.Action3, "Action3" );
			device.AddControl( InputControlType.Action4, "Action4" );

			device.AddControl( InputControlType.LeftBumper, "LeftBumper" );
			device.AddControl( InputControlType.RightBumper, "RightBumper" );

			device.AddControl( InputControlType.Menu, "Menu" );

			for (var control = InputControlType.Button0; control <= InputControlType.Button19; control++)
			{
				device.AddControl( control, control.ToString() );
			}

			InputManager.AttachDevice( device );
		}


		void UpdateDevice( ulong updateTick, float deltaTime )
		{
			UpdateTouches( updateTick, deltaTime );
			SubmitControlStates( updateTick, deltaTime );
		}


		void CommitDevice( ulong updateTick, float deltaTime )
		{
			CommitControlStates( updateTick, deltaTime );
		}


		void SubmitControlStates( ulong updateTick, float deltaTime )
		{
			var touchControlCount = touchControls.Length;
			for (int i = 0; i < touchControlCount; i++)
			{
				var touchControl = touchControls[i];
				if (touchControl.enabled && touchControl.gameObject.activeInHierarchy)
				{
					touchControl.SubmitControlState( updateTick, deltaTime );
				}
			}
		}


		void CommitControlStates( ulong updateTick, float deltaTime )
		{
			var touchControlCount = touchControls.Length;
			for (int i = 0; i < touchControlCount; i++)
			{
				var touchControl = touchControls[i];
				if (touchControl.enabled && touchControl.gameObject.activeInHierarchy)
				{
					touchControl.CommitControlState( updateTick, deltaTime );
				}
			}
		}


		void UpdateScreenSize( Vector2 currentScreenSize )
		{
			screenSize = currentScreenSize;
			halfScreenSize = screenSize / 2.0f;

			viewSize = ConvertViewToWorldPoint( Vector2.one ) * 0.02f;
			percentToWorld = Mathf.Min( viewSize.x, viewSize.y );
			halfPercentToWorld = percentToWorld / 2.0f;

			if (touchCamera != null)
			{
				halfPixelToWorld = touchCamera.orthographicSize / screenSize.y;
				pixelToWorld = halfPixelToWorld * 2.0f;
			}

			if (touchControls != null)
			{
				var touchControlCount = touchControls.Length;
				for (int i = 0; i < touchControlCount; i++)
				{
					touchControls[i].ConfigureControl();
				}
			}
		}


		void CreateTouches()
		{
			cachedTouches = new Touch[MaxTouches];
			for (int i = 0; i < MaxTouches; i++)
			{
				cachedTouches[i] = new Touch( i );
			}
			mouseTouch = cachedTouches[MaxTouches - 1];
			activeTouches = new List<Touch>( MaxTouches );
			readOnlyActiveTouches = new ReadOnlyCollection<Touch>( activeTouches );
		}


		void UpdateTouches( ulong updateTick, float deltaTime )
		{
			activeTouches.Clear();

			#if UNITY_EDITOR || UNITY_STANDALONE || UNITY_WEBPLAYER || UNITY_WEBGL
			if (mouseTouch.SetWithMouseData( updateTick, deltaTime ))
			{
				activeTouches.Add( mouseTouch );
			}
			#endif

			for (int i = 0; i < Input.touchCount; i++)
			{
				var unityTouch = Input.GetTouch( i );
				var cacheTouch = cachedTouches[unityTouch.fingerId];
				cacheTouch.SetWithTouchData( unityTouch, updateTick, deltaTime );
				activeTouches.Add( cacheTouch );
			}

			// Find any touches that Unity may have "forgotten" to end properly.
			for (int i = 0; i < MaxTouches; i++)
			{
				var touch = cachedTouches[i];
				if (touch.phase != TouchPhase.Ended && touch.updateTick != updateTick)
				{
					touch.phase = TouchPhase.Ended;
					activeTouches.Add( touch );
				}
			}

			InvokeTouchEvents();
		}


		void SendTouchBegan( Touch touch )
		{
			var touchControlCount = touchControls.Length;
			for (int i = 0; i < touchControlCount; i++)
			{
				var touchControl = touchControls[i];
				if (touchControl.enabled && touchControl.gameObject.activeInHierarchy)
				{
					touchControl.TouchBegan( touch );
				}
			}
		}


		void SendTouchMoved( Touch touch )
		{
			var touchControlCount = touchControls.Length;
			for (int i = 0; i < touchControlCount; i++)
			{
				var touchControl = touchControls[i];
				if (touchControl.enabled && touchControl.gameObject.activeInHierarchy)
				{
					touchControl.TouchMoved( touch );
				}
			}
		}


		void SendTouchEnded( Touch touch )
		{
			var touchControlCount = touchControls.Length;
			for (int i = 0; i < touchControlCount; i++)
			{
				var touchControl = touchControls[i];
				if (touchControl.enabled && touchControl.gameObject.activeInHierarchy)
				{
					touchControl.TouchEnded( touch );
				}
			}
		}


		void InvokeTouchEvents()
		{
			var touchCount = activeTouches.Count;

			if (enableControlsOnTouch)
			{
				if (touchCount > 0 && !controlsEnabled)
				{
					Device.RequestActivation();
					controlsEnabled = true;
				}
			}

			for (int i = 0; i < touchCount; i++)
			{
				var touch = activeTouches[i];
				switch (touch.phase)
				{
					case TouchPhase.Began:
						SendTouchBegan( touch );
						break;

					case TouchPhase.Moved:
						SendTouchMoved( touch );
						break;

					case TouchPhase.Ended:
						SendTouchEnded( touch );
						break;

					case TouchPhase.Canceled:
						SendTouchEnded( touch );
						break;
				}
			}
		}


		bool TouchCameraIsValid()
		{
			if (touchCamera == null)
			{
				return false;
			}

			if (Utility.IsZero( touchCamera.orthographicSize ))
			{
				return false;
			}

			if (Utility.IsZero( touchCamera.rect.width ) && Utility.IsZero( touchCamera.rect.height ))
			{
				return false;
			}

			if (Utility.IsZero( touchCamera.pixelRect.width ) && Utility.IsZero( touchCamera.pixelRect.height ))
			{
				return false;
			}

			return true;
		}


		Vector3 ConvertScreenToWorldPoint( Vector2 point )
		{
			if (TouchCameraIsValid())
			{
				return touchCamera.ScreenToWorldPoint( new Vector3( point.x, point.y, -touchCamera.transform.position.z ) );
			}
			else
			{
				return Vector3.zero;
			}
		}


		Vector3 ConvertViewToWorldPoint( Vector2 point )
		{
			if (TouchCameraIsValid())
			{
				return touchCamera.ViewportToWorldPoint( new Vector3( point.x, point.y, -touchCamera.transform.position.z ) );
			}
			else
			{
				return Vector3.zero;
			}
		}


		public bool controlsEnabled
		{
			get
			{
				return _controlsEnabled;
			}

			set
			{
				if (_controlsEnabled != value)
				{
					var touchControlCount = touchControls.Length;
					for (int i = 0; i < touchControlCount; i++)
					{
						touchControls[i].enabled = value;
					}

					_controlsEnabled = value;
				}
			}
		}


		#region Static interface.

		public static ReadOnlyCollection<Touch> Touches
		{
			get
			{
				return Instance.readOnlyActiveTouches;
			}
		}


		public static int TouchCount
		{
			get
			{
				return Instance.activeTouches.Count;
			}
		}


		public static Touch GetTouch( int touchIndex )
		{
			return Instance.activeTouches[touchIndex];
		}


		public static Touch GetTouchByFingerId( int fingerId )
		{
			return Instance.cachedTouches[fingerId];
		}


		public static Vector3 ScreenToWorldPoint( Vector2 point )
		{
			return Instance.ConvertScreenToWorldPoint( point );
		}


		public static Vector3 ViewToWorldPoint( Vector2 point )
		{
			return Instance.ConvertViewToWorldPoint( point );
		}


		public static float ConvertToWorld( float value, TouchUnitType unitType )
		{
			return value * (unitType == TouchUnitType.Pixels ? PixelToWorld : PercentToWorld);
		}


		public static Rect PercentToWorldRect( Rect rect )
		{
			return new Rect(
				(rect.xMin - 50.0f) * ViewSize.x,
				(rect.yMin - 50.0f) * ViewSize.y,
				rect.width * ViewSize.x,
				rect.height * ViewSize.y
			);
		}


		public static Rect PixelToWorldRect( Rect rect )
		{
			return new Rect(
				Mathf.Round( rect.xMin - HalfScreenSize.x ) * PixelToWorld,
				Mathf.Round( rect.yMin - HalfScreenSize.y ) * PixelToWorld,
				Mathf.Round( rect.width ) * PixelToWorld,
				Mathf.Round( rect.height ) * PixelToWorld
			);
		}


		public static Rect ConvertToWorld( Rect rect, TouchUnitType unitType )
		{
			return unitType == TouchUnitType.Pixels ? PixelToWorldRect( rect ) : PercentToWorldRect( rect );
		}


		public static Camera Camera
		{
			get
			{
				return Instance.touchCamera;
			}
		}


		public static InputDevice Device
		{
			get
			{
				return Instance.device;
			}
		}


		public static Vector3 ViewSize
		{
			get
			{
				return Instance.viewSize;
			}
		}


		public static float PercentToWorld
		{
			get
			{
				return Instance.percentToWorld;
			}
		}


		public static float HalfPercentToWorld
		{
			get
			{
				return Instance.halfPercentToWorld;
			}
		}


		public static float PixelToWorld
		{
			get
			{
				return Instance.pixelToWorld;
			}
		}


		public static float HalfPixelToWorld
		{
			get
			{
				return Instance.halfPixelToWorld;
			}
		}


		public static Vector2 ScreenSize
		{
			get
			{
				return Instance.screenSize;
			}
		}


		public static Vector2 HalfScreenSize
		{
			get
			{
				return Instance.halfScreenSize;
			}
		}


		public static GizmoShowOption ControlsShowGizmos
		{
			get
			{
				return Instance.controlsShowGizmos;
			}
		}


		public static bool ControlsEnabled
		{
			get
			{
				return Instance.controlsEnabled;
			}

			set
			{
				Instance.controlsEnabled = value;
			}
		}

		#endregion


		public static implicit operator bool( TouchManager instance )
		{
			return instance != null;
		}
	}
}

