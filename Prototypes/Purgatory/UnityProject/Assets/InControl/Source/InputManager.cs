using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Reflection;
using UnityEngine;


namespace InControl
{
	public class InputManager
	{
		public static readonly VersionInfo Version = VersionInfo.InControlVersion();

		public static event Action OnSetup;
		public static event Action<ulong,float> OnUpdate;
		public static event Action OnReset;

		public static event Action<InputDevice> OnDeviceAttached;
		public static event Action<InputDevice> OnDeviceDetached;
		public static event Action<InputDevice> OnActiveDeviceChanged;

		internal static event Action<ulong,float> OnUpdateDevices;
		internal static event Action<ulong,float> OnCommitDevices;

		static List<InputDeviceManager> deviceManagers = new List<InputDeviceManager>();
		static Dictionary<Type,InputDeviceManager> deviceManagerTable = new Dictionary<Type, InputDeviceManager>();

		static InputDevice activeDevice = InputDevice.Null;
		static List<InputDevice> devices = new List<InputDevice>();

		/// <summary>
		/// A readonly collection of devices.
		/// Not every device in this list is guaranteed to be attached or even a controller.
		/// This collection should be treated as a pool from which devices may be selected.
		/// The collection is in no particular order and the order may change at any time.
		/// Do not treat this collection as a list of players.
		/// </summary>
		public static ReadOnlyCollection<InputDevice> Devices;

		/// <summary>
		/// Query whether a command button was pressed on any device during the last frame of input.
		/// </summary>
		public static bool MenuWasPressed { get; private set; }

		/// <summary>
		/// Gets or sets a value indicating whether the Y axis should be inverted for
		/// two-axis (directional) controls. When false (default), the Y axis will be positive up,
		/// the same as Unity.
		/// </summary>
		public static bool InvertYAxis { get; set; }

		internal static string Platform { get; private set; }

		static bool isSetup;
		static float initialTime;
		static float currentTime;
		static float lastUpdateTime;
		static ulong currentTick;
		static VersionInfo? unityVersion;


		/// <summary>
		/// DEPRECATED: Use the InControlManager component instead.
		/// </summary>
		/// @deprecated
		/// Calling this method directly is no longer supported. Use the InControlManager component to
		/// manage the lifecycle of the input manager instead.
		[Obsolete( "Calling InputManager.Setup() directly is no longer supported. Use the InControlManager component to manage the lifecycle of the input manager instead.", true )]
		public static void Setup()
		{
			SetupInternal();
		}

		internal static bool SetupInternal()
		{
			if (isSetup)
			{
				return false;
			}

			Platform = (SystemInfo.operatingSystem + " " + SystemInfo.deviceModel).ToUpper();

			initialTime = 0.0f;
			currentTime = 0.0f;
			lastUpdateTime = 0.0f;
			currentTick = 0;

			deviceManagers.Clear();
			deviceManagerTable.Clear();
			devices.Clear();
			Devices = new ReadOnlyCollection<InputDevice>( devices );
			activeDevice = InputDevice.Null;

			isSetup = true;

			#if UNITY_STANDALONE_WIN || UNITY_EDITOR
			if (EnableXInput)
			{
				XInputDeviceManager.Enable();
			}
			#endif

			if (OnSetup != null)
			{
				OnSetup.Invoke();
				OnSetup = null;
			}

			var addUnityInputDeviceManager = true;

			#if UNITY_ANDROID && INCONTROL_OUYA && !UNITY_EDITOR
			addUnityInputDeviceManager = false;
			#endif

			if (addUnityInputDeviceManager)
			{
				AddDeviceManager<UnityInputDeviceManager>();
			}

			return true;
		}


		/// <summary>
		/// DEPRECATED: Use the InControlManager component instead.
		/// </summary>
		/// @deprecated
		/// Calling this method directly is no longer supported. Use the InControlManager component to
		/// manage the lifecycle of the input manager instead.
		[Obsolete( "Calling InputManager.Reset() method directly is no longer supported. Use the InControlManager component to manage the lifecycle of the input manager instead.", true )]
		public static void Reset()
		{
			ResetInternal();
		}

		internal static void ResetInternal()
		{
			if (OnReset != null)
			{
				OnReset.Invoke();
			}

			OnSetup = null;
			OnUpdate = null;
			OnReset = null;
			OnActiveDeviceChanged = null;
			OnDeviceAttached = null;
			OnDeviceDetached = null;
			OnUpdateDevices = null;
			OnCommitDevices = null;

			DestroyDeviceManagers();

			devices.Clear();
			activeDevice = InputDevice.Null;

			isSetup = false;
		}


		/// <summary>
		/// DEPRECATED: Use the InControlManager component instead.
		/// </summary>
		/// @deprecated
		/// Calling this method directly is no longer supported. Use the InControlManager component to
		/// manage the lifecycle of the input manager instead.
		[Obsolete( "Calling InputManager.Update() directly is no longer supported. Use the InControlManager component to manage the lifecycle of the input manager instead.", true )]
		public static void Update()
		{
			UpdateInternal();
		}

