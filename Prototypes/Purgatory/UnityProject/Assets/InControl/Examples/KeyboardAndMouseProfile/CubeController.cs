using System;
using UnityEngine;
using InControl;


namespace KeyboardAndMouseExample
{
	// WARNING: "Custom profiles" will soon be deprecated in favor of the new user bindings API
	// (see the PlayerAction and PlayerActionSet classes) which accomplishes the same goal
	// much more elegantly and supports runtime remapping.
	// http://www.gallantgames.com/pages/incontrol-binding-actions-to-controls
	//
	public class CubeController : MonoBehaviour
	{
		Renderer cachedRenderer;
		Vector3 targetPosition;


		void Start()
		{
			// Get the renderer.
			cachedRenderer = GetComponent<Renderer>();

			// Get the starting position of the object.
			targetPosition = transform.position;
		}


		void Update()
		{
			// Use last device which provided input.
			var inputDevice = InputManager.ActiveDevice;

			// Set target object material color based on which action is pressed.
			cachedRenderer.material.color = GetColorFromActionButtons( inputDevice );

			// Rotate target object with both sticks and d-pad.
			transform.Rotate( Vector3.down, 500.0f * Time.deltaTime * inputDevice.Direction.X, Space.World );
			transform.Rotate( Vector3.right, 500.0f * Time.deltaTime * inputDevice.Direction.Y, Space.World );
			transform.Rotate( Vector3.down, 500.0f * Time.deltaTime * inputDevice.RightStickX, Space.World );
			transform.Rotate( Vector3.right, 500.0f * Time.deltaTime * inputDevice.RightStickY, Space.World );

			// Zoom target object with scroll wheel.
			var lt = inputDevice.GetControl( InputControlType.LeftTrigger );
			var rt = inputDevice.GetControl( InputControlType.RightTrigger );
			targetPosition.z = Mathf.Clamp( targetPosition.z - lt + rt, -10.0f, 25.0f );
			transform.position = Vector3.MoveTowards( transform.position, targetPosition, Time.deltaTime * 100.0f );

			// Only supported on Windows with XInput and Xbox 360 controllers.
			InputManager.ActiveDevice.Vibrate( inputDevice.LeftTrigger, inputDevice.RightTrigger );
		}


		Color GetColorFromActionButtons( InputDevice inputDevice )
		{
			if (inputDevice.Action1)
			{
				return Color.green;
			}
			
			if (inputDevice.Action2)
			{
				return Color.red;
			}
			
			if (inputDevice.Action3)
			{
				return Color.blue;
			}
			
			if (inputDevice.Action4)
			{
				return Color.yellow;
			}

			// Test for a combo keypress, mapped to left bumper.
			if (inputDevice.LeftBumper)
			{
				return Color.magenta;
			}

			
			return Color.white;
		}
	}
}

