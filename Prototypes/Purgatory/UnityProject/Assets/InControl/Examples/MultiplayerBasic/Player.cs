using System;
using UnityEngine;
using InControl;


namespace MultiplayerBasicExample
{
	// This is just a simple "player" script that rotates and colors a cube
	// based on input read from the device on its inputDevice field.
	//
	// See comments in PlayerManager.cs for more details.
	//
	public class Player : MonoBehaviour
	{
		public InputDevice Device { get; set; }

		Renderer cachedRenderer;


		void Start()
		{
			cachedRenderer = GetComponent<Renderer>();
		}


		void Update()
		{
			if (Device == null)
			{
				// If no controller set, just make it translucent white.
				cachedRenderer.material.color = new Color( 1.0f, 1.0f, 1.0f, 0.2f );
			}
			else
			{
				// Set object material color based on which action is pressed.
				cachedRenderer.material.color = GetColorFromInput();

				// Rotate object with left stick or d-pad.
				transform.Rotate( Vector3.down, 500.0f * Time.deltaTime * Device.Direction.X, Space.World );
				transform.Rotate( Vector3.right, 500.0f * Time.deltaTime * Device.Direction.Y, Space.World );
			}
		}


		Color GetColorFromInput()
		{
			if (Device.Action1)
			{
				return Color.green;
			}

			if (Device.Action2)
			{
				return Color.red;
			}

			if (Device.Action3)
			{
				return Color.blue;
			}

			if (Device.Action4)
			{
				return Color.yellow;
			}

			return Color.white;
		}
	}
}

