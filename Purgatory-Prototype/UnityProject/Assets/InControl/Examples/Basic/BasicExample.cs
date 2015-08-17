using System;
using UnityEngine;
using InControl;


namespace BasicExample
{
	public class BasicExample : MonoBehaviour
	{
		void Update()
		{
			// Use last device which provided input.
			var inputDevice = InputManager.ActiveDevice;

			// Rotate target object with left stick.
			transform.Rotate( Vector3.down, 500.0f * Time.deltaTime * inputDevice.LeftStickX, Space.World );
			transform.Rotate( Vector3.right, 500.0f * Time.deltaTime * inputDevice.LeftStickY, Space.World );

			// Get two colors based on two action buttons.
			var color1 = inputDevice.Action1.IsPressed ? Color.red : Color.white;
			var color2 = inputDevice.Action2.IsPressed ? Color.green : Color.white;

			// Blend the two colors together to color the object.
			GetComponent<Renderer>().material.color = Color.Lerp( color1, color2, 0.5f );
		}
	}
}

