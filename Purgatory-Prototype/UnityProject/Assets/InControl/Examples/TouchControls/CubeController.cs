using System;
using UnityEngine;
using InControl;


namespace TouchExample
{
	public class CubeController : MonoBehaviour
	{
		Renderer cachedRenderer;


		void Start()
		{
			cachedRenderer = GetComponent<Renderer>();
		}


		void Update()
		{
			// Use last device which provided input.
			var inputDevice = InputManager.ActiveDevice;

			// Disable and hide touch controls if we use a controller.
			// If "Enable Controls On Touch" is ticked in Touch Manager inspector,
			// controls will be enabled and shown again when the screen is touched.
			if (inputDevice != InputDevice.Null && inputDevice != TouchManager.Device)
			{
				TouchManager.ControlsEnabled = false;
			}

			// Set target object material color based on which action is pressed.
			cachedRenderer.material.color = GetColorFromActionButtons( inputDevice );

			// Rotate target object.
			transform.Rotate( Vector3.down, 500.0f * Time.deltaTime * inputDevice.Direction.X, Space.World );
			transform.Rotate( Vector3.right, 500.0f * Time.deltaTime * inputDevice.Direction.Y, Space.World );
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

			return Color.white;
		}


		void OnGUI()
		{
			var y = 10.0f;

			var touchCount = TouchManager.TouchCount;
			for (int i = 0; i < touchCount; i++)
			{
				var touch = TouchManager.GetTouch( i );
				GUI.Label( new Rect( 10, y, 500, y + 15.0f ), "" + i + ": fingerId = " + touch.fingerId + ", phase = " + touch.phase.ToString() + ", position = " + touch.position );
				y += 20.0f;
			}
		}
	}
}

