using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using UnityEngine;


namespace InControl
{
	public class UnityInputDeviceManager : InputDeviceManager
	{
		float deviceRefreshTimer = 0.0f;
		const float deviceRefreshInterval = 1.0f;
		List<InputDeviceProfile> systemDeviceProfiles = new List<InputDeviceProfile>();
		List<InputDeviceProfile> customDeviceProfiles = new List<InputDeviceProfile>();

		bool hasJoystickHash;
		int joystickHash;


		public UnityInputDeviceManager()
		{
			AddSystemDeviceProfiles();
			// LoadDeviceProfiles();
			AttachDevices();
		}


		public override void Update( ulong updateTick, float deltaTime )
		{
			deviceRefreshTimer += deltaTime;
			if (!hasJoystickHash || deviceRefreshTimer >= deviceRefreshInterval)
			{
				deviceRefreshTimer = 0.0f;

				if (joystickHash != JoystickHash)
				{
					Logger.LogInfo( "Change in attached Unity joysticks detected; refreshing device list." );
					DetachDevices();
					AttachDevices();
				}
			}
		}


		void AttachDevices()
		{
			AttachKeyboardDevices();
			AttachJoystickDevices();
			joystickHash = JoystickHash;
			hasJoystickHash = true;
		}


		void DetachDevices()
		{
			var deviceCount = devices.Count;
			for (int i = 0; i < deviceCount; i++)
			{
				InputManager.DetachDevice( devices[i] );
			}
			devices.Clear();
		}


		public void ReloadDevices()
		{
			DetachDevices();
			AttachDevices();
		}


		void AttachDevice( UnityInputDevice device )
		{
			devices.Add( device );
			InputManager.AttachDevice( device );
		}


		void AttachKeyboardDevices()
		{
			int deviceProfileCount = systemDeviceProfiles.Count;
			for (int i = 0; i < deviceProfileCount; i++)
			{
				var deviceProfile = systemDeviceProfiles[i];
				if (deviceProfile.IsNotJoystick && deviceProfile.IsSupportedOnThisPlatform)
				{
					AttachDevice( new UnityInputDevice( deviceProfile ) );
				}
			}
		}


		void AttachJoystickDevices()
		{
			try
			{
				var joystickNames = Input.GetJoystickNames();
				var joystickNameCount = joystickNames.Length;
				for (int i = 0; i < joystickNameCount; i++)
				{
					DetectJoystickDevice( i + 1, joystickNames[i] );
				}
			}
			catch (Exception e)
			{
				Logger.LogError( e.Message );
				Logger.LogError( e.StackTrace );
			}
		}


		void DetectJoystickDevice( int unityJoystickId, string unityJoystickName )
		{
			#if UNITY_PS4
			if (unityJoystickName == "Empty")
			{
				// On PS4 console, disconnected controllers may have this name.
				return;
			}
			#endif

			if (unityJoystickName == "WIRED CONTROLLER" ||
			    unityJoystickName == " WIRED CONTROLLER")
			{
				// Ignore Steam controller for now.
				return;
			}

			if (unityJoystickName.IndexOf( "webcam", StringComparison.OrdinalIgnoreCase ) != -1)
			{
				// Unity thinks some webcams are joysticks. >_<
				return;
			}

			// PS4 controller only works properly as of Unity 4.5
			if (InputManager.UnityVersion < new VersionInfo( 4, 5, 0, 0 ))
			{
				if (Application.platform == RuntimePlatform.OSXEditor ||
				    Application.platform == RuntimePlatform.OSXPlayer ||
				    Application.platform == RuntimePlatform.OSXWebPlayer)
				{
					if (unityJoystickName == "Unknown Wireless Controller")
					{
						// Ignore PS4 controller in Bluetooth mode on Mac since it connects but does nothing.
						return;
					}
				}
			}

			// As of Unity 4.6.3p1, empty strings on windows represent disconnected devices.
			if (InputManager.UnityVersion >= new VersionInfo( 4, 6, 3, 0 ))
			{
				if (Application.platform == RuntimePlatform.WindowsEditor ||
				    Application.platform == RuntimePlatform.WindowsPlayer ||
				    Application.platform == RuntimePlatform.WindowsWebPlayer)
				{
					if (String.IsNullOrEmpty( unityJoystickName ))
					{
						return;
					}
				}
			}

			InputDeviceProfile deviceProfile = null;

			if (deviceProfile == null)
			{
				deviceProfile = customDeviceProfiles.Find( config => config.HasJoystickName( unityJoystickName ) );
			}

			if (deviceProfile == null)
			{
				deviceProfile = systemDeviceProfiles.Find( config => config.HasJoystickName( unityJoystickName ) );
			}

			if (deviceProfile == null)
			{
				deviceProfile = customDeviceProfiles.Find( config => config.HasLastResortRegex( unityJoystickName ) );
			}

			if (deviceProfile == null)
			{
				deviceProfile = systemDeviceProfiles.Find( config => config.HasLastResortRegex( unityJoystickName ) );
			}

			if (deviceProfile == null)
			{
				Logger.LogWarning( "Device " + unityJoystickId + " with name \"" + unityJoystickName + "\" does not match any supported profiles and will be considered an unknown controller." );
				var unknownDeviceProfile = new UnknownUnityDeviceProfile( unityJoystickName );
				var joystickDevice = new UnknownUnityInputDevice( unknownDeviceProfile, unityJoystickId );
				AttachDevice( joystickDevice );
				return;
			}

			if (!deviceProfile.IsHidden)
			{
				var joystickDevice = new UnityInputDevice( deviceProfile, unityJoystickId );
				AttachDevice( joystickDevice );
				Logger.LogInfo( "Device " + unityJoystickId + " matched profile " + deviceProfile.GetType().Name + " (" + deviceProfile.Name + ")" );
			}
			else
			{
				Logger.LogInfo( "Device " + unityJoystickId + " matching profile " + deviceProfile.GetType().Name + " (" + deviceProfile.Name + ")" + " is hidden and will not be attached." );
			}
		}