		internal static void UpdateInternal()
		{
			AssertIsSetup();
			if (OnSetup != null)
			{
				OnSetup.Invoke();
				OnSetup = null;
			}

			currentTick++;
			UpdateCurrentTime();
			var deltaTime = currentTime - lastUpdateTime;

			UpdateDeviceManagers( deltaTime );

			MenuWasPressed = false;
			UpdateDevices( deltaTime );
			CommitDevices( deltaTime );

			UpdateActiveDevice();

			if (OnUpdate != null)
			{
				OnUpdate.Invoke( currentTick, deltaTime );
			}

			lastUpdateTime = currentTime;
		}


		/// <summary>
		/// Force the input manager to reset and setup.
		/// </summary>
		public static void Reload()
		{
			ResetInternal();
			SetupInternal();
		}


		static void AssertIsSetup()
		{
			if (!isSetup)
			{
				throw new Exception( "InputManager is not initialized. Call InputManager.Setup() first." );
			}
		}


		static void SetZeroTickOnAllControls()
		{
			int deviceCount = devices.Count;
			for (int i = 0; i < deviceCount; i++)
			{
				var inputControls = devices[i].Controls;
				var inputControlCount = inputControls.Length;
				for (int j = 0; j < inputControlCount; j++)
				{
					var inputControl = inputControls[j];
					if (inputControl != null)
					{
						inputControl.SetZeroTick();
					}
				}
			}
		}


		internal static void OnApplicationFocus( bool focusState )
		{
			if (!focusState)
			{
				SetZeroTickOnAllControls();
			}
		}


		internal static void OnApplicationPause( bool pauseState )
		{
		}


		internal static void OnApplicationQuit()
		{
			ResetInternal();
		}


		internal static void OnLevelWasLoaded()
		{
			SetZeroTickOnAllControls();
		}


		/// <summary>
		/// Adds a device manager.
		/// Only one instance of a given type can be added. An error will be raised if
		/// you try to add more than one.
		/// </summary>
		/// <param name="inputDeviceManager">The device manager to add.</param>
		public static void AddDeviceManager( InputDeviceManager deviceManager )
		{
			AssertIsSetup();

			var type = deviceManager.GetType();

			if (deviceManagerTable.ContainsKey( type ))
			{
				Logger.LogError( "A device manager of type '" + type.Name + "' already exists; cannot add another." );
				return;
			}

			deviceManagers.Add( deviceManager );
			deviceManagerTable.Add( type, deviceManager );

			deviceManager.Update( currentTick, currentTime - lastUpdateTime );
		}


		/// <summary>
		/// Adds a device manager by type.
		/// </summary>
		/// <typeparam name="T">A subclass of InputDeviceManager.</typeparam>
		public static void AddDeviceManager<T>() where T : InputDeviceManager, new()
		{
			AddDeviceManager( new T() );
		}


		/// <summary>
		/// Get a device manager from the input manager by type if it one is present.
		/// </summary>
		/// <typeparam name="T">A subclass of InputDeviceManager.</typeparam>
		public static T GetDeviceManager<T>() where T : InputDeviceManager
		{
			InputDeviceManager deviceManager;
			if (deviceManagerTable.TryGetValue( typeof(T), out deviceManager ))
			{
				return deviceManager as T;
			}

			return null;
		}


		/// <summary>
		/// Query whether a device manager is present by type.
		/// </summary>
		/// <typeparam name="T">A subclass of InputDeviceManager.</typeparam>
		public static bool HasDeviceManager<T>() where T : InputDeviceManager
		{
			return deviceManagerTable.ContainsKey( typeof(T) );
		}


		static void UpdateCurrentTime()
		{
			// Have to do this hack since Time.realtimeSinceStartup is not set until AFTER Awake().
			if (initialTime < float.Epsilon)
			{
				initialTime = Time.realtimeSinceStartup;
			}

			currentTime = Mathf.Max( 0.0f, Time.realtimeSinceStartup - initialTime );
		}


		static void UpdateDeviceManagers( float deltaTime )
		{
			int inputDeviceManagerCount = deviceManagers.Count;
			for (int i = 0; i < inputDeviceManagerCount; i++)
			{
				deviceManagers[i].Update( currentTick, deltaTime );
			}
		}


		static void DestroyDeviceManagers()
		{
			int inputDeviceManagerCount = deviceManagers.Count;
			for (int i = 0; i < inputDeviceManagerCount; i++)
			{
				deviceManagers[i].Destroy();
			}

			deviceManagers.Clear();
			deviceManagerTable.Clear();
		}


