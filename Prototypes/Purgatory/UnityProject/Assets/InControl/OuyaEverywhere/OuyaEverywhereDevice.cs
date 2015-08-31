using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

#if UNITY_ANDROID && INCONTROL_OUYA && !UNITY_EDITOR
using tv.ouya.console.api;
#endif


namespace InControl
{
	public class OuyaEverywhereDevice : InputDevice
	{
		const float LowerDeadZone = 0.2f;
		const float UpperDeadZone = 0.9f;

		public int DeviceIndex { get; private set; }


		public OuyaEverywhereDevice( int deviceIndex )
			: base( "OUYA Controller" )
		{
			DeviceIndex = deviceIndex;
			SortOrder = deviceIndex;

			Meta = "OUYA Everywhere Device #" + deviceIndex;

			AddControl( InputControlType.LeftStickLeft, "Left Stick Left" );
			AddControl( InputControlType.LeftStickRight, "Left Stick Right" );
			AddControl( InputControlType.LeftStickUp, "Left Stick Up" );
			AddControl( InputControlType.LeftStickDown, "Left Stick Down" );

			AddControl( InputControlType.RightStickLeft, "Right Stick Left" );
			AddControl( InputControlType.RightStickRight, "Right Stick Right" );
			AddControl( InputControlType.RightStickUp, "Right Stick Up" );
			AddControl( InputControlType.RightStickDown, "Right Stick Down" );

			AddControl( InputControlType.LeftTrigger, "Left Trigger" );
			AddControl( InputControlType.RightTrigger, "Right Trigger" );

			AddControl( InputControlType.DPadUp, "DPad Up" );
			AddControl( InputControlType.DPadDown, "DPad Down" );
			AddControl( InputControlType.DPadLeft, "DPad Left" );
			AddControl( InputControlType.DPadRight, "DPad Right" );

			AddControl( InputControlType.Action1, "O" );
			AddControl( InputControlType.Action2, "A" );
			AddControl( InputControlType.Action3, "Y" );
			AddControl( InputControlType.Action4, "U" );

			AddControl( InputControlType.LeftBumper, "Left Bumper" );
			AddControl( InputControlType.RightBumper, "Right Bumper" );

			AddControl( InputControlType.LeftStickButton, "Left Stick Button" );
			AddControl( InputControlType.RightStickButton, "Right Stick Button" );

			AddControl( InputControlType.Menu, "Menu" );
		}


		public void BeforeAttach()
		{
			#if UNITY_ANDROID && INCONTROL_OUYA && !UNITY_EDITOR
			Name = OuyaController.getControllerByPlayer( DeviceIndex ).getDeviceName();
			#endif
		}


		public override void Update( ulong updateTick, float deltaTime )
		{
			#if UNITY_ANDROID && INCONTROL_OUYA && !UNITY_EDITOR
			var lsv = Utility.ApplyCircularDeadZone( 
				          OuyaSDK.OuyaInput.GetAxisRaw( DeviceIndex, OuyaController.AXIS_LS_X ), 
				          -OuyaSDK.OuyaInput.GetAxisRaw( DeviceIndex, OuyaController.AXIS_LS_Y ), 
				          LowerDeadZone, 
				          UpperDeadZone
			          );
			UpdateLeftStickWithValue( lsv, updateTick, deltaTime );

			var rsv = Utility.ApplyCircularDeadZone( 
				          OuyaSDK.OuyaInput.GetAxisRaw( DeviceIndex, OuyaController.AXIS_RS_X ), 
				          -OuyaSDK.OuyaInput.GetAxisRaw( DeviceIndex, OuyaController.AXIS_RS_Y ), 
				          LowerDeadZone, 
				          UpperDeadZone
			          );
			UpdateRightStickWithValue( rsv, updateTick, deltaTime );

			var lt = Utility.ApplyDeadZone(
				         OuyaSDK.OuyaInput.GetAxisRaw( DeviceIndex, OuyaController.AXIS_L2 ),
				         LowerDeadZone,
				         UpperDeadZone 
			         );
			UpdateWithValue( InputControlType.LeftTrigger, lt, updateTick, deltaTime );

			var rt = Utility.ApplyDeadZone(
				         OuyaSDK.OuyaInput.GetAxisRaw( DeviceIndex, OuyaController.AXIS_R2 ),
				         LowerDeadZone,
				         UpperDeadZone 
			         );
			UpdateWithValue( InputControlType.RightTrigger, rt, updateTick, deltaTime );

			UpdateWithState( InputControlType.DPadUp, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_DPAD_UP ), updateTick, deltaTime );
			UpdateWithState( InputControlType.DPadDown, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_DPAD_DOWN ), updateTick, deltaTime );
			UpdateWithState( InputControlType.DPadLeft, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_DPAD_LEFT ), updateTick, deltaTime );
			UpdateWithState( InputControlType.DPadRight, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_DPAD_RIGHT ), updateTick, deltaTime );

			UpdateWithState( InputControlType.Action1, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_O ), updateTick, deltaTime );
			UpdateWithState( InputControlType.Action2, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_A ), updateTick, deltaTime );
			UpdateWithState( InputControlType.Action3, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_U ), updateTick, deltaTime );
			UpdateWithState( InputControlType.Action4, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_Y ), updateTick, deltaTime );

			UpdateWithState( InputControlType.LeftBumper, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_L1 ), updateTick, deltaTime );
			UpdateWithState( InputControlType.RightBumper, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_R1 ), updateTick, deltaTime );

			UpdateWithState( InputControlType.LeftStickButton, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_L3 ), updateTick, deltaTime );
			UpdateWithState( InputControlType.RightStickButton, OuyaSDK.OuyaInput.GetButton( DeviceIndex, OuyaController.BUTTON_R3 ), updateTick, deltaTime );

			UpdateWithState( InputControlType.Menu, OuyaSDK.OuyaInput.GetButtonDown( DeviceIndex, OuyaController.BUTTON_MENU ), updateTick, deltaTime );

			Commit( updateTick, deltaTime );
			#endif
		}


		public bool IsConnected
		{
			get
			{ 
				#if UNITY_ANDROID && INCONTROL_OUYA && !UNITY_EDITOR
				return OuyaSDK.OuyaInput.IsControllerConnected( DeviceIndex ); 
				#else
				return false;
				#endif
			}
		}
	}
}

