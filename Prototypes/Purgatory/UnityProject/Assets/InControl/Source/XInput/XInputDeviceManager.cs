#if UNITY_STANDALONE_WIN || UNITY_EDITOR
using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using XInputDotNetPure;


namespace InControl
{
	public class XInputDeviceManager : InputDeviceManager
	{
		bool[] deviceConnected = new bool[] { false, false, false, false };

		const int maxDevices = 4;
		RingBuffer<GamePadState>[] gamePadState = new RingBuffer<GamePadState>[ maxDevices ];
		Thread thread;
		int timeStep;
		int bufferSize;


		public XInputDeviceManager()
		{
			if (InputManager.XInputUpdateRate == 0)
			{
				timeStep = Mathf.FloorToInt( Time.fixedDeltaTime * 1000.0f );
			}
			else
			{
				timeStep = Mathf.FloorToInt( 1.0f / InputManager.XInputUpdateRate * 1000.0f );
			}

			bufferSize = (int) Math.Max( InputManager.XInputBufferSize, 1 );

			for (int deviceIndex = 0; deviceIndex < maxDevices; deviceIndex++)
			{
				gamePadState[deviceIndex] = new RingBuffer<GamePadState>( bufferSize );
			}

			StartWorker();

			for (int deviceIndex = 0; deviceIndex < maxDevices; deviceIndex++)
			{
				devices.Add( new XInputDevice( deviceIndex, this ) );
			}

			Update( 0, 0.0f );
		}


		void StartWorker()
		{
			if (thread == null)
			{
				thread = new Thread( SetState );
				thread.IsBackground = true;
				thread.Start();
			}
		}


		void StopWorker()
		{
			if (thread != null)
			{
				thread.Abort();
				thread.Join();
				thread = null;
			}
		}


		public void SetState()
		{
			while (true)
			{
				for (int deviceIndex = 0; deviceIndex < maxDevices; deviceIndex++)
				{
					gamePadState[deviceIndex].Enqueue( GamePad.GetState( (PlayerIndex) deviceIndex ) );
				}
				
				Thread.Sleep( timeStep );
			}
		}


		internal GamePadState GetState( int deviceIndex )
		{
			return gamePadState[deviceIndex].Dequeue();
		}


		public override void Update( ulong updateTick, float deltaTime )
		{
			for (int deviceIndex = 0; deviceIndex < maxDevices; deviceIndex++)
			{
				var device = devices[deviceIndex] as XInputDevice;

				// Unconnected devices won't be updated otherwise, so poll here.
				if (!device.IsConnected)
				{
					device.GetState();
				}

				if (device.IsConnected != deviceConnected[deviceIndex])
				{
					if (device.IsConnected)
					{
						InputManager.AttachDevice( device );
					}
					else
					{
						InputManager.DetachDevice( device );
					}

					deviceConnected[deviceIndex] = device.IsConnected;
				}
			}
		}


		public override void Destroy()
		{
			StopWorker();
		}


		public static bool CheckPlatformSupport( ICollection<string> errors )
		{
			if (Application.platform != RuntimePlatform.WindowsPlayer &&
			    Application.platform != RuntimePlatform.WindowsEditor)
			{
				return false;
			}

			try
			{
				GamePad.GetState( PlayerIndex.One );
			}
			catch (DllNotFoundException e)
			{
				if (errors != null)
				{
					errors.Add( e.Message + ".dll could not be found or is missing a dependency." );
				}
				return false;
			}
			
			return true;
		}


		public static void Enable()
		{
			var errors = new List<string>();
			if (XInputDeviceManager.CheckPlatformSupport( errors ))
			{
				InputManager.HideDevicesWithProfile( typeof(Xbox360WinProfile) );
				InputManager.HideDevicesWithProfile( typeof(XboxOneWinProfile) );
				InputManager.HideDevicesWithProfile( typeof(LogitechF310ModeXWinProfile) );
				InputManager.HideDevicesWithProfile( typeof(LogitechF510ModeXWinProfile) );
				InputManager.HideDevicesWithProfile( typeof(LogitechF710ModeXWinProfile) );
				InputManager.AddDeviceManager<XInputDeviceManager>();
			}
			else
			{
				foreach (var error in errors)
				{
					Logger.LogError( error );
				}
			}
		}
	}
}
#endif