		int JoystickHash
		{
			get
			{
				var joystickNames = Input.GetJoystickNames();
				var joystickNamesCount = joystickNames.Length;
				int hash = 17 * 31 + joystickNamesCount;
				for (int i = 0; i < joystickNamesCount; i++)
				{
					hash = hash * 31 + joystickNames[i].GetHashCode();
				}
				return hash;
			}
		}


		void AddSystemDeviceProfile( UnityInputDeviceProfile deviceProfile )
		{
			if (deviceProfile.IsSupportedOnThisPlatform)
			{
				systemDeviceProfiles.Add( deviceProfile );
			}
		}


		void AddSystemDeviceProfiles()
		{
			foreach (var typeName in UnityInputDeviceProfileList.Profiles)
			{
				var deviceProfile = (UnityInputDeviceProfile) Activator.CreateInstance( Type.GetType( typeName ) );
				AddSystemDeviceProfile( deviceProfile );
			}
		}

		/*
		public void AddDeviceProfile( UnityInputDeviceProfile deviceProfile )
		{
			if (deviceProfile.IsSupportedOnThisPlatform)
			{
				customDeviceProfiles.Add( deviceProfile );
			}
		}


		public void LoadDeviceProfiles()
		{
			LoadDeviceProfilesFromPath( CustomProfileFolder );
		}


		public void LoadDeviceProfile( string data )
		{
			var deviceProfile = UnityInputDeviceProfile.Load( data );
			AddDeviceProfile( deviceProfile );
		}


		public void LoadDeviceProfileFromFile( string filePath )
		{
			var deviceProfile = UnityInputDeviceProfile.LoadFromFile( filePath );
			AddDeviceProfile( deviceProfile );
		}


		public void LoadDeviceProfilesFromPath( string rootPath )
		{
			if (Directory.Exists( rootPath ))
			{
				var filePaths = Directory.GetFiles( rootPath, "*.json", SearchOption.AllDirectories );
				foreach (var filePath in filePaths)
				{
					LoadDeviceProfileFromFile( filePath );
				}
			}
		}


		internal static void DumpSystemDeviceProfiles()
		{
			var filePath = CustomProfileFolder;
			Directory.CreateDirectory( filePath );

			foreach (var typeName in UnityInputDeviceProfileList.Profiles)
			{
				var deviceProfile = (UnityInputDeviceProfile) Activator.CreateInstance( Type.GetType( typeName ) );
				var fileName = deviceProfile.GetType().Name + ".json";
				deviceProfile.SaveToFile( filePath + "/" + fileName );
			}
		}


		static string CustomProfileFolder
		{
			get
			{
				return Environment.GetFolderPath( Environment.SpecialFolder.ApplicationData ) + "/InControl/Profiles";
			}
		}
		/**/
	}
}