		static void UpdateDevices( float deltaTime )
		{
			int deviceCount = devices.Count;
			for (int i = 0; i < deviceCount; i++)
			{
				var device = devices[i];
				device.Update( currentTick, deltaTime );
			}

			if (OnUpdateDevices != null)
			{
				OnUpdateDevices.Invoke( currentTick, deltaTime );
			}
		}


		static void CommitDevices( float deltaTime )
		{
			int deviceCount = devices.Count;
			for (int i = 0; i < deviceCount; i++)
			{
				var device = devices[i];
				device.Commit( currentTick, deltaTime );

				if (device.MenuWasPressed)
				{
					MenuWasPressed = true;
				}
			}

			if (OnCommitDevices != null)
			{
				OnCommitDevices.Invoke( currentTick, deltaTime );
			}
		}


		static void UpdateActiveDevice()
		{
			var lastActiveDevice = ActiveDevice;

			int deviceCount = devices.Count;
			for (int i = 0; i < deviceCount; i++)
			{
				var inputDevice = devices[i];
				if (inputDevice.LastChangedAfter( ActiveDevice ))
				{
					ActiveDevice = inputDevice;
				}
			}

			if (lastActiveDevice != ActiveDevice)
			{
				if (OnActiveDeviceChanged != null)
				{
					OnActiveDeviceChanged( ActiveDevice );
				}
			}
		}


		/// <summary>
		/// Attach a device to the input manager.
		/// </summary>
		/// <param name="inputDevice">The input device to attach.</param>
		public static void AttachDevice( InputDevice inputDevice )
		{
			AssertIsSetup();

			if (!inputDevice.IsSupportedOnThisPlatform)
			{
				return;
			}

			devices.Add( inputDevice );
			devices.Sort( ( d1, d2 ) => d1.SortOrder.CompareTo( d2.SortOrder ) );

			inputDevice.IsAttached = true;

			if (OnDeviceAttached != null)
			{
				OnDeviceAttached( inputDevice );
			}
		}


		/// <summary>
		/// Detach a device from the input manager.
		/// </summary>
		/// <param name="inputDevice">The input device to attach.</param>
		public static void DetachDevice( InputDevice inputDevice )
		{
			AssertIsSetup();

			devices.Remove( inputDevice );

			inputDevice.IsAttached = false;

			if (ActiveDevice == inputDevice)
			{
				ActiveDevice = InputDevice.Null;
			}

			if (OnDeviceDetached != null)
			{
				OnDeviceDetached( inputDevice );
			}
		}


		/// <summary>
		/// Hides the devices with a given profile.
		/// This must be called before the input manager is initialized.
		/// </summary>
		/// <param name="type">Type.</param>
		public static void HideDevicesWithProfile( Type type )
		{
			#if NETFX_CORE
			if (type.GetTypeInfo().IsAssignableFrom( typeof( UnityInputDeviceProfile ).GetTypeInfo() ))
			#else
			if (type.IsSubclassOf( typeof(UnityInputDeviceProfile) ))
			#endif
			{
				UnityInputDeviceProfile.Hide( type );
			}
		}


		/// <summary>
		/// Detects whether any (keyboard) key is currently pressed.
		/// For more flexibility, see <see cref="KeyCombo.Detect()"/>
		/// </summary>
		public static bool AnyKeyIsPressed
		{
			get
			{
				return KeyCombo.Detect( true ).Count > 0;
			}
		}


		/// <summary>
		/// Gets the currently active device if present, otherwise returns a null device which does nothing.
		/// The currently active device is defined as the last device that provided input events. This is
		/// a good way to query for a device in single player applications.
		/// </summary>
		public static InputDevice ActiveDevice
		{
			get
			{
				return (activeDevice == null) ? InputDevice.Null : activeDevice;
			}

			private set
			{
				activeDevice = (value == null) ? InputDevice.Null : value;
			}
		}


		/// <summary>
		/// Enable XInput support.
		/// When enabled on initialization, the input manager will first check
		/// whether XInput is supported on this platform and if so, it will add
		/// an XInputDeviceManager.
		/// </summary>
		public static bool EnableXInput { get; internal set; }


		/// <summary>
		/// Set the XInput background thread polling rate.
		/// When set to zero (default) it will equal the projects fixed updated rate.
		/// </summary>
		public static uint XInputUpdateRate { get; internal set; }


		/// <summary>
		/// Set the XInput buffer size. (Experimental)
		/// Usually you want this to be zero (default). Setting it higher will introduce 
		/// latency, but may smooth out input if querying input on FixedUpdate, which 
		/// tends to cluster calls at the end of a frame.
		/// </summary>
		public static uint XInputBufferSize { get; internal set; }


		internal static VersionInfo UnityVersion
		{
			get
			{
				if (!unityVersion.HasValue)
				{
					unityVersion = VersionInfo.UnityVersion();
				}

				return unityVersion.Value;
			}
		}


		internal static ulong CurrentTick
		{
			get
			{
				return currentTick;
			}
		}
	}
}


