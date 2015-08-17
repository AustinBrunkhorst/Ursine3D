using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEngine;
using InControl;


namespace BindingsExample
{
	public class BindingsExample : MonoBehaviour
	{
		Renderer cachedRenderer;
		PlayerActions playerActions;
		string saveData;


		void OnEnable()
		{
			// See PlayerActions.cs for this setup.
			playerActions = PlayerActions.CreateWithDefaultBindings();

			LoadBindings();
		}


		void OnDisable()
		{
			// This properly disposes of the action set and unsubscribes it from
			// update events so that it doesn't do additional processing unnecessarily.
			playerActions.Destroy();
		}


		void Start()
		{
			cachedRenderer = GetComponent<Renderer>();
		}


		void Update()
		{
			transform.Rotate( Vector3.down, 500.0f * Time.deltaTime * playerActions.Move.X, Space.World );
			transform.Rotate( Vector3.right, 500.0f * Time.deltaTime * playerActions.Move.Y, Space.World );

			var fireColor = playerActions.Fire.IsPressed ? Color.red : Color.white;
			var jumpColor = playerActions.Jump.IsPressed ? Color.green : Color.white;

			cachedRenderer.material.color = Color.Lerp( fireColor, jumpColor, 0.5f );
		}


		void SaveBindings()
		{
			saveData = playerActions.Save();
			PlayerPrefs.SetString( "Bindings", saveData );
		}


		void LoadBindings()
		{
			if (PlayerPrefs.HasKey( "Bindings" ))
			{
				saveData = PlayerPrefs.GetString( "Bindings" );
				playerActions.Load( saveData );
			}
		}


		void OnApplicationQuit()
		{
			PlayerPrefs.Save();
		}


		void OnGUI()
		{
			const float h = 22.0f;
			var y = 10.0f;

			GUI.Label( new Rect( 10, y, 300, y + h ), "Last Input Type: " + playerActions.LastInputType.ToString() );
			y += h;

			var actionCount = playerActions.Actions.Count;
			for (int i = 0; i < actionCount; i++)
			{
				var action = playerActions.Actions[i];

				var name = action.Name;
				if (action.IsListeningForBinding)
				{
					name += " (Listening)";
				}
				name += " = " + action.Value;
				GUI.Label( new Rect( 10, y, 300, y + h ), name );
				y += h;

				var bindingCount = action.Bindings.Count;
				for (int j = 0; j < bindingCount; j++)
				{
					var binding = action.Bindings[j];

					GUI.Label( new Rect( 45, y, 300, y + h ), binding.DeviceName + ": " + binding.Name );
					if (GUI.Button( new Rect( 20, y + 3.0f, 20, h - 5.0f ), "-" ))
					{
						action.RemoveBinding( binding );
					}
					y += h;
				}

				if (GUI.Button( new Rect( 20, y + 3.0f, 20, h - 5.0f ), "+" ))
				{
					action.ListenForBinding();
				}

				if (GUI.Button( new Rect( 50, y + 3.0f, 50, h - 5.0f ), "Reset" ))
				{
					action.ResetBindings();
				}

				y += 25.0f;
			}

			if (GUI.Button( new Rect( 20, y + 3.0f, 50, h ), "Load" ))
			{
				LoadBindings();
			}

			if (GUI.Button( new Rect( 80, y + 3.0f, 50, h ), "Save" ))
			{
				SaveBindings();
			}

			if (GUI.Button( new Rect( 140, y + 3.0f, 50, h ), "Reset" ))
			{
				playerActions.Reset();
			}
		}
	}
}

