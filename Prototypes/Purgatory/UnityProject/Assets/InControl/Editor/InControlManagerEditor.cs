#if UNITY_EDITOR
using System.IO;
using UnityEditor;
using UnityEngine;
using InControl.ReorderableList;


namespace InControl
{
	[CustomEditor( typeof(InControlManager) )]
	public class InControlManagerEditor : Editor
	{
		SerializedProperty logDebugInfo;
		SerializedProperty invertYAxis;
		SerializedProperty useFixedUpdate;
		SerializedProperty dontDestroyOnLoad;
		SerializedProperty customProfiles;

		SerializedProperty enableXInput;
		SerializedProperty xInputUpdateRate;
		SerializedProperty xInputBufferSize;

		Texture headerTexture;
		

		void OnEnable()
		{
			logDebugInfo = serializedObject.FindProperty( "logDebugInfo" );
			invertYAxis = serializedObject.FindProperty( "invertYAxis" );
			useFixedUpdate = serializedObject.FindProperty( "useFixedUpdate" );
			dontDestroyOnLoad = serializedObject.FindProperty( "dontDestroyOnLoad" );
			customProfiles = serializedObject.FindProperty( "customProfiles" );

			enableXInput = serializedObject.FindProperty( "enableXInput" );
			xInputUpdateRate = serializedObject.FindProperty( "xInputUpdateRate" );
			xInputBufferSize = serializedObject.FindProperty( "xInputBufferSize" );

			var path = AssetDatabase.GetAssetPath( MonoScript.FromScriptableObject( this ) );
			headerTexture = EditorUtility.LoadAssetAtPath<Texture>( Path.GetDirectoryName( path ) + "/Images/InControlHeader.png" );
		}


		public override void OnInspectorGUI()
		{
			serializedObject.Update();

			GUILayout.Space( 5.0f );

			var headerRect = GUILayoutUtility.GetRect( 0.0f, 5.0f );
			headerRect.width = headerTexture.width;
			headerRect.height = headerTexture.height;
			GUILayout.Space( headerRect.height );
			GUI.DrawTexture( headerRect, headerTexture );

			GUILayout.Box( "Version " + InputManager.Version.ToString(), GUILayout.ExpandWidth( true ) );

			logDebugInfo.boolValue = EditorGUILayout.ToggleLeft( "Log Debug Info", logDebugInfo.boolValue );
			invertYAxis.boolValue = EditorGUILayout.ToggleLeft( "Invert Y Axis", invertYAxis.boolValue );
			useFixedUpdate.boolValue = EditorGUILayout.ToggleLeft( "Use Fixed Update", useFixedUpdate.boolValue );
			dontDestroyOnLoad.boolValue = EditorGUILayout.ToggleLeft( "Don't Destroy On Load", dontDestroyOnLoad.boolValue );

			enableXInput.boolValue = EditorGUILayout.ToggleLeft( "Enable XInput (Windows)", enableXInput.boolValue );
			if (enableXInput.boolValue)
			{
				GUIStyle style = new GUIStyle( GUI.skin.box );
				style.alignment = TextAnchor.UpperLeft;
				style.padding.left = 10;
				style.padding.right = 10;
				style.padding.bottom = 5;
				style.richText = true;
				var text = "" +
				           "<b>Advanced XInput Settings</b>\n" +
				           "Do not modify these unless you perfectly understand what effect it will have.\n" +
				           "Set to zero to automatically use sensible defaults.";
				GUILayout.Box( text, style, GUILayout.ExpandWidth( true ) );

				xInputUpdateRate.intValue = EditorGUILayout.IntField( "XInput Update Rate (Hz)", xInputUpdateRate.intValue );
				xInputUpdateRate.intValue = Mathf.Max( xInputUpdateRate.intValue, 0 );

				xInputBufferSize.intValue = EditorGUILayout.IntField( "XInput Buffer Size", xInputBufferSize.intValue );
				xInputBufferSize.intValue = Mathf.Max( xInputBufferSize.intValue, 0 );
			}

			GUILayout.Space( 5.0f );

			ReorderableListGUI.Title( "Custom Profiles" );
			ReorderableListGUI.ListField( customProfiles );

			GUILayout.Space( 3.0f );
			
			serializedObject.ApplyModifiedProperties();
		}
	}
}
#endif