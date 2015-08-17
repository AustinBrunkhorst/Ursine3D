using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;


namespace InControl
{
	public class InControlManager : SingletonMonoBehavior<InControlManager>
	{
		public bool logDebugInfo = false;
		public bool invertYAxis = false;
		public bool useFixedUpdate = false;
		public bool dontDestroyOnLoad = false;

		public bool enableXInput = false;
		public int xInputUpdateRate = 0;
		public int xInputBufferSize = 0;

		public List<string> customProfiles = new List<string>();


		void OnEnable()
		{
			if (!SetupSingleton())
			{
				return;
			}

			InputManager.InvertYAxis = invertYAxis;
			InputManager.EnableXInput = enableXInput;
			InputManager.XInputUpdateRate = (uint) Mathf.Max( xInputUpdateRate, 0 );
			InputManager.XInputBufferSize = (uint) Mathf.Max( xInputBufferSize, 0 );

			if (InputManager.SetupInternal())
			{
				if (logDebugInfo)
				{
					Debug.Log( "InControl (version " + InputManager.Version + ")" );
					Logger.OnLogMessage += LogMessage;
				}

				foreach (var className in customProfiles)
				{
					var classType = Type.GetType( className );
					if (classType == null)
					{
						Debug.LogError( "Cannot find class for custom profile: " + className );
					}
					else
					{
						var customProfileInstance = Activator.CreateInstance( classType ) as InputDeviceProfile;
						if (customProfileInstance != null)
						{
							InputManager.AttachDevice( new UnityInputDevice( customProfileInstance ) );
						}
					}
				}
			}

			if (dontDestroyOnLoad)
			{
				DontDestroyOnLoad( this );
			}
		}


		void OnDisable()
		{
			if (InControlManager.Instance == this)
			{
				InputManager.ResetInternal();
			}
		}


		#if UNITY_ANDROID && INCONTROL_OUYA && !UNITY_EDITOR
		void Start()
		{
			StartCoroutine( CheckForOuyaEverywhereSupport() );
		}


		IEnumerator CheckForOuyaEverywhereSupport()
		{
			Debug.Log( "[InControl] Checking for OUYA Everywhere support..." );

			while (!OuyaSDK.isIAPInitComplete())
			{
				yield return null;
			}

			Debug.Log( "[InControl] OUYA SDK IAP initialization has completed." );

			OuyaEverywhereDeviceManager.Enable();
		}
		#endif


		void Update()
		{
			if (!useFixedUpdate || Utility.IsZero( Time.timeScale ))
			{
				InputManager.UpdateInternal();
			}
		}


		void FixedUpdate()
		{
			if (useFixedUpdate)
			{
				InputManager.UpdateInternal();
			}
		}


		void OnApplicationFocus( bool focusState )
		{
			InputManager.OnApplicationFocus( focusState );
		}


		void OnApplicationPause( bool pauseState )
		{
			InputManager.OnApplicationPause( pauseState );
		}


		void OnApplicationQuit()
		{
			InputManager.OnApplicationQuit();
		}


		void OnLevelWasLoaded( int level )
		{
			InputManager.OnLevelWasLoaded();
		}


		void LogMessage( LogMessage logMessage )
		{
			switch (logMessage.type)
			{
				case LogMessageType.Info:
					Debug.Log( logMessage.text );
					break;
				case LogMessageType.Warning:
					Debug.LogWarning( logMessage.text );
					break;
				case LogMessageType.Error:
					Debug.LogError( logMessage.text );
					break;
			}
		}
	}
}

