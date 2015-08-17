using System;
using UnityEngine;
using InControl;


namespace MultiplayerWithBindingsExample
{
	// This is just a simple "player" script that rotates and colors a cube
	// based on input read from the actions field.
	//
	// See comments in PlayerManager.cs for more details.
	//
	public class Player : MonoBehaviour
	{
		public PlayerActions Actions { get; set; }

		Renderer cachedRenderer;


		void OnDisable()
		{
			if (Actions != null)
			{
				Actions.Destroy();
			}
		}


		void Start()
		{
			cachedRenderer = GetComponent<Renderer>();
		}


		void Update()
		{
			if (Actions == null)
			{
				// If no controller exists for this cube, just make it translucent.
				cachedRenderer.material.color = new Color( 1.0f, 1.0f, 1.0f, 0.2f );
			}
			else
			{
				// Set object material color.
				cachedRenderer.material.color = GetColorFromInput();

				// Rotate target object.
				transform.Rotate( Vector3.down, 500.0f * Time.deltaTime * Actions.Rotate.X, Space.World );
				transform.Rotate( Vector3.right, 500.0f * Time.deltaTime * Actions.Rotate.Y, Space.World );
			}
		}


		Color GetColorFromInput()
		{
			if (Actions.Green)
			{
				return Color.green;
			}

			if (Actions.Red)
			{
				return Color.red;
			}

			if (Actions.Blue)
			{
				return Color.blue;
			}

			if (Actions.Yellow)
			{
				return Color.yellow;
			}

			return Color.white;
		}
	}
}

