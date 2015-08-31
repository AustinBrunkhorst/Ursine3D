#if UNITY_EDITOR
using System;
using UnityEditor;
using UnityEngine;
using System.Collections.Generic;


namespace InControl
{
	internal class TouchBuilder
	{
		[MenuItem( "GameObject/Create Other/InControl/Touch/Manager", false, 2 )]
		static void CreateTouchManager()
		{
			// Destroy any existing TouchManager components.
			foreach (var touchManagerComponent in GameObject.FindObjectsOfType<TouchManager>())
			{
				GameObject.DestroyImmediate( touchManagerComponent );
			}
			
			MonoBehaviour component;
			if (component = GameObject.FindObjectOfType<InControlManager>())
			{
				var gameObject = component.gameObject;
				
				// Destroy any existing camera components on TouchCamera game object.
				foreach (var cameraComponent in gameObject.GetComponentsInChildren<Camera>())
				{
					GameObject.DestroyImmediate( cameraComponent.gameObject );
				}
				
				var cameraGameObject = new GameObject( "TouchCamera" );
				cameraGameObject.transform.parent = gameObject.transform;
				
				var camera = cameraGameObject.AddComponent<Camera>();
				camera.transform.position = new Vector3( 0.0f, 0.0f, -10.0f );
				camera.clearFlags = CameraClearFlags.Nothing;
				camera.cullingMask = 1 << LayerMask.NameToLayer( "UI" );
				camera.orthographic = true;
				camera.orthographicSize = 5.0f;
				camera.nearClipPlane = 0.3f;
				camera.farClipPlane = 1000.0f;
				camera.rect = new Rect( 0.0f, 0.0f, 1.0f, 1.0f );
				camera.depth = 100;
				
				var touchManager = gameObject.AddComponent<TouchManager>();
				touchManager.touchCamera = camera;
				
				Selection.activeGameObject = gameObject;
				
				Debug.Log( "TouchManager component has been attached to the InControl manager object." );
				return;
			}
			else
			{
				Debug.LogError( "Could not find InControl manager object." );
			}			
		}


		[MenuItem( "GameObject/Create Other/InControl/Touch/Button Control", false, 3 )]
		public static void CreateTouchButtonControl()
		{			
			var touchManager = GameObject.FindObjectOfType<TouchManager>();
			if (touchManager != null)
			{
				var gameObject = touchManager.gameObject;
				
				var controlGameObject = new GameObject( "TouchButtonControl" );
				controlGameObject.transform.parent = gameObject.transform;
				controlGameObject.layer = touchManager.controlsLayer;

				var control = controlGameObject.AddComponent<TouchButtonControl>();
				control.button.Sprite = FindSpriteWithName( "TouchButton_A" );
				
				Selection.activeGameObject = controlGameObject;
				
				Debug.Log( "TouchButtonControl object has been created." );
				return;
			}
			else
			{
				Debug.LogError( "Could not find InControl manager object." );
			}			
		}


		[MenuItem( "GameObject/Create Other/InControl/Touch/Stick Control", false, 3 )]
		public static void CreateTouchStickControl()
		{			
			var touchManager = GameObject.FindObjectOfType<TouchManager>();
			if (touchManager != null)
			{
				var gameObject = touchManager.gameObject;
				
				var controlGameObject = new GameObject( "TouchStickControl" );
				controlGameObject.transform.parent = gameObject.transform;
				controlGameObject.layer = touchManager.controlsLayer;

				var control = controlGameObject.AddComponent<TouchStickControl>();
				control.ring.Sprite = FindSpriteWithName( "TouchStick_Ring" );
				control.knob.Sprite = FindSpriteWithName( "TouchStick_Knob" );
				
				Selection.activeGameObject = controlGameObject;
				
				Debug.Log( "TouchStickControl object has been created." );
				return;
			}
			else
			{
				Debug.LogError( "Could not find InControl manager object." );
			}			
		}


		[MenuItem( "GameObject/Create Other/InControl/Touch/Swipe Control", false, 3 )]
		public static void CreateTouchSwipeControl()
		{			
			var touchManager = GameObject.FindObjectOfType<TouchManager>();
			if (touchManager != null)
			{
				var gameObject = touchManager.gameObject;
				
				var controlGameObject = new GameObject( "TouchSwipeControl" );
				controlGameObject.transform.parent = gameObject.transform;
				controlGameObject.AddComponent<TouchSwipeControl>();
				controlGameObject.layer = touchManager.controlsLayer;
				
				Selection.activeGameObject = controlGameObject;
				
				Debug.Log( "TouchSwipeControl object has been created." );
				return;
			}
			else
			{
				Debug.LogError( "Could not find InControl manager object." );
			}			
		}


		[MenuItem( "GameObject/Create Other/InControl/Touch/Track Control", false, 3 )]
		public static void CreateTouchTrackControl()
		{			
			var touchManager = GameObject.FindObjectOfType<TouchManager>();
			if (touchManager != null)
			{
				var gameObject = touchManager.gameObject;
				
				var controlGameObject = new GameObject( "TouchTrackControl" );
				controlGameObject.transform.parent = gameObject.transform;
				controlGameObject.AddComponent<TouchTrackControl>();
				controlGameObject.layer = touchManager.controlsLayer;
				
				Selection.activeGameObject = controlGameObject;
				
				Debug.Log( "TouchTrackControl object has been created." );
				return;
			}
			else
			{
				Debug.LogError( "Could not find InControl manager object." );
			}			
		}


		public static void ChangeControlsLayer( int layer )
		{
			TouchManager.Instance.touchCamera.cullingMask = 1 << layer;

			foreach (var control in GameObject.FindObjectsOfType<TouchControl>())
			{
				foreach (var transform in control.gameObject.GetComponentsInChildren<Transform>( true ))
				{
					transform.gameObject.layer = layer;
				}
			}
		}


		static Sprite FindSpriteWithName( string name )
		{
			foreach (var sprite in Resources.FindObjectsOfTypeAll<Sprite>())
			{
				if (sprite.name == name)
				{
					return sprite;
				}
			}

			return null;
		}
	}
}
#endif

